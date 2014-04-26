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
//     USC Echo Canceller sample
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
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ipps.h>
#include <ippsc.h>
#include "util.h"

int OutputInfoString(int flConsole, FILE *fptrLog, const char *string)
{
   int count;

   if(fptrLog == NULL) count = printf("%s", string);
   else {
     if(flConsole == 0) count = printf("%s", string);
     count = fprintf(fptrLog, "%s", string);
   }
   return count;
}

void SetCommandLineByDefault(CommandLineParams *params)
{
   memset((void*)params,0,sizeof(CommandLineParams));
   strcpy(params->ECName, "IPP_EC_FP");
   params->alg = EC_SUBBAND;
   params->freq = 8000;
   params->tail = 16;
   params->ah_mode = 0;
   params->nRepeat = 1;
   params->puttolog = 0;
   params->puttocsv = 0;
   params->printSysInfo=0;
   params->adapt=AD_FULLADAPT;
   params->nlp=1;
   params->nr=4;
   params->cng=0;
   params->ap = 1;
   params->nr_smooth=0;
   //params->dcFlag=0;
}

int ReadCommandLine(CommandLineParams *params, int argc, char *argv[])
{
   int usage = 0;


   argc--;
   while(argc-- > 0){
      argv++;
      if ('-' == (*argv)[0]) {
         if (!strncmp("-f",*argv,2)){
            if(argc-->0){
               int f = atoi(*++argv);
               if (f == 8)
                  params->freq = 8000;
               else if (f == 16) {
                  params->freq = 16000;
               } else {
                  OutputInfoString(1, NULL,"The sample frequency must be 8 or 16 kHz\n");
                  usage = 1;
               }
            }
            continue;
         } else if(!strcmp("-a",*argv)) {
            if(argc-->0){
                argv ++;
                if (!strcmp("fb",*argv) || !strcmp("fullband",*argv)) {
                    params->alg = EC_FULLBAND;
                } else if (!strcmp("sb",*argv) || !strcmp("subband",*argv)) {
                    params->alg = EC_SUBBAND;
                } else if (!strcmp("sbf",*argv) || !strcmp("subbandfast",*argv)) {
                    params->alg = EC_FASTSUBBAND;
                } else {
                  OutputInfoString(1, NULL,"The algorithm specifier must be fb (fullband), sb (subband) or sbf (subband fast)\n");
                  usage = 1;
               }
            }
            continue;
         } else if(!strcmp("-ad",*argv)) {
             if(argc-->0){
                 argv ++;
                 if (!strcmp("full",*argv)) {
                     params->adapt = AD_FULLADAPT;
                 } else if (!strcmp("lite",*argv)) {
                     params->adapt = AD_LITEADAPT;
                 } else {
                     OutputInfoString(1, NULL,"The adaptation specifier must be full or lite.\n");
                     usage = 1;
                 }
             }
             continue;
         }else if(!strcmp("-l",*argv)) {/* set the echo tail length */
            if(argc-->0){
               int eplen = atoi(*++argv);
               if (eplen >= 1 && eplen <= 200)
                  params->tail = eplen;
               else {
                  OutputInfoString(1, NULL,"The echo tail length must be in the range [1, 200] ms.\n");
                  usage = 1;
               }
            }
            continue;
         }else if(!strcmp("-ap",*argv)) {/* set the affine projection order */
             if(argc-->0){
                 int ap = atoi(*++argv);
                 if (ap >= 1)
                     params->ap = ap;
                 else {
                     OutputInfoString(1, NULL,"The affine projection order must be positive, 1 or 4 recommended\n");
                     usage = 1;
                 }
                 if (ap == 1)
                     params->ap = 0;
             }
             continue;
         }else if(!strcmp("-m",*argv)) {/* set the NR smoothing type */
             if(argc-->0){
                 int ap = atoi(*++argv);
                 if (ap >= 0 && ap<=2)
                     params->nr_smooth = ap;
                 else {
                     OutputInfoString(1, NULL,"The smoothing type must be 0,1,2\n");
                     usage = 1;
                 }
             }
             continue;
         }else if (!strcmp(*argv,"-sys")){/* set the option to print the system information */
            params->printSysInfo=1;
            continue;
         }else if (!strcmp(*argv,"-delay")){
             if(argc-->0){
                 params->fdelay = atoi(*++argv);
                 if(params->fdelay < 0) params->fdelay=0;
             }
             continue;
         }else if (!strcmp(*argv,"-ah")){/* set the anti-howling device on and set the howling detector device mode*/
             if(argc-->0){
                 int hdmode = atoi(*++argv);
                 if(hdmode==0)
                     params->ah_mode=0;
                 else
                     if(hdmode==1)
                         params->ah_mode=1;
                     else{
                         OutputInfoString(1, NULL,"The anti-howler mode must be set to 0 or 1\n");
                         usage = 1;
                     }
             }
             continue;
         }else if (!strcmp(*argv,"-s")){/* set the number of repeats */
            if(argc-->0){
               int rep = atoi(*++argv);
               if(0 == rep) rep=1;
               if(rep > params->nRepeat) params->nRepeat=rep;
            }
            continue;
         }else if(!strcmp("-o",*argv)) {/* the log file is specified */
            if(argc-->0){
              argv++;
              strcpy(params->logFileName, *argv);
              params->puttolog = 1;
            }
            continue;
         }else if (!strcmp(*argv,"-list")){ /* Print out the modes list */
            params->enumerate = 1;
            usage=1;
            continue;
         }else if (!strcmp(*argv,"-nlp")){ /* set NLP on */
             if(argc-->0){
                 params->nlp = atoi(*++argv);
                 if(params->nlp<0 || params->nlp>3){
                     OutputInfoString(1, NULL,"The NLP mode must be set to 0, 1, 2 or 3\n");
                    usage = 1;
                 }
             }
            continue;
         }else if (!strcmp(*argv,"-nr")){ /* set nr on */
             if(argc-->0){
                 params->nr = atoi(*++argv);
                 if(params->nr<0 || params->nr>5){
                     OutputInfoString(1, NULL,"The nr mode must be set to 0,...,5 \n");
                    usage = 1;
                 }
             }
            continue;
         }else if (!strcmp(*argv,"-cng")){ /* set cng on */
             if(argc-->0){
                 params->cng = atoi(*++argv);
                 if(params->cng<0 || params->cng>2){
                     OutputInfoString(1, NULL,"The CNG mode must be set to 0, 1 or 2\n");
                     usage = 1;
                 }
             }
             continue;
         } else if (!strcmp(*argv,"-c")){ /* the "csv" output is specified */
             if(argc-->0){
                 params->puttocsv=1;
                 argv++;
                 strcpy(params->csvFileName, *argv);
             }else{
                 OutputInfoString(1, NULL,"Unknown csv file name.\n");
                 usage=1;
             }
             continue;
         }
      }else{
         argc++;
         argv--;
         break;
      }
   }
   
   if(usage) return usage;

   if(argc-->0){
      argv++;
      strcpy(params->rinFileName, *argv);
   }else{
      OutputInfoString(1, NULL,"Unknown r-in input file.\n");
      usage=1;
   }
   if(argc-->0){
      argv++;
      strcpy(params->sinFileName, *argv);
   }else{
      OutputInfoString(1, NULL,"Unknown s-in input file.\n");
      usage=1;
   }
   if(argc-->0){
      argv++;
      strcpy(params->soutFileName, *argv);
   }else{
      OutputInfoString(1, NULL,"Unknown output file.\n");
      usage=1;
   }
   return usage;
}

