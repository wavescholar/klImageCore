/*
Author: Cao Thanh Tung, Qi Meng
Date: 15/10/2011

File Name: cudaConstraint.cu

This file include all CUDA code to perform the inserting constraints step

===============================================================================

Copyright (c) 2011, School of Computing, National University of Singapore. 
All rights reserved.

Project homepage: http://www.comp.nus.edu.sg/~tants/cdt.html

If you use GPU-DT and you like it or have comments on its usefulness etc., we 
would love to hear from you at <tants@comp.nus.edu.sg>. You may share with us
your experience and any possibilities that we may improve the work/code.

===============================================================================

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the National University of University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission from the National University of Singapore. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

*/

#pragma warning(disable: 4311 4312)

#include <device_functions.h>
#include <stdio.h>
#include <string.h>
#include "gpudt.h"
#include "cudaDecl.h"
#include "common.h"
#include "cudaCCW.h"
#include <vector>

using namespace std;

#define MAXINT              2147483647

#define MAX_INNER_LOOP      5

/***********************************************************
* Declarations
***********************************************************/
#define WBLOCK                256  


#define SET_TRIANGLE(vOrg, vDest, vApex, nOrg, nDest, nApex, tri, ori) \
    ctriangles[(tri) * 9 + 3 + ((ori) + 1) % 3] = (vOrg); \
    ctriangles[(tri) * 9 + 3 + ((ori) + 2) % 3] = (vDest); \
    ctriangles[(tri) * 9 + 3 + (ori)] = (vApex); \
    ctriangles[(tri) * 9 + 6 + (ori)] = (nOrg); \
    ctriangles[(tri) * 9 + 6 + ((ori) + 1) % 3] = (nDest); \
    ctriangles[(tri) * 9 + 6 + ((ori) + 2) % 3] = (nApex) 

#define UPDATE_TEMP_LINK(pTriOri, pNext) \
    if ((pTriOri) >= 0) \
    ctriangles[decode_tri(pTriOri) * 9 + 6 + decode_ori(pTriOri)] = -(pNext)

#define UPDATE_LINK(pTriOri, pNext) \
    if ((pTriOri) >= 0) \
    ctriangles[decode_tri(pTriOri) * 9 + decode_ori(pTriOri)] = (pNext)

/**************************************************************
* Exported methods
**************************************************************/
extern "C" void cudaConstraint();

/**************************************************************
* Definitions
**************************************************************/
// Decode an oriented triangle. 
// An oriented triangle consists of 32 bits. 
// - 30 highest bits represent the triangle index, 
// - 2 lowest bits represent the orientation (the starting vertex, 0, 1 or 2)
#define decode_tri(x)            ((x) >> 2)
#define decode_ori(x)            ((x) & 3)
#define encode_tri(tri, ori)    (((tri) << 2) | (ori))

// Encode constraint information for each triangle
// each triangle use 32 bits to record informations as below:
// - 28 highest bits represent the constraint index (-1, if it is not intersected by any constraints; record the maximum index if it is intersected by more than one constriants).
// - 2  represent the orientation (the starting vertex, 0, 1 or 2).
// - 1  represent the apex vertex is on the left/right side of constraint x.
// - 1  represent whether the triangle is the first/last triangle intersected by constraint x.
#define decode_c(x)     (  (x)>>4      )
#define decode_cori(x)  ( ((x)>>2) & 3 )
#define decode_cside(x) ( ((x)>>1) & 1 )
#define decode_clast(x) (  (x) & 1     )

#define encode_constraint(x,ori,side,last)  ( ( ( ( ( (x<<2) | ori) << 1) | side) << 1 ) | last );

// Decode neighbor information for triangle. 
// information of triangle consists of 32 bits. 
// - 30 highest bits represent the neighbor triangle, 
// - 2 lowest bits represent the relationship between triangle and its neighbor
#define decode_neighbor(x)                  ((x) >> 2)
#define decode_candd(x)                     ((x) & 3)
#define encode_neighbor(neighbor, candd)    (((neighbor) << 2) | (candd))

#define MAX(x, y) ((x) < (y) ? (y) : (x))

