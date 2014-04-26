//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2005-2012 Intel Corporation. All Rights Reserved.
//

#if (defined _DEBUG && defined WINDOWS)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <memory>

#include "umc_config.h"

#ifdef UMC_ENABLE_FILE_READER
#include "umc_file_reader.h"
#endif

#ifdef UMC_ENABLE_FILE_WRITER
#include "umc_file_writer.h"
#endif

#ifdef UMC_ENABLE_MPEG2_VIDEO_ENCODER
#include "umc_mpeg2_video_encoder.h"
#endif
#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER
#include "umc_mpeg4_video_encoder.h"
#endif
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER
#include "umc_h264_video_encoder.h"
#endif
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER
#include "umc_vc1_video_encoder.h"
#endif

#include "umc_video_encoder.h"
#include "umc_parser_cmd.h"
#include "umc_parser_cfg.h"
#include "umc_video_processing.h"
#include "vm_sys_info.h"

#include "umc_app_utils.h"

#include "ippcore.h"
#include "ipps.h"
#include "ippi.h"
#include "ippvc.h"
#include "ippcv.h"
#include "ippcc.h"

using namespace UMC;


struct EncoderParams
{
    ParserCfg      *pParser;
    VideoStreamType videoStreamType;
    IppiSize size;
    Ipp64f   fFramerate;
    Ipp32u   iBitrate;
    Ipp32u   iThreads;
    Ipp32u   iFramesLimit;
    Ipp32u   iFlags;

    EncoderParams()
    {
        videoStreamType = UNDEF_VIDEO;
        pParser         = NULL;
        size.width      = 0;
        size.height     = 0;
        fFramerate      = 30;
        iBitrate        = 2000000;
        iThreads        = 0;
        iFramesLimit    = 0;
        iFlags          = 0;
    };
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
    PRINT_LIB_VERSION(vc, pVersion, cCVTString)
    PRINT_LIB_VERSION(cv, pVersion, cCVTString)
    PRINT_LIB_VERSION(cc, pVersion, cCVTString)
}

static void printHelp(ParserCmd *pCmd, const vm_char *pErrorMessage)
{
    DString   encoders;

    if(pCmd && !pCmd->m_bHelpPrinted)
    {
        vm_string_printf(VM_STRING("\nUsage 1: %s InputParFile [-i SrcFile] [-o DstEncodedFile] [Options]\n"), (vm_char*)pCmd->m_sProgNameShort);
        vm_string_printf(VM_STRING("Usage 2: %s -c VideoEncoder -i SrcFile [-o DstEncodedFile] -r SrcWidth SrcHeight [Options]\n"), (vm_char*)pCmd->m_sProgNameShort);
        vm_string_printf(VM_STRING("Options:\n"));
        pCmd->PrintHelp();
        vm_string_printf(VM_STRING("\nVideo encoders: "));

#ifdef UMC_ENABLE_MPEG2_VIDEO_ENCODER
        encoders = encoders + VM_STRING("mpeg2 ");
#endif

#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER
        encoders = encoders + VM_STRING("mpeg4 ");
#endif

#ifdef UMC_ENABLE_H264_VIDEO_ENCODER
        encoders = encoders + VM_STRING("h264 ");
#endif

#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER
        encoders = encoders + VM_STRING("vc1 ");
#endif

        vm_string_printf(VM_STRING("%s\n"), (vm_char*)encoders);

        vm_string_printf(VM_STRING("Color formats: gray yv12 nv12 yuy2 uyvy yuv420 yuv422 yuv444\n"));
        vm_string_printf(VM_STRING("               rgb24 rgb32 bgr24 bgr32 bgr565 bgr555 bgr444\n"));
        vm_string_printf(VM_STRING("\nNote: options -i,-o,-r,-f,-b,-n override corresponding values inside InputParFile;\n"));
        vm_string_printf(VM_STRING("      if InputParFile not specified, at least options -c, -i and -r must be present\n"));
    }

    if(pErrorMessage)
        vm_string_printf(VM_STRING("\nError: %s\n"), pErrorMessage);
}

