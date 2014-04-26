/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MEDIA_RECEIVER_H__
#define __UMC_MEDIA_RECEIVER_H__

#include "umc_structures.h"
#include "umc_media_data.h"

namespace UMC
{

class  MediaReceiverParams
{
public:
    DYNAMIC_CAST_DECL_BASE(MediaReceiverParams)

    MediaReceiverParams(void)
    {
        m_iFlags = 0;
    }
    virtual ~MediaReceiverParams(void) {}

    Ipp32u m_iFlags;
};

// Base class for renderers and buffers
class MediaReceiver
{
public:
    DYNAMIC_CAST_DECL_BASE(MediaReceiver)

    MediaReceiver(void) {}
    virtual ~MediaReceiver(void) {}

    // Initialize media receiver
    virtual Status Init(MediaReceiverParams* init) = 0;

    // Release all receiver resources
    virtual Status Close(void) { return UMC_OK; }

    // Lock input buffer
    virtual Status LockInputBuffer(MediaData* in) = 0;
    // Unlock input buffer
    virtual Status UnLockInputBuffer(MediaData* in, Status StreamStatus = UMC_OK) = 0;

    // Break waiting(s)
    virtual Status Stop(void) = 0;

    // Reset media receiver
    virtual Status Reset(void) {return UMC_OK;}
};

} // end namespace UMC

#endif // __UMC_MEDIA_RECEIVER_H__
