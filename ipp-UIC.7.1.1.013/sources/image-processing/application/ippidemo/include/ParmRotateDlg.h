/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRotateDlg.h: interface for CParmRotateDlg class.
// CParmRotateDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMROTATEDLG_H__C2918022_E111_11D2_8EE2_00AA00A03C3C__INCLUDED_)
#define AFX_PARMROTATEDLG_H__C2918022_E111_11D2_8EE2_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CParmRotateDlg dialog

#include "ParamDlg.h"

class CParmRotateDlg : public CParamDlg
{
// Construction
public:
   CParmRotateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    double m_Angle;
    double m_Shift[2];
   //{{AFX_DATA(CParmRotateDlg)
   enum { IDD = IDD_PARM_ROTATE };
   CButton  m_ShiftStatic;
   CString  m_AngleStr;
   int m_interpolate;
   int      m_Type;
   //}}AFX_DATA
   CString  m_ShiftStr[2];
   CEdit m_ShiftEdit[2];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmRotateDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   int      m_Inter;
   BOOL  m_Smooth;

    void UpdateInterpolation(BOOL save = TRUE) ;
    void UpdateType() ;
    void EnableShift() ;
    void UpdateShift() ;
    void UpdateMyData(BOOL save = TRUE) ;
    void FormatShift();
    void FormatAngle();

   // Generated message map functions
   //{{AFX_MSG(CParmRotateDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnType();
   afx_msg void OnChangeAngle();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMROTATEDLG_H__C2918022_E111_11D2_8EE2_00AA00A03C3C__INCLUDED_)
