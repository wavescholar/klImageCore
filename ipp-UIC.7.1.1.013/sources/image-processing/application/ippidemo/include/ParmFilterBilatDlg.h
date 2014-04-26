/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterBilatDlg.h: interface for CParmFilterBilatDlg class.
// CParmFilterBilatDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFILTERBILATDLG_H__F0211FC5_3E57_44D4_9069_59037C0767C0__INCLUDED_)
#define AFX_PARMFILTERBILATDLG_H__F0211FC5_3E57_44D4_9069_59037C0767C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
#include "RunFilterBilat.h"

/////////////////////////////////////////////////////////////////////////////
// CParmFilterBilatDlg dialog

class CParmFilterBilatDlg : public CParamDlg
{
// Construction
public:
   CParmFilterBilatDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    int filter;
    IppiSize kernelSize;
    int stepInKernel;
    Ipp32f valSquareSigma;
    Ipp32f posSquareSigma;
   //{{AFX_DATA(CParmFilterBilatDlg)
   enum { IDD = IDD_PARM_FILTERBILAT };
   //}}AFX_DATA
   CSpinButtonCtrl kernelSize_widthSpin;
   CSpinButtonCtrl kernelSize_heightSpin;
   CSpinButtonCtrl stepInKernelSpin;

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFilterBilatDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmFilterBilatDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFILTERBILATDLG_H__F0211FC5_3E57_44D4_9069_59037C0767C0__INCLUDED_)
