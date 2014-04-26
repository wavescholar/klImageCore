/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVFilterColDlg.h: interface for CParmCVFilterColDlg class.
// CParmCVFilterColDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCVFILTERCOLDLG_H__1A44189B_BD77_41BB_AC7A_8F1333F73372__INCLUDED_)
#define AFX_PARMCVFILTERCOLDLG_H__1A44189B_BD77_41BB_AC7A_8F1333F73372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParmFilterColDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCVFilterColDlg dialog

class CParmCVFilterColDlg : public CParmFilterColDlg
{
// Construction
public:
   CParmCVFilterColDlg(UINT nID = IDD);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCVFilterColDlg)
   enum { IDD = IDD_PARMCV_FILTERCOL };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVFilterColDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmCVFilterColDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCVFILTERCOLDLG_H__1A44189B_BD77_41BB_AC7A_8F1333F73372__INCLUDED_)
