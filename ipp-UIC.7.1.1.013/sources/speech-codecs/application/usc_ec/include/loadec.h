/*////////////////////////////////////////////////////////////////////////
//
// INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
// Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//   Intel(R)  Integrated Performance Primitives
//
//     USC Echo Canceller sample
//
// By downloading and installing this sample, you hereby agree that the
// accompanying Materials are being provided to you under the terms and
// conditions of the End User License Agreement for the Intel(R) Integrated
// Performance Primitives product previously accepted by you. Please refer
// to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
// product installation for more information.
//
// Purpose: Load static EC components.
//
*/

#ifndef __LOADEC_H__
#define __LOADEC_H__

#include "usc_ec.h"
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

   USCFUN USC_EC_Fxns USC_ECFP_Fxns;
typedef struct _StaticNameTableEC {
    char ECName[64];
    void *funcPtr;
}StaticNameTableEC;

extern StaticNameTableEC LinkedEC[];

void EnumerateStaticLinkedEC(FILE *fptrLog);
int GetNumLinkedEC(void);
int LoadECByName(const char *name, USC_EC_Params *params, FILE *f_log);
void* GetObjectEC(const char *name);
int GetNumLinkedEC(void);

#ifdef __cplusplus
}
#endif

#endif/* __LOADEC_H__ */
