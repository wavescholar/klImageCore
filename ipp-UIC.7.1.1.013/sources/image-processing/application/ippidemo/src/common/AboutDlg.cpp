/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// AboutDlg.cpp : implementation of the About dialog.
// Command: Menu-Help-About IPPSP Demo
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
   m_Copyright = _T("");
   m_DemoName = _T("");
   //}}AFX_DATA_INIT

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
   DDX_Text(pDX, IDC_COPYRIGHT, m_Copyright);
   DDX_Text(pDX, IDC_DEMO_NAME, m_DemoName);
   //}}AFX_DATA_MAP
   if (DEMO_APP->GetNumLibs() > 0) {
      DDX_Text(pDX, IDC_LIB_TITLE_0,m_LibTitle[0]);
      DDX_Text(pDX, IDC_LIB_DATE_0, m_LibDate[0]);
      DDX_Text(pDX, IDC_LIB_NAME_0, m_LibName[0]);
      DDX_Text(pDX, IDC_LIB_VERS_0, m_LibVers[0]);
   }
   if (DEMO_APP->GetNumLibs() > 1) {
      DDX_Text(pDX, IDC_LIB_TITLE_1,m_LibTitle[1]);
      DDX_Text(pDX, IDC_LIB_DATE_1, m_LibDate[1]);
      DDX_Text(pDX, IDC_LIB_NAME_1, m_LibName[1]);
      DDX_Text(pDX, IDC_LIB_VERS_1, m_LibVers[1]);
   }
   if (DEMO_APP->GetNumLibs() > 2) {
      DDX_Text(pDX, IDC_LIB_TITLE_2,m_LibTitle[2]);
      DDX_Text(pDX, IDC_LIB_DATE_2, m_LibDate[2]);
      DDX_Text(pDX, IDC_LIB_NAME_2, m_LibName[2]);
      DDX_Text(pDX, IDC_LIB_VERS_2, m_LibVers[2]);
   }
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
   SetWindowText(_T("About ") + DEMO_APP->GetDemoTitle() + _T(" Demo"));
   m_DemoName = DEMO_APP->GetDemoTitle() 
              + _T(" Demo  Version ") 
              + DEMO_APP->GetVersionString();

   for (int idx = 0; idx < DEMO_APP->GetNumLibs(); idx++) {
      m_LibTitle[idx] = DEMO_APP->GetLibTitle(idx) + _T(" Library");
      const IppLibraryVersion* libVer = DEMO_APP->GetLibVersion(idx);
      if (libVer) {
         m_LibName[idx] = libVer->Name;
         m_LibVers[idx] = libVer->Version;
         m_LibDate[idx] = libVer->BuildDate;
         if (m_LibVers[idx][0] == 'v') m_LibVers[idx] = m_LibVers[idx].Mid(1);
      }
   }

   CString sYear;
   int y1 = DEMO_APP->GetCopyrightFirstYear();
   int y2 = DEMO_APP->GetCopyrightLastYear();
   if (y1 < y2)
      sYear.Format("%d-%d", y1, y2);
   else
      sYear.Format("%d", y1);
   m_Copyright.Format(_T("%s%s %c %s %s %s"), 
      "Copy", "right", 0xA9, sYear, "Intel", "Corporation");

   CDialog::OnInitDialog();
   
   return TRUE; 
}
