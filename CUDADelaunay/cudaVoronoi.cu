/*
Author: Cao Thanh Tung, Rong Guodong, Stephanus
Date: 15/03/2011

File Name: cudaVoronoi.cu

This file include all CUDA code to perform Voronoi Diagram computation, 
remove islands, etc. 

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
#include "cudaDecl.h"
#include "common.h"
#include<iostream>   
#include<vector>   
#include<queue>   
using namespace std;

extern "C" void cudaDiscreteVoronoiDiagram();
extern "C" void pba2DCompute(int phase1Band, int phase2Band, int phase3Band);

#define MAX_INT        2147483647
#define MIN_SHORT    -32768
#define MAX_SHORT    32767

#define BLOCKX        16
#define BLOCKY        8
#define WBLOCK        BLOCKX * BLOCKY

// Parameters for PBA
int phase1Band = 16;
int phase2Band = 16;
int phase3Band = 16;

 

#define ROUND(x, y)    (((x) - 1) / (y) + 1)

/****** Global Variables *******/
extern PGPUDTPARAMS gpudtParams;        // Input parameters

int iCurrentBuffer; 
int texSize; 
int log2Width; 

texture<short2, 1, cudaReadModeElementType> texColor; 
texture<short2, 1, cudaReadModeElementType> texPattern; 
texture<int, 1, cudaReadModeElementType> texInt; 

// Shared with main module
extern short2 **pingpongColor, **pingpongPattern;   
extern int *voronoiPtr; 
extern short2 *patternPtr; 
extern REAL2 *cvertices;        // Original coordinates of the sites    
extern int sizeTexture; 
extern int *cflag; 
extern short *fordraw2;
/********* Kernels ********/
#include "kernelVoronoi.h"

/****** Helper modules ******/

#define SCALEX(x)    (((x) - minX) * texWidthNeg2 / rangeX)
#define SCALEY(y)    (((y) - minY) * texHeightNeg2 / rangeY) 

struct Node{ 
	short2 island, seed;   
	Node(short2 a, short2 b)
	{       
		island = a;
	    seed = b;
	}
}; 
bool operator<( Node a, Node b )
{    
	int x1 = a.island.x;
	int y1 = a.island.y;
	int x2 = a.seed.x;
	int y2 = a.seed.y;
	int dist1 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
	x1 = b.island.x;
	y1 = b.island.y;
	x2 = b.seed.x;
	y2 = b.seed.y;
	int dist2 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
	return dist1>dist2;
} 

void colorIsland(short2 * mapOfIsland, short2 *output, int islands, int texSize)
{
	priority_queue<Node> q;	
    short2 island, seed;	
    int min = 1, max = texSize - 1;	
	for(int t=0; t<islands; t++)
		if (output[t * 2 + 1].x == texSize)
		{		 
			int x,y;
			island = output[t * 2];			
			//check its 8 neighbors
			for (x = island.x-1; x <= island.x+1; x++)
				for (y = island.y-1; y <= island.y+1; y++)
					if (x >= min && x < max && y >= min && y < max)
					{							
						seed = mapOfIsland[texSize * y + x];							
						if (seed.x != texSize)
						{
							int dist1 = (x - seed.x) * (x - seed.x) + (y - seed.y) * (y - seed.y); 
							int dist2 = (island.x - seed.x) * (island.x - seed.x) + 
								(island.y - seed.y) * (island.y - seed.y); 

							if (dist1 <= dist2)							
								q.push(Node(island, seed)); 
						}
					}
		}

	while(!q.empty())
	{
		short2 island = q.top().island;
		short2 seed = q.top().seed;	
		q.pop();
		if(mapOfIsland[island.y*texSize+island.x].x==texSize)
		{
			mapOfIsland[island.y*texSize+island.x] = seed;

            for (int x = island.x-1; x <= island.x+1; x++)
				for (int y = island.y-1; y <= island.y+1; y++)
					if (x >= min && x < max && y >= min && y < max)
					{
						if (mapOfIsland[texSize * y + x].x == texSize)
						{
							q.push(Node(make_short2(x, y), seed)); 							
						}
					}
		}
	}
}

