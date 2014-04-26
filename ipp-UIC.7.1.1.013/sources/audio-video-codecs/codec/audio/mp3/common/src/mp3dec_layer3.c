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
#if defined (UMC_ENABLE_MP3_AUDIO_DECODER)

#include "mp3dec_own.h"

/* scfsi_band  scalefactor bands (p.24 ISO/IEC 11172-3 )*/
VM_ALIGN32_DECL(static const struct) {
    Ipp32s  l[5];
    Ipp32s  s[3];
} sfbtable = {
    { 0, 6, 11, 16, 21},
    { 0, 6, 12}
};

VM_ALIGN32_DECL(static const Ipp32s) slen[2][16] = {
    {0x0, 0x0, 0x0, 0x0, 0x3, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x4, 0x4},
    {0x0, 0x1, 0x2, 0x3, 0x0, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x2, 0x3}
};

/* Table 3-B.8. Layer III scalefactor bands (p.33 Annex_AB ISO/IEC 11172-3)*/
VM_ALIGN32_DECL(const ssfBandIndex_l) mp3dec_sfBandIndex_l[3][3] = {
    {
      { 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238,
          284, 336, 396, 464, 522, 576 },
      { 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 114, 136, 162, 194, 232,
          278, 332, 394, 464, 540, 576},
      { 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238,
          284, 336, 396, 464, 522, 576}
    },
    {
        { 0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 52, 62, 74, 90, 110, 134, 162,
            196, 238, 288, 342, 418, 576},
        { 0, 4, 8, 12, 16, 20, 24, 30, 36, 42, 50, 60, 72, 88, 106, 128, 156,
            190, 230, 276, 330, 384, 576},
        { 0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 54, 66, 82, 102, 126, 156, 194,
            240, 296, 364, 448, 550, 576}
    },
    {
        { 0,  6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238,
            284, 336, 396, 464, 522, 576},
        { 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238,
            284, 336, 396, 464, 522, 576},
        { 0, 12, 24, 36, 48, 60, 72, 88, 108, 132, 160, 192, 232, 280, 336, 400, 476,
          566, 568, 570, 572, 574, 576}
    }
};

VM_ALIGN32_DECL(const ssfBandIndex_s) mp3dec_sfBandIndex_s[3][3] = {
  {
    {0, 4, 8, 12, 18, 24, 32, 42, 56, 74, 100, 132, 174, 192},
    {0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 136, 180, 192},
    {0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 134, 174, 192}
  },
  {
    {0, 4, 8, 12, 16, 22, 30, 40, 52, 66, 84, 106, 136, 192},
    {0, 4, 8, 12, 16, 22, 28, 38, 50, 64, 80, 100, 126, 192},
    {0, 4, 8, 12, 16, 22, 30, 42, 58, 78, 104, 138, 180, 192}
  },
  {
    {0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 134, 174, 192},
    {0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 134, 174, 192},
    {0, 8,16, 24, 36, 52, 72, 96, 124, 160, 162, 164, 166, 192}
  }
};

VM_ALIGN32_DECL(const Ipp8u) mp3dec_nr_of_sfb[6][3][4] =
{
    {{0x6, 0x5, 0x5, 0x5}, { 0x9,  0x9, 0x9, 0x9}, {0x6,  0x9, 0x9, 0x9}},
    {{0x6, 0x5, 0x7, 0x3}, { 0x9,  0x9, 0xc, 0x6}, {0x6,  0x9, 0xc, 0x6}},
    {{0xb, 0xa, 0x0, 0x0}, {0x12, 0x12, 0x0, 0x0}, {0xf, 0x12, 0x0, 0x0}},
    {{0x7, 0x7, 0x7, 0x0}, { 0xc,  0xc, 0xc, 0x0}, {0x6,  0xf, 0xc, 0x0}},
    {{0x6, 0x6, 0x6, 0x3}, { 0xc,  0x9, 0x9, 0x6}, {0x6,  0xc, 0x9, 0x6}},
    {{0x8, 0x8, 0x5, 0x0}, { 0xf,  0xc, 0x9, 0x0}, {0x6, 0x12, 0x9, 0x0}}
};

/******************************************************************************
//  Name:
//    GetScaleFactors
//
//  Description:
//    read scalefactors for one granule of channel
//
//  Input Arguments:
//    state - point to MP3Dec structure
//    gr - number of granule
//    ch - number of channel
//
//  Output Arguments:
//    ScaleFactors - array of scalefactors
//
//  Returns:
//    1 - all ok
//
******************************************************************************/

