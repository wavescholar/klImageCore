/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmWarpqDlg.h: interface for CParmWarpqDlg class.
// CParmWarpqDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMWARPQDLG_H__44AAAF67_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
#define AFX_PARMWARPQDLG_H__44AAAF67_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CParmWarpqDlg dialog

#include "ParamDlg.h"

class CParmWarpqDlg : public CParamDlg
{
// Construction
public:
   CParmWarpqDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   double m_srcQuad[4][2];
   double m_dstQuad[4][2];
   //{{AFX_DATA(CParmWarpqDlg)
   enum { IDD = IDD_PARM_WARPQ };
   int      m_Inter;
   BOOL     m_Smooth;
   int      m_SType;
   int      m_DType;
   //}}AFX_DATA
   CButton  m_STypeButton[4];
   CButton  m_DTypeButton[4];
   CEdit    m_SQuadEdit[4][2];
   CString  m_SQuadStr[4][2];
   CEdit    m_DQuadEdit[4][2];
   CString  m_DQuadStr[4][2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmWarpqDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void UpdateTypes();
    void UpdateMyData(BOOL save = TRUE) ;

   // Generated message map functions
   //{{AFX_MSG(CParmWarpqDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnSType();
   afx_msg void OnDType();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMWARPQDLG_H__44AAAF67_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
