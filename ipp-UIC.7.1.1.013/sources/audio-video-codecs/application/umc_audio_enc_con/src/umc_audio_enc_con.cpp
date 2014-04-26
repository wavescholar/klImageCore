/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

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

#ifdef UMC_ENABLE_MP3_AUDIO_ENCODER
#include "umc_mp3_encoder.h"
#endif
#ifdef UMC_ENABLE_AAC_AUDIO_ENCODER
#include "umc_aac_encoder.h"
#endif

#ifdef UMC_ENABLE_MP4_MUXER
#include "umc_mp4_mux.h"
#endif

#include "functions_common.h"
#include "umc_splitter.h"
#include "umc_audio_encoder.h"
#include "vm_sys_info.h"
#include "umc_sample_buffer.h"
#include "umc_linear_buffer.h"
#include "umc_parser_cmd.h"

#include "umc_app_utils.h"

#include "audio_file.h"
#include "simple_file.h"
#include "wav_file.h"
#include "params.h"

#include "ippcore.h"
#include "ipps.h"
#include "ippi.h"
#include "ippac.h"
#include "ippvm.h"

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
    PRINT_LIB_VERSION(ac, pVersion, cCVTString)
    PRINT_LIB_VERSION(vm, pVersion, cCVTString)
}

static void printHelp(ParserCmd *pCmd, vm_char *pErrorMessage)
{
    if(pCmd && !pCmd->m_bHelpPrinted)
    {
        vm_string_printf(VM_STRING("\nUsage: %s -c EncoderName [-i] InputFile [[-o] OutputFile] [Options]\n"), (vm_char*)pCmd->m_sProgNameShort);
        vm_string_printf(VM_STRING("Options:\n"));
        pCmd->PrintHelp();
    }

    if(pErrorMessage)
        vm_string_printf(VM_STRING("\nError: %s\n"), pErrorMessage);
}

