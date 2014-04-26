/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives AAC Decode Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ipplic.htm located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
//  MPEG-4 and AAC are international standards promoted by ISO, IEC, ITU, ETSI
//  and other organizations. Implementations of these standards, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include <math.h>

#include "aac_enc_own_fp.h"
#include "aac_enc_own.h"
#include "aac_enc_fp.h"
#include "aac_sfb_tables.h"
#include "aac_enc_huff_tables.h"
#include "aaccmn_const.h"
#include "aac_enc_psychoacoustic_fp.h"
#include "aac_filterbank_fp.h"
#include "aac_enc_quantization_fp.h"
#include "aac_enc_ltp_fp.h"
#include "align.h"
#include "sbr_settings.h"
#include "sbr_huff_tabs.h"
#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"

/* AYAlog */
#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

#include "ippcore.h"

/* Save bit buffer is zero initialized or not */
#define ZERO_SAVE_BUFFER

void aac_UpdateThr(
  sEnc_individual_channel_stream* pStream,
  sQuantizationData*              qData,
  Ipp32f                          NeededPE,
  Ipp32s*                         msMask,
  Ipp32s                          ms_mask_present,
  Ipp32s                          numCh);

/********************************************************************/

void aacencUpdateMemMap(AACEnc *state,
                        Ipp32s shift)
{
  Ipp32s i;
  Ipp32s chNum = state->com.m_channel_number;

  for (i = 0; i < 12; i++) {
    AAC_UPDATE_PTR(IppsVLCEncodeSpec_32s, state->com.huffman_tables[i], shift)
  }

  AAC_UPDATE_PTR(sOneChannelInfo, state->com.chInfo, shift)
  AAC_UPDATE_PTR(sCrcSaveTable, state->com.crcSaveTable, shift)

  AAC_UPDATE_PTR(Ipp16s*, state->com.buff, shift)
  for (i = 0; i < chNum; i++) {
    AAC_UPDATE_PTR(Ipp16s, state->com.buff[i], shift)
  }

  AAC_UPDATE_PTR(Ipp32f*, state->m_buff_pointers, shift)
  for (i = 0; i < 3*chNum; i++) {
    AAC_UPDATE_PTR(Ipp32f, state->m_buff_pointers[i], shift)
  }

  AAC_UPDATE_PTR(Ipp32f*, state->ltp_buff, shift)
  AAC_UPDATE_PTR(Ipp32f*, state->ltp_overlap, shift)

  AAC_UPDATE_PTR(sPsychoacousticBlock, state->psychoacoustic_block, shift)
  AAC_UPDATE_PTR(IppsFFTSpec_R_32f, state->psychoacoustic_block_com.pFFTSpecShort, shift)
  AAC_UPDATE_PTR(IppsFFTSpec_R_32f, state->psychoacoustic_block_com.pFFTSpecLong, shift)
  AAC_UPDATE_PTR(Ipp8u, state->psychoacoustic_block_com.pBuffer, shift)

  //AAC_UPDATE_PTR(IppsMDCTInvSpec_32f, state->filterbank_block.p_mdct_inv_long, shift)
  //AAC_UPDATE_PTR(IppsMDCTInvSpec_32f, state->filterbank_block.p_mdct_inv_short, shift)
  //AAC_UPDATE_PTR(Ipp8u, state->filterbank_block.p_buffer_inv, shift)

  //AAC_UPDATE_PTR(IppsMDCTFwdSpec_32f, state->filterbank_block.p_mdct_fwd_long, shift)
  //AAC_UPDATE_PTR(IppsMDCTFwdSpec_32f, state->filterbank_block.p_mdct_fwd_short, shift)
  //AAC_UPDATE_PTR(Ipp8u, state->filterbank_block.p_buffer_fwd, shift)

  if (state->com.audioObjectType == AOT_AAC_LTP) {
    for (i = 0; i < chNum; i++) {
      AAC_UPDATE_PTR(Ipp32f, state->ltp_buff[i], shift)
    }
    for (i = 0; i < chNum; i++) {
      AAC_UPDATE_PTR(Ipp32f, state->ltp_overlap[i], shift)
    }
    AAC_UPDATE_PTR(IppsFFTSpec_R_32f, state->corrFft, shift)
      AAC_UPDATE_PTR(Ipp8u, state->corrBuff, shift)
  }

  if (state->com.auxAudioObjectType == AOT_SBR) {
    for (i = 0; i < 10; i++) {
      AAC_UPDATE_PTR(IppsVLCEncodeSpec_32s, state->com.sbrHuffTabs[i], shift)
    }
    AAC_UPDATE_PTR(sSBREncState, state->sbrState, shift)
    AAC_UPDATE_PTR(ownFilterSpec_SBR_C_32fc, state->pQMFSpec, shift)
    AAC_UPDATE_PTR(Ipp8u, state->pQMFSpec->pMemSpec, shift)
    AAC_UPDATE_PTR(Ipp8u, state->pQMFSpec->pWorkBuf, shift)
    AAC_UPDATE_PTR(IppsFFTSpec_C_32fc, state->pQMFSpec->pFFTSpec, shift)
  }
}

/********************************************************************/

