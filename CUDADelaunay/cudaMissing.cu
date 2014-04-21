/*
Author: Cao Thanh Tung, Qi Meng
Date: 15/03/2011

File Name: cudaMissing.cu

This file include all CUDA code to perform the inserting missing sites step

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

#define MAXINT        2147483647

/***********************************************************
 * Declarations
 ***********************************************************/
#define WBLOCK                256  
#define INSERT_TRIANGLE(v0, v1, v2, tri) \
    tmp = tri; \
    cnewtri[tmp] = step; \
    ctriangles[tmp * 9 + 3] = v2; \
    ctriangles[tmp * 9 + 4] = v0; \
    ctriangles[tmp * 9 + 5] = v1; \
    ctriangles[tmp * 9 + 6] = atomicExch(&cvertarr[v0], (tmp << 2)); \
    ctriangles[tmp * 9 + 7] = atomicExch(&cvertarr[v1], (tmp << 2) | 1); \
    ctriangles[tmp * 9 + 8] = atomicExch(&cvertarr[v2], (tmp << 2) | 2); \

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
extern "C" void cudaMissing();

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


#define MAX(x, y) ((x) < (y) ? (y) : (x))

/************************************************************
 * Variables and functions shared with the main module
 ************************************************************/
extern int nTris, nVerts, nPoints,nConstraints;       
extern int *ctriangles;            
extern int *cvertarr;            
extern int *tvertices; 
extern REAL2 *covertices;        
extern short *cnewtri; 
extern int step; 
extern int *cflag; 


/*******************************************************************
 * Fill an array with increasing numbers
 *******************************************************************/
__global__ void kernelFillIncrement(int *list, int start, int length) 
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
	int noThreads = blockDim.x * gridDim.x; 

	for (; x < length; x += noThreads)
	    list[x] = start + x; 
}


/********************************************************************
 * Collect all dead triangles into a list. 
 ********************************************************************/
__global__ void kernelMarkDeadTriangles(int *cmarker, short *cnewtri, int nTris) {
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if (x >= nTris)
        return ; 

    cmarker[x] = (cnewtri[x] >= 0 ? 0 : 1); 
}

__global__ void kernelCollectDeadTriangles(int *cdeadTri, short *cnewtri, int *cmarker, int nTris) {
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if (x >= nTris || cnewtri[x] >= 0)
        return ; 

    int id = cmarker[x]; 

    cdeadTri[id] = x; 
}

/********************************************************************
 * Locate the triangle which we are gonna insert a missing site to. 
 * - If the anchor of the missing site is not yet inserted, skip
 * - Locate the triangle and mark it to avoid two insertions into 
 *   the same triangle. 
 * - Guarantee that the missing site is not on the boundary due to 
 *   huge fake boundary added. 
 ********************************************************************/
