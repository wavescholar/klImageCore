/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMinMaxDlg.h: interface for CParmMinMaxDlg class.
// CParmMinMaxDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMMINMAXDLG_H__1019431D_5B5E_40D1_9D38_E7C427B7A477__INCLUDED_)
#define AFX_PARMMINMAXDLG_H__1019431D_5B5E_40D1_9D38_E7C427B7A477__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParamDlg.h"
class CRunMinMax;

/////////////////////////////////////////////////////////////////////////////
// CParmMinMaxDlg dialog

class CParmMinMaxDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmMinMaxDlg)
// Construction
public:
   CParmMinMaxDlg(CRunMinMax* pRun);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmMinMaxDlg)
   enum { IDD = IDD_PARM_MINMAX };
   int      m_IsIdx;
   //}}AFX_DATA
   CStatic  m_ImStatic[2];
   CEdit m_ImEdit[2];
   CStatic  m_IdxStatic[2];
   CEdit m_IdxEdit[2];
   CString  m_IdxStr[2];
   CString  m_ImStr[2];
   CString  m_ReStr[2];
   CString  m_ReName[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmMinMaxDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunMinMax* m_pRun;

   void EnableIdx();
   void EnableIm();

   // Generated message map functions
   //{{AFX_MSG(CParmMinMaxDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnFunc();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMMINMAXDLG_H__1019431D_5B5E_40D1_9D38_E7C427B7A477__INCLUDED_)
