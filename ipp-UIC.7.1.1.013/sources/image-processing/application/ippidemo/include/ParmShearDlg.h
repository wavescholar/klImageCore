/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmShearDlg.h: interface for CParmShearDlg class.
// CParmShearDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSHEARDLG_H__44AAAF62_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
#define AFX_PARMSHEARDLG_H__44AAAF62_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CParmShearDlg dialog

#include "ParamDlg.h"

class CParmShearDlg : public CParamDlg
{
// Construction
public:
   CParmShearDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    double m_Shear[2];
    double m_Shift[2];
   //{{AFX_DATA(CParmShearDlg)
   enum { IDD = IDD_PARM_SHEAR };
   int m_interpolate;
   int      m_Type;
   //}}AFX_DATA
   CString  m_ShearStr[2];
   CString  m_ShiftStr[2];
   CEdit m_ShiftEdit[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmShearDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   int   m_Inter;
   BOOL  m_Smooth;

   void UpdateType() ;
    void EnableShift() ;
    void UpdateShift() ;
    void UpdateInterpolation(BOOL save = TRUE) ;
    void UpdateMyData(BOOL save = TRUE) ;
    void FormatShift();
    void FormatShear();

   // Generated message map functions
   //{{AFX_MSG(CParmShearDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnType();
   afx_msg void OnChangeShear();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSHEARDLG_H__44AAAF62_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
