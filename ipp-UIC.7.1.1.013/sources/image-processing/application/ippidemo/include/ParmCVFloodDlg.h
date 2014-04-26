/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVFloodDlg.h: interface for CParmCVFloodDlg class.
// CParmCVFloodDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_PARMFLOODDLG_H__F050F650_9D62_4B05_80BD_FFF1F9031778__INCLUDED_)
#define AFX_PARMFLOODDLG_H__F050F650_9D62_4B05_80BD_FFF1F9031778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCVFloodDlg dialog

class CParmCVFloodDlg : public CParamDlg
{
// Construction
public:
   CParmCVFloodDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   IppiPoint m_seed;
   //{{AFX_DATA(CParmCVFloodDlg)
   enum { IDD = IDD_PARMCV_FLOOD };
   CStatic  m_UpValStatic;
   CStatic  m_LowValStatic;
   CEdit m_UpValEdit;
   CEdit m_LowValEdit;
   CEdit m_SeedYEdit;
   CEdit m_SeedXEdit;
   CEdit m_SeedValEdit;
   CSpinButtonCtrl   m_SeedYSpin;
   CSpinButtonCtrl   m_SeedXSpin;
   CButton  m_SeedTypeButton;
   CString  m_NewValStr;
   CString  m_SeedYStr;
   CString  m_SeedXStr;
   int      m_SeedType;
   CString  m_LowValStr;
   CString  m_UpValStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVFloodDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void EnableSeedType();
   void EnableSeed();
   void EnableVal();
   void SetSeed();
   void GetSeed();
   void FormatSeed();
   void FormatSeedVal();

   // Generated message map functions
   //{{AFX_MSG(CParmCVFloodDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnSeedtype();
   afx_msg void OnChangeSeed();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFLOODDLG_H__F050F650_9D62_4B05_80BD_FFF1F9031778__INCLUDED_)
