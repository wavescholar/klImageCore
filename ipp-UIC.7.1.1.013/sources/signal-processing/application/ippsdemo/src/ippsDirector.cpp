/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsDirector.cpp: implementation of the CDirector class.
// class CippsDirector processes vector by any ippSP function that
// can be called by one of CRun classes listed in CippsDirector constructor.
// See CDirector class for more information
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDirector.h"
#include "ippsProcess.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CippsDirector::CippsDirector() : CDirector()
{
   m_pProcess = NewProcess();
   InitSP();
   InitCH();
}

/////////////////////////////////////////////////////////////////////////////
// CDirector virtual function overrides

//---------------------------------------------------------------------------
// NewProcess creates new CProcess class that provides all implemented 
// IPP functions classification for ippIP, ippCC, ippCV, ippJP libraries
//---------------------------------------------------------------------------

CProcess* CippsDirector::NewProcess() 
{
   return new CippsProcess;
}

//---------------------------------------------------------------------------
// DeleteProcess delets CProcess class created by NewProcess function
//---------------------------------------------------------------------------

void CippsDirector::DeleteProcess(CProcess*& pProcess) { 
   if (pProcess) delete (CippsProcess*)pProcess;
   pProcess = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

//---------------------------------------------------------------------------
// ippSP initialization
//---------------------------------------------------------------------------

#if LIB_SP < LIB_NUM
#include "RunFirD.h" 
#include "RunRamp.h"
#include "RunConvCyclic.h"
#include "RunTone.h"
#include "RunTrian.h"
#include "RunRand.h"
#include "RunNormlze.h"
#include "RunNorm.h"
#include "RunStat.h"
#include "RunSamp.h"
#include "RunCorr.h"
#include "RunWt.h"
#include "RunWtHaar.h"
#include "RunLms.h"
#include "RunIir.h"
#include "RunFir.h"
#include "RunGoer.h"
#include "RunWin.h"
#include "RunFilter.h"
#include "RunThresh.h"
#include "RunFFT.h"
#include "RunDFT.h"
#include "RunDCT.h"
#include "RunDotprod.h"
#include "RunCnvrt.h"
#include "RunC.h"
#include "Run2.h"
#include "Run3.h"
#include "RunSumLn.h"
#include "RunLmsMR.h"
#include "RunFind.h"
#include "RunFindOne.h"
#include "RunUpdate.h"
#include "Run3S.h"
#include "RunFIRGen.h"
#include "RunHilbert.h"
#include "RunIirDirect.h"
#include "RunMinMax.h"
#include "RunMove.h"
#include "RunThreshLTGT.h"
#include "RunD2L.h"
#include "Run1S2.h"
#include "RunCopy1u.h"
#include "RunMin.h"
#include "RunScale2.h"
#include "RunConvBiased.h"
#include "RunFirSparse.h"
#include "RunIirSparse.h"

void CippsDirector::InitSP()
{
   AddTail(new CRunIirSparse); 
   AddTail(new CRunFirSparse); 
   AddTail(new CRunConvBiased); 
   AddTail(new CRunScale2); 
   AddTail(new CRunMin); 
   AddTail(new CRunCopy1u); 
   AddTail(new CRun1S2); 
   AddTail(new CRunThreshLTGT); 
   AddTail(new CRunMove); 
   AddTail(new CRunMinMax);
   AddTail(new CRunIirDirect);
   AddTail(new CRunHilbert);
   AddTail(new CRunFIRGen);
   AddTail(new CRun3S);
   AddTail(new CRunUpdate);
   AddTail(new CRunFindOne);
   AddTail(new CRunFind);
   AddTail(new CRunLmsMR);
   AddTail(new CRunLms);
   AddTail(new CRunFir);
   AddTail(new CRunFirD);
   AddTail(new CRunSumLn);
   AddTail(new CRunRamp);
   AddTail(new CRunConvCyclic);
   AddTail(new CRunTone);
   AddTail(new CRunTrian);
   AddTail(new CRunRand);
   AddTail(new CRunNormlze);
   AddTail(new CRunNorm);
   AddTail(new CRunStat);
   AddTail(new CRunWt);
   AddTail(new CRunWtHaar);
   AddTail(new CRunSamp);
   AddTail(new CRunCorr);
   AddTail(new CRunIir);
   AddTail(new CRunGoer);
   AddTail(new CRunWin);
   AddTail(new CRunFilter);
   AddTail(new CRunThresh);
   AddTail(new CRunFFT);
   AddTail(new CRunDFT);
   AddTail(new CRunDCT);
   AddTail(new CRunDotprod);
   AddTail(new CRunCnvrt);
   AddTail(new CRunC);
   AddTail(new CRun2);
   AddTail(new CRun3);
}
#else
void CippsDirector::InitSP() {}
#endif

//---------------------------------------------------------------------------
// ippCH initialization
//---------------------------------------------------------------------------

#if LIB_CH < LIB_NUM
#include "RunCh12.h"
#include "RunChConcat.h"
#include "RunChTrim.h"
#include "RunChInsert.h"
#include "RunChCompare.h"
#include "RunChReplace.h"
#include "RunChHash.h"
#include "RunChSplit.h"
#include "RunChFind.h"
#include "RunChFindC.h"
#include "RunChRegExp.h"

void CippsDirector::InitCH() 
{
    AddTail(new CRunChRegExp); 
    AddTail(new CRunChFindC); 
    AddTail(new CRunChFind); 
    AddTail(new CRunChSplit); 
    AddTail(new CRunChHash); 
    AddTail(new CRunChReplace); 
    AddTail(new CRunChCompare); 
    AddTail(new CRunChInsert); 
    AddTail(new CRunChTrim); 
    AddTail(new CRunChConcat); 
    AddTail(new CRunCh12); 
}
#else
void CippsDirector::InitCH() {}
#endif
