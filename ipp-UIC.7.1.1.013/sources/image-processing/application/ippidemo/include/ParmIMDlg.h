/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmIMDlg.h: interface for CParmIMDlg class.
// CParmIMDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMIMDLG_H__8DE19FCD_9FE0_47A4_A5DA_9BC4A7CC6F64__INCLUDED_)
#define AFX_PARMIMDLG_H__8DE19FCD_9FE0_47A4_A5DA_9BC4A7CC6F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmIMDlg dialog

class CParmIMDlg : public CParamDlg
{
// Construction
public:
   CParmIMDlg(UINT nID = IDD);

// Dialog Data
   CDemoDoc* m_pDocSrc3;
   //{{AFX_DATA(CParmIMDlg)
   enum { IDD = IDD_PARM_IM3 };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA
   CString     m_StrSrc3;

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmIMDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   virtual void InitInplace();

   // Generated message map functions
   //{{AFX_MSG(CParmIMDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMIMDLG_H__8DE19FCD_9FE0_47A4_A5DA_9BC4A7CC6F64__INCLUDED_)
