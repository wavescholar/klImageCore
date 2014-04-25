/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/

#include "kl_image_processing_functors.h"

#include "kl_ppm_image_io.h"

/* These are various low level image processing functions for handling the types of
   BigTiff subimages that are used in various digital pathology whole slide imaging devices
*/

//Rotates the greyscale WSI from the Hamamatsu.
extern "C" bool klBiggTiffSubDirRotateImage(const char* macro,const char* thumbnail,const char* ofile)
{
	unsigned int thumbwidth=0;
	unsigned int thumbheight=0;
	unsigned int thumbbands=0;

	klRasterBufferPointer thumbRBP;
	if(	query_ppm (thumbnail, thumbwidth, thumbheight,thumbbands))
	{
		thumbRBP=new klPackedHeapRasterBuffer<unsigned char>(thumbwidth,thumbheight, thumbbands);
		read_ppm (thumbnail, thumbwidth, thumbheight, thumbbands, thumbRBP->buffer());
	}
	else
	{
		throw "Bad input file for thumbnail image in MacroThumb_9_0(char* macro, char* thumb,char* ofile)";
	}

	unsigned int macrowidth=0;
	unsigned int macroheight=0;
	unsigned int macrobands=0;
	klRasterBufferPointer macroRBP;
	if(	query_ppm (macro, macrowidth, macroheight,macrobands))
	{
		macroRBP=new klPackedHeapRasterBuffer<unsigned char>(macrowidth,macroheight, macrobands);
		read_ppm (macro, macrowidth, macroheight, macrobands, macroRBP->buffer());
	}
	else
	{
		throw "Bad imput file for label image in MacroThumb_9_0(char* macro, char* thumb,char* ofile)";

	}	
	if(macrobands!=thumbbands)
	{
		throw "Bad imput file for image in MacroThumb_9_0(char* macro, char* thumb,char* ofile)";
	}
	//write_ppm ("MACRO_THUMB_macro_INPUT.ppm", macroRBP->width(), macroRBP->height(),macroRBP->buffer());
	//write_ppm ("MACRO_THUMB_thumb_INPUT.ppm", thumbRBP->width(), thumbRBP->height(),thumbRBP->buffer());

	//double angle=-90;
	//int interpolate=IPPI_INTER_NN;
	//klRotateFunctor<unsigned char> klro_u8(macroRBP, angle, interpolate);
	//klRasterBufferPointer macro_rotated =klro_u8();
	
	size_t outbands=macroRBP->numBands();
	size_t outwidth =macroRBP->width();
	size_t outheight=macroRBP->height(); 
	size_t outbandStride=macroRBP->bandStride();
	size_t outxStride =macroRBP->xStride();
	size_t outyStride= macroRBP->yStride();
		
	unsigned char* rotated_macro = new unsigned char[outheight*outwidth*outbands];
	
	int i=0;
	int j=0;
	int k=0;
	unsigned int pix_stride=3;
	unsigned int width=macroRBP->width();
	unsigned int height =macroRBP->height();
	unsigned char* buf=macroRBP->buffer();
	for(i=0;i<width;i++)
	{
		for(j=height-1, k=0;j>=0;j--, k++)
		{
			unsigned char R=*(buf+j*outyStride +i*outxStride +0);
			unsigned char G=*(buf+j*outyStride +i*outxStride +1);
			unsigned char B=*(buf+j*outyStride +i*outxStride +2);

			*(rotated_macro+ i*height*pix_stride +k*pix_stride +0)=R;
			*(rotated_macro+ i*height*pix_stride +k*pix_stride +1)=G;
			*(rotated_macro+ i*height*pix_stride +k*pix_stride +2)=B;
		}
	}
	write_ppm (ofile, outheight, outwidth,rotated_macro);
	//write_ppm (ofile,  macro_rotated->width(), macro_rotated->height(),macro_rotated->buffer());
	return true;
}

