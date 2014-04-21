/*
Author: Cao Thanh Tung, Qi Meng
Date: 12/09/2011

File Name: main.cpp

Example of the usage of GPU-DT (with Visualization and Verification)

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


#ifdef _WIN32
#  define WINDOWS_LEAN_AND_MEAN
#  include <windows.h>
#endif

#ifndef _WIN32
#define fopen_s(p,f,t) {(*p)=fopen(f,t);if((*p)==NULL){printf("Error \
opening file %s, file: %s, line: \
%d\n",f,__FILE__,__LINE__);exit(EXIT_FAILURE);}}
#endif

#ifndef _WIN32
#ifndef max
#define max(p1,p2) (p1>p2)?p1:p2
#endif
#endif

#ifndef _CLOCK_T_DEFINED
typedef long clock_t;
#define _CLOCK_T_DEFINED
#endif


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "glut.h"
#include <time.h>

#include "gpudt.h"
#include "predicates.h"


//**********************************************************************************************
//* Testing
//**********************************************************************************************
// Global Vars
int winWidth    = 512;
int winHeight   = 512;

int fboSize     = 2048;  

char vertex_file[]      = "vertex.txt"; 
char constraint_file[]  = "constraint.txt"; 

// Mouse Control
int     oldMouseX, oldMouseY;
int     currentMouseX, currentMouseY;
float   clickedX        = 0.0f;
float   clickedY        = 0.0f; 
bool    isLeftMouseActive, isRightMouseActive;

// Transformation
float zDepth          = 1.0f;
float xTranslate      = 0.0f;
float yTranslate      = 0.0f;

// Triangle selection
int clickedTri = -1; 

// GPU-DT Vars
PGPUDTPARAMS    pInput;
PGPUDTOUTPUT    pOutput;



void ReadDataFromFile(PGPUDTPARAMS &pParams)
{   
	pParams = new GPUDTPARAMS;

	printf("start to read data from file\n");
	FILE *fp1,*fp2;
	
	fp1 = fopen(vertex_file,"r");
	fscanf(fp1,"%i\n" , &pParams->nPoints);		
	fp2 = fopen(constraint_file,"r");	
	fscanf(fp2,"%i\n" , &pParams->nConstraints); 

	

	printf("nPoints:        %i\n", pParams->nPoints);
    printf("nConstraints:   %i\n", pParams->nConstraints);
	
	pParams->points  = new gpudtVertex[pParams->nPoints];
    pParams->constraints = new int[pParams->nConstraints * 2];	

	REAL minx = MAXINT, miny = MAXINT;
	REAL maxx = -1, maxy = -1;
	
	for(int i=0; i<pParams->nPoints;i++)
	{		
		int t1;
		REAL t2,t3;

#ifdef SINGLE_PRECISION
		fscanf(fp1,"%i %f %f \n",&t1,&t2,&t3);
#else
		fscanf(fp1,"%i %lf %lf\n",&t1,&t2,&t3);
#endif

		pParams->points[i].x = t2;    // x
        pParams->points[i].y = t3;    // y 
		if(pParams->points[i].x>maxx)
			maxx = pParams->points[i].x;
		if(pParams->points[i].x<minx)
			minx = pParams->points[i].x;
		if(pParams->points[i].y>maxy)
			maxy = pParams->points[i].y;
		if(pParams->points[i].y<miny)
			miny = pParams->points[i].y;

	}
	fclose(fp1);
    
	pParams->maxX = maxx;
	pParams->maxY = maxy;
	pParams->minX = minx;
	pParams->minY = miny;
	
	for(int i=0; i<pParams->nConstraints ;i++)	
	{
		int t1,t2,t3;
		fscanf(fp2,"%i %i %i \n",&t1,&t2,&t3);
		pParams->constraints[i*2] = t2;
		pParams->constraints[i*2+1] = t3;	
		
	} 
    fclose(fp2);
    printf("input is ready\n");
	
}

//**********************************************************************************************
//* GLUT Callback
//**********************************************************************************************
void glutReshape(int width,int height)
{
    winWidth  = width;
    winHeight = height;
}

void drawTri(gpudtTriangle *tri) {
    int id0 = tri->vtx[0]; 
    int id1 = tri->vtx[1]; 
    int id2 = tri->vtx[2];

    glVertex2d(pInput->points[id0].x, pInput->points[id0].y);    
    glVertex2d(pInput->points[id1].x, pInput->points[id1].y);    
    glVertex2d(pInput->points[id2].x, pInput->points[id2].y);    
}

void drawTriangles()
{
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Draw the selected triangle
    if (clickedTri >= 0) {
        gpudtOrientedTriangle otri; 
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        gpudtTriangle *tri = &pOutput->triangles[clickedTri]; 

        glBegin(GL_TRIANGLES);
            glColor3f(0.0, 0.0, 1.0);
            drawTri(tri); 

            glColor3f(0.0, 1.0, 0.0);
            decode(tri->tri[0], otri);
            if (otri.tri >= 0)
                drawTri(&pOutput->triangles[otri.tri]); 
            decode(tri->tri[1], otri); 
            if (otri.tri >= 0)
                drawTri(&pOutput->triangles[otri.tri]); 
            decode(tri->tri[2], otri); 
            if (otri.tri >= 0)
                drawTri(&pOutput->triangles[otri.tri]); 
        glEnd();
    }

    // Draw Triangles
    glLineWidth(0.5f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < pOutput->nTris; ++i)
        drawTri(&pOutput->triangles[i]);		  
    glEnd();	

    // Draw the points
    glPointSize(2.0);  
    glBegin(GL_POINTS);
        glColor3f(1.0, 0.0, 0.0);       
        if (clickedTri >= 0) {
            glColor3f(1.0, 1.0, 0.0); 
            glVertex2d(clickedX, clickedY); 
        }

        glColor3f(1.0, 0.0, 0.0);
        for (int i=0; i<pInput->nPoints; ++i)   
            glVertex2d(pInput->points[i].x, pInput->points[i].y); 
    glEnd();
    glPointSize(1.5); 

    //Draw constrains	
    glLineWidth(1.5f);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);	
        for(int i = 0; i<pInput->nConstraints; i++)
        { 
            glVertex2d(pInput->points[ pInput->constraints[i*2] ].x,
                pInput->points[ pInput->constraints[i*2] ].y);
            glVertex2d(pInput->points[ pInput->constraints[i*2+1] ].x,
                pInput->points[ pInput->constraints[i*2+1] ].y);
        }
    glEnd();  

    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
}

void glutDisplay()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Setup Matrices
    REAL centerX = (pInput->minX + pInput->maxX) / 2.0;
    REAL centerY = (pInput->minY + pInput->maxY) / 2.0;
    REAL range = max(pInput->maxX - pInput->minX, pInput->maxY - pInput->minY); 
    REAL scale = range / pInput->fboSize;       

    glViewport(0,0,winWidth,winHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(pInput->minX, pInput->minX + range, pInput->minY, pInput->minY + range);    

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(centerX + xTranslate * scale * zDepth, centerY + yTranslate * scale * zDepth, 0.0);
    glScalef(zDepth, zDepth, 1.0);
    glTranslatef(-centerX, -centerY, 0.0);

    // Draw Triangles		     
    drawTriangles();

    glPopAttrib();
    glutSwapBuffers();
}

int locateTriangle(PGPUDTOUTPUT pOutput, REAL x, REAL y)
{
    gpudtTriangle *triList = pOutput->triangles; 
    gpudtOrientedTriangle olooptri, oFirstTri, temptri;
    int ptr;
    int triOrg, triDest, triApex, vcenter;
    gpudtVertex v;
    REAL ccTest, ccTest1, ccTest2;
    bool bONext;

    // Search for this vertex
    v.x = x; v.y = y; 

    // Start from an arbitrary triangle
    olooptri.orient = 0; 
    olooptri.tri = 0; 
    org(triList, olooptri, vcenter); 

    //**********************************************************************************************
    // Record the first starting triangle
    //**********************************************************************************************
    otricopy(olooptri, oFirstTri);

    org(triList, olooptri, triOrg);
    dest(triList, olooptri, triDest);
    apex(triList, olooptri, triApex);
    ccTest = counterclockwise(&pInput->points[triOrg], &pInput->points[triDest], &v);
    bONext = (ccTest >= 0.0);

    // Walk around the triangle fan
    while (true)
    {
        ccTest = bONext ? 
            counterclockwise(&pInput->points[triOrg], &v, &pInput->points[triApex]) :
        counterclockwise(&pInput->points[triOrg], &pInput->points[triDest], &v);

        if (ccTest >= 0.0)  // Inside this angle of the triangle fan
        {
            if (counterclockwise(&pInput->points[triDest], &pInput->points[triApex], &v) >= 0.0) // Found
                return olooptri.tri; 

            // Start walking to find the triangle
            lnextself(olooptri);
            sym(triList, olooptri, temptri);
            while (true)
            {
                if (temptri.tri < 0) // Outside the mesh, triangle not found
                    return -1; 

                lprev(temptri, olooptri);
                org(triList, olooptri, triOrg);
                dest(triList, olooptri, triDest);
                apex(triList, olooptri, triApex);

                ccTest1 = counterclockwise(&pInput->points[triOrg], &pInput->points[triDest], &v);
                ccTest2 = counterclockwise(&pInput->points[triApex], &pInput->points[triOrg], &v);

                if (ccTest1 >= 0 && ccTest2 >= 0) // Found
                    return olooptri.tri; 
                else
                {
                    // Which way should we continue the search?
                    REAL ccTestCenter = counterclockwise(&pInput->points[triOrg], &pInput->points[vcenter], &v);
                    if (ccTestCenter>0)
                    {
                        lprevself(olooptri);
                        sym(triList, olooptri, temptri);
                    }
                    else //if (ccTestCenter<0)
                        sym(triList, olooptri, temptri);
                }
            }
        }

        // goto the next triangle
        otricopy(olooptri, temptri);
        if (bONext)
        {			
            onextself(triList, olooptri);

            if (olooptri.tri < 0) // Outside the mesh, not found
                return -1; 
        }
        else
        {
            oprevself(triList, olooptri);

            if (olooptri.tri < 0) // Outside the mesh, not found
                return -1; 
        }

        if (otriequal(olooptri, oFirstTri)) // One round, not found
            return -1; 

        org(triList, olooptri, triOrg);
        dest(triList, olooptri, triDest);
        apex(triList, olooptri, triApex);
    }

    return -1; 
}

void glutMouse(int button,int state,int x,int y)
{
    int modifiers = glutGetModifiers(); 

    if (state == GLUT_UP)
        switch (button)
    {
        case GLUT_LEFT_BUTTON:
            isLeftMouseActive = false;
            break;
        case GLUT_RIGHT_BUTTON:
            isRightMouseActive = false;
            break;
    }

    if (state == GLUT_DOWN)
    {
        oldMouseX = currentMouseX = x;
        oldMouseY = currentMouseY = y;

        switch (button)
        {
        case GLUT_LEFT_BUTTON:
            if ((modifiers & GLUT_ACTIVE_SHIFT) > 0) {
                REAL centerX = (pInput->minX + pInput->maxX) / 2.0;
                REAL centerY = (pInput->minY + pInput->maxY) / 2.0;
                REAL range = max(pInput->maxX - pInput->minX, pInput->maxY - pInput->minY); 
                REAL scale = range / pInput->fboSize;       

                REAL realX = double(x) / winWidth * range + pInput->minX; 
                REAL realY = double(winHeight - y) / winHeight * range + pInput->minY; 

                //glTranslatef(centerX + xTranslate*scale*zDepth, centerY + yTranslate*scale*zDepth, 0.0);
                //glScalef(zDepth, zDepth, 1.0);
                //glTranslatef(-centerX, -centerY, 0.0);
                clickedX = ( realX - centerX - xTranslate * scale * zDepth) / zDepth + centerX; 
                clickedY = ( realY - centerY - yTranslate * scale * zDepth) / zDepth + centerY; 
                clickedTri = locateTriangle(pOutput, clickedX, clickedY);                    
                glutPostRedisplay(); 
            }            
            else
                isLeftMouseActive = true;

            break;
        case GLUT_RIGHT_BUTTON:
            isRightMouseActive = true;
            break;
        }

    }
}

void glutMouseMotion(int x,int y)
{
    currentMouseX = x;
    currentMouseY = y;

    if (isLeftMouseActive)
    {
        xTranslate += (currentMouseX - oldMouseX) / (zDepth / 2);
        yTranslate -= (currentMouseY - oldMouseY) / (zDepth / 2);
        glutPostRedisplay();
    }
    else if (isRightMouseActive)
    {
        zDepth -= (currentMouseY - oldMouseY) * zDepth / 400.0;
        glutPostRedisplay();
    }

    oldMouseX = currentMouseX;
    oldMouseY = currentMouseY;

}

void checkResult(PGPUDTPARAMS input, PGPUDTOUTPUT output)
{
    gpudtVertex    *points = (gpudtVertex *) input->points; 
    int            *oneTri = new int[input->nPoints]; 

    // Find for each vertex one triangle containing it
    memset(oneTri, -1, input->nPoints * sizeof(int)); 

    printf("Checking vertex indices..."); 

    gpudtOrientedTriangle otri, onexttri; 

    for (int i = 0; i < output->nTris; i++) 
    {
        const gpudtTriangle& t = output->triangles[i]; 

        if ( ( t.vtx[0] < 0 || t.vtx[0] >= input->nPoints ) || 
            ( t.vtx[1] < 0 || t.vtx[1] >= input->nPoints ) || 
            ( t.vtx[2] < 0 || t.vtx[2] >= input->nPoints ) )
        {
            printf("\n*** ERROR *** Invalid vertex index in triangle %i\n", i); 
            printf("%i %i %i\n", t.vtx[0], t.vtx[1], t.vtx[2]); 
            continue; 
        }

        otri.tri = i; 

        otri.orient = 0; oneTri[ t.vtx[0] ] = encode( otri ); 
        otri.orient = 1; oneTri[ t.vtx[1] ] = encode( otri ); 
        otri.orient = 2; oneTri[ t.vtx[2] ] = encode( otri ); 
    }

    for (int i = 0; i < input->nPoints; i++) 
    {
        int firstTri = oneTri[i]; 

        if ( firstTri == -1 )
            printf("\n*** ERROR *** Vertex %i is not in the triangulation\n", i); 
    }

    printf("\nChecking orientations..."); 

    for (int i = 0; i < output->nTris; i++) 
    {
        gpudtTriangle t = output->triangles[i]; 
        REAL check = counterclockwise(&points[t.vtx[0]], &points[t.vtx[1]], &points[t.vtx[2]]); 

        if (check <= 0.0) 
        {
            printf("\n*** ERROR *** Wrong orientation at triangle %i (%i, %i, %i)\n", 
                i, t.vtx[0], t.vtx[1], t.vtx[2]); 
            printf("%.20f\n", check); 
        }
    }

    printf("\nChecking constraints..."); 

    bool *edgeIsConstraint = new bool[output->nTris * 3]; 
    int ptr; 

    memset(edgeIsConstraint, false, output->nTris * 3 * sizeof(bool)); 

    for (int i = 0; i < input->nConstraints; i++) 
    {
        int a = input->constraints[ i * 2 ]; 
        int b = input->constraints[ i * 2 + 1 ]; 

        // Walk around a
        bool    hasConstraint   = false; 
        int     nextTri         = oneTri[a]; 
        int     orgVert, destVert; 

        decode(nextTri, otri); 

        int firstTri = otri.tri; 

        do 
        {
            lnext(otri, onexttri); 
            symself(output->triangles, onexttri); 

            if (onexttri.tri < 0) 
                break; 

            lnext(onexttri, otri); 
        } while (otri.tri != firstTri); 

        if (onexttri.tri < 0)
            firstTri = -1; 

        do 
        {
            org(output->triangles, otri, orgVert); 
            dest(output->triangles, otri, destVert); 

            if ( orgVert == b ) 
            {
                lprev(otri, onexttri); 
                edgeIsConstraint[onexttri.tri * 3 + onexttri.orient] = true; 
                hasConstraint = true; 
            }

            if ( destVert == b ) 
            {
                lnext(otri, onexttri); 
                edgeIsConstraint[onexttri.tri * 3 + onexttri.orient] = true; 
                hasConstraint = true; 
            }

            lprevself(otri); 
            symself(output->triangles, otri); 

            if (otri.tri < 0) 
                break; 

            lprevself(otri); 

        } while (otri.tri != firstTri); 

        if (!hasConstraint) 
        {
            printf("\n*** ERROR *** Constraint %i(%i, %i) not found in the triangulation\n", i, a, b); 
        }
    }

    printf("\nChecking incircle property..."); 

    int edgeCount = 0; 

    for (otri.tri = 0; otri.tri < output->nTris; otri.tri++)       
        for (otri.orient = 0; otri.orient < 3; otri.orient++)
        {
            sym(output->triangles, otri, onexttri);	

            if (onexttri.tri < 0)
            {
                edgeCount += 2; 
                continue; 
            } else
                edgeCount++; 

            if (!edgeIsConstraint[otri.tri * 3 + otri.orient])
            {
                int triOrg, triDest, triApex, oppoApex; 

                // current triangle origin, destination and apex
                org(output->triangles, otri, triOrg);
                dest(output->triangles, otri, triDest);
                apex(output->triangles, otri, triApex);				

                // opposite triangle apex
                apex(output->triangles, onexttri, oppoApex);

                if (incircle(&input->points[triOrg], 
                    &input->points[triDest], 
                    &input->points[triApex], 
                    &input->points[oppoApex]) > 0) 
                {
                    printf("\n*** ERROR *** Incircle test fail for triangle %i and %i\n", 
                        otri.tri, onexttri.tri);    
                }
            } // Not a constraint

        } // Loop through 3 edges

        edgeCount /= 2; 

        int euler = input->nPoints - edgeCount + output->nTris + 1; 

        if (euler != 2)
        {
            printf("\n*** ERROR *** Euler characteristic test fail\n"); 
            printf("V = %i, E = %i, F = %i\n", input->nPoints, edgeCount, output->nTris + 1); 
        }

        printf("\nDONE\n"); 

        delete [] oneTri; 
        delete [] edgeIsConstraint; 
}

int main(int argc,char **argv)
{
    // Viualization initialization   
    glutInit(&argc,argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(winWidth, winHeight);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("GPU-DT");

    glutDisplayFunc(glutDisplay);
    glutReshapeFunc(glutReshape);
    glutMouseFunc(glutMouse);
    glutMotionFunc(glutMouseMotion);

    printf("GPU-DT - A Delaunay Triangulation library using the GPU\n"); 
    printf("\n");	

    // Read input from a file
	ReadDataFromFile(pInput); 
	

    // GPU-DT setting
    pInput->fboSize = fboSize;
    pOutput         = NULL; 

    // Run GPU-DT
    
    clock_t tv[2];
  

    printf("Running GPU-DT...\n");    

    if (pOutput) 
        gpudtReleaseDTOutput(pOutput);

    tv[0] = clock();

    pOutput = gpudtComputeDT(pInput);

	tv[1] = clock();

    printf("GPU-DT time: %.4fs\n", (tv[1]-tv[0])/(REAL)CLOCKS_PER_SEC); 	

    checkResult(pInput, pOutput);  

    // Visualization
    printf("\n"); 
    printf("Visualization\n"); 
    printf("  - Left  mouse + Motion : Move\n"); 
    printf("  - Right mouse + Motion : Zoom\n"); 
    printf("  - Shift + Left mouse   : Highlight a triangle and its 3 neighbors\n"); 

    glutMainLoop();

    // Release memory
    delete [] pInput->points;
    delete [] pInput->constraints;

    if (pOutput)
        gpudtReleaseDTOutput(pOutput); 


    return 0;
}