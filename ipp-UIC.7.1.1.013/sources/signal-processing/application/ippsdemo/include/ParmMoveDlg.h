/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMoveDlg.h: interface for CParmMoveDlg class.
// CParmMoveDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMMOVEDLG_H__F1045FE6_A645_4504_B12E_155597313F56__INCLUDED_)
#define AFX_PARMMOVEDLG_H__F1045FE6_A645_4504_B12E_155597313F56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParamDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CParmMoveDlg dialog

class CParmMoveDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmMoveDlg)
// Construction
public:
   CParmMoveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmMoveDlg)
   enum { IDD = IDD_PARM_MOVE };
   CString  m_OffsetSrcStr;
   CSpinButtonCtrl   m_OffsetSrcSpin;
   CString  m_OffsetDstStr;
   CSpinButtonCtrl   m_OffsetDstSpin;
   CSpinButtonCtrl   m_LenSpin;
   int      m_SrcDstLen;
   CString  m_LenStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmMoveDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmMoveDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMMOVEDLG_H__F1045FE6_A645_4504_B12E_155597313F56__INCLUDED_)
