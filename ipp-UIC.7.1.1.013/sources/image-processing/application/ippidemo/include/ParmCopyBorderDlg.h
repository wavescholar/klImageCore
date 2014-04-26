/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCopyBorderDlg.h: interface for CParmCopyBorderDlg class.
// CParmCopyBorderDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCOPYBORDERDLG_H__AE1CB7E2_B98D_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_PARMCOPYBORDERDLG_H__AE1CB7E2_B98D_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CParmCopyBorderDlg dialog

#include "ParamDlg.h"

class CParmCopyBorderDlg : public CParamDlg
{
// Construction
public:
   CParmCopyBorderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CVector m_value;
   //{{AFX_DATA(CParmCopyBorderDlg)
   enum { IDD = IDD_PARM_BORDER };
   //}}AFX_DATA

   BOOL m_bDst;
   BOOL m_aroundRoi;
   int m_tl[2];
   int m_br[2];
   int m_shw[2];
   int m_dhw[2];
   int m_ayx[2];
   int m_a_y_x[2];

   CButton         m_aroundButton;
   CSpinButtonCtrl m_borderSpin[4];
   CEdit           m_borderEdit[4];
   CString         m_borderStr[4];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCopyBorderDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
    void SetSpin();
    void EnableBorders();
    void UpdateBorders();
    void UpdateMyData(BOOL bSave = TRUE);

   // Generated message map functions
   //{{AFX_MSG(CParmCopyBorderDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnChangeBorder();
   afx_msg void OnBnClickedAround();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCOPYBORDERDLG_H__AE1CB7E2_B98D_11D1_AE6B_444553540000__INCLUDED_)
