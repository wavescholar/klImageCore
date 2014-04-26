/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
//  Intel(R) Integrated Performance Primitives Audio Processing
//  Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
//  Description:
//      Intel(R) IPP Sample Noise Reduction example built on Intel(R) IPP libraries.
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

#include "types.h"

#include "vm_time.h"

#define COPYRIGHT_STRING "Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved."

#ifdef _USC_ALL
#  undef _USC_ALL
#  define _USC_FILTERS
#endif

#include "usc_objects_decl.h"

typedef struct _StaticFilters {
   Ipp8s filterName[64];
   void *funcPtr;
}StaticFilters;

StaticFilters LinkedAlgs[] = {
//#if (defined _USC_ALL || defined _USC_FILTERS || defined _USC_EPHM_NR)
//   "IPP_NR_EPHM",&USC_NR_Fxns,
//#endif
#if (defined _USC_ALL || defined _USC_FILTERS || defined _USC_EC_NR)
   "IPP_NR_EC",&USC_ECNR_Fxns,
#endif
#if (defined _USC_ALL || defined _USC_FILTERS || defined _USC_G723_VAD)
   "IPP_VADG723",&USC_G723_VAD_Fxns,
#endif
#if (defined _USC_ALL  || defined _USC_FILTERS || defined _USC_G729FP_VAD)
   "IPP_VADG729_FP",&USC_G729FP_VAD_Fxns,
#endif
#if (defined _USC_ALL || defined _USC_FILTERS || defined _USC_G729_VAD)
   "IPP_VADG729",&USC_G729_VAD_Fxns,
#endif
//#if (defined _USC_ALL || defined _USC_FILTERS || defined _USC_INTEL_VAD)
//   "IPP_VAD",&USC_SR_VAD_Fxns,
//#endif
#if (defined _USC_ALL || defined _USC_FILTERS || defined _USC_AMRWB_VAD)
   "IPP_VADAMRWBE",&USC_AMRWBE_VAD_Fxns,
#endif
#if (defined _USC_ALL || defined _USC_FILTERS || defined _USC_RTA_NR)
   "IPP_NR_RTA_FP",&USC_RTANR_FP_Fxns,
#endif
#if (defined _USC_ALL || defined _USC_FILTERS || defined _USC_ALC)
   "IPP_ALC",&USC_ALC_Fxns,
#endif
   "END",NULL
};

/* /////////////////////////////////////////////////////////////////////////////
//  Name:        EnumerateStaticLinkedCodecs
//  Purpose:     Enumerate all static linked codecs by name.
//  Returns:     None.
//  Parameters:
*/
void EnumerateStaticLinkedAlgs(FILE *f_log)
{
   Ipp32s i = 0;
   USC_OutputString(f_log, "Following filters are linked:\n");
   while(strcmp(LinkedAlgs[i].filterName,"END")!=0) {
      USC_OutputString(f_log, "           %s\n",LinkedAlgs[i].filterName);
      i++;
   }

   return;
}

void *TryToFindStatic(const Ipp8s *algName)
{
   void *pAddr = NULL;
   Ipp32s i=0;
   while(strcmp(LinkedAlgs[i].filterName,"END")!=0) {
      if(strcmp(algName, LinkedAlgs[i].filterName) == 0) {
         pAddr = LinkedAlgs[i].funcPtr;
      }
      i++;
   }
   return pAddr;
}

#define OPEN_FILE_ERROR     -1   /* return code on file open error */
#define MEMORY_ERROR        -2   /* return code on memory allocation error */
#define USC_ERROR           -3   /* return code on error USC operation error*/

#define ASSERT_MSGBOX(cond, msg, title, exitcode, pObjects)   \
{                                                             \
  if (!(cond))                                                \
  {                                                           \
    USC_OutputString(Objects.f_log, "%s: %s\n", title, msg);  \
    ReleaseUsedObjects(pObjects);                             \
    exit(exitcode);                                           \
  }                                                           \
}