static void PrintInfo(VideoEncoderParams *pEncoderParams, VideoData *pInData)
{
    VideoStreamInfo *pInfo = &pEncoderParams->m_info;

    vm_string_printf(VM_STRING("\n"));
    vm_string_printf(VM_STRING("Input data: %s "), GetFormatTypeString(pInData->m_colorFormat));
    if(pInData->GetPlanesNumber())
    {
        vm_string_printf(VM_STRING("("));
        for(Ipp32u j = 0; j < pInData->GetPlanesNumber(); j++)
        {
            vm_string_printf(VM_STRING("%d"), pInData->GetPlaneBitDepth(j));
            if(j != pInData->GetPlanesNumber() - 1)
                vm_string_printf(VM_STRING(","));
        }
        vm_string_printf(VM_STRING(") "));
    }
    if(pInData->m_picStructure == PS_PROGRESSIVE)
        vm_string_printf(VM_STRING("PROG "));
    else if(pInData->m_picStructure == PS_TOP_FIELD_FIRST)
        vm_string_printf(VM_STRING("TFF "));
    else if(pInData->m_picStructure == PS_BOTTOM_FIELD_FIRST)
        vm_string_printf(VM_STRING("BFF "));
    vm_string_printf(VM_STRING("%dx%d (%d:%d) \n"), pInData->m_iWidth, pInData->m_iHeight, pInData->m_iSAWidth, pInData->m_iSAHeight);

    vm_string_printf(VM_STRING("Output stream: %s %s "), GetVideoTypeString(pInfo->streamType), GetFormatTypeString(pInfo->videoInfo.m_colorFormat));
    if(pEncoderParams->m_info.videoInfo.GetPlanesNumber())
    {
        vm_string_printf(VM_STRING("("));
        for(Ipp32u j = 0; j < pEncoderParams->m_info.videoInfo.GetPlanesNumber(); j++)
        {
            vm_string_printf(VM_STRING("%d"), pEncoderParams->m_info.videoInfo.GetPlaneBitDepth(j));
            if(j != pEncoderParams->m_info.videoInfo.GetPlanesNumber() - 1)
                vm_string_printf(VM_STRING(","));
        }
        vm_string_printf(VM_STRING(") "));
    }
    if(pEncoderParams->m_info.videoInfo.m_picStructure == PS_PROGRESSIVE)
        vm_string_printf(VM_STRING("PROG "));
    else if(pEncoderParams->m_info.videoInfo.m_picStructure == PS_TOP_FIELD_FIRST)
        vm_string_printf(VM_STRING("TFF "));
    else if(pEncoderParams->m_info.videoInfo.m_picStructure == PS_BOTTOM_FIELD_FIRST)
        vm_string_printf(VM_STRING("BFF "));
    vm_string_printf(VM_STRING("%dx%d (%d:%d) %.2ffps "), pInfo->videoInfo.m_iWidth, pInfo->videoInfo.m_iHeight, pInfo->videoInfo.m_iSAWidth, pInfo->videoInfo.m_iSAHeight, pInfo->fFramerate);
    if(pInfo->iBitrate > 0)
        vm_string_printf(VM_STRING("%dbps "), pInfo->iBitrate);
    else
        vm_string_printf(VM_STRING("NO_BRC "));
    if(pInfo->iProfile > 0)
        vm_string_printf(VM_STRING("P%d "), pInfo->iProfile);
    else
        vm_string_printf(VM_STRING("UNKNOWN_PROFILE "));
    if(pInfo->iLevel > 0)
        vm_string_printf(VM_STRING("L%d "), pInfo->iLevel);
    else
        vm_string_printf(VM_STRING("UNKNOWN_LEVEL "));
    vm_string_printf(VM_STRING("\n\n"));
}

