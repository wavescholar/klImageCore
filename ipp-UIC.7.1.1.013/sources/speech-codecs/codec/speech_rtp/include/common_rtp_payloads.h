/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/
#ifndef __COMMON_PAYLOAD_H__
#define __COMMON_PAYLOAD_H__

#include "dummy_rtp_payload.h"

namespace UMC
{

   /*G711 RTP payload*/
   class G711Packetizer : public DummyRTPVoicePacketizer{
      DYNAMIC_CAST_DECL(G711Packetizer, DummyRTPVoicePacketizer)
   public:
      G711Packetizer() : DummyRTPVoicePacketizer() {}
      ~G711Packetizer() {}

      Status AddFrame(MediaData *pFrame);
   };

   class G711DePacketizer  : public DummyRTPVoiceDePacketizer{
      DYNAMIC_CAST_DECL(G711DePacketizer, DummyRTPVoiceDePacketizer)
   public:
      G711DePacketizer() : DummyRTPVoiceDePacketizer() {}
      ~G711DePacketizer() {}

      Status SetPacket(MediaData *pPacket);
   };

   /*G723 RTP payload*/
   class G723Packetizer : public DummyRTPVoicePacketizer{
      DYNAMIC_CAST_DECL(G723Packetizer, DummyRTPVoicePacketizer)
   public:
      G723Packetizer() : DummyRTPVoicePacketizer() {}
      ~G723Packetizer() {}
   };

   class G723DePacketizer  : public DummyRTPVoiceDePacketizer{
      DYNAMIC_CAST_DECL(G723DePacketizer, DummyRTPVoiceDePacketizer)
   public:
      G723DePacketizer() : DummyRTPVoiceDePacketizer() {}
      ~G723DePacketizer() {}

