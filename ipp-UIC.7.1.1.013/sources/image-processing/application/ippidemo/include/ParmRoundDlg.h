/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRoundDlg.h: interface for CParmRoundDlg class.
// CParmRoundDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMROUNDDLG_H__46BB5DD5_5F01_47C1_BE7E_E0D69DFD4DA8__INCLUDED_)
#define AFX_PARMROUNDDLG_H__46BB5DD5_5F01_47C1_BE7E_E0D69DFD4DA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmRoundDlg dialog

class CParmRoundDlg : public CParamDlg
{
// Construction
public:
   CParmRoundDlg(UINT nID = CParmRoundDlg::IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmRoundDlg)
   enum { IDD = IDD_PARM_ROUND };
   int      m_roundMode;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmRoundDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmRoundDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMROUNDDLG_H__46BB5DD5_5F01_47C1_BE7E_E0D69DFD4DA8__INCLUDED_)
