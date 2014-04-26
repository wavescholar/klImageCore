/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include <math.h>

#include "null_audio_render.h"

#include "codec_selector.h"
#include "pipeline.h"

#include "ipps.h"

using namespace UMC;


#define TASK_SWITCH()   vm_time_sleep(0);
#define REPOSITION_AVSYNC_PRECISION 0.99
#define DEF_TIME_TO_SLEEP 5
#define SKIP_FRAME_TOLERENCE 7

Pipeline::Pipeline()
{
    m_pSplitInfo        = NULL;
    m_pDataReader       = NULL;
    m_pSplitter         = NULL;
    m_pAudioDecoder     = NULL;
    m_pDTAudioCodec     = NULL;
    m_pMediaBuffer      = NULL;
    m_pAudioRender      = NULL;
    m_pVideoDecoder     = NULL;
    m_pVideoRender      = NULL;
    m_bStopFlag         = false;
    m_bAudioPlaying     = false;
    m_bVideoPlaying     = false;
    m_bPaused           = false;
    m_bSync             = false;
    m_bExtFrameRate     = false;
    m_pVideoInfo        = NULL;
    m_pAudioInfo        = NULL;
    m_iAudioTrack       = -1;
    m_iVideoTrack       = -1;

    m_iFreq               = vm_time_get_frequency();
    m_bIsMuted            = false;
    m_fCurrentAudioVolume = 0.;
}

Pipeline::~Pipeline()
{
    Close();
}

Status Pipeline::Init(BasePipelineParams *pParams)
{
    Status umcRes = UMC_OK;

    if(!pParams)
        return UMC_ERR_NULL_PTR;

    m_params = *pParams;

    umcRes = InitDataReader(&m_params, &m_pDataReader);
    if(UMC_OK != umcRes)
        return umcRes;

    umcRes = InitSplitter(&m_params, m_pDataReader, &m_pSplitter);
    if(UMC_OK != umcRes)
        return umcRes;

    umcRes = m_pSplitter->GetInfo(&m_pSplitInfo);
    if(UMC_OK != umcRes)
        return umcRes;

    for(Ipp32u i = 0; i < m_pSplitInfo->m_iTracks; i++)
    {
        if(m_pSplitInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO)
        {
            if(m_pSplitInfo->m_ppTrackInfo[i]->m_bEnabled && (m_params.m_iSelectedVideoID == (Ipp32s)m_pSplitInfo->m_ppTrackInfo[i]->m_iPID))
                m_iVideoTrack = i;
        }
        else if(m_pSplitInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_AUDIO)
        {
            if(m_pSplitInfo->m_ppTrackInfo[i]->m_bEnabled && (m_params.m_iSelectedAudioID == (Ipp32s)m_pSplitInfo->m_ppTrackInfo[i]->m_iPID))
                m_iAudioTrack = i;
        }
    }

    for(Ipp32u i = 0; i < m_pSplitInfo->m_iTracks; i++)
    {
        if(m_pSplitInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO)
        {
            if(m_iVideoTrack != -1 && m_iVideoTrack != (Ipp32s)i)
                continue;
            if(m_pSplitInfo->m_ppTrackInfo[i]->m_bEnabled)
            {
                m_pVideoInfo  = (VideoStreamInfo*)m_pSplitInfo->m_ppTrackInfo[i]->m_pStreamInfo;
                m_iVideoTrack = i;
            }
        }
        else if(m_pSplitInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_AUDIO)
        {
            if(m_iAudioTrack != -1 && m_iAudioTrack != (Ipp32s)i)
                continue;
            if(m_pSplitInfo->m_ppTrackInfo[i]->m_bEnabled)
            {
                m_pAudioInfo  = (AudioStreamInfo*)m_pSplitInfo->m_ppTrackInfo[i]->m_pStreamInfo;
                m_iAudioTrack = i;
            }
        }
    }

    if(m_pAudioInfo)
    {
        // Get splitter some time to read out at least one audio frame
        vm_time_sleep(0);

        umcRes = InitDTAudioDecoder(&m_params, m_pAudioInfo, m_pSplitInfo->m_ppTrackInfo[m_iAudioTrack]->m_pHeader, &m_pMediaBuffer, &m_pAudioDecoder, &m_pDTAudioCodec);
        if(UMC_ERR_INVALID_STREAM == umcRes || UMC_ERR_UNSUPPORTED == umcRes)
        {
            m_pSplitter->SetTrackState(m_iAudioTrack, TRACK_DISABLED); // disable unsupported track
            m_pAudioInfo  = NULL;
            m_iAudioTrack = -1;
            umcRes = UMC_OK;
        }
        else if(UMC_OK != umcRes)
            return umcRes;

        if(m_pAudioInfo)
        {
            umcRes = InitAudioRender(&m_params, m_pAudioInfo, &m_pAudioRender);
            if(UMC_OK != umcRes)
            {
                vm_string_printf(VM_STRING("Audio render initialization problem, status = %d\n"), umcRes);
                return umcRes;
            }
            m_params.m_iPrefAudioRender = m_pAudioRender->GetRenderType();
        }
    }

    if(m_pVideoInfo)
    {
        VideoColorConversionParams vccParams;
        VideoDepthConversionParams vdcParams;
        VideoDeinterlaceParams      vdParams;
        VideoResizeParams           vrParams;
        VideoAnaglyphParams         vaParams;
        MediaData *pDecHeader = m_pSplitInfo->m_ppTrackInfo[m_iVideoTrack]->m_pHeader;

        if(m_pSplitInfo->m_ppTrackInfo[m_iVideoTrack]->m_pHeaderExt)
        {
            if(m_pVideoInfo->streamSubtype == MULTIVIEW_VIDEO)
                vm_string_printf(VM_STRING("Warning: stream multiview data is not supported\n"));
            else if(m_pVideoInfo->streamSubtype == SCALABLE_VIDEO)
                vm_string_printf(VM_STRING("Warning: stream scalability data is not supported\n"));
        }

        vdParams.m_bEnabled          = m_params.m_bDeinterlace;
        vdParams.m_deinterlacingType = m_params.m_deinterlacingType;
        vrParams.m_interpolationType = m_params.m_interpolationType;
        vaParams.m_bEnabled          = m_params.m_bAnaglyph;

        m_videoProcessor.AddFilter(FILTER_DEPTH_CONVERTER, (BaseCodecParams*)&vdcParams);
        m_videoProcessor.AddFilter(FILTER_DEINTERLACER,    (BaseCodecParams*)&vdParams);
        m_videoProcessor.AddFilter(FILTER_COLOR_CONVERTER, (BaseCodecParams*)&vccParams);
        m_videoProcessor.AddFilter(FILTER_ANAGLYPHER,      (BaseCodecParams*)&vaParams);
        m_videoProcessor.AddFilter(FILTER_RESIZER,         (BaseCodecParams*)&vrParams);

        umcRes = InitVideoDecoder(&m_params, m_pVideoInfo, pDecHeader, &m_videoProcessor, &m_pVideoDecoder);
        if(UMC_ERR_INVALID_STREAM == umcRes || UMC_ERR_UNSUPPORTED == umcRes)
        {
            m_pSplitter->SetTrackState(m_iVideoTrack, TRACK_DISABLED); // disable unsupported track
            m_pVideoInfo  = NULL;
            m_iVideoTrack = -1;
            umcRes = UMC_OK;
        }
        else if(UMC_OK != umcRes)
            return umcRes;

        if(m_pVideoInfo)
        {
            if(m_params.m_fFramerate == 0)
                m_params.m_fFramerate = (Ipp32f)m_pVideoInfo->fFramerate;
            else
                m_bExtFrameRate = true;

            m_params.m_dstVideoData.Init(&m_pVideoInfo->videoInfo);

            if(m_params.m_selectedSize.width && m_params.m_selectedSize.height)
            {
                m_params.m_dstVideoData.m_iWidth  = m_params.m_selectedSize.width;
                m_params.m_dstVideoData.m_iHeight = m_params.m_selectedSize.height;
            }
            if(m_params.m_bKeepAspect)
            {
                Ipp32f fScale = IPP_MIN((Ipp32f)m_params.m_dstVideoData.m_iWidth/m_pVideoInfo->videoInfo.m_iWidth, (Ipp32f)m_params.m_dstVideoData.m_iHeight/m_pVideoInfo->videoInfo.m_iHeight);
                m_params.m_dstVideoData.m_iWidth  = (Ipp32u)UMC_ROUND_32U(fScale*m_pVideoInfo->videoInfo.m_iWidth);
                m_params.m_dstVideoData.m_iHeight = (Ipp32u)UMC_ROUND_32U(fScale*m_pVideoInfo->videoInfo.m_iHeight);

                // correct DAR
                if(m_pVideoInfo->videoInfo.m_iSAWidth != m_pVideoInfo->videoInfo.m_iSAHeight)
                {
                    fScale = (Ipp32f)m_pVideoInfo->videoInfo.m_iSAWidth/m_pVideoInfo->videoInfo.m_iSAHeight;
                    if(fScale > 1)
                        m_params.m_dstVideoData.m_iWidth = UMC_ROUND_32U(m_params.m_dstVideoData.m_iWidth*fScale);
                    else
                        m_params.m_dstVideoData.m_iHeight = UMC_ROUND_32U(m_params.m_dstVideoData.m_iHeight/fScale);
                    m_params.m_dstVideoData.m_iSAWidth = m_params.m_dstVideoData.m_iSAHeight = 1;
                }
            }

            if(m_params.m_selectedFormat != NONE)
                m_params.m_dstVideoData.m_colorFormat = m_params.m_selectedFormat;

            if(m_params.m_bDeinterlace)
                m_params.m_dstVideoData.m_picStructure = PS_PROGRESSIVE;
            else
            {
                if(m_params.m_dstVideoData.m_picStructure == PS_TOP_FIELD)
                    m_params.m_dstVideoData.m_picStructure = PS_TOP_FIELD_FIRST;
                else if(m_params.m_dstVideoData.m_picStructure == PS_BOTTOM_FIELD)
                    m_params.m_dstVideoData.m_picStructure = PS_BOTTOM_FIELD_FIRST;
            }

            umcRes = InitVideoRender(&m_params, m_pVideoInfo, &m_pVideoRender);
            if(UMC_OK != umcRes)
                return umcRes;

            m_params.m_iPrefVideoRender = m_pVideoRender->GetRenderType();
        }
    }

    umcRes = m_StepEvent.Init(1,1);
    if(UMC_OK == umcRes && m_pVideoInfo)
    {
        if(m_params.m_bStep)
            umcRes = m_StepEvent.Reset();
        else
            umcRes = m_StepEvent.Set();
    }
    else if(UMC_OK != umcRes)
        return umcRes;

    m_bSync      = m_params.m_bSync;
    m_bStopFlag  = false;

    m_stat.Reset();
    m_stat.fFrameRate = m_params.m_fFramerate;

    m_params.m_iSelectedVideoID = m_iVideoTrack;
    m_params.m_iSelectedAudioID = m_iAudioTrack;

    m_pSplitter->Run();

    if(m_pAudioInfo && m_pVideoInfo)
        m_stat.fDuration = ((m_pVideoInfo->fDuration < m_pAudioInfo->fDuration) ? m_pAudioInfo->fDuration: m_pVideoInfo->fDuration);
    else if(m_pVideoInfo)
        m_stat.fDuration = m_pVideoInfo->fDuration;
    else if(m_pAudioInfo)
    {
        if(m_params.m_iAudioRenderFlags & FLAG_AREN_HIDDEN)
            Mute();
        m_stat.fDuration = m_pAudioInfo->fDuration;
    }

    m_videoWait.Init(0, 0);

    return umcRes;
}

