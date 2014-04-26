/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMomentDlg.h: interface for CParmMomentDlg class.
// CParmMomentDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMMOMENTDLG_H__7AA0615D_2495_4225_8B71_C66DE360F279__INCLUDED_)
#define AFX_PARMMOMENTDLG_H__7AA0615D_2495_4225_8B71_C66DE360F279__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

class CRunMoment;

/////////////////////////////////////////////////////////////////////////////
// CParmMomentDlg dialog

class CParmMomentDlg : public CParamDlg
{
// Construction
public:
   CParmMomentDlg(CRunMoment* pRun);   // standard constructor

   enum {
      SpatialMoment, CentralMoment,
      NormalizedSpatialMoment, NormalizedCentralMoment,
      HuMoments,
      MomentNum};

// Dialog Data
   CFunc m_GetFunc;
   //{{AFX_DATA(CParmMomentDlg)
   enum { IDD = IDD_PARM_MOMENT };
   int      m_Get;
   int      m_Channel;
   int      m_Hint;
   //}}AFX_DATA
   CButton  m_ChannelButton[4];
   CString  m_MomentStr[10];
   CEdit    m_MomentEdit[10];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmMomentDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunMoment* m_pRun;
   CString m_TypeStr;

   int MomentIndex(CString funcName);
   CString MomentFunc(int idx);
   void SetFunc();
   void EnableChannels();
   void ShowMoments() ;

   // Generated message map functions
   //{{AFX_MSG(CParmMomentDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnGet();
   afx_msg void OnHint();
   afx_msg void OnChan();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMMOMENTDLG_H__7AA0615D_2495_4225_8B71_C66DE360F279__INCLUDED_)
