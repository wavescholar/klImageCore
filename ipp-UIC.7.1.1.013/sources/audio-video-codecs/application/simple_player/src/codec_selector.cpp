/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "codec_selector.h"

#ifdef UMC_ENABLE_FILE_READER
#include "umc_file_reader.h"
#endif

// decoders video
#ifdef UMC_ENABLE_H264_VIDEO_DECODER
#include "umc_h264_video_decoder.h"
#endif
#ifdef UMC_ENABLE_MPEG2_VIDEO_DECODER
#include "umc_mpeg2_video_decoder.h"
#endif
#ifdef UMC_ENABLE_MPEG4_VIDEO_DECODER
#include "umc_mpeg4_video_decoder.h"
#endif
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER
#include "umc_vc1_video_decoder.h"
#endif
#ifdef UMC_ENABLE_MJPEG_VIDEO_DECODER
#include "umc_mjpeg_video_decoder.h"
#endif

// decoders audio
#ifdef UMC_ENABLE_MP3_AUDIO_DECODER
#include "umc_mp3_decoder.h"
#endif
#ifdef UMC_ENABLE_AAC_AUDIO_DECODER
#include "umc_aac_decoder.h"
#endif
#ifdef UMC_ENABLE_AC3_AUDIO_DECODER
#include "umc_ac3_decoder.h"
#endif

// splitters
#include "umc_wave_pure_splitter.h"
#include "umc_mjpeg_pure_splitter.h"
#ifdef UMC_ENABLE_AVI_SPLITTER
#include "umc_avi_splitter.h"
#endif
#ifdef UMC_ENABLE_MPEG2_SPLITTER
#include "umc_threaded_demuxer.h"
#endif
#ifdef UMC_ENABLE_MP4_SPLITTER
#include "umc_mp4_splitter.h"
#endif
#ifdef UMC_ENABLE_VC1_SPLITTER
#include "umc_vc1_splitter.h"
#endif

// renders video
#include "null_video_render.h"
#ifdef UMC_ENABLE_FW_VIDEO_RENDER
#include "fw_video_render.h"
#endif
#ifdef UMC_ENABLE_GDI_VIDEO_RENDER
#include "gdi_video_render.h"
#endif
#ifdef UMC_ENABLE_OPENGL_VIDEO_RENDER
#include "opengl_video_render.h"
#endif

// renders audio
#include "null_audio_render.h"
#ifdef UMC_ENABLE_FW_AUDIO_RENDER
#include "fw_audio_render.h"
#endif
#ifdef UMC_ENABLE_WINMM_AUDIO_RENDER
#include "winmm_render.h"
#endif
#ifdef UMC_ENABLE_DSOUND_AUDIO_RENDER
#include "directsound_render.h"
#endif
#ifdef UMC_ENABLE_OSS_AUDIO_RENDER
#include "oss_audio_render.h"
#endif

using namespace UMC;


Status InitDataReader(BasePipelineParams *pParams, DataReader **ppDataReader)
{
    DataReaderParams *pDataReaderParams = NULL;
    DataReader       *pDataReader       = *ppDataReader;

    if(!pParams || !ppDataReader)
        return UMC_ERR_NULL_PTR;

    UMC_DELETE(pDataReader);

    switch(pParams->m_iPrefDataReader)
    {
#ifdef UMC_ENABLE_FILE_READER
    case FILE_DATA_READER:
        pDataReader = (DataReader*)new FileReader();
        pDataReaderParams = (FileReaderParams*)new FileReaderParams();

        ((FileReaderParams*)pDataReaderParams)->m_sFileName = pParams->m_sInputFileName;
        break;
#endif

    default:
        return UMC_ERR_UNSUPPORTED;
    }

    if(!pDataReader || !pDataReaderParams)
    {
        UMC_DELETE(pDataReader);
        UMC_DELETE(pDataReaderParams); 
        return UMC_ERR_ALLOC;
    }

    if(UMC_OK != pDataReader->Init(pDataReaderParams))
    {
        UMC_DELETE(pDataReader);
        UMC_DELETE(pDataReaderParams);
        return UMC_ERR_FAILED;
    }

    UMC_DELETE(pDataReaderParams);

    *ppDataReader = pDataReader;

    return UMC_OK;
}

