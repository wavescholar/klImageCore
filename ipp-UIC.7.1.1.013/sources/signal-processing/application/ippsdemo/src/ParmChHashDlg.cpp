
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmChHash dialog gets parameters for certain ippCH functions.
//
/////////////////////////////////////////////////////////////////////////////

// ParmHashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmChHashDlg.h"
#include "RunChHash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmChHashDlg dialog


static CString BaseStr[CRunChHash::FUNC_NUM] = {
   _T("ippsHash_"),
   _T("ippsHashSJ2_"),
   _T("ippsHashMSCS_"),
};
IMPLEMENT_DYNAMIC(CParmChHashDlg, CParamDlg)
CParmChHashDlg::CParmChHashDlg(CRunChHash* pRun)
   : CParamDlg(CParmChHashDlg::IDD, NULL)
{
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmChHashDlg)
   m_FuncIdx = -1;
   //}}AFX_DATA_INIT
   for (int i=0; i<CRunChHash::FUNC_NUM; i++)
      m_ValStr[i] = _T("");
}


void CParmChHashDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmChHashDlg)
   DDX_Radio(pDX, IDC_FUNC_0, m_FuncIdx);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_FUNC_0, m_FuncButton[0]);
   DDX_Control(pDX, IDC_FUNC_1, m_FuncButton[1]);
   DDX_Control(pDX, IDC_FUNC_2, m_FuncButton[2]);
   DDX_Text(pDX, IDC_VAL_0, m_ValStr[0]);
   DDX_Text(pDX, IDC_VAL_1, m_ValStr[1]);
   DDX_Text(pDX, IDC_VAL_2, m_ValStr[2]);
}

BEGIN_MESSAGE_MAP(CParmChHashDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmChHashDlg)
   ON_BN_CLICKED(IDC_FUNC_0, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_1, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_2, OnFunc)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmChHashDlg message handlers

BOOL CParmChHashDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   InitFuncButtons();
   UpdateData(FALSE);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmChHashDlg::OnFunc() 
{
   UpdateData();
   m_Func = GetFuncName(m_FuncIdx);
   m_pRun->SetValues(this);
   SetFuncResult(m_FuncIdx);
   UpdateData(FALSE);
}

void CParmChHashDlg::InitFuncButtons() 
{
   m_FuncIdx = -1;
   for (int i=0; i<CRunChHash::FUNC_NUM; i++) {
      CFunc func = GetFuncName(i);
      m_FuncButton[i].SetWindowText(func);
   }
}

CFunc CParmChHashDlg::GetFuncName(int idx) 
{   
   return BaseStr[idx] + m_Func.TypeName();
}

void CParmChHashDlg::SetFuncResult(int idx) 
{
   m_ValStr[idx].Format(_T("%u"), m_result);
}
