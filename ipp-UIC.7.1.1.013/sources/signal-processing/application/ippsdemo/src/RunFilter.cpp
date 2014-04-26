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
#include "RunFilter.h"
#include "ParmFilterDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParamDlg* CRunFilter::CreateDlg() { 
   CParmFilterDlg* pDlg = new CParmFilterDlg;
   pDlg->m_Val.Init(pp32s);
   return pDlg;
}

void CRunFilter::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmFilterDlg *pDlg = (CParmFilterDlg*)parmDlg;
   if (save) {
      pDlg->m_Val.Get(maskSize);
   } else {
      pDlg->m_Val.Set(maskSize);
   }
}

IppStatus CRunFilter::CallIppFunction()
{
   FUNC_CALL(ippsFilterMedian_32f_I,((Ipp32f*)pSrc,len,maskSize))
   FUNC_CALL(ippsFilterMedian_64f_I,((Ipp64f*)pSrc,len,maskSize))
   FUNC_CALL(ippsFilterMedian_16s_I,((Ipp16s*)pSrc,len,maskSize))
   FUNC_CALL(ippsFilterMedian_32s_I,((Ipp32s*)pSrc,len,maskSize))
   FUNC_CALL(ippsFilterMedian_8u_I,((Ipp8u*)pSrc,len,maskSize))
   FUNC_CALL(ippsFilterMedian_32f,((Ipp32f*)pSrc, (Ipp32f*)pDst,len, maskSize ))
   FUNC_CALL(ippsFilterMedian_64f,((Ipp64f*)pSrc, (Ipp64f*)pDst,len, maskSize ))
   FUNC_CALL(ippsFilterMedian_16s,((Ipp16s*)pSrc, (Ipp16s*)pDst,len, maskSize ))
   FUNC_CALL(ippsFilterMedian_32s,((Ipp32s*)pSrc, (Ipp32s*)pDst,len, maskSize ))
   FUNC_CALL(ippsFilterMedian_8u,((Ipp8u*)pSrc, (Ipp8u*)pDst, len, maskSize ))
   FUNC_CALL(ippsSumWindow_8u32f,((const Ipp8u*)pSrc,(Ipp32f*)pDst,len, maskSize))
   FUNC_CALL(ippsSumWindow_16s32f,((const Ipp16s*)pSrc,(Ipp32f*)pDst,len, maskSize))

   return stsNoFunction;
}

CString CRunFilter::GetHistoryParms()
{
   CMyString parm;
   parm << maskSize;
   return parm;
}

