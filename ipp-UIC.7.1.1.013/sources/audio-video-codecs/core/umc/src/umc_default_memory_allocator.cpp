/*
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//       Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_default_memory_allocator.h"
#include "umc_automatic_mutex.h"

#include "ippcore.h"

namespace UMC
{
// structure to describe one memory block
struct MemoryInfo
{
    void*  pMemory;          // allocated memory block
    size_t Size;            // allocated size
    MemID  MID;             // MID_INVALID if unused
    Ipp32s Alignment;       // requested alignment
    Ipp32s LocksCount;      // lock counter
    Ipp32s InvalidatedFlag; // set after Free()

    void Init(void)
    {
        pMemory = 0;
        Size    = 0;
        MID     = MID_INVALID;
    }

    // Released descriptor with not released memory of suitable size
    Ipp32s CanReuse(size_t s_Size, Ipp32s s_Alignment)
    {
        if((MID == MID_INVALID) && (pMemory != 0) && ((s_Size + (align_pointer<Ipp8u*>(pMemory, s_Alignment) - (Ipp8u*)pMemory)) <= Size))
            return 1;

        return 0;
    }

    // assign memory to descriptor
    Status Alloc(size_t s_Size, Ipp32s s_Alignment)
    {
        if(!CanReuse(s_Size, s_Alignment))
        {
            // can't reuse
            pMemory = ippMalloc((Ipp32s)s_Size+s_Alignment);
            if(pMemory == 0)
                return UMC_ERR_ALLOC;

            Size = s_Size+s_Alignment; // align to be done on Lock() call
        }
        Alignment       = s_Alignment;
        LocksCount      = 0;
        InvalidatedFlag = 0;

        return UMC_OK;
    }

    // mark as no more used checking the state
    void Clear(void)
    {
        if(pMemory != 0)
        {
            if(InvalidatedFlag == 0)
                InvalidatedFlag = 1;

            if(LocksCount != 0)
                LocksCount = 0;

            MID = MID_INVALID;
        }
    }

    // release memory allocated to descriptor
    void Release(void)
    {
        if(pMemory != 0)
        {
            Clear();
            ippFree(pMemory);
            pMemory = 0;
        }
    }
};
}

using namespace UMC;


DefaultMemoryAllocator::DefaultMemoryAllocator(void)
{
    m_memInfo  = 0;
    m_memCount = 0; // allocate only on call
    m_memUsed  = 0;
    m_lastMID  = MID_INVALID;

    Init(NULL);
}

DefaultMemoryAllocator::~DefaultMemoryAllocator(void)
{
    DefaultMemoryAllocator::Close();
}

Status DefaultMemoryAllocator::Init(MemoryAllocatorParams* /*pParams*/)
{
    AutomaticMutex guard(m_guard);

    DefaultMemoryAllocator::Close();

    return UMC_OK;
}


Status DefaultMemoryAllocator::Close(void)
{
    AutomaticMutex guard(m_guard);

    Ipp32s i;

    for(i = 0; i < m_memUsed; i++)
    {
        m_memInfo[i].Release();
    }

    if(m_memInfo != 0)
    {
        ippFree(m_memInfo);
        m_memInfo = 0;
    }

    m_memUsed = 0;
    m_memCount = 0;

    return UMC_OK;
}


