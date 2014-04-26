/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSetDlg.h: interface for CParmSetDlg class.
// CParmSetDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSETDLG_H__2EB7D485_305E_42FC_B09E_99C0F32F63FF__INCLUDED_)
#define AFX_PARMSETDLG_H__2EB7D485_305E_42FC_B09E_99C0F32F63FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

class CParmSetDlg : public CParamDlg
{
// Construction
public:
   CParmSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CVector m_value;
   //{{AFX_DATA(CParmSetDlg)
   enum { IDD = IDD_PARM_SET };
   CButton  m_ChanStatic;
   CStatic  m_ValName;
   CButton  m_ValGroup;
   int      m_channel;
   //}}AFX_DATA
   CEdit m_ValEdit[4];
   CString  m_ValStr[4];
   CButton  m_ChannelButton[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmSetDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmSetDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSETDLG_H__2EB7D485_305E_42FC_B09E_99C0F32F63FF__INCLUDED_)
