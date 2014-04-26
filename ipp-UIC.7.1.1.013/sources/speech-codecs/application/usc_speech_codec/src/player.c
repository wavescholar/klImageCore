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
// Purpose: Speech sample. Main program file.
//
////////////////////////////////////////////////////////////////////////*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ippcore.h"
#include "ipps.h"
#include "ippsc.h"

#include "usc.h"

#include "util.h"
#include "loadcodec.h"
#include "wavfile.h"
#include "usccodec.h"
#include "vm_time.h"



#define COPYRIGHT_STRING "Copyright (c) 2005-2011 Intel Corporation. All Rights Reserved."
int pcLosts=0;
Ipp32s ProcessOneFrameOneChannel(USCParams *uscPrms, Ipp8s *inputBuffer,
                              Ipp8s *outputBuffer, Ipp32s *pSrcLenUsed, Ipp32s *pDstLenUsed, FILE *f_log)
{
   Ipp32s frmlen, infrmLen, FrmDataLen;
   USC_PCMStream PCMStream;
   USC_Bitstream Bitstream;
   if(uscPrms->pInfo->params.direction==USC_ENCODE) {
      /*Do the pre-procession of the frame*/
      infrmLen = USCEncoderPreProcessFrame(uscPrms, inputBuffer,
                                             outputBuffer,&PCMStream,&Bitstream);
      /*Encode one frame*/
      FrmDataLen = USCCodecEncode(uscPrms, &PCMStream,&Bitstream,f_log);
      if(FrmDataLen < 0) return USC_CALL_FAIL;
      infrmLen += FrmDataLen;
      /*Do the post-procession of the frame*/
      frmlen = USCEncoderPostProcessFrame(uscPrms, inputBuffer,
                                             outputBuffer,&PCMStream,&Bitstream);
      *pSrcLenUsed = infrmLen;
      *pDstLenUsed = frmlen;
   } else {
      /*Do the pre-procession of the frame*/
      int nbytes;
      USC_Bitstream *pBitstream = &Bitstream;

      infrmLen = USCDecoderPreProcessFrame(uscPrms, inputBuffer,
                                             outputBuffer, pBitstream, &PCMStream);
      nbytes = pBitstream->nbytes;
      if(pcLosts && rand() < RAND_MAX*pcLosts/100 )
         pBitstream = NULL;
      /*Decode one frame*/
      FrmDataLen = USCCodecDecode(uscPrms, pBitstream, &PCMStream, f_log);
      if(!pBitstream) {// lost frame
          FrmDataLen = nbytes;
      }
      else if(FrmDataLen < 0) return USC_CALL_FAIL;
      infrmLen += FrmDataLen;
      /*Do the post-procession of the frame*/
      frmlen = USCDecoderPostProcessFrame(uscPrms, inputBuffer,
                                             outputBuffer,&Bitstream,&PCMStream);
      *pSrcLenUsed = infrmLen;
      *pDstLenUsed = frmlen;
   }
   return 0;
}
#define MAX_LEN(a,b) ((a)>(b))? (a):(b)
Ipp32s ProcessOneChannel(LoadedCodec *codec, WaveFileParams *wfOutputParams, Ipp8s *inputBuffer,
                              Ipp32s inputLen, Ipp8s *outputBuffer, Ipp32s *pDuration, Ipp32s *dstLen, FILE *f_log)
{
   Ipp32s duration=0, outLen = 0, currLen;
   Ipp8s *pInputBuffPtr = inputBuffer;
   Ipp32s frmlen, infrmLen, cvtLen;
   Ipp32s lLowBound;
   currLen = inputLen;

   USCCodecGetTerminationCondition(&codec->uscParams, &lLowBound);

   while(currLen > lLowBound) {
      ProcessOneFrameOneChannel(&codec->uscParams, pInputBuffPtr,
                              outputBuffer, &infrmLen, &frmlen,f_log);
      /* Write encoded data to the output file*/
      cvtLen = frmlen;
      if((wfOutputParams->waveFmt.nFormatTag==ALAW_PCM)||(wfOutputParams->waveFmt.nFormatTag==MULAW_PCM)) {
         USC_CvtToLaw(&codec->uscParams, wfOutputParams->waveFmt.nFormatTag, outputBuffer, &cvtLen);
      }
      WavFileWrite(wfOutputParams, outputBuffer, cvtLen);
      /* Move pointer to the next position*/
      currLen -= infrmLen;
      pInputBuffPtr += infrmLen;
      duration += MAX_LEN(infrmLen,frmlen);
      outLen += frmlen;
   }

   *pDuration = duration;
   *dstLen = outLen;
   return 0;
}

