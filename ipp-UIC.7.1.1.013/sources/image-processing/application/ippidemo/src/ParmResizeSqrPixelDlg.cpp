/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmResizeSqrPixelDlg.cpp : implementation of the CParmResizeSqrPixelDlg class.
// CParmResizeSqrPixelDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ippiRun.h"
#include "ParmResizeSqrPixelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmResizeSqrPixelDlg dialog

enum {inter_nn, inter_linear, inter_cubic, inter_catmulrom, 
   inter_bspline, inter_b05c03, inter_lanczos, inter_super};


CParmResizeSqrPixelDlg::CParmResizeSqrPixelDlg(CWnd* pParent /*=NULL*/)
    : CParmResizeCDlg(CParmResizeSqrPixelDlg::IDD, pParent)
    , m_interSmooth(FALSE)
    , m_interSubpixel(FALSE)
    , m_interAntiAliasing(FALSE)
{
}


void CParmResizeSqrPixelDlg::DoDataExchange(CDataExchange* pDX)
{
    CParmResizeCDlg::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_INTER_SMOOTH, m_interSmooth);
    DDX_Check(pDX, IDC_INTER_SUBPIXEL, m_interSubpixel);
    DDX_Check(pDX, IDC_INTER_ANTI_ALIASING, m_interAntiAliasing);
    DDX_Control(pDX, IDC_INTER_ANTI_ALIASING, m_interAntiAliasing_Check);
}


BEGIN_MESSAGE_MAP(CParmResizeSqrPixelDlg, CParmResizeCDlg)
    ON_BN_CLICKED(IDC_INTER_0, OnBnClickedInter)
    ON_BN_CLICKED(IDC_INTER_1, OnBnClickedInter)
    ON_BN_CLICKED(IDC_INTER_2, OnBnClickedInter)
    ON_BN_CLICKED(IDC_INTER_3, OnBnClickedInter)
    ON_BN_CLICKED(IDC_INTER_4, OnBnClickedInter)
    ON_BN_CLICKED(IDC_INTER_5, OnBnClickedInter)
    ON_BN_CLICKED(IDC_INTER_6, OnBnClickedInter)
    ON_BN_CLICKED(IDC_INTER_7, OnBnClickedInter)
    ON_BN_CLICKED(IDC_INTER_SMOOTH, OnBnClickedInterSmooth)
    ON_BN_CLICKED(IDC_INTER_SUBPIXEL, OnBnClickedInterSubpixel)
    ON_BN_CLICKED(IDC_INTER_ANTI_ALIASING, OnBnClickedInterAntiAliasing)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmResizeSqrPixelDlg message handlers

BOOL CParmResizeSqrPixelDlg::OnInitDialog()
{
    CParmResizeCDlg::OnInitDialog();

    OnBnClickedInter();

    return TRUE;  
}

void CParmResizeSqrPixelDlg::UpdateMyData(BOOL save)
{
    if (save) {
         CParmResizeCDlg::UpdateMyData(save);
         UpdateInterpolation(save);
    } else {
         UpdateInterpolation(save);
         CParmResizeCDlg::UpdateMyData(save);
    }
}


void CParmResizeSqrPixelDlg::UpdateInterpolation(BOOL bUpdate)
{
    if (bUpdate)
    {
        m_interpolate = 0;
        switch (m_Inter)
        {
        case inter_nn       : m_interpolate = IPPI_INTER_NN                ; break;
        case inter_linear   : m_interpolate = IPPI_INTER_LINEAR            ; break;
        case inter_cubic    : m_interpolate = IPPI_INTER_CUBIC             ; break;
        case inter_catmulrom: m_interpolate = IPPI_INTER_CUBIC2P_CATMULLROM; break;
        case inter_bspline  : m_interpolate = IPPI_INTER_CUBIC2P_BSPLINE   ; break;
        case inter_b05c03   : m_interpolate = IPPI_INTER_CUBIC2P_B05C03    ; break;
        case inter_lanczos  : m_interpolate = IPPI_INTER_LANCZOS           ; break;
        case inter_super    : m_interpolate = IPPI_INTER_SUPER             ; break;
        }
        if (m_interSmooth      ) m_interpolate += IPPI_SMOOTH_EDGE  ;
        if (m_interSubpixel    ) m_interpolate += IPPI_SUBPIXEL_EDGE;
        if (m_interAntiAliasing) m_interpolate += IPPI_ANTIALIASING ;
    }
    else
    {
        int inter = m_interpolate & 0xFFFF;
        int flags = m_interpolate - inter;
        switch (inter)
        {
        case IPPI_INTER_NN                : m_Inter = inter_nn       ; break;
        case IPPI_INTER_LINEAR            : m_Inter = inter_linear   ; break;
        case IPPI_INTER_CUBIC             : m_Inter = inter_cubic    ; break;
        case IPPI_INTER_CUBIC2P_CATMULLROM: m_Inter = inter_catmulrom; break;
        case IPPI_INTER_CUBIC2P_BSPLINE   : m_Inter = inter_bspline  ; break;
        case IPPI_INTER_CUBIC2P_B05C03    : m_Inter = inter_b05c03   ; break;
        case IPPI_INTER_LANCZOS           : m_Inter = inter_lanczos  ; break;
        case IPPI_INTER_SUPER             : m_Inter = inter_super    ; break;
        }
        if (flags & IPPI_SMOOTH_EDGE  ) m_interSmooth       = TRUE;
        if (flags & IPPI_SUBPIXEL_EDGE) m_interSubpixel     = TRUE;
        if (flags & IPPI_ANTIALIASING ) m_interAntiAliasing = TRUE;
    }
}

void CParmResizeSqrPixelDlg::OnBnClickedInter()
{
    UpdateData();
    if ((m_Inter == inter_nn) || (m_Inter == inter_super))
    {
        m_interAntiAliasing_Check.EnableWindow(FALSE);
        m_interAntiAliasing = FALSE;
    }
    else
    {
        m_interAntiAliasing_Check.EnableWindow(TRUE);
    }
    UpdateData(FALSE);
}

void CParmResizeSqrPixelDlg::OnBnClickedInterSmooth()
{
    UpdateData();
    if (m_interSmooth)
    {
        m_interSubpixel = FALSE;
        m_interAntiAliasing = FALSE;
    }
    UpdateData(FALSE);
}

void CParmResizeSqrPixelDlg::OnBnClickedInterSubpixel()
{
    UpdateData();
    if (m_interSubpixel)
    {
        m_interAntiAliasing = FALSE;
        m_interSmooth = FALSE;
    }
    UpdateData(FALSE);
}

void CParmResizeSqrPixelDlg::OnBnClickedInterAntiAliasing()
{
    UpdateData();
    if (m_interAntiAliasing)
    {
        m_interSmooth = FALSE;
        m_interSubpixel = FALSE;
    }
    UpdateData(FALSE);
}