Ipp32s mp3dec_GetScaleFactorsL3(MP3Dec_com *state, Ipp32s gr, Ipp32s ch) {
    Ipp32s sfb, window, i;
    sBitsreamBuffer *BS = &(state->m_MainData);
    sScaleFactors *ScaleFactors = state->ScaleFactors;
    Ipp32u (*si_blockType)[2] = state->si_blockType;
    Ipp32u (*si_mixedBlock)[2] = state->si_mixedBlock;
    Ipp32u *si_scfsi = state->si_scfsi;
    Ipp32u (*si_sfCompress)[2] = state->si_sfCompress;
    Ipp32u (*si_winSwitch)[2] = state->si_winSwitch;

    if (si_winSwitch[gr][ch] && (si_blockType[gr][ch] == 2)) {
        if (si_mixedBlock[gr][ch]) {
            for (sfb = 0; sfb < 8; sfb++)
                if (slen[0][si_sfCompress[gr][ch]] != 0)
                    GET_BITS(BS, ScaleFactors[ch].l[sfb],
                        slen[0][si_sfCompress[gr][ch]], Ipp16s)
                else
                    ScaleFactors[ch].l[sfb] = 0;
            for (sfb = 3; sfb < 6; sfb++)
                for (window = 0; window < 3; window++)
                    if (slen[0][si_sfCompress[gr][ch]] != 0)
                        GET_BITS(BS, ScaleFactors[ch].s[window][sfb],
                            slen[0][si_sfCompress[gr][ch]], Ipp16s)
                    else
                        ScaleFactors[ch].s[window][sfb] = 0;

            for (sfb = 6; sfb < 12; sfb++)
                for (window = 0; window < 3; window++)
                    if (slen[1][si_sfCompress[gr][ch]] != 0)
                        GET_BITS(BS, ScaleFactors[ch].s[window][sfb],
                            slen[1][si_sfCompress[gr][ch]], Ipp16s)
                    else
                        ScaleFactors[ch].s[window][sfb] = 0;
        } else {  /* SHORT */
            for (i = 0; i < 2; i++)
                for (sfb = sfbtable.s[i]; sfb < sfbtable.s[i + 1]; sfb++)
                    for (window = 0; window < 3; window++)
                        if (slen[i][si_sfCompress[gr][ch]] != 0)
                            GET_BITS(BS, ScaleFactors[ch].s[window][sfb],
                                slen[i][si_sfCompress[gr][ch]], Ipp16s)
                        else
                            ScaleFactors[ch].s[window][sfb] = 0;
        }
        ScaleFactors[ch].s[0][12] = 0;
        ScaleFactors[ch].s[1][12] = 0;
        ScaleFactors[ch].s[2][12] = 0;
    } else {    /* LONG types 0,1,3 */
        for (i = 0; i < 4; i++) {
            if ((((si_scfsi[ch] >> (3 - i)) & 1) == 0) || (gr == 0)) {
                for (sfb = sfbtable.l[i]; sfb < sfbtable.l[i + 1]; sfb++) {
                    if (slen[(i < 2) ? 0 : 1][si_sfCompress[gr][ch]] != 0) {
                        GET_BITS(BS, ScaleFactors[ch].l[sfb],
                            slen[(i < 2) ? 0 : 1][si_sfCompress[gr][ch]], Ipp16s);
                    } else {
                        ScaleFactors[ch].l[sfb] = 0;
                    }
                }
            }
        }
    }

    return 1;
}

/******************************************************************************
//  Name:
//    audio_data_LayerIII
//
//  Description:
//    read side information
//
//  Input Arguments:
//    state - point to MP3Dec structure
//
//  Output Arguments:
//    state - point to MP3Dec structure, side info
//
//  Returns:
//    1 - all ok
//   -2 - wrong block type
//
******************************************************************************/

