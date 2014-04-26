/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_MP3_AUDIO_DECODER)

#include "mp3dec_own.h"
#include "ipps.h"

Ipp32s mp3dec_audio_data_LayerII(MP3Dec_com *state)
{
    Ipp16s (*allocation)[32] = state->allocation;
    Ipp32s (*sample)[32][36] = state->sample;
    Ipp16s (*scfsi)[32] = state->scfsi;
    Ipp16s (*scalefactor)[3][32] = state->scalefactor;
    const Ipp16s* degroup_table;
    const Ipp8u* alloc_table = state->alloc_table;
    const Ipp32s* nbal_alloc_table = state->nbal_alloc_table;
    sBitsreamBuffer *m_StreamData = &(state->m_StreamData);
    Ipp32s gr, ch, sb;
    Ipp32s jsbound = state->jsbound;
    Ipp32s stereo = state->stereo;
    Ipp16s val;
    Ipp32s sblimit = state->sblimit;

    if (state->header.protectionBit)
      mp3dec_CRC_start(state);

    for (sb = 0; sb < jsbound; sb++) {
        for (ch = 0; ch < stereo; ch++) {
            GET_BITS(m_StreamData, allocation[ch][sb], nbal_alloc_table[sb], Ipp16s);
            state->crc_nbits += nbal_alloc_table[sb];
        }
    }

    for (sb = jsbound; sb < sblimit; sb++) {
        GET_BITS(m_StreamData, allocation[0][sb], nbal_alloc_table[sb], Ipp16s);
        allocation[1][sb] = allocation[0][sb];
        state->crc_nbits += nbal_alloc_table[sb];
    }

    for (sb = 0; sb < sblimit; sb++) {
        for (ch = 0; ch < stereo; ch++) {
            if (allocation[ch][sb]) {
                GET_BITS(m_StreamData, scfsi[ch][sb], 2, Ipp16s);
                state->crc_nbits += 2;
            }
        }
    }

    if (state->header.protectionBit) {
      mp3dec_CRC_update(state, &state->crc);
      if (state->crc != state->crc_check)
        return 1;
    }

    for (sb = 0; sb < sblimit; sb++) {
        for (ch = 0; ch < stereo; ch++) {
            if (allocation[ch][sb] != 0) {
                if (scfsi[ch][sb] == 0) {
                    GET_BITS(m_StreamData, scalefactor[ch][0][sb], 6, Ipp16s);
                    GET_BITS(m_StreamData, scalefactor[ch][1][sb], 6, Ipp16s);
                    GET_BITS(m_StreamData, scalefactor[ch][2][sb], 6, Ipp16s);
                } else if ((scfsi[ch][sb] == 1)) {
                    GET_BITS(m_StreamData, scalefactor[ch][0][sb], 6, Ipp16s);
                    scalefactor[ch][1][sb] = scalefactor[ch][0][sb];
                    GET_BITS(m_StreamData, scalefactor[ch][2][sb], 6, Ipp16s);
                } else if (scfsi[ch][sb] == 2) {
                    GET_BITS(m_StreamData, scalefactor[ch][2][sb], 6, Ipp16s);
                    scalefactor[ch][0][sb] =
                        scalefactor[ch][1][sb] =
                        scalefactor[ch][2][sb];
                } else if (scfsi[ch][sb] == 3) {
                    GET_BITS(m_StreamData, scalefactor[ch][0][sb], 6, Ipp16s);
                    GET_BITS(m_StreamData, scalefactor[ch][2][sb], 6, Ipp16s);
                    scalefactor[ch][1][sb] =
                        scalefactor[ch][2][sb];
                }
            } else {
                scalefactor[ch][0][sb] =
                    scalefactor[ch][1][sb] =
                    scalefactor[ch][2][sb] = 63;
            }
        }
    }

    for (gr = 0; gr < 12; gr++) {
        for (sb = 0; sb < sblimit; sb++) {
            for (ch = 0; ch < ((sb < jsbound) ? stereo : 1); ch++) {
                if (allocation[ch][sb] != 0) {
                    Ipp32s idx = alloc_table[sb * 16 + allocation[ch][sb]];
                    Ipp32s c;

                    idx = mp3_cls_quant[idx];
                    if (idx < 0) {
                        idx = - idx;
                        degroup_table = mp3_degroup[idx];
                        GET_BITS(m_StreamData, c, idx, Ipp32s);
                        val = degroup_table[c];
                        sample[ch][sb][3 * gr + 0] = val >> 8;
                        sample[ch][sb][3 * gr + 1] = (val >> 4) & 0xF;
                        sample[ch][sb][3 * gr + 2] = val & 0xF;
                    } else {
                        GET_BITS(m_StreamData, sample[ch][sb][3 * gr + 0], idx, Ipp32s);
                        GET_BITS(m_StreamData, sample[ch][sb][3 * gr + 1], idx, Ipp32s);
                        GET_BITS(m_StreamData, sample[ch][sb][3 * gr + 2], idx, Ipp32s);
                    }

                    // joint stereo : copy L to R
                    if (stereo == 2 && sb >= jsbound) {
                        sample[1][sb][3 * gr + 0] = sample[0][sb][3 * gr + 0];
                        sample[1][sb][3 * gr + 1] = sample[0][sb][3 * gr + 1];
                        sample[1][sb][3 * gr + 2] = sample[0][sb][3 * gr + 2];
                    }
                } else {
                    sample[ch][sb][3 * gr + 0] = 0;
                    sample[ch][sb][3 * gr + 1] = 0;
                    sample[ch][sb][3 * gr + 2] = 0;
                }
            }
        }
    }
    return 0;
}

