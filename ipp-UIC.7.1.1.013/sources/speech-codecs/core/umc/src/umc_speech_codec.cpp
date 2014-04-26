/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/
#include "ipps.h"
#include "umc_structures.h"
#include "umc_audio_codec.h"
#include "umc_speech_codec.h"
namespace UMC
{

USCDecoder::USCDecoder(USC_Fxns *FNS)
{
   m_codec.pUSC_CODEC_Fxns = FNS;

   m_codec.pUSC_CODEC_Fxns->std.GetInfo((USC_Handle)NULL, &m_codec.pInfo);

   m_nVad = m_codec.pInfo.params.modes.vad;

   m_codec.decoder = NULL;
   m_codec.pBanks = NULL;
   m_codec.nBanks = 0;
   m_frame_num = 0;

}

bool USCDecoder::CheckBitRate(Ipp32s rateInBps)
{
   for(Ipp32s i = 0; i < m_codec.pInfo.nRates; i++) {
      if(m_codec.pInfo.pRateTbl[i].bitrate == rateInBps) {
         return true;
      }
   }
   return false;
}

Status
USCDecoder::Init(BaseCodecParams *init)
{
   Ipp32s i;
   USC_Status USCStatus;
   SpeechCodecParams* pSpeechCodecInit = DynamicCast<SpeechCodecParams, BaseCodecParams>(init);
   if(!pSpeechCodecInit) {
      return UMC_ERR_NULL_PTR;
   }

   Close();

   m_codec.pInfo.params.direction = USC_DECODE;
   m_codec.pInfo.params.law = pSpeechCodecInit->m_law;
   m_codec.pInfo.params.framesize = pSpeechCodecInit->m_framesize;
   m_codec.pInfo.params.modes.vad = pSpeechCodecInit->m_Vad;
   m_codec.pInfo.params.modes.truncate = pSpeechCodecInit->m_Truncate;
   m_codec.pInfo.params.modes.pf = pSpeechCodecInit->m_Pf;
   m_codec.pInfo.params.modes.hpf = pSpeechCodecInit->m_Hpf;
   m_codec.pInfo.params.modes.outMode = pSpeechCodecInit->m_outMode;
   m_codec.pInfo.params.pcmType.sample_frequency = pSpeechCodecInit->m_info_out.sample_frequency;
   m_codec.pInfo.params.pcmType.nChannels = pSpeechCodecInit->m_info_out.channels;
   if(CheckBitRate(pSpeechCodecInit->m_info_in.bitrate)) {
      m_codec.pInfo.params.modes.bitrate = pSpeechCodecInit->m_info_in.bitrate;
   }

   USCStatus = m_codec.pUSC_CODEC_Fxns->std.NumAlloc((const USC_Option *)&m_codec.pInfo.params, &m_codec.nBanks);
   if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);

   m_codec.pBanks            = (USC_MemBank*)ippsMalloc_8s(m_codec.nBanks*sizeof(USC_MemBank));
   if (!m_codec.pBanks)
      return (UMC_ERR_INIT);

   USCStatus = m_codec.pUSC_CODEC_Fxns->std.MemAlloc((const USC_Option *)&m_codec.pInfo.params, m_codec.pBanks);
   if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);
   for(i=0; i<m_codec.nBanks;i++){
      m_codec.pBanks[i].pMem            = (char *)ippsMalloc_8s(m_codec.pBanks[i].nbytes);
   }
   for(i=0; i<m_codec.nBanks;i++){
      if (!m_codec.pBanks[i].pMem)
         return (UMC_ERR_INIT);
   }

   USCStatus = m_codec.pUSC_CODEC_Fxns->std.Init((const USC_Option *)&m_codec.pInfo.params, m_codec.pBanks, &m_codec.decoder);
   if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);

   m_codec.pUSC_CODEC_Fxns->std.GetInfo(m_codec.decoder, &m_codec.pInfo);

   m_frame_num = 0;
   return UMC_OK;

}

