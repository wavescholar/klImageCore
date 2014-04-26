/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/
#ifndef __UMC_SPEECH_CODEC_H__
#define __UMC_SPEECH_CODEC_H__

#include "umc_structures.h"
#include "umc_audio_codec.h"
#include "umc_speech_data.h"
#include "umc_usc.h"


namespace UMC
{

    //class SpeechCodecParams;

    class SpeechCodecParams : public AudioCodecParams
    {
        DYNAMIC_CAST_DECL(SpeechCodecParams,AudioCodecParams)
        public:

      vm_char        *m_nameCodec;
      Ipp32s          m_framesize;
      Ipp32s          m_maxbitsize;
      Ipp32s          m_nVAD;
      Ipp32s          m_nRates;
      const Ipp32s    *m_tblRates;
      Ipp32s          m_Vad;
      Ipp32s          m_Truncate;
      Ipp32s          m_Pf;
      Ipp32s          m_Hpf;
      Ipp32s          m_BlockAlign;
      Ipp32s          m_law;
      USC_OutputMode  m_outMode;
};

   //class USCDecoder;

    class USCDecoder : public AudioCodec \
    {
        DYNAMIC_CAST_DECL(USCDecoder, AudioCodec)
    public:
        Status Init(BaseCodecParams *init);
        Status GetFrame(MediaData *in, MediaData *out);
        Status GetInfo(BaseCodecParams *info);
        Status Close();
        Status Reset();
        Status SetParams(BaseCodecParams* params);
        Status GetDuration(Ipp32f* p_duration);

        USCDecoder(USC_Fxns *FNS);
        ~USCDecoder();

    protected:

        struct {
            USC_Fxns*        pUSC_CODEC_Fxns;
            USC_CodecInfo    pInfo;
            Ipp32s           nBanks;
            USC_MemBank      *pBanks;
            USC_Handle       decoder;
            USC_Bitstream    input;
            USC_PCMStream    output;
        } m_codec;

        Ipp32s m_nVad;
        bool CheckBitRate(Ipp32s rateInBps);
    };

   //class USCEncoder;

   class USCEncoder : public AudioCodec \
   {
      DYNAMIC_CAST_DECL(USCEncoder, AudioCodec)
      public:
         Status Init(BaseCodecParams *init);
         Status GetFrame(MediaData *in, MediaData *out);
         Status GetInfo(BaseCodecParams *info);
         Status Close();
         Status Reset();
         Status SetParams(BaseCodecParams* params);
         Status GetDuration(Ipp32f* p_duration);

         USCEncoder(USC_Fxns *FNS);
         ~USCEncoder();

      protected:

         struct {
            USC_Fxns*        pUSC_CODEC_Fxns;
            USC_CodecInfo    pInfo;
            Ipp32s           nBanks;
            USC_MemBank      *pBanks;
            USC_Handle       encoder;
            USC_PCMStream    input;
            USC_Bitstream    output;
         } m_codec;

         Ipp32s m_nVad;
         bool CheckBitRate(Ipp32s rateInBps);
   };

}// namespace UMC

#endif
