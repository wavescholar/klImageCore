/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ProcessPage.cpp : implementation of the Process dialog page.
// Command: Menu-Process-Process by
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "demo.h"
#include "ProcessPage.h"
#include "ProcessSheet.h"
#include "Vector.h"
#include "Histo.h"
#ifdef FILTER_PAGE
#include "FilterDlg.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessPage property page

IMPLEMENT_DYNCREATE(CProcessPage, CPropertyPage)

CProcessPage::CProcessPage(CString caption, UINT idFilter) : CPropertyPage(CProcessPage::IDD)
{
   m_idFilter = idFilter;
   m_strCaption = caption;
   m_psp.pszTitle = m_strCaption;
   m_psp.dwFlags |= PSP_USETITLE;
   //{{AFX_DATA_INIT(CProcessPage)
   m_IdxBook = -1;
   m_IdxChapter = -1;
   m_FuncName = _T("");
   m_FuncTitle = _T("");
   //}}AFX_DATA_INIT
   m_pVector = NULL;
   m_bInit = FALSE;
   m_bModify = FALSE;
   m_bOK = FALSE;
}

CProcessPage::~CProcessPage()
{
}

void CProcessPage::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CProcessPage)
   DDX_Control(pDX, IDC_LIST_Func, m_FuncBox);
   DDX_Radio(pDX, IDC_BOOK_0, m_IdxBook);
   DDX_Radio(pDX, IDC_CHAPT_0, m_IdxChapter);
   DDX_LBString(pDX, IDC_LIST_Func, m_FuncName);
   DDX_Text(pDX, IDC_FUNC_STATIC, m_FuncTitle);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_BOOK_0 , m_BookButton[0 ]);
   DDX_Control(pDX, IDC_BOOK_1 , m_BookButton[1 ]);
   DDX_Control(pDX, IDC_BOOK_2 , m_BookButton[2 ]);
   DDX_Control(pDX, IDC_BOOK_3 , m_BookButton[3 ]);
   DDX_Control(pDX, IDC_BOOK_4 , m_BookButton[4 ]);
   DDX_Control(pDX, IDC_BOOK_5 , m_BookButton[5 ]);
   DDX_Control(pDX, IDC_BOOK_6 , m_BookButton[6 ]);
   DDX_Control(pDX, IDC_BOOK_7 , m_BookButton[7 ]);
   DDX_Control(pDX, IDC_BOOK_8 , m_BookButton[8 ]);
   DDX_Control(pDX, IDC_BOOK_9 , m_BookButton[9 ]);
   DDX_Control(pDX, IDC_BOOK_10, m_BookButton[10]);
   DDX_Control(pDX, IDC_BOOK_11, m_BookButton[11]);
   DDX_Control(pDX, IDC_BOOK_12, m_BookButton[12]);
   DDX_Control(pDX, IDC_BOOK_13, m_BookButton[13]);

   DDX_Control(pDX, IDC_CHAPT_0 , m_ChapterButton[0 ]);
   DDX_Control(pDX, IDC_CHAPT_1 , m_ChapterButton[1 ]);
   DDX_Control(pDX, IDC_CHAPT_2 , m_ChapterButton[2 ]);
   DDX_Control(pDX, IDC_CHAPT_3 , m_ChapterButton[3 ]);
   DDX_Control(pDX, IDC_CHAPT_4 , m_ChapterButton[4 ]);
   DDX_Control(pDX, IDC_CHAPT_5 , m_ChapterButton[5 ]);
   DDX_Control(pDX, IDC_CHAPT_6 , m_ChapterButton[6 ]);
   DDX_Control(pDX, IDC_CHAPT_7 , m_ChapterButton[7 ]);
   DDX_Control(pDX, IDC_CHAPT_8 , m_ChapterButton[8 ]);
   DDX_Control(pDX, IDC_CHAPT_9 , m_ChapterButton[9 ]);
   DDX_Control(pDX, IDC_CHAPT_10, m_ChapterButton[10]);
   DDX_Control(pDX, IDC_CHAPT_11, m_ChapterButton[11]);
   DDX_Control(pDX, IDC_CHAPT_12, m_ChapterButton[12]);
   DDX_Control(pDX, IDC_CHAPT_13, m_ChapterButton[13]);
}