Ipp32s mp3dec_audio_data_LayerIII(MP3Dec_com *state) {
    Ipp32s gr, ch;
    Ipp32s tmp;

    sBitsreamBuffer *BS = &(state->m_StreamData);

    Ipp32u si_main_data_begin;
    Ipp32u si_private_bits;
    Ipp32u (*si_part23Len)[2] = state->si_part23Len;
    Ipp32u (*si_bigVals)[2] = state->si_bigVals;
    Ipp16s (*si_globGain)[2] = state->si_globGain;
    Ipp32u (*si_sfCompress)[2] = state->si_sfCompress;
    Ipp32u (*si_winSwitch)[2] = state->si_winSwitch;
    Ipp32u (*si_blockType)[2] = state->si_blockType;
    Ipp32u (*si_mixedBlock)[2] = state->si_mixedBlock;
    Ipp32u (*si_pTableSelect)[2][3] = state->si_pTableSelect;
    Ipp16s (*si_pSubBlkGain)[2][3] = state->si_pSubBlkGain;
    Ipp32u (*si_reg0Cnt)[2] = state->si_reg0Cnt;
    Ipp32u (*si_reg1Cnt)[2] = state->si_reg1Cnt;
    Ipp32u (*si_preFlag)[2] = state->si_preFlag;
    Ipp32u (*si_sfScale)[2] = state->si_sfScale;
    Ipp32u (*si_cnt1TabSel)[2] = state->si_cnt1TabSel;
    Ipp32u *si_scfsi = state->si_scfsi;

    Ipp32s stereo = state->stereo;

    GET_BITS(BS, si_main_data_begin, 9, Ipp32s);

    if (stereo == 1)
        GET_BITS(BS, si_private_bits, 5, Ipp32s)
    else
        GET_BITS(BS, si_private_bits, 3, Ipp32s)

    state->si_main_data_begin = si_main_data_begin;
    state->si_private_bits = si_private_bits;

    for (ch = 0; ch < stereo; ch++)
        GET_BITS(BS, si_scfsi[ch], 4, Ipp32s);

    for (gr = 0; gr < 2; gr++) {
        for (ch = 0; ch < stereo; ch++) {
            GET_BITS(BS, tmp, 12 + 9, Ipp32s);
            si_part23Len[gr][ch] = tmp >> 9;
            si_bigVals[gr][ch] = tmp & 0x01ff;

            GET_BITS(BS, tmp, 8 + 4 + 1, Ipp32s);
            si_globGain[gr][ch] = (Ipp16s)(tmp >> 5);
            si_sfCompress[gr][ch] = (tmp >> 1) & 0x0f;
            si_winSwitch[gr][ch] = tmp & 1;

            GET_BITS(BS, tmp, 22, Ipp32s);
            if (si_winSwitch[gr][ch]) {
                si_blockType[gr][ch] = tmp >> 20;
                si_mixedBlock[gr][ch] = (tmp >> 19) & 1;
                si_pTableSelect[gr][ch][0] = (tmp >> 14) & 0x01f;
                si_pTableSelect[gr][ch][1] = (tmp >> 9) & 0x01f;
                si_pSubBlkGain[gr][ch][0] = (Ipp16s)(-8 * (Ipp32s)((tmp >> 6) & 0x07));
                si_pSubBlkGain[gr][ch][1] = (Ipp16s)(-8 * (Ipp32s)((tmp >> 3) & 0x07));
                si_pSubBlkGain[gr][ch][2] = (Ipp16s)(-8 * (Ipp32s)(tmp & 0x07));

                if (si_blockType[gr][ch] == 0)
                    return -2;  // wrong block type

                si_reg0Cnt[gr][ch] = 7 + (si_blockType[gr][ch] == 2 &&
                    si_mixedBlock[gr][ch] == 0);
                si_reg1Cnt[gr][ch] = 20 - si_reg0Cnt[gr][ch];
            } else {
                si_pTableSelect[gr][ch][0] = tmp >> 17;
                si_pTableSelect[gr][ch][1] = (tmp >> 12) & 0x01f;
                si_pTableSelect[gr][ch][2] = (tmp >> 7) & 0x01f;
                si_reg0Cnt[gr][ch] = (tmp >> 3) & 0x0f;
                si_reg1Cnt[gr][ch] = tmp & 7;

                si_blockType[gr][ch] = 0;
            }

            GET_BITS(BS, tmp, 3, Ipp32s);
            si_preFlag[gr][ch] = tmp >> 2;
            si_sfScale[gr][ch] = (tmp >> 1) & 1;
            si_cnt1TabSel[gr][ch] = tmp & 1;
        }
    }

    return 1;
}

/******************************************************************************
//  Name:
//    audio_data_LSF_LayerIII
//
//  Description:
//    read side information for LSF case
//
//  Input Arguments:
//    DC - point to sDecoderContext structure
//
//  Output Arguments:
//    DC - point to sDecoderContext structure, side info
//
//  Returns:
//    1 - all ok
//   -2 - wrong block type
//
******************************************************************************/