      Status SetPacket(MediaData *pPacket);
   protected:
      void getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset);
   };

   /*G726 RTP payload*/
   typedef enum {
      RTP = 0,
      AAL2 = 1
   } G726payloadType;

   class G726PacketizerParams : public RTPBasePacketizerParams{
      DYNAMIC_CAST_DECL(G726PacketizerParams, RTPBasePacketizerParams)
   public:

      G726PacketizerParams()  : RTPBasePacketizerParams () {
         m_ptType = RTP;
      }
      G726payloadType m_ptType;
   };

   class G726Packetizer : public DummyRTPVoicePacketizer{
      DYNAMIC_CAST_DECL(G726Packetizer, DummyRTPVoicePacketizer)
   public:
      G726Packetizer() : DummyRTPVoicePacketizer() {m_ptType = RTP;}
      ~G726Packetizer() {}
      Status Init(RTPBasePacketizerParams *pParams);
   protected:
      G726payloadType m_ptType;
   };

   class G726DePacketizerParams : public RTPBaseDePacketizerParams{
      DYNAMIC_CAST_DECL(G726DePacketizerParams, RTPBaseDePacketizerParams)
   public:

      G726DePacketizerParams()  : RTPBaseDePacketizerParams () {
         m_ptType = RTP;
         m_Bitrate = 0;
      }

      G726payloadType m_ptType;
      Ipp32s m_Bitrate;
   };

   class G726DePacketizer  : public DummyRTPVoiceDePacketizer{
      DYNAMIC_CAST_DECL(G723DePacketizer, DummyRTPVoiceDePacketizer)
   public:
      G726DePacketizer() : DummyRTPVoiceDePacketizer() {m_ptType = RTP;}
      ~G726DePacketizer() {}

      Status Init(RTPBaseDePacketizerParams *pParams);
      Status SetPacket(MediaData *pPacket);
   protected:
      void getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset);
      G726payloadType m_ptType;
      Ipp32s m_Bitrate;
   };

   /*G728 RTP payload*/
   class G728Packetizer : public DummyRTPVoicePacketizer{
      DYNAMIC_CAST_DECL(G728Packetizer, DummyRTPVoicePacketizer)
   public:
      G728Packetizer() : DummyRTPVoicePacketizer() {}
      ~G728Packetizer() {}

      Status AddFrame(MediaData *pFrame);
   };

   class G728DePacketizer  : public DummyRTPVoiceDePacketizer{
      DYNAMIC_CAST_DECL(G728DePacketizer, DummyRTPVoiceDePacketizer)
   public:
      G728DePacketizer() : DummyRTPVoiceDePacketizer() {}
      ~G728DePacketizer() {}

      Status SetPacket(MediaData *pPacket);
   };

   /*G729 RTP payload*/

   typedef enum {
      G729 = 0,
      G729E = 1,
      G729D = 2
   } G729payloadType;

   class G729Packetizer : public DummyRTPVoicePacketizer{
      DYNAMIC_CAST_DECL(G729Packetizer, DummyRTPVoicePacketizer)
   public:
      G729Packetizer() : DummyRTPVoicePacketizer() {}
      ~G729Packetizer() {}
      Status AddFrame(MediaData *pFrame);
   };

   class G729DePacketizerParams : public RTPBaseDePacketizerParams{
      DYNAMIC_CAST_DECL(G729DePacketizerParams, RTPBaseDePacketizerParams)
   public:

      G729DePacketizerParams()  : RTPBaseDePacketizerParams () {
         m_ptType = G729;
      }

      G729payloadType m_ptType;
   };

   class G729DePacketizer  : public DummyRTPVoiceDePacketizer{
      DYNAMIC_CAST_DECL(G729DePacketizer, DummyRTPVoiceDePacketizer)
   public:
      G729DePacketizer() : DummyRTPVoiceDePacketizer() {m_ptType = G729;}
      ~G729DePacketizer() {}

      Status Init(RTPBaseDePacketizerParams *pParams);
      Status SetPacket(MediaData *pPacket);
   protected:
      void getFrmFromPayload(int getSIDFlag, Ipp8u *pPayload, SpeechData *pNewFrame, int* offset);
      G729payloadType m_ptType;
   };

   /*G722.1 RTP payload*/
   class G7221Packetizer : public DummyRTPVoicePacketizer{
      DYNAMIC_CAST_DECL(G7221Packetizer, DummyRTPVoicePacketizer)
   public:
      G7221Packetizer() : DummyRTPVoicePacketizer() {}
      ~G7221Packetizer() {}
   };

   class G7221DePacketizerParams : public RTPBaseDePacketizerParams{
      DYNAMIC_CAST_DECL(G7221DePacketizerParams, RTPBaseDePacketizerParams)
   public:

      G7221DePacketizerParams()  : RTPBaseDePacketizerParams () {
         m_Bitrate = 0;
      }

      Ipp32s m_Bitrate;
   };

   class G7221DePacketizer  : public DummyRTPVoiceDePacketizer{
      DYNAMIC_CAST_DECL(G7221DePacketizer, DummyRTPVoiceDePacketizer)
   public:
      G7221DePacketizer() : DummyRTPVoiceDePacketizer() {m_Bitrate = 0;}
      ~G7221DePacketizer() {}

      Status Init(RTPBaseDePacketizerParams *pParams);
      Status SetPacket(MediaData *pPacket);
   protected:
      void getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset);
      Ipp32s m_Bitrate;
   };

   /*GSM RTP payload*/
   class GSMPacketizer : public DummyRTPVoicePacketizer{
      DYNAMIC_CAST_DECL(GSMPacketizer, DummyRTPVoicePacketizer)
   public:
      GSMPacketizer() : DummyRTPVoicePacketizer() {}
      ~GSMPacketizer() {}
      Status AddFrame(MediaData *pFrame);
   };

   class GSMDePacketizer  : public DummyRTPVoiceDePacketizer{
      DYNAMIC_CAST_DECL(GSMDePacketizer, DummyRTPVoiceDePacketizer)
   public:
      GSMDePacketizer() : DummyRTPVoiceDePacketizer() {m_Bitrate = 0;}
      ~GSMDePacketizer() {}

      Status SetPacket(MediaData *pPacket);
   protected:
      void getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset);
      Ipp32s m_Bitrate;
   };

   /*G722 RTP payload*/
   class G722Packetizer : public DummyRTPVoicePacketizer{
      DYNAMIC_CAST_DECL(G722Packetizer, DummyRTPVoicePacketizer)
   public:
      G722Packetizer() : DummyRTPVoicePacketizer() {}
      ~G722Packetizer() {}
      Status AddFrame(MediaData *pFrame);
   };

   class G722DePacketizer  : public DummyRTPVoiceDePacketizer{
      DYNAMIC_CAST_DECL(G722DePacketizer, DummyRTPVoiceDePacketizer)
   public:
      G722DePacketizer() : DummyRTPVoiceDePacketizer() {m_Bitrate = 0;}
      ~G722DePacketizer() {}

      Status SetPacket(MediaData *pPacket);
   protected:
      void getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset);
      Ipp32s m_Bitrate;
   };

   /*iLBC RTP payload*/

   class iLBCPacketizer : public DummyRTPVoicePacketizer{
      DYNAMIC_CAST_DECL(iLBCPacketizer, DummyRTPVoicePacketizer)
   public:
      iLBCPacketizer() : DummyRTPVoicePacketizer() {}
      ~iLBCPacketizer() {}
   };

   class iLBCDePacketizerParams : public RTPBaseDePacketizerParams{
      DYNAMIC_CAST_DECL(iLBCDePacketizerParams, RTPBaseDePacketizerParams)
   public:

      iLBCDePacketizerParams()  : RTPBaseDePacketizerParams () {
         m_mode = 20;
      }

      Ipp32s m_mode;
   };

   class iLBCDePacketizer  : public DummyRTPVoiceDePacketizer{
      DYNAMIC_CAST_DECL(iLBCDePacketizer, DummyRTPVoiceDePacketizer)
   public:
      iLBCDePacketizer() : DummyRTPVoiceDePacketizer() {m_mode = 20;}
      ~iLBCDePacketizer() {}

      Status Init(RTPBaseDePacketizerParams *pParams);
      Status SetPacket(MediaData *pPacket);
   protected:
      void getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset);
      Ipp32s m_mode;
   };

   /*RTA RTP payload*/
   class RTAPacketizer : public DummyRTPVoicePacketizer{
      DYNAMIC_CAST_DECL(RTAPacketizer, DummyRTPVoicePacketizer)
   public:
      RTAPacketizer() : DummyRTPVoicePacketizer() {}
      ~RTAPacketizer() {}
   };

   class RTADePacketizerParams : public RTPBaseDePacketizerParams{
      DYNAMIC_CAST_DECL(RTADePacketizerParams, RTPBaseDePacketizerParams)
   public:

      RTADePacketizerParams()  : RTPBaseDePacketizerParams () {
         m_Bitrate = 0;
      }
      Ipp32s m_Bitrate;
   };

   class RTADePacketizer  : public DummyRTPVoiceDePacketizer{
      DYNAMIC_CAST_DECL(RTADePacketizer, DummyRTPVoiceDePacketizer)
   public:
      RTADePacketizer() : DummyRTPVoiceDePacketizer() {m_Bitrate=0;}
      ~RTADePacketizer() {}

      Status Init(RTPBaseDePacketizerParams *pParams);
      Status SetPacket(MediaData *pPacket);
   protected:
      void getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset);
      Ipp32s m_Bitrate;
   };
}


#endif /*__COMMON_PAYLOAD_H__*/
