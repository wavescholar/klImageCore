/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmDotprodDlg.h: interface for CParmDotprodDlg class.
// CParmDotprodDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMDOTPRODDLG_H__24CB5BBD_A28B_4A5C_BBC4_D96F6E908671__INCLUDED_)
#define AFX_PARMDOTPRODDLG_H__24CB5BBD_A28B_4A5C_BBC4_D96F6E908671__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParamDlg.h"
class CRunDotprod;

/////////////////////////////////////////////////////////////////////////////
// CParmDotprodDlg dialog

class CParmDotprodDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmDotprodDlg)
// Construction
public:
   CParmDotprodDlg(CRunDotprod* pRun);   // standard constructor

   enum {_16s, _16s32s, _16s64s, _16s32f, TypeNum};
   enum {_32f, _32f64f};
// Dialog Data
   //{{AFX_DATA(CParmDotprodDlg)
   enum { IDD = IDD_PARM_DOTPROD };
   CEdit m_ImEdit;
   CStatic  m_ImStatic;
   CButton  m_TypeStatic;
   int      m_DstType;
   double   m_Im;
   double   m_Re;
   //}}AFX_DATA
   CButton  m_TypeButton[TypeNum];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmDotprodDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunDotprod* m_pRun;

   void InitType() ;
   void SetFunc() ;
   CString TypeName(int dstType) ;
   int TypeIndex(CFunc func);
   void ShowType();
   void ShowTypeButton(int i, CString text);

   // Generated message map functions
   //{{AFX_MSG(CParmDotprodDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnType();
   afx_msg void OnChangeScaleEdit();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMDOTPRODDLG_H__24CB5BBD_A28B_4A5C_BBC4_D96F6E908671__INCLUDED_)