void PrintUsage(const char * pAppName, FILE *fptrLog)
{
   char pString[MAX_LEN_STRING];

   sprintf(pString, "Usage: %s [options] <receive-in 16-bit PCM file> <send-in 16-bit PCM file> <send-out 16-bit PCM file>\n", pAppName);
   OutputInfoString(1, fptrLog, (const char*)pString);
   OutputInfoString(1, fptrLog,"   [-f <frequency>] Set the sampling frequency, kHz: 8 or 16. Default 8.\n");
   OutputInfoString(1, fptrLog,"   [-a <algorithm>] Algorithm specifier: fb (fullband), sb (subband type 1) or sbf (subband type 2). \n");
   OutputInfoString(1, fptrLog,"                    Default sb.\n");
   OutputInfoString(1, fptrLog,"   [-ad <algorithm>] Adaptation specifier: full or lite. Default full.\n");
   OutputInfoString(1, fptrLog,"   [-ah <mode>]     Set the anti-howling device mode:1 - on (cos shift up. Exclude sb modes). 0 - off. 0 as default.\n");
   OutputInfoString(1, fptrLog,"   [-l <num>]       Set the echo tail length, ms: in the range [1, 200].\n");
   OutputInfoString(1, fptrLog,"   [-delay <val>]   Set the delay in send-in PCM file,  integer number of milliseconds. \n");
   OutputInfoString(1, fptrLog,"                    Leading end of send-in PCM will be cut off prior to echo cancel. Default:no delay.\n");
   OutputInfoString(1, fptrLog,"   [-s <num>]       Set the number of times file to be processed (optional).\n");
   OutputInfoString(1, fptrLog,"   [-c <csv filename>] Set the csv file name for the performance info to be print to (optional).\n");
   OutputInfoString(1, fptrLog,"   [-o <log filename>] Set the log file name for the functional parameters to be print to (optional).\n");
   OutputInfoString(1, fptrLog,"   [-list] Print out the modes list(optional).\n");
   OutputInfoString(1, fptrLog,"   [-sys] Print out system information.\n");
   OutputInfoString(1, fptrLog,"   [-ap <val>]   Choose the order of affine projection applied in sb algorithm. 1 or 4 recommended. 1 is default\n");
   OutputInfoString(1, fptrLog,"   [-nlp <num>]  0- Set NLP off.1 as default\n");
   OutputInfoString(1, fptrLog,"                 1- NLP type actual for all modes,\n");
   OutputInfoString(1, fptrLog,"                 2- NLP type actual for sbf only\n");
   OutputInfoString(1, fptrLog,"                 3- 1+2 combined. Actual for sbf only\n");
   OutputInfoString(1, fptrLog,"   [-nr <num>]   0,...,5 Set IPP NR mode. 4 as default\n");
   OutputInfoString(1, fptrLog,"                 0- no NR performed,\n");
   OutputInfoString(1, fptrLog,"                 1- ippsNrLow mode of IPPS NR engaged,\n");
   OutputInfoString(1, fptrLog,"                 2- ippsNrMedium mode of IPPS NR engaged,\n");
   OutputInfoString(1, fptrLog,"                 3- ippsNrNormal mode of IPPS NR engaged,\n");
   OutputInfoString(1, fptrLog,"                 4- ippsNrHigh mode of IPPS NR engaged,\n");
   OutputInfoString(1, fptrLog,"                 5- ippsNrAuto mode of IPPS NR engaged.\n");
   OutputInfoString(1, fptrLog,"   [-m <num>]    0-Set NR smoothing off. 0 as default.\n");
   OutputInfoString(1, fptrLog,"                 1- static type of smoothing with constant filter applied.\n");
   OutputInfoString(1, fptrLog,"                 2- dynamic type of smoothing with changing filters applied.\n");
   OutputInfoString(1, fptrLog,"   [-cng <num>]  0-Set CNG off. 1 as default. Active when NLP is on only.\n");
   OutputInfoString(1, fptrLog,"                 1- CNG type actual for all modes.\n");
   OutputInfoString(1, fptrLog,"                 2- CNG type actual for sbf only (actual with -nlp type 2 only).\n");
}