AACStatus aacencInit(AACEnc *state,
                     Ipp32s sampling_frequency,
                     Ipp32s chNum,
                     Ipp32s bit_rate,
                     enum AudioObjectType audioObjectType,
                     enum AudioObjectType auxAudioObjectType,
                     Ipp32s stereo_mode,
                     Ipp32s ns_mode,
                     Ipp32s *size_all)
{
  AACEnc_com *state_com;
  Ipp8u *ptr;
  Ipp32s *sfb_offset_for_short;
  Ipp32s sf_index;
  Ipp32s i, k, w;
  Ipp32s size, tmpSize;
  Ipp32s num_sfb_for_short;
  AACStatus aacStatus = AAC_OK;
  IppStatus ippStatus = ippStsNoErr;

  if (chNum < 1) {
    return AAC_BAD_PARAMETER;
  }

  sf_index = 12;
  for (i = 0; i < 12; i ++) {
    if (sfb_tables[i].samp_rate == sampling_frequency) {
      sf_index = i;
      break;
    }
  }

  if (sf_index == 12)
    return AAC_BAD_PARAMETER;

  size = sizeof(AACEnc);
  if (auxAudioObjectType == AOT_SBR) {
    size += sizeof(sSBREncState);
  }

  ptr = (Ipp8u *)state;
  if (state) {
    ippsZero_8u(ptr, size);

    state_com = &(state->com);
    state_com->sampling_frequency_index = sf_index;
    state_com->m_sampling_frequency = sampling_frequency;
    state_com->m_frame_number = 0;
    state_com->m_buff_prev_index = 0;
    state_com->m_buff_curr_index = 1;
    state_com->m_buff_next_index = 2;
    state_com->stereo_mode_param = stereo_mode;
    state_com->audioObjectType = audioObjectType;
    ptr = (Ipp8u*)state + sizeof(AACEnc);

    if (auxAudioObjectType == AOT_SBR) {
      state_com->auxAudioObjectType = auxAudioObjectType;
      state->sbrState = (sSBREncState*)ptr;
      state->pQMFSpec = (ownFilterSpec_SBR_C_32fc*)((Ipp8u*)state->sbrState + sizeof(sSBREncState));

      aacStatus = ownAnalysisFilterInitAlloc_SBREnc_RToC_32f32fc(state->pQMFSpec,
                                                                 &tmpSize);
      if (aacStatus != AAC_OK) return aacStatus;

      size += tmpSize;
      state->com.sbrHuffTabs[0] = (IppsVLCEncodeSpec_32s*)((Ipp8u*)state->pQMFSpec + tmpSize);

      /* HUFFMAN TABS */
      ippStatus = ownInitSBREncHuffTabs((IppsVLCEncodeSpec_32s**)&(state->com.sbrHuffTabs),
                                        &tmpSize);
      if (ippStatus != ippStsNoErr) return AAC_ALLOC;

      ptr = (Ipp8u*)state->com.sbrHuffTabs[0] + tmpSize;
      size += tmpSize;

      /* copy from comStruct to locStruct */
      ippsCopy_8u((const Ipp8u *)(state->com.sbrHuffTabs),
                  (Ipp8u *)(state->sbrState->sbrHuffTabs),
                  NUM_SBR_HUFF_TABS * sizeof(IppsVLCEncodeSpec_32s*));
    }

    sfb_offset_for_short = sfb_tables[sf_index].sfb_offset_short_window;
    num_sfb_for_short = sfb_tables[sf_index].num_sfb_short_window;

    state_com->sfb_offset_for_short_window[0] = 0;
    k = 1;
    for (w = 0; w < 8; w ++) {
      for ( i = 0; i < num_sfb_for_short; i++) {
        state_com->sfb_offset_for_short_window[k] = state_com->sfb_offset_for_short_window[k-1] +
          (sfb_offset_for_short[i + 1] - sfb_offset_for_short[i]);
        k++;
      }
    }

    state_com->sfb_offset[0] = state_com->sfb_offset[1] = state_com->sfb_offset[3] =
      sfb_tables[sf_index].sfb_offset_long_window;
    state_com->sfb_offset[2] = state_com->sfb_offset_for_short_window;

    state_com->huffman_tables[0] = (IppsVLCEncodeSpec_32s*)ptr;
    aacStatus = BuildHuffmanTables((IppsVLCEncodeSpec_32s**)(&state_com->huffman_tables),
                                   &tmpSize);
    if (aacStatus != AAC_OK) return aacStatus;

    size += tmpSize;
    ptr = (Ipp8u*)state_com->huffman_tables[0] + tmpSize;
    aacStatus = InitFilterbank(&(state->filterbank_block), ptr, FB_DECODER | FB_ENCODER,
                               audioObjectType, &tmpSize);
    if (aacStatus != AAC_OK) return aacStatus;

    size += tmpSize;
    ptr += tmpSize;
    aacStatus = InitPsychoacousticCom(&state->psychoacoustic_block_com, ptr,
                                      sf_index, ns_mode, &tmpSize);
    if (aacStatus != AAC_OK) return aacStatus;

    state->psychoacoustic_block_com.filterbank_block = &(state->filterbank_block);

    size += tmpSize;
    ptr += tmpSize;

    aacStatus = aacencSetNumChannel(chNum, state, ptr, audioObjectType,
                                    auxAudioObjectType, &tmpSize);
    if (aacStatus != AAC_OK) return aacStatus;

    size += tmpSize;
    ptr += tmpSize;

    aacStatus = aacencSetBitrate(bit_rate, state);
    if (aacStatus != AAC_OK) return aacStatus;
  } else {
    if (auxAudioObjectType == AOT_SBR) {
      aacStatus = ownAnalysisFilterInitAlloc_SBREnc_RToC_32f32fc(0, &tmpSize);
      if (aacStatus != AAC_OK) return aacStatus;

      size += tmpSize;
      ippStatus = ownInitSBREncHuffTabs(0, &tmpSize);
      if (ippStatus != ippStsNoErr) return AAC_ALLOC;

      size += tmpSize;
    }

    aacStatus = BuildHuffmanTables(0, &tmpSize);
    if (aacStatus != AAC_OK) return aacStatus;

    size += tmpSize;
    aacStatus = InitFilterbank(0, 0, FB_DECODER | FB_ENCODER, audioObjectType, &tmpSize);
    if (aacStatus != AAC_OK) return aacStatus;

    size += tmpSize;
    aacStatus = InitPsychoacousticCom(0, ptr, sf_index, ns_mode, &tmpSize);
    if (aacStatus != AAC_OK) return aacStatus;

    size += tmpSize;

    aacStatus = aacencSetNumChannel(chNum, 0, ptr, audioObjectType,
                                    auxAudioObjectType, &tmpSize);
    if (aacStatus != AAC_OK) return aacStatus;

    size += tmpSize;
  }

  if (audioObjectType == AOT_AAC_LTP) {
    Ipp32s sizeSpec, sizeInit, sizeWork;
    Ipp8u  *pBufInit;

    ippStatus = ippsFFTGetSize_R_32f(12, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone,
                                     &sizeSpec, &sizeInit, &sizeWork);
    if (ippStatus != ippStsNoErr) return AAC_ALLOC;

    size += (sizeSpec + sizeWork + sizeInit);

    if (state) {
      state->corrBuff = ptr + sizeSpec;
      pBufInit = state->corrBuff + sizeWork;

      state->corrFft = (IppsFFTSpec_R_32f*)ptr;
      ippStatus = ippsFFTInit_R_32f(&state->corrFft, 12, IPP_FFT_DIV_INV_BY_N,
                                    ippAlgHintNone, ptr, pBufInit);
      if (ippStatus != ippStsNoErr) return AAC_ALLOC;

      ptr += (sizeSpec + sizeWork + sizeInit);
    }
  }

  /* AYAlog */
#ifdef SBR_NEED_LOG
  if( (logFile  = fopen( "sbr_enc.log", "wt" )) == NULL ){
    printf( "The file 'sbr_enc.log' was not opened\n" );
  }
#endif

  *size_all = size;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacencSetNumChannel(Ipp32s chNum,
                              AACEnc *state,
                              Ipp8u  *mem,
                              enum AudioObjectType audioObjectType,
                              enum AudioObjectType auxAudioObjectType,
                              Ipp32s *size_all)
{
  AACEnc_com *state_com;
  Ipp8u  *ptr;
  Ipp32s *indxSbr_CE_Tabs = NULL;
  Ipp32f **m_buff_pointers;
  Ipp32f **ltp_buff;
  Ipp32f **ltp_overlap;
  Ipp16s **buff;
  Ipp32s lfe_channel_present;
  Ipp32s sizeOfOneChannelInfo, sizeOfPsychoacousticBlock, sizeOfPointers;
  Ipp32s sizeOfHeader, sizeOfFreqTabs, sizeOf_SCE_Element, sizeOfsCrcSaveTable;
  Ipp32s sce_tag;
  Ipp32s cpe_tag;
  Ipp32s lfe_tag;
  Ipp32s i, ch, size;
  Ipp32s upsample = (auxAudioObjectType == AOT_SBR) ? 2 : 1;

  sizeOf_SCE_Element = 0;
  sizeOfFreqTabs = 0;
  sizeOfHeader = 0;

  if (chNum < 1) {
    return AAC_BAD_PARAMETER;
  }

  lfe_channel_present = 0;
  if ((chNum > 4) && ((chNum & 1) == 0))
    lfe_channel_present = 1;

  sizeOfsCrcSaveTable = (sizeof(sCrcSaveTable) + 31) & (~31);
  sizeOfOneChannelInfo = (sizeof(sOneChannelInfo) + 31) & (~31);
  sizeOfPsychoacousticBlock = (sizeof(sPsychoacousticBlock) + 31) & (~31);
  sizeOfPointers = (chNum * 5 * sizeof(Ipp32f*) + chNum * sizeof(Ipp16s*) + 31) & (~31);

  size = sizeOfPointers + chNum * (sizeOfsCrcSaveTable + sizeOfOneChannelInfo +
         sizeOfPsychoacousticBlock + 3 * 1024 * sizeof(Ipp32f) +
         upsample * 1024 * sizeof(Ipp16s)) + 32;

  if (audioObjectType == AOT_AAC_LTP) {
    size += chNum * (4096 + 32 + 1024) * sizeof(Ipp32f);
  }

  if (auxAudioObjectType == AOT_SBR) {
    Ipp32s flag = 1;
    if (chNum > 2) flag = 2;

    sizeOfHeader = sizeof(sSBRHeader) * flag;
    sizeOfFreqTabs = sizeof(sSBRFeqTabsState) * flag;
    sizeOf_SCE_Element = sizeof(sSBREnc_SCE_State) * (chNum - lfe_channel_present);

    size += sizeOfHeader + sizeOfFreqTabs + sizeOf_SCE_Element;
    if (lfe_channel_present) {
      size += (2048 + SBR_SD_FILTER_DELAY)*sizeof(Ipp32f);
    }
  }

  if (state) {
    state_com = &(state->com);
    state_com->m_channel_number = chNum;

    if (auxAudioObjectType == AOT_SBR) {
      indxSbr_CE_Tabs = state->sbrState->indx_CE_Tab;
      for (i = 0; i < SBR_MAX_CH; i++) {
        indxSbr_CE_Tabs[i] = EMPTY_MAPPING;
      }
    }

    ptr = mem;
    ippsZero_8u(ptr, size);

    state_com->chInfo = (sOneChannelInfo *)ptr;
    ptr += chNum * sizeOfOneChannelInfo;

    state_com->crcSaveTable = (sCrcSaveTable *)ptr;
    ptr += chNum * sizeOfsCrcSaveTable;

    m_buff_pointers = (Ipp32f**)ptr;
    ltp_buff = m_buff_pointers + 3 * chNum;
    ltp_overlap = ltp_buff + chNum;
    buff = (Ipp16s**)(ltp_overlap + chNum);

    ptr += sizeOfPointers;

    state->psychoacoustic_block = (sPsychoacousticBlock *)ptr;
    ptr += chNum * sizeOfPsychoacousticBlock;

    for (i = 0; i < 3 * chNum; i++) {
      m_buff_pointers[i] = (Ipp32f*)ptr;
      ptr += 1024 * sizeof(Ipp32f);
    }

    if (audioObjectType == AOT_AAC_LTP) {
      for (i = 0; i < chNum; i++) {
        ltp_buff[i] = (Ipp32f*)ptr;
        ptr += (4096 + 32) * sizeof(Ipp32f);
      }

      for (i = 0; i < chNum; i++) {
        ltp_overlap[i] = (Ipp32f*)ptr;
        ptr += 1024 * sizeof(Ipp32f);
      }
    }

    for (i = 0; i < chNum; i++) {
      buff[i] = (Ipp16s*)ptr;
      ptr += upsample * 1024 * sizeof(Ipp16s);
    }

    state->m_buff_pointers = m_buff_pointers;
    state->ltp_buff = ltp_buff;
    state->ltp_overlap = ltp_overlap;
    state_com->buff = buff;
    state_com->lfe_channel_present = lfe_channel_present;

    if (auxAudioObjectType == AOT_SBR) {
      state->sbrState->sbrHeader = (sSBRHeader*)ptr;
      ptr += sizeOfHeader;

      state->sbrState->sbrFreqTabsState = (sSBRFeqTabsState*)ptr;
      ptr += sizeOfFreqTabs;

      /* <xCE> */
      state->sbrState->pSCE_Element = (sSBREnc_SCE_State*)ptr;
      ptr += sizeOf_SCE_Element;

      /* LFE */
      if (lfe_channel_present) {
        state->sbrState->pInputBufferLFE = (Ipp32f*)ptr;
      }
    }

    for (ch = 0; ch < chNum; ch++) {
      InitPsychoacoustic(&state->psychoacoustic_block_com,
                         &state->psychoacoustic_block[ch]);
    }

    /* ********************************************************************* */
    /*                     CONFIG CHANNEL ELEMENT                            */
    /* ********************************************************************* */

    i = 0;

    if (chNum != 2) {
      state_com->chInfo[0].element_id = ID_SCE;
      i += 1;
    }

    for (ch = i ; ch < chNum - lfe_channel_present; ch++) {
      state_com->chInfo[ch].element_id = ID_CPE;
    }

    if (lfe_channel_present) {
      state_com->chInfo[chNum-1].element_id = ID_LFE;
    }

    if (chNum == 4) {
      state_com->chInfo[chNum-1].element_id = ID_SCE;
    }

    sce_tag = 0;
    cpe_tag = 0;
    lfe_tag = 0;

    for (ch = 0; ch < chNum;) {
      state_com->chInfo[ch].prev_window_shape = 1;
      state_com->chInfo[ch].common_scalefactor_update = 2;
      state_com->chInfo[ch].last_frame_common_scalefactor = 0;

      if (state_com->chInfo[ch].element_id == ID_CPE) {
        state_com->chInfo[ch+1].common_scalefactor_update = 2;
        state_com->chInfo[ch+1].last_frame_common_scalefactor = 0;

        state_com->chInfo[ch].element_instance_tag = cpe_tag;
        state_com->chInfo[ch].max_bits_in_buf = 768 * 8 * 2;
        cpe_tag++;
        ch += 2;
      } else if (state_com->chInfo[ch].element_id == ID_SCE) {
        state_com->chInfo[ch].element_instance_tag = sce_tag;
        state_com->chInfo[ch].max_bits_in_buf = 768 * 8;
        sce_tag++;
        ch += 1;
      } else {
        state_com->chInfo[ch].element_instance_tag = lfe_tag;
        state_com->chInfo[ch].max_bits_in_buf = 768 * 8;
        lfe_tag++;
        ch += 1;
      }
    }

    if (auxAudioObjectType == AOT_SBR) {
      for (ch = 0; ch < chNum; ch++) {
        indxSbr_CE_Tabs[ch] = state_com->chInfo[ch].element_id;
      }
    }
  }

  *size_all = size;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacencSetBitrate(Ipp32s bit_rate,
                           AACEnc *state)
{
  AACEnc_com *state_com = &(state->com);
  sPsychoacousticBlockCom *pBlockCom = &(state->psychoacoustic_block_com);
  Ipp32s* sfb_offset_for_long;
  Ipp32s* sfb_offset_for_short;
#if !defined(ANDROID)
  Ipp32s indxSbrTuningTabs[3] = {EMPTY_MAPPING, EMPTY_MAPPING, EMPTY_MAPPING};
#else
  static Ipp32s indxSbrTuningTabs[3] = {EMPTY_MAPPING, EMPTY_MAPPING, EMPTY_MAPPING};
#endif
  Ipp32s sampling_frequency = state_com->m_sampling_frequency;
  Ipp32s sf_index = state_com->sampling_frequency_index;
  Ipp32s chNum = state_com->m_channel_number;
  Ipp32s lfe_channel_present = state_com->lfe_channel_present;
  Ipp32s bits_for_lfe = 0;
  Ipp32s bits_per_frame;
  Ipp32s max_line, max_sfb;
  Ipp32s num_sfb_for_long;
  Ipp32s num_sfb_for_short;
  Ipp32s i, j, ch;
  Ipp32f norm;
  Ipp32f f_bits_per_frame;
  Ipp32f bit_rate_per_ch;
  Ipp32f cutoff_frequency;
  Ipp32f allowHolesFrequency;
  Ipp32f num_channel = (Ipp32f)(chNum - lfe_channel_present);
  AACStatus aacStatus = AAC_OK;

  if (num_channel <= 0) {
    return AAC_BAD_PARAMETER;
  }

  if (bit_rate <= 0) {
    return AAC_BAD_PARAMETER;
  }

  state_com->m_bitrate = bit_rate;

  /* <SCE> & <CPE> Elements: calculation needed bits */
  f_bits_per_frame = ((Ipp32f)bit_rate) * 1024/(sfb_tables[sf_index].samp_rate);
  if (f_bits_per_frame > 768.0f * 8 * chNum)
    f_bits_per_frame = 768.0f * 8 * chNum;
  bits_per_frame = (Ipp32s)f_bits_per_frame;

  bits_per_frame &= ~7;
  bits_per_frame -= 3; /* for ID_END */
  f_bits_per_frame = (Ipp32f)bits_per_frame;
  bit_rate_per_ch = ((Ipp32f)bit_rate) / chNum;

  /* <LFE> Element: calculation needed bits */
  if (lfe_channel_present != 0) {
    bits_for_lfe = (Ipp32s)(0.2 * f_bits_per_frame /
      ((chNum - lfe_channel_present) + 0.2 * lfe_channel_present));
    if (bits_for_lfe < 300) bits_for_lfe = 300;
    else if (bits_for_lfe > 768 * 8) bits_for_lfe = 768 * 8;

    f_bits_per_frame -= (Ipp32f)(bits_for_lfe * lfe_channel_present);
  }

  if (state_com->auxAudioObjectType == AOT_SBR) {
     Ipp32s sbr_bit_rate = (Ipp32s)(f_bits_per_frame * sfb_tables[sf_index].samp_rate / 1024.f);

    /* check params streams */
    /* simple model:        every channel has bit_rate/numCh [bits] per frame */
    /* if LFE present then: every channel has (bit_rate - bit_rate_LFE)/(numCh - 1) [bits] per frame */
    aacStatus = sbrencCheckParams(sampling_frequency, chNum, sbr_bit_rate, indxSbrTuningTabs);
    if (AAC_OK != aacStatus){
      return HEAAC_UNSUPPORTED;
    }

    aacStatus = sbrencReset(state->sbrState, indxSbrTuningTabs);
    if (AAC_OK != aacStatus) {
      return aacStatus;
    }
  }

  sfb_offset_for_long = sfb_tables[sf_index].sfb_offset_long_window;
  sfb_offset_for_short = sfb_tables[sf_index].sfb_offset_short_window;
  num_sfb_for_long = sfb_tables[sf_index].num_sfb_long_window;
  num_sfb_for_short = sfb_tables[sf_index].num_sfb_short_window;

  /* *************************************************************** */
  /*             IS SBR available THEN WE correct cutoff_freq        */
  /* *************************************************************** */
  if (AOT_SBR != state_com->auxAudioObjectType ) {
    ippsPow34_32f(&bit_rate_per_ch, &cutoff_frequency, 1);
    if (bit_rate_per_ch < 40000) {
      cutoff_frequency *= 4.3f;
    } else {
      cutoff_frequency *= 5.0f;
    }

    allowHolesFrequency = cutoff_frequency * 0.5f;

    if (cutoff_frequency > 20000)
      cutoff_frequency = 20000;

    if (allowHolesFrequency > 20000)
      allowHolesFrequency = 20000;

  } else {
    /* case (1) */
    //cutoff_frequency = 0.5*(Ipp32f)sampling_frequency *0.5;
    /* case (2) (3GPP formula) */
    {
      Ipp32s sampleRateSBR = sampling_frequency << 1;
      Ipp32s xFreq_SBR_Band = state->sbrState->sbrFreqTabsState->fLoBandTab[0];

      if (chNum > 2) {
        xFreq_SBR_Band = IPP_MIN (xFreq_SBR_Band, state->sbrState->sbrFreqTabsState[1].fLoBandTab[0]);
      }

      cutoff_frequency =  (Ipp32f)((xFreq_SBR_Band * sampleRateSBR / 64 + 1) >> 1);
      allowHolesFrequency = cutoff_frequency;
    }
  }

  max_line = (Ipp32s)(2048 * cutoff_frequency / (Ipp32f)sampling_frequency);
  if (max_line > 1024) max_line = 1024;

  max_sfb = num_sfb_for_long;

  for (i = 0; i < num_sfb_for_long; i++) {
    if (sfb_offset_for_long[i] >= max_line) {
      max_sfb = i;
      break;
    }
  }

  state_com->real_max_sfb[0] = state_com->real_max_sfb[1] =
    state_com->real_max_sfb[3] = max_sfb;

  state_com->real_max_line[0] = state_com->real_max_line[1] =
    state_com->real_max_line[3] = sfb_offset_for_long[max_sfb];

  state_com->non_zero_line[0] = state_com->non_zero_line[1] =
    state_com->non_zero_line[3] = max_line;

  max_line = (Ipp32s)(2048 * allowHolesFrequency / (Ipp32f)sampling_frequency);
  if (max_line > 1024) max_line = 1024;

  max_sfb = num_sfb_for_long;

  for (i = 0; i < num_sfb_for_long; i++) {
    if (sfb_offset_for_long[i] >= max_line) {
      max_sfb = i;
      break;
    }
  }

  state_com->allowHolesSfb[0] = state_com->allowHolesSfb[1] =
  state_com->allowHolesSfb[3] = max_sfb;

  max_line = (Ipp32s)(256 * cutoff_frequency / (Ipp32f)sampling_frequency);
  if (max_line > 128) max_line = 128;

  max_sfb = num_sfb_for_short;

  for (i = 0; i < num_sfb_for_short; i++) {
    if (sfb_offset_for_short[i] > max_line) {
      max_sfb = i;
      break;
    }
  }

  state_com->real_max_sfb[2] = max_sfb;
  state_com->real_max_line[2] = sfb_offset_for_short[max_sfb] * 8;
  state_com->non_zero_line[2] = max_line;

  max_line = (Ipp32s)(256 * allowHolesFrequency / (Ipp32f)sampling_frequency);
  if (max_line > 128) max_line = 128;

  max_sfb = num_sfb_for_short;

  for (i = 0; i < num_sfb_for_short; i++) {
    if (sfb_offset_for_short[i] > max_line) {
      max_sfb = i;
      break;
    }
  }

  state_com->allowHolesSfb[2] = max_sfb;

  pBlockCom->attackThreshold = (Ipp32f)(2530.0/sqrt(bit_rate_per_ch));

  /* min SNR calculation */
  /* long window */

  j = 1;
  for (i = 0; i < pBlockCom->longWindow->num_ptt; i++) {
    Ipp32f center = (pBlockCom->longWindow->w_high[i] + pBlockCom->longWindow->w_low[i]) * 0.5f;
    Ipp32f sfb_end = (Ipp32f)(sfb_offset_for_long[j] - 1);

    if (center >= sfb_end) {
      Ipp32f center1 = (pBlockCom->longWindow->w_high[i-1] + pBlockCom->longWindow->w_low[i-1]) * 0.5f;
      Ipp32f slope = (pBlockCom->longWindow->bval[i] - pBlockCom->longWindow->bval[i-1]) / (center - center1);
      while (center >= sfb_end) {
        state->minSNRLong[j-1] = pBlockCom->longWindow->bval[i-1] + slope * (sfb_end - center1);

        j++;
        if (j > num_sfb_for_long)
          break;
        sfb_end = (Ipp32f)(sfb_offset_for_long[j] - 1);
      }
    }
  }

  if (j <= num_sfb_for_long) {
    Ipp32f center0 = (pBlockCom->longWindow->w_high[pBlockCom->longWindow->num_ptt-2] +
                      pBlockCom->longWindow->w_low[pBlockCom->longWindow->num_ptt-2]) * 0.5f;
    Ipp32f center1 = (pBlockCom->longWindow->w_high[pBlockCom->longWindow->num_ptt-1] +
                      pBlockCom->longWindow->w_low[pBlockCom->longWindow->num_ptt-1]) * 0.5f;
    Ipp32f bval0 = pBlockCom->longWindow->bval[pBlockCom->longWindow->num_ptt-2];
    Ipp32f bval1 = pBlockCom->longWindow->bval[pBlockCom->longWindow->num_ptt-1];
    Ipp32f slope = (bval1 - bval0) / (center1 - center0);

    while (j <= num_sfb_for_long) {
      state->minSNRLong[j-1] = bval0 + slope * ((sfb_offset_for_long[j] - 1) - center0);
      j++;
    }
  }

  norm = state->minSNRLong[0];
  for (i = state_com->real_max_sfb[0] - 1; i > 0; i--) {
    state->minSNRLong[i] -= state->minSNRLong[i-1];
    norm += state->minSNRLong[i];
  }

  norm = (Ipp32f)(0.6 * 1.18 * (f_bits_per_frame/num_channel - 140) / norm);

  for (i = 0; i < state_com->real_max_sfb[0]; i++) {
    Ipp32s width = sfb_offset_for_long[i+1] - sfb_offset_for_long[i];
    Ipp32f tmp;
    tmp = (Ipp32f)pow(2, state->minSNRLong[i]*norm/width);
    if (tmp < 1) tmp = 1;
    tmp = 1 / tmp;
    if (tmp < 0.00316f) tmp = 0.00316f; /* 25 dB */
    if (tmp > /*0.794f */0.631f)   tmp = /*0.794f */ 0.631f;   /*  1 dB 2 db*/
    state->minSNRLong[i] = tmp;
  }

  /* short window */

  j = 1;
  for (i = 0; i < pBlockCom->shortWindow->num_ptt; i++) {
    Ipp32f center = (pBlockCom->shortWindow->w_high[i] + pBlockCom->shortWindow->w_low[i]) * 0.5f;
    Ipp32f sfb_end = (Ipp32f)(sfb_offset_for_short[j] - 1);

    if (center >= sfb_end) {
      Ipp32f center1 = (pBlockCom->shortWindow->w_high[i-1] + pBlockCom->shortWindow->w_low[i-1]) * 0.5f;
      Ipp32f slope = (pBlockCom->shortWindow->bval[i] - pBlockCom->shortWindow->bval[i-1]) / (center - center1);
      while (center >= sfb_end) {
        state->minSNRShort[j-1] = pBlockCom->shortWindow->bval[i-1] + slope * (sfb_end - center1);

        j++;
        if (j > num_sfb_for_short)
          break;
        sfb_end = (Ipp32f)(sfb_offset_for_short[j] - 1);
      }
    }
  }

  if (j <= num_sfb_for_short) {
    Ipp32f center0 = (pBlockCom->shortWindow->w_high[pBlockCom->shortWindow->num_ptt-2] +
                      pBlockCom->shortWindow->w_low[pBlockCom->shortWindow->num_ptt-2]) * 0.5f;
    Ipp32f center1 = (pBlockCom->shortWindow->w_high[pBlockCom->shortWindow->num_ptt-1] +
                      pBlockCom->shortWindow->w_low[pBlockCom->shortWindow->num_ptt-1]) * 0.5f;
    Ipp32f bval0 = pBlockCom->shortWindow->bval[pBlockCom->shortWindow->num_ptt-2];
    Ipp32f bval1 = pBlockCom->shortWindow->bval[pBlockCom->shortWindow->num_ptt-1];
    Ipp32f slope = (bval1 - bval0) / (center1 - center0);

    while (j <= num_sfb_for_short) {
      state->minSNRShort[j-1] = bval0 + slope * ((sfb_offset_for_short[j] - 1) - center0);
      j++;
    }
  }

  norm = state->minSNRShort[0];
  for (i = state_com->real_max_sfb[2] - 1; i > 0; i--) {
    state->minSNRShort[i] -= state->minSNRShort[i-1];
    norm += state->minSNRShort[i];
  }

  norm = (Ipp32f)(0.6 * 1.18 * (f_bits_per_frame/num_channel-140) / 8 / norm);

  for (i = 0; i < state_com->real_max_sfb[2]; i++) {
    Ipp32s width = sfb_offset_for_short[i+1] - sfb_offset_for_short[i];
    Ipp32f tmp;
    tmp = (Ipp32f)pow(2, state->minSNRShort[i]*norm/width);
    if (tmp < 1) tmp = 1;
    tmp = 1 / tmp;
    if (tmp < 0.00316f) tmp = 0.00316f; /* 25 dB */
    if (tmp > /*0.794f */0.631f)   tmp = /*0.794f */ 0.631f;   /*  1 dB 2 db*/
    state->minSNRShort[i] = tmp;
  }

  /* max_sfb for LFE channel calculating */

  if (cutoff_frequency > MAX_LFE_FREQUENCY) {
    cutoff_frequency = MAX_LFE_FREQUENCY;

    max_line = (Ipp32s)(2048 * cutoff_frequency / (Ipp32f)sampling_frequency);
    max_sfb = num_sfb_for_long;

    for (i = 0; i < num_sfb_for_long; i++) {
      if (sfb_offset_for_long[i] > max_line) {
        max_sfb = i;
        break;
      }
    }

    state_com->real_max_sfb_lfe[0] = state_com->real_max_sfb_lfe[1] =
      state_com->real_max_sfb_lfe[3] = max_sfb;

    state_com->real_max_line_lfe[0] = state_com->real_max_line_lfe[1] =
      state_com->real_max_line_lfe[3] = sfb_offset_for_long[max_sfb];

    state_com->non_zero_line_lfe[0] =
      state_com->non_zero_line_lfe[1] =
      state_com->non_zero_line_lfe[3] = max_line;

    max_line = (Ipp32s)(256 * cutoff_frequency / (Ipp32f)sampling_frequency);
    max_sfb = num_sfb_for_short;

    for (i = 0; i < num_sfb_for_short; i++) {
      if (sfb_offset_for_short[i] > max_line) {
        max_sfb = i;
        break;
      }
    }

    state_com->real_max_sfb_lfe[2] = max_sfb;
    state_com->real_max_line_lfe[2] = sfb_offset_for_short[max_sfb] * 8;
    state_com->non_zero_line_lfe[2] = max_line;

  } else {

    state_com->real_max_sfb_lfe[0] = state_com->real_max_sfb_lfe[1] =
      state_com->real_max_sfb_lfe[3] = state_com->real_max_sfb[0];

    state_com->real_max_sfb_lfe[2] = state_com->real_max_sfb[2];

    state_com->real_max_line_lfe[0] = state_com->real_max_line_lfe[1] =
      state_com->real_max_line_lfe[3] = state_com->real_max_line[0];

    state_com->real_max_line_lfe[2] = state_com->real_max_line[2];

    state_com->non_zero_line_lfe[0] = state_com->non_zero_line_lfe[1] =
      state_com->non_zero_line_lfe[3] = state_com->non_zero_line[0];

    state_com->non_zero_line_lfe[2] = state_com->non_zero_line[2];
  }

  for (ch = 0; ch < chNum;) {
    state_com->chInfo[ch].bits_in_buf = 0;
    if (state_com->chInfo[ch].element_id == ID_CPE) {
      state_com->chInfo[ch].mean_bits = (Ipp32s)(2 * f_bits_per_frame/num_channel);
      f_bits_per_frame -= (Ipp32f)(state_com->chInfo[ch].mean_bits);
#ifndef ZERO_SAVE_BUFFER
      state_com->chInfo[ch].bits_in_buf = state_com->chInfo[ch].max_bits_in_buf -
                                          state_com->chInfo[ch].mean_bits;
#endif
      ch += 2;
      num_channel -= 2;

      if ((state_com->stereo_mode_param != AAC_LR_STEREO) &&
          (state_com->stereo_mode_param != AAC_MS_STEREO) &&
          (state_com->stereo_mode_param != AAC_JOINT_STEREO)) {
        state_com->stereo_mode_param = AAC_JOINT_STEREO;
      }
    } else if (state_com->chInfo[ch].element_id == ID_SCE) {
      state_com->chInfo[ch].mean_bits = (Ipp32s)(f_bits_per_frame/num_channel);
      f_bits_per_frame -= (Ipp32f)(state_com->chInfo[ch].mean_bits);
#ifndef ZERO_SAVE_BUFFER
      state_com->chInfo[ch].bits_in_buf = state_com->chInfo[ch].max_bits_in_buf -
                                          state_com->chInfo[ch].mean_bits;
#endif
      ch += 1;
      num_channel -= 1;
    } else {
      state_com->chInfo[ch].mean_bits = bits_for_lfe;
#ifndef ZERO_SAVE_BUFFER
      state_com->chInfo[ch].bits_in_buf = state_com->chInfo[ch].max_bits_in_buf -
                                          state_com->chInfo[ch].mean_bits;
#endif
      ch += 1;
    }
  }

  for (ch = 0; ch < chNum;) {
    Ipp32f *peMin = &state->psychoacoustic_block[ch].peMin;
    Ipp32f *peMax = &state->psychoacoustic_block[ch].peMax;

    peMin[0] = 0.9f * ATTENUATION_DB * state_com->non_zero_line[0];
    peMax[0] = 1.1f * ATTENUATION_DB * state_com->non_zero_line[0];
    if (state_com->chInfo[ch].element_id == ID_CPE) {
      peMin[0] *= 2;
      peMax[0] *= 2;
      ch += 1;
    }
    ch += 1;
  }

  state->psychoacoustic_block_com.non_zero_line_long = state_com->non_zero_line[0];
  state->psychoacoustic_block_com.non_zero_line_short = state_com->non_zero_line[2];

  state->psychoacoustic_block_com.num_sfb_long = state_com->real_max_sfb[0];
  state->psychoacoustic_block_com.num_sfb_short = state_com->real_max_sfb[2];

  return AAC_OK;
}

/********************************************************************/

AACStatus aacencGetFrame(Ipp16s *inPointer,
                         Ipp32s *encodedBytes,
                         Ipp8u *outPointer,
                         AACEnc *state)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_line_i, 2*1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_line_i_ms, 2*1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_line_pred, 2*1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, predictedBuf, 2048);
  IPP_ALIGNED_ARRAY(32, Ipp32f, predictedSpectrum, 2*1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, inSignal, 4*1024);
  Ipp32s          sfb_offset[2][MAX_SECTION_NUMBER];
  Ipp32s          sfb_width[2][MAX_SECTION_NUMBER];
  Ipp32f          minSNR[2][MAX_SECTION_NUMBER];
  Ipp32f          energy[2][MAX_SECTION_NUMBER];
  Ipp32f          sfbPE[2][MAX_SECTION_NUMBER];
  Ipp32f          logEnergy[2][MAX_SECTION_NUMBER];
  Ipp32f          chPe[2], pe, outPe;
  Ipp32f          *mdct_line[2];
  Ipp32f          *p_mdct_line;
  Ipp32f          *p_mdct_line_pred[2];
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_line_i, 2*1024);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_line_i_ms, 2*1024);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_line_pred, 2*1024);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, predictedBuf, 2048);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, predictedSpectrum, 2*1024);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, inSignal, 4*1024);
  static Ipp32s          sfb_offset[2][MAX_SECTION_NUMBER];
  static Ipp32s          sfb_width[2][MAX_SECTION_NUMBER];
  static Ipp32f          minSNR[2][MAX_SECTION_NUMBER];
  static Ipp32f          energy[2][MAX_SECTION_NUMBER];
  static Ipp32f          sfbPE[2][MAX_SECTION_NUMBER];
  static Ipp32f          logEnergy[2][MAX_SECTION_NUMBER];
  static Ipp32f          chPe[2];
  static Ipp32f          *mdct_line[2];
  static Ipp32f          *p_mdct_line;
  static Ipp32f          *p_mdct_line_pred[2];
  Ipp32f           pe, outPe;
