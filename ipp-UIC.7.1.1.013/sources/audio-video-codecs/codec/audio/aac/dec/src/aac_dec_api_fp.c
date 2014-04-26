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
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
//  MPEG-4 and AAC are international standards promoted by ISO, IEC, ITU, ETSI
//  and other organizations. Implementations of these standards, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

/********************************************************************/

/* #define NEW_FDP_VERSION */

/* There are 2 possible ways to process frequency domain prediction (FDP) and */
/* intensity stereo (IS):                                                     */
/* 1. Process IS before FDP                                                   */
/* 2. Process IS after FDP                                                    */
/* Unfortunately, various sets of test vectors use both ways, for example set */
/* 2 use 1 way, 7 - 2.                                                        */


/********************************************************************/

#include <stdlib.h>
#include "aac_dec_fp.h"
#include "aac_dec_decoding_fp.h"
#include "aac_dec_own_fp.h"
#include "aac_dec_huff_tables_fp.h"
/* SBR */
#include "aac_dec_sbr_fp.h"
#include "sbr_dec_tabs_fp.h"
/* PS */
#include "ps_dec_parser.h"

/********************************************************************/

AACStatus aacdecReset(AACDec *state)
{
  Ipp32s i, j;

  if (!state)
    return AAC_NULL_PTR;

  state->com.decodedBytes = 0;
  state->com.m_frame_number = 0;
  state->com.m_index_1st = 0;
  state->com.m_index_2nd = 1;
  state->com.m_index_3rd = 2;

  state->com.noiseState = 0;

  state->com.SbrFlagPresent = 0;

  /* set default params: (HQ, DWNSMPL_ON, SBR_ENABLE) */
  //------------------------------------------------------------
  if( HEAAC_PARAMS_UNDEF == state->com.ModeDecodeHEAACprofile){
    state->com.ModeDecodeHEAACprofile = HEAAC_HQ_MODE;
  }

  if ( HEAAC_PARAMS_UNDEF == state->com.ModeDwnsmplHEAACprofile ) {
    state->com.ModeDwnsmplHEAACprofile  = HEAAC_DWNSMPL_ON;
  }

  if (SBR_UNDEF == state->com.m_flag_SBR_support_lev) {
    state->com.m_flag_SBR_support_lev   = SBR_ENABLE;
  }
  //------------------------------------------------------------

  for (i = 0; i < CH_MAX + COUPL_CH_MAX; i++) {
    ippsZero_32f(state->m_ltp_buf[i][0], 2048);
    ippsZero_32f(state->m_ltp_buf[i][1], 2048);
    ippsZero_32f(state->m_ltp_buf[i][2], 2048);
    ippsZero_32f(state->m_prev_samples[i], 2048);
    ippsZero_32f(state->m_gcOverlapBuffer[i], 2048);

    ippsZero_32f(state->SSR_IPQFState[i].app_pqfbuf0, 24);
    ippsZero_32f(state->SSR_IPQFState[i].app_pqfbuf1, 24);
    ippsZero_32f(state->SSR_IPQFState[i].app_pqfbuf2, 24);
    ippsZero_32f(state->SSR_IPQFState[i].app_pqfbuf3, 24);

    state->com.m_prev_win_shape[i] = 0;

    for (j = 0; j < 4; j++) {
      state->com.prevSSRInfo[i][j].adjust_num = 0;
    }

    for (j = 0; j < 8; j++) {
      state->com.SSRInfo[i][0][j].adjust_num = 0;
      state->com.SSRInfo[i][1][j].adjust_num = 0;
      state->com.SSRInfo[i][2][j].adjust_num = 0;
      state->com.SSRInfo[i][3][j].adjust_num = 0;
    }
  }

  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecGetSize(Ipp32s *pStateSize)
{
  Ipp32s  i;
  /* HE-AAC */
  Ipp32s SizeWorkBuf = 0, mStateSize;
  Ipp32s pSize[12], mSize;
  Ipp32s pSbrSize[10], mSbrSize;
  Ipp32s pPsSize[10], mPsSize = 0;
  Ipp32s mOneFilterSize, mFilterSize;
  Ipp32s pOneFilterSize[7];
  Ipp32s mSBRDecSize, mPSDecSize = 0;

  /* aligned size of AACDec */
  mStateSize = __ALIGNED(sizeof(AACDec));

  /* size of huffman_tables */
  mSize = 0;
  for (i = 0; i < 12; i++) {
    if(ippsVLCDecodeGetSize_32s(vlcBooks[i],
                                vlcTableSizes[i],
                                vlcSubTablesSizes[i],
                                vlcNumSubTables[i],
                                &(pSize[i])) != ippStsNoErr) {
      return AAC_ALLOC;
    } else {
      pSize[i] = __ALIGNED(pSize[i]);
      mSize += pSize[i];
    }
  }

  /* size of sbrHuffTables */
  mSbrSize = 0;
  for (i = 0; i < 10; i++) {
    if (ippsVLCDecodeGetSize_32s(vlcSbrBooks[i],
                                 vlcSbrTableSizes[i],
                                 vlcSbrSubTablesSizes[i],
                                 vlcSbrNumSubTables[i],
                                 &(pSbrSize[i])) != ippStsNoErr) {
      return AAC_ALLOC;
    } else {
      pSbrSize[i] = __ALIGNED(pSbrSize[i]);
      mSbrSize += pSbrSize[i];
    }
  }

#if 1
  /* size of psHuffTabs */
  mPsSize = 0;
  for (i = 0; i < 10; i++) {
    if (ippsVLCDecodeGetSize_32s(vlcPsBooks[i],
                                 vlcPsTableSizes[i],
                                 vlcPsSubTablesSizes[i],
                                 vlcPsNumSubTables[i],
                                 &(pPsSize[i])) != ippStsNoErr) {
        return AAC_ALLOC;
      } else {
        pPsSize[i] = __ALIGNED(pPsSize[i]);
        mPsSize += pPsSize[i];
      }
  }
#endif

  if (sbrdecGetFilterSize(pOneFilterSize))
    return AAC_ALLOC;
  mOneFilterSize = pOneFilterSize[0] + pOneFilterSize[0] + pOneFilterSize[1];
  mOneFilterSize += pOneFilterSize[2] + pOneFilterSize[2] + pOneFilterSize[3];
  mOneFilterSize += pOneFilterSize[4] + pOneFilterSize[4] + pOneFilterSize[5];
  SizeWorkBuf = IPP_MAX(pOneFilterSize[6], SBR_MINSIZE_OF_WORK_BUFFER);
  mFilterSize = __ALIGNED(mOneFilterSize * CH_MAX + SizeWorkBuf);

  sbrDecoderGetSize(&mSBRDecSize);

  psDecoderGetSize(&mPSDecSize);

  pStateSize[0] = mStateSize + mSize + mSbrSize + mPsSize +
                  mFilterSize + __ALIGNED( mSBRDecSize ) + __ALIGNED( mPSDecSize );

  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecInit(AACDec *state)
{
  Ipp32s  i;
  /* HE-AAC */
  Ipp32s SizeWorkBuf = 0, mStateSize;
  Ipp32s pSize[12], mSize;
  Ipp32s pSbrSize[10], mSbrSize, mSBRDecSize;
  Ipp32s pPsSize[10], mPsSize = 0;
  Ipp8u *pMem;
  Ipp32s mOneFilterSize, mFilterSize;
  Ipp32s pOneFilterSize[7];
  sSbrDecFilter* sbr_filter[CH_MAX];
  sSBRBlock* sbrBlock[CH_MAX];

  if (!state)
    return AAC_NULL_PTR;

  for (i = 0; i < CH_MAX; i++) {
    sbr_filter[i] = &(state->sbr_filter[i]);
    sbrBlock[i] = &(state->sbrBlock[i]);
  }

  /* aligned size of AACDec */
  mStateSize = __ALIGNED(sizeof(AACDec));

  /* size of huffman_tables */
  mSize = 0;
  for (i = 0; i < 12; i++) {
    if(ippsVLCDecodeGetSize_32s(vlcBooks[i],
                                vlcTableSizes[i],
                                vlcSubTablesSizes[i],
                                vlcNumSubTables[i],
                                &(pSize[i])) != ippStsNoErr) {
      return AAC_ALLOC;
    } else {
      pSize[i] = __ALIGNED(pSize[i]);
      mSize += pSize[i];
    }
  }

  /* size of sbrHuffTables */
  mSbrSize = 0;
  for (i = 0; i < 10; i++) {
    if (ippsVLCDecodeGetSize_32s(vlcSbrBooks[i],
                                 vlcSbrTableSizes[i],
                                 vlcSbrSubTablesSizes[i],
                                 vlcSbrNumSubTables[i],
                                 &(pSbrSize[i])) != ippStsNoErr) {
      return AAC_ALLOC;
    } else {
      pSbrSize[i] = __ALIGNED(pSbrSize[i]);
      mSbrSize += pSbrSize[i];
    }
  }

#if 1
  /* size of psHuffTables */
  mPsSize = 0;
  for (i = 0; i < 10; i++) {
    if (ippsVLCDecodeGetSize_32s(vlcPsBooks[i],
      vlcPsTableSizes[i],
      vlcPsSubTablesSizes[i],
      vlcPsNumSubTables[i],
      &(pPsSize[i])) != ippStsNoErr) {
        return AAC_ALLOC;
      } else {
        pPsSize[i] = __ALIGNED(pPsSize[i]);
        mPsSize += pPsSize[i];
      }
  }
#endif

  if (sbrdecGetFilterSize(pOneFilterSize))
    return AAC_ALLOC;
  mOneFilterSize = pOneFilterSize[0] + pOneFilterSize[0] + pOneFilterSize[1];
  mOneFilterSize += pOneFilterSize[2] + pOneFilterSize[2] + pOneFilterSize[3];
  mOneFilterSize += pOneFilterSize[4] + pOneFilterSize[4] + pOneFilterSize[5];
  SizeWorkBuf = IPP_MAX(pOneFilterSize[6], SBR_MINSIZE_OF_WORK_BUFFER);
  mFilterSize = __ALIGNED(mOneFilterSize * CH_MAX + SizeWorkBuf);

  /* important: here HEAAC params must be UNDEF */

  state->com.ModeDecodeHEAACprofile = HEAAC_PARAMS_UNDEF;
  state->com.ModeDwnsmplHEAACprofile= HEAAC_PARAMS_UNDEF;
  state->com.m_flag_SBR_support_lev = SBR_UNDEF;
  state->com.m_flag_PS_support_lev  = PS_DISABLE;

  aacdecReset(state);

  state->com.FirstID3Search = 0;
  state->com.id3_size = 0;

  state->com.m_is_chmap_valid = 0;
  state->com.m_is_pce_valid = 0;
  state->com.m_sampling_frequency = 0;
  state->com.m_sampling_frequency_index = 0;
  state->com.m_channel_number = 1;
  state->com.m_channel_number_save = 1;
  state->com.m_channel_number_all = 0;
  state->com.m_element_number = 0;
  state->com.m_channel_config = 0;
  state->com.m_frame_size = 1024;
  state->com.adts_channel_configuration = -1;

  state->com.m_audio_object_type = AOT_UNDEF;

  for (i = 0; i < 16; i++) {
    state->com.huffman_tables[i] = NULL;
  }

  for (i = 0; i < 10; i++) {
    state->com.sbrHuffTables[i] = NULL;
  }

  for (i = 0; i < CH_MAX + COUPL_CH_MAX; i++) {
    state->pFDPState[i] = NULL;
  }
  state->pWorkBuffer = NULL;

  state->m_filterbank.p_mdct_inv_long = NULL;
  state->m_filterbank.p_mdct_inv_short = NULL;
  state->m_filterbank.p_buffer_inv = NULL;
  state->m_filterbank.p_mdct_fwd_long = NULL;
  state->m_filterbank.p_mdct_fwd_short = NULL;
  state->m_filterbank.p_buffer_fwd = NULL;

  pMem = (Ipp8u *)state + mStateSize;
  for (i = 0; i < 12; i++) {
    state->com.huffman_tables[i] = pMem;
    if (ippsVLCDecodeInit_32s(vlcBooks[i],
                              vlcTableSizes[i],
                              vlcSubTablesSizes[i],
                              vlcNumSubTables[i],
                              state->com.huffman_tables[i]) != ippStsNoErr) {
//      aacdecClose(state);
      return AAC_ALLOC;
    } else {
      pMem += pSize[i];
    }
  }

  for (i = 0; i < 10; i++) {
    state->com.sbrHuffTables[i] = (IppsVLCDecodeSpec_32s *)pMem;
    if(ippsVLCDecodeInit_32s(vlcSbrBooks[i],
                             vlcSbrTableSizes[i],
                             vlcSbrSubTablesSizes[i],
                             vlcSbrNumSubTables[i],
                             state->com.sbrHuffTables[i]) != ippStsNoErr) {
//      aacdecClose(state);
      return AAC_ALLOC;
    } else {
      pMem += pSbrSize[i];
    }
  }

#if 1
  for (i = 0; i < 10; i++) {
    state->psState.comState.psHuffTables[i] = (IppsVLCDecodeSpec_32s *)pMem;
    if(ippsVLCDecodeInit_32s(vlcPsBooks[i],
                             vlcPsTableSizes[i],
                             vlcPsSubTablesSizes[i],
                             vlcPsNumSubTables[i],
                             state->psState.comState.psHuffTables[i]) != ippStsNoErr) {
        //      aacdecClose(state);
        return AAC_ALLOC;
      } else {
        pMem += pPsSize[i];
      }
  }
#endif

  (state->com.m_sce).stream.p_huffman_tables = (void **)&(state->com.huffman_tables[0]);
  (state->com.m_cpe).streams[0].p_huffman_tables = (void **)&(state->com.huffman_tables[0]);
  (state->com.m_cpe).streams[1].p_huffman_tables = (void **)&(state->com.huffman_tables[0]);
  (state->com.m_cce).stream.p_huffman_tables = (void **)&(state->com.huffman_tables[0]);
  (state->com.m_lfe).stream.p_huffman_tables = (void **)&(state->com.huffman_tables[0]);

  (state->com.m_sce).stream.vlcShifts = vlcShifts;
  (state->com.m_cpe).streams[0].vlcShifts = vlcShifts;
  (state->com.m_cpe).streams[1].vlcShifts = vlcShifts;
  (state->com.m_cce).stream.vlcShifts = vlcShifts;
  (state->com.m_lfe).stream.vlcShifts = vlcShifts;

  (state->com.m_sce).stream.vlcOffsets = vlcOffsets;
  (state->com.m_cpe).streams[0].vlcOffsets = vlcOffsets;
  (state->com.m_cpe).streams[1].vlcOffsets = vlcOffsets;
  (state->com.m_cce).stream.vlcOffsets = vlcOffsets;
  (state->com.m_lfe).stream.vlcOffsets = vlcOffsets;

  (state->com.m_sce).stream.vlcTypes = vlcTypes;
  (state->com.m_cpe).streams[0].vlcTypes = vlcTypes;
  (state->com.m_cpe).streams[1].vlcTypes = vlcTypes;
  (state->com.m_cce).stream.vlcTypes = vlcTypes;
  (state->com.m_lfe).stream.vlcTypes = vlcTypes;

  sbrdecDrawMemMap(sbr_filter, pMem, pOneFilterSize);
  state->pWorkBuffer = pMem + mOneFilterSize * CH_MAX;

  if (sbrdecInitFilter(state))
    return AAC_ALLOC;

  pMem += mFilterSize;
  sbrInitDecoder(sbrBlock, pMem);

  for (i = 0; i < CH_MAX; i++) {
    ippsCopy_8u((const Ipp8u *)(state->com.sbrHuffTables),
      (Ipp8u *)(state->sbrBlock[i].comState.sbrHuffTables), 10 * sizeof(void *));
  }

  /* PS DEC */
  sbrDecoderGetSize( &mSBRDecSize );
  pMem += __ALIGNED(mSBRDecSize);

  psInitDecoder_32f( &(state->psState), pMem );

  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecUpdateObjMemMap(AACDec *state, Ipp32s mShift)
{
  Ipp32s  i;

  if (state->com.m_audio_object_type == AOT_AAC_ALS) {
    alsdecUpdateState(&(state->alsState), mShift);
  } else {

    if (state->com.m_audio_object_type == AOT_ER_BSAC) {
      AACDEC_UPDATE_PTR(BSACDec, state->bsState, mShift)
    }

    AACDEC_UPDATE_PTR(IppsMDCTInvSpec_32f, state->m_filterbank.p_mdct_inv_long, mShift)
    AACDEC_UPDATE_PTR(IppsMDCTInvSpec_32f, state->m_filterbank.p_mdct_inv_short, mShift)
    AACDEC_UPDATE_PTR(Ipp8u, state->m_filterbank.p_buffer_inv, mShift)

    AACDEC_UPDATE_PTR(IppsMDCTFwdSpec_32f, state->m_filterbank.p_mdct_fwd_long, mShift)
    AACDEC_UPDATE_PTR(IppsMDCTFwdSpec_32f, state->m_filterbank.p_mdct_fwd_short, mShift)

    if (state->com.m_audio_object_type == AOT_AAC_MAIN) {
      for (i = 0; i < CH_MAX + COUPL_CH_MAX; i++) {
        AACDEC_UPDATE_PTR(IppsFDPState_32f, state->pFDPState[i], mShift)
      }
    }
  }
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecUpdateStateMemMap(AACDec *state, Ipp32s mShift)
{
  Ipp32s  i;

  if (!state)
    return AAC_NULL_PTR;

  for (i = 0; i < 16; i++) {
    AACDEC_UPDATE_PTR(void, state->com.huffman_tables[i], mShift)
  }
  for (i = 0; i < 10; i++) {
    AACDEC_UPDATE_PTR(IppsVLCDecodeSpec_32s, state->com.sbrHuffTables[i], mShift)
  }
  for (i = 0; i < 10; i++) {
    AACDEC_UPDATE_PTR(IppsVLCDecodeSpec_32s, state->psState.comState.psHuffTables[i], mShift)
  }

  AACDEC_UPDATE_PTR(Ipp8u, state->pWorkBuffer, mShift)

  (state->com.m_sce).stream.p_huffman_tables = (void **)&(state->com.huffman_tables[0]);
  (state->com.m_cpe).streams[0].p_huffman_tables = (void **)&(state->com.huffman_tables[0]);
  (state->com.m_cpe).streams[1].p_huffman_tables = (void **)&(state->com.huffman_tables[0]);
  (state->com.m_cce).stream.p_huffman_tables = (void **)&(state->com.huffman_tables[0]);
  (state->com.m_lfe).stream.p_huffman_tables = (void **)&(state->com.huffman_tables[0]);

  sbrdecUpdateMemMap(state, mShift);
  psdecUpdateMemMap (&(state->psState), mShift);

  return AAC_OK;
}

/********************************************************************/

#define RETURN_AAC_BAD_STREAM                                     \
{                                                                 \
  GET_BITS_COUNT(pBS, (*decodedBytes))                            \
  *decodedBytes = (*decodedBytes + 7) >> 3;                       \
  state_com->m_channel_number = state_com->m_channel_number_save; \
  state_com->m_is_chmap_valid = save_m_is_chmap_valid;            \
  state_com->m_is_pce_valid = save_m_is_pce_valid;                \
  return AAC_BAD_STREAM;                                          \
}

/********************************************************************/

static Ipp32s aacGetChannel(Ipp32s id,
                            Ipp32s tag,
                            Ipp32s element_number,
                            sEl_map_item elmap[])
{
  Ipp32s i;
  for (i = 0; i < element_number; i++) {
    if ((elmap[i].id == id) && (elmap[i].tag == tag)) {
      return elmap[i].ch;
    }
  }
  return -1;
}

/********************************************************************/

AACStatus aacdecGetFrame(Ipp8u  *inPointer,
                         Ipp32s *decodedBytes,
                         Ipp16s *outPointer,
                         Ipp32s inDataSize,
                         Ipp32s outBufferSize,
                         AACDec *state)
{
  sData_stream_element m_data;
  sDynamic_range_info m_drc_info;
  sEl_map_item m_elmap[EL_TAG_MAX];

  sBitsreamBuffer BS;
  sBitsreamBuffer *pBS = &BS;
  AACDec_com      *state_com;

  Ipp32s  el_num;
  Ipp32s  ch_num = 0;
  Ipp32s  order_counter = 0;
  Ipp32s  cur_frame_el_num = 0;
  Ipp32s  save_m_is_chmap_valid;
  Ipp32s  save_m_is_pce_valid;

  Ipp32s  id;
  Ipp32s  tag = 0;
  Ipp32s  ch = 0;
  Ipp32s  ncch = 0;
  Ipp32s  tag_counter[4][EL_TAG_MAX];

/* HE-AAC param */
  Ipp32s  cnt_fill_sbr_element = 0;
  Ipp32s  cnt_idaac_sbr_element = 0;
  Ipp32s  sbrFlagPresentLFE = 0;
  Ipp32s  NumRealCh = 0;
  Ipp32s  ch_counter;
  Ipp32s  errSBRParser = 0;
  Ipp32s  criterionPS  = 0;

  s_tns_data tns_data0;
  Ipp32s decodedBits = 0;
  Ipp32s  i, j;

  if (!inPointer || !outPointer)
    return AAC_NULL_PTR;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < EL_TAG_MAX; j++) {
      tag_counter[i][j] = 0;
    }
  }

  state_com = &(state->com);
  el_num = state_com->m_element_number;
  ch_num = state_com->m_channel_number_all;

  save_m_is_chmap_valid = state_com->m_is_chmap_valid;
  save_m_is_pce_valid = state_com->m_is_pce_valid;

  /* state_com->decodedBytes != 0 means that this frame   */
  /* has been decoded already but output buffer was small */
  if (state_com->decodedBytes == 0) {

    // init
    state->com.m_up_sample  = 1;

    if (AOT_ER_BSAC == state_com->m_audio_object_type) {
      AACStatus status;

      status = bsacdecGetFrame(inPointer, inDataSize, state->com.layer, state);

      if (status != AAC_OK) {
        return status;
      }

      for (i = 0; i < state_com->m_channel_number; i++) {
        state_com->m_order[i] = i;
      }
    } else {

      GET_INIT_BITSTREAM(pBS, inPointer)

      GET_BITS(pBS, id, 3, Ipp32s)
      while (id != ID_END) {
        if (CH_MAX - 1 < NumRealCh)
          RETURN_AAC_BAD_STREAM;
        switch (id) {
        case ID_SCE:
          /* may be there is HE-AAC element */
          state->sbrBlock[cnt_idaac_sbr_element++].comState.id_aac = ID_SCE;
          NumRealCh++;

          /* Parsing bitstream */
          (state_com->m_sce).crc = state_com->crc;
          (state_com->m_sce).crc_enable = state_com->crc_enable;
          if (dec_sce_channel_element(&(state_com->m_sce), pBS, state_com->m_audio_object_type) < 0)
            RETURN_AAC_BAD_STREAM;
          state_com->crc = (state_com->m_sce).crc;
          tag = (state_com->m_sce).element_instance_tag;
          /* several equal tags are supported */
          tag_counter[0][tag] += 1;
          tag += ((tag_counter[0][tag] - 1) * EL_TAG_MAX);
          break;
        case ID_CPE:
          /* may be there is HE-AAC element */
        state->sbrBlock[cnt_idaac_sbr_element++].comState.id_aac = ID_CPE;
          NumRealCh += 2;

          /* Parsing bitstream */
          (state_com->m_cpe).crc = state_com->crc;
          (state_com->m_cpe).crc_enable = state_com->crc_enable;
          if (dec_cpe_channel_element(&(state_com->m_cpe), pBS, state_com->m_audio_object_type) < 0)
            RETURN_AAC_BAD_STREAM;
          state_com->crc = (state_com->m_cpe).crc;
          tag = (state_com->m_cpe).element_instance_tag;
          /* several equal tags are supported */
          tag_counter[1][tag] += 1;
          tag += ((tag_counter[1][tag] - 1) * EL_TAG_MAX);
          break;
        case ID_CCE:
          if (COUPL_CH_MAX - 1 < ncch)
            RETURN_AAC_BAD_STREAM;

          (state_com->m_cce).crc = state_com->crc;
          (state_com->m_cce).crc_enable = state_com->crc_enable;
          if (dec_coupling_channel_element(&(state_com->m_cce), &(state_com->m_cdata[ncch]),
                pBS, state_com->m_audio_object_type) < 0)
            RETURN_AAC_BAD_STREAM;
          state_com->crc = (state_com->m_cce).crc;
          tag = (state_com->m_cce).element_instance_tag;
          /* several equal tags are supported */
          tag_counter[2][tag] += 1;
          tag += ((tag_counter[2][tag] - 1) * EL_TAG_MAX);
          break;
        case ID_LFE:
          /* may be there is HE-AAC element */
          if (CH_MAX - 1 < cnt_idaac_sbr_element)
            RETURN_AAC_BAD_STREAM;

          state->sbrBlock[cnt_idaac_sbr_element++].comState.id_aac = ID_LFE;

          if (cnt_fill_sbr_element > 0) {
            sbrFlagPresentLFE++;

            if ( CH_MAX - 1 < cnt_fill_sbr_element )
              RETURN_AAC_BAD_STREAM;

            cnt_fill_sbr_element++;
          }

          /* Parsing bitstream */
          (state_com->m_lfe).crc = state_com->crc;
          (state_com->m_lfe).crc_enable = state_com->crc_enable;
          if (dec_lfe_channel_element(&(state_com->m_lfe), pBS, state_com->m_audio_object_type) < 0)
            RETURN_AAC_BAD_STREAM;
          state_com->crc = (state_com->m_lfe).crc;
          tag = (state_com->m_lfe).element_instance_tag;
          /* several equal tags are supported */
          tag_counter[3][tag] += 1;
          tag += ((tag_counter[3][tag] - 1) * EL_TAG_MAX);
          NumRealCh++;
          break;
        case ID_DSE:
          dec_data_stream_element(&m_data, pBS);
          break;
        case ID_PCE:
          {
            Ipp32u  *crc_ptr = 0;
            Ipp32s  crc_offset = 0;
            Ipp32s  decodedBits0 = 0;
            Ipp32s  decodedBits2 = 0;

            if (state_com->crc_enable) {
              crc_ptr = pBS->pCurrent_dword;
              crc_offset = pBS->nBit_offset;
              GET_BITS_COUNT(pBS, decodedBits0)
            }

            dec_program_config_element(&(state_com->m_pce), pBS);

            if (state_com->crc_enable) {
              Ipp32u crc = (Ipp32u)state_com->crc;
              GET_BITS_COUNT(pBS, decodedBits2)
              bs_CRC_update(crc_ptr, crc_offset, decodedBits2 - decodedBits0, &crc);
              state_com->crc = (Ipp32s)crc;
            }
            state_com->m_is_chmap_valid = 0;
            state_com->m_is_pce_valid = 1;
          }
          break;
        case ID_FIL:
  /*
  * NOTES:
  * (1)SBR FILL ELEMENT there is for SCE & CPE modes only.
  * (2)If AOT AAC is MAIN, LC or LTP
  * then
  *   <SCE> <FIL <EXT_SBR_DATA(SCE)>> // center
  *   <CPE> <FIL <EXT_SBR_DATA(CPE)>> // front L/R
  *   <CPE> <FIL <EXT_SBR_DATA(CPE)>> // back L/R
  *   <LFE> // sub
  *   <END> // (end of raw data block)
  *
  * (3)If AOT AAC is ER LC or ER LTP
  * then
  *   <SCE> <CPE> <CPE> <LFE> <EXT <SBR(SCE)> <SBR(CPE)> <SBR(CPE)>>
  *
  * (4) may be more then one FILL ELEMENT, for ex:
  *  <SCE> <FILL.. <CPE> <FILL.. <CPE> <FILL..><FILL..> <LFE><FILL..><FILL..><FILL..>
  */
          if ( CH_MAX - 1 < cnt_fill_sbr_element )
            RETURN_AAC_BAD_STREAM;

        state->sbrBlock[cnt_fill_sbr_element].comState.sbr_freq_sample =
            state_com->m_sampling_frequency * 2;

          if (state->com.m_sampling_frequency_index >= 3)
          state->sbrBlock[cnt_fill_sbr_element].comState.sbr_freq_indx =
              state->com.m_sampling_frequency_index - 3;

          /* PARAMETRIC STEREO PATCH */
          criterionPS = 0;
          if( 0x1 & state_com->m_flag_PS_support_lev){

            if( 0 == cnt_fill_sbr_element &&
                state->sbrBlock[cnt_fill_sbr_element].comState.id_aac == ID_SCE ){
              criterionPS = 1;
            }
          }

          if( criterionPS){
            state->sbrBlock[cnt_fill_sbr_element].comState.psState = &(state->psState.comState);
          } else {
            state->sbrBlock[cnt_fill_sbr_element].comState.psState = NULL;
          }
        errSBRParser = dec_fill_element(
                          &(state->sbrBlock[cnt_fill_sbr_element].comState),
                          &cnt_fill_sbr_element,
                          &m_drc_info,
                          pBS,
                          //SBR_ENABLE
                          state->com.m_flag_SBR_support_lev
                          );

          /*************************************************************
          * if there is problem with HE-AAC parser then QUIT
          *************************************************************/
          if ( errSBRParser )
            RETURN_AAC_BAD_STREAM

          break;
        }

        /* robustness */
        GET_BITS_COUNT(pBS, decodedBits)
        if (decodedBits > inDataSize * 8) {
          RETURN_AAC_BAD_STREAM;
        }

        if ((ID_DSE > id) && (ID_CCE != id)) {
          ch = aacGetChannel(id, tag, el_num, state_com->m_elmap);

          if (ch < 0) {
            if (el_num >= CH_MAX)
              RETURN_AAC_BAD_STREAM
            state_com->m_elmap[el_num].id = (Ipp16s)id;
            state_com->m_elmap[el_num].tag = (Ipp16s)tag;
            state_com->m_elmap[el_num].ch = (Ipp16s)ch_num;
            ch = ch_num;
            el_num++;
            ch_num++;
            if (ID_CPE == id)
                ch_num++;
          }

          m_elmap[cur_frame_el_num].id = (Ipp16s)id;
          m_elmap[cur_frame_el_num].tag = (Ipp16s)tag;
          m_elmap[cur_frame_el_num].ch = (Ipp16s)ch;

          cur_frame_el_num++;

          state_com->m_order[order_counter] = ch;
          order_counter++;

          if (ch >= CH_MAX)
            RETURN_AAC_BAD_STREAM

          if (ID_CPE == id) {
            state_com->m_order[order_counter] = ch+1;
            order_counter++;
            if (ch+1 >= CH_MAX)
              RETURN_AAC_BAD_STREAM
          }
        }

        switch (id) {
        case ID_SCE:

          state_com->m_curr_win_shape[ch] = (state_com->m_sce).stream.window_shape;
          state_com->m_curr_win_sequence[ch] = (state_com->m_sce).stream.window_sequence;

          if (0 != (state_com->m_sce).stream.pulse_data_present) {
            ics_apply_pulse_I(&(state_com->m_sce).stream);
          }
          ics_apply_scale_factors(&(state_com->m_sce).stream, state->m_spectrum_data[ch]);
          if (!apply_pns_ref(&(state_com->m_sce).stream, NULL, state->m_spectrum_data[ch], NULL, 1,
                    0, NULL, &(state_com->noiseState))) {
             deinterlieve(&(state_com->m_sce).stream, state->m_spectrum_data[ch]);
          }

          ics_calc_tns_data(&(state_com->m_sce).stream, &(state->tns_data[ch]));

          if (AOT_AAC_MAIN == state_com->m_audio_object_type) {

            FDP(state->m_spectrum_data[ch], &(state_com->m_sce).stream,
                state->pFDPState[ch]);

          } else if (AOT_AAC_LTP == state_com->m_audio_object_type) {
            state->m_ltp.p_samples_1st_part = state->m_ltp_buf[ch][state_com->m_index_1st];
            state->m_ltp.p_samples_2nd_part = state->m_ltp_buf[ch][state_com->m_index_2nd];
            state->m_ltp.p_samples_3rd_part = state->m_ltp_buf[ch][state_com->m_index_3rd];
            state->m_ltp.prev_windows_shape = state_com->m_prev_win_shape[ch];

            state->m_ltp.p_filterbank_data = &(state->m_filterbank);
            state->m_ltp.p_tns_data = &(state->tns_data[ch]);

            ics_apply_ltp_I(&(state->m_ltp), &(state_com->m_sce).stream, state->m_spectrum_data[ch]);
          } else if (AOT_AAC_SSR == state_com->m_audio_object_type) {
            save_gain_control_data(ch, &(state_com->m_sce).stream, state_com);
          }

          //if (0 != state->tns_data[ch].m_tns_data_present) {
          //  ics_apply_tns_dec_I(&(state->tns_data[ch]), state->m_spectrum_data[ch]);
          //}
          break;
        case ID_CPE:

          state_com->m_curr_win_shape[ch] = (state_com->m_cpe).streams[0].window_shape;
          state_com->m_curr_win_sequence[ch] = (state_com->m_cpe).streams[0].window_sequence;
          state_com->m_curr_win_shape[ch + 1] = (state_com->m_cpe).streams[1].window_shape;
          state_com->m_curr_win_sequence[ch + 1] = (state_com->m_cpe).streams[1].window_sequence;

          if (0 != (state_com->m_cpe).streams[0].pulse_data_present) {
            ics_apply_pulse_I(&(state_com->m_cpe).streams[0]);
          }
          if (0 != (state_com->m_cpe).streams[1].pulse_data_present) {
            ics_apply_pulse_I(&(state_com->m_cpe).streams[1]);
          }

          ics_apply_scale_factors(&(state_com->m_cpe).streams[0], state->m_spectrum_data[ch]);
          ics_apply_scale_factors(&(state_com->m_cpe).streams[1],
                                  state->m_spectrum_data[ch + 1]);

          /* Joint stereo */
          cpe_apply_ms(&(state_com->m_cpe), state->m_spectrum_data[ch],
                        state->m_spectrum_data[ch + 1]);

          apply_pns(&(state_com->m_cpe).streams[0], &(state_com->m_cpe).streams[1],
                    state->m_spectrum_data[ch], state->m_spectrum_data[ch + 1], 2,
                    (state_com->m_cpe).ms_mask_present, (state_com->m_cpe).ms_used,
                    &(state_com->noiseState));

  #ifndef NEW_FDP_VERSION
          cpe_apply_intensity(&(state_com->m_cpe), state->m_spectrum_data[ch],
                              state->m_spectrum_data[ch + 1]);
  #endif

          if (AOT_AAC_MAIN == state_com->m_audio_object_type) {

            FDP(state->m_spectrum_data[ch], &(state_com->m_cpe).streams[0],
                state->pFDPState[ch]);

            FDP(state->m_spectrum_data[ch+1], &(state_com->m_cpe).streams[1],
                state->pFDPState[ch+1]);
          }

  #ifdef  NEW_FDP_VERSION
            cpe_apply_intensity(&(state_com->m_cpe), state->m_spectrum_data[ch],
                                state->m_spectrum_data[ch + 1]);
  #endif

          ics_calc_tns_data(&(state_com->m_cpe).streams[0], &(state->tns_data[ch]));
          ics_calc_tns_data(&(state_com->m_cpe).streams[1], &(state->tns_data[ch + 1]));

          deinterlieve(&(state_com->m_cpe).streams[0], state->m_spectrum_data[ch]);
          deinterlieve(&(state_com->m_cpe).streams[1], state->m_spectrum_data[ch + 1]);

          if (AOT_AAC_LTP == state_com->m_audio_object_type) {
            state->m_ltp.p_samples_1st_part = state->m_ltp_buf[ch][state_com->m_index_1st];
            state->m_ltp.p_samples_2nd_part = state->m_ltp_buf[ch][state_com->m_index_2nd];
            state->m_ltp.p_samples_3rd_part = state->m_ltp_buf[ch][state_com->m_index_3rd];
            state->m_ltp.prev_windows_shape = state_com->m_prev_win_shape[ch];

            state->m_ltp.p_filterbank_data = &(state->m_filterbank);
            state->m_ltp.p_tns_data = &(state->tns_data[ch]);

            ics_apply_ltp_I(&(state->m_ltp), &(state_com->m_cpe).streams[0],
                            state->m_spectrum_data[ch]);

            state->m_ltp.p_samples_1st_part = state->m_ltp_buf[ch + 1][state_com->m_index_1st];
            state->m_ltp.p_samples_2nd_part = state->m_ltp_buf[ch + 1][state_com->m_index_2nd];
            state->m_ltp.p_samples_3rd_part = state->m_ltp_buf[ch + 1][state_com->m_index_3rd];
            state->m_ltp.prev_windows_shape = state_com->m_prev_win_shape[ch + 1];

            state->m_ltp.p_tns_data = &(state->tns_data[ch + 1]);

            ics_apply_ltp_I(&(state->m_ltp), &(state_com->m_cpe).streams[1],
                            state->m_spectrum_data[ch + 1]);
          } else if (AOT_AAC_SSR == state_com->m_audio_object_type) {
            save_gain_control_data(ch, &(state_com->m_cpe).streams[0], state_com);
            save_gain_control_data(ch+1, &(state_com->m_cpe).streams[1], state_com);
          }

          //if (0 != state->tns_data[ch].m_tns_data_present) {
          //  ics_apply_tns_dec_I(&(state->tns_data[ch]), state->m_spectrum_data[ch]);
          //}

          //if (0 != state->tns_data[ch + 1].m_tns_data_present) {
          //  ics_apply_tns_dec_I(&(state->tns_data[ch + 1]), state->m_spectrum_data[ch + 1]);
          //}
          break;
        case ID_CCE:
          state_com->m_curr_win_shape[CH_MAX + ncch] =
            (state_com->m_cce).stream.window_shape;
          state_com->m_curr_win_sequence[CH_MAX + ncch] =
            (state_com->m_cce).stream.window_sequence;

          if (0 != (state_com->m_cce).stream.pulse_data_present) {
            ics_apply_pulse_I(&(state_com->m_cce).stream);
          }
          ics_apply_scale_factors(&(state_com->m_cce).stream,
                                  state->m_spectrum_data[CH_MAX + ncch]);
          apply_pns(&(state_com->m_cce).stream, NULL,
                    state->m_spectrum_data[CH_MAX + ncch], NULL, 1,
                    0, NULL, &(state_com->noiseState));

          deinterlieve(&(state_com->m_cce).stream, state->m_spectrum_data[CH_MAX + ncch]);

          ics_calc_tns_data(&(state_com->m_cce).stream, &tns_data0);

          if (AOT_AAC_MAIN == state_com->m_audio_object_type) {

            FDP(state->m_spectrum_data[CH_MAX + ncch], &(state_com->m_cce).stream,
                state->pFDPState[CH_MAX + ncch]);

          } else if (AOT_AAC_LTP == state_com->m_audio_object_type) {
            state->m_ltp.p_samples_1st_part =
              state->m_ltp_buf[CH_MAX + ncch][state_com->m_index_1st];
            state->m_ltp.p_samples_2nd_part =
              state->m_ltp_buf[CH_MAX + ncch][state_com->m_index_2nd];
            state->m_ltp.p_samples_3rd_part =
              state->m_ltp_buf[CH_MAX + ncch][state_com->m_index_3rd];
            state->m_ltp.prev_windows_shape =
              state_com->m_prev_win_shape[CH_MAX + ncch];

            state->m_ltp.p_filterbank_data = &(state->m_filterbank);
            state->m_ltp.p_tns_data = &tns_data0;

            ics_apply_ltp_I(&(state->m_ltp), &(state_com->m_cce).stream,
                            state->m_spectrum_data[CH_MAX + ncch]);
          } else if (AOT_AAC_SSR == state_com->m_audio_object_type) {
            save_gain_control_data(CH_MAX + ncch, &(state_com->m_cce).stream,
                                  state_com);
          }

          if (0 != tns_data0.m_tns_data_present) {
            ics_apply_tns_dec_I(&tns_data0, state->m_spectrum_data[CH_MAX + ncch]);
          }

          coupling_gain_calculation(&(state_com->m_cce), &(state_com->m_cdata[ncch]),
                                    state->cc_gain[ncch]);

          ncch++;
          break;
        case ID_LFE:
          state_com->m_curr_win_shape[ch] = (state_com->m_lfe).stream.window_shape;
          state_com->m_curr_win_sequence[ch] = (state_com->m_lfe).stream.window_sequence;

          if (0 != (state_com->m_lfe).stream.pulse_data_present) {
            ics_apply_pulse_I(&(state_com->m_lfe).stream);
          }
          ics_apply_scale_factors(&(state_com->m_lfe).stream, state->m_spectrum_data[ch]);
          state->tns_data[ch].m_tns_data_present = 0;
          break;
        default:
          break;
        }
        GET_BITS(pBS, id, 3, Ipp32s)
      }

      if (1 == state_com->m_is_pce_valid) {
        if (state_com->m_is_chmap_valid == 0) {
          if (chmap_create_by_pce(&(state_com->m_pce), state_com->m_chmap) < 0)
            RETURN_AAC_BAD_STREAM;
          state_com->m_is_chmap_valid = 1;
        }

        order_counter = chmap_order(state_com->m_chmap, m_elmap,
                                    cur_frame_el_num, state_com->m_order);
      } else if (state_com->adts_channel_configuration > 0) {
        if (chmap_create_by_adts(state_com->adts_channel_configuration,
            state_com->m_chmap, m_elmap, cur_frame_el_num) < 0)
            RETURN_AAC_BAD_STREAM;

        order_counter = chmap_order(state_com->m_chmap, m_elmap,
                                    cur_frame_el_num, state_com->m_order);
      }

      state_com->m_channel_number = order_counter;
      state_com->m_channel_number_all = ch_num;
      state_com->m_element_number = el_num;
    }

    for (i = 0; i < state_com->m_channel_number; i++) {
      ch = state_com->m_order[i];
      state->m_ordered_samples[i] = state->m_curr_samples[ch];
      // AYA: important! PS tool will correct it later
    }

    if ((AOT_AAC_LTP == state_com->m_audio_object_type)
#ifdef USE_LTP_IN_BSAC
      || (AOT_ER_BSAC == state_com->m_audio_object_type)
#endif
        ) {
      state_com->m_index_1st++;
      if (state_com->m_index_1st == 3)
        state_com->m_index_1st = 0;
      state_com->m_index_2nd++;
      if (state_com->m_index_2nd == 3)
        state_com->m_index_2nd = 0;
      state_com->m_index_3rd++;
      if (state_com->m_index_3rd == 3)
        state_com->m_index_3rd = 0;
    }

    /* coupling channel process */
    for (ch = 0; ch < ncch; ch++) {
      Ipp32s c;
      for (c = 0; c < state_com->m_cdata[ch].num_coupled_elements + 1; c++) {
        Ipp32s id = state_com->m_cdata[ch].cc_target_id[c];
        Ipp32s tag = state_com->m_cdata[ch].cc_target_tag[c];
        state_com->m_cdata[ch].cc_target_ch[c] =
          aacGetChannel(id, tag, state_com->m_element_number, state_com->m_elmap);
      }
    }

    for (ch = 0; ch < ncch; ch++) {
      if ((state_com->m_cdata[ch].ind_sw_cce_flag) ||
          (AOT_AAC_LTP == state_com->m_audio_object_type)) {
        if (AOT_AAC_SSR == state_com->m_audio_object_type) {
          SSR_GAIN *tmpSSRInfo[4];

          for (i = 0; i < 4; i++) {
            tmpSSRInfo[i] = state_com->SSRInfo[CH_MAX + ch][i];
          }

          ssr_gain_control(state->m_spectrum_data[CH_MAX + ch],
                          state_com->m_curr_win_shape[CH_MAX + ch],
                          state_com->m_prev_win_shape[CH_MAX + ch],
                          state_com->m_curr_win_sequence[CH_MAX + ch],
                          tmpSSRInfo,
                          state_com->prevSSRInfo[CH_MAX + ch],
                          state->m_curr_samples[CH_MAX + ch],
                          state->m_gcOverlapBuffer[CH_MAX + ch],
                          &(state->SSR_IPQFState[CH_MAX + ch]),
                          &(state->m_filterbank),
                          1024);

          state_com->m_prev_win_shape[CH_MAX + ch] =
            state_com->m_curr_win_shape[CH_MAX + ch];
        } else {
          FilterbankDec(&(state->m_filterbank), state->m_spectrum_data[CH_MAX + ch],
                        state->m_prev_samples[CH_MAX + ch],
                        state_com->m_curr_win_sequence[CH_MAX + ch],
                        state_com->m_curr_win_shape[CH_MAX + ch],
                        state_com->m_prev_win_shape[CH_MAX + ch],
                        state->m_curr_samples[CH_MAX + ch],
                        state->m_prev_samples[CH_MAX + ch]);

          state_com->m_prev_win_shape[CH_MAX + ch] =
            state_com->m_curr_win_shape[CH_MAX + ch];

          /* Update buffers for LTP */
          if (AOT_AAC_LTP == state_com->m_audio_object_type) {
            for (i = 0; i < 1024; i++) {
              Ipp32f  tmp;

              tmp = state->m_curr_samples[CH_MAX + ch][i];

              if (tmp > 0) tmp += 0.5f;
              else         tmp -= 0.5f;

              if (tmp > 32767)  tmp = 32767;
              if (tmp < -32768) tmp = -32768;

              state->m_ltp_buf[CH_MAX + ch][state_com->m_index_2nd][i] = (Ipp32f)(Ipp32s)tmp;

              tmp = state->m_prev_samples[CH_MAX + ch][i];

              if (tmp > 0) tmp += 0.5f;
              else tmp -= 0.5f;

              if (tmp > 32767)  tmp = 32767;
              if (tmp < -32768) tmp = -32768;

              state->m_ltp_buf[CH_MAX + ch][state_com->m_index_3rd][i] = (Ipp32f)(Ipp32s)tmp;
            }
          }
        }
      }
    }

    for (ch = 0; ch < ncch; ch++) {
      if ((!state_com->m_cdata[ch].ind_sw_cce_flag) &&
          (!state_com->m_cdata[ch].cc_domain)) {
      coupling_spectrum(state, &(state_com->m_cdata[ch]),
                        state->m_spectrum_data[CH_MAX + ch],
                        state->cc_gain[ch]);
      }
    }

    for (i = 0; i < state_com->m_channel_number; i++) {
      ch = state_com->m_order[i];
      if (0 != state->tns_data[ch].m_tns_data_present) {
        ics_apply_tns_dec_I(&(state->tns_data[ch]), state->m_spectrum_data[ch]);
      }
    }

    for (ch = 0; ch < ncch; ch++) {
      if ((!state_com->m_cdata[ch].ind_sw_cce_flag) &&
          (state_com->m_cdata[ch].cc_domain)) {
      coupling_spectrum(state, &(state_com->m_cdata[ch]),
                        state->m_spectrum_data[CH_MAX + ch],
                        state->cc_gain[ch]);
      }
    }

    if (AOT_AAC_SSR == state_com->m_audio_object_type) {
      SSR_GAIN *tmpSSRInfo[4];
      for (ch_counter = 0; ch_counter < state_com->m_channel_number; ch_counter++) {
        ch = state_com->m_order[ch_counter];

        for (i = 0; i < 4; i++) {
          tmpSSRInfo[i] = state_com->SSRInfo[ch][i];
        }

        ssr_gain_control(state->m_spectrum_data[ch],
                         state_com->m_curr_win_shape[ch],
                         state_com->m_prev_win_shape[ch],
                         state_com->m_curr_win_sequence[ch],
                         tmpSSRInfo,
                         state_com->prevSSRInfo[ch],
                         state->m_curr_samples[ch],
                         state->m_gcOverlapBuffer[ch],
                         &(state->SSR_IPQFState[ch]),
                         &(state->m_filterbank),
                         1024);

        state_com->m_prev_win_shape[ch] = state_com->m_curr_win_shape[ch];
      }
    } else {

      for (ch_counter = 0; ch_counter < state_com->m_channel_number; ch_counter++) {
        ch = state_com->m_order[ch_counter];
        FilterbankDec(&(state->m_filterbank), state->m_spectrum_data[ch],
                      state->m_prev_samples[ch], state_com->m_curr_win_sequence[ch],
                      state_com->m_curr_win_shape[ch], state_com->m_prev_win_shape[ch],
                      state->m_curr_samples[ch], state->m_prev_samples[ch]);

        state_com->m_prev_win_shape[ch] = state_com->m_curr_win_shape[ch];
      }
    }

    for (ch = 0; ch < ncch; ch++) {
      if (state_com->m_cdata[ch].ind_sw_cce_flag) {
      coupling_spectrum(state, &(state_com->m_cdata[ch]),
                        state->m_spectrum_data[CH_MAX + ch],
                        state->cc_gain[ch]);
      }
    }

    /* Update buffers for LTP */
    if ((AOT_AAC_LTP == state_com->m_audio_object_type)
#ifdef USE_LTP_IN_BSAC
     || (AOT_ER_BSAC == state_com->m_audio_object_type)
#endif
        ) {
      for (ch_counter = 0; ch_counter < state_com->m_channel_number; ch_counter++) {
        ch = state_com->m_order[ch_counter];
        for (i = 0; i < 1024; i++) {
          Ipp32f  tmp;

          tmp = state->m_curr_samples[ch][i];

          if (tmp > 0) tmp += 0.5f;
          else         tmp -= 0.5f;

          if (tmp > 32767)  tmp = 32767;
          if (tmp < -32768) tmp = -32768;

          state->m_ltp_buf[ch][state_com->m_index_2nd][i] = (Ipp32f)(Ipp32s)tmp;

          tmp = state->m_prev_samples[ch][i];

          if (tmp > 0) tmp += 0.5f;
          else tmp -= 0.5f;

          if (tmp > 32767)  tmp = 32767;
          if (tmp < -32768) tmp = -32768;

          state->m_ltp_buf[ch][state_com->m_index_3rd][i] = (Ipp32f)(Ipp32s)tmp;
        }
      }
    }

    /* HE-AAC patch */
    if ((sbrFlagPresentLFE) && (cnt_fill_sbr_element)) {

      /* we passed LFE if ER AAC mode is used */
      if (cnt_fill_sbr_element != cnt_idaac_sbr_element) {
        cnt_fill_sbr_element++;
      }

      /* may be error? if yes then we isn't doing sbr process */
      if (cnt_fill_sbr_element != cnt_idaac_sbr_element) {
        cnt_fill_sbr_element = 0;
      }
    }

    /* ******  PARAMETRIC STEREO PATCH ******************/
    /* PS is applied for                                 */
    /* (1) mono signal only [i = 0].                     */
    /* (2) MPEG4 container                               */
    /* (3) SBR explicit                                  */
    /* so, if PS active, decoder will convert mono2stereo*/
    /* ************************************************* */
    if( 0xA & state->com.m_flag_PS_support_lev){
      if( state->com.m_channel_number == 1 ){ // m_channel_number must be mono every call this function
        state->sbrBlock[0].pPSDecState = &(state->psState);
        state->com.m_channel_number = 2;
        state->m_ordered_samples[1] = state->m_curr_samples[1];
      } else {
//        vm_debug_trace(VM_DEBUG_ALL, VM_STRING("Err PS: ch > 1 !\n"));
        state->com.m_flag_PS_support_lev = PS_DISABLE;
      }
    }

    for (i = 0, j = 0; i < cnt_fill_sbr_element; i++) {
      Ipp32s nCall = (state->sbrBlock[i].comState.id_aac == ID_CPE) ? 2 : 1;
      Ipp32s call = 0;

      for( call = 0; call < nCall; call++ ){
        sbrGetFrame(state->m_curr_samples[j], //input
                    state->m_curr_samples[j], //output
                    (0xA & state->com.m_flag_PS_support_lev) ? state->m_curr_samples[j + 1] : NULL,
                    &(state->sbrBlock[i]),
                    &(state->sbr_filter[i]),
                    call, // number of "current" channel
                    state_com->ModeDecodeHEAACprofile, //HQ or LP
                    state_com->ModeDwnsmplHEAACprofile,//DWNSMPL ON or OFF
                    state_com->m_flag_PS_support_lev, // HEAACv1 or v2
                    state->pWorkBuffer );
        j++;
      }
    }// END FOR for (i = 0, j = 0; i < cnt_fill_sbr_element; i++)

    if (cnt_fill_sbr_element) {
      if (state_com->ModeDwnsmplHEAACprofile == HEAAC_DWNSMPL_OFF) {
        state->com.m_up_sample  = 2;
      }
      state_com->SbrFlagPresent = 1;
    }

    if (AOT_ER_BSAC != state_com->m_audio_object_type) {
      Byte_alignment(pBS);
      GET_BITS_COUNT(pBS, (*decodedBytes))
      *decodedBytes >>= 3;
    } else {
      *decodedBytes = (state->bsState)->frameLength;
    }

    state_com->m_channel_number_save = state_com->m_channel_number;
  } else {
    *decodedBytes = state_com->decodedBytes;
    state_com->crc = state_com->saved_crc;
  }

  if (outBufferSize < (state_com->m_channel_number *
    state->com.m_up_sample * 1024 * (Ipp32s)(sizeof(Ipp16s)))) {
    state_com->decodedBytes = *decodedBytes;
    state_com->saved_crc = state_com->crc;
    return AAC_NOT_ENOUGH_BUFFER;
  }

  ippsJoin_32f16s_D2L((const Ipp32f **)(state->m_ordered_samples),
                      state_com->m_channel_number, state->com.m_up_sample * 1024, outPointer);

  state_com->decodedBytes = 0;
  state_com->m_frame_number++;

  /* AYA: tmp diagn */
  //printf("\nframe %i\n", state_com->m_frame_number-1);

  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecSetSamplingFrequency(Ipp32s sampling_frequency_index,
                                     AACDec *state)
{
  return aacdecSetSamplingFrequencyCom(sampling_frequency_index,
                                       &(state->com));
}

/********************************************************************/

AACStatus aacdecSetLayer(Ipp32s layer,
                         AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  state->com.layer = layer;

  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecSetSBRModeDecode(Ipp32s ModeDecodeHEAAC,
                                 AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  if (state->com.ModeDecodeHEAACprofile != ModeDecodeHEAAC) {
    state->com.ModeDecodeHEAACprofile = ModeDecodeHEAAC;
    sbrdecInitFilter(state);
  }

  return AAC_OK;
}


/********************************************************************/

AACStatus aacdecSetSBRModeDwnsmpl(Ipp32s ModeDwnsmplHEAAC,
                                AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  state->com.ModeDwnsmplHEAACprofile = ModeDwnsmplHEAAC;

  return AAC_OK;
}


/********************************************************************/

AACStatus aacdecSetSBRSupport(eSBR_SUPPORT flag, AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  state->com.m_flag_SBR_support_lev = flag;
  return AAC_OK;
}

/* PARAMETRIC STEREO */
AACStatus aacdecSetPSSupport( Ipp32s flag,  AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  state->com.m_flag_PS_support_lev = flag;

  return AAC_OK;
}

/********************************************************************/

AACStatus bsacdecSetNumChannels(Ipp32s channelConfiguration,
                                AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  if ((channelConfiguration == 0) || (channelConfiguration > 7)) {
    return AAC_BAD_STREAM;
  }

  state->com.m_channel_number = channelConfiguration;

  return AAC_OK;
}


/********************************************************************/

AACStatus aacdecSetAudioObjectType(enum AudioObjectType audio_object_type,
                                   AACDec *state,
                                   sALS_specific_config *inState,
                                   Ipp8u *mem,
                                   Ipp32s *size_all)
{
  AACStatus aacStatus;
  IppStatus ippStatus;
  Ipp32s mode = FB_DECODER;
  Ipp32s size = 10;
  Ipp32s tmpSize, i;
  IppStatus ippRes;

  if (!state)
    return AAC_NULL_PTR;

  state->com.m_audio_object_type = audio_object_type;

  if (AOT_AAC_ALS != audio_object_type) {
    if ((AOT_AAC_LTP == audio_object_type)
#ifdef USE_LTP_IN_BSAC
     || (AOT_ER_BSAC == audio_object_type)
#endif
      ) {
      mode = mode | FB_ENCODER;
    }

    if (mem) {
      if (AOT_ER_BSAC == audio_object_type) {
        tmpSize = __ALIGNED(sizeof(BSACDec));
        state->bsState = (BSACDec*)mem;
        size += tmpSize;
        mem += tmpSize;
      }

      aacStatus = InitFilterbank(&(state->m_filterbank), mem, mode,
                                audio_object_type, &tmpSize);
      if (aacStatus != AAC_OK) return aacStatus;

      size += tmpSize;
      mem += tmpSize;

      if (AOT_AAC_MAIN == audio_object_type) {
        ippStatus = ippsFDPGetSize_32f(1024, &tmpSize);
        if (ippStatus != ippStsNoErr) return AAC_ALLOC;

        for (i = 0; i < CH_MAX + COUPL_CH_MAX; i++) {
          state->pFDPState[i] = (IppsFDPState_32f*)mem;
          ippRes = ippsFDPInit_32f(&(state->pFDPState[i]),1024, mem);
          if (ippRes != ippStsNoErr) return AAC_ALLOC;

          size += tmpSize;
          mem += tmpSize;
        }
      }
    } else {

      if (AOT_ER_BSAC == audio_object_type) {
        size += __ALIGNED(sizeof(BSACDec));
      }

      aacStatus = InitFilterbank(NULL, mem, mode,
                                audio_object_type, &tmpSize);
      if (aacStatus != AAC_OK) return aacStatus;

      size += tmpSize;

      if (AOT_AAC_MAIN == audio_object_type) {
        ippStatus = ippsFDPGetSize_32f(1024, &tmpSize);
        if (ippStatus != ippStsNoErr) return AAC_ALLOC;

        size += (CH_MAX + COUPL_CH_MAX) * tmpSize;
      }
    }
  } else {
    alsdecInit(inState, &(state->alsState), mem, &size);
  }
  *size_all = size;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecSetPCE(sProgram_config_element *pce,
                       AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  ippsCopy_8u((Ipp8u*)pce, (Ipp8u*)(&(state->com.m_pce)),
              sizeof(sProgram_config_element));

  state->com.m_is_pce_valid = 1;

  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecSetAdtsChannelConfiguration(Ipp32s adts_channel_configuration,
                                            AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  if ((adts_channel_configuration < 0) || (adts_channel_configuration > 7))
    return AAC_BAD_PARAMETER;

  state->com.adts_channel_configuration = adts_channel_configuration;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecInitCRC(Ipp32s crc_enable,
                        AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  bs_CRC_reset((Ipp32u*)&state->com.crc);
  state->com.crc_enable = crc_enable;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecSetCRC(Ipp32s crc,
                       AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  state->com.crc = crc;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecGetCRC(Ipp32s *crc,
                       AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  if (!crc)
    return AAC_NULL_PTR;

  *crc = state->com.crc;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecGetSbrFlagPresent(Ipp32s *SbrFlagPresent,
                                  AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  *SbrFlagPresent = state->com.SbrFlagPresent;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecGetFrameSize(Ipp32s *frameSize,
                             AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  *frameSize = state->com.m_frame_size * state->com.m_up_sample;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecGetSampleFrequency(Ipp32s *freq,
                                   AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  *freq = state->com.m_sampling_frequency * state->com.m_up_sample;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecGetNumDecodedFrames(Ipp32s *num,
                                    AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  *num = state->com.m_frame_number;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecGetChannels(Ipp32s *ch,
                            AACDec *state)
{
  if (!state)
    return AAC_NULL_PTR;

  *ch = state->com.m_channel_number;
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecClose(AACDec *state)
{
  if (state == NULL)
    return AAC_OK;

  /* parametric stereo */
  psFreeDecoder_32f( /* &(state->psState) */);
  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecGetInfo(cAudioCodecParams *a_info,
                        AACDec *state)
{
  Ipp32s frameNumber;
  if (!a_info)
    return AAC_NULL_PTR;

  a_info->m_SuggestedInputSize = CH_MAX * 768;
  a_info->m_SuggestedOutputSize = CH_MAX * 1024 * 2 /* upsampling SBR */ * sizeof(Ipp16s);
  a_info->is_valid = 0;

  if (!state)
    return AAC_OK;

  frameNumber = state->com.m_frame_number;

  if (state->com.m_audio_object_type == AOT_AAC_ALS) {
    Ipp32u maxHeaderSize = state->alsState.headerSize;

    if (state->alsState.trailerSize > maxHeaderSize) {
      maxHeaderSize = state->alsState.trailerSize;
    }

    a_info->m_SuggestedOutputSize = state->alsState.numChannels *
                                    state->alsState.frameLength *
                                    (state->alsState.resolution >> 3) +
                                    maxHeaderSize;

    a_info->m_SuggestedInputSize = a_info->m_SuggestedOutputSize;

    frameNumber = state->alsState.frameId;
  }

  if (frameNumber > 0) {
    if (state->com.m_audio_object_type == AOT_AAC_ALS) {
      a_info->m_info_out.bitPerSample = state->alsState.resolution;

      a_info->m_info_in.channels = state->alsState.numChannels;
      a_info->m_info_out.channels = state->alsState.numChannels;

      a_info->m_info_in.sample_frequency = state->alsState.sampFreq;
      a_info->m_info_out.sample_frequency = state->alsState.sampFreq;

      a_info->m_info_in.stream_type = AAC_AUD;
      a_info->m_info_out.stream_type = PCM_AUD;

      a_info->m_frame_num = state->alsState.frameId;

      a_info->is_valid = 1;
    } else {
      a_info->m_info_out.bitPerSample = 16;

      a_info->m_info_in.channels = state->com.m_channel_number;
      a_info->m_info_out.channels = state->com.m_channel_number;

      a_info->m_info_in.sample_frequency = state->com.m_sampling_frequency;
      a_info->m_info_out.sample_frequency = state->com.m_sampling_frequency * state->com.m_up_sample;

      a_info->m_info_in.stream_type = AAC_AUD;
      a_info->m_info_out.stream_type = PCM_AUD;

      a_info->m_frame_num = state->com.m_frame_number;
    }

    a_info->m_info_in.bitPerSample = 0;
    a_info->m_info_in.bitrate = 0;
    a_info->m_info_out.bitrate = a_info->m_info_out.sample_frequency *
                                 a_info->m_info_out.bitPerSample;

    a_info->m_info_in.stream_type = AAC_AUD;
    a_info->m_info_out.stream_type = PCM_AUD;

    a_info->is_valid = 1;

    return AAC_OK;
  }

  return AAC_OK;
}

/********************************************************************/

AACStatus aacdecGetDuration(Ipp32f *p_duration,
                            AACDec *state)
{
  Ipp32f  duration;

  if (state->com.m_audio_object_type != AOT_AAC_ALS) {
    duration = (Ipp32f)(state->com.m_frame_number) * 1024;
    *p_duration = duration / (Ipp32f)(state->com.m_sampling_frequency);
  } else {
    *p_duration = state->alsState.numSamples / (Ipp32f)(state->alsState.sampFreq);
  }

  return AAC_OK;
}

/********************************************************************/

AACStatus aacdec_GetID3Len(Ipp8u *in,
                           Ipp32s inDataSize,
                           AACDec *state)
{
  if (state->com.FirstID3Search != 0)
    return AAC_OK;

  if (inDataSize < 10)
    return AAC_NOT_ENOUGH_DATA;

  if ((in[0] == 'I') && (in[1] == 'D') && (in[2] == '3') && /* 'ID3' */
      (in[3] < 0xFF) && (in[4] < 0xFF) &&                   /* Version or revision will never be 0xFF */
      (in[6] < 0x80) && (in[7] < 0x80) && (in[8] < 0x80) && (in[9] < 0x80)) { /* size */
    state->com.id3_size = (in[6] << 21) + (in[7] << 14) + (in[8] << 7) + in[9] +
      (in[5] & 0x10 ? 20 : 10);
  } else {
    state->com.id3_size = 0;
  }

  state->com.FirstID3Search = 1;

  return AAC_OK;
}

/********************************************************************/

AACStatus aacdec_SkipID3(Ipp32s inDataSize,
                         Ipp32s *skipped,
                         AACDec *state)
{
  *skipped = 0;
  if (state->com.id3_size > 0) {
    if (inDataSize < state->com.id3_size) {
      *skipped = inDataSize;
      state->com.id3_size -= inDataSize;
      return AAC_NOT_ENOUGH_DATA;
    } else {
      *skipped = state->com.id3_size;
      state->com.id3_size = 0;
      return AAC_OK;
    }
  }
  return AAC_OK;
}

/********************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER

