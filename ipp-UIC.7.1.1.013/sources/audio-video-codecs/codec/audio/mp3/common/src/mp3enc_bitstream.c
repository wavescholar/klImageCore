/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_MP3_AUDIO_ENCODER)

#include "mp3enc_own.h"

#define PutBitsSI(val, len)       PUT_BITS(&state->sideStream, val, len)
#define PutBits(val, len)         PUT_BITS(&state->mainStream, val, len)
#define PutBitsDst(dst, val, len) PUT_BITS(dst, val, len)

/*******************************************************************/

static const VM_ALIGN32_DECL(Ipp8u) mp3enc_trans_channel_5[5][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {2, 5, 6, 2, 2, 2, 6, 5},
  {3, 3, 3, 5, 3, 5, 5, 3},
  {4, 4, 4, 4, 6, 6, 4, 6},
};

static const VM_ALIGN32_DECL(Ipp8u) mp3enc_trans_channel_4a[4][6] = {
  {0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1},
  {2, 5, 6, 2, 2, 5},
  {3, 3, 3, 5, 6, 6},
};

static const VM_ALIGN32_DECL(Ipp8u) mp3enc_trans_channel_4b[4][4] = {
  {0, 0, 0, 0},
  {1, 1, 1, 1},
  {2, 5, 2, 5},
  {3, 3, 6, 6},
};

static const VM_ALIGN32_DECL(Ipp8u) mp3enc_trans_channel_3[5][3] = {
  {0, 0, 0},
  {1, 1, 1},
  {2, 5, 6},
  {3, 3, 3},
  {4, 4, 4},
};