/************************************************************
* Variables and functions shared with the main module
************************************************************/
extern int nTris, nVerts, nPoints,nConstraints;
extern int *ctriangles;            
extern int *cvertarr;            
extern int *tvertices; 
extern int *cconstraints; 
extern REAL2 *covertices;        
extern short *cnewtri; 
extern int step; 
extern int *cflag; 
extern PGPUDTPARAMS gpudtParams;

/***************************************************************************
* 	For all intersected triangles, find its neighbors which are intersected by the same constraint. 
*	And compute the relationships between triangle and its neighbors; record relationships in CandD. 
***************************************************************************/
__global__ void KernelFlippingPhase_1(int *ctriangles, REAL2 *covertices, int *cconnection, 
                                      int nTris, int *cflag, int *flipBy, BYTE* doNot, BYTE *affect, 
									  int timestamp, int *active, int noActive)
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;
    if (x >= noActive) 
        return; 

	if ( x == 0 ) 
		*cflag = 0; 

	x = active[x]; 

	flipBy[x] = -1; 
    doNot[x] = -1;

    if (affect[x] != 1) 
        return;

	int cc = cconnection[x]; 

    if (cc < 0)
        return;

	int intersectby = decode_c(cc);
    int ori			= decode_cori(cc); 
    int side		= decode_cside(cc); 
    int last		= decode_clast(cc); 	

    int nRight;	

    if(side==1)
        nRight = ctriangles[x*9 + (ori+2)%3];
    else
        nRight = ctriangles[x*9 + (ori+1)%3];

    if(nRight<0)
        return;

    REAL2 p1, p2, p3, p4;	
    int pTri, pOri;
    REAL test1, test2;	
    int side_pTri, last_pTri;		

    pTri = decode_tri(nRight);	pOri = decode_ori(nRight);
	int cc_pTri = cconnection[pTri]; 
    int intersectby_pTri = decode_c(cc_pTri) ;

	if ( intersectby != intersectby_pTri ) 
		return ; 

    int configuration = 0;//initial as single/zero configuration

    //check whether there's concave situation
    p1 = covertices[ctriangles[x*9 + 3 + (ori+2)%3]]; 
    p2 = covertices[ctriangles[x*9 + 3 + (ori+1)%3]]; 
    p3 = covertices[ctriangles[x*9 + 3 +  ori     ]]; 
    p4 = covertices[ctriangles[pTri * 9 + 3 + pOri]];		

	if ( side == 0 ) 
	{
		REAL2 tmp;
		tmp = p1; p1 = p2; p2 = tmp; 
	}

    test1 = cuda_ccw(p1, p4, p2);
    test2 = cuda_ccw(p1, p4, p3);

    if (test1*test2 >= 0)   // concave		
        configuration  = 1;
    else
    {
        //check whether there's double-intersection situation
        side_pTri = decode_cside(cc_pTri);
        last_pTri = decode_clast(cc_pTri);
        if(side_pTri != side && last!=1 && last_pTri!=1)//double-intersection
            configuration  = 2;			
    }		

	doNot[x] = configuration; 
}

/***************************************************************************
* 	For all intersected triangles, check whether it satisfies the 4 cases with its neighbors. 
*	Record this information in doNot. 
***************************************************************************/