//This is called if (!isLabel && isMacro && isThumb) and we want to make a color tumbnail.  We assume the label is in the top part of the WSI Image.
//For Hamamatsu, this is the case. 
extern "C" bool klBiggTiffSubDirMacroThumb(const char* macro,const char* thumbnail,const char* ofile)
{
	unsigned int thumbwidth=0;
	unsigned int thumbheight=0;
	unsigned int thumbbands=0;

	klRasterBufferPointer thumbRBP;
	if(	query_ppm (thumbnail, thumbwidth, thumbheight,thumbbands))
	{
		thumbRBP=new klPackedHeapRasterBuffer<unsigned char>(thumbwidth,thumbheight, thumbbands);
		read_ppm (thumbnail, thumbwidth, thumbheight, thumbbands, thumbRBP->buffer());
	}
	else
	{
		throw "Bad input file for thumbnail image in MacroThumb_9_0(char* macro, char* thumb,char* ofile)";
	}

	unsigned int macrowidth=0;
	unsigned int macroheight=0;
	unsigned int macrobands=0;
	klRasterBufferPointer macroRBP;
	if(	query_ppm (macro, macrowidth, macroheight,macrobands))
	{
		macroRBP=new klPackedHeapRasterBuffer<unsigned char>(macrowidth,macroheight, macrobands);
		read_ppm (macro, macrowidth, macroheight, macrobands, macroRBP->buffer());
	}
	else
	{
		throw "Bad imput file for label image in MacroThumb_9_0(char* macro, char* thumb,char* ofile)";

	}	
	if(macrobands!=thumbbands)
	{
		throw "Bad imput file for image in MacroThumb_9_0(char* macro, char* thumb,char* ofile)";
	}
	//write_ppm ("MACRO_THUMB_macro_INPUT.ppm", macroRBP->width(), macroRBP->height(),macroRBP->buffer());
	//write_ppm ("MACRO_THUMB_thumb_INPUT.ppm", thumbRBP->width(), thumbRBP->height(),thumbRBP->buffer());

	double scaleFactor=0.31450827653359298928919182083739;//
	unsigned int lwn=macrowidth * scaleFactor;
	klRasterBufferPointer labelRBP=new klPackedHeapRasterBuffer<unsigned char>(lwn,macroRBP->height() , macroRBP->numBands());

	klRect iRoi1(0,0,labelRBP->width(),labelRBP->height() );
	klRect oRoi1(0,0,labelRBP->width(),labelRBP->height() );

	klCopyROIFunctor<unsigned char> kl_cf_u8_step1(labelRBP ,  macroRBP,  iRoi1, oRoi1);
	kl_cf_u8_step1();
	//write_ppm ("MACRO_THUMB_LABELEXTRACT_1.ppm", labelRBP->width(), labelRBP->height(),labelRBP->buffer());

	scaleFactor = (double) labelRBP->height() / (double)thumbRBP->width()   ;
	
	double xFactor=scaleFactor; 	
	double yFactor=scaleFactor;
	int interpolate=IPPI_INTER_NN;
	klResizeFunctor<unsigned char> klrs_thumb_u8(thumbRBP, xFactor, yFactor, interpolate);
	klRasterBufferPointer thumb_resampled =klrs_thumb_u8();
	//write_ppm ("MACRO_THUMB_RESAMPLED_THUMB_2.ppm", thumb_resampled->width(), thumb_resampled->height(),thumb_resampled->buffer());

	double angle=90;
		
	klRotateFunctor<unsigned char> klro_u8(thumb_resampled, angle, interpolate);
	klRasterBufferPointer thumb_rotated =klro_u8();
	//write_ppm ("MACRO_THUMB_ROTATED_3.ppm", thumb_rotated->width(), thumb_rotated->height(),thumb_rotated->buffer());

	klRasterBufferPointer fmacroRBP=new klPackedHeapRasterBuffer<unsigned char>(labelRBP->width()+thumb_resampled->height() ,labelRBP->height() , labelRBP->numBands());

	//Do a memset one the RBP
	unsigned char* bufMem=fmacroRBP->buffer();
	unsigned int bufSize=fmacroRBP->yStride()*fmacroRBP->height();
	memset(bufMem,128,bufSize);
	//write_ppm ("MACRO_THUMB_fMacroRBP_Memset_4.ppm", fmacroRBP->width(), fmacroRBP->height(),fmacroRBP->buffer());

	klRect iRoi2(0,0,labelRBP->width(),labelRBP->height() );
	klRect oRoi2(0,0,labelRBP->width(),labelRBP->height() );

	klCopyROIFunctor<unsigned char> kl_cf_u8_step2( fmacroRBP,labelRBP ,  iRoi2, oRoi2);
	kl_cf_u8_step2();
	//write_ppm ("MACRO_THUMB_copyROI_LABEINTOMCARO_5.ppm", fmacroRBP->width(), fmacroRBP->height(),fmacroRBP->buffer());

	klRect iRoi3(0,(thumb_rotated->height() - fmacroRBP->height())/2.0,thumb_rotated->width(),labelRBP->height() );
	klRect oRoi3(labelRBP->width(),0,thumb_rotated->width(),labelRBP->height() );

	klCopyROIFunctor<unsigned char> kl_cf_u8_step3(fmacroRBP  ,thumb_rotated ,oRoi3 ,iRoi3 );
	kl_cf_u8_step3();
	//write_ppm ("MACRO_THUMB_FINAL_6.ppm",fmacroRBP);

	write_ppm (ofile,  fmacroRBP->width(), fmacroRBP->height(),fmacroRBP->buffer());
	return true;
}

