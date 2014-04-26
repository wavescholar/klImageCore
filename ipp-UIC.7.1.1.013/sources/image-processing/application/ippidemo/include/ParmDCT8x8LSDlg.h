/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmDCT8x8LSDlg.h: interface for CParmDCT8x8LSDlg class.
// CParmDCT8x8LSDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMDCT8X8LSDLG_H__B771C6CA_0D95_4D02_B385_C9EBB529FFBC__INCLUDED_)
#define AFX_PARMDCT8X8LSDLG_H__B771C6CA_0D95_4D02_B385_C9EBB529FFBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmDCT8x8LSDlg dialog

class CParmDCT8x8LSDlg : public CParamDlg
{
// Construction
public:
   CParmDCT8x8LSDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmDCT8x8LSDlg)
   enum { IDD = IDD_DCT8x8LS };
   CString  m_ValStr;
   //}}AFX_DATA
   CString  m_ClipStr[2];
   CEdit m_SpinEdit[2];
   CSpinButtonCtrl   m_ClipSpin[2];
   CStatic  m_ClipStatic[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmDCT8x8LSDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmDCT8x8LSDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMDCT8X8LSDLG_H__B771C6CA_0D95_4D02_B385_C9EBB529FFBC__INCLUDED_)