Status
USCDecoder::SetParams(BaseCodecParams* params)
{
   USC_Status USCStatus;
   SpeechCodecParams* pSpeechCodecInit;

   pSpeechCodecInit = DynamicCast<SpeechCodecParams, BaseCodecParams>(params);
   if(pSpeechCodecInit==NULL) return UMC_ERR_NULL_PTR;

   if(CheckBitRate(pSpeechCodecInit->m_info_in.bitrate)) {
      m_codec.pInfo.params.modes.bitrate = pSpeechCodecInit->m_info_in.bitrate;
   }
   m_codec.pInfo.params.modes.truncate = pSpeechCodecInit->m_Truncate;
   m_codec.pInfo.params.modes.pf       = pSpeechCodecInit->m_Pf;
   m_codec.pInfo.params.modes.hpf      = pSpeechCodecInit->m_Hpf;
   m_codec.pInfo.params.modes.vad      = pSpeechCodecInit->m_Vad;
   m_codec.pInfo.params.modes.outMode  = pSpeechCodecInit->m_outMode;

   USCStatus = m_codec.pUSC_CODEC_Fxns->std.Control(&((USC_Option*)&m_codec.pInfo.params)->modes, m_codec.decoder);
   if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);
   return UMC_OK;
}

Status
USCDecoder::GetInfo(BaseCodecParams *info)
{
    USC_Status USCStatus;
    if(info==NULL) return UMC_ERR_NULL_PTR;

    SpeechCodecParams* pSpeechCodecInfo = DynamicCast<SpeechCodecParams, BaseCodecParams>(info);

    USCStatus = m_codec.pUSC_CODEC_Fxns->std.GetInfo(m_codec.decoder, &(m_codec.pInfo));
    if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);

    if (!pSpeechCodecInfo) {
       Ipp32s outSize = 0, outSizeMin, tmp, BitSize, minBitSize;
       // Save VAD
       tmp = m_codec.pInfo.params.modes.vad;
       // Enable VAD to get right information
       m_codec.pInfo.params.modes.vad = 1;
       // Get BlockAlign
       minBitSize = m_codec.pInfo.maxbitsize;
       outSizeMin = m_codec.pInfo.params.framesize;
       USCStatus = m_codec.pUSC_CODEC_Fxns->GetOutStreamSize(&m_codec.pInfo.params,
                                                  m_codec.pInfo.params.modes.bitrate,
                                                  1,
                                                  &outSize);
       if(outSize==m_codec.pInfo.params.framesize) {
          info->m_SuggestedInputSize = 1;
          m_codec.pInfo.params.modes.vad = tmp;
          return UMC_OK;
       }
       if(USCStatus == USC_UnsupportedVADType)
          m_codec.pInfo.params.modes.vad = 0;
       for(BitSize=2;BitSize<m_codec.pInfo.maxbitsize;BitSize++) {
         outSize = 0;
         USCStatus = m_codec.pUSC_CODEC_Fxns->GetOutStreamSize(&m_codec.pInfo.params,
                                                  m_codec.pInfo.params.modes.bitrate,
                                                  BitSize,
                                                  &outSize);
         if((USCStatus!=USC_NoError)&&((USCStatus!=USC_NoOperation))) {
            m_codec.pInfo.params.modes.vad = tmp;
            return UMC_ERR_FAILED;
         }
         if(USCStatus!=USC_NoOperation) {
            if(outSize>=outSizeMin) {
               outSize=outSizeMin;
               minBitSize = BitSize;
               break;
            }
         }
       }
       info->m_SuggestedInputSize = BitSize;
       m_codec.pInfo.params.modes.vad = tmp;
       return UMC_OK;
    }

    pSpeechCodecInfo->m_SuggestedInputSize     = m_codec.pInfo.maxbitsize;

    pSpeechCodecInfo->m_framesize  = m_codec.pInfo.params.framesize;
    pSpeechCodecInfo->m_maxbitsize = m_codec.pInfo.maxbitsize;
    pSpeechCodecInfo->m_nVAD = m_nVad;
    pSpeechCodecInfo->m_nRates = m_codec.pInfo.nRates;
    pSpeechCodecInfo->m_tblRates = (const Ipp32s *)m_codec.pInfo.pRateTbl;

    pSpeechCodecInfo->m_info_in.bitPerSample     = m_codec.pInfo.params.pcmType.bitPerSample;
    pSpeechCodecInfo->m_info_in.bitrate = m_codec.pInfo.params.modes.bitrate;
    pSpeechCodecInfo->m_info_in.channels = m_codec.pInfo.params.pcmType.nChannels;
    pSpeechCodecInfo->m_info_in.sample_frequency = m_codec.pInfo.params.pcmType.sample_frequency;

    pSpeechCodecInfo->m_info_out.bitPerSample    = m_codec.pInfo.params.pcmType.bitPerSample;
    pSpeechCodecInfo->m_info_out.channels = m_codec.pInfo.params.pcmType.nChannels;
    pSpeechCodecInfo->m_info_out.sample_frequency= m_codec.pInfo.params.pcmType.sample_frequency;
    pSpeechCodecInfo->m_info_out.stream_type     = PCM_AUDIO;

    pSpeechCodecInfo->m_law                      = m_codec.pInfo.params.law;
    pSpeechCodecInfo->m_Vad                      = m_codec.pInfo.params.modes.vad;
    pSpeechCodecInfo->m_Truncate                 = m_codec.pInfo.params.modes.truncate;
    pSpeechCodecInfo->m_Pf                       = m_codec.pInfo.params.modes.pf;
    pSpeechCodecInfo->m_Hpf                      = m_codec.pInfo.params.modes.hpf;
    pSpeechCodecInfo->m_outMode                  = m_codec.pInfo.params.modes.outMode;
    pSpeechCodecInfo->m_nameCodec                = (vm_char*)m_codec.pInfo.name;
    pSpeechCodecInfo->m_frame_num = m_frame_num;
    pSpeechCodecInfo->m_BlockAlign = pSpeechCodecInfo->m_info_out.channels * (pSpeechCodecInfo->m_info_out.bitPerSample>>3);

    return UMC_OK;
}