void mp3dec_mc_audio_data_l2(MP3Dec_com *state)
{
  const Ipp32s *nbal_alloc_table = state->nbal_alloc_table;
  sBitsreamBuffer *m_data = &(state->m_StreamData);
  Ipp16s (*allocation)[32] = state->allocation;
  Ipp16s (*scfsi)[32] = state->scfsi;
  Ipp32s stereo = state->stereo;
  Ipp32s ch_start = stereo;
  Ipp32s ch_end = stereo + state->mc_channel;
  Ipp32s i, j, sb;
  Ipp32s sblimit = state->sblimit;
  Ipp32s nbits = 0;

  if (state->mc_header.lfe) {
    GET_BITS(m_data, state->mc_lfe_alloc, nbal_alloc_table[0], Ipp32s);
    nbits += nbal_alloc_table[0];
  }

  if (state->mc_dyn_cross_on == 0)
    for (i = 0; i < sblimit; i++ )
      for (j = ch_start; j < ch_end; j++) {
        if ((state->mc_header.center != 3) || (i < 12) || (j != 2)) {
          GET_BITS(m_data, allocation[j][i], nbal_alloc_table[i], Ipp16s);
          nbits += nbal_alloc_table[i];
        } else
          allocation[j][i] = 0;
      }
  else
    for (i = 0; i < sblimit; i++) {
      sb = mp3_mc_sb_group[i];

      if (state->mc_dyn_cross_mode[sb] == 0) {
        for (j = ch_start; j < ch_end; j++)
          if (state->mc_header.center == 3 && i >= 12 && j == 2)
            allocation[j][i] = 0;
          else if (state->mc_header.surround == 3 && state->mc_dyn_second_stereo[sb] == 1) {
            if (state->mc_header.center != 0 && j == 4)
              allocation[j][i] = allocation[3][i];
            else if (state->mc_header.center == 0 && j == 3)
              allocation[j][i] = allocation[2][i];
            else {
              GET_BITS(m_data, allocation[j][i], nbal_alloc_table[i], Ipp16s);
              nbits += nbal_alloc_table[i];
            }
          }
          else {
            GET_BITS(m_data, allocation[j][i], nbal_alloc_table[i], Ipp16s);
            nbits += nbal_alloc_table[i];
          }
      }
      else if (state->mc_dyn_cross_bits == 1) {
        if ((state->mc_header.center == 3) && (i >= 12))
          allocation[2][i] = 0;
        else if (state->mc_tc_alloc[sb] == 1)
          allocation[2][i] = allocation[0][i];
        else if (state->mc_tc_alloc[sb] == 2)
          allocation[2][i] = allocation[1][i];
        else if (state->mc_dyn_cross_LR)
          allocation[2][i] = allocation[1][i];
        else
          allocation[2][i] = allocation[0][i];

        if (state->mc_header.surround == 3) {
          GET_BITS(m_data, allocation[3][i], nbal_alloc_table[i], Ipp16s);
          nbits += nbal_alloc_table[i];
          if (state->mc_dyn_second_stereo[sb] == 1)
            allocation[4][i] = allocation[3][i];
          else {
            GET_BITS(m_data, allocation[4][i], nbal_alloc_table[i], Ipp16s);
            nbits += nbal_alloc_table[i];
          }
        }
      }
      else if (state->mc_dyn_cross_bits == 3) {
        if ((state->mc_header.center == 3) && (i >= 12))
          allocation[2][i] = 0;
        else if ((state->mc_dyn_cross_mode[sb] == 1) || (state->mc_dyn_cross_mode[sb] == 4)) {
          GET_BITS(m_data, allocation[2][i], nbal_alloc_table[i], Ipp16s);
          nbits += nbal_alloc_table[i];
        } else {
          if (state->mc_header.surround == 2 ||
            state->mc_tc_alloc[sb] == 1 ||
            state->mc_tc_alloc[sb] == 5 ||
            (state->mc_tc_alloc[sb] != 2 && !state->mc_dyn_cross_LR))
            allocation[2][i] = allocation[0][i];
          else
            allocation[2][i] = allocation[1][i];
        }

        if (state->mc_dyn_cross_mode[sb] == 2) {
          GET_BITS(m_data, allocation[3][i], nbal_alloc_table[i], Ipp16s);
          nbits += nbal_alloc_table[i];
        } else if (state->mc_dyn_cross_mode[sb] == 4)
          allocation[3][i] = allocation[2][i];
        else {
          if (state->mc_header.surround == 2 ||
            state->mc_tc_alloc[sb] == 4 ||
            state->mc_tc_alloc[sb] == 5 ||
            (state->mc_tc_alloc[sb] < 3 && state->mc_dyn_cross_LR))
            allocation[3][i] = allocation[1][i];
          else
            allocation[3][i] = allocation[0][i];
        }
      }
      else if (state->mc_dyn_cross_bits == 4) {
        if ((state->mc_header.center == 3) && (i >= 12))
          allocation[2][i] = 0;
        else switch (state->mc_dyn_cross_mode[sb]) {
           case 1 :
           case 2 :
           case 4 :
           case 8 :
           case 9 :
           case 10:
           case 11:
           case 12:
           case 14:
             GET_BITS(m_data, allocation[2][i], nbal_alloc_table[i], Ipp16s);
             nbits += nbal_alloc_table[i];
             break;
           case 3 :
           case 5 :
           case 6 :
           case 7 :
           case 13:
             if ((state->mc_tc_alloc[sb] == 1) || (state->mc_tc_alloc[sb] == 7))
               allocation[2][i] = allocation[0][i];
             else if ((state->mc_tc_alloc[sb] == 2) || (state->mc_tc_alloc[sb] == 6))
               allocation[2][i] = allocation[1][i];
             else if (state->mc_dyn_cross_LR)
               allocation[2][i] = allocation[1][i];
             else
               allocation[2][i] = allocation[0][i];
             break;
        }

        switch (state->mc_dyn_cross_mode[sb]) {
           case 1 :
           case 3 :
           case 5 :
           case 8 :
           case 10:
           case 13:
             GET_BITS(m_data, allocation[3][i], nbal_alloc_table[i], Ipp16s);
             nbits += nbal_alloc_table[i];
             break;
           case 2 :
           case 4 :
           case 6 :
           case 7 :
           case 12:
             allocation[3][i] = allocation[0][i];
             break;
           case 9 :
           case 11:
           case 14:
             allocation[3][i] = allocation[2][i];
             break;
        }

        switch (state->mc_dyn_cross_mode[sb]) {
           case 2 :
           case 3 :
           case 6 :
           case 9 :
             GET_BITS(m_data, allocation[4][i], nbal_alloc_table[i], Ipp16s);
             nbits += nbal_alloc_table[i];
             break;
           case 1 :
           case 4 :
           case 5 :
           case 7 :
           case 11:
             allocation[4][i] = allocation[1][i];
             break;
           case 10:
           case 12:
           case 14:
             allocation[4][i] = allocation[2][i];
             break;
           case 8 :
           case 13:
             allocation[4][i] = allocation[3][i];
             break;
        }
      }
    }

    for (i = sblimit; i < 32; i++)
      for (j = ch_start; j < ch_end; j++)
        allocation[j][i] = 0;

    for (i = 0; i < sblimit; i++)  {
      for (j = ch_start; j < ch_end; j++)
        if (allocation[j][i]) {
          GET_BITS(m_data, scfsi[j][i], 2, Ipp16s);
          nbits += 2;
        } else
          scfsi[j][i] = 4;
    }

    for (i = sblimit; i < 32; i++)
      for (j = ch_start; j < ch_end; j++)
        scfsi[j][i] = 4;

    state->crc_nbits += nbits;
}

