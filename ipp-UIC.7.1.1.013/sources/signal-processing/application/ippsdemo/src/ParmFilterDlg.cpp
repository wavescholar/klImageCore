/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterDlg.cpp : implementation of the CParmFilterDlg class.
// CParmFilterDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFilterDlg dialog


IMPLEMENT_DYNAMIC(CParmFilterDlg, CParamDlg)
CParmFilterDlg::CParmFilterDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmFilterDlg::IDD, pParent)
{
   m_UsedScale = TRUE;
   m_ValName = "MaskSize";
   //{{AFX_DATA_INIT(CParmFilterDlg)
   m_ValStr  = _T("");
   //}}AFX_DATA_INIT
}


void CParmFilterDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFilterDlg)
   DDX_Control(pDX, IDC_VAL_SPIN, m_ValSpin);
   DDX_Control(pDX, IDC_VAL_EDIT, m_ValEdit);
   DDX_Text(pDX, IDC_VAL_EDIT, m_ValStr);
   DDX_Text(pDX, IDC_VAL_STATIC, m_ValName);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmFilterDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmFilterDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFilterDlg message handlers

BOOL CParmFilterDlg::OnInitDialog() 
{
   m_ValStr = m_Val.StringRe();
   CParamDlg::OnInitDialog(); 
   InitSpin();
   return TRUE; 
}

void CParmFilterDlg::OnOK() 
{
   UpdateData();
   m_Val.Set(m_ValStr);
   if (!Validate()) return;
   CParamDlg::OnOK();
}

BOOL CParmFilterDlg::Validate() 
{
   if (m_Func.BaseName() == "FilterMedian") {
      ASSERT(m_Val.Type() == pp32s);
      if ((int)m_Val < 1 ||  ((int)m_Val & 1) == 0) {
         AfxMessageBox("Enter an odd integer greater zero");
         m_ValEdit.SetFocus();
         return FALSE;
      }
      return TRUE;
   }
   return TRUE;
}

void CParmFilterDlg::InitSpin() 
{
   if (m_Val.Float()) {
      m_ValSpin.ShowWindow(SW_HIDE);
      return;
   }
   if (m_Func.BaseName() == "FilterMedian") {
      m_ValSpin.SetRange(1,99);
      return;
   }
   int type  = m_Val.Type() & ~PP_CPLX;
   switch (type) {
   case pp8u  : m_ValSpin.SetRange(0,UCHAR_MAX); break;
   case pp8s  : m_ValSpin.SetRange(CHAR_MIN,CHAR_MAX); break;
   case pp16u : m_ValSpin.SetRange32(0,USHRT_MAX); break;
   case pp16s : m_ValSpin.SetRange32(SHRT_MIN,SHRT_MAX); break;
   case pp32u : m_ValSpin.SetRange32(0,UINT_MAX); break;
   case pp32s : m_ValSpin.SetRange32(INT_MIN,INT_MAX); break;
   case pp64s : m_ValSpin.SetRange32(INT_MIN,INT_MAX); break;
   }   
}

