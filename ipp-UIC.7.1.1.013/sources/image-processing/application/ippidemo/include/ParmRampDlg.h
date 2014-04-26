/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRampDlg.h: interface for CParmRampDlg class.
// CParmRampDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMRAMPDLG_H__9F7E9C90_3E6D_4BAE_BEA5_9688984843DB__INCLUDED_)
#define AFX_PARMRAMPDLG_H__9F7E9C90_3E6D_4BAE_BEA5_9688984843DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmRampDlg dialog

class CParmRampDlg : public CParamDlg
{
// Construction
public:
   CParmRampDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmRampDlg)
   enum { IDD = IDD_PARM_RAMP };
   int      m_axis;
   CString  m_offsetStr;
   CString  m_slopeStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmRampDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmRampDlg)
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMRAMPDLG_H__9F7E9C90_3E6D_4BAE_BEA5_9688984843DB__INCLUDED_)
