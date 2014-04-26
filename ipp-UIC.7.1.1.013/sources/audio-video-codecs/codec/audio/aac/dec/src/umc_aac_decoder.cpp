/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_AAC_AUDIO_DECODER

#include "umc_aac_decoder.h"
#include "mp4cmn_config.h"
#include "aaccmn_adif.h"
#include "aaccmn_adts.h"
#include "aac_dec_own_fp.h"
#include "aac_dec_fp.h"
#include "vm_debug.h"
#include "umc_audio_data.h"

using namespace UMC;

/********************************************************************/

  AACDecoder::AACDecoder()
  {
    state = NULL;
    pObjMem = NULL;
    stateMemId = 0;
    objMemId = 0;
  }

/********************************************************************/

  Status AACDecoder::Init(BaseCodecParams * init)
  {
    AACStatus result;
    AudioDecoderParams *pAudioCodecInit = DynamicCast<AudioDecoderParams, BaseCodecParams>(init);
    Ipp32s mStateSize;

    // checks or create memory allocator;
    if (BaseCodec::Init(init) != UMC_OK) {
      return UMC_ERR_ALLOC;
    }

    result = aacdecGetSize(&mStateSize);
    if (result != AAC_OK)
      return StatusAAC_2_UMC(result);

    if (m_pMemoryAllocator->Alloc(&stateMemId, mStateSize,
      UMC_ALLOC_PERSISTENT) != UMC_OK) {
      return UMC_ERR_ALLOC;
    }

    state = (AACDec *)m_pMemoryAllocator->Lock(stateMemId);
    if(!state) {
      return UMC_ERR_ALLOC;
    }

    /* AYA: clear memory */
    ippsZero_8u( (Ipp8u*)state, mStateSize);

    result = aacdecInit(state);
    if (result != AAC_OK)
      return StatusAAC_2_UMC(result);

    initSubtype = 0;
    m_info.m_audio_object_type = AOT_UNDEF;
    m_info.m_decode_mode = DM_UNDEF_STREAM;
    m_info.m_init_stream_type = UNDEF_AUDIO;
    m_info.m_init_config_data_size = 0;
    m_info.m_sampling_frequency_index = 0;
    m_info.m_frame_number = 0;

    m_pts_prev = 0;
    params.is_valid = 0;

    if (!pAudioCodecInit)
      return UMC_OK;

    /* ***************************** *
     * usr settings for HEAAC
     * status = SetParams(init) - incorrect here
     * because default settings apply by constructor()
     * ***************************** */
    SetParams(init) ;
    /* is needed for post-parser process */
    state->psState.comState.modePS = state->com.m_flag_PS_support_lev;

    if (stateMemId) {
      if (UMC_OK != m_pMemoryAllocator->Unlock(stateMemId)) {
        return UMC_ERR_ALLOC;
      }
    }

    m_info.m_init_stream_type = pAudioCodecInit->m_info.streamType;

    if (NULL != pAudioCodecInit->m_pData) {
      m_info.m_init_config_data_size = pAudioCodecInit->m_pData->GetDataSize();

      if (AAC_MPEG4_STREAM == m_info.m_init_stream_type) {
        Ipp8u                  *inPointer;
        sAudio_specific_config audio_config_data;
        sBitsreamBuffer        BS;
        Status                 status;

        status = MemLock();
        if (status != UMC_OK) {
          return status;
        }

        inPointer = (Ipp8u *)pAudioCodecInit->m_pData->GetDataPointer();
        if (NULL == inPointer) {
          MemUnlock();
          return UMC_ERR_NULL_PTR;
        }

        GET_INIT_BITSTREAM(&BS, inPointer)

        BS.nDataLen = (Ipp32s)m_info.m_init_config_data_size;
        if (dec_audio_specific_config(&audio_config_data, &BS) < 0) {
          MemUnlock();
          return UMC_ERR_INIT;
        }

        /* if MP4 contains explicit HEAAC signalization       */
        /* then usrParam <ModeDwnsmplHEAACprofile> is ignored */

        if (5 == audio_config_data.extensionAudioObjectType ) {//AOT_SBR = 5
          if (audio_config_data.extensionSamplingFrequencyIndex ==
              audio_config_data.samplingFrequencyIndex ){
            aacdecSetSBRModeDwnsmpl(HEAAC_DWNSMPL_ON, state);
          } else {
            aacdecSetSBRModeDwnsmpl(HEAAC_DWNSMPL_OFF, state);
          }
        }

        /* WE SUPPORT "PS-TOOL" only for (MPEG4 + MONO + SBR/PS) */
#if !defined(ANDROID)
        if( !audio_config_data.sbrPresentFlag ){
#else
        if( audio_config_data.sbrPresentFlag == -1 ){
#endif
          state->com.m_flag_PS_support_lev = PS_DISABLE;
        }
        /* END SET_PARAM OF HEAAC FROM MP4 HEADER */

        m_info.m_audio_object_type =
          (AudioObjectType) audio_config_data.audioObjectType;

        status = SetObjectType(m_info.m_audio_object_type,
                               &audio_config_data.ALSSpecificConfig);
        if (status != UMC_OK) {
          MemUnlock();
          return status;
        }

        if (m_info.m_audio_object_type == AOT_ER_BSAC) {
          status = bsacdecSetNumChannels(audio_config_data.channelConfiguration, state);
          if (status != AAC_OK) {
            return UMC_ERR_INIT;
          }
        }

        m_info.m_decode_mode = DM_RAW_STREAM;

        if (m_info.m_audio_object_type != AOT_AAC_ALS) {
          m_info.m_sampling_frequency_index =
            get_sampling_frequency_index(&audio_config_data);

          if (audio_config_data.channelConfiguration == 0) {
            aacdecSetPCE(&audio_config_data.GASpecificConfig.pce, state);
          }

          /* Init tables */
          aacdecSetSamplingFrequency(m_info.m_sampling_frequency_index, state);
        }
        pAudioCodecInit->m_pData->MoveDataPointer((Ipp32s)m_info.m_init_config_data_size);

        MemUnlock();
      }
    } else {
      if (AAC_MPEG4_STREAM == m_info.m_init_stream_type) {
        return UMC_ERR_INVALID_PARAMS;
      }
    }

    return UMC_OK;
  }

/********************************************************************/

  Status  AACDecoder::SetParams(BaseCodecParams * params) {
    AACDecoderParams *info = DynamicCast<AACDecoderParams, BaseCodecParams>(params);

    if (info) {
      Status status;

      status = MemLock();
      if (status != UMC_OK) {
        return status;
      }

      if (state == NULL)
        return UMC_ERR_NOT_INITIALIZED;

      aacdecSetLayer(info->layer, state);

      if ((info->ModeDecodeHEAACprofile == HEAAC_HQ_MODE ||
           info->ModeDecodeHEAACprofile == HEAAC_LP_MODE) &&
          (info->ModeDwnsmplHEAACprofile == HEAAC_DWNSMPL_ON ||
           info->ModeDwnsmplHEAACprofile == HEAAC_DWNSMPL_OFF)) {

        /* SBR */
        aacdecSetSBRModeDecode(info->ModeDecodeHEAACprofile, state);
        aacdecSetSBRModeDwnsmpl(info->ModeDwnsmplHEAACprofile, state);
        aacdecSetSBRSupport(info->flag_SBR_support_lev, state);

        /* PS */
        if( info->flag_PS_support_lev == PS_ENABLE_BL ||
            info->flag_PS_support_lev == PS_ENABLE_UR){
          aacdecSetSBRModeDecode(HEAAC_HQ_MODE, state);
          aacdecSetSBRModeDwnsmpl(HEAAC_DWNSMPL_OFF, state);
        }

        if( info->flag_PS_support_lev == PS_ENABLE_BL ||
            info->flag_PS_support_lev == PS_ENABLE_UR ||
            info->flag_PS_support_lev == PS_PARSER    ||
            info->flag_PS_support_lev == PS_DISABLE){

          aacdecSetPSSupport(info->flag_PS_support_lev, state);
        } else {
          aacdecSetPSSupport(PS_DISABLE, state);
        }
      }
      MemUnlock();
    } else {
      return UMC_ERR_NULL_PTR;
    }

    return UMC_OK;
  }

/********************************************************************/

  Status  AACDecoder::SetObjectType(AudioObjectType mType,
                                    sALS_specific_config *inState)
  {
    Ipp32s mSize;
    AACStatus result;

    result = aacdecSetAudioObjectType(mType, state, inState, NULL, &mSize);
    if (result != AAC_OK) {
      return UMC_ERR_ALLOC;
    }

    if (m_pMemoryAllocator->Alloc(&objMemId, mSize, UMC_ALLOC_PERSISTENT) != UMC_OK) {
      return UMC_ERR_ALLOC;
    }

    pObjMem = (Ipp8u *)m_pMemoryAllocator->Lock(objMemId);
    if(!pObjMem) {
      return UMC_ERR_ALLOC;
    }

    result = aacdecSetAudioObjectType(mType, state, inState, pObjMem, &mSize);
    if (result != AAC_OK) {
      m_pMemoryAllocator->Free(objMemId);
      return UMC_ERR_ALLOC;
    }

    if (objMemId) {
      if (m_pMemoryAllocator->Unlock(objMemId) != UMC_OK) {
        return UMC_ERR_ALLOC;
      }
    }

    return UMC_OK;
  }

 /********************************************************************/

  Status  AACDecoder::GetFrame(MediaData * in,
                               MediaData * out)
  {
    AACStatus result;
    sBitsreamBuffer BS;
    Ipp32s res;
    Ipp32s nDecodedBytes, tmp_decodedBytes;
    sAdif_header           m_adif_header;
    sAdts_fixed_header     m_adts_fixed_header;
    sAdts_variable_header  m_adts_variable_header;
    Ipp8u *inPointer;
    Ipp32s inDataSize;
    Ipp64f pts_start;
    Ipp64f pts_end;
    Ipp32s SbrFlagPresent, frameSize, freq, ch, decodedBytes;
    Ipp32s outFrameLen, bitsPerSample;
    Ipp32s CRC = 0;
    Ipp32s crc_enable = 0;
    Status status;

    if (!in || !out)
      return UMC_ERR_NULL_PTR;

    if(!in->GetDataSize()) // zero-length sample
        return UMC_ERR_NOT_ENOUGH_DATA;

    status = MemLock();
    if (status != UMC_OK) {
      return status;
    }

    if (state == NULL)
      return UMC_ERR_NOT_INITIALIZED;

    pts_start = in->m_fPTSStart;

    aacdecInitCRC(0, state);

    if (AAC_MPEG4_STREAM != m_info.m_init_stream_type) {
      inPointer = (Ipp8u *)in->GetDataPointer();
      inDataSize = (Ipp32s)in->GetDataSize();

      if (inDataSize == 0) {
        MemUnlock();
        return UMC_ERR_NOT_ENOUGH_DATA;
      }

      result = aacdec_GetID3Len(inPointer, inDataSize, state);

      if (result != AAC_OK) {
        MemUnlock();
        return StatusAAC_2_UMC(result);
      }

      result = aacdec_SkipID3(inDataSize, &decodedBytes, state);
      in->MoveDataPointer(decodedBytes);

      if (result != AAC_OK) {
        MemUnlock();
        return StatusAAC_2_UMC(result);
      }
    }

    inPointer = (Ipp8u *)in->GetDataPointer();
    GET_INIT_BITSTREAM(&BS, inPointer)
    BS.nDataLen = (Ipp32s)in->GetDataSize();

    m_info.m_stream_subtype = UNDEF_AUDIO_SUBTYPE;

    if (DM_UNDEF_STREAM == m_info.m_decode_mode) {
      Ipp32u tmp;
      m_info.m_audio_object_type = AOT_UNDEF;

      /* PS tool isn't supported for raw AAC */
      state->com.m_flag_PS_support_lev = PS_DISABLE;
      /* is needed for post-parser process */
      state->psState.comState.modePS = state->com.m_flag_PS_support_lev;

      bs_save(&BS);
      GET_BITS(&BS, tmp, 32, Ipp32u)

      if (tmp == 0x414C5300) { /* ALS */
        sALS_specific_config inState;
        AudioDecoderParams paramsBefore;
        AudioDecoderParams paramsAfter;

        GetInfo(&paramsBefore);

        m_info.m_audio_object_type = AOT_AAC_ALS;

        if (decALSSpecificConfig(&inState, &BS) < 0) {
          Byte_alignment(&BS);
          GET_BITS_COUNT(&BS, nDecodedBytes)

          in->MoveDataPointer((nDecodedBytes >> 3));
          MemUnlock();

          return UMC_ERR_INIT;
        }

        status = SetObjectType(m_info.m_audio_object_type, &inState);
        if (status != UMC_OK) {
          MemUnlock();
          return status;
        }

        MemUnlock();
        MemLock();

        decALSGetDataFromSpecificConfig(&(state->alsState), &(inState.savedBS));

        m_info.m_decode_mode = DM_RAW_STREAM;

        GetInfo(&paramsAfter);

        if (paramsAfter.m_iSuggestedInputSize > paramsBefore.m_iSuggestedInputSize)
        {

          Byte_alignment(&BS);
          GET_BITS_COUNT(&BS, nDecodedBytes)
          nDecodedBytes >>= 3;

          in->MoveDataPointer(nDecodedBytes);
          out->SetDataSize(0);

          MemUnlock();
          return UMC_ERR_NOT_ENOUGH_DATA;
        }
      } else {
        bs_restore(&BS);

        res = dec_adif_header(&m_adif_header, &BS);
        if (res == 0) {
          sProgram_config_element *m_p_pce = &m_adif_header.pce[0];

          aacdecSetPCE(m_p_pce, state);
          m_info.m_sampling_frequency_index = m_p_pce->sampling_frequency_index;
          m_info.m_decode_mode = DM_RAW_STREAM;

          /* Init tables */
          aacdecSetSamplingFrequency(m_info.m_sampling_frequency_index, state);

          switch (m_p_pce->object_type) {
        case 0:
          m_info.m_audio_object_type = AOT_AAC_MAIN;
          break;
        case 1:
          m_info.m_audio_object_type = AOT_AAC_LC;
          break;
        case 2:
          m_info.m_audio_object_type = AOT_AAC_SSR;
          break;
        case 3:
          m_info.m_audio_object_type = AOT_AAC_LTP;
          break;
          }
          status = SetObjectType(m_info.m_audio_object_type, NULL);
          if (status != UMC_OK) {
            MemUnlock();
            return status;
          }

          MemUnlock();
          MemLock();
        } else {
          bs_restore(&BS);
          if ((0 == dec_adts_fixed_header(&m_adts_fixed_header, &BS)) &&
              (0 == dec_adts_variable_header(&m_adts_variable_header, &BS))) {

            bs_restore(&BS);

            m_info.m_sampling_frequency_index =
              m_adts_fixed_header.sampling_frequency_index;

            m_info.m_audio_object_type = (AudioObjectType)
              get_audio_object_type_by_adts_header(&m_adts_fixed_header);

            m_info.m_decode_mode = DM_ADTS_STREAM;

            status = SetObjectType(m_info.m_audio_object_type, NULL);
            if (status != UMC_OK) {
              MemUnlock();
              return status;
            }

            MemUnlock();
            MemLock();

            /* Init tables */
            aacdecSetSamplingFrequency(m_info.m_sampling_frequency_index, state);

            adts_sw = (0xFFF << 12) + ((m_adts_fixed_header.ID & 1) << 11) +
              ((m_adts_fixed_header.Layer & 3) << 8) +
              ((m_adts_fixed_header.Profile & 3) << 6) +
              ((m_adts_fixed_header.sampling_frequency_index & 15) << 2);
          }
        }
      }
    }

    if (DM_UNDEF_STREAM == m_info.m_decode_mode) {
      MemUnlock();
      return UMC_ERR_UNSUPPORTED;
    }

    if (m_info.m_audio_object_type == AOT_AAC_MAIN) {
        m_info.m_stream_subtype = AAC_MAIN_PROFILE;
    } else if (m_info.m_audio_object_type == AOT_AAC_LC) {
        m_info.m_stream_subtype = AAC_LC_PROFILE;
    } else if (m_info.m_audio_object_type == AOT_AAC_SSR) {
        m_info.m_stream_subtype = AAC_SSR_PROFILE;
    } else if (m_info.m_audio_object_type == AOT_AAC_LTP) {
        m_info.m_stream_subtype = AAC_LTP_PROFILE;
    } else if (m_info.m_audio_object_type == AOT_ER_BSAC) {
      m_info.m_stream_subtype = AAC_BSAC_PROFILE;
    } else if (m_info.m_audio_object_type == AOT_AAC_ALS) {
      m_info.m_stream_subtype = AAC_ALS_PROFILE;
    }

    if (AOT_UNDEF == m_info.m_audio_object_type) {
      MemUnlock();
      return UMC_ERR_UNSUPPORTED;
    }

    if (DM_ADTS_STREAM == m_info.m_decode_mode) {
      Ipp32s DataSize = (Ipp32s)in->GetDataSize();
      Ipp8u *tmp_ptr = (Ipp8u *)in->GetDataPointer();
      Ipp32u val;
      Ipp32u *crc_ptr;
      Ipp32s crc_offset;
      Ipp32s decodedBits0, decodedBits2;
      Ipp32s num_channel;

      if (DataSize < 9) {
        MemUnlock();
        return UMC_ERR_NOT_ENOUGH_DATA;
      }

      val = (tmp_ptr[0] << 16) + (tmp_ptr[1] << 8) + tmp_ptr[2];
      DataSize -= 3;
      tmp_ptr += 3;

      while (((val & 0xFFFEFC) != adts_sw) && (DataSize > 0)) {
        val = (val << 8) + tmp_ptr[0];
        DataSize--;
        tmp_ptr++;
      }

      if ((val & 0xFFFEFC) != adts_sw) {
        in->MoveDataPointer((Ipp32s)(in->GetDataSize()-2));
        MemUnlock();
        return UMC_ERR_NOT_ENOUGH_DATA;
      }

      DataSize += 3;
      in->MoveDataPointer((Ipp32s)(in->GetDataSize()-DataSize));

      if (DataSize < 9) {
        MemUnlock();
        return UMC_ERR_NOT_ENOUGH_DATA;
      }

      inPointer = (Ipp8u *)in->GetDataPointer();
      GET_INIT_BITSTREAM(&BS, inPointer)

      crc_ptr = BS.pCurrent_dword;
      crc_offset = BS.nBit_offset;
      GET_BITS_COUNT(&BS, decodedBits0)

      dec_adts_fixed_header(&m_adts_fixed_header, &BS);
      dec_adts_variable_header(&m_adts_variable_header, &BS);

      GET_BITS_COUNT(&BS, decodedBits2)

      if (m_adts_fixed_header.protection_absent == 0) {
        Ipp32s tmp_crc;
        CRC = (Ipp32s)Getbits(&BS, 16);
        aacdecInitCRC(1, state);
        aacdecGetCRC(&tmp_crc, state);
        bs_CRC_update(crc_ptr, crc_offset, decodedBits2 - decodedBits0, (Ipp32u*)&tmp_crc);
        aacdecSetCRC(tmp_crc, state);
        crc_enable = 1;
      }

      if (aacdecSetAdtsChannelConfiguration(
          m_adts_fixed_header.channel_configuration, state) != AAC_OK ) {
        in->MoveDataPointer(3);
        MemUnlock();
        return UMC_ERR_INVALID_STREAM;
      }

      Byte_alignment(&BS);
      GET_BITS_COUNT(&BS, tmp_decodedBytes)

      num_channel = m_adts_fixed_header.channel_configuration;
      if (num_channel == 7) num_channel = 8;

      if (m_adts_variable_header.aac_frame_length >
          num_channel * 768 + tmp_decodedBytes) {
        in->MoveDataPointer(3);
        MemUnlock();
        return UMC_ERR_INVALID_STREAM;
      }

      if (m_adts_variable_header.aac_frame_length > DataSize) {
        MemUnlock();
        return UMC_ERR_NOT_ENOUGH_DATA;
      }
    }

    Byte_alignment(&BS);
    GET_BITS_COUNT(&BS, tmp_decodedBytes)
    tmp_decodedBytes >>= 3;
    nDecodedBytes = 0;
    outFrameLen = 0; /* warning */

    if (m_info.m_audio_object_type == AOT_AAC_ALS) {

      result = alsdecGetFrame(inPointer + tmp_decodedBytes, &nDecodedBytes,
                              (Ipp8u *)out->GetDataPointer(), &outFrameLen,
                              ((Ipp32s)in->GetDataSize() - tmp_decodedBytes),
                              (Ipp32s)out->GetBufferSize() -
                              (Ipp32s)((Ipp8u *)out->GetDataPointer() -
                                       (Ipp8u *)out->GetBufferPointer()),
                              1, &(state->alsState));
    } else {

      result = aacdecGetFrame(inPointer + tmp_decodedBytes, &nDecodedBytes,
                              (Ipp16s *)out->GetDataPointer(),
                              ((Ipp32s)in->GetDataSize() - tmp_decodedBytes),
                              (Ipp32s)out->GetBufferSize() -
                              (Ipp32s)((Ipp8u *)out->GetDataPointer() -
                                      (Ipp8u *)out->GetBufferPointer()),
                              state);

      if (crc_enable) {
        if (AAC_OK == result) {
          Ipp32s streamCRC;
          aacdecGetCRC(&streamCRC, state);
          if (streamCRC != CRC) {
            result = AAC_BAD_STREAM;
          }
        }
      }
    }

    if (AAC_NOT_ENOUGH_BUFFER == result) {
      MemUnlock();
      return UMC_ERR_NOT_ENOUGH_BUFFER;
    }

    nDecodedBytes += tmp_decodedBytes;

    if (nDecodedBytes > (Ipp32s)in->GetDataSize()) {
#if !defined(ANDROID)
      if (AAC_MPEG4_STREAM == m_info.m_init_stream_type) {
        result = AAC_BAD_STREAM;
      } else {
        return UMC_ERR_NOT_ENOUGH_DATA;
      }
#else
        return UMC_ERR_NOT_ENOUGH_DATA;
#endif
    }

    SbrFlagPresent = 0;
    if (m_info.m_audio_object_type == AOT_AAC_ALS) {
      alsdecGetFrameSize(&frameSize, &(state->alsState));
      alsdecGetSampleFrequency(&freq, &(state->alsState));
      alsdecGetChannels(&ch, &(state->alsState));
      alsdecGetBitsPerSample(&bitsPerSample, &(state->alsState));
    } else {
      aacdecGetSbrFlagPresent(&SbrFlagPresent, state);
      aacdecGetFrameSize(&frameSize, state);
      aacdecGetSampleFrequency(&freq, state);
      aacdecGetChannels(&ch, state);
      bitsPerSample = 16;
    }

    if (AAC_BAD_STREAM == result) {
      Ipp32s size = frameSize * sizeof(Ipp16s) * ch;

      if (size > ((Ipp32s)out->GetBufferSize() - (Ipp32s)((Ipp8u *)out->GetDataPointer() -
        (Ipp8u *)out->GetBufferPointer()))) {
        MemUnlock();
        return UMC_ERR_NOT_ENOUGH_BUFFER;
      }
    }

    if ((0 == initSubtype) && (1 == SbrFlagPresent)) {
      initSubtype = 1;
      if (m_info.m_stream_subtype == AAC_MAIN_PROFILE) {
        m_info.m_stream_subtype =
          (UMC::AudioStreamSubType) (AAC_MAIN_PROFILE | AAC_HE_PROFILE);
      } else if (m_info.m_stream_subtype == AAC_LC_PROFILE) {
        m_info.m_stream_subtype =
          (UMC::AudioStreamSubType) (AAC_LC_PROFILE | AAC_HE_PROFILE);
      } else if (m_info.m_stream_subtype == AAC_SSR_PROFILE) {
        m_info.m_stream_subtype =
          (UMC::AudioStreamSubType) (AAC_SSR_PROFILE | AAC_HE_PROFILE);
      } else if (m_info.m_stream_subtype == AAC_LTP_PROFILE) {
        m_info.m_stream_subtype =
          (UMC::AudioStreamSubType) (AAC_LTP_PROFILE | AAC_HE_PROFILE);
      }
    }

#if !defined(ANDROID)
    if (AAC_MPEG4_STREAM == m_info.m_init_stream_type) {
      in->MoveDataPointer((Ipp32s)in->GetDataSize());
    } else {
      in->MoveDataPointer(nDecodedBytes);
    }
#else
    // modified to return the actual size decoded
    // NOTE: nDecodedBytes is wrong for some clip, this is a bug and need to be fixed
    in->MoveDataPointer(nDecodedBytes);
#endif

    if (AAC_BAD_STREAM == result) {
      Ipp32s size = frameSize * sizeof(Ipp16s) * ch;

      memset(out->GetDataPointer(),0,size);
    }

    {
      AudioData* pAudio = DynamicCast<AudioData, MediaData>(out);

      if (m_info.m_audio_object_type == AOT_AAC_ALS) {
        out->SetDataSize(outFrameLen);
      } else {
        out->SetDataSize(frameSize * sizeof(Ipp16s) * ch);
      }

      if (pts_start < 0)
        pts_start = m_pts_prev;

      m_pts_prev = pts_end = pts_start +
        ((Ipp64f)frameSize / (Ipp64f)freq);

      in->m_fPTSStart = pts_end;
      out->m_fPTSStart = pts_start;
      out->m_fPTSEnd   = pts_end;

      m_info.m_frame_number++;

        if(pAudio)
        {
            pAudio->m_iBitPerSample = bitsPerSample;
            pAudio->m_iChannels = ch;
            pAudio->m_iSampleFrequency = freq;

            switch(pAudio->m_iChannels)
            {
            case 1:
                pAudio->m_iChannelMask = CHANNEL_FRONT_CENTER;
                break;
            case 2:
                pAudio->m_iChannelMask  = CHANNEL_FRONT_LEFT;
                pAudio->m_iChannelMask |= CHANNEL_FRONT_RIGHT;
                break;
            case 3:
                pAudio->m_iChannelMask  = CHANNEL_FRONT_CENTER;
                pAudio->m_iChannelMask |= CHANNEL_FRONT_LEFT;
                pAudio->m_iChannelMask |= CHANNEL_FRONT_RIGHT;
                break;
            case 4:
                pAudio->m_iChannelMask  = CHANNEL_FRONT_CENTER;
                pAudio->m_iChannelMask |= CHANNEL_FRONT_LEFT;
                pAudio->m_iChannelMask |= CHANNEL_FRONT_RIGHT;
                pAudio->m_iChannelMask |= CHANNEL_LOW_FREQUENCY;
                break;
            case 5:
                pAudio->m_iChannelMask  = CHANNEL_FRONT_CENTER;
                pAudio->m_iChannelMask |= CHANNEL_FRONT_LEFT;
                pAudio->m_iChannelMask |= CHANNEL_FRONT_RIGHT;
                pAudio->m_iChannelMask |= CHANNEL_BACK_LEFT;
                pAudio->m_iChannelMask |= CHANNEL_BACK_RIGHT;
                break;
            case 6:
                pAudio->m_iChannelMask  = CHANNEL_FRONT_CENTER;
                pAudio->m_iChannelMask |= CHANNEL_FRONT_LEFT;
                pAudio->m_iChannelMask |= CHANNEL_FRONT_RIGHT;
                pAudio->m_iChannelMask |= CHANNEL_BACK_LEFT;
                pAudio->m_iChannelMask |= CHANNEL_BACK_RIGHT;
                pAudio->m_iChannelMask |= CHANNEL_LOW_FREQUENCY;
                break;
            default:
                break;
            }
        }
    }

    MemUnlock();
    return StatusAAC_2_UMC(result);
}

/********************************************************************/

  AACDecoder::~AACDecoder()
  {
    Close();
  }

/********************************************************************/

  Status AACDecoder::Close()
  {
    if ((state == NULL) && (pObjMem == NULL))
      return UMC_OK;

    if( m_pMemoryAllocator == NULL ) {
      return UMC_OK;
    }

    if (pObjMem) {
      m_pMemoryAllocator->Free(objMemId);
      pObjMem = NULL;
    }

    if (state) {
      m_pMemoryAllocator->Free(stateMemId);
      state = NULL;
    }

    BaseCodec::Close();
    return UMC_OK;
  }

/********************************************************************/

  Status  AACDecoder::Reset()
  {
    Status status;

    status = MemLock();
    if (status != UMC_OK) {
      return status;
    }

    if (state == NULL)
      return UMC_ERR_NOT_INITIALIZED;

    m_pts_prev = 0;
    aacdecReset(state);
    MemUnlock();
    return UMC_OK;
  }

/********************************************************************/

  Status  AACDecoder::GetInfo(BaseCodecParams * info)
  {
    Ipp32s SbrFlagPresent;
    Status status;

    if (!info)
      return UMC_ERR_NULL_PTR;

    AudioDecoderParams *p_info = DynamicCast<AudioDecoderParams, BaseCodecParams>(info);

    status = MemLock();
    if (status != UMC_OK) {
      return status;
    }

    aacdecGetInfo(&params, state);

    info->m_iSuggestedInputSize = params.m_SuggestedInputSize;
    info->m_iSuggestedOutputSize = params.m_SuggestedOutputSize;

    if(!p_info)
      return UMC_OK;

    p_info->m_info.iLevel = AAC_LEVEL_MAIN;

    if (params.is_valid) {
      p_info->m_info.iProfile = m_info.m_audio_object_type;
      aacdecGetSbrFlagPresent(&SbrFlagPresent, state);

      if (SbrFlagPresent) {
        p_info->m_info.iProfile |= AAC_PROFILE_SBR;
      }

      p_info->m_info.audioInfo.m_iBitPerSample = params.m_info_out.bitPerSample;
      p_info->m_info.audioInfo.m_iChannels = params.m_info_out.channels;
      p_info->m_info.streamType = AAC_AUDIO;
      p_info->m_info.audioInfo.m_iSampleFrequency = params.m_info_out.sample_frequency;
      p_info->m_info.iBitrate = params.m_info_out.bitrate;
      p_info->m_info.streamSubtype = m_info.m_stream_subtype;

      p_info->m_info.audioInfo.m_iChannelMask = 0;
      switch (p_info->m_info.audioInfo.m_iChannels) {
      case 1:
        p_info->m_info.audioInfo.m_iChannelMask  = CHANNEL_FRONT_CENTER;
        break;
      case 2:
        p_info->m_info.audioInfo.m_iChannelMask  = CHANNEL_FRONT_LEFT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_RIGHT;
        break;
      case 3:
        p_info->m_info.audioInfo.m_iChannelMask  = CHANNEL_FRONT_CENTER;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_LEFT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_RIGHT;
        break;
      case 4:
        p_info->m_info.audioInfo.m_iChannelMask  = CHANNEL_FRONT_CENTER;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_LEFT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_RIGHT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_LOW_FREQUENCY;
        break;
      case 5:
        p_info->m_info.audioInfo.m_iChannelMask  = CHANNEL_FRONT_CENTER;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_LEFT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_RIGHT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_BACK_LEFT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_BACK_RIGHT;
        break;
      case 6:
        p_info->m_info.audioInfo.m_iChannelMask  = CHANNEL_FRONT_CENTER;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_LEFT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_RIGHT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_BACK_LEFT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_BACK_RIGHT;
        p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_LOW_FREQUENCY;
        break;
      default:
        break;
      }
    }
    else
    {
        MemUnlock();
        return UMC_WRN_INFO_NOT_READY;
    }

    MemUnlock();
    return UMC_OK;
  }

/********************************************************************/

  Status AACDecoder::GetDuration(Ipp32f *p_duration)
  {
    Status status;

    if (!p_duration)
      return UMC_ERR_NULL_PTR;

    status = MemLock();
    if (status != UMC_OK) {
      return status;
    }

    if (state == NULL)
      return UMC_ERR_NOT_INITIALIZED;

    aacdecGetDuration(p_duration, state);
    MemUnlock();
    return UMC_OK;
  }

/********************************************************************/

  Status AACDecoder::StatusAAC_2_UMC(AACStatus st)
  {
    Status res;
    if (st == AAC_OK)
      res = UMC_OK;
    else if (st == AAC_NOT_ENOUGH_DATA)
      res = UMC_ERR_NOT_ENOUGH_DATA;
    else if (st == AAC_BAD_FORMAT)
      res = UMC_ERR_INVALID_STREAM;
    else if (st == AAC_ALLOC)
      res = UMC_ERR_ALLOC;
    else if (st == AAC_BAD_STREAM)
      res = UMC_ERR_INVALID_STREAM;
    else if (st == AAC_NULL_PTR)
      res = UMC_ERR_NULL_PTR;
    else if (st == AAC_NOT_FIND_SYNCWORD)
      res = UMC_ERR_SYNC;
    else if (st == AAC_UNSUPPORTED)
      res = UMC_ERR_UNSUPPORTED;
    else if (st == AAC_BAD_PARAMETER)
      res = UMC_ERR_UNSUPPORTED;
    else
      res = UMC_ERR_UNSUPPORTED;

    return res;
  }

  /****************************************************************************/

  Status AACDecoder::MemLock() {
    AACDec *pOldState = state;
    state = (AACDec *)m_pMemoryAllocator->Lock(stateMemId);
    if (!state) {
      return UMC_ERR_ALLOC;
    }
    if (state != pOldState) {
      aacdecUpdateStateMemMap(state, (Ipp32s)((Ipp8u *)state-(Ipp8u *)pOldState));
    }

    if (pObjMem) {
      Ipp8u *pOldObjMem = pObjMem;
      pObjMem = (Ipp8u *)m_pMemoryAllocator->Lock(objMemId);
      if (!state) {
        return UMC_ERR_ALLOC;
      }
      if (pObjMem != pOldObjMem) {
        aacdecUpdateObjMemMap(state, (Ipp32s)((Ipp8u *)pObjMem-(Ipp8u *)pOldObjMem));
      }
    }

    return UMC_OK;
  }

  /****************************************************************************/

  Status AACDecoder::MemUnlock() {
    if (stateMemId) {
      if (m_pMemoryAllocator->Unlock(stateMemId) != UMC_OK) {
        return UMC_ERR_ALLOC;
      }
    }

    if (objMemId) {
      if (m_pMemoryAllocator->Unlock(objMemId) != UMC_OK) {
        return UMC_ERR_ALLOC;
      }
    }
    return UMC_OK;
  }

#endif
