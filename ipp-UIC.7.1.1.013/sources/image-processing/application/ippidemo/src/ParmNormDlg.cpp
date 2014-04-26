/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmNormDlg.cpp : implementation of the CParmNormDlg class.
// CParmNormDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmNormDlg.h"
#include "RunNorm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmNormDlg dialog

    enum {
        Norm_Inf, Norm_L1, Norm_L2,
        NormNum};


CParmNormDlg::CParmNormDlg(CRunNorm* pRun, UINT nID)
    : CParamDlg(nID, NULL)
{
    m_pRun = pRun;
    //{{AFX_DATA_INIT(CParmNormDlg)
    m_Hint = -1;
    m_Norm = -1;
    //}}AFX_DATA_INIT
    for (int i=0; i<4; i++) {
        m_Value[i] = 0;
    }
}


void CParmNormDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CParmNormDlg)
    DDX_Control(pDX, IDC_HINT_STATIC, m_HintStatic);
    DDX_Radio(pDX, IDC_HINT_0, m_Hint);
    DDX_Radio(pDX, IDC_NORM_0, m_Norm);
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_VAL_0, m_Value[0]);
    DDX_Text(pDX, IDC_VAL_1, m_Value[1]);
    DDX_Text(pDX, IDC_VAL_2, m_Value[2]);
    DDX_Text(pDX, IDC_VAL_3, m_Value[3]);

    DDX_Control(pDX, IDC_VAL_0, m_ValueEdit[0]);
    DDX_Control(pDX, IDC_VAL_1, m_ValueEdit[1]);
    DDX_Control(pDX, IDC_VAL_2, m_ValueEdit[2]);
    DDX_Control(pDX, IDC_VAL_3, m_ValueEdit[3]);

    DDX_Control(pDX, IDC_HINT_0, m_HintButton[0]);
    DDX_Control(pDX, IDC_HINT_1, m_HintButton[1]);
    DDX_Control(pDX, IDC_HINT_2, m_HintButton[2]);

    DDX_Control(pDX, IDC_NORM_0, m_NormButton[0]);
    DDX_Control(pDX, IDC_NORM_1, m_NormButton[1]);
    DDX_Control(pDX, IDC_NORM_2, m_NormButton[2]);
}


BEGIN_MESSAGE_MAP(CParmNormDlg, CParamDlg)
    //{{AFX_MSG_MAP(CParmNormDlg)
    ON_BN_CLICKED(IDC_HINT_0, OnHint)
    ON_BN_CLICKED(IDC_HINT_1, OnHint)
    ON_BN_CLICKED(IDC_HINT_2, OnHint)
    ON_BN_CLICKED(IDC_NORM_0, OnNorm)
    ON_BN_CLICKED(IDC_NORM_1, OnNorm)
    ON_BN_CLICKED(IDC_NORM_2, OnNorm)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmNormDlg message handlers

BOOL CParmNormDlg::OnInitDialog()
{
    CParamDlg::OnInitDialog();

    SetWindowText("Norms");
    InitNorm();
    SetFunc();
    EnableValues();
    EnableHint();
    m_pRun->SetNorm(this);
    UpdateData(FALSE);
    return TRUE;
}

void CParmNormDlg::OnOK()
{
    CParamDlg::OnOK();
}

#define NORM_INDEX(name) { \
    if (funcName.Find(#name) != -1) \
        return Norm##name; \
}

int CParmNormDlg::NormIndex(CString funcName)
{
    NORM_INDEX(_Inf);
    NORM_INDEX(_L1 );
    NORM_INDEX(_L2 );
    return -1;
}

#define NORM_FUNC(name) { \
    if (idx == Norm##name) \
        return "Norm" #name; \
}

CString CParmNormDlg::NormFunc(int idx)
{
    NORM_FUNC(_Inf);
    NORM_FUNC(_L1 );
    NORM_FUNC(_L2 );
    return "";
}

void CParmNormDlg::InitNorm()
{
    m_Norm = NormIndex(m_Func);
}

void CParmNormDlg::EnableValues()
{
    int numChannels = m_Func.SrcChannels() - m_Func.SrcAlpha();
    for (int i=numChannels; i < 4; i++)
        m_ValueEdit[i].ShowWindow(SW_HIDE);
}

void CParmNormDlg::EnableHint()
{
    BOOL flag = m_Func.Found("32f") &&
                    !m_Func.Found("Inf") &&
                    !m_Func.Found("Quality");
    m_HintStatic.EnableWindow(flag);
    for (int i=0; i<3; i++)
        m_HintButton[i].EnableWindow(flag);
}

void CParmNormDlg::SetFunc()
{
    CString baseName = NormFunc(m_Norm);
    CString typeName = m_Func.TypeToString(m_Func.SrcType());
    if (baseName == "QualityIndex" && typeName != "32f")
        typeName = typeName + "32f";
    m_Func = m_Func.Prefix()
             + baseName + "_"
             + typeName + "_"
             + m_Func.DescrName();
}

void CParmNormDlg::OnHint()
{
    UpdateData();
    m_pRun->SetNorm(this);
    UpdateData(FALSE);
}

void CParmNormDlg::OnNorm()
{
    UpdateData();
    SetFunc();
    EnableHint();
    m_pRun->SetNorm(this);
    UpdateData(FALSE);
}