__global__ void KernelFlippingPhase_2(int *ctriangles, REAL2 *covertices, int *cconnection, int nTris,
                                     BYTE *doNot, BYTE *affect, int *flipBy, int *cflag, BYTE *flipOri, 
									 int timestamp, int *active, int noActive)
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if (x >= noActive ) 
        return ; 

	x = active[x]; 

    if (affect[x] == 0)
		return ; 

	affect[x] = 0; 

    int cc = cconnection[x];
    if( cc < 0)
        return;	

    int side = decode_cside(cc);
    int ori = decode_cori(cc);
    int previous = ctriangles[x*9 + (ori+1)%3]; 
    int next = ctriangles[x*9 + (ori+2)%3]; 

    if(side==0)
    {
        int temp = previous;
        previous = next;
        next = temp;
    }

    if(next<0)
        return;

    int pRight = decode_tri(next);
    int constraintsforx = decode_c(cc);
    int candd_right = doNot[x];

	int isFlippable = 0; 

    if(candd_right==0)	//case1
    { 
        atomicMax(&flipBy[x], x+(1<<30));              
        atomicMax(&flipBy[pRight], x+(1<<30));							
        flipOri[x] = (ori+side)%3;			
	    *cflag = 1;	

		return ; 
    } 

    if(previous<0)
        return;

    int pLeft = decode_tri(previous);
    int constraintsforleft = decode_c(cconnection[pLeft]);

	if ( constraintsforleft != constraintsforx ) 
		return ; 

    int candd_left = doNot[pLeft];

    if(    (candd_left==2 &&  candd_right==2)	//case2
		|| (candd_left==1 &&  candd_right==2) )	//case3
    { 
        int pLeftOri = decode_ori(previous);
        int pRightOri = decode_ori(next);

        REAL2 pApex = covertices[ctriangles[pLeft    * 9 + 3 + pLeftOri]]; 
        REAL2 pOrg  = covertices[ctriangles[pLeft    * 9 + 3 + (pLeftOri+1)%3]]; 
        REAL2 pDest = covertices[ctriangles[pLeft    * 9 + 3 + (pLeftOri+2)%3]]; 
        REAL2 pOpp  = covertices[ctriangles[pRight   * 9 + 3 + pRightOri]]; 

        REAL test1 = cuda_ccw(pOpp,pApex,pOrg);
        REAL test2 = cuda_ccw(pOpp,pApex,pDest);		

        if(test1*test2<0)	
			isFlippable = ( candd_left == 2 ? 2 : 3 );
		else
			return ; 

        int shift;

        if(isFlippable==2)
            shift = 29;
        else
            shift = 28;		

        atomicMax(&flipBy[x],       x + (1 << shift));
        atomicMax(&flipBy[pLeft],   x + (1 << shift));
        atomicMax(&flipBy[pRight],  x + (1 << shift));

        flipOri[x] = (ori+side)%3;	
	    *cflag = 1;	
    }
}

/***************************************************************************
* 	For each triangle, record the intersection information with constraints.
***************************************************************************/
__global__ void kernelCheckingPhase(int *ctriangles, int *cvertarr, int *cconnection, REAL2 *covertices, 
                                    int nConstraints, int *tconstraintLink_flip, BYTE* done)
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x+threadIdx.x;

    if (x >= nConstraints || done[x] == 1) 
        return ;

    int t = 0;    
    int point1 = tconstraintLink_flip[2*x];
    int point2 = tconstraintLink_flip[2*x+1];
    int pTri, pOri, pTri9, pApex, pDest;
    int neighbor;
    int p = cvertarr[point1];

    int nOpp, pnext;
    pTri = decode_tri(p);
    pOri = decode_ori(p);
    pApex = ctriangles[pTri*9 + 3 +  pOri];
    pDest = ctriangles[pTri*9 + 3 +  (pOri+2)%3];

    if(pApex == point2 || pDest==point2)
	{
		// Mark the constraint so we don't check it again next rounds
		done[x] = 1; 
        return; 
	}

    REAL2 point1_R,point2_R,point3_R,point4_R;
    point1_R = covertices[point1];
    point2_R = covertices[point2];

    point3_R = covertices[pApex];
    REAL test = cuda_ccw(point1_R,point2_R,point3_R);

    if(test<0)//pApex is on the right of direction point1point2, so we need to find leftside pTri*9+2
    {	
        pnext = ctriangles[pTri*9 + (2 + pOri)%3];
        //find the first triangle that intersection constraint x.		
        do
        {			
            pTri = decode_tri(pnext);
            pOri = decode_ori(pnext);		
            pApex = ctriangles[pTri*9 + 3 + (0+pOri)%3];	
            if(pApex==point2) //pOrg=point1, pApex=point2;
			{
				done[x] = 1; 
                return;
			}
            point4_R = covertices[pApex];
            test = cuda_ccw(point1_R,point2_R,point4_R);

            if( test > 0)
            {
                nOpp = ctriangles[pTri*9 + (1 + pOri)%3];
                t = encode_constraint(x,pOri,0,1);  //ori=pOri,side=0,last=1;	
                atomicMax(&cconnection[pTri],t);
                break; 
            }
            p = pnext;		
            pnext = ctriangles[pTri*9 + (2 + pOri)%3];

        } while (true);
    }
    else//pApex is on the left of direction point1point2, so we need to find rightside pTri*9+0
    {		
        pnext = cvertarr[point1];		
        //find the first triangle that intersection constraint x.	
        do
        {
            if(pDest==point2) //pOrg=point1, pDest=point2;
			{
				done[x] = 1; 
 				return;
			}
            point4_R = covertices[pDest];
            test = cuda_ccw(point1_R,point2_R,point4_R);			

            if( test < 0 )
            {			
                nOpp = ctriangles[pTri*9 + (1 + pOri)%3];
                t = encode_constraint(x,pOri,0,1);  //ori=pOri,side=0,last=1;	
                atomicMax(&cconnection[pTri],t);	          
                break; 
            }
            p = pnext;	
            pnext = ctriangles[pTri*9 + (0+pOri)%3];	
            pTri = decode_tri(pnext);
            pOri = ( decode_ori(pnext) + 1 ) % 3;	
			pDest = ctriangles[pTri*9 + 3 + (2+pOri)%3]; 	         
        } while (true);
    }

    //find other triangles that intersected by constraint x.	
    pnext = nOpp;	
    int ori, side, last;
    side = 0;
    int tp;
    do{		
        pTri = decode_tri(pnext);
        pOri = decode_ori(pnext);
        pTri9 = pTri*9;	
        tp = ctriangles[pTri9 + 3 + pOri]; 

        if(tp==point2)
        {
            side = 1;
            ori = (pOri+2)%3;			
            t = encode_constraint(x, ori, side, 1);
            atomicMax(&cconnection[pTri], t);			
            return;
        }
        point3_R = covertices[tp];
        test = cuda_ccw(point1_R, point2_R, point3_R);

        if(test<0)//pApex (tp) is on the rightside of the constraint.
        {   
            ori = (pOri + 1)%3;
            side = 0;
            last = 0;
            neighbor = ctriangles[pTri9 + (pOri+2)%3];		
            t = encode_constraint(x, ori, side, last);
            atomicMax(&cconnection[pTri], t);			
            pnext = neighbor;			
        }
        if(test >0)///pApex (tp) is on the leftside of the constraint.
        {			
            ori = (pOri + 2)%3;
            side = 1;			
            last = 0;
            neighbor = ctriangles[pTri9 + (pOri+1)%3];
            t = encode_constraint(x, ori, side, last);
            atomicMax(&cconnection[pTri], t);				
            pnext = neighbor;					
        }
    }while(true);
}

