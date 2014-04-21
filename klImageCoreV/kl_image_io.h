/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/

#ifndef KL_IMAGE_IO_H
#define KL_IMAGE_IO_H

#include "kl_image_buffer.h"

bool query_jpeg(const char* filename,unsigned int &width,unsigned int &height,unsigned int &bands);
bool read_jpeg(const char* filename,unsigned int width,unsigned int height,unsigned int bands, unsigned char * inputbuf);

class klImageFileSource
{
protected:
	const char* _filename;

private:
	size_t inbands;
	size_t inwidth ;
	size_t inheight;
	size_t bitsPerPixel;

	public:

	//Checks image file exists and looks for magic number where appropriate.
	//Fills in the image charateristics, width,height,bands, datatype.
	virtual bool queryImage()=0;
	
	void setImageParameters(unsigned int width,unsigned int height,unsigned int bands, unsigned int bpp)
	{
	     inbands=width;
		 inwidth=height;
		 inheight=bands;
		 bitsPerPixel=bpp;
	}

	const char* getFileName()
	{
		return _filename;
	}

	size_t getBands(void)
	{
		if(inbands==0)
		{
			queryImage();
		}
		return inbands;
	}

	size_t getWidth(void) 
	{
		if(inwidth==0)
		{
			queryImage();
		}
		return inwidth;

	}
	size_t getHeight(void)
	{
		if(inheight==0)
		{
			queryImage();
		}
		return inheight;

	}

	size_t getBitsPerPixel(void)
	{
			if(bitsPerPixel==0)
		{
			queryImage();
		}
		return bitsPerPixel;
	}

	klRasterBufferPointer render()
	{
		klRasterBufferPointer klrbp  = this->operator()();
		return klrbp;
	}	
	virtual klRasterBufferPointer operator()(void)=0;
	
};

class klImageFileSink
 {
 protected:
	const char* _filename;
	klRasterBufferPointer _rbp;

 public:
	 klImageFileSink(const char* fileName,	klRasterBufferPointer buffer)
	 {
		 _filename = fileName;
		 _rbp = buffer;
	 }
	 void render(void)
	 {
		 this->operator()();
	 }

	 virtual void operator()(void)=0;
 };
  
class  klJPEGSourceFunctor : public klImageFileSource
{

public :

	klJPEGSourceFunctor(const char* fileName) //: klImageFileSource(fileName)
	{
		_filename = fileName;
	}

	bool queryImage()
	{
		unsigned int width=0;
		unsigned int height=0;
		unsigned int bands=0;

		bool ans = query_jpeg (getFileName(), width, height, bands);
		
		if(ans ==1)
		{
			setImageParameters( width, height,bands, 8);
		}

		return ans;
	}

	klRasterBufferPointer operator()(void)
	{		
		if(	queryImage() )
		{
			klRasterBufferPointer dst = new klPackedHeapRasterBuffer<unsigned char>(getWidth(),getHeight(), getBands() );

			read_jpeg (getFileName(), getWidth(),getHeight(), getBands() , dst->buffer());
			return dst;
		}
		else
			return NULL;
	}

};

class klJPEGSinkFunctor : public klImageFileSink
 {
 public :

	 klJPEGSinkFunctor(const char* fileName,	klRasterBufferPointer buffer)
		 : klImageFileSink(fileName, buffer)
	 {
	 }

	 void operator()(void)
	 {
		 
	 }

 };


class  klJPEG2000SourceFunctor : public klImageFileSource
{

public :

	klJPEG2000SourceFunctor(const char* fileName) //: klImageFileSource(fileName)
	{
		_filename = fileName;
	}

	bool queryImage()
	{
		return false;
	}

	klRasterBufferPointer operator()(void)
	{		
		if(	queryImage() )
		{
			klRasterBufferPointer dst = new klPackedHeapRasterBuffer<unsigned char>(getWidth(),getHeight(), getBands() );

			//read_jpeg (getFileName(), getWidth(),getHeight(), getBands() , dst->buffer());
			return dst;
		}
		else
			return NULL;
	}
};

class klJPEG2000SinkFunctor : public klImageFileSink
 {
 public :

	 klJPEG2000SinkFunctor(const char* fileName,	klRasterBufferPointer buffer)
		 : klImageFileSink(fileName, buffer)
	 {
	 }

	 void operator()(void)
	 {
		 
	 }

 };
























#endif //KL_IMAGE_IO_H