Ipp32s mp3dec_audio_data_LSF_LayerIII(MP3Dec_com *state) {
    Ipp32s gr = 0, ch;
    Ipp32s tmp;

    sBitsreamBuffer *BS = &(state->m_StreamData);

    Ipp32u (*si_part23Len)[2] = state->si_part23Len;
    Ipp32u (*si_bigVals)[2] = state->si_bigVals;
    Ipp16s (*si_globGain)[2] = state->si_globGain;
    Ipp32u (*si_sfCompress)[2] = state->si_sfCompress;
    Ipp32u (*si_winSwitch)[2] = state->si_winSwitch;
    Ipp32u (*si_blockType)[2] = state->si_blockType;
    Ipp32u (*si_mixedBlock)[2] = state->si_mixedBlock;
    Ipp32u (*si_pTableSelect)[2][3] = state->si_pTableSelect;
    Ipp16s (*si_pSubBlkGain)[2][3] = state->si_pSubBlkGain;
    Ipp32u (*si_reg0Cnt)[2] = state->si_reg0Cnt;
    Ipp32u (*si_reg1Cnt)[2] = state->si_reg1Cnt;
    Ipp32u (*si_sfScale)[2] = state->si_sfScale;
    Ipp32u (*si_cnt1TabSel)[2] = state->si_cnt1TabSel;

    Ipp32s stereo = state->stereo;

    GET_BITS(BS, state->si_main_data_begin, 8, Ipp32s);

    if (stereo == 1)
        GET_BITS(BS, state->si_private_bits, 1, Ipp32s)
    else
        GET_BITS(BS, state->si_private_bits, 2, Ipp32s)

    for (ch = 0; ch < stereo; ch++) {
        GET_BITS(BS, si_part23Len[gr][ch], 12, Ipp32s);
        GET_BITS(BS, si_bigVals[gr][ch], 9, Ipp32s);
        GET_BITS(BS, si_globGain[gr][ch], 8, Ipp16s);
        GET_BITS(BS, si_sfCompress[gr][ch], 9, Ipp32s);
        GET_BITS(BS, si_winSwitch[gr][ch], 1, Ipp32s);

        if (si_winSwitch[gr][ch]) {
            GET_BITS(BS, si_blockType[gr][ch], 2, Ipp32s);
            GET_BITS(BS, si_mixedBlock[gr][ch], 1, Ipp32s);
            GET_BITS(BS, si_pTableSelect[gr][ch][0], 5, Ipp32s);
            GET_BITS(BS, si_pTableSelect[gr][ch][1], 5, Ipp32s);
            GET_BITS(BS, tmp, 3, Ipp32s);
            si_pSubBlkGain[gr][ch][0] = (Ipp16s)(-8 * tmp);
            GET_BITS(BS, tmp, 3, Ipp32s);
            si_pSubBlkGain[gr][ch][1] = (Ipp16s)(-8 * tmp);
            GET_BITS(BS, tmp, 3, Ipp32s);
            si_pSubBlkGain[gr][ch][2] = (Ipp16s)(-8 * tmp);

            if (si_blockType[gr][ch] == 0)
                return -2;    // wrong block type

            si_reg0Cnt[gr][ch] = 7 + (si_blockType[gr][ch] == 2 &&
                si_mixedBlock[gr][ch] == 0);
            si_reg1Cnt[gr][ch] = 20 - si_reg0Cnt[gr][ch];
        } else {
            GET_BITS(BS, si_pTableSelect[gr][ch][0], 5, Ipp32s);
            GET_BITS(BS, si_pTableSelect[gr][ch][1], 5, Ipp32s);
            GET_BITS(BS, si_pTableSelect[gr][ch][2], 5, Ipp32s);
            GET_BITS(BS, si_reg0Cnt[gr][ch], 4, Ipp32s);
            GET_BITS(BS, si_reg1Cnt[gr][ch], 3, Ipp32s);

            si_blockType[gr][ch] = 0;
        }

        GET_BITS(BS, si_sfScale[gr][ch], 1, Ipp32s);
        GET_BITS(BS, si_cnt1TabSel[gr][ch], 1, Ipp32s);
    }

    return 1;
}

/******************************************************************************
//  Name:
//    GetScaleFactors_LSF
//
//  Description:
//    read scalefactors for LSF case
//
//  Input Arguments:
//    DC - point to sDecoderContext structure
//    ch - current channel
//
//  Output Arguments:
//    DC - point to sDecoderContext structure, side info
//
//  Returns:
//
******************************************************************************/