// To flip a pair of triangle, we need to update the links between neighbor
// triangles. There is a chance that we need to flip two pairs in which 
// there are two neighboring triangles, thus the flipping is performed
// in 3 phases: 
// - Phase 1: Update the triangle vertices. Also, note down the new 
//            neighbors of each triangles in the 3 nexttri links (previously
//            used to store the vertex array). 
// - Phase 2: Each new triangle will then inform its new neighbors of its 
//            existence by writing itself in the corresponding nexttri link
//            of its neighbor.
// - Phase 3: Each new triangle update its links to its neighbors using the
//            3 new nexttri links. If any of the link is not updated, that
//            means the corresponding neighbor is not flipped in this pass. 
//            We then actively update the corresponding link in that triangle.

__global__ void kernelUpdatePhase11(int *ctriangles, BYTE *cflipOri, int *cflipStep, 
                                    int *cflipBy, int nTris, int step, int *cvertarr, 
                                    int *cconnection, BYTE *affect, BYTE *doNot, int *active, int noActive)
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if (x >= noActive)
        return ;

	x = active[x]; 

    int cc = cconnection[x];
    if(cc<0)
        return;		

    int xori, xside, xid, xlast, pori, pside, plast;
    int ori, pOpp, pOppTri, pOppOri, pOrg, pDest, pApex, nOrg, nApex, npDest, npApex;
    int newtag = -1; 
    int pLeft, pRight, pLeftTri, pRightTri;

    if ((cflipBy[x]&0xFFFFFFF) == x) 
    {     
        // I'm the one who win the right to flip myself

        xid   = decode_c(cc);
        xori  = decode_cori(cc);
        xside = decode_cside(cc);
        xlast = decode_clast(cc); 

        if(xside==0)	
            pLeft = ctriangles[x * 9 + (xori+2)%3];
        else	
            pLeft = ctriangles[x * 9 + (xori+1)%3];	

        ori     = cflipOri[x]; 
        pOpp    = ctriangles[x * 9 + (ori + 1) % 3]; 	
        pOppTri = decode_tri(pOpp);
        pOppOri = decode_ori(pOpp); 
        int pcc = cconnection[pOppTri];
        bool flip = false;

        if(cflipBy[x]>=(1 << 30) )
        {
            if((cflipBy[pOppTri]&0xFFFFFFF) == x)
                flip = true;
        }
        else
        {
            if(pLeft>-1)
            {
                pLeftTri = decode_tri(pLeft);
                if((cflipBy[pLeftTri]&0xFFFFFFF) == x && (cflipBy[pOppTri]&0xFFFFFFF) == x)
                    flip = true;
            }
        }
        if(flip)
        {    // I'm also the one who win the right

            newtag = pOpp;              // to flip this neighbor, so I can flip.
            pOrg = ctriangles[x * 9 + 3 + (ori + 1) % 3]; 
            pDest = ctriangles[x * 9 + 3 + (ori + 2) % 3]; 
            pApex = ctriangles[x * 9 + 3 + ori];
            pOpp = ctriangles[pOppTri * 9 + 3 + pOppOri]; 
            nOrg = ctriangles[x * 9 + ori]; 
            nApex = ctriangles[x * 9 + (ori + 2) % 3]; 
            npDest = ctriangles[pOppTri * 9 + (pOppOri + 1) % 3]; 
            npApex = ctriangles[pOppTri * 9 + (pOppOri + 2) % 3]; 

            // Update vertices + nexttri links
            SET_TRIANGLE(pOrg, pDest, pOpp, 3 + nOrg, -1, 3 + nApex, x, ori); 
            SET_TRIANGLE(pApex, pOrg, pOpp, -1, 3 + npDest, 3 + npApex, pOppTri, pOppOri); 

            //update cvertarr
            cvertarr[pOrg] = (x<<2)|((ori+1+2)%3);
            cvertarr[pDest] = (x<<2)|((ori+2+2)%3);
            cvertarr[pApex] = (pOppTri<<2)|((pOppOri+1+2)%3);
            cvertarr[pOpp] = (x<<2)|((ori+0+2)%3);

            pside = decode_cside(pcc);
            pori = decode_cori(pcc);
            plast = decode_clast(pcc);

            if(pside==0)
                pRight = ctriangles[pOppTri * 9 + (pori+1)%3];	 	 
            else
                pRight = ctriangles[pOppTri * 9 + (pori+2)%3];	

            affect[x] = 1;
            affect[pOppTri] = 1;	
            if(pRight>-1)
            {
                pRightTri = decode_tri(pRight);
                int c = decode_c(cconnection[pRightTri]);
                if(xid==c)
                {		
                    affect[pRightTri] = 1;			
                }
            }
            if(pLeft>-1)
            {
                pLeftTri = decode_tri(pLeft);
                int c = decode_c(cconnection[pLeftTri]);
                if(xid==c)
                {			
                    affect[pLeftTri] = 1;			
                }
            }

            ///update cconnection array
            pside = decode_cside(pcc);
            pori = decode_cori(pcc);
            plast = decode_clast(pcc);
            int sOrg,sDest,sApex,sOpp;
            if( ((ori + 1) % 3) == xori ) sOrg = xside; else sOrg = 1-xside;
            if( ((ori + 2) % 3) == xori ) sDest = xside;else sDest = 1-xside;
            if( ((ori    ) % 3) == xori ) sApex = xside;else sApex = 1-xside;
            if( pOppOri == pori) sOpp = pside;else sOpp = 1-pside;		

            if(xlast==1 && plast==1)
            {
                cconnection[x] = -1;
                cconnection[pOppTri] = -1;	

            }
            else if(xlast==0 && plast==0)
            {
                if (sOrg==sOpp)//only one triangle is intersected after flip
                {
                    if(sOrg == sDest)// x is not intersected
                    {
                        cconnection[x] = -1;
                        pori = (pOppOri+1)%3;
                        pside = sApex;
                        cconnection[pOppTri] = encode_constraint(xid,pori,pside,plast);
                    }
                    else// x is intersected
                    {
                        cconnection[pOppTri] = -1;						
                        xori = (ori+2)%3;
                        xside = sDest;
                        cconnection[x] = encode_constraint(xid,xori,xside,xlast);

                    }
                }
                else// both triangles are intersected after flip
                { 
                    if(sOrg ==sDest)
                    {
                        xori = ori;
                        xside = sOpp;						
                        pori = (pOppOri+2)%3;
                        pside = sOrg;						
                    }
                    else
                    {						
                        xori = (ori+1)%3;
                        xside = sOrg;					
                        pori = (pOppOri+0)%3;
                        pside = sOpp;
                    }
                    cconnection[x] = encode_constraint(xid,xori,xside,xlast);
                    cconnection[pOppTri] = encode_constraint(xid,pori,pside,plast);
                }
            }
            else if(xlast==1 && plast==0)
            {
                if(sDest == sOpp)// x is not intersected
                {
                    cconnection[x] = -1;					
                    pori = (pOppOri+1)%3;
                    pside = sApex;
                    plast = 1;
                    cconnection[pOppTri] = encode_constraint(xid,pori,pside,plast);
                }
                else// x is intersected
                {
                    cconnection[pOppTri] = -1;				
                    xori = (ori+2)%3;
                    xside = sDest;
                    xlast = 1;
                    cconnection[x] = encode_constraint(xid,xori,xside,xlast);
                }
            }
            else//xlast==0 && plast==1
            {
                if(sOrg == sDest)//x is not intersected
                {
                    cconnection[x] = -1;				
                    pori = (pOppOri+1)%3;
                    pside = sApex;
                    plast = 1;
                    cconnection[pOppTri] = encode_constraint(xid,pori,pside,plast);
                }
                else// x is intersected
                {
                    cconnection[pOppTri] = -1;			
                    xori = (1+ori)%3;
                    xside = sOrg;
                    xlast = 1;
                    cconnection[x] = encode_constraint(xid,xori,xside,xlast);
                }
            }
        }
    }	

    // Record the opp triangle to be used in the next phase
    cflipStep[x] = newtag;  
}

