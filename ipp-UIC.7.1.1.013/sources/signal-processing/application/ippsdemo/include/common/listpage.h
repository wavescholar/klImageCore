/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ListPage.h : interface for the List page.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTPAGE_H__3D262D03_1086_4745_8C5F_7EA9A5C11EFE__INCLUDED_)
#define AFX_LISTPAGE_H__3D262D03_1086_4745_8C5F_7EA9A5C11EFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CListPage dialog

class CListPage : public CPropertyPage
{
   DECLARE_DYNCREATE(CListPage)

// Construction
public:
   CListPage(CString caption = _T(""));
   ~CListPage();

// Dialog Data
   CString    m_Name;
   CFuncList* m_pFuncList;
   //{{AFX_DATA(CListPage)
   enum { IDD = IDD_LIST_PAGE };
   CListBox m_ListBox;
   //}}AFX_DATA


// Overrides
   // ClassWizard generate virtual function overrides
   //{{AFX_VIRTUAL(CListPage)
   public:
   virtual BOOL OnSetActive();
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   // Generated message map functions
   //{{AFX_MSG(CListPage)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTPAGE_H__3D262D03_1086_4745_8C5F_7EA9A5C11EFE__INCLUDED_)