// Initialize two Pingpong array
void cudaInitializePingpong()
{
    // Compute log2 of texSize
    int tmp = texSize;
    log2Width = 0; 
    while (tmp > 1) { log2Width += 1; tmp >>= 1; }

    // Initialize two color textures with MIN_SHORT
    dim3 block = dim3(BLOCKX, BLOCKY); 
    dim3 grid = dim3(texSize / block.x, texSize / block.y); 
	

    kernelFillShort<<< grid, block >>>(pingpongColor[0], MARKER, log2Width); 
    cutilCheckError(); 
}

// Deallocate all CUDA allocated arrays
void cudaVoronoiDeallocation()
{
    voronoiPtr = (int *) pingpongColor[0]; 
    patternPtr = pingpongPattern[0];

	cutilSafeCall( cudaFree(pingpongColor[1]) ); 
//	cutilSafeCall( cudaFree(pingpongPattern[1]) ); 
}

// Map points into the texture
// Include scaling and shifting to integer coordinates
void cudaMapPointsToTexture()
{
    dim3 block(WBLOCK); 
    dim3 grid(STRIPE, ROUND(gpudtParams->nPoints, block.x * STRIPE));

    kernelMapPointsToTexture<<< grid, block >>>(gpudtParams->nPoints, cvertices,
        pingpongColor[0], (int *) pingpongPattern[0], log2Width); 
    cutilCheckError(); 
}

// Detect islands
void cudaIslandDetection()
{
    dim3 block = dim3(BLOCKX, BLOCKY); 
    dim3 grid = dim3(texSize / block.x, texSize / block.y); 

    int flag;	

	do 
    {
        // Set flag = 0
        cutilSafeCall( cudaMemset(cflag, 0, sizeof(int)) ); 

        cutilSafeCall( cudaBindTexture(0, texColor, pingpongColor[iCurrentBuffer], sizeTexture) );  

        kernelIslandDetection<<< grid, block >>>(pingpongColor[1 - iCurrentBuffer], 
            texSize, log2Width, cflag); 

        cutilSafeCall( cudaUnbindTexture(texColor) ); 

        // Update the target texture
        iCurrentBuffer = 1 - iCurrentBuffer;
	
		cutilSafeCall( cudaMemcpy(&flag, cflag, sizeof(int), cudaMemcpyDeviceToHost) ); 

	} while(flag != 0);	

   
    iCurrentBuffer = 1 - iCurrentBuffer;

	// Mark the islands and its neighbors in the list	
	int *islandMark = (int *) pingpongColor[1 - iCurrentBuffer]; 
	int *prefix = (int *) pingpongPattern[1]; 

	cutilSafeCall( cudaMemset(islandMark, 0, texSize * texSize * sizeof(int)));	 

	kernelMarkIsland<<< grid, block >>>(pingpongColor[iCurrentBuffer], islandMark, texSize,log2Width);
	cutilCheckError(); 
	
    // Compute the offset of them in the new list
    thrust::exclusive_scan(
        IntDPtr(islandMark), IntDPtr(islandMark) + texSize * texSize,
        IntDPtr(prefix) ); 

	int islands, lastitem; 
    cutilSafeCall( cudaMemcpy(&islands, prefix + texSize * texSize - 1, sizeof(int), cudaMemcpyDeviceToHost) ); 
    cutilSafeCall( cudaMemcpy(&lastitem, islandMark + texSize * texSize - 1, sizeof(int), cudaMemcpyDeviceToHost) ); 
    islands += lastitem; 

	short2 *output; 	
	cutilSafeCall( cudaMalloc(&output, islands * 2 * sizeof(short2)) );
      
	kernelCollectIsland<<< grid, block >>>(pingpongColor[iCurrentBuffer], islandMark, 
		prefix, output, texSize, log2Width,cflag);
	cutilCheckError();

	short2 *output_cpu = new short2[2*islands];
	cutilSafeCall( cudaMemcpy(output_cpu, output, 2*islands * sizeof(short2), cudaMemcpyDeviceToHost) ); 
	
	short2 *madeMap = new short2[texSize*texSize];	

	short2 coords, color;
	for(int i=0; i<islands; i++)
	{ 
		coords = output_cpu[i*2    ];		
		color  = output_cpu[i*2 + 1];		
		madeMap[coords.y * texSize + coords.x] = color;		 
	}
   
	colorIsland(madeMap, output_cpu, islands, texSize);	
	
	for(int i=0; i<islands; i++)
	{
		coords = output_cpu[i*2];	
		output_cpu[i*2+1] = madeMap[coords.y * texSize + coords.x];		
	}  

	cutilSafeCall( cudaMemcpy(output, output_cpu, 2*islands * sizeof(short2), cudaMemcpyHostToDevice) ); 
   
	block = dim3(128); 
    grid = dim3(STRIPE, islands / (STRIPE * block.x) + 1);
	
	kernelRecolorIsland<<< grid, block >>>(pingpongColor[iCurrentBuffer], output, log2Width, islands);
	cutilCheckError(); 	
	
	cutilSafeCall( cudaFree(output)); 	
	delete [] output_cpu;
	delete [] madeMap;	
}

