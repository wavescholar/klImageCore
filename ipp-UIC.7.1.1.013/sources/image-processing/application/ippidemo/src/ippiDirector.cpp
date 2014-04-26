/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiDirector.cpp: implementation of the CippiDirector class.
// class CippiDirector processes vector by any ippIP, ippCC, ippCV or ippJP
// function that can be called by one of CRun classes listed in
// CippsDirector constructor.
// See CDirector class for more information
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDirector.h"
#include "ippiProcess.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CippiDirector::CippiDirector() : CDirector()
{
   m_pProcess = NewProcess();
   InitIP();
   InitCC();
   InitJP();
   InitCV();
}

/////////////////////////////////////////////////////////////////////////////
// CDirector virtual function overrides

//---------------------------------------------------------------------------
// NewProcess creates new CProcess class that provides all implemented
// IPP functions classification for ippIP, ippCC, ippCV, ippJP libraries
//---------------------------------------------------------------------------

CProcess* CippiDirector::NewProcess()
{
   return new CippiProcess;
}

//---------------------------------------------------------------------------
// DeleteProcess delets CProcess class created by NewProcess function
//---------------------------------------------------------------------------

void CippiDirector::DeleteProcess(CProcess*& pProcess) {
   if (pProcess) delete (CippiProcess*)pProcess;
   pProcess = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

//---------------------------------------------------------------------------
// ippIP initialization
//---------------------------------------------------------------------------

#if LIB_I < LIB_NUM
#include "RunDCT8x8LS.h"
#include "RunStat.h"
#include "RunStatIdx.h"
#include "RunStatCount.h"
#include "RunSwap.h"
#include "RunCorr.h"
#include "RunConv.h"
#include "RunWarpq.h"
#include "RunWarp.h"
#include "RunShear.h"
#include "RunRotate.h"
#include "RunFilterCol.h"
#include "RunFilterRow.h"
#include "RunFilterCustom.h"
#include "RunDFT.h"
#include "RunFFT.h"
#include "RunRand.h"
#include "RunAlpha.h"
#include "RunAlphaC.h"
#include "RunRamp.h"
#include "RunMorph.h"
#include "RunCmp.h"
#include "RunThresh.h"
#include "RunHint.h"
#include "RunFilterBox.h"
#include "RunSet.h"
#include "RunRound.h"
#include "RunScale.h"
#include "RunDct.h"
#include "RunDct8x8.h"
#include "RunNorm.h"
#include "RunMoment.h"
#include "RunC13.h"
#include "RunMirror.h"
#include "RunRemap.h"
#include "RunFilter.h"
#include "RunImg2.h"
#include "RunImg3.h"
#include "RunValue.h"
#include "RunHistoEven.h"
#include "RunHistoRange.h"
#include "RunLut.h"
#include "RunWT.h"
#include "RunCopyBorder.h"
#include "RunCopyM.h"
#include "RunFilterWiener.h"
#include "RunResizeSqrPixel.h"
#include "RunConvert1u.h"
#include "RunTranspose.h"
#include "RunRow.h"
#include "RunRow.h"
#include "RunSuper.h"
#include "RunResizeFilter.h"
#include "RunRound3.h"
#include "RunDecimate.h"
#include "RunCompColor.h"
#include "RunAlphaColor.h"
#include "RunCopyManaged.h"
#include "RunThreshOtsu.h"
#include "RunDeconvFFT.h"
#include "RunDeconvLR.h"
#include "RunFilterBilat.h"
#include "RunDotProd.h"

void CippiDirector::InitIP()
{
   AddTail(new CRunDotProd);
   AddTail(new CRunFilterBilat);
   AddTail(new CRunDeconvLR);
   AddTail(new CRunDeconvFFT);
   AddTail(new CRunThreshOtsu);
   AddTail(new CRunCopyManaged);
   AddTail(new CRunAlphaColor);
   AddTail(new CRunCompColor);
   AddTail(new CRunDecimate);
   AddTail(new CRunRound3);
   AddTail(new CRunResizeFilter);
   AddTail(new CRunSuper);
   AddTail(new CRunRow);
   AddTail(new CRunTranspose);
   AddTail(new CRunConvert1u);
   AddTail(new CRunResizeSqrPixel);
   AddTail(new CRunFilterWiener);
   AddTail(new CRunCopyM);
   AddTail(new CRunCopyBorder);
   AddTail(new CRunWT);
   AddTail(new CRunLut);
   AddTail(new CRunHistoRange);
   AddTail(new CRunHistoEven);
   AddTail(new CRunDCT8x8LS);
   AddTail(new CRunStat);
   AddTail(new CRunStatIdx);
   AddTail(new CRunStatCount);
   AddTail(new CRunSwap);
   AddTail(new CRunCorr);
   AddTail(new CRunConv);
   AddTail(new CRunWarpq);
   AddTail(new CRunWarp);
   AddTail(new CRunShear);
   AddTail(new CRunRotate);
   AddTail(new CRunFilterRow);
   AddTail(new CRunFilterCol);
   AddTail(new CRunFilterCustom);
   AddTail(new CRunDFT);
   AddTail(new CRunFFT);
   AddTail(new CRunRand);
   AddTail(new CRunAlpha);
   AddTail(new CRunAlphaC);
   AddTail(new CRunRamp);
   AddTail(new CRunMorph);
   AddTail(new CRunCmp);
   AddTail(new CRunThresh);
   AddTail(new CRunHint);
   AddTail(new CRunFilterBox);
   AddTail(new CRunSet);
   AddTail(new CRunRound);
   AddTail(new CRunScale);
   AddTail(new CRunDct);
   AddTail(new CRunDCT8x8);
   AddTail(new CRunNorm);
   AddTail(new CRunMoment);
   AddTail(new CRunC13);
   AddTail(new CRunMirror);
   AddTail(new CRunRemap);
   AddTail(new CRunFilter);
   AddTail(new CRunImg2);
   AddTail(new CRunImg3);
   AddTail(new CRunValue);
}
#else
void CippiDirector::InitIP() {}
#endif

//---------------------------------------------------------------------------
// ippJP initialization
//---------------------------------------------------------------------------

#if LIB_J < LIB_NUM
#include "RunJHuffman.h"

void CippiDirector::InitJP()
{
   AddTail(new CRunJHuffman);
}
#else
void CippiDirector::InitJP() {}
#endif

//---------------------------------------------------------------------------
// ippCC initialization
//---------------------------------------------------------------------------

#if LIB_CC < LIB_NUM

#include "RunGamma.h"
#include "RunReduce.h"
#include "RunColor.h"
#include "RunTwist.h"
#include "RunSlice.h"
#include "RunYuv.h"
#include "RunCC.h"
void CippiDirector::InitCC()
{
   AddTail(new CRunCC);
   AddTail(new CRunSlice);
   AddTail(new CRunReduce);
   AddTail(new CRunGamma);
   AddTail(new CRunColor);
   AddTail(new CRunTwist);
   AddTail(new CRunYuv);
}
#else
void CippiDirector::InitCC() {}
#endif

//---------------------------------------------------------------------------
// ippCV initialization
//---------------------------------------------------------------------------

#if LIB_CV < LIB_NUM
#include "RunCV2Value.h"
#include "RunCV3.h"
#include "RunCVFlood.h"
#include "RunCVMorph.h"
#include "RunCVMotion.h"
#include "RunCVPyr.h"
#include "RunCV2.h"
#include "RunCVFilter.h"
#include "RunCVFilterMask.h"
#include "RunCVCopy.h"
#include "RunCVFilterMax.h"
#include "RunCVFilterColumn.h"
#include "RunCVFilterRow.h"
#include "RunCVStat.h"
#include "RunCVNorm.h"

void CippiDirector::InitCV()
{
   AddTail(new CRunCVNorm);
   AddTail(new CRunCVStat);
   AddTail(new CRunCVFilterRow);
   AddTail(new CRunCVFilterColumn);
   AddTail(new CRunCVFilterMax);
   AddTail(new CRunCVCopy);
   AddTail(new CRunCVFilterMask);
   AddTail(new CRunCVFilter);
   AddTail(new CRunCV2);
   AddTail(new CRunCV2Value);
   AddTail(new CRunCV3);
   AddTail(new CRunCVMotion);
   AddTail(new CRunCVFlood);
   AddTail(new CRunCVPyr);
//   AddTail(new CRunCVSobel);
   AddTail(new CRunCVMorph);
}
#else
void CippiDirector::InitCV() {}
#endif
