/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMorphDlg.cpp : implementation of the CParmMorphDlg class.
// CParmMorphDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmMorphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmMorphDlg dialog


CParmMorphDlg::CParmMorphDlg(UINT nID)
   : CParamDlg(nID, NULL)
{
   //{{AFX_DATA_INIT(CParmMorphDlg)
   m_H = 8;
   m_W = 8;
   m_X = 0;
   m_Y = 0;
   m_AnchorCustom = 0;
   //}}AFX_DATA_INIT
   m_Init = FALSE;
   for (int i=0; i<MAX_SIZE; i++) {
      m_Kernel[i] = 1;
      m_Val[i] = 1;
      m_IsShown[i] = 1;
   }
   m_MaxX = m_MaxY = 4;
}


void CParmMorphDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmMorphDlg)
   DDX_Control(pDX, IDC_EDIT_Y, m_EditY);
   DDX_Control(pDX, IDC_EDIT_X, m_EditX);
   DDX_Control(pDX, IDC_SPIN_Y, m_SpinY);
   DDX_Control(pDX, IDC_SPIN_X, m_SpinX);
   DDX_Control(pDX, IDC_SPIN_W, m_SpinW);
   DDX_Control(pDX, IDC_SPIN_H, m_SpinH);
   DDX_Text(pDX, IDC_EDIT_H, m_H);
   DDV_MinMaxInt(pDX, m_H, 1, 8);
   DDX_Text(pDX, IDC_EDIT_W, m_W);
   DDV_MinMaxInt(pDX, m_W, 1, 8);
   DDX_Text(pDX, IDC_EDIT_X, m_X);
   DDX_Text(pDX, IDC_EDIT_Y, m_Y);
   DDX_Radio(pDX, IDC_A_CENTER, m_AnchorCustom);
   //}}AFX_DATA_MAP
   DDV_MinMaxInt(pDX, m_X, 0, m_MaxX);
   DDV_MinMaxInt(pDX, m_Y, 0, m_MaxY);

   DDX_Check(pDX, IDC_VALUE_0 , m_Val[0 ]);
   DDX_Check(pDX, IDC_VALUE_1 , m_Val[1 ]);
   DDX_Check(pDX, IDC_VALUE_2 , m_Val[2 ]);
   DDX_Check(pDX, IDC_VALUE_3 , m_Val[3 ]);
   DDX_Check(pDX, IDC_VALUE_4 , m_Val[4 ]);
   DDX_Check(pDX, IDC_VALUE_5 , m_Val[5 ]);
   DDX_Check(pDX, IDC_VALUE_6 , m_Val[6 ]);
   DDX_Check(pDX, IDC_VALUE_7 , m_Val[7 ]);
   DDX_Check(pDX, IDC_VALUE_8 , m_Val[8 ]);
   DDX_Check(pDX, IDC_VALUE_9 , m_Val[9 ]);
   DDX_Check(pDX, IDC_VALUE_10, m_Val[10]);
   DDX_Check(pDX, IDC_VALUE_11, m_Val[11]);
   DDX_Check(pDX, IDC_VALUE_12, m_Val[12]);
   DDX_Check(pDX, IDC_VALUE_13, m_Val[13]);
   DDX_Check(pDX, IDC_VALUE_14, m_Val[14]);
   DDX_Check(pDX, IDC_VALUE_15, m_Val[15]);
   DDX_Check(pDX, IDC_VALUE_16, m_Val[16]);
   DDX_Check(pDX, IDC_VALUE_17, m_Val[17]);
   DDX_Check(pDX, IDC_VALUE_18, m_Val[18]);
   DDX_Check(pDX, IDC_VALUE_19, m_Val[19]);
   DDX_Check(pDX, IDC_VALUE_20, m_Val[20]);
   DDX_Check(pDX, IDC_VALUE_21, m_Val[21]);
   DDX_Check(pDX, IDC_VALUE_22, m_Val[22]);
   DDX_Check(pDX, IDC_VALUE_23, m_Val[23]);
   DDX_Check(pDX, IDC_VALUE_24, m_Val[24]);
   DDX_Check(pDX, IDC_VALUE_25, m_Val[25]);
   DDX_Check(pDX, IDC_VALUE_26, m_Val[26]);
   DDX_Check(pDX, IDC_VALUE_27, m_Val[27]);
   DDX_Check(pDX, IDC_VALUE_28, m_Val[28]);
   DDX_Check(pDX, IDC_VALUE_29, m_Val[29]);
   DDX_Check(pDX, IDC_VALUE_30, m_Val[30]);
   DDX_Check(pDX, IDC_VALUE_31, m_Val[31]);
   DDX_Check(pDX, IDC_VALUE_32, m_Val[32]);
   DDX_Check(pDX, IDC_VALUE_33, m_Val[33]);
   DDX_Check(pDX, IDC_VALUE_34, m_Val[34]);
   DDX_Check(pDX, IDC_VALUE_35, m_Val[35]);
   DDX_Check(pDX, IDC_VALUE_36, m_Val[36]);
   DDX_Check(pDX, IDC_VALUE_37, m_Val[37]);
   DDX_Check(pDX, IDC_VALUE_38, m_Val[38]);
   DDX_Check(pDX, IDC_VALUE_39, m_Val[39]);
   DDX_Check(pDX, IDC_VALUE_40, m_Val[40]);
   DDX_Check(pDX, IDC_VALUE_41, m_Val[41]);
   DDX_Check(pDX, IDC_VALUE_42, m_Val[42]);
   DDX_Check(pDX, IDC_VALUE_43, m_Val[43]);
   DDX_Check(pDX, IDC_VALUE_44, m_Val[44]);
   DDX_Check(pDX, IDC_VALUE_45, m_Val[45]);
   DDX_Check(pDX, IDC_VALUE_46, m_Val[46]);
   DDX_Check(pDX, IDC_VALUE_47, m_Val[47]);
   DDX_Check(pDX, IDC_VALUE_48, m_Val[48]);
   DDX_Check(pDX, IDC_VALUE_49, m_Val[49]);
   DDX_Check(pDX, IDC_VALUE_50, m_Val[50]);
   DDX_Check(pDX, IDC_VALUE_51, m_Val[51]);
   DDX_Check(pDX, IDC_VALUE_52, m_Val[52]);
   DDX_Check(pDX, IDC_VALUE_53, m_Val[53]);
   DDX_Check(pDX, IDC_VALUE_54, m_Val[54]);
   DDX_Check(pDX, IDC_VALUE_55, m_Val[55]);
   DDX_Check(pDX, IDC_VALUE_56, m_Val[56]);
   DDX_Check(pDX, IDC_VALUE_57, m_Val[57]);
   DDX_Check(pDX, IDC_VALUE_58, m_Val[58]);
   DDX_Check(pDX, IDC_VALUE_59, m_Val[59]);
   DDX_Check(pDX, IDC_VALUE_60, m_Val[60]);
   DDX_Check(pDX, IDC_VALUE_61, m_Val[61]);
   DDX_Check(pDX, IDC_VALUE_62, m_Val[62]);
   DDX_Check(pDX, IDC_VALUE_63, m_Val[63]);
   DDX_Control(pDX, IDC_VALUE_0 , m_ValButton[0 ]);
   DDX_Control(pDX, IDC_VALUE_1 , m_ValButton[1 ]);
   DDX_Control(pDX, IDC_VALUE_2 , m_ValButton[2 ]);
   DDX_Control(pDX, IDC_VALUE_3 , m_ValButton[3 ]);
   DDX_Control(pDX, IDC_VALUE_4 , m_ValButton[4 ]);
   DDX_Control(pDX, IDC_VALUE_5 , m_ValButton[5 ]);
   DDX_Control(pDX, IDC_VALUE_6 , m_ValButton[6 ]);
   DDX_Control(pDX, IDC_VALUE_7 , m_ValButton[7 ]);
   DDX_Control(pDX, IDC_VALUE_8 , m_ValButton[8 ]);
   DDX_Control(pDX, IDC_VALUE_9 , m_ValButton[9 ]);
   DDX_Control(pDX, IDC_VALUE_10, m_ValButton[10]);
   DDX_Control(pDX, IDC_VALUE_11, m_ValButton[11]);
   DDX_Control(pDX, IDC_VALUE_12, m_ValButton[12]);
   DDX_Control(pDX, IDC_VALUE_13, m_ValButton[13]);
   DDX_Control(pDX, IDC_VALUE_14, m_ValButton[14]);
   DDX_Control(pDX, IDC_VALUE_15, m_ValButton[15]);
   DDX_Control(pDX, IDC_VALUE_16, m_ValButton[16]);
   DDX_Control(pDX, IDC_VALUE_17, m_ValButton[17]);
   DDX_Control(pDX, IDC_VALUE_18, m_ValButton[18]);
   DDX_Control(pDX, IDC_VALUE_19, m_ValButton[19]);
   DDX_Control(pDX, IDC_VALUE_20, m_ValButton[20]);
   DDX_Control(pDX, IDC_VALUE_21, m_ValButton[21]);
   DDX_Control(pDX, IDC_VALUE_22, m_ValButton[22]);
   DDX_Control(pDX, IDC_VALUE_23, m_ValButton[23]);
   DDX_Control(pDX, IDC_VALUE_24, m_ValButton[24]);
   DDX_Control(pDX, IDC_VALUE_25, m_ValButton[25]);
   DDX_Control(pDX, IDC_VALUE_26, m_ValButton[26]);
   DDX_Control(pDX, IDC_VALUE_27, m_ValButton[27]);
   DDX_Control(pDX, IDC_VALUE_28, m_ValButton[28]);
   DDX_Control(pDX, IDC_VALUE_29, m_ValButton[29]);
   DDX_Control(pDX, IDC_VALUE_30, m_ValButton[30]);
   DDX_Control(pDX, IDC_VALUE_31, m_ValButton[31]);
   DDX_Control(pDX, IDC_VALUE_32, m_ValButton[32]);
   DDX_Control(pDX, IDC_VALUE_33, m_ValButton[33]);
   DDX_Control(pDX, IDC_VALUE_34, m_ValButton[34]);
   DDX_Control(pDX, IDC_VALUE_35, m_ValButton[35]);
   DDX_Control(pDX, IDC_VALUE_36, m_ValButton[36]);
   DDX_Control(pDX, IDC_VALUE_37, m_ValButton[37]);
   DDX_Control(pDX, IDC_VALUE_38, m_ValButton[38]);
   DDX_Control(pDX, IDC_VALUE_39, m_ValButton[39]);
   DDX_Control(pDX, IDC_VALUE_40, m_ValButton[40]);
   DDX_Control(pDX, IDC_VALUE_41, m_ValButton[41]);
   DDX_Control(pDX, IDC_VALUE_42, m_ValButton[42]);
   DDX_Control(pDX, IDC_VALUE_43, m_ValButton[43]);
   DDX_Control(pDX, IDC_VALUE_44, m_ValButton[44]);
   DDX_Control(pDX, IDC_VALUE_45, m_ValButton[45]);
   DDX_Control(pDX, IDC_VALUE_46, m_ValButton[46]);
   DDX_Control(pDX, IDC_VALUE_47, m_ValButton[47]);
   DDX_Control(pDX, IDC_VALUE_48, m_ValButton[48]);
   DDX_Control(pDX, IDC_VALUE_49, m_ValButton[49]);
   DDX_Control(pDX, IDC_VALUE_50, m_ValButton[50]);
   DDX_Control(pDX, IDC_VALUE_51, m_ValButton[51]);
   DDX_Control(pDX, IDC_VALUE_52, m_ValButton[52]);
   DDX_Control(pDX, IDC_VALUE_53, m_ValButton[53]);
   DDX_Control(pDX, IDC_VALUE_54, m_ValButton[54]);
   DDX_Control(pDX, IDC_VALUE_55, m_ValButton[55]);
   DDX_Control(pDX, IDC_VALUE_56, m_ValButton[56]);
   DDX_Control(pDX, IDC_VALUE_57, m_ValButton[57]);
   DDX_Control(pDX, IDC_VALUE_58, m_ValButton[58]);
   DDX_Control(pDX, IDC_VALUE_59, m_ValButton[59]);
   DDX_Control(pDX, IDC_VALUE_60, m_ValButton[60]);
   DDX_Control(pDX, IDC_VALUE_61, m_ValButton[61]);
   DDX_Control(pDX, IDC_VALUE_62, m_ValButton[62]);
   DDX_Control(pDX, IDC_VALUE_63, m_ValButton[63]);
}


