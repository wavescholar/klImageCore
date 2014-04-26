/*
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//       Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_DEFAULT_FRAME_ALLOCATOR_H__
#define __UMC_DEFAULT_FRAME_ALLOCATOR_H__

#include "umc_frame_allocator.h"
#include "umc_default_memory_allocator.h"

#include <vector>

namespace UMC
{

class FrameInformation;

class DefaultFrameAllocator : public FrameAllocator
{
public:
    DYNAMIC_CAST_DECL(DefaultFrameAllocator, FrameAllocator)

    DefaultFrameAllocator(void);
    virtual ~DefaultFrameAllocator(void);

    // Initiates object
    virtual Status Init(FrameAllocatorParams *pParams);

    // Closes object and releases all allocated memory
    virtual Status Close();

    virtual Status Reset();

    // Allocates or reserves physical memory and returns unique ID
    // Sets lock counter to 0
    virtual Status Alloc(FrameMemID *pNewMemID, const VideoData* info, Ipp32u flags);

    // Lock() provides pointer from ID. If data is not in memory (swapped)
    // prepares (restores) it. Increases lock counter
    virtual const FrameData* Lock(FrameMemID mid);

    // Unlock() decreases lock counter
    virtual Status Unlock(FrameMemID mid);

    // Notifies that the data won't be used anymore. Memory can be freed.
    virtual Status IncreaseReference(FrameMemID mid);

    // Notifies that the data won't be used anymore. Memory can be freed.
    virtual Status DecreaseReference(FrameMemID mid);

protected:
    Status Free(FrameMemID mid);
    std::vector<FrameInformation*> m_frames;
};

} // namespace UMC

#endif // __UMC_DEFAULT_FRAME_ALLOCATOR_H__
