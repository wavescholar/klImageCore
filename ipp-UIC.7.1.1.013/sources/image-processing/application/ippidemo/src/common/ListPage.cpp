/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ListPage.cpp : implementation of the List page.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "demo.h"
#include "ListPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListPage property page

IMPLEMENT_DYNCREATE(CListPage, CPropertyPage)

CListPage::CListPage(CString caption) : CPropertyPage(CListPage::IDD)
{
   m_strCaption = caption;
   m_psp.pszTitle = m_strCaption;
   m_psp.dwFlags |= PSP_USETITLE;
   //{{AFX_DATA_INIT(CListPage)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
   m_pFuncList = NULL;
}

CListPage::~CListPage()
{
}

void CListPage::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CListPage)
   DDX_Control(pDX, IDC_NAME_LIST, m_ListBox);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListPage, CPropertyPage)
   //{{AFX_MSG_MAP(CListPage)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListPage message handlers

BOOL CListPage::OnInitDialog() 
{
   CPropertyPage::OnInitDialog();

   ASSERT(m_pFuncList);

   SetWindowText(m_Name);
   
   POSITION pos = m_pFuncList->GetHeadPosition();
   while (pos) {
      m_ListBox.AddString(m_pFuncList->GetNext(pos));
   }
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CListPage::OnSetActive() 
{
   GetParent()->GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
   
   return CPropertyPage::OnSetActive();
}
