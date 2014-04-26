/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSliceDlg.h: interface for CParmSliceDlg class.
// CParmSliceDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSLICEDLG_H__E5008AB7_3AAE_458B_B5B2_8EA49171E787__INCLUDED_)
#define AFX_PARMSLICEDLG_H__E5008AB7_3AAE_458B_B5B2_8EA49171E787__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmSliceDlg dialog

class CParmSliceDlg : public CParamDlg
{
// Construction
public:
   CParmSliceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmSliceDlg)
   enum { IDD = IDD_PARM_SLICE };
   CSpinButtonCtrl   m_SliceSpin;
   CString  m_HeightStr;
   CString  m_SliceStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmSliceDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmSliceDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

   UDACCEL m_Accel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSLICEDLG_H__E5008AB7_3AAE_458B_B5B2_8EA49171E787__INCLUDED_)
