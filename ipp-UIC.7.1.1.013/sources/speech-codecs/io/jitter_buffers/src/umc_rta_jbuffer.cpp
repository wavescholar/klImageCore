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
#include "umc_rta_jbuffer.h"

#include "ippsc.h"
#include "wmsprt.h"
#include "umc_rtp_packet.h"

namespace UMC
{

   MemoryAllocator *pMemoryAllocator = NULL;

   RTAJBuffer::RTAJBuffer()
   {
      m_pParams = NULL;
      m_hDJStatus = NULL;
      m_decHandle = NULL;
      m_EOS = 0;
   }

   RTAJBuffer::~RTAJBuffer()
   {
      Close();
   }

   Status RTAJBuffer::Close()
   {
      m_pParams = NULL;

      if(m_hDJStatus) {
         MSRTADejitterDelete(&m_hDJStatus);
         m_hDJStatus = NULL;
         m_decHandle = NULL;
      }

      MediaBuffer::Close();
      pMemoryAllocator = NULL;
      m_EOS = 0;
      return UMC_OK;
   }

   void * rtaMalloc(size_t lSize)
   {
       return RTAJBuffer::extMalloc(lSize);
   }

   void rtaFree( void *pvMemory )
   {
       RTAJBuffer::extFree(pvMemory);
   }

   void * RTAJBuffer::extMalloc(size_t lSize)
   {
      if(pMemoryAllocator) {
         MemID mID;
         if (UMC_OK != pMemoryAllocator->Alloc(&mID, lSize + sizeof(MemID),UMC_ALLOC_PERSISTENT))
            return NULL;
         Ipp8u *Buff = (Ipp8u *) pMemoryAllocator->Lock(mID);
         if (NULL == Buff)
            return NULL;
         ippsCopy_8u((Ipp8u *)&mID,Buff,sizeof(MemID));
         return (void *)(Buff + sizeof(MemID));
      } else {
         return malloc(lSize);
      }
   }

   void RTAJBuffer::extFree( void *pvMemory )
   {
      if(pMemoryAllocator && pvMemory) {
         MemID mID;
         ippsCopy_8u((Ipp8u*)pvMemory - sizeof(MemID),(Ipp8u *)&mID,sizeof(MemID));

         pMemoryAllocator->Unlock(mID);
         if (MID_INVALID != mID)
            pMemoryAllocator->Free(mID);
      } else {
         free(pvMemory);
      }
   }

   Status RTAJBuffer::Init(MediaReceiverParams* init)
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

      if(pMemoryAllocator == NULL) {
         pMemoryAllocator = m_pMemoryAllocator;
      }

      MSRTARESULT hres = MSRTADejitterNew(&m_hDJStatus, rtaMalloc, rtaFree);
      if(MSRTA_OK != hres) {
         return UMC_ERR_FAILED;
      }

      hres = MSRTADejitterInit( m_hDJStatus, &m_decHandle, pParams->m_SampleRate, 0);
      if(MSRTA_OK != hres) {
         return UMC_ERR_FAILED;
      }

      m_pParams = pParams;

      MSRTADecoderGetOutputSize( m_decHandle, (U32 *)&m_FrameSize );

      m_FrameSize = m_FrameSize*2;

      m_EOS = 0;

