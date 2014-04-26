//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2012 Intel Corporation. All Rights Reserved.
//
//
//
#include "timedplayer.h"
#include "umc_speech_data.h"
#include "umc_rtp_packet.h"
#include "umc_speech_jbuffer.h"
#include "umc_rta_jbuffer.h"

#include "vm_time.h"

#define MAX_UDP_PCKT_LEN 32768
#define PACKED_PRMS_SIZE 12

UMC::Status TimedPlayer::Init(TimedPlayerExternalParams *pParams)
{
   Close();

   m_pParams = pParams;

   m_pOut = new UMC::SpeechData(6144*2);
   if(NULL == m_pOut) {
      vm_string_printf(VM_STRING("Can't allocate memory\n"));
      return UMC::UMC_ERR_ALLOC;
   }

   m_pRTPPck = new UMC::RTP_Packet(MAX_UDP_PCKT_LEN);

   if(NULL == m_pRTPPck) {
      vm_string_printf(VM_STRING("Can't allocate memory\n"));
      return UMC::UMC_ERR_ALLOC;
   }
   return UMC::UMC_OK;
}

UMC::Status TimedPlayer::Close()
{
   m_pParams = NULL;
   if(m_pOut) {
      delete m_pOut;
      m_pOut = NULL;
   }

   if(m_pRTPPck) {
      delete m_pRTPPck;
      m_pRTPPck = NULL;
   }

   return UMC::UMC_OK;
}

