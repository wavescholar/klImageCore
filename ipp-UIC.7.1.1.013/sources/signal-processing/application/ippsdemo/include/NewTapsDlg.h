/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewTapsDlg.h : interface for the New Taps Creation dialog.
// Command: Menu-File-NewTaps
//
/////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"
#if !defined(AFX_NEWTAPSDLG_H__780FF9A3_878F_473D_8494_B8C49A24D3DC__INCLUDED_)
#define AFX_NEWTAPSDLG_H__780FF9A3_878F_473D_8494_B8C49A24D3DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewTapsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewTapsDlg dialog

class CNewTapsDlg : public CDialog
{
   DECLARE_DYNAMIC(CNewTapsDlg)
// Construction
public:
   CNewTapsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   ppType   m_Type;
   int      m_length;
   double   m_freqLow;
   double   m_freqHigh;
   //{{AFX_DATA(CNewTapsDlg)
   enum { IDD = IDD_FILE_NEW_TAPS };
   CStatic  m_LowStatic;
   CEdit m_LowEdit;
   CButton  m_ParmStatic;
   CStatic  m_LenStatic;
   CSpinButtonCtrl   m_LenSpin;
   CEdit m_LenEdit;
   CStatic  m_HighStatic;
   CEdit m_HighEdit;
   CButton  m_FilterStatic;
   int      m_TypeDlg;
   int      m_Filter;
   CString  m_HighStr;
   CString  m_LenStr;
   CString  m_LowStr;
   //}}AFX_DATA
   enum {FILTER_NONE, FILTER_LOW, FILTER_HIGH, FILTER_BANDPASS, FILTER_BANDSTOP, FILTER_NUM};
   CButton  m_FilterButton[FILTER_NUM];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CNewTapsDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   enum {
      DLG_16s, DLG_16sc,
      DLG_32s, DLG_32sc, DLG_32f, DLG_32fc,
      DLG_64s, DLG_64sc, DLG_64f, DLG_64fc,
   };
   ppType DLGtoPP(int type) ;
   int    PPtoDLG(ppType type) ;
   void UpdateParms(BOOL bSave = TRUE);
   void EnableFilter(); 
   void EnableParms(); 
   BOOL GetFilterFlag(); 
   BOOL GetParmsFlag(); 
   BOOL GetLowFlag(); 
   BOOL GetHighFlag(); 

   BOOL m_bInit;

   // Generated message map functions
   //{{AFX_MSG(CNewTapsDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnType();
   afx_msg void OnFilter();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWTAPSDLG_H__780FF9A3_878F_473D_8494_B8C49A24D3DC__INCLUDED_)
