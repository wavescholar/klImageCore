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
#ifndef __UTIL_H__
#define __UTIL_H__

#include "usc_td.h"
#include "vm_file.h"

#define MAX_FILENAME_LEN 256
#define MAX_STRING_LEN 256

typedef enum {
   Empty = -32768,
   DTMF=-1,
   ANS=0,
} TD_Type;

typedef struct _CommandLineParams {
   vm_char inputFileName[MAX_FILENAME_LEN];
   vm_char outputFileName[MAX_FILENAME_LEN];
   vm_char logFileName[MAX_FILENAME_LEN];
   vm_char tone_succession[MAX_FILENAME_LEN];
   int  optionReport;
   int  TDoption;
   int  lengthTone;
   int  lengthPause;
   int  volume;
   int  puttologfile;
   int  sample_frequency;
   int  frame_size;
   TD_Type  type;
}CommandLineParams;

typedef struct _UsedObjects {
   vm_file *f_in;
   vm_file *f_out;
   vm_file *f_log;
   char *pInputBuffer;
   char *pNoiseBuffer;
   int nBanks;
   USC_MemBank* pBanks;
   USC_TD_Info *pInfo;
}UsedObjects;

void PrintUsage(vm_char* pAppName, vm_file *flog);

void ReadCommandLine(CommandLineParams *params, int argc, vm_char *argv[]);
void SetCommandLineByDefault(CommandLineParams *params);
void USC_OutputString(vm_file *flog, vm_char *format,...);

void InitUsedObjects(UsedObjects *pObjects);
void ReleaseUsedObjects(UsedObjects *pObjects);

void USC_ToneId2Char(USC_ToneID tID, vm_char *pDstString);
void USC_ParseToneStr(vm_char *pTone, USC_ToneID *tID, int remainLen, int *pUsedLen);

#if defined (_BIG_ENDIAN)
void SwapBytes_16u(unsigned short *pSrc,int len);
#endif

#endif/* __UTIL_H__*/
