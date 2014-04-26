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
#include "dummy_rtp_payload.h"

namespace UMC
{

   DummyRTPVoicePacketizer::DummyRTPVoicePacketizer()
   {
   }
   DummyRTPVoicePacketizer::~DummyRTPVoicePacketizer()
   {
      while(m_List.Size() > 0) {
         SpeechData *pIt;
         m_List.Last(pIt);
         delete pIt;
         m_List.Remove();
      }
   }

   Status DummyRTPVoicePacketizer::Init(RTPBasePacketizerParams *pParams)
   {
      if(pParams==NULL) return UMC_ERR_NULL_PTR;

      return UMC_OK;
   }

   Status DummyRTPVoicePacketizer::SetControls(RTPBaseControlParams *pControls)
   {
      if(pControls==NULL) return UMC_ERR_NULL_PTR;

      return UMC_OK;
   }

   Status DummyRTPVoicePacketizer::AddFrame(MediaData *pFrame)
   {
      SpeechData *pSpeechFrame = DynamicCast<SpeechData, MediaData>(pFrame);
      if(pSpeechFrame==NULL) return UMC_ERR_NULL_PTR;

      SpeechData *pNewFrame = NULL;

      pNewFrame = new SpeechData((Ipp32u)pFrame->GetDataSize());
      if(NULL == pNewFrame) return UMC_ERR_ALLOC;

      pSpeechFrame->MoveDataTo(pNewFrame);

      m_List.Add(pNewFrame);

      return UMC_OK;
   }

   Status DummyRTPVoicePacketizer::GetPacket(MediaData *pPacket)
   {
      Ipp8u *pPacketPayload;
      Ipp8u *pFrameContent;
      Ipp32s PayloadSize = 0;
      Ipp32u i;
      SpeechData *pIt;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;
      if(m_List.Size()==0) return UMC_ERR_NOT_ENOUGH_DATA;

      pPacketPayload = (Ipp8u *)pPacket->GetDataPointer();

      /*Write audio data*/

      for(i=0;i<m_List.Size();i++) {
         Ipp32s bitSize;
         m_List.Get(pIt,i);
         pFrameContent = (Ipp8u *)pIt->GetDataPointer();
         bitSize = pIt->GetNBytes();

         ippsCopy_8u(pFrameContent, pPacketPayload,bitSize);
         pPacketPayload += bitSize;
         PayloadSize += bitSize;
      }

      pPacket->SetDataSize(PayloadSize);

      while(m_List.Size() > 0) { /*Clean list*/
         SpeechData *pIt;
         m_List.Last(pIt);
         delete pIt;
         m_List.Remove();
      }

      return UMC_OK;
   }

   DummyRTPVoiceDePacketizer::DummyRTPVoiceDePacketizer() : RTPBaseDepacketizer()
   {
   }

   DummyRTPVoiceDePacketizer::~DummyRTPVoiceDePacketizer()
   {
      while(m_List.Size() > 0) {
         SpeechData *pIt;
         m_List.Last(pIt);
         delete pIt;
         m_List.Remove();
      }
   }

   Status DummyRTPVoiceDePacketizer::Init(RTPBaseDePacketizerParams *pParams)
   {
      if(pParams==NULL) return UMC_ERR_NULL_PTR;

      return UMC_OK;
   }

   Status DummyRTPVoiceDePacketizer::GetControls(RTPBaseControlParams *pControls)
   {
      if(pControls==NULL) return UMC_ERR_NULL_PTR;

      return UMC_OK;
   }

   Status DummyRTPVoiceDePacketizer::GetNextFrame(MediaData *pFrame)
   {
      SpeechData *pSpeechFrame = DynamicCast<SpeechData, MediaData>(pFrame);
      if(pFrame==NULL) return UMC_ERR_NULL_PTR;

      if(m_List.Size() > 0) {
         SpeechData *pIt;
         m_List.First(pIt);
         pIt->MoveDataTo(pSpeechFrame);
         delete pIt;
         m_List.Remove(0);
      } else {
         return UMC_ERR_NOT_ENOUGH_DATA;
      }
      return UMC_OK;
   }

   void DummyRTPVoiceDePacketizer::Clean()
   {
      while(m_List.Size() > 0) {
         SpeechData *pIt;
         m_List.Last(pIt);
         delete pIt;
         m_List.Remove();
      }

      return;
   }

   void DummyRTPVoiceDePacketizer::AddExtractedFrame(SpeechData *pFrame)
   {
      m_List.Add(pFrame);
   }

};
