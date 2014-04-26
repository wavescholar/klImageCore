/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDecimate.cpp: implementation of the CRunDecimate class.
// CRunDecimate class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "CProcess.h"
#include "RunDecimate.h"
#include "ParmDecimateDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunDecimate::CRunDecimate()
{
    m_fraction = ippPolyphase_1_2;
}

CRunDecimate::~CRunDecimate()
{

}

BOOL CRunDecimate::Open(CFunc func)
{
    if (!CippiRun::Open(func)) return FALSE;

    m_bRow = m_Func.Found("Row");

    return TRUE;
}

CParamDlg* CRunDecimate::CreateDlg() { return new CParmDecimateDlg;}

void CRunDecimate::UpdateData(CParamDlg* parmDlg, BOOL save)
{
     CippiRun::UpdateData(parmDlg, save);
     CParmDecimateDlg* pDlg = (CParmDecimateDlg*)parmDlg;
     if (save) {
          m_fraction = (IppiFraction)pDlg->m_fraction;
     } else {
          pDlg->m_fraction = (int)m_fraction;
     }
}

int CRunDecimate::up()
{
    switch (m_fraction)
    {
    case ippPolyphase_1_2 : return 1;
    case ippPolyphase_3_5 : return 3;
    case ippPolyphase_2_3 : return 2;
    case ippPolyphase_7_10: return 7;
    case ippPolyphase_3_4 : return 3;
    default               : return 1;
    }
}

int CRunDecimate::down()
{
    switch (m_fraction)
    {
    case ippPolyphase_1_2 : return 2 ;
    case ippPolyphase_3_5 : return 5 ;
    case ippPolyphase_2_3 : return 3 ;
    case ippPolyphase_7_10: return 10;
    case ippPolyphase_3_4 : return 4 ;
    default               : return 1 ;
    }
}

IppiSize CRunDecimate::GetDstSize(IppiSize srcSize)
{
    IppiSize dstSize = srcSize;
    if (m_bRow)
        dstSize.width = (srcSize.width*up())/down();
    else
        dstSize.height = (srcSize.height*up())/down();
    return dstSize;
}

IppiSize CRunDecimate::GetSrcSize(IppiSize dstSize)
{
    IppiSize srcSize = dstSize;
    if (m_bRow)
        srcSize.width = (dstSize.width*down())/up();
    else
        srcSize.height = (dstSize.height*down())/up();
    return srcSize;
}

ImgHeader CRunDecimate::GetNewDstHeader()
{
    ImgHeader header;
    header = m_pDocSrc->GetImage()->GetHeader();
    IppiSize srcSize = m_pDocSrc->GetImage()->GetActualSize();
    IppiSize dstSize = GetDstSize(srcSize);
    header.SetSize(dstSize.width, dstSize.height);
    return header;
}

BOOL CRunDecimate::PrepareSrc()
{
    IppiRect srcRoi = m_pDocSrc->GetImage()->GetActualRoi();
    if (m_pDocDst)
    {
        IppiSize dstSize = m_pDocDst->GetImage()->GetActualSize();
        IppiSize srcSize = GetSrcSize(dstSize);
        if (srcRoi.width > srcSize.width)
            srcRoi.width = srcSize.width;
        if (srcRoi.height > srcSize.height)
            srcRoi.height = srcSize.height;
        if (!SetReducedRoi(m_pDocSrc, srcRoi))
            return FALSE;
    }
    return TRUE;
}

IppStatus CRunDecimate::CallIppFunction()
{
    FUNC_CALL( ippiDecimateFilterRow_8u_C1R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pDst, dstStep, m_fraction))
    FUNC_CALL( ippiDecimateFilterColumn_8u_C1R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pDst, dstStep, m_fraction))

    return stsNoFunction;
}

CString CRunDecimate::GetHistoryParms()
{
    char* fractonStr[] = {
        "Polyphase_1_2",  "Polyphase_3_5",  "Polyphase_2_3", "Polyphase_7_10", "Polyphase_3_4",
    };
    CMyString parm;
    return parm << fractonStr[m_fraction];
}



