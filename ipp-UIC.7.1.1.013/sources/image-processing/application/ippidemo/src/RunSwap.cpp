/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSwap.cpp: implementation of the CRunSwap class.
// CRunSwap class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunSwap.h"
#include "ParmSwapDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunSwap::CRunSwap()
{
   m_dstOrder[0] = 2;
   m_dstOrder[1] = 1;
   m_dstOrder[2] = 0;
}

CRunSwap::~CRunSwap()
{

}

CParamDlg* CRunSwap::CreateDlg() { return new CParmSwapDlg;}

void CRunSwap::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmSwapDlg* pDlg = (CParmSwapDlg*)parmDlg;
    if (save) {
       for (int i=0; i<3; i++)
          m_dstOrder[i] = pDlg->m_dstOrder[i];
    } else {
       for (int i=0; i<3; i++)
          pDlg->m_dstOrder[i] = m_dstOrder[i];
    }
}

IppStatus CRunSwap::CallIppFunction()
{
   FUNC_CALL(ippiSwapChannels_8u_C3IR, ((Ipp8u*)pSrc, srcStep, roiSize, m_dstOrder))
   FUNC_CALL(ippiSwapChannels_8u_C3R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_dstOrder))
   FUNC_CALL(ippiSwapChannels_8u_AC4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_dstOrder))
   FUNC_CALL(ippiSwapChannels_16u_C3R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_dstOrder))
   FUNC_CALL(ippiSwapChannels_16u_AC4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_dstOrder))
   FUNC_CALL(ippiSwapChannels_16s_C3R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_dstOrder))
   FUNC_CALL(ippiSwapChannels_16s_AC4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_dstOrder))
   FUNC_CALL(ippiSwapChannels_32s_C3R, ((Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, m_dstOrder))
   FUNC_CALL(ippiSwapChannels_32s_AC4R, ((Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, m_dstOrder))
   FUNC_CALL(ippiSwapChannels_32f_C3R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_dstOrder))
   FUNC_CALL(ippiSwapChannels_32f_AC4R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_dstOrder))

   return stsNoFunction;
}

CString CRunSwap::GetHistoryParms()
{
   CMyString parms;
   return parms << "{"
                << m_dstOrder[0] << ", "
                << m_dstOrder[1] << ", "
                << m_dstOrder[2] << "}";
}