Status InitSplitter(BasePipelineParams *pParams, DataReader *pDataReader, Splitter **ppSplitter)
{
    SystemStreamType type;
    SplitterParams   splitterParams;
    Splitter        *pSplitter = *ppSplitter;

    if(!pParams || !pDataReader || !ppSplitter)
        return UMC_ERR_NULL_PTR;

    UMC_DELETE(pSplitter);

    type = Splitter::GetStreamType(pDataReader);

    switch(type)
    {
    case MJPEG_STREAM:
        pSplitter = (Splitter*)new MJPEGSplitter();
        break;

#ifdef UMC_ENABLE_MPEG2_SPLITTER
    case WAVE_STREAM:
    case H264_PURE_VIDEO_STREAM:
    case MPEGx_SYSTEM_STREAM:
        pSplitter = (Splitter*)new ThreadedDemuxer();
        break;
#endif

#ifdef UMC_ENABLE_MP4_SPLITTER
    case MP4_ATOM_STREAM:
        pSplitter = (Splitter*)new MP4Splitter();
        break;
#endif

#ifdef UMC_ENABLE_AVI_SPLITTER
    case AVI_STREAM:
        pSplitter = (Splitter*)new AVISplitter();
        break;
#endif

#ifdef UMC_ENABLE_VC1_SPLITTER
    case VC1_PURE_VIDEO_STREAM:
        pSplitter = (Splitter*)new VC1Splitter();
        break;
#endif

    default:
        return UMC_ERR_UNSUPPORTED;
    }

    if(!pSplitter)
        return UMC_ERR_ALLOC;

    splitterParams.m_pDataReader = pDataReader;
    splitterParams.m_iFlags      = pParams->m_iSplitterFlags;

    if(UMC_OK != pSplitter->Init(&splitterParams))
    {
        UMC_DELETE(pSplitter);
        return UMC_ERR_FAILED;
    }

    pSplitter->Run();

    *ppSplitter = pSplitter;

    return UMC_OK;
}

Status InitVideoDecoder(BasePipelineParams *pParams, VideoStreamInfo *pVideoInfo, MediaData *pDecSpec, BaseCodec *pPostProcessing, VideoDecoder **ppVideoDecoder)
{
    VideoDecoderParams *pVideoDecParams = NULL;
    VideoDecoder       *pVideoDecoder   = *ppVideoDecoder;

    if(!pParams || !ppVideoDecoder || !pVideoInfo)
        return UMC_ERR_NULL_PTR;

    UMC_DELETE(pVideoDecoder);

    switch(pVideoInfo->streamType)
    {
#ifdef UMC_ENABLE_MPEG2_VIDEO_DECODER
    case MPEG1_VIDEO:
    case MPEG2_VIDEO:
        pVideoDecoder   = (VideoDecoder*)new MPEG2VideoDecoder();
        pVideoDecParams = (VideoDecoderParams*)new VideoDecoderParams();
        break;
#endif

#ifdef UMC_ENABLE_MPEG4_VIDEO_DECODER
    case MPEG4_VIDEO:
        pVideoDecoder   = (VideoDecoder*)new MPEG4VideoDecoder();
        pVideoDecParams = (VideoDecoderParams*)new VideoDecoderParams();
        break;
#endif

#ifdef UMC_ENABLE_H264_VIDEO_DECODER
    case H264_VIDEO:
        pVideoDecoder   = (VideoDecoder*)new H264VideoDecoder();
        pVideoDecParams = (VideoDecoderParams*)new H264VideoDecoderParams();
        break;
#endif

#ifdef UMC_ENABLE_VC1_VIDEO_DECODER
    case VC1_VIDEO:
    case WMV_VIDEO:
        pVideoDecoder   = (VideoDecoder*)new VC1VideoDecoder();
        pVideoDecParams = (VideoDecoderParams*)new VC1VideoDecoderParams();
        break;
#endif

#ifdef UMC_ENABLE_MJPEG_VIDEO_DECODER
    case MJPEG_VIDEO:
        pVideoDecoder   = (VideoDecoder*)(new MJPEGVideoDecoder());
        pVideoDecParams = (VideoDecoderParams*)new VideoDecoderParams();
        break;
#endif

    default:
        return UMC_ERR_UNSUPPORTED;
    }

    if(!pVideoDecoder || !pVideoDecParams)
    {
        UMC_DELETE(pVideoDecoder);
        UMC_DELETE(pVideoDecParams); 
        return UMC_ERR_ALLOC;
    }

    if(pDecSpec && pDecSpec->GetDataSize())
        pVideoDecParams->m_pData = pDecSpec;

    pVideoDecParams->m_info               = *pVideoInfo;
    pVideoDecParams->m_info.streamType    =  pVideoInfo->streamType;
    pVideoDecParams->m_info.streamSubtype =  pVideoInfo->streamSubtype;
    pVideoDecParams->m_iThreads           =  pParams->m_iVideoThreads;

    pVideoDecParams->m_pPostProcessor = pPostProcessing;
    pVideoDecParams->m_iFlags = pParams->m_iVideoDecoderFlags;

    if(UMC_OK != pVideoDecoder->Init(pVideoDecParams))
    {
        UMC_DELETE(pVideoDecoder);
        UMC_DELETE(pVideoDecParams);
        return UMC_ERR_FAILED;
    }

    UMC_DELETE(pVideoDecParams);

    *ppVideoDecoder = pVideoDecoder;

    return UMC_OK;
}

