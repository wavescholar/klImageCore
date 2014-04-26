/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFirDlg.h: interface for CParmFirDlg class.
// CParmFirDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFIRDLG_H__6E3F2666_BE23_4280_A30F_63C0AEA61D8F__INCLUDED_)
#define AFX_PARMFIRDLG_H__6E3F2666_BE23_4280_A30F_63C0AEA61D8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
#include "Vector.h"

class CDemoDoc;

/////////////////////////////////////////////////////////////////////////////
// CParmFirDlg dialog


class CParmFirDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmFirDlg)
// Construction
public:
   CParmFirDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CDemoDoc*   m_pDocTaps;
   int m_tapsFactor;
   int m_upFactor  ;
   int m_upPhase   ;
   int m_downFactor;
   int m_downPhase ;
   //{{AFX_DATA(CParmFirDlg)
   enum { IDD = IDD_PARM_FIR };
   CButton  m_MRButton;
   CButton  m_TapStatic;
   CButton  m_DownStatic;
   CButton  m_UpStatic;
   CStatic  m_FactorStatic;
   CSpinButtonCtrl   m_FactorSpin;
   CEdit m_FactorEdit;
   CMyString   m_TapStr;
   CString  m_FactorStr;
   int      m_TapLen;
   CString  m_TapTitle;
   BOOL  m_MR;
   //}}AFX_DATA
   CStatic  m_MrStatic[4];
   CSpinButtonCtrl   m_MrSpin[4];
   CEdit m_MrEdit[4];
   CString  m_MrStr[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFirDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   void InitItems();
   void InitMr();
   void EnableMr() ;
   void InitMrSpin(); 
   void UpdateMr(BOOL bSave = TRUE) ;
   void EnableFactor();

   // Generated message map functions
   //{{AFX_MSG(CParmFirDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnMR();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFIRDLG_H__6E3F2666_BE23_4280_A30F_63C0AEA61D8F__INCLUDED_)
