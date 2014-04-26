/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//  Intel(R) Integrated Performance Primitives Audio Processing
//  Sample for Windows*
//
*/

#include <stdio.h>
#include <stdlib.h>

#if defined (WIN32)
#include <windows.h>
#endif

#include <ippcore.h>
#include <ipps.h>
#include <ippsc.h>

#include "util.h"

#ifdef _USC_ALL
#  undef _USC_ALL
#endif

#define _USC_TONES
#include "usc_objects_decl.h"

#define COPYRIGHT_STRING "Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved."

#define OPEN_FILE_ERROR     -1   /* return code on file open error */
#define MEMORY_ERROR        -2   /* return code on memory allocation error */
#define USC_ERROR           -3   /* return code on error USC operation error*/

#define ASSERT_MSGBOX(cond, msg, title, exitcode, pObjects)   \
{                                                             \
  if (!(cond))                                                \
  {                                                           \
    USC_OutputString(Objects.f_log, VM_STRING("%s: %s\n"), title, msg);  \
    ReleaseUsedObjects(pObjects);                             \
    exit(exitcode);                                           \
  }                                                           \
}

#define FILE_ASSERT_MSGBOX(cond, msg, title, exitcode, pObjects)   \
{                                                             \
  if (!(cond))                                                \
  {                                                           \
    USC_OutputString(Objects.f_log, VM_STRING("%s: TD example. Cannot open %s file!\n"), title, msg);  \
    ReleaseUsedObjects(pObjects);                             \
    exit(exitcode);                                           \
  }                                                           \
}

