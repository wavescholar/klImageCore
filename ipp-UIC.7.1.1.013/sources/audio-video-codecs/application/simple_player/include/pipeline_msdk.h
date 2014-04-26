/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __PIPELINE_MSDK_H__
#define __PIPELINE_MSDK_H__

#include "vm_time.h"

#include "umc_structures.h"
#include "umc_thread.h"
#include "umc_event.h"
#include "umc_mutex.h"
#include "umc_data_reader.h"
#include "umc_splitter.h"
#include "umc_base_codec.h"
#include "umc_audio_decoder.h"
#include "umc_audio_render.h"
#include "umc_video_decoder.h"
#include "umc_video_render.h"
#include "umc_dual_thread_codec.h"
#include "umc_video_data.h"
#include "umc_event.h"
#include "umc_string.h"
#include "umc_video_processing.h"

#include "umc_app_pipeline.h"

#include "msdk_base_allocator.h"

#include "mfxvideo.h"
#include "mfxvideo++.h"

using namespace UMC;


class MSDKPipelineParams: public BasePipelineParams
{
public:
    DYNAMIC_CAST_DECL(MSDKPipelineParams, BasePipelineParams)

    MSDKPipelineParams()
    {
        m_iMsdkImpl = 0;     // 0 - auto, 1 - force software, 2 - force hardware
        m_bMsdkMvc  = false;
    }

    Ipp32u  m_iMsdkImpl;
    bool    m_bMsdkMvc;
};

class PipelineMSDK: public BasePipeline
{
public:
    PipelineMSDK();
    virtual ~PipelineMSDK();

    virtual Status Init(BasePipelineParams *pParams);
    virtual Status Close();

    virtual Status Start();
    virtual Status Stop();
    virtual Status Pause();
    virtual Status Resume();
    virtual Status SetSpeed(Ipp32f fSpeed);

    virtual bool IsPaused() { return m_bPaused; }
    virtual bool IsPlaying();

    //  Stream position control
    virtual Ipp64u GetStreamSize();
    virtual Ipp64f GetPosition();
    virtual Ipp64f SetPosition(Ipp64f fPos);

    virtual Ipp32f SetVolume(Ipp32f fVolume)
    {
        if(m_pAudioRender)
            return m_pAudioRender->SetVolume(fVolume);
        return 0;
    }
    virtual Ipp32f GetVolume()
    {
        if(m_pAudioRender)
            return m_pAudioRender->GetVolume();
        return 0;
    }
    virtual void Mute()
    {
        if (m_bIsMuted)
            SetVolume(m_fCurrentAudioVolume);
        else
        {
            m_fCurrentAudioVolume = GetVolume();
            SetVolume(0.);
        }
        m_bIsMuted = !m_bIsMuted;
    }

    virtual void GetParams(BasePipelineParams *pParams) { *pParams = m_params; }
    virtual void GetStat(Statistic &stat);

    void   WaitForStop();

protected:
    static Ipp32u VM_THREAD_CALLCONVENTION SyncThreadProc(void* pvParam);
    static Ipp32u VM_THREAD_CALLCONVENTION AudioThreadProc(void* pvParam);
    static Ipp32u VM_THREAD_CALLCONVENTION VideoThreadProc(void* pvParam);

    void SyncProc();
    void AudioProc();
    void VideoProc();

    MSDKPipelineParams  m_params;

    Event m_videoWait;
    Event m_StepEvent;
    Mutex m_MutAccess;

    Thread m_SyncThread;
    Thread m_AudioThread;
    Thread m_VideoThread;

    DataReader   *m_pDataReader;
    Splitter     *m_pSplitter;
    AudioRender  *m_pAudioRender;
    VideoRender  *m_pVideoRender;

    //  Merge it to single class!!!
    AudioDecoder      *m_pAudioDecoder;
    DualThreadedCodec *m_pDTAudioCodec;
    MediaBuffer       *m_pMediaBuffer;

    VideoProcessing    m_videoProcessor;

    MFXVideoDECODE         *m_pVideoDecoder;
    mfxFrameSurface1       *m_pSurfaces;        // frames array
    MFXFrameAllocator      *m_pAllocator;
    mfxAllocatorParams     *m_pAllocatorParams;
    mfxFrameAllocResponse   m_response;         // memory allocation response for decoder
    mfxVideoParam           m_videoParams;
    MFXVideoSession         m_session;
    bool                    m_bMVCBufferAllocated;
    bool                    m_bD3DAlloc;        // use d3d surfaces
    bool                    m_bExternalAlloc;   // use memory allocator as external for Media SDK

    VideoStreamInfo   *m_pVideoInfo;
    AudioStreamInfo   *m_pAudioInfo;
    Ipp32s             m_iAudioTrack;
    Ipp32s             m_iVideoTrack;

    Ipp32f             m_fCurrentAudioVolume;
    volatile bool      m_bStopFlag;
    volatile bool      m_bAudioPlaying;
    volatile bool      m_bVideoPlaying;
    volatile bool      m_bPaused;
    volatile bool      m_bIsMuted;

    bool               m_bExtFrameRate;
    bool               m_bSync;
    vm_tick            m_iFreq;
};

#endif
