//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2011-2012 Intel Corporation. All Rights Reserved.
//

#if (defined _DEBUG && defined WINDOWS)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <math.h>
#include <memory>

#include "umc_config.h"

#ifdef UMC_ENABLE_FILE_READER
#include "umc_file_reader.h"
#endif

#ifdef UMC_ENABLE_FILE_WRITER
#include "umc_file_writer.h"
#endif

#ifdef UMC_ENABLE_MPEG2_VIDEO_DECODER
#include "umc_mpeg2_video_decoder.h"
#endif
#ifdef UMC_ENABLE_MPEG4_VIDEO_DECODER
#include "umc_mpeg4_video_decoder.h"
#endif
#ifdef UMC_ENABLE_H264_VIDEO_DECODER
#include "umc_h264_video_decoder.h"
#endif
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER
#include "umc_vc1_video_decoder.h"
#endif
#ifdef UMC_ENABLE_MJPEG_VIDEO_DECODER
#include "umc_mjpeg_video_decoder.h"
#endif

#include "umc_mjpeg_pure_splitter.h"
#ifdef UMC_ENABLE_MPEG2_SPLITTER
#include "umc_threaded_demuxer.h"
#endif
#ifdef UMC_ENABLE_AVI_SPLITTER
#include "umc_avi_splitter.h"
#endif
#ifdef UMC_ENABLE_MP4_SPLITTER
#include "umc_mp4_splitter.h"
#endif
#ifdef UMC_ENABLE_VC1_SPLITTER
#include "umc_vc1_splitter.h"
#endif

#include "functions_common.h"
#include "umc_splitter.h"
#include "umc_video_decoder.h"
#include "umc_video_processing.h"
#include "umc_parser_cmd.h"
#include "vm_sys_info.h"

#include "umc_app_utils.h"

#include "ippcore.h"
#include "ipps.h"
#include "ippi.h"
#include "ippj.h"
#include "ippvc.h"
#include "ippdc.h"
#include "ippcc.h"

using namespace UMC;


struct DecoderParams
{
    ParserCfg       *pParser;
    VideoStreamInfo *pVideoInfo;
    MediaData       *pDecSpec;
    VideoProcessing *pPostProc;
    Ipp32u     iThreads;
    Ipp32u     iFlags;

    DecoderParams()
    {
        pParser     = NULL;
        pVideoInfo  = NULL;
        pDecSpec    = NULL;
        pPostProc   = NULL;
        iThreads    = 0;
        iFlags      = 0;
    };
};

class TimedVideoProcessing : public VideoProcessing
{
public:
    TimedVideoProcessing() { m_tickProc = 0;};
    virtual ~TimedVideoProcessing() {};

    Status GetFrame(MediaData* in, MediaData* out)
    {
        Status  umcRes;
        vm_tick tickPostProc = vm_time_get_tick();

        umcRes      = VideoProcessing::GetFrame(in, out);
        m_tickProc += (vm_time_get_tick() - tickPostProc);

        return umcRes;
    }

    vm_tick GetProcTime()   { return m_tickProc; };
    void   ResetProcTime() { m_tickProc = 0;  };

protected:
    vm_tick m_tickProc;
};

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
    PRINT_LIB_VERSION(dc, pVersion, cCVTString)
    PRINT_LIB_VERSION(cc, pVersion, cCVTString)
}

static void printHelp(ParserCmd *pCmd, const vm_char *pErrorMessage)
{
    if(pCmd && !pCmd->m_bHelpPrinted)
    {
        vm_string_printf(VM_STRING("\nUsage: %s [-i] InputFile [[-o] OutputFile] [Options]\n"), (vm_char*)pCmd->m_sProgNameShort);
        vm_string_printf(VM_STRING("Options:\n"));
        pCmd->PrintHelp();
        vm_string_printf(VM_STRING("\nColor formats: gray yv12 nv12 yuy2 uyvy yuv420 yuv422 yuv444\n"));
        vm_string_printf(VM_STRING("               rgb24 rgb32 bgr24 bgr32 bgr565 bgr555 bgr444\n"));
    }

    if(pErrorMessage)
        vm_string_printf(VM_STRING("\nError: %s\n"), pErrorMessage);
}

