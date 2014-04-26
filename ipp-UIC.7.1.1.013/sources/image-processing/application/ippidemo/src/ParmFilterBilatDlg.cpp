/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterBilatDlg.cpp : implementation of the CParmFilterBilatDlg class.
// CParmFilterBilatDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmFilterBilatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFilterBilatDlg dialog


CParmFilterBilatDlg::CParmFilterBilatDlg(UINT nID, CWnd* pParent /*=NULL*/)
    : CParamDlg(nID, pParent)
{
    //{{AFX_DATA_INIT(CParmFilterBilatDlg)
    //}}AFX_DATA_INIT
}


void CParmFilterBilatDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);

    DDX_Radio(pDX, IDC_FILTER_0, filter);
    DDX_Text(pDX, IDC_KERNEL_W, kernelSize.width);
    DDX_Text(pDX, IDC_KERNEL_H, kernelSize.height);
    DDX_Text(pDX, IDC_KERNEL_S, stepInKernel);
    DDX_Text(pDX, IDC_SIGMA_VAL, valSquareSigma);
    DDX_Text(pDX, IDC_SIGMA_POS, posSquareSigma);

    DDX_Control(pDX, IDC_KERNEL_W_SPIN, kernelSize_widthSpin );
    DDX_Control(pDX, IDC_KERNEL_H_SPIN, kernelSize_heightSpin);
    DDX_Control(pDX, IDC_KERNEL_S_SPIN, stepInKernelSpin     );
}


BEGIN_MESSAGE_MAP(CParmFilterBilatDlg, CParamDlg)
    //{{AFX_MSG_MAP(CParmFilterBilatDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFilterBilatDlg message handlers

BOOL CParmFilterBilatDlg::OnInitDialog()
{
    CParamDlg::OnInitDialog();

    kernelSize_widthSpin .SetRange(1,IPP_MAX_16S);
    kernelSize_heightSpin.SetRange(1,IPP_MAX_16S);
    stepInKernelSpin     .SetRange(1,IPP_MAX_16S);

    return TRUE;
}

void CParmFilterBilatDlg::OnOK()
{
    CParamDlg::OnOK();
}
