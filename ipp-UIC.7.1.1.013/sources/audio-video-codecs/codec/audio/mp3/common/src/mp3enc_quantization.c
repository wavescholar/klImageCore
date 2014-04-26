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
#if defined (UMC_ENABLE_MP3_AUDIO_ENCODER)

#include "align.h"
#include "mp3enc_own.h"

/******************************************************************************
//  Name:
//    chooseTableLong
//
//  Description:
//    The function selects proper huffman table for part of spectrum and
//    calculate count of bits necessary to encoding
//
//  Input Arguments:
//    EC     - pointer to encoder context
//    gr     - number of granule
//    ch     - number of channel
//    pInput - pointer to array of quantized samples
//    length - count of samples in the input array which should be encoded
//    table  - number of part of spectrum. can be 0,1,2.
//             (big values are divided on three part. each is coded by using different table)
//
//  Output Arguments:
//    -
//
//  Returns:
//    count of bits necessary to encoding
//
******************************************************************************/

static const VM_ALIGN32_DECL(Ipp8s*) tables_info_full[16] = {
    "\0",
    "\1\2\3\5\6\7\10\11\12\13\14\15\17",
    "\2\3\5\6\7\10\11\12\13\14\15\17",
    "\5\6\7\10\11\12\13\14\15\17",
    "\7\10\11\12\13\14\15\17",
    "\7\10\11\12\13\14\15\17",
    "\12\13\14\15\17",
    "\12\13\14\15\17",
    "\15\17",
    "\15\17",
    "\15\17",
    "\15\17",
    "\15\17",
    "\15\17",
    "\15\17",
    "\0\0"
};

static const VM_ALIGN32_DECL(Ipp8s*) tables_info_short[16] = {
    "\0",
    "\1",
    "\2\3",
    "\5\6",
    "\7\10\11",
    "\7\10\11",
    "\12\13\14",
    "\12\13\14",
    "\15\17",
    "\15\17",
    "\15\17",
    "\15\17",
    "\15\17",
    "\15\17",
    "\15\17",
    "\0\0"
};

Ipp32s mp3enc_quantChooseTableLong(MP3Enc_com *state,
                                   Ipp32s gr,
                                   Ipp32s ch,
                                   Ipp16s *pInput,
                                   Ipp32s length,
                                   Ipp32s table)
{
    IPP_ALIGNED_ARRAY(32, Ipp16s, tmp_src, 288);
    const Ipp8s* tables = NULL;
    MP3Enc_HuffmanTable* htables = NULL;
    Ipp8s  tbuf[3];
    Ipp32s choice0, choice1, i, tbl;
    Ipp32s sum0, sum1;
    Ipp16s max;

    htables = state->htables;
    ippsMax_16s(pInput, length, &max);

    if (max == 0 || length == 0)
      return 0;

    choice0 = 0;
    choice1 = 0;

    sum0 = sum1 = 32767;

    if (max >= 15) {
/*
 * try tables with linbits
 */
      choice0 = 15;
      while (htables[choice0].mav_value < max)
        choice0++;

      choice1 = 24;
      while (htables[choice1].mav_value < max)
        choice1++;

        tables = tbuf;
        tbuf[0] = (Ipp8u)choice0;
        tbuf[1] = (Ipp8u)choice1;
        tbuf[2] = 0;
    } else {
        if (!state->quant_mode_fast)
            tables = tables_info_full[max];
        else
            tables = tables_info_short[max];
    }

    for (i = 0; (tbl = tables[i]) != 0; i++) {
      IppsVLCEncodeSpec_32s* VLCSpec;
      Ipp32s linbits;
      VLCSpec = (IppsVLCEncodeSpec_32s*)(htables[tbl].phuftable);
      linbits = htables[tbl].linbits;

      if (linbits == 0) {
        Ipp16s *q;
        Ipp32s j;
        Ipp32s shift = mp3enc_VLCShifts[tbl];
        Ipp32s offset = mp3enc_VLCOffsets[tbl];

        q = pInput;

        for (j = 0; j < (length >> 1); j++) {
          tmp_src[j] = (Ipp16s)((q[0] << shift) + (q[1] + offset));
          q += 2;
        }

        ippsVLCCountBits_16s32s(tmp_src, length >> 1, &sum1, VLCSpec);
      } else {
        ippsVLCCountEscBits_MP3_16s32s(pInput, length, linbits,
                                       &sum1, VLCSpec);
      }

      if (sum1 < sum0) {
        choice0 = tbl;
        sum0 = sum1;
      }
    }
    state->si_pTableSelect[gr][ch][table] = choice0;

    return sum0;
}