static void PrintInfo(SplitterInfo *pInfo, Ipp32u iTrack, VideoData *pOutData)
{
    if(pInfo->m_systemType != UNDEF_STREAM)
    {
        VideoStreamInfo *pVideoInfo = NULL;
        vm_string_printf(VM_STRING("\n"));
        vm_string_printf(VM_STRING("Stream type: %s\n"), GetStreamTypeString(pInfo->m_systemType));
        for(Ipp32u i = 0; i < pInfo->m_iTracks; i++)
        {
            if(pInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO)
            {
                pVideoInfo = (VideoStreamInfo*)pInfo->m_ppTrackInfo[i]->m_pStreamInfo;

                vm_string_printf(VM_STRING("Track %d: "), pInfo->m_ppTrackInfo[i]->m_iPID);
                vm_string_printf(VM_STRING("%s "),        GetVideoTypeString(pVideoInfo->streamType));
                vm_string_printf(VM_STRING("%s "),        GetFormatTypeString(pVideoInfo->videoInfo.m_colorFormat));
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
                vm_string_printf(VM_STRING("%dx%d (%d:%d) "), pVideoInfo->videoInfo.m_iWidth, pVideoInfo->videoInfo.m_iHeight, pVideoInfo->videoInfo.m_iSAWidth, pVideoInfo->videoInfo.m_iSAHeight);
                vm_string_printf(VM_STRING("%.2lffps "),  pVideoInfo->fFramerate);
                if(pVideoInfo->iProfile > 0)
                    vm_string_printf(VM_STRING("P%d "), pVideoInfo->iProfile);
                if(pVideoInfo->iLevel > 0)
                    vm_string_printf(VM_STRING("L%d "), pVideoInfo->iLevel);
                if(pInfo->m_ppTrackInfo[i]->m_iPID == iTrack)
                    vm_string_printf(VM_STRING("* "));
                vm_string_printf(VM_STRING("\n"));
            }
        }
        vm_string_printf(VM_STRING("\n"));

        vm_string_printf(VM_STRING("Output data: %s "), GetFormatTypeString(pOutData->m_colorFormat));
        if(pOutData->GetPlanesNumber())
        {
            vm_string_printf(VM_STRING("("));
            for(Ipp32u j = 0; j < pOutData->GetPlanesNumber(); j++)
            {
                vm_string_printf(VM_STRING("%d"), pOutData->GetPlaneBitDepth(j));
                if(j != pOutData->GetPlanesNumber() - 1)
                    vm_string_printf(VM_STRING(","));
            }
            vm_string_printf(VM_STRING(") "));
        }
        if(pOutData->m_picStructure == PS_PROGRESSIVE)
            vm_string_printf(VM_STRING("PROG "));
        else if(pOutData->m_picStructure == PS_TOP_FIELD_FIRST)
            vm_string_printf(VM_STRING("TFF "));
        else if(pOutData->m_picStructure == PS_BOTTOM_FIELD_FIRST)
            vm_string_printf(VM_STRING("BFF "));
        vm_string_printf(VM_STRING("%dx%d (%d:%d) "), pOutData->m_iWidth, pOutData->m_iHeight, pOutData->m_iSAWidth, pOutData->m_iSAHeight);
        vm_string_printf(VM_STRING("\n\n"));
    }
    else
        vm_string_printf(VM_STRING("Unknown stream type \n"));
}

Status getFrameCRC32(VideoData *pData, Ipp32u &iCRC32)
{
    IppStatus ippStatus;
    VideoData::PlaneData *pPlane;
    Ipp32u iLenght;
    Ipp32u i;
    Ipp32s j;

    for(i = 0; i < pData->GetPlanesNumber(); i++)
    {
        pPlane = pData->GetPtrToPlane(i);
        iLenght = pPlane->m_size.width * pPlane->m_iSamples * pPlane->m_iSampleSize;

        for(j = 0; j < pPlane->m_size.height; j++)
        {
            ippStatus = ippsCRC32_8u(pPlane->m_pPlane + pPlane->m_iPitch*j, iLenght, &iCRC32);
            if(ippStatus != ippStsNoErr)
                return UMC_ERR_FAILED;
        }
    }

    return UMC_OK;
}