void mp3dec_mc_decode_scale_l2(MP3Dec_com *state)
{
  sBitsreamBuffer *m_data = &(state->m_StreamData);
  Ipp32s stereo = state->stereo;
  Ipp32s ch_start = stereo;
  Ipp32s ch_end = stereo + state->mc_channel;
  Ipp32s i, j, px, pci;
  Ipp32s sblimit = state->sblimit;
  Ipp16s (*allocation)[32] = state->allocation;
  Ipp16s (*scfsi)[32] = state->scfsi;
  Ipp16s (*scalefactor)[3][32] = state->scalefactor;

  if (ch_end > stereo && ch_end < 7) {
    if (state->mc_prediction_on == 1) {
      ippsZero_32s(&state->mc_delay_comp[0][0], 48);
      ippsSet_32s(127, &state->mc_pred_coeff[0][0][0], 48*3);
      for (i = 0; i < 8; i++)
        if (state->mc_prediction[i] == 1)
          for (px = 0; px < mp3_mc_pred_coef_table[state->mc_pred_mode][state->mc_dyn_cross_mode[i]]; px++)
            if (state->mc_predsi[i][px] != 0) {
              GET_BITS(m_data, state->mc_delay_comp[i][px], 3, Ipp32s);
              for (pci = 0; pci < state->mc_predsi[i][px]; pci++) {
                GET_BITS(m_data, state->mc_pred_coeff[i][px][pci], 8, Ipp32s);
              }
            }
            else {
              state->mc_pred_coeff[i][px][0] = 127;
              state->mc_delay_comp[i][px] = 0;
            }
    }
  }

  if (ch_start == stereo)
    if (state->mc_header.lfe)
      if (state->mc_lfe_alloc) {
        GET_BITS(m_data, state->mc_lfe_scf, 6, Ipp32s);
      }

  for (i = 0; i < sblimit; i++)
    for (j = ch_start; j < ch_end; j++)
      if (allocation[j][i])
        switch (scfsi[j][i]) {
          case 0 :
            GET_BITS(m_data, scalefactor[j][0][i], 6, Ipp16s);
            GET_BITS(m_data, scalefactor[j][1][i], 6, Ipp16s);
            GET_BITS(m_data, scalefactor[j][2][i], 6, Ipp16s);
            break;
          case 1 :
            GET_BITS(m_data, scalefactor[j][0][i], 6, Ipp16s);
            scalefactor[j][1][i] = scalefactor[j][0][i];
            GET_BITS(m_data, scalefactor[j][2][i], 6, Ipp16s);
            break;
          case 3 :
            GET_BITS(m_data, scalefactor[j][0][i], 6, Ipp16s);
            GET_BITS(m_data, scalefactor[j][1][i], 6, Ipp16s);
            scalefactor[j][2][i] = scalefactor[j][1][i];
            break;
          case 2 :
            GET_BITS(m_data, scalefactor[j][0][i], 6, Ipp16s);
            scalefactor[j][1][i] = scalefactor[j][2][i] =
              scalefactor[j][0][i];
            break;
          default: scalefactor[j][0][i] =
                     scalefactor[j][1][i] =
                     scalefactor[j][2][i] = 63;
            break;
        }
      else
        scalefactor[j][0][i] = scalefactor[j][1][i] =
        scalefactor[j][2][i] = 63;

  for (i = sblimit; i < 32; i++)
    for (j = ch_start; j < ch_end; j++)
      scalefactor[j][0][i] = scalefactor[j][1][i] =
      scalefactor[j][2][i] = 63;
}

