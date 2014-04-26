
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmChCompare dialog gets parameters for certain ippCH functions.
//
/////////////////////////////////////////////////////////////////////////////

// ParmCompareDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmChCompareDlg.h"
#include "RunChCompare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmChCompareDlg dialog

static CString BaseStr[CRunChCompare::FUNC_NUM] = {
   _T("ippsCompare_"),
   _T("ippsCompareIgnoreCase_"),
   _T("ippsCompareIgnoreCaseLatin_"),
   _T("ippsEqual_")
};

IMPLEMENT_DYNAMIC(CParmChCompareDlg, CParamDlg)
CParmChCompareDlg::CParmChCompareDlg(CRunChCompare* pRun)
   : CParamDlg(CParmChCompareDlg::IDD, NULL)
{
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmChCompareDlg)
   m_CompStr = _T("");
   m_FuncIdx = -1;
   //}}AFX_DATA_INIT
}


void CParmChCompareDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmChCompareDlg)
   DDX_Text(pDX, IDC_COMP, m_CompStr);
   DDX_Radio(pDX, IDC_FUNC_0, m_FuncIdx);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_FUNC_0, m_FuncButton[0]);
   DDX_Control(pDX, IDC_FUNC_1, m_FuncButton[1]);
   DDX_Control(pDX, IDC_FUNC_2, m_FuncButton[2]);
   DDX_Control(pDX, IDC_FUNC_3, m_FuncButton[3]);
}


BEGIN_MESSAGE_MAP(CParmChCompareDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmChCompareDlg)
   ON_BN_CLICKED(IDC_FUNC_0, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_1, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_2, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_3, OnFunc)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmChCompareDlg message handlers

BOOL CParmChCompareDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   InitFuncButtons();
   m_pRun->SetValues(this);
   SetCompStr();
   UpdateData(FALSE);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmChCompareDlg::OnFunc() 
{
   UpdateData();
   m_Func = GetFuncName(m_FuncIdx);
   m_pRun->SetValues(this);
   SetCompStr();
   UpdateData(FALSE);
}

void CParmChCompareDlg::InitFuncButtons() 
{
   m_FuncIdx = -1;
   for (int i=0; i<CRunChCompare::FUNC_NUM; i++) {
      CFunc func = GetFuncName(i);
      m_FuncButton[i].SetWindowText(func);
      if (m_Func == func) m_FuncIdx = i;
   }
   m_FuncButton[CRunChCompare::FUNC_COMP_CASE].EnableWindow(m_Func.SrcType() == pp16u);
}

CFunc CParmChCompareDlg::GetFuncName(int idx) 
{   
   return BaseStr[idx] + m_Func.TypeName();
}

void CParmChCompareDlg::SetCompStr() 
{
   if (m_FuncIdx == CRunChCompare::FUNC_EQUAL)
      m_CompStr = m_Comp ? _T("   = ") : _T("  !=");
   else 
      m_CompStr = m_Comp == 0 ? _T("   = ") : 
                  m_Comp <  0 ? _T("   < ") :
                                 _T("   > ");
}