static Ipp32s mp3enc_quantChooseTableLong_opt(MP3Enc_com *state,
                                              Ipp16s *pInput,
                                              Ipp16s *max_table,
                                              Ipp16s (*bit_table)[22],
                                              const Ipp32u* sfb_table,
                                              Ipp32s sfb_start,
                                              Ipp32s sfb_num,
                                              Ipp32s bigv_region,
                                              Ipp32s *table)
{
  IPP_ALIGNED_ARRAY(32, Ipp16s, tmp_src, 288);
  const Ipp8s* tables = NULL;
  MP3Enc_HuffmanTable* htables = NULL;
  Ipp8s  tbuf[3];
  Ipp32s choice0, choice1, i, jj, tbl;
  Ipp32s sum0, sum1, csum;
  Ipp16s max;

  htables = state->htables;

  *table = 0;
  max = 0;
  for (i = sfb_start; i < sfb_start + sfb_num; i++) {
    if (max < max_table[i])
      max = max_table[i];
  }

  if (max == 0)
    return 0;

  choice0 = 0;
  choice1 = 0;

  sum0 = sum1 = 32767;

  if (max >= 15) {
    /*
    * try tables with linbits
    */
    choice0 = 15;
    while (htables[choice0].mav_value < max)
      choice0++;

    choice1 = 24;
    while (htables[choice1].mav_value < max)
      choice1++;

    tables = tbuf;
    tbuf[0] = (Ipp8s)choice0;
    tbuf[1] = (Ipp8s)choice1;
    tbuf[2] = 0;
  } else {
    if (!state->quant_mode_fast)
      tables = tables_info_full[max];
    else
      tables = tables_info_short[max];
  }

  for (i = 0; (tbl = tables[i]) != 0; i++) {
    sum1 = 0;
    for (jj = sfb_start; jj < sfb_start + sfb_num; jj++) {
      if (sfb_table[jj] >= (Ipp32u)bigv_region)
        break;

      if (bit_table[tbl][jj] == -1) {
        Ipp32s length;
        IppsVLCEncodeSpec_32s* VLCSpec;
        Ipp32s linbits;
        VLCSpec = (IppsVLCEncodeSpec_32s*)(htables[tbl].phuftable);
        linbits = htables[tbl].linbits;
        if (sfb_table[jj + 1] > (Ipp32u)bigv_region)
          length = bigv_region - sfb_table[jj];
        else
          length = sfb_table[jj + 1] - sfb_table[jj];

        if (linbits == 0) {
          Ipp16s *q;
          Ipp32s j;
          Ipp32s shift = mp3enc_VLCShifts[tbl];
          Ipp32s offset = mp3enc_VLCOffsets[tbl];

          q = pInput + sfb_table[jj];

          for (j = 0; j < (length >> 1); j++) {
            tmp_src[j] = (Ipp16s)((q[0] << shift) + (q[1] + offset));
            q += 2;
          }

          ippsVLCCountBits_16s32s(tmp_src, length >> 1, &csum, VLCSpec);
        } else {
          ippsVLCCountEscBits_MP3_16s32s(pInput + sfb_table[jj], length, linbits,
            &csum, VLCSpec);

        }
        bit_table[tbl][jj] = (Ipp16s)csum;
      }
      sum1 += bit_table[tbl][jj];
    }

    if (sum1 < sum0) {
      choice0 = tbl;
      sum0 = sum1;
    }
  }

  *table = choice0;

  return sum0;
}

