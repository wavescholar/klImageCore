/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDFT.cpp: implementation of the CRunDFT class.
// CRunDFT class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunDFT.h"
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


CRunDFT::CRunDFT()
{
    m_hint = ippAlgHintFast;
    m_flag = IPP_FFT_DIV_BY_SQRTN;
    m_bufSize = 0;
    m_pBuffer = NULL;
    m_pSpec = NULL;
}

CRunDFT::~CRunDFT()
{

}

BOOL CRunDFT::Open(CFunc func)
{
    if (!CippiRun::Open(func)) return FALSE;
    CString sType = (m_Func.Found("CToC") ? "_C_" : "_R_");
    sType += m_Func.Found("32s") ? "32s" : m_Func.TypeName();
    m_initName = "ippiDFTInitAlloc" + sType;
    m_freeName = "ippiDFTFree" + sType;
    m_getBufSizeName = "ippiDFTGetBufSize" + sType;
    return TRUE;
}

void CRunDFT::Close()
{
}

CParamDlg* CRunDFT::CreateDlg() { return new CParmFFTDlg;}

void CRunDFT::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmFFTDlg* pDlg = (CParmFFTDlg*)parmDlg;
    if (save) {
        m_hint = (IppHintAlgorithm)pDlg->m_Hint;
        m_flag = CRunFFT::radioToFlag(pDlg->m_FlagRadio, m_Func.Found("Fwd"));
    } else {
        pDlg->m_Hint = (int)m_hint;
        pDlg->m_FlagRadio = CRunFFT::flagToRadio(m_flag);
    }
}

BOOL CRunDFT::BeforeCall()
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

BOOL CRunDFT::AfterCall(BOOL bOK)
{
    IppStatus status = CallFree(m_freeName); m_pSpec = NULL;
    IppErrorMessage(m_freeName, status);
    ippFree(m_pBuffer); m_pBuffer = NULL;
    return TRUE;
}

IppStatus CRunDFT::CallInit(CString name)
{
    MATE_CALL(name,ippiDFTInitAlloc_C_32fc,
                         ( (IppiDFTSpec_C_32fc**)&m_pSpec,
                            roiSize, m_flag, m_hint))
    MATE_CALL(name,ippiDFTInitAlloc_R_32f,
                         ( (IppiDFTSpec_R_32f**)&m_pSpec,
                            roiSize, m_flag, m_hint))
    MATE_CALL(name,ippiDFTInitAlloc_R_32s,
                         ( (IppiDFTSpec_R_32s**)&m_pSpec,
                            roiSize, m_flag, m_hint))
    return stsNoFunction;
}

IppStatus CRunDFT::CallFree(CString name)
{
    MATE_CALL(name,ippiDFTFree_C_32fc,( (IppiDFTSpec_C_32fc*)m_pSpec ))
    MATE_CALL(name,ippiDFTFree_R_32f, ( (IppiDFTSpec_R_32f*) m_pSpec ))
    MATE_CALL(name,ippiDFTFree_R_32s, ( (IppiDFTSpec_R_32s*) m_pSpec ))
    return stsNoFunction;
}

IppStatus CRunDFT::CallGetBufSize(CString name)
{
    MATE_CALL(name,ippiDFTGetBufSize_C_32fc,
                         ( (IppiDFTSpec_C_32fc*)m_pSpec, &m_bufSize ))
    MATE_CALL(name,ippiDFTGetBufSize_R_32f,
                         ( (IppiDFTSpec_R_32f*)m_pSpec, &m_bufSize ))
    MATE_CALL(name,ippiDFTGetBufSize_R_32s,
                         ( (IppiDFTSpec_R_32s*)m_pSpec, &m_bufSize ))
    return stsNoFunction;
}

IppStatus CRunDFT::CallIppFunction()
{
    FUNC_CALL(ippiDFTFwd_CToC_32fc_C1R, ( (Ipp32fc*)pSrc, srcStep, (Ipp32fc*)pDst, dstStep, (IppiDFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTInv_CToC_32fc_C1R, ( (Ipp32fc*)pSrc, srcStep, (Ipp32fc*)pDst, dstStep, (IppiDFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTFwd_RToPack_32f_C1R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTFwd_RToPack_32f_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTFwd_RToPack_32f_C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTFwd_RToPack_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTInv_PackToR_32f_C1R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTInv_PackToR_32f_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTInv_PackToR_32f_C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTInv_PackToR_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTFwd_RToPack_8u32s_C1RSfs, ( (Ipp8u*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, (IppiDFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTFwd_RToPack_8u32s_C3RSfs, ( (Ipp8u*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, (IppiDFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTFwd_RToPack_8u32s_C4RSfs, ( (Ipp8u*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, (IppiDFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTFwd_RToPack_8u32s_AC4RSfs, ( (Ipp8u*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, (IppiDFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTInv_PackToR_32s8u_C1RSfs, ( (Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, (IppiDFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTInv_PackToR_32s8u_C3RSfs, ( (Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, (IppiDFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTInv_PackToR_32s8u_C4RSfs, ( (Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, (IppiDFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))
    FUNC_CALL(ippiDFTInv_PackToR_32s8u_AC4RSfs, ( (Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, (IppiDFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_pBuffer ))

    FUNC_CALL ( ippiDFTFwd_CToC_32fc_C1IR, ((Ipp32fc*)pSrc, srcStep, (IppiDFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiDFTInv_CToC_32fc_C1IR, ((Ipp32fc*)pSrc, srcStep, (IppiDFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_pBuffer ))

    FUNC_CALL ( ippiDFTFwd_RToPack_32f_C1IR, ((Ipp32f*)pSrc, srcStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiDFTFwd_RToPack_32f_C3IR, ((Ipp32f*)pSrc, srcStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiDFTFwd_RToPack_32f_C4IR, ((Ipp32f*)pSrc, srcStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiDFTFwd_RToPack_32f_AC4IR, ((Ipp32f*)pSrc, srcStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))

    FUNC_CALL ( ippiDFTInv_PackToR_32f_C1IR, ((Ipp32f*)pSrc, srcStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiDFTInv_PackToR_32f_C3IR, ((Ipp32f*)pSrc, srcStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiDFTInv_PackToR_32f_C4IR, ((Ipp32f*)pSrc, srcStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
    FUNC_CALL ( ippiDFTInv_PackToR_32f_AC4IR, ((Ipp32f*)pSrc, srcStep, (IppiDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))

    return stsNoFunction;
}

CString CRunDFT::GetHistoryParms()
{
    CMyString parms;
    return parms << (EIppFlag)m_flag << ", " << m_hint;
}