__global__ void kernelLocateTriangleContainer(int *ctriangles, int *cvertarr, int *tvertices, 
                                              int *clocation, int *ctags, REAL2 *covertices, 
                                              int nVerts, int *active, int noActive)
{
    int t = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if ( t >= noActive )
        return ;

    int x = active[t]; 

    int anchor = tvertices[x]; 

    if (cvertarr[anchor] < 0) {
        clocation[x] = MAXINT; 
        return ; 
    }

    REAL2 v = covertices[x]; 
    REAL2 vanchor = covertices[anchor]; 

    REAL ccDest, ccApex, ccOpposite; 
    int pNextTri, pTri, pOri; 

    pNextTri = cvertarr[anchor]; 

    pTri = decode_tri(pNextTri);
    pOri = decode_ori(pNextTri); 

    int pDest, pApex, pOrg, pTmp; 
    REAL2 vDest, vApex, vOrg, vTmp; 

    pDest = ctriangles[pTri * 9 + 3 + (pOri + 2) % 3];    // Dest
    vDest = covertices[pDest]; 

    ccDest = cuda_ccw(vanchor, vDest, v); 

    do {
        pApex = ctriangles[pTri * 9 + 3 + pOri];        // apex
        vApex = covertices[pApex]; 

        ccApex = cuda_ccw(vanchor, vApex, v); 

        if (ccDest >= 0.0 && ccApex <= 0.0)
            // Inside the angle
            break; 

        pDest = pApex; vDest = vApex; ccDest = ccApex; 

        pNextTri = ctriangles[pTri * 9 + (pOri + 2) % 3]; 

        pTri = decode_tri(pNextTri);
        pOri = decode_ori(pNextTri); 
    } while (true);

    // Found an angle, now look for the actual triangle 
    // containing me. 
    ccOpposite = cuda_ccw(vDest, vApex, v);
    if (ccOpposite < 0.0) {
        // It's not right here, need to walk a bit further
        while (true) {
            // Get the opposite triangle
            pNextTri = ctriangles[pTri * 9 + (pOri + 1) % 3]; 
            //if (pNextTri < 0) {
            //    cvertarr[x] = -100; 
            //    clocation[x] = encode_tri(pTri, (pOri + 1) % 3); 
            //    return ; 
            //}
            pTri = decode_tri(pNextTri);
            // Rotate the triangle so that the org is opposite the previous org
            pOri = (decode_ori(pNextTri) + 2) % 3;    
            
            pOrg = ctriangles[pTri * 9 + 3 + (pOri + 1) % 3]; 
            vOrg = covertices[pOrg]; 

            pTmp = pDest; pDest = pApex; pApex = pTmp; 
            vTmp = vDest; vDest = vApex; vApex = vTmp; 

            ccDest = cuda_ccw(vOrg, vDest, v); 
            ccApex = cuda_ccw(vApex, vOrg, v); 

            bool moveleft; 

            if (ccDest >= 0.0)
                if (ccApex >= 0.0) 
                    // Found it!
                    break; 
                else
                    moveleft = false; 
            else
                if (ccApex >= 0.0)
                    moveleft = true; 
                else 
                    moveleft = (vOrg.x - v.x) * (vApex.x - vDest.x) + 
                               (vOrg.y - v.y) * (vApex.y - vDest.y) > 0.0; 

            if (moveleft) {
                pOri = (pOri + 2) % 3; 
                pApex = pDest; pDest = pOrg; 
                vApex = vDest; vDest = vOrg; 
                ccOpposite = ccDest;    // Orientation is unimportant
            }
            else {
                pOri = (pOri + 1) % 3; 
                pDest = pApex; pApex = pOrg; 
                vDest = vApex; vApex = vOrg; 
                ccOpposite = ccApex; 
            }
        }
    }

    int c0 = 0; 

    if (ccDest == 0.0) c0++; 
    if (ccApex == 0.0) c0++; 
    if (ccOpposite == 0.0) c0++; 

    if (c0 == 0) {
        // Easiest case, it's right here!
        clocation[x] = pNextTri + 1;    // Mark to indicate that it's a simple case.
        atomicMin(&ctags[pTri], x); 
    } else if (c0 > 1) {
        // Duplicate point
        clocation[x] = pNextTri + 1;
        cvertarr[x] = -2;
        active[t] = -1; 
        return ; 
    } else {
        // On an edge. 
        // Make sure our 'location' triangle always face toward that edge
        // (i.e. that edge will be opposite to the origin)
        if (ccDest == 0.0) 
            pOri = (pOri + 2) % 3; 
        else if (ccApex == 0.0) 
            pOri = (pOri + 1) % 3; 

        clocation[x] = -encode_tri(pTri, pOri) - 1;

        // To avoid deadlock when 3 sites want to insert on 3 edges, 
        // and they try to mark 3 pairs triangles: (a, b), (b, c), (c, a) 
        atomicMin(&ctags[pTri], x); 
        atomicMin(&ctags[decode_tri(ctriangles[pTri * 9 + (pOri + 1) % 3])], x); 
    }
}

/***************************************************************************
 * Determine which missing point insertion can be take place, 
 * mark those triangles that need to be deleted. 
 ***************************************************************************/
