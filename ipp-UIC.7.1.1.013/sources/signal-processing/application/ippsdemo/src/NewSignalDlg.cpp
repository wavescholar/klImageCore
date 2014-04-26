/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewSignalDlg.cpp : implementation of the creation of New Signal Creation dialog.
// Command: Menu-File-NewSignal
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "NewSignalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewSignalDlg dialog


CNewSignalDlg::CNewSignalDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CNewSignalDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CNewSignalDlg)
   m_LenStr = _T("");
   m_Offset = -1;
   m_Form = -1;
   m_TypeDlg = -1;
   //}}AFX_DATA_INIT
}


void CNewSignalDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CNewSignalDlg)
   DDX_Control(pDX, IDC_LEN_SPIN, m_LenSpin);
   DDX_Text(pDX, IDC_LEN, m_LenStr);
   DDX_Radio(pDX, IDC_OFFS_0, m_Offset);
   DDX_Radio(pDX, IDC_FORM_0, m_Form);
   DDX_Radio(pDX, IDC_TYPE_0, m_TypeDlg);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_PARM_STATIC_0, m_ParmStatic[0]);
   DDX_Control(pDX, IDC_PARM_STATIC_1, m_ParmStatic[1]);
   DDX_Control(pDX, IDC_PARM_STATIC_2, m_ParmStatic[2]);
   DDX_Control(pDX, IDC_PARM_STATIC_3, m_ParmStatic[3]);
   DDX_Control(pDX, IDC_PARM_STATIC_4, m_ParmStatic[4]);
   DDX_Control(pDX, IDC_PARM_SPIN_0, m_ParmSpin[0]);
   DDX_Control(pDX, IDC_PARM_SPIN_1, m_ParmSpin[1]);
   DDX_Control(pDX, IDC_PARM_SPIN_2, m_ParmSpin[2]);
   DDX_Control(pDX, IDC_PARM_SPIN_3, m_ParmSpin[3]);
   DDX_Control(pDX, IDC_PARM_SPIN_4, m_ParmSpin[4]);
   DDX_Control(pDX, IDC_PARM_0, m_ParmEdit[0]);
   DDX_Control(pDX, IDC_PARM_1, m_ParmEdit[1]);
   DDX_Control(pDX, IDC_PARM_2, m_ParmEdit[2]);
   DDX_Control(pDX, IDC_PARM_3, m_ParmEdit[3]);
   DDX_Control(pDX, IDC_PARM_4, m_ParmEdit[4]);
   DDX_Text(pDX, IDC_PARM_0, m_ParmStr[0]);
   DDX_Text(pDX, IDC_PARM_1, m_ParmStr[1]);
   DDX_Text(pDX, IDC_PARM_2, m_ParmStr[2]);
   DDX_Text(pDX, IDC_PARM_3, m_ParmStr[3]);
   DDX_Text(pDX, IDC_PARM_4, m_ParmStr[4]);
   DDX_Control(pDX, IDC_PARM_PI_0, m_ParmPi[0]);
   DDX_Control(pDX, IDC_PARM_PI_1, m_ParmPi[1]);
   DDX_Control(pDX, IDC_PARM_PI_2, m_ParmPi[2]);
   DDX_Control(pDX, IDC_PARM_PI_3, m_ParmPi[3]);
   DDX_Control(pDX, IDC_PARM_PI_4, m_ParmPi[4]);

   DDX_Control(pDX, IDC_OFFS_STATIC, m_OffsWnd[0]);
   DDX_Control(pDX, IDC_OFFS_0,      m_OffsWnd[1]);
   DDX_Control(pDX, IDC_OFFS_1,      m_OffsWnd[2]);
}


