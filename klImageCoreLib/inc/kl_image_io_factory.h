/*******************************
 * WSCMP [2003] - [2012] WSCMP  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/

#ifndef KL_IMAGE_IO_FACTORY_H
#define KL_IMAGE_IO_FACTORY_H

#include "kl_image_buffer.h"

//bbc revisit 
#define KL_TIFF
#define KL_PPM
#define KL_JPG


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