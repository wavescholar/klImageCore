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
// CParmFFTDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFFTDLG_H__CC7E7EAB_5DE3_4016_B1C6_A0223D9C9A13__INCLUDED_)
#define AFX_PARMFFTDLG_H__CC7E7EAB_5DE3_4016_B1C6_A0223D9C9A13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmFFTDlg dialog

class CParmFFTDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmFFTDlg)
// Construction
public:
   CParmFFTDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   BOOL  m_Flag;
   enum { NUM_FLAGS = 4};
   //{{AFX_DATA(CParmFFTDlg)
   enum { IDD = IDD_PARM_FFT };
   CStatic  m_OrderStatic;
   CEdit m_OrderEdit;
   int      m_Order;
   int      m_Hint;
   //}}AFX_DATA
   BOOL  m_bFlag[NUM_FLAGS];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFFTDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void SetOrder();
   // Generated message map functions
   //{{AFX_MSG(CParmFFTDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFFTDLG_H__CC7E7EAB_5DE3_4016_B1C6_A0223D9C9A13__INCLUDED_)
