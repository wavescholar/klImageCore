/* ////////////////////////////////////////////////////////////////////////// */
/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_MJPEG_VIDEO_DECODER_H
#define __UMC_MJPEG_VIDEO_DECODER_H

#include "umc_defs.h"
//#pragma message (UMC_DEPRECATED_MESSAGE("MJPEG decoder"))

#include "umc_structures.h"
#include "umc_video_decoder.h"

// internal JPEG decoder object forward declaration
class  CBaseStreamInput;
class  CJPEGDecoder;
struct JPEGInternalParams;

namespace UMC
{

class MJPEGVideoDecoder : public VideoDecoder
{
public:
    DYNAMIC_CAST_DECL(MJPEGVideoDecoder, VideoDecoder)

    MJPEGVideoDecoder(void);
    virtual ~MJPEGVideoDecoder(void);

    // Initialize for subsequent frame decoding.
    virtual Status Init(BaseCodecParams* init);

    // Reset decoder to initial state
    virtual Status Reset(void);

    // Close decoding & free all allocated resources
    virtual Status Close(void);

    // Get decoder info
    virtual Status GetInfo(BaseCodecParams* info);

    // Get next frame
    virtual Status GetFrame(MediaData* in, MediaData* out);

    Status  ResetSkipCount(void)       { return UMC_ERR_NOT_IMPLEMENTED; }
    Status  SkipVideoFrame(Ipp32s)     { return UMC_ERR_NOT_IMPLEMENTED; }
    Ipp32u GetNumOfSkippedFrames(void) { return 0; }

protected:
    CJPEGDecoder*       m_pDecoder;
    VideoDecoderParams  m_params;
    JPEGInternalParams *m_pIntParams;
    MemID               m_frameMID;
    Ipp8u              *m_pDst[4];
    Ipp32s              m_iDstStep[4];
    bool                m_bField;
    bool                m_bFirstField;
};

}

#endif
