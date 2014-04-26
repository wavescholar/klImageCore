/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFind.h: interface for CParmFind class.
// CParmFind dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFIND_H__368ACF59_FA53_4FA4_A419_DCF69FAC1588__INCLUDED_)
#define AFX_PARMFIND_H__368ACF59_FA53_4FA4_A419_DCF69FAC1588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"
#include "ParmFindOneDlg.h"
class CRunFind;

/////////////////////////////////////////////////////////////////////////////
// CParmFindDlg dialog

class CParmFindDlg : public CParmFindOneDlg
{
   DECLARE_DYNAMIC(CParmFindDlg)
// Construction
public:
   CParmFindDlg(CRunFind* pRun);
   
   Ipp16u* m_pSrc;
   Ipp16u* m_pDst;
   int* m_pIndex;
// Dialog Data
   //{{AFX_DATA(CParmFindDlg)
   enum { IDD = IDD_PARM_FIND };
   CEdit m_ValTitleEdit;
   CEdit m_ValEdit;
   CMyString   m_ValStr;
   CString  m_ValTitleStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFindDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   virtual void UpdateVal() ;
   virtual void ClearVal() ;

   // Generated message map functions
   //{{AFX_MSG(CParmFindDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFIND_H__368ACF59_FA53_4FA4_A419_DCF69FAC1588__INCLUDED_)