Status Pipeline::Close()
{
    m_MutAccess.Lock();

    if(!m_bStopFlag)
    {
        m_bStopFlag = true;
        m_StepEvent.Set();
        m_videoWait.Set();

        if(m_VideoThread.IsValid())
            m_VideoThread.Wait();

        if(m_SyncThread.IsValid())
            m_SyncThread.Wait();

        if(m_AudioThread.IsValid())
            m_AudioThread.Wait();

        if(NULL != m_pSplitter)
            m_pSplitter->Stop();
    }

    UMC_DELETE(m_pVideoRender);
    if(m_pAudioRender)
    {
        m_pAudioRender->Stop();
        UMC_DELETE(m_pAudioRender);
    }
    UMC_DELETE(m_pVideoDecoder);
    UMC_DELETE(m_pDTAudioCodec);
    UMC_DELETE(m_pMediaBuffer);
    UMC_DELETE(m_pAudioDecoder);
    UMC_DELETE(m_pSplitter);
    UMC_DELETE(m_pDataReader);

    m_MutAccess.Unlock();
    return UMC_OK;
}

Status Pipeline::Start()
{
    Status umcRes = UMC_OK;

    m_MutAccess.Lock();

    if(!IsPlaying())
    {
        m_bStopFlag = false;
        m_bAudioPlaying = false;
        m_bVideoPlaying = false;
        m_stat.Reset();

        if (UMC_OK == umcRes && m_pVideoInfo)
            umcRes = m_VideoThread.Create(VideoThreadProc, this);

        if (UMC_OK == umcRes && m_pAudioInfo)
            umcRes = m_AudioThread.Create(AudioThreadProc, this);

        if (UMC_OK == umcRes && (NULL != m_pVideoRender))
            umcRes = m_SyncThread.Create(SyncThreadProc, this);

        if (UMC_OK == umcRes)
        {
            vm_time_sleep(1);

            m_SyncThread.SetPriority(VM_THREAD_PRIORITY_HIGHEST);
            m_AudioThread.SetPriority(VM_THREAD_PRIORITY_HIGH);
            m_VideoThread.SetPriority(VM_THREAD_PRIORITY_NORMAL);
        }

        if (UMC_OK != umcRes)
            Stop();
    }

    m_MutAccess.Unlock();
    return umcRes;
}

