
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1998-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "ntddk.h"
#include "devioctl.h"
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
#include "ippsold.h"




static WCHAR deviceNameBuffer[] = L"\\Device\\IPPSOLD";
static WCHAR deviceLinkBuffer[] = L"\\DosDevices\\IPPSOLD";


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

  NTSTATUS                  ntStatus            = STATUS_SUCCESS;
  PDEVICE_OBJECT            deviceObject        = NULL;
  PIPPSOLD_DEVICE_EXTENSION deviceExtension     = NULL;
  UNICODE_STRING            deviceNameUnicodeString;
  UNICODE_STRING            deviceLinkUnicodeString;

//  if ( ippStaticInit() )
//  {
    TRACE(("IPPSOLD.SYS: entering DriverEntry (Build: %s/%s\n",__DATE__,__TIME__));


    //
    // A real driver would:
    //
    //     1. Report it's resources (IoReportResourceUsage)
    //
    //     2. Attempt to locate the device(s) it supports


    //
    // OK, we've claimed our resources & found our h/w, so create
    // a device and initialize stuff...
    //

    RtlInitUnicodeString(&deviceNameUnicodeString,deviceNameBuffer);


    //
    // Create an EXCLUSIVE device, i.e. only 1 thread at a time can send
    // i/o requests.
    //

    ntStatus = IoCreateDevice(
      DriverObject,
      sizeof (IPPSOLD_DEVICE_EXTENSION),
      &deviceNameUnicodeString,
      FILE_DEVICE_IPPSOLD,
      0,
      FALSE,
//      TRUE, // exclusive device
      &deviceObject);

    if(FALSE == NT_SUCCESS(ntStatus))
    {
      TRACE(("IPPSOLD.SYS: IoCreateDevice failed [0x%08X]\n",ntStatus));
      goto Exit;
    }


    deviceExtension = deviceObject->DeviceExtension;


    //
    // Set up synchronization objects, state info,, etc.
    //


    //
    // Create a symbolic link that Win32 apps can specify to gain access
    // to this driver/device
    //

    RtlInitUnicodeString(&deviceLinkUnicodeString,deviceLinkBuffer);

    ntStatus = IoCreateSymbolicLink(
      &deviceLinkUnicodeString,
      &deviceNameUnicodeString);


    if(FALSE == NT_SUCCESS(ntStatus))
    {
      TRACE(("IPPSOLD.SYS: IoCreateSymbolicLink failed [0x%08X]\n",ntStatus));
      goto Exit;
    }



    //
    // Create dispatch points for device control, create, close.
    //

    DriverObject->MajorFunction[IRP_MJ_CREATE]         = ippsDrvDispatch;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]          = ippsDrvDispatch;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ippsDrvDispatch;
    DriverObject->DriverUnload                         = ippsDrvUnload;

    ippStaticInit();

Exit:

    if(FALSE == NT_SUCCESS(ntStatus))
    {
      //
      // Something went wrong, so clean up (free resources, etc.)
      //

      if(NULL != deviceObject)
      {
        IoDeleteDevice(deviceObject);
      }
//    }

    TRACE(("IPPSOLD.SYS: leaving DriverEntry\n"));
  }

  return ntStatus;
}  // DriverEntry()




/*
//
//  Routine Description:
//
//    Process the IRPs sent to this device.
//
//  Arguments:
//
//    DeviceObject - pointer to a device object
//
//    Irp          - pointer to an I/O Request Packet
//
//  Return Value:
//
//
*/

NTSTATUS
ippsDrvDispatch(
  IN PDEVICE_OBJECT DeviceObject,
  IN PIRP           Irp)
{
  NTSTATUS                 ntStatus        = STATUS_SUCCESS;
  PIO_STACK_LOCATION       irpStack        = NULL;
  PIPPSOLD_DEVICE_EXTENSION deviceExtension = NULL;


  TRACE(("IPPSOLD.SYS: entering in ippsDrvDispatch\n"));

  Irp->IoStatus.Status      = STATUS_SUCCESS;
  Irp->IoStatus.Information = 0;


  //
  // Get a pointer to the current location in the Irp. This is where
  // the function codes and parameters are located.
  //

  irpStack = IoGetCurrentIrpStackLocation(Irp);


  //
  // Get a pointer to the device extension
  //

  deviceExtension = DeviceObject->DeviceExtension;


  switch(irpStack->MajorFunction)
  {
    case IRP_MJ_CREATE:

      TRACE(("IPPSOLD.SYS: IRP_MJ_CREATE\n"));

      break;

    case IRP_MJ_CLOSE:

      TRACE(("IPPSOLD.SYS: IRP_MJ_CLOSE\n"));

      break;

    case IRP_MJ_DEVICE_CONTROL:

      TRACE(("IPPSOLD.SYS: IRP_MJ_DEVICE_CONTROL\n"));

      ippsDrvProcessIOCTL(deviceExtension,Irp);

      break;

    default:

      TRACE(("IPPSOLD.SYS: other IRP_MJ_XXX\n"));

      break;

  } // switch(irpStack->MajorFunction)


  //
  // DON'T get cute and try to use the status field of
  // the irp in the return status.  That IRP IS GONE as
  // soon as you call IoCompleteRequest.
  //

  ntStatus = Irp->IoStatus.Status;

  IoCompleteRequest(Irp,IO_NO_INCREMENT);

  //
  // We never have pending operation so always return the status code.
  //

  TRACE(("IPPSOLD.SYS: leaving ippsDrvDispatch\n"));

  return ntStatus;
}  // ippsDrvDispatch()




/*
//
//  Routine Description:
//
//    Free all the allocated resources, etc.
//
//  Arguments:
//
//    DriverObject - pointer to a driver object
//
//  Return Value:
//    void
//
*/

VOID
ippsDrvUnload(
  IN PDRIVER_OBJECT DriverObject)
{
  UNICODE_STRING deviceLinkUnicodeString;


  TRACE(("IPPSOLD.SYS: entering in ippsDrvUnload\n"));

  //
  // Free any resources
  //


  //
  // Delete the symbolic link
  //

  RtlInitUnicodeString(&deviceLinkUnicodeString,deviceLinkBuffer);

  IoDeleteSymbolicLink(&deviceLinkUnicodeString);


  //
  // Delete the device object
  //

  IoDeleteDevice(DriverObject->DeviceObject);

  TRACE(("IPPSOLD.SYS: leaving ippsDrvDispatch\n"));

  return;
}  // ippsDrvUnload()

