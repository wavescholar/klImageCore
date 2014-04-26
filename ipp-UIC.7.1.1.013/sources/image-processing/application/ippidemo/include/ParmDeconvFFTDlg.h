/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmDeconvFFTDlg.h: interface for CParmDeconvFFTDlg class.
// CParmDeconvFFTDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMDECONVFFTDLG_H__F0211FC5_3E57_44D4_9069_59037C0767C0__INCLUDED_)
#define AFX_PARMDECONVFFTDLG_H__F0211FC5_3E57_44D4_9069_59037C0767C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
#include "RunDeconvFFT.h"

/////////////////////////////////////////////////////////////////////////////
// CParmDeconvFFTDlg dialog

class CParmDeconvFFTDlg : public CParamDlg
{
// Construction
public:
   CParmDeconvFFTDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   int kernelSize;
   int order;
   Ipp32f threshold;
   //{{AFX_DATA(CParmDeconvFFTDlg)
   enum { IDD = IDD_PARM_DECONVFFT };
   //}}AFX_DATA
   CStatic orderStatic;
   CSpinButtonCtrl orderSpin;

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmDeconvFFTDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmDeconvFFTDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMDECONVFFTDLG_H__F0211FC5_3E57_44D4_9069_59037C0767C0__INCLUDED_)
