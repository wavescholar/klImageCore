//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2012 Intel Corporation. All Rights Reserved.
//
//
//
#ifndef __REPACKETIZER_H__
#define __REPACKETIZER_H__

#include "umc_linked_list.h"
#include "umc_speech_data.h"
#include "base_rtp_cnvrt.h"

#define MAX_UDP_PCKT_LEN 32768

class Repacker {
public:
   Repacker(UMC::RTPBasePacketizer *pPacketizer, UMC::RTPBaseDepacketizer *pDepacketizer)
   {
      m_pPacketizer = pPacketizer;
      m_pDepacketizer = pDepacketizer;
   }
   /*Convert bitstream to the RTP payload*/
   UMC::Status PackToRTP(UMC::LinkedList<UMC::SpeechData *> *pIn, UMC::MediaData *pOut);
   /*Convert RTP payload to th USC supported bitstream*/
   UMC::Status UnPackToUSC(UMC::MediaData *pIn, UMC::LinkedList<UMC::SpeechData *> *pOut);
protected:
   UMC::RTPBasePacketizer *m_pPacketizer;
   UMC::RTPBaseDepacketizer *m_pDepacketizer;
};

#endif//__REPACKETIZER_H__
