/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFFT.cpp: implementation of the CRunFFT class.
// CRunFFT class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunFFT.h"
#include "ParmFFTDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFFT::CRunFFT()
{
    m_hint = ippAlgHintFast;
    m_flag = IPP_FFT_DIV_BY_SQRTN;
    m_orderX = 0;
    m_orderY = 0;
    m_bufSize = 0;
    m_pBuffer = NULL;
    m_pSpec = NULL;
}

CRunFFT::~CRunFFT()
{

}

static int order(int x)
{
    if ( x <= 0) return -1;
    int y = 2;
    for (int i=0; ;i++) {
        if (y > x) return i;
        y <<= 1;
    }
}


BOOL CRunFFT::Open(CFunc func)
{
    if (!CippiRun::Open(func)) return FALSE;
    CString sType = (m_Func.Found("CToC") ? "_C_" : "_R_");
    sType += m_Func.Found("32s") ? "32s" : m_Func.TypeName();
    m_initName = "ippiFFTInitAlloc" + sType;
    m_freeName = "ippiFFTFree" + sType;
    m_getBufSizeName = "ippiFFTGetBufSize" + sType;

    IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
    m_orderX = order(roi.width);
    m_orderY = order(roi.height);
    return TRUE;
}

void CRunFFT::Close()
{
}

CParamDlg* CRunFFT::CreateDlg() { return new CParmFFTDlg;}

void CRunFFT::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmFFTDlg* pDlg = (CParmFFTDlg*)parmDlg;
    if (save) {
        m_hint = (IppHintAlgorithm)pDlg->m_Hint;
        m_flag = radioToFlag(pDlg->m_FlagRadio, m_Func.Found("Fwd"));
        m_orderX = pDlg->m_orderX;
        m_orderY = pDlg->m_orderY;
    } else {
        pDlg->m_Hint = (int)m_hint;
        pDlg->m_FlagRadio = flagToRadio(m_flag);
        pDlg->m_orderX = m_orderX;
        pDlg->m_orderY = m_orderY;
    }
}

BOOL CRunFFT::PrepareSrc()
{
    IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
    int width = 1 << m_orderX;
    int height = 1 << m_orderY;
    ASSERT((width <= roi.width) && (height <= roi.height));

    if ((width == roi.width) &&
         (height == roi.height)) return TRUE;
    roi.width = width;
    roi.height = height;
    if (DEMO_APP->MessageBox(
        (m_pDocSrc->GetImage()->GetRoi() ? " ROI will be updated in " :
                                      " ROI will be created in ") +
        m_pDocSrc->GetTitle(),
        MB_OKCANCEL) != IDOK) return FALSE;
    m_pDocSrc->GetImage()->SetRoi(&roi);
    return TRUE;
}

BOOL CRunFFT::BeforeCall()
{
    IppStatus status = CallInit(m_initName);
    IppErrorMessage(m_initName, status);
    if (status < 0) return FALSE;
    status = CallGetBufSize(m_getBufSizeName);
    IppErrorMessage(m_getBufSizeName, status);
    if (status < 0) return FALSE;
    if (m_bufSize) m_pBuffer = (Ipp8u*)ippMalloc(m_bufSize);
    return TRUE;
}

BOOL CRunFFT::AfterCall(BOOL bOK)
{
    IppStatus status = CallFree(m_freeName); m_pSpec = NULL;
    IppErrorMessage(m_freeName, status);
    ippFree(m_pBuffer); m_pBuffer = NULL;
    return TRUE;
}

IppStatus CRunFFT::CallInit(CString name)
{
    MATE_CALL(name,ippiFFTInitAlloc_C_32fc,
                         ( (IppiFFTSpec_C_32fc**)&m_pSpec,
                            m_orderX, m_orderY, m_flag, m_hint))
    MATE_CALL(name,ippiFFTInitAlloc_R_32f,
                         ( (IppiFFTSpec_R_32f**)&m_pSpec,
                            m_orderX, m_orderY, m_flag, m_hint))
    MATE_CALL(name,ippiFFTInitAlloc_R_32s,
                         ( (IppiFFTSpec_R_32s**)&m_pSpec,
                            m_orderX, m_orderY, m_flag, m_hint))
    return stsNoFunction;
}

