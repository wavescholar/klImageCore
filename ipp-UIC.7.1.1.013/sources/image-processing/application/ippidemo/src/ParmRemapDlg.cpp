/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRemapDlg.cpp : implementation of the CParmRemapDlg class.
// CParmRemapDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmRemapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmRemapDlg dialog
enum {inter_nn, inter_linear, inter_cubic, inter_catmullrom, inter_lanczos}; 

CParmRemapDlg::CParmRemapDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmRemapDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmRemapDlg)
   m_Inter = -1;
   m_Smooth = FALSE;
   m_Type = -1;
   m_ShowMaps = FALSE;
   //}}AFX_DATA_INIT
    m_pMapPtrArray = NULL;
    m_pData = &m_MapDummy;
}


void CParmRemapDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CParmRemapDlg)
    DDX_Radio(pDX, IDC_INTER_0, m_Inter);
    DDX_Check(pDX, IDC_SMOOTH, m_Smooth);
    DDX_Radio(pDX, IDC_TYPE_0, m_Type);
    DDX_Check(pDX, IDC_SHOW_MAP, m_ShowMaps);
    //}}AFX_DATA_MAP
    DDX_Radio(pDX, IDC_RAD_0, m_pData->m_Radio);
    DDX_Text(pDX, IDC_TEXT_0, m_pData->m_TextName[0]);
    DDX_Text(pDX, IDC_TEXT_1, m_pData->m_TextName[1]);
    DDX_Text(pDX, IDC_TEXT_2, m_pData->m_TextName[2]);
    DDX_Text(pDX, IDC_TEXT_3, m_pData->m_TextName[3]);
    DDX_Text(pDX, IDC_TEXT_4, m_pData->m_TextName[4]);
    DDX_Text(pDX, IDC_TEXT_5, m_pData->m_TextName[5]);
    DDX_Text(pDX, IDC_EDIT_0, m_pData->m_Text[0]);
    DDX_Text(pDX, IDC_EDIT_1, m_pData->m_Text[1]);
    DDX_Text(pDX, IDC_EDIT_2, m_pData->m_Text[2]);
    DDX_Text(pDX, IDC_EDIT_3, m_pData->m_Text[3]);
    DDX_Text(pDX, IDC_EDIT_4, m_pData->m_Text[4]);
    DDX_Text(pDX, IDC_EDIT_5, m_pData->m_Text[5]);

    DDX_Control(pDX, IDC_EDIT_0, m_TextEdit[0]);
    DDX_Control(pDX, IDC_EDIT_1, m_TextEdit[1]);
    DDX_Control(pDX, IDC_EDIT_2, m_TextEdit[2]);
    DDX_Control(pDX, IDC_EDIT_3, m_TextEdit[3]);
    DDX_Control(pDX, IDC_EDIT_4, m_TextEdit[4]);
    DDX_Control(pDX, IDC_EDIT_5, m_TextEdit[5]);
    DDX_Control(pDX, IDC_TEXT_0, m_NameStatic[0]);
    DDX_Control(pDX, IDC_TEXT_1, m_NameStatic[1]);
    DDX_Control(pDX, IDC_TEXT_2, m_NameStatic[2]);
    DDX_Control(pDX, IDC_TEXT_3, m_NameStatic[3]);
    DDX_Control(pDX, IDC_TEXT_4, m_NameStatic[4]);
    DDX_Control(pDX, IDC_TEXT_5, m_NameStatic[5]);

    DDX_Control(pDX, IDC_RAD_0, m_RadioButton[0]);
    DDX_Control(pDX, IDC_RAD_1, m_RadioButton[1]);
    DDX_Control(pDX, IDC_RAD_2, m_RadioButton[2]);
    DDX_Control(pDX, IDC_RAD_3, m_RadioButton[3]);

    DDX_Control(pDX, IDC_TYPE_0, m_TypeButton[0]);
    DDX_Control(pDX, IDC_TYPE_1, m_TypeButton[1]);
    DDX_Control(pDX, IDC_TYPE_2, m_TypeButton[2]);
    DDX_Control(pDX, IDC_TYPE_3, m_TypeButton[3]);
    DDX_Control(pDX, IDC_TYPE_4, m_TypeButton[4]);
    DDX_Control(pDX, IDC_TYPE_5, m_TypeButton[5]);
    DDX_Control(pDX, IDC_TYPE_6, m_TypeButton[6]);
}


