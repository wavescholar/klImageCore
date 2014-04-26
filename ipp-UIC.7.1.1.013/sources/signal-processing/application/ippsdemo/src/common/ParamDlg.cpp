/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParamDlg.cpp : implementation of the CParamDlg class.
// CParamDlg is the base class for all dialog classes that get IPP
// function parameters.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "DemoDoc.h"
#include "Histo.h"
#include "ParamDlg.h"
#include "Run.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamDlg dialog

IMPLEMENT_DYNAMIC(CParamDlg, CDialog)

CParamDlg::CParamDlg(UINT nID, CWnd* pParent)
   : CDialog(nID, pParent)
{
   m_bInit = FALSE;
   m_UsedVectors = 0;
   m_UsedScale   = FALSE;
   m_pDocSrc  = NULL;
   m_pDocSrc2 = NULL;
   m_pDocSrc3 = NULL;
   m_pDocMask = NULL;
   m_pDocDst  = NULL;
   m_pDocDst2 = NULL;
   //{{AFX_DATA_INIT(CParamDlg)
   m_scaleString = _T("0");
   //}}AFX_DATA_INIT
}


void CParamDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParamDlg)
   //}}AFX_DATA_MAP
   if (m_UsedVectors & VEC_SRC) {
      DDX_Text(pDX, IDC_EDIT_SRC, m_StrSrc);
      DDX_Control(pDX, IDC_EDIT_SRC, m_EditSrc);
      DDX_Control(pDX, IDC_STATIC_SRC, m_StaticSrc);
   }
   if (m_UsedVectors & VEC_SRC2){ 
      DDX_Text(pDX, IDC_EDIT_SRC2, m_StrSrc2);
      DDX_Control(pDX, IDC_EDIT_SRC2, m_EditSrc2);
      DDX_Control(pDX, IDC_STATIC_SRC2, m_StaticSrc2);
   }
   if (m_UsedVectors & VEC_SRC3){ 
      DDX_Text(pDX, IDC_EDIT_SRC3, m_StrSrc3);
      DDX_Control(pDX, IDC_EDIT_SRC3, m_EditSrc3);
      DDX_Control(pDX, IDC_STATIC_SRC3, m_StaticSrc3);
   }
   if (m_UsedVectors & VEC_MASK){ 
      DDX_Text(pDX, IDC_EDIT_MASK, m_StrMask);
      DDX_Control(pDX, IDC_EDIT_MASK, m_EditMask);
      DDX_Control(pDX, IDC_STATIC_MASK, m_StaticMask);
   }
   if (m_UsedVectors & VEC_DST) { 
      DDX_Text(pDX, IDC_EDIT_DST, m_StrDst);
      DDX_Control(pDX, IDC_EDIT_DST, m_EditDst);
      DDX_Control(pDX, IDC_STATIC_DST, m_StaticDst);
   }    
   if (m_UsedVectors & VEC_DST2) { 
      DDX_Text(pDX, IDC_EDIT_DST2, m_StrDst2);
      DDX_Control(pDX, IDC_EDIT_DST2, m_EditDst2);
      DDX_Control(pDX, IDC_STATIC_DST2, m_StaticDst2);
   }    
   if (m_UsedScale) {
      DDX_Control(pDX, IDC_SCALE_STATIC, m_scaleStatic);
      DDX_Control(pDX, IDC_SCALE_SPIN, m_scaleSpin);
      DDX_Control(pDX, IDC_SCALE_EDIT, m_scaleEdit);
      DDX_Text(pDX, IDC_SCALE_EDIT, m_scaleString);
   }
}


BEGIN_MESSAGE_MAP(CParamDlg, CDialog)
   //{{AFX_MSG_MAP(CParamDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamDlg message handlers

BOOL CParamDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();
   m_bInit = TRUE;
   InitInplace();
   InitVectors();
   SetWindowText(m_Func);
   InitScale();

   UpdateData(FALSE);
   return TRUE;
}

void CParamDlg::OnOK() 
{
   CDialog::OnOK();
   m_bInit = FALSE;
}

void CParamDlg::InitScale()
{
   if (!m_UsedScale) return;
   m_scaleSpin.SetRange(MinScale(),MaxScale());
   EnableScale();
}

void CParamDlg::EnableScale()
{
   if (!m_UsedScale) return;
   int mode = m_Func.Scale() ? SW_SHOWNA : SW_HIDE;
   m_scaleStatic.ShowWindow(mode);
   m_scaleEdit.ShowWindow(mode);
   m_scaleSpin.ShowWindow(mode);
}

int CParamDlg::MaxScale()
{
   return 999;
}

int CParamDlg::MinScale()
{
   return -999;
}

void CParamDlg::InitInplace()
{
   if (!m_Func.Inplace()) return;
   if (m_UsedVectors & VEC_SRC3) {
      m_StaticSrc.SetWindowText(_T("Src1"));
      m_StaticSrc2.SetWindowText(_T("Src2"));
      m_StaticSrc3.SetWindowText(_T("SrcDst"));
   } else if (m_UsedVectors & VEC_SRC2) {
      m_StaticSrc.SetWindowText(_T("Src"));
      m_StaticSrc2.SetWindowText(_T("SrcDst"));
   } else if (m_Func.OnlyDst()) {
      m_StaticSrc.SetWindowText(_T("Dst"));
   } else {
      m_StaticSrc.SetWindowText(_T("SrcDst"));
   }
   if (!m_Func.OnlyDst()) {
      if (m_UsedVectors & VEC_DST) {
         m_StaticDst.ShowWindow(SW_HIDE);
         m_EditDst.ShowWindow(SW_HIDE);
      }
      if (m_UsedVectors & VEC_DST2) {
         m_StaticDst2.ShowWindow(SW_HIDE);
         m_EditDst2.ShowWindow(SW_HIDE);
      }
   }
}

void CParamDlg::InitVectors()
{
    if (m_UsedVectors & VEC_SRC) 
       m_StrSrc = m_pDocSrc->GetTitle();
    if (m_UsedVectors & VEC_SRC2) 
       m_StrSrc2 = m_pDocSrc2->GetTitle();
    if (m_UsedVectors & VEC_SRC3) 
       m_StrSrc3 = m_pDocSrc3->GetTitle();
    if (m_UsedVectors & VEC_MASK) 
       m_StrMask = m_pDocMask->GetTitle();
    if (m_UsedVectors & VEC_DST) {
       if (m_pDocDst)
          m_StrDst = m_pDocDst->GetTitle();
       else
          m_StrDst = NAME_NEW;
    }
    if (m_UsedVectors & VEC_DST2) {
       if (m_pDocDst2)
          m_StrDst2 = m_pDocDst2->GetTitle();
       else
          m_StrDst2 = NAME_NEW;
    }
}

void CParamDlg::SetFlags(int flag, BOOL bFlags[], int n)
{
   for (int i=0; i<n; i++)
      bFlags[i] = (flag & (1 << i)) ? TRUE : FALSE;
}

int  CParamDlg::GetFlags(const BOOL bFlags[], int n)
{
   int flag = 0;
   for (int i=0; i<n; i++) {
      if (bFlags[i]) flag |= 1 << i;
   }
   return flag;
}

