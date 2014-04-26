/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_DUAL_THREAD_CODEC_H__
#define __UMC_DUAL_THREAD_CODEC_H__

#include "umc_media_data.h"
#include "umc_media_buffer.h"
#include "umc_base_codec.h"

namespace UMC
{

class DualThreadCodecParams
{
public:
    DYNAMIC_CAST_DECL_BASE(DualThreadCodecParams)

    // Default constructor
    DualThreadCodecParams(void)
    {
        m_pMediaBufferParams = NULL;
        m_pMediaBuffer       = NULL;
        m_pCodecInitParams   = NULL;
        m_pCodec             = NULL;
    }

    virtual ~DualThreadCodecParams(void) {}

    MediaBufferParams* m_pMediaBufferParams;  // pointer to media buffer parameter(s)
    MediaBuffer*       m_pMediaBuffer;        // pointer to media buffer

    BaseCodecParams*   m_pCodecInitParams;    // pointer to audio codec parameter(s)
    BaseCodec*         m_pCodec;              // pointer to audio codec
};


class DualThreadedCodec : public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(DualThreadedCodec, BaseCodec)

    DualThreadedCodec(void);
    virtual ~DualThreadedCodec(void);

    virtual Status Init(BaseCodec*   pCodec,
                        MediaBuffer* pInputBuffer,
                        MediaBuffer* pOutputBuffer = NULL);

    // BaseCodec methods
    virtual Status Init(BaseCodecParams* init);
    virtual Status GetFrame(MediaData* in, MediaData* out);
    virtual Status GetInfo(BaseCodecParams* info);
    virtual Status Close(void);
    virtual Status Reset(void);
    virtual Status SetParams(BaseCodecParams* params);

    // old API of DualThreadedCodec
    Status InitCodec(DualThreadCodecParams* init);
    Status LockInputBuffer(MediaData* in);
    Status UnLockInputBuffer(MediaData* in, Status StreamsStatus = UMC_OK);
    Status GetFrame(MediaData* out);

protected:
    Status PostponedInit(void);

    BaseCodec*       m_pCodec;          // Pointer to codec
    MediaBuffer*     m_pInputBuffer;    // Pointer to input buffer
    MediaBuffer*     m_pOutputBuffer;   // Pointer to output buffer
    bool             m_bDeleteResourcesOnClose;
    bool             m_bGetFrameLoop;

    BaseCodecParams* m_pPostponedParams;  // Keeps pointer to CodecParams for postponed initialization
    MediaData        m_pTmpInput[1];
    MediaData        m_pTmpOutput[1];
};

// Utility functions
Status PutDataToBuffer(MediaBuffer* pBuffer, MediaData* in, Status StreamStatus = UMC_OK);
Status GetDataFromBuffer(MediaBuffer* pBuffer, MediaData* out);

} // end namespace UMC

#endif /* __UMC_DUAL_THREAD_CODEC_H__ */
