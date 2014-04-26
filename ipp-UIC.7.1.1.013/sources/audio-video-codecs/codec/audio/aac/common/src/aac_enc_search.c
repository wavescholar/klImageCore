/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//     Intel Integrated Performance Primitives AAC Encode Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel IPP
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

#include "align.h"
#include "aac_enc_huff_tables.h"
#include "aac_enc_search.h"
#include "aac_enc_own.h"

#include "ippac.h"

/****************************************************************************/

Ipp32s best_codebooks_search(sEnc_individual_channel_stream* pStream,
                             Ipp16s *px_quant_unsigned,
                             Ipp16s *px_quant_signed,
                             Ipp16s *px_quant_unsigned_pred,
                             Ipp16s *px_quant_signed_pred)
{
  Ipp32s *sfb_offset;
  Ipp32s *pred_flag;
  Ipp16s *p_sfb_cb;
  Ipp32u index;
#if !defined(ANDROID)
  Ipp32s sfb_bit_len[MAX_SFB][12];
  Ipp32s sfb_bit_len1[MAX_SFB][12];
  Ipp32s cb_trace[MAX_SFB][12];
  Ipp32s isPred[MAX_SFB][12];
#else
  static Ipp32s sfb_bit_len[MAX_SFB][12];
  static Ipp32s sfb_bit_len1[MAX_SFB][12];
  static Ipp32s cb_trace[MAX_SFB][12];
  static Ipp32s isPred[MAX_SFB][12];
#endif
  Ipp32s min_value, min_value_pred;
  Ipp32s sect_bits;
  Ipp32s len_esc_value;
  Ipp32s pred, side_info, predPresent;
  Ipp32s bits_for_changes, win, sfb, i, j;

  side_info = predPresent = min_value_pred = min_value = 0;
  pred_flag = NULL;

  if (pStream->windows_sequence == EIGHT_SHORT_SEQUENCE) {
    len_esc_value = 3;
  } else {
    len_esc_value = 5;
  }

  pred = 0;
  if (px_quant_unsigned_pred) {
    pred = 1;
    if (pStream->audioObjectType == AOT_AAC_LTP) {
      Ipp32s max_sfb = pStream->max_sfb;
      if (max_sfb > MAX_LTP_SFB_LONG) max_sfb = MAX_LTP_SFB_LONG;
      pred_flag = pStream->ltp_long_used;
      side_info = 1 + 11 + 3 + max_sfb;
    }
  }

  sfb_offset = pStream->sfb_offset;
  p_sfb_cb = pStream->sfb_cb;

  bits_for_changes = 4 + len_esc_value;
  sect_bits = 0;

  for (win = 0; win < pStream->num_window_groups; win++) {
    predPresent = 0;
#if 1
    bit_count(pStream, px_quant_unsigned,
              px_quant_signed, sfb_offset, sfb_bit_len);
#else
    if (px_quant_signed[0]>0){
       px_quant_signed[0] = px_quant_signed[0];
    }
    ippsVLCCountBits_AAC_16s32s(px_quant_unsigned, px_quant_signed, (const Ipp32u*)sfb_offset, (Ipp32s*)sfb_bit_len,
                                                pStream->max_sfb, (const IppsVLCEncodeSpec_32s**) pStream->pHuffTables);
#endif

    if (pred) {
#if 1
      bit_count(pStream, px_quant_unsigned_pred,
                px_quant_signed_pred, sfb_offset, sfb_bit_len1);
#else
      ippsVLCCountBits_AAC_16s32s(px_quant_unsigned_pred, px_quant_signed_pred, (const Ipp32u*)sfb_offset, (Ipp32s*)sfb_bit_len1,
                                                pStream->max_sfb, (const IppsVLCEncodeSpec_32s**) pStream->pHuffTables);
#endif

      for (i = 0; i < pStream->max_sfb; i++) {
        for (j = 0; j < 12; j++) {
          if (sfb_bit_len[i][j] <= sfb_bit_len1[i][j]) {
            sfb_bit_len1[i][j] = sfb_bit_len[i][j];
            isPred[i][j] = 0;
          } else {
            isPred[i][j] = 1;
          }
        }
      }

      tree_build(sfb_bit_len1, cb_trace, pStream->max_sfb, len_esc_value);
#if 0
      ippsMinIndx_32s(sfb_bit_len1[pStream->max_sfb - 1], 12, &min_value_pred, &index);
#else
      min_value_pred = sfb_bit_len1[pStream->max_sfb - 1][0];
      index = 0;
      for ( i = 1; i < 12; i++ ) {
        if ( sfb_bit_len1[pStream->max_sfb - 1][i] < min_value_pred ) {
                min_value_pred = sfb_bit_len1[pStream->max_sfb - 1][i];
                index = i;
        }
      }
#endif

      predPresent = 0;
      p_sfb_cb[pStream->max_sfb - 1] = (Ipp16s)index;
      pred_flag[pStream->max_sfb - 1] = isPred[pStream->max_sfb - 1][index];
      predPresent |= pred_flag[pStream->max_sfb - 1];

      for (sfb = pStream->max_sfb - 2; sfb >= 0; sfb--) {
        index = cb_trace[sfb][index];
        p_sfb_cb[sfb] = (Ipp16s)index;
        pred_flag[sfb] = isPred[sfb][index];
        predPresent |= pred_flag[sfb];
      }

      if (predPresent == 0) {
        min_value = min_value_pred;
      } else {
        min_value_pred += side_info;
      }
    }

    if ((!pred) || predPresent) {

      tree_build(sfb_bit_len, cb_trace, pStream->max_sfb, len_esc_value);
#if 0
      ippsMinIndx_32s(sfb_bit_len[pStream->max_sfb - 1], 12, &min_value, &index);
#else
      min_value = sfb_bit_len[pStream->max_sfb - 1][0];
      index = 0;
      for ( i = 1; i < 12; i++ ) {
        if ( sfb_bit_len[pStream->max_sfb - 1][i] < min_value ) {
                min_value = sfb_bit_len[pStream->max_sfb - 1][i];
                index = i;
        }
      }
#endif
      if (pred) {
        if (min_value < min_value_pred) {
          predPresent = 0;

          p_sfb_cb[pStream->max_sfb - 1] = (Ipp16s)index;

          for (sfb = pStream->max_sfb - 2; sfb >= 0; sfb--) {
            index = cb_trace[sfb][index];
            p_sfb_cb[sfb] = (Ipp16s)index;
          }
        } else {
          min_value = min_value_pred;
        }
      } else {
        p_sfb_cb[pStream->max_sfb - 1] = (Ipp16s)index;

        for (sfb = pStream->max_sfb - 2; sfb >= 0; sfb--) {
          index = cb_trace[sfb][index];
          p_sfb_cb[sfb] = (Ipp16s)index;
        }
      }
    }

    sect_bits += min_value + bits_for_changes;
    sfb_offset += pStream->max_sfb;
    p_sfb_cb += pStream->max_sfb;
  }

  if (pred) {
    if (pStream->audioObjectType == AOT_AAC_LTP) {
      pStream->ltp_data_present = predPresent;
    }
  }

  return (sect_bits);
}

