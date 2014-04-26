/*////////////////////////////////////////////////////////////////////////
//
// INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
// Copyright (c) 2005-2012 Intel Corporation. All Rights Reserved.
//
//   Intel(R)  Integrated Performance Primitives
//
//     USC speech codec sample
//
// Purpose: Auxiliary functions file.
//
////////////////////////////////////////////////////////////////////////*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "util.h"

#include <ippcore.h>
#include <ipps.h>

void SetCommandLineByDefault(CommandLineParams *params)
{
   ippsZero_8u((Ipp8u*)params,sizeof(CommandLineParams));
   params->inputFileName[0] = '\0';
   params->logFileName[0] = '\0';
   params->tone_succession[0] = '\0';
   params->lengthPause = 20;
   params->lengthTone = 40;
   params->volume = 6;
   params->TDoption = -1;  //1 - Detection; 0 - Generation
   params->optionReport = 0;
   params->outputFileName[0] = '\0';
   params->puttologfile = 0;
   params->sample_frequency = 8000;
   params->type = Empty;
   params->frame_size = 0;
}
void ReadCommandLine(CommandLineParams *params, int argc, vm_char *argv[])
{
   argc--;
   while(argc-- > 0){ /*process command line */
      argv++;
      if ('-' == (*argv)[0]) {
         if (!vm_string_strcmp(*argv,VM_STRING("-n"))){ /* check option report mode */
            params->optionReport = 1;
            continue;
         }else if (!vm_string_strncmp(*argv,VM_STRING("-dt"), 2)){ /* get TD options: detection */
            params->TDoption = 1;
            continue;
         }else if (!vm_string_strncmp(*argv,VM_STRING("-gt"), 2)){ /* get TD options: generation */
            params->TDoption = 0;
            if (vm_string_strlen(*argv) == 3){
                argc--;
                argv++;
                vm_string_strncpy(params->tone_succession, *argv,IPP_MIN(MAX_FILENAME_LEN,vm_string_strlen(*argv)));
            } else {
                vm_string_strncpy(params->tone_succession, *argv+3,IPP_MIN(MAX_FILENAME_LEN,vm_string_strlen(*argv)));
            }
            continue;
         }else if (!vm_string_strcmp(*argv,VM_STRING("-type"))){ /* get TD type */
            argv++;
            argc--;
            if(vm_string_strcmp(*argv,VM_STRING("ANS"))==0) {
               params->type = ANS;
            } else if(vm_string_strcmp(*argv,VM_STRING("DTMF"))==0) {
               params->type = DTMF;
            }
            continue;
         }else if (!vm_string_strncmp(*argv,VM_STRING("-t"),2)){ /* check if sample frequency specified */
            params->lengthTone = vm_string_atol(*argv+2);
            if(params->lengthTone <= 0) params->lengthTone = 40;
            continue;
         }else if (!vm_string_strncmp(*argv,VM_STRING("-p"),2)){ /* check if sample frequency specified */
            params->lengthPause = vm_string_atol(*argv+2);
            if(params->lengthPause <= 0) params->lengthPause = 20;
            continue;
         }else if (!vm_string_strncmp(*argv,VM_STRING("-v"),2)){ /* check if sample frequency specified */
            params->volume = vm_string_atol(*argv+2);
            if(params->volume <= 0) params->volume = 6;
            continue;
         }else if (!vm_string_strncmp(*argv,VM_STRING("-f"),2)){ /* check if sample frequency specified */
            params->sample_frequency = vm_string_atol(*argv+2);
            continue;
         }else if (!vm_string_strcmp(*argv,VM_STRING("-o"))){ /* check if log output is specified */
            params->puttologfile=1;
            argv++;
            argc--;
            vm_string_strncpy(params->logFileName, *argv,IPP_MIN(MAX_FILENAME_LEN,vm_string_strlen(*argv)));
            continue;
         }else if (!vm_string_strncmp(*argv,VM_STRING("-sf"),3)){ /* check if frame size is specified */
            params->frame_size = vm_string_atol(*argv+3);
            argc--;
            continue;
         }
      }else{ /* unknown option */
         argc++; /* skip */
         argv--;
         break;
      }
   }

   if (params->TDoption == 1) {
      if(argc-->0) { /* check if input file name is specified */
         argv++;
         vm_string_strncpy(params->inputFileName, *argv,IPP_MIN(MAX_FILENAME_LEN,vm_string_strlen(*argv)));
      }
      if(argc-->0) { /* check if output file name is specified */
         argv++;
         vm_string_strncpy(params->outputFileName, *argv,IPP_MIN(MAX_FILENAME_LEN,vm_string_strlen(*argv)));
      }
   } else {
      if(argc-->0) { /* check if output file name is specified */
         argv++;
         vm_string_strncpy(params->outputFileName, *argv,IPP_MIN(MAX_FILENAME_LEN,vm_string_strlen(*argv)));
      }
   }
   return;
}

