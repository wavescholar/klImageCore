/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Parm3SDlg.h: interface for CParm3SDlg class.
// CParm3SDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARM3SDLG_H__8BD1F06A_1AF5_4A47_A7CA_937E259E8CF7__INCLUDED_)
#define AFX_PARM3SDLG_H__8BD1F06A_1AF5_4A47_A7CA_937E259E8CF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParm3SDlg dialog

class CParm3SDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParm3SDlg)
// Construction
public:
   CParm3SDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParm3SDlg)
   enum { IDD = IDD_PARM_3S };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParm3SDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParm3SDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARM3SDLG_H__8BD1F06A_1AF5_4A47_A7CA_937E259E8CF7__INCLUDED_)
