/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmToneDlg.h: interface for CParmToneDlg class.
// CParmToneDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMTONEDLG_H__1FB9A195_0924_48E4_9378_ADE8293E6391__INCLUDED_)
#define AFX_PARMTONEDLG_H__1FB9A195_0924_48E4_9378_ADE8293E6391__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
#include "RunTone.h"

/////////////////////////////////////////////////////////////////////////////
// CParmToneDlg dialog

class CParmToneDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmToneDlg)
// Construction
public:
   CParmToneDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmToneDlg)
   enum { IDD = IDD_PARM_TONE };
   int      m_hint;
   //}}AFX_DATA
   CString  m_ParmStr[CRunTone::tn_NUM];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmToneDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmToneDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMTONEDLG_H__1FB9A195_0924_48E4_9378_ADE8293E6391__INCLUDED_)
