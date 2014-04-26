/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmWtDlg.h: interface for CParmWtDlg class.
// CParmWtDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMWTDLG_H__B1DF129D_891B_4ED3_A23F_6BF24C6BFF66__INCLUDED_)
#define AFX_PARMWTDLG_H__B1DF129D_891B_4ED3_A23F_6BF24C6BFF66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
#include "Vector.h"

/////////////////////////////////////////////////////////////////////////////
// CParmWtDlg dialog

class CParmWtDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmWtDlg)
// Construction
public:
   CParmWtDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CDemoDoc* m_pDocTapsLow;
   CDemoDoc* m_pDocTapsHigh;
   int       m_offsLow;
   int       m_offsHigh;
   //{{AFX_DATA(CParmWtDlg)
   enum { IDD = IDD_PARM_WT };
   //}}AFX_DATA
   CMyString   m_TapStr[2];
   CString  m_TapTitle[2];
   int      m_TapLen[2];
   CString  m_OffsetStr[2];
   CSpinButtonCtrl   m_OffsetSpin[2];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmWtDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   // Generated message map functions
   //{{AFX_MSG(CParmWtDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMWTDLG_H__B1DF129D_891B_4ED3_A23F_6BF24C6BFF66__INCLUDED_)
