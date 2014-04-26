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

#ifndef __UMC_H264_DEC_DEFS_YUV_H__
#define __UMC_H264_DEC_DEFS_YUV_H__

#include "umc_h264_dec_defs_dec.h"
#include "umc_video_decoder.h"
#include "umc_frame_data.h"

namespace UMC
{

class H264DecYUVBufferPadded
{
public:
    DYNAMIC_CAST_DECL_BASE(H264DecYUVBufferPadded)

    Ipp32s  m_bpp_luma;      // should be >= 8
    Ipp32s  m_bpp_chroma;    // should be >= 8
    Ipp32s  m_chroma_format; // AVC standard value chroma_format_idc

    PlanePtrYCommon               m_pYPlane;

    PlanePtrUVCommon              m_pUVPlane;  // for NV12 support

    PlanePtrUVCommon              m_pUPlane;
    PlanePtrUVCommon              m_pVPlane;

    H264DecYUVBufferPadded();
    virtual ~H264DecYUVBufferPadded();

    void Init(const VideoData *info);

    void allocate(const FrameData * frameData, const VideoData *info);

    void deallocate();

    const IppiSize& lumaSize() const { return m_lumaSize; }
    const IppiSize& chromaSize() const { return m_chromaSize; }

    Ipp32u pitch_luma() const { return m_pitch_luma; }
    Ipp32u pitch_chroma() const { return m_pitch_chroma; }

    const FrameData * GetFrameData() const;

    ColorFormat GetColorFormat() const;

protected:

    IppiSize            m_lumaSize;
    IppiSize            m_chromaSize;

    Ipp32s  m_pitch_luma;
    Ipp32s  m_pitch_chroma;

    FrameData m_frameData;

    ColorFormat m_color_format;
};

} // namespace UMC

#endif // __UMC_H264_DEC_DEFS_YUV_H__
