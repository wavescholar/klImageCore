/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewSignalDlg.h : interface for the creation of New Signal Creation dialog.
// Command: Menu-File-NewSignal
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWSIGNALDLG_H__970DB95E_6948_4207_94B9_1906FF5CAFCC__INCLUDED_)
#define AFX_NEWSIGNALDLG_H__970DB95E_6948_4207_94B9_1906FF5CAFCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewSignalDlg.h : header file
//

#include "NewSignal.h"

/////////////////////////////////////////////////////////////////////////////
// CNewSignalDlg dialog

class CNewSignalDlg : public CDialog
{
// Construction
public:
   CNewSignalDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   ppType   m_Type;
   CDlgItemArray m_FormItems[newNUM];
   enum {NUM_PARMS = 5};
   //{{AFX_DATA(CNewSignalDlg)
   enum { IDD = IDD_FILE_NEW_SIGNAL };
   CSpinButtonCtrl   m_LenSpin;
   CString  m_LenStr;
   int      m_Offset;
   int      m_Form;
   int      m_TypeDlg;
   //}}AFX_DATA
   CStatic  m_ParmStatic[NUM_PARMS];
   CSpinButtonCtrl   m_ParmSpin[NUM_PARMS];
   CString  m_ParmStr[NUM_PARMS];
   CEdit m_ParmEdit[NUM_PARMS];
   CStatic  m_ParmPi[NUM_PARMS];

   CWnd  m_OffsWnd[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CNewSignalDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   enum {
      DLG_8u,  DLG_8s,  DLG_8sc, 
      DLG_16u, DLG_16s, DLG_16sc,
      DLG_32u, DLG_32s, DLG_32sc, DLG_32f, DLG_32fc,
               DLG_64s, DLG_64sc, DLG_64f, DLG_64fc,
   };
   void ShowParms() ;
   void ShowOffset() ;
   void ShowLength() ;
   void ShowEdit() ;
   void ShowName() ;
   void ShowSpin() ;
   void ShowPi() ;
   void GetValue(int form, int i);
   void SetValue(int form, int i);
   void GetValues();
   void SetValues(int form);
   void ChangeValues();
   ppType DLGtoPP(int type) ;
   int    PPtoDLG(ppType type) ;

   // Generated message map functions
   //{{AFX_MSG(CNewSignalDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnForm();
   afx_msg void OnOffsType();
   afx_msg void OnType();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSIGNALDLG_H__970DB95E_6948_4207_94B9_1906FF5CAFCC__INCLUDED_)