__global__ void kernelPreprocessTriangles(int *ctriangles, int *cvertarr, int *clocation, 
                                          int *ctags, int *tvertices, short *cnewtri, 
                                          int *cmarker, BYTE *caffected, int nVerts,
                                          int step, int *active, int noActive) {
    int t = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if ( t >= noActive )
        return ;

    int x = active[t]; 

    int pNextTri = clocation[x]; 

    if (pNextTri == MAXINT)
        return ; 

    int pTri = decode_tri(abs(pNextTri) - 1); 
    int pOri = decode_ori(abs(pNextTri) - 1); 
    int popp, pOppTri, pOppOri;

    bool success; 

    if (pNextTri >= 0)    // one triangle
        success = (ctags[pTri] == x); 
    else {
        popp = (ctriangles[pTri * 9 + (pOri + 1) % 3]); 
        pOppTri = decode_tri(popp); 
        pOppOri = decode_ori(popp); 
        success = (ctags[pTri] == x && ctags[pOppTri] == x); 
    }

    if (success) {
        cmarker[x] = 2;        
        cnewtri[pTri] = -step; 

        caffected[ctriangles[pTri * 9 + 3]] = 1; 
        caffected[ctriangles[pTri * 9 + 4]] = 1; 
        caffected[ctriangles[pTri * 9 + 5]] = 1; 

        if (pNextTri < 0) {
            cnewtri[pOppTri] = -step; 
            caffected[ctriangles[pOppTri * 9 + 3 + pOppOri]] = 1; 
        }
    } 
}

/************************************************************
 * Fix the vertex array for those affected sites 
 ************************************************************/
__global__ void kernelFixVertexArrayMissing(int *ctriangles, int *cvertarr, BYTE *caffected, 
                                            short *cnewtri, int nVerts) {    
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if (x >= nVerts || caffected[x] != 1) 
        return ; 

    int p = cvertarr[x], pnext = p; 

    // Find the first valid triangle
    while (pnext >= 0 && cnewtri[decode_tri(pnext)] < 0)
        pnext = ctriangles[decode_tri(pnext) * 9 + 6 + decode_ori(pnext)]; 
    
    if (pnext != p)
        cvertarr[x] = pnext;

    while (pnext >= 0) {
        // Find an invalid triangle
        do {
            p = pnext; 
            pnext = ctriangles[decode_tri(p) * 9 + 6 + decode_ori(p)]; 
        } while (pnext >= 0 && cnewtri[decode_tri(pnext)] >= 0); 
        
        if (pnext >= 0)    {
            // Now pnext is deleted, so we fix the link for p. 

            // Find the next valid triangle
            while (pnext >= 0 && cnewtri[decode_tri(pnext)] < 0)
                pnext = ctriangles[decode_tri(pnext) * 9 + 6 + decode_ori(pnext)]; 
            
            ctriangles[decode_tri(p) * 9 + 6 + decode_ori(p)] = pnext; 
        }
    }
}

__global__ void kernelInsertMissingSites(int *ctriangles, int *cvertarr, int *clocation, 
                                         int *cmarker, int *cavailtri, int *cprefix, 
                                         short *cnewtri, int nVerts, int step,
                                         int *active, int noActive) 
{
    int t = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if ( t >= noActive )
        return;

    int x = active[t];

    if (cmarker[x] != 2)
        return ; 

    active[t] = -1; 

    int pNextTri = clocation[x]; 

    int pTri = decode_tri(abs(pNextTri) - 1); 
    int pOri = decode_ori(abs(pNextTri) - 1); 
    int pOrg, pDest, pApex, pOpposite; 
    int t1, t2, t3, t4, tmp; 

    int offset = cprefix[x]; 

    t1 = pTri;
    t2 = cavailtri[offset]; 
    t3 = cavailtri[offset + 1]; 

    pApex = ctriangles[pTri * 9 + 3 + pOri]; 
    pOrg = ctriangles[pTri * 9 + 3 + (pOri + 1) % 3];  
    pDest = ctriangles[pTri * 9 + 3 + (pOri + 2) % 3];  

    if (pNextTri >= 0) {    // one triangle
        INSERT_TRIANGLE(pOrg, pDest, x, t1); 
        INSERT_TRIANGLE(pDest, pApex, x, t2); 
        INSERT_TRIANGLE(pApex, pOrg, x, t3); 
    } else {
        int nDest = ctriangles[pTri * 9 + (pOri + 1) % 3]; 
        int pOppTri = decode_tri(nDest); 
        int pOppOri = decode_ori(nDest); 
        pOpposite = ctriangles[pOppTri * 9 + 3 + pOppOri]; 
        t4 = pOppTri; 

        INSERT_TRIANGLE(pOrg, pDest, x, t1); 
        INSERT_TRIANGLE(pDest, pOpposite, x, t2); 
        INSERT_TRIANGLE(pOpposite, pApex, x, t3); 
        INSERT_TRIANGLE(pApex, pOrg, x, t4); 
    }
}

