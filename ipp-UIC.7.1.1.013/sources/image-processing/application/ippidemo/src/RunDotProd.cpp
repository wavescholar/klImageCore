/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDotProd.cpp: implementation of the CRunDotProd class.
// CRunDotProd class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Timing.h"
#include "RunDotProd.h"
#include "ParmDotProdDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunDotProd::CRunDotProd()
{
    pDotProd = NULL;
    m_Num = 0;
    hint = ippAlgHintFast;
}

CRunDotProd::~CRunDotProd()
{

}

BOOL CRunDotProd::Open(CFunc func)
{
    if (!CippiRun::Open(func)) return FALSE;

    m_UsedVectors = VEC_SRC|VEC_SRC2;

    return TRUE;
}

void CRunDotProd::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg,save);
    CParmDotProdDlg *pDlg = (CParmDotProdDlg*)parmDlg;
    if (save) {
    } else {
        pDlg->m_Num = m_Num;
        for (int i=0; i<m_Num; i++)
            pDlg->m_DotProd[i] = m_DotProd[i];
    }
}

BOOL CRunDotProd::ProcessCall()
{
    BOOL result = TRUE;

    m_Num = m_pDocSrc->GetImage()->Channels() - m_Func.SrcAlpha();

    if (ByPlanes())
    {
        double perf = 0;
        for (int coi = 0; coi < m_Num; coi++) {
            SetCoiToDocs(coi);
            pDotProd = m_DotProd + coi;
            if (!CRun::ProcessCall()) {
                result = FALSE;
                break;
            }
            perf += m_pPerf->GetClocks();
        }
        m_pPerf->SetClocks(perf);
        SetCoiToDocs(-1);
    }
    else
    {
        pDotProd = m_DotProd;
        result = CRun::ProcessCall();
    }
    if (result)
    {
        CParmDotProdDlg dlg;
        UpdateData(&dlg,FALSE);
        dlg.DoModal();
    }
    return result;
}

IppStatus CRunDotProd::CallIppFunction()
{
    FUNC_CALL( ippiDotProd_8u64f_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_8s64f_C1R,((Ipp8s*)pSrc, srcStep, (Ipp8s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_16u64f_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_16s64f_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32u64f_C1R,((Ipp32u*)pSrc, srcStep, (Ipp32u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32s64f_C1R,((Ipp32s*)pSrc, srcStep, (Ipp32s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32f64f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, pDotProd, hint))

    FUNC_CALL( ippiDotProd_8u64f_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_8s64f_C3R,((Ipp8s*)pSrc, srcStep, (Ipp8s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_16u64f_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_16s64f_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32u64f_C3R,((Ipp32u*)pSrc, srcStep, (Ipp32u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32s64f_C3R,((Ipp32s*)pSrc, srcStep, (Ipp32s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32f64f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, pDotProd, hint))

    FUNC_CALL( ippiDotProd_8u64f_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_8s64f_C4R,((Ipp8s*)pSrc, srcStep, (Ipp8s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_16u64f_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_16s64f_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32u64f_C4R,((Ipp32u*)pSrc, srcStep, (Ipp32u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32s64f_C4R,((Ipp32s*)pSrc, srcStep, (Ipp32s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32f64f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, pDotProd, hint))

    FUNC_CALL( ippiDotProd_8u64f_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_8s64f_AC4R,((Ipp8s*)pSrc, srcStep, (Ipp8s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_16u64f_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_16s64f_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32u64f_AC4R,((Ipp32u*)pSrc, srcStep, (Ipp32u*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32s64f_AC4R,((Ipp32s*)pSrc, srcStep, (Ipp32s*)pSrc2, srcStep2, roiSize, pDotProd))
    FUNC_CALL( ippiDotProd_32f64f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, pDotProd, hint))

    return stsNoFunction;
}