UMC::Status TimedPlayer::Play(Ipp64f &t_encode_time, Ipp32s &numFramesToProcess)
{
   UMC::Status sts = UMC::UMC_OK;
   Ipp32u isFirstPacketFlag = 1;
   UMC::MediaData inMediaBuffer;
   UMC::JPacketMediaData pJitterPckt(MAX_UDP_PCKT_LEN + PACKED_PRMS_SIZE);
   UMC::SpeechData pDeJitterPckt(MAX_UDP_PCKT_LEN);

   UMC::JBufferParams jPrms;
   int frmSizeMS = (m_pParams->m_pAudioCodecParams->m_framesize / ((m_pParams->m_pAudioCodecParams->m_info_out.bitPerSample >> 3) * (m_pParams->m_pAudioCodecParams->m_info_out.sample_frequency/1000)));

   vm_tick t_freq, t_end;
   t_encode_time = 0.;
   numFramesToProcess = 0;

   Ipp32s size = 0, i;

   jPrms.m_MSSizePacket = frmSizeMS;
   jPrms.m_numberOfFrames = 1000;
   jPrms.m_numSizeJitter = 3;
   jPrms.m_numThresholdDiscard = 100;
   jPrms.m_prefInputBufferSize = m_pParams->m_pAudioCodecParams->m_maxbitsize;
   jPrms.m_prefOutputBufferSize = m_pParams->m_pAudioCodecParams->m_framesize;
   jPrms.m_SampleRate = m_pParams->m_pAudioCodecParams->m_info_out.sample_frequency;
   jPrms.m_pDecoder = m_pParams->m_pDSCodec;

   if(NULL == m_pParams->m_pJitterBuffer) return UMC::UMC_ERR_NULL_PTR;

   m_pParams->m_pJitterBuffer->Init(&jPrms);
   Ipp32u timpStampOffset = 0;

   /*Start of the processing*/
   t_freq = vm_time_get_frequency();
   t_end = 0;

   UMC::LinkedList<UMC::SpeechData *> FrameList;

   do {
      sts = m_pParams->m_pMediaBuffer->LockInputBuffer(&inMediaBuffer);

      if (sts == UMC::UMC_OK) {
         size = m_pParams->m_p_in_file->Read(inMediaBuffer.GetDataPointer(),inMediaBuffer.GetBufferSize());
         inMediaBuffer.SetDataSize(size);
         m_pParams->m_pMediaBuffer->UnLockInputBuffer(&inMediaBuffer);
         if(size==0) break;
      }

      do {
            Ipp32s pcktSize;
            sts = m_pParams->m_pMediaBuffer->LockOutputBuffer(&inMediaBuffer);

            if (sts == UMC::UMC_OK) {
//               vm_timeval nowTime;
//               vm_timeval nextPtime;
//               vm_timeval periodSleep;
               /*Load RTP packet with payload*/
               UMC::RTPDumpFile::RTPDumpFilePacketHeader *pckHeader;
               UMC::MediaData rtpPayload;
               pckHeader = (UMC::RTPDumpFile::RTPDumpFilePacketHeader *)inMediaBuffer.GetDataPointer();

               pckHeader->WholeLength = LITTLE_ENDIAN_SWAP16(pckHeader->WholeLength);
               pckHeader->pckLen = LITTLE_ENDIAN_SWAP16(pckHeader->pckLen);
               pckHeader->RecordOffsetMS = LITTLE_ENDIAN_SWAP32(pckHeader->RecordOffsetMS);

               m_pParams->m_pTimer->SetNextTime(pckHeader->RecordOffsetMS);

               pcktSize = pckHeader->WholeLength;

               if(m_pParams->m_pTimer->isTime()) {

                  inMediaBuffer.MoveDataPointer(sizeof(UMC::RTPDumpFile::RTPDumpFilePacketHeader));

                  m_pRTPPck->SetBufferPointer((Ipp8u*)inMediaBuffer.GetDataPointer(),pcktSize);

                  if(m_pRTPPck->isValidPacket() && m_pRTPPck->GetPayloadType() == m_pParams->m_pPT->GetPayloadType()) {
                     /*Get RTP payload from packet*/
                     if(isFirstPacketFlag) {
                        timpStampOffset = m_pRTPPck->GetTimeStamp();
                        isFirstPacketFlag = 0;
                     }
                     rtpPayload.SetBufferPointer((Ipp8u*)m_pRTPPck->GetPayloadDataPointer(),m_pRTPPck->GetPayloadDataLen());
                     rtpPayload.SetDataSize(m_pRTPPck->GetPayloadDataLen());
                     //printf("seg=%d, time=%d \n",m_pRTPPck->GetSequenceNumber(),m_pRTPPck->GetTimeStamp());
                     rtpPayload.SetTime(m_pRTPPck->GetTimeStamp()-timpStampOffset);

                     /*Unpack to the USC bitstream format*/
                     m_pParams->m_pRepacker->UnPackToUSC(&rtpPayload,&FrameList);

                     /*Decode*/
                     for(i=0;i<(Ipp32s)FrameList.Size();i++) {
                        UMC::SpeechData *pIt;
                        FrameList.Get(pIt,i);
                        m_pParams->m_pJitterBuffer->LockInputBuffer(pIt);
                        m_pParams->m_pJitterBuffer->UnLockInputBuffer(pIt);
                     }
                  }

                  while(FrameList.Size() > 0) {
                     UMC::SpeechData *pIt;
                     FrameList.Last(pIt);
                     delete pIt;
                     FrameList.Remove();
                  }

                  inMediaBuffer.MoveDataPointer(pcktSize);
               }
            }

            m_pParams->m_pMediaBuffer->UnLockOutputBuffer(&inMediaBuffer);
            UMC::Status status_jb = UMC::UMC_OK;
            status_jb =  m_pParams->m_pJitterBuffer->LockOutputBuffer(m_pOut);
            if (m_pOut->GetDataSize()) {
            //if (sts == UMC::UMC_OK && status_jb == UMC::UMC_OK) {
               /*Store decoded samples*/
               m_pParams->m_p_out_file->Write(m_pOut->GetDataPointer(), m_pOut->GetDataSize());
               m_pOut->Reset();

               numFramesToProcess++;
            }
            m_pParams->m_pTimer->Continue();
      } while (sts == UMC::UMC_OK);
   } while (size >= m_pParams->m_pAudioCodecParams->m_framesize);

   UMC::Status status_jb = UMC::UMC_OK;
   m_pParams->m_pJitterBuffer->UnLockInputBuffer(NULL,UMC::UMC_ERR_END_OF_STREAM);
   while(status_jb != UMC::UMC_ERR_END_OF_STREAM) {
      status_jb =  m_pParams->m_pJitterBuffer->LockOutputBuffer(m_pOut);
      if (sts == UMC::UMC_OK && status_jb == UMC::UMC_OK) {
         /*Store decoded samples*/
         m_pParams->m_p_out_file->Write(m_pOut->GetDataPointer(), m_pOut->GetDataSize());
         m_pOut->Reset();

         numFramesToProcess++;
      }
   }

   t_encode_time  = (Ipp64f)((Ipp64s)(t_end))/(Ipp64s)t_freq;
   return UMC::UMC_OK;
}

