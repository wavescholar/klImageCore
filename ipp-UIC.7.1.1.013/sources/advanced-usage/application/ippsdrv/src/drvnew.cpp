
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
#ifndef __DRVNEW_H__
#include "drvnew.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_SERIAL(CNewDriver,CDriver,1)


CNewDriver::CNewDriver()
{
  TRACE("-> CNewDriver constructor\n");

  memset(&m_ioBuffer,0,sizeof(IPPS_DEVICE_IOBUFFER));

  TRACE("<- CNewDriver constructor\n");

  return;
} // ctor


CNewDriver::~CNewDriver()
{
  TRACE("-> CNewDriver destructor\n");

  TRACE("<- CNewDriver destructor\n");
  return;
} // dtor




CString& CNewDriver::GetLibName(void)
{
  m_LibName.Format("%s",m_ioBuffer.LibName);
  return m_LibName;
} // CNewDriver::GetLibName()


CString& CNewDriver::GetLibVers(void)
{
  m_LibVers.Format("%s",m_ioBuffer.LibVers);
  return m_LibVers;
} // CNewDriver::GetLibVers()


CString& CNewDriver::GetLibBild(void)
{
  m_LibBild.Format("%s",m_ioBuffer.LibBild);
  return m_LibBild;
} // CNewDriver::GetLibBild()


float CNewDriver::GetNorma(void)
{
  return m_ioBuffer.fNorma;
} // CNewDriver::GetNorma()


DWORD CNewDriver::GetError(void)
{
  return m_ioBuffer.Error;
} // CNewDriver::GetError()


void CNewDriver::DevGetLibVersion(void)
{
  TRACE("-> CNewDriver::DevGetLibVersion()\n");

  BOOL  bres  = FALSE;
  DWORD count = 0;
  HANDLE hDevice;
  DWORD size  = sizeof(IPPS_DEVICE_IOBUFFER);

  hDevice = GetDeviceHandle();

  bres = ::DeviceIoControl(
    hDevice,
    (ULONG)IOCTL_IPPSDRV_GET_LIB_VERSION,
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

  TRACE("<- CNewDriver::DevGetLibVersion()\n");

  return;
} // CNewDriver::DevGetLibVersion()


void CNewDriver::StartFFT(int Length,int typeCore)
{
  TRACE("-> CNewDriver::StartFFT()\n");

  BOOL  bres  = FALSE;
  DWORD count = 0;
  HANDLE hDevice;
  DWORD size  = sizeof(IPPS_DEVICE_IOBUFFER);

  m_ioBuffer.Length = Length;
  m_ioBuffer.Type   = typeCore;

  hDevice = GetDeviceHandle();

  bres = ::DeviceIoControl(
    hDevice,
    (ULONG)IOCTL_IPPSDRV_START_FFT,
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

  TRACE("<- CNewDriver::StartFFT()\n");

  return;
} // CNewDriver::StartFFT()