//This is called if(isThumb && isLabel) which is the case for older Aperio images.  The thnumbnail indicates the image data
//actually in the file.  There is no WSI in this case.  These types of images also come from exports from the Aperio ScanScope software.
extern "C" bool klBiggTiffSubDirMacro(const char* thumbnail,const char* label,const char* ofile)
{

	unsigned int thumbwidth=0;
	unsigned int thumbheight=0;
	unsigned int thumbbands=0;

	klRasterBufferPointer thumbRBP;
	if(	query_ppm (thumbnail, thumbwidth, thumbheight,thumbbands))
	{
		thumbRBP=new klPackedHeapRasterBuffer<unsigned char>(thumbwidth,thumbheight, thumbbands);
		read_ppm (thumbnail, thumbwidth, thumbheight, thumbbands, thumbRBP->buffer());
	}
	else
	{
		throw "Bad input file for thumbnail image in klIPP_Make_SlideImage(char* thumbnail, char* label)";
	}

	unsigned int labelwidth=0;
	unsigned int labelheight=0;
	unsigned int labelbands=0;
	klRasterBufferPointer labelRBP;
	if(	query_ppm (label, labelwidth, labelheight,labelbands))
	{
		labelRBP=new klPackedHeapRasterBuffer<unsigned char>(labelwidth,labelheight, labelbands);
		read_ppm (label, labelwidth, labelheight, labelbands, labelRBP->buffer());
	}
	else
	{
		throw "Bad imput file for label image in klIPP_Make_SlideImage(char* thumbnail, char* label)";

	}	
	if(labelbands!=thumbbands)
	{
		throw "Bad imput file for image in klIPP_Make_SlideImage(char* thumbnail, char* label) - Labael and Thumb bands must be the same";
	}
	//write_ppm ("MACRO_LABEL_INPUT.ppm", labelRBP->width(), labelRBP->height(),labelRBP->buffer());
	//write_ppm ("MACRO_THUMB_INPUT.ppm", thumbRBP->width(), thumbRBP->height(),thumbRBP->buffer());

	unsigned int lhn=thumbheight;
	double scaleFactor=double(lhn)/(double)labelheight;
	unsigned int lwn=labelwidth * scaleFactor;
	double xFactor=scaleFactor; 	
	double yFactor=scaleFactor;
	int interpolate=IPPI_INTER_NN;
	klResizeFunctor<unsigned char> klrs_u8(labelRBP, xFactor, yFactor, interpolate);
	klRasterBufferPointer label_resampled =klrs_u8();
	//write_ppm ("MACRO_LABEL_RESAMPLED_u8_C3R.ppm", label_resampled->width(), label_resampled->height(),label_resampled->buffer());


	///////////////////////////////////IF WE USE MACRO - CROP LEFT 1/2 LABEL WIDTH///
	klRasterBufferPointer thumb_crop=new klPackedHeapRasterBuffer<unsigned char>(thumbwidth-1.0/2.0*lwn -180 ,thumbheight, labelbands);
	klRect thciRoi2(0  ,0,thumb_crop->width() ,thumb_crop->height());
	klRect thcoRoi2(1.0/2.0* lwn,0,thumb_crop->width() ,thumb_crop->height());

	klCopyROIFunctor<unsigned char> kl_thc_cf_u8_step2(thumb_crop,thumbRBP,  thciRoi2, thcoRoi2);
	kl_thc_cf_u8_step2();
	//write_ppm ("MACRO_THUMB_CROP.ppm", thumb_crop->width(), thumb_crop->height(),thumb_crop->buffer());

	//Now cat the label and thumnail together
	klRasterBufferPointer label_thumb_catRBP=new klPackedHeapRasterBuffer<unsigned char>(lwn+thumb_crop->width(),thumb_crop->height(), labelbands);
	
	//Do a memset one the RBP
	unsigned char* bufMem=label_thumb_catRBP->buffer();
	unsigned int bufSize=label_thumb_catRBP->yStride()*label_thumb_catRBP->height();
	memset(bufMem,128,bufSize);

	klRect iRoi2(lwn,0,thumb_crop->width(),thumb_crop->height() );
	klRect oRoi2(0,0,thumb_crop->width(),thumb_crop->height() );

	klCopyROIFunctor<unsigned char> kl_cf_u8_step2( label_thumb_catRBP,  thumb_crop,  iRoi2, oRoi2);
	kl_cf_u8_step2();
	//write_ppm ("MACRO_label_thumb_catRBP_Step1_u8_C3R.ppm", label_thumb_catRBP->width(), label_thumb_catRBP->height(),label_thumb_catRBP->buffer());

	klRect iRoi(0,0,lwn,lhn);
	klRect oRoi(0,0,lwn,lhn);

	klCopyROIFunctor<unsigned char> kl_cf_u8( label_thumb_catRBP,  label_resampled,  iRoi, oRoi);
	kl_cf_u8();
	//write_ppm ("MACRO_label_thumb_catRBP_Step2_u8_C3R.ppm", label_thumb_catRBP->width(), label_thumb_catRBP->height(),label_thumb_catRBP->buffer());

	size_t outbands=label_thumb_catRBP->numBands();
	size_t outwidth =label_thumb_catRBP->width();
	size_t outheight=label_thumb_catRBP->height(); 
	size_t outbandStride=label_thumb_catRBP->bandStride();
	size_t outxStride =label_thumb_catRBP->xStride();
	size_t outyStride= label_thumb_catRBP->yStride();
		
	unsigned char* rotated_macro = new unsigned char[outheight*outwidth*outbands];
	
	int i=0;
	int j=0;
	int k=0;
	unsigned int pix_stride=3;
	unsigned int width=label_thumb_catRBP->width();
	unsigned int height =label_thumb_catRBP->height();
	unsigned char* buf=label_thumb_catRBP->buffer();
	for(i=0;i<width;i++)
	{
		for(j=height-1, k=0;j>=0;j--, k++)
		{
			unsigned char* iPtr = buf+j*outyStride +i*outxStride;

			unsigned char R=*(iPtr  +0);
			unsigned char G=*(iPtr  +1);
			unsigned char B=*(iPtr  +2);

			unsigned char* oPtr =rotated_macro+ i*height*pix_stride +k*pix_stride;

			*(oPtr +0)=R;
			*(oPtr +1)=G;
			*(oPtr +2)=B;
		}
	}
	
	write_ppm (ofile, outheight, outwidth,rotated_macro);
	delete rotated_macro;
	return true;
}

