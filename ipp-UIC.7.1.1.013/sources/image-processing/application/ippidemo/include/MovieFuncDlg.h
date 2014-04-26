/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MovieFuncDlg.h : interface for the Customize Movie dialog.
// Command: Menu-Movie-Customize
//
/////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"
#if !defined(AFX_MOVIEFUNCDLG_H__AAB1740A_0FA6_4FF1_A563_1F94F6E75E26__INCLUDED_)
#define AFX_MOVIEFUNCDLG_H__AAB1740A_0FA6_4FF1_A563_1F94F6E75E26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMovieFuncDlg dialog

class CMovieFuncDlg : public CDialog
{
// Construction
public:
   CMovieFuncDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CMovieFuncDlg)
   enum { IDD = IDD_MOVIE_FUNC };
   CSliderCtrl m_SpeedSlider;
   CListCtrl   m_FuncBox;
   int      m_Speed;
   int      m_MaskIdx;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CMovieFuncDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void InitFuncBox() ;
   void SaveFunctions();
   void InitMask() ;
   void SaveMask();
   void InitSpeed();
   void SaveSpeed();
   BOOL GetCheckState(int i);
   void SetCheckState(int i, BOOL state);
   // Generated message map functions
   //{{AFX_MSG(CMovieFuncDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedSelectAll();
    afx_msg void OnBnClickedDeselectAll();
    afx_msg void OnNMCustomdrawSpeed(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedSelectPerf();
    CEdit m_SpeedEditPerf;
    CEdit m_SpeedEditRythm;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVIEFUNCDLG_H__AAB1740A_0FA6_4FF1_A563_1F94F6E75E26__INCLUDED_)
