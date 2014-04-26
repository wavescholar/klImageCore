/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunAlphaColor.cpp: implementation of the CRunAlphaColor class.
// CRunAlphaColor class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CRunCompColor & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunAlphaColor.h"
#include "ParmAlphaColorDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRunAlphaColor::CRunAlphaColor()
{
    m_alphaType = ippAlphaOver;
    m_alpha1.Init(pp8u, 192);
    m_alpha1.Init(pp16u, 48000);
    m_alpha2.Init(pp8u, 64);
    m_alpha2.Init(pp16u, 16000);
}

CRunAlphaColor::~CRunAlphaColor()
{

}

BOOL CRunAlphaColor::Open(CFunc func)
{
    if (!CRunCompColor::Open(func)) return FALSE;
    m_alpha1.Init(func.SrcType());
    m_alpha2.Init(func.SrcType());
    return TRUE;
}

CParamDlg* CRunAlphaColor::CreateDlg() { return new CParmAlphaColorDlg;}

void CRunAlphaColor::UpdateData(CParamDlg* pParmDlg, BOOL save)
{
    CRunCompColor::UpdateData(pParmDlg, save);
    CParmAlphaColorDlg* pDlg = (CParmAlphaColorDlg*)pParmDlg;
    if (save) {
        m_alphaType = (IppiAlphaType)pDlg->m_alphaType;
        m_alpha1.Set(pDlg->m_alpha1Str);
        m_alpha2.Set(pDlg->m_alpha2Str);
    } else {
        pDlg->m_alphaType = m_alphaType;
        m_alpha1.Get(pDlg->m_alpha1Str);
        m_alpha2.Get(pDlg->m_alpha2Str);
    }
}

IppStatus CRunAlphaColor::CallIppFunction()
{
    FUNC_CALL ( ippiAlphaCompColorKey_8u_AC4R, (
        (Ipp8u*)pSrc, srcStep,(Ipp8u)m_alpha1,
        (Ipp8u*)pSrc2, srcStep2, (Ipp8u)m_alpha2,
        (Ipp8u*)pDst, dstStep,
        roiSize, (Ipp8u*)m_colorKey, m_alphaType ))
    return stsNoFunction;
}

CString CRunAlphaColor::GetHistoryParms()
{
    CMyString parms;
    return parms 
        << m_alpha1 << ", " 
        << m_alpha2 << ", " 
        << CRunCompColor::GetHistoryParms() << ", " 
        << m_alphaType;
}
