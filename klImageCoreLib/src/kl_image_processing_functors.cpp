/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/ 

#include "kl_image_processing_functors.h"
#include "kl_ppm_image_io.h"
#include "kl_img_pca.h"

DWORD gdwTlsIndex;

//// If linking klMatrix lib - then we don't need this
//klMutex klThreadMap::lock;
//map<klThreadId, unsigned long> klThreadMap::threadMap;
//klMemMgr* klGlobalMemoryManager::_globalMemoryManager=NULL;

void klPadFunctor::operator()(void)
{
	unsigned char* outbuf=dst->buffer();
	size_t outbands=dst->numBands();
	size_t outwidth =dst->width();
	size_t outheight=dst->height(); 
	size_t outbandStride=dst->bandStride();
	size_t outxStride =dst->xStride();
	size_t outyStride= dst->yStride();

	unsigned char* inbuf=src->buffer();
	size_t inbands=src->numBands();
	size_t inwidth =src->width();
	size_t inheight=src->height(); 
	size_t inbandStride=src->bandStride();
	size_t inxStride =src->xStride();
	size_t inyStride= src->yStride();

	size_t writeBytes=outxStride/outbands;

	if(outbands!=inbands )
		throw "klPadFunctor: in and out bands should be the same for input and output buffers.";

	//First fill the roi
	unsigned int i;
	unsigned int j;
	unsigned int b;
	for (i=0; i<rect.h; i++)
	{
		for (j=0; j<rect.w; j++)
		{
			for(b=0;b<inbands;b++)
			{ 
				memcpy((outbuf+(i+rect.y0)*outyStride+(j+rect.x0)*outxStride+b*writeBytes),
					(inbuf+i*inyStride +j*inxStride+b*writeBytes),  writeBytes);
			}			
		}
	}

	//fill left 
	for(i=0;i<outheight;i++)
	{
		for(j=0;j<=rect.x0;j++)
		{
			for(b=0;b<inbands;b++)
			{
				memcpy( (outbuf + i*outyStride + j*outxStride+b*writeBytes), 
					(outbuf + i*outyStride + (2*rect.x0-j)*outxStride+b*writeBytes),	writeBytes);
			}
		}
	}

	//fill right
	for(i=0;i<outheight;i++)
	{
		for(j=0;j<(outwidth-rect.x0-rect.w);j++)
		{
			for(b=0;b<inbands;b++)
			{
				memcpy( (outbuf + i*outyStride + (rect.x0+rect.w+j)*outxStride+b*writeBytes),   
					(outbuf+i*outyStride+ (rect.x0+rect.w-j-1)*outxStride+b*writeBytes),	writeBytes);
			}
		}
	}

	//fill top 
	for(i=0;i<rect.y0;i++)
	{
		for(j=0;j<=outwidth;j++)
		{
			for(b=0;b<inbands;b++)
			{
				memcpy( (outbuf + i*outyStride + j*outxStride+b*writeBytes), 
					(outbuf + (2*rect.y0-i)*outyStride + (j)*outxStride+b*writeBytes),	writeBytes);
			}
		}
	}

	//fill bottom
	for(i=0;i<(outheight-rect.y0-rect.h )  ;i++)
	{
		for(j=0;j<outwidth;j++)
		{
			for(b=0;b<inbands;b++)
			{
				memcpy( (outbuf + (rect.y0+rect.h+i )*outyStride + j*outxStride+b*writeBytes), 
					(outbuf + (rect.y0+rect.h-i-1 )*outyStride + (j)*outxStride+b*writeBytes),	writeBytes);
			}
		}
	}
}

//MT ImageDemo
#include "kl_thread_workflow.h"

