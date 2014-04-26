/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSuperDlg.cpp : implementation of the CParmSuperDlg class.
// CParmSuperDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmSuperDlg.h"


// CParmSuperDlg dialog

IMPLEMENT_DYNAMIC(CParmSuperDlg, CParamDlg)

CParmSuperDlg::CParmSuperDlg(UINT nID, CWnd* pParent)
   : CParamDlg(nID, pParent)
{

}

CParmSuperDlg::~CParmSuperDlg()
{
}

void CParmSuperDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DST_WIDTH, m_DstWidthEdit);
    DDX_Control(pDX, IDC_DST_HEIGHT, m_DstHeightEdit);
    DDX_Text(pDX, IDC_DST_WIDTH , m_dstSize.width );
    DDX_Text(pDX, IDC_DST_HEIGHT, m_dstSize.height);
    DDX_Text(pDX, IDC_SRC_WIDTH , m_srcSize.width );
    DDX_Text(pDX, IDC_SRC_HEIGHT, m_srcSize.height);
}


BEGIN_MESSAGE_MAP(CParmSuperDlg, CParamDlg)
END_MESSAGE_MAP()


// CParmSuperDlg message handlers

BOOL CParmSuperDlg::OnInitDialog()
{
    CParamDlg::OnInitDialog();

    IppiRect srcRoi = m_pDocSrc->GetImage()->GetActualRoi();
    m_srcSize.width  = srcRoi.width ; 
    m_srcSize.height = srcRoi.height; 
    if (m_pDocDst)
    {
        IppiRect dstRoi = m_pDocDst->GetImage()->GetActualRoi();
        m_dstSize.width  = dstRoi.width ; 
        m_dstSize.height = dstRoi.height; 
        m_DstWidthEdit.SetReadOnly();
        m_DstHeightEdit.SetReadOnly();
    }
    else
    {
        m_dstSize.width  = srcRoi.width  >> 1; 
        m_dstSize.height = srcRoi.height >> 1; 
    }

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
