/*////////////////////////////////////////////////////////////////////////
//
// INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
// Copyright (c) 2005-2011 Intel Corporation. All Rights Reserved.
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
#include "usc.h"
#include "util.h"
#include "vm_sys_info.h"

#include "ippcore.h"
#include "ipps.h"
#include "ippsc.h"

void SetCommandLineByDefault(CommandLineParams *params)
{
   memset((void*)params,0,sizeof(CommandLineParams));
}
void ReadCommandLine(CommandLineParams *params, Ipp32s argc, Ipp8s *argv[])
{
   argc--;
   while(argc-- > 0){ /*process command line */
      argv++;
      if ('-' == (*argv)[0]) {
         if (!strncmp("-r",*argv,2)){ /* check if rate is specified */
            params->bitrate = atol(*argv+2);
            continue;
         }else if (!strncmp("-v",*argv,2)){ /* check vad */
            params->Vad = atol(*argv+2);
            if(params->Vad==0) params->Vad++;
            continue;
         }else if (!strcmp(*argv,"-format")){ /* check format type */
            if(argc-->0){
               strcpy(params->codecName, *++argv);
            }
            continue;
         }else if (!strncmp("-timing",*argv,7)){ /* check format type */
            params->timing = 1;
            if(strlen(*argv)!=strlen("-timing")) {
               /*Time single codec*/
               strcpy(params->TimingCodecName,*argv+7);
            }
            if(argc-->0){
               params->nRepeat = atol(*++argv);
               if(params->nRepeat < 1) params->nRepeat = 1;
            }
            continue;
         }else if (!strcmp(*argv,"-n")){ /* check option report mode */
            params->optionReport = 1;
            continue;
         }else if (!strcmp(*argv,"-list")){ /* check enumerate formats mode */
            params->enumerate = 1;
            return;
         }else if (!strcmp(*argv,"-c")){ /* check if csv output is specified */
            params->puttocsv=1;
            argv++;
            argc--;
            strcpy(params->csvFileName, *argv);
            continue;
         }else if (!strcmp(*argv,"-o")){ /* check if log output is specified */
            params->puttologfile=1;
            argv++;
            argc--;
            strcpy(params->logFileName, *argv);
            continue;
         }else if (!strcmp(*argv,"-alaw")){
            params->nOutputPCMType=ALAW_PCM;
            continue;
         }else if (!strcmp(*argv,"-mulaw")){
            params->nOutputPCMType=MULAW_PCM;
            continue;
         }else if(!strncmp("-lost",*argv,5)){ /* % lost */
            params->pcLost = atol(*argv+5);
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
   if(!params->timing) {
      if(argc-->0){ /* check if output file name is specified */
         argv++;
         strcpy(params->outputFileName, *argv);
      }
   } else {
      strcpy(params->outputFileName, "Not_Applicable");
   }
   return;
}

void PrintUsage(Ipp8s *pAppName)
{
   printf("Usage : %s <options> <inFile> <outFile>\n",pAppName);
   printf("  options:\n");
   printf("   [-format CodecName] Set codec (for encoder only). \n");
   printf("   [-r<mode>]        Set bitrate mode. \n");
   printf("   [-v[<nVAD>]]      Enables DTX mode number <nVAD>, default VAD mode if omitted.(optional).\n");
   printf("   [-n]              Option reports (optional) .\n");
   printf("   [-c <csvFile>]    Print out performance info line to <csvFile> file (optional).\n");
   printf("   [-o <logFile>]    Print out to <logFile> file (optional).\n");
   printf("   [-list]           Enumerate all stiticly linked codecs(optional). \n");
   printf("   [-alaw]           Output file will be A-Law PCM (optional, only for decoder part).\n");
   printf("   [-mulaw]          Output file will be Mu-Law PCM (optional, only for decoder part).\n");
   printf("For timing:  %s -timing[CodecName] <NRepeat> <options> <inWaveFile>.\n",pAppName);
   printf("  options:\n");
   printf("   <NRepeat>         How many time repeat input file.\n");
   printf("   [-c <csvFile>]    Print out performance info line to <csvFile> file (optional).\n");
   printf("   [-o <logFile>]    Print out to <logFile> file (optional).\n");
   printf("   [CodecName]       Codec name, if unspecified, all linked codec, corresponding <inWaveFile>.\n");
   printf("                     will be timed.\n");

   return;
}

Ipp32s OutputString2CSVFile(CommandLineParams *params, USC_CodecInfo *pInfo, Ipp32f spSeconds,Ipp32f speed_in_mhz)
{
   if (params->puttocsv) { /* open the csv file if any */
      FILE *f_csv=NULL;
      Ipp8s* pFileName;

      if ( (f_csv = fopen(params->csvFileName, "a")) == NULL) {
         printf("\nWarning: File %s could not be open. Ignored.\n", params->csvFileName);
         return -1;
      }
      if ((pFileName = strrchr( params->inputFileName, '/')) != NULL) {
        pFileName += 1;
      } else if ((pFileName = strrchr( params->inputFileName, '\\')) != NULL) {
        pFileName += 1;
      } else
        pFileName = params->inputFileName;
      fprintf(f_csv,"%s,%s,%s,%d,%s,%4.2f,%4.2f,%d\n",pInfo->name,
         (pInfo->params.direction)? "decode":"encode",
         (pInfo->params.modes.vad == 1)?"VAD1":(pInfo->params.modes.vad == 2)?"VAD2":"    ",
         pInfo->params.modes.bitrate,pFileName,spSeconds,speed_in_mhz,1);
      fclose(f_csv);
   }
   return 0;
}

vm_file *OpenCSVFile(Ipp8s *csvFileName)
{
   vm_file *f_csv=NULL;
   f_csv = vm_file_fopen((const vm_char *)csvFileName, "a");
   return f_csv;
}
void CloseCSVFile(vm_file *f_csv)
{
   if(f_csv) {
      vm_file_fclose(f_csv);
   }
}
Ipp32s OutputDuplexString2CSVFile(CommandLineParams *params, USC_CodecInfo *pInfo, Ipp32f spSeconds,
                               Ipp32f ENCspeed_in_mhz, Ipp32f DECspeed_in_mhz)
{
   if (params->puttocsv) { /* open the csv file if any */
      vm_file *f_csv=NULL;
      Ipp8s* pFileName;

      f_csv = OpenCSVFile(params->csvFileName);

      if (f_csv == NULL) {
         printf("\nWarning: File %s could not be open. Ignored.\n", params->csvFileName);
         return -1;
      }
      if ((pFileName = strrchr( params->inputFileName, '/')) != NULL) {
        pFileName += 1;
      } else if ((pFileName = strrchr( params->inputFileName, '\\')) != NULL) {
        pFileName += 1;
      } else
        pFileName = params->inputFileName;
      vm_string_fprintf(f_csv,VM_STRING("%s,%s,%d,%s,%4.2f,%4.2f,%4.2f,%d\n"),pInfo->name,
         (pInfo->params.modes.vad == 1)?"VAD1":(pInfo->params.modes.vad == 2)?"VAD2":"    ",
         pInfo->params.modes.bitrate,pFileName,spSeconds,ENCspeed_in_mhz,DECspeed_in_mhz,pInfo->params.pcmType.nChannels);
      CloseCSVFile(f_csv);
   }
   return 0;
}

Ipp32s OutputSysInfo2CSVFile(vm_file *f_csv)
{
   vm_char infoBuffer[256];
   const  IppLibraryVersion *verIppSC, *verIppSP;

   if(f_csv == NULL) return -1;

   infoBuffer[0] = '\0';
   vm_sys_info_get_date(infoBuffer,DDMMYY);
   vm_string_fprintf(f_csv,VM_STRING("Date,%s"),infoBuffer);

   infoBuffer[0] = '\0';
   vm_sys_info_get_time(infoBuffer,HHMMSS);
   vm_string_fprintf(f_csv,VM_STRING(",Time,%s\n"),infoBuffer);

   infoBuffer[0] = '\0';
   vm_sys_info_get_cpu_name(infoBuffer);
   vm_string_fprintf(f_csv,VM_STRING("CPU,%s\n"),infoBuffer);

   infoBuffer[0] = '\0';
   vm_sys_info_get_os_name(infoBuffer);
   vm_string_fprintf(f_csv,VM_STRING("OS,%s\n"),infoBuffer);

   infoBuffer[0] = '\0';
   vm_sys_info_get_computer_name(infoBuffer);
   vm_string_fprintf(f_csv,VM_STRING("Computer name,%s\n"),infoBuffer);

   verIppSC = ippscGetLibVersion();
   verIppSP = ippsGetLibVersion();

   vm_string_fprintf(f_csv,VM_STRING("IPPS library,%s,%s,%s\n"),
      verIppSP->Name,verIppSP->Version,verIppSP->BuildDate);

   vm_string_fprintf(f_csv,VM_STRING("IPPSC library,%s,%s,%s\n"),
      verIppSC->Name,verIppSC->Version,verIppSC->BuildDate);
   return 0;
}

void AddCodecTableHeader2CSVFile(vm_file *f_csv)
{
   if(f_csv==NULL) return;
   vm_string_fprintf(f_csv,VM_STRING("Codec,Voice Activity Detection,Bitrate(bps),Audio file,Duration(sec),Encode(MHz),Decode(MHz),nChannels\n"));
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