      return UMC_OK;
   }
   Status RTAJBuffer::Reset(void)
   {
      UMC_CHECK_PTR(m_pParams);

      UMC_CALL(MediaBuffer::Reset());
      
      MSRTARESULT hres = MSRTARestartDejitterBuffer( m_hDJStatus );
      if(MSRTA_OK != hres) {
         return UMC_ERR_FAILED;
      }

      m_EOS = 0;

      return UMC_OK;
   }

   Status RTAJBuffer::Stop(void)
   {
      return UMC_OK;
   }

   Status RTAJBuffer::LockInputBuffer(MediaData* in)
   {
      UMC_CHECK_PTR(in);

      /*Sould check for possible buffer overflow*/
      return UMC_OK;
   }

   static const char *pEndStreamID = "----!!!END_OF_STREAM!!!----";
   #define END_ID_LEN 28

   Status RTAJBuffer::UnLockInputBuffer(MediaData* in, Status StreamStatus)
   {
      /*Currently FEC (if present) must be in the currentn frame*/
      // Insert current frames into jitter buffer
      // As many frames as there are in the packet can be inserted here thru multiple calls to MSRTASendToJitterBuffer()
      if(StreamStatus==UMC_OK) {
         Ipp8u *pBitstr = (Ipp8u *)in->GetDataPointer()/*, *pbFECBuf*/;
         /*Ipp32u cbSrcSizeUsed;
         Ipp32u cbFECSizeUsed;*/
         MSRTARESULT result;

         SpeechData *pJData = DynamicCast<SpeechData> (in);

         /*cbSrcSizeUsed = pBitstr[1] << 8 | pBitstr[0];
         pbFECBuf = pBitstr + 2 + cbSrcSizeUsed;
         cbFECSizeUsed = pbFECBuf[1] << 8 | pbFECBuf[0];*/ 

         //if( cbSrcSizeUsed > 0 ) {
             Ipp32u dwDataType = RT_PACKET_DATA;
             result = MSRTASendToJitterBuffer( m_hDJStatus,
                                               dwDataType,
                                               m_pParams->m_SampleRate,
                                               pBitstr,
                                               pJData->GetNBytes(),
                                               pJData->GetTime()/*/2*//*pJData->GetMarker()*m_FrameSize*/,
                                               pJData->GetTime()/*/2*/,
                                               pJData->GetTime() /m_FrameSize/* pJData->GetMarker()*/,
                                               0 );
             if( result != MSRTA_OK ){
                 return UMC_ERR_FAILED;
             }
         //}

         // Insert current FECs into jitter buffer
         // As many FEC as there are in the packet can be inserted here thru multiple calls to MSRTASendToJitterBuffer()
         // The application must parse the RTP packet, calculate absolute timestamps for FEC and then do the call.
         //if( cbFECSizeUsed > 0 ) {
         //    Ipp32u dwDataType = RT_PACKET_INDEPFEC + 1;
         //    result = MSRTASendToJitterBuffer( m_hDJStatus,
         //                                      dwDataType,
         //                                      m_pParams->m_SampleRate,
         //                                      pbFECBuf + 2,
         //                                      cbFECSizeUsed,
         //                                      pJData->GetTime()-160/*pJData->GetMarker()*m_FrameSize*/,
         //                                      pJData->GetTime()-160,
         //                                      pJData->GetTime() /m_FrameSize - 1/*pJData->GetMarker() - 1*/,
         //                                      0 );
         //    if( result != MSRTA_OK ){
         //       printf("failed %d \n",result);
         //        return UMC_ERR_FAILED;
         //    }
         //}
      } else {
         m_EOS = 1;
      }

      return UMC_OK;
   }

   Status RTAJBuffer::LockOutputBuffer(MediaData* out)
   {
      Ipp32u dwPullTime = 10;
      Ipp32s iProperty = 0;
      REFERENCE_TIME      tmDecodeTimestamp;
      Ipp32u              nDecSamplesPerSec;

      if(0 == m_EOS) {

         iProperty = iProperty & ~(1<<SET_DEC_COMFORTNOISE_SHIFT);
         iProperty = iProperty | (SET_DEC_COMFORTNOISE_ON << SET_DEC_COMFORTNOISE_SHIFT);

         MSRTARESULT result = MSRTADecodePullData( m_hDJStatus, (U8 *)out->GetDataPointer(), dwPullTime,
                                       &tmDecodeTimestamp, (U32 *)&nDecSamplesPerSec, (I32 *)&iProperty, NULL );
         if( result != MSRTA_OK ){
             return UMC_ERR_FAILED;
         }

         out->SetDataSize((nDecSamplesPerSec * dwPullTime / 1000) * sizeof(short));
      } else {
         return UMC_ERR_END_OF_STREAM;
      }

      
      return UMC_OK;
   }

   Status RTAJBuffer::UnLockOutputBuffer(MediaData* out)
   {
      UMC_CHECK_PTR(out);
      /*Sould check for possible data in a buffer*/
      return UMC_OK;
   }
} // namespace UMC