static const VM_ALIGN32_DECL(Ipp32s) mp3enc_dyn_channel_4[3][15] = {
  {1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1},
  {1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0},
  {1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
};

static const VM_ALIGN32_DECL(Ipp32s) mp3enc_dyn_channel_3[2][5] = {
  {1, 1, 0, 0, 1},
  {1, 0, 1, 0, 0}
};

static const VM_ALIGN32_DECL(Ipp32s) mp3enc_dyn_channel_1[3][2] = {
  {1, 0},
  {1, 1},
  {1, 1}
};

/*******************************************************************/

static void mp3enc_encodeBigValues(MP3Enc_com *state,
                                   Ipp16s *pInput,
                                   Ipp32s gr,
                                   Ipp32s ch)
{
    IPP_ALIGNED_ARRAY(32, Ipp16s, tmp_src, 288);
    Ipp32s i, j, bigvalues, len;
    Ipp32s shift, offset;
    Ipp32s bitoffset;
    Ipp16s *q;
    Ipp8u  *pDst;
    IppsVLCEncodeSpec_32s* pVLCEncSpec;
    MP3Enc_HuffmanTable *htables = state->htables;

    pDst = (Ipp8u*)state->mainStream.pCurrent_dword +
           ((32 - state->mainStream.nBit_offset) >> 3);
    bitoffset = (32 - state->mainStream.nBit_offset) & 0x7;

    SAVE_BITSTREAM(&state->mainStream)

    bigvalues = state->si_bigVals[gr][ch] << 1;

    if (state->si_address[gr][ch][0] > 0) {
      i = state->si_pTableSelect[gr][ch][0];
      shift = mp3enc_VLCShifts[i];
      offset = mp3enc_VLCOffsets[i];
      pVLCEncSpec = (IppsVLCEncodeSpec_32s*)htables[i].phuftable;
      len = state->si_address[gr][ch][0] >> 1;
      q = pInput;

      if (htables[i].linbits == 0) {
        for (j = 0; j < len; j++) {
          tmp_src[j] = (Ipp16s)((q[0] << shift) + (q[1] + offset));
          q += 2;
        }

        ippsVLCEncodeBlock_16s1u(tmp_src, len,
                                 &pDst, &bitoffset, pVLCEncSpec);

      } else
        ippsVLCEncodeEscBlock_MP3_16s1u(q, len * 2,
                                        htables[i].linbits,
                                        &pDst, &bitoffset, pVLCEncSpec);
    }

    if (state->si_address[gr][ch][1] > state->si_address[gr][ch][0]) {
      i = state->si_pTableSelect[gr][ch][1];
      shift = mp3enc_VLCShifts[i];
      offset = mp3enc_VLCOffsets[i];
      pVLCEncSpec = (IppsVLCEncodeSpec_32s*)htables[i].phuftable;
      len = (state->si_address[gr][ch][1] - state->si_address[gr][ch][0]) >> 1;
      q = pInput + state->si_address[gr][ch][0];

        if (htables[i].linbits == 0) {
        for (j = 0; j < len; j++) {
          tmp_src[j] = (Ipp16s)((q[0] << shift) + (q[1] + offset));
          q += 2;
        }
        ippsVLCEncodeBlock_16s1u(tmp_src, len,
                                 &pDst, &bitoffset, pVLCEncSpec);

      } else
        ippsVLCEncodeEscBlock_MP3_16s1u(q, len * 2,
                                        htables[i].linbits,
                                        &pDst, &bitoffset, pVLCEncSpec);
    }

    if ((Ipp32u)bigvalues > state->si_address[gr][ch][1]) {
      i = state->si_pTableSelect[gr][ch][2];
      shift = mp3enc_VLCShifts[i];
      offset = mp3enc_VLCOffsets[i];
      pVLCEncSpec = (IppsVLCEncodeSpec_32s*)htables[i].phuftable;
      len = (bigvalues - state->si_address[gr][ch][1]) >> 1;
      q = pInput + state->si_address[gr][ch][1];

      if (htables[i].linbits == 0) {
        for (j = 0; j < len; j++) {
          tmp_src[j] = (Ipp16s)((q[0] << shift) + (q[1] + offset));
          q += 2;
        }
        ippsVLCEncodeBlock_16s1u(tmp_src, len,
                                 &pDst, &bitoffset, pVLCEncSpec);
      } else
        ippsVLCEncodeEscBlock_MP3_16s1u(q, len * 2,
                                        htables[i].linbits,
                                        &pDst, &bitoffset, pVLCEncSpec);
    }

    state->mainStream.pCurrent_dword = (Ipp32u*)(pDst - ((size_t)(pDst) & 3));
    state->mainStream.nBit_offset = (Ipp32s)(32 -
      ((pDst - (Ipp8u*)state->mainStream.pCurrent_dword) << 3) -  bitoffset);

    LOAD_DWORD(&state->mainStream)

    return;
}

/*******************************************************************/

static void mp3enc_huffmanCodeBits(MP3Enc_com *state,
                                   Ipp32s gr,
                                   Ipp32s ch)
{
    const Ipp16s* h = NULL;
    Ipp32s count1End;
    Ipp32s bits, bits_before;
    Ipp32s i, bigvalues;
    Ipp16s  x, y, w, v, p, val, len;

    bigvalues = state->si_bigVals[gr][ch] << 1;

    if (bigvalues) {
      if (!(state->si_mixedBlock[gr][ch]) && state->si_winSwitch[gr][ch] &&
          (state->si_blockType[gr][ch] == 2)) {

        GET_BITS_COUNT(&state->mainStream, bits_before)
        mp3enc_encodeBigValues(state, state->quant_ix[gr][ch], gr, ch);
        GET_BITS_COUNT(&state->mainStream, bits)
        bits -= bits_before;

      } else if (state->si_mixedBlock[gr][ch] && state->si_blockType[gr][ch] == 2) {
//        vm_debug_trace(VM_DEBUG_ALL, VM_STRING("mixed block is not implemented"));
      } else {
        mp3enc_encodeBigValues(state, state->quant_ix[gr][ch], gr, ch);
      }
    }

    if (state->si_cnt1TabSel[gr][ch])
      h = mp3enc_table33;
    else
      h = mp3enc_table32;

    count1End = bigvalues + (state->si_count1[gr][ch] * 4);
    for (i = bigvalues; i < count1End; i += 4) {
      v = (Ipp16s)abs(state->quant_ix[gr][ch][i]);
      w = (Ipp16s)abs(state->quant_ix[gr][ch][i + 1]);
      x = (Ipp16s)abs(state->quant_ix[gr][ch][i + 2]);
      y = (Ipp16s)abs(state->quant_ix[gr][ch][i + 3]);
      p = (v << 3) | (w << 2) | (x << 1) | y;

      val = h[3 * p + 2];
      len = h[3 * p + 1];

      val = (val << v) | ((state->quant_ix[gr][ch][i] < 0) ? 1 : 0);
      val = (val << w) | ((state->quant_ix[gr][ch][i + 1] < 0) ? 1 : 0);
      val = (val << x) | ((state->quant_ix[gr][ch][i + 2] < 0) ? 1 : 0);
      val = (val << y) | ((state->quant_ix[gr][ch][i + 3] < 0) ? 1 : 0);

      PutBits(val, len);
    }
}

/*******************************************************************/

static Ipp32s mp3enc_encodeHeader(MP3Enc_com *state, sBitsreamBuffer *stream) {
  Ipp32s  stereo = state->stereo;

  state->header.mode = (stereo == 1) ? 0x3 : 0x0;
  state->header.modeExt = 0;
  if (state->header.layer == 3) {
    if (state->stereo_mode == MPA_MS_STEREO) {
      state->header.mode = 1;
      state->header.modeExt = 2;
    }
  } else {
    if (state->stereo_mode == MPA_JOINT_STEREO) {
      state->header.mode = 1;
      state->header.modeExt = state->stereo_mode_ext;
    }
  }

  // header
  PutBitsDst(stream, 0xfff, 12);
  PutBitsDst(stream, state->header.id, 1);
  PutBitsDst(stream, (4 - state->header.layer), 2);
  PutBitsDst(stream, !state->header.protectionBit, 1);
  PutBitsDst(stream, state->header.bitRate, 4);
  PutBitsDst(stream, state->header.samplingFreq, 2);
  PutBitsDst(stream, state->header.paddingBit, 1);
  PutBitsDst(stream, state->header.privateBit, 1);
  PutBitsDst(stream, state->header.mode, 2);
  PutBitsDst(stream, state->header.modeExt, 2);
  PutBitsDst(stream, state->header.copyright, 1);
  PutBitsDst(stream, state->header.originalCopy, 1);
  PutBitsDst(stream, state->header.emphasis, 2);

  if(state->header.protectionBit)
    PutBitsDst(stream, 0, 16);

  return 0;
}

/*******************************************************************/

void mp3enc_mc_encodeHeader(MP3Enc_com *state, sBitsreamBuffer *stream, Ipp32s *nbits)
{
  Ipp32s bits = 16;
  PutBitsDst(stream, state->mc_header.ext_bit_stream_present, 1);
  if(state->mc_header.ext_bit_stream_present == 1) {
    PutBitsDst(stream, state->mc_header.n_ad_bytes, 8);
    bits = 24;
  }
  PutBitsDst(stream, state->mc_header.center, 2);
  PutBitsDst(stream, state->mc_header.surround, 2);
  PutBitsDst(stream, state->mc_header.lfe, 1);
  PutBitsDst(stream, state->mc_header.audio_mix, 1);
  PutBitsDst(stream, state->mc_header.dematrix_procedure, 2);
  PutBitsDst(stream, state->mc_header.no_of_multi_lingual_ch, 3);
  PutBitsDst(stream, state->mc_header.multi_lingual_fs, 1);
  PutBitsDst(stream, state->mc_header.multi_lingual_layer, 1);
  PutBitsDst(stream, state->mc_header.copyright_identification_bit, 1);
  PutBitsDst(stream, state->mc_header.copyright_identification_start, 1);
  *nbits = bits;
}

/*******************************************************************/

static void mp3enc_mc_encode_composite_status_info(MP3Enc_com *state, sBitsreamBuffer *stream)
{
  Ipp32s i, j;

  PutBitsDst(stream, state->mc_tc_sbgr_select, 1);
  PutBitsDst(stream, state->mc_dyn_cross_on, 1);
  PutBitsDst(stream, state->mc_prediction_on, 1);

  if (state->mc_channel_conf == 320 || state->mc_channel_conf == 310)
  {
    if (state->mc_tc_sbgr_select == 1) {
      PutBitsDst(stream, state->mc_tc_allocation, 3);
    } else {
      for (i = 0; i < 12; i++)
        PutBitsDst(stream, state->mc_tc_alloc[i], 3);
    }
  }
  else if (state->mc_channel_conf == 300 || state->mc_channel_conf == 302 ||
    state->mc_channel_conf == 220 || state->mc_channel_conf == 210)
  {
    if (state->mc_tc_sbgr_select == 1) {
      PutBitsDst(stream, state->mc_tc_allocation, 2);
    } else {
      for (i = 0; i < 12; i++)
        PutBitsDst(stream, state->mc_tc_alloc[i], 2);
    }
  }

  if (state->mc_dyn_cross_on == 1)
  {
    PutBitsDst(stream, state->mc_dyn_cross_LR, 1);
    for (i = 0; i < 12; i++)
    {
      if (state->mc_channel_conf) {
        PutBitsDst(stream, state->mc_dyn_cross_mode[i], 4);
      } else if (state->mc_channel_conf == 310 || state->mc_channel_conf == 220) {
        PutBitsDst(stream, state->mc_dyn_cross_mode[i], 3);
      } else if (state->mc_channel_conf == 300 || state->mc_channel_conf == 302 || state->mc_channel_conf == 210) {
        PutBitsDst(stream, state->mc_dyn_cross_mode[i], 1);
      }

      if (state->mc_header.surround == 3) {
        PutBitsDst(stream, state->mc_dyn_second_stereo[i], 1);
      }
    }
  }

  if (state->mc_prediction_on == 1)
  {
    for(i = 0; i < 8; i++)
    {
      PutBitsDst(stream, state->mc_prediction[i], 1);
      if (state->mc_prediction[i] == 1)
      {
        Ipp32s val;
        val = mp3_mc_pred_coef_table[state->mc_pred_mode][state->mc_dyn_cross_mode[i]];
        for (j = 0; j < val; j++) {
          PutBitsDst(stream, state->mc_predsi[i][j], 2);
        }
      }
    }
  }
}

/*******************************************************************/


Ipp32s mp3enc_mc_trans_channel(MP3Enc_com *state, Ipp32s sbgr, Ipp32s ch)
{
  Ipp32s tca = state->mc_tc_alloc[sbgr];
#if 1
  Ipp32s res;

  switch ( state->mc_channel_conf ) {
  case 320 :
     res = mp3enc_trans_channel_5[ch][tca]; break;
  case 310 :
     res = mp3enc_trans_channel_4a[ch][tca]; break;
  case 220 :
     res = mp3enc_trans_channel_4b[ch][tca]; break;
  case 300 :
  case 302 :
  case 210 :
     res = mp3enc_trans_channel_3[ch][tca]; break;
  default :
     res = ch;
  }
  return res;
#else
  if (state->mc_channel_conf == 320)
      return mp3enc_trans_channel_5[ch][tca];
  else if (state->mc_channel_conf == 310)
    return mp3enc_trans_channel_4a[ch][tca];
  else if (state->mc_channel_conf == 220)
    return mp3enc_trans_channel_4b[ch][tca];
  else if (state->mc_channel_conf == 300 ||
    state->mc_channel_conf == 302 || state->mc_channel_conf == 210)
    return mp3enc_trans_channel_3[ch][tca];
  else
    return ch;
#endif
}

/*******************************************************************/

static Ipp32s mp3enc_mc_dyn_channel (MP3Enc_com *state, Ipp32s sbgr, Ipp32s ch)
{
  Ipp32s dyn_cross = state->mc_dyn_cross_mode[sbgr];
  Ipp32s dyn_2stereo = state->mc_dyn_second_stereo[sbgr];

  if (state->mc_channel_conf == 320)
      return mp3enc_dyn_channel_4[ch-2][dyn_cross];
  else if (state->mc_channel_conf == 310 ||
    state->mc_channel_conf == 220)
    return mp3enc_dyn_channel_3[ch-2][dyn_cross];
  else if (state->mc_channel_conf == 300 ||
    state->mc_channel_conf == 302 ||
    state->mc_channel_conf == 210)
  {
    if (state->mc_channel_conf == 302 && dyn_2stereo && ch == 4)
      return 0;
    else
      return mp3enc_dyn_channel_1[ch-2][dyn_cross];
  }
  else if (state->mc_channel_conf == 202 && dyn_2stereo && ch == 3)
    return 0;
  else if (state->mc_channel_conf == 102 && dyn_2stereo && ch == 2)
    return 0;
  else
    return 1;
}

/*******************************************************************/

static void mp3enc_mc_encode_bit_alloc(MP3Enc_com *state,
                                       sBitsreamBuffer *stream)
{
  Ipp32s i, k, l, m, n;
  Ipp32s stereo      = state->stereo;
  Ipp32s stereomc    = state->mc_channel;
  Ipp32s sblimit     = state->mc_sblimit;
  Ipp16s (*allocation)[32] = state->allocation;
  Ipp32s *nbal_alloc_table = state->nbal_alloc_table;
  Ipp32s center = state->mc_header.center;
  Ipp16u (*scfsi)[32] = state->scfsi;

  if (state->mc_header.lfe)
  {
    PutBitsDst(stream, state->mc_lfe_alloc, 4);
  }

  for (i = 0; i < sblimit; i++)
  {
    l = mp3_mc_sb_group[i];
    for (m = stereo; m < stereo + stereomc; ++m)
    {
      k = mp3enc_mc_trans_channel(state, l, m);

      if ((state->mc_header.center != 3) || (i < 12) || (k !=2))
        if (mp3enc_mc_dyn_channel(state, l, m) == 1)
          PutBitsDst(stream, allocation[k][i], nbal_alloc_table[i]);
    }
  }

  for (i = 0; i < sblimit; i++)
  {
    n = mp3_mc_sb_group[i];
    for (m = stereo; m < stereo + stereomc; m++)
    {
      k = mp3enc_mc_trans_channel(state, n, m);

      if (allocation[k][i] && (i < 12 || m != 2 || center != 3))
        PutBitsDst(stream, scfsi[k][i], 2);
    }
  }
}

/*******************************************************************/

void mp3enc_mc_encode_scale(MP3Enc_com *state,
                      sBitsreamBuffer *stream)
{
  Ipp32s stereo  = state->stereo;
  Ipp32s stereomc    = state->mc_channel;
  Ipp32s sblimit;
  Ipp32s i, j, k, m, n;
//  Ipp32s pci, pred;
  Ipp32s center = state->mc_header.center;
  Ipp32s lfe = state->mc_header.lfe;
  Ipp16s (*allocation)[32] = state->allocation;
  Ipp16u (*scfsi)[32] = state->scfsi;
  Ipp16s (*scalefactor)[3][32] = state->scalefactor;

  sblimit = state->mc_sblimit;

/*
  pred = 0;
  if (start == stereo)
  {
    if (state->mc_prediction_on == 1)
    {
      for (i = 0; i < 8; i++)
      {
        if (state->mc_prediction[i] == 1)
        {
          Ipp32s val;
          val = mp3_mc_pred_coef_table[state->mc_pred_mode][state->mc_dyn_cross_mode[i]];
          for (m = 0; m < val; m++)
          {
            if (state->mc_predsi[i][m] != 0)
            {
              PutBitsDst(stream, state->mc_delay_comp[i][m], 3);
              pred += 3;
              for (pci = 0; pci < state->mc_predsi[i][m]; pci++)
              {
                PutBitsDst(stream, state->mc_pred_coeff[i][m][pci], 8);
                pred += 8;
              }
            }
          }
        }
      }
    }
  }*/

  if (/*start == stereo && */
    lfe && state->mc_lfe_alloc)
    PutBitsDst(stream, state->mc_lfe_scf, 6);

  for (i = 0; i < sblimit; i++)
  {
    n = mp3_mc_sb_group[i];
    for (m = stereo; m < stereo + stereomc; m++)
    {
      k = mp3enc_mc_trans_channel (state, n, m);

      if (allocation[k][i] && (i < 12 || m != 2 || center != 3)) {
        switch (scfsi[k][i])       {
        case 0:
          for (j = 0; j < 3; j++)
            PutBitsDst (stream, scalefactor[k][j][i], 6);
          break;
        case 1:
        case 3:
          PutBitsDst (stream, scalefactor[k][0][i], 6);
          PutBitsDst (stream, scalefactor[k][2][i], 6);
          break;
        case 2:
          PutBitsDst (stream, scalefactor[k][0][i], 6);
        }
      }
    }
  }
}

/*******************************************************************/

static void mp3enc_mc_sample_encoding_l2(MP3Enc_com *state,
                            sBitsreamBuffer *stream)
{
  Ipp32s i, j, k, s, l, m;
  Ipp32s center      = state->mc_header.center;
  Ipp32s stereo      = state->stereo;
  Ipp32s stereomc    = state->mc_channel;
  Ipp32s sblimit     = state->mc_sblimit;
  Ipp32s lfe  =     state->mc_header.lfe;
  Ipp8u  *alloc_table = state->alloc_table;
  Ipp32s (*sample)[32][36] = state->sample;
  Ipp16s (*allocation)[32] = state->allocation;

  for (s = 0; s < 3; s++)
    for (j = 0; j < 4; j ++)
    {
      if (lfe)
        PutBitsDst(stream, state->mc_lfe_spl[s*4+j], state->mc_lfe_alloc + 1);

      for (i = 0; i < sblimit; i++)
      {
        l = mp3_mc_sb_group[i];
        for (m = stereo; m < stereo + stereomc; m++)
        {
          k = mp3enc_mc_trans_channel(state, l, m);

          if (allocation[k][i] && (i < 12 || m != 2 || center != 3))
          {
            Ipp32s idx = alloc_table[i * 16 + state->allocation[m][i]];
            Ipp32s c;
            Ipp32s gr = s*4+j;

            idx = mp3_cls_quant[idx];

            if (idx < 0) {
              idx = - idx;

              if(idx == 5) {
                c = sample[m][i][3 * gr + 0] +
                  sample[m][i][3 * gr + 1] * 3 +
                  sample[m][i][3 * gr + 2] * 9;
              }
              else if(idx == 7) {
                c = sample[m][i][3 * gr + 0] +
                  sample[m][i][3 * gr + 1] * 5 +
                  sample[m][i][3 * gr + 2] * 25;
              }
              else {
                c = sample[m][i][3 * gr + 0] +
                  sample[m][i][3 * gr + 1] * 9 +
                  sample[m][i][3 * gr + 2] * 81;
              }

              PutBits(c, idx);
            } else {
              PutBits(sample[m][i][3 * gr + 0], idx);
              PutBits(sample[m][i][3 * gr + 1], idx);
              PutBits(sample[m][i][3 * gr + 2], idx);
            }
          }
        }
      }
    }
}

/*******************************************************************/

static Ipp32s mp3enc_encodeSideInfo_m1l3(MP3Enc_com *state) {
  Ipp32s i, gr, ch;
  Ipp32s stereo = state->stereo;

  mp3enc_encodeHeader(state, &state->sideStream);

// side info
    PutBitsSI(state->si_main_data_begin, 9);

    if (stereo == 2)
      PutBitsSI(state->si_private_bits, 3)
    else
     PutBitsSI(state->si_private_bits, 5);

    for (ch = 0; ch < stereo; ch++)
      for (i = 0; i < 4; i++)
        PutBitsSI(state->si_scfsi[ch][i], 1);

    for (gr = 0; gr < 2; gr++)
      for (ch = 0; ch < stereo; ch++) {
        PutBitsSI(state->si_part23Len[gr][ch], 12);
        PutBitsSI(state->si_bigVals[gr][ch], 9);
        PutBitsSI(state->si_globGain[gr][ch], 8);
        PutBitsSI(state->si_sfCompress[gr][ch], 4);
        PutBitsSI(state->si_winSwitch[gr][ch], 1);

        if (state->si_winSwitch[gr][ch]) {
          PutBitsSI(state->si_blockType[gr][ch], 2);
          PutBitsSI(state->si_mixedBlock[gr][ch], 1);

          for (i = 0; i < 2; i++)
            PutBitsSI(state->si_pTableSelect[gr][ch][i], 5);
          for (i = 0; i < 3; i++)
            PutBitsSI(state->si_pSubBlkGain[gr][ch][i], 3);
        } else {
          for (i = 0; i < 3; i++)
            PutBitsSI(state->si_pTableSelect[gr][ch][i], 5);

          PutBitsSI(state->si_reg0Cnt[gr][ch], 4);
          PutBitsSI(state->si_reg1Cnt[gr][ch], 3);
        }

        PutBitsSI(state->si_preFlag[gr][ch], 1);
        PutBitsSI(state->si_sfScale[gr][ch], 1);
        PutBitsSI(state->si_cnt1TabSel[gr][ch], 1);
      }

    return 0;
}

static Ipp32s mp3enc_encodeSideInfo_m2l3(MP3Enc_com *state) {
    Ipp32s i, gr = 0, ch;
    Ipp32s stereo = state->stereo;

    mp3enc_encodeHeader(state, &state->sideStream);

    PutBitsSI(state->si_main_data_begin, 8);

    if (stereo == 2)
      PutBitsSI(state->si_private_bits, 2)
    else
      PutBitsSI(state->si_private_bits, 1);

    for (ch = 0; ch < stereo; ch++) {
      PutBitsSI(state->si_part23Len[gr][ch], 12);
      PutBitsSI(state->si_bigVals[gr][ch], 9);
      PutBitsSI(state->si_globGain[gr][ch], 8);
      PutBitsSI(state->si_sfCompress[gr][ch], 9);
      PutBitsSI(state->si_winSwitch[gr][ch], 1);

      if (state->si_winSwitch[gr][ch]) {
        PutBitsSI(state->si_blockType[gr][ch], 2);
        PutBitsSI(state->si_mixedBlock[gr][ch], 1);

        for (i = 0; i < 2; i++)
          PutBitsSI(state->si_pTableSelect[gr][ch][i], 5);
        for (i = 0; i < 3; i++)
          PutBitsSI(state->si_pSubBlkGain[gr][ch][i], 3);
      } else {
        for (i = 0; i < 3; i++)
          PutBitsSI(state->si_pTableSelect[gr][ch][i], 5);

        PutBitsSI(state->si_reg0Cnt[gr][ch], 4);
        PutBitsSI(state->si_reg1Cnt[gr][ch], 3);
      }

      PutBitsSI(state->si_sfScale[gr][ch], 1);
      PutBitsSI(state->si_cnt1TabSel[gr][ch], 1);
    }

    return 0;
}

/*******************************************************************/

static void mp3enc_encodeMainData_l1(MP3Enc_com *state, Ipp32s *nbits)
{
  Ipp32s ch, sb, s, nb;
  Ipp32s stereo = state->stereo;

  for(sb = 0; sb < 32; sb++)
    for(ch = 0; ch < stereo; ch++)
      PutBits(state->allocation[ch][sb], 4);

  GET_BITS_COUNT(&state->mainStream, nb)
  *nbits = nb;

  for(sb = 0; sb < 32; sb++)
    for(ch = 0; ch < stereo; ch++)
      if (state->allocation[ch][sb] != 0)
        PutBits(state->scalefactor_l1[ch][sb],6);

  for(s = 0; s < 12; s++)
    for(sb = 0; sb < 32; sb++)
      for(ch = 0; ch < stereo; ch++)
        if (state->allocation[ch][sb] != 0)
          PutBits(state->sample[ch][sb][s], state->allocation[ch][sb] + 1);
}

static void mp3enc_encodeMainData_l2(MP3Enc_com *state, Ipp32s *nbits) {
  Ipp32s sb, ch, gr, nb;
  Ipp32s sblimit = state->sblimit;
  Ipp32s jsbound = state->jsbound;
  Ipp32s stereo = state->stereo;

  Ipp32s *nbal_alloc_table = state->nbal_alloc_table;
  Ipp8u  *alloc_table = state->alloc_table;

  for (sb = 0; sb < jsbound; sb++) {
    for (ch = 0; ch < stereo; ch++) {
      PutBits(state->allocation[ch][sb], nbal_alloc_table[sb]);
    }
  }

  for (sb = jsbound; sb < sblimit; sb++) {
    PutBits(state->allocation[0][sb], nbal_alloc_table[sb]);
  }

  for (sb = 0; sb < sblimit; sb++) {
    for (ch = 0; ch < stereo; ch++) {
      if (state->allocation[ch][sb]) {
        PutBits(state->scfsi[ch][sb], 2);
      }
    }
  }

  GET_BITS_COUNT(&state->mainStream, nb)
  *nbits = nb;

  for (sb = 0; sb < sblimit; sb++) {
    for (ch = 0; ch < stereo; ch++) {
      if (state->allocation[ch][sb] != 0) {
        if (state->scfsi[ch][sb] == 0) {
          PutBits(state->scalefactor[ch][0][sb], 6);
          PutBits(state->scalefactor[ch][1][sb], 6);
          PutBits(state->scalefactor[ch][2][sb], 6);
        } else if ((state->scfsi[ch][sb] == 1)) {
          PutBits(state->scalefactor[ch][0][sb], 6);
          PutBits(state->scalefactor[ch][2][sb], 6);
        } else if (state->scfsi[ch][sb] == 2) {
          PutBits(state->scalefactor[ch][0][sb], 6);
        } else if (state->scfsi[ch][sb] == 3) {
          PutBits(state->scalefactor[ch][0][sb], 6);
          PutBits(state->scalefactor[ch][1][sb], 6);
        }
      }
    }
  }

  for (gr = 0; gr < 12; gr++) {
    for (sb = 0; sb < sblimit; sb++) {
      for (ch = 0; ch < ((sb < jsbound) ? stereo : 1); ch++) {
        if (state->allocation[ch][sb] != 0) {
          Ipp32s idx = alloc_table[sb * 16 + state->allocation[ch][sb]];
          Ipp32s c;

          idx = mp3_cls_quant[idx];

          if (idx < 0) {
            idx = - idx;

            if(idx == 5){
              c = state->sample[ch][sb][3 * gr + 0] +
                state->sample[ch][sb][3 * gr + 1] * 3 +
                state->sample[ch][sb][3 * gr + 2] * 9;
            }
            else if(idx == 7){
              c = state->sample[ch][sb][3 * gr + 0] +
                state->sample[ch][sb][3 * gr + 1] * 5 +
                state->sample[ch][sb][3 * gr + 2] * 25;
            }
            else {
              c = state->sample[ch][sb][3 * gr + 0] +
                state->sample[ch][sb][3 * gr + 1] * 9 +
                state->sample[ch][sb][3 * gr + 2] * 81;
            }

            PutBits(c, idx);
          } else {
            PutBits(state->sample[ch][sb][3 * gr + 0], idx);
            PutBits(state->sample[ch][sb][3 * gr + 1], idx);
            PutBits(state->sample[ch][sb][3 * gr + 2], idx);
          }
        }
      }
    }
  }
}

/*******************************************************************/

static void mp3enc_encodeMainData_l3(MP3Enc_com *state)
{
    Ipp32s i, gr, ch, sfb, window;

    if (state->header.id) {
      for (gr = 0; gr < 2; gr++) {
        for (ch = 0; ch < state->stereo; ch++) {
          Ipp16s slen1 = (Ipp16s)mp3enc_slen1_tab[state->si_sfCompress[gr][ch]];
          Ipp16s slen2 = (Ipp16s)mp3enc_slen2_tab[state->si_sfCompress[gr][ch]];

          if ((state->si_winSwitch[gr][ch] == 1) && (state->si_blockType[gr][ch] == 2)) {
            if (state->si_mixedBlock[gr][ch]) {
              for (sfb = 0; sfb < 8; sfb++)
                PutBits(state->scalefac_l[gr][ch][sfb], slen1);

              for (sfb = 3; sfb < 6; sfb++)
                for (window = 0; window < 3; window++)
                  PutBits(state->scalefac_s[gr][ch][sfb][window], slen1);

              for (sfb = 6; sfb < 12; sfb++)
                for (window = 0; window < 3; window++)
                  PutBits(state->scalefac_s[gr][ch][sfb][window], slen2);

            } else {
              for (sfb = 0; sfb < 6; sfb++)
                for (window = 0; window < 3; window++)
                  PutBits(state->scalefac_s[gr][ch][sfb][window], slen1);

              for (sfb = 6; sfb < 12; sfb++)
                for (window = 0; window < 3; window++)
                  PutBits(state->scalefac_s[gr][ch][sfb][window], slen2);
            }
          } else {
            if ((gr == 0) || (state->si_scfsi[ch][0] == 0))
              for (sfb = 0; sfb < 6; sfb++)
                PutBits(state->scalefac_l[gr][ch][sfb], slen1);

            if ((gr == 0) || (state->si_scfsi[ch][1] == 0))
              for (sfb = 6; sfb < 11; sfb++)
                PutBits(state->scalefac_l[gr][ch][sfb], slen1);

            if ((gr == 0) || (state->si_scfsi[ch][2] == 0))
              for (sfb = 11; sfb < 16; sfb++)
                PutBits(state->scalefac_l[gr][ch][sfb], slen2);

            if ((gr == 0) || (state->si_scfsi[ch][3] == 0))
              for (sfb = 16; sfb < 21; sfb++)
                PutBits(state->scalefac_l[gr][ch][sfb], slen2);
          }

          mp3enc_huffmanCodeBits(state, gr, ch);

        }
      }
    } else {
      gr = 0;
      for (ch = 0; ch < state->stereo; ch++) {
        Ipp32s sfb_part;
        Ipp32s s = 0;
        if ((state->si_winSwitch[gr][ch] == 1) && (state->si_blockType[gr][ch] == 2)) {
          if (state->si_mixedBlock[gr][ch]) {
            sfb_part = 0;
            for (sfb = 0; sfb < 6; sfb++)
              PutBits(state->scalefac_l[gr][ch][sfb], state->slen[ch][sfb_part] );

            sfb = 3;
            for (sfb_part = 1; sfb_part < 4; sfb_part++) {
              Ipp32s sfbs = state->sfb_part_table[ch][sfb_part] / 3;
              Ipp32s slen = state->slen[ch][sfb_part];
              for (i = 0; i < sfbs; i++, sfb++)
                for (window = 0; window < 3; window++)
                  PutBits(state->scalefac_s[gr][ch][sfb][window], slen );
            }
          } else {
            sfb = 0;
            for (sfb_part = 0; sfb_part < 4; sfb_part++ ) {
              Ipp32s sfbs = state->sfb_part_table[ch][sfb_part] / 3;
              Ipp32s slen = state->slen[ch][sfb_part];
              for (i = 0; i < sfbs; i++, sfb++)
                for (window = 0; window < 3; window++)
                  PutBits(state->scalefac_s[gr][ch][sfb][window], slen );
            }
          }
        } else {
          sfb = 0;
          for (sfb_part = 0; sfb_part < 4; sfb_part++ ) {
            Ipp32s sfbs = state->sfb_part_table[ch][sfb_part];
            Ipp32s slen = state->slen[ch][sfb_part];
            for (i = 0; i < sfbs; i++, sfb++) {
              PutBits(state->scalefac_l[gr][ch][sfb], slen );
              s += slen;
            }
          }
        }
        mp3enc_huffmanCodeBits(state, gr, ch);
      }
    }
}

/*******************************************************************/

static Ipp32s mp3enc_writeFrame(MP3Enc_com *state,
                                Ipp32s si_bits,
                                Ipp32s bits,
                                Ipp8u *pOutputData)
{
    Ipp32s i, slot, res, len;
    Ipp8u *ptr_main = (Ipp8u *)state->buffer_main_data;
    Ipp8u *ptr_side = (Ipp8u *)state->buffer_side_info;
    Ipp8u *ptr_out = pOutputData;
    Ipp32s si_bytes, bytes;

    Ipp32s resr_bytes;
    Ipp32s resr_mod_slot;
    Ipp32s si_beg, si_new, si_num;
    Ipp32s slot_size;
    Ipp32u si_main_data_begin;
    Ipp8u (*si_buf)[40];
    Ipp32s maxbits;

    if (state->header.id)
      maxbits = 511;
    else
      maxbits = 255;

    resr_bytes = state->resr_bytes;
    resr_mod_slot = state->resr_mod_slot;
    si_beg = state->si_beg;
    si_new = state->si_new;
    si_num = state->si_num;
    slot_size = state->slot_size;
    si_main_data_begin = state->si_main_data_begin;
    si_buf = state->si_buf;

    slot = slot_size;
    bits = (bits + 7) & ~7;
    si_bytes = si_bits >> 3;
    bytes = bits >> 3;
    res = 0;

    if (state->br_mode == MPAENC_ABR) {
      len = (Ipp32s)si_main_data_begin < bytes ? (Ipp32s)si_main_data_begin : bytes;
      for (i = 0; i < len; i++)
        *ptr_out++ = *ptr_main++;
      res += len;
      bytes -= len;
      if ((Ipp32s)si_main_data_begin > len) {
        for (i = 0; i < (Ipp32s)si_main_data_begin - len; i++)
          *ptr_out++ = 0;
         res += si_main_data_begin - len;
      }

      for (i = 0; i < si_bytes; i++)
        *ptr_out++ = *ptr_side++;
      res += si_bytes;

      for (i = 0; i < bytes; i++)
        *ptr_out++ = *ptr_main++;
      res += bytes;

      len = state->slot_sizes[state->header.bitRate] - si_bytes - bytes;
      if (len > maxbits) {
        for (i = 0; i < len - maxbits; i++)
          *ptr_out++ = 0;
        res += len - maxbits;
        len = maxbits;
      }
      si_main_data_begin = len;

    } else {
      resr_bytes += slot;

      for (i = 0; i < si_bytes; i++)
          state->si_buf[si_new][i] = *ptr_side++;

      si_num++;
      si_new++;

      if (si_new >= SI_MAX)
          si_new = 0;

      do {
          Ipp32s left = resr_mod_slot;

          len = left < bytes ? left : bytes;

          for (i = 0; i < len; i++)
              *ptr_out++ = *ptr_main++;

          res += len;
          bytes -= len;
          resr_bytes -= len;
          resr_mod_slot -= len;
          if (resr_mod_slot < 0) // It is impossible to change! There will be an error!
              resr_mod_slot += slot;

          if (len == left && si_num > 0) {
              for (i = 0; i < si_bytes; i++)
                  *ptr_out++ =
                      si_buf[si_beg][i];
              res += si_bytes;
              resr_bytes -= si_bytes;
              resr_mod_slot -= si_bytes;
              if (resr_mod_slot < 0) // It is impossible to change! There will be an error!
                  resr_mod_slot += slot;
              si_num--;
              si_beg++;
              if (si_beg >= SI_MAX)
                  si_beg = 0;
          }
      } while(bytes);

      while ((si_main_data_begin = resr_bytes  - si_num * si_bytes) > (Ipp32u)maxbits) {
          Ipp32s left = resr_mod_slot;

          len = left < ((Ipp32s)si_main_data_begin - maxbits) ?
              left : ((Ipp32s)si_main_data_begin - maxbits);

          for (i = 0; i < len; i++)
              *ptr_out++ = 0;

          res += len;
          resr_bytes -= len;
          resr_mod_slot -= len;
          if (resr_mod_slot < 0)
              resr_mod_slot += slot;

          if (len == left && si_num > 0) {
              for (i = 0; i < si_bytes; i++)
                  *ptr_out++ = si_buf[si_beg][i];

              res += si_bytes;
              resr_bytes -= si_bytes;
              resr_mod_slot -= si_bytes;
              if (resr_mod_slot < 0)
                  resr_mod_slot += slot;
              si_num--;
              si_beg++;
              if (si_beg >= SI_MAX)
                  si_beg = 0;
          }
      }
  /*
      if (bitrate[header.bitRate] == 320) {
          if (si_main_data_begin > 0) {
              for (i = 0; i < si_main_data_begin; i++)
                  *ptr_out++ = 0;
              res += si_main_data_begin;
              resr_bytes -= si_main_data_begin;
          }
          si_main_data_begin = 0;
      }
  */
      state->resr_bytes = resr_bytes;
      state->resr_mod_slot = resr_mod_slot;
      state->si_beg = si_beg;
      state->si_new = si_new;
      state->si_num = si_num;
      state->slot_size = slot_size;
    }
    state->si_main_data_begin = si_main_data_begin;

    return res;
}

/*******************************************************************/

Ipp32s mp3enc_formatBitstream_l12(MP3Enc_com *state, Ipp8u *pOutputData)
{
  Ipp32s bits, bytes;
  Ipp32s layer = state->header.layer;
  Ipp32u crc, crc_mc = 0;
  Ipp32s crc_nbits, crc_mc_nbits;
  Ipp32u *ptr, *ptr_mc_crc = 0;
  Ipp32s offset, offset_mc_crc = 0;

  INIT_BITSTREAM(&state->mainStream, pOutputData)

  mp3enc_encodeHeader(state, &state->mainStream);

  if (layer == 1)
    mp3enc_encodeMainData_l1(state, &crc_nbits);
  else
    mp3enc_encodeMainData_l2(state, &crc_nbits);

  crc_nbits -= 48;

/* encode multichannel data */
  if (state->mc_channel/* || state->mc_header.lfe*/) {
    ptr = state->mainStream.pCurrent_dword;
    offset = state->mainStream.nBit_offset;
    mp3enc_mc_encodeHeader(state, &state->mainStream, &crc_mc_nbits);
    SAVE_BITSTREAM(&state->mainStream)
    bs_CRC_reset(&crc_mc);
    bs_CRC_update(ptr, offset, crc_mc_nbits, &crc_mc);

    ptr_mc_crc = state->mainStream.pCurrent_dword;
    offset_mc_crc = state->mainStream.nBit_offset;
    PutBits(crc_mc, 16);

    ptr = state->mainStream.pCurrent_dword;
    offset = state->mainStream.nBit_offset;

    GET_BITS_COUNT(&state->mainStream, bits)

    mp3enc_mc_encode_composite_status_info(state, &state->mainStream);

    mp3enc_mc_encode_bit_alloc(state, &state->mainStream);

    GET_BITS_COUNT(&state->mainStream, crc_mc_nbits)
    crc_mc_nbits -= bits;
    SAVE_BITSTREAM(&state->mainStream)
    bs_CRC_update(ptr, offset, crc_mc_nbits, &crc_mc);

    mp3enc_mc_encode_scale(state, &state->mainStream);

    mp3enc_mc_sample_encoding_l2(state, &state->mainStream);
  }

  GET_BITS_COUNT(&state->mainStream, bits)
  SAVE_BITSTREAM(&state->mainStream)

  if (state->mc_channel) {
    INIT_BITSTREAM_OFFSET(&state->mainStream, ptr_mc_crc, offset_mc_crc)
    PutBits(crc_mc, 16);
    SAVE_BITSTREAM(&state->mainStream)
  }

  bytes = ((bits + 7) >> 3);

  if (bytes < state->slot_size)
    ippsZero_8u(pOutputData + bytes, state->slot_size - bytes);

  if (state->header.protectionBit) {
    bs_CRC_reset(&crc);
    bs_CRC_update_ptr(pOutputData + 2, 16, &crc);
    bs_CRC_update_ptr(pOutputData + 6, crc_nbits, &crc);

    INIT_BITSTREAM(&state->mainStream, pOutputData+4)
    PutBitsDst((&state->mainStream), crc, 16);
    SAVE_BITSTREAM(&state->mainStream)
  }

  return state->slot_size;
}

/*******************************************************************/

Ipp32s mp3enc_formatBitstream_l3(MP3Enc_com *state, Ipp32s (*mdct_out)[2][LEN_MDCT_LINE],
                                 Ipp8u *pOutputData)
{
    Ipp32s gr, ch, i, bits, si_bits;
    Ipp32s bytes;
    Ipp32u crc;
    Ipp32s crc_nbits;

    IPP_ALIGNED_ARRAY(32, Ipp32s, sign, LEN_MDCT_LINE);
    for (gr = 0; gr < state->grnum; gr++) {
      for (ch = 0; ch < state->stereo; ch++) {
        ippsRShiftC_32s(mdct_out[gr][ch], 31, sign, LEN_MDCT_LINE);
        for (i = 0; i < LEN_MDCT_LINE; i++)
          state->quant_ix[gr][ch][i] -= (state->quant_ix[gr][ch][i] << 1) & (Ipp16s)sign[i] ;
      }
    }

// si_bits - count of bits for header and sideinfo
// bits - count of bits for main data
    INIT_BITSTREAM(&state->mainStream, state->buffer_main_data)
    INIT_BITSTREAM(&state->sideStream, state->buffer_side_info)

    if (state->header.id)
      mp3enc_encodeSideInfo_m1l3(state);
    else
      mp3enc_encodeSideInfo_m2l3(state);

    GET_BITS_COUNT(&state->sideStream, si_bits)
    SAVE_BITSTREAM(&state->sideStream)

    state->si_bits = si_bits;

    if (state->header.protectionBit) {
      if (state->header.id == 1) {
        if (state->header.mode == 0x3)
          crc_nbits = 136;
        else
          crc_nbits = 256;
      } else {
        if (state->header.mode == 0x3)
          crc_nbits = 72;
        else
          crc_nbits = 136;
      }
      bs_CRC_reset(&crc);
      bs_CRC_update_ptr((Ipp8u*)state->buffer_side_info + 2, 16, &crc);
      bs_CRC_update_ptr((Ipp8u*)state->buffer_side_info + 6, crc_nbits, &crc);
      INIT_BITSTREAM(&state->sideStream, (Ipp8u*)state->buffer_side_info+4)
      PutBitsDst(&state->sideStream, crc, 16);
      SAVE_BITSTREAM(&state->sideStream)
    }

    mp3enc_encodeMainData_l3(state);
    GET_BITS_COUNT(&state->mainStream, bits)
    SAVE_BITSTREAM(&state->mainStream)

    bytes = mp3enc_writeFrame(state, si_bits, bits, pOutputData);

    return bytes;
}

/*******************************************************************/

Ipp32s mp3enc_writeLastFrames(MP3Enc_com *state,
                              Ipp8u *pOutputData)
{
  Ipp8u *ptr = pOutputData;
  Ipp32s res;

  if (state->br_mode == MPAENC_ABR) {
    if (state->si_main_data_begin <= 0) {
      return -1;
    }
    ippsZero_8u(ptr, state->si_main_data_begin);
    res = state->si_main_data_begin;
    state->si_main_data_begin = 0;
  } else {
    if ((state->si_num <= 0) &&  (state->resr_mod_slot <= 0)) {
      return -1;
    }

    ippsZero_8u(ptr, state->resr_mod_slot);
    ptr += state->resr_mod_slot;
    res = state->resr_mod_slot;
    state->resr_mod_slot = 0;

    if (state->si_num > 0) {
      Ipp32s si_bytes = state->si_bits >> 3;

      ippsCopy_8u(&(state->si_buf[state->si_beg][0]), ptr, si_bytes);
      ptr += si_bytes;
      ippsZero_8u(ptr, state->slot_size - si_bytes);
      res += state->slot_size;

      state->si_num--;
      state->si_beg++;
      if (state->si_beg >= SI_MAX) {
        state->si_beg = 0;
      }
    }
  }

  return res;
}

/*******************************************************************/

#endif //UMC_ENABLE_XXX
