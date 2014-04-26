/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2012 Intel Corporation. All Rights Reserved.
//
//
*/
#include "ipps.h"
#include "umc_speech_jbuffer.h"
#include "vm_time.h"

namespace UMC
{

   SpeechJBuffer::SpeechJBuffer()
   {
      m_pJBuffer = NULL;
      m_pParams = NULL;
      m_PrevPut = 0;
      m_pJpIn = NULL;
      m_TimeStampInc = 0;
      m_EOSFlag = 0;
   }

   SpeechJBuffer::~SpeechJBuffer()
   {
      Close();
   }

   Status SpeechJBuffer::Close()
   {
      m_pParams = NULL;
      UMC_DELETE(m_pJBuffer);

      if(m_pJpIn) {
         delete m_pJpIn;
         m_pJpIn = NULL;
      }

      m_PrevPut = 0;
      m_TimeStampInc = 0;
      m_EOSFlag = 0;

      MediaBuffer::Close();
      return UMC_OK;
   }

   Status SpeechJBuffer::Init(MediaReceiverParams* init)
   {
      JBufferParams *pParams = DynamicCast<JBufferParams> (init);
      Status umcRes;

      // check error(s)
      if (NULL == pParams)
          return UMC_ERR_NULL_PTR;

      if ((0 == pParams->m_numberOfFrames) ||
          (0 == pParams->m_prefInputBufferSize) ||
          (0 == pParams->m_prefOutputBufferSize))
          return UMC_ERR_INIT;

      // release the object before initialization
      Close();

      // call the parent's method
      umcRes = MediaBuffer::Init(init);
      UMC_CHECK_STATUS(umcRes);

      m_pJBuffer = new JitterBuffer(IPP_MAX(pParams->m_prefInputBufferSize,pParams->m_prefOutputBufferSize)*pParams->m_numberOfFrames);
      UMC_CHECK(m_pJBuffer != NULL, UMC_ERR_ALLOC);

      UMC_CALL(m_pJBuffer->Init(pParams->m_MSSizePacket,pParams->m_numSizeJitter,pParams->m_numThresholdDiscard));

      m_pParams = pParams;

      m_PrevPut = 0;
      m_TimeStampInc = /*2**/ (m_pParams->m_MSSizePacket * m_pParams->m_SampleRate) / 1000;
      m_EOSFlag = 0;
      return UMC_OK;
   }
   Status SpeechJBuffer::Reset(void)
   {
      UMC_CHECK_PTR(m_pJBuffer);
      UMC_CHECK_PTR(m_pParams);

      UMC_CALL(MediaBuffer::Reset());
      UMC_CALL(m_pJBuffer->Init(m_pParams->m_MSSizePacket,m_pParams->m_numSizeJitter,m_pParams->m_numThresholdDiscard));
      m_PrevPut = 0;
      m_EOSFlag = 0;

      return UMC_OK;
   }

   Status SpeechJBuffer::Stop(void)
   {
      m_EOSFlag = 1;
      return UMC_OK;
   }

   Status SpeechJBuffer::LockInputBuffer(MediaData* in)
   {
      UMC_CHECK_PTR(in);

      /*Sould check for possible buffer overflow*/
      return UMC_OK;
   }

   static const char *pEndStreamID = "----!!!END_OF_STREAM!!!----";
   #define END_ID_LEN 28
   #define PACKED_PRMS_SIZE (3*sizeof(Ipp32s))

   Status SpeechJBuffer::UnLockInputBuffer(MediaData* in, Status StreamStatus)
   {
      SpeechData *pSData = NULL;

      if(0 != m_EOSFlag) return UMC_OK;

      if(UMC_OK != StreamStatus) {
         m_EOSFlag = 1;
      } else {
         UMC_CHECK_PTR(in);
         UMC_CHECK_PTR(in->GetDataPointer());
         pSData = DynamicCast<SpeechData> (in);
         UMC_CHECK_PTR(pSData);
         if(m_pJpIn==NULL) {
            m_pJpIn = new JPacketMediaData(pSData->GetDataSize() + PACKED_PRMS_SIZE);
            UMC_CHECK(m_pJpIn!=NULL, UMC_ERR_ALLOC);
         } else {
            if((pSData->GetDataSize() + PACKED_PRMS_SIZE) > m_pJpIn->GetBufferSize()) {
               m_pJpIn->Alloc(pSData->GetDataSize() + PACKED_PRMS_SIZE);
            }
         }

         ownCnvtToJPacket(pSData, m_pJpIn);
         UMC_CALL(m_pJBuffer->PutPacket(m_pJpIn));
         m_PrevPut = m_pJpIn->GetMarker();
      }

      return UMC_OK;
   }

