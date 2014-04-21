/*
Author: Cao Thanh Tung
Date: 15/03/2011

File Name: cudaReconstruction.cu

This file include all CUDA code to perform the reconstruction step

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

/***********************************************************
 * Declarations
 ***********************************************************/
#define WBLOCK                256        
#define SBLOCK                128
#define MBLOCK                32

/**************************************************************
 * Exported methods
 **************************************************************/
extern "C" void cudaReconstruction();

/**************************************************************
 * Definitions
 **************************************************************/
#define MAX(a, b)            (((a) > (b)) ? (a) : (b))
#define SHIFT                5

// Buffer to be used when constructing vertex array in CPU
#define BUFFER_ITEM        910
#define BUFFER_SIZE        BUFFER_ITEM * 9        // Buffer 32K

// Decode an oriented triangle. 
// An oriented triangle consists of 32 bits. 
// - 30 highest bits represent the triangle index, 
// - 2 lowest bits represent the orientation (the starting vertex, 0, 1 or 2)
#define decode_tri(x)    ((x) >> 2)
#define decode_ori(x)    ((x) & 3)

__constant__ int minus1mod3[4] = {2, 0, 1, -1};
__constant__ int plus1mod3[4] = {1, 2, 0, -1};

// Mark those shifted sites (either can or cannot)
// Shared with the Shifting stage. 
int *cactive;                
/* cactive is used in the shifting stage. 
 * The code used in cactive is as follow: 
 *         0        : Missing sites
 *        -1        : Boundary
 *        -2, 2    : Marked during the reconstruction stage
 *        -x, x   : Different steps in the shifting detection algorithm. 
 *      +inf    : Unprocessed
 *
 * Any value > 0 indicate that that vertex can be shifted safely. 
 */

/************************************************************
 * Variables and functions shared with the main module
 ************************************************************/
extern int *voronoiPtr;        // Discrete voronoi diagram
extern short2 *patternPtr;        // Voronoi vertex patterns

extern int nTris, nVerts, nPoints;    
extern int *ctriangles;        
extern int *cvertarr;        
extern int *tvertices; 
extern REAL2 *cvertices;    
extern REAL2 *covertices;    
extern PGPUDTPARAMS gpudtParams;    
extern int *cactive;                

extern int sizeTexture; 
extern REAL scale, shiftX, shiftY; 

extern int *boundary;
extern gpudtVertex *gpudtVertices; 
extern gpudtTriangle *gpudtTriangles;

extern int gpudtFixConvexHull(int *additionalTriangles, int fboWidth, int *boundary);

/*********************************************************************************
 * Count the number of triangle generated for each row of the texture. 
 * Used to calculate the offset to which each thread processing a texture row 
 * will insert the generated triangles.
 * Also, collect the boundary pixels of the texture to be used in the next CPU step
 *********************************************************************************/
__global__ void kernelCountRow(int *voronoiPtr, short2 *patternPtr, int *count, int width, int min, int max, int *cboundary) {
    // Get the row we are working on
    int x = blockIdx.x * blockDim.x + threadIdx.x; 

    // Collect the boundary (up, left, down, right)
    if (x > 0 && x <= max) {
        cboundary[width * 0 + x] = voronoiPtr[min * width + x]; 
        cboundary[width * 1 + x] = voronoiPtr[x * width + min]; 
        cboundary[width * 2 + x] = voronoiPtr[max * width + x]; 
        cboundary[width * 3 + x] = voronoiPtr[x * width + max]; 
    }

    // Actual counting
    if (x < min || x >= max)
        return ;  

    int xwidth = x * width; 
    int result = 0;
    short2 t = patternPtr[xwidth + min]; 
    
    // Keep jumping and counting
    while (t.y > 0 && t.y < max) {
        result += 1 + (t.x >> 2); 
        t = patternPtr[xwidth + t.y + 1];
    }

    count[x] = result; 
}