Status
USCDecoder::GetFrame(MediaData *in, MediaData *out)
{
   Ipp32s  outSamples;
   size_t  sizeBitstream;
   Ipp32s  frame_size;
   bool lostFrame = false;
   USC_Status USCStatus;
   SpeechData *spData;

   if(in==NULL) return UMC_ERR_NULL_PTR;
   if(out==NULL) return UMC_ERR_NULL_PTR;

   spData = DynamicCast<SpeechData, MediaData>(in);

   sizeBitstream = in->GetDataSize();

   if(spData==NULL) {
      lostFrame = true;
   } else {
      if((sizeBitstream <= 0) && (in->GetDataPointer() == NULL)) {
         lostFrame = true;
      }
   }

   if(lostFrame) {
      frame_size = 0;
      m_codec.output.pBuffer = (char *)out->GetDataPointer();

      USCStatus = m_codec.pUSC_CODEC_Fxns->Decode (m_codec.decoder, NULL, &m_codec.output);
      if(USCStatus!=USC_NoError) return (UMC_ERR_INVALID_STREAM);
   } else  {
      m_codec.input.frametype = spData->GetFrameType();
      m_codec.input.bitrate = spData->GetBitrate();
      m_codec.input.pBuffer = (char *)spData->GetDataPointer();
      m_codec.input.nbytes = spData->GetNBytes();

      m_codec.output.pBuffer = (char *)out->GetDataPointer();

      USCStatus = m_codec.pUSC_CODEC_Fxns->Decode (m_codec.decoder, &m_codec.input, &m_codec.output);
      if(USCStatus!=USC_NoError) return (UMC_ERR_INVALID_STREAM);
      frame_size = m_codec.input.nbytes;
   }
   outSamples = m_codec.output.nbytes/(m_codec.pInfo.params.pcmType.bitPerSample>>3);
   in->SetDataSize(sizeBitstream-frame_size);
   in->SetTime(in->GetTime()+((Ipp32f)outSamples/(Ipp32f)m_codec.pInfo.params.pcmType.sample_frequency));
   out->SetDataSize(m_codec.output.nbytes);
   out->SetTime(in->GetTime(), in->GetTime()+((Ipp32f)outSamples/(Ipp32f)m_codec.pInfo.params.pcmType.sample_frequency));
   m_frame_num++;

   return UMC_OK;
}

