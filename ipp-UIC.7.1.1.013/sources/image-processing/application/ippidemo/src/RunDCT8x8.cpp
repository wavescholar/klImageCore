/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDCT8x8.cpp: implementation of the CRunDCT8x8 class.
// CRunDCT8x8 class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunDCT8x8.h"
#include "ParmImg2Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunDCT8x8::CRunDCT8x8()
{
    m_pSrc = NULL;
    m_pDst = NULL;
    m_srcBuffer = NULL;
    m_dstBuffer = NULL;
}

CRunDCT8x8::~CRunDCT8x8()
{

}

CParamDlg* CRunDCT8x8::CreateDlg() {
    return new CParmImg2Dlg;
}

static void Copy8(char* src, int srcStep, char* dst, int dstStep, int len)
{
    for (int i=0; i<8; i++) {
        memcpy(dst,src,len);
        src += srcStep;
        dst += dstStep;
    }
}

void CRunDCT8x8::CopyBuffer(CImage* pImg, char* buffer, BOOL toBuffer)
{
    int nx = GetBlockNX();
    int ny = GetBlockNY();
    int step = GetBlockStep();
    char* imgPtr = pImg->GetRoiPtr();
    char* bufPtr = buffer;
    for (int y=0; y<ny; y++) {
        for (int x=0; x<nx; x++) {
            if (toBuffer)
                Copy8(imgPtr + x*step, pImg->Step(), bufPtr, step, step);
            else
                Copy8(bufPtr, step, imgPtr + x*step, pImg->Step(), step);
            bufPtr += step << 3;
        }
        imgPtr += pImg->Step() << 3;
    }
}

int CRunDCT8x8::GetBlockNX() { return roiSize.width >> 3;}
int CRunDCT8x8::GetBlockNY() { return roiSize.height >> 3;}
int CRunDCT8x8::GetBlockN() { return GetBlockNY()*GetBlockNX();}
int CRunDCT8x8::GetBlockSqr() { return GetBlockStep()<<3;}
int CRunDCT8x8::GetBlockStep() { return m_pDocSrc->GetImage()->Type() & PP_MASK;}
int CRunDCT8x8::GetBufferSize() { return GetBlockSqr()*GetBlockN();}

BOOL CRunDCT8x8::BeforeCall()
{
    m_srcBuffer = (char*)ippMalloc(GetBufferSize());
    if (!m_srcBuffer) return FALSE;
    if (!m_Func.Inplace()) {
        m_dstBuffer = (char*)ippMalloc(GetBufferSize());
        if (!m_dstBuffer) return FALSE;
    }
    return TRUE;
}

BOOL CRunDCT8x8::AfterCall(BOOL bOK)
{
    ippFree(m_srcBuffer); m_srcBuffer = NULL;
    if (!m_Func.Inplace()) {
        ippFree(m_dstBuffer); m_dstBuffer = NULL;
    }
    return TRUE;
}

BOOL CRunDCT8x8::CallIpp(BOOL bMessage)
{
    BOOL result = FALSE;
    CopyBuffer(m_pDocSrc->GetImage(), m_srcBuffer, TRUE);
    int n = GetBlockN();
    int step = GetBlockSqr();
    m_pSrc = m_srcBuffer;
    m_pDst = m_dstBuffer;
    for (int i=0; i<n; i++) {
        result = CippiRun::CallIpp(bMessage);
        if (result == FALSE) break;
        m_pSrc += step;
        m_pDst += step;
    }
    if (result) {
        if (m_Func.Inplace())
            CopyBuffer(m_pDocSrc->GetImage(), m_srcBuffer, FALSE);
        else
            CopyBuffer(m_pDocDst->GetImage(), m_dstBuffer, FALSE);
    }
    return result;
}

void CRunDCT8x8::Loop(int num)
{
    while (num--) {
        int n = GetBlockN();
        int step = GetBlockSqr();
        m_pSrc = m_srcBuffer;
        m_pDst = m_dstBuffer;
        for (int i=0; i<n; i++) {
            CallIppFunction();
            m_pSrc += step;
            m_pDst += step;
        }
    }
}

double CRunDCT8x8::GetPerfDivisor()
{
    return GetBlockN() << 6;
}

IppStatus CRunDCT8x8::CallIppFunction()
{
    FUNC_CALL(ippiDCT8x8Fwd_16s_C1, ((Ipp16s*)m_pSrc, (Ipp16s*)m_pDst ))
    FUNC_CALL(ippiDCT8x8Inv_16s_C1, ((Ipp16s*)m_pSrc, (Ipp16s*)m_pDst ))
    FUNC_CALL(ippiDCT8x8Fwd_32f_C1, ((Ipp32f*)m_pSrc, (Ipp32f*)m_pDst ))
    FUNC_CALL(ippiDCT8x8Inv_32f_C1, ((Ipp32f*)m_pSrc, (Ipp32f*)m_pDst ))
    FUNC_CALL(ippiDCT8x8Fwd_16s_C1I,((Ipp16s*)m_pSrc))
    FUNC_CALL(ippiDCT8x8Inv_16s_C1I,((Ipp16s*)m_pSrc))
    FUNC_CALL(ippiDCT8x8Fwd_32f_C1I,((Ipp32f*)m_pSrc))
    FUNC_CALL(ippiDCT8x8Inv_32f_C1I,((Ipp32f*)m_pSrc))

    FUNC_CALL ( ippiDCT8x8Inv_A10_16s_C1, ((Ipp16s*)pSrc, (Ipp16s*)pDst ))
    FUNC_CALL ( ippiDCT8x8Inv_A10_16s_C1I, ((Ipp16s*)m_pSrc ))

    FUNC_CALL(ippiDCT8x8Inv_2x2_16s_C1,  ((Ipp16s*)m_pSrc, (Ipp16s*)m_pDst ))
    FUNC_CALL(ippiDCT8x8Inv_4x4_16s_C1,  ((Ipp16s*)m_pSrc, (Ipp16s*)m_pDst ))
    FUNC_CALL(ippiDCT8x8Inv_2x2_16s_C1I, ((Ipp16s*)m_pSrc ))
    FUNC_CALL(ippiDCT8x8Inv_4x4_16s_C1I, ((Ipp16s*)m_pSrc ))

    FUNC_CALL ( ippiDCT8x8To2x2Inv_16s_C1, ((Ipp16s*)pSrc, (Ipp16s*)pDst ))
    FUNC_CALL ( ippiDCT8x8To4x4Inv_16s_C1, ((Ipp16s*)pSrc, (Ipp16s*)pDst ))
    FUNC_CALL ( ippiDCT8x8To2x2Inv_16s_C1I, ((Ipp16s*)m_pSrc ))
    FUNC_CALL ( ippiDCT8x8To4x4Inv_16s_C1I, ((Ipp16s*)m_pSrc ))

    FUNC_CALL(ippiZigzagFwd8x8_16s_C1,((Ipp16s*)m_pSrc, (Ipp16s*)m_pDst))
    FUNC_CALL(ippiZigzagInv8x8_16s_C1,((Ipp16s*)m_pSrc, (Ipp16s*)m_pDst))


return stsNoFunction;
}

