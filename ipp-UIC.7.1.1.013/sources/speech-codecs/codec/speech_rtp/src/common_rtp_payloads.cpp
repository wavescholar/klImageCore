/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "ipps.h"
#include "common_rtp_payloads.h"

namespace UMC
{

   Status G711Packetizer::AddFrame(MediaData *pFrame)
   {
      SpeechData *pSpeechFrame = DynamicCast<SpeechData, MediaData>(pFrame);
      if(pSpeechFrame==NULL) return UMC_ERR_NULL_PTR;

      if(pSpeechFrame->GetFrameType()==3) {
         return DummyRTPVoicePacketizer::AddFrame(pFrame);
      }
      return UMC_OK;
   }


   #define G711_BIT_SIZE 80

   Status G711DePacketizer::SetPacket(MediaData *pPacket)
   {
      Ipp8u *pBitPtr;
      Ipp32s payloadSize;
      Ipp32u tsOffset = 0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      DummyRTPVoiceDePacketizer::Clean();

      pBitPtr = (Ipp8u*)pPacket->GetDataPointer();
      payloadSize = (Ipp32s)pPacket->GetDataSize();

      while(payloadSize >= G711_BIT_SIZE) { /*10 ms frame*/
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(G711_BIT_SIZE);
         pNewFrame->SetBitrate(64000);
         pNewFrame->SetFrameType(3);
         pNewFrame->SetNBytes(G711_BIT_SIZE);
         pNewFrame->SetDataSize(G711_BIT_SIZE);
         ippsCopy_8u(pBitPtr,(Ipp8u*)pNewFrame->GetDataPointer(),G711_BIT_SIZE);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);
         pBitPtr += G711_BIT_SIZE;
         payloadSize -= G711_BIT_SIZE;
         tsOffset += 80;
      }