Ipp32s mp3dec_GetScaleFactorsL3_LSF(MP3Dec_com *state, Ipp32s ch) {
    Ipp32s i, j, k, sfb;
    Ipp32u scale_fact, int_scale_fact;
    Ipp16s scale_buffer[36];
    const Ipp8u* ptr = NULL;

    IppMP3FrameHeader *header = &(state->header);
    sBitsreamBuffer *BS = &(state->m_MainData);
    sScaleFactors *ScaleFactors = state->ScaleFactors;
    Ipp32u (*si_blockType)[2] = state->si_blockType;
    Ipp32u (*si_mixedBlock)[2] = state->si_mixedBlock;
    Ipp32u (*si_sfCompress)[2] = state->si_sfCompress;
    Ipp32u (*si_winSwitch)[2] = state->si_winSwitch;
    Ipp32u (*si_preFlag)[2] = state->si_preFlag;

    Ipp32s blocknumber = 0;
    Ipp32s blocktypenumber;
    Ipp32s *s_len = state->s_len;

    memset(scale_buffer, 0, sizeof(Ipp16s) * 36);

    scale_fact = si_sfCompress[0][ch];
    blocktypenumber = 0;
    if ((si_blockType[0][ch] == 2) && (si_mixedBlock[0][ch] == 0))
        blocktypenumber = 1;

    if ((si_blockType[0][ch] == 2) && (si_mixedBlock[0][ch] == 1))
        blocktypenumber = 2;

    if (!(((header->modeExt == 1) || (header->modeExt == 3)) && (ch == 1))) {
        if (scale_fact < 400) {
            s_len[0] = (scale_fact >> 4) / 5;
            s_len[1] = (scale_fact >> 4) % 5;
            s_len[2] = (scale_fact % 16) >> 2;
            s_len[3] = (scale_fact % 4);
            si_preFlag[0][ch] = 0;
            blocknumber = 0;
        } else if (scale_fact < 500) {
            s_len[0] = ((scale_fact - 400) >> 2) / 5;
            s_len[1] = ((scale_fact - 400) >> 2) % 5;
            s_len[2] = (scale_fact - 400) % 4;
            s_len[3] = 0;
            si_preFlag[0][ch] = 0;
            blocknumber = 1;
        } else if (scale_fact < 512) {
            s_len[0] = (scale_fact - 500) / 3;
            s_len[1] = (scale_fact - 500) % 3;
            s_len[2] = 0;
            s_len[3] = 0;
            si_preFlag[0][ch] = 1;
            blocknumber = 2;
        }
    } else {
        int_scale_fact = scale_fact >> 1;

        if (int_scale_fact < 180) {
            s_len[0] = int_scale_fact / 36;
            s_len[1] = (int_scale_fact % 36) / 6;
            s_len[2] = (int_scale_fact % 36) % 6;
            s_len[3] = 0;
            si_preFlag[0][ch] = 0;
            blocknumber = 3;
        } else if (int_scale_fact < 244) {
            s_len[0] = ((int_scale_fact - 180) % 64) >> 4;
            s_len[1] = ((int_scale_fact - 180) % 16) >> 2;
            s_len[2] = (int_scale_fact - 180) % 4;
            s_len[3] = 0;
            si_preFlag[0][ch] = 0;
            blocknumber = 4;
        } else if (int_scale_fact < 255) {
            s_len[0] = (int_scale_fact - 244) / 3;
            s_len[1] = (int_scale_fact - 244) % 3;
            s_len[2] = 0;
            s_len[3] = 0;
            si_preFlag[0][ch] = 0;
            blocknumber = 5;
        }
    }

    k = 0;
    ptr = mp3dec_nr_of_sfb[blocknumber][blocktypenumber];

    for (i = 0; i < 4; i++) {
        Ipp32s num = ptr[i];
        Ipp32s len = s_len[i];
        if (len) {
            for (j = 0; j < num; j++) {
                GET_BITS(BS, scale_buffer[k++], len, Ipp16s);
            }
        } else {
            for (j = 0; j < num; j++) {
                scale_buffer[k++] = 0;
            }
        }
    }

    k = 0;
    if (si_winSwitch[0][ch] && (si_blockType[0][ch] == 2)) {

        if (si_mixedBlock[0][ch]) {
            for (sfb = 0; sfb < 8; sfb++) {
                ScaleFactors[ch].l[sfb] = scale_buffer[k];
                k++;
            }
            for (sfb = 3; sfb < 12; sfb++)
                for (i = 0; i < 3; i++) {
                    ScaleFactors[ch].s[i][sfb] = scale_buffer[k];
                    k++;
                }

        } else {

            for (k = 0, sfb = 0; sfb < 12; sfb++)
                for (i = 0; i < 3; i++) {
                    ScaleFactors[ch].s[i][sfb] = scale_buffer[k];
                    k++;
                }
        }
    } else {
        for (sfb = 0; sfb < 21; sfb++) {
            ScaleFactors[ch].l[sfb] = scale_buffer[sfb];
        }
    }

    state->blocknumber = blocknumber;
    state->blocktypenumber = blocktypenumber;

    return 1;
}

#endif //UMC_ENABLE_XXX
