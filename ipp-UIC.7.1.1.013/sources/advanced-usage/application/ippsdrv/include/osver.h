
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

#ifndef __OSVER_H__
#define __OSVER_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _FULLDIAG
#pragma message("  osver.h")
#endif


#ifndef __AFXWIN_H__
#include "afxwin.h"
#endif
#ifndef __EXCEPT_H__
#include "except.h"
#endif




/////////////////////////////////////////////////////////////////////////////
// COSVersion represent OS version info

class COSVersion : public CObject
{
private:
  CString       m_OsNameStr;
  CString       m_OsVersStr;
  CString       m_OsBildStr;
  CString       m_OsAddsStr;
  OSVERSIONINFOEX m_OsVersion;

public:
  COSVersion();

  DWORD GetMajorVersion(void);
  DWORD GetMinorVersion(void);
  DWORD GetBuildNumber(void);
  DWORD GetPlatformID(void);
  DWORD GetProductType(void);

  CString& GetOsNameStr(void);
  CString& GetOsVersStr(void);
  CString& GetOsBildStr(void);
  CString& GetOsAddsStr(void);

  void Initialize(void);

  DECLARE_SERIAL(COSVersion)
};


#endif // __OSVER_H__