/******************************************************************************
//  Name:
//    calc_bits_long
//
//  Description:
//
//
//  Input Arguments:
//
//
//  Output Arguments:
//
//
//  Returns:
//
//
******************************************************************************/

Ipp32s mp3enc_quantCalcBitsLong(MP3Enc_com *state,
                                Ipp16s *pInput,
                                Ipp32s gr,
                                Ipp32s ch)
{
    Ipp32s bits = 0;
    Ipp32s bigv_region = state->si_bigVals[gr][ch] << 1;

    state->si_pTableSelect[gr][ch][0] = 0;
    state->si_pTableSelect[gr][ch][1] = 0;
    state->si_pTableSelect[gr][ch][2] = 0;

    if (state->si_address[gr][ch][0] > (Ipp32u)bigv_region)
      state->si_address[gr][ch][0] = bigv_region;
    if (state->si_address[gr][ch][1] < state->si_address[gr][ch][0])
      state->si_address[gr][ch][1] = state->si_address[gr][ch][0];
    if (state->si_address[gr][ch][2] < state->si_address[gr][ch][1])
      state->si_address[gr][ch][2] = state->si_address[gr][ch][1];

    if (state->si_address[gr][ch][0] > 0) {
      bits +=
        mp3enc_quantChooseTableLong(state, gr, ch, pInput, state->si_address[gr][ch][0], 0);
    }

    if (state->si_address[gr][ch][1] > state->si_address[gr][ch][0]) {
      bits +=
        mp3enc_quantChooseTableLong(state, gr, ch, pInput + state->si_address[gr][ch][0],
                        state->si_address[gr][ch][1] - state->si_address[gr][ch][0], 1);
    }

    if ((Ipp32u)bigv_region > state->si_address[gr][ch][1]) {
      bits +=
        mp3enc_quantChooseTableLong(state, gr, ch, pInput + state->si_address[gr][ch][1],
                        bigv_region - state->si_address[gr][ch][1], 2);
    }

    return bits;
}

/******************************************************************************
//  Name:
//    calc_bits
//
//  Description:
//
//
//  Input Arguments:
//
//
//  Output Arguments:
//
//
//  Returns:
//
//
******************************************************************************/

