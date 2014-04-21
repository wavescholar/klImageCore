GPU-DT: A 2D Delaunay Triangulator using Graphics Hardware (version 2.1)
=======================================================================

Copyright (c) 2011 School of Computing, National University of Singapore. 
All rights reserved.

Project homepage: http://www.comp.nus.edu.sg/~tants/cdt.html

If you use GPU-DT and you like it or have comments on its usefulness etc., we 
would love to hear from you at <tants@comp.nus.edu.sg>. You may share with us
your experience and any possibilities that we may improve the work/code.

-------------------------------------------------------------------------

GPU-DT is a C Library that utilizes graphics hardware to compute Delaunay and constrained 
Delaunay triangulation. The result is a triangle mesh, each contain the
index of its 3 vertices and the three neighbor triangles.

References: 

  
1.  Computing Two-dimensional Delaunay Triangulation Using Graphics Hardware
    G.D. Rong, T.S. Tan, Thanh-Tung Cao and Stephanus
    The 2008 ACM Symposium on Interactive 3D Graphics and Games, 
    15--17 Feb, Redwood City, CA, USA, pp. 89--97. 

2.  Parallel Banding Algorithm to Compute Exact Distance Transform with the GPU
    T.T. Cao, K. Tang, A. Mohamed, and T.S. Tan
    The 2010 ACM Symposium on Interactive 3D Graphics and Games, 19-21 Feb, 
    Washington DC, USA.

3.  Proof of Correctness of the Digital Delaunay Triangulation Algorithm
    T.T. Cao, H. Edelsbrunner, and T.S. Tan
    Manuscript Jan, 2010. 

4.  Computing Two-Dimensional Constrained Delaunay Triangulation Using Graphics Hardware    
    Meng Qi, Thanh-Tung Cao, Tiow-Seng Tan
    Technical report (TRB3/11 (March 2011))


1. Requirement
==============
- CUDA Toolkit version 2.0 and above. 
- A GPU capable of running CUDA.

By default, GPU-DT performs all floating point computation in Double precision. 
You can also turn on the definition SINGLE_PRECISION (see gpudt.h) to switch 
to Single precision mode. 

To run GPU-DT with Double precision, you need a GPU with compute capability 1.3 (NVIDIA GT200 series onward). 
In Single precision mode, GPU-DT only requires a GPU with compute capability 1.1 (NVIDIA G8xxx series onward, 
except Geforce 8800GTX). 


2. Tested
=========
GPU-DT has been tested on NVIDIA Geforce 9500GT, GTX280, GTX 460, GTX 470, GTX 560, GTX 580 and Tesla C2050.

3. File format
==============
In order to using GPU-DT to compute the Delaunay triangulation for a set of points, or constrained Delaunay triangulation 
(if some edges in the triangulation are specified before hand ), user should provide input data files into GPU-DT. 
There are two kinds of data should be provided. One is the vertex.txt file, which states the vertices. The other one is 
the constraint.txt file, which states the specified edges. Each edge is specified by listing the indices of its two endpoints. 
This means that you must include its endpoints in the vertex list.


-  vertex.txt files 

    - First iterm: <# of vertices> 

    - Remaining iterms: <x> <y> (i.e., the coordinates for the points)

-  constraint.txt files 

    - First iterm: <# of constraints> 

    - Remaining iterms: <indice of endpoint1> <indice of endpoint2> 


Note: If you only want to compute the Delaunay triangulation for a set of points, the number of constraints should be "0".


4. In this folder
=================
The following files are included in this distribution:
	
	readme.txt		    The file you're reading now
	gpudt.cpp		    The main CPU code of GPU-DT
	gpudt.h		        Header file, include some configurations, 
				        instructions, and some useful macro. 
	*.cu			    CUDA source codes
	

The distribution includes a sample Visual Studio 2008 project using
GPU-DT with CUDA Toolkit 4.0 (32-bit) to compute Delaunay triangulation 
for a set of points or constrained Delaunay Triangulation for a set of PSLG. 
The triangulation is then drawn using OpenGL, and the 
user can zoom in and move around the triangle mesh. 

Note: When compiling the CUDA code using Double precision, you have to 
enable compute capability 1.3 using the switch -sm_13. If you use Single precision, 
you can use the switch -sm_11. 


5. Acknowledgements
===================
We acknowledge that the code in predicates.h is extracted from the file predicates.c 
obtained from the webpage http://www.cs.cmu.edu/~quake/robust.html. The code in 
cudaCCW.cu is also extracted from the same file, with some minor adjustment to make 
it work in CUDA. 

6. Change logs
+ Version 2.0 (31/05/2011)
   - Amended to work with CUDA Toolkit3.2
   - Amended to be suitable for both 32-bit and 64-bit machine
   - Amended to be suitable for 64-bit Linux machine
   - Amended to handle edges specified before hand (i.e. constrained Delauney Triangulation)
 
+ Version 2.1 (20/10/2011)
   - Implemented code optimization.
   - Already work with CUDA Toolkit4.0.
  
----------------------------------------------------------------------------------
Graphics, Geometry & Games Lab
School of Computing, National University of Singapore
Computing 1
13 Computing Drive
Singapore 117417
Repulic of Singapore
January 2011
----------------------------------------------------------------------------------
Please send bugs and comments to: tants@comp.nus.edu.sg
