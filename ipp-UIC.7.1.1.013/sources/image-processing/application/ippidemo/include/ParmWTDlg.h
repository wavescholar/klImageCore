/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmWTDlg.h: interface for CParmWTDlg class.
// CParmWTDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMWTDLG_H__902F3681_D85D_4E79_BDF0_719B3A779FD3__INCLUDED_)
#define AFX_PARMWTDLG_H__902F3681_D85D_4E79_BDF0_719B3A779FD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmWTDlg dialog

class CParmWTDlg : public CParamDlg
{
// Construction
public:
   CParmWTDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   enum {LEN = 12};
   CDemoDoc* m_pDocSrcWT[4];
   CDemoDoc* m_pDocDstWT[4];
   //{{AFX_DATA(CParmWTDlg)
   enum { IDD = IDD_PARM_WT };
   CStatic  m_SrcStatic0;
   BOOL  m_bDstBorder;
   CStatic  m_DstStatic0;
   //}}AFX_DATA
   CButton  m_InvFrame[2];
   CButton  m_FwdFrame[2];
   CButton  m_IsSrcButton[4];
   CButton  m_IsDstButton[4];
   BOOL  m_IsSrcWT[4];
   BOOL  m_IsDstWT[4];
   CMyString   m_StrSrcWT[4];
   CMyString   m_StrDstWT[4];
   CEdit m_EditSrcWT[4];
   CEdit m_EditDstWT[4];
   CMyString   m_TapStr[2][LEN];
   CEdit m_TapEdit[2][LEN];
   CSpinButtonCtrl   m_LenSpin[2];
   CSpinButtonCtrl   m_AnchorSpin[2];
   CString  m_AnchorStr[2];
   CString  m_LenStr[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmWTDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   BOOL m_bFwd;

   void InitImagesWT() ;
   void ShowImagesWT() ;
   void EnableImagesWT() ;
   void RangeLen(int i);
   void RangeAnchor(int i);
   void ShowKernel(int i);
   int GetLen(int i);

   // Generated message map functions
   //{{AFX_MSG(CParmWTDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnChangeTap0();
   afx_msg void OnChangeTap1();
   afx_msg void OnChangeSize0();
   afx_msg void OnChangeSize1();
   afx_msg void OnChangeLen0();
   afx_msg void OnChangeLen1();
   afx_msg void OnIsImg();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMWTDLG_H__902F3681_D85D_4E79_BDF0_719B3A779FD3__INCLUDED_)