void ImageWork(string fileName,string outputPath)
{
	const char* infilename=fileName.c_str();
	unsigned int inwidth=0;
	unsigned int inheight=0;
	unsigned int inbands=0;
	
	klRasterBufferPointer lsrc;
	if(	query_ppm (infilename, inwidth, inheight,inbands))
	{
		lsrc=new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, inbands);
		read_ppm (infilename, inwidth,inheight, inbands,lsrc->buffer());
	}
	else
	{
		throw "Bad input file in void testKLImageFunctors()";
	}	
	{//Test the smart pointer x64 port went well
		size_t width=128;
		size_t height=128;
		size_t bands = 3;
		klRasterBufferPointer lsrc_No_boo_boo_pointer=new klPackedHeapRasterBuffer<unsigned char>(width,height, bands);
	}//YAY!
	
	string output =  outputPath;
	output.append("PCA_input.ppm");
	write_ppm(output.c_str(), lsrc->width(), lsrc->height(),lsrc->buffer());

	klPCAFunctor pca(lsrc);
	klRasterBufferPointer dst =  pca();
	const klRasterBufferPointer ldst_cptbipca=new klPackedHeapRasterBuffer<unsigned char> (dst->width(),dst->height(),3 );
	klCopySplitBandFunctor<unsigned char> kl_cfpitbipca_u8( dst,  ldst_cptbipca);
	kl_cfpitbipca_u8();
	output =  outputPath;
	output.append("PCA_PIXTOBAND.pgm");
	write_ppm_single_band(output.c_str(), ldst_cptbipca->width(), ldst_cptbipca->height()*3,ldst_cptbipca->buffer());

	{

	unsigned lkernelWidthDF=3;
	unsigned lkernelHeightDF=3;
	//Mask values. Only pixels that correspond to nonzero 
	//mask values are taken into account during operation.
	unsigned char* lkernelDF = new unsigned char[ lkernelWidthDF * lkernelHeightDF ];
	lkernelDF[0]=1;lkernelDF[1]=1;lkernelDF[2]=1;
	lkernelDF[3]=1;lkernelDF[4]=0;lkernelDF[5]=1;
	lkernelDF[6]=1;lkernelDF[7]=1;lkernelDF[8]=1;
	const klRasterBufferPointer lsrc_dilated=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
	klCopyFunctor<unsigned char> klcfdf_u8(dst, lsrc_dilated);
	klcfdf_u8();//Do the copy
	klDilateFunctor< unsigned char> kldf_u8( lsrc,  lsrc_dilated,  lkernelDF,  lkernelWidthDF, lkernelHeightDF);
	kldf_u8();
	delete lkernelDF;
	output =  outputPath;
	output.append("DILATED_u8_C3R.ppm");
	write_ppm(output.c_str(), lsrc_dilated->width(), lsrc_dilated->height(),lsrc_dilated->buffer());

	klApplyColorUnmixingBasis ssb(lsrc_dilated);
	klRasterBufferPointer dstssb =  ssb();
	output =  outputPath;	output.append("StainSpaceBasis_u8.ppm");
	write_ppm(output.c_str(), dstssb->width(), dstssb->height(),dstssb->buffer());


	//klHandERatio ssp(dstssb);
	klHandERatio ssp(lsrc_dilated);
	klRasterBufferPointer dstssp =  ssp();
	output =  outputPath;	output.append("StainPurity_u8_C3R.pgm");
	write_ppm_single_band(output.c_str(), dstssp->width(), dstssp->height(),dstssp->buffer());

	double lthresholdHi=196;
	double lthresholdLow=56;
	bool luseLowThreshold=true;
	bool luseHighThreshold=true;
	double lthresholdHiVal=255;
	double lthresholdLowVal=0;
	bool luseThresholdVals=true;
	const klRasterBufferPointer lsrc_thresh=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
	//Skip Stain Purity 
	klCopyFunctor<unsigned char> klcfth_u8(dstssp, lsrc_thresh);
	klcfth_u8();
	klThresholdFunctor<unsigned char> kltf_u8( lsrc_thresh, lthresholdHi, lthresholdLow,luseLowThreshold,luseHighThreshold,lthresholdHiVal,lthresholdLowVal, luseThresholdVals);
	kltf_u8();
	output =  outputPath;	output.append("THRESHOLDED_u8_C3R.pgm");
	write_ppm_single_band (output.c_str(), lsrc_thresh->width(), lsrc_thresh->height(),lsrc_thresh->buffer());



	/*const klRasterBufferPointer ldst_cptbissb=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),3 );
	klCopySplitBandFunctor<unsigned char> kl_cfpitbissb_u8( dstssb,  ldst_cptbissb);
	kl_cfpitbissb_u8();
	write_ppm_single_band ("SSB_PIXTOBAND.pgm", ldst_cptbissb->width(), ldst_cptbissb->height()*3,ldst_cptbissb->buffer());*/

	//unsigned lkernelWidthEF=3;
	//unsigned lkernelHeightEF=3;
	//unsigned char* lkernelEF = new unsigned char[ lkernelWidthEF * lkernelHeightEF ];
	//lkernelEF[0]=1;lkernelEF[1]=1;lkernelEF[2]=1;
	//lkernelEF[3]=1;lkernelEF[4]=0;lkernelEF[5]=1;
	//lkernelEF[6]=1;lkernelEF[7]=1;lkernelEF[8]=1;
	//const klRasterBufferPointer lsrc_erroded=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
	//klCopyFunctor<unsigned char> klcfef_u8(dstssb, lsrc_erroded);
	//klcfef_u8();//Do the copy
	

	//klErodeFunctor< unsigned char> klef_u8( dstssb,  lsrc_erroded,  lkernelEF,  lkernelWidthEF, lkernelHeightEF);
	//klef_u8();//Do the errode
	//write_ppm("ERRODED_u8_C3R.ppm", lsrc_erroded->width(), lsrc_erroded->height(),lsrc_erroded->buffer());
	//delete lkernelEF;

	const klRasterBufferPointer lsrc_logical_B1=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );

	klCopyFunctor<unsigned char> klcflf_B1_u8(lsrc_thresh, lsrc_logical_B1);
	klcflf_B1_u8();
	output =  outputPath;	output.append("LOGICAL_IN_B1.pgm");
	write_ppm_single_band(output.c_str(), lsrc_logical_B1->width(), lsrc_logical_B1->height(),lsrc_logical_B1->buffer());

	unsigned lkernelWidthO=3;
	unsigned lkernelHeightO=3;
	unsigned char* lkernelO = new unsigned char[ lkernelWidthO * lkernelHeightO ];
	lkernelO[0]=0;lkernelO[1]=1;lkernelO[2]=0;
	lkernelO[3]=1;lkernelO[4]=1;lkernelO[5]=1;
	lkernelO[6]=0;lkernelO[7]=1;lkernelO[8]=0;
	const klRasterBufferPointer lsrc_opened=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),3 );
	klOpenFunctor< unsigned char> klof_u8( lsrc,  lsrc_opened,  lkernelO,  lkernelWidthO, lkernelHeightO);
	klof_u8();
	output =  outputPath;	output.append("Opened_u8_C3R.ppm");
	write_ppm(output.c_str(), lsrc_opened->width(), lsrc_opened->height(),lsrc_opened->buffer());
	delete lkernelO;

	
	klCompositeMask klcm( lsrc, lsrc_thresh ,0.5,0,120,0);
	klRasterBufferPointer blended = klcm();
	output =  outputPath;	output.append("WhiteSpaceBlended_u8_C3R.ppm");
	write_ppm(output.c_str(), blended->width(), blended->height(),blended->buffer());

	}

	const klRasterBufferPointer ldst=new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, inbands);
	int lband=-1;
	klCopyFunctor<unsigned char> klcf_u8(lsrc, ldst);
	klcf_u8();
	output =  outputPath;	output.append("SRC_u8_C3R.ppm");
	write_ppm (output.c_str(), ldst->width(), ldst->height(),ldst->buffer());

	//Test filter functor
	unsigned int lkernelWidth=3;
	unsigned int lkernelHeight=3;
	unsigned char*  lkernel=new unsigned char[ lkernelWidth * lkernelHeight ];
	lkernel[0]= 0.0113*256;lkernel[1]= 0.0838*256;lkernel[2]= 0.0113*256;
	lkernel[3]= 0.0838*256;lkernel[4]= 0.0113*256;lkernel[5]= 0.08383*256;
	lkernel[6]= 0.0113*256;lkernel[7]= 0.0838*256;lkernel[8]= 0.0113*256;
	//unsigned char *  lkernel=new unsigned char[ lkernelWidth * lkernelHeight ];
	//lkernel[0]= 0;lkernel[1]= 0;lkernel[2]=0;
	//lkernel[3]= 0;lkernel[4]= 1;lkernel[5]= 0;
	//lkernel[6]= 0;lkernel[7]= 0;lkernel[8]= 0;

	klFilterFunctor<unsigned char> klff_u8(ldst, lkernel,  lkernelWidth, lkernelHeight);
	klff_u8();
	delete lkernel;
	output =  outputPath;	output.append("FILTERED_u8_C3R.ppm");
	write_ppm (output.c_str(), ldst->width(), ldst->height(),ldst->buffer());

	const klRasterBufferPointer ldst_color_to_gray=new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, 1);
	klColorToGrayFunctor<unsigned char> klctg_u8(lsrc,ldst_color_to_gray);
	klctg_u8();
	output =  outputPath;	output.append("COLOR_TO_GRAY.pgm");
	write_ppm_single_band (output.c_str(), ldst_color_to_gray->width(), ldst_color_to_gray->height(),ldst_color_to_gray->buffer());


	////////////////////////////////Test threshold functor
	//Maps [0,thresholdLow] U [thresholdHigh,MAXVAL(TYPE) ] -----> LowVal , HighVal
	double lthresholdHi=240;
	double lthresholdLow=32;
	bool luseLowThreshold=true;
	bool luseHighThreshold=true;
	double lthresholdHiVal=200;
	double lthresholdLowVal=40;
	bool luseThresholdVals=true;
	const klRasterBufferPointer lsrc_thresh=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
	klCopyFunctor<unsigned char> klcfth_u8(lsrc, lsrc_thresh);
	klcfth_u8();
	klThresholdFunctor<unsigned char> kltf_u8( lsrc_thresh, lthresholdHi, lthresholdLow,luseLowThreshold,luseHighThreshold,lthresholdHiVal,lthresholdLowVal, luseThresholdVals);
	kltf_u8();
	output =  outputPath;	output.append("THRESHOLDED_u8_C3R.ppm");
	write_ppm (output.c_str(), lsrc_thresh->width(), lsrc_thresh->height(),lsrc_thresh->buffer());


	/////////////////////////////Test morphological functors 
	//Our morphological structure elelment
	unsigned lkernelWidthEF=3;
	unsigned lkernelHeightEF=3;
	unsigned char* lkernelEF = new unsigned char[ lkernelWidthEF * lkernelHeightEF ];
	lkernelEF[0]=1;lkernelEF[1]=1;lkernelEF[2]=1;
	lkernelEF[3]=1;lkernelEF[4]=0;lkernelEF[5]=1;
	lkernelEF[6]=1;lkernelEF[7]=1;lkernelEF[8]=1;
	const klRasterBufferPointer lsrc_erroded=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
	klCopyFunctor<unsigned char> klcfef_u8(lsrc, lsrc_erroded);
	klcfef_u8();//Do the copy
	delete lkernelEF;

	klErodeFunctor< unsigned char> klef_u8( lsrc,  lsrc_erroded,  lkernelEF,  lkernelWidthEF, lkernelHeightEF);
	klef_u8();//Do the errode
	output =  outputPath;	output.append("ERRODED_u8_C3R.ppm");
	write_ppm(output.c_str(), lsrc_erroded->width(), lsrc_erroded->height(),lsrc_erroded->buffer());

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
	output =  outputPath;output.append("DILATED_u8_C3R.ppm");
	write_ppm(output.c_str(), lsrc_dilated->width(), lsrc_dilated->height(),lsrc_dilated->buffer());

	
	/////////////////////////////////////Test the rotate functor.
	//Let's write the file out with the ppm writer
	double angle=-45;
	//int interpolate=IPPI_INTER_CUBIC2P_BSPLINE;
	int interpolate=IPPI_INTER_NN;
	klRotateFunctor<unsigned char> klro_u8(lsrc, angle, interpolate);
	klRasterBufferPointer src_rotated =klro_u8();
	output =  outputPath;	output.append("ROTATED_u8_C3R.ppm");
	write_ppm (output.c_str(), src_rotated->width(), src_rotated->height(),src_rotated->buffer());

	///////////////////////Test resample functor.
	double xFactor=2;
	double yFactor=2;
	interpolate=IPPI_INTER_CUBIC2P_BSPLINE;
	klResizeFunctor<unsigned char> klrs_u8(lsrc, xFactor, yFactor, interpolate);
	klRasterBufferPointer src_resampled =klrs_u8();
	output =  outputPath;	output.append("RESAMPLED_u8_C3R.ppm");
	write_ppm (output.c_str(), src_resampled->width(), src_resampled->height(),src_resampled->buffer());


	//////////////////////Test roi  copy functor.
	klRect iRoi(128,128,256,256);
	klRect oRoi(128,128,256,256);
	klCopyROIFunctor<unsigned char> kl_cf_u8( src_rotated,  src_resampled,  iRoi, oRoi);
	kl_cf_u8();
	output =  outputPath;	output.append("COPYFUNCTION_DIFF_SIZE_u8_C3R.ppm");
	write_ppm (output.c_str(), src_rotated->width(), src_rotated->height(),src_rotated->buffer());

	klCopyROIFunctor<unsigned char> kl_cf_u8t( lsrc_thresh,  lsrc_dilated,  iRoi, oRoi);
	kl_cf_u8t();
	output =  outputPath;	output.append("COPYFUNCTION_SAME_SIZE_u8_C3R.ppm");
	write_ppm (output.c_str(), lsrc_thresh->width(), lsrc_thresh->height(),lsrc_thresh->buffer());
	
	/////////////////////Test Copy Pixel To Band Interleaved
	const klRasterBufferPointer ldst_cptbi=new klPackedHeapRasterBuffer<unsigned char> (src_rotated->width(),src_rotated->height(),src_rotated->numBands() );
	klCopySplitBandFunctor<unsigned char> kl_cfpitbi_u8( src_rotated,  ldst_cptbi);
	kl_cfpitbi_u8();
	output =  outputPath;	output.append("COPYFUNCTION_PIXTOBAND_u8_P3R.pgm");
	write_ppm_single_band (output.c_str(), ldst_cptbi->width(), ldst_cptbi->height()*3,ldst_cptbi->buffer());

	//Test logical functor - and the first and second bands
	const klRasterBufferPointer lsrc_logical_B1=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
	const klRasterBufferPointer lsrc_logical_B2=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
	const klRasterBufferPointer lsrc_logical_B3=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );

	const klRasterBufferPointer logical_result=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
		
	klCopyFunctor<unsigned char> klcflf_B1_u8(lsrc, lsrc_logical_B1,0);
	klcflf_B1_u8();
	output =  outputPath;	output.append("LOGICAL_IN_B1.pgm");
	write_ppm_single_band(output.c_str(), lsrc_logical_B1->width(), lsrc_logical_B1->height(),lsrc_logical_B1->buffer());
	
	klCopyFunctor<unsigned char> klcflf_B2_u8(lsrc, lsrc_logical_B2,1);
	klcflf_B2_u8();
	output =  outputPath;	output.append("LOGICAL_IN_B2.pgm");
	write_ppm_single_band(output.c_str(), lsrc_logical_B2->width(), lsrc_logical_B2->height(),lsrc_logical_B2->buffer());

	klCopyFunctor<unsigned char> klcflf_B3_u8(lsrc, lsrc_logical_B3,2);
	klcflf_B3_u8();
	output =  outputPath;	output.append("LOGICAL_IN_B3.pgm");
	write_ppm_single_band(output.c_str(), lsrc_logical_B3->width(), lsrc_logical_B3->height(),lsrc_logical_B3->buffer());

	klLogicalFunctor klflf_u8(lsrc_logical_B1, lsrc_logical_B2, logical_result,klLogicalFunctor::LogicalFunctorOperationType::AND);
	klflf_u8();
	output =  outputPath;output.append("LOGICAL_u8_C1R.pgm");
	write_ppm_single_band(output.c_str(), logical_result->width(), logical_result->height(),logical_result->buffer());


	klFillHolesFunctor<unsigned char> klfhf_NOT_GREEN_u8(lsrc_logical_B2,255,0);
	klfhf_NOT_GREEN_u8();
	output =  outputPath;output.append("HOLES_FILLED_B2_u8_C1R.pgm");
	write_ppm_single_band(output.c_str(), lsrc_logical_B2->width(), lsrc_logical_B2->height(),lsrc_logical_B2->buffer());


	unsigned char lbackgroundcolor=0;
	unsigned char lforegroundcolor=255;
	const klRasterBufferPointer lsrc_holefill=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
	klCopyFunctor<unsigned char> klcfhf_u8(lsrc, lsrc_holefill,1);//Do the first band only
	klcfhf_u8();
	klFillHolesFunctor<unsigned char> klfhf_u8(lsrc_holefill,lbackgroundcolor,lforegroundcolor);
	klfhf_u8();
	output =  outputPath;output.append("HOLES_FILLED_u8_C1R.pgm");
	write_ppm_single_band(output.c_str(), lsrc_holefill->width(), lsrc_holefill->height(),lsrc_holefill->buffer());

	//Test type convert functor.
	klRasterBufferPointer lfloatSrc=new klPackedHeapRasterBuffer<float> (lsrc->width(),lsrc->height(),lsrc->numBands());
	unsigned char linmin=0;
	unsigned char linmax=255;
	float loutmin=0.0f;
	float loutmax=1.0f;
	klTypeConvertFunctor<unsigned char,float> kltcfn_f_u8( lsrc, lfloatSrc,  true,linmin, linmax,loutmin, loutmax);
	kltcfn_f_u8();

	//{
		/////////////////////////////////////////////////////////TEST FLOATING POINT FN
		//unsigned int lkernelWidthf32=3;
		//unsigned int lkernelHeightf32=3;
		//float *  lkernelf32=new float [ lkernelWidth * lkernelHeight ];
		//lkernel[0]= 0.0113;lkernel[1]= 0.0838;lkernel[2]= 0.0113;
		//lkernel[3]= 0.0838;lkernel[4]= 0.0113;lkernel[5]= 0.08383;
		//lkernel[6]= 0.0113;lkernel[7]= 0.0838;lkernel[8]= 0.0113;

		//klFilterFunctor<float > klff_f32(lfloatSrc, lkernelf32,  lkernelWidthf32, lkernelHeightf32);
		//klff_f32();
		//delete lkernelf32;

		//BBCREVISIT PROBLEM WITH FLOAT MORPH Functors

		//const klRasterBufferPointer lsrc_erroded_f32=new klPackedHeapRasterBuffer<float> (lfloatSrc->width(),lfloatSrc->height(),lfloatSrc->numBands() );
		//klCopyFunctor<float> klcfef_f32(lfloatSrc, lsrc_erroded_f32);
		//klcfef_f32();//Do the copy
		//klErodeFunctor< float> klef_f32( lfloatSrc,  lsrc_erroded_f32,  lkernelEF,  lkernelWidthEF, lkernelHeightEF);
		//klef_f32();//Do the errode

		//const klRasterBufferPointer lsrc_dilatedf32=new klPackedHeapRasterBuffer<float> (lfloatSrc->width(),lfloatSrc->height(),lfloatSrc->numBands() );
		//klCopyFunctor<float > klcfdf_f32(lfloatSrc, lsrc_dilatedf32);
		//klcfdf_f32();//Do the copy
		//klDilateFunctor< float > kldf_f32( lfloatSrc,  lsrc_dilatedf32,  lkernelDF,  lkernelWidthDF, lkernelHeightDF);
		//kldf_f32();


	//	double angle=-45;
	//	//int interpolate=IPPI_INTER_CUBIC2P_BSPLINE;
	//	int interpolate=IPPI_INTER_NN;
	//	klRotateFunctor<float> klro_f32(lfloatSrc, angle, interpolate);
	//	klRasterBufferPointer src_rotatedf32 =klro_f32();

	//	double xFactor=2;
	//	double yFactor=2;
	//	interpolate=IPPI_INTER_CUBIC2P_BSPLINE;

	//	klResizeFunctor<float  > klrs_f32(lfloatSrc, xFactor, yFactor, interpolate);
	//	klRasterBufferPointer src_resampledf32 =klrs_f32();

	//	klRect iRoi(128,128,256,256);
	//	klRect oRoi(128,128,256,256);

	//	klCopyROIFunctor<float > kl_cf_f32( src_rotatedf32,  src_resampledf32,  iRoi, oRoi);
	//	kl_cf_f32();
	//}
	///////////////////////////////////END FLOAT TEST //////////////////////////////////////////

}

