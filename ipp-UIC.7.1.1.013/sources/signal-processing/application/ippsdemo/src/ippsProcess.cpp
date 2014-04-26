/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsProcess.cpp: implementation of the CippsProcess
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////
// CippsProcess class implementation
// class CippsProcess provides classification for ippSP library.
///////////////////////////////////////////////////////////////////////

CippsProcess::CippsProcess() : CProcess()
{
   if (LIB_SP  < LIB_NUM) InitSP();
   if (LIB_CH  < LIB_NUM) InitCH();
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

//---------------------------------------------------------------------------
// ippSP initialization
//---------------------------------------------------------------------------


void CippsProcess::InitSP()
{
   CBook* pBook = NULL;
   CChapter* pChapter = NULL;

   pBook = Add("Vector Initialization");
   pChapter = pBook->Add("Copy, Move"); 
      pChapter->AddBase("Copy");
      pChapter->AddBase("Move");
   pChapter = pBook->Add("Set, Zero"); 
      pChapter->AddBase("Set");
      pChapter->AddBase("Zero");
   pChapter = pBook->Add("Tone, Triangle"); 
      pChapter->AddBase("Tone_Direct");
      pChapter->AddBase("Triangle_Direct");
      pChapter->AddBase("ToneQ15");
      pChapter->AddBase("TriangleQ15");
      pChapter->AddBase("ToneQ15_Direct");
      pChapter->AddBase("TriangleQ15_Direct");
   pChapter = pBook->Add("Random"); 
      pChapter->AddBase("RandUniform_Direct");
      pChapter->AddBase("RandUniform");
      pChapter->AddBase("RandGauss_Direct");
      pChapter->AddBase("RandGauss");
   pChapter = pBook->Add("Jaehne"); 
      pChapter->AddBase("VectorJaehne");
   pChapter = pBook->Add("Ramp"); 
      pChapter->AddBase("VectorRamp");

   pBook = Add("Logical and Shift Functions");
   pChapter = pBook->Add("And"); 
      pChapter->AddBase("And");
      pChapter->AddBase("AndC");
   pChapter = pBook->Add("Or"); 
      pChapter->AddBase("Or");
      pChapter->AddBase("OrC");
   pChapter = pBook->Add("Xor"); 
      pChapter->AddBase("Xor");
      pChapter->AddBase("XorC");
   pChapter = pBook->Add("Not"); 
      pChapter->AddBase("Not");
   pChapter = pBook->Add("Shift"); 
      pChapter->AddBase("LShiftC");
      pChapter->AddBase("RShiftC");

   pBook = Add("Arithmetic Functions");
   pChapter = pBook->Add("Add"); 
      pChapter->AddBase("Add");
      pChapter->AddBase("AddC");
      pChapter->AddBase("AddProduct");
      pChapter->AddBase("AddProductC");
   pChapter = pBook->Add("Multiply"); 
      pChapter->AddBase("Mul");
      pChapter->AddBase("MulC");
      pChapter->AddBase("MulC_Low");
   pChapter = pBook->Add("Subtract"); 
      pChapter->AddBase("Sub");
      pChapter->AddBase("SubC");
      pChapter->AddBase("SubCRev");
   pChapter = pBook->Add("Divide"); 
      pChapter->AddBase("Div");
      pChapter->AddBase("DivC");
      pChapter->AddBase("DivCRev");
   pChapter = pBook->Add("Abs"); 
      pChapter->AddBase("Abs");
   pChapter = pBook->Add("Square"); 
      pChapter->AddBase("Sqr");
   pChapter = pBook->Add("Square && Cube Root"); 
      pChapter->AddBase("Sqrt");
      pChapter->AddBase("Cubrt");
   pChapter = pBook->Add("Exponent"); 
      pChapter->AddBase("Exp");
   pChapter = pBook->Add("Logarithm"); 
      pChapter->AddBase("Ln");
      pChapter->AddBase("10Log10");
   pChapter = pBook->Add("Sum Logarithm"); 
      pChapter->AddBase("SumLn");
   pChapter = pBook->Add("Normalize");          
      pChapter->AddBase("Normalize");

   pBook = Add("Conversion Functions");
   pChapter = pBook->Add("Sort"); 
      pChapter->AddBase("SortAscend");
      pChapter->AddBase("SortDescend");
   pChapter = pBook->Add("Swap"); 
      pChapter->AddBase("SwapBytes");
   pChapter = pBook->Add("Convert"); 
      pChapter->AddBase("Convert");
   pChapter = pBook->Add("Join && Split"); 
      pChapter->AddBase("Join");
      pChapter->AddBase("Split");
      pChapter->AddBase("JoinScaled");
      pChapter->AddBase("SplitScaled");
   pChapter = pBook->Add("Conjugation"); 
      pChapter->AddBase("Conj");
      pChapter->AddBase("ConjFlip");
      pChapter->AddBase("ConjCcs");
      pChapter->AddBase("ConjPack");
      pChapter->AddBase("ConjPerm");
   pChapter = pBook->Add("Real to Complex"); 
      pChapter->AddBase("RealToCplx");
   pChapter = pBook->Add("Complex to Real"); 
      pChapter->AddBase("CplxToReal");
      pChapter->AddBase("Real");
      pChapter->AddBase("Imag");
   pChapter = pBook->Add("Threshold"); 
      pChapter->AddBase("Threshold");
      pChapter->AddBase("Threshold_LT");
      pChapter->AddBase("Threshold_GT");
      pChapter->AddBase("Threshold_LTAbs");
      pChapter->AddBase("Threshold_GTAbs");
      pChapter->AddBase("Threshold_LTInv");
   pChapter = pBook->Add("Threshold with Value"); 
      pChapter->AddBase("Threshold_LTVal");
      pChapter->AddBase("Threshold_GTVal");
      pChapter->AddBase("Threshold_LTValGTVal");
   pChapter = pBook->Add("Find"); 
      pChapter->AddBase("FindNearest");
      pChapter->AddBase("FindNearestOne");
   pChapter = pBook->Add("Cartesian<->Polar"); 
      pChapter->AddBase("CartToPolar");
      pChapter->AddBase("Magnitude");
      pChapter->AddBase("MagSquared");      
      pChapter->AddBase("Phase");
      pChapter->AddBase("Arctan");
      pChapter->AddBase("PolarToCart");
   pChapter = pBook->Add("Power Spectr"); 
      pChapter->AddBase("PowerSpectr");
   pChapter = pBook->Add("Preemphasize"); 
      pChapter->AddBase("Preemphasize");
   pChapter = pBook->Add("Flip"); 
      pChapter->AddBase("Flip");

   pBook = Add("Companding Functions");
   pChapter = pBook->Add("ALaw and Lin"); 
      pChapter->AddBase("ALawToLin");
      pChapter->AddBase("LinToALaw");
   pChapter = pBook->Add("MuLaw and Lin"); 
      pChapter->AddBase("MuLawToLin");
      pChapter->AddBase("LinToMuLaw");
   pChapter = pBook->Add("ALaw and MuLaw"); 
      pChapter->AddBase("ALawToMuLaw");
      pChapter->AddBase("MuLawToALaw");

   pBook = Add("Windowing Functions");
   pChapter = pBook->Add("Bartlett"); 
      pChapter->AddBase("WinBartlett");
   pChapter = pBook->Add("Blackman"); 
      pChapter->AddBase("WinBlackman");
      pChapter->AddBase("WinBlackmanQ15");
      pChapter->AddBase("WinBlackmanStd");
      pChapter->AddBase("WinBlackmanOpt");
   pChapter = pBook->Add("Hamming"); 
      pChapter->AddBase("WinHamming");
   pChapter = pBook->Add("Hann"); 
      pChapter->AddBase("WinHann");
   pChapter = pBook->Add("Kaiser"); 
      pChapter->AddBase("WinKaiser");
      pChapter->AddBase("WinKaiserQ15");

   pBook = Add("Statistical Functions");
   pChapter = pBook->Add("Min, Max"); 
      pChapter->AddBase("Max");
      pChapter->AddBase("Min");
      pChapter->AddBase("MaxIndx");
      pChapter->AddBase("MinIndx");
      pChapter->AddBase("MaxAbs");
      pChapter->AddBase("MinAbs");
      pChapter->AddBase("MaxAbsIndx");
      pChapter->AddBase("MinAbsIndx");
      pChapter->AddBase("MaxOrder");
   pChapter = pBook->Add("MinMax"); 
      pChapter->AddBase("MinMax");
      pChapter->AddBase("MinMaxIndx");
   pChapter = pBook->Add("Sum, Mean, StdDev"); 
      pChapter->AddBase("Sum");
      pChapter->AddBase("Mean");
      pChapter->AddBase("StdDev");
   pChapter = pBook->Add("Min/Max Every"); 
      pChapter->AddBase("MaxEvery");
      pChapter->AddBase("MinEvery");
   pChapter = pBook->Add("Norm"); 
      pChapter->AddBase("Norm_Inf");
      pChapter->AddBase("Norm_L1");
      pChapter->AddBase("Norm_L2");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
      pChapter->AddBase("Norm_L2Sqr");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
      pChapter->AddBase("NormDiff_Inf");
      pChapter->AddBase("NormDiff_L1");
      pChapter->AddBase("NormDiff_L2");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
      pChapter->AddBase("NormDiff_L2Sqr");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
   pChapter = pBook->Add("Dot Product"); 
      pChapter->AddBase("DotProd");

   pBook = Add("Sampling  Functions");
   pChapter = pBook->Add("Up Sampling"); 
      pChapter->AddBase("SampleUp");
   pChapter = pBook->Add("Down Sampling"); 
      pChapter->AddBase("SampleDown");

   pBook = Add("Convolution and Correlation");
   pChapter = pBook->Add("Convolution"); 
      pChapter->AddBase("Conv");
      pChapter->AddBase("ConvBiased");
   pChapter = pBook->Add("Cyclic Convolution"); 
      pChapter->AddBase("ConvCyclic8x8");
      pChapter->AddBase("ConvCyclic4x4");
   pChapter = pBook->Add("Cross Correlation"); 
      pChapter->AddBase("CrossCorr");
   pChapter = pBook->Add("Auto Correlation"); 
      pChapter->AddBase("AutoCorr");
      pChapter->AddBase("AutoCorr_NormA");
      pChapter->AddBase("AutoCorr_NormB");
   pChapter = pBook->Add("Update"); 
      pChapter->AddBase("UpdateLinear");
      pChapter->AddBase("UpdatePower");

   pBook = Add("Filtering Functions");
   pChapter = pBook->Add("FIR"); 
      pChapter->AddBase("FIR");
      pChapter->AddBase("FIR64f");
      pChapter->AddBase("FIR32f");
      pChapter->AddBase("FIR32s");
      pChapter->AddBase("FIR32sc");
      pChapter->AddBase("FIR32fc");
      pChapter->AddBase("FIR64fc");
      pChapter->AddBase("FIROne");
      pChapter->AddBase("FIROne64f");
      pChapter->AddBase("FIROne32f");
      pChapter->AddBase("FIROne32s");
      pChapter->AddBase("FIROne64fc");
      pChapter->AddBase("FIROne32fc");
      pChapter->AddBase("FIROne32sc");
   pChapter = pBook->Add("FIR Direct"); 
      pChapter->AddBase("FIR_Direct");
      pChapter->AddBase("FIR64f_Direct");
      pChapter->AddBase("FIR32f_Direct");
      pChapter->AddBase("FIR32s_Direct");
      pChapter->AddBase("FIR64fc_Direct");
      pChapter->AddBase("FIR32fc_Direct");
      pChapter->AddBase("FIR32sc_Direct");
      pChapter->AddBase("FIRMR_Direct");
      pChapter->AddBase("FIRMR64f_Direct");
      pChapter->AddBase("FIRMR32f_Direct");
      pChapter->AddBase("FIRMR32s_Direct");
      pChapter->AddBase("FIRMR64fc_Direct");
      pChapter->AddBase("FIRMR32fc_Direct");
      pChapter->AddBase("FIRMR32sc_Direct");
      pChapter->AddBase("FIROne_Direct");
      pChapter->AddBase("FIROne64f_Direct");
      pChapter->AddBase("FIROne32f_Direct");
      pChapter->AddBase("FIROne32s_Direct");
      pChapter->AddBase("FIROne64fc_Direct");
      pChapter->AddBase("FIROne32fc_Direct");
      pChapter->AddBase("FIROne32sc_Direct");
   pChapter = pBook->Add("FIR Sparse"); 
      pChapter->AddBase("FIRSparse");
   pChapter = pBook->Add("FIR Generation"); 
      pChapter->AddBase("FIRGenLowpass");
      pChapter->AddBase("FIRGenHighpass");
      pChapter->AddBase("FIRGenBandpass");
      pChapter->AddBase("FIRGenBandstop");
   pChapter = pBook->Add("FIR LMS"); 
      pChapter->AddBase("FIRLMS");
      pChapter->AddBase("FIRLMS32f");
      pChapter->AddBase("FIRLMSMROne32s");
      pChapter->AddBase("FIRLMSMROne32sc");
      pChapter->AddBase("FIRLMSMROneVal32s");
      pChapter->AddBase("FIRLMSMROneVal32sc");
   pChapter = pBook->Add("FIR LMS Direct"); 
      pChapter->AddBase("FIRLMSOne_Direct");
      pChapter->AddBase("FIRLMSOne_Direct32f");
      pChapter->AddBase("FIRLMSOne_DirectQ15");
   pChapter = pBook->Add("IIR"); 
      pChapter->AddBase("IIR");
      pChapter->AddBase("IIR64f");
      pChapter->AddBase("IIR32f");
      pChapter->AddBase("IIR32s");
      pChapter->AddBase("IIR64fc");
      pChapter->AddBase("IIR32fc");
      pChapter->AddBase("IIR32sc");
      pChapter->AddBase("IIROne");
      pChapter->AddBase("IIROne64f");
      pChapter->AddBase("IIROne32f");
      pChapter->AddBase("IIROne32s");
      pChapter->AddBase("IIROne64fc");
      pChapter->AddBase("IIROne32fc");
      pChapter->AddBase("IIROne32sc");
   pChapter = pBook->Add("IIR Direct"); 
      pChapter->AddBase("IIR_Direct");
      pChapter->AddBase("IIR_BiQuadDirect");
      pChapter->AddBase("IIROne_Direct");
      pChapter->AddBase("IIROne_BiQuadDirect");
   pChapter = pBook->Add("IIR Sparse"); 
      pChapter->AddBase("IIRSparse");
   pChapter = pBook->Add("Median"); 
      pChapter->AddBase("FilterMedian");
   pChapter = pBook->Add("SumWindow"); 
      pChapter->AddBase("SumWindow");

   pBook = Add("Transform Functions");
   pChapter = pBook->Add("Packed Data Mul"); 
      pChapter->AddBase("MulPack");
      pChapter->AddBase("MulPackConj");
      pChapter->AddBase("MulPerm");
   pChapter = pBook->Add("FFT"); 
      pChapter->AddBase("FFTFwd_CToC");
      pChapter->AddBase("FFTInv_CToC");
      pChapter->AddBase("FFTFwd_RToPack");
      pChapter->AddBase("FFTFwd_RToPerm");
      pChapter->AddBase("FFTFwd_RToCCS");
      pChapter->AddBase("FFTInv_PackToR");
      pChapter->AddBase("FFTInv_PermToR");
      pChapter->AddBase("FFTInv_CCSToR");
   pChapter = pBook->Add("DFT"); 
      pChapter->AddBase("DFTFwd_CToC");
      pChapter->AddBase("DFTInv_CToC");
      pChapter->AddBase("DFTFwd_RToPack");
      pChapter->AddBase("DFTFwd_RToPerm");
      pChapter->AddBase("DFTFwd_RToCCS");
      pChapter->AddBase("DFTInv_PackToR");
      pChapter->AddBase("DFTInv_PermToR");
      pChapter->AddBase("DFTInv_CCSToR");
      pChapter->AddBase("DFTOutOrdFwd_CToC");
      pChapter->AddBase("DFTOutOrdInv_CToC");
   pChapter = pBook->Add("Goertzel"); 
      pChapter->AddBase("Goertz");
      pChapter->AddBase("GoertzTwo");
      pChapter->AddBase("GoertzQ15");
      pChapter->AddBase("GoertzTwoQ15");
   pChapter = pBook->Add("DCT"); 
      pChapter->AddBase("DCTFwd");
      pChapter->AddBase("DCTInv");
   pChapter = pBook->Add("Hilbert"); 
      pChapter->AddBase("Hilbert");
   pChapter = pBook->Add("Fixed Wavelet"); 
      pChapter->AddBase("WTHaarFwd");
      pChapter->AddBase("WTHaarInv");
   pChapter = pBook->Add("User Wavelet"); 
      pChapter->AddBase("WTFwd");
      pChapter->AddBase("WTInv");
}

//---------------------------------------------------------------------------
// ippCH initialization
//---------------------------------------------------------------------------

#define AddCH(r) Add(r, LIB_CH)
void CippsProcess::InitCH()
{
   CBook* pBook = NULL;
   CChapter* pChapter = NULL;

   pBook = AddCH(_T("Find && Replace"));
   pChapter = pBook->Add(_T("Find")); 
      pChapter->AddBase(_T("Find"));
   pChapter = pBook->Add(_T("FindC")); 
      pChapter->AddBase(_T("FindC"));
   pChapter = pBook->Add(_T("FindCAny")); 
      pChapter->AddBase(_T("FindCAny"));
   pChapter = pBook->Add(_T("FindRev")); 
      pChapter->AddBase(_T("FindRev"));
   pChapter = pBook->Add(_T("FindRevC")); 
      pChapter->AddBase(_T("FindRevC"));
   pChapter = pBook->Add(_T("FindRevCAny")); 
      pChapter->AddBase(_T("FindRevCAny"));
   pChapter = pBook->Add(_T("ReplaceC")); 
      pChapter->AddBase(_T("ReplaceC"));

   pBook = AddCH(_T("Compare"));
   pChapter = pBook->Add(_T("Compare")); 
      pChapter->AddBase(_T("Compare"));
   pChapter = pBook->Add(_T("CompareIgnoreCase")); 
      pChapter->AddBase(_T("CompareIgnoreCase"));
      pChapter->AddBase(_T("CompareIgnoreCaseLatin"));
   pChapter = pBook->Add(_T("Equal")); 
      pChapter->AddBase(_T("Equal"));

   pBook = AddCH(_T("Trim"));
   pChapter = pBook->Add(_T("TrimC")); 
      pChapter->AddBase(_T("TrimC"));
   pChapter = pBook->Add(_T("TrimCAny")); 
      pChapter->AddBase(_T("TrimCAny"));
   pChapter = pBook->Add(_T("Trim Edges")); 
      pChapter->AddBase(_T("TrimEndCAny"));
      pChapter->AddBase(_T("TrimStartCAny"));

   pBook = AddCH(_T("Upper && Lower Case"));
   pChapter = pBook->Add(_T("UppercaseLatin")); 
      pChapter->AddBase(_T("UppercaseLatin"));
   pChapter = pBook->Add(_T("LowercaseLatin")); 
      pChapter->AddBase(_T("LowercaseLatin"));
   pChapter = pBook->Add(_T("Uppercase")); 
      pChapter->AddBase(_T("Uppercase"));
   pChapter = pBook->Add(_T("Lowercase")); 
      pChapter->AddBase(_T("Lowercase"));

   pBook = AddCH(_T("Hash"));
   pChapter = pBook->Add(_T("Hash")); 
      pChapter->AddBase(_T("Hash"));
      pChapter->AddBase(_T("HashSJ2"));
      pChapter->AddBase(_T("HashMSCS"));

   pBook = AddCH(_T("Concat && Split"));
   pChapter = pBook->Add(_T("Concat")); 
      pChapter->AddBase(_T("Concat"));
      pChapter->AddBase(_T("ConcatC"));
   pChapter = pBook->Add(_T("Split")); 
      pChapter->AddBase(_T("SplitC"));

   pBook = AddCH(_T("Insert && Remove"));
   pChapter = pBook->Add(_T("Insert")); 
      pChapter->AddBase(_T("Insert"));
   pChapter = pBook->Add(_T("Remove")); 
      pChapter->AddBase(_T("Remove"));

   pBook = AddCH(_T("Regular Expression"));
   pChapter = pBook->Add(_T("RegExpFind")); 
      pChapter->AddBase(_T("RegExpFind"));
}


BOOL CippsProcess::DepthEssencial(const CFunc& func)
{
   CString baseName = func.BaseName();
    return (
         baseName == "Copy"
      || baseName == "Move"
      || baseName == "Zero"
      || baseName == "Flip"
      || baseName == "Move"
      || baseName == "RealToCplx"
      || baseName == "CplxToReal"
      || baseName == "Real"
      || baseName == "Imag"
      || baseName == "SwapBytes");
}