Status Pipeline::Stop()
{
    Status umcRes = UMC_OK;
    m_MutAccess.Lock();

    m_bStopFlag = true;
    m_StepEvent.Set();
    m_videoWait.Set();

    if(m_VideoThread.IsValid())
        m_VideoThread.Wait();

    if(m_SyncThread.IsValid())
        m_SyncThread.Wait();

    if(m_AudioThread.IsValid())
        m_AudioThread.Wait();

    //Splitter can be stopped only after the video thread has finished
    if(NULL != m_pSplitter)
        umcRes = m_pSplitter->Stop();

    m_MutAccess.Unlock();
    return umcRes;
}

Status Pipeline::Pause()
{
    Status umcRes = UMC_OK;
    m_MutAccess.Lock();

    m_bPaused = true;

    if (!m_AudioThread.IsValid())
        umcRes = m_StepEvent.Pulse();
    else
        umcRes = m_pAudioRender->Pause(true);

    m_MutAccess.Unlock();
    return umcRes;
}

Status Pipeline::Resume()
{
    Status umcRes = UMC_OK;
    m_MutAccess.Lock();

    if (NULL != m_pAudioRender)
        umcRes = m_pAudioRender->Pause(false);

    if (UMC_OK == umcRes)
    {
        m_bPaused = false;
        umcRes = m_StepEvent.Set();
    }

    m_MutAccess.Unlock();
    return umcRes;
}

Status Pipeline::SetSpeed(Ipp32f fSpeed)
{
    fSpeed = fSpeed;
    return UMC_ERR_NOT_IMPLEMENTED;
}

bool Pipeline::IsPlaying()
{
    return m_SyncThread.IsValid() || m_AudioThread.IsValid() || m_VideoThread.IsValid();
}

void Pipeline::GetStat(Statistic &stat)
{
    m_MutAccess.Lock();
    stat = m_stat;
    m_MutAccess.Unlock();
}

void Pipeline::WaitForStop()
{
    //always synchronized stream finishing with SyncProc
    if(m_SyncThread.IsValid())
        m_SyncThread.Wait();
    else
    {
        //there is an exception: pure audio files
        //SyncProc is absent in this case
        if(m_AudioThread.IsValid())
            m_AudioThread.Wait();
    }
}

Ipp64u Pipeline::GetStreamSize()
{
    Ipp64u stRes = 0;
    if (NULL != m_pDataReader)
        stRes = m_pDataReader->GetSize();
    return stRes;
}

Ipp64f Pipeline::GetPosition()
{
    return -1;
}

Ipp64f Pipeline::SetPosition(Ipp64f fPos)
{
    fPos = fPos;
    return -1;
}