void PrintUsage(vm_char* pAppName, vm_file *flog)
{
   if(flog) {
      vm_file_fprintf(flog,VM_STRING("Usage : %s -type ANS/DTMF -dt/gt <options> <inFile> <outFile>\n"),pAppName);
      vm_file_fprintf(flog,VM_STRING("-gtXXX (or -gt Y)  where XXX is generated succession of tones (0-9, A, B, C, D, *, #), Y - generated one from tones: ANS, \\ANS.\n"));
      vm_file_fprintf(flog,VM_STRING("  options:\n"));
      vm_file_fprintf(flog,VM_STRING("   [-t]              length of tone (ms). 40 ms by default. Only for generation (-gt)(optional)\n"));
      vm_file_fprintf(flog,VM_STRING("   [-p]              length of pause between tone (ms). 20 ms by default. Only for generation (-gt)(optional)\n"));
      vm_file_fprintf(flog,VM_STRING("   [-v]              amplitude level (dB). 6 dB by default. Only for generation (-gt)(optional)\n"));
      vm_file_fprintf(flog,VM_STRING("   [-f]              Sample frequency (8000 or 16000) .\n"));
      vm_file_fprintf(flog,VM_STRING("   [-n]              Option reports (optional) .\n"));
      vm_file_fprintf(flog,VM_STRING("   [-o] <logFile>    Print out to <logFile> file (optional).\n"));
      vm_file_fprintf(flog,VM_STRING("  <inFile> <outFile> - 8000 kHz  or 16000 kHz 16 bps PCM files. <inFile> Only for detection (-dt)\n"));
   } else {
      vm_string_printf(VM_STRING("Usage : %s -type ANS/DTMF -dt/gt <options> <inFile> <outFile>\n"),pAppName);
      vm_string_printf(VM_STRING("-gtXXX (or -gt Y)  where XXX is generated succession of tones (0-9, A, B, C, D, *, #), Y - generated one from tones: ANS, \\ANS.\n"));
      vm_string_printf(VM_STRING("  options:\n"));
      vm_string_printf(VM_STRING("   [-t]              length of tone (ms). 40 ms by default. Only for generation (-gt)(optional)\n"));
      vm_string_printf(VM_STRING("   [-p]              length of pause between tones (ms). 20 ms by default. Only for generation (-gt)(optional)\n"));
      vm_string_printf(VM_STRING("   [-v]              amplitude level (dB). 6 dB by default. Only for generation (-gt)(optional)\n"));
      vm_string_printf(VM_STRING("   [-f]              Sample frequency (8000 or 16000) .\n"));
      vm_string_printf(VM_STRING("   [-n]              Option reports (optional) .\n"));
      vm_string_printf(VM_STRING("   [-o] <logFile>    Print out to <logFile> file (optional).\n"));
      vm_string_printf(VM_STRING("  <inFile> <outFile> - 8000 kHz  or 16000 kHz 16 bps PCM files. <inFile> Only for detection (-dt)\n"));
   }

   return;
}

#define MAX_LINE_LEN 1024

void USC_OutputString(vm_file *flog, vm_char *format,...)
{
   vm_char  line[MAX_LINE_LEN];
   va_list  args;

   va_start(args, format);
   vm_string_vsprintf(line, format, args);
   va_end(args);

   if(flog) {
      vm_file_fprintf(flog,VM_STRING("%s"), line);
   } else {
      vm_string_printf(VM_STRING("%s"), line);
   }

   return;
}

void InitUsedObjects(UsedObjects *pObjects)
{
   pObjects->f_in = NULL;
   pObjects->f_out = NULL;
   pObjects->f_log = NULL;
   pObjects->pInputBuffer = NULL;
   pObjects->nBanks = 0;
   pObjects->pBanks = NULL;
   pObjects->pInfo = NULL;
   pObjects->pNoiseBuffer = NULL;
}
void ReleaseUsedObjects(UsedObjects *pObjects)
{
   if(pObjects->f_in) {
      vm_file_fclose(pObjects->f_in);
      pObjects->f_in = NULL;
   }
   if(pObjects->f_out) {
      vm_file_fclose(pObjects->f_out);
      pObjects->f_out = NULL;
   }
   if(pObjects->f_log) {
      vm_file_fclose(pObjects->f_log);
      pObjects->f_log = NULL;
   }
   if(pObjects->pInputBuffer) {
      ippsFree(pObjects->pInputBuffer);
      pObjects->pInputBuffer = NULL;
   }
   if(pObjects->pNoiseBuffer) {
      ippsFree(pObjects->pNoiseBuffer);
      pObjects->pNoiseBuffer = NULL;
   }
   if(pObjects->pBanks) {
      int i;

      for(i=0; i<pObjects->nBanks;i++){
         ippsFree(pObjects->pBanks[i].pMem);
         pObjects->pBanks[i].pMem = NULL;
      }
      ippsFree(pObjects->pBanks);
      pObjects->pBanks = NULL;
      pObjects->nBanks=0;
   }
   if(pObjects->pInfo) {
      ippsFree(pObjects->pInfo);
      pObjects->pInfo = NULL;
   }
}