Status InitVideoRender(BasePipelineParams *pParams, VideoStreamInfo *pVideoInfo, VideoRender **ppVideoRender)
{
    VideoRenderParams *pVideoRendParams = NULL;
    VideoRender       *pVideoRender     = *ppVideoRender;

    if(!pParams || !ppVideoRender)
        return UMC_ERR_NULL_PTR;

    UMC_DELETE(pVideoRender);

    switch(pParams->m_iPrefVideoRender)
    {
#ifdef UMC_ENABLE_GDI_VIDEO_RENDER
    case GDI_VIDEO_RENDER:
        pVideoRender     = (VideoRender*)new GDIVideoRender();
        pVideoRendParams = (VideoRenderParams*)new GDIVideoRenderParams();
        ((GDIVideoRenderParams*)pVideoRendParams)->m_context = pParams->m_renderContext;
        break;
#endif

#ifdef UMC_ENABLE_OPENGL_VIDEO_RENDER
    case OPENGL_VIDEO_RENDER:
        pVideoRender     = (VideoRender*)new OpenGLVideoRender();
        pVideoRendParams = (VideoRenderParams*)new OpenGLVideoRenderParams();
        ((OpenGLVideoRenderParams*)pVideoRendParams)->m_context = pParams->m_renderContext;
        break;
#endif

#ifdef UMC_ENABLE_FW_VIDEO_RENDER
    case FW_VIDEO_RENDER:
        pVideoRender     = (VideoRender*)new FWVideoRender();
        pVideoRendParams = (VideoRenderParams*)new FWVideoRenderParams();
        ((FWVideoRenderParams*)pVideoRendParams)->m_sFileName = pParams->m_sOutputVideoFile;
        break;
#endif

    default:
        pVideoRender     = (VideoRender*)new NULLVideoRender();
        pVideoRendParams = (VideoRenderParams*)new NULLVideoRenderParams();
    }

    if(!pVideoRender || !pVideoRendParams)
    {
        UMC_DELETE(pVideoRender);
        UMC_DELETE(pVideoRendParams);
        return UMC_ERR_ALLOC;
    }

    pParams->m_dstVideoData.m_colorFormat = pVideoRender->CheckColorFormat(pParams->m_dstVideoData.m_colorFormat);

    pParams->m_iSelectedBitDepth = pVideoRender->CheckBitDepth(pParams->m_iSelectedBitDepth);
    if(pParams->m_iSelectedBitDepth)
        pParams->m_dstVideoData.SetBitDepth(pParams->m_iSelectedBitDepth);

    // update planes info
    pParams->m_dstVideoData.MapImage();
    pVideoRendParams->m_videoData.Init(&pParams->m_dstVideoData);

    pVideoRendParams->m_iFlags = pParams->m_iVideoRenderFlags;

    if(UMC_OK != pVideoRender->Init(pVideoRendParams))
    {
        UMC_DELETE(pVideoRender);
        UMC_DELETE(pVideoRendParams);
        return UMC_ERR_FAILED;
    }

    UMC_DELETE(pVideoRendParams);

    *ppVideoRender = pVideoRender;

    return UMC_OK;
}

