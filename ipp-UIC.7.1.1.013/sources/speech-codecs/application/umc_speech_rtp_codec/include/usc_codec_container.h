/*
////////////////////////////////////////////////////////////////////////
//
// INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
// Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//   Intel(R)  Integrated Performance Primitives
//
// By downloading and installing this sample, you hereby agree that the
// accompanying Materials are being provided to you under the terms and
// conditions of the End User License Agreement for the Intel(R) Integrated
// Performance Primitives product previously accepted by you. Please refer
// to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
// product installation for more information.
//
// Purpose: Speech codec and format library.
*/

#ifndef __USC_CODEC_CONTAINER_H__
#define __USC_CODEC_CONTAINER_H__

#include "vm_types.h"
#include "umc_speech_codec.h"
#include "base_rtp_cnvrt.h"
#include "rtppayload.h"
#include "timer.h"

namespace UMC {
class MediaBuffer;
}

#define USC_MAX_OBJECT_NAME 64
class USC_CodecContainer
{
public:
   USC_CodecContainer()
   {
      m_Inited = 0;
   }
   ~USC_CodecContainer()
   {
      m_Inited = 0;
   }

   //Iterations methods
   //  Get number of the linked codecs
   Ipp32s GetNumCodecs();
   //  Get ith codec name
   vm_char *GetCodecName(Ipp32s index);

   //  Retrieve codec depended parameters
   void SetCodecName(const vm_char *nameCodec);
   //  Check codec presence by name
   void *TryToFindStatic(const vm_char *nameCodec);
   //  Create USC codec by name
   UMC::AudioCodec* CreateUSCCodec(Ipp32s direction);
   //  Create RTP packetizer and Depacketizer
   UMC::Status CreateRTPRePacketizer(Ipp32s direction, Ipp32s bitrate, Ipp32s btstrmFmt, UMC::RTPBasePacketizer** pPck,UMC::RTPBaseDepacketizer** pDePck);
   UMC::MediaBuffer* CreateJBuffer(Ipp32s direction);
   BaseTimer* CreatePlayoutTimer(Ipp32s direction);
   //  Get RTP payload type
   UMC::RTP_PayloadType * GetPayloadType();
protected:
   vm_char *m_pCodecName[USC_MAX_OBJECT_NAME];
   int m_Inited;
};

#endif /* __USC_CODEC_CONTAINER_H__*/
