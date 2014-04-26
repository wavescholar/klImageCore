/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunC13.cpp: implementation of the CRunC13 class.
// CRunC13 class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunC13.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "ParmC13Dlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunC13::CRunC13()
{
   m_srcChannel = 0;
   m_dstChannel = 0;
}

CRunC13::~CRunC13()
{

}

CParamDlg* CRunC13::CreateDlg() { return new CParmC13Dlg;}

void CRunC13::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg,save);
    CParmC13Dlg* pDlg = (CParmC13Dlg*)parmDlg;
    if (save) {
        m_srcChannel = pDlg->m_srcChannel;
        m_dstChannel = pDlg->m_dstChannel;
    } else {
        pDlg->m_srcChannel = m_srcChannel;
        pDlg->m_dstChannel = m_dstChannel;
    }
}

IppStatus CRunC13::CallIppFunction()
{
   FUNC_CALL(ippiCopy_8u_C3C1R, ( (Ipp8u*)pSrc + m_srcChannel, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_8u_C1C3R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_8u_C4C1R, ( (Ipp8u*)pSrc + m_srcChannel, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_8u_C1C4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_8u_C3CR, ( (Ipp8u*)pSrc + m_srcChannel, srcStep, (Ipp8u*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_8u_C4CR, ( (Ipp8u*)pSrc + m_srcChannel, srcStep, (Ipp8u*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16u_C3C1R, ( (Ipp16u*)pSrc + m_srcChannel, srcStep, (Ipp16u*)pDst, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16u_C1C3R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16u_C4C1R, ( (Ipp16u*)pSrc + m_srcChannel, srcStep, (Ipp16u*)pDst, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16u_C1C4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16u_C3CR, ( (Ipp16u*)pSrc + m_srcChannel, srcStep, (Ipp16u*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16u_C4CR, ( (Ipp16u*)pSrc + m_srcChannel, srcStep, (Ipp16u*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16s_C3C1R, ( (Ipp16s*)pSrc + m_srcChannel, srcStep, (Ipp16s*)pDst, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16s_C1C3R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16s_C4C1R, ( (Ipp16s*)pSrc + m_srcChannel, srcStep, (Ipp16s*)pDst, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16s_C1C4R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16s_C3CR, ( (Ipp16s*)pSrc + m_srcChannel, srcStep, (Ipp16s*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_16s_C4CR, ( (Ipp16s*)pSrc + m_srcChannel, srcStep, (Ipp16s*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32s_C3C1R, ( (Ipp32s*)pSrc + m_srcChannel, srcStep, (Ipp32s*)pDst, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32s_C1C3R, ( (Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32s_C4C1R, ( (Ipp32s*)pSrc + m_srcChannel, srcStep, (Ipp32s*)pDst, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32s_C1C4R, ( (Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32s_C3CR, ( (Ipp32s*)pSrc + m_srcChannel, srcStep, (Ipp32s*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32s_C4CR, ( (Ipp32s*)pSrc + m_srcChannel, srcStep, (Ipp32s*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32f_C3C1R, ( (Ipp32f*)pSrc + m_srcChannel, srcStep, (Ipp32f*)pDst, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32f_C1C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32f_C4C1R, ( (Ipp32f*)pSrc + m_srcChannel, srcStep, (Ipp32f*)pDst, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32f_C1C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32f_C3CR, ( (Ipp32f*)pSrc + m_srcChannel, srcStep, (Ipp32f*)pDst + m_dstChannel, dstStep, roiSize ))
   FUNC_CALL(ippiCopy_32f_C4CR, ( (Ipp32f*)pSrc + m_srcChannel, srcStep, (Ipp32f*)pDst + m_dstChannel, dstStep, roiSize ))

   return stsNoFunction;
}

BOOL CRunC13::BeforeCall()
{
   return TRUE;
}

void CRunC13::Close()
{
}

CString CRunC13::GetHistoryParms()
{
    CMyString parms;
    parms << "src";
    if (m_pDocSrc->GetImage()->Channels() > 1)
       parms << " + " << m_srcChannel;
    parms << ", dst";
    if (m_pDocDst->GetImage()->Channels() > 1)
       parms << " + " << m_dstChannel;
    return parms;
}

