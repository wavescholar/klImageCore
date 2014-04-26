
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
#ifndef __SERVICE_H__
#include "service.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




static LPCSTR pcszSlash   = "\\";
static LPCSTR pcszExt     = ".sys";

static const DWORD MAX_STOP_TRYING = 5;


IMPLEMENT_SERIAL(CService,CObject,1)


CService::CService()
{
  TRACE("-> CService constructor\n");

  m_string   = _T("");
  m_hService = NULL;

  TRACE("<- CService constructor\n");

  return;
} // ctor


CService::~CService()
{
  TRACE("-> CService destructor\n");

  UninstallService();

  TRACE("<- CService destructor\n");

  return;
} // dtor




BOOL CService::InstallService(LPCSTR pcszServiceName)
{
  DWORD     buflen;
  DWORD     count;
  BOOL      bres     = FALSE;
  LPSTR     buffer   = NULL;
  SC_HANDLE hManager = NULL;

  TRACE("-> CService::InstallService()\n");

  __try
  {
    // get size for directory name
    buflen = ::GetCurrentDirectory(0,NULL);
    if(0 == buflen)
    {
      TRACE("  can't get size of current directory name\n");
      bres = FALSE;
      __leave;
    }

    // add room for service name
    buflen += strlen(pcszSlash) + strlen(pcszServiceName) + strlen(pcszExt);

    buffer = new CHAR [buflen];
    TRACE("  allocating %d bytes at 0x%08X for current directory name\n",
      buflen,buffer);
    // Memory exceptions are thrown automatically by new

    count = ::GetCurrentDirectory(buflen,buffer);
    if(0 == count)
    {
      TRACE("  can't get current directory name\n");
      bres = FALSE;
      __leave;
    }

    // prepare full name of driver image file
    strcat(buffer,pcszSlash);
    strcat(buffer,pcszServiceName);
    strcat(buffer,pcszExt);
    TRACE("  service image: %s\n",buffer);

    hManager = ::OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if(NULL == hManager)
    {
      bres = FALSE;
      DWORD syserr = ::GetLastError();
      m_string.Format(IDS_E_SCMGR_OPEN);
      TRACE("  can't open service control manager\n");
      THROWEX(m_string,syserr);
      __leave;
    }

    // check, if service already exist...
    // if yes, delete it
    m_hService = ::OpenService(hManager,pcszServiceName,SERVICE_ALL_ACCESS);
    if(NULL != m_hService)
    {
      TRACE("  service already exist\n");
      bres = FALSE;
      __leave;
    }

    // create new service
    m_hService = ::CreateService(
      hManager,
      pcszServiceName,
      pcszServiceName,
      SERVICE_ALL_ACCESS,
      SERVICE_KERNEL_DRIVER,
      SERVICE_DEMAND_START,
      SERVICE_ERROR_NORMAL,
      buffer,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL);

    if(NULL == m_hService)
    {
      bres = FALSE;
      DWORD syserr = ::GetLastError();
      m_string.Format(IDS_E_SCREATE,buffer);
      TRACE("  can't create service %s\n",pcszServiceName);
      THROWEX(m_string,syserr);
      __leave;
    }

    TRACE("  create service %s [HANDLE: 0x%08X]\n",pcszServiceName,m_hService);

    bres = ::StartService(m_hService,0,NULL);
    if(FALSE == bres)
    {
      DWORD syserr = ::GetLastError();
      m_string.Format(IDS_E_SSTART,buffer);
      TRACE("  can't start service %s\n",buffer);
      THROWEX(m_string,syserr);
      __leave;
    }

    bres = TRUE;
  }

  __finally
  {
    if(NULL != buffer)
    {
      TRACE("  freeing memory at 0x%08X\n",buffer);
      delete buffer;
    }

    if(FALSE == bres)
    {
      UninstallService();
    }

    if(NULL != hManager)
    {
      ::CloseServiceHandle(hManager);
    }

  }

  TRACE("<- CService::InstallService()\n");

  return bres;
} // CService::InstallService()


BOOL CService::UninstallService()
{
  BOOL bres;

  TRACE("-> CService::UninstallService()\n");

  if(NULL == m_hService)
  {
    TRACE("  already uninstalled\n");
    bres = TRUE;
    goto Exit;
  }

  bres = Stop();
  if(FALSE == bres)
  {
    TRACE("  can't stop service\n");
  }

  bres = ::DeleteService(m_hService);
  if(FALSE == bres)
  {
    DWORD syserr = ::GetLastError();
    m_string.Format(IDS_E_SCMGR_OPEN);
    TRACE("  can't delete service\n");
    THROWEX(m_string,syserr);
  }

  bres = ::CloseServiceHandle(m_hService);
  if(FALSE == bres)
  {
    DWORD syserr = ::GetLastError();
    m_string.Format(IDS_E_SCMGR_OPEN);
    TRACE("  can't close service handle\n");
    THROWEX(m_string,syserr);
  }

  m_hService = NULL;

Exit:

  TRACE("<- CService::UninstallService()\n");

  return bres;
} // CService::UninstallService()


BOOL CService::Stop(void)
{
  BOOL           bres;
  DWORD          count;
  SERVICE_STATUS serviceStatus;

  TRACE("-> CService::Stop()\n");

  count = MAX_STOP_TRYING;

  do
  {
    bres = ::QueryServiceStatus(m_hService,&serviceStatus);
    if(FALSE == bres)
    {
      bres = FALSE;
      DWORD syserr = ::GetLastError();
      m_string.Format(IDS_E_SCMGR_OPEN);
      TRACE("  can't get service status\n");
      THROWEX(m_string,syserr);
      goto Exit;
    }

    // if stop service command in progress, wait a little...
    if(SERVICE_STOP_PENDING == serviceStatus.dwCurrentState)
    {
      TRACE("  Wait for service stopped...\n");
      ::Sleep(100);
      count --;
      continue;
    }

    // if service was stopped, finishing
    if(SERVICE_STOPPED == serviceStatus.dwCurrentState)
    {
      TRACE("  Service was stopped\n");
      goto Exit;
    }

    // send stop command to service...
    bres = ::ControlService(m_hService,SERVICE_CONTROL_STOP,&serviceStatus);
    if(FALSE == bres)
    {
      bres = FALSE;
      DWORD syserr = ::GetLastError();
      m_string.Format(IDS_E_SCMGR_OPEN);
      TRACE("  can't send stop command\n");
      THROWEX(m_string,syserr);
      goto Exit;
    }

    TRACE("  Trying stop service...\n");

  } while(count != 0);

Exit:

  bres = (SERVICE_STOPPED == serviceStatus.dwCurrentState);

  TRACE("<- CService::Stop()\n");

  return bres;
} // CService::Stop()


