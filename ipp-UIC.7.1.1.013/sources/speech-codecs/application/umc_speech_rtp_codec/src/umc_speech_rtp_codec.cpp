//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
//

#include "ippcore.h"
#include "vm_time.h"

//#define VM_DEBUG 7

#include "vm_debug.h"

#include "umc_speech_codec.h"
#include "umc_linked_list.h"
#include "cmd_params.h"

#include "wav_file.h"
#include "rtpdump_file.h"

#include "umc_dual_thread_codec.h"

#include "umc_sample_buffer.h"
#include "usc_codec_container.h"

#include "umc_rtp_packet.h"

#include "umc_speech_jbuffer.h"
#include "umc_rta_jbuffer.h"

#include "repacketizer.h"
#include "timedplayer.h"

using namespace UMC;

void PrintCodecParams(UMC::SpeechCodecParams* info_codec_params, Ipp32s direction);

int main( int argc, char **argv )
{
   AudioFile* p_in_file;
   AudioFile* p_out_file;

   UMC::WavFile wav_file_in;
   UMC::WavFile wav_file_out;

   RTPDumpFile rtpdump_file_in;
   RTPDumpFile rtpdump_file_out;

   UMC::WavFile::Info info_file_in;
   UMC::WavFile::Info info_file_out;

   UMC::Status errIO;

   Ipp64f t_encode_time = 0.;
   Ipp32s numFramesToProcess = 0;

   UMC::MediaData inMediaBuffer;

   UMC::Status sts;

   UMC::AudioCodec* pAudioCodec;
   UMC::MediaBuffer* pMediaBuffer;
   DualThreadedCodec dual_treaded_codec;

   UMC::SpeechCodecParams* audio_codec_params;
   UMC::MediaBufferParams* media_buffer_params;
   UMC::DualThreadCodecParams codec_init;

   sProgramParameters params;

   USC_CodecContainer uscObjContainer;
   UMC::RTPBasePacketizer* pPck = NULL;
   UMC::RTPBaseDepacketizer* pDePck = NULL;

   Ipp32s encode = 0;

   /// Initialization
   pAudioCodec         = NULL;
   pMediaBuffer        = NULL;
   audio_codec_params  = NULL;
   media_buffer_params = NULL;

   ippStaticInit();

   if( argc < 2 ) {
      params_print_usage(argv[0]);
      return -1;
   }
   params_fill_by_default(&params);
   params_parse_command_line(&params,argc,argv);

   if (params.enumerate) {
      UMC::SpeechCodecParams          info_codec_params;

      Ipp32s i, nObjs = uscObjContainer.GetNumCodecs();

      vm_string_printf(VM_STRING("The following codecs are available:\n"));
      for(i=0;i < nObjs;i++) {
         vm_string_printf(VM_STRING("      %s\n"),uscObjContainer.GetCodecName(i));
      }
      return 0;
   }

   audio_codec_params = new UMC::SpeechCodecParams;

   /*Try to open input file as WAVE file*/
   p_in_file = &wav_file_in;
   p_out_file = &rtpdump_file_out;

   errIO = p_in_file->Open(params.input_file,AudioFile::AFM_NO_CONTENT_WRN);

   if(errIO == UMC_ERR_OPEN_FAILED) {
      vm_string_printf(VM_STRING("Can't open input file '%s'"), params.input_file);
      delete audio_codec_params;
      return -2;
   }

   if(errIO == UMC_WRN_INVALID_STREAM) {
      /*Input file isn't WAVE file. Try to open input file as file in rtpdump format*/
      p_in_file = &rtpdump_file_in;
      p_out_file = &wav_file_out;

      errIO = p_in_file->Open(params.input_file,AudioFile::AFM_NO_CONTENT_WRN);
   } else {
      encode = 1;
   }

   if(errIO == UMC_WRN_INVALID_STREAM) {
      vm_string_printf(VM_STRING("Unknown input file '%s' format"), params.input_file);
      delete audio_codec_params;
      return -2;
   }

   /*Find USC codec*/
   if (NULL == uscObjContainer.TryToFindStatic(params.codec_name)) {
      vm_string_printf(VM_STRING("The codec '%s'is not found.\n"), params.codec_name);
      delete audio_codec_params;
      return -4;
   }

   /*Load USC codec*/
   uscObjContainer.SetCodecName(params.codec_name);

   pAudioCodec = uscObjContainer.CreateUSCCodec(encode);
   if (pAudioCodec == NULL) {
      vm_string_printf(VM_STRING("Can't create audio codec object '%s'\n"), params.codec_name);
      delete audio_codec_params;
      return -5;
   }

   /*Create RTP repacketizers*/
   if(uscObjContainer.CreateRTPRePacketizer(encode,params.bitrate,params.btstrmFmt,&pPck,&pDePck) != UMC_OK){
       if(encode==1 && pPck==NULL || encode==0 && pDePck==NULL) {
          vm_string_printf(VM_STRING("Can't create %spacketizer for audio codec object '%s'\n"), encode? "": "de-", params.codec_name);
          delete audio_codec_params;
          delete pAudioCodec;
          return -5;
       }
       if(0 == params.bitrate)
           vm_string_printf("Default ");
       else
           vm_string_printf(VM_STRING(" %d "),params.bitrate);

       vm_string_printf(VM_STRING("bitstream is not supported for audio codec object '%s'\n"), params.codec_name);
       delete audio_codec_params;
       delete pAudioCodec;
       delete pDePck;
       delete pPck;
       return -5;
   }

   Repacker repacker(pPck,pDePck);

   /*Set I/O parameters of the USC codec*/
   pAudioCodec->GetInfo(audio_codec_params);

   if(encode == 1) {
      errIO = wav_file_in.GetInfo(&info_file_in);
      if (errIO == UMC_OK) {
         if(info_file_in.channels_number != 1) {
            vm_string_printf(VM_STRING("Input file must be mono.\n"));
            delete pAudioCodec;
            return -5;
         }
         if(info_file_in.format_tag != 1) {
            vm_string_printf(VM_STRING("Unsupported input file type.\n"));
            delete pAudioCodec;
            return -5;
         }
         audio_codec_params->m_info_in.stream_type = PCM_AUDIO;

         audio_codec_params->m_info_in.channel_mask = 0;
         audio_codec_params->m_info_out.channel_mask = 0;
         audio_codec_params->m_info_in.sample_frequency = info_file_in.sample_rate;
         audio_codec_params->m_info_in.bitPerSample = info_file_in.resolution;
         audio_codec_params->m_info_out.sample_frequency = info_file_in.sample_rate;
         audio_codec_params->m_info_out.bitPerSample = info_file_in.resolution;
         audio_codec_params->m_info_in.channels = info_file_in.channels_number;
         audio_codec_params->m_info_out.channels = info_file_in.channels_number;

         audio_codec_params->m_info_out.bitrate = params.bitrate;
      }
   } else {
      audio_codec_params->m_info_in.stream_type = UNDEF_AUDIO;

      audio_codec_params->m_info_in.channel_mask = 0;
      audio_codec_params->m_info_out.channel_mask = 0;

      audio_codec_params->m_info_in.bitrate = params.bitrate;
      if(params.DecSampleFreq > 0) {
         audio_codec_params->m_info_out.sample_frequency = params.DecSampleFreq;
      }
   }

   errIO = p_out_file->Open(params.output_file,AudioFile::AFM_CREATE);
   if (errIO != UMC_OK) {
      vm_string_printf(VM_STRING("Can't create output file '%s'"), params.output_file);
      delete pAudioCodec;
      return -3;
   }


   /*Create and init input buffer*/
   pMediaBuffer        = DynamicCast<MediaBuffer>(new SampleBuffer);

   media_buffer_params =new MediaBufferParams;

   media_buffer_params->m_numberOfFrames = 1;
   media_buffer_params->m_prefInputBufferSize = 2304000;

   pMediaBuffer->Init(media_buffer_params);

   /*Init codec*/
   codec_init.m_pCodec       = pAudioCodec;

   codec_init.m_pCodecInitParams   = audio_codec_params;

   audio_codec_params->m_Vad = params.isVad;
   audio_codec_params->m_law = 0;

   sts = dual_treaded_codec.Init(pAudioCodec,NULL,NULL);
   sts = dual_treaded_codec.Init(audio_codec_params);

   pAudioCodec->GetInfo(audio_codec_params);

   if(sts != UMC_OK) {
      if(!encode) {
         vm_string_printf(VM_STRING("Codec '%s' couldn't work with %d KHz sample frequence for %d bitrate\n"),
                  audio_codec_params->m_nameCodec,audio_codec_params->m_info_out.sample_frequency,params.bitrate);
         PrintCodecParams(audio_codec_params, encode);
      }
   }

   /*Check file params mapping to the codec*/
   {
      UMC::SpeechCodecParams          info_codec_params;
      pAudioCodec->GetInfo(&info_codec_params);
      if(encode) {
         if(info_codec_params.m_info_in.sample_frequency!=audio_codec_params->m_info_in.sample_frequency) {
            vm_string_printf(VM_STRING("Codec '%s' couldn't work with %d KHz sample frequence\n"),
               info_codec_params.m_nameCodec,audio_codec_params->m_info_in.sample_frequency);
            PrintCodecParams(&info_codec_params, encode);
            return -5;
         }
         if(info_codec_params.m_info_in.bitPerSample!=audio_codec_params->m_info_in.bitPerSample) {
            vm_string_printf(VM_STRING("Codec '%s' couldn't work with %d bits per sample\n"),
               info_codec_params.m_nameCodec,audio_codec_params->m_info_in.bitPerSample);
            PrintCodecParams(&info_codec_params, encode);
            return -5;
         }
      }
   }

   /*Start of the processing*/
   TimedPlayerExternalParams playerPrms;
   TimedPlayer player;

   playerPrms.m_nFrmInPkcts = params.nFrmInPkcts;
   playerPrms.m_p_in_file = p_in_file;
   playerPrms.m_p_out_file = p_out_file;
   playerPrms.m_pAudioCodecParams = audio_codec_params;
   playerPrms.m_pDSCodec = &dual_treaded_codec;
   playerPrms.m_pMediaBuffer = pMediaBuffer;
   playerPrms.m_pPT = uscObjContainer.GetPayloadType();
   playerPrms.m_pRepacker = &repacker;
   playerPrms.m_pJitterBuffer = uscObjContainer.CreateJBuffer(encode);
   playerPrms.m_pTimer = uscObjContainer.CreatePlayoutTimer(encode);

   player.Init(&playerPrms);

   if (sts == UMC_OK) {
      if(encode==1) {
         player.Record(t_encode_time, numFramesToProcess);
      } else {
         player.Play(t_encode_time, numFramesToProcess);
      }
   }

   player.Close();
   /*End of the processing*/
   Ipp32f dur;
   pAudioCodec->GetInfo(audio_codec_params);

   pAudioCodec->GetDuration(&dur);
   vm_string_printf(VM_STRING("Encoding/decoding time, sec : %f [%06d frames]\n"),t_encode_time,numFramesToProcess);
   vm_string_printf(VM_STRING("Stream duration, sec        : %f\n"),dur);
   t_encode_time /= dur;

   vm_string_printf(VM_STRING("Performance,(sec/sec)       : %f\n"),t_encode_time);
   vm_string_printf(VM_STRING("* Multiply the performance by the CPU frequency to convert into MHz\n"));
   /*if(!encode) {
      JBufferStat dejitterStat;
      dejitter.GetStatistics(&dejitterStat);
      vm_string_printf(VM_STRING(" Jitter buffer statistic:\n"));
      vm_string_printf(VM_STRING(" Recieced packets,       : %d\n"),dejitterStat.nReceivedPacket);
      vm_string_printf(VM_STRING("     Lost packets,       : %d\n"),dejitterStat.nLostPacket);
      vm_string_printf(VM_STRING("Discarded packets,       : %d\n"),dejitterStat.nDiscardedPacket);
   }*/

   info_file_out.channels_number = audio_codec_params->m_info_out.channels;
   info_file_out.sample_rate = audio_codec_params->m_info_out.sample_frequency;
   info_file_out.resolution = audio_codec_params->m_info_out.bitPerSample;
   info_file_out.channel_mask = audio_codec_params->m_info_out.channel_mask;

   if (audio_codec_params->m_info_out.stream_type == PCM_AUDIO) {
      info_file_out.format_tag = 1;
      wav_file_out.SetInfo(&info_file_out);
   }

   p_out_file->Close();
   p_in_file->Close();

   if (pMediaBuffer)
      delete pMediaBuffer;

   if (audio_codec_params)
      delete audio_codec_params;

   if (media_buffer_params)
      delete media_buffer_params;

   if(pPck)
      delete pPck;

   if(pDePck)
      delete pDePck;

   return 0;
}