/******************************************************************
 * Update the links between triangles after adding new triangles
 ******************************************************************/
__global__ void kernelUpdateMissingTriangleLinks(int *ctriangles, int *cvertarr, short *cnewtri, 
                                                 int nTris, int step) {
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if (x >= nTris || cnewtri[x] != step) 
        return ; 

    int p0, p1, p2, n0 = -1, n1 = -1, n2 = -1;
    int nCounter, pNextTri, pTri, pOri, pTri9; 
    int x9 = x * 9;

    p2 = ctriangles[x9 + 3]; 
    p1 = ctriangles[x9 + 5]; 
    p0 = ctriangles[x9 + 4]; 
    nCounter = 0; 

    // orientation 0
    // Travel through the list of triangles sharing vertex 0 with this triangle.
    // In this list we can find at most two triangles sharing edge (p0, p1) and 
    // (p2, p0) with our triangle. 
    pNextTri = cvertarr[p0];

    while (pNextTri >= 0 && nCounter < 2) {
        pTri = decode_tri(pNextTri); 
        pOri = decode_ori(pNextTri); 
        pTri9 = pTri * 9; 

        if (p2 == ctriangles[pTri9 + 3 + (pOri + 2) % 3]) {    // NextDest
            n2 = pNextTri; 
            ctriangles[pTri9 + pOri] = (x << 2) | 2;  
            nCounter++; 
        }

        if (p1 == ctriangles[pTri9 + 3 + pOri]) {    // NextApex
            n0 = (pTri << 2) | ((pOri + 2) % 3);  
            ctriangles[pTri9 + (pOri + 2) % 3] = (x << 2);  
            nCounter++; 
        }

        pNextTri = ctriangles[pTri9 + 6 + pOri]; 
    }

    // orientation 1
    // Find the triangle with edge (p1, p2)
    pNextTri = cvertarr[p1]; 

    while (pNextTri >= 0) {
        pTri = decode_tri(pNextTri); 
        pOri = decode_ori(pNextTri); 
        pTri9 = pTri * 9; 

        if (p2 == ctriangles[pTri9 + 3 + pOri]) {    // NextApex
            n1 = (pTri << 2) | ((pOri + 2) % 3); 
            ctriangles[pTri9 + (pOri + 2) % 3] = (x << 2) | 1;  
            break ; 
        }

        pNextTri = ctriangles[pTri9 + 6 + pOri]; 
    }

    ctriangles[x9 + 0] = n0; 
    ctriangles[x9 + 1] = n1; 
    ctriangles[x9 + 2] = n2; 
}

/********************************************************************
 * Fix vertex array
 ********************************************************************/

__global__ void kernelMarkValidTriangles1(short *cnewtri, int *cvalid, int nTris)
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if (x >= nTris) 
        return ; 

    cvalid[x] = (cnewtri[x] >= 0) ? 1 : 0; 
}

__global__ void kernelCollectEmptySlots1(short *cnewtri, int *cprefix, int *cempty, int nTris)
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if (x >= nTris || cnewtri[x] >= 0) 
        return ; 

    int id = x - cprefix[x]; 

    cempty[id] = x; 
}

