/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVStatDlg.h: interface for CParmCVStatDlg class.
// CParmCVStatDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCVSTATDLG_H__73551B0B_0AA7_4C20_A21C_3123C5EA6A76__INCLUDED_)
#define AFX_PARMCVSTATDLG_H__73551B0B_0AA7_4C20_A21C_3123C5EA6A76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParamDlg.h"
class CRunCVStat;

/////////////////////////////////////////////////////////////////////////////
// CParmCVStatDlg dialog

class CParmCVStatDlg : public CParamDlg
{
// Construction
public:
   CParmCVStatDlg(CRunCVStat* pRun);   // standard constructor

// Dialog Data
   CRunCVStat* m_pRun;
   IppiPoint m_pIndex[2];
   //{{AFX_DATA(CParmCVStatDlg)
   enum { IDD = IDD_PARMCV_STAT };
   CStatic  m_coiStatic;
   CEdit m_coiEdit;
   int      m_funcIndex;
   CString  m_coiStr;
   //}}AFX_DATA
   CButton  m_funcRadio[3];

   CString  m_valStr[2];
   CEdit m_valEdit[2];
   CString  m_valName[2];
   CStatic  m_valStatic[2];
   CButton  m_IndexStatic[2];
   CEdit m_xEdit[2];
   CEdit m_yEdit[2];
   CStatic  m_xStatic[2];
   CStatic  m_yStatic[2];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVStatDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void ShowValues() ;
   void SetNames() ;

   // Generated message map functions
   //{{AFX_MSG(CParmCVStatDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnChange();
   afx_msg void OnDeltaposCoiSpin(NMHDR* pNMHDR, LRESULT* pResult);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCVSTATDLG_H__73551B0B_0AA7_4C20_A21C_3123C5EA6A76__INCLUDED_)