static Status InitVideoEncoder(EncoderParams *pParams, VideoEncoderParams **ppVideoEncParams, VideoEncoder **ppVideoEncoder)
{
    VideoEncoderParams *pVideoEncParams = *ppVideoEncParams;
    VideoEncoder       *pVideoEncoder   = *ppVideoEncoder;

    if(!pParams || !ppVideoEncParams || !ppVideoEncoder)
        return UMC_ERR_NULL_PTR;

    UMC_DELETE(pVideoEncoder);
    UMC_DELETE(pVideoEncParams);

    switch(pParams->videoStreamType)
    {
#ifdef UMC_ENABLE_MPEG2_VIDEO_ENCODER
    case MPEG2_VIDEO:
        pVideoEncoder   = (VideoEncoder*)new MPEG2VideoEncoder();
        pVideoEncParams = (VideoEncoderParams*)new MPEG2EncoderParams();
        break;
#endif

#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER
    case MPEG4_VIDEO:
        pVideoEncoder   = (VideoEncoder*)new MPEG4VideoEncoder();
        pVideoEncParams = (VideoEncoderParams*)new MPEG4EncoderParams();
        break;
#endif

#ifdef UMC_ENABLE_H264_VIDEO_ENCODER
    case H264_VIDEO:
        pVideoEncoder   = (VideoEncoder*)new H264VideoEncoder();
        pVideoEncParams = (VideoEncoderParams*)new H264EncoderParams();
        break;
#endif

#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER
    case VC1_VIDEO:
        pVideoEncoder   = (VideoEncoder*)new VC1VideoEncoder();
        pVideoEncParams = (VideoEncoderParams*)new VC1EncoderParams();
        break;
#endif

    default:
        return UMC_ERR_UNSUPPORTED;
    }

    if(!pVideoEncoder || !pVideoEncParams) 
    {
        UMC_DELETE(pVideoEncoder);
        UMC_DELETE(pVideoEncParams);
        return UMC_ERR_ALLOC;
    }

    pVideoEncParams->m_info.videoInfo.m_iWidth  = pParams->size.width;
    pVideoEncParams->m_info.videoInfo.m_iHeight = pParams->size.height;
    pVideoEncParams->m_info.fFramerate          = pParams->fFramerate;
    pVideoEncParams->m_info.iBitrate            = pParams->iBitrate;
    pVideoEncParams->m_iThreads                 = pParams->iThreads;
    pVideoEncParams->m_iFlags                   = pParams->iFlags;

    if(UMC_OK != pVideoEncParams->ReadParams(pParams->pParser))
    {
        UMC_DELETE(pVideoEncoder);
        UMC_DELETE(pVideoEncParams);
        return UMC_ERR_FAILED;
    }

    if(UMC_OK != pVideoEncoder->Init(pVideoEncParams))
    {
        UMC_DELETE(pVideoEncoder);
        UMC_DELETE(pVideoEncParams);
        return UMC_ERR_FAILED;
    }

    *ppVideoEncoder   = pVideoEncoder;
    *ppVideoEncParams = pVideoEncParams;

    return UMC_OK;
}