Ipp32s mp3enc_quantCalcBits(MP3Enc_com *state,
                            Ipp32s gr,
                            Ipp32s ch)
{
    Ipp32s last_reg_bits[22];
    Ipp32s last_reg_tbl[22];
    Ipp16s tbits[32][22];
    Ipp16s tmax[22];
    const Ipp32u* sfb_long = NULL;
    Ipp32s* ptr_int = NULL;

    Ipp32u zero_region = LEN_MDCT_LINE;
    Ipp32u bigv_region = LEN_MDCT_LINE;
    Ipp32s bits = 0;
    Ipp32s sum0 = 0;
    Ipp32s sum1 = 0;
    Ipp32s i;
    Ipp32s t0, t1, t2, st_t0 = 0, st_t1 = 0, st_t2 = 0;
    Ipp32s p;

    ptr_int = (Ipp32s *)&state->quant_ix[gr][ch][zero_region - 2];

    sfb_long = mp3enc_sfBandIndex[state->header.id][state->header.samplingFreq].l;

    //if ((state->si_blockType[gr][ch] != 2)) {

        while(zero_region > 1 &&
            (*ptr_int-- == 0))
            zero_region -= 2;

        bigv_region = zero_region;

        while(bigv_region > 3) {
            Ipp32s p0, p1, p2, p3;
            p0 = state->quant_ix[gr][ch][bigv_region - 4];
            p1 = state->quant_ix[gr][ch][bigv_region - 3];
            p2 = state->quant_ix[gr][ch][bigv_region - 2];
            p3 = state->quant_ix[gr][ch][bigv_region - 1];

            if ((p0 | p1 | p2 | p3) > 1)
                break;

            bigv_region -= 4;

            p = (p0 << 3) | (p1 << 2) | (p2 << 1) | (p3);

            sum0 += mp3enc_table32[3 * p + 1];
            sum1 += mp3enc_table33[3 * p + 1];
        }
    //}

    state->si_count1[gr][ch] = (zero_region - bigv_region) >> 2;
    state->si_bigVals[gr][ch] = bigv_region >> 1;

    if (sum0 < sum1) {
        bits += sum0;
        state->si_cnt1TabSel[gr][ch] = 0;
    } else {
        bits += sum1;
        state->si_cnt1TabSel[gr][ch] = 1;
    }

    if (bigv_region) {
        ippsSet_16s(-1, &tbits[0][0], 32*22);
        for (i = 0; i < 22; i++) {
          Ipp32s length = sfb_long[i+1] - sfb_long[i];
          if (sfb_long[i] >= bigv_region) {
            tmax[i] = 0;
            continue;
          }
          if (sfb_long[i+1] > bigv_region)
            length = bigv_region - sfb_long[i];

          ippsMax_16s(state->quant_ix[gr][ch] + sfb_long[i],
            length, tmax + i);
        }
        if (state->si_winSwitch[gr][ch] == 0) {
            if (!state->quant_mode_fast) {
                Ipp32s bits0, bits1, bits2;
                Ipp32s index0, index1, index2, st_reg0 = 0, st_reg1 = 0;
                Ipp32u reg1_end;
                Ipp32s min_bits = 1 << 30, cur_bits;
                Ipp32s reg0, reg1;
                Ipp32s regmax = 0;

                for (i = 0; i < 22; i++)
                    last_reg_bits[i] = -1;

                regmax = 0;
                while (sfb_long[regmax] < bigv_region)
                    regmax++;

                for (reg0 = 0; reg0 <= 15; reg0++) {
                    index0 = (state->si_reg0Cnt[gr][ch] = reg0) + 1;

                    if (index0 > regmax)
                        continue;

                    bits0 = mp3enc_quantChooseTableLong_opt(state, state->quant_ix[gr][ch], tmax,
                      tbits, sfb_long, 0, index0, bigv_region, &t0);

                    for (reg1 = 0; reg1 <= 7; reg1++) {
                        index1 = (state->si_reg1Cnt[gr][ch] = reg1) + 1;

                        if (index0 + index1 > regmax)
                          continue;

                        index2 = regmax - index0 - index1;

                        reg1_end = sfb_long[index0 + index1];
                        if (reg1_end > bigv_region)
                            reg1_end = bigv_region;

                        bits1 = mp3enc_quantChooseTableLong_opt(state, state->quant_ix[gr][ch], tmax,
                          tbits, sfb_long, index0, index1, bigv_region, &t1);

                        t2 = last_reg_tbl[index2];
                        bits2 = last_reg_bits[index2];
                        if (bits2 == -1) {
                          bits2 = mp3enc_quantChooseTableLong_opt(state, state->quant_ix[gr][ch], tmax,
                            tbits, sfb_long, index0 + index1, index2, bigv_region, &t2);

                          last_reg_bits[index2] = bits2;
                          last_reg_tbl[index2] = t2;
                        }
                        cur_bits = bits0 + bits1 + bits2;
                        if (cur_bits < min_bits) {
                            min_bits = cur_bits;
                            st_reg0 = reg0;
                            st_reg1 = reg1;
                            st_t0 = t0;
                            st_t1 = t1;
                            st_t2 = t2;
                        }
                    }
                }
                index0 = (state->si_reg0Cnt[gr][ch] = st_reg0) + 1;
                index1 = (state->si_reg1Cnt[gr][ch] = st_reg1) + 1;

                state->si_pTableSelect[gr][ch][0] = st_t0;
                state->si_pTableSelect[gr][ch][1] = st_t1;
                state->si_pTableSelect[gr][ch][2] = st_t2;

                state->si_address[gr][ch][0] = sfb_long[index0];
                state->si_address[gr][ch][1] = sfb_long[index0 + index1];
                if (state->si_address[gr][ch][0] > bigv_region)
                    state->si_address[gr][ch][0] = bigv_region;
                if (state->si_address[gr][ch][1] > bigv_region)
                    state->si_address[gr][ch][1] = bigv_region;
                state->si_address[gr][ch][2] = bigv_region;

                if (min_bits < (1 << 30))  {
                  bits += min_bits;
                }
                else {
                  bits += mp3enc_quantCalcBitsLong(state, state->quant_ix[gr][ch], gr, ch);
                }
            } else {
                Ipp32s  index0, index1;

                i = 0;
                while (sfb_long[i] < bigv_region)
                    i++;

                index0 = (state->si_reg0Cnt[gr][ch] = mp3enc_region01_table[i][0]) + 1;
                index1 = (state->si_reg1Cnt[gr][ch] = mp3enc_region01_table[i][1]) + 1;

                state->si_address[gr][ch][0] = sfb_long[index0];
                state->si_address[gr][ch][1] = sfb_long[index0 + index1];
                if (state->si_address[gr][ch][0] > bigv_region)
                  state->si_address[gr][ch][0] = bigv_region;
                if (state->si_address[gr][ch][1] > bigv_region)
                  state->si_address[gr][ch][1] = bigv_region;
                state->si_address[gr][ch][2] = bigv_region;

                bits += mp3enc_quantCalcBitsLong(state, state->quant_ix[gr][ch], gr, ch);
            }
        } else {
            if (state->si_mixedBlock[gr][ch] || (state->si_blockType[gr][ch] != 2)) {
                state->si_reg0Cnt[gr][ch] = 7;
                state->si_reg1Cnt[gr][ch] = 13;
                state->si_address[gr][ch][0] = sfb_long[state->si_reg0Cnt[gr][ch] + 1];
            } else {
                state->si_reg0Cnt[gr][ch] = 8;
                state->si_reg1Cnt[gr][ch] = 36;
                state->si_address[gr][ch][0] = 36;
            }

            if (state->si_address[gr][ch][0] > bigv_region) {
              state->si_address[gr][ch][0] = bigv_region;
            }

            state->si_address[gr][ch][1] = bigv_region;
            state->si_address[gr][ch][2] = 0;
            bits += mp3enc_quantCalcBitsLong(state, state->quant_ix[gr][ch], gr, ch);
        }
    } else {
        state->si_address[gr][ch][0] = state->si_address[gr][ch][1] = state->si_address[gr][ch][2] = 0;
        state->si_reg0Cnt[gr][ch] = 0;
        state->si_reg1Cnt[gr][ch] = 0;
        bits += mp3enc_quantCalcBitsLong(state, state->quant_ix[gr][ch], gr, ch);
    }

    bits += mp3enc_quantcalcPart2Len(state, gr, ch);

    return bits;
}

