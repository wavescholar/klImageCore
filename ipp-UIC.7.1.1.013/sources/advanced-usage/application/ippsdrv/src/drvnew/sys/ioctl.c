
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
#include "devioctl.h"
#include "stdarg.h"
#include "stdio.h"
#define __DRIVER__
#include "ippsdrv.h"




void ippsDrvProcessIOCTL(
  IN PIPPS_DEVICE_EXTENSION deviceExtension,
  IN PIRP                  Irp)
{
  PIO_STACK_LOCATION   irpStack           = NULL;
  PIPPS_DEVICE_IOBUFFER ioBuffer          = NULL;
  ULONG                ioControlCode      = 0;
  ULONG                inputBufferLength  = 0;
  ULONG                outputBufferLength = 0;


  TRACE(("IPPSDRV.SYS: entering in ippsDrvProcessIOCTL\n"));

  //
  // Get a pointer to the current location in the Irp. This is where
  // the function codes and parameters are located.
  //

  irpStack = IoGetCurrentIrpStackLocation(Irp);

  //
  // Get the pointer to the input/output buffer and it's length
  //

  ioBuffer           = (PIPPS_DEVICE_IOBUFFER)(Irp->AssociatedIrp.SystemBuffer);
  inputBufferLength  = irpStack->Parameters.DeviceIoControl.InputBufferLength;
  outputBufferLength = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
  ioControlCode      = irpStack->Parameters.DeviceIoControl.IoControlCode;

  TRACE(("inputBufferLength:  [%d]\n",inputBufferLength));
  TRACE(("outputBufferLength: [%d]\n",outputBufferLength));

  switch(ioControlCode)
  {

    case IOCTL_IPPSDRV_GET_LIB_VERSION:
      {

        TRACE(("IPPSDRV.SYS: IOCTL_IPPSDRV_GET_VERSION\n"));

        ippsDrvGetVersion(ioBuffer);

        Irp->IoStatus.Information = sizeof(IPPS_DEVICE_IOBUFFER);
        Irp->IoStatus.Status      = STATUS_SUCCESS;

        TRACE(("transferring %d bytes\n",Irp->IoStatus.Information));

        break;
      }


    case IOCTL_IPPSDRV_START_FFT:
      {

        TRACE(("IPPSDRV.SYS: IOCTL_IPPS_DRV_START_FFT\n"));

        ippsDrvDoFFT(ioBuffer);

        Irp->IoStatus.Information = sizeof(ULONG);
        Irp->IoStatus.Status      = STATUS_SUCCESS;

        break;
      }


    default:

      TRACE(("IPPSDRV.SYS: unknown IOCTL code [0x%08X]\n",ioControlCode));

      Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;

      break;

  }


  TRACE(("IPPSDRV.SYS: leaving ippsDrvProcessIOCTL\n"));

  return;
} // ippsDrvProcessIOCTL()