Status
USCDecoder::Reset(void)
{
   USC_Status USCStatus;
   USCStatus = m_codec.pUSC_CODEC_Fxns->std.Reinit(&((USC_Option*)&m_codec.pInfo.params)->modes, m_codec.decoder);
   if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);

   return UMC_OK;
}

Status
USCDecoder::Close(void)
{
   Ipp32s i;
   if(m_codec.nBanks) {
      if(m_codec.pBanks) {
         for(i=0;i<m_codec.nBanks;i++) {
            if(m_codec.pBanks[i].pMem)  {
               ippsFree(m_codec.pBanks[i].pMem);
               m_codec.pBanks[i].pMem = NULL;
               m_codec.pBanks[i].nbytes = 0;
            }
         }
         ippsFree(m_codec.pBanks);
         m_codec.pBanks = NULL;
         m_codec.nBanks = 0;
      }
   }
   m_frame_num=0;

   return UMC_OK;
}

USCDecoder::~USCDecoder(void)
{
   Close();
}

Status
USCDecoder::GetDuration(Ipp32f* p_duration)
{
    Ipp32f duration;

    duration = (Ipp32f)m_frame_num * (m_codec.pInfo.params.framesize/(m_codec.pInfo.params.pcmType.bitPerSample>>3));
    duration /= m_codec.pInfo.params.pcmType.sample_frequency;

    p_duration[0] = duration;

    return UMC_OK;
}

USCEncoder::USCEncoder(USC_Fxns *FNS)
{
   m_codec.pUSC_CODEC_Fxns = FNS;

   m_codec.pUSC_CODEC_Fxns->std.GetInfo((USC_Handle)NULL, &m_codec.pInfo);

   m_nVad = m_codec.pInfo.params.modes.vad;

   m_codec.encoder = NULL;
   m_codec.pBanks = NULL;
   m_codec.nBanks = 0;
   m_frame_num = 0;
}

bool USCEncoder::CheckBitRate(Ipp32s rateInBps)
{
   for(Ipp32s i = 0; i < m_codec.pInfo.nRates; i++) {
      if(m_codec.pInfo.pRateTbl[i].bitrate == rateInBps) {
         return true;
      }
   }
   return false;
}