/****************************************************************************/

void tree_build(Ipp32s sfb_bit_len[MAX_SFB][12],
                Ipp32s cb_trace[MAX_SFB][12],
                Ipp32s max_sfb,
                Ipp32s len_esc_value)
{
#if !defined(ANDROID)
  Ipp16s sect_len[12];
#else
  static Ipp16s sect_len[12];
#endif
  Ipp32u index, i;
  Ipp32s min_value;
  Ipp32s sect_esc_value;
  Ipp32s bits_for_changes, sfb, cb;

#if 0
  ippsSet_16s(1, (Ipp16s*)sect_len, 12);
#else
  for (i=0; i<12; i++){
     sect_len[i] = 1;
  }
#endif

  bits_for_changes = 4 + len_esc_value;
  sect_esc_value = (1 << len_esc_value) - 2;

  for (sfb = 0; sfb < max_sfb - 1; sfb++) {
#if 0
    ippsMinIndx_32s(sfb_bit_len[sfb], 12, &min_value, &index);
#else
    int i;
    min_value = sfb_bit_len[sfb][0];
    index = 0;
    for ( i = 1; i < 12; i++ ) {
      if ( sfb_bit_len[sfb][i] < min_value ) {
         min_value = sfb_bit_len[sfb][i];
         index = i;
      }
    }
#endif
    min_value += bits_for_changes;

    for (cb = 0; cb < 12; cb++) {
      /* if it is very expensive to change a codebook */
      if (sfb_bit_len[sfb][cb] <= min_value) {
        if (sect_len[cb] != sect_esc_value) {
          sfb_bit_len[sfb+1][cb] += sfb_bit_len[sfb][cb];
          cb_trace[sfb][cb] = cb;
          sect_len[cb]++;
        } else {
          if (sfb_bit_len[sfb][cb] + len_esc_value <= min_value) {
            sfb_bit_len[sfb+1][cb] += sfb_bit_len[sfb][cb] + len_esc_value;
            cb_trace[sfb][cb] = cb;
            sect_len[cb] = 0;
          } else {
            sfb_bit_len[sfb+1][cb] += min_value;
            cb_trace[sfb][cb] = index;
            sect_len[cb] = 1;
          }
        }
      } else { /* if no */
        sfb_bit_len[sfb+1][cb] += min_value;
        cb_trace[sfb][cb] = index;
        sect_len[cb] = 1;
      }
    }
  }
}