Status InitAudioDecoder(BasePipelineParams *pParams, AudioStreamInfo *pAudioInfo, MediaData *pDecSpec, AudioDecoder **ppAudioDecoder)
{
    AudioDecoderParams *pAudioDecParams = NULL;
    AudioDecoder       *pAudioDecoder   = *ppAudioDecoder;

    if(!pParams || !ppAudioDecoder || !pAudioInfo)
        return UMC_ERR_NULL_PTR;

    UMC_DELETE(pAudioDecoder);

    switch(pAudioInfo->streamType)
    {
#ifdef UMC_ENABLE_MP3_AUDIO_DECODER
    case MP1L1_AUDIO:
    case MP1L2_AUDIO:
    case MP1L3_AUDIO:
    case MP2L1_AUDIO:
    case MP2L2_AUDIO:
    case MP2L3_AUDIO:
    case MPEG1_AUDIO:
    case MPEG2_AUDIO:
        pAudioDecoder   = (AudioDecoder*)new MP3Decoder();
        pAudioDecParams = (AudioDecoderParams*)new MP3DecoderParams();
        break;
#endif

#ifdef UMC_ENABLE_AAC_AUDIO_DECODER
    case AAC_AUDIO:
    case AAC_MPEG4_STREAM:
        pAudioDecoder   = (AudioDecoder*)new AACDecoder();
        pAudioDecParams = (AudioDecoderParams*)new AACDecoderParams();
        break;
#endif

#ifdef UMC_ENABLE_AC3_AUDIO_DECODER
    case AC3_AUDIO:
        pAudioDecoder   = (AudioDecoder*)new AC3Decoder();
        pAudioDecParams = (AudioDecoderParams*)new AC3DecoderParams();
        break;
#endif

    case PCM_AUDIO:
    case LPCM_AUDIO:
        pAudioDecoder = NULL;
        return UMC_OK;

    case UNDEF_AUDIO:
    case TWINVQ_AUDIO:
    default:
        return UMC_ERR_UNSUPPORTED;
    }

    if(!pAudioDecoder || !pAudioDecParams)
    {
        UMC_DELETE(pAudioDecoder);
        UMC_DELETE(pAudioDecParams);
        return UMC_ERR_ALLOC;
    }

    pAudioDecParams->m_iFlags   = pParams->m_iAudioDecoderFlags;
    pAudioDecParams->m_iThreads = pParams->m_iAudioThreads;

    if(UMC_OK != pAudioDecoder->Init(pAudioDecParams))
    {
        UMC_DELETE(pAudioDecoder);
        UMC_DELETE(pAudioDecParams);
        return UMC_ERR_FAILED;
    }

    UMC_DELETE(pAudioDecParams);

    *ppAudioDecoder = pAudioDecoder;

    return UMC_OK;
}