int vm_main(Ipp32s argc, vm_char *argv[])
{
#if (defined _DEBUG && defined WINDOWS)
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

    ParserCmd cmd;
    DString   sIppCpu;
    Ipp32u    iIppThreads = 0;
    bool      bPrintHelp  = false;
    Status    status;

    AudioFile*            p_in_file = NULL;
    AudioFile*            p_out_file = NULL;
    SimpleFile            file_in;
    SimpleFile            file_out;
    WavFile               wav_file_in;
    WavFile::Info         info_file_in;
    Ipp32s                is_input_wav = 0;
    Ipp32s                err = 0;
    Ipp32s                acp_allocated = 0;
    Ipp32s                numFramesToProcess = 0;
    MediaData*            pOut = NULL;
    MediaData             inMediaBuffer, outMediaBuffer;
    size_t                size = 0;
    Status                sts, StreamStatus = UMC_OK;
    AudioEncoder*         pAudioEncoder = NULL;
    MediaBuffer*          pMediaBuffer = NULL;
    MediaBuffer*          pMediaBufferTmp = NULL;
    AudioEncoderParams*   audio_encoder_params = NULL;
    MediaBufferParams *   media_buffer_params = NULL;
    FileWriter*           pFileWriter = NULL;
    FileWriterParams*     pFileWriterParams = NULL;
    Ipp8u*                dpndTrTbl = NULL;
    ProgramParameters     params;
    MediaData             in_data;
    AudioStreamType       streamType;
    info_file_in.resolution = 0;

#ifdef UMC_ENABLE_MP4_MUXER
    MP4Muxer*             pMP4Muxer = NULL;
    MuxerParams*          pMP4MuxerParams = NULL;
    TrackParams*          pMP4TrackParams = NULL;
#endif

    Ipp64f      fTickFreq          = (Ipp64f)vm_time_get_frequency();
    Ipp64f      fTimeEncode        = 0;
    Ipp64f      fTimeAll           = 0;
    vm_tick     tickEncode = 0, tickAll = 0, tickInt = 0;

    cmd.AddKey(VM_STRING("c"), VM_STRING(""),        VM_STRING("codec name (mp3, aac)"),             String);
    cmd.AddKey(VM_STRING("i"), VM_STRING(""),        VM_STRING("input file name"),                   String, 1, true);
    cmd.AddKey(VM_STRING("o"), VM_STRING(""),        VM_STRING("output file name"),                  String, 1, true);
    cmd.AddKey(VM_STRING("b"), VM_STRING(""),        VM_STRING("bitrate in bps (128000 - default)"), Integer);
    cmd.AddKey(VM_STRING("h"), VM_STRING("help"),    VM_STRING("print help"),                        Boolean);

    cmd.AddKey(VM_STRING(""),  VM_STRING("ipp_cpu"),     VM_STRING("target IPP optimization (SSE, SSE2, SSE3, SSSE3, SSE41, SSE42, AES, AVX, AVX2)"), String, 1, false, VM_STRING("IPP options"));
    cmd.AddKey(VM_STRING(""),  VM_STRING("ipp_threads"), VM_STRING("limit internal IPP threading"), Integer, 1, false, VM_STRING("IPP options"));

#if defined UMC_ENABLE_AAC_AUDIO_ENCODER
    cmd.AddKey(VM_STRING(""), VM_STRING("aac_o"),  VM_STRING("output format (adif, adts - default, mp4)"), String, 1, false, VM_STRING("AAC options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("aac_p"),  VM_STRING("profile (lc - default, ltp, he) if he profile is selected, lc is internal encoder"), String, 1, false, VM_STRING("AAC options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("aac_s"),  VM_STRING("stereo mode\n0 - left/right stereo mode in all frames (default)\n1 - joint stereo mode\n2 - forced mid/side stereo mode in all frames"), Integer, 1, false, VM_STRING("AAC options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("aac_ns"), VM_STRING("noise shaping model\n0 - simple: fast, lower quality (default)\n1 - advanced: slow, higher quality"), Integer, 1, false, VM_STRING("AAC options"));
#endif

#if defined UMC_ENABLE_MP3_AUDIO_ENCODER
    cmd.AddKey(VM_STRING(""), VM_STRING("mpeg_l"),   VM_STRING("layer\n1 - Layer 1\n2 - Layer 2\n3 - Layer 3 (default)"), Integer, 1, false, VM_STRING("MPEG options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("mpeg_rc"),  VM_STRING("rate control mode\n0 - CBR (default)\n1 - VBR(ABR)"), Integer, 1, false, VM_STRING("MPEG options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("mpeg_s"),   VM_STRING("stereo mode\n0 - left/right stereo mode in all frames (default)\n1 - joint stereo mode\n2 - forced mid/side stereo mode in all frames (layer 3 only)"), Integer, 1, false, VM_STRING("MPEG options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("mpeg_ns"),  VM_STRING("noise shaping model (layer 3 only)\n0 - simple: fast, lower quality (default)\n1 - advanced: slow, higher quality"), Integer, 1, false, VM_STRING("MPEG options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("mpeg_mp"),  VM_STRING("matrix procedure\n0 (default),1,2 - MPEG1 compatible L,R channels are mixed input channels\n3 - MPEG1 compatible L,R channels are L,R input channels"), Integer, 1, false, VM_STRING("MPEG options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("mpeg_cc"),  VM_STRING("input channel configuration\n0 - Autoconfig (default)\n1 - C\n2 - L, R\n3 - L, R, C\n4 - L, R, S\n5 - L, R, C, S\n6 - L, R, SL, SR\n7 - L, R, C, SL, SR\n13 - L, R, C, LFE\n14 - L, R, S, LFE\n15 - L, R, C, S, LFE\n16 - L, R, SL, SR, LFE\n17 - L, R, C, SL, SR, LFE"), Integer, 1, false, VM_STRING("MPEG options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("mpeg_lfe_filt_off"), VM_STRING("disable LFE channel filtering"), Boolean, 1, false, VM_STRING("MPEG options"));
#endif

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

    params_fill_by_default(&params);

    cmd.GetParam(VM_STRING("c"), VM_STRING(""),        &params.codec_name,      1);
    cmd.GetParam(VM_STRING("i"), VM_STRING(""),        &params.input_file,      1);
    cmd.GetParam(VM_STRING("o"), VM_STRING(""),        &params.output_file,     1);
    cmd.GetParam(VM_STRING("b"), VM_STRING(""),        &params.bitrate,         1);
    cmd.GetParam(VM_STRING("h"), VM_STRING("help"),    &bPrintHelp,      1);

    if(bPrintHelp)
        printHelp(&cmd, NULL);

    if(!params.codec_name.Size())
    {
        if(bPrintHelp)
            return 0;
        printHelp(&cmd, VM_STRING("codec name was not specified"));
        return 1;
    }

    if(!params.input_file.Size())
    {
        printHelp(&cmd, VM_STRING("source file name not found"));
        return 1;
    }

    status = GetParams(&cmd, &params);
    if(status != UMC_OK)
    {
        printHelp(NULL, VM_STRING("invalid codec params"));
        return 1;
    }

    p_in_file = &wav_file_in;
    p_out_file = &file_out;

    streamType = GetPreferredAudioEncoder(params.codec_name);
    switch(streamType)
    {
#ifdef UMC_ENABLE_AAC_AUDIO_DECODER
    case AAC_AUDIO:
        audio_encoder_params = DynamicCast<AudioEncoderParams, AACEncoderParams>(&params.aacenc_params);
        break;
#endif

#ifdef UMC_ENABLE_MP3_AUDIO_DECODER
    case MPEG2_AUDIO:
        audio_encoder_params = DynamicCast<AudioEncoderParams, MP3EncoderParams>(&params.mp3enc_params);
        break;
#endif

    default:
        audio_encoder_params = new AudioEncoderParams;
        acp_allocated = 1;
        break;
    }

    if (NULL == audio_encoder_params)
    {
        vm_string_printf(VM_STRING("Memory allocation error\n"));
        return -1;
    }

    SplitterParams   splInitParams;
    FileReaderParams frInitParams;

    FileReader* pStream = NULL;
    Splitter*   pSpl     = NULL;

    Status umcRes = UMC_OK;

    for (;;)
    {
        // Init FileReader
        pStream = new FileReader;

        if (NULL == pStream)
        {
            vm_string_printf(VM_STRING("Memory allocation error\n"));
            err = -1;
            break;
        }

//        vm_string_strcpy(frInitParams.m_file_name, params.input_file);
        frInitParams.m_sFileName = params.input_file;

        if (pStream->Init(&frInitParams) != UMC_OK)
        {
            vm_string_printf(VM_STRING("Can't open input file: %s\n"), (vm_char*)params.input_file);
            err = -1;
            break;
        }

        {
            audio_encoder_params->m_info.streamType = UNDEF_AUDIO;
            err = p_in_file->Open((vm_char*)params.input_file, AudioFile::AFM_NO_CONTENT_WRN);

            if (err < 0)
            {
                vm_string_printf(VM_STRING("Can't open input file '%s'"), (vm_char*)params.input_file);
                err = -2;
                break;
            }
            else if (err == 0)
            {
                is_input_wav = 1;
                err = wav_file_in.GetInfo(&info_file_in);

                if (err == 0)
                {
                    if (info_file_in.format_tag == 1)
                    {
                        audio_encoder_params->m_info.streamType = PCM_AUDIO;
                        p_out_file = &file_out;
                    }

                    audio_encoder_params->m_info.audioInfo.m_iChannelMask = 0;
                    err = 0;
                    switch(params.channel_conf)
                    {
                    case 1:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_CENTER;
                        break;
                    case 2:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT;
                        break;
                    case 3:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER;
                        break;
                    case 4:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_BACK_CENTER;
                        break;
                    case 5:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER | CHANNEL_BACK_CENTER;
                        break;
                    case 6:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_BACK_LEFT | CHANNEL_BACK_RIGHT;
                        break;
                    case 7:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER |
                        CHANNEL_BACK_LEFT | CHANNEL_BACK_RIGHT;
                        break;
                    case 13:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER |
                        CHANNEL_LOW_FREQUENCY;
                        break;
                    case 14:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_BACK_CENTER |
                        CHANNEL_LOW_FREQUENCY;
                        break;
                    case 15:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER |
                        CHANNEL_BACK_CENTER | CHANNEL_LOW_FREQUENCY;
                        break;
                    case 16:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_BACK_LEFT |
                        CHANNEL_BACK_RIGHT | CHANNEL_LOW_FREQUENCY;
                        break;
                    case 17:
                        audio_encoder_params->m_info.audioInfo.m_iChannelMask =
                        CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER |
                        CHANNEL_BACK_LEFT | CHANNEL_BACK_RIGHT | CHANNEL_LOW_FREQUENCY;
                        break;
                    case 0:
                        break;
                    default:
                        err = -6;
                        vm_string_printf(VM_STRING("Unsupported channel configuration '%d'"),
                        params.channel_conf);
                    }
                    if (err)
                        break;
                    audio_encoder_params->m_info.audioInfo.m_iSampleFrequency =
                        info_file_in.sample_rate;
                    audio_encoder_params->m_info.audioInfo.m_iBitPerSample = info_file_in.resolution;
                    audio_encoder_params->m_info.audioInfo.m_iChannels =
                        info_file_in.channels_number;
                    audio_encoder_params->m_info.iBitrate = 0;
                    audio_encoder_params->m_info.iBitrate = params.bitrate;
                }
            }
            else
            {
                p_in_file->Close();
                p_in_file = &file_in;

                err = p_in_file->Open((vm_char*)params.input_file, AudioFile::AFM_NO_CONTENT_WRN);

                if (err < 0)
                {
                    vm_string_printf(VM_STRING("Can't open input file '%s'"), (vm_char*)params.input_file);
                    err = -2;
                    break;
                }
            }
        }

        switch(streamType)
        {
#ifdef UMC_ENABLE_AAC_AUDIO_DECODER
        case AAC_AUDIO:
            pAudioEncoder = new AACEncoder();
            break;
#endif

#ifdef UMC_ENABLE_MP3_AUDIO_DECODER
        case MPEG2_AUDIO:
            pAudioEncoder = new MP3Encoder();
            break;
#endif

        default:
            pAudioEncoder = NULL;
            break;
        }

        if (pAudioEncoder == NULL)
        {
            vm_string_printf(VM_STRING("Can't create audio codec object '%s'\n"), (vm_char*)params.codec_name);
            err = -5;
            break;
        }

        {
            audio_encoder_params->m_pData = NULL;
            pMediaBuffer = (MediaBuffer*)new LinearBuffer();

            if (NULL == pMediaBuffer)
            {
                vm_string_printf(VM_STRING("Memory allocation error\n"));
                err = -1;
                break;
            }
        }

        sts = pAudioEncoder->Init(audio_encoder_params);
        if (sts != UMC_OK)
        {
            vm_string_printf(VM_STRING("audioCodec Init failed\n"));
            err = UMC_ERR_FAILED;
            break;
        }

        sts = pAudioEncoder->GetInfo(audio_encoder_params);
        if (sts < 0)
        {
            vm_string_printf(VM_STRING("audioCodec GetInfo failed\n"));
            err = UMC_ERR_FAILED;
            break;
        }

        pOut = new MediaData();
        if (NULL == pOut)
        {
            vm_string_printf(VM_STRING("Memory allocation error\n"));
            err = -1;
            break;
        }
        pOut->Alloc(audio_encoder_params->m_iSuggestedOutputSize);

        media_buffer_params = new MediaBufferParams();
        media_buffer_params->m_numberOfFrames      = 10;
        media_buffer_params->m_prefInputBufferSize =
            audio_encoder_params->m_iSuggestedInputSize * 10;
        media_buffer_params->m_prefOutputBufferSize =
            audio_encoder_params->m_iSuggestedInputSize * 10;

        sts = pMediaBuffer->Init(media_buffer_params);
        if (sts != UMC_OK)
        {
            vm_string_printf(VM_STRING("mediaBuffer Init failed\n"));
            err = UMC_ERR_FAILED;
            break;
        }

        if(params.output_file.Size())
        {
#ifdef UMC_ENABLE_MP4_MUXER
            if (params.isMP4MuxerRequired)
            {
                pFileWriter = new FileWriter;
                pFileWriterParams = new FileWriterParams;
                pMP4Muxer = new MP4Muxer;
                pMP4MuxerParams = new MuxerParams;

                if ((pFileWriter == NULL) || (pFileWriterParams == NULL) ||
                    (pMP4Muxer == NULL) || (pMP4MuxerParams == NULL))
                {
                    vm_string_printf(VM_STRING("Memory allocation error\n"));
                    err = -1;
                    break;
                }

                //vm_string_strcpy(pFileWriterParams->m_file_name,params.output_file);
                pFileWriterParams[0].m_sFileName = params.output_file;
                umcRes = pFileWriter->Init(pFileWriterParams);

                if (umcRes != UMC_OK)
                {
                    vm_string_printf(VM_STRING("fileWriter Init failed\n"));
                    err = UMC_ERR_FAILED;
                    break;
                }

                pMP4TrackParams = new TrackParams;
                pMP4TrackParams->type = AUDIO_TRACK;
                audio_encoder_params->m_info.streamType = AAC_AUDIO;
                pMP4TrackParams->info.audio = &audio_encoder_params->m_info;

                pMP4MuxerParams->m_lpDataWriter = pFileWriter;
                pMP4MuxerParams->m_nNumberOfTracks = 1;
                pMP4MuxerParams->m_pTrackParams = pMP4TrackParams;

                umcRes = pMP4Muxer->Init(pMP4MuxerParams);

                if (umcRes != UMC_OK)
                {
                    vm_string_printf(VM_STRING("fileWriter Init failed\n"));
                    err = UMC_ERR_NOT_INITIALIZED;
                    break;
                }
            }
            else
#endif
            {
                err = p_out_file->Open((vm_char*)params.output_file, AudioFile::AFM_CREATE);
                if (err)
                {
                    vm_string_printf(VM_STRING("Can't create output file '%s'"), (vm_char*)params.output_file);
                    err = -3;
                    break;
                }
            }
        }

        tickAll = vm_time_get_tick();
        tickInt = tickAll;

        {
            do
            {
                sts = pMediaBuffer->LockInputBuffer(&inMediaBuffer);

                if (sts == UMC_OK)
                {
                    size_t needSize = inMediaBuffer.GetBufferSize();

                    if (is_input_wav)
                    {  /*** encoding ***/
                        if (info_file_in.resolution == 16)
                        {
                            needSize = needSize & (~1);
                        }
                        else if (info_file_in.resolution == 24)
                        {
                            needSize = (needSize / 3) * 3;
                        }
                        if (info_file_in.resolution == 32)
                        {
                            needSize = needSize & (~3);
                        }
                    }

                    size = p_in_file->Read(inMediaBuffer.GetBufferPointer(), needSize);
                    inMediaBuffer.SetDataSize(size);

                    /* we should transform 24 and 32 bits input to 16 bits one */
                    if (is_input_wav)
                    {  /*** encoding ***/
                        Ipp16s *tmp16 = (Ipp16s*)inMediaBuffer.GetBufferPointer();
                        unsigned int ii;
                        if (info_file_in.resolution == 24)
                        {
                            Ipp8u *tmp8 = (Ipp8u*)inMediaBuffer.GetBufferPointer();
                            for (ii = 0; ii < size/3; ii++)
                            {
                            #ifdef _BIG_ENDIAN_
                                Ipp32s tmp = (((((Ipp32s)tmp8[0]) << 24) >> 8) +
                                                (((Ipp32s)tmp8[1]) << 8) +
                                                ((Ipp32s)tmp8[2]));
                            # else
                                Ipp32s tmp = (((((Ipp32s)tmp8[2]) << 24) >> 8) +
                                                (((Ipp32s)tmp8[1]) << 8) +
                                                ((Ipp32s)tmp8[0]));
                            #endif
                                tmp16[ii] = (Ipp16s)((tmp + 128) >> 8);
                                tmp8 += 3;
                            }
                            inMediaBuffer.SetDataSize(2*(size/3));
                        }
                        else if (info_file_in.resolution == 32)
                        {
                            Ipp32s *tmp32 = (Ipp32s*)inMediaBuffer.GetBufferPointer();
                            for (ii = 0; ii < size/4; ii++)
                            {
                                tmp16[ii] = (Ipp16s)((tmp32[ii] + 32768) >> 16);
                            }
                            inMediaBuffer.SetDataSize(2*(size/4));
                        }
                    }

                    if (size == needSize)
                        StreamStatus = UMC_OK;
                    else
                        StreamStatus = UMC_ERR_END_OF_STREAM;

                    pMediaBuffer->UnLockInputBuffer(&inMediaBuffer, StreamStatus);
                }
                do
                {
                    sts = pMediaBuffer->LockOutputBuffer(&outMediaBuffer);

                    if (sts == UMC_OK)
                    {
                        if ((is_input_wav) && (size == 0))
                        { /* Last frames of encoder */
                            tickEncode   = vm_time_get_tick();
                            sts = pAudioEncoder->GetFrame(NULL, pOut);
                            fTimeEncode += (vm_time_get_tick() - tickEncode);
                        }
                        else
                        {
                            tickEncode   = vm_time_get_tick();
                            sts = pAudioEncoder->GetFrame(&outMediaBuffer, pOut);
                            fTimeEncode += (vm_time_get_tick() - tickEncode);
                        }

                        pMediaBuffer->UnLockOutputBuffer(&outMediaBuffer);

                        if (sts == UMC_OK)
                        {
#ifdef UMC_ENABLE_MP4_MUXER
                            if(params.isMP4MuxerRequired)
                            {
                                if(params.output_file.Size())
                                {
                                    umcRes = pMP4Muxer->PutData(pOut, 0);
                                    if (umcRes != UMC_OK)
                                    {
                                        vm_string_printf(VM_STRING("PutData failed\n"));
                                        err = UMC_ERR_FAILED;
                                        break;
                                    }
                                }

                                numFramesToProcess++;
                            }
                            else
#endif
                            {
                                if (numFramesToProcess == 0)
                                {
                                    sts = pAudioEncoder->GetInfo(audio_encoder_params);

                                    if (params.output_file.Size() && audio_encoder_params->m_info.streamSubtype == AAC_ALS_PROFILE)
                                    {
                                        p_out_file->Close();

                                        /* No need to add wav header for ALS profile */
                                        p_out_file = &file_out;
                                        err = p_out_file->Open((vm_char*)params.output_file, AudioFile::AFM_CREATE);

                                        if (err)
                                        {
                                            vm_string_printf(VM_STRING("Can't create output file '%s'"), (vm_char*)params.output_file);
                                            err = -3;
                                            break;
                                        }
                                    }
                                }

                                if(params.output_file.Size())
                                    p_out_file->Write(pOut->GetDataPointer(), pOut->GetDataSize());

                                if (size != 0)
                                {
                                    numFramesToProcess++;
                                }
                            }
                        }
                        else if (sts == UMC_ERR_NOT_ENOUGH_BUFFER)
                        {
                            UMC_DELETE(pOut);

                            sts = pAudioEncoder->GetInfo(audio_encoder_params);

                            pOut = new MediaData();
                            if (NULL == pOut)
                            {
                                vm_string_printf(VM_STRING("Memory allocation error\n"));
                                err = -1;
                                break;
                            }
                            pOut->Alloc(audio_encoder_params->m_iSuggestedOutputSize);
                            sts = UMC_OK;
                        }
                    }
                }
                while (sts == UMC_OK);
                if (err < 0) break;
            }
            while ((size) && (sts != UMC_ERR_INVALID_STREAM));
            if (err < 0) break;
        }

        pAudioEncoder->GetInfo(audio_encoder_params);

        UMC_DELETE(pStream);
        break;
    } // while (1)

    if(tickAll)
        fTimeAll = (Ipp64f)(vm_time_get_tick() - tickAll);

    if(params.output_file.Size())
    {
#ifdef UMC_ENABLE_MP4_MUXER
        if(params.isMP4MuxerRequired)
        {
            if (pMP4Muxer)
              pMP4Muxer->PutEndOfStream(0);
            UMC_DELETE(pFileWriterParams);
            UMC_DELETE(pMP4Muxer);
            UMC_DELETE(pMP4MuxerParams);
            UMC_DELETE(pMP4TrackParams);
            UMC_DELETE(pFileWriter);
        }
        else
#endif
            p_out_file->Close();
    }

    p_in_file->Close();

    if (acp_allocated)
        UMC_DELETE(audio_encoder_params);

    UMC_DELETE(pOut);
    UMC_DELETE(pAudioEncoder);
    UMC_DELETE(pMediaBuffer);

    UMC_DELETE(media_buffer_params);

    if(dpndTrTbl)
        ippFree(dpndTrTbl);

    UMC_DELETE(pMediaBufferTmp);
    UMC_DELETE(pStream);
    UMC_DELETE(pSpl);

    if(!err)
    {
        vm_string_printf(VM_STRING("\n"));
        vm_string_printf(VM_STRING("Statistic (%d frames):\n"), numFramesToProcess);
        if(fTimeAll)
        {
            vm_string_printf(VM_STRING("  Overall:    %6.4f sec; %7.2f fps\n"),
                fTimeAll/fTickFreq,
                fTickFreq*numFramesToProcess/fTimeAll);
        }
        if(fTimeEncode)
        {
            vm_string_printf(VM_STRING("  Encoding:   %6.4f sec; %7.2f fps\n"),
                fTimeEncode/fTickFreq,
                fTickFreq*numFramesToProcess/fTimeEncode);
        }
    }
    else
        vm_string_printf(VM_STRING("\nError: code %d\n"), err);

    return err;
}