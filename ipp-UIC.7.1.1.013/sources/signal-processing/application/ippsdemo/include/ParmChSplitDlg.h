
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmChSplit.h: interface for CParmChSplit class.
// CParmChSplit dialog gets parameters for certain ippCH functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCHSPLITDLG_H__4F25DF66_2F37_4104_8574_2F8A0CE94358__INCLUDED_)
#define AFX_PARMCHSPLITDLG_H__4F25DF66_2F37_4104_8574_2F8A0CE94358__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParmChValueDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmChSplitDlg dialog

class CParmChSplitDlg : public CParmChValueDlg
{
   DECLARE_DYNCREATE(CParmChSplitDlg)
// Construction
public:
   CParmChSplitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmChSplitDlg)
   enum { IDD = IDD_PARMCH_SPLIT };
   CSpinButtonCtrl   m_NumSpin;
   CEdit m_LenEdit;
   CButton  m_LenStatic;
   CMyString   m_LenStr;
   CString  m_NumStr;
   CString  m_NumName;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmChSplitDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void EnableLen(BOOL flag);

   // Generated message map functions
   //{{AFX_MSG(CParmChSplitDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCHSPLITDLG_H__4F25DF66_2F37_4104_8574_2F8A0CE94358__INCLUDED_)
