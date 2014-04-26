
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1998-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "stdafx.h"
#ifndef __IPPSAPP_H__
#include "ippsapp.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CApp

BEGIN_MESSAGE_MAP(CApp,CWinApp)
  //{{AFX_MSG_MAP(CApp)
  //}}AFX_MSG
  ON_COMMAND(ID_HELP,CWinApp::OnHelp)
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// CApp construction

CApp::CApp(void)
{
  return;
} // ctor


/////////////////////////////////////////////////////////////////////////////
// CApp initialization

BOOL CApp::InitInstance(void)
{
  TRY

  AfxEnableControlContainer();

#ifdef _AFXDLL
  Enable3dControls();
#else
  Enable3dControlsStatic();
#endif

  CDrvDlg dlg;
  m_pMainWnd = &dlg;

  dlg.DoModal();


  CATCH(CMemoryException,e)

  e->ReportError();

  CATCH(CResourceException,e)

  e->ReportError();

  CATCH_ALL

  AfxMessageBox("Unexpected error has occured");
  exit(1);

  END_CATCH

  return FALSE;
} // CApp::InitInstance()




/////////////////////////////////////////////////////////////////////////////
// The one and only CApp object

CApp theApp;

