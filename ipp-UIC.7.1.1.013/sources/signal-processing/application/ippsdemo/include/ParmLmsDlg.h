/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmLmsDlg.h: interface for CParmLmsDlg class.
// CParmLmsDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMLMSDLG_H__E6DB8A17_C34E_4CE0_960B_84CA8F07F79D__INCLUDED_)
#define AFX_PARMLMSDLG_H__E6DB8A17_C34E_4CE0_960B_84CA8F07F79D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
#include "Vector.h"

class CDemoDoc;

/////////////////////////////////////////////////////////////////////////////
// CParmLmsDlg dialog

class CParmLmsDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmLmsDlg)
// Construction
public:
   CParmLmsDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CFunc       m_InitName;
   CValue  m_mu;
   int m_tapLen;
   //{{AFX_DATA(CParmLmsDlg)
   enum { IDD = IDD_PARM_LMS };
   CSpinButtonCtrl   m_TapLenSpin;
   CMyString   m_MuStr;
   CString  m_MuName;
   CString  m_TapLenStr;
   int      m_DlyTypeDlg;
   //}}AFX_DATA
   CButton  m_DlyTypeButton[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmLmsDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void InitMu() ;
   void EnableDly();

   // Generated message map functions
   //{{AFX_MSG(CParmLmsDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMLMSDLG_H__E6DB8A17_C34E_4CE0_960B_84CA8F07F79D__INCLUDED_)