__global__ void kernelUpdatePhase22(int *ctriangles, BYTE *cflipOri, int *cflipStep, int nTris, int *active, int noActive) 
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

	if (x >= noActive)
		return; 

	x = active[x]; 

    if (cflipStep[x] < 0) 
        return ; 

    int ori, pOpp, pOppTri, pOppOri, nOrg, nApex, npDest, npApex;

    ori = cflipOri[x]; 
    pOpp = cflipStep[x]; 
    pOppTri = decode_tri(pOpp);
    pOppOri = decode_ori(pOpp); 
    nOrg = ctriangles[x * 9 + ori]; 
    nApex = ctriangles[x * 9 + (ori + 2) % 3]; 
    npDest = ctriangles[pOppTri * 9 + (pOppOri + 1) % 3]; 
    npApex = ctriangles[pOppTri * 9 + (pOppOri + 2) % 3]; 

    // Update my neighbors of my existence
    UPDATE_TEMP_LINK(nOrg, encode_tri(x, ori)); 
    UPDATE_TEMP_LINK(nApex, encode_tri(pOppTri, pOppOri)); 
    UPDATE_TEMP_LINK(npDest, encode_tri(x, (ori + 1) % 3)); 
    UPDATE_TEMP_LINK(npApex, encode_tri(pOppTri, (pOppOri + 2) % 3)); 
}