#endif
  Ipp32f          *p_smr, *ptrMinSNR;
  Ipp32f          *peMin, *peMax;
  AACEnc_com      *state_com;
#if !defined(ANDROID)
  sEnc_individual_channel_stream ics[2];
#else
  static sEnc_individual_channel_stream ics[2];
#endif
  sQuantizationBlock             quantization_block;
  sQuantizationData              qData[2];
  sEnc_single_channel_element    sce;
  sEnc_channel_pair_element      cpe;
  sBitsreamBuffer BS;
  sBitsreamBuffer *pBS = &BS;
  sCrcSaveTable*   pCrcSaveTable;
#if !defined(ANDROID)
  Ipp32f maxEn[2];
#else
  static Ipp32f maxEn[2];
#endif
  Ipp32f minMul, maxMul, bitMul;
  Ipp32f startMinMul, startMaxMul;
  Ipp32f slopeMinMul, slopeMaxMul;
  Ipp32f deltaPe, maxDeltaPe, peCoeff;
  Ipp32f deltaFullness;
  Ipp32f neededPE;
  Ipp32s win_seq;
  Ipp32s win_shape = 0;
  Ipp32s i, j, numCh, ch, procCh, sfb;
  Ipp32s bits_per_frame, bits_in_buf;
  Ipp32s used_bits, save_bits;
  Ipp32s max_bits_in_buf;
  Ipp32s max_sfb_pred = 0;
  Ipp32s maxBitsPerFrame, available_bits;
  AACStatus result = AAC_OK;

  /* HEAAC variable */
  Ipp32s upsample = 1;
    /* internal state */
  sBitsreamBuffer auxBS;
  sBitsreamBuffer *pAuxBS = &auxBS;
