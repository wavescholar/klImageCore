/*////////////////////////////////////////////////////////////////////////
//
// INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
// Copyright (c) 2012 Intel Corporation. All Rights Reserved.
//
//   Intel(R)  Integrated Performance Primitives
//
//     USC speech codec sample
//
// By downloading and installing this sample, you hereby agree that the
// accompanying Materials are being provided to you under the terms and
// conditions of the End User License Agreement for the Intel(R) Integrated
// Performance Primitives product previously accepted by you. Please refer
// to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
// product installation for more information.
//
// Purpose: Auxiliary structures and functions header file.
//
////////////////////////////////////////////////////////////////////////*/
#ifndef __TYPES_H__
#define __TYPES_H__

#include <ippdefs.h>
#include "usc_filter.h"

#define MAX_FILENAME_LEN 256

typedef struct _CommandLineParams {
   Ipp8s inputFileName[MAX_FILENAME_LEN];
   Ipp8s outputFileName[MAX_FILENAME_LEN];
   Ipp8s logFileName[MAX_FILENAME_LEN];
   Ipp8s algFilter[MAX_FILENAME_LEN];
   Ipp32s  optionReport;
   Ipp32s  puttologfile;
   Ipp32s  enumerate;
   Ipp32s  SampleFreq;
}CommandLineParams;

typedef struct _UsedObjects {
   FILE *f_in;
   FILE *f_out;
   FILE *f_log;
   Ipp8s *pInputBuffer;
   Ipp8s *pOutputBuffer;
   Ipp32s nBanks;
   USC_MemBank* pBanks;
   USC_FilterInfo *pInfo;
}UsedObjects;

void PrintUsage(Ipp8s *pAppName, FILE *flog);

void ReadCommandLine(CommandLineParams *params, Ipp32s argc, Ipp8s *argv[]);
void SetCommandLineByDefault(CommandLineParams *params);
void USC_OutputString(FILE *flog, Ipp8s *format,...);

void InitUsedObjects(UsedObjects *pObjects);
void ReleaseUsedObjects(UsedObjects *pObjects);

#if defined (_BIG_ENDIAN)
void SwapBytes_16u(Ipp16u *pSrc,Ipp32s len);
#endif

#endif/* __TYPES_H__*/
