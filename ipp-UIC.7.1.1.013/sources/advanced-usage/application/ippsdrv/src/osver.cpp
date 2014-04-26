
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
#ifndef __OSVER_H__
#include "osver.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_SERIAL(COSVersion,CObject,1)


COSVersion::COSVersion()
{
  memset((LPVOID)&m_OsVersion,0,sizeof(OSVERSIONINFO));
  return;
} // ctor




void COSVersion::Initialize(void)
{
  BOOL bres = FALSE;

  TRY

  m_OsVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

  bres = GetVersionEx((OSVERSIONINFO *)&m_OsVersion);
  THROW_IF(FALSE==bres,"GetVersionEx() failed!");

  CATCH(CMyException,exception)

  exception->ReportError();

  END_CATCH

  return;
} // COSVersion::Initialize()




DWORD COSVersion::GetMajorVersion(void)
{
  return m_OsVersion.dwMajorVersion;
} // COSVersion::GetMajorVersion()


DWORD COSVersion::GetMinorVersion(void)
{
  return m_OsVersion.dwMinorVersion;
} // COSVersion::GetMinorVersion()


DWORD COSVersion::GetBuildNumber(void)
{
  DWORD build;

  if(VER_PLATFORM_WIN32_WINDOWS == m_OsVersion.dwPlatformId)
  {
    build = (m_OsVersion.dwBuildNumber) & 0x0000FFFFL;
  } else {
    build = m_OsVersion.dwBuildNumber;
  }

  return build;
} // COSVersion::GetBuildNumber()


DWORD COSVersion::GetPlatformID(void)
{
  return m_OsVersion.dwPlatformId;
} // COSVersion::GetBuildNumber()

DWORD COSVersion::GetProductType(void)
{
   return m_OsVersion.wProductType;
} // COSVersion::GetProductType()


CString& COSVersion::GetOsNameStr(void)
{
  LPTSTR temp;

  switch(GetPlatformID())
  {
    case VER_PLATFORM_WIN32_NT:
      // Test for the specific product family.
       if((GetMajorVersion() == 6) && (GetMinorVersion() == 1)) {
          if( GetProductType() == VER_NT_WORKSTATION ) {
             temp = "Windows 7";
          } else {
             temp = "Windows Server 2008 R2";
          }
       }

       if((GetMajorVersion() == 6) && (GetMinorVersion() == 0)) {
          if( GetProductType() == VER_NT_WORKSTATION ) {
             temp = "Windows Vista";
          } else {
             temp = "Windows Server 2008";
          }
       }
     
      if((GetMajorVersion() == 5) && (GetMinorVersion() == 2))
        temp = "Windows Server 2003 family";

      if((GetMajorVersion() == 5) && (GetMinorVersion() == 1))
        temp = "Windows XP";

      if((GetMajorVersion() == 5) && (GetMinorVersion() == 0))
        temp = "Windows 2000";

      if(GetMajorVersion() <= 4)
        temp = "Windows NT";
      break;

    case VER_PLATFORM_WIN32_WINDOWS:
      if((GetMajorVersion() == 4) && (GetMinorVersion() == 0))
        temp = "Windows 95";

      if((GetMajorVersion() == 4) && (GetMinorVersion() == 10))
        temp = "Windows 98";

      if((GetMajorVersion() == 4) && (GetMinorVersion() == 90))
        temp = "Windows Millennium Edition";
      break;

    case VER_PLATFORM_WIN32s:
      temp = "Windows 3.x with Win32s";
      break;

    default:
      temp = "Unknown";
      break;
  }

  m_OsNameStr.Format("%s",temp);

  return m_OsNameStr;
} // COSVersion::GetOsNameStr()


CString& COSVersion::GetOsVersStr(void)
{
  m_OsVersStr.Format("%d.%d",GetMajorVersion(),GetMinorVersion());
  return m_OsVersStr;
} // COSVersion::GetOsVersStr()


CString& COSVersion::GetOsBildStr(void)
{
  m_OsBildStr.Format("%d",GetBuildNumber());
  return m_OsBildStr;
} // COSVersion::GetOsBildStr()


CString& COSVersion::GetOsAddsStr(void)
{
  m_OsAddsStr.Format("%s",m_OsVersion.szCSDVersion);
  return m_OsAddsStr;
} // COSVersion::GetOsAddsStr()


