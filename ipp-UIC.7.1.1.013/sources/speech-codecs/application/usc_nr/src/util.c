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
// By downloading and installing this sample, you hereby agree that the
// accompanying Materials are being provided to you under the terms and
// conditions of the End User License Agreement for the Intel(R) Integrated
// Performance Primitives product previously accepted by you. Please refer
// to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
// product installation for more information.
//
// Purpose: Auxiliary functions file.
//
////////////////////////////////////////////////////////////////////////*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "types.h"

#include <ippcore.h>
#include <ipps.h>
void SetCommandLineByDefault(CommandLineParams *params)
{
   ippsZero_8u((Ipp8u*)params,sizeof(CommandLineParams));
   params->SampleFreq = 8000;
}
void ReadCommandLine(CommandLineParams *params, Ipp32s argc, Ipp8s *argv[])
{
   argc--;
   while(argc-- > 0){ /*process command line */
      argv++;
      if ('-' == (*argv)[0]) {
         if (!strcmp(*argv,"-n")){ /* check option report mode */
            params->optionReport = 1;
            continue;
         }else if (!strcmp(*argv,"-o")){ /* check if log output is specified */
            params->puttologfile=1;
            argv++;
            argc--;
            strcpy(params->logFileName, *argv);
            continue;
         }else if (!strcmp(*argv,"-list")){ /* check if log output is specified */
            params->enumerate=1;
            continue;
         } else if (!strcmp(*argv,"-alg")){
            argv++;
            argc--;
            strcpy(params->algFilter, *argv);
            continue;
         } else if (!strncmp(*argv,"-f",2)){
            params->SampleFreq = atoi(*argv+2);
            continue;
         }
      }else{ /* unknown option */
         argc++; /* skip */
         argv--;
         break;
      }
   }

   if(argc-->0){ /* check if input file name is specified */
      argv++;
      strcpy(params->inputFileName, *argv);
   }

   if(argc-->0){ /* check if output file name is specified */
      argv++;
      strcpy(params->outputFileName, *argv);
   }
   return;
}

void PrintUsage(Ipp8s *pAppName, FILE *flog)
{
   if(flog) {
      fprintf(flog,"Usage : %s <options> <inFile> <outFile>\n",pAppName);
      fprintf(flog,"  options:\n");
      fprintf(flog,"   [-alg AlgName]    Load <AlgName> algorithm.\n");
      fprintf(flog,"   [-f<freq>]        Sample frequency (8000 or 16000 kHZ).\n");
      fprintf(flog,"   [-n]              Option reports (optional).\n");
      fprintf(flog,"   [-o] <logFile>    Print out to <logFile> file (optional).\n");
      fprintf(flog,"   [-list]           Enumerate linked algorithms and exit (optional).\n");
      fprintf(flog,"  <inFile> <outFile> - 8000 kHz 16 bps or 16000 kHz 16 bps PCM files.\n");
   } else {
      printf("Usage : %s <options> <inFile> <outFile>\n",pAppName);
      printf("  options:\n");
      printf("   [-alg AlgName]    Load <AlgName> algorithm.\n");
      printf("   [-f<freq>]        Sample frequency (8000 or 16000 kHZ).\n");
      printf("   [-n]              Option reports (optional).\n");
      printf("   [-o] <logFile>    Print out to <logFile> file (optional).\n");
      printf("   [-list]           Enumerate linked algorithms and exit (optional).\n");
      printf("  <inFile> <outFile> - 8000 kHz 16 bps or 16000 kHz 16 bps PCM files.\n");
   }

   return;
}

#define MAX_LINE_LEN 1024

void USC_OutputString(FILE *flog, Ipp8s *format,...)
{
   Ipp8s line[MAX_LINE_LEN];
   va_list  args;

   va_start(args, format);
   vsprintf(line, format, args);
   va_end(args);

   if(flog) {
      fprintf(flog,"%s", line);
   } else {
      printf("%s", line);
   }

   return;
}

void InitUsedObjects(UsedObjects *pObjects)
{
   pObjects->f_in = NULL;
   pObjects->f_out = NULL;
   pObjects->f_log = NULL;
   pObjects->pInputBuffer = NULL;
   pObjects->pOutputBuffer = NULL;
   pObjects->nBanks = 0;
   pObjects->pBanks = NULL;
   pObjects->pInfo = NULL;
}
void ReleaseUsedObjects(UsedObjects *pObjects)
{
   if(pObjects->f_in) {
      fclose(pObjects->f_in);
      pObjects->f_in = NULL;
   }
   if(pObjects->f_out) {
      fclose(pObjects->f_out);
      pObjects->f_out = NULL;
   }
   if(pObjects->f_log) {
      fclose(pObjects->f_log);
      pObjects->f_log = NULL;
   }
   if(pObjects->pInputBuffer) {
      ippsFree(pObjects->pInputBuffer);
      pObjects->pInputBuffer = NULL;
   }
   if(pObjects->pOutputBuffer) {
      ippsFree(pObjects->pOutputBuffer);
      pObjects->pOutputBuffer = NULL;
   }
   if(pObjects->pBanks) {
      Ipp32s i;

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

#if defined (_BIG_ENDIAN)
void SwapBytes_16u(Ipp16u *pSrc,Ipp32s len)
{
   Ipp32s i;
   for(i=0;i<len;i++) {
      pSrc[i] = (((pSrc[i]) << 8) | ((pSrc[i]) >> 8 ));
   }
   return;
}
#endif