Status
USCEncoder::Init(BaseCodecParams *init)
{
   Ipp32s i;
   USC_Status USCStatus;

   SpeechCodecParams* pSpeechCodecInit = DynamicCast<SpeechCodecParams, BaseCodecParams>(init);
   if(!pSpeechCodecInit) return UMC_ERR_NULL_PTR;

   Close();

   m_codec.pInfo.params.direction = USC_ENCODE;
   m_codec.pInfo.params.law = pSpeechCodecInit->m_law;
   m_codec.pInfo.params.framesize = pSpeechCodecInit->m_framesize;
   m_codec.pInfo.params.modes.vad = pSpeechCodecInit->m_Vad;
   m_codec.pInfo.params.modes.truncate = pSpeechCodecInit->m_Truncate;
   m_codec.pInfo.params.modes.pf = pSpeechCodecInit->m_Pf;
   m_codec.pInfo.params.modes.hpf = pSpeechCodecInit->m_Hpf;
   m_codec.pInfo.params.modes.outMode  = pSpeechCodecInit->m_outMode;
   m_codec.pInfo.params.pcmType.sample_frequency = pSpeechCodecInit->m_info_in.sample_frequency;
   m_codec.pInfo.params.pcmType.nChannels = pSpeechCodecInit->m_info_in.channels;
   if(CheckBitRate(pSpeechCodecInit->m_info_out.bitrate))
      m_codec.pInfo.params.modes.bitrate = pSpeechCodecInit->m_info_out.bitrate;

   USCStatus = m_codec.pUSC_CODEC_Fxns->std.NumAlloc((const USC_Option *)&m_codec.pInfo.params, &m_codec.nBanks);
   if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);

   m_codec.pBanks            = (USC_MemBank*)ippsMalloc_8s(m_codec.nBanks*sizeof(USC_MemBank));
   if (!m_codec.pBanks)
      return (UMC_ERR_INIT);

   USCStatus = m_codec.pUSC_CODEC_Fxns->std.MemAlloc((const USC_Option *)&m_codec.pInfo.params, m_codec.pBanks);
   if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);
   for(i=0; i<m_codec.nBanks;i++){
      m_codec.pBanks[i].pMem            = (char *)ippsMalloc_8s(m_codec.pBanks[i].nbytes);
   }
   for(i=0; i<m_codec.nBanks;i++){
      if (!m_codec.pBanks[i].pMem)
         return (UMC_ERR_INIT);
   }

   m_codec.input.bitrate = m_codec.pInfo.params.modes.bitrate;
   m_codec.input.pcmType.bitPerSample = m_codec.pInfo.params.pcmType.bitPerSample;
   m_codec.input.pcmType.sample_frequency = m_codec.pInfo.params.pcmType.sample_frequency;
   m_codec.input.pcmType.nChannels = m_codec.pInfo.params.pcmType.nChannels;

   USCStatus = m_codec.pUSC_CODEC_Fxns->std.Init((const USC_Option *)&m_codec.pInfo.params, m_codec.pBanks, &m_codec.encoder);
   if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);

   m_codec.pUSC_CODEC_Fxns->std.GetInfo(m_codec.encoder, &m_codec.pInfo);

   m_frame_num = 0;
   return UMC_OK;

}

Status
USCEncoder::SetParams(BaseCodecParams* params)
{
   USC_Status USCStatus;
   SpeechCodecParams* pSpeechCodecInit;
   int try2SetBandwidth,tmpBitrate;

   pSpeechCodecInit = DynamicCast<SpeechCodecParams, BaseCodecParams>(params);
   if(pSpeechCodecInit==NULL) return UMC_ERR_NULL_PTR;

   if(CheckBitRate(pSpeechCodecInit->m_info_out.bitrate)) {
      m_codec.pInfo.params.modes.bitrate = pSpeechCodecInit->m_info_out.bitrate;
      try2SetBandwidth = 0;
   } else {
      try2SetBandwidth = 1;
   }

   tmpBitrate = m_codec.pInfo.params.modes.bitrate;

   m_codec.pInfo.params.modes.truncate = pSpeechCodecInit->m_Truncate;
   m_codec.pInfo.params.modes.pf       = pSpeechCodecInit->m_Pf;
   m_codec.pInfo.params.modes.hpf      = pSpeechCodecInit->m_Hpf;
   m_codec.pInfo.params.modes.vad      = pSpeechCodecInit->m_Vad;
   m_codec.pInfo.params.modes.outMode  = pSpeechCodecInit->m_outMode;

   USCStatus = m_codec.pUSC_CODEC_Fxns->SetFrameSize(&(m_codec.pInfo.params),
      m_codec.encoder, pSpeechCodecInit->m_framesize);
   if((USCStatus!=USC_NoError)&&((USCStatus!=USC_NoOperation))) return (UMC_ERR_INIT);
   if(try2SetBandwidth) {
      m_codec.pInfo.params.modes.bitrate = pSpeechCodecInit->m_info_out.bitrate;
   }
   USCStatus = m_codec.pUSC_CODEC_Fxns->std.Control(&((USC_Option*)&m_codec.pInfo.params)->modes, m_codec.encoder);
   m_codec.pInfo.params.modes.bitrate = tmpBitrate;
   if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);

   return UMC_OK;
}

