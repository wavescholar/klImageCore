/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "kl_image_io.h"

//////////////////////////////////////////PPM/////////////////////////////////////////////////////////
//Only use this for unpadded buffers where linestride = width* precision * bands
	//Write the ppm header 
	//PBM is for bitmaps (black and white, no grays)
	//PGM is for grayscale
	//PPM is for "pixmaps" which represent full RGB color.
	//Each file starts with a two-byte file descriptor (in ASCII) that explains the type of file it is 
	//(PBM, PGM, and PPM) and its encoding (ASCII or binary). 
	//The descriptor is a capital P followed by a single digit number.

	//File Descriptor Type Encoding 
	//P1 Portable bitmap ASCII 
	//P2 Portable graymap ASCII 
	//P3 Portable pixmap ASCII 
	//P4 Portable bitmap Binary 
	//P5 Portable graymap Binary 
	//P6 Portable pixmap Binary 

void write_ppm(const char* filename, int width, int height, unsigned char * buf)
{
	FILE * ppmfile;

	ppmfile = fopen (filename, "wb");

	if (!ppmfile) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+=" void write_ppm(char* filename, int width, int height, unsigned char * buf) :  Could not open file for writing PPM image data";
		throw err;
	}

	fprintf (ppmfile, "P6\n%d %d\n255\n", width, height);
	unsigned int stepSize=3 * width;
	for (int i=0; i<height; i++)
		fwrite (&buf[i*stepSize], 3 * width, 1, ppmfile);

	flushall();
	fclose(ppmfile);
}

void write_ppm_single_band (const char* filename, unsigned int width,unsigned int height, unsigned char * buf)
{
	FILE * ppmfile;

	ppmfile = fopen (filename, "wb");

	if (!ppmfile) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+=" void write_ppm_single_band (const char* filename, unsigned int width,unsigned int height, unsigned char * buf) :  Could not open file for writing PPM image data";
		throw err;
	}

	fprintf (ppmfile, "P5\n%d %d\n255\n", width, height);
	unsigned int stepSize=1 * width;
	for (int i=0; i<height; i++)
		fwrite (&buf[i*stepSize], 1 * width, 1, ppmfile);
	flushall();
	fclose(ppmfile);
}

