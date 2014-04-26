
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

#ifndef __IPPSAPP_H__
#define __IPPSAPP_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _FULLDIAG
#pragma message("  ippsapp.h")
#endif


#ifndef __AFXWIN_H__
#include "afxwin.h"
#endif
#ifndef __DRVDLG_H__
#include "drvdlg.h"
#endif
#ifndef __EXCEPT_H__
#include "except.h"
#endif
#include "resource.h"




/////////////////////////////////////////////////////////////////////////////
// CApp:
// See ippsdrvnt.cpp for the implementation of this class
//

class CApp : public CWinApp
{
public:
  CApp();

  //{{AFX_VIRTUAL(CApp)
  public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL


  //{{AFX_MSG(CApp)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // __IPPSAPP_H__
