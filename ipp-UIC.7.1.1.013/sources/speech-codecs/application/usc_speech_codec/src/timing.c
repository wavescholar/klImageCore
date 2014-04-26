/*////////////////////////////////////////////////////////////////////////
//
// INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
// Copyright (c) 2006-2011 Intel Corporation. All Rights Reserved.
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
// Purpose: Codec Timing function.
//
////////////////////////////////////////////////////////////////////////*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ippcore.h"
#include "ipps.h"

#include "usc.h"

#include "util.h"
#include "loadcodec.h"
#include "wavfile.h"
#include "usccodec.h"
#include "vm_time.h"

#ifndef USC_MAX_NAME_LEN
   #define USC_MAX_NAME_LEN 64
#endif
extern int pcLosts;
Ipp32s Timing(CommandLineParams *clParams, WaveFileParams *inputWParams, FILE *f_log)
{
   vm_time_handle handle;
   vm_time ENCmeasure;
   vm_time DECmeasure;
   Ipp64f dTimeENC = 0.;
   Ipp64f dTimeDEC = 0.;
   Ipp32s numCodecs, lCallResult;
   Ipp32s lCodecNum, lVAD, nVAD, lBitrate, nRates, lRepeat;
   Ipp8s codecName[USC_MAX_NAME_LEN];
   USCParams uscENCParams, uscDECParams;
   Ipp32s duration = 0;
   Ipp32f spSeconds;

   Ipp8s *inputBuffer,*outputBuffer,*outputDecodedBuffer;

   int lCPUFreq;

#if defined( _WIN32_WCE )
   lCPUFreq = 400;
#else
   ippGetCpuFreqMhz(&lCPUFreq);
#endif

   inputBuffer = NULL;
   inputBuffer = (Ipp8s *)ippsMalloc_8u(inputWParams->DataSize);
   if(!inputBuffer) {
      USC_OutputString(f_log, "FAIL:Cannot allocate %d bytes memory\n",inputWParams->DataSize);
      if(f_log) fclose(f_log);
      return -1;
   }

   /*Read data from the input file to the buffer*/
   WavFileRead(inputWParams, (void *)inputBuffer, inputWParams->DataSize);

   if(clParams->puttocsv) {
      vm_file *f_csv = NULL;
      f_csv = OpenCSVFile(clParams->csvFileName);
      if (f_csv == NULL) {
         printf("\nWarning: File %s could not be open. CSV output ignored.\n", clParams->csvFileName);
      } else {
         OutputSysInfo2CSVFile(f_csv);
         AddCodecTableHeader2CSVFile(f_csv);
         CloseCSVFile(f_csv);
      }
   }

   numCodecs = GetNumLinkedCodecs();
   USCClear(&uscENCParams);
   USCClear(&uscDECParams);

   for(lCodecNum=0;lCodecNum < numCodecs;lCodecNum++) {

      uscENCParams.USC_Fns = Get_ith_StaticLinkedCodecs(lCodecNum, (Ipp8s *)codecName);
      if(clParams->TimingCodecName[0] != 0) {
         if(strcmp(codecName,clParams->TimingCodecName) != 0) continue;
      }
      uscDECParams.USC_Fns = uscENCParams.USC_Fns;
      lCallResult = USCCodecAllocInfo(&uscENCParams,f_log);
      if(lCallResult<0) return -1;
      lCallResult = USCCodecAllocInfo(&uscDECParams,f_log);
      if(lCallResult<0) return -1;
      lCallResult = USCCodecGetInfo(&uscENCParams,f_log);
      if(lCallResult<0) return -1;
      lCallResult = USCCodecGetInfo(&uscDECParams,f_log);
      if(lCallResult<0) return -1;

      {
         USC_PCMType streamType;
         streamType.bitPerSample = 16; //  
         streamType.nChannels = inputWParams->waveFmt.nChannels;
         streamType.sample_frequency = inputWParams->waveFmt.nSamplesPerSec;

         lCallResult = SetUSCEncoderPCMType(&uscENCParams, LINEAR_PCM, &streamType, f_log);
         if(lCallResult < 0) {
            continue;
         }
         lCallResult = SetUSCDecoderPCMType(&uscENCParams, LINEAR_PCM, &streamType, f_log);
         if(lCallResult < 0) {
            continue;
         }
      }
      /*If input file in PCM format than encode else decode*/
      if( (inputWParams->waveFmt.nFormatTag==ALAW_PCM/*A-law*/)||
          (inputWParams->waveFmt.nFormatTag==MULAW_PCM/*mu-law*/)) {
         lCallResult = USC_CvtToLinear(uscENCParams.pInfo->params.framesize, inputWParams->waveFmt.nFormatTag,
             &inputBuffer, (Ipp32s*)&inputWParams->DataSize);
         if(lCallResult<0) return MEMORY_FAIL;
 
      } 

      nVAD = uscENCParams.pInfo->params.modes.vad;
      nRates = uscENCParams.pInfo->nRates;
      for(lVAD=0;lVAD <= nVAD;lVAD++) {
         for(lBitrate=0;lBitrate < nRates;lBitrate++) {
            lCallResult = USCCodecGetInfo(&uscENCParams,f_log);
            if(lCallResult<0) {
               USC_OutputString(f_log, "FAIL:USCCodecGetInfo failed for encoder\n");
               if(f_log) fclose(f_log);
               return -1;
            }
            lCallResult = USCCodecGetInfo(&uscDECParams,f_log);
            if(lCallResult<0) {
               USC_OutputString(f_log, "FAIL:USCCodecGetInfo failed for decoder\n");
               if(f_log) fclose(f_log);
               return -1;
            }

            clParams->Vad = lVAD;
            clParams->bitrate = uscENCParams.pInfo->pRateTbl[lBitrate].bitrate;

            /*Set params for encode*/
            SetUSCEncoderParams(&uscENCParams, clParams);
            /*Set params for decode*/
            SetUSCDecoderParams(&uscDECParams, clParams);
            {
               USC_PCMType streamType;
               streamType.bitPerSample = 16;
               streamType.nChannels = inputWParams->waveFmt.nChannels;
               streamType.sample_frequency = inputWParams->waveFmt.nSamplesPerSec;

               lCallResult = SetUSCEncoderPCMType(&uscENCParams, LINEAR_PCM, &streamType, f_log);
               if(lCallResult < 0) {
                  USC_OutputString(f_log, "FAIL:SetUSCEncoderPCMType\n");
                  if(f_log) fclose(f_log);
                  return -1;
               }
               lCallResult = SetUSCDecoderPCMType(&uscDECParams, LINEAR_PCM, &streamType, f_log);
               if(lCallResult < 0) {
                  USC_OutputString(f_log, "FAIL:SetUSCDecoderPCMType\n");
                  if(f_log) fclose(f_log);
                  return -1;
               }
            }
            /*Alloc memory for the codec*/
            lCallResult = USCCodecAlloc(&uscENCParams, clParams);
            if(lCallResult<0) {
               USC_OutputString(f_log, "FAIL:USCCodecAlloc failed for encoder.\n");
               if(f_log) fclose(f_log);
               return -1;
            }
            lCallResult = USCCodecAlloc(&uscDECParams, clParams);
            if(lCallResult<0) {
               USC_OutputString(f_log, "FAIL:USCCodecAlloc failed for decoder.\n");
               if(f_log) fclose(f_log);
               return -1;
            }

            /*Init encoder*/
            lCallResult = USCEncoderInit(&uscENCParams, clParams,f_log);
            if(lCallResult<0) {
               USC_OutputString(f_log, "    skipped bitrate: %d\n",uscENCParams.pInfo->pRateTbl[lBitrate].bitrate);
               continue;
            }
            lCallResult = USCDecoderInit(&uscDECParams, clParams,f_log);
            if(lCallResult<0) {
               USC_OutputString(f_log, "FAIL:USCDecoderInit\n");
               if(f_log) fclose(f_log);
               return -1;
            }
            duration = 0;
            vm_time_open(&handle);
            vm_time_init(&ENCmeasure);
            vm_time_init(&DECmeasure);
            for(lRepeat = 0;lRepeat < clParams->nRepeat;lRepeat++) {
               outputBuffer = NULL;
               outputBuffer = (Ipp8s *)ippsMalloc_8u(uscENCParams.pInfo->params.framesize);
               if(!outputBuffer) {
                  USC_OutputString(f_log, "FAIL:Cannot allocate %d bytes memory\n",uscENCParams.pInfo->params.framesize);
                  if(f_log) fclose(f_log);
                  return -1;
               }
               outputDecodedBuffer = NULL;
               outputDecodedBuffer = (Ipp8s *)ippsMalloc_8u(uscDECParams.pInfo->params.framesize);
               if(!outputDecodedBuffer) {
                  USC_OutputString(f_log, "FAIL:Cannot allocate %d bytes memory\n",uscENCParams.pInfo->params.framesize);
                  if(f_log) fclose(f_log);
                  return -1;
               }
               /*Allocate memory for the input buffer. Size of input buffer is equal to the size of input file*/
               {
                  Ipp32s currLen, lLowBound;
                  Ipp32s infrmLen, FrmDataLen;
                  Ipp8s *pInputBuffPtr;
                  USC_PCMStream PCMStream;
                  USC_PCMStream DecodedPCMStream;
                  USC_Bitstream Bitstream;

                  pInputBuffPtr = inputBuffer;
                  currLen = inputWParams->DataSize;
                  DecodedPCMStream.pBuffer = outputDecodedBuffer;

                  USCCodecGetTerminationCondition(&uscENCParams, &lLowBound);

                  while(currLen > lLowBound) {
                     int nbytes;
                     USC_Bitstream *pBitstream = &Bitstream;

                     PCMStream.bitrate = uscENCParams.pInfo->params.modes.bitrate;
                     PCMStream.nbytes = uscENCParams.pInfo->params.framesize;
                     PCMStream.pBuffer = pInputBuffPtr;
                     PCMStream.pcmType.bitPerSample = uscENCParams.pInfo->params.pcmType.bitPerSample;
                     PCMStream.pcmType.sample_frequency = uscENCParams.pInfo->params.pcmType.sample_frequency;
                     PCMStream.pcmType.nChannels = uscENCParams.pInfo->params.pcmType.nChannels;
                     Bitstream.pBuffer = outputBuffer;
                     infrmLen = 0;
                     /*Encode one frame*/
                     vm_time_start(handle, &ENCmeasure);
                     FrmDataLen = USCCodecEncode(&uscENCParams, &PCMStream,pBitstream,f_log);
                     dTimeENC = vm_time_stop(handle, &ENCmeasure);
                     if(FrmDataLen < 0) {
                        USC_OutputString(f_log, "FAIL:USCCodecEncode\n");
                        if(f_log) fclose(f_log);
                        return -1;
                     }
                     infrmLen += FrmDataLen;

                     /*Decode one frame*/
                     vm_time_start(handle, &DECmeasure);
                     nbytes = pBitstream->nbytes; 
                     if(pcLosts && rand() < RAND_MAX*pcLosts/100 ) 
                         pBitstream = NULL;
                     FrmDataLen = USCCodecDecode(&uscDECParams, pBitstream,&DecodedPCMStream,f_log);
                     if(!pBitstream) {// lost frame
                         FrmDataLen = nbytes;
                     }
                     dTimeDEC = vm_time_stop(handle, &DECmeasure);
                     if(FrmDataLen < 0) {
                        USC_OutputString(f_log, "FAIL:USCCodecDecode\n");
                        if(f_log) fclose(f_log);
                        return -1;
                     }
                     /* Move pointer to the next position*/
                     currLen -= infrmLen;
                     pInputBuffPtr += infrmLen;
                     duration += infrmLen;
                  }/*while(currLen > lLowBound)*/
               }
               if(outputBuffer) ippsFree(outputBuffer);
               if(outputDecodedBuffer) ippsFree(outputDecodedBuffer);
            } /*lRepeat*/
            spSeconds = (duration/(Ipp32f)(uscENCParams.pInfo->params.pcmType.bitPerSample>>3))/
                        (Ipp32f)uscENCParams.pInfo->params.pcmType.sample_frequency;

            if(f_log) {
               fprintf(f_log,"Processed %g sec of speech\n",spSeconds);
            } else {
               printf("Processed %g sec of speech\n",spSeconds);
            }
            if(clParams->puttologfile) {
               fprintf(f_log,"%4.2f MHz per channel\n",(dTimeENC/spSeconds)*lCPUFreq);
               fprintf(f_log,"%4.2f MHz per channel\n",(dTimeDEC/spSeconds)*lCPUFreq);
            } else {
               printf("%4.2f MHz per channel\n",(dTimeENC/spSeconds)*lCPUFreq);
               printf("%4.2f MHz per channel\n",(dTimeDEC/spSeconds)*lCPUFreq);
            }

            OutputDuplexString2CSVFile(clParams, uscENCParams.pInfo, spSeconds,
                                      (Ipp32f)(dTimeENC/spSeconds)*(Ipp32f)lCPUFreq,(Ipp32f)(dTimeDEC/spSeconds)*(Ipp32f)lCPUFreq);
            vm_time_close(&handle);
            /*Free codec memory*/
            USCFree(&uscENCParams);
            USCFree(&uscDECParams);
            lCallResult = USCCodecAllocInfo(&uscENCParams,f_log);
            if(lCallResult<0) return -1;
            lCallResult = USCCodecAllocInfo(&uscDECParams,f_log);
            if(lCallResult<0) return -1;
         }/*lBitrate*/
      }/*lVAD*/
   }/*lCodecNum*/

   /*Free codec memory*/
   USCFree(&uscENCParams);
   USCFree(&uscDECParams);

   /* Close input file*/
   WavFileClose(inputWParams);
   if(inputBuffer) ippsFree(inputBuffer);

   return 0;
}
