/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunResizeShift.cpp: implementation of the CRunResizeShift class.
// CRunResizeShift class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ippidemodoc.h"
#include "RunResizeShift.h"
#include "ParmResizeCDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunResizeShift::CRunResizeShift()
{
     m_ShiftType    = GEOM_CONTOUR;
     m_xShift = 10;
     m_yShift = 10;
     m_xFract = 1;
     m_yFract = 1;
}

BOOL  CRunResizeShift::Open(CFunc func)
{
    if (!CRunResize::Open(func)) 
        return FALSE;
    return TRUE;
}

CRunResizeShift::~CRunResizeShift()
{

}

CParamDlg* CRunResizeShift::CreateDlg() { return new CParmResizeCDlg;}

void CRunResizeShift::UpdateData(CParamDlg* parmDlg, BOOL save)
{
     CRunResize::UpdateData(parmDlg,save);
     CParmResizeCDlg* pDlg = (CParmResizeCDlg*)parmDlg;
     if (save) {
          m_ShiftType = pDlg->m_CenterType;
          m_xShift = pDlg->m_Center[0];
          m_yShift = pDlg->m_Center[1];
     } else {
          pDlg->m_CenterType = m_ShiftType  ;
          pDlg->m_Center[0] = m_xShift;
          pDlg->m_Center[1] = m_yShift;
     }
}

ImgHeader CRunResizeShift::GetNewDstHeader()
{
    ImgHeader header;
    header = m_pDocSrc->GetImage()->GetHeader();
    IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
    double xShift = 0;
    double yShift = 0;
    int w = (int)(roi.width  * m_xFactor + xShift);
    int h = (int)(roi.height * m_yFactor + yShift);
    header.SetSize(w, h);
    return header;
}

BOOL CRunResizeShift::PrepareSrc()
{
    BOOL update = FALSE;
    IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
    IppiSize size = m_pDocSrc->GetImage()->GetSize();
    if (roi.x + (int)(m_xShift - 1) < 0) {
        roi.x = -(int)(m_xShift - 1);
        update = TRUE;
    }
    if (roi.y + (int)(m_yShift - 1) < 0) {
        roi.y = -(int)(m_yShift - 1);
        update = TRUE;
    }
    if (roi.x + roi.width + (int)(m_xShift + 1) > size.width) {
        roi.width = size.width - roi.x - (int)(m_xShift + 1);
        if (roi.width <= 0) {
            roi.x = 0;
            roi.width = size.width - (int)(m_xShift + 1);
        }
        update = TRUE;
    }
    if (roi.y + roi.height + (int)(m_yShift + 1) > size.height) {
        roi.height = size.height - roi.y - (int)(m_yShift + 1);
        if (roi.height <= 0) {
            roi.y = 0;
            roi.height = size.height - (int)(m_yShift + 1);
        }
        update = TRUE;
    }
    if (update) {
        if (roi.width <= 0 || roi.height <= 0) {
            DEMO_APP->MessageBox(
                 " Shift of " + m_Func + " is too big",
                MB_OK | MB_ICONEXCLAMATION);
            return FALSE;
        }
        if (DEMO_APP->MessageBox(
            (m_pDocSrc->GetImage()->GetRoi() ?
             " ROI will be updated in " :
             " ROI will be created in ") +
            m_pDocSrc->GetTitle(),
            MB_OKCANCEL) != IDOK) return FALSE;
        m_pDocSrc->GetImage()->SetRoi(&roi);
    }

    if (m_ShiftType == GEOM_CONTOUR) return TRUE;

    double center[2] = {roi.x, roi.y};
    double shift[2] = {m_xShift, m_yShift};
    if (m_xShift < 0)
        center[0] += roi.width;
    if (m_yShift < 0)
        center[1] += roi.height;

    m_pDocSrc->GetImage()->SetCenterShift(center,shift);

    return TRUE;
}

BOOL CRunResizeShift::PrepareDst()
{
    if (!CRunResize::PrepareDst()) return FALSE;

    return TRUE;
}

void CRunResizeShift::PrepareParameters()
{
    CRunResize::PrepareParameters();
}

BOOL CRunResizeShift::BeforeCall()
{
    //CHECK_CALL(ippiGetResizeFract, (m_srcSize, m_srcROI, m_xFactor, m_yFactor,
    //                                          &m_xFract, &m_yFract, m_interpolate))
    return TRUE;
}

BOOL CRunResizeShift::AfterCall(BOOL bOk)
{
    return TRUE;
}

IppStatus CRunResizeShift::CallIppFunction()
{
    return stsNoFunction;
}

CString CRunResizeShift::GetHistoryParms()
{
     CMyString parms;
     parms << m_xFactor << ", "
           << m_yFactor << ", "
           << m_xShift << ", "
           << m_yShift << ", "
           << GetInterParm(m_interpolate);
     return parms;
}