__global__ void kernelUpdatePhase33(int *ctriangles, BYTE *cflipOri, int *cflipStep, 
                                    int nTris, int *active, int noActive) 
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

	if (x >= noActive)
		return ;

	x = active[x]; 

    if (cflipStep[x] < 0) 
        return ; 

    int ori, pOpp, pOppTri, pOppOri, nOrg, nApex, npDest, npApex;

    ori = cflipOri[x]; 
    pOpp = cflipStep[x]; 
    pOppTri = decode_tri(pOpp);
    pOppOri = decode_ori(pOpp); 

    // Update other links
    nOrg = ctriangles[x * 9 + 6 + ori]; 
    nApex = ctriangles[x * 9 + 6 + (ori + 2) % 3]; 
    npDest = ctriangles[pOppTri * 9 + 6 + (pOppOri + 1) % 3]; 
    npApex = ctriangles[pOppTri * 9 + 6 + (pOppOri + 2) % 3]; 

    if (nOrg > 0) {        // My neighbor do not update me, update him
        nOrg = -(nOrg - 3); 
        UPDATE_LINK(-nOrg, encode_tri(x, ori)); 
    }

    if (nApex > 0) {
        nApex = -(nApex - 3); 
        UPDATE_LINK(-nApex, encode_tri(pOppTri, pOppOri)); 
    }

    if (npDest > 0) {
        npDest = -(npDest - 3); 
        UPDATE_LINK(-npDest, encode_tri(x, (ori + 1) % 3)); 
    }

    if (npApex > 0) {
        npApex = -(npApex - 3); 
        UPDATE_LINK(-npApex, encode_tri(pOppTri, (pOppOri + 2) % 3)); 
    }

    // Update my own links
    ctriangles[x * 9 + ori] = -nOrg; 
    ctriangles[x * 9 + (ori + 1) % 3] = -npDest; 
    ctriangles[x * 9 + (ori + 2) % 3] = encode_tri(pOppTri, (pOppOri + 1) % 3); 
    ctriangles[pOppTri * 9 + pOppOri] = -nApex; 
    ctriangles[pOppTri * 9 + (pOppOri + 1) % 3] = encode_tri(x, (ori + 2) % 3); 
    ctriangles[pOppTri * 9 + (pOppOri + 2) % 3] = -npApex; 

    // Mark the affected triangles, so that we will check again in the next pass
    cflipStep[x] = -1;   
}

