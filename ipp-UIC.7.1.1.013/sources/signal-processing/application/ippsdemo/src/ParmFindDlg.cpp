/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFind.cpp : implementation of the CParmFind class.
// CParmFind dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmFindDlg.h"
#include "RunFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFindDlg dialog

IMPLEMENT_DYNAMIC(CParmFindDlg, CParmFindOneDlg)
CParmFindDlg::CParmFindDlg(CRunFind* pRun)
   : CParmFindOneDlg(pRun, CParmFindDlg::IDD)
{
   m_UsedScale   = TRUE;
   //{{AFX_DATA_INIT(CParmFindDlg)
   m_ValStr = _T("");
   m_ValTitleStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmFindDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmFindOneDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFindDlg)
   DDX_Control(pDX, IDC_VAL_TITLE, m_ValTitleEdit);
   DDX_Control(pDX, IDC_VAL_EDIT, m_ValEdit);
   DDX_Text(pDX, IDC_VAL_EDIT, m_ValStr);
   DDX_Text(pDX, IDC_VAL_TITLE, m_ValTitleStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmFindDlg, CParmFindOneDlg)
   //{{AFX_MSG_MAP(CParmFindDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFindDlg message handlers


BOOL CParmFindDlg::OnInitDialog() 
{
   CParmFindOneDlg::OnInitDialog();
   m_ValTitleStr = "pSrc\tpDst\tpIndex";
   UpdateData(FALSE);

   return TRUE;
}

void CParmFindDlg::UpdateVal() 
{
   m_ValStr = "";
   for (int i = 0; i < m_pDocSrc->GetVector()->Length(); i++) {
      CString buffer;
      buffer.Format("%d\t%d\t%d\r\n",
         m_pSrc[i], m_pDst[i], m_pIndex[i]);
      m_ValStr += buffer;
   }
}

void CParmFindDlg::ClearVal() 
{
   m_ValStr = "";
   for (int i = 0; i < m_pDocSrc->GetVector()->Length(); i++) {
      CString buffer;
      buffer.Format("%d\r\n", m_pSrc[i]);
      m_ValStr += buffer;
   }
}