int LoadImageFiles(string* files)
{
	int numImages = 15;
	char** fileList = new char*[numImages];
fileList[0] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_10240_13312_1024_1024__SRC_u8_C3R.ppm";
fileList[1] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_15360_1024_1024__SRC_u8_C3R.ppm";
fileList[2] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_16384_1024_1024__SRC_u8_C3R.ppm";
fileList[3] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_17408_1024_1024__SRC_u8_C3R.ppm";
fileList[4] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_18432_1024_1024__SRC_u8_C3R.ppm";
fileList[5] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_19456_1024_1024__SRC_u8_C3R.ppm";
fileList[6] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_20480_1024_1024__SRC_u8_C3R.ppm";
fileList[7] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_21504_1024_1024__SRC_u8_C3R.ppm";
fileList[8] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_22528_1024_1024__SRC_u8_C3R.ppm";
fileList[9] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_23552_1024_1024__SRC_u8_C3R.ppm";
fileList[10] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_24576_1024_1024__SRC_u8_C3R.ppm";
fileList[11] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_25600_1024_1024__SRC_u8_C3R.ppm";
fileList[12] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_26624_1024_1024__SRC_u8_C3R.ppm";
fileList[13] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_27648_1024_1024__SRC_u8_C3R.ppm";
fileList[14] = "D:\\klDll\\TestDll\\MTImageTestData\\1553_11264_28672_1024_1024__SRC_u8_C3R.ppm";


for(int i=0;i<numImages;i++)
{
	*(files+i)= fileList[i];
}

	return numImages;

}