void USC_ToneId2Char(USC_ToneID tID, vm_char *pDstString)
{
   if(tID==USC_NoTone) {
      pDstString[0] = '\0';
      return;
   }
   switch(tID) {
      case USC_Tone_0:
         pDstString[0] = VM_STRING('0');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_1:
         pDstString[0] = VM_STRING('1');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_2:
         pDstString[0] = VM_STRING('2');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_3:
         pDstString[0] = VM_STRING('3');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_4:
         pDstString[0] = VM_STRING('4');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_5:
         pDstString[0] = VM_STRING('5');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_6:
         pDstString[0] = VM_STRING('6');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_7:
         pDstString[0] = VM_STRING('7');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_8:
         pDstString[0] = VM_STRING('8');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_9:
         pDstString[0] = VM_STRING('9');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_ASTERISK:
         pDstString[0] = VM_STRING('*');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_HASH:
         pDstString[0] = VM_STRING('#');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_A:
         pDstString[0] = VM_STRING('A');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_B:
         pDstString[0] = VM_STRING('B');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_C:
         pDstString[0] = VM_STRING('C');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_Tone_D:
         pDstString[0] = VM_STRING('D');
         pDstString[1] = VM_STRING('\0');
         break;
      case USC_ANS:
         vm_string_strcpy(pDstString,VM_STRING("ANS"));
         break;
      case USC_slashANS:
         vm_string_strcpy(pDstString,VM_STRING("/ANS"));
         break;
      case USC_ANSam:
         vm_string_strcpy(pDstString,VM_STRING("ANSam"));
         break;
      case USC_slashANSam:
         vm_string_strcpy(pDstString,VM_STRING("/ANSam"));
         break;
   }
   return;
}

void USC_ParseToneStr(vm_char *pTone, USC_ToneID *tID,int remainLen, int *pUsedLen)
{
   *pUsedLen = 0;
   *tID = USC_NoTone;

   if(remainLen >= 5) {
      if (vm_string_strncmp(pTone, VM_STRING("ANSam"),5)==0) {
         *tID = USC_ANSam;
         *pUsedLen = 5;
         return;
      }
   }
   if(remainLen >= 6) {
      if (vm_string_strncmp(pTone, VM_STRING("/ANSam"),6)==0) {
         *tID = USC_slashANSam;
         *pUsedLen = 6;
         return;
      }
   }
   if(remainLen >= 3) {
      if (vm_string_strncmp(pTone, VM_STRING("ANS"),3)==0) {
         *tID = USC_ANS;
         *pUsedLen = 3;
         return;
      }
   }
   if(remainLen >= 4) {
      if (vm_string_strncmp(pTone, VM_STRING("/ANS"),4)==0) {
         *tID = USC_slashANS;
         *pUsedLen = 4;
         return;
      }
   }

   if(remainLen >= 1) {
      switch(*pTone) {
         case VM_STRING('0'): *tID = USC_Tone_0; *pUsedLen = 1; break;
         case VM_STRING('1'): *tID = USC_Tone_1; *pUsedLen = 1; break;
         case VM_STRING('2'): *tID = USC_Tone_2; *pUsedLen = 1; break;
         case VM_STRING('3'): *tID = USC_Tone_3; *pUsedLen = 1; break;
         case VM_STRING('4'): *tID = USC_Tone_4; *pUsedLen = 1; break;
         case VM_STRING('5'): *tID = USC_Tone_5; *pUsedLen = 1; break;
         case VM_STRING('6'): *tID = USC_Tone_6; *pUsedLen = 1; break;
         case VM_STRING('7'): *tID = USC_Tone_7; *pUsedLen = 1; break;
         case VM_STRING('8'): *tID = USC_Tone_8; *pUsedLen = 1; break;
         case VM_STRING('9'): *tID = USC_Tone_9; *pUsedLen = 1; break;
         case VM_STRING('*'): *tID = USC_Tone_ASTERISK; *pUsedLen = 1; break;
         case VM_STRING('#'): *tID = USC_Tone_HASH; *pUsedLen = 1; break;
         case VM_STRING('A'): *tID = USC_Tone_A; *pUsedLen = 1; break;
         case VM_STRING('B'): *tID = USC_Tone_B; *pUsedLen = 1; break;
         case VM_STRING('C'): *tID = USC_Tone_C; *pUsedLen = 1; break;
         case VM_STRING('D'): *tID = USC_Tone_D; *pUsedLen = 1; break;
         default: *tID = USC_NoTone; *pUsedLen = 0; break;
      }
   }

   return;
}
#if defined (_BIG_ENDIAN)
void SwapBytes_16u(unsigned short *pSrc,int len)
{
   int i;
   for(i=0;i<len;i++) {
      pSrc[i] = (((pSrc[i]) << 8) | ((pSrc[i]) >> 8 ));
   }
   return;
}
#endif
