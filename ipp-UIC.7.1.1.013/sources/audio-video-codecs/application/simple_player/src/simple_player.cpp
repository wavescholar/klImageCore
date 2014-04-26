//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//

#if (defined _DEBUG && defined WINDOWS)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <memory>

#include "umc_config.h"
#include "umc_parser_cmd.h"
#include "umc_video_processing.h"

#include "umc_app_utils.h"

#include "umc_app_wnd.h"
#include "pipeline.h"
#ifdef UMC_ENABLE_MSDK_INTERFACE
#include "pipeline_msdk.h"
#endif

#include "ippcore.h"
#include "ipps.h"
#include "ippi.h"
#include "ippj.h"
#include "ippvc.h"
#include "ippac.h"
#include "ippdc.h"
#include "ippcc.h"

using namespace UMC;


static void printVersion()
{
#if defined WINDOWS && defined _UNICODE
    vm_char cCVTString[255] = {0};
#endif
    const IppLibraryVersion *pVersion;
    vm_string_printf(VM_STRING("\nIntel(R) IPP:\n"));
    PRINT_LIB_VERSION(  , pVersion, cCVTString)
    PRINT_LIB_VERSION(s,  pVersion, cCVTString)
    PRINT_LIB_VERSION(i,  pVersion, cCVTString)
    PRINT_LIB_VERSION(j,  pVersion, cCVTString)
    PRINT_LIB_VERSION(vc, pVersion, cCVTString)
    PRINT_LIB_VERSION(ac, pVersion, cCVTString)
    PRINT_LIB_VERSION(dc, pVersion, cCVTString)
    PRINT_LIB_VERSION(cc, pVersion, cCVTString)
}

static void printHelp(ParserCmd *pCmd, const vm_char *pErrorMessage)
{
    DString   renders;

    if(pCmd && !pCmd->m_bHelpPrinted)
    {
        vm_string_printf(VM_STRING("\nUsage: %s [-i] InputFile [Options]\n"), (vm_char*)pCmd->m_sProgNameShort);
        vm_string_printf(VM_STRING("Options:\n"));
        pCmd->PrintHelp();
        vm_string_printf(VM_STRING("\nColor formats: gray yv12 nv12 yuy2 uyvy yuv420 yuv422 yuv444\n"));
        vm_string_printf(VM_STRING("               rgb24 rgb32 bgr24 bgr32 bgr565 bgr555 bgr444\n"));

        vm_string_printf(VM_STRING("Video renders: "));

#ifdef UMC_ENABLE_OPENGL_VIDEO_RENDER
        renders += VM_STRING("opengl ");
#endif

#ifdef UMC_ENABLE_GDI_VIDEO_RENDER
        renders += VM_STRING("gdi ");
#endif

#ifdef UMC_ENABLE_FW_VIDEO_RENDER
        renders += VM_STRING("fw ");
#endif
        renders += VM_STRING("null ");

        vm_string_printf(VM_STRING("%s\n"), (vm_char*)renders);
        renders.Clear();

        vm_string_printf(VM_STRING("Audio renders: "));

#ifdef UMC_ENABLE_DSOUND_AUDIO_RENDER
        renders += VM_STRING("dsound ");
#endif

#ifdef UMC_ENABLE_WINMM_AUDIO_RENDER
        renders += VM_STRING("winmm ");
#endif

#ifdef UMC_ENABLE_OSS_AUDIO_RENDER
        renders += VM_STRING("oss ");
#endif

#ifdef UMC_ENABLE_FW_AUDIO_RENDER
        renders += VM_STRING("fw ");
#endif
        renders += VM_STRING("null ");

        vm_string_printf(VM_STRING("%s\n"), (vm_char*)renders);
    }

    if(pErrorMessage)
        vm_string_printf(VM_STRING("\nError: %s\n"), pErrorMessage);
}

static Status ReadPlaylist(vm_file *pPlaylist, DString &sCurrentFile)
{
    sCurrentFile.Clear();
    vm_char  cBuffer[2048];

    if(!vm_file_gets(cBuffer, 2048, pPlaylist))
        return UMC_ERR_END_OF_STREAM;

    if(10 == cBuffer[vm_string_strlen(cBuffer) - 1])
        cBuffer[vm_string_strlen(cBuffer) - 1] = 0;
    if(13 == cBuffer[vm_string_strlen(cBuffer) - 1])
        cBuffer[vm_string_strlen(cBuffer) - 1] = 0;

    sCurrentFile = cBuffer;

    return UMC_OK;
}

