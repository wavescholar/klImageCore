/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AC3_STATUS_H__
#define __AC3_STATUS_H__

#ifdef __cplusplus
extern "C" {
#endif

  typedef enum
  {
      AC3_OK = 0,
      AC3_NOT_ENOUGH_DATA,
      AC3_FLAGS_ERROR,
      AC3_ALLOC,
      AC3_BAD_STREAM,
      AC3_NULL_PTR,
      AC3_NOT_FIND_SYNCWORD,
      AC3_NOT_ENOUGH_BUFFER,
      AC3_BAD_PARAMETER,
      AC3_UNSUPPORTED
  } AC3Status;

#ifdef __cplusplus
}
#endif

#endif
