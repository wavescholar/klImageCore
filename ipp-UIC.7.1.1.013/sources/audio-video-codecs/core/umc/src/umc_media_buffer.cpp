/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_media_buffer.h"
#include "umc_default_memory_allocator.h"

using namespace UMC;


MediaBuffer::MediaBuffer(void)
{
    m_pMemoryAllocator = 0;
    m_pAllocated = 0;

}

MediaBuffer::~MediaBuffer(void)
{
    Close();
}

Status MediaBuffer::Init(MediaReceiverParams *pInit)
{
    MediaBufferParams *pParams = DynamicCast<MediaBufferParams, MediaReceiverParams>(pInit);

    // check error(s)
    if (NULL == pParams)
        return UMC_ERR_NULL_PTR;

    // release the object before initialization
    MediaBuffer::Close();

    // use the external memory allocator
    if (pParams->m_pMemoryAllocator)
    {
        m_pMemoryAllocator = pParams->m_pMemoryAllocator;
    }
    // allocate default memory allocator
    else
    {
        m_pAllocated = new DefaultMemoryAllocator();
        if (NULL == m_pAllocated)
            return UMC_ERR_ALLOC;

        m_pMemoryAllocator = m_pAllocated;
    }

    return UMC_OK;
}

Status MediaBuffer::Close(void)
{
    if (m_pAllocated)
        delete m_pAllocated;

    m_pMemoryAllocator = NULL;
    m_pAllocated = NULL;

    return UMC_OK;
}
