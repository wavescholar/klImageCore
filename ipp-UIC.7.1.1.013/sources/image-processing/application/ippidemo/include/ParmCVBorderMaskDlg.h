/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVBorderMaskDlg.h: interface for CParmCVBorderMaskDlg class.
// CParmCVBorderMaskDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCVBORDERMASKDLG_H__FAEC1744_CC18_4D4B_A39B_F13A029679E5__INCLUDED_)
#define AFX_PARMCVBORDERMASKDLG_H__FAEC1744_CC18_4D4B_A39B_F13A029679E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParmCVBorderDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CParmCVBorderMaskDlg dialog

class CParmCVBorderMaskDlg : public CParmCVBorderDlg
{
// Construction
public:
   CParmCVBorderMaskDlg(UINT nID = IDD);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCVBorderMaskDlg)
   enum { IDD = IDD_PARMCV_BORDERMASK };
   int      m_Mask;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVBorderMaskDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmCVBorderMaskDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCVBORDERMASKDLG_H__FAEC1744_CC18_4D4B_A39B_F13A029679E5__INCLUDED_)