// Find real Voronoi vertices
void cudaFindRealVoronoiVertices()
{
    dim3 block = dim3(BLOCKX, BLOCKY); 
    dim3 grid = dim3(texSize / block.x, texSize / block.y); 

    cutilSafeCall( cudaBindTexture(0, texInt, pingpongPattern[0], sizeTexture) ); 
    cutilSafeCall( cudaBindTexture(0, texColor, pingpongColor[iCurrentBuffer], sizeTexture) ); 

    kernelFindRealVoronoiVertices<<< grid, block >>>(
        (int *) pingpongColor[1 - iCurrentBuffer], pingpongPattern[1],
        texSize, log2Width); 
    cutilCheckError(); 

    cutilSafeCall( cudaUnbindTexture(texInt) ); 
    cutilSafeCall( cudaUnbindTexture(texColor) ); 

    // Update the target texture
    iCurrentBuffer = 1 - iCurrentBuffer;

    // Make sure we have the latest buffer at buffer 0
    if (iCurrentBuffer != 0)
        cutilSafeCall( cudaMemcpy(pingpongColor[0], pingpongColor[1], sizeTexture, 
                   cudaMemcpyDeviceToDevice) ); 
}

// Perform prefix sum to link up voronoi vertices in the same row
void cudaPrefixSum()
{
    dim3 block = dim3(BLOCKX, BLOCKY); 
    dim3 grid = dim3(texSize / block.x, texSize / block.y); 

    iCurrentBuffer = 1;        // Working with pattern texture only

    for (int iStepLength = 1; iStepLength < texSize; iStepLength *= 2)
    {
        cutilSafeCall( cudaBindTexture(0, texPattern, pingpongPattern[iCurrentBuffer], sizeTexture) ); 

        kernelFlood1D<<< grid, block >>>(pingpongPattern[1 - iCurrentBuffer],
            texSize, log2Width, iStepLength); 

        cutilSafeCall( cudaUnbindTexture(texPattern) ); 

        // Update the target texture
        iCurrentBuffer = 1 - iCurrentBuffer;
    }

	cutilCheckError(); 

	// Make sure we have the latest buffer at buffer 0
    if (iCurrentBuffer != 0)
        cutilSafeCall( cudaMemcpy(pingpongPattern[0], pingpongPattern[1], sizeTexture, 
                   cudaMemcpyDeviceToDevice) ); 
}

/**********************************************************************
 * Construct the discrete voronoi diagram
 **********************************************************************/
void cudaDiscreteVoronoiDiagram() 
{
    // Initialization
    texSize = gpudtParams->fboSize; 

    cudaInitializePingpong(); 
    
    // Map points into the texture
    cudaMapPointsToTexture(); 

	pba2DCompute(phase1Band, phase2Band, phase3Band);
	iCurrentBuffer = 1; 

	cudaIslandDetection(); 
    cudaFindRealVoronoiVertices(); 
    cudaPrefixSum(); 

    // Deallocations
    cudaVoronoiDeallocation(); 

    return ; 
}