BEGIN_MESSAGE_MAP(CParmMorphDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmMorphDlg)
   ON_EN_CHANGE(IDC_EDIT_W, OnChangeSize)
   ON_BN_CLICKED(IDC_A_CENTER, OnAnchorType)
   ON_EN_CHANGE(IDC_EDIT_H, OnChangeSize)
   ON_BN_CLICKED(IDC_A_CUSTOM, OnAnchorType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmMorphDlg message handlers

BOOL CParmMorphDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

    m_SpinW.SetRange(1,8);
    m_SpinH.SetRange(1,8);
    EnableAnchor();
    DoChangeSize();
    UpdateKernel(FALSE);
    UpdateData(FALSE);
    m_Init = TRUE;

   return TRUE;
}

void CParmMorphDlg::OnOK()
{
   CParamDlg::OnOK();
   UpdateKernel();
}

void CParmMorphDlg::OnChangeSize()
{
    if (!m_Init) return;
    UpdateData();
    DoChangeSize();
    DoChangeKernel();
    UpdateData(FALSE);
}

void CParmMorphDlg::OnAnchorType()
{
   if (!m_Init) return;
   UpdateData();
   EnableAnchor();
   SetAnchor();
   UpdateData(FALSE);
}

void CParmMorphDlg::DoChangeSize()
{
    m_MaxX = m_W - 1;
    m_MaxY = m_H - 1;
    if (m_X > m_MaxX) m_X = m_MaxX;
    if (m_Y > m_MaxY) m_Y = m_MaxY;
    m_SpinX.SetRange(0,m_MaxX);
    m_SpinY.SetRange(0,m_MaxY);
    ShowValues(m_W, m_H);
    SetAnchor();
}