BEGIN_MESSAGE_MAP(CProcessPage, CPropertyPage)
   //{{AFX_MSG_MAP(CProcessPage)
   ON_BN_CLICKED(IDC_BOOK_0, OnBook)
   ON_BN_CLICKED(IDC_CHAPT_0, OnChapter)
   ON_BN_CLICKED(IDC_BOOK_1, OnBook)
   ON_BN_CLICKED(IDC_BOOK_2, OnBook)
   ON_BN_CLICKED(IDC_BOOK_3, OnBook)
   ON_BN_CLICKED(IDC_BOOK_4, OnBook)
   ON_BN_CLICKED(IDC_BOOK_5, OnBook)
   ON_BN_CLICKED(IDC_BOOK_6, OnBook)
   ON_BN_CLICKED(IDC_BOOK_7, OnBook)
   ON_BN_CLICKED(IDC_BOOK_8, OnBook)
   ON_BN_CLICKED(IDC_BOOK_9, OnBook)
   ON_BN_CLICKED(IDC_BOOK_10, OnBook)
   ON_BN_CLICKED(IDC_BOOK_11, OnBook)
   ON_BN_CLICKED(IDC_BOOK_12, OnBook)
   ON_BN_CLICKED(IDC_BOOK_13, OnBook)
   ON_BN_CLICKED(IDC_CHAPT_1, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_2, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_3, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_4, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_5, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_6, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_7, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_8, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_9, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_10, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_11, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_12, OnChapter)
   ON_BN_CLICKED(IDC_CHAPT_13, OnChapter)
   ON_WM_SHOWWINDOW()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessPage message handlers

BOOL CProcessPage::OnInitDialog() 
{
   ASSERT(m_Process.GetParentProcess());

   CPropertyPage::OnInitDialog();
#ifdef FILTER_PAGE
   m_Process.ApplyFilter(&m_Filter);
#endif
   m_bInit = TRUE;
   m_bModify = TRUE;
   InitBooks();
   EnableBooks();
   InitChapters();
   EnableChapters();
   InitFuncBox();
   SetFuncTitle();
   UpdateData(FALSE);  
   return TRUE;
}

BOOL CProcessPage::OnSetActive() 
{  
   if (!CPropertyPage::OnSetActive())
      return FALSE;
   int numFunc = m_Process.GetFuncCount();
   GetParent()->GetDlgItem(IDOK)->EnableWindow(numFunc);
#ifdef FILTER_PAGE
   if (m_bModify) {
      m_bModify = FALSE;
      SetModified();
   }
#endif
   return TRUE;
}

void CProcessPage::OnOK() 
{
   m_bOK = TRUE;
   if (!m_bInit) return;
   UpdateData();
   CPropertyPage::OnOK();
}

BOOL CProcessPage::OnApply() 
{
   if (m_bModify || m_bOK) return TRUE;
#ifdef FILTER_PAGE
   ((CProcessSheet*)GetParent())->OnFilter();
#endif
   return FALSE;
}

#ifdef FILTER_PAGE

BOOL CProcessPage::StartFilterProblem() 
{
   return (m_Process.GetParentProcess()->GetFuncCount() > 0) 
       && (m_Process.ApplyFilter(&m_Filter) == 0);
}

BOOL CProcessPage::OnFilter() 
{
   if (m_idFilter == NULL) return FALSE;
   UpdateData();
   if (m_IdxBook >= 0 && m_IdxChapter >= 0)
      m_Process.SetCurrentFunction(m_IdxBook, m_IdxChapter, m_FuncName);

   CFilterDlg dlg(m_idFilter, m_strCaption);
   dlg.m_Filter = m_Filter;
   if (dlg.DoModal() != IDOK) return FALSE;
   m_Filter = dlg.m_Filter;

   int numFunc = m_Process.ApplyFilter(&m_Filter);
   GetParent()->GetDlgItem(IDOK)->EnableWindow(numFunc);
   m_FuncName = m_Process.GetCurrentFunction();
   
   EnableBooks();
   InitChapters();
   EnableChapters();
   InitFuncBox();
   SetFuncTitle();
   UpdateData(FALSE);
   return TRUE;
}
#endif
void CProcessPage::OnBook() 
{
    int prevIndex = m_IdxBook;
    UpdateData();
    if (prevIndex >= 0 && m_IdxChapter >= 0)
       m_Process.SetCurrentFunction(prevIndex, m_IdxChapter, m_FuncName);
    m_IdxChapter = m_Process.GetAt(m_IdxBook).GetCurrentChapter();
    InitChapters();
    EnableChapters();
    InitFuncBox();
    UpdateData(FALSE);
}