__global__ void kernelFillEmptySlots1(short *cnewtri, int *cprefix, int *cempty, int *ctriangles, 
                                     int nTris, int newnTris, int offset)
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if (x >= nTris || cnewtri[x] < 0) 
        return ; 

    int value;

    if (x < newnTris) 
        value = x; 
    else {
        value = cempty[cprefix[x] - offset]; 

        for (int i = 0; i < 9; i++)
            ctriangles[value * 9 + i] = ctriangles[x * 9 + i]; 
    }        

    cprefix[x] = value; 
}

__global__ void kernelFixIndices1(int *ctriangles, int *newindex, int nTris) {
    __shared__ int ct[WBLOCK * 9]; 

    int tId = threadIdx.x; 
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x, x9 = x * 9;
    int i, id; 
    
    if (x >= nTris)
        return ;

    // Cooperatively read all triangles processed by one block
    for (i = 0, id = tId; i < 9; i++, id += WBLOCK)
        ct[id] = ctriangles[x9 + id]; 

    __syncthreads(); 
    
    if (x + tId < nTris) {
        i = tId * 9;         
        if (ct[i] >= 0)
            ct[i] = encode_tri(newindex[decode_tri(ct[i])], decode_ori(ct[i])); 
        i++; 
        if (ct[i] >= 0)
            ct[i] = encode_tri(newindex[decode_tri(ct[i])], decode_ori(ct[i])); 
        i++; 
        if (ct[i] >= 0)
            ct[i] = encode_tri(newindex[decode_tri(ct[i])], decode_ori(ct[i])); 
        i++; 
        i++; 
        i++; 
        i++; 
        if (ct[i] >= 0)
            ct[i] = encode_tri(newindex[decode_tri(ct[i])], decode_ori(ct[i])); 
        i++; 
        if (ct[i] >= 0)
            ct[i] = encode_tri(newindex[decode_tri(ct[i])], decode_ori(ct[i])); 
        i++; 
        if (ct[i] >= 0)
            ct[i] = encode_tri(newindex[decode_tri(ct[i])], decode_ori(ct[i]));        
    }

    __syncthreads(); 

    for (i = 0, id = tId; i < 9; i++, id += WBLOCK)
        ctriangles[x9 + id] = ct[id]; 
}

/********************************************************************
 * Insert missing sites caused by overlapping or bad case shifting
 ********************************************************************/