void Pipeline::SyncProc()
{
    vm_tick t1 = 0, t2 = 0, t2_prev = 0;
    Ipp64f flip_time = 0.001;
    Ipp64f flip_times[3] = {0.001,0.001,0.001};
    Status umcRes = UMC_OK;
    bool bNoAudioAnyMore = (NULL == m_pAudioRender);
    bool bNoVideoAnyMore = (NULL == m_pVideoRender);
    bool bNullAudioRender = false;
    AudioRender* pStoppedAudioRender = NULL;
    Ipp64f prevVideoPts = 0;
    NULLAudioRenderParams renderParams;

    // Wait until video and audio decoding threads will pass some data to
    // the renders - we can't start synchronization without it
    while (!m_bStopFlag &&
          ((NULL != m_pVideoRender && !m_bVideoPlaying) ||
           (NULL != m_pAudioRender && !m_bAudioPlaying)))
    {   vm_time_sleep(1);   }

    m_stat.iFramesRendered = 0;
    // Continue video data rendering until no more data in audio and video
    // render left
    for (Ipp32s frame_num = 0, skip_window = 0;
         UMC_OK == umcRes && (!bNoAudioAnyMore || !bNoVideoAnyMore) && !m_bStopFlag;
         frame_num++)
    {
        m_stat.iFrameNum = frame_num + m_stat.iSkippedNum;

        // Check next frame PTS if any
        if (!bNoVideoAnyMore)
        {
            Status get_fr_sts = UMC_OK;
            do
            {
                VM_ASSERT(NULL != m_pVideoRender);
                get_fr_sts = m_pVideoRender->GetRenderFrame(&(m_stat.fFrameTime));
                if (UMC_OK != get_fr_sts)
                    vm_time_sleep(DEF_TIME_TO_SLEEP);
            } while (get_fr_sts == UMC_ERR_TIMEOUT && !m_bStopFlag);

            if (m_stat.fFrameTime == -1.0 || get_fr_sts != UMC_OK)
            {
               bNoVideoAnyMore = true;
            }
        }
        if(frame_num == 0)
        {
            prevVideoPts = m_stat.fFrameInitTime = m_stat.fFrameTime;
            if(m_stat.fDuration > 0)
                m_stat.fFullDuration = m_stat.fDuration + m_stat.fFrameInitTime;
        }

        // If we have no more audio but some video or if we have no audio at all
        if (m_bSync && ((NULL == m_pAudioRender ) ||
                        (NULL == pStoppedAudioRender &&
                         m_bAudioPlaying &&
                         bNoAudioAnyMore &&
                        !bNoVideoAnyMore)))
        {
            pStoppedAudioRender = m_pAudioRender;
            m_pAudioRender = new NULLAudioRender(m_stat.fFrameTime);
            if (NULL == m_pAudioRender)
            {
                //  signal error, stop everything
                m_bStopFlag = true;
                umcRes = UMC_ERR_ALLOC;
            }
            else
            {
                // Start time counting
                m_pAudioRender->Pause(false);
                m_bAudioPlaying = true;
                bNullAudioRender = true;
            }
        }


        TASK_SWITCH();

        Ipp64f ft = m_stat.fFrameTime - flip_time;


        // Let's synchronize video to audio if there is some data in the audio
        // render or NULLAudioRender is used
        if (!bNoAudioAnyMore || bNullAudioRender)
        {
            VM_ASSERT(NULL != m_pAudioRender);
            VM_ASSERT(NULL != m_pVideoDecoder);

            Ipp64f dfAudioTime = m_pAudioRender->GetTime();
            dfAudioTime += m_pAudioRender->GetDelay();

            // Wait for the next video frame display time if we have one
            if (!bNoVideoAnyMore && m_bSync)
            {
                if(m_params.m_bPrintPTS)
                    printf("PTS V-A: %.4f - %.4f = %.4f", ft, dfAudioTime, dfAudioTime - ft);

                if(prevVideoPts > ft + 1.0 || prevVideoPts + 1.0 < ft) //PTS jump
                {
                    if(abs(dfAudioTime - ft) > 1.0)
                    {
                        // try to syncronize video and audio after PTS jump
                        if (!bNullAudioRender)
                        {
                            volatile Ipp64f st1;
                            Ipp32u   n = 0;
                            for (st1 = dfAudioTime;
                                st1 > 0 && (abs(st1 - ft) > 1.0) && !m_bStopFlag && n < 100; n++)
                            {
                                vm_time_sleep(DEF_TIME_TO_SLEEP);
                                st1 = m_pAudioRender->GetTime();
                                dfAudioTime = st1;
                            }
                        }
                        else
                        {

                            renderParams.m_InitPTS = m_stat.fFrameTime;
                            m_pAudioRender->SetParams(&renderParams);
                            m_pAudioRender->Pause(false);
                        }
                        if(m_params.m_bPrintPTS)
                            printf(" - pts jump");
                     }
                }

                if (ft > dfAudioTime)
                {
                    skip_window = 0;
                    umcRes = m_pVideoDecoder->ResetSkipCount();
                    if (UMC_ERR_NOT_IMPLEMENTED == umcRes)
                    {    umcRes = UMC_OK;    }

                    volatile Ipp64f st1;
                    Ipp32u   n = 0;

                    for (st1 = dfAudioTime; st1 >= dfAudioTime && ft > st1 && !m_bStopFlag && n < 100 ;n++)
                    {
                        Ipp32f a=0;
                        vm_time_sleep(IPP_MAX(0,IPP_MIN(1,(Ipp32s)((ft-st1)*1000))));
                        st1 = m_pAudioRender->GetTime();
                        a = (Ipp32f) m_pAudioRender->GetDelay();
                        st1 += a;

                        if(m_bPaused)
                        {
                            m_pVideoRender->ShowLastFrame();
                        }
                    }
                }
                else if (ft < dfAudioTime &&
                         (dfAudioTime - ft > (0.7/m_params.m_fFramerate)))
                {
                    if (++skip_window >= SKIP_FRAME_TOLERENCE)
                    {
                        skip_window = 0;
                        umcRes = m_pVideoDecoder->SkipVideoFrame(1);
                        if (UMC_ERR_NOT_IMPLEMENTED == umcRes)
                        {   umcRes = UMC_OK;   }
                    }
                    if(m_params.m_bPrintPTS)
                        printf(" - skip attempt (%d).", skip_window);
                }
                if(m_params.m_bPrintPTS)
                    printf("\n");
            }

            // Stop synchronization efforts and play the rest of the video on
            // maximum speed if we have no more audo samples
            if (-1.0 == dfAudioTime)
            {   bNoAudioAnyMore = true; }
        }
        prevVideoPts = m_stat.fFrameTime;

        // It's time to display next video frame
        if (UMC_OK == umcRes && !bNoVideoAnyMore)
        {
            t1 = vm_time_get_tick();
            umcRes = m_pVideoRender->RenderFrame();
            t2 = vm_time_get_tick();
            m_stat.iFramesRendered++;
            m_videoWait.Set();
        }

        // Update Statistic structure and frame display statistic
        if (UMC_OK == umcRes)
        {
            m_stat.iSkippedNum = m_pVideoDecoder->GetNumOfSkippedFrames();

            Ipp64f this_flip_time =
                    (Ipp64f)(t2-t1)/(Ipp64f)m_iFreq;

            flip_times[0] = flip_times[1];
            flip_times[1] = flip_times[2];
            flip_times[2] = this_flip_time;

            flip_time = (flip_times[0] + flip_times[1] + flip_times[2]) / 3;

            while (VM_TIMEOUT == m_StepEvent.Wait(500))
            {
                m_pVideoRender->ShowLastFrame();
                if (m_bStopFlag)
                {   break;  }
            }

            // ignore the first frame (might be a long wait to be synchronized
            if (1 < m_stat.iFrameNum)
            {
                m_stat.fRenderTime += (Ipp64f)(t2-t2_prev) /
      (Ipp64f)m_iFreq;
                m_stat.fRenderRate =
                    (Ipp64f)(m_stat.iFrameNum - 1) / m_stat.fRenderTime;
            }
            t2_prev = t2;

            TASK_SWITCH();
        }

    }

    UMC_DELETE(pStoppedAudioRender);
}

