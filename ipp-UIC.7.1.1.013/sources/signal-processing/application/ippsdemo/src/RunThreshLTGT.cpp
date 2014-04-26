/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunThreshLTGT.cpp : implementation of the CRunThreshLTGT class.
// CRunThreshLTGT class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "RunThreshLTGT.h"
#include "ParmThreshLTGTDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunThreshLTGT::CRunThreshLTGT()
{
   level[0].Init(pp16s, -2000);
   value[0].Init(pp16s, -2000);
   level[1].Init(pp16s, 2000);
   value[1].Init(pp16s, 2000);

   level[0].Init(pp32f, -0.5);
   value[0].Init(pp32f, -0.5);
   level[1].Init(pp32f, 0.5);
   value[1].Init(pp32f, 0.5);

   level[0].Init(pp64f, -0.5);
   value[0].Init(pp64f, -0.5);
   level[1].Init(pp64f, 0.5);
   value[1].Init(pp64f, 0.5);
}

CRunThreshLTGT::~CRunThreshLTGT()
{

}

BOOL CRunThreshLTGT::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;

   int type = func.SrcType();
   for (int i=0; i<2; i++) {
      value[i].Init((ppType)type);
      level[i].Init((ppType)(type &= ~PP_CPLX));
   }
   return TRUE;
}

CParamDlg* CRunThreshLTGT::CreateDlg() { 
   return new CParmThreshLTGTDlg;
}

void CRunThreshLTGT::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmThreshLTGTDlg *pDlg = (CParmThreshLTGTDlg*)parmDlg;
   if (save) {
      for (int i=0; i<2; i++) {
         value[i].Set(pDlg->m_ValueStr[i]);      
         level[i].Set(pDlg->m_LevelStr[i]);      
      }
   } else {
      for (int i=0; i<2; i++) {
         value[i].Get(pDlg->m_ValueStr[i]);      
         level[i].Get(pDlg->m_LevelStr[i]);
      }
   }
}

IppStatus CRunThreshLTGT::CallIppFunction()
{
   FUNC_CALL(ippsThreshold_LTValGTVal_32f_I,( (Ipp32f*)pSrc, len,
          (Ipp32f)(level[0]), (Ipp32f)(value[0]), (Ipp32f)(level[1]), (Ipp32f)(value[1])))
   FUNC_CALL(ippsThreshold_LTValGTVal_64f_I,( (Ipp64f*)pSrc, len,
          (Ipp64f)(level[0]), (Ipp64f)(value[0]), (Ipp64f)(level[1]), (Ipp64f)(value[1])))
   FUNC_CALL(ippsThreshold_LTValGTVal_32f,((Ipp32f*)pSrc,
          (Ipp32f*)pDst, len, (Ipp32f)(level[0]), (Ipp32f)(value[0]), (Ipp32f)(level[1]),
          (Ipp32f)(value[1])))
   FUNC_CALL(ippsThreshold_LTValGTVal_64f,((Ipp64f*)pSrc,
          (Ipp64f*)pDst, len, (Ipp64f)(level[0]), (Ipp64f)(value[0]), (Ipp64f)(level[1]),
          (Ipp64f)(value[1])))
   FUNC_CALL(ippsThreshold_LTValGTVal_16s_I,( (Ipp16s*)pSrc, len,
          (Ipp16s)(level[0]), (Ipp16s)(value[0]), (Ipp16s)(level[1]), (Ipp16s)(value[1])))
   FUNC_CALL(ippsThreshold_LTValGTVal_16s,((Ipp16s*)pSrc,
          (Ipp16s*)pDst, len, (Ipp16s)(level[0]), (Ipp16s)(value[0]), (Ipp16s)(level[1]),
          (Ipp16s)(value[1])))
   FUNC_CALL(ippsThreshold_LTValGTVal_32s_I,( (Ipp32s*)pSrc, len,
          (Ipp32s)(level[0]), (Ipp32s)(value[0]), (Ipp32s)(level[1]), (Ipp32s)(value[1])))
   FUNC_CALL(ippsThreshold_LTValGTVal_32s,((Ipp32s*)pSrc,
          (Ipp32s*)pDst, len, (Ipp32s)(level[0]), (Ipp32s)(value[0]), (Ipp32s)(level[1]),
          (Ipp32s)(value[1])))
   return stsNoFunction;
}

CString CRunThreshLTGT::GetHistoryParms()
{
   CMyString parm;
   parm << level[0].String() << value[0].String()
        << level[1].String() << value[1].String();
   return parm;
}

