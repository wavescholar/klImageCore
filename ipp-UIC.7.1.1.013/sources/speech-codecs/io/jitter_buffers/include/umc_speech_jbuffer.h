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

#ifndef __UMC_SPEECH_JBUFFER_H__
#define __UMC_SPEECH_JBUFFER_H__

#include "umc_base_jbuffer.h"
#include "jitterbuffer.h"

namespace UMC
{

class SpeechJBuffer : public BaseJBuffer
{
    DYNAMIC_CAST_DECL(SpeechJBuffer, BaseJBuffer)

public:
    // Default constructor
    SpeechJBuffer(void);
    // Destructor
    virtual
    ~SpeechJBuffer(void);

    // Initialize buffer
    virtual
    Status Init(MediaReceiverParams* init);

    // Lock input buffer
    virtual
    Status LockInputBuffer(MediaData* in);
    // Unlock input buffer
    virtual
    Status UnLockInputBuffer(MediaData* in, Status StreamStatus = UMC_OK);

    // Lock output buffer
    virtual
    Status LockOutputBuffer(MediaData* out);
    // Unlock output buffer
    virtual
    Status UnLockOutputBuffer(MediaData* out);

    // Stop buffer
    virtual
    Status Stop(void);

    // Release object
    virtual
    Status Close(void);

    // Reset object
    virtual
    Status Reset(void);

protected:
   void ownCnvtToJPacket(SpeechData *pIn, JPacketMediaData *pOut);
   void ownCnvtToSpeechData(JPacketMediaData *pIn, SpeechData *pOut);

   JitterBuffer *m_pJBuffer;
   JBufferParams *m_pParams;
   JPacketMediaData *m_pJpIn;
   Ipp32u m_PrevPut;
   Ipp32s m_TimeStampInc;
   Ipp32s m_EOSFlag;
};

} // namespace UMC

#endif // __UMC_SPEECH_JBUFFER_H__