IppStatus CRunFFT::CallFree(CString name)
{
    MATE_CALL(name,ippiFFTFree_C_32fc, ( (IppiFFTSpec_C_32fc*)m_pSpec ))
    MATE_CALL(name,ippiFFTFree_R_32f, ( (IppiFFTSpec_R_32f*)m_pSpec ))
    MATE_CALL(name,ippiFFTFree_R_32s, ( (IppiFFTSpec_R_32s*) m_pSpec ))
    return stsNoFunction;
}

IppStatus CRunFFT::CallGetBufSize(CString name)
{
    MATE_CALL(name,ippiFFTGetBufSize_C_32fc,
                         ( (IppiFFTSpec_C_32fc*)m_pSpec, &m_bufSize ))
    MATE_CALL(name,ippiFFTGetBufSize_R_32f,
                         ( (IppiFFTSpec_R_32f*)m_pSpec, &m_bufSize ))
    MATE_CALL(name,ippiFFTGetBufSize_R_32s,
                         ( (IppiFFTSpec_R_32s*)m_pSpec, &m_bufSize ))
    return stsNoFunction;
}

IppStatus CRunFFT::CallIppFunction()
{
    FUNC_CALL(ippiFFTFwd_CToC_32fc_C1R, ( (Ipp32fc*)pSrc, srcStep, (Ipp32fc*)pDst, dstStep, (IppiFFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTInv_CToC_32fc_C1R, ( (Ipp32fc*)pSrc, srcStep, (Ipp32fc*)pDst, dstStep, (IppiFFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTFwd_RToPack_32f_C1R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTFwd_RToPack_32f_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTFwd_RToPack_32f_C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTFwd_RToPack_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTInv_PackToR_32f_C1R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTInv_PackToR_32f_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTInv_PackToR_32f_C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTInv_PackToR_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTFwd_RToPack_8u32s_C1RSfs, ( (Ipp8u*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, (IppiFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTFwd_RToPack_8u32s_C3RSfs, ( (Ipp8u*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, (IppiFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTFwd_RToPack_8u32s_C4RSfs, ( (Ipp8u*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, (IppiFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTFwd_RToPack_8u32s_AC4RSfs, ( (Ipp8u*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, (IppiFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTInv_PackToR_32s8u_C1RSfs, ( (Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, (IppiFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTInv_PackToR_32s8u_C3RSfs, ( (Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, (IppiFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTInv_PackToR_32s8u_C4RSfs, ( (Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, (IppiFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiFFTInv_PackToR_32s8u_AC4RSfs, ( (Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, (IppiFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))

    FUNC_CALL ( ippiFFTFwd_CToC_32fc_C1IR, ((Ipp32fc*)pSrc, srcStep, (IppiFFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiFFTInv_CToC_32fc_C1IR, ((Ipp32fc*)pSrc, srcStep, (IppiFFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiFFTFwd_RToPack_32f_C1IR, ((Ipp32f*)pSrc, srcStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiFFTFwd_RToPack_32f_C3IR, ((Ipp32f*)pSrc, srcStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiFFTFwd_RToPack_32f_C4IR, ((Ipp32f*)pSrc, srcStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiFFTFwd_RToPack_32f_AC4IR, ((Ipp32f*)pSrc, srcStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiFFTInv_PackToR_32f_C1IR, ((Ipp32f*)pSrc, srcStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiFFTInv_PackToR_32f_C3IR, ((Ipp32f*)pSrc, srcStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiFFTInv_PackToR_32f_C4IR, ((Ipp32f*)pSrc, srcStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiFFTInv_PackToR_32f_AC4IR, ((Ipp32f*)pSrc, srcStep, (IppiFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))

    return stsNoFunction;
}

CString CRunFFT::GetHistoryParms()
{
    CMyString parms;
    return parms << (EIppFlag)m_flag << ", " << m_hint;
}

int CRunFFT::flagToRadio(int flag)
{
    switch (flag) {
    case IPP_FFT_DIV_FWD_BY_N:
    case IPP_FFT_DIV_INV_BY_N: return 0;
    case IPP_FFT_DIV_BY_SQRTN: return 1;
    case IPP_FFT_NODIV_BY_ANY: return 2;
    default: return -1;
    }
}

int CRunFFT::radioToFlag(int radio, BOOL bFwd)
{
    switch (radio) {
    case 0:
        if (bFwd)
            return IPP_FFT_DIV_FWD_BY_N;
        else
            return IPP_FFT_DIV_INV_BY_N;
    case 1: return IPP_FFT_DIV_BY_SQRTN;
    case 2: return IPP_FFT_NODIV_BY_ANY;
    default: return 0;
    }
}



