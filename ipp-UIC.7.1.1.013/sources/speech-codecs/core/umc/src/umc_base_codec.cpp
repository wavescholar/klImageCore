/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_base_codec.h"
#include "umc_default_memory_allocator.h"

using namespace UMC;

// Default constructor
BaseCodecParams::BaseCodecParams(void)
{
    m_SuggestedOutputSize = 0;
    m_SuggestedInputSize = 0;
    lpMemoryAllocator = 0;
    numThreads = 0;

    m_pData=NULL;

    profile = 0;
    level = 0;
}

// Constructor
BaseCodec::BaseCodec(void)
{
    m_pMemoryAllocator = 0;
    m_bOwnAllocator = false;
}

// Destructor
BaseCodec::~BaseCodec(void)
{
  BaseCodec::Close();
}

// Initialize codec with specified parameter(s)
// Has to be called if MemoryAllocator interface is used
Status BaseCodec::Init(BaseCodecParams *init)
{
  if (init == 0)
    return UMC_ERR_NULL_PTR;

  // care about reentering as well
  if (init->lpMemoryAllocator) {
    if (m_bOwnAllocator || m_pMemoryAllocator != init->lpMemoryAllocator) {
      return UMC_ERR_INIT;
    }
    m_pMemoryAllocator = init->lpMemoryAllocator;
    m_bOwnAllocator = false;
  } else {
    if (m_pMemoryAllocator != 0 && !m_bOwnAllocator) {
      return UMC_ERR_INIT;
    }
    if (m_pMemoryAllocator == 0)
      m_pMemoryAllocator = new DefaultMemoryAllocator;
    m_bOwnAllocator = true;
  }
  return UMC_OK;
}

// Close all codec resources
Status BaseCodec::Close(void)
{
  if ( m_bOwnAllocator && m_pMemoryAllocator != 0 )
  {
    delete m_pMemoryAllocator;
    m_bOwnAllocator = false;
    m_pMemoryAllocator = 0;
  }
  return UMC_OK;
}

