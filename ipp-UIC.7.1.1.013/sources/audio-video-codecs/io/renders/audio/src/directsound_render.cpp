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
#ifdef UMC_ENABLE_DSOUND_AUDIO_RENDER

#include "directsound_render.h"

using namespace UMC;

//const Ipp32u DSoundAudioRender::m_cuiSizeOfFrame = 6144;
const Ipp32u DSoundAudioRender::m_cuiNumOfFrames = 10;

DSoundAudioRender::DSoundAudioRender():
    m_hTimer(NULL),
    m_dwSampleAlign(0)
{}

DSoundAudioRender::~DSoundAudioRender(void)
{   Close();    }

Status DSoundAudioRender::Init(MediaReceiverParams* pInit)
{
    Status umcRes = UMC_OK;

    AudioRenderParams* pParams = DynamicCast<AudioRenderParams, MediaReceiverParams>(pInit);
    if (NULL == pParams)
    {   
        umcRes = UMC_ERR_NULL_PTR;
        return umcRes;
    }

    SysRenderContext* pContext = pParams->m_pSysContext;
    if (UMC_OK == umcRes)
    {
        if (NULL == pContext)
        {    umcRes = UMC_ERR_INIT; }
    }

    AudioStreamInfo* info = NULL;
    Ipp32s iLength = 0;
    Ipp32u dwBufferSize = 0;
    if (UMC_OK == umcRes)
    {
        info = &pParams->m_info;

        Ipp32s iSamples;
        switch (info->streamType)
        {
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

        m_dwSampleAlign = info->audioInfo.m_iChannels * (info->audioInfo.m_iBitPerSample >> 3);
        iLength = m_dwSampleAlign * iSamples;
        dwBufferSize = iLength * m_cuiNumOfFrames;
    }

    if (UMC_OK == umcRes)
    {
        umcRes = m_DSBuffer.Init(pContext->m_hWnd,
                                 dwBufferSize,
                                 (Ipp16u)info->audioInfo.m_iChannels,
                                 info->audioInfo.m_iSampleFrequency,
                                 (Ipp16u)(info->audioInfo.m_iBitPerSample >> 3));
    }

    if (UMC_OK == umcRes && NULL == m_hTimer)
    {
        m_hTimer = CreateWaitableTimer(0,false,0);
        if (NULL == m_hTimer)
        { umcRes = UMC_ERR_FAILED;  }
    }

    if (UMC_OK == umcRes)
    {   umcRes = BasicAudioRender::Init(pInit); }

    if (UMC_OK == umcRes)
    {   umcRes = m_AudioPtsArray.Init(m_cuiNumOfFrames, m_dfNorm);    }

    if (UMC_OK == umcRes)
    {
        LONG period = (LONG) (iLength / 2 * m_dfNorm * 1000);   // 32/2 ms
        LARGE_INTEGER interval;
        interval.QuadPart = - period/4*10000;  // in terms of 100ns

        if (!SetWaitableTimer(m_hTimer,&interval,period,0,0,true))
        {   umcRes = UMC_ERR_FAILED;  }
    }

    return umcRes;
}

Status DSoundAudioRender::SendFrame(MediaData* in)
{
    Status umcRes = UMC_OK;

    VM_ASSERT(NULL != in);
    if (!in)
    {  
        umcRes = UMC_ERR_NULL_PTR;
        return umcRes;
    }

    if (UMC_OK == umcRes && NULL == in->GetDataPointer())
    {   umcRes = UMC_ERR_NULL_PTR;  }

    Ipp32u uiLength = (Ipp32u)in->GetDataSize();
    Ipp32u uiStartWritePos = m_DSBuffer.GetNextWriteOffset();
    Ipp32u uiWrapSize = 0;
    Ipp64f dfStartPTS = in->m_fPTSStart;

    if (UMC_OK == umcRes) {
        umcRes = m_AudioPtsArray.SetFramePTS(m_DSBuffer.GetNextWriteOffset(),
                                             0, in->m_fPTSStart);
    }

    for (Ipp8u *data_ptr = (Ipp8u *) in->GetDataPointer();
         UMC_OK == umcRes && 0 != uiLength && !m_bStop;)  {
        Ipp32u dwPlayCursor = 0;
        Ipp32u dwWriteCursor = 0;
        Ipp32u dwBytesWrote = 0;

        if (WAIT_OBJECT_0 != WaitForSingleObject(m_hTimer,INFINITE))
        {   umcRes = UMC_ERR_FAILED;  }

        if (UMC_OK == umcRes)
        { umcRes = m_DSBuffer.GetPlayPos(dwPlayCursor);   }

        if (UMC_OK == umcRes)
        { umcRes = m_DSBuffer.GetWritePos(dwWriteCursor); }

        if (UMC_OK == umcRes) {

            if (dwPlayCursor <= dwWriteCursor) {
                //  Do not put data up to the Play position itself - leave some space behind it
                if (m_dwSampleAlign <= dwPlayCursor)
                    {  dwPlayCursor -= m_dwSampleAlign;    }

                if (m_DSBuffer.GetNextWriteOffset() >= dwWriteCursor) {
                    umcRes = m_DSBuffer.CopyDataToBuffer(data_ptr, uiLength, dwBytesWrote);
                    if (UMC_OK == umcRes) {
                        VM_ASSERT(uiStartWritePos == 0 || 0 == uiWrapSize);
                        data_ptr += dwBytesWrote;
                        uiLength -= dwBytesWrote;
                        umcRes = m_AudioPtsArray.SetFramePTS(uiStartWritePos,
                                                             (Ipp32u)(in->GetDataSize() - uiLength - uiWrapSize),
                                                             dfStartPTS);
                        if (0 == m_DSBuffer.GetNextWriteOffset()) {
                            uiWrapSize = (Ipp32u)(in->GetDataSize() - uiLength);
                            dfStartPTS = in->m_fPTSStart + uiWrapSize * m_dfNorm;
                            uiStartWritePos = 0;
                        }
                    }
                }

                if ((UMC_OK == umcRes) && (m_DSBuffer.GetNextWriteOffset() < dwPlayCursor) && uiLength) {
                    dwBytesWrote = 0;
                    umcRes = m_DSBuffer.CopyDataToBuffer(data_ptr,
                                                         IPP_MIN(uiLength, dwPlayCursor - m_DSBuffer.GetNextWriteOffset()),
                                                         dwBytesWrote);
                    if (UMC_OK == umcRes) {
                        VM_ASSERT(uiStartWritePos == 0 || 0 == uiWrapSize);
                        data_ptr += dwBytesWrote;
                        uiLength -= dwBytesWrote;
                        umcRes = m_AudioPtsArray.SetFramePTS(uiStartWritePos,
                                                             (Ipp32u)(in->GetDataSize() - uiLength - uiWrapSize), dfStartPTS);
                    }
                }
            } else {
                if ((m_DSBuffer.GetNextWriteOffset() > dwWriteCursor) &&
                    (m_DSBuffer.GetNextWriteOffset() < dwPlayCursor)) {
                    //  Do not put data up to the Play position itself - leave some space behind it
                    if (m_DSBuffer.GetNextWriteOffset() + m_dwSampleAlign <= dwPlayCursor)
                    {   dwPlayCursor -= m_dwSampleAlign;    }

                    umcRes = m_DSBuffer.CopyDataToBuffer(data_ptr,
                                                         IPP_MIN(uiLength, dwPlayCursor - m_DSBuffer.GetNextWriteOffset()),
                                                         dwBytesWrote);
                    if (UMC_OK == umcRes) {
                        VM_ASSERT(uiStartWritePos == 0 || 0 == uiWrapSize);
                        data_ptr += dwBytesWrote;
                        uiLength -= dwBytesWrote;
                        umcRes = m_AudioPtsArray.SetFramePTS(uiStartWritePos,
                                                             (Ipp32u)(in->GetDataSize() - uiLength - uiWrapSize),
                                                             dfStartPTS);
                        if (0 == m_DSBuffer.GetNextWriteOffset()) {
                            uiWrapSize = (Ipp32u)(in->GetDataSize() - uiLength);
                            dfStartPTS = in->m_fPTSStart + uiWrapSize * m_dfNorm;
                            uiStartWritePos = 0;
                        }
                    }
                }
            }
        }
    }
    return umcRes;
}

Status DSoundAudioRender::DrainBuffer()
{
    Status umcRes = UMC_OK;
    Ipp32u dwPlayCursor = 0;
    Ipp32u dwPlayCursorLast = 0;
    Ipp32u dwWriteCursor = 0;
    Ipp32u dwStopCursor = m_DSBuffer.GetNextWriteOffset();
    Ipp32s m_Cycle;

    if (UMC_OK == umcRes) {
        umcRes = m_DSBuffer.GetPlayPos(dwPlayCursor);
    }

    if (UMC_OK == umcRes) {
        umcRes = m_DSBuffer.GetWritePos(dwWriteCursor);
    }

    m_Cycle = (dwPlayCursor > dwStopCursor) ? 1 : 0;

    while ((UMC_OK == umcRes) && !((m_Cycle == 0) && (dwPlayCursor > dwStopCursor)) ) {

        if (WAIT_OBJECT_0 != WaitForSingleObject(m_hTimer,INFINITE)) {
            umcRes = UMC_ERR_FAILED;
        }

        if (UMC_OK == umcRes) {
            if (dwPlayCursor <= dwWriteCursor) {

                if (m_dwSampleAlign <= dwPlayCursor) {
                    dwPlayCursor -= m_dwSampleAlign;
                }

                if (m_DSBuffer.GetNextWriteOffset() >= dwWriteCursor) {
                    umcRes = m_DSBuffer.CopyZerosToBuffer(0);
                }

                if ((UMC_OK == umcRes) && (m_DSBuffer.GetNextWriteOffset() < dwPlayCursor)) {
                    umcRes = m_DSBuffer.CopyZerosToBuffer(dwPlayCursor - m_DSBuffer.GetNextWriteOffset());
                }

            } else {
                if ((m_DSBuffer.GetNextWriteOffset() > dwWriteCursor) &&
                    (m_DSBuffer.GetNextWriteOffset() < dwPlayCursor)) {

                    if (dwWriteCursor + m_dwSampleAlign <= dwPlayCursor) {
                        dwPlayCursor -= m_dwSampleAlign;
                    }
                    umcRes = m_DSBuffer.CopyZerosToBuffer(dwPlayCursor - m_DSBuffer.GetNextWriteOffset());
                }
            }
        }

        if (UMC_OK == umcRes) {
            dwPlayCursorLast = dwPlayCursor;
            umcRes = m_DSBuffer.GetPlayPos(dwPlayCursor);
        }

        if (UMC_OK == umcRes) {
            if (dwPlayCursor < dwPlayCursorLast)
                m_Cycle--;
            umcRes = m_DSBuffer.GetWritePos(dwWriteCursor);
        }
    }
    return umcRes;
}

Status DSoundAudioRender::Close()
{
    BasicAudioRender::Close();
    m_DSBuffer.Close();
    if (NULL != m_hTimer) {
        CloseHandle(m_hTimer);
        m_hTimer = NULL;
    }
    m_AudioPtsArray.Reset();
    m_dwSampleAlign = 0;
    return UMC_OK;
}

Ipp64f DSoundAudioRender::GetTimeTick()
{
    Ipp64f pts = 0;
    Ipp32u dwPlayCursor;
    Status umcRes = m_DSBuffer.GetPlayPos(dwPlayCursor);

    if (UMC_OK == umcRes)
    {
        pts = m_AudioPtsArray.GetTime(dwPlayCursor);
        umcRes = BasicAudioRender::GetTimeTick(pts);
        if(UMC_OK == umcRes)
        {
            m_DSBuffer.GetPlayPos(dwPlayCursor);
            pts = m_AudioPtsArray.GetTime(dwPlayCursor);
        }

        if (m_DSBuffer.GetCompensation() <= pts)
        {   pts -= m_DSBuffer.GetCompensation();    }
        else
        {   pts = 0;    }
    }
    else
    {   pts = -1.;  }
    return pts;
}

Status DSoundAudioRender::Pause(bool new_value)
{   return m_DSBuffer.Pause(new_value); }

Ipp32f DSoundAudioRender::SetVolume(Ipp32f volume)
{   return m_DSBuffer.SetVolume(volume);    }

Ipp32f DSoundAudioRender::GetVolume()
{   return m_DSBuffer.GetVolume();  }

Ipp32u VM_THREAD_CALLCONVENTION BasicAudioRenderThreadProc(void* vpParam);

Status DSoundAudioRender::Reset()
{
    Status ret = BasicAudioRender::Reset();
    m_AudioPtsArray.Reset();
    m_DSBuffer.Reset();
    m_Thread.Wait();

    m_Thread.Create(BasicAudioRenderThreadProc, this);
    return ret;
}

Status DSoundAudioRender::SetParams(MediaReceiverParams *pMedia,
                                              Ipp32u  trickModes)
{

    Status umcRes = UMC_OK;
    AudioRenderParams* pParams = DynamicCast<AudioRenderParams, MediaReceiverParams>(pMedia);
    if (NULL == pParams)
    {   umcRes = UMC_ERR_NULL_PTR;  }

    if (UMC_OK == umcRes)
    {
        AudioStreamInfo* info = NULL;
        info = &pParams->m_info;

        if((info->audioInfo.m_iChannels         != m_wInitedChannels) ||
           (info->audioInfo.m_iSampleFrequency != m_dwInitedFrequency))
        {
            umcRes = m_DSBuffer.DynamicSetParams((Ipp16u)info->audioInfo.m_iChannels,
                                                 info->audioInfo.m_iSampleFrequency,
                                                 (Ipp16u)(info->audioInfo.m_iBitPerSample >> 3));
            if(UMC_OK == umcRes)
            {
                 umcRes = BasicAudioRender::SetParams(pMedia, trickModes);
                 m_AudioPtsArray.DynamicSetParams(m_dfNorm);
            }
        }
        else
        {
             umcRes = UMC_ERR_FAILED;
        }
    }
    return umcRes;
}

#endif  // UMC_ENABLE_DSOUND_AUDIO_RENDER
