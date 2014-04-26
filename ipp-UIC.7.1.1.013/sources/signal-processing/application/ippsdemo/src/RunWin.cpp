/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWin.cpp : implementation of the CRunWin class.
// CRunWin class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunWin.h"
#include "ParmFilterDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunWin::CRunWin()
{
   alpha.Init(pp32s,1024);
   alpha.Init(pp32f,0.03);
}

CRunWin::~CRunWin()
{

}

BOOL CRunWin::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;

   ppType type = func.Find("Q15") == -1 ? pp32f : pp32s;
   alpha.Init(type);
   return TRUE;
}

CParamDlg* CRunWin::CreateDlg() { 
   return new CParmFilterDlg;
}

void CRunWin::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmFilterDlg *pDlg = (CParmFilterDlg*)parmDlg;
   if (save) {
      alpha = pDlg->m_Val;
   } else {
      pDlg->m_Val = alpha;
      if (m_Func.Found("Q15"))
         pDlg->m_ValName = "AlphaQ15";
      else
         pDlg->m_ValName = "Alpha";
   }
}

IppStatus CRunWin::CallIppFunction()
{
   FUNC_CALL(ippsWinBlackmanQ15_16s_ISfs, ((Ipp16s*)pSrc, len, (int)alpha, scaleFactor))
   FUNC_CALL(ippsWinBlackmanQ15_16s_I, ((Ipp16s*)pSrc, len, (int)alpha))
   FUNC_CALL(ippsWinBlackmanQ15_16sc_I, ((Ipp16sc*)pSrc, len, (int)alpha))
   FUNC_CALL(ippsWinBlackman_16s_I, ((Ipp16s*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinBlackman_16sc_I, ((Ipp16sc*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinBlackman_32f_I, ((Ipp32f*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinBlackman_32fc_I, ((Ipp32fc*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinBlackman_64f_I, ((Ipp64f*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinBlackman_64fc_I, ((Ipp64fc*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinBlackmanQ15_16s, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, (int)alpha))
   FUNC_CALL(ippsWinBlackmanQ15_16sc, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, (int)alpha))
   FUNC_CALL(ippsWinBlackman_16s, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinBlackman_16sc, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinBlackman_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinBlackman_32fc, ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinBlackman_64f, ((Ipp64f*)pSrc, (Ipp64f*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinBlackman_64fc, ((Ipp64fc*)pSrc, (Ipp64fc*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinKaiser_16s,    ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinKaiser_16s_I,  ((Ipp16s*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinKaiserQ15_16s, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, (int)alpha))
   FUNC_CALL(ippsWinKaiserQ15_16s_I,((Ipp16s*)pSrc, len, (int)alpha))
   FUNC_CALL(ippsWinKaiser_16sc,   ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinKaiser_16sc_I, ((Ipp16sc*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinKaiserQ15_16sc,((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, (int)alpha))
   FUNC_CALL(ippsWinKaiserQ15_16sc_I,((Ipp16sc*)pSrc, len, (int)alpha))
   FUNC_CALL(ippsWinKaiser_32f,    ((Ipp32f*)pSrc, (Ipp32f*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinKaiser_32f_I,  ((Ipp32f*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinKaiser_32fc,   ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinKaiser_32fc_I, ((Ipp32fc*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinKaiser_64f,    ((Ipp64f*)pSrc, (Ipp64f*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinKaiser_64f_I,  ((Ipp64f*)pSrc, len, (float)alpha))
   FUNC_CALL(ippsWinKaiser_64fc,    ((Ipp64fc*)pSrc, (Ipp64fc*)pDst, len, (float)alpha))
   FUNC_CALL(ippsWinKaiser_64fc_I,  ((Ipp64fc*)pSrc, len, (float)alpha))

   return stsNoFunction;
}

CString CRunWin::GetHistoryParms()
{
   return alpha.String();
}