UMC::Status TimedPlayer::Record(Ipp64f &t_encode_time, Ipp32s &numFramesToProcess)
{
   vm_tick t_freq, t_start, t_end;

   UMC::Status sts = UMC::UMC_OK;
   UMC::MediaData inMediaBuffer;

   Ipp32s size = 0, i;

   t_encode_time = 0.;
   numFramesToProcess = 0;

   /*Start of the processing*/
   t_freq = vm_time_get_frequency();
   t_end = 0;

   Ipp32u isFirstPacketFlag = 1;
   Ipp32u dynPayloadType = m_pParams->m_pPT->GetPayloadType();
   Ipp32u dynPayloadClockRate = m_pParams->m_pPT->GetClockRate();
   Ipp16u SequenceNumber = 0;
   Ipp32u SyncSource = 11111l;
   Ipp32u TimeStamp = vm_time_get_tick();
   UMC::LinkedList<UMC::SpeechData *> FrameList;

   do {
      sts = m_pParams->m_pMediaBuffer->LockInputBuffer(&inMediaBuffer);

      if (sts == UMC::UMC_OK) {
         size = m_pParams->m_p_in_file->Read(inMediaBuffer.GetDataPointer(),inMediaBuffer.GetBufferSize());
         inMediaBuffer.SetDataSize(size);
         m_pParams->m_pMediaBuffer->UnLockInputBuffer(&inMediaBuffer);
         if(size==0) break;
      }

      do {
            sts = m_pParams->m_pMediaBuffer->LockOutputBuffer(&inMediaBuffer);

            /* Load and encode samples */
            for(i=0;i<m_pParams->m_nFrmInPkcts;i++) {
               if(sts == UMC::UMC_OK) {
                  size_t preencsize = inMediaBuffer.GetDataSize();
                  UMC::SpeechData *newData = new UMC::SpeechData(m_pParams->m_pAudioCodecParams->m_framesize);
                  t_start = vm_time_get_tick();
                  sts = m_pParams->m_pDSCodec->GetFrame(&inMediaBuffer,newData);
                  t_end += (vm_time_get_tick()-t_start);
                  FrameList.Add(newData);
                  size_t encsize = inMediaBuffer.GetDataSize();
                  inMediaBuffer.MoveDataPointer((Ipp32s)(preencsize-encsize));
                  inMediaBuffer.SetDataSize(encsize);
               }
            }

            if (sts == UMC::UMC_OK) {
               /*Get bitstream and form RTP packet with payload*/
               m_pParams->m_pMediaBuffer->UnLockOutputBuffer(&inMediaBuffer);

               m_pParams->m_pRepacker->PackToRTP(&FrameList,m_pOut);

               m_pRTPPck->SetPayloadData((Ipp8u*)m_pOut->GetDataPointer(),m_pOut->GetDataSize());

               m_pRTPPck->SetVersion(RTP_VERSION);
               m_pRTPPck->SetPadding(0);
               m_pRTPPck->SetCSRCCount(0);
               if(isFirstPacketFlag) {
                  m_pRTPPck->SetMarked(1);
                  isFirstPacketFlag = 0;
               } else {
                  m_pRTPPck->SetMarked(0);
               }
               m_pRTPPck->SetExtension(0);
               m_pRTPPck->SetPayloadType(dynPayloadType);
               m_pRTPPck->SetSequenceNumber(SequenceNumber);
               SequenceNumber++;
               m_pRTPPck->SetTimeStamp(TimeStamp);
               TimeStamp = TimeStamp + ((m_pParams->m_nFrmInPkcts*m_pParams->m_pAudioCodecParams->m_framesize) / sizeof(Ipp16s));
               m_pRTPPck->SetSyncSource(SyncSource);
            }

            while(FrameList.Size() > 0) {
               UMC::SpeechData *pIt;
               FrameList.Last(pIt);
               delete pIt;
               FrameList.Remove();
            }

            /*Store RTP packet with payload*/
            if (sts == UMC::UMC_OK) {
               UMC::RTPDumpFile::RTPDumpFilePacketHeader pckHeader;
               Ipp16u pckSize = (Ipp16u)m_pRTPPck->GetDataSize();

               pckHeader.WholeLength = LITTLE_ENDIAN_SWAP16(pckSize);
               pckHeader.pckLen = LITTLE_ENDIAN_SWAP16(pckSize+sizeof(UMC::RTPDumpFile::RTPDumpFilePacketHeader));
               Ipp32u msOffset = (m_pRTPPck->GetTimeStamp() * 1000 )/ m_pParams->m_pAudioCodecParams->m_info_in.sample_frequency;
               pckHeader.RecordOffsetMS = LITTLE_ENDIAN_SWAP32(msOffset);
               m_pParams->m_p_out_file->Write(&pckHeader, sizeof(UMC::RTPDumpFile::RTPDumpFilePacketHeader));
               m_pParams->m_p_out_file->Write(m_pRTPPck->GetDataPointer(), m_pRTPPck->GetDataSize());

               numFramesToProcess+=m_pParams->m_nFrmInPkcts;
            }
      } while (sts == UMC::UMC_OK);
   } while (size >= m_pParams->m_pAudioCodecParams->m_framesize);

   t_encode_time  = (Ipp64f)((Ipp64s)(t_end))/(Ipp64s)t_freq;
   return UMC::UMC_OK;
}

