/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmUpdateDlg.h: interface for CParmUpdateDlg class.
// CParmUpdateDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMUPDATEDLG_H__5A8397A8_A43A_4B1A_80DE_8C655C8A2B87__INCLUDED_)
#define AFX_PARMUPDATEDLG_H__5A8397A8_A43A_4B1A_80DE_8C655C8A2B87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CRunUpdate;

/////////////////////////////////////////////////////////////////////////////
// CParmUpdateDlg dialog

class CParmUpdateDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmUpdateDlg)

// Construction
public:
   CParmUpdateDlg(CRunUpdate* pRun);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmUpdateDlg)
   enum { IDD = IDD_PARM_UPDATE };
   CSpinButtonCtrl   m_ShiftSpin;
   CSpinButtonCtrl   m_InValSpin;
   CSpinButtonCtrl   m_AlphaSpin;
   CString  m_AlphaStr;
   int      m_hint;
   CString  m_InValStr;
   CString  m_OutValStr;
   CString  m_ShiftStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmUpdateDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunUpdate* m_pRun;

   // Generated message map functions
   //{{AFX_MSG(CParmUpdateDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnChange();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMUPDATEDLG_H__5A8397A8_A43A_4B1A_80DE_8C655C8A2B87__INCLUDED_)