bool read_ppm (const char* filename,unsigned int width,unsigned int height,unsigned int bands, unsigned char * inputbuf)
{
	FILE * ppmfile;

	ppmfile = fopen (filename, "rb");

	if (!ppmfile) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="bool read_ppm (const char* filename,unsigned int width,unsigned int height,unsigned int bands, unsigned char * inputbuf) :  Could not open file for reading PPM image data";
		throw err;
	}
	FILE* pf=ppmfile;
	unsigned int PPMREADBUFLEN = 256;
	char buf[256];
	char * t;   
	unsigned int w, h, d;  
	int r;         
	if (pf == NULL) 
	{
		return NULL;
	} 
	t = fgets(buf, PPMREADBUFLEN, pf); 
	if ( (t == NULL) || ( strncmp(buf, "P6\n", 3) != 0 ) )
	{
		return NULL; 
	}

	do        
	{ /* Px formats can have # comments after first line */    
		t = fgets(buf, PPMREADBUFLEN, pf);        
		if ( t == NULL )
			return NULL; 
	} 
	while ( strncmp(buf, "#", 1) == 0 );     
	r = sscanf(buf, "%u %u", &w, &h);   
	if ( r < 2 )
		return NULL;       
	// The program fails if the first byte of the image is equal to 32. because 
	// the fscanf eats the space and the image is read with some bit less     
	r = fscanf(pf, "%u\n", &d);  
	if ( (r < 1) || ( d != 255 ) ) return NULL;  
	unsigned char* imagebuf =inputbuf;
	size_t rd;
	if ( imagebuf != NULL )    
	{       
		rd = fread(imagebuf, sizeof(unsigned char), w*h*3, pf);   
	}
	if ( rd < w*h )       
	{           
		delete imagebuf;      
		return NULL;      
	}          
	flushall();
	fclose(ppmfile);
}//Only use this for unpadded buffers where linestride = width* precision * bands
bool query_ppm (const char* filename,unsigned int &width,unsigned int &height,unsigned int &bands)
{
	FILE * ppmfile;

	ppmfile = fopen (filename, "rb");

	if (!ppmfile) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="bool query_ppm (const char* filename,unsigned int &width,unsigned int &height,unsigned int &bands):  Could not open file for querying PPM image data";
		throw err;
	}
	//Read the ppm header 
	//PBM is for bitmaps (black and white, no grays)
	//PGM is for grayscale
	//PPM is for "pixmaps" which represent full RGB color.
	//Each file starts with a two-byte file descriptor (in ASCII) that explains the type of file it is 
	//(PBM, PGM, and PPM) and its encoding (ASCII or binary). 
	//The descriptor is a capital P followed by a single digit number.

	//File Descriptor Type Encoding 
	//P1 Portable bitmap ASCII 
	//P2 Portable graymap ASCII 
	//P3 Portable pixmap ASCII 
	//P4 Portable bitmap Binary 
	//P5 Portable graymap Binary 
	//P6 Portable pixmap Binary 
	FILE* pf=ppmfile;
	unsigned int PPMREADBUFLEN = 1024;
	char buf[1024];
	char * t;   
	unsigned int w, h, d;  
	int r;         
	if (pf == NULL) 
	{
		return false;
	} 
	t = fgets(buf, PPMREADBUFLEN, pf); 
	if ( (t == NULL) || ( strncmp(buf, "P6\n", 3) != 0 ) )
	{
		return false;
	}

	do        
	{ /* Px formats can have # comments after first line */    
		t = fgets(buf, PPMREADBUFLEN, pf);        
		if ( t == NULL )
		{
			return false; 
		}
	} 
	while ( strncmp(buf, "#", 1) == 0 );     
	r = sscanf(buf, "%u %u", &w, &h);   
	if ( r < 2 )
	{
		return false;       
	}
	// The program fails if the first byte of the image is equal to 32. because 
	// the fscanf eats the space and the image is read with some bit less     
	r = fscanf(pf, "%u\n", &d);  
	if ( (r < 1) || ( d != 255 ) ) 
	{
		return false;
	}

	width=w;
	height=h;
	bands=3;

	flushall();
	fclose(ppmfile);
	return true;


}

void write_ppm(const char* filename, klRasterBufferPointer klrbp)
{
	int width=klrbp->width(); 
	int height=klrbp->height(); 
	unsigned char * buf=klrbp->buffer();
	FILE * ppmfile;

	ppmfile = fopen (filename, "wb");

	if (!ppmfile) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="void write_ppm(char* filename, klRasterBufferPointer klrbp):  Could not open file for writing PPM image data";
		throw err;

	}

	fprintf (ppmfile, "P6\n%d %d\n255\n", width, height);
	unsigned int stepSize=klrbp->yStride();
	for (int i=0; i<height; i++)
		fwrite (&buf[i*stepSize], 3 * width, 1, ppmfile);

	flushall();
	fclose(ppmfile);
}

void write_ppm_single_band (const char* filename,klRasterBufferPointer klrbp)
{ 
	unsigned int width=klrbp->width();
	unsigned int height=klrbp->height();
	unsigned char * buf=klrbp->buffer();
	FILE * ppmfile;

	ppmfile = fopen (filename, "wb");

	if (!ppmfile) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="void write_ppm_single_band (const char* filename,klRasterBufferPointer klrbp):  Could not open file for writing PPM image data";
		throw err;
	}

	fprintf (ppmfile, "P5\n%d %d\n255\n", width, height);
	unsigned int stepSize=0;
	if(klrbp->numBands()==1)
		stepSize=klrbp->yStride();
	else
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="void write_ppm_single_band (const char* filename,klRasterBufferPointer klrbp):  Image buffer is not single band.";
		throw err;
	}
	for (int i=0; i<height; i++)
		fwrite (&buf[i*stepSize], 1 * width, 1, ppmfile);
	flushall();
	fclose(ppmfile);
}








