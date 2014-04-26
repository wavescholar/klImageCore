/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiProcess.cpp: implementation of the CippiProcess
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////
// CippiProcess class implementation
///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction

CippiProcess::CippiProcess() : CProcess()
{
   if (LIB_I  < LIB_NUM) InitIP();
   if (LIB_J  < LIB_NUM) InitJP();
   if (LIB_CC  < LIB_NUM) InitCC();
   if (LIB_CV < LIB_NUM) InitCV();
}

/////////////////////////////////////////////////////////////////////////////
// CProcess virtual function overrides

//---------------------------------------------------------------------------
// GetIdFilter returns FilterDialog ID for IPP library with specified
// index
//---------------------------------------------------------------------------
UINT CippiProcess::GetIdFilter(int idx)
{
   switch (idx) {
   case LIB_I : return IDD_FILTER_IP;
   case LIB_J : return IDD_FILTER_JP;
   case LIB_CC: return NULL;
   case LIB_CV: return IDD_FILTER_CV;
   }
   return CProcess::GetIdFilter(idx);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

//---------------------------------------------------------------------------
// ippIP initialization
//---------------------------------------------------------------------------

void CippiProcess::InitIP()
{
   CBook* pBook = NULL;
   CChapter* pChapter = NULL;

   pBook = Add("Data Exchange");
   pChapter = pBook->Add("Convert");
      pChapter->AddBase("Convert");
   pChapter = pBook->Add("Scale");
      pChapter->AddBase("Scale");
   pChapter = pBook->Add("Set");
      pChapter->AddBase("Set");
   pChapter = pBook->Add("Copy");
      pChapter->AddBase("Copy");
      pChapter->AddBase("CopyManaged");
   pChapter = pBook->Add("Duplicate Channel");
      pChapter->AddBase("Dup");
   pChapter = pBook->Add("Copy Border");
      pChapter->AddBase("CopyReplicateBorder");
      pChapter->AddBase("CopyConstBorder");
      pChapter->AddBase("CopyWrapBorder");
   pChapter = pBook->Add("Transpose");
      pChapter->AddBase("Transpose");
   pChapter = pBook->Add("Swap Channels");
      pChapter->AddBase("SwapChannels");
   pChapter = pBook->Add("PolarToCart");
      pChapter->AddBase("PolarToCart");
   pChapter = pBook->Add("Zigzag");
      pChapter->AddBase("ZigzagFwd8x8");
      pChapter->AddBase("ZigzagInv8x8");

   pBook = Add("Data Initialization");
   pChapter = pBook->Add("Add Random");
      pChapter->AddBase("AddRandUniform_Direct");
      pChapter->AddBase("AddRandGauss_Direct");
   pChapter = pBook->Add("Jaehne");
      pChapter->AddBase("ImageJaehne");
   pChapter = pBook->Add("Ramp");
      pChapter->AddBase("ImageRamp");

   pBook = Add("Arithmetic");
   pChapter = pBook->Add("Add");
      pChapter->AddBase("Add");
      pChapter->AddBase("AddC");
      pChapter->AddBase("AddSquare");
      pChapter->AddBase("AddProduct");
      pChapter->AddBase("AddWeighted");
   pChapter = pBook->Add("Multiply");
      pChapter->AddBase("Mul");
      pChapter->AddBase("MulC");
      pChapter->AddBase("MulScale");
      pChapter->AddBase("MulCScale");
   pChapter = pBook->Add("Subtract");
      pChapter->AddBase("Sub");
      pChapter->AddBase("SubC");
   pChapter = pBook->Add("Divide");
      pChapter->AddBase("Div");
      pChapter->AddBase("DivC");
      pChapter->AddBase("Div_Round");
   pChapter = pBook->Add("Absolute");
      pChapter->AddBase("Abs");
   pChapter = pBook->Add("Square");
      pChapter->AddBase("Sqr");
   pChapter = pBook->Add("Square Root");
      pChapter->AddBase("Sqrt");
   pChapter = pBook->Add("Logarithm");
      pChapter->AddBase("Ln");
   pChapter = pBook->Add("Exponent");
      pChapter->AddBase("Exp");
   pChapter = pBook->Add("Complement");
      pChapter->AddBase("Complement");
   pChapter = pBook->Add("Dot Product");
      pChapter->AddBase("DotProd");

   pBook = Add("Logical && Alpha Comp");
   pChapter = pBook->Add("AND");
      pChapter->AddBase("And");
      pChapter->AddBase("AndC");
   pChapter = pBook->Add("NOT");
      pChapter->AddBase("Not");
   pChapter = pBook->Add("OR");
      pChapter->AddBase("Or");
      pChapter->AddBase("OrC");
   pChapter = pBook->Add("XOR");
      pChapter->AddBase("Xor");
      pChapter->AddBase("XorC");
   pChapter = pBook->Add("SHIFT");
      pChapter->AddBase("LShiftC");
      pChapter->AddBase("RShiftC");
   pChapter = pBook->Add("Alpha Composition");
      pChapter->AddBase("AlphaComp");
      pChapter->AddBase("AlphaCompC");
   pChapter = pBook->Add("Color Keying");
      pChapter->AddBase("CompColorKey");
      pChapter->AddBase("AlphaCompColorKey");

   pBook = Add("Intensity Transformation");
   pChapter = pBook->Add("LUT");
      pChapter->AddBase("LUT");
      pChapter->AddBase("LUT_Linear");
      pChapter->AddBase("LUT_Cubic");
   pChapter = pBook->Add("LUT Palette");
      pChapter->AddBase("LUTPalette");
      pChapter->AddBase("LUTPaletteSwap");

   pBook = Add("Threshold and Compare");
   pChapter = pBook->Add("Threshold");
      pChapter->AddBase("Threshold");
      pChapter->AddBase("Threshold_GT");
      pChapter->AddBase("Threshold_LT");
   pChapter = pBook->Add("Threshold with Value");
      pChapter->AddBase("Threshold_Val");
      pChapter->AddBase("Threshold_GTVal");
      pChapter->AddBase("Threshold_LTVal");
      pChapter->AddBase("Threshold_LTValGTVal");
   pChapter = pBook->Add("Compute Threshold");
      pChapter->AddBase("ComputeThreshold_Otsu");
   pChapter = pBook->Add("Compare");
      pChapter->AddBase("Compare");
      pChapter->AddBase("CompareC");
      pChapter->AddBase("CompareEqualEps");
      pChapter->AddBase("CompareEqualEpsC");

   pBook = Add("Morphological");
   pChapter = pBook->Add("Dilate");
      pChapter->AddBase("Dilate");
      pChapter->AddBase("Dilate3x3");
   pChapter = pBook->Add("Erode");
      pChapter->AddBase("Erode");
      pChapter->AddBase("Erode3x3");
   pChapter = pBook->Add("DilateStrip");
      pChapter->AddBase("DilateStrip");
      pChapter->AddBase("DilateStrip_Rect");
      pChapter->AddBase("DilateStrip_Cross");
   pChapter = pBook->Add("ErodeStrip");
      pChapter->AddBase("ErodeStrip");
      pChapter->AddBase("ErodeStrip_Rect");
      pChapter->AddBase("ErodeStrip_Cross");

   pBook = Add("General Filtering");
   pChapter = pBook->Add("Box Filter");
      pChapter->AddBase("FilterBox");
   pChapter = pBook->Add("Sum Window");
      pChapter->AddBase("SumWindowColumn");
      pChapter->AddBase("SumWindowRow");
   pChapter = pBook->Add("Min/Max Filter");
      pChapter->AddBase("FilterMax");
      pChapter->AddBase("FilterMin");
   pChapter = pBook->Add("Bilateral Filter");
      pChapter->AddBase("FilterBilateral");
   pChapter = pBook->Add("Median Filters");
      pChapter->AddBase("FilterMedian");
      pChapter->AddBase("FilterMedianHoriz");
      pChapter->AddBase("FilterMedianVert");
      pChapter->AddBase("FilterMedianCross");
      pChapter->AddBase("FilterMedianColor");
   pChapter = pBook->Add("General Linear Filters");
      pChapter->AddBase("Filter");
      pChapter->AddBase("Filter32f");
      pChapter->AddBase("Filter_Round16s");
      pChapter->AddBase("Filter_Round32s");
      pChapter->AddBase("Filter_Round32f");
   pChapter = pBook->Add("Separable Filters");
      pChapter->AddBase("FilterColumn");
      pChapter->AddBase("FilterColumn32f");
      pChapter->AddBase("FilterRow");
      pChapter->AddBase("FilterRow32f");
      pChapter->AddBase("DecimateFilterRow");
      pChapter->AddBase("DecimateFilterColumn");
   pChapter = pBook->Add("Wiener Filters");
      pChapter->AddBase("FilterWiener");
   pChapter = pBook->Add("Convolution");
      pChapter->AddBase("Convolve");
      pChapter->AddBase("ConvFull");
      pChapter->AddBase("ConvValid");
   pChapter = pBook->Add("Deconvolution");
      pChapter->AddBase("DeconvFFT");
      pChapter->AddBase("DeconvLR");
   pChapter = pBook->Add("Cross Correlation");
      pChapter->AddBase("CrossCorrFull_Norm");
      pChapter->AddBase("CrossCorrFull_NormLevel");
      pChapter->AddBase("CrossCorrValid");
      pChapter->AddBase("CrossCorrValid_Norm");
      pChapter->AddBase("CrossCorrValid_NormLevel");
      pChapter->AddBase("CrossCorrSame_Norm");
      pChapter->AddBase("CrossCorrSame_NormLevel");
   pChapter = pBook->Add("Square Distance");
      pChapter->AddBase("SqrDistanceFull_Norm");
      pChapter->AddBase("SqrDistanceValid_Norm");
      pChapter->AddBase("SqrDistanceSame_Norm");
   pChapter = pBook->Add("Windowing");
      pChapter->AddBase("WinBartlett");
      pChapter->AddBase("WinBartlettSep");
      pChapter->AddBase("WinHamming");
      pChapter->AddBase("WinHammingSep");

   pBook = Add("Fixed Filters");
   pChapter = pBook->Add("Prewitt Filters");
      pChapter->AddBase("FilterPrewittHoriz");
      pChapter->AddBase("FilterPrewittVert");
   pChapter = pBook->Add("Scharr Filters");
      pChapter->AddBase("FilterScharrHoriz");
      pChapter->AddBase("FilterScharrVert");
   pChapter = pBook->Add("Sobel Filters");
      pChapter->AddBase("FilterSobelCross");
      pChapter->AddBase("FilterSobelHoriz");
      pChapter->AddBase("FilterSobelVert");
      pChapter->AddBase("FilterSobelHorizMask");
      pChapter->AddBase("FilterSobelVertMask");
      pChapter->AddBase("FilterSobelHorizSecond");
      pChapter->AddBase("FilterSobelVertSecond");
   pChapter = pBook->Add("Roberts Filters");
      pChapter->AddBase("FilterRobertsUp");
      pChapter->AddBase("FilterRobertsDown");
   pChapter = pBook->Add("Sharpen Filters");
      pChapter->AddBase("FilterSharpen");
   pChapter = pBook->Add("Laplace && Gauss Filters");
      pChapter->AddBase("FilterLaplace");
      pChapter->AddBase("FilterGauss");
   pChapter = pBook->Add("Hipass && Lowpass Filters");
      pChapter->AddBase("FilterHipass");
      pChapter->AddBase("FilterLowpass");


   pBook = Add("Linear Transforms");
   pChapter = pBook->Add("Fast Fourier Transforms");
      pChapter->AddBase("FFTFwd_CToC");
      pChapter->AddBase("FFTInv_CToC");
      pChapter->AddBase("FFTFwd_RToPack");
      pChapter->AddBase("FFTInv_PackToR");
      pChapter->AddBase("MulPack");
      pChapter->AddBase("MulPackConj");
   pChapter = pBook->Add("Magnitude && Phase");
      pChapter->AddBase("PackToCplxExtend");
      pChapter->AddBase("CplxExtendToPack");
      pChapter->AddBase("Phase");
      pChapter->AddBase("PhasePack");
      pChapter->AddBase("Magnitude");
      pChapter->AddBase("MagnitudePack");
   pChapter = pBook->Add("Discrete Fourier Transforms");
      pChapter->AddBase("DFTFwd_CToC");
      pChapter->AddBase("DFTInv_CToC");
      pChapter->AddBase("DFTFwd_RToPack");
      pChapter->AddBase("DFTInv_PackToR");
   pChapter = pBook->Add("Discrete Cosine Transforms");
      pChapter->AddBase("DCTFwd");
      pChapter->AddBase("DCTInv");
      pChapter->AddBase("DCT8x8Fwd");
      pChapter->AddBase("DCT8x8Inv");
      pChapter->AddBase("DCT8x8Inv_A10");
      pChapter->AddBase("DCT8x8Inv_2x2");
      pChapter->AddBase("DCT8x8Inv_4x4");
      pChapter->AddBase("DCT8x8To2x2Inv");
      pChapter->AddBase("DCT8x8To4x4Inv");
      pChapter->AddBase("DCT8x8FwdLS");
      pChapter->AddBase("DCT8x8InvLSClip");

   pBook = Add("Image Statistics");
   pChapter = pBook->Add("Sum, Mean, Min, Max");
      pChapter->AddBase("Min");
      pChapter->AddBase("Max");
      pChapter->AddBase("MinMax");
      pChapter->AddBase("Sum");
      pChapter->AddBase("Mean");
   pChapter = pBook->Add("Min && Max Index");
      pChapter->AddBase("MinIndx");
      pChapter->AddBase("MaxIndx");
      pChapter->AddBase("MinMaxIndx");
   pChapter = pBook->Add("MinEvery && MaxEvery");
      pChapter->AddBase("MinEvery");
      pChapter->AddBase("MaxEvery");
   pChapter = pBook->Add("Histogram");
      pChapter->AddBase("HistogramRange");
      pChapter->AddBase("HistogramEven");
      pChapter->AddBase("CountInRange");
   pChapter = pBook->Add("Moments");
      pChapter->AddBase("Moments64f");
      pChapter->AddBase("Moments64s");
   pChapter = pBook->Add("Norms");
      pChapter->AddBase("Norm_Inf");
      pChapter->AddBase("Norm_L1");
      pChapter->AddBase("Norm_L2");
   pChapter = pBook->Add("Image Compare");
      pChapter->AddBase("NormDiff_Inf");
      pChapter->AddBase("NormDiff_L1");
      pChapter->AddBase("NormDiff_L2");
      pChapter->AddBase("NormRel_Inf");
      pChapter->AddBase("NormRel_L1");
      pChapter->AddBase("NormRel_L2");
      pChapter->AddBase("QualityIndex");

   pBook = Add("Geometric Transforms");
   pChapter = pBook->Add("Resize");
      pChapter->AddBase("ResizeFilter");
      pChapter->AddBase("ResizeYUV422");
      pChapter->AddBase("ResizeSqrPixel");
      pChapter->AddBase("DecimateBlur");
      pChapter->AddBase("SuperSampling");
   pChapter = pBook->Add("Mirror");
      pChapter->AddBase("Mirror");
   pChapter = pBook->Add("Remap");
      pChapter->AddBase("Remap");
   pChapter = pBook->Add("Rotate and Shear");
      pChapter->AddBase("Rotate");
      pChapter->AddBase("RotateCenter");
      pChapter->AddBase("Shear");
   pChapter = pBook->Add("Warp Affine");
      pChapter->AddBase("WarpAffine");
      pChapter->AddBase("WarpAffineBack");
      pChapter->AddBase("WarpAffineQuad");
   pChapter = pBook->Add("Warp Perspective");
      pChapter->AddBase("WarpPerspective");
      pChapter->AddBase("WarpPerspectiveBack");
      pChapter->AddBase("WarpPerspectiveQuad");
   pChapter = pBook->Add("Warp Bilinear");
      pChapter->AddBase("WarpBilinear");
      pChapter->AddBase("WarpBilinearBack");
      pChapter->AddBase("WarpBilinearQuad");

   pBook = Add("Wavelete Transforms");
   pChapter = pBook->Add("Wavelete Transforms");
      pChapter->AddBase("WTFwd");
      pChapter->AddBase("WTInv");
//      pChapter->AddBase("WTHaarFwd");
//      pChapter->AddBase("WTHaarInv");
//   pChapter = pBook->Add("Resampling");
//      pChapter->AddBase("ResampUp");
//      pChapter->AddBase("ResampUp32f");
//      pChapter->AddBase("ResampDown");
//      pChapter->AddBase("ResampDown32f");
}

//---------------------------------------------------------------------------
// ippCC initialization
//---------------------------------------------------------------------------

#define AddCC(r) Add(r, LIB_CC)
void CippiProcess::InitCC()
{
   CBook* pBook = NULL;
   CChapter* pChapter = NULL;

   pBook = AddCC("Color Model Conversion");
   pChapter = pBook->Add("RGB <-> YUV");
      pChapter->AddBase("RGBToYUV");
      pChapter->AddBase("RGBToYUV422");
      pChapter->AddBase("RGBToYUV420");
      pChapter->AddBase("YUVToRGB");
      pChapter->AddBase("YUV422ToRGB");
      pChapter->AddBase("YUV420ToRGB");
      pChapter->AddBase("YUV420ToRGB565");
      pChapter->AddBase("YUV420ToRGB555");
      pChapter->AddBase("YUV420ToRGB444");
      pChapter->AddBase("YUV420ToRGB565Dither");
      pChapter->AddBase("YUV420ToRGB555Dither");
      pChapter->AddBase("YUV420ToRGB444Dither");
   pChapter = pBook->Add("BGR <-> YUV");
      pChapter->AddBase("YUV420ToBGR");
      pChapter->AddBase("YUV420ToBGR565");
      pChapter->AddBase("YUV420ToBGR555");
      pChapter->AddBase("YUV420ToBGR444");
      pChapter->AddBase("YUV420ToBGR565Dither");
      pChapter->AddBase("YUV420ToBGR555Dither");
      pChapter->AddBase("YUV420ToBGR444Dither");
      pChapter->AddBase("BGR555ToYUV420");
      pChapter->AddBase("BGR565ToYUV420");
   pChapter = pBook->Add("RGB <-> YCbCr");
      pChapter->AddBase("RGBToYCbCr");
      pChapter->AddBase("RGBToYCbCr422");
      pChapter->AddBase("RGBToYCbCr422Gamma");
      pChapter->AddBase("RGBToYCbCr420");
      pChapter->AddBase("YCbCrToRGB");
      pChapter->AddBase("YCbCrToRGB444");
      pChapter->AddBase("YCbCrToRGB555");
      pChapter->AddBase("YCbCrToRGB565");
      pChapter->AddBase("YCbCrToRGB444Dither");
      pChapter->AddBase("YCbCrToRGB555Dither");
      pChapter->AddBase("YCbCrToRGB565Dither");
      pChapter->AddBase("YCbCr420ToRGB");
      pChapter->AddBase("YCbCr420ToRGB444");
      pChapter->AddBase("YCbCr420ToRGB555");
      pChapter->AddBase("YCbCr420ToRGB565");
      pChapter->AddBase("YCbCr420ToRGB444Dither");
      pChapter->AddBase("YCbCr420ToRGB555Dither");
      pChapter->AddBase("YCbCr420ToRGB565Dither");
      pChapter->AddBase("YCbCr422ToRGB");
      pChapter->AddBase("YCbCr422ToRGB444");
      pChapter->AddBase("YCbCr422ToRGB555");
      pChapter->AddBase("YCbCr422ToRGB565");
      pChapter->AddBase("YCbCr422ToRGB444Dither");
      pChapter->AddBase("YCbCr422ToRGB555Dither");
      pChapter->AddBase("YCbCr422ToRGB565Dither");
   pChapter = pBook->Add("BGR <-> YCbCr");
      pChapter->AddBase("YCbCrToBGR");
      pChapter->AddBase("YCbCrToBGR444");
      pChapter->AddBase("YCbCrToBGR555");
      pChapter->AddBase("YCbCrToBGR565");
      pChapter->AddBase("YCbCrToBGR444Dither");
      pChapter->AddBase("YCbCrToBGR555Dither");
      pChapter->AddBase("YCbCrToBGR565Dither");
      pChapter->AddBase("YCbCr420ToBGR");
      pChapter->AddBase("YCbCr420ToBGR444");
      pChapter->AddBase("YCbCr420ToBGR555");
      pChapter->AddBase("YCbCr420ToBGR565");
      pChapter->AddBase("YCbCr420ToBGR444Dither");
      pChapter->AddBase("YCbCr420ToBGR555Dither");
      pChapter->AddBase("YCbCr420ToBGR565Dither");
      pChapter->AddBase("YCbCr422ToBGR");
      pChapter->AddBase("YCbCr422ToBGR444");
      pChapter->AddBase("YCbCr422ToBGR555");
      pChapter->AddBase("YCbCr422ToBGR565");
      pChapter->AddBase("YCbCr422ToBGR444Dither");
      pChapter->AddBase("YCbCr422ToBGR555Dither");
      pChapter->AddBase("YCbCr422ToBGR565Dither");
      pChapter->AddBase("YCbCr411ToBGR");
      pChapter->AddBase("YCbCr411ToBGR555");
      pChapter->AddBase("YCbCr411ToBGR565");
      pChapter->AddBase("BGRToYCbCr411");
      pChapter->AddBase("BGRToYCbCr420");
      pChapter->AddBase("BGRToYCbCr422");
      pChapter->AddBase("BGR555ToYCbCr411");
      pChapter->AddBase("BGR555ToYCbCr420");
      pChapter->AddBase("BGR555ToYCbCr422");
      pChapter->AddBase("BGR565ToYCbCr411");
      pChapter->AddBase("BGR565ToYCbCr420");
      pChapter->AddBase("BGR565ToYCbCr422");
   pChapter = pBook->Add("BGR <-> YCrCb");
      pChapter->AddBase("BGRToYCrCb420");
      pChapter->AddBase("BGR555ToYCrCb420");
      pChapter->AddBase("BGR565ToYCrCb420");
   pChapter = pBook->Add("BGR(RGB) <-> CbYCr");
      pChapter->AddBase("CbYCr422ToBGR");
      pChapter->AddBase("CbYCr422ToRGB");
      pChapter->AddBase("BGRToCbYCr422");
      pChapter->AddBase("RGBToCbYCr422");
      pChapter->AddBase("RGBToCbYCr422Gamma");
   pChapter = pBook->Add("RGB <-> XYZ");
      pChapter->AddBase("RGBToXYZ");
      pChapter->AddBase("XYZToRGB");
   pChapter = pBook->Add("RGB <-> LUV");
      pChapter->AddBase("RGBToLUV");
      pChapter->AddBase("LUVToRGB");
   pChapter = pBook->Add("BGR <-> Lab");
      pChapter->AddBase("BGRToLab");
      pChapter->AddBase("LabToBGR");
   pChapter = pBook->Add("RGB <-> YCC");
      pChapter->AddBase("RGBToYCC");
      pChapter->AddBase("YCCToRGB");
   pChapter = pBook->Add("RGB(BGR) <-> HLS");
      pChapter->AddBase("RGBToHLS");
      pChapter->AddBase("HLSToRGB");
      pChapter->AddBase("BGRToHLS");
      pChapter->AddBase("HLSToBGR");
   pChapter = pBook->Add("RGB <-> HSV");
      pChapter->AddBase("RGBToHSV");
      pChapter->AddBase("HSVToRGB");
   pChapter = pBook->Add("BGR(SBGR) <-> HSV");
      pChapter->AddBase("YCoCgToBGR");
      pChapter->AddBase("YCoCgToSBGR");
      pChapter->AddBase("BGRToYCoCg");
      pChapter->AddBase("SBGRToYCoCg");
   pChapter = pBook->Add("BGR(SBGR) <-> HSV Rev");
      pChapter->AddBase("YCoCgToBGR_Rev");
      pChapter->AddBase("YCoCgToSBGR_Rev");
      pChapter->AddBase("BGRToYCoCg_Rev");
      pChapter->AddBase("SBGRToYCoCg_Rev");

   pBook = AddCC("Color to Gray Conversion");
   pChapter = pBook->Add("Color to Gray");
      pChapter->AddBase("ColorToGray");
   pChapter = pBook->Add("RGB to Gray");
      pChapter->AddBase("RGBToGray");

   pBook = AddCC("Reducing Bit Resolution");
   pChapter = pBook->Add("Reduce Bits");
      pChapter->AddBase("ReduceBits");

   pBook = AddCC("Format Conversion");
   pChapter = pBook->Add("YCbCr");
      pChapter->AddBase("YCbCr411");
      pChapter->AddBase("YCbCr420");
      pChapter->AddBase("YCbCr422");
      pChapter->AddBase("YCbCr411ToYCbCr420");
      pChapter->AddBase("YCbCr411ToYCbCr422");
      pChapter->AddBase("YCbCr420ToYCbCr411");
      pChapter->AddBase("YCbCr420ToYCbCr422");
      pChapter->AddBase("YCbCr422ToYCbCr411");
      pChapter->AddBase("YCbCr422ToYCbCr420");
      pChapter->AddBase("YCbCr420ToYCbCr422_Filter");
   pChapter = pBook->Add("YCbCr <-> YCrCb");
      pChapter->AddBase("YCbCr420ToYCrCb420");
      pChapter->AddBase("YCbCr420ToYCrCb422");
      pChapter->AddBase("YCbCr422ToYCrCb422");
      pChapter->AddBase("YCbCr422ToYCrCb420");
      pChapter->AddBase("YCbCr411ToYCrCb422");
      pChapter->AddBase("YCbCr411ToYCrCb420");
      pChapter->AddBase("YCrCb420ToYCbCr411");
      pChapter->AddBase("YCrCb420ToYCbCr420");
      pChapter->AddBase("YCrCb420ToYCbCr422");
      pChapter->AddBase("YCrCb422ToYCbCr411");
      pChapter->AddBase("YCrCb422ToYCbCr420");
      pChapter->AddBase("YCrCb422ToYCbCr422");
      pChapter->AddBase("YCrCb420ToYCbCr422_Filter");
      pChapter->AddBase("YCbCr420ToYCrCb420_Filter");
  pChapter = pBook->Add("YCbCr <-> CbYCr");
      pChapter->AddBase("CbYCr420ToYCbCr422");
      pChapter->AddBase("CbYCr422ToYCbCr411");
      pChapter->AddBase("CbYCr422ToYCbCr420");
      pChapter->AddBase("CbYCr422ToYCbCr422");
      pChapter->AddBase("YCbCr420ToCbYCr422");
      pChapter->AddBase("YCbCr422ToCbYCr422");
  pChapter = pBook->Add("YCrCb <-> CbYCr");
      pChapter->AddBase("YCrCb420ToCbYCr422");
      pChapter->AddBase("CbYCr422ToYCrCb420");

   pBook = AddCC("Color Twist");
   pChapter = pBook->Add("Color Twist");
      pChapter->AddBase("ColorTwist");
      pChapter->AddBase("ColorTwist32f");

   pBook = AddCC("Gamma Correction");
   pChapter = pBook->Add("Forward");
      pChapter->AddBase("GammaFwd");
   pChapter = pBook->Add("Inverse");
      pChapter->AddBase("GammaInv");
}

//---------------------------------------------------------------------------
// ippJP initialization
//---------------------------------------------------------------------------

#define AddJP(r) Add(r, LIB_J)
void CippiProcess::InitJP()
{
   CBook* pBook = NULL;
   CChapter* pChapter = NULL;

   pBook = AddJP("Support");

   pBook = AddJP("Color Conversion");

   pBook = AddJP("Combined Clr Cnvrsn");

   pBook = AddJP("Quantization");

   pBook = AddJP("Combined DCT");

   pBook = AddJP("Level Shift");

   pBook = AddJP("Sampling");

   pBook = AddJP("Planar<->Pixel");

   pBook = AddJP("Huffman Codec");
   pChapter = pBook->Add("Huffman8x8");
      pChapter->AddBase("EncodeHuffman8x8_JPEG");
      pChapter->AddBase("DecodeHuffman8x8_JPEG");

   pBook = AddJP("Wavelet Transform");

   pBook = AddJP("JPEG2000");

   pBook = AddJP("Component Transform");
}

//---------------------------------------------------------------------------
// ippCV initialization
//---------------------------------------------------------------------------

#define AddCV(r) Add(r, LIB_CV)
void CippiProcess::InitCV()
{
   CBook* pBook = NULL;
   CChapter* pChapter = NULL;

   pBook = AddCV("Arithmetic");
   pChapter = pBook->Add("Add");
      pChapter->AddBase("Add");
   pChapter = pBook->Add("AddSquare");
      pChapter->AddBase("AddSquare");
   pChapter = pBook->Add("AddProduct");
      pChapter->AddBase("AddProduct");
   pChapter = pBook->Add("AddWeighted");
      pChapter->AddBase("AddWeighted");
   pChapter = pBook->Add("Abs Diff");
      pChapter->AddBase("AbsDiff");
      pChapter->AddBase("AbsDiffC");

   pBook = AddCV("Copy");
   pChapter = pBook->Add("Copy");
      pChapter->AddBase("CopySubpix");
      pChapter->AddBase("CopySubpixIntersect");

   pBook = AddCV("Morphological");
   pChapter = pBook->Add("Dilate");
      pChapter->AddBase("DilateBorderReplicate");
   pChapter = pBook->Add("Erode");
      pChapter->AddBase("ErodeBorderReplicate");

   pBook = AddCV("Filters");
   pChapter = pBook->Add("Scharr");
      pChapter->AddBase("FilterScharrHorizBorder");
      pChapter->AddBase("FilterScharrVertBorder");
   pChapter = pBook->Add("Sobel");
      pChapter->AddBase("FilterSobelHorizBorder");
      pChapter->AddBase("FilterSobelVertBorder");
      pChapter->AddBase("FilterSobelHorizSecondBorder");
      pChapter->AddBase("FilterSobelVertSecondBorder");
      pChapter->AddBase("FilterSobelCrossBorder");
   pChapter = pBook->Add("Laplacian");
      pChapter->AddBase("FilterLaplacianBorder");
   pChapter = pBook->Add("Lowpass");
      pChapter->AddBase("FilterLowpassBorder");
   pChapter = pBook->Add("Min");
      pChapter->AddBase("FilterMinBorderReplicate");
   pChapter = pBook->Add("Max");
      pChapter->AddBase("FilterMaxBorderReplicate");
   pChapter = pBook->Add("Column");
      pChapter->AddBase("FilterColumnPipeline");
      pChapter->AddBase("FilterColumnPipeline_Low");
   pChapter = pBook->Add("Row");
      pChapter->AddBase("FilterRowBorderPipeline");
      pChapter->AddBase("FilterRowBorderPipeline_Low");

   //pBook = AddCV("Feature Detection");
   //pChapter = pBook->Add("MatchTemplate");
   //   pChapter->AddBase("MatchTemplate_SqDiff");
   //   pChapter->AddBase("MatchTemplate_SqDiffNormed");
   //   pChapter->AddBase("MatchTemplate_Corr");
   //   pChapter->AddBase("MatchTemplate_CorrNormed");
   //   pChapter->AddBase("MatchTemplate_Coeff");
   //   pChapter->AddBase("MatchTemplate_CoeffNormed");
   //pChapter = pBook->Add("Eigen");
   //   pChapter->AddBase("MinEigenVal");
   //   pChapter->AddBase("EigenValsVecs");
   //pChapter = pBook->Add("Canny");
   //   pChapter->AddBase("Canny");

   //pBook = AddCV("Distance Transform");
   //pChapter = pBook->Add("DistanceTransform");
   //   pChapter->AddBase("DistanceTransform_3x3");
   //   pChapter->AddBase("DistanceTransform_5x5");

   pBook = AddCV("Flood Fill");
   pChapter = pBook->Add("FloodFill");
      pChapter->AddBase("FloodFill_4Con");
      pChapter->AddBase("FloodFill_8Con");
   pChapter = pBook->Add("FloodFill_Grad");
      pChapter->AddBase("FloodFill_Grad4Con");
      pChapter->AddBase("FloodFill_Grad8Con");

   pBook = AddCV("Motion Templates");
   pChapter = pBook->Add("MotionHistory");
      pChapter->AddBase("UpdateMotionHistory");

   pBook = AddCV("Piramids");
   pChapter = pBook->Add("PyrDown");
      pChapter->AddBase("PyrDown_Gauss5x5");
   pChapter = pBook->Add("PyrUp");
      pChapter->AddBase("PyrUp_Gauss5x5");

   pBook = AddCV("Statistics");
   pChapter = pBook->Add("Mean, StdDev, Min, Max");
      pChapter->AddBase("MinMaxIndx");
      pChapter->AddBase("Mean");
      pChapter->AddBase("Mean_StdDev");
   pChapter = pBook->Add("Norms");
      pChapter->AddBase("Norm_Inf");
      pChapter->AddBase("Norm_L1");
      pChapter->AddBase("Norm_L2");
   pChapter = pBook->Add("Diff Norms");
      pChapter->AddBase("NormDiff_Inf");
      pChapter->AddBase("NormDiff_L1");
      pChapter->AddBase("NormDiff_L2");
      pChapter->AddBase("NormRel_Inf");
      pChapter->AddBase("NormRel_L1");
      pChapter->AddBase("NormRel_L2");

}



