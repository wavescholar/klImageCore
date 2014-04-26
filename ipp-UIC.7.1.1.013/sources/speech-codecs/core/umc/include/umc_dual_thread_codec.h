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

// Create codec with input or/and output buffering.
// Returned pointer should be deleted via delete operator.
// Pointers pCodec, pInputBuffer, pOutputBuffer will be deleted automatically
BaseCodec *CreateBufferingCodec(BaseCodec   *pCodec,
                                MediaBuffer *pInputBuffer,
                                MediaBuffer *pOutputBuffer = NULL);

class DualThreadCodecParams
{
public:
    // Default constructor
    DualThreadCodecParams(void);

    MediaBufferParams *m_pMediaBufferParams;                    // (MediaBufferParams *) pointer to media buffer parameter(s)
    MediaBuffer *m_pMediaBuffer;                                // (MediaBuffer *) pointer to media buffer

    BaseCodecParams *m_pCodecInitParams;                        // (BaseCodecParams *) pointer to audio codec parameter(s)
    BaseCodec *m_pCodec;                                        // (BaseCodec *) pointer to audio codec
};

class DualThreadedCodec : public BaseCodec
{
    DYNAMIC_CAST_DECL(DualThreadedCodec, BaseCodec)

public:
    // Constructor
    DualThreadedCodec(void);
    // Destructor
    virtual ~DualThreadedCodec(void);

    virtual Status Init(BaseCodec *pCodec,
                        MediaBuffer *pInputBuffer,
                        MediaBuffer *pOutputBuffer = NULL);

    // BaseCodec methods
    virtual Status Init(BaseCodecParams *init);
    virtual Status GetFrame(MediaData *in, MediaData *out);
    virtual Status GetInfo(BaseCodecParams *info);
    virtual Status Close(void);
    virtual Status Reset(void);
    virtual Status SetParams(BaseCodecParams *params);

    // old API of DualThreadedCodec
    Status InitCodec(DualThreadCodecParams *init);
    Status LockInputBuffer(MediaData *in);
    Status UnLockInputBuffer(MediaData *in, Status StreamsStatus = UMC_OK);
    Status GetFrame(MediaData *out);

protected:
    Status PostponedInit();

    BaseCodec       *m_pCodec;          // Pointer to codec
    MediaBuffer     *m_pInputBuffer;    // Pointer to input buffer
    MediaBuffer     *m_pOutputBuffer;   // Pointer to output buffer
    bool            m_bDeleteResourcesOnClose;
    bool            m_bGetFrameLoop;

    BaseCodecParams *m_pPostponedParams;    // Keeps pointer to CodecParams for postponed initialization
    MediaData       m_pTmpInput[1];
    MediaData       m_pTmpOutput[1];
};

// Utility functions
Status PutDataToBuffer(MediaBuffer *pBuffer, MediaData* in, Status StreamStatus = UMC_OK);
Status GetDataFromBuffer(MediaBuffer *pBuffer, MediaData* out);

} // end namespace UMC

#endif /* __UMC_DUAL_THREAD_CODEC_H__ */