#if !defined(ANDROID)
  __ALIGN Ipp8u    bufAuxBS[4*100];
  Ipp32s payloadBits_FILL_SBR_data[12] = {0};
#else
  static __ALIGN Ipp8u    bufAuxBS[4*100];
  static Ipp32s payloadBits_FILL_SBR_data[12] = {0};
#endif

  /* code */
  if (!inPointer || !outPointer)
    return AAC_NULL_PTR;

  INIT_BITSTREAM(pBS, outPointer)
  state_com = &(state->com);

  /* AYAlog */
#ifdef SBR_NEED_LOG
  fprintf(logFile, "frame = %i\n", state_com->m_frame_number);
#endif

  ippsZero_32f(inSignal + 2048, 2048);

  ics[0].pHuffTables = (void**)&state_com->huffman_tables;
  ics[1].pHuffTables = (void**)&state_com->huffman_tables;

  ics[0].audioObjectType = state_com->audioObjectType;
  ics[1].audioObjectType = state_com->audioObjectType;

  ics[0].predictor_data_present = 0;
  ics[1].predictor_data_present = 0;

  sce.p_individual_channel_stream = &ics[0];

  cpe.p_individual_channel_stream_0 = &ics[0];
  cpe.p_individual_channel_stream_1 = &ics[1];

  numCh = state_com->m_channel_number;
  pCrcSaveTable = state_com->crcSaveTable;

  /* HEAAC change */
  if( AOT_SBR == state->com.auxAudioObjectType ){
    upsample = 2;
    state->sbrState->sbr_offset = 0;
  }

  //---------------------------------------------------------------------------

  ippsDeinterleave_16s(inPointer, numCh, upsample*1024, state_com->buff);

  //---------------------------------------------------------------------------
  for (ch = 0; ch < numCh; ch += procCh) {

    procCh = 1;

    if (state_com->chInfo[ch].element_id == ID_CPE) {
      procCh = 2;
    }

    /* ********************************************************************* */
    /*                   HEAAC PROFILE                                       */
    /* ********************************************************************* */
    if ((AOT_SBR == state->com.auxAudioObjectType) &&
        (state_com->chInfo[ch].element_id != ID_LFE)) {

      /* update buffer */
      for (i = 0; i < procCh; i++) {
        Ipp32s sbr_offset = state->sbrState->sbr_offset;
        Ipp32f* sbrInputBuffer = state->sbrState->pSCE_Element[sbr_offset].sbrInputBuffer;

        ippsMove_32f( sbrInputBuffer + 2048, sbrInputBuffer, SBR_QMFA_DELAY + TOT_SBR_INPUT_DELAY);
        ippsConvert_16s32f(state_com->buff[ch+i], sbrInputBuffer + TOT_SBR_INPUT_DELAY + SBR_QMFA_DELAY, 2048);

        sbrencResampler_v2_32f(sbrInputBuffer + TOT_SBR_INPUT_DELAY + SBR_QMFA_DELAY - (2*SBR_SD_FILTER_DELAY + 1),
                               state->m_buff_pointers[3*(ch+i)+state_com->m_buff_next_index]);
      }

        /* main sbrEnc function */
        result = sbrencGetFrame( state->sbrState, state->pQMFSpec );
        if (result != AAC_OK) return result;

        /* cnt bits (FILL_SBR_DATA) */
        INIT_BITSTREAM(pAuxBS, bufAuxBS);
#ifdef SBR_NEED_LOG
        printf("\n START EMTY FILL BIT \n");
#endif
        payloadBits_FILL_SBR_data[ch] = enc_fill_element( state->sbrState, pAuxBS);
#ifdef SBR_NEED_LOG
        printf("\n END EMTY FILL BIT \n");
#endif
        SAVE_BITSTREAM(pAuxBS);

        /* check max_payload */
        if (payloadBits_FILL_SBR_data[ch] > SBR_MAX_SIZE_PAYLOAD_BITS) {
#ifdef SBR_NEED_LOG
          vm_string_printf(VM_STRING("sbr err: MAX_PAYLOAD has been culminated | patched 0\n"));
#endif
          payloadBits_FILL_SBR_data[ch] = 0;
        }

        /* move increment to end file */
        //state->sbrState->sbr_offset += procCh;

        /* dbg */
        //payloadBits_FILL_SBR_data = 0;

    } else if ((AOT_SBR == state->com.auxAudioObjectType) &&
               (state_com->chInfo[ch].element_id == ID_LFE)) {
      //---------------------------------
      Ipp32f* sbrInputBuffer = state->sbrState->pInputBufferLFE;

      i = 0;

      ippsMove_32f( sbrInputBuffer + 2048, sbrInputBuffer, SBR_SD_FILTER_DELAY);
      ippsConvert_16s32f(state_com->buff[ch+i], sbrInputBuffer + SBR_SD_FILTER_DELAY, 2048);

      sbrencResampler_v2_32f(sbrInputBuffer, state->m_buff_pointers[3*(ch+i)+state_com->m_buff_next_index]);

      //---------------------------------
    } else { //if AAC_LC/LTP/MAIN etc ...
      for (i = 0; i < procCh; i++) {
        ippsConvert_16s32f(state_com->buff[ch+i],
                           state->m_buff_pointers[3*(ch+i)+state_com->m_buff_next_index],
                           1024);
      }
    }
    /* ********************************************************************* */
    /*                     END of HEAAC PROFILE                              */
    /* ********************************************************************* */

    if (state_com->chInfo[ch].element_id != ID_LFE) {
#if !defined(ANDROID)
      sPsychoacousticBlock* pBlock[2];
      Ipp32s prev_window_shape[2];
      Ipp32s window_shape[2];
#else
      static sPsychoacousticBlock* pBlock[2];
      static Ipp32s prev_window_shape[2];
      static Ipp32s window_shape[2];
#endif

      for (i = 0; i < procCh; i++) {
        state->psychoacoustic_block_com.input_data[i][0] =
          state->m_buff_pointers[3*(ch+i)+state_com->m_buff_prev_index];

        state->psychoacoustic_block_com.input_data[i][1] =
          state->m_buff_pointers[3*(ch+i)+state_com->m_buff_curr_index];

        state->psychoacoustic_block_com.input_data[i][2] =
          state->m_buff_pointers[3*(ch+i)+state_com->m_buff_next_index];

        pBlock[i] = &state->psychoacoustic_block[ch+i];
        prev_window_shape[i] = state_com->chInfo[ch+i].prev_window_shape;
      }

      Psychoacoustic(pBlock, &state->psychoacoustic_block_com, mdct_line,
                     window_shape, prev_window_shape,
                     state_com->stereo_mode_param, procCh);

      for (i = 0; i < procCh; i++) {
        ics[i].window_shape = window_shape[i];
      }

      quantization_block.ns_mode = state->psychoacoustic_block_com.ns_mode;
    } else {
      state->psychoacoustic_block[ch].block_type = ONLY_LONG_SEQUENCE;
      mdct_line[0] = state->psychoacoustic_block[ch].r[0];
      ics[0].window_shape = 1;

      FilterbankEnc(&(state->filterbank_block),
                    state->m_buff_pointers[3*(ch)+state_com->m_buff_prev_index],
                    state->m_buff_pointers[3*(ch)+state_com->m_buff_curr_index],
                    ONLY_LONG_SEQUENCE, 1,
                    state_com->chInfo[ch].prev_window_shape,
                    mdct_line[0], 0);

      ippsZero_32f(mdct_line[0] + state_com->non_zero_line_lfe[0],
                   1024 - state_com->non_zero_line_lfe[0]);

      quantization_block.ns_mode = 0; /* No noiseShaping mode for LFE */
    }

    //---------------------------------------------------------------------------
    for (i = 0; i < procCh; i++) {
      win_seq = ics[i].windows_sequence =
        state->psychoacoustic_block[ch+i].block_type;

      win_shape = ics[i].window_shape;

      if (state_com->chInfo[ch+i].element_id != ID_LFE) {
        ics[i].max_sfb = state_com->real_max_sfb[win_seq];
        ics[i].max_line = state_com->real_max_line[win_seq];
        ics[i].ath_max_sfb = state_com->ath_max_sfb[win_seq];
      } else {
        ics[i].max_sfb = state_com->real_max_sfb_lfe[win_seq];
        ics[i].max_line = state_com->real_max_line_lfe[win_seq];
        ics[i].ath_max_sfb = state_com->ath_max_sfb_lfe[win_seq];
      }

      ics[i].sfb_offset = state_com->sfb_offset[win_seq];
    }

    for (i = 0; i < procCh; i++) {
      p_mdct_line_pred[i] = NULL;

      /* Prediction block */
      if (state_com->audioObjectType == AOT_AAC_LTP) {
        ics[i].ltp_data_present = 0;

        win_seq = ics[i].windows_sequence;
        win_shape = ics[i].window_shape;

        if (ics[i].windows_sequence != EIGHT_SHORT_SEQUENCE) {
          Ipp32f energyPred[MAX_LTP_SFB_LONG];
          Ipp32f savedBits = 0;
          max_sfb_pred = ics[i].max_sfb;
          if (max_sfb_pred > MAX_LTP_SFB_LONG) max_sfb_pred = MAX_LTP_SFB_LONG;

          ippsCopy_32f(state->m_buff_pointers[3*(ch+i)+state_com->m_buff_prev_index],
                       inSignal, 1024);
          ippsCopy_32f(state->m_buff_pointers[3*(ch+i)+state_com->m_buff_curr_index],
                       inSignal + 1024, 1024);

          ltpEncode(inSignal, state->ltp_buff[ch+i], predictedBuf,
                    &(ics[i].ltp_lag), &(ics[i].ltp_coef),
                    state->corrFft, state->corrBuff);

          if (ics[i].ltp_lag >= 0) {
            FilterbankEnc(&(state->filterbank_block),
                          predictedBuf, predictedBuf + 1024,
                          win_seq, win_shape,
                          state_com->chInfo[ch+i].prev_window_shape,
                          &predictedSpectrum[i*1024], 0);

            ippsSub_32f_I(predictedBuf, inSignal, 2048);

            ippsZero_32f(&predictedSpectrum[i*1024] + ics[i].sfb_offset[max_sfb_pred],
                         1024 - ics[i].sfb_offset[max_sfb_pred]);

            p_mdct_line_pred[i] = &mdct_line_pred[i*1024];
            ippsSub_32f(&predictedSpectrum[i*1024], mdct_line[i], p_mdct_line_pred[i], 1024);
            for (sfb = 0; sfb < max_sfb_pred; sfb++) {
              Ipp32s sfb_start = ics[i].sfb_offset[sfb];
              Ipp32s width = ics[i].sfb_offset[sfb+1] - ics[i].sfb_offset[sfb];

              ippsDotProd_32f(p_mdct_line_pred[i] + sfb_start, p_mdct_line_pred[i] + sfb_start,
                              width, energyPred + sfb);

              ippsDotProd_32f(mdct_line[i] + sfb_start, mdct_line[i] + sfb_start,
                              width, energy[i] + sfb);

              if (energyPred[sfb] < energy[i][sfb]) {
                ics[i].ltp_data_present = 1;
                ics[i].ltp_long_used[sfb] = 1;
                savedBits += width * ((Ipp32f)log10(energy[i][sfb]/energyPred[sfb]));
              } else {
                ics[i].ltp_long_used[sfb] = 0;
              }
            }

            savedBits = savedBits / state->psychoacoustic_block[ch].bitsToPECoeff;

            if (savedBits > (1 + 11 + 3 + max_sfb_pred)) {
              for (sfb = 0; sfb < max_sfb_pred; sfb++) {
               if (ics[i].ltp_long_used[sfb] == 1) {
                  Ipp32s sfb_start = ics[i].sfb_offset[sfb];
                  Ipp32s width = ics[i].sfb_offset[sfb+1] - ics[i].sfb_offset[sfb];

                  ippsCopy_32f(p_mdct_line_pred[i] + sfb_start, mdct_line[i] + sfb_start, width);
                  energy[i][sfb] = energyPred[sfb];
                }
              }
            } else {
              ics[i].ltp_data_present = 0;
            }
          }
        }
      } /* end of prediction block */
    }

    /* Mid-Side calculations */
    if ((procCh == 2) && (state_com->stereo_mode_param != AAC_LR_STEREO)) {
      Ipp32f *ptrLin = state->psychoacoustic_block[ch+0].r[0];
      Ipp32f *ptrRin = state->psychoacoustic_block[ch+1].r[0];
      Ipp32f *ptrLout;
      Ipp32f *ptrRout;
      Ipp32f mult = 0.5f;
      Ipp32s k;

      if (state_com->stereo_mode_param == AAC_MS_STEREO) {
        ptrLout = state->psychoacoustic_block[ch+0].r[0];
        ptrRout = state->psychoacoustic_block[ch+1].r[0];
      } else {
        ptrLout = state->psychoacoustic_block[ch+0].r[1];
        ptrRout = state->psychoacoustic_block[ch+1].r[1];
      }

      for (k = 0; k < 1024; k++) {
        Ipp32f tmpL = ptrLin[k];
        Ipp32f tmpR = ptrRin[k];

        ptrLout[k] = mult * (tmpL + tmpR);
        ptrRout[k] = mult * (tmpL - tmpR);
      }
    }

    /* Scalefactor grouping calculation */
    for (i = 0; i < procCh; i++) {
      ics[i].num_window_groups = 1;
      ics[i].len_window_group[0] = 1;
      qData[i].predAttackWindow = -1;

      if (ics[i].windows_sequence == EIGHT_SHORT_SEQUENCE) {
        Ipp32s *len_window_group = ics[i].len_window_group;
        Ipp32s num_window_groups = 1;
        Ipp32s g;
        Ipp32s attackIndex = state->psychoacoustic_block[ch+i].attackIndex;
#if !defined(ANDROID)
        Ipp32f tmpEn, enThr, shortEn[8];
#else
        static Ipp32f tmpEn, enThr, shortEn[8];
#endif

        maxEn[i] = 0;
        for (g = 0; g < 8; g++) {
          ippsDotProd_32f(mdct_line[i] + 128 * g + 32, mdct_line[i] + 128 * g + 32,
                          96, &shortEn[g]);

          if (shortEn[g] > maxEn[i]) {
            maxEn[i] = shortEn[g];
          }
        }

        enThr = state->psychoacoustic_block_com.attackThreshold * 0.5f;
        tmpEn = shortEn[0];
        num_window_groups = 1;
        len_window_group[0] = 1;

        if (state->psychoacoustic_block[ch+i].attackIndex < 0) {
          for (g = 1; g < 8; g++) {
            if ((shortEn[g] * enThr >= tmpEn) && (shortEn[g] <= enThr * tmpEn)) {
              len_window_group[num_window_groups-1]++;
            } else {
              tmpEn = shortEn[g];
              num_window_groups++;
              len_window_group[num_window_groups-1] = 1;
            }
          }
        } else {
          for (g = 1; g < attackIndex; g++) {
            if ((shortEn[g] * enThr >= tmpEn) && (shortEn[g] <= enThr * tmpEn)) {
              len_window_group[num_window_groups-1]++;
            } else {
              tmpEn = shortEn[g];
              num_window_groups++;
              len_window_group[num_window_groups-1] = 1;
            }
          }

          /* attackIndex */
          if (attackIndex != 0) {
            num_window_groups++;
            len_window_group[num_window_groups-1] = 1;
            qData[i].predAttackWindow = num_window_groups-2;
          }

          if (attackIndex < 7) {
            tmpEn = shortEn[attackIndex + 1];
            num_window_groups++;
            len_window_group[num_window_groups-1] = 1;

            for (g = attackIndex + 2; g < 8; g++) {
              if ((shortEn[g] * enThr >= tmpEn) && (shortEn[g] <= enThr * tmpEn)) {
                len_window_group[num_window_groups-1]++;
              } else {
                tmpEn = shortEn[g];
                num_window_groups++;
                len_window_group[num_window_groups-1] = 1;
              }
            }
          }
        }
        ics[i].num_window_groups = num_window_groups;
      }
    }

    if ((procCh == 2) && (state_com->stereo_mode_param != AAC_LR_STEREO)) {
      /* In the case of mid-side window groups should be */
      /* the same in the both channels                   */
      if (ics[0].windows_sequence == EIGHT_SHORT_SEQUENCE) {
        Ipp32s g;

        if (maxEn[0] > maxEn[1]) {
          ics[1].num_window_groups = ics[0].num_window_groups;
          for (g = 0; g < ics[0].num_window_groups; g++) {
            ics[1].len_window_group[g] = ics[0].len_window_group[g];
          }
        } else {
          ics[1].num_window_groups = ics[0].num_window_groups;
          for (g = 0; g < ics[0].num_window_groups; g++) {
            ics[1].len_window_group[g] = ics[0].len_window_group[g];
          }
        }
      }
    }

    for (i = 0; i < procCh; i++) {
      win_seq = ics[i].windows_sequence;

      /* Ipp16s block interleave */
      if (win_seq == EIGHT_SHORT_SEQUENCE) {
        Ipp32s *len_window_group = ics[i].len_window_group;
        Ipp32s *scale_factor_grouping = ics[i].scale_factor_grouping;
        Ipp32s *tmp_sfb_offset = state_com->sfb_offset[win_seq];
        Ipp32s num_window_groups = ics[i].num_window_groups;
        Ipp32s max_sfb = ics[i].max_sfb;
        Ipp32f *ptrIn = mdct_line[i];
        Ipp32f *ptrOut = &mdct_line_i[i*1024];
        Ipp32f *ptrNoiseIn, *ptrNoiseOut;
        Ipp32s g, sfb, w, ind;

        ind = 0;
        for (g = 0; g < num_window_groups; g++) {
          if (g != 0) {
            scale_factor_grouping[ind] = 0;
            ind++;
          }
          for (j = 1; j < len_window_group[g]; j++) {
            scale_factor_grouping[ind] = 1;
            ind++;
          }
        }

        /* Interleaving */
        sfb_offset[i][0] = 0;
        ind = 1;

        for (g = 0; g < num_window_groups; g++) {
          for (sfb = 0; sfb < max_sfb; sfb++) {
            Ipp32s sfb_start = tmp_sfb_offset[sfb];
            Ipp32s sfb_end = tmp_sfb_offset[sfb+1];
            Ipp32s width = sfb_end - sfb_start;

            sfb_offset[i][ind] = sfb_offset[i][ind - 1] +
                                 width * len_window_group[g];
            ind++;

            for (j = 0; j < len_window_group[g]; j++) {
              for (w = 0; w < width; w++) {
                *ptrOut = ptrIn[w + sfb_start + 128 * j];
                ptrOut++;
              }
            }
          }
          ptrIn += 128 * len_window_group[g];
        }

        ptrNoiseOut = &state->psychoacoustic_block_com.noiseThr[i][0];
        ptrNoiseIn = &state->psychoacoustic_block_com.noiseThr[i][0];
        ptrMinSNR = minSNR[i];
        for (g = 0; g < num_window_groups; g++) {
          for (sfb = 0; sfb < max_sfb; sfb++) {
            *ptrNoiseOut = ptrNoiseIn[sfb];
            *ptrMinSNR = state->minSNRShort[sfb];
            for (j = 1; j < len_window_group[g]; j++) {
              *ptrNoiseOut += ptrNoiseIn[sfb + MAX_SFB_SHORT * j];
            }
            ptrNoiseOut++;
            ptrMinSNR++;
          }
          ptrNoiseIn += MAX_SFB_SHORT * len_window_group[g];
        }

        if ((procCh == 2) && (state_com->stereo_mode_param == AAC_JOINT_STEREO)) {
          Ipp32f *ptrIn = state->psychoacoustic_block[ch+i].r[1];
          Ipp32f *ptrOut = &mdct_line_i_ms[i*1024];

          for (g = 0; g < num_window_groups; g++) {
            for (sfb = 0; sfb < max_sfb; sfb++) {
              Ipp32s sfb_start = tmp_sfb_offset[sfb];
              Ipp32s sfb_end = tmp_sfb_offset[sfb+1];
              Ipp32s width = sfb_end - sfb_start;

              for (j = 0; j < len_window_group[g]; j++) {
                for (w = 0; w < width; w++) {
                  *ptrOut = ptrIn[w + sfb_start + 128 * j];
                  ptrOut++;
                }
              }
            }
            ptrIn += 128 * len_window_group[g];
          }
        }
        ics[i].sfb_offset = &sfb_offset[i][0];
        p_mdct_line = &mdct_line_i[i*1024];
      } else {
        if (quantization_block.ns_mode) {
          for (sfb = 0; sfb < ics[i].max_sfb; sfb++) {
            minSNR[i][sfb] = state->minSNRLong[sfb];
          }
        }
        p_mdct_line = mdct_line[i];
      }

      /* Energy calculation */

      for (sfb = 0; sfb < ics[i].num_window_groups * ics[i].max_sfb; sfb++) {
        Ipp32s sfb_start = ics[i].sfb_offset[sfb];
        Ipp32s width = ics[i].sfb_offset[sfb+1] - ics[i].sfb_offset[sfb];

        ippsDotProd_32f(p_mdct_line + sfb_start, p_mdct_line + sfb_start,
                        width, energy[i] + sfb);
      }

      for (sfb = 0; sfb < ics[i].num_window_groups * ics[i].max_sfb; sfb++) {
        sfb_width[i][sfb] = ics[i].sfb_offset[sfb+1] - ics[i].sfb_offset[sfb];
      }

      ics[i].sfb_width = &sfb_width[i][0];

      qData[i].mdct_line = p_mdct_line;
      qData[i].noiseThr = p_smr = &state->psychoacoustic_block_com.noiseThr[i][0];
      qData[i].bitsToPeCoeff = &state->psychoacoustic_block[ch].bitsToPECoeff;
      qData[i].minSNR = minSNR[i];
      qData[i].energy = &energy[i][0];
      qData[i].logEnergy = &logEnergy[i][0];
      qData[i].sfbPE = &sfbPE[i][0];
      qData[i].scalefactorDataBits = &state->psychoacoustic_block[ch].scalefactorDataBits;
      qData[i].PEtoNeededPECoeff = &state->psychoacoustic_block[ch].PEtoNeededPECoeff;
      qData[i].allowHolesSfb = state_com->allowHolesSfb[ics[i].windows_sequence];
    }

    cpe.ms_mask_present = 0;
    if ((procCh == 2) && (state_com->stereo_mode_param == AAC_MS_STEREO))
      cpe.ms_mask_present = 2;

    /* Mid-Side decision */
    if ((procCh == 2) && (state_com->stereo_mode_param == AAC_JOINT_STEREO)) {
      Ipp32f energyMid, energySide;
      Ipp32f *p_mdct_line_m, *p_mdct_line_s;
      Ipp32f *p_mdct_line_0, *p_mdct_line_1;
      Ipp32s isLR, isMS;

      p_mdct_line_m = state->psychoacoustic_block[ch+0].r[1];
      p_mdct_line_s = state->psychoacoustic_block[ch+1].r[1];
      p_mdct_line_0 = mdct_line[0];
      p_mdct_line_1 = mdct_line[1];
      if (ics[0].windows_sequence == EIGHT_SHORT_SEQUENCE) {
        p_mdct_line_m = &mdct_line_i_ms[0*1024];
        p_mdct_line_s = &mdct_line_i_ms[1*1024];
        p_mdct_line_0 = &mdct_line_i[0*1024];
        p_mdct_line_1 = &mdct_line_i[1*1024];
      }

      isLR = isMS = 0;

      for (sfb = 0; sfb < ics[0].num_window_groups * ics[0].max_sfb; sfb++) {
        Ipp32f noiseThrLeft = state->psychoacoustic_block_com.noiseThr[0][sfb];
        Ipp32f noiseThrRight = state->psychoacoustic_block_com.noiseThr[1][sfb];
        Ipp32f noiseThrMin, noiseThrMax;
        Ipp32s sfb_start = ics[0].sfb_offset[sfb];
        Ipp32s width = ics[0].sfb_offset[sfb+1] - ics[0].sfb_offset[sfb];

        ippsDotProd_32f(p_mdct_line_m + sfb_start, p_mdct_line_m + sfb_start,
                        width, &energyMid);

        ippsDotProd_32f(p_mdct_line_s + sfb_start, p_mdct_line_s + sfb_start,
                        width, &energySide);

        if (noiseThrLeft > noiseThrRight) {
          noiseThrMin = noiseThrRight;
          noiseThrMax = noiseThrLeft;
        } else {
          noiseThrMin = noiseThrLeft;
          noiseThrMax = noiseThrRight;
        }

        if (noiseThrMax * energyMid * energySide <
            noiseThrMin * energy[0][sfb] * energy[1][sfb]) {

          cpe.ms_used[sfb] = 1;
          isMS = 1;
          state->psychoacoustic_block_com.noiseThr[0][sfb] = noiseThrMin;
          state->psychoacoustic_block_com.noiseThr[1][sfb] = noiseThrMin;
          energy[0][sfb] = energyMid;
          energy[1][sfb] = energySide;

          ippsCopy_32f(p_mdct_line_m + sfb_start,
                       p_mdct_line_0 + sfb_start, width);

          ippsCopy_32f(p_mdct_line_s + sfb_start,
                       p_mdct_line_1 + sfb_start, width);
        } else {
          cpe.ms_used[sfb] = 0;
          isLR = 1;
        }
      }

      if (isMS) {
        if (isLR) {
          cpe.ms_mask_present = 1;
        } else {
          cpe.ms_mask_present = 2;
        }
      } else {
        cpe.ms_mask_present = 0;
      }
    }

    /* Perceptual entropy calculation */
    pe = 0;
    for (i = 0; i < procCh; i++) {
      chPe[i] = 0;
      for (sfb = 0; sfb < ics[i].num_window_groups * ics[i].max_sfb; sfb++) {
        Ipp32s width = ics[i].sfb_offset[sfb+1] - ics[i].sfb_offset[sfb];
        Ipp32f tmp;

        if (energy[i][sfb] <= 0) {
          logEnergy[i][sfb] = -100000000;
        } else {
          logEnergy[i][sfb] = (Ipp32f)log10(energy[i][sfb]);
        }

        if (state->psychoacoustic_block_com.noiseThr[i][sfb] > 0) {
          tmp = (Ipp32f)log10(state->psychoacoustic_block_com.noiseThr[i][sfb]);
        } else {
          tmp = -100000000;
        }

        if (logEnergy[i][sfb] > tmp) {
          sfbPE[i][sfb] = width * (logEnergy[i][sfb] - tmp);
          chPe[i] += sfbPE[i][sfb];
        } else {
          sfbPE[i][sfb] = -1;
        }
      }
      if (ics[i].windows_sequence == EIGHT_SHORT_SEQUENCE) {
        chPe[i] *= 1.3f;
      }
      pe += chPe[i];
    }


    /* bits_per_frame counting */
    bits_per_frame = state_com->chInfo[ch].mean_bits;
    bits_in_buf = state_com->chInfo[ch].bits_in_buf;
    max_bits_in_buf = state_com->chInfo[ch].max_bits_in_buf;

    used_bits = 3; /* Syntactic element ID */
    cpe.common_window = 1; /* stupid MS warning */

    if (procCh == 1) {
      used_bits += enc_single_channel_element(&sce,
        state_com->chInfo[ch].element_instance_tag, pBS, 0, 0);
      save_bits = used_bits;
    } else {
      cpe.common_window = 0;
      if (ics[0].windows_sequence == ics[1].windows_sequence) {
        if (ics[0].num_window_groups == ics[1].num_window_groups) {
          Ipp32s g;
          cpe.common_window = 1;
          for (g = 0; g < ics[0].num_window_groups; g++) {
            if (ics[0].len_window_group[g] != ics[1].len_window_group[g]) {
              cpe.common_window = 0;
            }
          }
        }
      }

      used_bits += enc_channel_pair_element(&cpe,
        state_com->chInfo[ch].element_instance_tag, pBS, 0, 0);
      save_bits = (used_bits + 1) >> 1;
    }

    maxBitsPerFrame = (Ipp32s)(0.7 * state_com->chInfo[ch].mean_bits + bits_in_buf);

    win_seq = ONLY_LONG_SEQUENCE;

    for (i = 0; i < procCh; i++) {
      if (ics[i].windows_sequence == EIGHT_SHORT_SEQUENCE)
        win_seq = EIGHT_SHORT_SEQUENCE;
    }

    if (win_seq != EIGHT_SHORT_SEQUENCE) {
      startMinMul  =  0.7f;
      startMaxMul =  0.9f;
      slopeMinMul  = 0.35f;
      slopeMaxMul = 0.5f;
    } else {
      startMinMul  =  0.8f;
      startMaxMul =  0.95f;
      slopeMinMul  = 0.2f;
      slopeMaxMul = 0.55f;
    }

    if (state_com->chInfo[ch].element_id != ID_LFE) {
      peMin = &state->psychoacoustic_block[ch].peMin;
      peMax = &state->psychoacoustic_block[ch].peMax;

      deltaPe = pe - state->psychoacoustic_block[ch].peMin;
      maxDeltaPe = state->psychoacoustic_block[ch].peMax -
                   state->psychoacoustic_block[ch].peMin;

      if (deltaPe < 0) deltaPe = 0;
      if (deltaPe > maxDeltaPe) deltaPe = maxDeltaPe;

      peCoeff = deltaPe/maxDeltaPe;

      if (pe > 0) {
        if (pe > state->psychoacoustic_block[ch].peMax) {
          state->psychoacoustic_block[ch].peMin +=
            0.1f * (pe - state->psychoacoustic_block[ch].peMax);
          state->psychoacoustic_block[ch].peMax = pe;
        } else if (pe < state->psychoacoustic_block[ch].peMin) {
          state->psychoacoustic_block[ch].peMax -=
            0.1f * (state->psychoacoustic_block[ch].peMin - pe);
          state->psychoacoustic_block[ch].peMin = pe;
        }
      }
    } else {
      peCoeff = 0.5f;
    }

    deltaFullness = (Ipp32f)bits_in_buf / (Ipp32f)max_bits_in_buf;

    if (deltaFullness < 0) deltaFullness = 0;
    if (deltaFullness > 1) deltaFullness = 1;

    minMul = startMinMul + slopeMinMul * deltaFullness;
    maxMul = startMaxMul + slopeMaxMul * deltaFullness;

    bitMul = minMul + (maxMul - minMul) * peCoeff;

    bits_per_frame = (Ipp32s)(state_com->chInfo[ch].mean_bits * bitMul);

    available_bits = state_com->chInfo[ch].bits_in_buf +
                     state_com->chInfo[ch].mean_bits - used_bits;
    //-------
    /* ********************************************************************* */
    /*                   HEAAC PROFILE                                       */
    /* SIMPLE MODEL OF HEAAC BIT ALLOCATION -                                */
    /* "remove need bits from core AAC quantization bits"                    */
    /* (1)                                                                   */
    /* payloadBits_FILL_SBR_data =                                           */
    /*  = ext_FILL_EL + SBR_HEADER + SBR_DATA + align_BS_FILL_BITS           */
    /*                                                                       */
    /* (2)                                                                   */
    /* MAX( payloadBits_FILL_SBR_data ) = 270 bytes = 270*8 bits             */
    /*                                                                       */
    /* (3)                                                                   */
    /*  FIL_DATA = FIL_ID + count + esc_count(?) + payloadBits_FILL_SBR_data */
    /* ********************************************************************* */
    if( AOT_SBR == state->com.auxAudioObjectType ){
      Ipp32s tagBits = 3 + 4;
      Ipp32s count = 0;
      Ipp32s sum;

      count = IPP_MIN((payloadBits_FILL_SBR_data[ch])/8, 15);

#ifdef SBR_NEED_LOG
      printf("\ncnt (1) = %i\n", count);

       printf("\ncnt_bits = %i\n", payloadBits_FILL_SBR_data[ch]);
#endif

      if(15 == count){
        tagBits += 8;
      }

      sum = payloadBits_FILL_SBR_data[ch] + tagBits;

      bits_per_frame -= sum;

      available_bits -= sum;
    }

    if (bits_per_frame > maxBitsPerFrame) {
      bits_per_frame = maxBitsPerFrame;
    }

    if (quantization_block.ns_mode) {
      neededPE = qData[0].PEtoNeededPECoeff[0] * qData[0].bitsToPeCoeff[0] *
                 (bits_per_frame - (qData[0].scalefactorDataBits[0] * procCh));


      aac_UpdateThr(ics, qData, neededPE, cpe.ms_used, cpe.ms_mask_present, procCh);

      pe = 0;
      for (i = 0; i < procCh; i++) {
        chPe[i] = 0;
        for (sfb = 0; sfb < ics[i].num_window_groups * ics[i].max_sfb; sfb++) {
          if (sfbPE[i][sfb] > 0) {
            chPe[i] += sfbPE[i][sfb];
          }
        }
        pe += chPe[i];
      }
    }

    //---------------------------------------------------------------------------
    for (i = 0; i < procCh; i++) {
      if (pe > 0) {
        quantization_block.bits_per_frame = (Ipp32s)(bits_per_frame * chPe[i]/pe);
      } else {
        quantization_block.bits_per_frame = bits_per_frame / procCh;
      }

      if (quantization_block.bits_per_frame > (768 * 8 - save_bits)) {
        quantization_block.bits_per_frame = 768 * 8 - save_bits;
      } else if (quantization_block.bits_per_frame < 0) {
        quantization_block.bits_per_frame = 0;
      }

      if (quantization_block.ns_mode) {
        quantization_block.available_bits = available_bits;

        if (i == 0) {
          if (pe > 0) {
            quantization_block.available_bits =
              (Ipp32s)(quantization_block.available_bits * chPe[i]/pe);
          }
        }

        if (quantization_block.available_bits > (768 * 8 - save_bits)) {
          quantization_block.available_bits = 768 * 8 - save_bits;
        } else if (quantization_block.available_bits < 0) {
          quantization_block.available_bits = 0;
        }
      } else {
        quantization_block.available_bits = quantization_block.bits_per_frame;
      }

      quantization_block.common_scalefactor_update =
        &(state_com->chInfo[ch+i].common_scalefactor_update);
      quantization_block.last_frame_common_scalefactor =
        &(state_com->chInfo[ch+i].last_frame_common_scalefactor);

      Quantization(&quantization_block, &ics[i], &qData[i]);

      available_bits -= quantization_block.used_bits;

      if (state_com->audioObjectType == AOT_AAC_LTP) {
        ics[i].predictor_data_present = ics[i].ltp_data_present;
        if (ics[i].predictor_data_present) {
          for (sfb = 0; sfb < max_sfb_pred; sfb++) {
            if (!ics[i].ltp_long_used[sfb]) {
              Ipp32s begin = ics[i].sfb_offset[sfb];
              Ipp32s end = ics[i].sfb_offset[sfb+1];

              for (j = begin; j < end; j++) {
                predictedSpectrum[i*1024+j] = 0;
              }
            }
          }
        }
      }
    }

    if (quantization_block.ns_mode) {
      outPe = 0;

      for (i = 0; i < procCh; i++) {
        if (qData[i].outPe > 0) {
          outPe += qData[i].outPe;
        }
      }

      if (outPe > 0) {
        qData[0].PEtoNeededPECoeff[0] = qData[0].PEtoNeededPECoeff[0] * 0.9f +
                                        (pe/outPe) * 0.1f;
      }
    }

    //---------------------------------------------------------------------------
    if (state_com->audioObjectType == AOT_AAC_LTP) {
      Ipp32f *predSpectrum[2];
      Ipp32f *ltp_buff[2];
      Ipp32f *ltp_overlap[2];
      Ipp32s prev_window_shape[2];

      for (i = 0; i < procCh; i++) {
        predSpectrum[i] = &predictedSpectrum[i*1024];
        ltp_buff[i] = state->ltp_buff[ch+i];
        ltp_overlap[i] = state->ltp_overlap[ch+i];
        prev_window_shape[i] = state_com->chInfo[ch+i].prev_window_shape;
      }

      ltpBufferUpdate(ltp_buff, ltp_overlap, predSpectrum, &cpe,
                      &(state->filterbank_block),
                      state_com->sfb_offset[EIGHT_SHORT_SEQUENCE],
                      prev_window_shape, procCh);
    }

    for (i = 0; i < procCh; i++) {
      state_com->chInfo[ch+i].prev_window_shape = win_shape;
    }

    GET_BITS_COUNT(pBS, save_bits)

    //---------------------------------------------------------------------------
    if (state_com->chInfo[ch].element_id == ID_SCE) {
      PUT_BITS(pBS,ID_SCE,3);
      enc_single_channel_element(&sce, state_com->chInfo[ch].element_instance_tag,
                                 pBS, 1, pCrcSaveTable);
      pCrcSaveTable++;

    } else if (state_com->chInfo[ch].element_id == ID_LFE) {
      PUT_BITS(pBS,ID_LFE,3);
      enc_single_channel_element(&sce, state_com->chInfo[ch].element_instance_tag,
                                 pBS, 1, pCrcSaveTable);
      pCrcSaveTable++;
    } else {
      if (cpe.common_window) {
        if (state_com->audioObjectType == AOT_AAC_LTP) {
          if (ics[1].predictor_data_present) {
            ics[0].predictor_data_present = 1;
          }
        }
      }
      PUT_BITS(pBS,ID_CPE,3);
      enc_channel_pair_element(&cpe, state_com->chInfo[ch].element_instance_tag,
                               pBS, 1, pCrcSaveTable);
      pCrcSaveTable+=2;
    }

    //---------------------------------------------------------------------------
    if (AOT_SBR == state->com.auxAudioObjectType && state_com->chInfo[ch].element_id != ID_LFE ){

#ifdef SBR_NEED_LOG
      printf("\n START REAL FILL BIT \n");
#endif
      enc_fill_element_write( pBS, bufAuxBS, payloadBits_FILL_SBR_data[ch] );
#ifdef SBR_NEED_LOG
      printf("\n STOP REAL FILL BIT \n");
#endif

      /* correct work area */
      state->sbrState->sbr_offset += procCh;
    }
    //---------------------------------------------------------------------------
//#endif

    GET_BITS_COUNT(pBS, used_bits)
    used_bits -= save_bits;

    state_com->chInfo[ch].bits_in_buf +=
      state_com->chInfo[ch].mean_bits - used_bits;

    //if (state_com->chInfo[ch].bits_in_buf > state_com->chInfo[ch].max_bits_in_buf)
    //  state_com->chInfo[ch].bits_in_buf = state_com->chInfo[ch].max_bits_in_buf;
  }

  PUT_BITS(pBS,ID_END,3);
  SAVE_BITSTREAM(pBS)
  Byte_alignment(pBS);

  state_com->m_buff_prev_index++;
  if (state_com->m_buff_prev_index == 3)
    state_com->m_buff_prev_index = 0;

  state_com->m_buff_curr_index++;
  if (state_com->m_buff_curr_index == 3)
    state_com->m_buff_curr_index = 0;

  state_com->m_buff_next_index++;
  if (state_com->m_buff_next_index == 3)
    state_com->m_buff_next_index = 0;

  state->psychoacoustic_block_com.nb_curr_index++;
  state->psychoacoustic_block_com.nb_curr_index &= 1;

  state->psychoacoustic_block_com.nb_prev_index++;
  state->psychoacoustic_block_com.nb_prev_index &= 1;

  GET_BITS_COUNT(pBS, (*encodedBytes))
  *encodedBytes >>= 3;
  state_com->m_frame_number++;
  //fprintf(stderr,"%i\r", state_com->m_frame_number); fflush(stderr);

  return AAC_OK;
}