/******************************************************************************
//  Name:
//    iter_reset
//
//  Description:
//    resets all side_info variables before itaration process
******************************************************************************/

void mp3enc_quantIterReset(MP3Enc_com *state,
                           Ipp32s gr,
                           Ipp32s ch)
{
    Ipp32s  i, sfb, b;

    for (i = 0; i < 4; i++)
      state->si_scfsi[ch][i] = 0;

    for (i = 0; i < 4; i++)
      state->slen[ch][i] = 0;

    for (sfb = 0; sfb < SBBND_L; sfb++)
      state->scalefac_l[gr][ch][sfb] = 0;

    for (sfb = 0; sfb < SBBND_S; sfb++)
      for (b = 0; b < 3; b++)
        state->scalefac_s[gr][ch][sfb][b] = 0;

    state->si_cnt1TabSel[gr][ch] = 0;
    state->si_reg0Cnt[gr][ch] = 0;
    state->si_reg1Cnt[gr][ch] = 0;
    state->si_part23Len[gr][ch] = 0;
    state->si_part2Len[gr][ch] = 0;
    state->si_bigVals[gr][ch] = 0;
    state->si_preFlag[gr][ch] = 0;
    state->si_sfCompress[gr][ch] = 0;
    state->si_sfScale[gr][ch] = 0;
    state->si_count1[gr][ch] = 0;

    state->si_pSubBlkGain[gr][ch][0] = 0;
    state->si_pSubBlkGain[gr][ch][1] = 0;
    state->si_pSubBlkGain[gr][ch][2] = 0;

    state->si_pTableSelect[gr][ch][0] = 0;
    state->si_pTableSelect[gr][ch][1] = 0;
    state->si_pTableSelect[gr][ch][2] = 0;
}

