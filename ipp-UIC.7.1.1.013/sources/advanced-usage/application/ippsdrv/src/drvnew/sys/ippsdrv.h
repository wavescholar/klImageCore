
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

#ifndef __IPPSDRV_H__
#define __IPPSDRV_H__


#include "ippcore.h"
#ifndef __IPPS_H__
#include "ipps.h"
#endif




//
//   A structure representing the input-output buffer for IOCTLs
//   commands ippsdrv device
//

typedef struct _IPPS_DEVICE_IOBUFFER
{
  float          fNorma;
  int            Type;
  int            Length;
  ULONG          Error;
  CHAR           LibName[32];
  CHAR           LibVers[32];
  CHAR           LibBild[32];
} IPPS_DEVICE_IOBUFFER, *PIPPS_DEVICE_IOBUFFER;


#ifdef __DRIVER__


//
//   The following is the debug print macro- when we are building checked
//   drivers "DBG" will be defined (by the \ddk\setenv.cmd script), and we
//   will see debug messages appearing on the KD screen on the host debug
//   machine. When we build free drivers "DBG" is not defined, and calls
//   to TRACE are removed.
//

#ifdef DBG
#define TRACE(arg) DbgPrint arg
#else
#define TRACE(arg)
#endif




//
//   A structure representing the instance information associated with
//   a particular device
//

typedef struct _IPPS_DEVICE_EXTENSION
{
  IppLibraryVersion* LibVersion;
  IppStatus      status;
} IPPS_DEVICE_EXTENSION, *PIPPS_DEVICE_EXTENSION;





NTSTATUS
ippsDrvDispatch(
  IN PDEVICE_OBJECT DeviceObject,
  IN PIRP           Irp);


VOID
ippsDrvProcessIOCTL(
  IN PIPPS_DEVICE_EXTENSION deviceExtension,
  IN PIRP                  Irp);


VOID
ippsDrvUnload(
  IN PDRIVER_OBJECT DriverObject);


VOID
ippsDrvGetVersion(
  IN PIPPS_DEVICE_IOBUFFER ioBuffer);


VOID
ippsDrvDoFFT(
  IN PIPPS_DEVICE_IOBUFFER ioBuffer);


#endif //  __DRIVER__




//
//   Define the various device type values. Note that values used by Microsoft
//   Corporation are in the range 0-32767, and 32768-65535 are reserved for use
//   by customers.
//

#define FILE_DEVICE_IPPSDRV  0x00008003




//
//   Macro definition for defining IOCTL and FSCTL function control codes.  Note
//   that function codes 0-2047 are reserved for Microsoft Corporation, and
//   2048-4095 are reserved for customers.
//

#define IPPSDRV_IOCTL_INDEX  0x803




//
//             The ippsdrv device driver IOCTL Definitions
//
//   User mode applications wishing to send IOCTLs to a kernel mode driver
//   must use this file to set up the correct type of IOCTL code permissions.
//
//   Note: this file depends on the file DEVIOCTL.H which contains the macro
//         definition for "CTL_CODE" below. Include that file before you
//         include this one in your source code.
//

#define IOCTL_IPPSDRV_GET_LIB_VERSION \
  CTL_CODE(FILE_DEVICE_IPPSDRV,       \
           (IPPSDRV_IOCTL_INDEX + 1), \
           METHOD_BUFFERED,          \
           FILE_ANY_ACCESS)

#define IOCTL_IPPSDRV_START_FFT       \
  CTL_CODE(FILE_DEVICE_IPPSDRV,       \
           (IPPSDRV_IOCTL_INDEX + 2), \
           METHOD_BUFFERED,          \
           FILE_ANY_ACCESS)


#endif // __IPPSDRV_H__

