/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmDeconvFFTDlg.cpp : implementation of the CParmDeconvFFTDlg class.
// CParmDeconvFFTDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmDeconvFFTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmDeconvFFTDlg dialog


CParmDeconvFFTDlg::CParmDeconvFFTDlg(UINT nID, CWnd* pParent /*=NULL*/)
    : CParamDlg(nID, pParent)
{
    //{{AFX_DATA_INIT(CParmDeconvFFTDlg)
    //}}AFX_DATA_INIT
}


void CParmDeconvFFTDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CParmDeconvFFTDlg)
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_ORDER_SPIN, orderSpin);
    DDX_Control(pDX, IDC_ORDER_STATIC, orderStatic);

    DDX_Text(pDX, IDC_KERNEL_SIZE, kernelSize);
    DDX_Text(pDX, IDC_ORDER, order);
    DDX_Text(pDX, IDC_THRESHOLD, threshold);
}


BEGIN_MESSAGE_MAP(CParmDeconvFFTDlg, CParamDlg)
    //{{AFX_MSG_MAP(CParmDeconvFFTDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmDeconvFFTDlg message handlers

BOOL CParmDeconvFFTDlg::OnInitDialog()
{
    CParamDlg::OnInitDialog();

    if (m_Func.Found("LR"))
    {
        orderStatic.SetWindowText("Num Iters");
        orderSpin.SetRange(1,IPP_MAX_16S);
    }
    else
    {
        orderStatic.SetWindowText("FFT Order");
        orderSpin.SetRange(order,31);
    }

    return TRUE;
}

void CParmDeconvFFTDlg::OnOK()
{
    CParamDlg::OnOK();
}
