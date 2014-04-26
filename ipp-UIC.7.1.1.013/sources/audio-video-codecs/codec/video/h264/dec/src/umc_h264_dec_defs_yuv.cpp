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
#include "umc_config.h"
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include "umc_h264_dec_defs_yuv.h"

namespace UMC
{

H264DecYUVBufferPadded::H264DecYUVBufferPadded()
    : m_pYPlane(0), m_pUVPlane(0), m_pUPlane(0), m_pVPlane(0), m_pitch_luma(0),
    m_pitch_chroma(0), m_color_format(YUV420)
{
    m_lumaSize.width = 0;
    m_lumaSize.height = 0;
}

H264DecYUVBufferPadded::~H264DecYUVBufferPadded()
{
    deallocate();
}

const FrameData * H264DecYUVBufferPadded::GetFrameData() const
{
    return &m_frameData;
}

void H264DecYUVBufferPadded::deallocate()
{
    if (m_frameData.GetFrameMID() != FRAME_MID_INVALID)
    {
        m_frameData.Close();
        return;
    }

    m_pYPlane = m_pUPlane = m_pVPlane = m_pUVPlane = 0;

    m_lumaSize.width = 0;
    m_lumaSize.height = 0;
    m_pitch_luma = 0;
    m_pitch_chroma = 0;
}

void H264DecYUVBufferPadded::Init(const VideoData *info)
{
    VM_ASSERT(info);

    m_bpp_luma   = info->GetPlaneBitDepth(0);
    m_bpp_chroma = info->GetPlaneBitDepth(1);

    m_color_format = info->m_colorFormat;
    m_chroma_format = GetH264ColorFormat(info->m_colorFormat);
    m_lumaSize = info->GetPtrToPlane(0)->m_size;
    m_pYPlane = 0;
    m_pUPlane = 0;
    m_pVPlane = 0;
    m_pUVPlane = 0;

    if (m_chroma_format > 0)
        m_chromaSize = info->GetPtrToPlane(1)->m_size;
    else
    {
        m_chromaSize.width = 0;
        m_chromaSize.height = 0;
    }
}

void H264DecYUVBufferPadded::allocate(const FrameData * frameData, const VideoData *info)
{
    VM_ASSERT(info);
    VM_ASSERT(frameData);

    VideoData::PlaneData *pPlane;
    pPlane = info->GetPtrToPlane(0);

    m_frameData = *frameData;

    if (frameData->GetPlaneMemoryInfo(0)->m_planePtr)
        m_frameData.m_locked = true;

    m_color_format = info->m_colorFormat;
    m_bpp_luma   = info->GetPlaneBitDepth(0);
    m_bpp_chroma = info->GetPlaneBitDepth(1);

    m_chroma_format = GetH264ColorFormat(info->m_colorFormat);
    m_lumaSize = pPlane->m_size;
    m_pitch_luma = (Ipp32s)m_frameData.GetPlaneMemoryInfo(0)->m_pitch / pPlane->m_iSampleSize;

    m_pYPlane = m_frameData.GetPlaneMemoryInfo(0)->m_planePtr;

    if (m_chroma_format > 0 || GetH264ColorFormat(frameData->GetInfo()->m_colorFormat) > 0)
    {
        if (m_chroma_format == 0)
            info = frameData->GetInfo();

        pPlane = info->GetPtrToPlane(1);

        m_chromaSize = pPlane->m_size;
        m_pitch_chroma = (Ipp32s)m_frameData.GetPlaneMemoryInfo(1)->m_pitch / pPlane->m_iSampleSize;

        if (m_frameData.GetInfo()->m_colorFormat == UMC::NV12)
        {
            m_pUVPlane = m_frameData.GetPlaneMemoryInfo(1)->m_planePtr;
            m_pUPlane = 0;
            m_pVPlane = 0;
        }
        else
        {
            m_pUPlane = m_frameData.GetPlaneMemoryInfo(1)->m_planePtr;
            m_pVPlane = m_frameData.GetPlaneMemoryInfo(2)->m_planePtr;
            m_pUVPlane = 0;
        }
    }
    else
    {
        m_chromaSize.width = 0;
        m_chromaSize.height = 0;
        m_pitch_chroma = 0;
        m_pUPlane = 0;
        m_pVPlane = 0;
    }
}

ColorFormat H264DecYUVBufferPadded::GetColorFormat() const
{
    return m_color_format;
}

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