BEGIN_MESSAGE_MAP(CNewSignalDlg, CDialog)
   //{{AFX_MSG_MAP(CNewSignalDlg)
   ON_BN_CLICKED(IDC_FORM_0, OnForm)
   ON_BN_CLICKED(IDC_OFFS_0, OnOffsType)
   ON_BN_CLICKED(IDC_FORM_1, OnForm)
   ON_BN_CLICKED(IDC_FORM_2, OnForm)
   ON_BN_CLICKED(IDC_FORM_3, OnForm)
   ON_BN_CLICKED(IDC_FORM_4, OnForm)
   ON_BN_CLICKED(IDC_FORM_5, OnForm)
   ON_BN_CLICKED(IDC_OFFS_1, OnOffsType)
   ON_BN_CLICKED(IDC_TYPE_0, OnType)
   ON_BN_CLICKED(IDC_TYPE_1, OnType)
   ON_BN_CLICKED(IDC_TYPE_2, OnType)
   ON_BN_CLICKED(IDC_TYPE_3, OnType)
   ON_BN_CLICKED(IDC_TYPE_4, OnType)
   ON_BN_CLICKED(IDC_TYPE_5, OnType)
   ON_BN_CLICKED(IDC_TYPE_6, OnType)
   ON_BN_CLICKED(IDC_TYPE_7, OnType)
   ON_BN_CLICKED(IDC_TYPE_8, OnType)
   ON_BN_CLICKED(IDC_TYPE_9, OnType)
   ON_BN_CLICKED(IDC_TYPE_10, OnType)
   ON_BN_CLICKED(IDC_TYPE_11, OnType)
   ON_BN_CLICKED(IDC_TYPE_12, OnType)
   ON_BN_CLICKED(IDC_TYPE_13, OnType)
   ON_BN_CLICKED(IDC_TYPE_14, OnType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewSignalDlg message handlers

BOOL CNewSignalDlg::OnInitDialog() 
{
   m_TypeDlg = PPtoDLG(m_Type);
   ChangeValues();
   GetValues();

   CDialog::OnInitDialog();
   
   ShowParms();
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewSignalDlg::OnOK() 
{
   CDialog::OnOK();
   SetValues(m_Form);
   m_Type = DLGtoPP(m_TypeDlg);
}

void CNewSignalDlg::OnType() 
{
   UpdateData();
   SetValues(m_Form);
   ChangeValues();
   GetValues();
   UpdateData(FALSE);

   ShowSpin();
}

void CNewSignalDlg::OnForm() 
{
   int form = m_Form;
   UpdateData();
   SetValues(form);
   ChangeValues();
   GetValues();
   UpdateData(FALSE);

   ShowParms();
}

void CNewSignalDlg::OnOffsType() 
{
   UpdateData();
   ShowOffset();
   UpdateData(FALSE);
}

void CNewSignalDlg::ShowParms() 
{
   ShowLength();
   ShowEdit();
   ShowName();
   ShowSpin();
   ShowPi();
   ShowOffset();
}

void CNewSignalDlg::ShowEdit() 
{
   CDlgItemArray array = m_FormItems[m_Form];
   for (int i=0; i<NUM_PARMS; i++) {
      if (i >= array.GetSize() || array[i].m_value.Type() == ppNONE) {
         m_ParmEdit[i].ShowWindow(SW_HIDE);
         continue;
      }
      m_ParmEdit[i].ShowWindow(SW_SHOWNA);
   }
}

void CNewSignalDlg::ShowName() 
{
   CDlgItemArray array = m_FormItems[m_Form];
   for (int i=0; i<NUM_PARMS; i++) {
      if (i >= array.GetSize() || array[i].m_value.Type() == ppNONE) {
         m_ParmStatic[i].ShowWindow(SW_HIDE);
         continue;
      }
      CString name = array[i].m_name;
      if (name.IsEmpty()) {
         m_ParmStatic[i].ShowWindow(SW_HIDE);
      } else {
         m_ParmStatic[i].ShowWindow(SW_SHOWNA);
         m_ParmStatic[i].SetWindowText(name);
      }
   }
}

void CNewSignalDlg::ShowPi() 
{
   CDlgItemArray array = m_FormItems[m_Form];
   for (int i=0; i<NUM_PARMS; i++) {
      if (i >= array.GetSize() || array[i].m_value.Type() == ppNONE) {
         m_ParmPi[i].ShowWindow(SW_HIDE);
         continue;
      }
      CString name = array[i].m_unit;
      if (name.IsEmpty()) {
         m_ParmPi[i].ShowWindow(SW_HIDE);
      } else {
         m_ParmPi[i].ShowWindow(SW_SHOWNA);
         m_ParmPi[i].SetWindowText(name);
      }
   }
}

void CNewSignalDlg::ShowSpin() 
{
   CDlgItemArray array = m_FormItems[m_Form];
   for (int i=0; i<NUM_PARMS; i++) {
      if (i >= array.GetSize() || array[i].m_value.Type() == ppNONE) {
         m_ParmSpin[i].ShowWindow(SW_HIDE);
         continue;
      }
      if (!array[i].m_unit.IsEmpty() || 
           array[i].m_value.Type() & PP_FLOAT ||
          (array[i].m_value.Type() & PP_MASK) == 64) {
         m_ParmSpin[i].ShowWindow(SW_HIDE);
      } else {
         m_ParmSpin[i].ShowWindow(SW_SHOWNA);
         if (array[i].m_min == array[i].m_max)
            m_ParmSpin[i].SetRange32(
               array[i].m_value.IntMin(),array[i].m_value.IntMax());
         else
            m_ParmSpin[i].SetRange32(array[i].m_min, array[i].m_max);
      }
   }
}

void CNewSignalDlg::ShowLength() 
{
   m_LenSpin.SetRange32(
      0,CValue::IntMax(pp32s));
}

void CNewSignalDlg::ShowOffset() 
{
   int flag = (m_Form == newSpike) ? SW_SHOWNA : SW_HIDE;
   for (int i=0; i<3; i++) m_OffsWnd[i].ShowWindow(flag);
   m_ParmEdit[4].EnableWindow(m_Offset);
   m_ParmSpin[4].EnableWindow(m_Offset);
}

void CNewSignalDlg::SetValue(int form, int i) 
{
   m_FormItems[form][i].m_value.Set(m_ParmStr[i]);
}

void CNewSignalDlg::GetValue(int form, int i) 
{
   m_FormItems[form][i].m_value.Get(m_ParmStr[i]);
}

void CNewSignalDlg::SetValues(int form) 
{
   for (int i=0; i<m_FormItems[form].GetSize(); i++)
      SetValue(form, i);
}

void CNewSignalDlg::GetValues() 
{
   for (int i=0; i<m_FormItems[m_Form].GetSize(); i++)
      GetValue(m_Form, i);
}

void CNewSignalDlg::ChangeValues() 
{
   m_Type = DLGtoPP(m_TypeDlg);
   for (int i=0; i<m_FormItems[m_Form].GetSize(); i++) {
      if (m_FormItems[m_Form][i].m_valueVar) {
         if (m_FormItems[m_Form][i].m_valueVar == CDlgItem::varType)
            m_FormItems[m_Form][i].m_value.Init(m_Type);
         else if (m_FormItems[m_Form][i].m_valueVar == CDlgItem::varReal)
            m_FormItems[m_Form][i].m_value.Init((ppType)(m_Type & ~PP_CPLX));
      }
   }
}

ppType CNewSignalDlg::DLGtoPP(int type) 
{ 
   switch (type) {
   case DLG_8u  : return pp8u  ;
   case DLG_8s  : return pp8s  ;
   case DLG_8sc : return pp8sc ;
   case DLG_16u : return pp16u ;
   case DLG_16s : return pp16s ;
   case DLG_16sc: return pp16sc;
   case DLG_32u : return pp32u ;
   case DLG_32s : return pp32s ;
   case DLG_32sc: return pp32sc;
   case DLG_32f : return pp32f ;
   case DLG_32fc: return pp32fc;
   case DLG_64s : return pp64s ;
   case DLG_64sc: return pp64sc;
   case DLG_64f : return pp64f ;
   case DLG_64fc: return pp64fc;
   default: return ppNONE;
   }
}

int CNewSignalDlg::PPtoDLG(ppType type) 
{ 
   switch (type) {
   case pp8u  : return DLG_8u  ;
   case pp8s  : return DLG_8s  ;
   case pp8sc : return DLG_8sc ;
   case pp16u : return DLG_16u ;
   case pp16s : return DLG_16s ;
   case pp16sc: return DLG_16sc;
   case pp32u : return DLG_32u ;
   case pp32s : return DLG_32s ;
   case pp32sc: return DLG_32sc;
   case pp32f : return DLG_32f ;
   case pp32fc: return DLG_32fc;
   case pp64s : return DLG_64s ;
   case pp64sc: return DLG_64sc;
   case pp64f : return DLG_64f ;
   case pp64fc: return DLG_64fc;
   default: return -1;
   }
}
