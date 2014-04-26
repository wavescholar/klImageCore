
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

#include "stdafx.h"
#ifndef __DRVBASE_H__
#include "drvbase.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




static LPCSTR pcszObjDir  = "\\\\.\\";


IMPLEMENT_SERIAL(CDriver,CObject,1)


CDriver::CDriver()
{
  TRACE("-> CDriver constructor\n");

  m_hDevice = INVALID_HANDLE_VALUE;

  TRACE("<- CDriver constructor\n");

  return;
} // ctor


CDriver::~CDriver()
{
  TRACE("-> CDriver destructor\n");

  CloseDevice();

  TRACE("<- CDriver destructor\n");

  return;
} // dtor




BOOL CDriver::OpenDevice(LPCSTR pcszDrvName)
{
  BOOL bres;
  char buffer[MAX_PATH];

  TRACE("-> CDriver::OpenDevice()\n");

  memset(buffer,0,MAX_PATH);
  strcat(buffer,pcszObjDir);
  strcat(buffer,pcszDrvName);

  m_hDevice = ::CreateFile(
    buffer,
    GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL);

  if(INVALID_HANDLE_VALUE == m_hDevice)
  {
    DWORD syserr = ::GetLastError();
    m_strError.Format(IDS_E_DCREATE,buffer);
    TRACE("CreateFile() failed, 0x%08X\n",m_hDevice);
    THROWEX(m_strError,syserr);
  }

  TRACE("m_hDevice: 0x%08X\nPath: %s\n",m_hDevice,buffer);

  bres = TRUE;

  TRACE("<- CDriver::OpenDevice()\n");

  return bres;
} // CDriver::OpenDevice()


BOOL CDriver::CloseDevice(void)
{
  BOOL bres;

  TRACE("-> CDriver::CloseDevice()\n");

  if(INVALID_HANDLE_VALUE == m_hDevice)
  {
    TRACE("  already closed\n");
    bres = TRUE;
    goto Exit;
  }

  TRACE("  close device handle: 0x%08X\n",m_hDevice);
  bres = ::CloseHandle(m_hDevice);
  m_hDevice = INVALID_HANDLE_VALUE;

Exit:

  TRACE("<- CDriver::CloseDevice()\n");

  return bres;
} // CDriver::CloseDevice()


HANDLE CDriver::GetDeviceHandle(void)
{
  return m_hDevice;
} // CDriver::GetDeviceHandle()

