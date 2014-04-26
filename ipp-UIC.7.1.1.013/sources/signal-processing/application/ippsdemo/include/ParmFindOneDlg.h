/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFindOneDlg.h: interface for CParmFindOneDlg class.
// CParmFindOneDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFINDONEDLG_H__67E3DD66_5D94_4817_A459_07F595D26E1B__INCLUDED_)
#define AFX_PARMFINDONEDLG_H__67E3DD66_5D94_4817_A459_07F595D26E1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"
#include "ParamDlg.h"
class CRunFindOne;

/////////////////////////////////////////////////////////////////////////////
// CParmFindOneDlg dialog

class CParmFindOneDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmFindOneDlg)
// Construction
public:
   CParmFindOneDlg(CRunFindOne* pRun, UINT nID = IDD);   // standard constructor

   enum {TBL_EVEN, TBL_CUSTOM, };

   CVector* m_pTable;
// Dialog Data
   //{{AFX_DATA(CParmFindOneDlg)
   enum { IDD = IDD_PARM_FIND_ONE };
   CSpinButtonCtrl   m_InValSpin;
   int      m_TableMode;
   CEdit m_TableLenEdit;
   CString  m_TableLenStr;
   CSpinButtonCtrl   m_TableLenSpin;
   CEdit m_TableEdit;
   CMyString   m_TableStr;
   CString  m_IndexStr;
   CString  m_OutValStr;
   CString  m_InValStr;
   //}}AFX_DATA
   CButton  m_TableModeButton[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFindOneDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunFindOne* m_pRun;
   CVector   m_TableSave;

   void EnableTable() ;
   void ModifyTable();
   void UpdateTable(BOOL bSave = TRUE) ;
   void UpdateTableLen() ;
   BOOL SetValues();
   virtual void UpdateVal() ;
   virtual void ClearVal() ;

   // Generated message map functions
   //{{AFX_MSG(CParmFindOneDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnChangeTablEdit();
   afx_msg void OnTablMode();
   afx_msg void OnChangeTablLenEdit();
   virtual void OnOK();
   virtual void OnCancel();
   afx_msg void OnChangeInvalEdit();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFINDONEDLG_H__67E3DD66_5D94_4817_A459_07F595D26E1B__INCLUDED_)