/*********************************************************************************
 * Prefix sum on the counted value to calculate the offset
 *********************************************************************************/
void cudaPrefixSum(int *cpuCount, int min, int max) {
    cpuCount[min-1] = 0; 
    for (int i = min; i < max; i++)
        cpuCount[i] += cpuCount[i-1]; 
}

/*********************************************************************************
 * Generate triangles from the Voronoi vertices and insert them into the triangle list.
 *********************************************************************************/
__global__ void kernelGenerateTriangles(int *voronoiPtr, short2 *patternPtr, int3 *ctriangles, 
										int *offset, int width, int min, int max) {
    int x = blockIdx.x * blockDim.x + threadIdx.x; 

    if (x < min || x >= max)
        return ;  

    int xwidth = x * width; 
    short2 pattern = patternPtr[xwidth + min];
    int i0, i1, i2, i3;
    int3 *pT = &ctriangles[offset[x-1] * 3 + 1]; 

    // Jump through all voronoi vertices in a texture row
    while (pattern.y > 0 && pattern.y < max) {
        i0 = voronoiPtr[xwidth + pattern.y]; 
        i1 = voronoiPtr[xwidth + pattern.y + 1]; 
        i2 = voronoiPtr[xwidth + width + pattern.y + 1];
        i3 = voronoiPtr[xwidth + width + pattern.y]; 

        if (pattern.x == 0) *pT = make_int3(i3, i1, i2); 
        if (pattern.x == 1) *pT = make_int3(i0, i2, i3); 
        if (pattern.x == 2) *pT = make_int3(i1, i3, i0); 
        if (pattern.x == 3) *pT = make_int3(i2, i0, i1); 
        if (pattern.x == 4) {
            // Generate 2 triangles. 
            // Since the hole is convex, no need to do CCW test
            *pT = make_int3(i2, i0, i1); pT += 3; 
            *pT = make_int3(i3, i0, i2); 
        }
        
        pattern = patternPtr[xwidth + pattern.y + 1]; 
        pT += 3; 
    }
}

/************************************************************
 * Scale back the point set
 ************************************************************/
__global__ void kernelScaleBack(REAL2 *cvertices, REAL scale, REAL shiftX, REAL shiftY, int nPoints) {
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x; 

    if (x >= nPoints)
        return ; 

    REAL2 t = cvertices[x]; 

    t.x = shiftX + t.x * scale; 
    t.y = shiftY + t.y * scale; 

    cvertices[x] = t; 
}

/*********************************************************************************
 * Map all sites to its ID, including missing sites
 *********************************************************************************/
__global__ void kernelMapToId(int *voronoiPtr, int nVerts, REAL2 *cvertices, 
                              int *tvertices, int width) {
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x; 

    if (x >= nVerts)
        return ; 

    REAL2 t = cvertices[x]; 
    tvertices[x] = voronoiPtr[(int(t.y) + 1) * width + (int(t.x) + 1)];
}

/*********************************************************************************
 * Find 3 neighbours sharing one edge with each triangle.
 *********************************************************************************/
