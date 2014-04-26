/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterDlg.h: interface for CParmFilterDlg class.
// CParmFilterDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_PARMFILTERDLG_H__191E9BF5_2586_42C5_A20E_981701E20FDB__INCLUDED_)
#define AFX_PARMFILTERDLG_H__191E9BF5_2586_42C5_A20E_981701E20FDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
#include "Value.h"

/////////////////////////////////////////////////////////////////////////////
// CParmFilterDlg dialog

class CParmFilterDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmFilterDlg)
// Construction
public:
   CParmFilterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CValue m_Val;
   //{{AFX_DATA(CParmFilterDlg)
   enum { IDD = IDD_PARM_FILTER };
   CSpinButtonCtrl   m_ValSpin;
   CEdit m_ValEdit;
   CString  m_ValStr;
   CString  m_ValName;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFilterDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   BOOL Validate();
   void InitSpin();
   // Generated message map functions
   //{{AFX_MSG(CParmFilterDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFILTERDLG_H__191E9BF5_2586_42C5_A20E_981701E20FDB__INCLUDED_)
