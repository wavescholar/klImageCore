/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmHistoRangeDlg.h: interface for CParmHistoRangeDlg class.
// CParmHistoRangeDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMHISTORANGEDLG_H__C3C942E3_979B_4190_92FA_68FD1BE5DB5F__INCLUDED_)
#define AFX_PARMHISTORANGEDLG_H__C3C942E3_979B_4190_92FA_68FD1BE5DB5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CLUT;
class CRunHistoRange;

/////////////////////////////////////////////////////////////////////////////
// CParmHistoRangeDlg dialog

class CParmHistoRangeDlg : public CParamDlg
{

// Construction
public:
   CParmHistoRangeDlg(CRunHistoRange* pRun, UINT nID = CParmHistoRangeDlg::IDD);   // standard constructor

// Dialog Data
   CLUT* m_pLUT;

   //{{AFX_DATA(CParmHistoRangeDlg)
   enum { IDD = IDD_PARM_HISTO_RANGE };
   //}}AFX_DATA
   CButton  m_GroupStatic[4];
   CEdit m_ValEdit[4];
   CButton  m_ValButton[4];
   CStatic  m_LevelsNumStatic[4];
   CEdit m_LevelsNumEdit[4];
   CEdit m_LevelsEdit[4];
   CButton  m_LevelsButton[4];
   CMyString   m_LevelsStr[4];
   CMyString   m_ValStr[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmHistoRangeDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunHistoRange* m_pRun;
   BOOL m_LevelsFlag[4];

   void EnableGroups();
   void ShowVal();
   void ShowVal(int idx);
   void FormatVal(BOOL bValid);
   void FormatNumLevels(int idx);
   void UpdateLevels(BOOL bSave = TRUE);
   void OnLevelsButton(int idx);
   void OnValButton(int idx);
   void OnChangeLevelEdit(int idx);

   // Generated message map functions
   //{{AFX_MSG(CParmHistoRangeDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnEven();
   afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
   afx_msg void OnLevelsButton0();
   afx_msg void OnLevelsButton1();
   afx_msg void OnLevelsButton2();
   afx_msg void OnLevelsButton3();
   afx_msg void OnValButton0();
   afx_msg void OnValButton1();
   afx_msg void OnValButton2();
   afx_msg void OnValButton3();
   virtual void OnOK();
   afx_msg void OnChangeLevelEdit0();
   afx_msg void OnChangeLevelEdit1();
   afx_msg void OnChangeLevelEdit2();
   afx_msg void OnChangeLevelEdit3();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMHISTORANGEDLG_H__C3C942E3_979B_4190_92FA_68FD1BE5DB5F__INCLUDED_)