/******************************************************************************
//  Name:
//    calc_part2Len
//
//  Description:
//    Calculate the number of main_data bits used for scalefactors.
//
//  Input Arguments:
//    EC - pointer to encoder context
//    gr - number of granule
//    ch - number of channel
//
//  Output Arguments:
//
//
//  Returns:
//    number of main_data bits used for scalefactors
//
******************************************************************************/

static const VM_ALIGN32_DECL(Ipp32s) mp3enc_nr_of_sfb_block [6][3][4] =
{
  {
    {6, 5, 5, 5},
    {9, 9, 9, 9},
    {6, 9, 9, 9}
  },
  {
    {6, 5, 7, 3},
    {9, 9, 12, 6},
    {6, 9, 12, 6}
  },
  {
    {11, 10, 0, 0},
    {18, 18, 0, 0},
    {15,18,0,0}
  },
  {
    {7, 7, 7, 0},
    {12, 12, 12, 0},
    {6, 15, 12, 0}
  },
  {
    {6, 6, 6, 3},
    {12, 9, 9, 6},
    {6, 12, 9, 6}
  },
  {
    {8, 8, 5, 0},
    {15,12,9,0},
    {6,18,9,0}
  }
};

static const VM_ALIGN32_DECL(Ipp32s) mp3enc_log2tab[] = { 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4 };

Ipp32s mp3enc_quantcalcPart2Len(MP3Enc_com *state,
                                Ipp32s gr,
                                Ipp32s ch)
{
    Ipp32s  slen1, slen2, bits;

    mp3enc_quantScaleBitCount(state, gr, ch);

    bits = 0;

    if (state->header.id) {
      slen1 = mp3enc_slen1_tab[state->si_sfCompress[gr][ch]];
      slen2 = mp3enc_slen2_tab[state->si_sfCompress[gr][ch]];

      if ((state->si_winSwitch[gr][ch] == 1) && (state->si_blockType[gr][ch] == 2)) {
        if (state->si_mixedBlock[gr][ch]) {
          bits += (8 * slen1) + (9 * slen1) + (18 * slen2);
        } else {
          bits += (18 * slen1) + (18 * slen2);
        }
      } else {
        if ((gr == 0) || (state->si_scfsi[ch][0] == 0))
          bits += (6 * slen1);

        if ((gr == 0) || (state->si_scfsi[ch][1] == 0))
          bits += (5 * slen1);

        if ((gr == 0) || (state->si_scfsi[ch][2] == 0))
          bits += (5 * slen2);

        if ((gr == 0) || (state->si_scfsi[ch][3] == 0))
          bits += (5 * slen2);
      }
    } else {
      Ipp32s i, table_num, row_in_table, sfb, part, nr_sfb, window;
      Ipp32s max_sfac[4];
      const Ipp32s *part_table;
      gr = 0;
      if (state->si_preFlag[gr][ch])
        table_num = 2;
      else
        table_num = 0;

      for (i = 0; i < 4; i++)
        max_sfac[i] = 0;

      if (state->si_blockType[gr][ch] == 2) {
        row_in_table = 1;
        part_table = &mp3enc_nr_of_sfb_block[table_num][row_in_table][0];
        sfb = 0;
        for (part = 0; part < 4; part++)
        {
          nr_sfb = part_table[part] / 3;
          for (i = 0; i < nr_sfb; i++, sfb++)
            for ( window = 0; window < 3; window++)
              if ( state->scalefac_s[gr][ch][sfb][window] > max_sfac[part] )
                max_sfac[part] = state->scalefac_s[gr][ch][sfb][window];
        }
      } else {
        row_in_table = 0;
        part_table = &mp3enc_nr_of_sfb_block[table_num][row_in_table][0];
        sfb = 0;
        for (part = 0; part < 4; part++)
        {
          nr_sfb = part_table[part];
          for (i = 0; i < nr_sfb; i++, sfb++)
            if (state->scalefac_l[gr][ch][sfb] > max_sfac[part])
              max_sfac[part] = state->scalefac_l[gr][ch][sfb];
        }
      }

      state->sfb_part_table[ch] = &mp3enc_nr_of_sfb_block[table_num][row_in_table][0];
      for (part = 0; part < 4; part++)
        state->slen[ch][part] = mp3enc_log2tab[max_sfac[part]];

      switch (table_num)
      {
      case 0:
        state->si_sfCompress[gr][ch] = (((state->slen[ch][0] * 5) +
          state->slen[ch][1]) << 4) + (state->slen[ch][2] << 2) + state->slen[ch][3];
        break;
      case 1:
        state->si_sfCompress[gr][ch] = 400 + (((state->slen[ch][0] * 5) +
          state->slen[ch][1]) << 2) + state->slen[ch][2];
        break;
      case 2:
        state->si_sfCompress[gr][ch] = 500 + (state->slen[ch][0] * 3) +
          state->slen[ch][1];
        break;
      default:
        break;
      }
      for (i = 0; i < 4; i++ )
        bits += state->slen[ch][i] * state->sfb_part_table[ch][i];
    }

    state->si_part2Len[gr][ch] = bits;

    return bits;
}

