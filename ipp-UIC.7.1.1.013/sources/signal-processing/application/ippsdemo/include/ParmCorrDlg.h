/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCorrDlg.h: interface for CParmCorrDlg class.
// CParmCorrDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCORRDLG_H__3CB265E1_9D0A_4CE9_9F84_17A56EB6A647__INCLUDED_)
#define AFX_PARMCORRDLG_H__3CB265E1_9D0A_4CE9_9F84_17A56EB6A647__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCorrDlg dialog

class CParmCorrDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmCorrDlg)
// Construction
public:
   CParmCorrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCorrDlg)
   enum { IDD = IDD_PARM_CORR };
   CStatic  m_LolagStatic;
   CEdit m_LolagEdit;
   CEdit m_SrcLenEdit;
   CEdit m_Src2LenEdit;
   CSpinButtonCtrl   m_DstlenSpin;
   CSpinButtonCtrl   m_LolagSpin;
   CMyString   m_LolagStr;
   CMyString   m_DstlenStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCorrDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void EnableSrc2();
   void SetLen1();
   void SetLen2();
   void SetLenEdit(CEdit& edit, CDemoDoc* pDoc);

   // Generated message map functions
   //{{AFX_MSG(CParmCorrDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCORRDLG_H__3CB265E1_9D0A_4CE9_9F84_17A56EB6A647__INCLUDED_)
