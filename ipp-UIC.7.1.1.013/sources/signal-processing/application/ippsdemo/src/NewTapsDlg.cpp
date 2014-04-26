/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewTapsDlg.cpp : implementation of the New Taps Creation dialog.
// Command: Menu-File-NewTaps
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "NewTapsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewTapsDlg dialog

IMPLEMENT_DYNAMIC(CNewTapsDlg, CDialog)

CNewTapsDlg::CNewTapsDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CNewTapsDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CNewTapsDlg)
   m_TypeDlg = -1;
   m_Filter = -1;
   m_HighStr = _T("");
   m_LenStr = _T("");
   m_LowStr = _T("");
   //}}AFX_DATA_INIT
}


void CNewTapsDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CNewTapsDlg)
   DDX_Control(pDX, IDC_LOW_STATIC, m_LowStatic);
   DDX_Control(pDX, IDC_LOW_EDIT, m_LowEdit);
   DDX_Control(pDX, IDC_PARM_STATIC, m_ParmStatic);
   DDX_Control(pDX, IDC_LEN_STATIC, m_LenStatic);
   DDX_Control(pDX, IDC_LEN_SPIN, m_LenSpin);
   DDX_Control(pDX, IDC_LEN_EDIT, m_LenEdit);
   DDX_Control(pDX, IDC_HIGH_STATIC, m_HighStatic);
   DDX_Control(pDX, IDC_HIGH_EDIT, m_HighEdit);
   DDX_Control(pDX, IDC_FILTER_STATIC, m_FilterStatic);
   DDX_Radio(pDX, IDC_TYPE_0, m_TypeDlg);
   DDX_Radio(pDX, IDC_FILTER_0, m_Filter);
   DDX_Text(pDX, IDC_HIGH_EDIT, m_HighStr);
   DDX_Text(pDX, IDC_LEN_EDIT, m_LenStr);
   DDX_Text(pDX, IDC_LOW_EDIT, m_LowStr);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_FILTER_0, m_FilterButton[0]);
   DDX_Control(pDX, IDC_FILTER_1, m_FilterButton[1]);
   DDX_Control(pDX, IDC_FILTER_2, m_FilterButton[2]);
   DDX_Control(pDX, IDC_FILTER_3, m_FilterButton[3]);
   DDX_Control(pDX, IDC_FILTER_4, m_FilterButton[4]);
}


BEGIN_MESSAGE_MAP(CNewTapsDlg, CDialog)
   //{{AFX_MSG_MAP(CNewTapsDlg)
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
   ON_BN_CLICKED(IDC_FILTER_0, OnFilter)
   ON_BN_CLICKED(IDC_FILTER_1, OnFilter)
   ON_BN_CLICKED(IDC_FILTER_2, OnFilter)
   ON_BN_CLICKED(IDC_FILTER_3, OnFilter)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewTapsDlg message handlers

BOOL CNewTapsDlg::OnInitDialog() 
{
   m_TypeDlg = PPtoDLG(m_Type);
   CDialog::OnInitDialog();

   m_LenSpin.SetRange(1, SHRT_MAX);
   EnableFilter();
   EnableParms();
   UpdateParms(FALSE);
   UpdateData(FALSE);
   return TRUE;
}

void CNewTapsDlg::OnOK() 
{
   CDialog::OnOK();
   m_Type = DLGtoPP(m_TypeDlg);
   UpdateParms();
}

void CNewTapsDlg::UpdateParms(BOOL bSave)
{
   if (bSave) {
      if (GetParmsFlag())
         m_length = atoi(m_LenStr);
      if (GetLowFlag())
         m_freqLow = atof(m_LowStr);
      if (GetHighFlag())
         m_freqHigh = atof(m_HighStr);
   } else {
      m_LenStr .Format("%d", m_length);
      m_LowStr .Format("%.4g", m_freqLow );
      m_HighStr.Format("%.4g", m_freqHigh);
   }
}

ppType CNewTapsDlg::DLGtoPP(int type) 
{ 
   switch (type) {
   case DLG_16s : return pp16s ;
   case DLG_16sc: return pp16sc;
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

int CNewTapsDlg::PPtoDLG(ppType type) 
{ 
   switch (type) {
   case pp16s : return DLG_16s ;
   case pp16sc: return DLG_16sc;
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

void CNewTapsDlg::OnType() 
{
   UpdateData();
   EnableFilter();
   EnableParms();
}

void CNewTapsDlg::OnFilter() 
{
   UpdateData();
   EnableParms();
}

void CNewTapsDlg::EnableFilter() 
{
   BOOL flag = GetFilterFlag();
   m_FilterStatic.EnableWindow(flag);
   for (int i=0; i<FILTER_NUM; i++)
      m_FilterButton[i].EnableWindow(flag);
}

void CNewTapsDlg::EnableParms() 
{
   BOOL flag;

   flag = GetParmsFlag();
   m_ParmStatic.EnableWindow(flag);
   m_LenStatic.EnableWindow(flag);
   m_LenEdit.EnableWindow(flag);
   m_LenSpin.EnableWindow(flag);

   flag = GetLowFlag();
   m_LowStatic.EnableWindow(flag);
   m_LowEdit.EnableWindow(flag);

   flag = GetHighFlag();
   m_HighStatic.EnableWindow(flag);
   m_HighEdit.EnableWindow(flag);
}

BOOL CNewTapsDlg::GetFilterFlag() 
{
   switch (m_TypeDlg) {
   case DLG_64s : 
   case DLG_64sc: 
      return FALSE;
   case DLG_16s : 
   case DLG_16sc: 
   case DLG_32s : 
   case DLG_32sc: 
   case DLG_32f : 
   case DLG_32fc: 
   case DLG_64f : 
   case DLG_64fc: 
      return TRUE;
   }
   return FALSE;
}

BOOL CNewTapsDlg::GetParmsFlag() 
{
   int filter = GetFilterFlag() ? m_Filter : 0;
   switch (filter) {
   case FILTER_NONE: 
      return FALSE;
   case FILTER_LOW: 
   case FILTER_HIGH: 
   case FILTER_BANDPASS: 
   case FILTER_BANDSTOP: 
      return TRUE;
   }
   return FALSE;
}

BOOL CNewTapsDlg::GetLowFlag() 
{
   int filter = GetFilterFlag() ? m_Filter : 0;
   switch (filter) {
   case FILTER_NONE: 
   case FILTER_HIGH: 
      return FALSE;
   case FILTER_LOW: 
   case FILTER_BANDPASS: 
   case FILTER_BANDSTOP: 
      return TRUE;
   }
   return FALSE;
}

BOOL CNewTapsDlg::GetHighFlag() 
{
   int filter = GetFilterFlag() ? m_Filter : 0;
   switch (filter) {
   case FILTER_NONE: 
   case FILTER_LOW: 
      return FALSE;
   case FILTER_HIGH: 
   case FILTER_BANDPASS: 
   case FILTER_BANDSTOP: 
      return TRUE;
   }
   return FALSE;
}
