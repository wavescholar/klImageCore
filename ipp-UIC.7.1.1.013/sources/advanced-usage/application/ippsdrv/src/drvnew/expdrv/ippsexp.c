
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

#include "ntddk.h"
#include "stdarg.h"
#include "stdio.h"

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef _IPPS_H__
#include "ipps.h"
#endif
#define __DRIVER__
#include "ippsexp.h"




/*
//
//  Routine Description:
//
//    Installable driver initialization entry point.
//    This entry point is called directly by the I/O system.
//
//  Arguments:
//
//    DriverObject - pointer to the driver object
//
//    RegistryPath - pointer to a unicode string representing the path
//                   to driver-specific key in the registry
//
//  Return Value:
//
//    STATUS_SUCCESS      if successful,
//    STATUS_UNSUCCESSFUL otherwise
//
*/

NTSTATUS
DriverEntry(
  IN PDRIVER_OBJECT  DriverObject,
  IN PUNICODE_STRING RegistryPath)
{

  NTSTATUS ntStatus = STATUS_SUCCESS;


  TRACE(("IPPSEXP.SYS: entering DriverEntry (Build: %s/%s\n",__DATE__,__TIME__));

  // do nothing
  TRACE(("IPPSEXP.SYS: leaving DriverEntry\n"));
  return ntStatus;
}  // DriverEntry()

