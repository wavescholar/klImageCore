
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

#ifndef __SERVICE_H__
#define __SERVICE_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _FULLDIAG
#pragma message("  service.h")
#endif


#ifndef _WINSVC_
#include "winsvc.h"
#endif
#ifndef __AFXWIN_H__
#include "afxwin.h"
#endif
#ifndef __EXCEPT_H__
#include "except.h"
#endif
#include "resource.h"




/////////////////////////////////////////////////////////////////////////////
// CService represent Win32 service

class CService : public CObject
{
private:
  CString   m_string;
  SC_HANDLE m_hService;

public:
  CService();
  virtual ~CService();

  BOOL InstallService(LPCSTR pcszServiceName);
  BOOL UninstallService(void);

private:
  BOOL Stop(void);

  DECLARE_SERIAL(CService)
};


#endif // __SERVICE_H__
