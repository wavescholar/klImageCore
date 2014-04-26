/*
 *
 *                  INTEL CORPORATION PROPRIETARY INFORMATION
 *     This software is supplied under the terms of a license agreement or
 *     nondisclosure agreement with Intel Corporation and may not be copied
 *     or disclosed except in accordance with the terms of that agreement.
 *       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
 *
 */

#include "null_video_render.h"

using namespace UMC;


NULLVideoRender::NULLVideoRender(void)
{
    m_bStop = false;
}

NULLVideoRender::~NULLVideoRender(void)
{
    Close();
}

Status NULLVideoRender::Init(MediaReceiverParams* pInit)
{
    NULLVideoRenderParams *params = DynamicCast<NULLVideoRenderParams, MediaReceiverParams>(pInit);
    Status status;

    if(NULL == params)
        return UMC_ERR_NULL_PTR;

    Close();

    status = VideoRender::Init(pInit);
    if(UMC_OK != status)
        return status;

    m_params = *params;

    if(m_params.m_videoData.m_colorFormat == NONE)
        return UMC_ERR_NOT_INITIALIZED;

    if(m_params.m_iNumberOfBuffers == 0)
        m_params.m_iNumberOfBuffers = 1;
    if(m_params.m_iNumberOfBuffers > IPP_MAX_8U)
        m_params.m_iNumberOfBuffers = IPP_MAX_8U;

    if(0 == m_BufferParam.m_prefInputBufferSize)
    {
        // set buffer parameters
        m_BufferParam.m_numberOfFrames = m_params.m_iNumberOfBuffers;
        m_BufferParam.m_prefInputBufferSize = m_BufferParam.m_prefOutputBufferSize = m_params.m_videoData.GetMappingSize();

        status = m_Frames.Init(&m_BufferParam);
        if(UMC_OK != status)
            return status;
    }

    return UMC_OK;
}


Status NULLVideoRender::Close(void)
{
    m_BufferParam.m_numberOfFrames       = 0;
    m_BufferParam.m_prefInputBufferSize  = 0;
    m_BufferParam.m_prefOutputBufferSize = 0;
    m_Frames.Close();
    m_bStop = false;

    VideoRender::Close();

    return UMC_OK;
}

Status NULLVideoRender::LockInputBuffer(MediaData *pInData)
{
    VideoData *pSrc = DynamicCast<VideoData, MediaData>(pInData);

    if(pSrc == NULL)
        return UMC_ERR_FAILED;

    *pSrc = m_params.m_videoData;
    return m_Frames.LockInputBuffer(pInData);
}

Status NULLVideoRender::UnLockInputBuffer(MediaData *pInData, Status streamStatus)
{
    VideoData *pSrc = DynamicCast<VideoData, MediaData>(pInData);

    if(pSrc == NULL || pSrc->GetDataSize() == 0)
        return UMC_ERR_FAILED;

    return m_Frames.UnLockInputBuffer(pInData, streamStatus);
}

Status NULLVideoRender::GetRenderFrame(Ipp64f *pTime)
{
    MediaData mediaData;
    Status    status = UMC_OK;

    if(pTime == NULL)
        return UMC_ERR_FAILED;

    *pTime = -1;

    // get latest frame time stamp
    status = m_Frames.LockOutputBuffer(&mediaData);
    if(status == UMC_ERR_NOT_ENOUGH_DATA)
        return m_bStop ? UMC_ERR_END_OF_STREAM : UMC_ERR_TIMEOUT;
    if(status != UMC_OK)
        return UMC_ERR_FAILED;

    *pTime = mediaData.m_fPTSStart;
    return m_Frames.UnLockOutputBuffer(&mediaData);
}

Status NULLVideoRender::RenderFrame(void)
{
    MediaData  srcData;
    Status     status = UMC_OK;

    status = m_Frames.LockOutputBuffer(&srcData);
    if(UMC_OK != status)
        return status;

    srcData.MoveDataPointer((Ipp32s)srcData.GetDataSize());
    return m_Frames.UnLockOutputBuffer(&srcData);
}
