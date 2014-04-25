/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 03 22 2013  *
 ********************************/

#ifndef KL_TIFF_IMAGE_IO_H
#define KL_TIFF_IMAGE_IO_H

#include "kl_image_buffer.h"
#include "kl_image_io.h"

bool query_tiff(const char* filename,unsigned int &width,unsigned int &height,unsigned int &bands);
bool read_tiff(const char* filename,unsigned int width,unsigned int height,unsigned int bands, unsigned char * inputbuf);
bool read_tiff(const char* filename,unsigned int width,unsigned int height,unsigned int bands, klRasterBufferPointer rbp);
klRasterBufferPointer read_tiff (const char* filename);
void writeTiffFile(const char* fileName,	klRasterBufferPointer buffer);

class klTIFFSinkFunctor : public klImageFileSink
 {
 public :

	 klTIFFSinkFunctor(const char* fileName,	klRasterBufferPointer buffer)
		 : klImageFileSink(fileName, buffer)
	 {
	 }

	 void operator()(void)
	 {
		 writeTiffFile(_filename, _rbp);
	 }

 };

class klTIFFSourceFunctor : public klImageFileSource
{

public :

	klTIFFSourceFunctor(const char* fileName) //: klImageFileSource(fileName)
	{
		_filename = fileName;
	}

	bool queryImage()
	{
		unsigned int width=0;
		unsigned int height=0;
		unsigned int bands=0;

		bool ans = query_tiff (getFileName(), width, height, bands);
		
		if(ans ==1)
		{
			setImageParameters( width, height,bands, 8);
		}

		return ans;
	}

	klRasterBufferPointer operator()(void)
	{
		klRasterBufferPointer rbp =  read_tiff (_filename);
		return rbp;
	}

};
 

























#endif //KL_TIFF_IMAGE_IO_H