__global__ void kernelFindNextTriangles(int *ctriangles, int *cvertarr, int nTris) {
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

    if (x >= nTris) 
        return ; 

    int p0, p1, p2;
    int nCounter, pNextTri, pTri, pOri, pTri9; 
    int x9 = x * 9;

	p0 = ctriangles[x9+4]; 
    p1 = ctriangles[x9+5]; 
    p2 = ctriangles[x9+3]; 
    nCounter = 0; 

	// orientation 0
	// Travel through the list of triangles sharing vertex 0 with this triangle.
	// In this list we can find at most two triangles sharing edge (p0, p1) and 
	// (p2, p0) with our triangle. 
	if ( p1 < p0 || p0 < p2 ) 
	{
		pNextTri = cvertarr[p0];  //ctriangles[x9+6];

		while (pNextTri >= 0 && nCounter < 2) {
			pTri = decode_tri(pNextTri); 
			pOri = decode_ori(pNextTri); 
			pTri9 = pTri * 9; 

			if (p2 == ctriangles[pTri9 + 3 + minus1mod3[pOri]]) {    // NextDest
				ctriangles[x9 + 2] = pNextTri; 
				ctriangles[pTri9 + pOri] = (x << 2) | 2;  
				nCounter++; 
			}

			if (p1 == ctriangles[pTri9 + 3 + pOri]) {    // NextApex
				ctriangles[x9 + 0] = (pTri << 2) | minus1mod3[pOri];  
				ctriangles[pTri9 + minus1mod3[pOri]] = (x << 2);  
				nCounter++; 
			}

			pNextTri = ctriangles[pTri9 + 6 + pOri]; 
		}
	}

    // orientation 1
    // Find the triangle with edge (p1, p2)
	if ( p2 < p1 ) 
	{
		pNextTri = cvertarr[p1]; //ctriangles[x9+7]; 

		while (pNextTri >= 0) {
			pTri = decode_tri(pNextTri); 
			pOri = decode_ori(pNextTri); 
			pTri9 = pTri * 9; 

			if (p2 == ctriangles[pTri9 + 3 + pOri]) {    // NextApex
				ctriangles[x9 + 1] = (pTri << 2) | minus1mod3[pOri]; 
				ctriangles[pTri9 + minus1mod3[pOri]] = (x << 2) | 1;  
				break ; 
			}

			pNextTri = ctriangles[pTri9 + 6 + pOri]; 
		}
	}
}

__global__ void kernelAppendTri(int3* dest, int3* source, int noTris)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x; 

	if (x >= noTris) 
		return; 

	dest[x * 3 + 1] = source[x]; 
}

__global__ void kernelSetVertArray(int *ctriangles, int nTris, int *cvertarr) 
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;	
    if (x >= nTris)
        return ; 

    int v0 = ctriangles[x * 9 + 4];
    int v1 = ctriangles[x * 9 + 5];
    int v2 = ctriangles[x * 9 + 3];

    ctriangles[x * 9 + 6] = atomicExch(&cvertarr[v0], (x << 2)); 
    ctriangles[x * 9 + 7] = atomicExch(&cvertarr[v1], (x << 2) | 1); 
    ctriangles[x * 9 + 8] = atomicExch(&cvertarr[v2], (x << 2) | 2); 
}

__global__ void kernelClearNeighbors(int3 * ctriangles, int nTris)
{
    int x = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x + threadIdx.x;

	if (x >= nTris) 
		return; 

	ctriangles[x * 3] = make_int3(-1, -1, -1); 
}

//////////////////////////////////////////////////////////////

