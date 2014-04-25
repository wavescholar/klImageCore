/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/

#ifndef KL_IMAGE_IO_FACTORY_H
#define KL_IMAGE_IO_FACTORY_H

#include "kl_image_buffer.h"

#include "kl_image_io.h"

#ifdef KL_TIFF
#include "kl_tiff_image_io.h"
#endif

#ifdef KL_PPM
#include "kl_ppm_image_io.h"
#endif

class klImageFileSourceFactory
 {
 public:
	 static klImageFileSource* getFileSource(const char* fileName)
	 {
		 string str(fileName);
		 size_t found;

#ifdef KL_TIFF
		 found=str.find(".tif");
		 if (found!=string::npos)
		 {
			 klTIFFSourceFunctor*  klTif = new klTIFFSourceFunctor(fileName);

			 return klTif;
		 }

		 found=str.find(".tiff");
		 if (found!=string::npos)
		 {
			 klTIFFSourceFunctor*  klTif = new klTIFFSourceFunctor(fileName);

			 return klTif;
		 }
#endif 

#ifdef KL_JPEG
		 found=str.find(".jpg");
		 if (found!=string::npos)
		 {
			 klJPEGSourceFunctor* klJpg = new klJPEGSourceFunctor(fileName);

			 return klJpg;
		 }

		 found=str.find(".jpeg");
		 if (found!=string::npos)
		 {
			klJPEGSourceFunctor* klJpg = new klJPEGSourceFunctor(fileName);

			 return klJpg;
		 }
#endif 

#ifdef KL_PPM
		 found=str.find(".ppm");
		 if (found!=string::npos)
		 {
			 klPPMSourceFunctor* klPpm = new klPPMSourceFunctor(fileName);

			 return klPpm;
		 }
#endif
		 return NULL;

	 }

 };

























#endif //KL_IMAGE_IO_FACTORY_H