static void PrintInfo(SplitterInfo *pInfo, BasePipelineParams *pParams)
{
    if(pInfo->m_systemType != UNDEF_STREAM)
    {
        VideoStreamInfo *pVideoInfo;
        AudioStreamInfo *pAudioInfo;
        Ipp32s           iVideoTrack = pParams->m_iSelectedVideoID;
        Ipp32s           iAudioTrack = pParams->m_iSelectedAudioID;

        vm_string_printf(VM_STRING("\n"));
        vm_string_printf(VM_STRING("Stream type: %s\n"), GetStreamTypeString(pInfo->m_systemType));
        for(Ipp32u i = 0; i < pInfo->m_iTracks; i++)
        {
            if(pInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO)
            {
                pVideoInfo = (VideoStreamInfo*)pInfo->m_ppTrackInfo[i]->m_pStreamInfo;
                vm_string_printf(VM_STRING("Track %d (video): "), pInfo->m_ppTrackInfo[i]->m_iPID);
                vm_string_printf(VM_STRING("%s "),                GetVideoTypeString(pVideoInfo->streamType));
                vm_string_printf(VM_STRING("%s "),                GetFormatTypeString(pVideoInfo->videoInfo.m_colorFormat));
                if(pVideoInfo->videoInfo.GetPlanesNumber())
                {
                    vm_string_printf(VM_STRING("("));
                    for(Ipp32u j = 0; j < pVideoInfo->videoInfo.GetPlanesNumber(); j++)
                    {
                        vm_string_printf(VM_STRING("%d"), pVideoInfo->videoInfo.GetPlaneBitDepth(j));
                        if(j != pVideoInfo->videoInfo.GetPlanesNumber() - 1)
                            vm_string_printf(VM_STRING(","));
                    }
                    vm_string_printf(VM_STRING(") "));
                }
                if(pVideoInfo->videoInfo.m_picStructure == PS_PROGRESSIVE)
                    vm_string_printf(VM_STRING("PROG "));
                else if(pVideoInfo->videoInfo.m_picStructure == PS_TOP_FIELD_FIRST)
                    vm_string_printf(VM_STRING("TFF "));
                else if(pVideoInfo->videoInfo.m_picStructure == PS_BOTTOM_FIELD_FIRST)
                    vm_string_printf(VM_STRING("BFF "));
                else if(pVideoInfo->videoInfo.m_picStructure == PS_TOP_FIELD || pVideoInfo->videoInfo.m_picStructure == PS_BOTTOM_FIELD)
                    vm_string_printf(VM_STRING("FIELD "));
                vm_string_printf(VM_STRING("%dx%d (%d:%d) "),     pVideoInfo->videoInfo.m_iWidth, pVideoInfo->videoInfo.m_iHeight, pVideoInfo->videoInfo.m_iSAWidth, pVideoInfo->videoInfo.m_iSAHeight);
                vm_string_printf(VM_STRING("%.2lffps "),          pVideoInfo->fFramerate);
                if((Ipp32u)iVideoTrack == i)
                    vm_string_printf(VM_STRING("* "));
                if(!pInfo->m_ppTrackInfo[i]->m_bEnabled)
                    vm_string_printf(VM_STRING("- disabled"));
                vm_string_printf(VM_STRING("\n"));
            }
            else if(pInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_AUDIO)
            {
                pAudioInfo = (AudioStreamInfo*)pInfo->m_ppTrackInfo[i]->m_pStreamInfo;
                vm_string_printf(VM_STRING("Track %d (audio): "), pInfo->m_ppTrackInfo[i]->m_iPID);
                vm_string_printf(VM_STRING("%s "),                GetAudioTypeString(pAudioInfo->streamType));
                vm_string_printf(VM_STRING("%dkHz "),             pAudioInfo->audioInfo.m_iSampleFrequency);
                vm_string_printf(VM_STRING("%dch "),              pAudioInfo->audioInfo.m_iChannels);
                vm_string_printf(VM_STRING("%dbit "),             pAudioInfo->audioInfo.m_iBitPerSample);
                if(pAudioInfo->iLanguage)
                    vm_string_printf(VM_STRING("(%d) "), pAudioInfo->iLanguage);
                if((Ipp32u)iAudioTrack == i)
                    vm_string_printf(VM_STRING("* "));
                if(!pInfo->m_ppTrackInfo[i]->m_bEnabled)
                    vm_string_printf(VM_STRING("- disabled"));
                vm_string_printf(VM_STRING("\n"));
            }
            else
            {
                vm_string_printf(VM_STRING("Track %d (unknown) "), pInfo->m_ppTrackInfo[i]->m_iPID);
                vm_string_printf(VM_STRING("\n"));
            }
        }
        vm_string_printf(VM_STRING("\n"));
        if(iVideoTrack != -1)
        {
            vm_string_printf(VM_STRING("Video render: %s\n"),    GetVideoRenderTypeString((VideoRenderType)pParams->m_iPrefVideoRender));
            vm_string_printf(VM_STRING("      Format: %s "),    GetFormatTypeString(pParams->m_dstVideoData.m_colorFormat));
            if(pParams->m_dstVideoData.GetPlanesNumber())
            {
                vm_string_printf(VM_STRING("("));
                for(Ipp32u j = 0; j < pParams->m_dstVideoData.GetPlanesNumber(); j++)
                {
                    vm_string_printf(VM_STRING("%d"), pParams->m_dstVideoData.GetPlaneBitDepth(j));
                    if(j != pParams->m_dstVideoData.GetPlanesNumber() - 1)
                        vm_string_printf(VM_STRING(","));
                }
                vm_string_printf(VM_STRING(") "));
            }
            if(pParams->m_dstVideoData.m_picStructure == PS_PROGRESSIVE)
                vm_string_printf(VM_STRING("PROG "));
            else if(pParams->m_dstVideoData.m_picStructure == PS_TOP_FIELD_FIRST)
                vm_string_printf(VM_STRING("TFF "));
            else if(pParams->m_dstVideoData.m_picStructure == PS_BOTTOM_FIELD_FIRST)
                vm_string_printf(VM_STRING("BFF "));
            vm_string_printf(VM_STRING("\n"));
            vm_string_printf(VM_STRING("        Size: %dx%d (%d:%d)\n"), pParams->m_dstVideoData.m_iWidth, pParams->m_dstVideoData.m_iHeight, pParams->m_dstVideoData.m_iSAWidth, pParams->m_dstVideoData.m_iSAHeight);
            vm_string_printf(VM_STRING("       Track: %d\n"),    iVideoTrack + 1);
            vm_string_printf(VM_STRING("\n"));
        }
        if(iAudioTrack != -1)
        {
            vm_string_printf(VM_STRING("Audio render: %s\n"), GetAudioRenderTypeString((AudioRenderType)pParams->m_iPrefAudioRender));
            vm_string_printf(VM_STRING("       Track: %d\n"), iAudioTrack + 1);
            vm_string_printf(VM_STRING("\n"));
        }
    }
    else
        vm_string_printf(VM_STRING("Unknown stream type \n"));
}