   Status SpeechJBuffer::LockOutputBuffer(MediaData* out)
   {
      Status umcRes, st = UMC_OK;
      UMC_CHECK_PTR(out);
      JPacketMediaData pJData;
      SpeechData pSData;
      MediaData DecData(m_TimeStampInc*2);

      StatusData jDatastatus;
      int gotFrames = 0;
      /*Sould check for possible data in a buffer*/
      while(gotFrames < m_pParams->m_numSizeJitter && st==UMC_OK) {
         umcRes = m_pJBuffer->GetPacket(&pJData,&jDatastatus);
         if(UMC_OK==umcRes && SJB_OK==jDatastatus) {
            st = UMC_OK;
            if(pJData.GetDataSize()==END_ID_LEN) {
               if(strcmp((const char *)pJData.GetDataPointer(),pEndStreamID)==0) {
                  st = UMC_ERR_END_OF_STREAM;
                  break;
                  //return st;
               }
            }
            ownCnvtToSpeechData(&pJData, &pSData);
            UMC_CALL(m_pParams->m_pDecoder->GetFrame(&pSData,&DecData));
            DecData.MoveDataTo(out);
            DecData.Reset();
            gotFrames++;
            if( m_EOSFlag == 1 && pJData.GetMarker() > m_PrevPut) {
               return UMC_ERR_END_OF_STREAM;
            }
         } else if((UMC_OK==umcRes && SJB_WAIT==jDatastatus) || (UMC_ERR_NOT_ENOUGH_DATA==umcRes && SJB_WAIT==jDatastatus)) {
            //vm_time_sleep(2);
            st = UMC_ERR_NOT_ENOUGH_DATA;
         } else if((UMC_OK==umcRes && SJB_LOST==jDatastatus) || (UMC_ERR_NOT_ENOUGH_DATA==umcRes && SJB_LOST==jDatastatus)) {
            /* Lost frame*/
            UMC::MediaData lostPacket;
            UMC_CALL(m_pParams->m_pDecoder->GetFrame(&lostPacket,&DecData));
            DecData.MoveDataTo(out);
            DecData.Reset();
            st = UMC_OK;
            gotFrames++;
            if(m_EOSFlag == 1 && pJData.GetMarker() > m_PrevPut) {
               return UMC_ERR_END_OF_STREAM;
            }
         } else {
            if(m_EOSFlag == 1 && pJData.GetMarker() > m_PrevPut) {
               return UMC_ERR_END_OF_STREAM;
            }
         }
      }
      return st;
   }

   Status SpeechJBuffer::UnLockOutputBuffer(MediaData* out)
   {
      UMC_CHECK_PTR(out);
      /*Sould check for possible data in a buffer*/
      return UMC_OK;
   }

   #define PACKED_PRMS_SIZE (3*sizeof(Ipp32s))
   void SpeechJBuffer::ownCnvtToJPacket(UMC::SpeechData *pIn, UMC::JPacketMediaData *pOut)
   {
      Ipp64f stTime, endTime;
      Ipp32s param;
      if(pOut->GetBufferSize() < pIn->GetDataSize() + PACKED_PRMS_SIZE) {
         pOut->Alloc(pIn->GetDataSize() + PACKED_PRMS_SIZE);
      } else {
         pOut->Reset();
      }
      pIn->GetTime(stTime, endTime);
      pOut->SetTime(stTime, endTime);

      param = pIn->GetBitrate();
      ippsCopy_8u((Ipp8u*)&param,(Ipp8u*)pOut->GetBufferPointer(),sizeof(Ipp32s));
      param = pIn->GetFrameType();
      ippsCopy_8u((Ipp8u*)&param,(Ipp8u*)pOut->GetBufferPointer() + sizeof(Ipp32s),sizeof(Ipp32s));
      param = pIn->GetNBytes();
      ippsCopy_8u((Ipp8u*)&param,(Ipp8u*)pOut->GetBufferPointer() + 2*sizeof(Ipp32s),sizeof(Ipp32s));

      ippsCopy_8u((Ipp8u*)pIn->GetDataPointer(),(Ipp8u*)pOut->GetBufferPointer() + PACKED_PRMS_SIZE,(int)pIn->GetDataSize());
      pOut->SetDataSize(pIn->GetDataSize() + PACKED_PRMS_SIZE);

      pOut->SetMarker(pIn->GetTime()/m_TimeStampInc);
   }

   void SpeechJBuffer::ownCnvtToSpeechData(UMC::JPacketMediaData *pIn, UMC::SpeechData *pOut)
   {
      Ipp64f stTime, endTime;
      Ipp32s param;
      if(pOut->GetBufferSize() < pIn->GetDataSize()) {
         pOut->Alloc(pIn->GetDataSize());
      } else {
         pOut->Reset();
      }
      pIn->GetTime(stTime, endTime);
      pOut->SetTime(stTime, endTime);

      ippsCopy_8u((Ipp8u*)pIn->GetBufferPointer(),(Ipp8u*)&param,sizeof(Ipp32s));
      pOut->SetBitrate(param);
      ippsCopy_8u((Ipp8u*)pIn->GetBufferPointer() + sizeof(Ipp32s),(Ipp8u*)&param,sizeof(Ipp32s));
      pOut->SetFrameType(param);
      ippsCopy_8u((Ipp8u*)pIn->GetBufferPointer() + 2*sizeof(Ipp32s),(Ipp8u*)&param,sizeof(Ipp32s));
      pOut->SetNBytes(param);

      ippsCopy_8u((Ipp8u*)pIn->GetDataPointer()+PACKED_PRMS_SIZE,(Ipp8u*)pOut->GetBufferPointer(),(int)pIn->GetDataSize()-PACKED_PRMS_SIZE);
      pOut->SetDataSize(pIn->GetDataSize() - PACKED_PRMS_SIZE);
   }
} // namespace UMC


