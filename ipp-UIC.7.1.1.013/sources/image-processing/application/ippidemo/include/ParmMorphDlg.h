/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMorphDlg.h: interface for CParmMorphDlg class.
// CParmMorphDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMMORPHDLG_H__BE65C974_3800_4DA4_B300_2F8C4CCA3624__INCLUDED_)
#define AFX_PARMMORPHDLG_H__BE65C974_3800_4DA4_B300_2F8C4CCA3624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmMorphDlg dialog

class CParmMorphDlg : public CParamDlg
{
// Construction
public:
   CParmMorphDlg(UINT nID = IDD);   // standard constructor
   enum {MAX_SIDE = 8, MAX_SIZE = 64};

// Dialog Data
   int m_Kernel[MAX_SIZE];
   //{{AFX_DATA(CParmMorphDlg)
   enum { IDD = IDD_PARM_MORPH };
   CEdit m_EditY;
   CEdit m_EditX;
   CSpinButtonCtrl   m_SpinY;
   CSpinButtonCtrl   m_SpinX;
   CSpinButtonCtrl   m_SpinW;
   CSpinButtonCtrl   m_SpinH;
   int      m_H;
   int      m_W;
   int      m_X;
   int      m_Y;
   int      m_AnchorCustom;
   //}}AFX_DATA
   CButton  m_ValButton[MAX_SIZE];
   BOOL     m_Val[MAX_SIZE];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmMorphDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   BOOL m_IsShown[MAX_SIZE];
   int  m_MaxX, m_MaxY;
   BOOL m_Init;

   void DoChangeSize();
   void DoChangeKernel();
   void UpdateKernel(BOOL save = TRUE) ;
   void EnableAnchor();
   void SetAnchor();
   virtual void ShowValues(int width, int height);

   // Generated message map functions
   //{{AFX_MSG(CParmMorphDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnChangeSize();
   afx_msg void OnAnchorType();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMMORPHDLG_H__BE65C974_3800_4DA4_B300_2F8C4CCA3624__INCLUDED_)