void CProcessPage::OnChapter() 
{
    int prevIndex = m_IdxChapter;
    UpdateData();
    if (prevIndex >= 0 && m_IdxBook >= 0)
       m_Process.SetCurrentFunction(m_IdxBook, prevIndex, m_FuncName);
    InitFuncBox();
    UpdateData(FALSE);
}

void CProcessPage::SetFuncTitle() 
{
   m_FuncTitle = 
#ifdef FILTER_PAGE
      m_Filter.Enabled() ? 
      _T("Functions ( Filter Applied )") : 
#endif
      _T("Functions");
}

void CProcessPage::InitBooks() 
{
   for (int i=0; i<chapterMAX; i++) {
      if (i < m_Process.GetSize()) {
         m_BookButton[i].ShowWindow(SW_SHOWNA);
         m_BookButton[i].SetWindowText(
            m_Process.GetAt(i).GetName());
      } else {
         m_BookButton[i].ShowWindow(SW_HIDE);
      }
   }
}

void CProcessPage::EnableBooks() 
{
   if (m_Process.GetSize() == 0) return;
   if (m_IdxBook < 0) m_IdxBook = 0;
   int prevIdx = m_IdxBook;
   if (m_Process.GetAt(m_IdxBook).GetFuncCount() == 0) m_IdxBook = -1;
   for (int i=0; i<m_Process.GetSize(); i++) {
      int numFunc = m_Process.GetAt(i).GetFuncCount();
      m_BookButton[i].EnableWindow(numFunc);
      if (m_IdxBook < 0 && numFunc) m_IdxBook = i;
   }
   if (m_IdxBook < 0)
      m_IdxBook = prevIdx;
   if (m_IdxBook != prevIdx)
      m_IdxChapter = m_Process.GetAt(m_IdxBook).GetCurrentChapter();
}

void CProcessPage::InitChapters() 
{
   CBook* pBook = m_IdxBook < 0 ? NULL : &(m_Process.ElementAt(m_IdxBook));
   for (int i=0; i<chapterMAX; i++) {
      if (pBook && i < pBook->GetSize()) {
         m_ChapterButton[i].ShowWindow(SW_SHOWNA);
         m_ChapterButton[i].SetWindowText(
            pBook->GetAt(i).GetChapterName());
      } else {
         m_ChapterButton[i].ShowWindow(SW_HIDE);
      }
   }
}

void CProcessPage::EnableChapters() 
{
   if (m_IdxBook < 0) return;
   CBook* pBook = &(m_Process.ElementAt(m_IdxBook));
   if (pBook->GetSize() == 0) return;
   if (m_IdxChapter < 0) m_IdxChapter = 0;
   int prevIdx = m_IdxChapter;
   if (pBook->GetAt(m_IdxChapter).GetCount() == 0) m_IdxChapter = -1;
   for (int i=0; i<pBook->GetSize(); i++) {
      int numFunc = (int)pBook->GetAt(i).GetCount();
      m_ChapterButton[i].EnableWindow(numFunc);
      if (m_IdxChapter < 0 && numFunc) m_IdxChapter = i;
   }
   if (m_IdxChapter < 0)
      m_IdxChapter = prevIdx;
}

void CProcessPage::InitFuncBox()
{
   m_FuncBox.ResetContent();
   if (m_IdxBook < 0 || m_IdxChapter < 0) return;
   CChapter* pChapter = &(m_Process.ElementAt(m_IdxBook).ElementAt(m_IdxChapter));
   POSITION pos = pChapter->GetHeadPosition();
   while (pos) m_FuncBox.AddString(pChapter->GetNext(pos));
   m_FuncBox.SelectString(-1,m_Process.GetAt(m_IdxBook).GetAt(m_IdxChapter).
      GetCurrentFunction(m_pVector->GetHisto()->LastFunc()));
   if (m_FuncBox.GetCurSel() < 0) m_FuncBox.SetCurSel(0);
}
