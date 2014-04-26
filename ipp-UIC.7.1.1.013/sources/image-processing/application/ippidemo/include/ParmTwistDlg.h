/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmTwistDlg.h: interface for CParmTwistDlg class.
// CParmTwistDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMTWISTDLG_H__EBE03F89_92A3_4261_AA01_AB27C1EE26D8__INCLUDED_)
#define AFX_PARMTWISTDLG_H__EBE03F89_92A3_4261_AA01_AB27C1EE26D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmTwistDlg dialog

class CParmTwistDlg : public CParamDlg
{
// Construction
public:
   CParmTwistDlg(CWnd* pParent = NULL);   // standard constructor
   void GetAver(float twist[3][4]);
   void GetEmph(float twist[3][4]);

// Dialog Data
   //{{AFX_DATA(CParmTwistDlg)
   enum { IDD = IDD_PARM_TWIST };
   int      m_twistType;
   //}}AFX_DATA
   CString  m_twistStr[3][4];
   CEdit m_twistEdit[3][4];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmTwistDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void EnableTwist() ;
   void SetTwist() ;
   void SetCopy ();
   void SetCycle();
   void SetAver ();
   void SetEmph ();
   void TwistToFloat(float twist[3][4]);

   // Generated message map functions
   //{{AFX_MSG(CParmTwistDlg)
   afx_msg void OnType();
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMTWISTDLG_H__EBE03F89_92A3_4261_AA01_AB27C1EE26D8__INCLUDED_)
