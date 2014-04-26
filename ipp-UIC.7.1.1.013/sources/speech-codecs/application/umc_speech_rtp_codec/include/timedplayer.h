//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2012 Intel Corporation. All Rights Reserved.
//
//
//
#ifndef __TIMED_PLAYER_H__
#define __TIMED_PLAYER_H__

#include "umc_structures.h"
#include "umc_media_buffer.h"
#include "umc_linked_list.h"
#include "umc_speech_codec.h"
#include "rtpdump_file.h"
#include "repacketizer.h"
#include "umc_rtp_packet.h"
#include "rtppayload.h"
#include "umc_dual_thread_codec.h"
#include "timer.h"

class TimedPlayerExternalParams {
public:
   UMC::SpeechCodecParams *m_pAudioCodecParams;
   UMC::MediaBuffer *m_pMediaBuffer;
   UMC::AudioFile* m_p_in_file;
   UMC::AudioFile* m_p_out_file;
   Repacker *m_pRepacker;
   UMC::DualThreadedCodec *m_pDSCodec;
   UMC::RTP_PayloadType *m_pPT;
   Ipp32s m_nFrmInPkcts;
   UMC::MediaBuffer *m_pJitterBuffer;
   BaseTimer *m_pTimer;
};

class TimedPlayer {
public:
   TimedPlayer(){m_pParams=NULL;m_pOut=NULL;m_pRTPPck=NULL;}
   ~TimedPlayer(){Close();}

   UMC::Status Init(TimedPlayerExternalParams *pParams);
   UMC::Status Close();
   UMC::Status Play(Ipp64f &t_encode_time, Ipp32s &numFramesToProcess);
   UMC::Status Record(Ipp64f &t_encode_time, Ipp32s &numFramesToProcess);
protected:
   TimedPlayerExternalParams *m_pParams;
   UMC::SpeechData *m_pOut;
   UMC::RTP_Packet *m_pRTPPck;
};

#endif//__TIMED_PLAYER_H__
