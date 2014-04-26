/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_SPEECH_MEDIADATA_H__
#define __UMC_SPEECH_MEDIADATA_H__

#include "umc_structures.h"
#include "umc_media_data.h"

namespace UMC
{

    class SpeechData : public MediaData
       {
          DYNAMIC_CAST_DECL(SpeechData,MediaData)
          public:

      SpeechData(Ipp32u length = 0);

      //  Move data to another MediaData
      virtual Status MoveDataTo(MediaData *dst);

      Status SetBitrate(Ipp32s bitrate);
      Ipp32s GetBitrate() { return m_bitrate;};
      Status SetFrameType(Ipp32s frametype);
      Ipp32s GetFrameType() { return m_frametype;};
      Status SetNBytes(Ipp32s nbytes);
      Ipp32s GetNBytes() { return m_nbytes;};
   protected:
      Ipp32s m_bitrate;
      Ipp32s m_frametype;
      Ipp32s m_nbytes;
};
}// namespace UMC

#endif /* __UMC_SPEECH_MEDIADATA_H__ */
