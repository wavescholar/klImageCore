/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmTrianDlg.h: interface for CParmTrianDlg class.
// CParmTrianDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMTRIANDLG_H__A7C05A0D_F263_4A48_91ED_DD8070A5CE92__INCLUDED_)
#define AFX_PARMTRIANDLG_H__A7C05A0D_F263_4A48_91ED_DD8070A5CE92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
#include "RunTrian.h"

/////////////////////////////////////////////////////////////////////////////
// CParmTrianDlg dialog

class CParmTrianDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmTrianDlg)
// Construction
public:
   CParmTrianDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmTrianDlg)
   enum { IDD = IDD_PARM_TRIAN };
   //}}AFX_DATA
   CString  m_ParmStr[CRunTrian::tr_NUM];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmTrianDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmTrianDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMTRIANDLG_H__A7C05A0D_F263_4A48_91ED_DD8070A5CE92__INCLUDED_)