Ipp32s main(int argc, char **argv)
{
   Ipp32s lCallResult, encode;
   Ipp32s PCMType = -1;
   LoadedCodec codec;
   WaveFileParams wfInputParams;
   WaveFileParams wfOutputParams;
   CommandLineParams clParams;
   Ipp8s *inputBuffer=NULL;
   Ipp8s *outputBuffer=NULL;
   Ipp32s currLen, duration;
   const  IppLibraryVersion *verIppSC;
   Ipp32f spSeconds;
   FILE *f_log=NULL;

   /*
      Use IPP Run-Time dispatching, enabled if static libs are linked
      ippStaticInit() - IPP best suited for this CPU
      ippStaticInitCpu(IppCpuType) - IPP by choice, for example
          IppCpuType = ippCpuUnknown - C-coded IPP (PX)
   */
   srand((unsigned)time(0));
   ippStaticInit();

   SetCommandLineByDefault(&clParams);
   /*Get params from comman line.*/
   ReadCommandLine(&clParams, argc, argv);
   pcLosts = clParams.pcLost; // % losts for decode

   if(clParams.puttologfile) {
      f_log=fopen(clParams.logFileName,"a");
      if(f_log==NULL) {
         printf("Warning:Cannot open %s log file for writing\n",clParams.logFileName);
         printf("Warning:Log file ignored.\n");
         clParams.puttologfile = 0;
      }
   }

   if(clParams.optionReport) {
      verIppSC = ippscGetLibVersion();
      USC_OutputString(f_log, "%s\n",COPYRIGHT_STRING);
      USC_OutputString(f_log, "Intel Unified Speech Codec interface based console player\n");
      USC_OutputString(f_log, "The Intel(R) IPPSC library used:  %d.%d.%d Build %d, name %s\n",
             verIppSC->major,verIppSC->minor,verIppSC->majorBuild,verIppSC->build,verIppSC->Name);
   }

   if(clParams.enumerate) {
      EnumerateStaticLinkedCodecs(f_log);
      if(f_log) fclose(f_log);
      return 0;
   }

   if((!clParams.inputFileName[0]) ||(!clParams.outputFileName[0])) {
      PrintUsage(argv[0]);
      return 0;
   }
   /* Open input file and read header*/
   InitWavFile(&wfInputParams);
   lCallResult = OpenWavFile(&wfInputParams, clParams.inputFileName, FILE_READ);
   if(lCallResult==-1) {
      USC_OutputString(f_log, "FAIL:Cannot open %s file for reading\n",clParams.inputFileName);
      if(f_log) fclose(f_log);
      return FOPEN_FAIL;
   }
   /*Read WAVE file header*/
   lCallResult = WavFileReadHeader(&wfInputParams);
   if(lCallResult<0) {
      if(lCallResult==-1) USC_OutputString(f_log, "FAIL:Cannot read from the %s file\n",clParams.inputFileName);
      if(lCallResult==-2) USC_OutputString(f_log, "FAIL:File %s isn't in RIFF format\n",clParams.inputFileName);
      if(lCallResult==-3) USC_OutputString(f_log, "FAIL:File %s is in RIFF format but not in a WAVE foramt\n",
                                       clParams.inputFileName);;
      WavFileClose(&wfInputParams);
      if(f_log) fclose(f_log);
      return FOPEN_FAIL;
   }
   if(clParams.timing) {
      lCallResult = Timing(&clParams, &wfInputParams, f_log);
      return lCallResult;
   }

   USCClear(&codec.uscParams);
   codec.pSOHandle = NULL;

   /*If input file in PCM format than encode else decode*/
   if((wfInputParams.waveFmt.nFormatTag==LINEAR_PCM)||
            (wfInputParams.waveFmt.nFormatTag==ALAW_PCM/*A-law*/)||
            (wfInputParams.waveFmt.nFormatTag==MULAW_PCM/*mu-law*/)) {
      encode = 1;
      PCMType = wfInputParams.waveFmt.nFormatTag;
      strcpy((Ipp8s*)codec.codecName, clParams.codecName);
      codec.lIsVad = (clParams.Vad>0);
   } else {
      encode = 0;
      codec.lFormatTag = wfInputParams.waveFmt.nFormatTag;
      clParams.bitrate = wfInputParams.bitrate;
   }

   if(encode) {
      /*Load codec by name from command line*/
      lCallResult = LoadUSCCodecByName(&codec,f_log);
      if(lCallResult<0) {
         USC_OutputString(f_log, "FAIL:Cannot find %s encoder.\n",codec.codecName);
         if(f_log) fclose(f_log);
         return LOAD_CODEC_FAIL;
      }
      /*Get USC codec params*/
      lCallResult = USCCodecAllocInfo(&codec.uscParams,f_log);
      if(lCallResult<0) return USC_CALL_FAIL;
      lCallResult = USCCodecGetInfo(&codec.uscParams,f_log);
      if(lCallResult<0) return USC_CALL_FAIL;
      /*Get its supported format details*/
      lCallResult = GetUSCCodecParamsByFormat(&codec,BY_NAME,f_log);
      if(lCallResult<0) {
         USC_OutputString(f_log, "FAIL:Cannot find %s encode format description.\n",codec.codecName);
         if(f_log) fclose(f_log);
         return LOAD_CODEC_FAIL;
      }
   } else {
      /*Get its supported format details*/
      lCallResult = GetUSCCodecParamsByFormat(&codec,BY_FORMATTAG,f_log);
      if(lCallResult<0) {
         USC_OutputString(f_log, "FAIL:Unknown decode format.\n");
         if(f_log) fclose(f_log);
         return LOAD_CODEC_FAIL;
      }
      /*Load codec by name from format description*/
      lCallResult = LoadUSCCodecByName(&codec,f_log);
      if(lCallResult<0) {
         USC_OutputString(f_log, "FAIL:Cannot find %s decoder.\n",codec.codecName);
         if(f_log) fclose(f_log);
         return LOAD_CODEC_FAIL;
      }
      /*Get USC codec params*/
      lCallResult = USCCodecAllocInfo(&codec.uscParams,f_log);
      if(lCallResult<0) return USC_CALL_FAIL;
      lCallResult = USCCodecGetInfo(&codec.uscParams,f_log);
      if(lCallResult<0) return USC_CALL_FAIL;
   }
   /* Check sample_frequence, bitspersample and number of channels*/
   if(encode) {
      USC_PCMType streamType;
      streamType.bitPerSample = wfInputParams.waveFmt.nBitPerSample;
      streamType.nChannels = wfInputParams.waveFmt.nChannels;
      streamType.sample_frequency = wfInputParams.waveFmt.nSamplesPerSec;

      lCallResult = SetUSCEncoderPCMType(&codec.uscParams, PCMType, &streamType, f_log);
      if(lCallResult < 0) {
         USC_OutputString(f_log, "FAIL:Unsupported PCM type: %d bps, %d channels %d sample freq by %s codec\n",
               streamType.bitPerSample,streamType.nChannels,streamType.sample_frequency, codec.uscParams.pInfo->name);
         if(f_log) fclose(f_log);
         return UNKNOWN_FORMAT;
      }
   } else {
      USC_PCMType streamType;
      streamType.bitPerSample = wfInputParams.waveFmt.nBitPerSample;
      streamType.nChannels = wfInputParams.waveFmt.nChannels;
      streamType.sample_frequency = wfInputParams.waveFmt.nSamplesPerSec;

      lCallResult = SetUSCDecoderPCMType(&codec.uscParams, PCMType, &streamType, f_log);
      if(lCallResult < 0) {
         USC_OutputString(f_log, "FAIL:Unsupported PCM type: %d bps, %d channels %d sample freq by %s codec\n",
               streamType.bitPerSample,streamType.nChannels,streamType.sample_frequency, codec.uscParams.pInfo->name);
         if(f_log) fclose(f_log);
         return UNKNOWN_FORMAT;
      }
   }

   if(encode) {
      /*Set params for encode*/
      SetUSCEncoderParams(&codec.uscParams, &clParams);
   } else {
      /*Set params for decode*/
      SetUSCDecoderParams(&codec.uscParams, &clParams);
   }
   /*Alloc memory for the codec*/
   lCallResult = USCCodecAlloc(&codec.uscParams, &clParams);
   if(lCallResult<0) return USC_CALL_FAIL;
   if(encode) {
      /*Init encoder*/
      lCallResult = USCEncoderInit(&codec.uscParams, &clParams,f_log);
   } else {
      /*Init decoder*/
      lCallResult = USCDecoderInit(&codec.uscParams, &clParams,f_log);
   }
   if(lCallResult<0) return USC_CALL_FAIL;

   /*Initialize Wave File params*/
   InitWavFile(&wfOutputParams);
   if (!encode) {
      /*Output file is pcm*/
      wfOutputParams.waveFmt.nFormatTag = LINEAR_PCM;
      wfOutputParams.waveFmt.nBitPerSample = (Ipp16s)codec.uscParams.pInfo->params.pcmType.bitPerSample;
      if(clParams.nOutputPCMType==ALAW_PCM) {
         wfOutputParams.waveFmt.nFormatTag = ALAW_PCM;
         wfOutputParams.waveFmt.nBitPerSample = 8;
      } else if(clParams.nOutputPCMType==MULAW_PCM) {
         wfOutputParams.waveFmt.nFormatTag = MULAW_PCM;
         wfOutputParams.waveFmt.nBitPerSample = 8;
      }
   } else {
      wfOutputParams.waveFmt.nFormatTag = (Ipp16s)codec.lFormatTag;
      wfOutputParams.waveFmt.nBitPerSample = 0;
   }
   /*Open output wave file*/
   lCallResult=OpenWavFile(&wfOutputParams, clParams.outputFileName, FILE_WRITE);
   if(lCallResult==-1) {
      USC_OutputString(f_log, "FAIL:Cannot open %s file for writing\n",clParams.outputFileName);
      if(f_log) fclose(f_log);
      return FOPEN_FAIL;
   }
   if(wfInputParams.DataSize == 0){
      USC_OutputString(f_log, "FAIL:Input file of zero length.\n",clParams.outputFileName);
      if(f_log) fclose(f_log);
      return FOPEN_FAIL;
   }

   /*Allocate memory for the input buffer. Size of input buffer is equal to the size of input file*/
   inputBuffer = (Ipp8s *)ippsMalloc_8u(wfInputParams.DataSize);
   if(!inputBuffer) {
      USC_OutputString(f_log, "FAIL:Cannot allocate %d bytes memory\n",wfInputParams.DataSize);
      if(f_log) fclose(f_log);
      return MEMORY_FAIL;
   }

   /*Allocate memory for the output buffer. Size of output buffer is equal to the size of 1 frame*/
   outputBuffer = (Ipp8s *)ippsMalloc_8u(codec.uscParams.pInfo->params.framesize);
   if(!outputBuffer) {
      USC_OutputString(f_log, "FAIL:Cannot allocate %d bytes memory\n",codec.uscParams.pInfo->params.framesize);
      if(f_log) fclose(f_log);
      return MEMORY_FAIL;
   }

   /*Read data from the input file to the buffer*/
   WavFileRead(&wfInputParams, (void *)inputBuffer, wfInputParams.DataSize);
   if((encode)&&(PCMType > LINEAR_PCM)) {
      lCallResult = USC_CvtToLinear(codec.uscParams.pInfo->params.framesize, PCMType, &inputBuffer, (Ipp32s*)&wfInputParams.DataSize);
      if(lCallResult<0) return MEMORY_FAIL;
   }

   /*Print codec current settings*/
   USC_OutputString(f_log, "  Bitrate %d kbps\n",codec.uscParams.pInfo->params.modes.bitrate);
   USC_OutputString(f_log, "  Band %d Hz\n",codec.uscParams.pInfo->params.pcmType.sample_frequency);
   USC_OutputString(f_log, "  VAD %d\n",codec.uscParams.pInfo->params.modes.vad);
   /*Set pointer to the start of the data*/

   ProcessOneChannel(&codec, &wfOutputParams, inputBuffer, wfInputParams.DataSize,
                        outputBuffer, &duration, &currLen,f_log);

   spSeconds = (duration/(Ipp32f)(codec.uscParams.pInfo->params.pcmType.bitPerSample>>3))/
                        (Ipp32f)codec.uscParams.pInfo->params.pcmType.sample_frequency;

   if(f_log) {
      fprintf(f_log,"Processed %g sec of speech\n",spSeconds);
   } else {
      printf("Processed %g sec of speech\n",spSeconds);
   }

   /* Fill output file params*/
   wfOutputParams.waveFmt.nChannels = (Ipp16s)codec.uscParams.pInfo->params.pcmType.nChannels;
   wfOutputParams.waveFmt.nSamplesPerSec = codec.uscParams.pInfo->params.pcmType.sample_frequency;

   if (!encode) {
      /*Output file is pcm*/
      wfOutputParams.waveFmt.nBlockAlign = wfOutputParams.waveFmt.nChannels*(wfOutputParams.waveFmt.nBitPerSample>>3);
   } else {
      Ipp32s nBlockAlign;
      wfOutputParams.bitrate = codec.uscParams.pInfo->params.modes.bitrate;
      /* Calc nBlockAlign of the current codec*/
      USCCodecGetSize(&codec.uscParams, codec.uscParams.pInfo->params.framesize, &nBlockAlign,f_log);
      wfOutputParams.waveFmt.nBlockAlign = (Ipp16s)((nBlockAlign+USCGetFrameHeaderSize())*wfOutputParams.waveFmt.nChannels);
   }

   /* Close input file*/
   WavFileClose(&wfInputParams);
   /* Write file header*/
   WavFileWriteHeader(&wfOutputParams);
   /* Close output file*/
   WavFileClose(&wfOutputParams);
   /*Free resources*/
   if(inputBuffer) ippsFree(inputBuffer);
   if(outputBuffer) ippsFree(outputBuffer);
   /*Free codec memory*/
   USCFree(&codec.uscParams);
   /* Close plug-ins*/
   FreeUSCSharedObjects(&codec);
   if(f_log) fclose(f_log);
   return 0;
}
