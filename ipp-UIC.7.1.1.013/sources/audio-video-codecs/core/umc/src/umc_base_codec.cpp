/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_base_codec.h"
#include "umc_default_memory_allocator.h"

using namespace UMC;


BaseCodec::BaseCodec(void)
{
    m_pMemoryAllocator = 0;
    m_bOwnAllocator    = false;
}

BaseCodec::~BaseCodec(void)
{
    BaseCodec::Close();
}

Status BaseCodec::Init(BaseCodecParams *pParams)
{
    if(pParams == 0)
        return UMC_ERR_NULL_PTR;

    if(pParams->m_lpMemoryAllocator)
    {
        if(m_bOwnAllocator && m_pMemoryAllocator != pParams->m_lpMemoryAllocator)
            return UMC_ERR_INIT;

        m_pMemoryAllocator = pParams->m_lpMemoryAllocator;
        m_bOwnAllocator    = false;
    }
    else
    {
        if(m_pMemoryAllocator != 0 && !m_bOwnAllocator)
            return UMC_ERR_INIT;

        if(m_pMemoryAllocator == 0)
            m_pMemoryAllocator = new DefaultMemoryAllocator;
        m_bOwnAllocator = true;
    }

    return UMC_OK;
}

Status BaseCodec::Close(void)
{
    if(m_bOwnAllocator && m_pMemoryAllocator != 0)
    {
        delete m_pMemoryAllocator;
        m_bOwnAllocator    = false;
        m_pMemoryAllocator = 0;
    }

    return UMC_OK;
}