      return UMC_OK;
   }

   void G723DePacketizer::getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset)
   {
      int i, HDR = 0;

      for ( i = 0 ; i < 2 ; i ++ ){
         Ipp32s  lTemp;
         lTemp = (pPayload[0] >> (i & 0x7)) & 1;
         HDR +=  lTemp << i ;
      }

      if(HDR==0) {
         pNewFrame->SetNBytes(24);
         pNewFrame->SetBitrate(6300);
         pNewFrame->SetFrameType(0);
         pNewFrame->SetDataSize(24);
         *offset = 24;
      } else if(HDR==1) {
         pNewFrame->SetNBytes(20);
         pNewFrame->SetBitrate(5300);
         pNewFrame->SetFrameType(0);
         pNewFrame->SetDataSize(20);
         *offset = 20;
      } else if(HDR==2) /*SID*/{
         pNewFrame->SetNBytes(4);
         pNewFrame->SetBitrate(5300);/*Prev bitrayte*/
         pNewFrame->SetFrameType(0);
         pNewFrame->SetDataSize(4);
         *offset = 4;
      } else if(HDR==3)/*Untr*/ {
         pNewFrame->SetNBytes(1);
         pNewFrame->SetBitrate(5300); /*Prev bitrayte*/
         pNewFrame->SetFrameType(0);
         pNewFrame->SetDataSize(1);
         *offset = 1;
      }
      ippsCopy_8u(pPayload,(Ipp8u*)pNewFrame->GetDataPointer(),*offset);

      return;
   }

   #define G723_MAX_BIT_SIZE 24
   Status G723DePacketizer::SetPacket(MediaData *pPacket)
   {
      Ipp8u *pBitPtr;
      Ipp32s payloadSize;
      Ipp32u tsOffset = 0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      DummyRTPVoiceDePacketizer::Clean();

      pBitPtr = (Ipp8u*)pPacket->GetDataPointer();
      payloadSize = (Ipp32s)pPacket->GetDataSize();

      while(payloadSize > 0) {
         int offset;
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(G723_MAX_BIT_SIZE);
         getFrmFromPayload(pBitPtr, pNewFrame, &offset);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);

         pBitPtr += offset;
         payloadSize -= offset;
         tsOffset += 240;
      }

      return UMC_OK;
   }

   Status G726Packetizer::Init(RTPBasePacketizerParams *pParams)
   {
      G726PacketizerParams *pG726Params = DynamicCast<G726PacketizerParams, RTPBasePacketizerParams>(pParams);

      if(pG726Params==NULL) return UMC_ERR_NULL_PTR;

      m_ptType = pG726Params->m_ptType;

      if(AAL2 == m_ptType) {
         return UMC_ERR_NOT_IMPLEMENTED;
      }

      return UMC_OK;
   }

   Status G726DePacketizer::Init(RTPBaseDePacketizerParams *pParams)
   {
      G726DePacketizerParams *pG726Params = DynamicCast<G726DePacketizerParams, RTPBaseDePacketizerParams>(pParams);

      if(pG726Params==NULL) return UMC_ERR_NULL_PTR;

      m_ptType = pG726Params->m_ptType;

      if(AAL2 == m_ptType) {
         return UMC_ERR_NOT_IMPLEMENTED;
      }

      m_Bitrate = pG726Params->m_Bitrate;

      if((m_Bitrate != 16000)&&(m_Bitrate != 24000)&&
         (m_Bitrate != 32000)&&(m_Bitrate != 40000)) {
            return UMC_ERR_INVALID_PARAMS;
      }
      return UMC_OK;
   }

   void G726DePacketizer::getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset)
   {
      if(m_Bitrate==16000) {
         pNewFrame->SetNBytes(20);
         pNewFrame->SetBitrate(16000);
         pNewFrame->SetDataSize(20);
         *offset = 20;
      } else if(m_Bitrate==24000) {
         pNewFrame->SetNBytes(30);
         pNewFrame->SetBitrate(24000);
         pNewFrame->SetDataSize(30);
         *offset = 30;
      } else if(m_Bitrate==32000) {
         pNewFrame->SetNBytes(40);
         pNewFrame->SetBitrate(32000);
         pNewFrame->SetDataSize(40);
         *offset = 40;
      } else if(m_Bitrate==40000) {
         pNewFrame->SetNBytes(50);
         pNewFrame->SetBitrate(40000);
         pNewFrame->SetDataSize(50);
         *offset = 50;
      }
      pNewFrame->SetFrameType(0);
      ippsCopy_8u(pPayload,(Ipp8u*)pNewFrame->GetDataPointer(),*offset);

      return;
   }

   #define G726_MAX_BIT_SIZE 50
   Status G726DePacketizer::SetPacket(MediaData *pPacket)
   {
      Ipp8u *pBitPtr;
      Ipp32s payloadSize;
      Ipp32u tsOffset = 0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      DummyRTPVoiceDePacketizer::Clean();

      pBitPtr = (Ipp8u*)pPacket->GetDataPointer();
      payloadSize = (Ipp32s)pPacket->GetDataSize();

      while(payloadSize > 0) {
         int offset;
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(G726_MAX_BIT_SIZE);
         getFrmFromPayload(pBitPtr, pNewFrame, &offset);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);

         pBitPtr += offset;
         payloadSize -= offset;
         tsOffset += 80;
      }

      return UMC_OK;
   }

   Status G728Packetizer::AddFrame(MediaData *pFrame)
   {
      SpeechData *pSpeechFrame = DynamicCast<SpeechData, MediaData>(pFrame);
      if(pSpeechFrame==NULL) return UMC_ERR_NULL_PTR;

      if(pSpeechFrame->GetBitrate()==16000) {
         return DummyRTPVoicePacketizer::AddFrame(pFrame);
      }
      return UMC_OK;
   }

   #define G728_BIT_SIZE 20

   Status G728DePacketizer::SetPacket(MediaData *pPacket)
   {
      Ipp8u *pBitPtr;
      Ipp32s payloadSize;
      Ipp32u tsOffset = 0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      DummyRTPVoiceDePacketizer::Clean();

      pBitPtr = (Ipp8u*)pPacket->GetDataPointer();
      payloadSize = (Ipp32s)pPacket->GetDataSize();

      while(payloadSize >= G728_BIT_SIZE) {
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(G728_BIT_SIZE);
         pNewFrame->SetBitrate(16000);
         pNewFrame->SetFrameType(0);
         pNewFrame->SetNBytes(G728_BIT_SIZE);
         pNewFrame->SetDataSize(G728_BIT_SIZE);
         ippsCopy_8u(pBitPtr,(Ipp8u*)pNewFrame->GetDataPointer(),G728_BIT_SIZE);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);
         pBitPtr += G728_BIT_SIZE;
         payloadSize -= G728_BIT_SIZE;
         tsOffset += 80;
      }

      return UMC_OK;
   }

   Status G729Packetizer::AddFrame(MediaData *pFrame)
   {
      SpeechData *pSpeechFrame = DynamicCast<SpeechData, MediaData>(pFrame);
      if(pSpeechFrame==NULL) return UMC_ERR_NULL_PTR;

      if(pSpeechFrame->GetFrameType()!=0) { /*Not untransmitted*/
         return DummyRTPVoicePacketizer::AddFrame(pFrame);
      }
      return UMC_OK;
   }

   Status G729DePacketizer::Init(RTPBaseDePacketizerParams *pParams)
   {
      G729DePacketizerParams *pG729Params = DynamicCast<G729DePacketizerParams, RTPBaseDePacketizerParams>(pParams);

      if(pG729Params==NULL) return UMC_ERR_NULL_PTR;

      m_ptType = pG729Params->m_ptType;

      return UMC_OK;
   }

   void G729DePacketizer::getFrmFromPayload(int getSIDFlag, Ipp8u *pPayload, SpeechData *pNewFrame, int* offset)
   {
      if(getSIDFlag) {
         pNewFrame->SetNBytes(2);
         if(m_ptType==G729) {
            pNewFrame->SetBitrate(8000);
         } else if(m_ptType==G729D) {
            pNewFrame->SetBitrate(6400);
         } else if(m_ptType==G729E) {
            pNewFrame->SetBitrate(11800);
         }
         pNewFrame->SetDataSize(2);
         pNewFrame->SetFrameType(1);
         *offset = 2;
      } else if(m_ptType==G729) {
         pNewFrame->SetNBytes(10);
         pNewFrame->SetBitrate(8000);
         pNewFrame->SetDataSize(10);
         pNewFrame->SetFrameType(3);
         *offset = 10;
      } else if(m_ptType==G729D) {
         pNewFrame->SetNBytes(8);
         pNewFrame->SetBitrate(6400);
         pNewFrame->SetDataSize(8);
         pNewFrame->SetFrameType(2);
         *offset = 8;
      } else if(m_ptType==G729E) {
         pNewFrame->SetNBytes(15);
         pNewFrame->SetBitrate(11800);
         pNewFrame->SetDataSize(15);
         pNewFrame->SetFrameType(4);
         *offset = 15;
      }
      ippsCopy_8u(pPayload,(Ipp8u*)pNewFrame->GetDataPointer(),*offset);

      return;
   }

   #define G729_MAX_BIT_SIZE 15
   Status G729DePacketizer::SetPacket(MediaData *pPacket)
   {
      Ipp8u *pBitPtr;
      Ipp32s payloadSize;
      Ipp32s framesize = 0;
      Ipp32u tsOffset = 0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      DummyRTPVoiceDePacketizer::Clean();

      pBitPtr = (Ipp8u*)pPacket->GetDataPointer();
      payloadSize = (Ipp32s)pPacket->GetDataSize();

      if(m_ptType==G729) {
         framesize = 10;
      } else if(m_ptType==G729D) {
         framesize = 8;
      } else if(m_ptType==G729E) {
         framesize = 15;
      }

      while(payloadSize >= framesize) {
         int offset;
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(G729_MAX_BIT_SIZE);
         getFrmFromPayload(0,pBitPtr, pNewFrame, &offset);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);

         pBitPtr += offset;
         payloadSize -= offset;
         tsOffset += 80;
      }

      if(payloadSize != 0) {/*SID present*/
         int offset;
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(G729_MAX_BIT_SIZE);
         getFrmFromPayload(1,pBitPtr, pNewFrame, &offset);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);
         payloadSize -= offset;
      }

      if(payloadSize != 0) {
         return UMC_ERR_INVALID_STREAM;
      }
      return UMC_OK;
   }

   Status G7221DePacketizer::Init(RTPBaseDePacketizerParams *pParams)
   {
      G7221DePacketizerParams *pG7221Params = DynamicCast<G7221DePacketizerParams, RTPBaseDePacketizerParams>(pParams);

      if(pG7221Params==NULL) return UMC_ERR_NULL_PTR;

      m_Bitrate = pG7221Params->m_Bitrate;

      if((m_Bitrate != 16000)&&(m_Bitrate != 24000)&&(m_Bitrate != 32000)) {
            return UMC_ERR_INVALID_PARAMS;
      }
      return UMC_OK;
   }

   void G7221DePacketizer::getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset)
   {
      pNewFrame->SetNBytes(m_Bitrate/50/8);
      pNewFrame->SetBitrate(m_Bitrate);
      pNewFrame->SetDataSize(m_Bitrate/50/8);
      *offset = m_Bitrate/50/8;

      pNewFrame->SetFrameType(0);
      ippsCopy_8u(pPayload,(Ipp8u*)pNewFrame->GetDataPointer(),*offset);

      return;
   }

   #define G7221_MAX_BIT_SIZE (32000/50/8)
   Status G7221DePacketizer::SetPacket(MediaData *pPacket)
   {
      Ipp8u *pBitPtr;
      Ipp32s payloadSize;
      Ipp32u tsOffset = 0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      DummyRTPVoiceDePacketizer::Clean();

      pBitPtr = (Ipp8u*)pPacket->GetDataPointer();
      payloadSize = (Ipp32s)pPacket->GetDataSize();

      while(payloadSize > 0) {
         int offset;
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(G7221_MAX_BIT_SIZE);
         getFrmFromPayload(pBitPtr, pNewFrame, &offset);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);

         pBitPtr += offset;
         payloadSize -= offset;
         tsOffset += 160;
      }

      return UMC_OK;
   }

   Status GSMPacketizer::AddFrame(MediaData *pFrame)
   {
      SpeechData *pSpeechFrame = DynamicCast<SpeechData, MediaData>(pFrame);
      if(pSpeechFrame==NULL) return UMC_ERR_NULL_PTR;
      Ipp8u *pBitstream;

      pBitstream = (Ipp8u*)pSpeechFrame->GetDataPointer();

      pBitstream[0] = pBitstream[0] & 0xF;
      pBitstream[0] = pBitstream[0] | (0xD << 4);

      return DummyRTPVoicePacketizer::AddFrame(pFrame);
   }

   #define GSM_MAX_BIT_SIZE 33
   void GSMDePacketizer::getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset)
   {
      pNewFrame->SetNBytes(GSM_MAX_BIT_SIZE);
      pNewFrame->SetBitrate(13000);
      pNewFrame->SetDataSize(GSM_MAX_BIT_SIZE);
      *offset = GSM_MAX_BIT_SIZE;

      pNewFrame->SetFrameType(-1);
      pPayload[0] = pPayload[0] & 0xF;
      ippsCopy_8u(pPayload,(Ipp8u*)pNewFrame->GetDataPointer(),*offset);

      return;
   }

   Status GSMDePacketizer::SetPacket(MediaData *pPacket)
   {
      Ipp8u *pBitPtr;
      Ipp32s payloadSize;
      Ipp32u tsOffset = 0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      DummyRTPVoiceDePacketizer::Clean();

      pBitPtr = (Ipp8u*)pPacket->GetDataPointer();
      payloadSize = (Ipp32s)pPacket->GetDataSize();

      while(payloadSize > 0) {
         int offset;
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(GSM_MAX_BIT_SIZE);
         getFrmFromPayload(pBitPtr, pNewFrame, &offset);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);

         tsOffset += 80;
         pBitPtr += offset;
         payloadSize -= offset;
      }

      return UMC_OK;
   }

   Status G722Packetizer::AddFrame(MediaData *pFrame)
   {
      SpeechData *pSpeechFrame = DynamicCast<SpeechData, MediaData>(pFrame);
      if(pSpeechFrame==NULL) return UMC_ERR_NULL_PTR;

      if(pSpeechFrame->GetBitrate()==64000) {
         return DummyRTPVoicePacketizer::AddFrame(pFrame);
      }
      return UMC_ERR_INVALID_STREAM;
   }

   #define G722_MAX_BIT_SIZE 160
   void G722DePacketizer::getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset)
   {
      pNewFrame->SetNBytes(G722_MAX_BIT_SIZE);
      pNewFrame->SetBitrate(64000);
      pNewFrame->SetDataSize(G722_MAX_BIT_SIZE);
      pNewFrame->SetFrameType(0);

      *offset = G722_MAX_BIT_SIZE;

      ippsCopy_8u(pPayload,(Ipp8u*)pNewFrame->GetDataPointer(),*offset);

      return;
   }

   Status G722DePacketizer::SetPacket(MediaData *pPacket)
   {
      Ipp8u *pBitPtr;
      Ipp32s payloadSize;
      Ipp32u tsOffset = 0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      DummyRTPVoiceDePacketizer::Clean();

      pBitPtr = (Ipp8u*)pPacket->GetDataPointer();
      payloadSize = (Ipp32s)pPacket->GetDataSize();

      while(payloadSize > 0) {
         int offset;
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(G722_MAX_BIT_SIZE);
         getFrmFromPayload(pBitPtr, pNewFrame, &offset);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);

         tsOffset += 160;
         pBitPtr += offset;
         payloadSize -= offset;
      }

      return UMC_OK;
   }

   Status iLBCDePacketizer::Init(RTPBaseDePacketizerParams *pParams)
   {
      iLBCDePacketizerParams *piLBCParams = DynamicCast<iLBCDePacketizerParams, RTPBaseDePacketizerParams>(pParams);

      if(piLBCParams==NULL) return UMC_ERR_NULL_PTR;

      m_mode = piLBCParams->m_mode;

      if((m_mode!=20) && (m_mode!=30)) {
         return UMC_ERR_INVALID_PARAMS;
      }

      return UMC_OK;
   }

   void iLBCDePacketizer::getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset)
   {
      if(m_mode==20) {
         pNewFrame->SetNBytes(38);
         pNewFrame->SetBitrate(15200);
         pNewFrame->SetDataSize(38);
         *offset = 38;
      } else if(m_mode==30) {
         pNewFrame->SetNBytes(50);
         pNewFrame->SetBitrate(13330);
         pNewFrame->SetDataSize(50);
         *offset = 50;
      }
      pNewFrame->SetFrameType(0);
      ippsCopy_8u(pPayload,(Ipp8u*)pNewFrame->GetDataPointer(),*offset);

      return;
   }

   #define iLBC_MAX_BIT_SIZE 50
   Status iLBCDePacketizer::SetPacket(MediaData *pPacket)
   {
      Ipp8u *pBitPtr;
      Ipp32s payloadSize;
      Ipp32u tsOffset = 0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      DummyRTPVoiceDePacketizer::Clean();

      pBitPtr = (Ipp8u*)pPacket->GetDataPointer();
      payloadSize = (Ipp32s)pPacket->GetDataSize();

      while(payloadSize > 0) {
         int offset;
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(iLBC_MAX_BIT_SIZE);
         getFrmFromPayload(pBitPtr, pNewFrame, &offset);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);

         pBitPtr += offset;
         payloadSize -= offset;
         if(m_mode==20) {
            tsOffset += 160;
         } else if(m_mode==30) {
            tsOffset += 240;
         }
      }

      return UMC_OK;
   }

   #define RTA_MAX_BIT_SIZE 200

   Status RTADePacketizer::Init(RTPBaseDePacketizerParams *pParams)
   {
      RTADePacketizerParams *pRTAParams = DynamicCast<RTADePacketizerParams, RTPBaseDePacketizerParams>(pParams);

      if(pRTAParams==NULL) return UMC_ERR_NULL_PTR;

      m_Bitrate = pRTAParams->m_Bitrate;

      if((m_Bitrate != 8800)&&(m_Bitrate != 18000)&&
         (m_Bitrate != 22056)&&(m_Bitrate != 32000)) {
            return UMC_ERR_INVALID_PARAMS;
      }
      return UMC_OK;
   }

   void RTADePacketizer::getFrmFromPayload(Ipp8u *pPayload, SpeechData *pNewFrame, int* offset)
   {
      /*Currently one frame in packet is supported*/
      pNewFrame->SetNBytes(*offset);
      pNewFrame->SetBitrate(m_Bitrate);
      pNewFrame->SetFrameType(*offset);
      pNewFrame->SetDataSize(*offset);

      ippsCopy_8u(pPayload,(Ipp8u*)pNewFrame->GetDataPointer(),*offset);
      return;
   }

   Status RTADePacketizer::SetPacket(MediaData *pPacket)
   {
      Ipp8u *pBitPtr;
      Ipp32s payloadSize;
      Ipp32u tsOffset = 0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      DummyRTPVoiceDePacketizer::Clean();

      pBitPtr = (Ipp8u*)pPacket->GetDataPointer();
      payloadSize = (Ipp32s)pPacket->GetDataSize();

      while(payloadSize > 0) {
         int offset;
         SpeechData *pNewFrame;
         pNewFrame = new SpeechData(RTA_MAX_BIT_SIZE);
         offset = payloadSize;
         getFrmFromPayload(pBitPtr, pNewFrame, &offset);
         Ipp32u ts = (Ipp32u)pPacket->GetTime();
         pNewFrame->SetTime(ts+tsOffset);
         DummyRTPVoiceDePacketizer::AddExtractedFrame(pNewFrame);

         pBitPtr += offset;
         payloadSize -= offset;
         tsOffset += 160;
      }

      return UMC_OK;
   }

}
