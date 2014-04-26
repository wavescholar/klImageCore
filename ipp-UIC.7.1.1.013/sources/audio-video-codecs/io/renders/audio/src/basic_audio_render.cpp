/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "basic_audio_render.h"
#include "umc_audio_data.h"

using namespace UMC;

Ipp32u VM_THREAD_CALLCONVENTION BasicAudioRenderThreadProc(void* vpParam)
{
  VM_ASSERT(NULL != vpParam);
  BasicAudioRender* pThis =
      reinterpret_cast<BasicAudioRender*>(vpParam);
  pThis->ThreadProc();
  return 0;
}

BasicAudioRender::BasicAudioRender()
{
  m_dfNorm = 1;
  m_tStartTime = 0;
  m_tStopTime = 0;
  m_bStop = false;
  m_bPause = false;
  m_RendererIsStopped = false;
  m_dwInitedFrequency = 0;
  m_wInitedChannels = 0;

  m_tFreq = vm_time_get_frequency();
  m_eventSyncPoint.Init(1, 1);
  m_eventSyncPoint1.Init(1, 0);
  m_dDynamicChannelPTS = -1;
  m_wDynamicChannels   = -1;
}

BasicAudioRender::~BasicAudioRender()
{
  Close();
}

Status BasicAudioRender::Init(MediaReceiverParams* pInit)
{
  Status umcRes = UMC_OK;
  AudioRenderParams* pParams = DynamicCast<AudioRenderParams, MediaReceiverParams>(pInit);

  m_RendererIsStopped = false;

  if (NULL == pParams) {
    umcRes = UMC_ERR_NULL_PTR;
  }

  if (UMC_OK == umcRes) {
    MediaBufferParams Params;
    Ipp32s iSamples = 0;
    AudioStreamInfo* pAudioInfo = &pParams->m_info;

    m_dfNorm = 1.0 / pAudioInfo->audioInfo.m_iSampleFrequency
                   / (pAudioInfo->audioInfo.m_iBitPerSample >> 3)
                   / pAudioInfo->audioInfo.m_iChannels;

    m_wInitedChannels   = pAudioInfo->audioInfo.m_iChannels;
    m_dwInitedFrequency = pAudioInfo->audioInfo.m_iSampleFrequency ;
    switch (pAudioInfo->streamType) {
      case AC3_AUDIO:
        iSamples = 256*6;
        break;
      case MP1L2_AUDIO:
      case MP1L3_AUDIO:
        iSamples = 1152;
        break;
      case MP2L3_AUDIO:
        iSamples = 576;
        break;
      default: iSamples = 1024;
    }

    Params.m_prefInputBufferSize = //2304;
                                   pAudioInfo->audioInfo.m_iChannels *
                                   (pAudioInfo->audioInfo.m_iBitPerSample >> 3) *
                                   iSamples*2;
    Params.m_numberOfFrames = 100;
    Params.m_prefOutputBufferSize = Params.m_prefInputBufferSize;
                                    //2304;//4 * 1024;

    umcRes = m_DataBuffer.Init(&Params);
  }

  if (UMC_OK == umcRes) {
    m_bStop = false;
    umcRes = m_Thread.Create(BasicAudioRenderThreadProc, this);
  }

  return umcRes;
}

void BasicAudioRender::ThreadProc()
{
  Status umcRes = UMC_OK;
  AudioData data;

  while (UMC_OK == umcRes && !m_bStop) {
    if (UMC_OK == umcRes) {
      umcRes = m_DataBuffer.LockOutputBuffer(&data);
    }

    if (UMC_ERR_END_OF_STREAM == umcRes) {
      break;
    } else
    if (UMC_ERR_NOT_ENOUGH_DATA == umcRes) {
      umcRes = UMC_OK;
      vm_time_sleep(300);
      continue;
    }

    if (data.m_fPTSStart == -1) { // first frame
      data.m_fPTSStart = 0;
    }

    Ipp64f dfEndTime = data.m_fPTSStart + m_dfNorm * data.GetDataSize();
    VM_ASSERT(UMC_OK == umcRes || UMC_ERR_TIMEOUT == umcRes || UMC_WRN_REPOSITION_INPROGRESS == umcRes);
    if (UMC_OK == umcRes) {
      data.m_fPTSEnd = dfEndTime;
      umcRes = SendFrame(&data);
      if (umcRes == UMC_ERR_FAILED) {
        umcRes = UMC_OK;
        m_RendererIsStopped = true;
      } else
      if (umcRes != UMC_OK) {
        break;
      }
    }

    if (UMC_OK == umcRes) {
      data.m_fPTSStart = dfEndTime;
      data.MoveDataPointer((Ipp32s)data.GetDataSize());
      umcRes = m_DataBuffer.UnLockOutputBuffer(&data);
    }

    if (m_RendererIsStopped) {
      m_bStop = true;
      break;
    }

    if (umcRes == UMC_WRN_REPOSITION_INPROGRESS) {
      break;
    }
  }

  if (!m_bStop && (umcRes == UMC_ERR_END_OF_STREAM))
  {
    umcRes = DrainBuffer();
    m_bStop = true;
  }
}

