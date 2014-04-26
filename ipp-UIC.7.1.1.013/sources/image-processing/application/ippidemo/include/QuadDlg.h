/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// QuadDlg.h : interface for the Set Quadrangle or Parallelogramm dialog.
// Command: Menu-Contour-{Quadrangle|Parallelogramm}-Set
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADDLG_H__ECC74632_E0E8_11D2_8EE2_00AA00A03C3C__INCLUDED_)
#define AFX_QUADDLG_H__ECC74632_E0E8_11D2_8EE2_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CQuadDlg dialog

class CQuadDlg : public CDialog
{
// Construction
public:
   CQuadDlg(CDemoDoc* pDoc, BOOL isParl = FALSE);   // standard constructor
   static void GetDefaultParl(IppiRect roi, double quad[4][2]);
   static void GetDefaultQuad(IppiRect roi, double quad[4][2]);

// Dialog Data
    double m_Quad[4][2];
   //{{AFX_DATA(CQuadDlg)
   enum { IDD = IDD_QUAD };
   //}}AFX_DATA
   CEdit m_Edit[4][2];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CQuadDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
    CDemoDoc* m_pDoc;
    BOOL      m_IsParl;
    void UpdateVals() ;
    void FormatVal3() ;

   // Generated message map functions
   //{{AFX_MSG(CQuadDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnChange();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUADDLG_H__ECC74632_E0E8_11D2_8EE2_00AA00A03C3C__INCLUDED_)