void CParmMorphDlg::ShowValues(int width, int height)
{
    int i, j;
    int i1 = (8 - width)/2;
    for (i=0; i<MAX_SIZE; i++) m_IsShown[i] = 0;
    for (j=0; j<height; j++) {
        for (i=i1; i<i1+width; i++) {
            m_IsShown[j*8+i] = 1;
        }
    }
    for (i=0; i<MAX_SIZE; i++) {
        if (m_IsShown[i])
            m_ValButton[i].ShowWindow(SW_SHOWNA);
        else
            m_ValButton[i].ShowWindow(SW_HIDE);
    }
}

void CParmMorphDlg::DoChangeKernel()
{
    UpdateKernel();
    UpdateKernel(FALSE);
}

void CParmMorphDlg::UpdateKernel(BOOL save)
{
    if (save) {
        int j = 0;
        for (int i=0; i<MAX_SIZE; i++) {
            if (m_IsShown[i])
                m_Kernel[j++] = m_Val[i];
        }
    } else {
        int j = 0;
        for (int i=0; i<MAX_SIZE; i++) {
            if (m_IsShown[i])
               m_Val[i] = m_Kernel[j++] ? 1 : 0;
        }
    }
}

void CParmMorphDlg::SetAnchor()
{
   if (m_AnchorCustom) return;
   m_X = m_W >> 1;
   m_Y = m_H >> 1;
}

void CParmMorphDlg::EnableAnchor()
{
   m_EditX.SetReadOnly(!m_AnchorCustom);
   m_EditY.SetReadOnly(!m_AnchorCustom);
   m_SpinX.EnableWindow(m_AnchorCustom);
   m_SpinY.EnableWindow(m_AnchorCustom);
}