//This is called when (isMacro && isLabel).  This should be the default case.
//The label image is not included in the WSI.  This is most likely a scanner specific setting, but we assume it is not there and place 
//the label provided in the proper location of the WSI.
extern "C" bool klBiggTiffSubDirSlideImage(const char* thumbnail,const  char* label,const char* ofile)
{

	unsigned int thumbwidth=0;
	unsigned int thumbheight=0;
	unsigned int thumbbands=0;
	klRasterBufferPointer thumbRBP;
	if(	query_ppm (thumbnail, thumbwidth, thumbheight,thumbbands))
	{
		thumbRBP=new klPackedHeapRasterBuffer<unsigned char>(thumbwidth,thumbheight, thumbbands);
		read_ppm (thumbnail, thumbwidth, thumbheight, thumbbands, thumbRBP->buffer());
	}
	else
	{
		throw "Bad imput file for thumbnail image in klIPP_Make_SlideImage(char* thumbnail, char* label)";
	}

	unsigned int labelwidth=0;
	unsigned int labelheight=0;
	unsigned int labelbands=0;
	klRasterBufferPointer labelRBP;
	if(	query_ppm (label, labelwidth, labelheight,labelbands))
	{
		labelRBP=new klPackedHeapRasterBuffer<unsigned char>(labelwidth,labelheight, labelbands);
		read_ppm (label, labelwidth, labelheight, labelbands, labelRBP->buffer());
	}
	else
	{
		throw "Bad imput file for label image in klIPP_Make_SlideImage(char* thumbnail, char* label)";

	}	
	if(labelbands!=thumbbands)
	{
		throw "Bad imput file for image in klIPP_Make_SlideImage(char* thumbnail, char* label) - Labael and Thumb bands must be the same";
	}
	write_ppm ("MSI_LABEL_INPUT.ppm", labelRBP->width(), labelRBP->height(),labelRBP->buffer());
	write_ppm ("MSI_THUMB_INPUT.ppm", thumbRBP->width(), thumbRBP->height(),thumbRBP->buffer());

	unsigned int lhn=thumbheight;
	double scaleFactor=double(lhn)/(double)labelheight;
	unsigned int lwn=labelwidth * scaleFactor;
	double xFactor=scaleFactor; 	
	double yFactor=scaleFactor;
	int interpolate=IPPI_INTER_NN;
	klResizeFunctor<unsigned char> klrs_u8(labelRBP, xFactor, yFactor, interpolate);
	klRasterBufferPointer label_resampled =klrs_u8();
	//write_ppm ("MSI_LABEL_RESAMPLED_u8_C3R.ppm", label_resampled->width(), label_resampled->height(),label_resampled->buffer());
	//Now cat the label and thumnail together
	klRasterBufferPointer label_thumb_catRBP=new klPackedHeapRasterBuffer<unsigned char>(lwn+thumbwidth,thumbheight, labelbands);

	klRect iRoi2(lwn,0,thumbwidth,thumbheight);
	klRect oRoi2(0,0,thumbwidth,thumbheight);

	klCopyROIFunctor<unsigned char> kl_cf_u8_step2( label_thumb_catRBP,  thumbRBP,  iRoi2, oRoi2);
	kl_cf_u8_step2();
	//write_ppm ("MSI_label_thumb_catRBP_Step1_u8_C3R.ppm", label_thumb_catRBP->width(), label_thumb_catRBP->height(),label_thumb_catRBP->buffer());

	klRect iRoi(0,0,lwn,lhn);
	klRect oRoi(0,0,lwn,lhn);

	klCopyROIFunctor<unsigned char> kl_cf_u8( label_thumb_catRBP,  label_resampled,  iRoi, oRoi);
	kl_cf_u8();
	//write_ppm ("MSI_label_thumb_catRBP_Step2_u8_C3R.ppm", label_thumb_catRBP->width(), label_thumb_catRBP->height(),label_thumb_catRBP->buffer());

	size_t outbands=label_thumb_catRBP->numBands();
	size_t outwidth =label_thumb_catRBP->width();
	size_t outheight=label_thumb_catRBP->height(); 
	size_t outbandStride=label_thumb_catRBP->bandStride();
	size_t outxStride =label_thumb_catRBP->xStride();
	size_t outyStride= label_thumb_catRBP->yStride();
		
	unsigned char* rotated_macro = new unsigned char[outheight*outwidth*outbands];
	
	int i=0;
	int j=0;
	int k=0;
	unsigned int pix_stride=3;
	unsigned int width=label_thumb_catRBP->width();
	unsigned int height =label_thumb_catRBP->height();
	unsigned char* buf=label_thumb_catRBP->buffer();
	for(i=0;i<width;i++)
	{
		for(j=height-1, k=0;j>=0;j--, k++)
		{
			unsigned char* iPtr = buf+j*outyStride +i*outxStride;
			unsigned char* oPtr =rotated_macro+ i*height*pix_stride +k*pix_stride;

			unsigned char R=*(iPtr   +0);
			unsigned char G=*(iPtr   +1);
			unsigned char B=*(iPtr   +2);

			*(oPtr +0)=R;
			*(oPtr +1)=G;
			*(oPtr +2)=B;
		}
	}
	write_ppm (ofile, outheight, outwidth,rotated_macro);
	delete rotated_macro;
	return true;
}

