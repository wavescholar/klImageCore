/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"

#ifdef UMC_ENABLE_OSS_AUDIO_RENDER

#include "oss_audio_render.h"
#include "vm_time.h"

using namespace UMC;

OSSAudioRender::OSSAudioRender():
    m_dfStartPTS(-1.0),
    m_tickStartTime(0),
    m_tickPrePauseTime(0)
{}

Status OSSAudioRender::Init(MediaReceiverParams* pInit)
{
    Status umcRes = UMC_OK;
    AudioRenderParams* pParams = DynamicCast<AudioRenderParams, MediaReceiverParams>(pInit);
    if (NULL == pParams)
    {   umcRes = UMC_ERR_NULL_PTR;  }

    m_tickStartTime = 0;
    m_tickPrePauseTime = 0;
    m_dfStartPTS = -1.0;

    if (UMC_OK == umcRes)
    {
        umcRes = m_Dev.Init(pParams->m_info.audioInfo.m_iBitPerSample,
                            pParams->m_info.audioInfo.m_iChannels,
                            pParams->m_info.audioInfo.m_iSampleFrequency);
    }

    if (UMC_OK == umcRes)
    {   umcRes = BasicAudioRender::Init(pInit); }

    return umcRes;
}

Status OSSAudioRender::SendFrame(MediaData* in)
{
    Status umcRes = UMC_OK;

    if (UMC_OK == umcRes && (NULL == in->GetDataPointer()))
    {    umcRes = UMC_ERR_NULL_PTR;    }

    if (-1.0 == m_dfStartPTS)
    {   m_dfStartPTS = in->m_fPTSStart;   }

    if (UMC_OK == umcRes)
    {
        if (0 == m_tickStartTime)
        {   m_tickStartTime = vm_time_get_tick();   }
        umcRes = m_Dev.RenderData((Ipp8u*)in->GetDataPointer(), in->GetDataSize());
    }
    return umcRes;
}

Status OSSAudioRender::Pause(bool pause)
{
    Status umcRes = UMC_OK;
    if (pause)
    {   umcRes = m_Dev.Post();  }

    m_tickPrePauseTime += vm_time_get_tick() - m_tickStartTime;
    m_tickStartTime = 0;
    return umcRes;
}

Ipp32f OSSAudioRender::SetVolume(Ipp32f volume)
{
    return -1;
}

Ipp32f OSSAudioRender::GetVolume()
{
    return -1;
}

Status OSSAudioRender::Close()
{
    m_Dev.Reset();
    m_dfStartPTS = -1.0;
    return BasicAudioRender::Close();
}

Status OSSAudioRender::Reset()
{
    m_Dev.Reset();
    m_dfStartPTS = -1.0;
    m_tickStartTime = 0;
    m_tickPrePauseTime = 0;
    return BasicAudioRender::Reset();
}

Ipp64f OSSAudioRender::GetTimeTick()
{
    Ipp64f dfRes = -1;

    if (0 != m_tickStartTime)
    {
        dfRes = m_dfStartPTS +
            ((Ipp64f)(m_tickPrePauseTime +
            vm_time_get_tick() - m_tickStartTime)) / vm_time_get_frequency();
    }
    else
    {
        dfRes = m_dfStartPTS + ((Ipp64f)m_tickPrePauseTime) / vm_time_get_frequency();
    }
    BasicAudioRender::GetTimeTick(dfRes);
    return dfRes;
}

Status OSSAudioRender::SetParams(MediaReceiverParams *pMedia, Ipp32u  trickModes)
{
    Status umcRes = UMC::UMC_OK;
    AudioRenderParams* pParams = DynamicCast<AudioRenderParams, MediaReceiverParams>(pMedia);
    if (NULL == pParams)
    {   return UMC_ERR_NULL_PTR;  }

    if (UMC_OK == umcRes)
    {
        AudioStreamInfo* info = NULL;
        info = &pParams->m_info;

        if (info->audioInfo.m_iChannels != m_wInitedChannels)
        {
            m_Dev.SetNumOfChannels(info->audioInfo.m_iChannels);
            m_wInitedChannels = info->audioInfo.m_iChannels;
        }
        if (info->audioInfo.m_iSampleFrequency != m_dwInitedFrequency)
        {
            m_Dev.SetFreq(info->audioInfo.m_iSampleFrequency);
            m_dwInitedFrequency = info->audioInfo.m_iSampleFrequency;
        }
    }
    else
    {
         umcRes = UMC_ERR_FAILED;
    }
    return umcRes;
}

#endif // UMC_ENABLE_OSS_AUDIO_RENDER