// Audio Reference Clock
Ipp64f BasicAudioRender::GetTime()
{
  m_tStartTime = vm_time_get_tick();
  Ipp64f dfTime = -1.0;
  if (!m_bStop) {
    dfTime = GetTimeTick();
  } else {
    if (m_Thread.IsValid()) {
      m_Thread.Wait();
    }
  }

  m_tStopTime = vm_time_get_tick();

  // we must return -1 if playback is already stopped, so zero GetDelay()
  // result is required
  if (-1.0 == dfTime) {
    m_tStartTime = m_tStopTime;
  }

  return dfTime >= 0 ? dfTime : -1.0;
}

Status BasicAudioRender::GetTimeTick(Ipp64f pts)
{
  Status umcRes = UMC_ERR_FAILED;

  if((m_dDynamicChannelPTS > 0) && (pts >= m_dDynamicChannelPTS)) {
    AudioRenderParams par;

    memset(&par.m_info, 0, sizeof(AudioStreamInfo));
    par.m_pSysContext = NULL;
    par.m_info.audioInfo.m_iSampleFrequency = m_dwInitedFrequency;
    par.m_info.audioInfo.m_iChannels         = m_wDynamicChannels;
    par.m_info.audioInfo.m_iBitPerSample     = 16;

    umcRes = SetParams(&par);

    if(UMC_OK == umcRes)
      m_dDynamicChannelPTS = -1;
  }
  return umcRes;
}

// Estimated value of device latency
Ipp64f BasicAudioRender::GetDelay()
{
  return (Ipp64f)(Ipp64s)(m_tStopTime - m_tStartTime) / (Ipp64f)(Ipp64s)m_tFreq;
}

Status BasicAudioRender::LockInputBuffer(MediaData *in)
{
  if (m_RendererIsStopped) {
    return UMC_ERR_FAILED;
  } else
  return m_DataBuffer.LockInputBuffer(in);
}

Status BasicAudioRender::UnLockInputBuffer(MediaData *in,
                                                     Status StreamStatus)
{
  Status umcRes = UMC_OK;

  if (UMC_OK == umcRes) {
    umcRes = m_DataBuffer.UnLockInputBuffer(in, StreamStatus);
  }

  AudioData* pData = DynamicCast<AudioData, MediaData>(in);

  if(pData) {
    if(m_wInitedChannels == pData->m_iChannels && m_dwInitedFrequency != pData->m_iSampleFrequency)
    {
      AudioRenderParams param;
      param.m_info.audioInfo = *pData;
      if(UMC_OK == SetParams(&param)) {
        m_wInitedChannels   = pData->m_iChannels;
        m_dwInitedFrequency = pData->m_iSampleFrequency;
      }
    } else
    if((m_wInitedChannels != pData->m_iChannels) &&
       (m_dDynamicChannelPTS == -1)) {
      m_dDynamicChannelPTS = in->m_fPTSStart;
      m_wDynamicChannels   = pData->m_iChannels;
    }
  }
  return umcRes;
}

Status BasicAudioRender::Stop()
{
  m_bStop = true;
  return UMC_OK;
}

Status BasicAudioRender::Pause(bool pause)
{
  if (pause)
    m_bPause = true;
  else
    m_bPause = false;
  return UMC_OK;
}

Status BasicAudioRender::Close()
{
  if (m_Thread.IsValid())
  {
    m_bStop = true;
    m_Thread.Wait();
  }
  m_DataBuffer.Close();
  return UMC_OK;
}

Status BasicAudioRender::Reset()
{
  m_Thread.Wait();
  m_DataBuffer.Reset();
  return UMC_OK;
}

Status BasicAudioRender::SetParams(
  MediaReceiverParams* pMedia,
  Ipp32u  /* trickModes */)
{
  Status umcRes = 0;

  AudioRenderParams* pParams = DynamicCast<AudioRenderParams, MediaReceiverParams>(pMedia);
  if (NULL == pParams)
    umcRes = UMC_ERR_NULL_PTR;

  if (UMC_OK == umcRes)
  {
    AudioStreamInfo* info = NULL;
    info = &pParams->m_info;
    if (info->audioInfo.m_iSampleFrequency &&
        info->audioInfo.m_iBitPerSample &&
        info->audioInfo.m_iChannels)
    {
      m_dfNorm = 1.0 / info->audioInfo.m_iSampleFrequency
                     / (info->audioInfo.m_iBitPerSample >> 3)
                     / info->audioInfo.m_iChannels;
      m_wInitedChannels = info->audioInfo.m_iChannels;
      m_dwInitedFrequency = info->audioInfo.m_iSampleFrequency;
    } else {
      umcRes = UMC_ERR_FAILED;
    }
  }

  return umcRes;
}
