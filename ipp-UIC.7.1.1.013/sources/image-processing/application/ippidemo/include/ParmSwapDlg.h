/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSwapDlg.h: interface for CParmSwapDlg class.
// CParmSwapDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSWAPDLG_H__9ED7F0B2_E326_467A_BD11_FD6622823217__INCLUDED_)
#define AFX_PARMSWAPDLG_H__9ED7F0B2_E326_467A_BD11_FD6622823217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmSwapDlg dialog

class CParmSwapDlg : public CParamDlg
{
// Construction
public:
   CParmSwapDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmSwapDlg)
   enum { IDD = IDD_PARM_SWAP };
   //}}AFX_DATA
   CSpinButtonCtrl   m_OrderSpin[3];
   CEdit m_OrderEdit[3];
   int      m_dstOrder[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmSwapDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmSwapDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSWAPDLG_H__9ED7F0B2_E326_467A_BD11_FD6622823217__INCLUDED_)
