/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFFTDlg.h: interface for CParmFFTDlg class.
// CParmFFTDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFFTDLG_H__E64FA8F8_2651_4A7A_8225_55AAE8B4BE76__INCLUDED_)
#define AFX_PARMFFTDLG_H__E64FA8F8_2651_4A7A_8225_55AAE8B4BE76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParmHintDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmFFTDlg dialog

class CParmFFTDlg : public CParmHintDlg
{
// Construction
public:
   CParmFFTDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmFFTDlg)
   enum { IDD = IDD_PARM_FFT };
   CStatic  m_orderYStatic;
   CStatic  m_orderXStatic;
   CEdit m_orderYEdit;
   CEdit m_orderXEdit;
   int      m_orderX;
   int      m_orderY;
   int      m_FlagRadio;
   //}}AFX_DATA
   CButton  m_FlagButton[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFFTDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void ShowOrders();
   void SetOrders();
   void EnableFlags();

   // Generated message map functions
   //{{AFX_MSG(CParmFFTDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFFTDLG_H__E64FA8F8_2651_4A7A_8225_55AAE8B4BE76__INCLUDED_)