void Pipeline::VideoProc()
{
    Status     status = UMC_OK;
    MediaData  inData;
    VideoData  outData;
    MediaData *pDataIn = &inData;
    bool       bRenderLocked = false;

    outData.Init(&m_params.m_dstVideoData);

    for(; !m_bStopFlag && UMC_OK == status;)
    {
        // Wait for the free buffer in the video render
        do
        {
            status = m_pVideoRender->LockInputBuffer(&outData);
            if(UMC_OK == status)   // Be aware that video render surface was locked and not released yet
            {
                bRenderLocked = true;
                break;
            }
            // there is only one legal error return value, all other are incorrect.
            else if(!m_bStopFlag && (UMC_ERR_NOT_ENOUGH_BUFFER == status ||UMC_ERR_TIMEOUT == status))
                m_videoWait.Wait();
            else
            {
                vm_string_printf(VM_STRING("Error in video render\n"));
                break;
            }
        } while(!m_bStopFlag);

        // Repeat decode procedure until the decoder will agree to decompress at least one frame
        do
        {
            // Get some more data from the the splitter if we've decoded all data from the previous buffer
            if(pDataIn && (pDataIn->GetDataSize() <= 4 || status == UMC_ERR_NOT_ENOUGH_DATA))
            {
                do
                {
                    status = m_pSplitter->GetNextData(pDataIn, m_iVideoTrack);
                } while(status == UMC_ERR_NOT_ENOUGH_DATA && !m_bStopFlag);
                if(status == UMC_ERR_END_OF_STREAM)
                    pDataIn = NULL; // Ok, here is no more data in the splitter. Let's extract the rest of decoded data from the decoder
                else if(status == UMC_WRN_REPOSITION_INPROGRESS)
                    status = UMC_OK;
                else if(status != UMC_OK)
                {
                    vm_string_printf(VM_STRING("Error in splitter\n"));
                    break;
                }
            }

            status = m_pVideoDecoder->GetFrame(pDataIn, &outData);

        } while (((UMC_ERR_NOT_ENOUGH_DATA == status && pDataIn) || UMC_ERR_SYNC == status || UMC_ERR_INVALID_STREAM == status) && !m_bStopFlag);

        if (UMC_OK != status || m_bStopFlag)
            break;

        if(m_bExtFrameRate)
            outData.m_fPTSStart = m_stat.iFramesDecoded/m_stat.fFrameRate;
        m_stat.iFramesDecoded++;

        // Unlock video render surface
        if (bRenderLocked)
        {
            status = m_pVideoRender->UnLockInputBuffer(&outData);
            bRenderLocked = false;
        }
        m_bVideoPlaying = true;
    }

    if(m_bVideoPlaying == false)
        m_bStopFlag = true;

    if (bRenderLocked)
    {
        outData.m_fPTSStart = -1;
        outData.m_frameType = NONE_PICTURE;
        status = m_pVideoRender->UnLockInputBuffer(&outData, UMC_ERR_END_OF_STREAM);
    }

    m_pVideoRender->Stop();
}

