
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
#ifndef __DRVOLD_H__
#include "drvold.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_SERIAL(COldDriver,CDriver,1)


COldDriver::COldDriver()
{
  TRACE("-> COldDriver constructor\n");

  memset(&m_ioBuffer,0,sizeof(IPPSOLD_DEVICE_IOBUFFER));

  TRACE("<- COldDriver constructor\n");

  return;
} // ctor


COldDriver::~COldDriver()
{
  TRACE("-> COldDriver destructor\n");

  TRACE("<- COldDriver destructor\n");
  return;
} // dtor




CString& COldDriver::GetLibName(void)
{
  m_LibName.Format("%s",m_ioBuffer.LibName);
  return m_LibName;
} // COldDriver::GetLibName()


CString& COldDriver::GetLibVers(void)
{
  m_LibVers.Format("%s",m_ioBuffer.LibVers);
  return m_LibVers;
} // COldDriver::GetLibVers()


CString& COldDriver::GetLibBild(void)
{
  m_LibBild.Format("%s",m_ioBuffer.LibBild);
  return m_LibBild;
} // COldDriver::GetLibBild()


float COldDriver::GetNorma(void)
{
  return m_ioBuffer.fNorma;
} // COldDriver::GetNorma()


DWORD COldDriver::GetError(void)
{
  return m_ioBuffer.Error;
} // COldDriver::GetError()


void COldDriver::DevGetLibVersion(void)
{
  TRACE("-> COldDriver::DevGetLibVersion()\n");

  BOOL   bres  = FALSE;
  DWORD  count = 0;
  HANDLE hDevice;
  DWORD  size  = sizeof(IPPSOLD_DEVICE_IOBUFFER);

  hDevice = GetDeviceHandle();

  bres = ::DeviceIoControl(
    hDevice,
    (ULONG)IOCTL_IPPSOLD_GET_LIB_VERSION,
    &m_ioBuffer,
    size,
    &m_ioBuffer,
    size,
    &count,
    NULL);

  if(FALSE == bres)
  {
    m_strError.Format(IDS_E_DCTRL,hDevice);
    THROW(m_strError);
  }

  TRACE("<- COldDriver::DevGetLibVersion()\n");

  return;
} // COldDriver::DevGetLibVersion()


void COldDriver::StartFFT(int Length,int typeCore)
{
  TRACE("-> COldDriver::StartFFT()\n");

  BOOL  bres  = FALSE;
  DWORD count = 0;
  HANDLE hDevice;
  DWORD size  = sizeof(IPPSOLD_DEVICE_IOBUFFER);

  m_ioBuffer.Length = Length;
  m_ioBuffer.Type   = typeCore;

  hDevice = GetDeviceHandle();

  bres = ::DeviceIoControl(
    hDevice,
    (ULONG)IOCTL_IPPSOLD_START_FFT,
    &m_ioBuffer,
    size,
    &m_ioBuffer,
    size,
    &count,
    NULL);

  if(FALSE == bres)
  {
    m_strError.Format(IDS_E_DCTRL,hDevice);
    THROW(m_strError);
  }

  TRACE("<- COldDriver::StartFFT()\n");

  return;
} // COldDriver::StartFFT()