BEGIN_MESSAGE_MAP(CParmRemapDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmRemapDlg)
   ON_BN_CLICKED(IDC_TYPE_0, OnType)
   ON_BN_CLICKED(IDC_TYPE_1, OnType)
   ON_BN_CLICKED(IDC_TYPE_2, OnType)
   ON_BN_CLICKED(IDC_TYPE_3, OnType)
   ON_BN_CLICKED(IDC_TYPE_4, OnType)
   ON_BN_CLICKED(IDC_TYPE_5, OnType)
   ON_BN_CLICKED(IDC_TYPE_6, OnType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmRemapDlg message handlers

void CParmRemapDlg::Init(CMapPtrArray* pArray)
{
    m_pMapPtrArray = pArray;
    m_Maps = *pArray;
}

BOOL CParmRemapDlg::OnInitDialog()
{
    ASSERT(m_pMapPtrArray);
    ASSERT(m_Type >= 0);
    if (m_pDocDst == NULL) m_pDocDst = m_pDocSrc;
    UpdateInterpolation(FALSE);
    CParamDlg::OnInitDialog();
    InitType();
    UpdateType();
    ShowOptions();
    UpdateQuadOptions();
    UpdateData(FALSE);
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmRemapDlg::OnOK()
{
   CParamDlg::OnOK();
    *m_pMapPtrArray->ElementAt(m_Type) = *m_pData;
    UpdateInterpolation();
}

void CParmRemapDlg::UpdateInterpolation(BOOL bUpdate)
{
    if (bUpdate)
    {
        m_interpolate = 0;
        switch (m_Inter)
        {
        case inter_nn        : m_interpolate = IPPI_INTER_NN                ; break;
        case inter_linear    : m_interpolate = IPPI_INTER_LINEAR            ; break;
        case inter_cubic     : m_interpolate = IPPI_INTER_CUBIC             ; break;
        case inter_catmullrom: m_interpolate = IPPI_INTER_CUBIC2P_CATMULLROM; break;
        case inter_lanczos   : m_interpolate = IPPI_INTER_LANCZOS           ; break;
        }
        if (m_Smooth) m_interpolate += IPPI_SMOOTH_EDGE  ;
    }
    else
    {
        int inter = m_interpolate & 0xFFFF;
        int flags = m_interpolate - inter;
        switch (inter)
        {
        case IPPI_INTER_NN                : m_Inter = inter_nn        ; break;
        case IPPI_INTER_LINEAR            : m_Inter = inter_linear    ; break;
        case IPPI_INTER_CUBIC             : m_Inter = inter_cubic     ; break;
        case IPPI_INTER_CUBIC2P_CATMULLROM: m_Inter = inter_catmullrom; break;
        case IPPI_INTER_LANCZOS           : m_Inter = inter_lanczos   ; break;
        }
        m_Smooth = (flags & IPPI_SMOOTH_EDGE) ? TRUE : FALSE;
    }
}

void CParmRemapDlg::OnType()
{
    UpdateData();
    ShowOptions();
    UpdateQuadOptions();
    UpdateData(FALSE);
}

void CParmRemapDlg::InitType()
{
    for (int i=0; i<MOD_NUM; i++) {
        m_TypeButton[i].SetWindowText(m_Maps[i]->m_Name);
    }
}

void CParmRemapDlg::UpdateType()
{
}

void CParmRemapDlg::ShowOptions()
{
    m_pData = m_Maps[m_Type];
    int i;
    for (i=0; i<NUM_RADIO; i++) {
        int flag = i < m_pData->m_RadioNum ? SW_SHOWNA : SW_HIDE;
        m_RadioButton[i].ShowWindow(flag);
        m_RadioButton[i].SetWindowText(m_pData->m_RadioName[i]);
    }
    for (i=0; i<NUM_EDIT; i++) {
        int flag = i >= m_pData->m_EditStart && i < m_pData->m_EditEnd ?
               SW_SHOWNA : SW_HIDE;
        m_NameStatic[i].ShowWindow(flag);
        m_TextEdit[i].ShowWindow(flag);
    }
}

void CParmRemapDlg::UpdateQuadOptions()
{
}