int vm_main(int argc, vm_char **argv)
{
   USC_TD_Fxns *pUSC_TD = NULL;
   USC_Handle hUSCTD;
   USC_Status uscRes;
   USC_PCMStream in, out;

   CommandLineParams clParams;
   UsedObjects Objects;

   unsigned int     cSamplesRead;                       /* number of samples read from input file */
   int i, infoSize=0, modeReq=-1;

   ippStaticInit();

   SetCommandLineByDefault(&clParams);
   InitUsedObjects(&Objects);

   ReadCommandLine(&clParams, argc, argv);

   if(clParams.puttologfile) {
      Objects.f_log=vm_file_fopen((const vm_char *)clParams.logFileName,(const vm_char *)"a");
      if(Objects.f_log==NULL) {
         printf("Cannot open %s log file for writing\n",clParams.logFileName);
         printf("Log file ignored.\n");
         clParams.puttologfile = 0;
      }
   }

   if(clParams.optionReport) {
      const  IppLibraryVersion *verIppSC;
      verIppSC = ippscGetLibVersion();
      USC_OutputString(Objects.f_log, VM_STRING("%s\n"),VM_STRING(COPYRIGHT_STRING));
      USC_OutputString(Objects.f_log, VM_STRING("Intel IPP, Unified Speech Codec interface based Tone detector sample\n"));
      USC_OutputString(Objects.f_log, VM_STRING("The Intel(R) IPPSC library used:  %d.%d.%d Build %d, name %s\n"),
             verIppSC->major,verIppSC->minor,verIppSC->majorBuild,verIppSC->build,verIppSC->Name);
   }

   if (clParams.TDoption == 1) {
      if((!clParams.inputFileName[0]) ||(!clParams.outputFileName[0])) {
         PrintUsage(argv[0], Objects.f_log);
         ReleaseUsedObjects(&Objects);
         return 0;
      }
   } else if(clParams.TDoption == 0) {
      if(!clParams.outputFileName[0]) {
         PrintUsage(argv[0], Objects.f_log);
         ReleaseUsedObjects(&Objects);
         return 0;
      }
   } else {
      PrintUsage(argv[0], Objects.f_log);
      ReleaseUsedObjects(&Objects);
      return 0;
   }

   if(clParams.type == ANS) {
       if(strchr(clParams.tone_succession,(int)('//'))){
          pUSC_TD = &USC_ANSs_Fxns;
       }else{
          pUSC_TD = &USC_ANSam_Fxns;
       }
   } else if (clParams.type == DTMF){
      pUSC_TD = &USC_DTMF_Fxns;
   } else {
      USC_OutputString(Objects.f_log, VM_STRING("Error: Unsupported TD type.\n"));
      PrintUsage(argv[0], Objects.f_log);
      ReleaseUsedObjects(&Objects);
      return 0;
   }

   /* Open input and output files */
   if (clParams.TDoption == 1){
      Objects.f_in = vm_file_fopen((const vm_char*)clParams.inputFileName,VM_STRING("rb"));
      FILE_ASSERT_MSGBOX(Objects.f_in != NULL, clParams.inputFileName, VM_STRING("Error"), OPEN_FILE_ERROR, &Objects);
   }

   Objects.f_out = vm_file_fopen((const vm_char*)clParams.outputFileName,VM_STRING("wb"));
   FILE_ASSERT_MSGBOX(Objects.f_out != NULL, clParams.outputFileName, VM_STRING("Error"), OPEN_FILE_ERROR, &Objects);

   pUSC_TD->std.GetInfoSize(&infoSize);
   Objects.pInfo = (USC_TD_Info*)ippsMalloc_8u(infoSize);
   ASSERT_MSGBOX(Objects.pInfo != NULL, VM_STRING("Out of memory!\n"), VM_STRING("Error"), MEMORY_ERROR, &Objects);

   /* Get the TD info */
   uscRes = pUSC_TD->std.GetInfo((USC_Handle)NULL, Objects.pInfo);
   ASSERT_MSGBOX(uscRes == USC_NoError,  VM_STRING("TD example. USC GetInfo failed!\n"),  VM_STRING("Error"), USC_ERROR, &Objects);

   for(i=0;i<Objects.pInfo->nOptions;i++) {
      if(Objects.pInfo->params[i].pcmType.sample_frequency==clParams.sample_frequency) {
         modeReq = i;
      }
   }

   ASSERT_MSGBOX(modeReq != -1,  VM_STRING("TD example. Unsupported PCM type!\n"),  VM_STRING("Error"), USC_ERROR, &Objects);

   /* Learn how many memory block needed  for the encoder */
   uscRes = pUSC_TD->std.NumAlloc(&Objects.pInfo->params[modeReq], &Objects.nBanks);
   ASSERT_MSGBOX(uscRes == USC_NoError,  VM_STRING("TD example. USC NumAlloc failed!\n"),  VM_STRING("Error"), USC_ERROR, &Objects);

   /* allocate memory for memory bank table */
   Objects.pBanks = (USC_MemBank*)ippsMalloc_8u(sizeof(USC_MemBank)*Objects.nBanks);
   ASSERT_MSGBOX(Objects.pBanks != NULL,  VM_STRING("Out of memory!\n"),  VM_STRING("Error"), MEMORY_ERROR, &Objects);

   /* Query how big has to be each block */
   uscRes = pUSC_TD->std.MemAlloc(&Objects.pInfo->params[modeReq], Objects.pBanks);
   ASSERT_MSGBOX(uscRes == USC_NoError,  VM_STRING("TD example. USC MemAlloc failed!\n"),  VM_STRING("Error"), USC_ERROR, &Objects);

   /* allocate memory for each block */
   for(i=0; i<Objects.nBanks;i++){
      Objects.pBanks[i].pMem = NULL;
      Objects.pBanks[i].pMem = (char*)ippsMalloc_8u(Objects.pBanks[i].nbytes);
   }

   for(i=0; i<Objects.nBanks;i++){
      ASSERT_MSGBOX(Objects.pBanks[i].pMem != NULL,  VM_STRING("Out of memory!\n"),  VM_STRING("Error"), MEMORY_ERROR, &Objects);
   }

   if (clParams.type == DTMF){
      Objects.pInfo->params[modeReq].framesize = clParams.frame_size;
   }

   /* Create encoder instance */
   uscRes = pUSC_TD->std.Init(&Objects.pInfo->params[modeReq], Objects.pBanks, &hUSCTD);
   ASSERT_MSGBOX(uscRes == USC_NoError, VM_STRING("TD example. USC Init failed!\n"), VM_STRING("Error"), USC_ERROR, &Objects);

   uscRes = pUSC_TD->std.GetInfo(hUSCTD, Objects.pInfo);
   ASSERT_MSGBOX(uscRes == USC_NoError, VM_STRING("TD example. USC GetInfo failed!\n"), VM_STRING("Error"), USC_ERROR, &Objects);

   USC_OutputString(Objects.f_log, VM_STRING("\nStart processing.\n"));
   USC_OutputString(Objects.f_log, VM_STRING("Input  file: %s\n"),clParams.inputFileName);
   USC_OutputString(Objects.f_log, VM_STRING("Output file: %s\n"),clParams.outputFileName);

/*Detection*/
   if (clParams.TDoption == 1){
      USC_OutputString(Objects.f_log, VM_STRING("\nTone detections.\n"));
      Objects.pInputBuffer = (char*)ippsMalloc_8u(Objects.pInfo->params[0].framesize);
      ASSERT_MSGBOX(Objects.pInputBuffer != NULL, VM_STRING("Out of memory!\n"), VM_STRING("Error"), MEMORY_ERROR, &Objects);
      cSamplesRead = 1;
      while(cSamplesRead > 0)
      {
         USC_ToneID toneID;
         cSamplesRead = (unsigned int)vm_file_fread(Objects.pInputBuffer, 1, Objects.pInfo->params[0].framesize, Objects.f_in);
   #if defined (_BIG_ENDIAN)
         SwapBytes_16u((unsigned short *)Objects.pInputBuffer,cSamplesRead>>1);
   #endif

         in.bitrate = Objects.pInfo->params[0].pcmType.sample_frequency * Objects.pInfo->params[0].pcmType.bitPerSample;
         in.nbytes = Objects.pInfo->params[0].framesize;
         in.pBuffer = Objects.pInputBuffer;
         in.pcmType.bitPerSample = Objects.pInfo->params[0].pcmType.bitPerSample;
         in.pcmType.nChannels = Objects.pInfo->params[0].pcmType.nChannels;
         in.pcmType.sample_frequency = Objects.pInfo->params[0].pcmType.sample_frequency;

         uscRes = pUSC_TD->DetectTone(hUSCTD, &in, &toneID);
         ASSERT_MSGBOX(uscRes == USC_NoError,  VM_STRING("TD example. USC DetectTone failed!\n"),  VM_STRING("Error"), USC_ERROR, &Objects);
         if(toneID!=USC_NoTone) {
            vm_char buffer[MAX_STRING_LEN];
            USC_ToneId2Char(toneID, buffer);
            vm_file_fprintf(Objects.f_out,VM_STRING("%s\n"),buffer);
            USC_OutputString(Objects.f_log, VM_STRING("%s"), buffer);
         }
      }
   } else if(clParams.TDoption == 0){ /*Generation*/
      int j, ndurationPause, ndurationTone, nLen;
      vm_char *pToneSuccPtr = clParams.tone_succession;

      ndurationTone = (clParams.lengthTone*Objects.pInfo->params[0].pcmType.sample_frequency/1000) * sizeof(short);

      USC_OutputString(Objects.f_log, VM_STRING("Tone generation.\nGenerated:\n"));

      Objects.pInputBuffer = (char*)ippsMalloc_8u(ndurationTone);
      ASSERT_MSGBOX(Objects.pInputBuffer != NULL, VM_STRING("Out of memory!\n"), VM_STRING("Error"), MEMORY_ERROR, &Objects);

      out.bitrate = Objects.pInfo->params[0].pcmType.sample_frequency * Objects.pInfo->params[0].pcmType.bitPerSample;
      out.nbytes = 0;
      out.pBuffer = Objects.pInputBuffer;
      out.pcmType.bitPerSample = Objects.pInfo->params[0].pcmType.bitPerSample;
      out.pcmType.nChannels = Objects.pInfo->params[0].pcmType.nChannels;
      out.pcmType.sample_frequency = Objects.pInfo->params[0].pcmType.sample_frequency;

      ndurationPause = (clParams.lengthPause*out.pcmType.sample_frequency/1000) * sizeof(short);
      Objects.pNoiseBuffer = ippsMalloc_8s(ndurationPause);
      ASSERT_MSGBOX(Objects.pNoiseBuffer != NULL,  VM_STRING("Out of memory!\n"),  VM_STRING("Error"), MEMORY_ERROR, &Objects);
      ippsZero_8u(Objects.pNoiseBuffer, ndurationPause);

      nLen = (Ipp16s)vm_string_strlen(pToneSuccPtr);
      for (j = 0; j < nLen;){
         USC_ToneID num = USC_NoTone;
         int nextToneOffset=0;
         USC_ParseToneStr(pToneSuccPtr, &num,nLen - j,&nextToneOffset);
         j += nextToneOffset;
         pToneSuccPtr += nextToneOffset;
         if (num != USC_NoTone){
            vm_char buffer[MAX_STRING_LEN];
            uscRes = pUSC_TD->GenerateTone(hUSCTD, num, clParams.volume, clParams.lengthTone, &out);/*USC_NoOperation*/
            if(USC_NoError == uscRes) {
               if (vm_file_fwrite(out.pBuffer, 1, out.nbytes, Objects.f_out)!=out.nbytes) {
                  ASSERT_MSGBOX(1,  VM_STRING("TD example. Error at writing a pcm-file!\n"),  VM_STRING("Error"), USC_ERROR, &Objects);
               }
               USC_ToneId2Char(num, buffer);
               USC_OutputString(Objects.f_log, VM_STRING("%s "),buffer);
               if (clParams.type == DTMF){
                  if (vm_file_fwrite(Objects.pNoiseBuffer, 1, ndurationPause, Objects.f_out)!=ndurationPause){
                      ASSERT_MSGBOX(1,  VM_STRING("TD example. Error at writing a pcm-file!\n"),  VM_STRING("Error"), USC_ERROR, &Objects);
                  }
               }
            } else if(USC_NoOperation == uscRes) {
               vm_char buffer[MAX_STRING_LEN];
               USC_ToneId2Char(num, buffer);
               USC_OutputString(Objects.f_log, VM_STRING("\nUnsupported tone (%s). Skipped.\n"),buffer);
            } else {
               ASSERT_MSGBOX(uscRes == USC_NoError,  VM_STRING("TD example. USC GenerateTone failed!\n"),  VM_STRING("Error"), USC_ERROR, &Objects);
            }
         } else {
            USC_OutputString(Objects.f_log, VM_STRING("\nUnknown tone.\n"));
            break;
         }
      }
   }
   USC_OutputString(Objects.f_log, VM_STRING("\nFinish.\n"));

   /* Close all objects */
   ReleaseUsedObjects(&Objects);

   return 0;
}