Status PlayFile(BasePipelineParams *pParams, Interface *pWnd, DString &sFileName, bool bMSDK)
{
    Status        umcRes = UMC_OK;
    BasePipeline *pPipeline = NULL;
    Statistic     stat;
    Ipp32u        j;

    std::auto_ptr<BasePipeline> autoPipeline;

    TimeFormatted timeTotal;
    TimeFormatted timeCurrent;

#ifdef UMC_ENABLE_MSDK_INTERFACE
    if(bMSDK)
        pPipeline = new PipelineMSDK();
    else
#endif
        pPipeline = new Pipeline();

    autoPipeline.reset(pPipeline);

    pParams->m_sInputFileName = sFileName;

    vm_string_printf(VM_STRING("\nFile name: %s\n"), (vm_char*)pParams->m_sInputFileName);

    umcRes = pPipeline->Init(pParams);
    if(UMC_OK != umcRes)
    {
        printHelp(NULL, VM_STRING("can not initialize pipeline"));
        return umcRes;
    }

    pWnd->AttachPipeline(pPipeline);

    pPipeline->GetParams(pParams);
    PrintInfo(pPipeline->m_pSplitInfo, pParams);

    if(pParams->m_iSelectedVideoID == -1) // no decodable video
        pWnd->WindowClose();

    if(pWnd->IsWindowExist())
    {
        IppiSize dstSize = {pParams->m_dstVideoData.m_iWidth, pParams->m_dstVideoData.m_iHeight};
        if(pParams->m_initWndSize.width == 0 || pParams->m_initWndSize.height == 0)
            pWnd->WindowResize(dstSize);
        else
            pWnd->WindowResize(pParams->m_initWndSize);

        pWnd->WindowSetFullscreen(pParams->m_bFullscreen);

        if(!(pParams->m_iVideoRenderFlags & FLAG_VREN_HIDDEN))
            pWnd->WindowShow();
    }

    umcRes = pPipeline->Start();
    if(UMC_OK != umcRes)
    {
        printHelp(NULL, VM_STRING("can not start playback"));
        return umcRes;
    }

    j = 10;
    while(pPipeline->IsPlaying())
    {
        pPipeline->GetStat(stat);

        if(j < stat.iFramesDecoded)
        {
            if(stat.fDuration > 0)
            {
                timeTotal.iHours   = (Ipp16u)(stat.fFullDuration/60/60);
                timeTotal.iMinutes = (Ipp8u)((stat.fFullDuration - timeTotal.iHours*60*60)/60);
                timeTotal.iSeconds = (Ipp8u)(stat.fFullDuration - timeTotal.iHours*60*60 - timeTotal.iMinutes*60);
            }
            if(stat.fFrameTime > 0)
            {
                timeCurrent.iHours   = (Ipp16u)(stat.fFrameTime/60/60);
                timeCurrent.iMinutes = (Ipp8u)((stat.fFrameTime - timeCurrent.iHours*60*60)/60);
                timeCurrent.iSeconds = (Ipp8u)(stat.fFrameTime - timeCurrent.iHours*60*60 - timeCurrent.iMinutes*60);
            }

            vm_string_printf(VM_STRING("\rTime: %02d:%02d:%02d/%02d:%02d:%02d; Frames: %4d; FPS: %6.2f; Skipped: %d"), timeCurrent.iHours, timeCurrent.iMinutes, timeCurrent.iSeconds,
                timeTotal.iHours, timeTotal.iMinutes, timeTotal.iSeconds, stat.iFramesDecoded, stat.fRenderRate, stat.iSkippedNum);
            j += 10;
        }

        pWnd->ProcMessages();
        vm_time_sleep(50);
    }

    pPipeline->GetStat(stat);
    if(stat.iFramesDecoded != 0)
    {
        vm_string_printf(VM_STRING("\rTime: %02d:%02d:%02d/%02d:%02d:%02d; Frames: %4d; FPS: %6.2f; Skipped: %d\n"), timeCurrent.iHours, timeCurrent.iMinutes, timeCurrent.iSeconds,
            timeTotal.iHours, timeTotal.iMinutes, timeTotal.iSeconds, stat.iFramesDecoded, stat.fRenderRate, stat.iSkippedNum);
    }

    pPipeline->Close();

    return umcRes;
}

