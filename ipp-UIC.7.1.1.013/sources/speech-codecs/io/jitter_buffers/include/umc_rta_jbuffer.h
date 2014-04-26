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

#ifndef __UMC_RTA_JBUFFER_H__
#define __UMC_RTA_JBUFFER_H__

#include "umc_base_jbuffer.h"

namespace UMC
{

class RTAJBuffer : public BaseJBuffer
{
    DYNAMIC_CAST_DECL(RTAJBuffer, BaseJBuffer)

public:
    // Default constructor
    RTAJBuffer(void);
    // Destructor
    virtual
    ~RTAJBuffer(void);

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
   friend void * rtaMalloc(size_t lSize);
   friend void rtaFree( void *pvMemory );
   static void * extMalloc(size_t lSize);
   static void extFree( void *pvMemory );

   JBufferParams *m_pParams;
   Ipp32u m_FrameSize;

   void *m_hDJStatus;
   void *m_decHandle;
   int m_EOS;
};

} // namespace UMC

#endif // __UMC_RTA_JBUFFER_H__
