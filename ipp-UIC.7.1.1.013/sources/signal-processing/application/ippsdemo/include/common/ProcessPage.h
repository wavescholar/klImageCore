/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ProcessPage.h : interface for the Process dialog page.
// Command: Menu-Process-Proces by
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSPAGE_H__9C1BF4AB_24AF_4FFC_81F5_D23EC652AAB6__INCLUDED_)
#define AFX_PROCESSPAGE_H__9C1BF4AB_24AF_4FFC_81F5_D23EC652AAB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessPage.h : header file
//

#define FILTER_PAGE

#include "CProcess.h"
class CProcessSheet;

#ifdef FILTER_PAGE
#include "CFilter.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessPage dialog

class CProcessPage : public CPropertyPage
{
   DECLARE_DYNCREATE(CProcessPage)

// Construction
public:
   CProcessPage(CString caption = _T(""), UINT idFilter = IDD_FILTER);
   ~CProcessPage();

   CSubProcess  m_Process;
   CVector*  m_pVector;
#ifdef FILTER_PAGE
   CFilter   m_Filter;
#endif

   BOOL StartFilterProblem();
   BOOL OnFilter();

   enum { chapterMAX = 14};

   friend class CProcessSheet;

// Dialog Data
   //{{AFX_DATA(CProcessPage)
   enum { IDD = IDD_PROCESS_PG };
   CListBox m_FuncBox;
   int      m_IdxChapter;
   int      m_IdxBook;
   CString  m_FuncName;
   CString  m_FuncTitle;
   //}}AFX_DATA
   CButton  m_BookButton[chapterMAX];
   CButton  m_ChapterButton[chapterMAX];


// Overrides
   // ClassWizard generate virtual function overrides
   //{{AFX_VIRTUAL(CProcessPage)
   public:
   virtual BOOL OnSetActive();
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   virtual BOOL OnApply();
   //}}AFX_VIRTUAL

// Implementation
protected:
   BOOL m_bInit;
   BOOL m_bOK;
   BOOL m_bModify;
   UINT m_idFilter;

   void InitBooks();
   void EnableBooks();
   void InitChapters();
   void EnableChapters();
   void InitFuncBox();
   void SetFuncTitle();
   void EnableOK(BOOL flag);

   // Generated message map functions
   //{{AFX_MSG(CProcessPage)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnBook();
   afx_msg void OnChapter();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSPAGE_H__9C1BF4AB_24AF_4FFC_81F5_D23EC652AAB6__INCLUDED_)