void mp3dec_mc_decode_sample_l2(MP3Dec_com *state)
{
  const Ipp8u  *alloc_table = state->alloc_table;
  sBitsreamBuffer *m_data = &(state->m_StreamData);
  Ipp16s (*allocation)[32] = state->allocation;
  Ipp32s (*sample)[32][36] = state->sample;
  Ipp32s stereo = state->stereo;
  Ipp32s ch_start = stereo;
  Ipp32s ch_end = stereo + state->mc_channel;
  Ipp32s i, j, k, gr, sb_gr;
  Ipp32s transmitted;
  Ipp32s sblimit = state->sblimit;

  for (gr = 0; gr < 12; gr++) {
    if(state->mc_header.lfe && state->mc_lfe_alloc > 0)
      GET_BITS(m_data, state->mc_lfe_spl[gr], state->mc_lfe_alloc + 1, Ipp32s);

    for (i = 0; i < sblimit; i++) {
      sb_gr = mp3_mc_sb_group[i];

      for (j = ch_start; j < ch_end; j++) {
        if (allocation[j][i]) {
          transmitted = 1;
          if (state->mc_dyn_cross_on == 1) {
            if (state->mc_dyn_cross_bits == 4)
              switch(state->mc_dyn_cross_mode[sb_gr]) {
                case 1: if (j == 4) transmitted = 0; break;
                case 2: if (j == 3) transmitted = 0; break;
                case 3: if (j == 2) transmitted = 0; break;
                case 4: if (j != 2) transmitted = 0; break;
                case 5: if (j != 3) transmitted = 0; break;
                case 6: if (j != 4) transmitted = 0; break;
                case 7: transmitted = 0; break;
                case 8: if (j == 4) transmitted = 0; break;
                case 9: if (j == 3) transmitted = 0; break;
                case 10: if (j == 4) transmitted = 0; break;
                case 11: if (j != 2) transmitted = 0; break;
                case 12: if (j != 2) transmitted = 0; break;
                case 13: if (j != 3) transmitted = 0; break;
                case 14: if (j != 2) transmitted = 0; break;
                default:
                  break;
              }
            else if (state->mc_dyn_cross_bits == 3 && (
              (state->mc_dyn_cross_mode[sb_gr]==1  && j==3) ||
              (state->mc_dyn_cross_mode[sb_gr]==2  && j==2) ||
              (state->mc_dyn_cross_mode[sb_gr]==3) ||
              (state->mc_dyn_cross_mode[sb_gr]==4  && j==3)
              ))
              transmitted = 0;
            else if (state->mc_dyn_cross_bits == 1 &&
              state->mc_dyn_cross_mode[sb_gr]==1 && j==2)
              transmitted = 0;
            if (state->mc_header.surround == 3 &&
              state->mc_dyn_second_stereo[sb_gr] == 1) {
                if ((state->mc_header.center == 1 || state->mc_header.center == 3) && j == 4)
                  transmitted = 0;
                else if (state->mc_header.center == 0 && j == 3)
                  transmitted = 0;
              }
          }
        }
        else
          transmitted = 0;

        if (transmitted) {
          k = mp3_cls_quant[alloc_table[i * 16 + allocation[j][i]]];
          if (k>0) {
            GET_BITS(m_data, sample[j][i][3 * gr + 0], k, Ipp32s);
            GET_BITS(m_data, sample[j][i][3 * gr + 1], k, Ipp32s);
            GET_BITS(m_data, sample[j][i][3 * gr + 2], k, Ipp32s);
          } else {
            Ipp32s c;
            Ipp16s val;
            k = -k;
            GET_BITS(m_data, c, k, Ipp32s);
            val = mp3_degroup[k][c];
            sample[j][i][3 * gr + 0] = val >> 8;
            sample[j][i][3 * gr + 1] = (val >> 4) & 0xF;
            sample[j][i][3 * gr + 2] = val & 0xF;
          }
        }
        else {
          for(k = 0; k < 3; k++)
            sample[j][i][3 * gr + k] = 0;
        }
      }
    }

    for (j = ch_start; j < ch_end; j++)
      for (i = sblimit; i < 32; i++)
        for (k = 0; k < 3; k++)
          sample[j][i][3 * gr + k] = 0;
  }
}

#endif //UMC_ENABLE_XXX
