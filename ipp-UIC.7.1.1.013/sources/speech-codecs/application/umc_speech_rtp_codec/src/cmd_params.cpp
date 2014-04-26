//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2012 Intel Corporation. All Rights Reserved.
//
//
//
#include "cmd_params.h"
#include "vm_strings.h"

Ipp32s params_fill_by_default(sProgramParameters * p_param)
{
   memset((void*)p_param,0,sizeof(sProgramParameters));

   p_param->bitrate = 0;
   p_param->isVad = 0;
   p_param->btstrmFmt = 0;
   p_param->nFrmInPkcts = 1;
   p_param->DecSampleFreq = -1;
   return 0;
}

Ipp32s
params_parse_command_line(sProgramParameters * p_param, Ipp32s argc, vm_char* argv[])
{
   Ipp32s i;

   if (argc < 2 ) return 1;

   for (i = 1; i < argc; i ++) {
      if (argv[i][0] == '-') {
         if (argv[i][1] == '-') {
         } else {
            /// Program specific command line parameters!
            if (!vm_string_strcmp(&argv[i][1],VM_STRING("format"))) {
               i++;
               vm_string_strcpy(p_param->codec_name, argv[i]);
            } else if (argv[i][1] == 'r') {
               p_param->bitrate = vm_string_atoi(&argv[i][2]);
            } else if (!vm_string_strcmp(&argv[i][1],VM_STRING("list"))) {
               p_param->enumerate = 1;
            } else if (argv[i][1] == 'v') {
               p_param->isVad = vm_string_atoi(&argv[i][2]);
               if(p_param->isVad==0) p_param->isVad++;
            } else if (argv[i][1] == 'b') {
               p_param->btstrmFmt = vm_string_atoi(&argv[i][2]);
               if(p_param->btstrmFmt<0) p_param->btstrmFmt = 0;
               if(p_param->btstrmFmt>1) p_param->btstrmFmt = 1;
            } else if (argv[i][1] == 'p') {
               p_param->nFrmInPkcts = vm_string_atoi(&argv[i][2]);
               if(p_param->nFrmInPkcts<=0) p_param->nFrmInPkcts = 1;
            } else if (argv[i][1] == 'f') {
               p_param->DecSampleFreq = vm_string_atoi(&argv[i][2]);
            }
         }
      } else {
         if (p_param->input_file[0] != 0 )
            vm_string_strcpy(p_param->output_file, argv[i]);
         else
            vm_string_strcpy(p_param->input_file, argv[i]);
      }
   }

   if (!p_param->output_file[0] && (i < argc)) {
      vm_string_strncpy(p_param->output_file, p_param->input_file, vm_string_strlen(p_param->input_file)-3);
      vm_string_strcat(p_param->output_file, VM_STRING("rtpdump") );
   }

   return 0;
}

void
params_print_usage(vm_char *pAppName)
{
   vm_string_printf(VM_STRING("Usage : %s <options> <inFile> <outFile>\n"),pAppName);
   vm_string_printf(VM_STRING("  options:\n"));
   vm_string_printf(VM_STRING("    [-format CodecName] - Set codec. \n"));
   vm_string_printf(VM_STRING("    [-r<num>]           - desired bitrate\n"));
   vm_string_printf(VM_STRING("    [-b<num>]           - RTP payload type\n"));
   vm_string_printf(VM_STRING("    [-p<num>]           - Number of frames in the RTP packet\n"));
   vm_string_printf(VM_STRING("    [-f<num>]           - Sample frequency for decoder with multy PCM types. (optional)\n"));
   vm_string_printf(VM_STRING("    [-v]                - Enable VAD mode\n"));
   vm_string_printf(VM_STRING("    [-list]             - Enumerate codecs\n"));
   vm_string_printf(VM_STRING("\n"));
}