//This assumes there is a green ROI indicater in the WSI image.  We do a rough
//calculation to determine if the image data in the SVS file contains most/all of the 
//tissue on the slide.
 extern "C" bool klBiggTiffSubDirSlideImage_LT(const char* thumbnail,const char* label,const char* ofile)
{

	unsigned int thumbwidth=0;
	unsigned int thumbheight=0;
	unsigned int thumbbands=0;

	klRasterBufferPointer thumbRBP;
	if(	query_ppm (thumbnail, thumbwidth, thumbheight,thumbbands))
	{
		thumbRBP=new klPackedHeapRasterBuffer<unsigned char>(thumbwidth,thumbheight, thumbbands);
		read_ppm (thumbnail, thumbwidth, thumbheight, thumbbands, thumbRBP->buffer());
	}
	else
	{
		throw "Bad imput file for thumbnail image in klIPP_Make_SlideImage(char* thumbnail, char* label)";
	}

	unsigned int labelwidth=0;
	unsigned int labelheight=0;
	unsigned int labelbands=0;
	klRasterBufferPointer labelRBP;
	if(	query_ppm (label, labelwidth, labelheight,labelbands))
	{
		labelRBP=new klPackedHeapRasterBuffer<unsigned char>(labelwidth,labelheight, labelbands);
		read_ppm (label, labelwidth, labelheight, labelbands, labelRBP->buffer());
	}
	else
	{
		throw "Bad imput file for label image in klIPP_Make_SlideImage(char* thumbnail, char* label)";

	}	
	if(labelbands!=thumbbands)
	{
		throw "Bad imput file for image in klIPP_Make_SlideImage(char* thumbnail, char* label) - Labael and Thumb bands must be the same";
	}
	write_ppm ("LT_9_0LABEL_INPUT.ppm", labelRBP->width(), labelRBP->height(),labelRBP->buffer());

	write_ppm ("LT_9_0THUMB_INPUT.ppm", thumbRBP->width(), thumbRBP->height(),thumbRBP->buffer());

	unsigned int lhn=thumbheight;
	double scaleFactor=double(lhn)/(double)labelheight;
	unsigned int lwn=labelwidth * scaleFactor;
	double xFactor=scaleFactor; 	
	double yFactor=scaleFactor;
	int interpolate=IPPI_INTER_NN;
	klResizeFunctor<unsigned char> klrs_u8(labelRBP, xFactor, yFactor, interpolate);
	klRasterBufferPointer label_resampled =klrs_u8();
	write_ppm ("LT_9_0LABEL_RESAMPLED_u8_C3R.ppm", label_resampled->width(), label_resampled->height(),label_resampled->buffer());
	//Now cat the label and thumnail together
	klRasterBufferPointer label_thumb_catRBP=new klPackedHeapRasterBuffer<unsigned char>(lwn+thumbwidth,thumbheight, labelbands);

	klRect iRoi2(lwn,0,thumbwidth,thumbheight);
	klRect oRoi2(0,0,thumbwidth,thumbheight);

	klCopyROIFunctor<unsigned char> kl_cf_u8_step2( label_thumb_catRBP,  thumbRBP,  iRoi2, oRoi2);
	kl_cf_u8_step2();
	write_ppm ("LT_9_0label_thumb_catRBP_Step1_u8_C3R.ppm", label_thumb_catRBP->width(), label_thumb_catRBP->height(),label_thumb_catRBP->buffer());

	klRect iRoi(0,0,lwn,lhn);
	klRect oRoi(0,0,lwn,lhn);

	klCopyROIFunctor<unsigned char> kl_cf_u8( label_thumb_catRBP,  label_resampled,  iRoi, oRoi);
	kl_cf_u8();
	write_ppm ("LT_9_0label_thumb_catRBP_Step2_u8_C3R.ppm", label_thumb_catRBP->width(), label_thumb_catRBP->height(),label_thumb_catRBP->buffer());

	size_t outbands=label_thumb_catRBP->numBands();
	size_t outwidth =label_thumb_catRBP->width();
	size_t outheight=label_thumb_catRBP->height(); 
	size_t outbandStride=label_thumb_catRBP->bandStride();
	size_t outxStride =label_thumb_catRBP->xStride();
	size_t outyStride= label_thumb_catRBP->yStride();
		
	unsigned char* rotated_macro = new unsigned char[outheight*outwidth*outbands];
	
	int i=0;
	int j=0;
	int k=0;
	unsigned int pix_stride=3;
	unsigned int width=label_thumb_catRBP->width();
	unsigned int height =label_thumb_catRBP->height();
	unsigned char* buf=label_thumb_catRBP->buffer();
	for(i=0;i<width;i++)
	{
		for(j=height-1, k=0;j>=0;j--, k++)
		{
			unsigned char R=*(buf+j*outyStride +i*outxStride +0);
			unsigned char G=*(buf+j*outyStride +i*outxStride +1);
			unsigned char B=*(buf+j*outyStride +i*outxStride +2);

			*(rotated_macro+ i*height*pix_stride +k*pix_stride +0)=R;
			*(rotated_macro+ i*height*pix_stride +k*pix_stride +1)=G;
			*(rotated_macro+ i*height*pix_stride +k*pix_stride +2)=B;
		}
	}
	write_ppm ("LT_9_0label_thumb_catRBP_Step3_u8_C3R.ppm", outheight, outwidth,rotated_macro);
	write_ppm (ofile, outheight, outwidth,rotated_macro);
	delete rotated_macro;
	return true;
}
 
 extern "C" double klBiggTiffSubDirROIToTissue_Ratio(const char* WSI)
{
	const char* infilename="WSI.ppm" ;
	unsigned int inwidth=0;
	unsigned int inheight=0;
	unsigned int inbands=0;
	klRasterBufferPointer lsrc;
	//Test read ppm
	if(	query_ppm (infilename, inwidth, inheight,inbands))
	{
		lsrc=new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, inbands);
		read_ppm (infilename, inwidth,inheight, inbands,lsrc->buffer());
	}
	else
	{
		throw "Bad imput file in void testKLImageFunctors()";
	}

	/////////////////////////////////////Perform a morphological opening to clean up image
	////////////////////////////////////MRopen =  errode+dilate 

	unsigned lkernelWidthDF=3;
	unsigned lkernelHeightDF=3;
	//Mask values. Only pixels that correspond to nonzero 
	//mask values are taken into account during operation.
	unsigned char* lkernelDF = new unsigned char[ lkernelWidthDF * lkernelHeightDF ];
	lkernelDF[0]=1;lkernelDF[1]=1;lkernelDF[2]=1;
	lkernelDF[3]=1;lkernelDF[4]=0;lkernelDF[5]=1;
	lkernelDF[6]=1;lkernelDF[7]=1;lkernelDF[8]=1;
	const klRasterBufferPointer lsrc_dilated=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
	klCopyFunctor<unsigned char> klcfdf_u8(lsrc, lsrc_dilated);
	klcfdf_u8();//Do the copy
	klDilateFunctor< unsigned char> kldf_u8( lsrc,  lsrc_dilated,  lkernelDF,  lkernelWidthDF, lkernelHeightDF);
	kldf_u8();
	delete lkernelDF;
	write_ppm("WSI_DILATED_u8_C3R.ppm", lsrc_dilated->width(), lsrc_dilated->height(),lsrc_dilated->buffer());
	
	
	unsigned lkernelWidthEF=3;
	unsigned lkernelHeightEF=3;
	unsigned char* lkernelEF = new unsigned char[ lkernelWidthEF * lkernelHeightEF ];
	lkernelEF[0]=1;lkernelEF[1]=1;lkernelEF[2]=1;
	lkernelEF[3]=1;lkernelEF[4]=0;lkernelEF[5]=1;
	lkernelEF[6]=1;lkernelEF[7]=1;lkernelEF[8]=1;
	const klRasterBufferPointer lsrc_erroded=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands());
	klCopyFunctor<unsigned char> klcfef_u8(lsrc_dilated, lsrc_erroded);
	klcfef_u8();//Do the copy
	
	klErodeFunctor< unsigned char> klef_u8( lsrc_dilated,  lsrc_erroded,  lkernelEF,  lkernelWidthEF, lkernelHeightEF);
	klef_u8();//Do the errode
	delete lkernelEF;
	write_ppm("WSI_ERRODED_u8_C3R.ppm", lsrc_erroded->width(), lsrc_erroded->height(),lsrc_erroded->buffer());

	//Make the src image the opened image
	klCopyFunctor<unsigned char> klcfdf_op_sr_u8(lsrc_erroded,lsrc);
	klcfdf_op_sr_u8();//Do the copy
	//////////////////////////////////////////END OPEN


	////////////////////////////THRESHOLD SRC/////////////////////////////
	//Maps [0,thresholdLow] U [thresholdHigh,MAXVAL(TYPE) ] -----> LowVal , HighVal
	double lthresholdLow=252;
	double lthresholdLowVal=0;

	double lthresholdHi=253;
	double lthresholdHiVal=255;

	bool luseLowThreshold=true;
	bool luseHighThreshold=true;
	
	bool luseThresholdVals=true;
	klThresholdFunctor<unsigned char> kltf_RBG_u8( lsrc, lthresholdHi, lthresholdLow,luseLowThreshold,luseHighThreshold,lthresholdHiVal,lthresholdLowVal, luseThresholdVals);
	kltf_RBG_u8();
	write_ppm ("WSI_THRESHOLDED_RGB.ppm", lsrc->width(), lsrc->height(),lsrc->buffer());
	//////////////////////////END THRESHOLD////////////////////////////////////

	const klRasterBufferPointer lsrc_logical_B1=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
	const klRasterBufferPointer lsrc_logical_B2=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
	const klRasterBufferPointer lsrc_logical_B3=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );

	const klRasterBufferPointer logical_result=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
		
	klCopyFunctor<unsigned char> klcflf_B1_u8(lsrc, lsrc_logical_B1,0);
	klcflf_B1_u8();
	write_ppm_single_band("WSI_B1.pgm", lsrc_logical_B1->width(), lsrc_logical_B1->height(),lsrc_logical_B1->buffer());
	
	klCopyFunctor<unsigned char> klcflf_B2_u8(lsrc, lsrc_logical_B2,1);
	klcflf_B2_u8();
	write_ppm_single_band("WSI_B2.pgm", lsrc_logical_B2->width(), lsrc_logical_B2->height(),lsrc_logical_B2->buffer());

	klCopyFunctor<unsigned char> klcflf_B3_u8(lsrc, lsrc_logical_B3,2);
	klcflf_B3_u8();
	write_ppm_single_band("WSI_B3.pgm", lsrc_logical_B3->width(), lsrc_logical_B3->height(),lsrc_logical_B3->buffer());

	klLogicalFunctor klflf_u8(lsrc_logical_B1, lsrc_logical_B2, logical_result,klLogicalFunctor::LogicalFunctorOperationType::AND);
	klflf_u8();
	write_ppm_single_band("WSI_LOGICAL_C1R.pgm", logical_result->width(), logical_result->height(),logical_result->buffer());

	klFillHolesFunctor<unsigned char> klfhf_NOT_GREEN_u8(lsrc_logical_B2,0,255);
	klfhf_NOT_GREEN_u8();
	write_ppm_single_band ("WSI_HOLEFILL_B2.pgm", lsrc_logical_B2->width(), lsrc_logical_B2->height(),lsrc_logical_B2->buffer());

	return 42;
}