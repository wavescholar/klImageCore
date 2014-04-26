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

#ifndef __UMC_BASE_JBUFFER_H__
#define __UMC_BASE_JBUFFER_H__

#include "umc_base_codec.h"
#include "umc_media_buffer.h"

namespace UMC
{

   class JBufferParams : public MediaBufferParams
   {
       DYNAMIC_CAST_DECL(JBufferParams, MediaBufferParams)

   public:
       // Default constructor
       JBufferParams(void) : MediaBufferParams() {m_MSSizePacket = 0;m_numSizeJitter = 0;m_numThresholdDiscard = 0;m_pDecoder=NULL;m_SampleRate=0;}
       Ipp32s m_MSSizePacket;
       Ipp32s m_numSizeJitter;
       Ipp32s m_numThresholdDiscard;
       Ipp32s m_SampleRate;
       BaseCodec *m_pDecoder;
   };

   class BaseJBuffer : public MediaBuffer
   {
       DYNAMIC_CAST_DECL(BaseJBuffer, MediaBuffer)

   public:
       // Default constructor
      BaseJBuffer(void) {}
       // Destructor
      virtual ~BaseJBuffer(void){}
   };

}
#endif // __UMC_BASE_JBUFFER_H__
