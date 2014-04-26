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

#ifndef __UMC_APP_PIPELINE_H__
#define __UMC_APP_PIPELINE_H__

#include "umc_defs.h"
#include "umc_string.h"
#include "umc_splitter.h"
#include "umc_filter_deinterlace.h"
#include "umc_system_context.h"

using namespace UMC;


struct TimeFormatted
{
    TimeFormatted()
    {
        iHours   = 0;
        iMinutes = 0;
        iSeconds = 0;
    }

    Ipp16u iHours;
    Ipp8u  iMinutes;
    Ipp8u  iSeconds;

};

struct Statistic
{
    Statistic()
    {
        Reset();
    }

    void Reset()
    {
        fFrameTime      = 0.0;
        fFrameInitTime  = 0.0;
        fRenderTime     = 0.0;
        fRenderRate     = 0.0;
        iFramesDecoded  = 0;
        iFramesRendered = 0;
        iFrameNum       = 0;
        iSkippedNum     = 0;
    }

    Ipp64f fDuration;             // stream duration
    Ipp64f fFullDuration;         // stream duration ralative to first frame pts
    Ipp64f fFrameRate;            // frame rate
    Ipp64f fFrameTime;            // frame presentation time
    Ipp64f fFrameInitTime;        // initial frame presentation time
    Ipp64f fRenderTime;           // total time spent for rendering
    Ipp64f fRenderRate;           // estimated rendering rate (including decoding)
    Ipp32u iFramesDecoded;        // number decoded video frames
    Ipp32u iFramesRendered;       // number rendered video frames
    Ipp32u iFrameNum;             // current frame number at rendering
    Ipp32u iSkippedNum;           // skipped frames

};

class BasePipelineParams
{
public:
    DYNAMIC_CAST_DECL_BASE(BasePipelineParams)

    BasePipelineParams()
    {
        m_iFramesLimit = 0;
        m_fFramerate   = 0;
        m_fVolume      = 100;
        m_iTrickMode   = 0;
        m_bFullscreen  = false;
        m_bStep        = false;
        m_bRepeat      = false;
        m_bSync        = true;
        m_bPrintPTS    = false;
        m_bStatistic   = true;

        m_bDeinterlace       = false;
        m_selectedFormat     = NONE;
        m_iSelectedBitDepth  = 0;
        m_selectedSize.width = m_selectedSize.height = 0;
        m_iSplitterFlags     = UNDEF_SPLITTER;
        m_iPrefDataReader    = FILE_DATA_READER;

        m_iVideoDecoderFlags = 0;
        m_iVideoThreads      = 0;
        m_iAudioDecoderFlags = 0;
        m_iAudioThreads      = 0;

        m_iVideoRenderFlags = 0;
        m_iPrefVideoRender  = DEF_VIDEO_RENDER;
        m_iAudioRenderFlags = 0;
        m_iPrefAudioRender  = DEF_AUDIO_RENDER;

        m_deinterlacingType = DEINTERLACING_DUPLICATE;
        m_interpolationType = IPPI_INTER_NN;
        m_bAnaglyph         = false;
        m_bKeepAspect       = true;
        m_iSelectedVideoID  = 0;
        m_iSelectedAudioID  = 0;
        m_initWndSize.width = m_initWndSize.height = 0;
    };
    virtual ~BasePipelineParams() {};

    Ipp32u  m_iFramesLimit;
    Ipp32f  m_fFramerate;
    Ipp32f  m_fVolume;
    Ipp32u  m_iTrickMode;
    bool    m_bFullscreen;
    bool    m_bStep;
    bool    m_bRepeat;
    bool    m_bSync;
    bool    m_bPrintPTS;
    bool    m_bStatistic;

    DString m_sInputFileName;
    DString m_sOutputVideoFile;
    DString m_sOutputAudioFile;
    Ipp32u  m_iPrefDataReader; // Prefered data reader
    Ipp32u  m_iSplitterFlags;

    Ipp32u m_iVideoDecoderFlags;
    Ipp32u m_iVideoThreads;
    Ipp32u m_iAudioDecoderFlags;
    Ipp32u m_iAudioThreads;

    bool              m_bDeinterlace;
    ColorFormat       m_selectedFormat;
    IppiSize          m_selectedSize;
    Ipp32u            m_iSelectedBitDepth;
    VideoData         m_dstVideoData;
    Ipp32u            m_iVideoRenderFlags;
    VideoRenderType   m_iPrefVideoRender; // Prefered video render
    Ipp32u            m_iAudioRenderFlags;
    AudioRenderType   m_iPrefAudioRender; // Prefered audio render
    SysRenderContext  m_renderContext;

    DeinterlacingType m_deinterlacingType;
    Ipp32s            m_interpolationType;
    bool              m_bAnaglyph;
    bool              m_bKeepAspect;
    Ipp32s            m_iSelectedVideoID;
    Ipp32s            m_iSelectedAudioID;
    IppiSize          m_initWndSize;      // initial render window size
};

class BasePipeline
{
public:
    BasePipeline()
    {
        m_pSplitInfo    = NULL;
    }
    virtual ~BasePipeline() {};

    // Initialize playback
    virtual Status Init(BasePipelineParams *pParams) = 0;

    // Finalize playback
    virtual Status Close() = 0;

    // Playback control
    virtual Status Start() = 0;
    virtual Status Stop() = 0;
    virtual Status Pause() = 0;
    virtual Status Resume() = 0;
    // set speed and directon of playback
    virtual Status SetSpeed(Ipp32f fSpeed) = 0;

    virtual bool IsPaused() = 0;
    virtual bool IsPlaying() = 0;

    //  Stream position control, in pts
    virtual Ipp64u GetStreamSize() = 0;
    virtual Ipp64f GetPosition() = 0;
    virtual Ipp64f SetPosition(Ipp64f fPos) = 0;

    //  Volume control
    virtual Ipp32f GetVolume() = 0;
    virtual Ipp32f SetVolume(Ipp32f fVolume) = 0;
    virtual void   Mute() = 0;

    virtual void GetParams(BasePipelineParams *pParams) = 0;
    virtual void GetStat(Statistic &stat) = 0;

public:
    SplitterInfo *m_pSplitInfo;

protected:
    Statistic   m_stat;
};

#endif
