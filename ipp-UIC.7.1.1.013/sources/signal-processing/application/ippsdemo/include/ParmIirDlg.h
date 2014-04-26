/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmIirDlg.h: interface for CParmIirDlg class.
// CParmIirDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMIIRDLG_H__7C369D7D_138D_4B36_AF47_611DF74D5A62__INCLUDED_)
#define AFX_PARMIIRDLG_H__7C369D7D_138D_4B36_AF47_611DF74D5A62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParamDlg.h"
#include "Vector.h"

class CDemoDoc;

/////////////////////////////////////////////////////////////////////////////
// CParmIirDlg dialog

class CParmIirDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmIirDlg)
// Construction
public:
   CParmIirDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CDemoDoc*   m_pDocTaps;
   int m_tapsFactor;
   //{{AFX_DATA(CParmIirDlg)
   enum { IDD = IDD_PARM_IIR };
   CButton  m_TypeFrame;
   CButton  m_TypeButton0;
   CButton  m_TypeButton1;
   CButton  m_TapStatic;
   CStatic  m_FactorStatic;
   CSpinButtonCtrl   m_FactorSpin;
   CEdit m_FactorEdit;
   CEdit m_TapEdit;
   int      m_TapLen;
   CMyString   m_TapStr;
   CString  m_LenName;
   CString  m_FactorStr;
   CString  m_TapTitle;
   int      m_BQ;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmIirDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   void SetTapLen() ;
   void EnableFactor();
   // Generated message map functions
   //{{AFX_MSG(CParmIirDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnBQ();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMIIRDLG_H__7C369D7D_138D_4B36_AF47_611DF74D5A62__INCLUDED_)
