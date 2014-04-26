/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterWiener.h: interface for CParmFilterWiener class.
// CParmFilterWiener dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFILTERWIENER_H__95879311_8B0A_4113_9744_FEE5D25E9641__INCLUDED_)
#define AFX_PARMFILTERWIENER_H__95879311_8B0A_4113_9744_FEE5D25E9641__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParmFilterBoxDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmFilterWienerDlg dialog

class CParmFilterWienerDlg : public CParmFilterBoxDlg
{
// Construction
public:
   CParmFilterWienerDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CVector m_value;
   //{{AFX_DATA(CParmFilterWienerDlg)
   enum { IDD = IDD_PARM_FILWIENER };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA
   CEdit m_ValEdit[4];
   CString  m_ValStr[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFilterWienerDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmFilterWienerDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFILTERWIENER_H__95879311_8B0A_4113_9744_FEE5D25E9641__INCLUDED_)