class klStringArgThread :public  klThread<klMutex>
{
public:
	klStringArgThread(void (*pf)(string,string ) )
    {
        fPtr=pf;
    }
    void run()
    {
        fPtr(arg,outpath);
    }
    void (*fPtr)(string, string);

	string arg;
	string outpath;
};

void klRunImageWorkFlowImage(const char* basefilename)
{
	unsigned int numFiles = 15;
	string* files = new string[15];
	int loaded = LoadImageFiles(files);

	HANDLE	 hProcess=GetCurrentProcess ();

	klThreadId thisThread=klThread<klMutex>::getCurrentThreadId();	

	klThreadWorkflow<int,klSmartPtr<klStringArgThread > > workflow;

	void (*pf)(string,string) =ImageWork;
	 
	for(int i=0;i<8;i++)
	{
		
		klSmartPtr<klStringArgThread > element=new klStringArgThread(pf);
		element->arg = *(files+i);
		
		std::stringstream out;out << i;
		string outpath = out.str();
		element->outpath = outpath;
		workflow.insertWorkElement(i, element);
	}
	workflow.doWork();
	flushall();
}


#define KL_TIFF
#include "kl_image_io.h"
#include "kl_tiff_image_io.h"
#include "kl_image_io_factory.h"

void Dithering(string fileName,string outputPath)
{	
	klImageFileSource* klifs= klImageFileSourceFactory::getFileSource(fileName.c_str());

	bool ansOK = klifs->queryImage();

	klRasterBufferPointer klrbp = klifs->operator()();

	klTIFFSourceFunctor kltfs(fileName.c_str());
	
	string outName = fileName;
	size_t pos = outName.find(".tif");
	outName.replace(pos,pos+4,"").append("_kltfs_input.ppm");

	klRasterBufferPointer lsrc  =kltfs();

	write_ppm(outName.c_str() , lsrc->width(), lsrc->height(),lsrc->buffer());

	unsigned lkernelWidthDF=3;

	unsigned lkernelHeightDF=3;
	
	unsigned char* lkernelDF = new unsigned char[ lkernelWidthDF * lkernelHeightDF ];
	lkernelDF[0]=1;lkernelDF[1]=1;lkernelDF[2]=1;
	lkernelDF[3]=1;lkernelDF[4]=0;lkernelDF[5]=1;
	lkernelDF[6]=1;lkernelDF[7]=1;lkernelDF[8]=1;

	const klRasterBufferPointer lsrc_dilated=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
	klDilateFunctor< unsigned char> kldf_u8( lsrc,  lsrc_dilated,  lkernelDF,  lkernelWidthDF, lkernelHeightDF);
	kldf_u8();
	delete lkernelDF;
	write_ppm("kltfs_DILATED.ppm", lsrc_dilated->width(), lsrc_dilated->height(),lsrc_dilated->buffer());
		
	const klRasterBufferPointer ldst_color_to_gray=new klPackedHeapRasterBuffer<unsigned char>(lsrc_dilated->width(), lsrc_dilated->height(), 1);
	klColorToGrayFunctor<unsigned char> klctg_u8(lsrc_dilated,ldst_color_to_gray);
	klctg_u8();
	write_ppm_single_band ("kltfsCOLOR_TO_GRAY.pgm", ldst_color_to_gray->width(), ldst_color_to_gray->height(),ldst_color_to_gray->buffer());
	
	char* fileNameC = new char[2048];
	
	  /*if (AllocConsole())
	  {
	      freopen("CONOUT$", "wt", stdout);
	      SetConsoleTitle("Debug Console");
	      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	  }*/

	HWND handle =  GetConsoleWindow();
	bool ans =SetStdHandle(STD_OUTPUT_HANDLE, handle);


	for(int Levels =2;Levels<16;Levels+=1)
	{
		for(int Noise =0;Noise<30;Noise+=5)
		{
			klTimer  dt;
			dt.tic();
			{
				//Floid-Steinberg error diffusion
				IppiDitherType dithertype = IppiDitherType::ippDitherFS;

				const klRasterBufferPointer lsrc_dither=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
				klDitheringFunctor< unsigned char> kldither_u8( lsrc_dilated,  lsrc_dither, Levels, dithertype,Noise);
				kldither_u8();
				sprintf(fileNameC,"3Band_%03dLevels_%03dNoise_Floid-SteinbergErrorDiffusion.tif",Levels,Noise);
				klTIFFSinkFunctor kltsf(fileNameC,lsrc_dither);
				kltsf();

				const klRasterBufferPointer ldst_color_to_gray_dithered=new klPackedHeapRasterBuffer<unsigned char>(lsrc_dither->width(), lsrc_dither->height(), 1);
				klDitheringFunctor< unsigned char> kldither_u8_1Band( ldst_color_to_gray,  ldst_color_to_gray_dithered, Levels, dithertype,Noise);
				kldither_u8_1Band();
				sprintf(fileNameC,"1Band_%03dLevels_%03dNoise_Floid-SteinbergErrorDiffusion.tif",Levels,Noise);
				klTIFFSinkFunctor kltsfsb(fileNameC,ldst_color_to_gray_dithered);
				kltsfsb();
			}

			{ 
				//Stucki error diffusion dithering 
				IppiDitherType dithertype =  IppiDitherType::ippDitherStucki;
				const klRasterBufferPointer lsrc_dither=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
				klDitheringFunctor< unsigned char> kldither_u8( lsrc_dilated,  lsrc_dither, Levels, dithertype,Noise);
				kldither_u8();
				sprintf(fileNameC,"3Band_%03dLevels_%03dNoise_StuckiErrorDiffusion.tif",Levels,Noise);
				klTIFFSinkFunctor kltsf(fileNameC,lsrc_dither);
				kltsf();
				const klRasterBufferPointer ldst_color_to_gray_dithered=new klPackedHeapRasterBuffer<unsigned char>(lsrc_dither->width(), lsrc_dither->height(), 1);
				klDitheringFunctor< unsigned char> kldither_u8_1Band( ldst_color_to_gray,  ldst_color_to_gray_dithered, Levels, dithertype,Noise);
				kldither_u8_1Band();
				sprintf(fileNameC,"1Band_%03dLevels_%03dNoise_StuckiErrorDiffusion.tif",Levels,Noise);
				klTIFFSinkFunctor kltsfsb(fileNameC,ldst_color_to_gray_dithered);
				kltsfsb();
			}


			{
				//Jarvice-Judice-Ninke error diffusion dithering	
				IppiDitherType dithertype =  IppiDitherType::ippDitherJJN ;  	
				const klRasterBufferPointer lsrc_dither=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
				klDitheringFunctor< unsigned char> kldither_u8( lsrc_dilated,  lsrc_dither, Levels, dithertype,Noise);
				kldither_u8();
				sprintf(fileNameC,"3Band_%03dLevels_%03dNoise_Jarvice-Judice-NinkeiErrorDiffusion.tif",Levels,Noise);
				klTIFFSinkFunctor kltsf(fileNameC,lsrc_dither);
				kltsf();
				
				const klRasterBufferPointer ldst_color_to_gray_dithered=new klPackedHeapRasterBuffer<unsigned char>(lsrc_dither->width(), lsrc_dither->height(), 1);
				klDitheringFunctor< unsigned char> kldither_u8_1Band( ldst_color_to_gray,  ldst_color_to_gray_dithered, Levels, dithertype,Noise);
				kldither_u8_1Band();
				sprintf(fileNameC,"1Band_%03dLevels_%03dNoise_Jarvice-Judice-NinkeErrorDiffusion.tif",Levels,Noise);
				klTIFFSinkFunctor kltsfsb(fileNameC,ldst_color_to_gray_dithered);
				kltsfsb();
			}

			{
				//Bayer’s threshold dithering  BayerThreshold
				IppiDitherType dithertype =  IppiDitherType::ippDitherBayer;  	
				const klRasterBufferPointer lsrc_dither=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
				klDitheringFunctor< unsigned char> kldither_u8( lsrc_dilated,  lsrc_dither, Levels, dithertype,Noise);
				kldither_u8();
				sprintf(fileNameC,"3Band_%03dLevels_%03dNoise_BayerThreshold.tif",Levels,Noise);
				klTIFFSinkFunctor kltsf(fileNameC,lsrc_dither);
				kltsf();
				
				const klRasterBufferPointer ldst_color_to_gray_dithered=new klPackedHeapRasterBuffer<unsigned char>(lsrc_dither->width(), lsrc_dither->height(), 1);
				klDitheringFunctor< unsigned char> kldither_u8_1Band( ldst_color_to_gray,  ldst_color_to_gray_dithered, Levels, dithertype,Noise);
				kldither_u8_1Band();
				sprintf(fileNameC,"1Band_%03dLevels_%03dNoise_BayerThreshold.tif",Levels,Noise);
				klTIFFSinkFunctor kltsfsb(fileNameC,ldst_color_to_gray_dithered);
				kltsfsb();
			}
			
			cout<<"TicToc = " <<dt.toc()<<endl;
			printf("TicToc = %f\n",dt.toc());
		}
		
	}
	delete fileNameC;

	
}