void PrintCodecParams(UMC::SpeechCodecParams* info_codec_params, Ipp32s direction)
{
   Ipp32s i;
   vm_string_printf(VM_STRING("Codec name:                    %s\n"),info_codec_params->m_nameCodec);
   if(direction) {
      vm_string_printf(VM_STRING("Sample frequence:              %d KHz\n"),info_codec_params->m_info_in.sample_frequency);
      vm_string_printf(VM_STRING("Bits per sample:               %d\n"),info_codec_params->m_info_in.bitPerSample);
   } else {
      vm_string_printf(VM_STRING("Sample frequence:              %d KHz\n"),info_codec_params->m_info_out.sample_frequency);
      vm_string_printf(VM_STRING("Bits per sample:               %d\n"),info_codec_params->m_info_out.bitPerSample);
   }
   vm_string_printf(VM_STRING("Number of supported rates:     %d\n"),info_codec_params->m_nRates);
   vm_string_printf(VM_STRING("Supported rates:\n"));
   for(i=0;i<info_codec_params->m_nRates;i++) {
      vm_string_printf(VM_STRING("     %d bps\n"),info_codec_params->m_tblRates[i]);
   }
   if(info_codec_params->m_nVAD)
      vm_string_printf(VM_STRING("Number of supported VAD modes: %d\n"),info_codec_params->m_nVAD);

   return;
}
