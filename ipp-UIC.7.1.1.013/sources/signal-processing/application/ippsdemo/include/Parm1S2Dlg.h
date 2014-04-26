/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// Parm1S2Dlg.h: interface for CParm1S2Dlg class.
// CParm1S2Dlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARM1S2DLG_H__54019105_CADC_46E5_B353_26E4BADEF7C6__INCLUDED_)
#define AFX_PARM1S2DLG_H__54019105_CADC_46E5_B353_26E4BADEF7C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParm1S2Dlg dialog

class CParm1S2Dlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParm1S2Dlg)
// Construction
public:
   CParm1S2Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParm1S2Dlg)
   enum { IDD = IDD_PARM1_S2 };
   CSpinButtonCtrl   m_PhaseFactorSpin;
   CSpinButtonCtrl   m_MagnFactorSpin;
   int      magnScaleFactor;
   int      phaseScaleFactor;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParm1S2Dlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParm1S2Dlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARM1S2DLG_H__54019105_CADC_46E5_B353_26E4BADEF7C6__INCLUDED_)
