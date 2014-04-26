/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVMotionDlg.h: interface for CParmCVMotionDlg class.
// CParmCVMotionDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCVMOTIONDLG_H__79937BAB_E2D9_4D25_9B23_2979C3A126F0__INCLUDED_)
#define AFX_PARMCVMOTIONDLG_H__79937BAB_E2D9_4D25_9B23_2979C3A126F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCVMotionDlg dialog

class CParmCVMotionDlg : public CParamDlg
{
// Construction
public:
   CParmCVMotionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCVMotionDlg)
   enum { IDD = IDD_PARMCV_MOTION };
   CString  m_DurationStr;
   CString  m_StampStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVMotionDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   virtual void InitInplace();

   // Generated message map functions
   //{{AFX_MSG(CParmCVMotionDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCVMOTIONDLG_H__79937BAB_E2D9_4D25_9B23_2979C3A126F0__INCLUDED_)