/********************************************************************/

AACStatus aacencFillProgramConfigElement(sProgram_config_element* p_data,
                                         Ipp32s element_instance_tag,
                                         AACEnc *state)
{
  Ipp32s ret = fill_program_config_element(p_data, element_instance_tag,
                                           &(state->com));
  if (ret < 0) return AAC_BAD_PARAMETER;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacencClose(AACEnc *state)
{
  Ipp32s ch;

  if (state == NULL)
    return AAC_OK;

  for (ch = 0; ch < state->com.m_channel_number; ch++) {
    if (state->psychoacoustic_block[ch].IIRfilterState) {
      ippFree(state->psychoacoustic_block[ch].IIRfilterState);
    }
  }

  /* AYAlog */
#ifdef SBR_NEED_LOG
  fclose(logFile);
#endif

  return AAC_OK;
}

/********************************************************************/

AACStatus aacencGetCrcSaveTable(sCrcSaveTable **crcSaveTable,
                                AACEnc *state)
{
  if (!state)
    return AAC_NULL_PTR;

  if (!crcSaveTable)
    return AAC_NULL_PTR;

  *crcSaveTable = state->com.crcSaveTable;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacencGetSampleFrequencyIndex(Ipp32s *freq_index,
                                        AACEnc *state)
{
  if (!state)
    return AAC_NULL_PTR;

  *freq_index = state->com.sampling_frequency_index;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacencGetNumEncodedFrames(Ipp32s *m_frame_number,
                                    AACEnc *state)
{
  if (!state)
    return AAC_NULL_PTR;

  *m_frame_number = state->com.m_frame_number;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacencGetDuration(Ipp32f *p_duration,
                            AACEnc *state)
{
  Ipp32f  duration;

  duration = (Ipp32f)(state->com.m_frame_number) * 1024;
  *p_duration = duration / (Ipp32f)(state->com.m_sampling_frequency);

  return AAC_OK;
}

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

