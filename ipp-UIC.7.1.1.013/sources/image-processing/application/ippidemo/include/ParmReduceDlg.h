/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmReduceDlg.h: interface for CParmReduceDlg class.
// CParmReduceDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMREDUCEDLG_H__741F1376_3A85_11D3_8F3C_00AA00A03C3C__INCLUDED_)
#define AFX_PARMREDUCEDLG_H__741F1376_3A85_11D3_8F3C_00AA00A03C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmReduceDlg dialog

class CParmReduceDlg : public CParamDlg
{
// Construction
public:
   CParmReduceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmReduceDlg)
   enum { IDD = IDD_PARM_REDUCE };
   int      m_Levels;
   int      m_Noise;
   int      m_DitherType;
   CString  m_LevelName;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmReduceDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
    int m_MaxLevel;

    void InitMaxLevel();
   // Generated message map functions
   //{{AFX_MSG(CParmReduceDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnType();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMREDUCEDLG_H__741F1376_3A85_11D3_8F3C_00AA00A03C3C__INCLUDED_)
