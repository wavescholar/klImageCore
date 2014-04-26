/*
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//       Copyright (c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MEMORY_ALLOCATOR_H__
#define __UMC_MEMORY_ALLOCATOR_H__

#include "ippdefs.h"
#include "umc_structures.h"
#include "umc_dynamic_cast.h"
#include "umc_mutex.h"

#define MID_INVALID 0

namespace UMC
{

typedef Ipp32u MemID;

enum UMC_ALLOC_FLAGS
{
    UMC_ALLOC_PERSISTENT  = 0x01,
    UMC_ALLOC_FUNCLOCAL   = 0x02
};

class MemoryAllocatorParams
{
public:
    DYNAMIC_CAST_DECL_BASE(MemoryAllocatorParams)

    MemoryAllocatorParams(void) {}
    virtual ~MemoryAllocatorParams(void) {}
};

class MemoryAllocator
{
public:
    DYNAMIC_CAST_DECL_BASE(MemoryAllocator)

    MemoryAllocator(void) {}
    virtual ~MemoryAllocator(void) {}

    // Initiates object
    virtual Status Init(MemoryAllocatorParams* pParams) = 0;

    // Closes object and releases all allocated memory
    virtual Status Close(void) = 0;

    // Allocates or reserves physical memory and returns unique ID
    // Sets lock counter to 0
    virtual Status Alloc(MemID* pNewMemID, size_t Size, Ipp32u Flags, Ipp32u Align = 16) = 0;

    // Lock() provides pointer from ID. If data is not in memory (swapped)
    // prepares (restores) it. Increases lock counter
    virtual void* Lock(MemID MID) = 0;

    // Unlock() decreases lock counter
    virtual Status Unlock(MemID MID) = 0;

    // Notifies that the data won't be used anymore. Memory can be freed.
    virtual Status Free(MemID MID) = 0;

    // Immediately deallocates memory regardless of whether it is in use (locked) or no
    virtual Status DeallocateMem(MemID MID) = 0;

protected:
    Mutex m_guard;
};

} //namespace UMC

#endif //__UMC_MEMORY_ALLOCATOR_H__
