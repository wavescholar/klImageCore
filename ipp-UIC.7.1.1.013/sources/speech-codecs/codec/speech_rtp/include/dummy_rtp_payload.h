/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/
#ifndef __VOID_PAYLOAD_H__
#define __VOID_PAYLOAD_H__

#include "umc_linked_list.h"
#include "base_rtp_cnvrt.h"
#include "umc_speech_data.h"

namespace UMC
{

   class DummyRTPVoicePacketizer : public RTPBasePacketizer{
      DYNAMIC_CAST_DECL(DummyRTPVoicePacketizer, RTPBasePacketizer)
   public:
      DummyRTPVoicePacketizer();
      ~DummyRTPVoicePacketizer();

      Status Init(RTPBasePacketizerParams *pParams);

      Status GetPacket(MediaData *pPacket);
      Status AddFrame(MediaData *pFrame);
      Status SetControls(RTPBaseControlParams *pControls);

   protected:
      LinkedList<SpeechData*> m_List;
   };

   class DummyRTPVoiceDePacketizer  : public RTPBaseDepacketizer{
      DYNAMIC_CAST_DECL(DummyRTPVoiceDePacketizer, RTPBaseDepacketizer)
   public:
      DummyRTPVoiceDePacketizer();
      ~DummyRTPVoiceDePacketizer();

      virtual Status Init(RTPBaseDePacketizerParams *pParams);
      virtual Status GetNextFrame(MediaData *pFrame);
      virtual Status SetPacket(MediaData *pPacket) = 0;
      virtual Status GetControls(RTPBaseControlParams *pControls);

   protected:
      void Clean();
      void AddExtractedFrame(SpeechData *pFrame);
      LinkedList<SpeechData*> m_List;
      Ipp32s m_PrevBitrate;
   };
}


#endif /*__VOID_PAYLOAD_H__*/
