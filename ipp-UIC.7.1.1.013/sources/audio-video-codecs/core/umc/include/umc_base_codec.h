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
#include "umc_parser_cfg.h"

namespace UMC
{

class BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL_BASE(BaseCodecParams)

    BaseCodecParams(void)
    {
        m_pData                 = NULL;
        m_lpMemoryAllocator     = NULL;

        m_iSuggestedOutputSize  = 0;
        m_iSuggestedInputSize   = 0;
        m_iFlags                = 0;
        m_iThreads              = 0;
        m_iFramesCounter        = 0;
    }
    virtual ~BaseCodecParams(void) {}

    virtual Status ReadParams(ParserCfg*)
    {
        return UMC_ERR_NOT_IMPLEMENTED;
    }

    MediaData       *m_pData;
    MemoryAllocator *m_lpMemoryAllocator;       // pointer to memory allocator object

    Ipp32u           m_iSuggestedInputSize;     // max suggested frame size of input stream
    Ipp32u           m_iSuggestedOutputSize;    // max suggested frame size of output stream
    Ipp32u           m_iFlags;                  // codec flags
    Ipp32u           m_iThreads;                // maximum number of threads to use
    Ipp32u           m_iFramesCounter;          // amount of processed frames
};

class BaseCodec
{
public:
    DYNAMIC_CAST_DECL_BASE(BaseCodec)

    BaseCodec(void);
    virtual ~BaseCodec(void);

    // Initialize codec with specified parameters
    virtual Status Init(BaseCodecParams *pParams);

    // Try to get next processed frame
    virtual Status GetFrame(MediaData *pDataIn, MediaData *pDataOut) = 0;

    // Get current parameters
    virtual Status GetInfo(BaseCodecParams* info) = 0;

    // Close all codec resources
    virtual Status Close(void);

    // Set codec to initial state
    virtual Status Reset(void) = 0;

    // Set new working parameters
    virtual Status SetParams(BaseCodecParams *pParams)
    {
        if(NULL == pParams)
            return UMC_ERR_NULL_PTR;

        return UMC_ERR_NOT_IMPLEMENTED;
    }

protected:
    MemoryAllocator* m_pMemoryAllocator; // pointer to memory allocator
    bool             m_bOwnAllocator;    // True when default allocator is used
};

}

#endif
