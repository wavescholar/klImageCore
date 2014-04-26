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

#ifdef UMC_ENABLE_MP3_AUDIO_DECODER
#include "umc_mp3_decoder.h"
#endif
#ifdef UMC_ENABLE_AAC_AUDIO_DECODER
#include "umc_aac_decoder.h"
#endif
#ifdef UMC_ENABLE_AC3_AUDIO_DECODER
#include "umc_ac3_decoder.h"
#endif

#ifdef UMC_ENABLE_MP4_SPLITTER
#include "umc_mp4_splitter.h"
#endif

#include "functions_common.h"
#include "umc_splitter.h"
#include "umc_audio_decoder.h"
#include "vm_sys_info.h"
#include "umc_sample_buffer.h"
#include "umc_linear_buffer.h"
#include "umc_parser_cmd.h"

#include "umc_app_utils.h"

#include "audio_file.h"
#include "params.h"
#include "simple_file.h"
#include "wav_file.h"

#include "ippcore.h"
#include "ipps.h"
#include "ippi.h"
#include "ippac.h"
#include "ippdc.h"
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
    PRINT_LIB_VERSION(dc, pVersion, cCVTString)
    PRINT_LIB_VERSION(vm, pVersion, cCVTString)
}

static void printHelp(ParserCmd *pCmd, vm_char *pErrorMessage)
{
    if(pCmd && !pCmd->m_bHelpPrinted)
    {
        vm_string_printf(VM_STRING("\nUsage: %s -c DecoderName [-i] InputFile [[-o] OutputFile] [Options]\n"), (vm_char*)pCmd->m_sProgNameShort);
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
    WavFile               wav_file_out;
    WavFile::Info         info_file_out;
    Ipp32s                err = 0;
    Ipp32s                isSplitterRequired = 0;
    Ipp32s                acp_allocated = 0;
    Ipp32s                numFramesToProcess = 0;
    MediaData*            pOut = NULL;
    MediaData             inMediaBuffer, outMediaBuffer;
    size_t                size = 0;
    Status                sts, StreamStatus = UMC_OK;
    AudioDecoder*         pAudioDecoder = NULL;
    MediaBuffer*          pMediaBuffer = NULL;
    MediaBuffer*          pMediaBufferTmp = NULL;
    AudioDecoderParams*   audio_decoder_params = NULL;
    MediaBufferParams *   media_buffer_params = NULL;
    Ipp8u*                dpndTrTbl = NULL;
    ProgramParameters     params;
    MediaData             in_data;
    Ipp32u                i, j;
    AudioStreamType       streamType;

    Ipp64f      fTickFreq          = (Ipp64f)vm_time_get_frequency();
    Ipp64f      fTimeDecode        = 0;
    Ipp64f      fTimeAll           = 0;
    vm_tick     tickDecode = 0, tickAll = 0, tickInt = 0;

    cmd.AddKey(VM_STRING("c"), VM_STRING(""),        VM_STRING("codec name (ac3, mp3, aac)"),           String);
    cmd.AddKey(VM_STRING("i"), VM_STRING(""),        VM_STRING("input file name"),                      String, 1, true);
    cmd.AddKey(VM_STRING("o"), VM_STRING(""),        VM_STRING("output file name"),                     String, 1, true);
    cmd.AddKey(VM_STRING("h"), VM_STRING("help"),    VM_STRING("print help"),                           Boolean);

    cmd.AddKey(VM_STRING(""),  VM_STRING("ipp_cpu"),     VM_STRING("target IPP optimization (SSE, SSE2, SSE3, SSSE3, SSE41, SSE42, AES, AVX, AVX2)"), String, 1, false, VM_STRING("IPP options"));
    cmd.AddKey(VM_STRING(""),  VM_STRING("ipp_threads"), VM_STRING("limit internal IPP threading"), Integer, 1, false, VM_STRING("IPP options"));

#if defined UMC_ENABLE_AC3_AUDIO_DECODER
    cmd.AddKey(VM_STRING(""), VM_STRING("ac3_u"),        VM_STRING("dualmonomode\n0 - STEREO (default)\n1 - LEFTMONO\n2 - RGHTMONO\n3 - MIXMONO"), Integer, 1, false, VM_STRING("AC3 options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("ac3_m"),        VM_STRING("output channel configuration\n0 - Dolby Surround\n1 - C\n2 - L, R\n3 - L, C, R\n4 - L, R, S\n5 - L, C, R, S\n6 - L, R, SL, SR\n7 - L, C, R, SL, SR (deafult)"), Integer, 1, false, VM_STRING("AC3 options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("ac3_c"),        VM_STRING("karaoke capable mode\n0 - no vocal channels\n1 - left vocal channel\n2 - right vocal channel\n3 - both vocal channels (deafult)"), Integer, 1, false, VM_STRING("AC3 options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("ac3_k"),        VM_STRING("DRC mode (default - 0)"),                   Integer, 1, false, VM_STRING("AC3 options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("ac3_x"),        VM_STRING("DRC high scale factor (default - 1.0)"),    Real, 1, false, VM_STRING("AC3 options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("ac3_y"),        VM_STRING("DRC low scale factor (default - 1.0)"),     Real, 1, false, VM_STRING("AC3 options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("ac3_lfe_off"),  VM_STRING("disable LFE channel"),                      Boolean, 1, false, VM_STRING("AC3 options"));
#endif

#if defined UMC_ENABLE_AAC_AUDIO_DECODER
    cmd.AddKey(VM_STRING(""), VM_STRING("aac_sbr_mode"),       VM_STRING("HE-AAC mode\n0 - disable\n1 - HQ mode (default)\n2 - LP mode"), Integer, 1, false, VM_STRING("AAC options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("aac_sbr_ds_off"),     VM_STRING("disable downsampling for HE-AAC"),     Boolean, 1, false, VM_STRING("AAC options"));
    cmd.AddKey(VM_STRING(""), VM_STRING("aac_ps_mode"),        VM_STRING("parametric stereo (PS) mode\n0 - disable (default)\n1 - enable PS parser\n2 - enable PS in baseline mode\n3 - enable PS in unrestricted mode"), Integer, 1, false, VM_STRING("AAC options"));
#endif

#if defined UMC_ENABLE_MP3_AUDIO_DECODER
    cmd.AddKey(VM_STRING(""), VM_STRING("mpeg_lfe_filt_off"),  VM_STRING("disable LFE channel filtering"),    Boolean, 1, false, VM_STRING("MPEG options"));
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

    p_in_file = &file_in;
    p_out_file = &wav_file_out;

    streamType = GetPreferredAudioDecoder(params.codec_name);
    switch(streamType)
    {
#ifdef UMC_ENABLE_AC3_AUDIO_DECODER
    case AC3_AUDIO:
        audio_decoder_params = DynamicCast<AudioDecoderParams, AC3DecoderParams>(&params.ac3dec_params);
        break;
#endif

#ifdef UMC_ENABLE_AAC_AUDIO_DECODER
    case AAC_AUDIO:
        audio_decoder_params = DynamicCast<AudioDecoderParams, AACDecoderParams>(&params.aacdec_params);
        break;
#endif

#ifdef UMC_ENABLE_MP3_AUDIO_DECODER
    case MPEG2_AUDIO:
        audio_decoder_params = DynamicCast<AudioDecoderParams, MP3DecoderParams>(&params.mp3dec_params);
        break;
#endif

    default:
        audio_decoder_params = new AudioDecoderParams;
        acp_allocated = 1;
        break;
    }

    if(NULL == audio_decoder_params)
    {
        vm_string_printf(VM_STRING("Memory allocation error\n"));
        return -1;
    }

    SplitterParams   splInitParams;
    SplitterInfo     *spl_info = NULL;
    FileReaderParams frInitParams;

    FileReader* pStream = NULL;
    Splitter*   pSpl     = NULL;

    Status umcRes = UMC_OK;
    Ipp32u trNumber = 0;

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

        // Select splitter
        SystemStreamType type_splitter = Splitter::GetStreamType(pStream);

#ifdef UMC_ENABLE_MP4_SPLITTER
        if(type_splitter == MP4_ATOM_STREAM)
        {
            pSpl = new MP4Splitter;
            if (NULL == pSpl)
            {
                vm_string_printf(VM_STRING("Memory allocation error\n"));
                err = -1;
                break;
            }

            if (NULL == pSpl)
            {
                vm_string_printf(VM_STRING("Required splitter is not supported. Decoding is not possible\n\n"));
                err = -6;
                break;
            };

            // Parameters of splitter
            splInitParams.m_iFlags      = AUDIO_SPLITTER;
            splInitParams.m_pDataReader = pStream;

            // Init Splitter
            if (pSpl->Init(&splInitParams) != UMC_OK)
            {
                vm_string_printf(VM_STRING("Splitter initialization error\n\n"));
                err = -1;
                break;
            }

            pSpl->Run();

            // Get info about a mp4 file
            pSpl->GetInfo(&spl_info);
            for (trNumber = 0; trNumber < spl_info->m_iTracks; trNumber++)
            {
                MP4TrackInfo *pMP4TrackInfo = (MP4TrackInfo *)spl_info->m_ppTrackInfo[trNumber];
                if ((spl_info->m_ppTrackInfo[trNumber]->m_bEnabled) &&
                (spl_info->m_ppTrackInfo[trNumber]->m_type & TRACK_ANY_AUDIO) &&
                (pMP4TrackInfo->m_DpndPID == 0))
                {
                    break;
                }
            }
            if (trNumber == spl_info->m_iTracks)
            {
                vm_string_printf(VM_STRING("Audio tracks are absent in input files\n\n"));
                err = -7;
                break;
            }

            audio_decoder_params->m_info = *((AudioStreamInfo *)(spl_info->m_ppTrackInfo[trNumber]->m_pStreamInfo));
            isSplitterRequired = 1;
        }
        else
#endif
        {
            isSplitterRequired = 0;
            audio_decoder_params->m_info.streamType = UNDEF_AUDIO;
            err = p_in_file->Open((vm_char*)params.input_file, AudioFile::AFM_NO_CONTENT_WRN);
            if (err < 0)
            {
                vm_string_printf(VM_STRING("Can't open input file '%s'"), (vm_char*)params.input_file);
                err = -2;
                break;
            }
        }

        switch(streamType)
        {
#ifdef UMC_ENABLE_AC3_AUDIO_DECODER
        case AC3_AUDIO:
            pAudioDecoder = new AC3Decoder();
            break;
#endif

#ifdef UMC_ENABLE_AAC_AUDIO_DECODER
        case AAC_AUDIO:
            pAudioDecoder = new AACDecoder();
            break;
#endif

#ifdef UMC_ENABLE_MP3_AUDIO_DECODER
        case MPEG2_AUDIO:
            pAudioDecoder = new MP3Decoder();
            break;
#endif

        default:
            pAudioDecoder = NULL;
            break;
        }

        if (pAudioDecoder == NULL)
        {
            vm_string_printf(VM_STRING("Can't create audio codec object '%s'\n"), (vm_char*)params.codec_name);
            err = -5;
            break;
        }
        if (isSplitterRequired)
        {
            audio_decoder_params->m_pData = spl_info->m_ppTrackInfo[trNumber]->m_pHeader;
            pMediaBuffer = (MediaBuffer*)new SampleBuffer();
            if (NULL == pMediaBuffer)
            {
                vm_string_printf(VM_STRING("Memory allocation error\n"));
                err = -1;
                break;
            }
        }
        else
        {
            audio_decoder_params->m_pData = NULL;
            pMediaBuffer = (MediaBuffer*)new LinearBuffer();

            if (NULL == pMediaBuffer)
            {
                vm_string_printf(VM_STRING("Memory allocation error\n"));
                err = -1;
                break;
            }
        }

        sts = pAudioDecoder->Init(audio_decoder_params);
        if (sts != UMC_OK)
        {
            vm_string_printf(VM_STRING("audioCodec Init failed\n"));
            err = UMC_ERR_FAILED;
            break;
        }

        sts = pAudioDecoder->GetInfo(audio_decoder_params);
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
        pOut->Alloc(audio_decoder_params->m_iSuggestedOutputSize);
        media_buffer_params = new MediaBufferParams();
        media_buffer_params->m_numberOfFrames      = 10;
        media_buffer_params->m_prefInputBufferSize =
            audio_decoder_params->m_iSuggestedInputSize * 10;
        media_buffer_params->m_prefOutputBufferSize =
            audio_decoder_params->m_iSuggestedInputSize * 10;

        sts = pMediaBuffer->Init(media_buffer_params);
        if (sts != UMC_OK)
        {
            vm_string_printf(VM_STRING("mediaBuffer Init failed\n"));
            err = UMC_ERR_FAILED;
            break;
        }

        if(params.output_file.Size())
        {
            err = p_out_file->Open((vm_char*)params.output_file, AudioFile::AFM_CREATE);
            if (err)
            {
                vm_string_printf(VM_STRING("Can't create output file '%s'"), (vm_char*)params.output_file);
                err = -3;
                break;
            }
        }

        tickAll = vm_time_get_tick();
        tickInt = tickAll;

        if (isSplitterRequired)
        {
#ifdef UMC_ENABLE_MP4_SPLITTER
            if (type_splitter == MP4_ATOM_STREAM)
            {
                MP4TrackInfo *pMP4TrackInfo = (MP4TrackInfo *)spl_info->m_ppTrackInfo[trNumber];
                Ipp8u *pTbl;
                Ipp32u baseID = pMP4TrackInfo->m_iPID;
                Ipp32u nDpndTracks = 1;

                dpndTrTbl = (Ipp8u*)ippMalloc(spl_info->m_iTracks);
                pTbl = dpndTrTbl;

                *pTbl++ = (Ipp8u)(trNumber & 0xFF);
                for (j = 0; j < spl_info->m_iTracks; j++)
                {
                    for (i = 0; i < spl_info->m_iTracks; i++)
                    {
                        pMP4TrackInfo = (MP4TrackInfo *)spl_info->m_ppTrackInfo[i];
                        if ((pMP4TrackInfo->m_bEnabled) &&
                            (pMP4TrackInfo->m_type & TRACK_ANY_AUDIO) &&
                            (pMP4TrackInfo->m_DpndPID == baseID))
                        {
                            *pTbl++ = (Ipp8u)(i & 0xFF);
                            nDpndTracks++;
                            baseID = pMP4TrackInfo->m_iPID;
                            break;
                        }
                    }
                }
                do
                {
                    sts = pMediaBuffer->LockInputBuffer(&inMediaBuffer);

                    if (sts == UMC_OK)
                    {
                        Ipp64f start = -2, end = -2;
                        Ipp8u *pInPtr = (Ipp8u *)inMediaBuffer.GetBufferPointer();
                        Ipp32u curSize = 0, eos_flag = 0;

                        /*** gluing dpnd tracks ***/
                        for (i = 0; i < nDpndTracks; i++)
                        {
                            umcRes = pSpl->GetNextData(&in_data, dpndTrTbl[i]);
                            while (umcRes == UMC_ERR_NOT_ENOUGH_DATA)
                            {
                                vm_time_sleep(5);
                                umcRes = pSpl->GetNextData(&in_data, dpndTrTbl[i]);
                            }
                            if (umcRes == UMC_OK)
                            {
                                memcpy(pInPtr, in_data.GetDataPointer(), in_data.GetDataSize());
                                pInPtr += in_data.GetDataSize();
                                curSize += (Ipp32u)in_data.GetDataSize();
                                if (start == -2)
                                {
                                    start = in_data.m_fPTSStart;
                                    end   = in_data.m_fPTSEnd;
                                }
                            }
                            else if (umcRes == UMC_ERR_END_OF_STREAM)
                            {
                                eos_flag = 1;
                            }
                        }

                        inMediaBuffer.SetDataSize(curSize);
                        inMediaBuffer.m_fPTSStart = start;
                        inMediaBuffer.m_fPTSEnd   = end;
                        if (eos_flag)
                        {
                            sts = pMediaBuffer->UnLockInputBuffer(&inMediaBuffer, UMC_ERR_END_OF_STREAM);
                        }
                        else
                        {
                            sts = pMediaBuffer->UnLockInputBuffer(&inMediaBuffer);
                        }
                    }
                    do
                    {
                        sts = pMediaBuffer->LockOutputBuffer(&outMediaBuffer);

                        if (sts == UMC_OK)
                        {
                            tickDecode   = vm_time_get_tick();
                            sts = pAudioDecoder->GetFrame(&outMediaBuffer, pOut);
                            fTimeDecode += (vm_time_get_tick() - tickDecode);

                            pMediaBuffer->UnLockOutputBuffer(&outMediaBuffer);

                            if (sts == UMC_OK)
                            {
                                if (numFramesToProcess == 0)
                                {
                                    sts = pAudioDecoder->GetInfo(audio_decoder_params);

                                    if(params.output_file.Size() && audio_decoder_params->m_info.streamSubtype == AAC_ALS_PROFILE)
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

                                numFramesToProcess++;
                            }
                            else if (sts == UMC_ERR_NOT_ENOUGH_BUFFER)
                            {
                                UMC_DELETE(pOut);

                                sts = pAudioDecoder->GetInfo(audio_decoder_params);

                                pOut = new MediaData();
                                if (NULL == pOut)
                                {
                                    vm_string_printf(VM_STRING("Memory allocation error\n"));
                                    err = -1;
                                    break;
                                }
                                pOut->Alloc(audio_decoder_params->m_iSuggestedOutputSize);
                                sts = UMC_OK;
                            }
                        }
                    }
                    while (sts == UMC_OK);
                    if (err < 0) break;
                }
                while ((umcRes != UMC_ERR_END_OF_STREAM) && (sts != UMC_ERR_INVALID_STREAM));
            }
#endif
        }
        else
        {
            do
            {
                sts = pMediaBuffer->LockInputBuffer(&inMediaBuffer);

                if (sts == UMC_OK)
                {
                    size_t needSize = inMediaBuffer.GetBufferSize();

                    size = p_in_file->Read(inMediaBuffer.GetBufferPointer(), needSize);
                    inMediaBuffer.SetDataSize(size);

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
                        tickDecode   = vm_time_get_tick();
                        sts = pAudioDecoder->GetFrame(&outMediaBuffer, pOut);
                        fTimeDecode += (vm_time_get_tick() - tickDecode);

                        pMediaBuffer->UnLockOutputBuffer(&outMediaBuffer);

                        /*** for decoding only ***/
                        if ((sts == UMC_ERR_NOT_ENOUGH_DATA))
                        {
                            /* Create new MediaBuffer */
                            pMediaBufferTmp = (MediaBuffer*)new LinearBuffer();

                            if (NULL == pMediaBufferTmp)
                            {
                                vm_string_printf(VM_STRING("Memory allocation error\n"));
                                err = -1;
                                break;
                            }

                            sts = pAudioDecoder->GetInfo(audio_decoder_params);

                            if (media_buffer_params->m_prefInputBufferSize <
                                audio_decoder_params->m_iSuggestedInputSize)
                            {
                                media_buffer_params->m_numberOfFrames      = 10;
                                media_buffer_params->m_prefInputBufferSize =
                                    audio_decoder_params->m_iSuggestedInputSize;
                                media_buffer_params->m_prefOutputBufferSize =
                                    audio_decoder_params->m_iSuggestedInputSize;

                                sts = pMediaBufferTmp->Init(media_buffer_params);
                                if (sts != UMC_OK)
                                {
                                    vm_string_printf(VM_STRING("mediaBuffer Init failed\n"));
                                    err = UMC_ERR_FAILED;
                                    break;
                                }

                                pMediaBufferTmp->LockInputBuffer(&inMediaBuffer);
                                pMediaBuffer->LockOutputBuffer(&outMediaBuffer);

                                memcpy(inMediaBuffer.GetBufferPointer(), outMediaBuffer.GetDataPointer(),
                                        outMediaBuffer.GetDataSize());

                                inMediaBuffer.SetDataSize(outMediaBuffer.GetDataSize());
                                inMediaBuffer.m_fPTSStart = outMediaBuffer.m_fPTSStart;
                                inMediaBuffer.m_fPTSEnd   = outMediaBuffer.m_fPTSEnd;
                                pMediaBufferTmp->UnLockInputBuffer(&inMediaBuffer, StreamStatus);

                                UMC_DELETE(pMediaBuffer);

                                pMediaBuffer = pMediaBufferTmp;
                                pMediaBufferTmp = NULL;
                            }
                            sts = UMC_ERR_NOT_ENOUGH_DATA;
                        }
                        else if (sts == UMC_OK)
                        {
                            if (numFramesToProcess == 0)
                            {
                                sts = pAudioDecoder->GetInfo(audio_decoder_params);

                                if (params.output_file.Size() && audio_decoder_params->m_info.streamSubtype == AAC_ALS_PROFILE)
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
                                numFramesToProcess++;
                        }
                        else if (sts == UMC_ERR_NOT_ENOUGH_BUFFER)
                        {
                            UMC_DELETE(pOut);

                            sts = pAudioDecoder->GetInfo(audio_decoder_params);

                            pOut = new MediaData();
                            if (NULL == pOut)
                            {
                                vm_string_printf(VM_STRING("Memory allocation error\n"));
                                err = -1;
                                break;
                            }
                            pOut->Alloc(audio_decoder_params->m_iSuggestedOutputSize);
                            sts = UMC_OK;
                        }
                        else
                            err = sts;
                    }
                }
                while (sts == UMC_OK);
                if (err < 0) break;
            }
            while ((size) && (sts != UMC_ERR_INVALID_STREAM));
        }

        pAudioDecoder->GetInfo(audio_decoder_params);

        {
            info_file_out.format_tag = 1;
            info_file_out.channels_number = audio_decoder_params->m_info.audioInfo.m_iChannels;
            info_file_out.sample_rate =
                audio_decoder_params->m_info.audioInfo.m_iSampleFrequency;
            info_file_out.resolution = audio_decoder_params->m_info.audioInfo.m_iBitPerSample;
            info_file_out.channel_mask = audio_decoder_params->m_info.audioInfo.m_iChannelMask;

            wav_file_out.SetInfo(&info_file_out);
        }

        UMC_DELETE(pStream);
        break;
    } // while (1)

    if(tickAll)
        fTimeAll = (Ipp64f)(vm_time_get_tick() - tickAll);

    if(params.output_file.Size())
        p_out_file->Close();

    if (!isSplitterRequired)
        p_in_file->Close();

    if (acp_allocated)
        UMC_DELETE(audio_decoder_params);

    UMC_DELETE(pOut);
    UMC_DELETE(pAudioDecoder);
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
        if(fTimeDecode)
        {
            vm_string_printf(VM_STRING("  Decoding:   %6.4f sec; %7.2f fps\n"),
                fTimeDecode/fTickFreq,
                fTickFreq*numFramesToProcess/fTimeDecode);
        }
    }
    else
        vm_string_printf(VM_STRING("\nError: code %d\n"), err);

    return err;
}