void TissueAnalysis(string fileName,string outputPath)
{
	const char* infilename=fileName.c_str();
	unsigned int inwidth=0;
	unsigned int inheight=0;
	unsigned int inbands=0;

	string output =  outputPath;
	
	//This was the old way - now we have
	klImageFileSource* ifs = klImageFileSourceFactory::getFileSource(fileName.c_str() );
	
	ifs->queryImage();
	inwidth = ifs->getWidth();
	inheight=ifs->getHeight();
	inbands=ifs->getBands();
	klRasterBufferPointer lsrc= ifs->render();

	klTIFFSinkFunctor kltsf("test.tif",lsrc);

	kltsf();
/*	(	query_ppm (infilename, inwidth, inheight,inbands))
	{
		lsrc=new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, inbands);
		read_ppm (infilename, inwidth,inheight, inbands,lsrc->buffer());
	}
	else
	{
		throw "Bad input file in void testKLImageFunctors()";
	}*/	
	
	klPCAFunctor pca(lsrc);
	klRasterBufferPointer dst =  lsrc;
	const klRasterBufferPointer ldst_cptbipca=new klPackedHeapRasterBuffer<unsigned char> (dst->width(),dst->height(),3 );
	klCopySplitBandFunctor<unsigned char> kl_cfpitbipca_u8( dst,  ldst_cptbipca);
	kl_cfpitbipca_u8();
	output =  outputPath;
	output.append("PCA_PIXTOBAND.pgm");
	write_ppm_single_band(output.c_str(), ldst_cptbipca->width(), ldst_cptbipca->height()*3,ldst_cptbipca->buffer());

	{

	unsigned lkernelWidthDF=3;
	unsigned lkernelHeightDF=3;
	//Mask values. Only pixels that correspond to nonzero 
	//mask values are taken into account during operation.
	unsigned char* lkernelDF = new unsigned char[ lkernelWidthDF * lkernelHeightDF ];
	lkernelDF[0]=1;lkernelDF[1]=1;lkernelDF[2]=1;
	lkernelDF[3]=1;lkernelDF[4]=0;lkernelDF[5]=1;
	lkernelDF[6]=1;lkernelDF[7]=1;lkernelDF[8]=1;
	const klRasterBufferPointer lsrc_dilated=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
	klCopyFunctor<unsigned char> klcfdf_u8(dst, lsrc_dilated);
	klcfdf_u8();//Do the copy
	klDilateFunctor< unsigned char> kldf_u8( lsrc,  lsrc_dilated,  lkernelDF,  lkernelWidthDF, lkernelHeightDF);
	kldf_u8();
	delete lkernelDF;
	output =  outputPath;
	output.append("DILATED_u8_C3R.ppm");
	write_ppm(output.c_str(), lsrc_dilated->width(), lsrc_dilated->height(),lsrc_dilated->buffer());

	klApplyColorUnmixingBasis ssb(lsrc_dilated);
	klRasterBufferPointer dstssb =  ssb();
	output =  outputPath; output.append("SSB_PIXTOBAND.pgm");
	write_ppm_single_band(output.c_str(), ldst_cptbipca->width(), ldst_cptbipca->height()*3,ldst_cptbipca->buffer());
	output =  outputPath; output.append("SSB.ppm");
	write_ppm(output.c_str(), dstssb->width(), dstssb->height(),dstssb->buffer());
	{
		const klRasterBufferPointer ldst_cptbipca_ssb=new klPackedHeapRasterBuffer<unsigned char> (dst->width(),dst->height(),3 );
	
		klCopySplitBandFunctor<unsigned char> kl_cfpitbipca_ssb_u8( dstssb,  ldst_cptbipca_ssb);
		kl_cfpitbipca_ssb_u8();
		output =  outputPath; output.append("SSB_2.pgm");
		write_ppm_single_band(output.c_str(), ldst_cptbipca_ssb->width(), ldst_cptbipca_ssb->height()*3,ldst_cptbipca_ssb->buffer());
	}

	//klHandERatio ssp(dstssb);
	klHandERatio ssp(lsrc_dilated);
	klRasterBufferPointer dstssp =  ssp();
	output =  outputPath;	output.append("StainPurity_u8_C3R.pgm");
	write_ppm_single_band(output.c_str(), dstssp->width(), dstssp->height(),dstssp->buffer());

	double lthresholdHi=196;
	double lthresholdLow=56;
	bool luseLowThreshold=true;
	bool luseHighThreshold=true;
	double lthresholdHiVal=255;
	double lthresholdLowVal=0;
	bool luseThresholdVals=true;
	const klRasterBufferPointer lsrc_thresh=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
	//Skip Stain Purity 
	klCopyFunctor<unsigned char> klcfth_u8(dstssp, lsrc_thresh);
	klcfth_u8();
	klThresholdFunctor<unsigned char> kltf_u8( lsrc_thresh, lthresholdHi, lthresholdLow,luseLowThreshold,luseHighThreshold,lthresholdHiVal,lthresholdLowVal, luseThresholdVals);
	kltf_u8();
	output =  outputPath;	output.append("THRESHOLDED_u8_C3R.pgm");
	write_ppm_single_band (output.c_str(), lsrc_thresh->width(), lsrc_thresh->height(),lsrc_thresh->buffer());

	const klRasterBufferPointer ldst_cptbissb=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),3 );
	klCopySplitBandFunctor<unsigned char> kl_cfpitbissb_u8( lsrc_dilated,  ldst_cptbissb);
	kl_cfpitbissb_u8();
	write_ppm_single_band ("SplitBandFuntor_SSB_PIXTOBAND.pgm", ldst_cptbissb->width(), ldst_cptbissb->height()*3,ldst_cptbissb->buffer());

	//unsigned lkernelWidthEF=3;
	//unsigned lkernelHeightEF=3;
	//unsigned char* lkernelEF = new unsigned char[ lkernelWidthEF * lkernelHeightEF ];
	//lkernelEF[0]=1;lkernelEF[1]=1;lkernelEF[2]=1;
	//lkernelEF[3]=1;lkernelEF[4]=0;lkernelEF[5]=1;
	//lkernelEF[6]=1;lkernelEF[7]=1;lkernelEF[8]=1;
	//const klRasterBufferPointer lsrc_erroded=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
	//klCopyFunctor<unsigned char> klcfef_u8(dstssb, lsrc_erroded);
	//klcfef_u8();//Do the copy
	

	//klErodeFunctor< unsigned char> klef_u8( dstssb,  lsrc_erroded,  lkernelEF,  lkernelWidthEF, lkernelHeightEF);
	//klef_u8();//Do the errode
	//write_ppm("ERRODED_u8_C3R.ppm", lsrc_erroded->width(), lsrc_erroded->height(),lsrc_erroded->buffer());
	//delete lkernelEF;

	const klRasterBufferPointer lsrc_logical_B1=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );

	klCopyFunctor<unsigned char> klcflf_B1_u8(lsrc_thresh, lsrc_logical_B1);
	klcflf_B1_u8();
	output =  outputPath;	output.append("LOGICAL_IN_B1.pgm");
	write_ppm_single_band(output.c_str(), lsrc_logical_B1->width(), lsrc_logical_B1->height(),lsrc_logical_B1->buffer());

	unsigned lkernelWidthO=3;
	unsigned lkernelHeightO=3;
	unsigned char* lkernelO = new unsigned char[ lkernelWidthO * lkernelHeightO ];
	lkernelO[0]=0;lkernelO[1]=1;lkernelO[2]=0;
	lkernelO[3]=1;lkernelO[4]=1;lkernelO[5]=1;
	lkernelO[6]=0;lkernelO[7]=1;lkernelO[8]=0;
	const klRasterBufferPointer lsrc_opened=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),3 );
	klOpenFunctor< unsigned char> klof_u8( lsrc,  lsrc_opened,  lkernelO,  lkernelWidthO, lkernelHeightO);
	klof_u8();
	output =  outputPath;	output.append("Opened_u8_C3R.ppm");
	write_ppm(output.c_str(), lsrc_opened->width(), lsrc_opened->height(),lsrc_opened->buffer());
	delete lkernelO;
	
	klCompositeMask klcm( lsrc, lsrc_thresh ,0.5,0,120,0);
	klRasterBufferPointer blended = klcm();
	output =  outputPath;	output.append("WhiteSpaceBlended_u8_C3R.ppm");
	write_ppm(output.c_str(), blended->width(), blended->height(),blended->buffer());

	}

	const klRasterBufferPointer ldst=new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, inbands);
	int lband=-1;
	klCopyFunctor<unsigned char> klcf_u8(lsrc, ldst);
	klcf_u8();
	output =  outputPath;	output.append("SRC_u8_C3R.ppm");
	write_ppm (output.c_str(), ldst->width(), ldst->height(),ldst->buffer());

	unsigned int lkernelWidth=3;
	unsigned int lkernelHeight=3;
	unsigned char*  lkernel=new unsigned char[ lkernelWidth * lkernelHeight ];
	lkernel[0]= 0.0113*256;lkernel[1]= 0.0838*256;lkernel[2]= 0.0113*256;
	lkernel[3]= 0.0838*256;lkernel[4]= 0.0113*256;lkernel[5]= 0.08383*256;
	lkernel[6]= 0.0113*256;lkernel[7]= 0.0838*256;lkernel[8]= 0.0113*256;

	klFilterFunctor<unsigned char> klff_u8(ldst, lkernel,  lkernelWidth, lkernelHeight);
	klff_u8();
	delete lkernel;
	output =  outputPath;	output.append("FILTERED_u8_C3R.ppm");
	write_ppm (output.c_str(), ldst->width(), ldst->height(),ldst->buffer());

	const klRasterBufferPointer ldst_color_to_gray=new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, 1);
	klColorToGrayFunctor<unsigned char> klctg_u8(lsrc,ldst_color_to_gray);
	klctg_u8();
	output =  outputPath;	output.append("COLOR_TO_GRAY.pgm");
	write_ppm_single_band (output.c_str(), ldst_color_to_gray->width(), ldst_color_to_gray->height(),ldst_color_to_gray->buffer());

	//Maps [0,thresholdLow] U [thresholdHigh,MAXVAL(TYPE) ] -----> LowVal , HighVal
	double lthresholdHi=240;
	double lthresholdLow=32;
	bool luseLowThreshold=true;
	bool luseHighThreshold=true;
	double lthresholdHiVal=200;
	double lthresholdLowVal=40;
	bool luseThresholdVals=true;
	const klRasterBufferPointer lsrc_thresh=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
	klCopyFunctor<unsigned char> klcfth_u8(lsrc, lsrc_thresh);
	klcfth_u8();
	klThresholdFunctor<unsigned char> kltf_u8( lsrc_thresh, lthresholdHi, lthresholdLow,luseLowThreshold,luseHighThreshold,lthresholdHiVal,lthresholdLowVal, luseThresholdVals);
	kltf_u8();
	output =  outputPath;	output.append("THRESHOLDED_u8_C3R.ppm");
	write_ppm (output.c_str(), lsrc_thresh->width(), lsrc_thresh->height(),lsrc_thresh->buffer());

	//Our morphological structure elelment
	unsigned lkernelWidthEF=3;
	unsigned lkernelHeightEF=3;
	unsigned char* lkernelEF = new unsigned char[ lkernelWidthEF * lkernelHeightEF ];
	lkernelEF[0]=1;lkernelEF[1]=1;lkernelEF[2]=1;
	lkernelEF[3]=1;lkernelEF[4]=0;lkernelEF[5]=1;
	lkernelEF[6]=1;lkernelEF[7]=1;lkernelEF[8]=1;
	const klRasterBufferPointer lsrc_erroded=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),lsrc->numBands() );
	klCopyFunctor<unsigned char> klcfef_u8(lsrc, lsrc_erroded);
	klcfef_u8();//Do the copy
	delete lkernelEF;

	klErodeFunctor< unsigned char> klef_u8( lsrc,  lsrc_erroded,  lkernelEF,  lkernelWidthEF, lkernelHeightEF);
	klef_u8();//Do the errode
	output =  outputPath;	output.append("ERRODED_u8_C3R.ppm");
	write_ppm(output.c_str(), lsrc_erroded->width(), lsrc_erroded->height(),lsrc_erroded->buffer());

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
	output =  outputPath;output.append("DILATED_u8_C3R.ppm");
	write_ppm(output.c_str(), lsrc_dilated->width(), lsrc_dilated->height(),lsrc_dilated->buffer());

	
	/////////////////////Test Copy Pixel To Band Interleaved
	const klRasterBufferPointer ldst_cptbi=new klPackedHeapRasterBuffer<unsigned char> (lsrc_dilated->width(),lsrc_dilated->height(),lsrc_dilated->numBands() );
	klCopySplitBandFunctor<unsigned char> kl_cfpitbi_u8( lsrc_dilated,  ldst_cptbi);
	kl_cfpitbi_u8();
	output =  outputPath;	output.append("COPYFUNCTION_PIXTOBAND_u8_P3R.pgm");
	write_ppm_single_band (output.c_str(), ldst_cptbi->width(), ldst_cptbi->height()*3,ldst_cptbi->buffer());

	//Test logical functor - and the first and second bands
	const klRasterBufferPointer lsrc_logical_B1=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
	const klRasterBufferPointer lsrc_logical_B2=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
	const klRasterBufferPointer lsrc_logical_B3=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );

	const klRasterBufferPointer logical_result=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
		
	klCopyFunctor<unsigned char> klcflf_B1_u8(lsrc, lsrc_logical_B1,0);
	klcflf_B1_u8();
	output =  outputPath;	output.append("LOGICAL_IN_B1.pgm");
	write_ppm_single_band(output.c_str(), lsrc_logical_B1->width(), lsrc_logical_B1->height(),lsrc_logical_B1->buffer());
	
	klCopyFunctor<unsigned char> klcflf_B2_u8(lsrc, lsrc_logical_B2,1);
	klcflf_B2_u8();
	output =  outputPath;	output.append("LOGICAL_IN_B2.pgm");
	write_ppm_single_band(output.c_str(), lsrc_logical_B2->width(), lsrc_logical_B2->height(),lsrc_logical_B2->buffer());

	klCopyFunctor<unsigned char> klcflf_B3_u8(lsrc, lsrc_logical_B3,2);
	klcflf_B3_u8();
	output =  outputPath;	output.append("LOGICAL_IN_B3.pgm");
	write_ppm_single_band(output.c_str(), lsrc_logical_B3->width(), lsrc_logical_B3->height(),lsrc_logical_B3->buffer());

	klLogicalFunctor klflf_u8(lsrc_logical_B1, lsrc_logical_B2, logical_result,klLogicalFunctor::LogicalFunctorOperationType::AND);
	klflf_u8();
	output =  outputPath;output.append("LOGICAL_u8_C1R.pgm");
	write_ppm_single_band(output.c_str(), logical_result->width(), logical_result->height(),logical_result->buffer());


	klFillHolesFunctor<unsigned char> klfhf_NOT_GREEN_u8(lsrc_logical_B2,255,0);
	klfhf_NOT_GREEN_u8();
	output =  outputPath;output.append("HOLES_FILLED_B2_u8_C1R.pgm");
	write_ppm_single_band(output.c_str(), lsrc_logical_B2->width(), lsrc_logical_B2->height(),lsrc_logical_B2->buffer());

	unsigned char lbackgroundcolor=0;
	unsigned char lforegroundcolor=255;
	const klRasterBufferPointer lsrc_holefill=new klPackedHeapRasterBuffer<unsigned char> (lsrc->width(),lsrc->height(),1 );
	klCopyFunctor<unsigned char> klcfhf_u8(lsrc, lsrc_holefill,1);//Do the first band only
	klcfhf_u8();
	klFillHolesFunctor<unsigned char> klfhf_u8(lsrc_holefill,lbackgroundcolor,lforegroundcolor);
	klfhf_u8();
	output =  outputPath;output.append("HOLES_FILLED_u8_C1R.pgm");
	write_ppm_single_band(output.c_str(), lsrc_holefill->width(), lsrc_holefill->height(),lsrc_holefill->buffer());
}
