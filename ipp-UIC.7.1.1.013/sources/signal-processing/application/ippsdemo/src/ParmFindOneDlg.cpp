/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFindOneDlg.cpp : implementation of the CParmFindOneDlg class.
// CParmFindOneDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmFindOneDlg.h"
#include "RunFindOne.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFindOneDlg dialog


IMPLEMENT_DYNAMIC(CParmFindOneDlg, CParamDlg)
CParmFindOneDlg::CParmFindOneDlg(CRunFindOne* pRun, UINT nID)
   : CParamDlg(nID, NULL)
{
   m_pRun = pRun;
   m_UsedScale = FALSE;
   //{{AFX_DATA_INIT(CParmFindOneDlg)
   m_TableMode = -1;
   m_TableLenStr = _T("");
   m_TableStr = _T("");
   m_IndexStr = _T("");
   m_OutValStr = _T("");
   m_InValStr = _T("555");
   //}}AFX_DATA_INIT
}


void CParmFindOneDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFindOneDlg)
   DDX_Control(pDX, IDC_INVAL_SPIN, m_InValSpin);
   DDX_Radio(pDX, IDC_TABL_TYPE_0, m_TableMode);
   DDX_Control(pDX, IDC_TABL_LEN_EDIT, m_TableLenEdit);
   DDX_Text(pDX, IDC_TABL_LEN_EDIT, m_TableLenStr);
   DDX_Control(pDX, IDC_TABL_LEN_SPIN, m_TableLenSpin);
   DDX_Control(pDX, IDC_TABL_EDIT, m_TableEdit);
   DDX_Text(pDX, IDC_TABL_EDIT, m_TableStr);
   DDX_Text(pDX, IDC_INDEX, m_IndexStr);
   DDX_Text(pDX, IDC_OUTVAL, m_OutValStr);
   DDX_Text(pDX, IDC_INVAL_EDIT, m_InValStr);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_TABL_TYPE_0, m_TableModeButton[0]);
   DDX_Control(pDX, IDC_TABL_TYPE_1, m_TableModeButton[1]);
}


BEGIN_MESSAGE_MAP(CParmFindOneDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmFindOneDlg)
   ON_EN_CHANGE(IDC_TABL_EDIT, OnChangeTablEdit)
   ON_BN_CLICKED(IDC_TABL_TYPE_0, OnTablMode)
   ON_EN_CHANGE(IDC_TABL_LEN_EDIT, OnChangeTablLenEdit)
   ON_BN_CLICKED(IDC_TABL_TYPE_1, OnTablMode)
   ON_EN_CHANGE(IDC_INVAL_EDIT, OnChangeInvalEdit)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFindOneDlg message handlers

BOOL CParmFindOneDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();

   m_TableSave = *m_pTable;

   SetValues();

   m_InValSpin.SetRange(0, SHRT_MAX);
   m_TableLenSpin.SetRange(1, SHRT_MAX);

   EnableTable();
   m_TableLenStr.Format("%d", m_pTable->Length());
   UpdateTable(FALSE);
   UpdateData(FALSE);

   return TRUE;
}

void CParmFindOneDlg::OnOK() 
{
   UpdateData();
   if (!SetValues()) {
      AfxMessageBox("Can't apply " + m_Func + ": table isn't increasing");
      return;
   }
   CParamDlg::OnOK();
}

void CParmFindOneDlg::OnCancel() 
{
   *m_pTable = m_TableSave;
   CParamDlg::OnCancel();
}

void CParmFindOneDlg::OnTablMode() 
{
   UpdateData();
   EnableTable();
   ModifyTable();
   UpdateData(FALSE);
}

void CParmFindOneDlg::OnChangeTablLenEdit() 
{
   if (!m_bInit) return;
   UpdateData();
   ModifyTable();
   UpdateData(FALSE);
}

void CParmFindOneDlg::OnChangeTablEdit() 
{
   if (!m_bInit) return;
   UpdateData();
   ModifyTable();
   UpdateData(FALSE);
}

void CParmFindOneDlg::OnChangeInvalEdit() 
{
   if (!m_bInit) return;
   UpdateData();
   SetValues();
   UpdateData(FALSE);
}

void CParmFindOneDlg::EnableTable() 
{
   m_TableLenEdit.SetReadOnly(m_TableMode != TBL_EVEN);
   m_TableLenSpin.EnableWindow(m_TableMode == TBL_EVEN);
   m_TableEdit.SetReadOnly(m_TableMode != TBL_CUSTOM);
}

void CParmFindOneDlg::ModifyTable()
{
   switch (m_TableMode) {
   case TBL_CUSTOM:
      UpdateTable();
      UpdateTableLen();
      break;
   case TBL_EVEN:
      m_pRun->CreateEvenTable(atoi(m_TableLenStr));
      UpdateTable(FALSE);
      break;
   }
   SetValues();
}

void CParmFindOneDlg::UpdateTable(BOOL bSave) 
{
   if (bSave) {
      m_pTable->Set(m_TableStr);
   } else {
      m_pTable->Get(m_TableStr);
   }
}

void CParmFindOneDlg::UpdateTableLen() 
{
   m_TableLenStr.Format("%d", m_pTable->Length());
}

BOOL CParmFindOneDlg::SetValues()
{ 
   if (m_pRun->SetValues(this)) {
      UpdateVal();
      return TRUE;
   } else {
      ClearVal();
      return FALSE;
   }
}

void CParmFindOneDlg::UpdateVal() 
{
}

void CParmFindOneDlg::ClearVal() 
{
   m_OutValStr = "";
   m_IndexStr  = "";
}
