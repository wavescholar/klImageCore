/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RoiDlg.h : interface for the Set ROI dialog.
// Command: Menu-Contour-ROI-Set
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROIDLG_H__F5D0FAE5_BC0F_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_ROIDLG_H__F5D0FAE5_BC0F_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//

/////////////////////////////////////////////////////////////////////////////
// CRoiDlg dialog

class CRoiDlg : public CDialog
{
// Construction
public:
   CRoiDlg(CDemoDoc* pDoc);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CRoiDlg)
   enum { IDD = IDD_ROI };
   CEdit m_YEdit;
   CEdit m_XEdit;
   CEdit m_WidthEdit;
   CEdit m_HeightEdit;
   CSpinButtonCtrl   m_SpinY;
   CSpinButtonCtrl   m_SpinX;
   CSpinButtonCtrl   m_SpinWidth;
   CSpinButtonCtrl   m_SpinHeight;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CRoiDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CDemoDoc* m_pDoc;
    IppiRect   m_Roi;
    int m_ImageWidth, m_ImageHeight;

    void UpdateRange();
    int GetMaxX();
   int GetMaxY();
   int GetMaxWidth();
   int GetMaxHeight();

   // Generated message map functions
   //{{AFX_MSG(CRoiDlg)
   virtual void OnOK();
   virtual void OnCancel();
   virtual BOOL OnInitDialog();
   afx_msg void OnKillfocusROIHeight();
   afx_msg void OnKillfocusROIWidth();
   afx_msg void OnKillfocusRoiX();
   afx_msg void OnKillfocusRoiY();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROIDLG_H__F5D0FAE5_BC0F_11D1_AE6B_444553540000__INCLUDED_)
