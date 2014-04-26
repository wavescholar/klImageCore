
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

#ifndef __IPPSEXP_H__
#define __IPPSEXP_H__


#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef _IPPS_H__
#include "ipps.h"
#endif


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


#endif //  __DRIVER__


#endif // __IPPSEXP_H__

