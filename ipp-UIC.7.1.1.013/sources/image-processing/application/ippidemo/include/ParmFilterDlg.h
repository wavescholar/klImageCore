/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterDlg.h: interface for CParmFilterDlg class.
// CParmFilterDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFILTERDLG_H__AE430481_BA51_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_PARMFILTERDLG_H__AE430481_BA51_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "ParamDlg.h"
#include "RunFilter.h"

/////////////////////////////////////////////////////////////////////////////
// CParmFilterDlg dialog

class CParmFilterDlg : public CParamDlg
{
// Construction
public:
   CParmFilterDlg(CWnd* pParent = NULL);   // standard constructor
   enum {
      msk1x3 = CRunFilter::msk1x3,
      msk3x1 = CRunFilter::msk3x1,
      msk3x3 = CRunFilter::msk3x3,
      msk1x5 = CRunFilter::msk1x5,
      msk5x1 = CRunFilter::msk5x1,
      msk5x5 = CRunFilter::msk5x5,
      mskNUM = CRunFilter::mskNUM};

// Dialog Data
   //{{AFX_DATA(CParmFilterDlg)
   enum { IDD = IDD_PARM_FILTER };
   int      m_RadioMask;
   //}}AFX_DATA
   CButton  m_MaskButton[mskNUM];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFilterDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   // Generated message map functions
   //{{AFX_MSG(CParmFilterDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFILTERDLG_H__AE430481_BA51_11D1_AE6B_444553540000__INCLUDED_)