Status
USCEncoder::GetInfo(BaseCodecParams *info)
{
   USC_Status USCStatus;
   Ipp32s tmp;
    if(info==NULL) return UMC_ERR_NULL_PTR;

    USCStatus = m_codec.pUSC_CODEC_Fxns->std.GetInfo(m_codec.encoder, &(m_codec.pInfo));
    if(USCStatus!=USC_NoError) return (UMC_ERR_INIT); // ??status??

    SpeechCodecParams* pSpeechCodecInfo = DynamicCast<SpeechCodecParams, BaseCodecParams>(info);
    if (!pSpeechCodecInfo) {
       info->m_SuggestedInputSize = m_codec.pInfo.params.framesize;
       return UMC_OK;
    }

    pSpeechCodecInfo->m_SuggestedInputSize     = m_codec.pInfo.params.framesize;

    pSpeechCodecInfo->m_framesize  = m_codec.pInfo.params.framesize;
    pSpeechCodecInfo->m_maxbitsize = m_codec.pInfo.maxbitsize;
    pSpeechCodecInfo->m_nVAD = m_nVad;
    pSpeechCodecInfo->m_nRates = m_codec.pInfo.nRates;
    pSpeechCodecInfo->m_tblRates = (const Ipp32s *)m_codec.pInfo.pRateTbl;

    m_codec.input.bitrate = m_codec.pInfo.params.modes.bitrate;
    pSpeechCodecInfo->m_info_in.bitPerSample     = m_codec.pInfo.params.pcmType.bitPerSample;
    pSpeechCodecInfo->m_info_in.bitrate = m_codec.input.bitrate;
    pSpeechCodecInfo->m_info_in.channels = m_codec.pInfo.params.pcmType.nChannels;
    pSpeechCodecInfo->m_info_in.sample_frequency = m_codec.pInfo.params.pcmType.sample_frequency;

    pSpeechCodecInfo->m_info_out.bitPerSample    = m_codec.pInfo.params.pcmType.bitPerSample;
    pSpeechCodecInfo->m_info_out.channels = m_codec.pInfo.params.pcmType.nChannels;
    pSpeechCodecInfo->m_info_out.sample_frequency= m_codec.pInfo.params.pcmType.sample_frequency;
    pSpeechCodecInfo->m_info_out.bitrate = m_codec.input.bitrate;
    pSpeechCodecInfo->m_info_out.stream_type     = UNDEF_AUDIO;

    pSpeechCodecInfo->m_law                      = m_codec.pInfo.params.law;
    pSpeechCodecInfo->m_Vad                      = m_codec.pInfo.params.modes.vad;
    pSpeechCodecInfo->m_Truncate                 = m_codec.pInfo.params.modes.truncate;
    pSpeechCodecInfo->m_Pf                       = m_codec.pInfo.params.modes.pf;
    pSpeechCodecInfo->m_Hpf                      = m_codec.pInfo.params.modes.hpf;
    pSpeechCodecInfo->m_outMode                  = m_codec.pInfo.params.modes.outMode;
    pSpeechCodecInfo->m_nameCodec                = (vm_char*)m_codec.pInfo.name;
    pSpeechCodecInfo->m_frame_num = m_frame_num;

    //Save VAD mode
    tmp = m_codec.pInfo.params.modes.vad;
    // Disable VAD to get right information
    m_codec.pInfo.params.modes.vad = 0;
    // Get BlockAlign
    m_codec.pUSC_CODEC_Fxns->GetOutStreamSize(&m_codec.pInfo.params,
                                                  m_codec.pInfo.params.modes.bitrate,
                                                  m_codec.pInfo.params.framesize,
                                                  &pSpeechCodecInfo->m_BlockAlign);
    //Restore VAD mode
    m_codec.pInfo.params.modes.vad = tmp;

    return UMC_OK;
}

