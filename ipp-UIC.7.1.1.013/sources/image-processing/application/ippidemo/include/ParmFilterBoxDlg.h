/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterBoxDlg.h: interface for CParmFilterBoxDlg class.
// CParmFilterBoxDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMMAXDLG_H__F0211FC5_3E57_44D4_9069_59037C0767C0__INCLUDED_)
#define AFX_PARMMAXDLG_H__F0211FC5_3E57_44D4_9069_59037C0767C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmFilterBoxDlg dialog

class CParmFilterBoxDlg : public CParamDlg
{
// Construction
public:
   CParmFilterBoxDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   IppiPoint m_anchor;
   IppiSize m_maskSize;
   //{{AFX_DATA(CParmFilterBoxDlg)
   enum { IDD = IDD_PARM_MAX };
   BOOL  m_bCenter;
   //}}AFX_DATA
   CSpinButtonCtrl   m_AnchorSpin[2];
   CSpinButtonCtrl   m_SizeSpin[2];
   CEdit m_SizeEdit[2];
   CEdit m_AnchorEdit[2];
   CStatic  m_AnchorStatic[2];
   CStatic  m_SizeStatic[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFilterBoxDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void EnableAnchor();
   void CenterAnchor();
   BOOL GetCenter();
   int  GetSize(int i);
   void SetSize(int i, int val);
   int  GetAnchor(int i);
   void SetAnchor(int i, int val);

   // Generated message map functions
   //{{AFX_MSG(CParmFilterBoxDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnCenter();
   afx_msg void OnChangeSize();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMMAXDLG_H__F0211FC5_3E57_44D4_9069_59037C0767C0__INCLUDED_)
