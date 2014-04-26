/*
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//       Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_DEFAULT_MEMORY_ALLOCATOR_H__
#define __UMC_DEFAULT_MEMORY_ALLOCATOR_H__

#include "umc_memory_allocator.h"

namespace UMC
{

struct MemoryInfo;

class DefaultMemoryAllocator : public MemoryAllocator
{
public:
    DYNAMIC_CAST_DECL(DefaultMemoryAllocator, MemoryAllocator)

    DefaultMemoryAllocator(void);
    virtual ~DefaultMemoryAllocator(void);

    // Initiates object
    virtual Status Init(MemoryAllocatorParams* pParams);

    // Closes object and releases all allocated memory
    virtual Status Close(void);

    // Allocates or reserves physical memory and return unique ID
    // Sets lock counter to 0
    virtual Status Alloc(MemID* pNewMemID, size_t Size, Ipp32u Flags, Ipp32u Align = 16);

    // Lock() provides pointer from ID. If data is not in memory (swapped)
    // prepares (restores) it. Increases lock counter
    virtual void* Lock(MemID MID);

    // Unlock() decreases lock counter
    virtual Status Unlock(MemID MID);

    // Notifies that the data won't be used anymore. Memory can be freed.
    virtual Status Free(MemID MID);

    // Immediately deallocates memory regardless of whether it is in use (locked) or no
    virtual Status DeallocateMem(MemID MID);

protected:
    MemoryInfo* m_memInfo;  // memory blocks descriptors
    Ipp32s      m_memCount; // number of allocated descriptors
    Ipp32s      m_memUsed;  // number of used descriptors
    MemID       m_lastMID;  // last value assigned to descriptor
};

} // namespace UMC

#endif // __UMC_DEFAULT_MEMORY_ALLOCATOR_H
