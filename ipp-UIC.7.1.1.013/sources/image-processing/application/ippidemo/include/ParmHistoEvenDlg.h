/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmHistoEvenDlg.h: interface for CParmHistoEvenDlg class.
// CParmHistoEvenDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMHISTOEVENDLG_H__65B7CF51_94D2_44DE_AB0E_08CE0744AF0B__INCLUDED_)
#define AFX_PARMHISTOEVENDLG_H__65B7CF51_94D2_44DE_AB0E_08CE0744AF0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CLUT;
class CRunHistoEven;

/////////////////////////////////////////////////////////////////////////////
// CParmHistoEvenDlg dialog

class CParmHistoEvenDlg : public CParamDlg
{
// Construction
public:
   CParmHistoEvenDlg(CRunHistoEven* pRun, UINT nID = CParmHistoEvenDlg::IDD);

// Dialog Data
   CLUT* m_pLUT;

   //{{AFX_DATA(CParmHistoEvenDlg)
   enum { IDD = IDD_PARM_HISTO_EVEN };
   //}}AFX_DATA
   CEdit m_ValTitleEdit[4];
   CEdit m_ValEdit[4];
   CSpinButtonCtrl   m_LevelNumSpin[4];
   CEdit m_LevelNumEdit[4];
   CStatic  m_LevelNumStatic[4];
   CButton  m_GroupStatic[4];
   CMyString   m_LevelNumStr[4];
   CMyString   m_ValStr[4];
   CString  m_ValTitleStr[4];

   CStatic  m_UpperStatic[4];
   CEdit m_UpperEdit[4];
   CStatic  m_LowerStatic[4];
   CEdit m_LowerEdit[4];
   CMyString   m_LowerStr[4];
   CMyString   m_UpperStr[4];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmHistoEvenDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunHistoEven* m_pRun;
   void ShowVal() ;
   void FormatVal(BOOL bHisto) ;

   // Generated message map functions
   //{{AFX_MSG(CParmHistoEvenDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnChangeLevel();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMHISTOEVENDLG_H__65B7CF51_94D2_44DE_AB0E_08CE0744AF0B__INCLUDED_)