int vm_main(Ipp32s argc, vm_char* argv[])
{
#if (defined _DEBUG && defined WINDOWS)
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

    std::auto_ptr<BasePipelineParams> autoParams;
    std::auto_ptr<Interface> autoWnd;

    BasePipelineParams *pParams;
    Interface          *pWnd = NULL;
    ParserCmd           cmd;
    SysRenderContext    renderContext;
    Status              umcRes = UMC_OK;

    DString sAppName = VM_STRING("IPP UMC Simple Player");
    DString sInputFile;
    DString sPlaylist;
    DString sColorFormat;
    DString sAudioRender;
    DString sVideoRender;
    DString sIppCpu;
    Ipp32u  iIppThreads         = 0;
    Ipp32u  iWndRes[2]          = {0, 0};
    Ipp32u  iDstRes[2]          = {0, 0};
    Ipp32u  iDecodingMode       = 0;
    bool    bNKeepAspect        = false;
    bool    bNKeepRenderAspect  = false;
    bool    bUseTextures        = false;
    bool    bSilentMode         = false;
    bool    bRealTime           = false;
    bool    bWindowStatus       = true;
    bool    bPrintHelp          = false;
    bool    bMsdk               = false;

    cmd.AddKey(VM_STRING("i"), VM_STRING(""),               VM_STRING("input file name"),                                                              String, 1, true);
    cmd.AddKey(VM_STRING("p"), VM_STRING(""),               VM_STRING("playlist file name"),                                                           String);
    cmd.AddKey(VM_STRING("f"), VM_STRING(""),               VM_STRING("output color format"),                                                          String);
    cmd.AddKey(VM_STRING("v"), VM_STRING(""),               VM_STRING("video render"),                                                                 String);
    cmd.AddKey(VM_STRING("a"), VM_STRING(""),               VM_STRING("audio render"),                                                                 String);
    cmd.AddKey(VM_STRING("V"), VM_STRING(""),               VM_STRING("output video file name (if file render selected)"),                             String);
    cmd.AddKey(VM_STRING("A"), VM_STRING(""),               VM_STRING("output audio file name (if file render selected)"),                             String);
    cmd.AddKey(VM_STRING("r"), VM_STRING(""),               VM_STRING("change output video resolution (width height)"),                                Integer, 2);
    cmd.AddKey(VM_STRING("k"), VM_STRING(""),               VM_STRING("don't keep aspect ratio during resize"),                                        Boolean);
    cmd.AddKey(VM_STRING("S"), VM_STRING(""),               VM_STRING("override frame structure information: frame, tff, bff, tf, bf"),                String);
    cmd.AddKey(VM_STRING("R"), VM_STRING(""),               VM_STRING("initial window resolution for OpenGL render (width height)"),                   Integer, 2);
    cmd.AddKey(VM_STRING("K"), VM_STRING(""),               VM_STRING("don't keep aspect ratio during resize in render"),                              Boolean);
    cmd.AddKey(VM_STRING("x"), VM_STRING(""),               VM_STRING("use textures for OpenGL render"),                                               Boolean);
    cmd.AddKey(VM_STRING("t"), VM_STRING(""),               VM_STRING("number of threads"),                                                            Integer);
    cmd.AddKey(VM_STRING("F"), VM_STRING(""),               VM_STRING("playback framerate"),                                                           Real);
    cmd.AddKey(VM_STRING("s"), VM_STRING(""),               VM_STRING("silent mode, rendering will be hidden"),                                        Boolean);
    cmd.AddKey(VM_STRING("m"), VM_STRING(""),               VM_STRING("decoding mode: 0 - video and audio (default); 1 - video only; 2 - audio only"), Integer);
    cmd.AddKey(VM_STRING("n"), VM_STRING(""),               VM_STRING("real time mode"),                                                               Boolean);
    cmd.AddKey(VM_STRING("d"), VM_STRING(""),               VM_STRING("perform deinterlacing"),                                                        Boolean);
    cmd.AddKey(VM_STRING("j"), VM_STRING(""),               VM_STRING("render anaglyph picture"),                                                      Boolean);
    cmd.AddKey(VM_STRING(""),  VM_STRING("track-video"),    VM_STRING("PID of video track to use (see tracks print)"),                                 Integer);
    cmd.AddKey(VM_STRING(""),  VM_STRING("track-audio"),    VM_STRING("PID of audio track to use (see tracks print)"),                                 Integer);
    cmd.AddKey(VM_STRING("h"), VM_STRING("help"),           VM_STRING("print help"),                                                                   Boolean);

#ifdef UMC_ENABLE_MSDK_INTERFACE
    cmd.AddKey(VM_STRING(""),  VM_STRING("msdk"),       VM_STRING("use MediaSDK decoder pipeline"),                                                Boolean, 1, false, VM_STRING("MediaSDK options"));
    cmd.AddKey(VM_STRING(""),  VM_STRING("msdk_impl"),  VM_STRING("MediaSDK implementation: 0 - auto (default); 1 - force sw; 2 - force hw "),     Integer, 1, false, VM_STRING("MediaSDK options"));
    cmd.AddKey(VM_STRING(""),  VM_STRING("msdk_mvc"),   VM_STRING("use multiview info in MediaSDK decoder"),                                       Boolean, 1, false, VM_STRING("MediaSDK options"));
#endif

    cmd.AddKey(VM_STRING(""),  VM_STRING("ipp_cpu"),     VM_STRING("target IPP optimization (SSE, SSE2, SSE3, SSSE3, SSE41, SSE42, AES, AVX, AVX2)"), String, 1, false, VM_STRING("IPP options"));
    cmd.AddKey(VM_STRING(""),  VM_STRING("ipp_threads"), VM_STRING("limit internal IPP threading"), Integer, 1, false, VM_STRING("IPP options"));

    if(cmd.Init(argv, argc))
    {
        printHelp(&cmd, VM_STRING("invalid input parameters"));
        return 1;
    }

    cmd.GetParam(VM_STRING(""), VM_STRING("ipp_cpu"),     &sIppCpu, 1);
    cmd.GetParam(VM_STRING(""), VM_STRING("ipp_threads"), &iIppThreads, 1);
    InitPreferredCpu(sIppCpu);
    if(iIppThreads != 0)
        ippSetNumThreads(iIppThreads);
    printVersion();

#ifdef UMC_ENABLE_MSDK_INTERFACE
    cmd.GetParam(VM_STRING(""),  VM_STRING("msdk"), &bMsdk, 1);
    if(bMsdk)
    {
        MSDKPipelineParams *pMsdkParams = new MSDKPipelineParams();
        pParams = pMsdkParams;
        cmd.GetParam(VM_STRING(""),  VM_STRING("msdk_impl"),    &pMsdkParams->m_iMsdkImpl,  1);
        cmd.GetParam(VM_STRING(""),  VM_STRING("msdk_mvc"),     &pMsdkParams->m_bMsdkMvc,   1);
    }
    else
#endif
        pParams = new BasePipelineParams();
    autoParams.reset(pParams);

    cmd.GetParam(VM_STRING("i"), VM_STRING(""),             &sInputFile,         1);
    cmd.GetParam(VM_STRING("V"), VM_STRING(""),             &pParams->m_sOutputVideoFile,   1);
    cmd.GetParam(VM_STRING("A"), VM_STRING(""),             &pParams->m_sOutputAudioFile,   1);
    cmd.GetParam(VM_STRING(""),  VM_STRING("track-video"),  &pParams->m_iSelectedVideoID,   1);
    cmd.GetParam(VM_STRING(""),  VM_STRING("track-audio"),  &pParams->m_iSelectedAudioID,   1);
    cmd.GetParam(VM_STRING("p"), VM_STRING(""),             &sPlaylist,              1);
    cmd.GetParam(VM_STRING("f"), VM_STRING(""),             &sColorFormat,           1);
    cmd.GetParam(VM_STRING("v"), VM_STRING(""),             &sVideoRender,           1);
    cmd.GetParam(VM_STRING("a"), VM_STRING(""),             &sAudioRender,           1);
    cmd.GetParam(VM_STRING("r"), VM_STRING(""),             &iDstRes[0],             2);
    cmd.GetParam(VM_STRING("R"), VM_STRING(""),             &iWndRes[0],             2);
    cmd.GetParam(VM_STRING("k"), VM_STRING(""),             &bNKeepAspect,           1);
    cmd.GetParam(VM_STRING("K"), VM_STRING(""),             &bNKeepRenderAspect,     1);
    cmd.GetParam(VM_STRING("x"), VM_STRING(""),             &bUseTextures,           1);
    cmd.GetParam(VM_STRING("t"), VM_STRING(""),             &pParams->m_iVideoThreads, 1);
    cmd.GetParam(VM_STRING("F"), VM_STRING(""),             &pParams->m_fFramerate,    1);
    cmd.GetParam(VM_STRING("s"), VM_STRING(""),             &bSilentMode,            1);
    cmd.GetParam(VM_STRING("m"), VM_STRING(""),             &iDecodingMode,          1);
    cmd.GetParam(VM_STRING("n"), VM_STRING(""),             &bRealTime,              1);
    cmd.GetParam(VM_STRING("d"), VM_STRING(""),             &pParams->m_bDeinterlace,  1);
    cmd.GetParam(VM_STRING("j"), VM_STRING(""),             &pParams->m_bAnaglyph,     1);
    cmd.GetParam(VM_STRING("h"), VM_STRING("help"),         &bPrintHelp,             1);

    if(bPrintHelp)
        printHelp(&cmd, NULL);

    if(!sPlaylist.Size() && !sInputFile.Size())
    {
        if(bPrintHelp)
            return 0;
        printHelp(&cmd, VM_STRING("input file must be specified"));
        return 1;
    }

    pParams->m_iPrefVideoRender = GetPreferredVideoRender(sVideoRender);
    if(!pParams->m_iPrefVideoRender)
    {
        printHelp(&cmd, VM_STRING("invalid video render"));
        return 1;
    }

    pParams->m_iPrefAudioRender = GetPreferredAudioRender(sAudioRender);
    if(!pParams->m_iPrefAudioRender)
    {
        printHelp(&cmd, VM_STRING("invalid audio render"));
        return 1;
    }

    pParams->m_selectedFormat = GetPreferredColorFormat(sColorFormat);

    // select OS specific window interface
#if defined UMC_ENABLE_SYS_WIN
    pWnd = new InterfaceWIN(); // Win32 interface
#elif defined UMC_ENABLE_SYS_GLX
    pWnd = new InterfaceX(); // X-Window interface
#else
    pWnd = new Interface(); // dummy interface
#endif
    autoWnd.reset(pWnd);

    if(pParams->m_iPrefVideoRender == OPENGL_VIDEO_RENDER)
    {
        bWindowStatus = pWnd->WindowCreate((vm_char*)sAppName);
        pWnd->GetWindowContext(&renderContext);
    }
    else if(pParams->m_iPrefVideoRender == GDI_VIDEO_RENDER)
    {   // GDI render does not support resizing
        bWindowStatus = pWnd->WindowCreate((vm_char*)sAppName, WSTYLE_FIXED_SIZE);
        pWnd->GetWindowContext(&renderContext);
    }
    if(!bWindowStatus)
    {
        printHelp(NULL, VM_STRING("window cannot be created; switching to NULL render"));
        pParams->m_iPrefVideoRender = NULL_VIDEO_RENDER;
    }

    if(iDecodingMode == 0)
        pParams->m_iSplitterFlags = AUDIO_SPLITTER | VIDEO_SPLITTER;
    else if(iDecodingMode == 1)
        pParams->m_iSplitterFlags = VIDEO_SPLITTER;
    else if(iDecodingMode == 2)
        pParams->m_iSplitterFlags = AUDIO_SPLITTER;
    else
    {
        printHelp(&cmd, VM_STRING("invalid decoding mode"));
        return 1;
    }
    pParams->m_iVideoDecoderFlags       = FLAG_VDEC_REORDER;
    if(!bNKeepRenderAspect)
        pParams->m_iVideoRenderFlags   |= FLAG_VREN_KEEPASPECT;
    if(bUseTextures)
        pParams->m_iVideoRenderFlags   |= FLAG_VREN_USETEXTURES;
    if(bSilentMode)
        pParams->m_iVideoRenderFlags   |= FLAG_VREN_HIDDEN;
    pParams->m_interpolationType        = IPPI_INTER_LINEAR;
    pParams->m_bKeepAspect              = !bNKeepAspect;
    pParams->m_deinterlacingType        = DEINTERLACING_CAVT;
    pParams->m_iAudioDecoderFlags       = 0;
    if(bSilentMode)
        pParams->m_iAudioRenderFlags   |= FLAG_AREN_HIDDEN;
    pParams->m_bStatistic               = true;
    pParams->m_bRepeat                  = false;
    pParams->m_bFullscreen              = false;
    pParams->m_bSync                    = !bRealTime;
    pParams->m_bStep                    = false;
    pParams->m_renderContext            = renderContext;
    pParams->m_initWndSize.width        = iWndRes[0];
    pParams->m_initWndSize.height       = iWndRes[1];
    pParams->m_selectedSize.width       = iDstRes[0];
    pParams->m_selectedSize.height      = iDstRes[1];

    if(sPlaylist.Size())
    {
        DString  sCurrentFile;
        vm_file *pPlaylist = vm_file_open(sPlaylist, VM_STRING("rt"));
        if(!pPlaylist)
        {
            printHelp(NULL, VM_STRING("can not read play list"));
            return 1;
        }

        while(ReadPlaylist(pPlaylist, sCurrentFile) != UMC_ERR_END_OF_STREAM)
        {
            if(sCurrentFile.Size())
                PlayFile(pParams, pWnd, sCurrentFile, bMsdk);
        }

        vm_file_fclose(pPlaylist);
    }
    else
    {
        if(sInputFile.Size())
        {
            umcRes = PlayFile(pParams, pWnd, sInputFile, bMsdk);
            if(UMC_OK != umcRes)
                return 1;
        }
        else
        {
            printHelp(&cmd, VM_STRING("input file must be specified"));
            return 1;
        }
    }

    return 0;
}