Status InitDTAudioDecoder(BasePipelineParams *pParams, AudioStreamInfo *pAudioInfo, MediaData *pDecSpec, MediaBuffer **ppMediaBuffer, AudioDecoder **ppAudioDecoder, DualThreadedCodec **ppDTAudioCodec)
{
    DualThreadedCodec    *pDTAudioCodec = *ppDTAudioCodec;
    AudioDecoder         *pAudioDecoder = *ppAudioDecoder;
    MediaBuffer          *pMediaBuffer  = *ppMediaBuffer;
    AudioDecoderParams    audioParams;
    MediaBufferParams     bufferParams;
    DualThreadCodecParams dtCodecParams;
    Status status = UMC_OK;

    if(!pParams || !ppAudioDecoder || !ppDTAudioCodec || !ppMediaBuffer || !pAudioInfo)
        return UMC_ERR_NULL_PTR;

    status = InitAudioDecoder(pParams, pAudioInfo, pDecSpec, &pAudioDecoder);
    if(status != UMC_OK)
        return status;
    if(!pAudioDecoder) // PCM audio
        return UMC_OK;

    UMC_DELETE(pDTAudioCodec);

    pDTAudioCodec = new DualThreadedCodec();
    if(!pDTAudioCodec)
        return UMC_ERR_ALLOC;

    // due to nature of AAC bitstream wrapped into MP4 files sample buffer is required
    if(pAudioInfo->streamType != AAC_MPEG4_STREAM && pAudioInfo->streamType != VORBIS_AUDIO)
        pMediaBuffer = (MediaBuffer*)new LinearBuffer();
    else
        pMediaBuffer = (MediaBuffer*)new SampleBuffer();

    if(!pMediaBuffer)
    {
        UMC_DELETE(pDTAudioCodec);
        return UMC_ERR_ALLOC;
    }

    audioParams.m_info            = *pAudioInfo;
    audioParams.m_pData           =  pDecSpec;
    bufferParams.m_numberOfFrames =  100;

    if(pDecSpec)
        bufferParams.m_prefInputBufferSize = (pDecSpec->GetDataSize() < 188) ? 188: pDecSpec->GetDataSize();
    else
        bufferParams.m_prefInputBufferSize = 2304;

    dtCodecParams.m_pCodec             = pAudioDecoder;
    dtCodecParams.m_pMediaBuffer       = pMediaBuffer;
    dtCodecParams.m_pCodecInitParams   = &audioParams;
    dtCodecParams.m_pMediaBufferParams = &bufferParams;

    status = pDTAudioCodec->InitCodec(&dtCodecParams);
    if(status != UMC_OK)
    {
        UMC_DELETE(pAudioDecoder);
        UMC_DELETE(pDTAudioCodec);
        UMC_DELETE(pMediaBuffer);
        return UMC_ERR_FAILED;
    }

    *ppAudioDecoder = pAudioDecoder;
    *ppDTAudioCodec = pDTAudioCodec;
    *ppMediaBuffer  = pMediaBuffer;

    return status;
}

Status InitAudioRender(BasePipelineParams *pParams, AudioStreamInfo *pAudioInfo, AudioRender **ppAudioRender)
{
    AudioRenderParams *pAudioRendParams = NULL;
    AudioRender       *pAudioRender     = *ppAudioRender;

    if(!pParams || !ppAudioRender || !pAudioInfo)
        return UMC_ERR_NULL_PTR;

    UMC_DELETE(pAudioRender);

    switch(pParams->m_iPrefAudioRender)
    {
#ifdef UMC_ENABLE_DSOUND_AUDIO_RENDER
    case DSOUND_AUDIO_RENDER:
        pAudioRender     = (AudioRender*)new DSoundAudioRender();
        pAudioRendParams = (AudioRenderParams*)new AudioRenderParams();
        break;
#endif

#ifdef UMC_ENABLE_WINMM_AUDIO_RENDER
    case WINMM_AUDIO_RENDER:
        pAudioRender     = (AudioRender*)new WinMMAudioRender();
        pAudioRendParams = (AudioRenderParams*)new AudioRenderParams();
        break;
#endif

#ifdef UMC_ENABLE_OSS_AUDIO_RENDER
    case OSS_AUDIO_RENDER:
        pAudioRender     = (AudioRender*)new OSSAudioRender();
        pAudioRendParams = (AudioRenderParams*)new AudioRenderParams();
        break;
#endif

#ifdef UMC_ENABLE_FW_AUDIO_RENDER
    case FW_AUDIO_RENDER:
        pAudioRender     = (AudioRender*)new FWAudioRender();
        pAudioRendParams = (AudioRenderParams*)new FWAudioRenderParams();
        ((FWAudioRenderParams*)pAudioRendParams)->sFileName = pParams->m_sOutputAudioFile;
        break;
#endif

    default:
        pAudioRender     = (AudioRender*)new NULLAudioRender();
        pAudioRendParams = (AudioRenderParams*)new NULLAudioRenderParams();
    }

    if(!pAudioRender || !pAudioRendParams)
    {
        UMC_DELETE(pAudioRender);
        UMC_DELETE(pAudioRendParams);
        return UMC_ERR_ALLOC;
    }

    pAudioRendParams->m_info        = *pAudioInfo;
    pAudioRendParams->m_pSysContext = &pParams->m_renderContext;

    if(UMC_OK != pAudioRender->Init(pAudioRendParams))
    {
        UMC_DELETE(pAudioRender);
        UMC_DELETE(pAudioRendParams);
        return UMC_ERR_FAILED;
    }

    UMC_DELETE(pAudioRendParams);

    *ppAudioRender = pAudioRender;

    return UMC_OK;
}
