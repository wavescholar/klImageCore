/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "null_audio_render.h"

using namespace UMC;


NULLAudioRender::NULLAudioRender(Ipp64f dfDelay,
                                      UMC_FFMODE_MULTIPLIER mult):
    m_Freq(0),
    m_LastStartTime(0),
    m_PrePauseTime(0),
    m_bPaused(true),
    m_uiFFModeMult(1)
{
    m_Freq = vm_time_get_frequency();

    m_dInitialPTS = dfDelay;

    switch(mult)
    {
    case UMC_1X_FFMODE:
        m_uiFFModeMult  = 1;
        break;
    case UMC_2X_FFMODE:
        m_uiFFModeMult  = 2;
        break;
    case UMC_4X_FFMODE:
        m_uiFFModeMult  = 4;
        break;
    case UMC_8X_FFMODE:
        m_uiFFModeMult  = 8;
        break;
    case UMC_16X_FFMODE:
    default:
        m_uiFFModeMult  = 16;
        break;
   }
}

Ipp64f NULLAudioRender::GetTimeTick()
{
    Ipp64f dfRes = ((m_bPaused) ? (Ipp64f)(Ipp64s)(m_PrePauseTime):
        (Ipp64f)(Ipp64s)(vm_time_get_tick() - m_LastStartTime +
        m_PrePauseTime)) / (Ipp64s)m_Freq;

    dfRes += m_dInitialPTS;
    return dfRes * m_uiFFModeMult;
}

Status NULLAudioRender::Init(MediaReceiverParams* pInit)
{
    Status umcRes = UMC_OK;
    umcRes = BasicAudioRender::Init(pInit);
    Reset();
    return umcRes;
}

Status NULLAudioRender::Pause(bool bPause)
{
    if (bPause && !m_bPaused)
    {
        m_PrePauseTime += vm_time_get_tick() - m_LastStartTime;
        m_bPaused = true;
    }
    else if (!bPause && m_bPaused)
    {
        m_LastStartTime = vm_time_get_tick();
        m_bPaused = false;
    }
    return UMC_OK;
}

Ipp32f NULLAudioRender::SetVolume(Ipp32f /*volume*/)
{   return -1;    }

Ipp32f NULLAudioRender::GetVolume()
{   return -1;    }

Status NULLAudioRender::Close ()
{
    m_LastStartTime = 0;
    m_PrePauseTime = 0;
    m_bPaused = true;
    return BasicAudioRender::Close();
}

Status NULLAudioRender::Reset ()
{
    m_LastStartTime = 0;
    m_PrePauseTime = 0;
    m_bPaused = true;

    //return BasicAudioRender::Reset();
    return m_DataBuffer.Reset();
}

Status NULLAudioRender::SendFrame(MediaData* in)
{
    Ipp64f dfStart = 0.0;
    Ipp64f dfEnd = 0.0;
    Status umcRes = UMC_OK;

    if (UMC_OK == umcRes)
    {
        dfStart = in->m_fPTSStart;
        dfEnd   = in->m_fPTSEnd;
    }

    //  If we "render" first frame and PTS starts
    if (0 == m_LastStartTime && 0 == m_PrePauseTime)
    {
        if (0 != dfStart)
        {   m_PrePauseTime = (vm_tick)(dfStart * (Ipp64s)m_Freq); }
    }

    if (UMC_OK == umcRes)
    {   umcRes = Pause(false);  }

    while ((GetTimeTick() < dfEnd - 0.1) && (!m_bPaused))
    {   vm_time_sleep(500);    }
    return umcRes;
}

Status NULLAudioRender::SetParams(MediaReceiverParams *pMedia, Ipp32u  trickModes)
{
    Status umcRes = UMC_OK;
    AudioRenderParams* pParams = DynamicCast<AudioRenderParams, MediaReceiverParams>(pMedia);
    if (NULL == pParams)
    {   umcRes = UMC_ERR_NULL_PTR;  }

    NULLAudioRenderParams* pNULLParams = DynamicCast<NULLAudioRenderParams, MediaReceiverParams>(pMedia);

    if ((pNULLParams) && ((pNULLParams->m_InitPTS) > 0)) {
        m_dInitialPTS = pNULLParams->m_InitPTS;
     }

    switch(trickModes)
    {
    case UMC_TRICK_MODES_FFW_FAST:
        m_uiFFModeMult  = 2;
        break;
    case UMC_TRICK_MODES_FFW_FASTER:
        m_uiFFModeMult  = 4;
        break;
    case UMC_TRICK_MODES_SFW_SLOW:
        m_uiFFModeMult  = 0.5;
        break;
    case UMC_TRICK_MODES_SFW_SLOWER:
        m_uiFFModeMult  = 0.25;
        break;
    default:
        m_uiFFModeMult  = 1;
        break;
   }
    return UMC_OK;
}
