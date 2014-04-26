/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef ___UMC_VIDEO_RENDER_H___
#define ___UMC_VIDEO_RENDER_H___

#include "umc_structures.h"
#include "umc_video_data.h"
#include "umc_media_receiver.h"

namespace UMC
{

enum // video renderer flags
{
    FLAG_VREN_KEEPASPECT    = 0x00000008,
    FLAG_VREN_USETEXTURES   = 0x00000010,
    FLAG_VREN_HIDDEN        = 0x00000020
};

class VideoRenderParams: public MediaReceiverParams
{
public:
    DYNAMIC_CAST_DECL(VideoRenderParams, MediaReceiverParams)

    VideoRenderParams(void) {}

    VideoData m_videoData;  // image information: color format, video size, etc
};

class VideoRender: public MediaReceiver
{
public:
    DYNAMIC_CAST_DECL(VideoRender, MediaReceiver)

    VideoRender(void) {};
    virtual ~VideoRender(void)
    {
        Close();
    }

    // Initialize the render
    virtual Status Init(MediaReceiverParams*) { return UMC_OK; };

    // Peek presentation of next frame, return presentation time
    virtual Status GetRenderFrame(Ipp64f* time) = 0;

    // Rendering the current frame
    virtual Status RenderFrame(void) = 0;

    // Show the last rendered frame
    virtual Status ShowLastFrame(void) = 0;

    virtual Status PrepareForRePosition(void)  { return UMC_OK; };

    virtual ColorFormat CheckColorFormat(ColorFormat format) { return format; };
    virtual Ipp32u      CheckBitDepth(Ipp32u iDepth) { return iDepth; };

    virtual VideoRenderType GetRenderType(void)  = 0;

protected:
    VideoData  m_dataTemplate;
};

}

#endif
