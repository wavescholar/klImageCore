
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1998-2012 Intel Corporation. All Rights Reserved.
//
//
//
*/

#ifndef __ABOUTDLG_H__
#define __ABOUTDLG_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _FULLDIAG
#pragma message("  aboutdlg.h")
#endif


#include "stdafx.h"
#ifndef __AFXWIN_H__
#include "afxwin.h"
#endif
#include "resource.h"




/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA

  //{{AFX_VIRTUAL(CAboutDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:
  //{{AFX_MSG(CAboutDlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


#endif // __ABOUTDLG_H__