static const VM_ALIGN32_DECL(Ipp32s) mp3enc_pow2[5] = { 1, 2, 4, 8, 16 };

Ipp32s mp3enc_quantScaleBitCount(MP3Enc_com *state,
                                 Ipp32s gr,
                                 Ipp32s ch)
{
    Ipp32s  i, k, sfb, max_slen1 = 0, max_slen2 = 0;

    Ipp16u (*scalefac_l)[2][32];
    Ipp16u (*scalefac_s)[2][12][3];
    scalefac_l = state->scalefac_l;
    scalefac_s = state->scalefac_s;

    if ((state->si_winSwitch[gr][ch] == 1) && (state->si_blockType[gr][ch] == 2)) {
      if (state->si_mixedBlock[gr][ch] == 0) {
        for (i = 0; i < 3; i++) {
          for (sfb = 0; sfb < 6; sfb++)
            if (scalefac_s[gr][ch][sfb][i] > max_slen1)
              max_slen1 = scalefac_s[gr][ch][sfb][i];
          for (sfb = 6; sfb < 12; sfb++)
            if (scalefac_s[gr][ch][sfb][i] > max_slen2)
              max_slen2 = scalefac_s[gr][ch][sfb][i];
        }
      } else {
        for (sfb = 0; sfb < 8; sfb++)
          if (scalefac_l[gr][ch][sfb] > max_slen1)
            max_slen1 = scalefac_l[gr][ch][sfb];

        for (i = 0; i < 3; i++) {
          for (sfb = 3; sfb < 6; sfb++)
            if (scalefac_s[gr][ch][sfb][i] > max_slen1)
              max_slen1 = scalefac_s[gr][ch][sfb][i];
          for (sfb = 6; sfb < 12; sfb++)
            if (scalefac_s[gr][ch][sfb][i] > max_slen2)
              max_slen2 = scalefac_s[gr][ch][sfb][i];
        }
      }
    } else {    // block_type == 0,1,or 3
      for (sfb = 0; sfb < 11; sfb++)
        if (scalefac_l[gr][ch][sfb] > max_slen1)
          max_slen1 = scalefac_l[gr][ch][sfb];
      for (sfb = 11; sfb < 21; sfb++)
        if (scalefac_l[gr][ch][sfb] > max_slen2)
          max_slen2 = scalefac_l[gr][ch][sfb];
    }

    for (k = 0; k < 16; k++) {
      if ((max_slen1 < mp3enc_pow2[mp3enc_slen1_tab[k]]) &&
          (max_slen2 < mp3enc_pow2[mp3enc_slen2_tab[k]])) {
        state->si_sfCompress[gr][ch] = k;
        return 0;
      }
    }

    return 1;
}

#endif //UMC_ENABLE_XXX
