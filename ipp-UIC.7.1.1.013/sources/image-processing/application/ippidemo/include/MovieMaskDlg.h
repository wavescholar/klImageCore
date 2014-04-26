/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

#if !defined(AFX_MOVIEMASKDLG_H__E352037F_49E7_47A8_A834_465E3F62796C__INCLUDED_)
#define AFX_MOVIEMASKDLG_H__E352037F_49E7_47A8_A834_465E3F62796C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MovieMaskDlg.h : header file
//

#include "SampleWnd.h"
#include "MovieMask.h"

/////////////////////////////////////////////////////////////////////////////
// CMovieMaskDlg dialog

class CMovieMaskDlg : public CDialog
{
// Construction
public:
   CMovieMaskDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CMovieMaskDlg)
   enum { IDD = IDD_MOVIE_MASK };
   CButton  m_RemoveButton;
   CListBox m_NameBox;
   CSliderCtrl m_ThreshSlider;
   CSampleWnd  m_Sample;
   int      m_Threshold;
   int      m_NameIdx;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CMovieMaskDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CMovieMaskArray m_MaskArr;

   void InitNameBox();
   int AddToBox(CMovieMask* pMask);
   CMovieMask* GetCurMask();
   BOOL LoadMask(BOOL bAsk = TRUE);
   void ShowMask();
   void InitSlider();
   void RedrawSample();

   // Generated message map functions
   //{{AFX_MSG(CMovieMaskDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnAdd();
   afx_msg void OnRemove();
   afx_msg void OnMaskSlider(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnSelchangeNameList();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVIEMASKDLG_H__E352037F_49E7_47A8_A834_465E3F62796C__INCLUDED_)