/********************************************************************
* Fix vertex array
********************************************************************/

//__global__ void kernelFixVertArray(int *ctriangles, int nTris, int *cvertarr) 
//
//{
//    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;	
//    if (x >= nTris)
//        return ; 
//
//    int v0 = ctriangles[x * 9 + 4];
//    int v1 = ctriangles[x * 9 + 5];
//    int v2 = ctriangles[x * 9 + 3];
//
//    ctriangles[x * 9 + 6] = atomicExch(&cvertarr[v0], (x << 2)); 
//    ctriangles[x * 9 + 7] = atomicExch(&cvertarr[v1], (x << 2) | 1); 
//    ctriangles[x * 9 + 8] = atomicExch(&cvertarr[v2], (x << 2) | 2); 
//}
//
__global__ void kernelSetIsActive(int *active, int *cconnection, int noActive)
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x+threadIdx.x;

    if (x >= noActive) 
        return ;

	if ( cconnection[ active[x] ] < 0 )
		active[x] = -1; 
}

__global__ void kernelInitialize(int *active, BYTE *affect, int *cconnection, int noActive)
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x+threadIdx.x;

    if (x >= noActive) 
        return ;

	x = active[x]; 

	affect[x] = 1; 
	cconnection[x] = -1; 
}

/********************************************************************
* Insert constraints 
********************************************************************/
void cudaConstraint()
{
    if (nConstraints == 0)
        return;

    cutilSafeCall( cudaMemcpyToSymbol(constData, hostConst, 13 * sizeof(REAL)) ); 	

    dim3 block = dim3(128);
    dim3 grid = dim3(STRIPE, nTris/(STRIPE * block.x) + 1);

    int *cconnection, *active; 
    int flag_inner = 0, timestamp = 0;		
    int *cflipStep, *cflipBy;
    BYTE  *cflipOri;	
    BYTE *doNot;
    BYTE *affect;
	BYTE *done; 

	cutilSafeCall( cudaMalloc( (void**)&done, nConstraints ) ); 
    cutilSafeCall( cudaMalloc( (void**)&cconstraints,   2 * nConstraints    * sizeof(int) ) );
    cutilSafeCall( cudaMalloc( (void**)&cconnection,    nTris               * sizeof(int) ));
    cutilSafeCall( cudaMalloc( (void**)&active,    		nTris               * sizeof(int) ));
    cutilSafeCall( cudaMalloc( (void**)&doNot,          nTris               * sizeof(BYTE) )); 
    cutilSafeCall( cudaMalloc( (void**)&cflipStep,      nTris               * sizeof(int) ));
    cutilSafeCall( cudaMalloc( (void**)&cflipBy,        nTris               * sizeof(int) ));
    cutilSafeCall( cudaMalloc( (void**)&cflipOri,       nTris               * sizeof(BYTE)));		 
    cutilSafeCall( cudaMalloc( (void**)&affect,         nTris               * sizeof(BYTE)));

	IntDPtr activePtr( active ); 
	int noActive = nTris;  

	thrust::sequence( activePtr, activePtr + nTris ); 

    cutilSafeCall( cudaMemset(cflipStep, 255, nTris * sizeof(int)) );	
	cutilSafeCall( cudaMemset(done, 0, nConstraints) ); 

    // Copy constraints from host to device. 
    // This is also used in cudaFlipping, and will be released there. 
    cutilSafeCall( cudaMemcpy(cconstraints, gpudtParams->constraints, nConstraints * 2 * sizeof(int), cudaMemcpyHostToDevice) );

    int step = 0;

	do 
    { 	
		dim3 bigBlock = dim3(256);
		grid = dim3(STRIPE, noActive/(STRIPE * bigBlock.x) + 1); 
		kernelInitialize<<< grid, bigBlock >>>(active, affect, cconnection, noActive); 
		cutilCheckError(); 

        // outer-loop. Checking for all constraints, find all intersected triangles.

        grid = dim3(STRIPE, nConstraints/(STRIPE * block.x) + 1);		
        kernelCheckingPhase<<< grid, block >>>(ctriangles, cvertarr,  cconnection,covertices, nConstraints, cconstraints, done); 			
        cutilCheckError();

		grid = dim3(STRIPE, noActive/(STRIPE * bigBlock.x) + 1); 
		kernelSetIsActive<<< grid, bigBlock >>>(active, cconnection, noActive); 
		cutilCheckError(); 

		IntDPtr lastActivePtr = thrust::remove_if( activePtr, activePtr + noActive, isNegative() ); 

		noActive = lastActivePtr - activePtr; 

        if (noActive > 0) 
        {
            //inner-loop. Find all flippable triangle pairs and flip them until there's no flippalbe triangle pairs left.
            do
            {
                timestamp++;				
                grid = dim3(STRIPE, noActive/(STRIPE * block.x) + 1);	

                //For all intersected triangles, find its neighbors which are intersected by the same constraint. 
                //And compute the relationships between triangle and its neighbors; record relationships in doNot.
                KernelFlippingPhase_1<<< grid, block >>>(ctriangles, covertices, cconnection, nTris, cflag, cflipBy, 
					doNot, affect,timestamp, active, noActive);
                cutilCheckError();

                // For all intersected triangles, check whether it can be flipped with its neighbors.		
                KernelFlippingPhase_2<<< grid, block >>>(ctriangles, covertices, cconnection, nTris, doNot, affect, 
					cflipBy, cflag, cflipOri, timestamp, active, noActive);
                cutilCheckError();
                cutilSafeCall( cudaMemcpy(&flag_inner, cflag, sizeof(int), cudaMemcpyDeviceToHost) ); 

                if(flag_inner>0)				
                {
                    // do flipping on flippable triangle pairs, and update link information between triangles.
                    kernelUpdatePhase11<<< grid, block >>>(ctriangles, cflipOri, cflipStep, cflipBy, nTris, 
                        timestamp, cvertarr, cconnection, affect, doNot, active, noActive);				

                    kernelUpdatePhase22<<< grid, block >>>(ctriangles, cflipOri, cflipStep,	nTris, active, noActive);  
                    kernelUpdatePhase33<<< grid, block >>>(ctriangles, cflipOri, cflipStep,	nTris, active, noActive); 	
                }	

                // We only run a few inner loops for each outer loop
                // After that, the number of flippable pairs are too small.
                if (timestamp % MAX_INNER_LOOP == 0)
                    flag_inner = 0;		
            } while (flag_inner > 0);
        }	
        step ++;	

    } while ( noActive > 0 );

	cutilSafeCall( cudaFree(done) );
	cutilSafeCall( cudaFree(active) ); 
    cutilSafeCall( cudaFree(cconnection)); 	
    cutilSafeCall( cudaFree(cflipOri));
    cutilSafeCall( cudaFree(cflipBy));
    cutilSafeCall( cudaFree(cflipStep));
    cutilSafeCall( cudaFree(doNot));	
    cutilSafeCall( cudaFree(affect));  
}