/****************************************************************************/
#if 1
static Ipp32s choice_table[14] = {
  5, 5, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 0
};

void bit_count(sEnc_individual_channel_stream* pStream,
               Ipp16s *px_quant_unsigned,
               Ipp16s *px_quant_signed,
               Ipp32s *sfb_offset,
               Ipp32s sfb_bit_len[MAX_SFB][12])
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp16s, tmp_src, 512);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp16s, tmp_src, 512);
#endif
  Ipp16s *q;
  Ipp32s sfb;
  Ipp32s sfb_start;
  Ipp32s sfb_end;
  Ipp32s i, done;
  Ipp16s max_value;
  Ipp32s shift, offset;

  for (sfb = 0; sfb < pStream->max_sfb; sfb++) {
    sfb_start = sfb_offset[sfb];
    sfb_end = sfb_offset[sfb+1];
    done = 0;

//    ippsMax_16s(&(px_quant_unsigned[sfb_start]), sfb_end - sfb_start, &max_value);
    max_value = px_quant_unsigned[sfb_start];
    for ( i = 0; i < (sfb_end - sfb_start); i++ ) {
      if ( px_quant_unsigned[sfb_start+i] > max_value ) max_value = px_quant_unsigned[sfb_start+i];
    }

    for (i = 0; i < 12; i++) {
      sfb_bit_len[sfb][i] = 0x7FFF;
    }

    if (max_value == 0) {
      sfb_bit_len[sfb][0] = 0;
    }

    if (max_value > MAX_NON_ESC_VALUE)
      max_value = 13;

    switch (choice_table[max_value]) {

    case 5:
      shift = vlcEncShifts[1];
      offset = vlcEncOffsets[1];
      q = &(px_quant_signed[sfb_start]);
      for (i = 0; i < (sfb_end - sfb_start) >> 2; i++) {
        tmp_src[i] = (Ipp16s)((q[0] << (3*shift)) + ((q[1] + offset) << (2*shift)) +
                             ((q[2] + offset) << shift) + (q[3] + offset));
        q += 4;
      }
      ippsVLCCountBits_16s32s(tmp_src, (sfb_end - sfb_start) >> 2,
                              &sfb_bit_len[sfb][1],
                              (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[1]));
      ippsVLCCountBits_16s32s(tmp_src, (sfb_end - sfb_start) >> 2,
                              &sfb_bit_len[sfb][2],
                              (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[2]));

    case 4:
      shift = vlcEncShifts[3];
      offset = vlcEncOffsets[3];
      q = &(px_quant_signed[sfb_start]);
      for (i = 0; i < (sfb_end - sfb_start) >> 2; i++) {
        tmp_src[i] = (Ipp16s)((q[0] << (3*shift)) + ((q[1] + offset) << (2*shift)) +
                             ((q[2] + offset) << shift) + (q[3] + offset));
        q += 4;
      }
      ippsVLCCountBits_16s32s(tmp_src, (sfb_end - sfb_start) >> 2,
                              &sfb_bit_len[sfb][3],
                              (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[3]));
      ippsVLCCountBits_16s32s(tmp_src, (sfb_end - sfb_start) >> 2,
                              &sfb_bit_len[sfb][4],
                              (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[4]));

    case 3:
      shift = vlcEncShifts[5];
      offset = vlcEncOffsets[5];
      q = &(px_quant_signed[sfb_start]);
      for (i = 0; i < (sfb_end - sfb_start) >> 1; i++) {
        tmp_src[i] = (Ipp16s)((q[0] << shift) + (q[1] + offset));
        q += 2;
      }
      done = 1;
      ippsVLCCountBits_16s32s(tmp_src, (sfb_end - sfb_start) >> 1,
                              &sfb_bit_len[sfb][5],
                              (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[5]));
      ippsVLCCountBits_16s32s(tmp_src, (sfb_end - sfb_start) >> 1,
                              &sfb_bit_len[sfb][6],
                              (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[6]));

    case 2:
      /* shift and offset for 7 and 8 are equal to shift and offset for 5 and 6 */
      if (0 == done) {
        shift = vlcEncShifts[7];
        offset = vlcEncOffsets[7];
        q = &(px_quant_signed[sfb_start]);
        for (i = 0; i < (sfb_end - sfb_start) >> 1; i++) {
          tmp_src[i] = (Ipp16s)((q[0] << shift) + (q[1] + offset));
          q += 2;
        }
      }
      ippsVLCCountBits_16s32s(tmp_src, (sfb_end - sfb_start) >> 1,
                              &sfb_bit_len[sfb][7],
                              (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[7]));
      ippsVLCCountBits_16s32s(tmp_src, (sfb_end - sfb_start) >> 1,
                              &sfb_bit_len[sfb][8],
                              (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[8]));

    case 1:
      shift = vlcEncShifts[9];
      offset = vlcEncOffsets[9];
      q = &(px_quant_signed[sfb_start]);
      for (i = 0; i < (sfb_end - sfb_start) >> 1; i++) {
        tmp_src[i] = (Ipp16s)((q[0] << shift) + (q[1] + offset));
        q += 2;
      }
      ippsVLCCountBits_16s32s(tmp_src, (sfb_end - sfb_start) >> 1,
                              &sfb_bit_len[sfb][9],
                              (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[9]));
      ippsVLCCountBits_16s32s(tmp_src, (sfb_end - sfb_start) >> 1,
                              &sfb_bit_len[sfb][10],
                              (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[10]));

    case 0:
      ippsVLCCountEscBits_AAC_16s32s(&(px_quant_signed[sfb_start]),
                                     (sfb_end - sfb_start),
                                     &sfb_bit_len[sfb][11],
                                     (IppsVLCEncodeSpec_32s*)(pStream->pHuffTables[11]));
    }
  }
}
#endif

#endif //UMC_ENABLE_XXX

