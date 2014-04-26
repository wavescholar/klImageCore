/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCopyBorder.cpp: implementation of the CRunCopyBorder class.
// CRunCopyBorder class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ippiDemoDoc.h"
#include "RunCopyBorder.h"
#include "ParmCopyBorderDlg.h"
#include "ParmValueDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

enum {BORDER_SIZE = 5};

CRunCopyBorder::CRunCopyBorder()
{
    m_aroundRoi = TRUE;
    for (int i=0; i<2; i++)
    {
        m_topLeft[i] = 3;
        m_bottomRight[i] = 3;
    }
    m_valueSave.Init(pp8u,  4);
    m_valueSave.Init(pp16s, 4);
    m_valueSave.Init(pp16u, 4);
    m_valueSave.Init(pp32s, 4);
    m_valueSave.Init(pp32f, 4);
    for (int i=0; i<4; i++) {
        m_valueSave.Vector(pp8u ).Set(i, 64);
        m_valueSave.Vector(pp16u).Set(i, 16000 );
        m_valueSave.Vector(pp16s).Set(i, 16000 );
        m_valueSave.Vector(pp32s).Set(i, 130000);
        m_valueSave.Vector(pp32f).Set(i, 0);
    }
}

CRunCopyBorder::~CRunCopyBorder()
{

}

BOOL CRunCopyBorder::Open(CFunc func)
{
    if (!CippiRun::Open(func)) return FALSE;

    m_valueSave.Init(func.SrcType());
    m_value = m_valueSave;
    m_value.SetLength(func.SrcChannels() - func.SrcAlpha());
    return TRUE;
}

void CRunCopyBorder::Close()
{
    m_valueSave = m_value;
    CippiRun::Close();
}

CParamDlg* CRunCopyBorder::CreateDlg()
{
    if (m_Func.Found("Const"))
        return new CParmValueDlg;
    else
        return new CParmCopyBorderDlg;
}

void CRunCopyBorder::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    if (m_Func.Found("Const"))
    {
        CParmValueDlg* pDlg = (CParmValueDlg*)parmDlg;
        if (save) {
            m_value = pDlg->m_value;
        } else {
            pDlg->m_value = m_value;
        }
    }
    else
    {
        CParmCopyBorderDlg* pDlg = (CParmCopyBorderDlg*)parmDlg;
        if (save) {
            m_aroundRoi = pDlg->m_aroundRoi;
            for (int i=0; i<2; i++)
            {
                m_topLeft    [i] = pDlg->m_tl[i];
                m_bottomRight[i] = pDlg->m_br[i];
            }
        } else {
            pDlg->m_aroundRoi = m_aroundRoi;
            for (int i=0; i<2; i++)
            {
                pDlg->m_tl[i] = m_topLeft    [i];
                pDlg->m_br[i] = m_bottomRight[i];
            }
        }
    }
}

ImgHeader CRunCopyBorder::GetNewDstHeader()
{
    CippiDemoDoc* pDoc = (CippiDemoDoc*)m_pDocSrc;
    ImgHeader header = CippiRun::GetNewDstHeader();
    header.SetSize(pDoc->Width() + m_topLeft[1] + m_bottomRight[1],
                   pDoc->Height() +  + m_topLeft[0] + m_bottomRight[0]);
    return header;
}

void CRunCopyBorder::CopyContoursToNewDst()
{
}

BOOL CRunCopyBorder::PrepareSrc()
{
    //if (m_Func.Inplace()) {
    //    CippiDemoDoc* pDoc = (CippiDemoDoc*)m_pDocSrc;
    //    if (!pDoc->GetRoi()) {
    //        IppiRect roi = {BORDER_SIZE, BORDER_SIZE,
    //                             pDoc->Width()  - BORDER_SIZE*2,
    //                             pDoc->Height() - BORDER_SIZE*2};
    //        pDoc->SetRoi(&roi);
    //    }
    //}
    return CippiRun::PrepareSrc();
}

BOOL CRunCopyBorder::BeforeCall()
{
    if (m_Func.Inplace())
    {
        if (!m_aroundRoi)
        {
            CImage* srcImage = m_pDocSrc->GetImage();
            IppiRect srcRoi = srcImage->GetActualRoi();
            IppiRect newRoi = {
                srcRoi.x + m_topLeft[1],
                srcRoi.y + m_topLeft[0],
                srcRoi.width  - m_topLeft[1] - m_bottomRight[1],
                srcRoi.height - m_topLeft[0] - m_bottomRight[0]
            };
            srcImage->SetRoi(&newRoi);
            pSrc = srcImage->GetRoiPtr();
            m_srcRoiSize = srcImage->GetRoiSize();
        }
    }
    else
    {
        if (m_aroundRoi)
        {
            pDst = pDstImg;
            m_dstRoiSize = m_dstSize;
        }
    }

    return TRUE;
}

IppStatus CRunCopyBorder::CallIppFunction()
{
    FUNC_CALL(ippiCopyReplicateBorder_8u_C1R, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, (Ipp8u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_8u_C3R, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, (Ipp8u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_8u_AC4R, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, (Ipp8u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_8u_C4R, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, (Ipp8u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16u_C1R, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, (Ipp16u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16u_C3R, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, (Ipp16u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16u_AC4R, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, (Ipp16u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16u_C4R, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, (Ipp16u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16s_C1R, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, (Ipp16s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16s_C3R, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, (Ipp16s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16s_AC4R, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, (Ipp16s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16s_C4R, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, (Ipp16s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32s_C1R, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, (Ipp32s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32s_C3R, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, (Ipp32s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32s_AC4R, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, (Ipp32s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32s_C4R, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, (Ipp32s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32f_C1R, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, (Ipp32f*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32f_C3R, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, (Ipp32f*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32f_AC4R, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, (Ipp32f*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32f_C4R, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, (Ipp32f*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_8u_C1IR, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_8u_C3IR, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_8u_AC4IR, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_8u_C4IR, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16u_C1IR, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16u_C3IR, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16u_AC4IR, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16u_C4IR, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16s_C1IR, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16s_C3IR, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16s_AC4IR, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_16s_C4IR, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32s_C1IR, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32s_C3IR, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32s_AC4IR, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32s_C4IR, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32f_C1IR, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32f_C3IR, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32f_AC4IR, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyReplicateBorder_32f_C4IR, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )

    FUNC_CALL(ippiCopyConstBorder_8u_C1R, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, (Ipp8u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp8u)m_value))
    FUNC_CALL(ippiCopyConstBorder_8u_C3R, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, (Ipp8u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp8u*)m_value))
    FUNC_CALL(ippiCopyConstBorder_8u_AC4R, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, (Ipp8u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp8u*)m_value))
    FUNC_CALL(ippiCopyConstBorder_8u_C4R, ((Ipp8u*)pSrc,  srcStep, m_srcRoiSize, (Ipp8u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp8u*)m_value))
    FUNC_CALL(ippiCopyConstBorder_16u_C1R, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, (Ipp16u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp16u)m_value))
    FUNC_CALL(ippiCopyConstBorder_16u_C3R, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, (Ipp16u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp16u*)m_value))
    FUNC_CALL(ippiCopyConstBorder_16u_AC4R, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, (Ipp16u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp16u*)m_value))
    FUNC_CALL(ippiCopyConstBorder_16u_C4R, ((Ipp16u*)pSrc,  srcStep, m_srcRoiSize, (Ipp16u*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp16u*)m_value))
    FUNC_CALL(ippiCopyConstBorder_16s_C1R, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, (Ipp16s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp16s)m_value))
    FUNC_CALL(ippiCopyConstBorder_16s_C3R, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, (Ipp16s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp16s*)m_value))
    FUNC_CALL(ippiCopyConstBorder_16s_AC4R, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, (Ipp16s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp16s*)m_value))
    FUNC_CALL(ippiCopyConstBorder_16s_C4R, ((Ipp16s*)pSrc,  srcStep, m_srcRoiSize, (Ipp16s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp16s*)m_value))
    FUNC_CALL(ippiCopyConstBorder_32s_C1R, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, (Ipp32s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp32s)m_value))
    FUNC_CALL(ippiCopyConstBorder_32s_C3R, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, (Ipp32s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp32s*)m_value))
    FUNC_CALL(ippiCopyConstBorder_32s_AC4R, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, (Ipp32s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp32s*)m_value))
    FUNC_CALL(ippiCopyConstBorder_32s_C4R, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, (Ipp32s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp32s*)m_value))
    FUNC_CALL(ippiCopyConstBorder_32f_C1R, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, (Ipp32f*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp32f)m_value))
    FUNC_CALL(ippiCopyConstBorder_32f_C3R, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, (Ipp32f*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp32f*)m_value))
    FUNC_CALL(ippiCopyConstBorder_32f_AC4R, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, (Ipp32f*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp32f*)m_value))
    FUNC_CALL(ippiCopyConstBorder_32f_C4R, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, (Ipp32f*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1], (Ipp32f*)m_value))

    FUNC_CALL(ippiCopyWrapBorder_32s_C1R, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, (Ipp32s*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyWrapBorder_32s_C1IR, ((Ipp32s*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyWrapBorder_32f_C1R, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, (Ipp32f*)pDst,  dstStep, m_dstRoiSize, m_topLeft[0], m_topLeft[1] ) )
    FUNC_CALL(ippiCopyWrapBorder_32f_C1IR, ((Ipp32f*)pSrc,  srcStep, m_srcRoiSize, m_srcSize, m_topLeft[0], m_topLeft[1] ) )
    return stsNoFunction;
}

CString CRunCopyBorder::GetHistoryParms()
{
    CMyString parms;
    CMyString valueParm;
    if (m_Func.Found("Const")) {
        m_value.Get(valueParm,", ");
        if (m_value.Length() > 1)
            valueParm = "{" + valueParm + "}";
        valueParm = ", " + valueParm;
    }
    parms << m_topLeft[0] << ", " << + m_topLeft[1] << valueParm;
    return parms;
}