int sysInfoToCSV(vm_file *f_csv)
{
    char infoBuffer[256];
    const  IppLibraryVersion *verIppSC, *verIppSP;

    if(f_csv == NULL) return -1;

    infoBuffer[0] = '\0';
    vm_sys_info_get_date((vm_char *)infoBuffer,DDMMYY);
    vm_string_fprintf(f_csv,VM_STRING("Date,%s"),infoBuffer);

    infoBuffer[0] = '\0';
    vm_sys_info_get_time((vm_char *)infoBuffer,HHMMSS);
    vm_string_fprintf(f_csv,VM_STRING(",Time,%s\n"),infoBuffer);

    infoBuffer[0] = '\0';
    vm_sys_info_get_cpu_name((vm_char *)infoBuffer);
    vm_string_fprintf(f_csv,VM_STRING("CPU,%s\n"),infoBuffer);

    infoBuffer[0] = '\0';
    vm_sys_info_get_os_name((vm_char *)infoBuffer);
    vm_string_fprintf(f_csv,VM_STRING("OS,%s\n"),infoBuffer);

    infoBuffer[0] = '\0';
    vm_sys_info_get_computer_name((vm_char *)infoBuffer);
    vm_string_fprintf(f_csv,VM_STRING("Computer name,%s\n"),infoBuffer);

    verIppSC = ippscGetLibVersion();
    verIppSP = ippsGetLibVersion();

    infoBuffer[0] = '\0';
    sprintf(infoBuffer,"%s,%s,%s\n",verIppSP->Name,verIppSP->Version,verIppSP->BuildDate);
    vm_string_fprintf(f_csv,VM_STRING("IPPS library %s\n"),infoBuffer);

    infoBuffer[0] = '\0';
    sprintf(infoBuffer,"%s,%s,%s\n",verIppSC->Name,verIppSC->Version,verIppSC->BuildDate);
    vm_string_fprintf(f_csv,VM_STRING("IPPSC library %s\n"),infoBuffer);

    infoBuffer[0] = '\0';
    sprintf(infoBuffer,"EC TYPE,alg type,adaptation,bits,sampling rate,rin file,sin file,speech in sec,speed in mHz\n");
    vm_string_fprintf(f_csv,VM_STRING("%s\n"),infoBuffer);

return 0;
}