void cudaMissing()
{
	cudaFuncSetCacheConfig( kernelLocateTriangleContainer, cudaFuncCachePreferL1 ); 
	cudaFuncSetCacheConfig( kernelUpdateMissingTriangleLinks, cudaFuncCachePreferL1 ); 

	cutilSafeCall( cudaMemcpyToSymbol(constData, hostConst, 13 * sizeof(REAL)) ); 

    // Collect dead triangles, insert new triangles to these slots first. 
    int *cdeadtri, *cmarker, *cprefix; 

	cutilSafeCall( cudaMalloc((void **) &cprefix, 2 * nVerts * sizeof(int)) ); 
	cutilSafeCall( cudaMalloc((void **) &cmarker, 2 * nVerts * sizeof(int)) ); 

    dim3 block = dim3(WBLOCK); 
    dim3 grid = dim3(STRIPE, nTris / (STRIPE * block.x) + 1); 
    int lastItem; 

    // Mark all dead triangles as 1 in cmarker array, 0 if not. 
    kernelMarkDeadTriangles<<< grid, block >>>(cmarker, cnewtri, nTris); 
    cutilCheckError(); 

    cutilSafeCall( cudaMemcpy(&lastItem, cmarker + nTris - 1, sizeof(int), cudaMemcpyDeviceToHost) ); 

    // Use prefix sum to compute the offset
    thrust::exclusive_scan( IntDPtr(cmarker), IntDPtr(cmarker) + nTris, IntDPtr(cprefix) ); 

    // Temporary release what we no longer need. 
	cutilSafeCall( cudaFree(cmarker));

    // Compute the size needed for the list of dead triangles
    // We also store the unused slots (after nTris but less than 2 * nVerts)
    int deadCounter; 
    cutilSafeCall( cudaMemcpy(&deadCounter, cprefix + nTris - 1, sizeof(int), cudaMemcpyDeviceToHost) ); 
    deadCounter += lastItem; 

    int tailTri = nVerts * 2 - nTris; 

    int deadListSize = deadCounter + tailTri; 

    cutilSafeCall( cudaMalloc((void **) &cdeadtri, deadListSize * sizeof(int)) ); 

    // Collect these dead triangles into the 
    kernelCollectDeadTriangles<<< grid, block >>>(cdeadtri, cnewtri, cprefix, nTris); 
    cutilCheckError(); 

    //printf("Dead triangles: %i\n", deadCounter); 

     grid = dim3(256); //tailTri / block.x + 1);
    kernelFillIncrement<<< grid, block >>>(cdeadtri + deadCounter, nTris, tailTri); 
    cutilCheckError(); 
    /********************************************************
     * Process missing sites
     ********************************************************/
	int *active; 
    cutilSafeCall( cudaMalloc( (void**)&active, nVerts * sizeof(int) ));
	IntDPtr activePtr( active );
    IntDPtr cvertarrPtr( cvertarr ); 

	thrust::sequence( activePtr, activePtr + nVerts );
    
	IntDPtr lastActivePtr = thrust::remove_if(
		activePtr, activePtr + nVerts, cvertarrPtr, isNotMinusOne() ); 

	int noActive = lastActivePtr - activePtr; 

    int *clocation;
    BYTE *caffected; 

	cutilSafeCall( cudaMalloc((void **) &caffected, nVerts * sizeof(BYTE)) ); 
	cutilSafeCall( cudaMalloc((void **) &clocation, nVerts * sizeof(int)) ); 
	cutilSafeCall( cudaMalloc((void **) &cmarker, nVerts * sizeof(int)) ); 

    block = dim3(128); 
    dim3 gridFull = dim3(STRIPE, nVerts / (STRIPE * block.x) + 1);
    
    int triUsed = 0; 

    do {
        // cprefix will be used as a marker for voting which insertion can be processed
        cutilSafeCall( cudaMemset(cprefix, 127, nVerts * 2 * sizeof(int)) ); 
        cutilSafeCall( cudaMemset(cflag, 0, sizeof(int)) ); 

        // Locate triangles containing the missing sites
        grid = dim3(STRIPE, noActive / (STRIPE * block.x) + 1); 
        kernelLocateTriangleContainer<<< grid, block >>>(ctriangles, cvertarr,  
            tvertices, clocation, cprefix, covertices, nVerts, active, noActive); 
        cutilCheckError(); 

        cutilSafeCall( cudaMemset(cmarker, 0, nVerts * sizeof(int)) ); 
        cutilSafeCall( cudaMemset(caffected, 0, nVerts) ); 

        // Determine which missing point insertion can be done in this pass
        kernelPreprocessTriangles<<< grid, block >>>(ctriangles, cvertarr, clocation, 
            cprefix, tvertices, cnewtri, cmarker, caffected, nVerts, step,
            active, noActive); 

        // In cmarker we have the number of new triangles 
        // that will be generated by inserting each site (0 or 2). 
        thrust::exclusive_scan( IntDPtr(cmarker), IntDPtr(cmarker) + nVerts, IntDPtr(cprefix) ); 

        // We remove the container triangle and fix the vertex array. 
        kernelFixVertexArrayMissing<<< gridFull, block >>>(ctriangles, cvertarr, caffected, 
                                 cnewtri, nVerts);
        // We then insert three new triangles for each missing site inserted. 
        kernelInsertMissingSites<<< grid, block >>>(ctriangles, cvertarr, clocation, 
            cmarker, cdeadtri + triUsed, cprefix, cnewtri, nVerts, step, active, noActive); 

        // Update the offset in the dead triangle list
        cutilSafeCall( cudaMemcpy(&lastItem, cmarker + nVerts - 1, sizeof(int), cudaMemcpyDeviceToHost) ); 
    
        int used; 
        cutilSafeCall( cudaMemcpy(&used, cprefix + nVerts - 1, sizeof(int), cudaMemcpyDeviceToHost) ); 
        triUsed += used + lastItem; 
        
        int newsize = MAX(nTris, nTris - deadCounter + triUsed); 

        // Update links between the new triangles and the old one. 
        grid = dim3(STRIPE, newsize / (STRIPE * block.x) + 1); 
        kernelUpdateMissingTriangleLinks<<< grid, block >>>(ctriangles, cvertarr, cnewtri, newsize, step); 

        //printf("--------Insert missing sites - step %i ; Inserted %i triangles\n", step, used + lastItem);

        IntDPtr lastActivePtr = thrust::remove_if(
            activePtr, activePtr + noActive, isNegative() ); 

        noActive = lastActivePtr - activePtr; 
        
        step++; 

    } while (noActive > 0); 

    cutilSafeCall( cudaFree( active ) ); 

    // We do not keep track of the dead triangles after this, 
    // because after removing the fake boundary, there would be a lot more, and they
    // will be recompute and recompact by then. 

    deadCounter -= triUsed; 

    // If we have used up all the dead triangles and more, we update nTris
    if (deadCounter < 0) 
        nTris -= deadCounter; 

    /******* DONE *******/   

	 /*********************************************************
     * Compact the triangle list 
    *********************************************************/

	cutilSafeCall( cudaFree(cmarker));
	cutilSafeCall( cudaFree(clocation));
	cutilSafeCall( cudaFree(caffected));
	cutilSafeCall( cudaFree(cprefix));
	cutilSafeCall( cudaFree(tvertices));
	cutilSafeCall( cudaFree(cdeadtri) ); 
	 /*********************************************************
     * Compact the triangle list 
     *********************************************************/
    if(deadCounter>0)
	{
		int *cvalid, *cprefix1;

		cutilSafeCall( cudaMalloc((void **) &cvalid, 2 * nVerts * sizeof(int)) ); 
		cutilSafeCall( cudaMalloc((void **) &cprefix1, 2 * nVerts * sizeof(int)) ); 

		block = dim3(WBLOCK); 
		grid = dim3(STRIPE, nTris / (STRIPE * block.x) + 1); 

		// Mark the valid triangles in the list
		kernelMarkValidTriangles1<<< grid, block >>>(cnewtri, cvalid, nTris); 
		cutilCheckError(); 

		// Compute the offset of them in the new list
        thrust::exclusive_scan( IntDPtr(cvalid), IntDPtr(cvalid) + nTris, IntDPtr(cprefix1) ); 

		int newnTris, lastitem, offset; 
		cutilSafeCall( cudaMemcpy(&newnTris, cprefix1 + nTris - 1, sizeof(int), cudaMemcpyDeviceToHost) ); 
		cutilSafeCall( cudaMemcpy(&lastitem, cvalid + nTris - 1, sizeof(int), cudaMemcpyDeviceToHost) ); 
		newnTris += lastitem; 
		cutilSafeCall( cudaMemcpy(&offset, cprefix1 + newnTris, sizeof(int), cudaMemcpyDeviceToHost) ); 

	//    printf("nTris = %i, new nTris = %i\n", nTris, newnTris); 

		// Find all empty slots in the list
		kernelCollectEmptySlots1<<< grid, block >>>(cnewtri, cprefix1, cvalid, nTris); 
		cutilCheckError(); 

		// Move those valid triangles at the end of the list
		// to the holes in the list. 
		grid = dim3(STRIPE, nTris / (STRIPE * block.x) + 1); 
		kernelFillEmptySlots1<<< grid, block >>>(cnewtri, cprefix1, cvalid, ctriangles, 
			nTris, newnTris, offset); 
		cutilCheckError(); 

		// Fix the links after the index of our triangles are mixed up
		grid = dim3(STRIPE, newnTris / (STRIPE * block.x) + 1); 
		kernelFixIndices1<<< grid, block >>>(ctriangles, cprefix1, newnTris); 
		cutilCheckError(); 


		cutilSafeCall( cudaFree(cprefix1));
	    cutilSafeCall( cudaFree(cvalid));
		nTris = newnTris; 
	}
}