Status getFrameMSE(DataReader *pReader, VideoData* pData, VideoData *pRefData, Ipp64f &fMSE)
{
    Status status;
    VideoData::PlaneData *pPlane, *pRefPlane;
    Ipp8s *pSrc, *pRef;
    Ipp32u iTmp;
    size_t iLenght;
    Ipp64f fMSEFrame = 0;
    Ipp64f fMSEPlane = 0;
    Ipp32u i;
    Ipp32s j, k;

    // Read reference file
    for(i = 0; i < pRefData->GetPlanesNumber(); i++)
    {
        pPlane = pRefData->GetPtrToPlane(i);
        iLenght = pPlane->m_size.width * pPlane->m_iSamples * pPlane->m_iSampleSize;

        for(j = 0; j < pPlane->m_size.height; j++)
        {
            status = pReader->GetData(pPlane->m_pPlane + pPlane->m_iPitch*j, iLenght);
            if(status != UMC_OK)
                return status;
        }
    }

    // Calculate MSE
    for(i = 0; i < pData->GetPlanesNumber(); i++)
    {
        pPlane = pData->GetPtrToPlane(i);
        pRefPlane = pRefData->GetPtrToPlane(i);
        iLenght   = pPlane->m_size.width * pPlane->m_iSamples * pPlane->m_iSampleSize;
        fMSEPlane = 0;

        for(j = 0; j < pPlane->m_size.height; j++)
        {
            pSrc = (Ipp8s*)(pPlane->m_pPlane + pPlane->m_iPitch*j);
            pRef = (Ipp8s*)(pRefPlane->m_pPlane + pRefPlane->m_iPitch*j);
            for(k = 0; k < pPlane->m_size.width; k++)
            {
                iTmp = ABS(pSrc[k] - pRef[k]);
                fMSEPlane += (iTmp*iTmp);
            }
        }
        fMSEFrame += fMSEPlane/(iLenght*pPlane->m_size.height);
    }
    fMSE += fMSEFrame/pData->GetPlanesNumber();

    return UMC_OK;
}

Ipp64f convertMSEToPSNR(Ipp64f fMSE, Ipp32s iBitDepth)
{
    Ipp32u iMax = (Ipp32u)pow((float)2, iBitDepth) - 1;
    return 10*log10((iMax*iMax)/fMSE);
}

static Status InitSplitter(Ipp32u iFlags, DataReader *pDataReader, Splitter **ppSplitter)
{
    SystemStreamType type;
    SplitterParams   splitterParams;
    Splitter *pSplitter = *ppSplitter;

    if(!pDataReader)
        return UMC_ERR_NULL_PTR;

    UMC_DELETE(pSplitter);

    type = Splitter::GetStreamType(pDataReader);

    switch(type)
    {
    case MJPEG_STREAM:
        pSplitter = (Splitter*)new MJPEGSplitter();
        break;

#ifdef UMC_ENABLE_MPEG2_SPLITTER
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
        pSplitter  = (Splitter*)new AVISplitter();
        break;
#endif

#ifdef UMC_ENABLE_VC1_SPLITTER
    case VC1_PURE_VIDEO_STREAM:
        pSplitter  = (Splitter*)new VC1Splitter();
        break;
#endif

    default:
        return UMC_ERR_UNSUPPORTED;
    }

    if(!pSplitter)
        return UMC_ERR_ALLOC;

    splitterParams.m_pDataReader = pDataReader;
    splitterParams.m_iFlags      = iFlags;

    if(UMC_OK != pSplitter->Init(&splitterParams))
    {
        UMC_DELETE(pSplitter);
        return UMC_ERR_FAILED;
    }

    pSplitter->Run();

    *ppSplitter = pSplitter;

    return UMC_OK;
}

