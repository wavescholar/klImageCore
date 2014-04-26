/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/
#include "umc_structures.h"
#include "umc_speech_data.h"

namespace UMC
{

   SpeechData::SpeechData(Ipp32u length) : MediaData(length)
   {
      m_bitrate = 0;
      m_frametype = 0;
      m_nbytes = 0;
   }

   Status SpeechData::SetBitrate(Ipp32s bitrate)
   {
      m_bitrate     = bitrate;
      return UMC_OK;
   }

   Status SpeechData::SetFrameType(Ipp32s frametype)
   {
      m_frametype     = frametype;
      return UMC_OK;
   }

   Status SpeechData::SetNBytes(Ipp32s nbytes)
   {
      m_nbytes     = nbytes;
      return UMC_OK;
   }

   Status SpeechData::MoveDataTo(MediaData* dst)
   {
      Status umcRes = UMC_OK;

      umcRes = MediaData::MoveDataTo(dst);

      if(UMC_OK == umcRes) {
         SpeechData *src = DynamicCast<SpeechData, MediaData>(this);
         SpeechData *dstSpeechMediaData = DynamicCast<SpeechData, MediaData>(dst);

         if(dstSpeechMediaData && src) {
            dstSpeechMediaData->SetFrameType(src->GetFrameType());
            dstSpeechMediaData->SetBitrate(src->GetBitrate());
            dstSpeechMediaData->SetNBytes(src->GetNBytes());
         }
      }

      return umcRes;
   }
}// namespace UMC