int main(int argc, char **argv)
{
   USC_Filter_Fxns *pUSC_Filter = NULL;
   static USC_FrameType nState, old_nState;
   static Ipp32s FirstAccess = 1, oldduration = 0, dur_voice = 0, duration = 0;
   USC_Handle hUSCFilter;
   USC_Status uscRes;
   USC_PCMStream in, out;
   Ipp32f spSeconds;
   Ipp32s VADAlgoFlag;
   Ipp32s pInfoSize=0;

   CommandLineParams clParams;
   UsedObjects Objects;

   Ipp32u     cSamplesRead;                       /* number of samples read from input file */
   Ipp32s i;

   /*Timing vars*/
   Ipp64f dTime = 0.;
   int lCPUFreq=400;
   vm_time measure;
   vm_time_handle handle;
   Ipp32s choosenMode = -1;

#if defined( _WIN32_WCE )

   GetModuleFileName( hinst, wexename, WINCE_EXENAME_SIZE );
   sprintf( exename, "%ls", wexename );
   sprintf( cmdline, "%ls", lpCmdLine );
   argc = parseCmndLine( exename, cmdline, line, WINCE_CMDLINE_SIZE, argv, WINCE_NCMD_PARAMS );

#endif

   ippStaticInit();

   SetCommandLineByDefault(&clParams);
   InitUsedObjects(&Objects);

   ReadCommandLine(&clParams, argc, argv);

   if(clParams.puttologfile) {
      Objects.f_log=fopen(clParams.logFileName,"a");
      if(Objects.f_log==NULL) {
         printf("Cannot open %s log file for writing\n",clParams.logFileName);
         printf("Log file ignored.\n");
         clParams.puttologfile = 0;
      }
   }

   if(clParams.optionReport) {
      const  IppLibraryVersion *verIppSC;
      verIppSC = ippscGetLibVersion();
      USC_OutputString(Objects.f_log, "%s\n",COPYRIGHT_STRING);
      USC_OutputString(Objects.f_log, "Intel Unified Speech Codec interface based Noise Reduction and VAD sample\n");
      USC_OutputString(Objects.f_log, "The Intel(R) IPPSR library used:  %d.%d.%d Build %d, name %s\n",
         verIppSC->major,verIppSC->minor,verIppSC->majorBuild,verIppSC->build,verIppSC->Name);
   }

   if(clParams.enumerate) {
      if(clParams.puttologfile) {
         EnumerateStaticLinkedAlgs(Objects.f_log);
      } else {
         EnumerateStaticLinkedAlgs(NULL);
      }
      ReleaseUsedObjects(&Objects);
      return 0;
   }

   if((!clParams.inputFileName[0]) ||(!clParams.outputFileName[0])) {
      PrintUsage(argv[0], Objects.f_log);
      ReleaseUsedObjects(&Objects);
      return 0;
   }

   pUSC_Filter = (USC_Filter_Fxns *)TryToFindStatic(clParams.algFilter);
   if(NULL == pUSC_Filter) {
      USC_OutputString(Objects.f_log, "Error: Couldn't load %s filter\n", clParams.algFilter);
      ReleaseUsedObjects(&Objects);
      exit(USC_ERROR);
   }
   USC_OutputString(Objects.f_log, "Statically load %s filter\n", clParams.algFilter);

   /* Check loaded USC object type */
   if(pUSC_Filter->std.algType==USC_FilterVAD) {
      VADAlgoFlag = 1;
   } else if(pUSC_Filter->std.algType==USC_Filter) {
      VADAlgoFlag = 0;
   } else {
      USC_OutputString(Objects.f_log, "Error: Unsupported %d USC object type.\n", pUSC_Filter->std.algType);
      ReleaseUsedObjects(&Objects);
      exit(USC_ERROR);
   }

   /* Open input and output files */
   Objects.f_in = fopen(clParams.inputFileName,"rb");
   ASSERT_MSGBOX(Objects.f_in != NULL, "Filter example. Cannot open input file!\n", "Error", OPEN_FILE_ERROR, &Objects);
   Objects.f_out = fopen(clParams.outputFileName,"wb");
   ASSERT_MSGBOX(Objects.f_out != NULL, "Filter example. Cannot open output file!\n", "Error", OPEN_FILE_ERROR, &Objects);

   /* Get the NR filter info */
   uscRes = pUSC_Filter->std.GetInfoSize(&pInfoSize);
   ASSERT_MSGBOX(uscRes == USC_NoError, "Filter example. USC GetInfoSize failed!\n", "Error", USC_ERROR, &Objects);

   Objects.pInfo =  (USC_FilterInfo*)ippsMalloc_8u(pInfoSize);
   ASSERT_MSGBOX(Objects.pInfo != NULL, "Out of memory!\n", "Error", MEMORY_ERROR, &Objects);

   /* Get the NR filter info */
   uscRes = pUSC_Filter->std.GetInfo((USC_Handle)NULL, Objects.pInfo);
   ASSERT_MSGBOX(uscRes == USC_NoError, "Filter example. USC GetInfo failed!\n", "Error", USC_ERROR, &Objects);

   for(i=0;i<Objects.pInfo->nOptions;i++) {
      if(Objects.pInfo->params[i].pcmType.sample_frequency == clParams.SampleFreq) {
         choosenMode = i;
         break;
      }
   }

   ASSERT_MSGBOX(choosenMode >= 0, "Filter example. Unsupported sample frequency!\n", "Error", USC_ERROR, &Objects);

   uscRes = pUSC_Filter->std.NumAlloc(&Objects.pInfo->params[choosenMode], &Objects.nBanks);
   ASSERT_MSGBOX(uscRes == USC_NoError, "Filter example. USC NumAlloc failed!\n", "Error", USC_ERROR, &Objects);

   /* allocate memory for memory bank table */
   Objects.pBanks = (USC_MemBank*)ippsMalloc_8u(sizeof(USC_MemBank)*Objects.nBanks);
   ASSERT_MSGBOX(Objects.pBanks != NULL, "Out of memory!\n", "Error", MEMORY_ERROR, &Objects);

   /* Query how big has to be each block */
   uscRes = pUSC_Filter->std.MemAlloc(&Objects.pInfo->params[choosenMode], Objects.pBanks);
   ASSERT_MSGBOX(uscRes == USC_NoError, "Filter example. USC MemAlloc failed!\n", "Error", USC_ERROR, &Objects);

   /* allocate memory for each block */
   for(i=0; i<Objects.nBanks;i++){
      Objects.pBanks[i].pMem = NULL;
      Objects.pBanks[i].pMem = (Ipp8s*)ippsMalloc_8u(Objects.pBanks[i].nbytes);
      ippsZero_8u((Ipp8u*)Objects.pBanks[i].pMem,Objects.pBanks[i].nbytes);
   }

   for(i=0; i<Objects.nBanks;i++){
      ASSERT_MSGBOX(Objects.pBanks[i].pMem != NULL, "Out of memory!\n", "Error", MEMORY_ERROR, &Objects);
   }

   uscRes = pUSC_Filter->std.Init(&Objects.pInfo->params[choosenMode], Objects.pBanks, &hUSCFilter);
   ASSERT_MSGBOX(uscRes == USC_NoError, "Filter example. USC Init failed!\n", "Error", USC_ERROR, &Objects);
   Objects.pInputBuffer = (Ipp8s*)ippsMalloc_8u(Objects.pInfo->params[choosenMode].framesize);
   ASSERT_MSGBOX(Objects.pInputBuffer != NULL, "Out of memory!\n", "Error", MEMORY_ERROR, &Objects);
   Objects.pOutputBuffer = (Ipp8s*)ippsMalloc_8u(Objects.pInfo->params[choosenMode].framesize);
   ASSERT_MSGBOX(Objects.pOutputBuffer != NULL, "Out of memory!\n", "Error", MEMORY_ERROR, &Objects);

   USC_OutputString(Objects.f_log, "\nStart processing.\n");
   USC_OutputString(Objects.f_log, "Input  file: %s\n",clParams.inputFileName);
   USC_OutputString(Objects.f_log, "Output file: %s\n",clParams.outputFileName);

   if(VADAlgoFlag==0){
      cSamplesRead = (Ipp32u)fread(Objects.pInputBuffer, 1, Objects.pInfo->params[choosenMode].framesize, Objects.f_in);

      if(cSamplesRead==(Ipp32u)Objects.pInfo->params[choosenMode].framesize) {
#if defined (_BIG_ENDIAN)
         SwapBytes_16u((Ipp16u *)Objects.pInputBuffer,cSamplesRead>>1);
#endif
         uscRes = pUSC_Filter->SetDlyLine(hUSCFilter, Objects.pInputBuffer);
         if(uscRes == USC_NoOperation) { /*Current algorithm doesn't req such operation*/
            fseek(Objects.f_in,-Objects.pInfo->params[choosenMode].framesize,SEEK_CUR);
         } else {
            ASSERT_MSGBOX(uscRes == USC_NoError, "Filter example. USC SetDlyLine failed!\n", "Error", USC_ERROR, &Objects);
         }
      } else {
         cSamplesRead=0;
      }
   } else {
      cSamplesRead=(Ipp32u)Objects.pInfo->params[choosenMode].framesize;
   }

   in.bitrate = Objects.pInfo->params[choosenMode].pcmType.sample_frequency * Objects.pInfo->params[choosenMode].pcmType.bitPerSample;
   in.nbytes = Objects.pInfo->params[choosenMode].framesize;
   in.pBuffer = Objects.pInputBuffer;
   out.bitrate = in.bitrate;

   in.pcmType.bitPerSample = Objects.pInfo->params[choosenMode].pcmType.bitPerSample;
   in.pcmType.sample_frequency = Objects.pInfo->params[choosenMode].pcmType.sample_frequency;

   vm_time_open(&handle);
   vm_time_init(&measure);

   while(cSamplesRead > 0)
   {
      cSamplesRead = (Ipp32u)fread(Objects.pInputBuffer, 1, Objects.pInfo->params[choosenMode].framesize, Objects.f_in);

      if(cSamplesRead != (Ipp32u)Objects.pInfo->params[choosenMode].framesize) break;
#if defined (_BIG_ENDIAN)
      SwapBytes_16u((Ipp16u *)Objects.pInputBuffer,cSamplesRead>>1);
#endif

      out.pBuffer = Objects.pOutputBuffer;
      out.nbytes = Objects.pInfo->params[choosenMode].framesize;

      vm_time_start(handle, &measure);

      uscRes = pUSC_Filter->Filter(hUSCFilter, &in, &out, &nState);

      dTime = vm_time_stop(handle, &measure);

      ASSERT_MSGBOX(uscRes == USC_NoError, "Filter example. USC Filt failed!\n", "Error", USC_ERROR, &Objects);

      if(out.nbytes!=0) {
#if defined (_BIG_ENDIAN)
         SwapBytes_16u((Ipp16u *)out.pBuffer,cSamplesRead>>1);
#endif
         fwrite(out.pBuffer, cSamplesRead, 1, Objects.f_out);
      }
      duration += cSamplesRead;
      if (FirstAccess) {
         FirstAccess = 0;
         old_nState = nState;
         oldduration = duration;
      }
      if (old_nState == ACTIVE &&
         ((nState == NODECISION) || (nState == INACTIVE)))
            dur_voice = dur_voice + (duration - oldduration);

      else if (nState == ACTIVE &&
         ((old_nState == NODECISION) || (old_nState == INACTIVE)))
            oldduration = duration;
      old_nState = nState;
   }
   if(VADAlgoFlag==1){
      USC_OutputString(Objects.f_log, "\nStatistics:\n Lenght of input audio: %d ms",duration/(Objects.pInfo->params[choosenMode].pcmType.sample_frequency*2/1000));
      USC_OutputString(Objects.f_log, "\n Total time of voice activity: %d ms", dur_voice/(Objects.pInfo->params[choosenMode].pcmType.sample_frequency*2/1000));
   }
   USC_OutputString(Objects.f_log, "\nFinish.\n");

   ippGetCpuFreqMhz(&lCPUFreq);

   spSeconds = (duration/(Ipp32f)(Objects.pInfo->params[choosenMode].pcmType.bitPerSample>>3))/
      (Ipp32f)Objects.pInfo->params[choosenMode].pcmType.sample_frequency;

   if(clParams.puttologfile) {
      fprintf(Objects.f_log,"%4.2f MHz per channel\n",(Ipp32f)(dTime/spSeconds)*(Ipp32f)lCPUFreq);
   } else {
      printf("%4.2f MHz per channel\n",(Ipp32f)(dTime/spSeconds)*(Ipp32f)lCPUFreq);
   }

   vm_time_close(&handle);

   /* Close all objects */
   ReleaseUsedObjects(&Objects);

   return 0;
}