static Status InitVideoDecoder(DecoderParams *pParams, VideoDecoderParams **ppVideoDecParams, VideoDecoder **ppVideoDecoder)
{
    VideoDecoderParams *pVideoDecParams = *ppVideoDecParams;
    VideoDecoder       *pVideoDecoder   = *ppVideoDecoder;

    if(!pParams || !ppVideoDecParams || !ppVideoDecoder || !pParams->pVideoInfo)
        return UMC_ERR_NULL_PTR;

    UMC_DELETE(pVideoDecoder);
    UMC_DELETE(pVideoDecParams);

    switch(pParams->pVideoInfo->streamType)
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
        pVideoDecoder   = (VideoDecoder*)new MJPEGVideoDecoder();
        pVideoDecParams = (VideoDecoderParams*)new VideoDecoderParams();
        break;
#endif

    default:
        return UMC_ERR_UNSUPPORTED;
    }

    if(!pVideoDecoder || !pVideoDecParams)
    {
        UMC_DELETE(pVideoDecParams);
        UMC_DELETE(pVideoDecoder);
        return UMC_ERR_ALLOC;
    }

    pVideoDecParams->m_pPostProcessor  = pParams->pPostProc;
    pVideoDecParams->m_pData           = pParams->pDecSpec;
    pVideoDecParams->m_info            = *pParams->pVideoInfo;
    pVideoDecParams->m_iThreads        = pParams->iThreads;
    pVideoDecParams->m_iFlags          = pParams->iFlags;

    if(UMC_OK != pVideoDecoder->Init(pVideoDecParams))
    {
        UMC_DELETE(pVideoDecoder);
        UMC_DELETE(pVideoDecParams);
        return UMC_ERR_FAILED;
    }

    *ppVideoDecoder   = pVideoDecoder;
    *ppVideoDecParams = pVideoDecParams;

    return UMC_OK;
}

