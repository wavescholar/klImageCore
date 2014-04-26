//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2012 Intel Corporation. All Rights Reserved.
//
//
//
#ifndef __CMD_PARAMS_H__
#define __CMD_PARAMS_H__

#include "vm_types.h"

#define MAX_STRING_PARAM_LEN 256

typedef struct
{
   vm_char input_file[MAX_STRING_PARAM_LEN];
   vm_char output_file[MAX_STRING_PARAM_LEN];
   vm_char codec_name[MAX_STRING_PARAM_LEN];
   Ipp32s  bitrate;
   Ipp32s  enumerate;
   Ipp32s  isVad;
   Ipp32s  btstrmFmt;
   Ipp32s  nFrmInPkcts;
   Ipp32s  DecSampleFreq;
} sProgramParameters;

Ipp32s  params_parse_command_line(sProgramParameters * p_param, Ipp32s argc, vm_char* argv[]);
void params_print_usage(vm_char *pAppName);
Ipp32s  params_fill_by_default(sProgramParameters * p_param);

#endif//__CMD_PARAMS_H__