Status
USCEncoder::GetFrame(MediaData *in, MediaData *out)
{
   Ipp32s  inSamples, outSize;
   size_t  sizePCMstream;
   USC_Status USCStatus;
   SpeechData *spOutData;

   if(in==NULL) return UMC_ERR_NULL_PTR;

   spOutData = DynamicCast<SpeechData, MediaData>(out);
   if(spOutData==NULL) return UMC_ERR_NULL_PTR;

   sizePCMstream = in->GetDataSize();
   m_codec.input.pBuffer = (char *)in->GetDataPointer();
   m_codec.input.bitrate = m_codec.pInfo.params.modes.bitrate;
   m_codec.input.nbytes = (int)sizePCMstream;
   m_codec.input.pcmType.bitPerSample = m_codec.pInfo.params.pcmType.bitPerSample;
   m_codec.input.pcmType.sample_frequency = m_codec.pInfo.params.pcmType.sample_frequency;
   m_codec.input.pcmType.nChannels = m_codec.pInfo.params.pcmType.nChannels;

   m_codec.output.pBuffer = (char *)out->GetDataPointer();

   USCStatus = m_codec.pUSC_CODEC_Fxns->Encode (m_codec.encoder, &m_codec.input, &m_codec.output);
   if(USCStatus!=USC_NoError) return (UMC_ERR_INVALID_STREAM);
   spOutData->SetFrameType(m_codec.output.frametype);
   spOutData->SetBitrate(m_codec.output.bitrate);
   spOutData->SetNBytes(m_codec.output.nbytes);
   outSize = m_codec.output.nbytes;

   inSamples = m_codec.input.nbytes/(m_codec.pInfo.params.pcmType.bitPerSample>>3);
   in->SetDataSize(sizePCMstream-m_codec.input.nbytes);
   in->SetTime(in->GetTime()+((Ipp32f)inSamples/(Ipp32f)m_codec.pInfo.params.pcmType.sample_frequency));
   out->SetDataSize(outSize);
   out->SetTime(in->GetTime(), in->GetTime()+((Ipp32f)inSamples/(Ipp32f)m_codec.pInfo.params.pcmType.sample_frequency));
   m_frame_num++;

   return UMC_OK;
}

Status
USCEncoder::Reset(void)
{
   USC_Status USCStatus;
   USCStatus = m_codec.pUSC_CODEC_Fxns->std.Reinit(&((USC_Option*)&m_codec.pInfo.params)->modes, m_codec.encoder);
   if(USCStatus!=USC_NoError) return (UMC_ERR_INIT);
   return UMC_OK;
}

Status
USCEncoder::Close(void)
{
   Ipp32s i;
   if(m_codec.nBanks) {
      if(m_codec.pBanks) {
         for(i=0;i<m_codec.nBanks;i++) {
            if(m_codec.pBanks[i].pMem)  {
               ippsFree(m_codec.pBanks[i].pMem);
               m_codec.pBanks[i].pMem = NULL;
               m_codec.pBanks[i].nbytes = 0;
            }
         }
         ippsFree(m_codec.pBanks);
         m_codec.pBanks = NULL;
         m_codec.nBanks = 0;
      }
   }
   m_frame_num = 0;

   return UMC_OK;
}

USCEncoder::~USCEncoder(void)
{
   Close();
}

Status
USCEncoder::GetDuration(Ipp32f* p_duration)
{
    Ipp32f duration;

    duration = (Ipp32f)m_frame_num * (m_codec.pInfo.params.framesize/(m_codec.pInfo.params.pcmType.bitPerSample>>3));
    duration /= m_codec.pInfo.params.pcmType.sample_frequency;

    p_duration[0] = duration;

    return UMC_OK;
}

};// namespace UMC