int vm_main(Ipp32s argc, vm_char *argv[])
{
#if (defined _DEBUG && defined WINDOWS)
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

    Status      status;
    ParserCmd   cmd;
    DString     sInputFile;
    DString     sOutputFile;
    DString     sInputRefFile;
    DString     sColorFormat;
    DString     sCRC32Ref;
    DString     sIppCpu;
    Ipp32u      iIppThreads    = 0;
    ColorFormat dstFormat      = NONE;
    Ipp32u      iFramesLimit   = 0;
    Ipp32u      iTrack         = 0;
    Ipp32s      iDstBitDepth   = 0;
    Ipp32u      iDstSize[2]    = {0, 0};
    IppiSize    dstSize        = {0, 0};
    Ipp32u      iStatistic     = 0;
    bool        bEndOfStream   = false;
    bool        bDeinterlace   = false;
    bool        bNKeepAspect   = false;
    bool        bPrintHelp     = false;

    Ipp32u      iPrintInterval     = 10;
    Ipp32u      iFramesDecoded     = 0;
    Ipp64u      iDecodedSize       = 0;
    Ipp64u      iStreamPosition    = 0;
    Ipp64u      iStreamPositionInt = 0;
    Ipp32f      fProgress          = 0;
    Ipp64u      iStreamSize        = 0;
    Ipp32u      iCRC32             = 0;
    Ipp64f      fMSE               = 0;
    Ipp64f      fTickFreq          = (Ipp64f)vm_time_get_frequency();
    Ipp64f      fTimeDecode        = 0;
    Ipp64f      fTimePost          = 0;
    Ipp64f      fTimeAll           = 0;
    vm_tick     tickDecode, tickAll, tickInt;

    DecoderParams        params;
    FileReader           reader;
    FileReader           readerRef;
    FileReaderParams     readerParams;
    FileWriter           writer;
    FileWriterParams     writerParams;
    Splitter            *pSplitter      = NULL;
    SplitterInfo        *pInfo          = NULL;
    VideoDecoder        *pDecoder       = NULL;
    VideoDecoderParams  *pDecoderParams = NULL;
    VideoStreamInfo     *pVideoInfo     = NULL;
    TimedVideoProcessing videoProc;
    MediaData            dataIn;
    VideoData            dataOut;
    VideoData            dataRef;
    MediaData           *pDataIn    = &dataIn;
    VideoData           *pDataOut   = &dataOut;

    VideoDepthConversionParams vdcParams;
    VideoColorConversionParams vccParams;
    VideoDeinterlaceParams      vdParams;
    VideoResizeParams           vrParams;

    // auto destructable objects
    std::auto_ptr<VideoDecoder>       autoDecoder;
    std::auto_ptr<VideoDecoderParams> autoDecoderParams;
    std::auto_ptr<Splitter>           autoSplitter;

    cmd.AddKey(VM_STRING("i"), VM_STRING(""),               VM_STRING("input file name"),                                  String, 1, true);
    cmd.AddKey(VM_STRING("o"), VM_STRING(""),               VM_STRING("output file name"),                                 String, 1, true);
    cmd.AddKey(VM_STRING("c"), VM_STRING(""),               VM_STRING("input reference file name to calculate PSNR"),      String);
    cmd.AddKey(VM_STRING("t"), VM_STRING(""),               VM_STRING("number of threads"),                                Integer);
    cmd.AddKey(VM_STRING("r"), VM_STRING(""),               VM_STRING("output resolution (width height)"),                 Integer, 2);
    cmd.AddKey(VM_STRING("f"), VM_STRING(""),               VM_STRING("output color format"),                              String);
    cmd.AddKey(VM_STRING("b"), VM_STRING(""),               VM_STRING("output bitdepth"),                                  Integer);
    cmd.AddKey(VM_STRING("n"), VM_STRING(""),               VM_STRING("frames limit"),                                     Integer);
    cmd.AddKey(VM_STRING("d"), VM_STRING(""),               VM_STRING("perform deinterlacing"),                            Boolean);
    cmd.AddKey(VM_STRING("k"), VM_STRING(""),               VM_STRING("don't keep aspect ratio during resize"),            Boolean);
    cmd.AddKey(VM_STRING("s"), VM_STRING(""),               VM_STRING("frames interval for statistic"),                    Integer);
    cmd.AddKey(VM_STRING(""),  VM_STRING("crc"),            VM_STRING("input crc32 value to compare with decoded data"),   String);
    cmd.AddKey(VM_STRING("h"), VM_STRING("help"),           VM_STRING("print help"),                                       Boolean);

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

    cmd.GetParam(VM_STRING("i"), VM_STRING(""),        &sInputFile,      1);
    cmd.GetParam(VM_STRING("o"), VM_STRING(""),        &sOutputFile,     1);
    cmd.GetParam(VM_STRING("c"), VM_STRING(""),        &sInputRefFile,   1);
    cmd.GetParam(VM_STRING("t"), VM_STRING(""),        &params.iThreads, 1);
    cmd.GetParam(VM_STRING("r"), VM_STRING(""),        &iDstSize[0],     2);
    cmd.GetParam(VM_STRING("f"), VM_STRING(""),        &sColorFormat,    1);
    cmd.GetParam(VM_STRING("b"), VM_STRING(""),        &iDstBitDepth,    1);
    cmd.GetParam(VM_STRING("d"), VM_STRING(""),        &bDeinterlace,    1);
    cmd.GetParam(VM_STRING("n"), VM_STRING(""),        &iFramesLimit,    1);
    cmd.GetParam(VM_STRING("k"), VM_STRING(""),        &bNKeepAspect,    1);
    cmd.GetParam(VM_STRING("s"), VM_STRING(""),        &iStatistic,      1);
    cmd.GetParam(VM_STRING(""),  VM_STRING("crc"),     &sCRC32Ref,       1);
    cmd.GetParam(VM_STRING("h"), VM_STRING("help"),    &bPrintHelp,      1);

    if(bPrintHelp)
        printHelp(&cmd, NULL);

    if(!sInputFile.Size())
    {
        if(bPrintHelp)
            return 0;
        printHelp(&cmd, VM_STRING("source file name not found"));
        return 1;
    }

    vm_string_printf(VM_STRING("\nInput file name: %s\n"), (vm_char*)sInputFile);

    dstSize.width  = iDstSize[0];
    dstSize.height = iDstSize[1];
    dstFormat = GetPreferredColorFormat(sColorFormat);

    readerParams.m_sFileName = sInputFile;
    status = reader.Init(&readerParams);
    if(status != UMC_OK)
    {
        printHelp(NULL, VM_STRING("can not open input file"));
        return 1;
    }

    iStreamSize = reader.GetSize();

    if(sOutputFile.Size())
    {
        vm_string_printf(VM_STRING("Output file name: %s\n"), (vm_char*)sOutputFile);
        writerParams.m_sFileName = sOutputFile;
        status = writer.Init(&writerParams);
        if(status != UMC_OK)
        {
            printHelp(NULL, VM_STRING("can not open output file"));
            return 1;
        }
    }

    status = InitSplitter(VIDEO_SPLITTER, &reader, &pSplitter);
    if(status != UMC_OK)
    {
        printHelp(NULL, VM_STRING("can not initialize splitter"));
        return 1;
    }
    autoSplitter.reset(pSplitter);

    status = pSplitter->GetInfo(&pInfo);
    if(status != UMC_OK)
    {
        printHelp(NULL, VM_STRING("can not get splitter info"));
        return 1;
    }

    for(Ipp32u i = 0; i < pInfo->m_iTracks; i++)
    {
        if(pInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO && pInfo->m_ppTrackInfo[i]->m_bEnabled)
        {
            params.pDecSpec = pInfo->m_ppTrackInfo[i]->m_pHeader;
            pVideoInfo      = (VideoStreamInfo*)pInfo->m_ppTrackInfo[i]->m_pStreamInfo;
            iTrack          = i;
            break;
        }
    }

    if(pVideoInfo == NULL)
    {
        printHelp(NULL, VM_STRING("no video stream to decode"));
        return 1;
    }

    if(pVideoInfo->videoInfo.m_colorFormat == NONE)
    {
        printHelp(NULL, VM_STRING("undefined color format"));
        return 1;
    }

    vdParams.m_deinterlacingType = DEINTERLACING_MEDIAN;
    vdParams.m_bEnabled          = bDeinterlace;
    vrParams.m_interpolationType = IPPI_INTER_LINEAR;

    videoProc.AddFilter(FILTER_DEPTH_CONVERTER, (BaseCodecParams*)&vdcParams);
    videoProc.AddFilter(FILTER_DEINTERLACER,    (BaseCodecParams*)&vdParams);
    videoProc.AddFilter(FILTER_COLOR_CONVERTER, (BaseCodecParams*)&vccParams);
    videoProc.AddFilter(FILTER_RESIZER,         (BaseCodecParams*)&vrParams);

    params.iFlags     = FLAG_VDEC_REORDER;
    params.pPostProc  = &videoProc;
    params.pVideoInfo = pVideoInfo;
    status = InitVideoDecoder(&params, &pDecoderParams, &pDecoder);
    if(status != UMC_OK)
    {
        if(status == UMC_ERR_UNSUPPORTED)
            printHelp(NULL, VM_STRING("unsupported codec type"));
        else
            printHelp(NULL, VM_STRING("can not initialize decoder"));
        return 1;
    }
    autoDecoder.reset(pDecoder);
    autoDecoderParams.reset(pDecoderParams);

    if(dstSize.width == 0 || dstSize.height == 0)
    {
        dstSize.width  = pVideoInfo->videoInfo.m_iWidth;
        dstSize.height = pVideoInfo->videoInfo.m_iHeight;
    }
    else if(!bNKeepAspect)
    {
        Ipp32f fScale = IPP_MIN((Ipp32f)dstSize.width/pVideoInfo->videoInfo.m_iWidth, (Ipp32f)dstSize.height/pVideoInfo->videoInfo.m_iHeight);
        dstSize.width  = UMC_ROUND_32U(fScale*pVideoInfo->videoInfo.m_iWidth);
        dstSize.height = UMC_ROUND_32U(fScale*pVideoInfo->videoInfo.m_iHeight);
    }

    pDataOut->Init(&pVideoInfo->videoInfo);

    pDataOut->m_iWidth      = dstSize.width;
    pDataOut->m_iHeight     = dstSize.height;

    if(dstFormat != NONE)
        pDataOut->m_colorFormat = dstFormat;

    if(iDstBitDepth)
        pDataOut->SetBitDepth(iDstBitDepth);

    if(bDeinterlace)
        pDataOut->m_picStructure = PS_PROGRESSIVE;

    pDataOut->Alloc();

    if(sInputRefFile.Size())
    {
        readerParams.m_sFileName = sInputRefFile;
        status = readerRef.Init(&readerParams);
        if(status != UMC_OK)
        {
            printHelp(NULL, VM_STRING("can not open reference file"));
            return 1;
        }
        dataRef.Init(dstSize.width, dstSize.height, dstFormat, iDstBitDepth);
        dataRef.Alloc();
        dataRef.m_picStructure = pDataOut->m_picStructure;
    }

    PrintInfo(pInfo, iTrack, pDataOut);

    tickAll = vm_time_get_tick();
    tickInt = tickAll;

    for(;;)
    {
        if(!bEndOfStream && (pDataIn->GetDataSize() < 4 || status == UMC_ERR_NOT_ENOUGH_DATA))
        {
            do
            {
                status = pSplitter->GetNextData(pDataIn, iTrack);
            } while(status == UMC_ERR_NOT_ENOUGH_DATA);
            if(status == UMC_ERR_END_OF_STREAM)
            {
                iStreamSize  = iStreamPosition; // correct stream size for video data
                pDataIn      = NULL;
                bEndOfStream = true;
            }
            else if(status != UMC_OK)
            {
                printHelp(NULL, VM_STRING("can not get data from splitter"));
                break;
            }
            if(pDataIn)
            {
                iStreamPosition    += pDataIn->GetDataSize();
                iStreamPositionInt += pDataIn->GetDataSize();
            }
        }

        tickDecode   = vm_time_get_tick();
        status       = pDecoder->GetFrame(pDataIn, pDataOut);
        fTimeDecode += (vm_time_get_tick() - tickDecode);

        if(status == UMC_ERR_NOT_ENOUGH_DATA || status == UMC_ERR_SYNC)
        {
            if(!bEndOfStream)
                continue;
            break;
        }

        if(status != UMC_OK)
        {
            printHelp(NULL, VM_STRING("can not decode data"));
            continue;
        }
        iFramesDecoded++;
        iDecodedSize += pDataOut->GetDataSize();

        if(sOutputFile.Size())
        {
            status = writeVideoFrame(&writer, pDataOut);
            if(status != UMC_OK)
            {
                printHelp(NULL, VM_STRING("can not write data to file"));
                break;
            }
        }

        if(sCRC32Ref.Size())
        {
            status = getFrameCRC32(pDataOut, iCRC32);
            if(status != UMC_OK)
            {
                printHelp(NULL, VM_STRING("can not calculate CRC"));
                break;
            }
        }

        if(sInputRefFile.Size())
        {
            status = getFrameMSE(&readerRef, pDataOut, &dataRef, fMSE);
            if(status != UMC_OK)
            {
                printHelp(NULL, VM_STRING("can not read reference data from file"));
                break;
            }
        }

        if(iStatistic)
        {
            if(!(iFramesDecoded%iStatistic))
            {
                fTimeAll = (Ipp64f)(vm_time_get_tick() - tickInt);
                vm_string_printf(VM_STRING("%6d - %6.2f fps - %d bps\n"), iFramesDecoded, 1/(fTimeAll/fTickFreq/iStatistic), (Ipp32u)((iStreamPositionInt*8*pVideoInfo->fFramerate)/iStatistic));
                tickInt  = vm_time_get_tick();
                iStreamPositionInt = 0;
            }
        }
        else
        {
            if(!(iFramesDecoded%iPrintInterval))
            {
                fTimeAll = (Ipp64f)(vm_time_get_tick() - tickInt);
                if(iFramesLimit)
                    vm_string_printf(VM_STRING("\rFrames: %.d/%.d; FPS: %6.2f"), iFramesDecoded, iFramesLimit, 1/(fTimeAll/fTickFreq/iPrintInterval));
                else
                {
                    fProgress = ((Ipp32f)iStreamPosition*100)/iStreamSize;
                    vm_string_printf(VM_STRING("\rProgress: %6.2f%%; Frames: %4d; FPS: %6.2f"), fProgress, iFramesDecoded, 1/(fTimeAll/fTickFreq/iPrintInterval));
                }
                tickInt  = vm_time_get_tick();
            }
        }

        if(iFramesLimit != 0 && (iFramesDecoded >= iFramesLimit))
            break;
    }

    if(!iStatistic && iFramesDecoded%iPrintInterval)
    {
        fTimeAll = (Ipp64f)(vm_time_get_tick() - tickInt);
        if(iFramesLimit)
            vm_string_printf(VM_STRING("\rFrames: %.d/%.d; FPS: %6.2f\n"), iFramesDecoded, iFramesLimit, 1/(fTimeAll/fTickFreq/(iFramesDecoded%iPrintInterval)));
        else
        {
            fProgress = (Ipp32f)iStreamPosition/iStreamSize*100;
            vm_string_printf(VM_STRING("\rProgress: %6.2f%%; Frames: %4d; FPS: %6.2f\n"), fProgress, iFramesDecoded, 1/(fTimeAll/fTickFreq/(iFramesDecoded%iPrintInterval)));
        }
    }

    fTimeAll     = (Ipp64f)(vm_time_get_tick() - tickAll);
    fTimePost    = (Ipp64f)videoProc.GetProcTime();
    fTimeDecode -= fTimePost;

    vm_string_printf(VM_STRING("\n"));
    vm_string_printf(VM_STRING("Statistic (%d frames):\n"), iFramesDecoded);
    vm_string_printf(VM_STRING("  Overall:    %6.2f sec; %7.2f fps\n"),
        fTimeAll/fTickFreq,
        fTickFreq*iFramesDecoded/fTimeAll);
    vm_string_printf(VM_STRING("  Decoding:   %6.2f sec; %7.2f fps\n"),
        fTimeDecode/fTickFreq,
        fTickFreq*iFramesDecoded/fTimeDecode);
    vm_string_printf(VM_STRING("  Conversion: %6.2f sec; %7.2f fps\n"),
        fTimePost/fTickFreq,
        fTickFreq*iFramesDecoded/fTimePost);
    vm_string_printf(VM_STRING("\n"));
    vm_string_printf(VM_STRING("  Decoded:      %lld bytes\n"), iDecodedSize);
    vm_string_printf(VM_STRING("  Avg. bitrate: %d bps\n"), (Ipp32u)((iStreamPosition*8*pVideoInfo->fFramerate)/iFramesDecoded));

    if(sInputRefFile.Size())
    {
        Ipp64f fPSNR;
        fMSE = fMSE/iFramesDecoded;

        vm_string_printf(VM_STRING("\nReference file name: %s\n"), (vm_char*)sInputRefFile);
        if(fMSE)
        {
            fPSNR = convertMSEToPSNR(fMSE, iDstBitDepth);
            vm_string_printf(VM_STRING("PSNR: %.2f "), fPSNR);
        }
        else
            vm_string_printf(VM_STRING("PSNR: inf "));

        if(iDecodedSize > readerRef.GetSize())
            vm_string_printf(VM_STRING("(reference file is smaller)"));
        else if(iDecodedSize < readerRef.GetSize())
            vm_string_printf(VM_STRING("(reference file is bigger)"));
        vm_string_printf(VM_STRING("\n"));
    }

    if(sCRC32Ref.Size())
    {
        Ipp32u iCRC32Ref;
        vm_string_sscanf((vm_char*)sCRC32Ref, VM_STRING("%x"), &iCRC32Ref);

        vm_string_printf(VM_STRING("\nRef. CRC32: %08x\n"), iCRC32Ref);
        vm_string_printf(VM_STRING("CRC32: %08x\n"), iCRC32);
        if(iCRC32 == iCRC32Ref)
            vm_string_printf(VM_STRING("Checksums match\n"));
        else
        {
            vm_string_printf(VM_STRING("Checksums do not match\n"));
            return 1;
        }
    }

    return 0;
}
