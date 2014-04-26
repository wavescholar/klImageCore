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

#ifdef UMC_ENABLE_FW_AUDIO_RENDER

#include "fw_audio_render.h"
#include "vm_time.h"

using namespace UMC;


FWAudioRender::FWAudioRender()
{
    m_tickStartTime    = 0;
    m_dfStartPTS       = -1;
    m_tickPrePauseTime = 0;
}

FWAudioRender::~FWAudioRender()
{
    Close();
}

Status FWAudioRender::Init(MediaReceiverParams* pInit)
{
    Status umcRes = UMC_OK;

    FileWriterParams writerParams;
    AudioRenderParams* pParams = DynamicCast<AudioRenderParams, MediaReceiverParams>(pInit);
    if (NULL == pParams)
    {   return UMC_ERR_NULL_PTR;   }

    m_tickStartTime = 0;
    m_tickPrePauseTime = 0;
    m_dfStartPTS = -1.0;

    FWAudioRenderParams* pFWParams = DynamicCast<FWAudioRenderParams, MediaReceiverParams>(pInit);

    writerParams.m_sFileName = pFWParams->sFileName;
    m_fileWriter.Init(&writerParams);

    umcRes = m_wav_writer.Init(&m_fileWriter, pParams->m_info.audioInfo.m_iSampleFrequency, pParams->m_info.audioInfo.m_iBitPerSample, pParams->m_info.audioInfo.m_iChannels);
    if(umcRes != UMC_OK)
        return UMC_ERR_INIT;

    if (UMC_OK == umcRes)
    {   umcRes = BasicAudioRender::Init(pInit); }

    return umcRes;
}

Status FWAudioRender::SendFrame(MediaData* in)
{
    Status umcRes = UMC_OK;

    if (UMC_OK == umcRes && (NULL == in->GetDataPointer())) {
        umcRes = UMC_ERR_NULL_PTR;
    }

    if (-1.0 == m_dfStartPTS) {
        m_dfStartPTS = in->m_fPTSStart;
    }

    if (UMC_OK == umcRes) {
        if (0 == m_tickStartTime)
        {   m_tickStartTime = vm_time_get_tick();   }
        size_t nbytes = in->GetDataSize();
        size_t nwritten = m_wav_writer.WriteData(in);
#ifdef UMC_RENDERS_LOG
        vm_string_printf(VM_STRING("FW AUDIO: Wrote %i bytes from %i\n"), nwritten, nbytes);

#endif
        if (nwritten != nbytes)
            return UMC_ERR_FAILED;
    }
    return umcRes;
}

Status FWAudioRender::Pause(bool /* pause */)
{
    m_tickPrePauseTime += vm_time_get_tick() - m_tickStartTime;
    m_tickStartTime = 0;
    return UMC_OK;
}

Ipp32f FWAudioRender::SetVolume(Ipp32f /* volume */)
{
    return -1;
}

Ipp32f FWAudioRender::GetVolume()
{
    return -1;
}

Status FWAudioRender::Close()
{
//    m_Thread.Wait();
    m_wav_writer.Finalize();
    m_dfStartPTS = -1.0;
    return UMC_OK;
//    return BasicAudioRender::Close();
}

Status FWAudioRender::Reset()
{
    m_tickStartTime = 0;
    m_tickPrePauseTime = 0;
    m_dfStartPTS = -1.0;
    return UMC_OK;
//    return BasicAudioRender::Reset();
}

Ipp64f FWAudioRender::GetTimeTick()
{
    Ipp64f dfRes = -1;
//    Ipp32u uiBytesPlayed = 0;

    if (0 != m_tickStartTime) {
        dfRes = m_dfStartPTS + ((Ipp64f)(Ipp64s)(m_tickPrePauseTime +
            vm_time_get_tick() - m_tickStartTime)) / (Ipp64f)(Ipp64s)vm_time_get_frequency();
    } else {
        dfRes = m_dfStartPTS + ((Ipp64f)(Ipp64s)m_tickPrePauseTime) / (Ipp64f)(Ipp64s)vm_time_get_frequency();
    }
    return dfRes;
}

#endif // UMC_ENABLE_FW_AUDIO_RENDER
