/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCompColor.cpp: implementation of the CRunCompColor class.
// CRunCompColor class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunCompColor.h"
#include "ParmCompColorDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCompColor::CRunCompColor()
{
   m_colorKey.Init(pp8u,  4);
   m_colorKey.Init(pp16u, 4);
   m_colorKey.Init(pp16s, 4);

   for (int i=0; i<4; i++) {
      m_colorKey.Vector(pp8u ).Set(i, 64);
      m_colorKey.Vector(pp16u).Set(i, 16000 );
      m_colorKey.Vector(pp16s).Set(i, 16000 );
   }

}

CRunCompColor::~CRunCompColor()
{

}

BOOL CRunCompColor::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;

   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;

   m_colorKey.Init(func.SrcType());
   m_colorKey.Vector().SetLength(func.SrcChannels());
   return TRUE;
}

CParamDlg* CRunCompColor::CreateDlg() { return new CParmCompColorDlg;}

void CRunCompColor::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmCompColorDlg* pDlg = (CParmCompColorDlg*)parmDlg;
    if (save) {
        m_colorKey.Set(pDlg->m_colorKeyStr); 
    } else {
        m_colorKey.Get(pDlg->m_colorKeyStr); 
    }
}

IppStatus CRunCompColor::CallIppFunction()
{
    FUNC_CALL ( ippiCompColorKey_8u_C1R, ((Ipp8u*)pSrc, srcStep,
            (Ipp8u*)pSrc2, srcStep2,
             (Ipp8u*)pDst, dstStep,
            roiSize, ((Ipp8u*)m_colorKey)[0]))

    FUNC_CALL ( ippiCompColorKey_8u_C3R, ((Ipp8u*)pSrc, srcStep,
            (Ipp8u*)pSrc2, srcStep2,
             (Ipp8u*)pDst, dstStep,
            roiSize, (Ipp8u*)m_colorKey))

    FUNC_CALL ( ippiCompColorKey_8u_C4R, ((Ipp8u*)pSrc, srcStep,
            (Ipp8u*)pSrc2, srcStep2,
             (Ipp8u*)pDst, dstStep,
            roiSize, (Ipp8u*)m_colorKey))

    FUNC_CALL ( ippiCompColorKey_16u_C1R, ((Ipp16u*)pSrc, srcStep,
            (Ipp16u*)pSrc2, srcStep2,
             (Ipp16u*)pDst, dstStep,
            roiSize, ((Ipp16u*)m_colorKey)[0]))

    FUNC_CALL ( ippiCompColorKey_16u_C3R, ((Ipp16u*)pSrc, srcStep,
            (Ipp16u*)pSrc2, srcStep2,
              (Ipp16u*)pDst, dstStep,
            roiSize, (Ipp16u*)m_colorKey))

    FUNC_CALL ( ippiCompColorKey_16u_C4R, ((Ipp16u*)pSrc, srcStep,
            (Ipp16u*)pSrc2, srcStep2,
             (Ipp16u*)pDst, dstStep,
            roiSize, (Ipp16u*)m_colorKey))

    FUNC_CALL ( ippiCompColorKey_16s_C1R, ((Ipp16s*)pSrc, srcStep,
            (Ipp16s*)pSrc2, srcStep2,
             (Ipp16s*)pDst, dstStep,
            roiSize, ((Ipp16s*)m_colorKey)[0]))

    FUNC_CALL ( ippiCompColorKey_16s_C3R, ((Ipp16s*)pSrc, srcStep,
            (Ipp16s*)pSrc2, srcStep2,
             (Ipp16s*)pDst, dstStep,
            roiSize, (Ipp16s*)m_colorKey))

    FUNC_CALL ( ippiCompColorKey_16s_C4R, ((Ipp16s*)pSrc, srcStep,
            (Ipp16s*)pSrc2, srcStep2,
             (Ipp16s*)pDst, dstStep,
            roiSize, (Ipp16s*)m_colorKey))

   return stsNoFunction;
}

CString CRunCompColor::GetHistoryParms()
{
   CMyString parms;
   m_colorKey.Get(parms,", ");
   if (m_colorKey.Length() > 1)
      parms = "{" + parms + "}";
   return parms;
}