void cudaReconstruction() {
    int *count,                // Number of triangle generated in one texture row
        *cboundary;            // Boundary pixels of the texture

    dim3 grid, block; 

    /****************************************************************************************
     * Initialization
     ****************************************************************************************/
    int texSize    = gpudtParams->fboSize; 
    int min        = 1; 
    int max        = texSize - 2;    // Assume width = height
    
    // EXACT test constants
    cutilSafeCall( cudaMemcpyToSymbol(constData, hostConst, 13 * sizeof(REAL)) ); 

    /****************************************************************************************
     * CUDA: Count the number of Voronoi Vertices in each row of the texture
     ****************************************************************************************/
    cutilSafeCall( cudaMalloc((void **) &count, texSize * sizeof(int)) ); 
    cutilSafeCall( cudaMalloc((void **) &cboundary, texSize * 4 * sizeof(int)) ); 

    int *cpuCount = (int *) malloc(texSize * sizeof(int)); 
    int *boundary = (int *) malloc(texSize * 4 * sizeof(int)); 

    block = dim3(MBLOCK);    
    grid = dim3(texSize / block.x);
    kernelCountRow<<< grid, block >>>(voronoiPtr, patternPtr, count, texSize, min, max, cboundary);
    cutilCheckError(); 

    cutilSafeCall( cudaMemcpy(boundary, cboundary, texSize * 4 * sizeof(int), cudaMemcpyDeviceToHost) ); 
    cutilSafeCall( cudaMemcpy(cpuCount, count, texSize * sizeof(int), cudaMemcpyDeviceToHost) ); 

    /****************************************************************************************
     * CPU: Prefix sum
     ****************************************************************************************/

    cudaPrefixSum(cpuCount, min, max); 
    cutilSafeCall( cudaMemcpy(count, cpuCount, texSize * sizeof(int), cudaMemcpyHostToDevice) );
    nTris = cpuCount[max-1];

    free(cpuCount); 

    /****************************************************************************************
     * CUDA: Generate triangles
     ****************************************************************************************/
    // We use a very small block size here because there are 
    // very few texture rows, we want to fully utilize the multiprocessors.
    block = dim3(MBLOCK);
    grid = dim3(texSize / block.x);
    kernelGenerateTriangles<<< grid, block >>>(voronoiPtr, patternPtr, (int3 *) ctriangles, 
		count, texSize, min, max);
    cutilCheckError(); 

	block = dim3(WBLOCK);    
    grid = dim3(STRIPE, nPoints / (block.x * STRIPE) + 1); 
    kernelMapToId<<< grid, block >>>(voronoiPtr, nPoints, cvertices, tvertices, texSize); 
    cutilCheckError(); 

    // Fix the convex hull right here. 
    int boundaryTris = gpudtFixConvexHull((int *)gpudtTriangles, texSize, boundary);

	int *convexHullTri; 
    cutilSafeCall( cudaMalloc(&convexHullTri, boundaryTris * 3 * sizeof(int)) ); 
	cutilSafeCall( cudaMemcpy(convexHullTri, gpudtTriangles, boundaryTris * 3 * sizeof(int), cudaMemcpyHostToDevice) ); 

	grid = dim3(boundaryTris / block.x + 1); 

	kernelAppendTri<<< grid, block >>>(((int3 *) ctriangles) + nTris * 3, (int3 *) convexHullTri, boundaryTris); 
    cutilCheckError(); 

    nTris += boundaryTris; 

    free(boundary); 
	cutilSafeCall( cudaFree(convexHullTri) ); 
    cutilSafeCall( cudaFree(cboundary) ); 
    cutilSafeCall( cudaFree(count) ); 

    // Scale the point set back
    block = dim3(WBLOCK); 
    grid = dim3(STRIPE, nPoints / (STRIPE * block.x) + 1); 
    kernelScaleBack<<< grid, block >>>(cvertices, scale, shiftX, shiftY, nPoints); 
    cutilCheckError(); 

	// Calculate the vertex array
	cutilSafeCall( cudaMalloc((void **) &cvertarr, nVerts * sizeof(int)) ); 
    block = dim3(128);
    grid = dim3(STRIPE, nTris/(STRIPE * block.x) + 1);

    cutilSafeCall( cudaMemset(cvertarr, 255, nVerts * sizeof(int)) );
    kernelSetVertArray<<< grid, block >>>(ctriangles, nTris, cvertarr); //fix vertex array 
    cutilCheckError(); 

	/****************************************************************************************
     * Find next triangles
     ****************************************************************************************/
	// First, fill the next triangles links with -1
    block = dim3(WBLOCK); 
    grid = dim3(STRIPE, nTris / (STRIPE * block.x) + 1); 

	kernelClearNeighbors<<< grid, block >>>((int3 *) ctriangles, nTris); 
    cutilCheckError(); 

    kernelFindNextTriangles<<< grid, block >>>(ctriangles, cvertarr, nTris); 
    cutilCheckError(); 

    /****************************************************************************************
     * Done!!!
     ****************************************************************************************/
}