template<class typeSource, class typeMedium>
Status LockInputBuffer(typeSource *pSource, typeMedium *pMedium, bool *pbStop)
{
    Status umcRes;

    do
    {
        umcRes = pSource->LockInputBuffer(pMedium);
        if (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes)
            vm_time_sleep(DEF_TIME_TO_SLEEP);

    } while ((false == *pbStop) &&
             (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes));

    return umcRes;

} // Status LockInputBuffer(typeSource *pSource, typeMedium *pMedium, bool *pbStop)

template<class typeDestination, class typeMedium>
Status LockOutputBuffer(typeDestination *pDestination, typeMedium *pMedium, bool *pbStop)
{
    Status umcRes;

    do
    {
        umcRes = pDestination->LockOutputBuffer(pMedium);
        if (UMC_ERR_NOT_ENOUGH_DATA == umcRes)
            vm_time_sleep(DEF_TIME_TO_SLEEP);

    } while ((false == *pbStop) &&
             (UMC_ERR_NOT_ENOUGH_DATA == umcRes));

    return umcRes;

} // Status LockOutputBuffer(typeDestination *pDestination, typeMedium *pMedium, bool *pbStop)

void Pipeline::AudioProc()
{
    //LOG  (VM_STRING("AudioProc,+"));
    Status umcRes = UMC_OK;
    MediaData ComprData;
    AudioData  UncomprData;
    bool bSplitterIsEmpty = false;
    Ipp64f dfStartTime = 0;
    Ipp32u uiComprSize = 0;
    Ipp32u uiShift = 0;

    // check error(s)
    VM_ASSERT(NULL != m_pSplitter);

    // Continue passing data from the splitter to decoder and from
    // decoder to the render
    UncomprData = m_pAudioInfo->audioInfo;

    while ((false == m_bStopFlag) &&
           (false == bSplitterIsEmpty))
    {
        // 1st step: obtain data from splitter
        do
        {
            umcRes = m_pSplitter->GetNextData(&ComprData, m_iAudioTrack);
            if ((UMC_ERR_NOT_ENOUGH_DATA == umcRes) || (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes))
                vm_time_sleep(5);
        } while ((false == m_bStopFlag) && ((UMC_ERR_NOT_ENOUGH_DATA == umcRes) || (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes)));
        // check error(s) & end of stream
        if (UMC_ERR_END_OF_STREAM == umcRes)
        {
            bSplitterIsEmpty = true;
            ComprData.SetDataSize(0);
        }
        else if (UMC_OK != umcRes)
            break;
        // save data size and data time
        uiComprSize = (Ipp32u) ComprData.GetDataSize();
        dfStartTime = ComprData.m_fPTSStart;

        // decode data and pass them to renderer
        uiShift = 0;
        do
        {
            // 2nd step: compressed data should be passed to the decoder first
            if (m_pDTAudioCodec)
            {
                MediaData buff;

                // get decoder's internal buffer
                umcRes = m_pDTAudioCodec->LockInputBuffer(&buff);
                // check error(s)
                if (UMC_OK != umcRes)
                    break;

                // Copy compressed data to the decoder's buffer
                if (UMC_OK == umcRes)
                {
                    Ipp32u uiDataToCopy = IPP_MIN((Ipp32u) buff.GetBufferSize(), uiComprSize);

                    memcpy(buff.GetDataPointer(),
                           (Ipp8u*)ComprData.GetDataPointer() + uiShift,
                           uiDataToCopy);

                    buff.SetDataSize(uiDataToCopy);
                    buff.m_fPTSStart = dfStartTime;

                    umcRes = m_pDTAudioCodec->UnLockInputBuffer(&buff, (bSplitterIsEmpty) ? (UMC_ERR_END_OF_STREAM) : (UMC_OK));
                    // check error(s)
                    if (UMC_OK != umcRes)
                        break;


                    uiShift += uiDataToCopy;
                    uiComprSize -= uiDataToCopy;
                }
            }

            do
            {
                // wait until audio renderer will free enough internal buffers
                umcRes = LockInputBuffer(m_pAudioRender, &UncomprData, (bool *) &m_bStopFlag);
                // check error(s)
                if (UMC_OK != umcRes)
                {
                   if (!m_bStopFlag)
                      vm_string_printf(VM_STRING("Error in audio render\n"));

                    TASK_SWITCH();
                    break;
                }

                // move decoded data to the renderer

                // brunch for compressed data
                if (m_pDTAudioCodec)
                {
                    UncomprData.SetDataSize(0);
                    vm_tick ullDecTime = vm_time_get_tick();
                    umcRes = m_pDTAudioCodec->GetFrame(&UncomprData);
                    ullDecTime = vm_time_get_tick() - ullDecTime;

                    if (UMC_OK != umcRes)
                        break;
                }
                // branch for PCM data
                else
                {
                    Ipp64f dfStart = 0.0;
                    Ipp64f dfEnd = 0.0;

                    if (0 == uiComprSize)
                        break;

                    Ipp32u uiDataToCopy = (Ipp32u) IPP_MIN(uiComprSize, UncomprData.GetBufferSize());

                    memcpy(UncomprData.GetDataPointer(),
                           ((Ipp8u*)ComprData.GetDataPointer()) + uiShift,
                           uiDataToCopy);
                    UncomprData.SetDataSize(uiDataToCopy);

                    dfStart = ComprData.m_fPTSStart;
                    dfEnd   = ComprData.m_fPTSEnd;
                    Ipp64f dfNorm = (dfEnd - dfStart) / (uiShift + uiComprSize);
                    dfStart += dfNorm * uiShift;
                    dfEnd = dfStart + dfNorm * uiDataToCopy;
                    UncomprData.m_fPTSStart = dfStart;
                    UncomprData.m_fPTSEnd   = dfEnd;

                    uiShift += uiDataToCopy;
                    uiComprSize -= uiDataToCopy;
                }

                // call finalizing function
                if (UncomprData.GetDataSize())
                {
                    umcRes = m_pAudioRender->UnLockInputBuffer(&UncomprData);
                    // check error(s)
                    TASK_SWITCH();
                    if (UMC_OK != umcRes)
                        break;
                    // open SyncProc() only after render starts
                    if (-1. != m_pAudioRender->GetTime())
                        m_bAudioPlaying = true;
                }
            } while (false == m_bStopFlag);
            // check after-cicle error(s)
            if ((UMC_OK != umcRes) &&
                (UMC_ERR_NOT_ENOUGH_DATA != umcRes) &&
                (UMC_ERR_SYNC != umcRes))
                break;

            umcRes = UMC_OK;

        } while ((false == m_bStopFlag) && (0 != uiComprSize));
        // check after-cicle error(s)
        if (UMC_OK != umcRes)
            break;
    }

    // send end of stream to renderer
    if(!m_bStopFlag)
    {
        // wait until audio renderer will free enough intermal buffers
        do
        {
            umcRes = m_pAudioRender->LockInputBuffer(&UncomprData);
            if (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes)
                vm_time_sleep(DEF_TIME_TO_SLEEP);

        } while ((false == m_bStopFlag) &&
                 (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes));
        // check error(s)
        if (UMC_OK == umcRes)
        {
            UncomprData.SetDataSize(0);
            UncomprData.m_fPTSStart = 0;
            umcRes = m_pAudioRender->UnLockInputBuffer(&UncomprData, UMC_ERR_END_OF_STREAM);
        }
    }
}

Ipp32u VM_THREAD_CALLCONVENTION Pipeline::SyncThreadProc(void* pvParam)
{
    VM_ASSERT(NULL != pvParam);
    ((Pipeline*)pvParam)->SyncProc();
    return 0;
}

Ipp32u VM_THREAD_CALLCONVENTION Pipeline::VideoThreadProc(void* pvParam)
{
    VM_ASSERT(NULL != pvParam);
    ((Pipeline*)pvParam)->VideoProc();
    return 0;
}

Ipp32u VM_THREAD_CALLCONVENTION Pipeline::AudioThreadProc(void* pvParam)
{
    VM_ASSERT(NULL != pvParam);
    ((Pipeline*)pvParam)->AudioProc();
    return 0;
}