int vm_main(Ipp32s argc, vm_char *argv[])
{
#if (defined _DEBUG && defined WINDOWS)
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

    Status      status;
    ParserCmd   cmd;
    ParserCfg   par;
    DString     sInputFile;
    DString     sInputAlphaFile;
    DString     sOutputFile;
    DString     sParFile;
    DString     sCodecName;
    DString     sSrcFormat;
    DString     sIppCpu;
    Ipp32u      iIppThreads    = 0;
    ColorFormat srcFormat      = NONE;
    Ipp32u      iSrcBitDepth   = 0;
    size_t      iBufferSize    = 0;
    Ipp32u      iSrcSize[2]    = {0, 0};
    Ipp32u      iDstSize[2]    = {0, 0};
    Ipp32u      iStatistic     = 0;
    bool        bEndOfStream   = false;
    bool        bPrintHelp     = false;

    Ipp32f      fProgress        = 0;
    Ipp32u      iPrintInterval   = 5;
    Ipp32u      iFramesTaken     = 0;
    Ipp32u      iFramesEncoded   = 0;
    Ipp64u      iBytesEncoded    = 0;
    Ipp64u      iBytesEncodedInt = 0;
    Ipp64u      iRawSize         = 0;
    Ipp64u      iRawCompleted    = 0;
    size_t      iFrameSize       = 0;
    Ipp64f      fTickFreq        = (Ipp64f)vm_time_get_frequency();
    Ipp64f      fTimeEncode      = 0;
    Ipp64f      fTimePre         = 0;
    Ipp64f      fTimeAll         = 0;
    vm_tick     tickEncode, tickPre, tickAll, tickInt;

    EncoderParams        params;
    FileReader           reader;
    FileReaderParams     readerParams;
    FileWriter           writer;
    FileWriterParams     writerParams;
    VideoEncoder        *pEncoder       = NULL;
    VideoEncoderParams  *pEncoderParams = NULL;
    VideoProcessing      videoProc;
    VideoData            dataIn;
    VideoData            dataCvt;
    MediaData            dataOut;
    VideoData           *pDataIn  = &dataIn;
    MediaData           *pDataOut = &dataOut;

    VideoColorConversionParams vccParams;
    VideoDepthConversionParams vdcParams;
    VideoResizeParams          vrParams;

    // auto destructable objects
    std::auto_ptr<VideoEncoder>       autoEncoder;
    std::auto_ptr<VideoEncoderParams> autoEncoderParams;

    cmd.AddKey(VM_STRING(""),  VM_STRING(""),     VM_STRING(""),                VM_STRING("parameters file"),                                            String, 1, true);
    cmd.AddKey(VM_STRING("c"), VM_STRING(""),     VM_STRING("sCodec"),          VM_STRING("video encoder"),                                              String);
    cmd.AddKey(VM_STRING("i"), VM_STRING(""),     VM_STRING("sSrcFile"),        VM_STRING("input file name"),                                            String);
    cmd.AddKey(VM_STRING("o"), VM_STRING(""),     VM_STRING("sDstFile"),        VM_STRING("output file name"),                                           String);
    cmd.AddKey(VM_STRING("a"), VM_STRING(""),     VM_STRING(""),                VM_STRING("input alpha file name"),                                      String);
    cmd.AddKey(VM_STRING("r"), VM_STRING(""),     VM_STRING(""),                VM_STRING("input file resolution (width height)"),                       Integer, 2);
    cmd.AddKey(VM_STRING("R"), VM_STRING(""),     VM_STRING(""),                VM_STRING("output file resolution (width height), as input by default"), Integer, 2);
    cmd.AddKey(VM_STRING("f"), VM_STRING(""),     VM_STRING(""),                VM_STRING("input file color format, YUV420 - default"),                  String);
    cmd.AddKey(VM_STRING("d"), VM_STRING(""),     VM_STRING(""),                VM_STRING("input file bit depth, 8 - default"),                          Integer);
    cmd.AddKey(VM_STRING("b"), VM_STRING(""),     VM_STRING("iBitrate"),        VM_STRING("bitrate (bits/second), 2000000 - default"),                   Integer);
    cmd.AddKey(VM_STRING("F"), VM_STRING(""),     VM_STRING("fFramerate"),      VM_STRING("framerate (frames/second), 30 - default"),                    Real);
    cmd.AddKey(VM_STRING("n"), VM_STRING(""),     VM_STRING("iFramesLimit"),    VM_STRING("number of frames to encode"),                                 Integer);
    cmd.AddKey(VM_STRING("t"), VM_STRING(""),     VM_STRING(""),                VM_STRING("number of threads"),                                          Integer);
    cmd.AddKey(VM_STRING("n"), VM_STRING(""),     VM_STRING(""),                VM_STRING("number of frames to encode"),                                 Integer);
    cmd.AddKey(VM_STRING("s"), VM_STRING(""),     VM_STRING(""),                VM_STRING("frames interval for statistic"),                              Integer);
    cmd.AddKey(VM_STRING("p"), VM_STRING(""),     VM_STRING(""),                VM_STRING("configuration pair entry (Name=Value), see parfiles"),        Config);
    cmd.AddKey(VM_STRING("h"), VM_STRING("help"), VM_STRING(""),                VM_STRING("print help"),                                                 Boolean);

    cmd.AddKey(VM_STRING(""),  VM_STRING("ipp_cpu"),     VM_STRING("target IPP optimization (SSE, SSE2, SSE3, SSSE3, SSE41, SSE42, AES, AVX, AVX2)"), String, 1, false, VM_STRING("IPP options"));
    cmd.AddKey(VM_STRING(""),  VM_STRING("ipp_threads"), VM_STRING("limit internal IPP threading"), Integer, 1, false, VM_STRING("IPP options"));

    if(cmd.Init(argv, argc, &par))
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

    cmd.GetParam(VM_STRING(""),  VM_STRING(""),             &sParFile,        1);
    cmd.GetParam(VM_STRING("t"), VM_STRING(""),             &params.iThreads, 1);
    cmd.GetParam(VM_STRING("a"), VM_STRING(""),             &sInputAlphaFile, 1);
    cmd.GetParam(VM_STRING("R"), VM_STRING(""),             &iDstSize[0],     2);
    cmd.GetParam(VM_STRING("f"), VM_STRING(""),             &sSrcFormat,      1);
    cmd.GetParam(VM_STRING("d"), VM_STRING(""),             &iSrcBitDepth,    1);
    cmd.GetParam(VM_STRING("s"), VM_STRING(""),             &iStatistic,      1);
    cmd.GetParam(VM_STRING("h"), VM_STRING("help"),         &bPrintHelp,      1);

    if(bPrintHelp)
        printHelp(&cmd, NULL);

    if(sParFile.Size())
        par.ParseFile(sParFile);

    if(!par.GetParam(VM_STRING("sCodec"), &sCodecName, 1))
    {
        if(bPrintHelp)
            return 0;
        printHelp(&cmd, VM_STRING("codec type was not specified"));
        return 1;
    }

    par.GetParam(VM_STRING("sSrcFile"), &sInputFile, 1);
    par.GetParam(VM_STRING("sDstFile"), &sOutputFile, 1);

    if(!cmd.GetParam(VM_STRING("r"), VM_STRING(""), &iSrcSize[0], 2))
    {
        par.GetParam(VM_STRING("iWidth"), &iSrcSize[0], 1);
        par.GetParam(VM_STRING("iHeight"), &iSrcSize[1], 1);
    }
    else
    {
        par.SetParam(VM_STRING("iWidth"), &iSrcSize[0], 1);
        par.SetParam(VM_STRING("iHeight"), &iSrcSize[1], 1);
    }

    par.GetParam(VM_STRING("fFramerate"), &params.fFramerate, 1);
    par.GetParam(VM_STRING("iFramesLimit"), &params.iFramesLimit, 1);
    par.GetParam(VM_STRING("iBitrate"), &params.iBitrate, 1);

    params.videoStreamType = GetPreferredVideoEncoder(sCodecName);
    if(params.videoStreamType == UNDEF_VIDEO)
    {
        printHelp(&cmd, VM_STRING("unsupported codec"));
        return 1;
    }

    if(!sInputFile.Size())
    {
        printHelp(&cmd, VM_STRING("source file name not found"));
        return 1;
    }

    if (!sParFile.Size() && (iSrcSize[0] == 0 || iSrcSize[1] == 0))
    {
        printHelp(&cmd, VM_STRING("source file resolution not specified"));
        return 1;
    }

    sCodecName = GetVideoTypeString(params.videoStreamType);
    if(sSrcFormat.Size() != 0)
    {
        srcFormat = GetPreferredColorFormat(sSrcFormat);
        if(srcFormat == NONE)
        {
            printHelp(NULL, VM_STRING("unsupported color format"));
            return 1;
        }
    }

    if(iDstSize[0] == 0 || iDstSize[1] == 0)
    {
        iDstSize[0] = iSrcSize[0];
        iDstSize[1] = iSrcSize[1];
    }
    params.size.width  = iDstSize[0];
    params.size.height = iDstSize[1];

    vm_string_printf(VM_STRING("\nInput file name: %s\n"), (vm_char*)sInputFile);

    readerParams.m_sFileName = sInputFile;
    status = reader.Init(&readerParams);
    if(status != UMC_OK)
    {
        printHelp(NULL, VM_STRING("can not open input file"));
        return 1;
    }

    iRawSize = reader.GetSize();

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

    params.pParser = &par;
    status = InitVideoEncoder(&params, &pEncoderParams, &pEncoder);
    if(status != UMC_OK)
    {
        if(status == UMC_ERR_UNSUPPORTED)
            printHelp(NULL, VM_STRING("unsupported codec type"));
        else
            printHelp(NULL, VM_STRING("can not initialize encoder"));
        return 1;
    }
    autoEncoder.reset(pEncoder);
    autoEncoderParams.reset(pEncoderParams);

    status = pEncoder->GetInfo(pEncoderParams);
    if(status != UMC_OK)
    {
        printHelp(NULL, VM_STRING("can not get encoder info"));
        return 1;
    }

    if(pEncoderParams->m_info.videoInfo.m_colorFormat == NONE)
    {
        printHelp(NULL, VM_STRING("undefined color format"));
        return 1;
    }
    params.iBitrate = pEncoderParams->m_info.iBitrate;

    vrParams.m_interpolationType = IPPI_INTER_LINEAR;

    if(iSrcBitDepth)
        videoProc.AddFilter(FILTER_DEPTH_CONVERTER, (BaseCodecParams*)&vdcParams);
    else if(!pEncoderParams->m_info.videoInfo.GetPtrToPlane(0) || pEncoderParams->m_info.videoInfo.GetMaxSampleSize() == 1)
    {
        videoProc.AddFilter(FILTER_COLOR_CONVERTER, (BaseCodecParams*)&vccParams);
        videoProc.AddFilter(FILTER_RESIZER,         (BaseCodecParams*)&vrParams);
        videoProc.AddFilter(FILTER_DEPTH_CONVERTER, (BaseCodecParams*)&vdcParams);
    }

    dataCvt.Init(&pEncoderParams->m_info.videoInfo);
    if(iSrcSize[0] != 0 && iSrcSize[1] != 0)
    {
        dataCvt.m_iWidth  = iSrcSize[0];
        dataCvt.m_iHeight = iSrcSize[1];
    }
    if(srcFormat != NONE)
        dataCvt.m_colorFormat = srcFormat;
    if(iSrcBitDepth != 0)
        dataCvt.SetBitDepth(iSrcBitDepth);
    dataCvt.Alloc();

    pDataIn->Init(&pEncoderParams->m_info.videoInfo);
    pDataIn->m_iWidth  = params.size.width;
    pDataIn->m_iHeight = params.size.height;
    pDataIn->Alloc();

    iFrameSize = pDataIn->GetMappingSize();
    pDataOut->Alloc(iFrameSize + 1000000);

    PrintInfo(pEncoderParams, &dataCvt);

    tickAll = vm_time_get_tick();
    tickInt = tickAll;

    for(;;)
    {
        if(params.iFramesLimit != 0 && (iFramesTaken >= params.iFramesLimit))
        {
            pDataIn = NULL;
            bEndOfStream = true;
        }

        if(pDataIn)
        {
            status = readVideoFrame(&reader, &dataCvt);
            if(status == UMC_ERR_END_OF_STREAM)
            {
                pDataIn      = NULL;
                bEndOfStream = true;
            }
            else if(status != UMC_OK)
            {
                printHelp(NULL, VM_STRING("can not read data from file"));
                break;
            }
            else
            {
                iRawCompleted += dataCvt.GetMappingSize();
                tickPre   = vm_time_get_tick();
                status    = videoProc.GetFrame(&dataCvt, pDataIn);
                fTimePre += (vm_time_get_tick() - tickPre);
                if(status != UMC_OK)
                {
                    printHelp(NULL, VM_STRING("video preprocessing error"));
                    break;
                }
                if(pEncoderParams->m_info.fFramerate)
                {
                    pDataIn->m_fPTSStart = iFramesEncoded/pEncoderParams->m_info.fFramerate;
                    pDataIn->m_fPTSEnd   = (iFramesEncoded+1)/pEncoderParams->m_info.fFramerate;
                }
            }
        }

        tickEncode   = vm_time_get_tick();
        status       = pEncoder->GetFrame(pDataIn, pDataOut);
        fTimeEncode += (vm_time_get_tick() - tickEncode);

        if(status == UMC_ERR_NOT_ENOUGH_DATA)
        {
            if(bEndOfStream)
                break;
            status = UMC_OK;
        }

        if(status == UMC_ERR_NOT_ENOUGH_BUFFER)
        {
            pDataOut->Alloc(pDataOut->GetBufferSize()*2);
            vm_string_printf(VM_STRING("\nWarning: not enough output buffer size, buffer increased, frame lost\n"));
            continue;
        }

        if(status == UMC_ERR_END_OF_STREAM)
        {
            status = UMC_OK;
            bEndOfStream = true;
        }

        if(status != UMC_OK)
        {
            printHelp(NULL, VM_STRING("can not encode data"));
            break;
        }

        if(pDataOut->GetDataSize())
        {
            iBufferSize       = (Ipp32s)pDataOut->GetDataSize();
            iBytesEncoded    += iBufferSize;
            iBytesEncodedInt += iBufferSize;

            if(sOutputFile.Size())
            {
                status = writer.PutData(pDataOut->GetDataPointer(), iBufferSize);
                if(status != UMC_OK)
                {
                    printHelp(NULL, VM_STRING("can not write data to file"));
                    break;
                }
            }

            pDataOut->Reset();

            iFramesEncoded++;
            if(iStatistic)
            {
                if(!(iFramesEncoded%iStatistic))
                {
                    fTimeAll = (Ipp64f)(vm_time_get_tick() - tickInt);
                    vm_string_printf(VM_STRING("%6d - %6.2f fps - %d bps\n"), iFramesEncoded, 1/(fTimeAll/fTickFreq/iStatistic), (Ipp32u)((iBytesEncodedInt*8*params.fFramerate)/iStatistic));
                    tickInt  = vm_time_get_tick();
                    iBytesEncodedInt = 0;
                }
            }
            else
            {
                if(!(iFramesEncoded%iPrintInterval))
                {
                    fTimeAll = (Ipp64f)(vm_time_get_tick() - tickInt);
                    if(params.iFramesLimit)
                        vm_string_printf(VM_STRING("\rFrames: %.d/%.d; FPS: %6.2f"), iFramesEncoded, params.iFramesLimit, 1/(fTimeAll/fTickFreq/iPrintInterval));
                    else
                    {
                        fProgress = (Ipp32f)iRawCompleted/iRawSize*100;
                        vm_string_printf(VM_STRING("\rProgress: %6.2f%%; Frames: %4d; FPS: %6.2f"), fProgress, iFramesEncoded, 1/(fTimeAll/fTickFreq/iPrintInterval));
                    }
                    tickInt  = vm_time_get_tick();
                }
            }
        }
        else if(bEndOfStream)
            break;

        if(!bEndOfStream)
            iFramesTaken++;
    }

    if(!iStatistic && iFramesEncoded%iPrintInterval)
    {
        fTimeAll = (Ipp64f)(vm_time_get_tick() - tickInt);
        if(params.iFramesLimit)
            vm_string_printf(VM_STRING("\rFrames: %.d/%.d; FPS: %6.2f\n"), iFramesEncoded, params.iFramesLimit, 1/(fTimeAll/fTickFreq/(iFramesEncoded%iPrintInterval)));
        else
        {
            fProgress = (Ipp32f)iRawCompleted/iRawSize*100;
            vm_string_printf(VM_STRING("\rProgress: %6.2f%%; Frames: %4d; FPS: %6.2f\n"), fProgress, iFramesEncoded, 1/(fTimeAll/fTickFreq/(iFramesEncoded%iPrintInterval)));
        }
    }

    fTimeAll = (Ipp64f)(vm_time_get_tick() - tickAll);

    vm_string_printf(VM_STRING("\n"));
    vm_string_printf(VM_STRING("Statistic (%d frames):\n"), iFramesEncoded);
    vm_string_printf(VM_STRING("  Overall:    %6.2f sec; %7.2f fps\n"),
        fTimeAll/fTickFreq,
        fTickFreq*iFramesEncoded/fTimeAll);
    vm_string_printf(VM_STRING("  Encoding:   %6.2f sec; %7.2f fps\n"),
        fTimeEncode/fTickFreq,
        fTickFreq*iFramesEncoded/fTimeEncode);
    vm_string_printf(VM_STRING("  Conversion: %6.2f sec; %7.2f fps\n"),
        fTimePre/fTickFreq,
        fTickFreq*iFramesEncoded/fTimePre);
    vm_string_printf(VM_STRING("\n"));
    vm_string_printf(VM_STRING("  Encoded size:      %lld bytes\n"), iBytesEncoded);
    vm_string_printf(VM_STRING("  Compression ratio: %.2f\n"), iFramesEncoded*iFrameSize/(Ipp64f)iBytesEncoded);
    if(params.fFramerate > 0 && params.iBitrate > 0)
        vm_string_printf(VM_STRING("  Encoded/expected:  %.2f\n"), (iBytesEncoded*8)/((Ipp64f)iFramesEncoded*params.iBitrate/params.fFramerate));

    return 0;
}
