/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_BASE_CODEC_H__
#define __UMC_BASE_CODEC_H__

#include "umc_media_data.h"
#include "umc_memory_allocator.h"

namespace UMC
{

class BaseCodecParams
{
    DYNAMIC_CAST_DECL_BASE(BaseCodecParams)

public:
    // Default constructor
    BaseCodecParams(void);

    // Destructor
    virtual ~BaseCodecParams(void){}

    MediaData *m_pData;
    MemoryAllocator *lpMemoryAllocator; // (MemoryAllocator *) pointer to memory allocator object

    Ipp32u m_SuggestedInputSize;   //max suggested frame size of input stream
    Ipp32u m_SuggestedOutputSize;  //max suggested frame size of output stream

    Ipp32s             numThreads; // maximum number of threads to use

    Ipp32s  profile; // profile
    Ipp32s  level;  // level
};

class BaseCodec
{
    DYNAMIC_CAST_DECL_BASE(BaseCodec)

public:
    // Constructor
    BaseCodec(void);

    // Destructor
    virtual ~BaseCodec(void);

    // Initialize codec with specified parameter(s)
    // Has to be called if MemoryAllocator interface is used
    virtual Status Init(BaseCodecParams *init);

    // Compress (decompress) next frame
    virtual Status GetFrame(MediaData *in, MediaData *out) = 0;

    // Get codec working (initialization) parameter(s)
    virtual Status GetInfo(BaseCodecParams *info) = 0;

    // Close all codec resources
    virtual Status Close(void);

    // Set codec to initial state
    virtual Status Reset(void) = 0;

    // Set new working parameter(s)
    virtual Status SetParams(BaseCodecParams *params)
    {
        if (NULL == params)
            return UMC_ERR_NULL_PTR;

        return UMC_ERR_NOT_IMPLEMENTED;
    }

protected:
    MemoryAllocator *m_pMemoryAllocator; // (MemoryAllocator*) pointer to memory allocator
    bool             m_bOwnAllocator;    // True when default allocator is used
};

} // end namespace UMC

#endif /* __UMC_BASE_CODEC_H__ */
