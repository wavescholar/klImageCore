/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParamDlg.cpp : implementation of the CParamDlg class.
// CParamDlg class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunCnvrt.h"
#include "ParmCnvrtDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParamDlg* CRunCnvrt::CreateDlg() { 
   return new CParmCnvrtDlg;
}

void CRunCnvrt::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmCnvrtDlg *pDlg = (CParmCnvrtDlg*)parmDlg;
   if (save) {
      rndmode = (IppRoundMode)pDlg->m_Rnd;
   } else {
      pDlg->m_Rnd = (int)rndmode;
   }
}

IppStatus CRunCnvrt::CallIppFunction()
{
   FUNC_CALL(ippsConvert_32f8s_Sfs,((Ipp32f*)pSrc, (Ipp8s*)pDst, 
       len, rndmode, scaleFactor))
   FUNC_CALL(ippsConvert_32f8u_Sfs,((Ipp32f*)pSrc, (Ipp8u*)pDst, 
       len, rndmode, scaleFactor))
   FUNC_CALL(ippsConvert_32f16s_Sfs,((Ipp32f*)pSrc, (Ipp16s*)pDst, 
       len, rndmode, scaleFactor))
   FUNC_CALL(ippsConvert_32f16u_Sfs,((Ipp32f*)pSrc, (Ipp16u*)pDst, 
       len, rndmode, scaleFactor))
   FUNC_CALL(ippsConvert_64f32s_Sfs,((Ipp64f*)pSrc, (Ipp32s*)pDst, 
       len, rndmode, scaleFactor))
   FUNC_CALL(ippsConvert_32f32s_Sfs,((Ipp32f*)pSrc, (Ipp32s*)pDst, 
       len, rndmode, scaleFactor))
   FUNC_CALL(ippsConvert_16s16f,((Ipp16s*)pSrc,(Ipp16f*)pDst,len,rndmode))
   FUNC_CALL(ippsConvert_16f16s_Sfs,((Ipp16f*)pSrc,(Ipp16s*)pDst,len,rndmode,scaleFactor))
   FUNC_CALL(ippsConvert_32f16f,((Ipp32f*)pSrc,(Ipp16f*)pDst,len,rndmode))
   FUNC_CALL(ippsConvert_64s32s_Sfs,((Ipp64s*)pSrc,(Ipp32s*)pDst,len,rndmode,scaleFactor))

   return stsNoFunction;
}

CString CRunCnvrt::GetHistoryParms()
{
   CMyString parms;
   parms << rndmode;
   return parms;
}
