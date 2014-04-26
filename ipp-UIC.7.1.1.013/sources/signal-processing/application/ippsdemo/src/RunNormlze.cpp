/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunNormlze.cpp : implementation of the CRunNormlze class.
// CRunNormlze class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunNormlze.h"
#include "ParmNormlzeDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunNormlze::CRunNormlze()
{   
   vdiv.Init(pp32s, 2);
   vdiv.Init(pp32f, 2);
   vdiv.Init(pp64f, 2);

   vsub.Init(pp16s, 0);
   vsub.Init(pp32f, 0);
   vsub.Init(pp64f, 0);
   vsub.Init(pp16sc, 0, 0);
   vsub.Init(pp32fc, 0, 0);
   vsub.Init(pp64fc, 0, 0);
}

BOOL CRunNormlze::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;

   int type = func.SrcType();
   vsub.Init((ppType)type);
   type &= ~PP_CPLX;
   if (!(type & PP_FLOAT)) type = pp32s;
   vdiv.Init((ppType)type);
   return TRUE;
}

CParamDlg* CRunNormlze::CreateDlg() { 
   return new CParmNormlzeDlg;
}

void CRunNormlze::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmNormlzeDlg *pDlg = (CParmNormlzeDlg*)parmDlg;
   if (save) {
      vsub.Set(pDlg->m_SubStr[0], pDlg->m_SubStr[1]);      
      vdiv.Set(pDlg->m_DivStr);      
   } else {
      vsub.Get(pDlg->m_SubStr[0], pDlg->m_SubStr[1]);      
      vdiv.Get(pDlg->m_DivStr);
   }
}

IppStatus CRunNormlze::CallIppFunction()
{
   FUNC_CALL(ippsNormalize_64fc,( (Ipp64fc*)pSrc, (Ipp64fc*)pDst, 
       len, (Ipp64fc)vsub, (Ipp64f)vdiv))
   FUNC_CALL(ippsNormalize_32fc,( (Ipp32fc*)pSrc, (Ipp32fc*)pDst, 
       len, (Ipp32fc)vsub, (Ipp32f)vdiv))
   FUNC_CALL(ippsNormalize_16sc_Sfs,( (Ipp16sc*)pSrc, (Ipp16sc*)pDst, 
       len, (Ipp16sc)vsub, (Ipp32s)vdiv, scaleFactor))
   FUNC_CALL(ippsNormalize_64f,( (Ipp64f*)pSrc, (Ipp64f*)pDst, len,
       (Ipp64f)vsub, (Ipp64f)vdiv))
   FUNC_CALL(ippsNormalize_32f,( (Ipp32f*)pSrc, (Ipp32f*)pDst, len,
       (Ipp32f)vsub, (Ipp32f)vdiv))
   FUNC_CALL(ippsNormalize_16s_Sfs,( (Ipp16s*)pSrc, (Ipp16s*)pDst, 
       len, (Ipp16s)vsub, (Ipp32s)vdiv, scaleFactor ))

   return stsNoFunction;
}

CString CRunNormlze::GetHistoryParms()
{
   CMyString parm;
   parm << vsub.String() << ", " << vdiv.String();
   return parm;
}

