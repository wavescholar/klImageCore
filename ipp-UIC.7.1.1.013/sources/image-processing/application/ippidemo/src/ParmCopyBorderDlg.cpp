/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCopyBorderDlg.cpp : implementation of the CParmCopyBorderDlg class.
// CParmCopyBorderDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmCopyBorderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCopyBorderDlg dialog

CParmCopyBorderDlg::CParmCopyBorderDlg(CWnd* pParent /*=NULL*/)
    : CParamDlg(CParmCopyBorderDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CParmCopyBorderDlg)
    //}}AFX_DATA_INIT
}

void CParmCopyBorderDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CParmCopyBorderDlg)
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_BORDER_SPIN_0, m_borderSpin[0]);
    DDX_Control(pDX, IDC_BORDER_SPIN_1, m_borderSpin[1]);
    DDX_Control(pDX, IDC_BORDER_SPIN_2, m_borderSpin[2]);
    DDX_Control(pDX, IDC_BORDER_SPIN_3, m_borderSpin[3]);
    DDX_Control(pDX, IDC_BORDER_EDIT_0, m_borderEdit[0]);
    DDX_Control(pDX, IDC_BORDER_EDIT_1, m_borderEdit[1]);
    DDX_Control(pDX, IDC_BORDER_EDIT_2, m_borderEdit[2]);
    DDX_Control(pDX, IDC_BORDER_EDIT_3, m_borderEdit[3]);
    DDX_Text(pDX, IDC_BORDER_EDIT_0, m_borderStr[0]);
    DDX_Text(pDX, IDC_BORDER_EDIT_1, m_borderStr[1]);
    DDX_Text(pDX, IDC_BORDER_EDIT_2, m_borderStr[2]);
    DDX_Text(pDX, IDC_BORDER_EDIT_3, m_borderStr[3]);

    DDX_Control(pDX, IDC_AROUND, m_aroundButton);
    DDX_Check(pDX, IDC_AROUND, m_aroundRoi);
}


BEGIN_MESSAGE_MAP(CParmCopyBorderDlg, CParamDlg)
    ON_EN_CHANGE(IDC_BORDER_EDIT_0, OnChangeBorder)
    ON_EN_CHANGE(IDC_BORDER_EDIT_1, OnChangeBorder)
    ON_BN_CLICKED(IDC_AROUND, OnBnClickedAround)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCopyBorderDlg message handlers

BOOL CParmCopyBorderDlg::OnInitDialog()
{
    IppiSize srcSize = {0, 0}, dstSize = {0, 0};
    srcSize = m_pDocSrc->GetImage()->GetActualSize();
    m_bDst = m_pDocDst ? TRUE : FALSE;
    if (m_bDst)
        dstSize = m_pDocDst->GetImage()->GetActualSize();
    m_dhw[0] = dstSize.height;
    m_dhw[1] = dstSize.width;
    m_shw[0] = srcSize.height;
    m_shw[1] = srcSize.width;
    CImage* aroundImage = NULL;
    if (m_bDst)
        aroundImage = m_pDocDst->GetImage();
    else if (m_Func.Inplace())
        aroundImage = m_pDocSrc->GetImage();
    if (aroundImage)
    {
        const IppiRect* roi = aroundImage->GetRoi();
        if (roi)
        {
            m_ayx[0] = roi->y;
            m_ayx[1] = roi->x;
            m_a_y_x[0] = aroundImage->Height() - roi->height - roi->y;
            m_a_y_x[1] = aroundImage->Width () - roi->width  - roi->x;
        }
        else
            aroundImage = NULL; 
    }
    if (!aroundImage)
        m_aroundRoi = FALSE;

    UpdateBorders();
    CParamDlg::OnInitDialog();

    if (!aroundImage)
        m_aroundButton.EnableWindow(FALSE);
    SetSpin();
    EnableBorders();
    UpdateMyData(FALSE);

    return TRUE;
}

void CParmCopyBorderDlg::OnOK()
{
    CParamDlg::OnOK();
    UpdateMyData();
    UpdateBorders();
}

void CParmCopyBorderDlg::SetSpin()
{
    int i;
    for (i=0; i<4; i++)
        m_borderSpin[i].SetRange(0, 1000);
}

void CParmCopyBorderDlg::EnableBorders()
{
    int i;
    BOOL enable1 = !m_aroundRoi;
    BOOL enable2 = !m_aroundRoi && !m_bDst;
    for (i=0; i<2; i++)
    {
        m_borderSpin[i].EnableWindow(enable1);
        m_borderEdit[i].SetReadOnly(!enable1);
    }
    for (i=2; i<4; i++)
    {
        m_borderSpin[i].EnableWindow(enable2);
        m_borderEdit[i].SetReadOnly(!enable2);
    }
}

void CParmCopyBorderDlg::OnChangeBorder()
{
    if (!m_bInit) return;
    if (!m_bDst) return;
    UpdateMyData();
    UpdateBorders();
    UpdateMyData(FALSE);
}

void CParmCopyBorderDlg::OnBnClickedAround()
{
    UpdateMyData();
    UpdateBorders();
    UpdateMyData(FALSE);
    EnableBorders();
}

void CParmCopyBorderDlg::UpdateBorders()
{
    int i;
    if (m_aroundRoi)
    {
        for (i=0; i<2; i++)
        {
            m_tl[i] = m_ayx[i];
            m_br[i] = m_a_y_x[i];
        }
    }
    else
    {
        if (m_bDst)
        {
            for (i=0; i<2; i++)
            {
                m_br[i] = m_dhw[i] - m_shw[i] - m_tl[i];
                if (m_br[i] < 0)
                    m_br[i] = 0;
            }
        }
        else
        {
            for (i=0; i<2; i++)
                m_dhw[i] = m_shw[i] + m_tl[i] + m_br[i];
        }
    }
}

void CParmCopyBorderDlg::UpdateMyData(BOOL bSave)
{
    int i;
    if (bSave)
    {
        UpdateData();
        for (i=0; i<2; i++)
        {
            m_tl[i] = atoi(m_borderStr[i    ]);
            m_br[i] = atoi(m_borderStr[i + 2]);
        }
    }
    else
    {
        for (i=0; i<2; i++)
        {
            m_borderStr[i    ].Format("%d", m_tl[i]);
            m_borderStr[i + 2].Format("%d", m_br[i]);
        }
        UpdateData(FALSE);
    }
}