Status DefaultMemoryAllocator::Alloc(MemID* pNewMemID, size_t Size, Ipp32u /*Flags*/, Ipp32u Align/*=16*/)
{
    MemoryInfo* pmem = 0;
    MemoryInfo* pmemtofree = 0;
    Ipp32s i;

    AutomaticMutex guard(m_guard);

    if(pNewMemID == NULL)
        return UMC_ERR_NULL_PTR;

    if (Size == 0 || Align == 0)
        return UMC_ERR_INVALID_PARAMS;

    *pNewMemID = MID_INVALID;

    for (i = 1; i <= (1 << 20); i <<= 1)
    {
        if (i & Align)
            break; // stop at nonzero bit
    }

    if (i != (Ipp32s)Align) // no 1 in 20 ls bits or more than 1 nonzero bit
        return UMC_ERR_INVALID_PARAMS;

    for (i = 0; i < m_memUsed; i++)
    {
        // search unused or free
        if (m_memInfo[i].pMemory == 0 || m_memInfo[i].CanReuse(Size, Align))
        {
            pmem = &m_memInfo[i];
            break;
        }
        else if(m_memInfo[i].MID == MID_INVALID)
            pmemtofree = &m_memInfo[i];
    }

    if (pmem == 0 && m_memUsed < m_memCount)
    {
        // take from never used
        pmem = &m_memInfo[m_memUsed];
        m_memUsed++;
    }

    if(pmem == 0 && pmemtofree != 0)
    {
        // release last unsuitable
        pmemtofree->Release();
        pmem = pmemtofree;
    }

    if (pmem == 0)
    {
        // relocate all descriptors
        Ipp32s newcount = IPP_MAX(8, m_memCount*2);
        MemoryInfo* newmem = (MemoryInfo*)ippMalloc((int)(newcount*sizeof(MemoryInfo)));
        if (newmem == 0)
            return UMC_ERR_ALLOC;

        for (i = 0; i < m_memCount; i++) // copy existing
            newmem[i] = m_memInfo[i];

        // free old descriptors
        if (m_memInfo)
            ippFree(m_memInfo);

        m_memInfo = newmem;
        m_memCount = newcount;

        for (; i < m_memCount; i++)
            m_memInfo[i].Init(); // init new

        pmem = &m_memInfo[m_memUsed]; // take first in new
        m_memUsed++;
    }

    if(UMC_OK != pmem->Alloc(Size, Align))
        return UMC_ERR_ALLOC;

    m_lastMID++;

    pmem->MID  = m_lastMID;
    *pNewMemID = m_lastMID;

    return UMC_OK;
}

void* DefaultMemoryAllocator::Lock(MemID MID)
{
    Ipp32s i;

    if( MID == MID_INVALID )
        return NULL;

    AutomaticMutex guard(m_guard);

    for (i = 0; i < m_memUsed; i++)
    {
        if (m_memInfo[i].MID == MID)
        {
            if(m_memInfo[i].pMemory == 0 || m_memInfo[i].InvalidatedFlag)
                return NULL; // no memory or invalidated
            m_memInfo[i].LocksCount++;
            // return with aligning
            return align_pointer<Ipp8u*>(m_memInfo[i].pMemory, m_memInfo[i].Alignment);
        }
    }

    return NULL;
}


Status DefaultMemoryAllocator::Unlock(MemID MID)
{
    Ipp32s i;

    if( MID == MID_INVALID )
        return UMC_ERR_FAILED;

    AutomaticMutex guard(m_guard);

    for (i = 0; i < m_memUsed; i++)
    {
        if (m_memInfo[i].MID == MID)
        {
            if(m_memInfo[i].pMemory == 0 || m_memInfo[i].LocksCount <= 0)
                return UMC_ERR_FAILED; // no mem or lock /unlock mismatch

            m_memInfo[i].LocksCount--;
            if(m_memInfo[i].LocksCount == 0 && m_memInfo[i].InvalidatedFlag)
                m_memInfo[i].Clear(); //  no more use

            return UMC_OK;
        }
    }

    return UMC_ERR_FAILED;
}


Status DefaultMemoryAllocator::Free(MemID MID)
{
    Ipp32s i;

    if( MID == MID_INVALID )
        return UMC_ERR_FAILED;

    AutomaticMutex guard(m_guard);

    for (i = 0; i < m_memUsed; i++)
    {
        if (m_memInfo[i].MID == MID)
        {
            if(m_memInfo[i].pMemory == 0 || m_memInfo[i].InvalidatedFlag != 0)
                return UMC_ERR_FAILED; // no mem or re-free

            m_memInfo[i].InvalidatedFlag = 1;
            if(m_memInfo[i].LocksCount == 0)
                m_memInfo[i].Clear(); // not in use

            return UMC_OK;
        }
    }

    return UMC_ERR_FAILED;
}


Status DefaultMemoryAllocator::DeallocateMem(MemID MID)
{
    Ipp32s i;

    if( MID == MID_INVALID )
        return UMC_ERR_FAILED;

    AutomaticMutex guard(m_guard);

    for (i = 0; i < m_memUsed; i++)
    {
        if (m_memInfo[i].MID == MID)
        {
            if(m_memInfo[i].pMemory == 0)
                return UMC_ERR_FAILED; // no memory

            m_memInfo[i].InvalidatedFlag = 1;
            m_memInfo[i].Clear();

            return UMC_OK;
        }
    }

    return UMC_OK;
}
