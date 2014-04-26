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

#include "align.h"
#include "mp3dec_own_fp.h"
#include "ipps.h"

static struct {
    Ipp32s  l[22];
    Ipp32s  s[39];
    Ipp32s  m[38];
} sfBandWidths[2][3] = {
    { /* MPEG 2 */
        { /* sampling frequency = 22.05 kHz */
            /* long bloks */
            {  6,  6,  6,  6,  6,  6,  8, 10, 12, 14,
              16, 20, 24, 28, 32, 38, 46, 52, 60, 68,
              58, 54
            },
            /* short bloks */
            {  4,  4,  4,  4,  4,  4,  4,  4,  4,  6,
               6,  6,  6,  6,  6,  8,  8,  8, 10, 10,
              10, 14, 14, 14, 18, 18, 18, 26, 26, 26,
              32, 32, 32, 42, 42, 42, 18, 18, 18
            },
            /* mixed bloks */
            {  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
               6,  6,  8,  8,  8, 10, 10, 10, 14, 14,
              14, 18, 18, 18, 26, 26, 26, 32, 32, 32,
              42, 42, 42, 18, 18, 18,  0,  0
            }
        },
        { /* sampling frequency = 24 kHz */
            /* long bloks */
            {  6,  6,  6,  6,  6,  6,  8, 10, 12, 14,
              16, 18, 22, 26, 32, 38, 46, 54, 62, 70,
              76, 36
            },
            /* short bloks */
            {  4,  4,  4,  4,  4,  4,  4,  4,  4,  6,
               6,  6,  8,  8,  8, 10, 10, 10, 12, 12,
              12, 14, 14, 14, 18, 18, 18, 24, 24, 24,
              32, 32, 32, 44, 44, 44, 12, 12, 12
            },
            /* mixed bloks */
            {  6,  6,  6,  6,  6,  6,  6,  6,  6,  8,
               8,  8, 10, 10, 10, 12, 12, 12, 14, 14,
              14, 18, 18, 18, 24, 24, 24, 32, 32, 32,
              44, 44, 44, 12, 12, 12,  0,  0
            }
        },
        { /* sampling frequency = 16 kHz       */
            /* long bloks */
            {  6,  6,  6,  6,  6,  6,  8, 10, 12, 14,
              16, 20, 24, 28, 32, 38, 46, 52, 60, 68,
              58, 54
            },
            /* short bloks */
            {  4,  4,  4,  4,  4,  4,  4,  4,  4,  6,
               6,  6,  8,  8,  8, 10, 10, 10, 12, 12,
              12, 14, 14, 14, 18, 18, 18, 24, 24, 24,
              30, 30, 30, 40, 40, 40, 18, 18, 18
            },
            /* mixed bloks */
            {  6,  6,  6,  6,  6,  6,  6,  6,  6,  8,
               8,  8, 10, 10, 10, 12, 12, 12, 14, 14,
              14, 18, 18, 18, 24, 24, 24, 30, 30, 30,
              40, 40, 40, 18, 18, 18,  0,  0
            }
        }
    },
    { /* MPEG 1 */
        { /* sampling frequency = 44.1 kHz */
            /* long bloks */
            {  4,  4,  4,  4,  4,  4,  6,  6,  8,  8,
              10, 12, 16, 20, 24, 28, 34, 42, 50, 54,
              76, 158
            },
            /* short bloks */
            {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
               4,  4,  6,  6,  6,  8,  8,  8, 10, 10,
              10, 12, 12, 12, 14, 14, 14, 18, 18, 18,
              22, 22, 22, 30, 30, 30, 56, 56, 56
            },
            /* mixed bloks */
            {  4,  4,  4,  4,  4,  4,  6,  6,  4,  4,
               4,  6,  6,  6,  8,  8,  8, 10, 10, 10,
              12, 12, 12, 14, 14, 14, 18, 18, 18, 22,
              22, 22, 30, 30, 30, 56, 56, 56
            }
        },
        { /* sampling frequency = 48 kHz */
            /* long bloks */
            {  4,  4,  4,  4,  4,  4,  6,  6,  6,  8,
              10, 12, 16, 18, 22, 28, 34, 40, 46, 54,
              54, 192
            },
            /* short bloks */
            {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
               4,  4,  6,  6,  6,  6,  6,  6, 10, 10,
              10, 12, 12, 12, 14, 14, 14, 16, 16, 16,
              20, 20, 20, 26, 26, 26, 66, 66, 66
            },
            /* mixed bloks */
            {  4,  4,  4,  4,  4,  4,  6,  6,  4,  4,
               4,  6,  6,  6,  6,  6,  6, 10, 10, 10,
              12, 12, 12, 14, 14, 14, 16, 16, 16, 20,
              20, 20, 26, 26, 26, 66, 66, 66
            }
        },
        { /* sampling frequency = 32 kHz */
            /* long bloks */
            {  4,  4,  4,  4,  4,  4,  6,  6,  8, 10,
              12, 16, 20, 24, 30, 38, 46, 56, 68, 84,
              102, 26
            },
            /* short bloks */
            {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
               4,  4,  6,  6,  6,  8,  8,  8, 12, 12,
              12, 16, 16, 16, 20, 20, 20, 26, 26, 26,
              34, 34, 34, 42, 42, 42, 12, 12, 12
            },
            /* mixed bloks */
            {  4,  4,  4,  4,  4,  4,  6,  6,  4,  4,
               4,  6,  6,  6,  8,  8,  8, 12, 12, 12,
              16, 16, 16, 20, 20, 20, 26, 26, 26, 34,
              34, 34, 42, 42, 42, 12, 12, 12
            }
        }
    }
};

static const Ipp32s sfBandNum[2][3][2] =
{
    { /* MPEG 2 */
        {22, 0}, {0, 13}, {6, 10}
    },
    { /* MPEG 1 */
        {22, 0}, {0, 13}, {8, 10}
    }
};

static const Ipp32f mpeg1_intensity[7] = {
    0.000000000f, 0.211324865f, 0.366025404f, 0.500000000f,
    0.633974596f, 0.788675135f, 1.000000000f
};

static const Ipp32f mpeg2_intensity[2][16] =
{
    {
        1.000000000f, 0.840896415f, 0.707106781f, 0.594603558f,
        0.500000000f, 0.420448208f, 0.353553391f, 0.297301779f,
        0.250000000f, 0.210224104f, 0.176776695f, 0.148650889f,
        0.125000000f, 0.105112052f, 0.088388348f, 0.074325445f
    },
    {
        1.000000000f, 0.707106781f, 0.500000000f, 0.353553391f,
        0.250000000f, 0.176776695f, 0.125000000f, 0.088388348f,
        0.062500000f, 0.044194174f, 0.031250000f, 0.022097087f,
        0.015625000f, 0.011048543f, 0.007812500f, 0.005524272f
    }
};

/* Table 3-B.6. Layer III Preemphasis (p.14 Annex_AB ISO/IEC 11172-3 )*/
static Ipp16s pretab[22] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 3, 2, 0
};

/* Table 3-B.9  Layer III coefficients for aliasing reduction (p.36 Annex_AB ISO/IEC 11172-3)*/
static Ipp32f cs[8] = {
    0.857492926f, 0.881741997f, 0.949628649f, 0.983314592f,
    0.995517816f, 0.999160558f, 0.999899195f, 0.999993155f,
};

static Ipp32f ca[8] = {
    -0.514495755f, -0.471731969f, -0.313377454f, -0.181913200f,
    -0.094574193f, -0.040965583f, -0.014198569f, -0.003699975f,
};

static Ipp32f mp3dec_imdct_table[4][36] = {
    {
        +7.85148978e-01f, +2.34947157e+00f, +3.89591312e+00f, +5.41270494e+00f,
        +6.88830233e+00f, +8.31147575e+00f, +9.67139339e+00f, +1.09577055e+01f,
        +1.21606236e+01f, +1.32709923e+01f, +1.42803612e+01f, +1.51810465e+01f,
        +1.59661961e+01f, +1.66298313e+01f, +1.71669044e+01f, +1.75733280e+01f,
        +1.78460083e+01f, +1.79828682e+01f, +1.79828682e+01f, +1.78460064e+01f,
        +1.75733280e+01f, +1.71669044e+01f, +1.66298313e+01f, +1.59661942e+01f,
        +1.51810455e+01f, +1.42803593e+01f, +1.32709904e+01f, +1.21606226e+01f,
        +1.09577045e+01f, +9.67139244e+00f, +8.31147385e+00f, +6.88830042e+00f,
        +5.41270304e+00f, +3.89591146e+00f, +2.34946990e+00f, +7.85147429e-01f,
    },
    {
        +7.85148978e-01f, +2.34947157e+00f, +3.89591312e+00f, +5.41270494e+00f,
        +6.88830233e+00f, +8.31147575e+00f, +9.67139339e+00f, +1.09577055e+01f,
        +1.21606236e+01f, +1.32709923e+01f, +1.42803612e+01f, +1.51810465e+01f,
        +1.59661961e+01f, +1.66298313e+01f, +1.71669044e+01f, +1.75733280e+01f,
        +1.78460083e+01f, +1.79828682e+01f, +1.80000000e+01f, +1.80000000e+01f,
        +1.80000000e+01f, +1.80000000e+01f, +1.80000000e+01f, +1.80000000e+01f,
        +1.78460064e+01f, +1.66298313e+01f, +1.42803593e+01f, +1.09577045e+01f,
        +6.88830042e+00f, +2.34946990e+00f, +0.00000000e+00f, +0.00000000e+00f,
        +0.00000000e+00f, +0.00000000e+00f, +0.00000000e+00f, +0.00000000e+00f,
    },
    {
        +7.83157229e-01f, +2.29610062e+00f, +3.65256858e+00f, +4.76012039e+00f,
        +5.54327726e+00f, +5.94866943e+00f, +5.94866896e+00f, +5.54327679e+00f,
        +4.76011992e+00f, +3.65256834e+00f, +2.29610014e+00f, +7.83156633e-01f,
        +7.83157229e-01f, +2.29610062e+00f, +3.65256858e+00f, +4.76012039e+00f,
        +5.54327726e+00f, +5.94866943e+00f, +5.94866896e+00f, +5.54327679e+00f,
        +4.76011992e+00f, +3.65256834e+00f, +2.29610014e+00f, +7.83156633e-01f,
        +7.83157229e-01f, +2.29610062e+00f, +3.65256858e+00f, +4.76012039e+00f,
        +5.54327726e+00f, +5.94866943e+00f, +5.94866896e+00f, +5.54327679e+00f,
        +4.76011992e+00f, +3.65256834e+00f, +2.29610014e+00f, +7.83156633e-01f,
    },
    {
        +0.00000000e+00f, +0.00000000e+00f, +0.00000000e+00f, +0.00000000e+00f,
        +0.00000000e+00f, +0.00000000e+00f, +2.34947157e+00f, +6.88830233e+00f,
        +1.09577055e+01f, +1.42803612e+01f, +1.66298313e+01f, +1.78460083e+01f,
        +1.80000000e+01f, +1.80000000e+01f, +1.80000000e+01f, +1.80000000e+01f,
        +1.80000000e+01f, +1.80000000e+01f, +1.79828682e+01f, +1.78460064e+01f,
        +1.75733280e+01f, +1.71669044e+01f, +1.66298313e+01f, +1.59661942e+01f,
        +1.51810455e+01f, +1.42803593e+01f, +1.32709904e+01f, +1.21606226e+01f,
        +1.09577045e+01f, +9.67139244e+00f, +8.31147385e+00f, +6.88830042e+00f,
        +5.41270304e+00f, +3.89591146e+00f, +2.34946990e+00f, +7.85147429e-01f,
    }
};

/******************************************************************************
//  Name:
//    initialize_huffman
//
//  Description:
//    initialize huffman tables by using ippAC functionality
//
//  Input Arguments:
//    huff_table - point to array of Huff_table structures.
//    huff_table[i]->ptable is the table in user format.
//
//  Output Arguments:
//    huff_table - point to array of Huff_table structures.
//    huff_table[i]->phuftable is the table in internal format.
//
//  Returns:
//    -
//
******************************************************************************/

Ipp32s mp3dec_initialize_huffman(sHuffmanTable *huff_table, Ipp8u *mem, Ipp32s *size_all)
{
    Ipp32s  i, ts, size = 0;

    if (huff_table) {
      for(i = 0; i < 16; i++)
        huff_table[i].linbits = 0;
      huff_table[16].linbits = 1;
      huff_table[17].linbits = 2;
      huff_table[18].linbits = 3;
      huff_table[19].linbits = 4;
      huff_table[20].linbits = 6;
      huff_table[21].linbits = 8;
      huff_table[22].linbits = 10;
      huff_table[23].linbits = 13;
      huff_table[24].linbits = 4;
      huff_table[25].linbits = 5;
      huff_table[26].linbits = 6;
      huff_table[27].linbits = 7;
      huff_table[28].linbits = 8;
      huff_table[29].linbits = 9;
      huff_table[30].linbits = 11;
      huff_table[31].linbits = 13;
      huff_table[32].linbits = 0;
      huff_table[33].linbits = 0;
    }

    for (i = 1; i <= 16; i++) {
        if (mp3dec_VLCBooks[i]) {
            ippsVLCDecodeGetSize_32s(mp3dec_VLCBooks[i], mp3dec_VLCTableSizes[i],
              mp3dec_VLCSubTablesSizes[i],
              mp3dec_VLCNumSubTables[i], &ts);
            size += ts;
            if (mem) {
              huff_table[i].phuftable = (IppsVLCDecodeSpec_32s *)mem;
              mem += ts;
              ippsVLCDecodeInit_32s(mp3dec_VLCBooks[i], mp3dec_VLCTableSizes[i],
                  mp3dec_VLCSubTablesSizes[i],
                  mp3dec_VLCNumSubTables[i],
                  (IppsVLCDecodeSpec_32s *) (huff_table[i].
                  phuftable));
            }
        }
    }

    if (huff_table) {
      for (i = 17; i < 24; i++) {
          huff_table[i].phuftable = huff_table[16].phuftable;
      }
    }

    ippsVLCDecodeGetSize_32s(mp3dec_VLCBooks[24], mp3dec_VLCTableSizes[24],
      mp3dec_VLCSubTablesSizes[24], mp3dec_VLCNumSubTables[24], &ts);
    size += ts;
    if (mem) {
      huff_table[24].phuftable = (IppsVLCDecodeSpec_32s *)mem;
      mem += ts;
      ippsVLCDecodeInit_32s(mp3dec_VLCBooks[24], mp3dec_VLCTableSizes[24],
          mp3dec_VLCSubTablesSizes[24], mp3dec_VLCNumSubTables[24],
          (IppsVLCDecodeSpec_32s *) (huff_table[24].
          phuftable));
    }

    if (huff_table) {
      for (i = 25; i < 32; i++) {
          huff_table[i].phuftable = huff_table[24].phuftable;
      }
    }

    for (i = 32; i <= 33; i++) {
      if (mp3dec_VLCBooks[i]) {
        ippsVLCDecodeGetSize_32s(mp3dec_VLCBooks[i], mp3dec_VLCTableSizes[i],
          mp3dec_VLCSubTablesSizes[i],
          mp3dec_VLCNumSubTables[i], &ts);
        size += ts;
        if (mem) {
          huff_table[i].phuftable = (IppsVLCDecodeSpec_32s *)mem;
          mem += ts;
          ippsVLCDecodeInit_32s(mp3dec_VLCBooks[i], mp3dec_VLCTableSizes[i],
            mp3dec_VLCSubTablesSizes[i],
            mp3dec_VLCNumSubTables[i],
            (IppsVLCDecodeSpec_32s *) (huff_table[i].
            phuftable));
        }
      }
    }

    if (size_all)
      *size_all = size;

    return 1;
}

/******************************************************************************
//  Name:
//    Huffmancodebits
//
//  Description:
//    Huffman decoding of spectral samples
//
//  Input Arguments:
//    DC - point to sDecoderContext structure
//    gr - number of granule
//    ch - number of channel
//
//  Output Arguments:
//    smpl_xs[ch] - array of decoded samples
//
//  Returns:
//    1 - all ok
//
******************************************************************************/

Ipp32s mp3dec_Huffmancodebits(MP3Dec_com *state, Ipp32s gr, Ipp32s ch) {
#if !defined(ANDROID)
    IPP_ALIGNED_ARRAY(32, Ipp16s, pDst, LEN_MDCT_LINE);
#else
    Ipp16s pDst[LEN_MDCT_LINE];
#endif
    Ipp32s  reg[3];
    IppsVLCDecodeSpec_32s *pVLCDecSpec;
    Ipp32s  shift, offset, mask;
    sBitsreamBuffer *BS = &(state->m_MainData);
    Ipp8u  *pSrc;
    Ipp32s  bitoffset;
    IppMP3FrameHeader *header = &(state->header);
    sHuffmanTable *huff_table = state->huff_table;
    Ipp32s *non_zero = state->non_zero;
    Ipp32s  part2_start = state->part2_start;

    Ipp32u (*si_part23Len)[2] = state->si_part23Len;
    Ipp32u (*si_bigVals)[2] = state->si_bigVals;
    Ipp32u (*si_winSwitch)[2] = state->si_winSwitch;
    Ipp32u (*si_blockType)[2] = state->si_blockType;
    Ipp32u (*si_pTableSelect)[2][3] = state->si_pTableSelect;
    Ipp32u (*si_reg0Cnt)[2] = state->si_reg0Cnt;
    Ipp32u (*si_reg1Cnt)[2] = state->si_reg1Cnt;
    Ipp32u (*si_cnt1TabSel)[2] = state->si_cnt1TabSel;
    Ipp32s  j, i, idx, nbits;

    sampleshort *smpl_xs = state->smpl_xs;       // out of huffman

    // the block_type is only used if the window_switching_flag is set to 1.


    if (!si_winSwitch[gr][ch] || (si_blockType[gr][ch] != 2 &&
      !(state->mpg25 == 2 && header->samplingFreq == 2))) {
        reg[0] =
            mp3dec_sfBandIndex_l[header->id + state->mpg25][header->samplingFreq][si_reg0Cnt[gr][ch] + 1];
        reg[1] =
            mp3dec_sfBandIndex_l[header->id + state->mpg25][header->samplingFreq][si_reg0Cnt[gr][ch] +
            si_reg1Cnt[gr][ch] + 2];
    } else  {
      if (si_blockType[gr][ch] == 2) {
        if (state->mpg25 == 2 && header->samplingFreq == 2)
          reg[0] = 72;
        else
          reg[0] = 36;
      } else
        reg[0] = 108;
      reg[1] = LEN_MDCT_LINE;
    }

    reg[2] = si_bigVals[gr][ch] * 2;
    if (reg[2] > LEN_MDCT_LINE)   // reg[2] upper-bounded by LEN_MDCT_LINE
      reg[2] = LEN_MDCT_LINE;

    if (reg[0] > LEN_MDCT_LINE)   // reg[0] upper-bounded by LEN_MDCT_LINE
        reg[0] = LEN_MDCT_LINE;
    if (reg[1] > reg[2])        // reg[1] upper-bounded by reg[2]
        reg[1] = reg[2];
    if (reg[0] > reg[2])        // reg[0] upper-bounded by reg[2]
        reg[0] = reg[2];

    pSrc = (Ipp8u *)BS->pCurrent_dword + ((32 - BS->nBit_offset) >> 3);
    bitoffset = (32 - BS->nBit_offset) & 0x7;

    i = 0;
    for (j = 0; j < 3; j++) {
        Ipp32s  linbits, ii;
        Ipp16s *qp;

        idx = si_pTableSelect[gr][ch][j];
        if (idx == 4 || idx == 14) {
          return 1;
        }

        pVLCDecSpec = (IppsVLCDecodeSpec_32s *) huff_table[idx].phuftable;
        linbits = huff_table[idx].linbits;
        shift = mp3dec_VLCShifts[idx];
        offset = mp3dec_VLCOffsets[idx];
        mask = (1 << (shift)) - 1;

        if (idx) {
            if (reg[j] < i) i = reg[j];
            if (linbits == 0) {
                ippsVLCDecodeBlock_1u16s(&pSrc, &bitoffset, pDst, (reg[j] - i) >> 1,
                    pVLCDecSpec);
                qp = &(*smpl_xs)[ch][i];
                for (ii = 0; ii < (reg[j] - i) >> 1; ii++) {
                    Ipp32s  tmp = pDst[ii];

                    qp[0] = (Ipp16s)(tmp >> shift);
                    qp[1] = (Ipp16s)((tmp & mask) - offset);

                    qp += 2;
                }
            } else {
                ippsVLCDecodeEscBlock_MP3_1u16s(&pSrc, &bitoffset, linbits,
                    &(*smpl_xs)[ch][i], (reg[j] - i),
                    pVLCDecSpec);
            }
            i = reg[j];

        } else {
            for (; i < reg[j]; i += 2) {
                (*smpl_xs)[ch][i] = 0;
                (*smpl_xs)[ch][i + 1] = 0;
            }
        }
    }   // for

    BS->pCurrent_dword =
        (Ipp32u *)(pSrc -
        (((size_t)(pSrc) & 3) - ((size_t)(BS->pCurrent_dword) & 3)));
    BS->dword = BSWAP(BS->pCurrent_dword[0]);
    BS->nBit_offset =
        (Ipp32s)(32 - ((pSrc - (Ipp8u *)BS->pCurrent_dword) << 3) - bitoffset);

    idx = si_cnt1TabSel[gr][ch] + 32;

    nbits =
        (Ipp32s)(si_part23Len[gr][ch] -
        (((BS->pCurrent_dword - BS->pBuffer) * 32 + 32 -
        BS->nBit_offset) - part2_start));
    /*
    if (nbits < 0) {
        vm_debug_trace2(VM_DEBUG_ALL, VM_STRING("Frame %d, granule %d,"), state->m_frame_num, gr);
        vm_debug_trace2(VM_DEBUG_ALL, VM_STRING(" channel %d, error: nbits %d < 0 before zero-one part\n"), ch, nbits);
    }
    */
    pVLCDecSpec = (IppsVLCDecodeSpec_32s *) huff_table[idx].phuftable;
    shift = mp3dec_VLCShifts[idx];
    offset = mp3dec_VLCOffsets[idx];
    mask = (1 << (shift)) - 1;

    // decoding continues until all huffman bits have been decoded or until LEN_MDCT_LINE
    // frequency lines have been decoded
    while ((nbits > 0) && ((i + 4) <= LEN_MDCT_LINE)) {
        Ipp8u  *saved_pSrc;
        Ipp32s  saved_bitoffset;
        Ipp32s  decoded_bits;
        Ipp32s  tmp;

        saved_pSrc = pSrc;
        saved_bitoffset = bitoffset;

        ippsVLCDecodeOne_1u16s(&pSrc, &bitoffset, pDst, pVLCDecSpec);

        decoded_bits = (Ipp32s)(((pSrc - saved_pSrc) << 3) + (bitoffset - saved_bitoffset));
        nbits -= decoded_bits;

        tmp = pDst[0];

        (*smpl_xs)[ch][i] = (Ipp16s)(tmp >> (3 * shift));
        (*smpl_xs)[ch][i + 1] = (Ipp16s)(((tmp >> (2 * shift)) & mask) - offset);
        (*smpl_xs)[ch][i + 2] = (Ipp16s)(((tmp >> (shift)) & mask) - offset);
        (*smpl_xs)[ch][i + 3] = (Ipp16s)((tmp & mask) - offset);

        i += 4;
    }

    if (nbits < 0) {
        i -= 4;
        (*smpl_xs)[ch][i] = 0;
        (*smpl_xs)[ch][i + 1] = 0;
        (*smpl_xs)[ch][i + 2] = 0;
        (*smpl_xs)[ch][i + 3] = 0;
    }

    j = i;

    for (; i < LEN_MDCT_LINE; i++)
        (*smpl_xs)[ch][i] = 0;

    while(j > 0 && (*smpl_xs)[ch][j - 1] == 0)
        j--;
    if (j < 0) j = 0;
    non_zero[ch] = j;

    BS->pCurrent_dword =
        (Ipp32u *)(pSrc -
        (((size_t)(pSrc) & 3) - ((size_t)(BS->pCurrent_dword) & 3)));
    BS->dword = BSWAP(BS->pCurrent_dword[0]);
    BS->nBit_offset =
        (Ipp32s)(32 - ((pSrc - (Ipp8u *)BS->pCurrent_dword) << 3) - bitoffset);

    return 0;
}

/******************************************************************************
//  Name:
//    Dequant
//
//  Description:
//    Decoding of coded subband samples in order to recover the original
//    quantized values
//
//  Input Arguments:
//    DC - point to sDecoderContext structure
//    gr - number of granule
//    ch - number of channel
//
//  Output Arguments:
//    smpl_xr[ch] - array of recovered samples
//
//  Returns:
//    1 - all ok
//
******************************************************************************/
static Ipp32s mp3dec_Dequant(MP3Dec *state, Ipp32s gr, Ipp32s ch)
{
    IppMP3FrameHeader *header = &(state->com.header);
    const Ipp32s  *sfb_long = mp3dec_sfBandIndex_l[header->id + state->com.mpg25][header->samplingFreq];
    Ipp32f  (*GlobalScaleFactor)[2] = state->GlobalScaleFactor;
    Ipp32s  *non_zero = state->com.non_zero;
    Ipp32s  count = non_zero[ch];
    sScaleFactors *ScaleFactors = state->com.ScaleFactors;
    Ipp32u  (*si_blockType)[2] = state->com.si_blockType;
    Ipp32u  (*si_mixedBlock)[2] = state->com.si_mixedBlock;
    Ipp16s  (*si_pSubBlkGain)[2][3] = state->com.si_pSubBlkGain;
    Ipp32u  (*si_preFlag)[2] = state->com.si_preFlag;
    Ipp32u  (*si_sfScale)[2] = state->com.si_sfScale;
    samplefloat *smpl_xr = state->smpl_xr;       /* out of dequantizer */
    sampleshort *smpl_xs = state->com.smpl_xs;       // out of huffman
    Ipp32f  xx;    // xx it is recalculated global gain
    Ipp16s  scl;
    Ipp32s  sfb, j, last_sb;

    scl = -2 - 2 * (Ipp16s)si_sfScale[gr][ch];
    xx = GlobalScaleFactor[gr][ch];

    ippsPow43_16s32f((*smpl_xs)[ch], (*smpl_xr)[ch], state->com.non_zero[ch]);
    ippsZero_32f((*smpl_xr)[ch] + state->com.non_zero[ch], LEN_MDCT_LINE - state->com.non_zero[ch]);

    if (si_blockType[gr][ch] != 2) {
        Ipp32f  tmp22[22];
        Ipp16s  sf_tmp[21];

        if (si_preFlag[gr][ch] == 0) {
            ippsMulC_16s_Sfs(&(ScaleFactors[ch].l[0]), scl, sf_tmp, 21, 0);
        } else {
            ippsAdd_16s_Sfs(&(ScaleFactors[ch].l[0]), pretab, sf_tmp, 21, 0);
            ippsMulC_16s_ISfs(scl, sf_tmp, 21, 0);
        }

        last_sb = 22;
        for (j = 1; j <= 22; j++) {
          if (sfb_long[j] >= state->com.non_zero[ch]) {
            last_sb = j;
            break;
          }
        }

        ippsCalcSF_16s32f(sf_tmp, 0, tmp22, last_sb > 21 ? 21 : last_sb);
        tmp22[21] = 1;
        ippsMulC_32f_I(xx, tmp22, last_sb);
        ippsScale_32f_I((*smpl_xr)[ch], tmp22, sfb_long, last_sb);
    } else {
        const Ipp32s* sfb_short = mp3dec_sfBandIndex_s[header->id + state->com.mpg25][header->samplingFreq];
        Ipp32f  tmpSF[3], tmp13[3][13];
        Ipp16s  sf_tmp[13];
        Ipp32s  n, j, wnd;
        Ipp32s  sfbStart, sfbEnd;

        sfbEnd = 0; sfbStart = 0;
        if (si_mixedBlock[gr][ch]) {
            if (header->id == 1) {
                sfbEnd = 8;
            } else {
                sfbEnd = 6;
            }
            sfbStart = 3;

            ippsMulC_16s_Sfs(&ScaleFactors[ch].l[0], scl, &sf_tmp[0], sfbEnd, 0);

            ippsCalcSF_16s32f(sf_tmp, 0, tmp13[0], sfbEnd);
            ippsMulC_32f_I(xx, tmp13[0], sfbEnd);
            ippsScale_32f_I((*smpl_xr)[ch], tmp13[0], sfb_long, sfbEnd);
        }

        ippsCalcSF_16s32f(si_pSubBlkGain[gr][ch], 0, tmpSF, 3);

        for (wnd = 0; wnd < 3; wnd++) {
            ippsMulC_16s_Sfs(&ScaleFactors[ch].s[wnd][sfbStart], scl, &sf_tmp[sfbStart], 13 - sfbStart, 0);

            ippsCalcSF_16s32f(&sf_tmp[sfbStart], 0, &tmp13[wnd][sfbStart], 13 - sfbStart);
            ippsMulC_32f_I(xx * tmpSF[wnd], &tmp13[wnd][sfbStart], 13 - sfbStart);
        }

        j = sfb_long[sfbEnd];
        for (sfb = sfbStart; sfb <= 12; sfb++) {
            n = sfb_short[sfb + 1] - sfb_short[sfb];
            for (wnd = 0; wnd < 3; wnd++) {
                ippsScale_32f_I(&((*smpl_xr)[ch][j]) - sfb_short[sfb], &tmp13[wnd][sfb], sfb_short + sfb, 1);
                j += n;
            }
        } // for
    }

    while(count > 0 && ((Ipp32s *)((*smpl_xr)[ch]))[count - 1] == 0)
        count --;
    if (count < 0) count = 0;
    state->com.non_zero[ch] = count;

    ippsMulC_32f_I(32768, (*smpl_xr)[ch], count);

    return 1;
}

/******************************************************************************
//  Name:
//    Reordering
//
//  Description:
//
//
//
//  Input Arguments:
//    DC - point to sDecoderContext structure
//    gr - number of granule
//    ch - number of channel
//
//  Output Arguments:
//    smpl_ro - array of reordered samples
//
//  Returns:
//    1 - all ok
//
******************************************************************************/
static Ipp32s mp3dec_Reordering(MP3Dec *state, Ipp32s gr, Ipp32s ch)
{
#if !defined(ANDROID)
    IPP_ALIGNED_ARRAY(32, Ipp32f, tmp_buf, LEN_MDCT_LINE);
#else
    Ipp32f tmp_buf[LEN_MDCT_LINE];
#endif
    Ipp32f  *dstPtr, *srcPtr;
    Ipp32s  i, sfb, shift, startSfb, wl = 0;
    Ipp32s  nz = state->com.non_zero[ch];

    IppMP3FrameHeader *header = &(state->com.header);
    Ipp32u (*si_blockType)[2] = state->com.si_blockType;
    Ipp32u (*si_mixedBlock)[2] = state->com.si_mixedBlock;
    Ipp32u (*si_winSwitch)[2] = state->com.si_winSwitch;
    samplefloat *smpl_xr = state->smpl_xr;       /* out of dequantizer */

    if (si_winSwitch[gr][ch] && (si_blockType[gr][ch] == 2)) {
        const Ipp32s *sfbShort = mp3dec_sfBandIndex_s[header->id + state->com.mpg25][header->samplingFreq];
        if (si_mixedBlock[gr][ch]) {
            shift = sfbShort[3] * 3;
            startSfb = 3;
        } else {
            shift = 0;
            startSfb = 0;
        }
        srcPtr = (*smpl_xr)[ch] + shift;
        dstPtr = tmp_buf;
        for (sfb = startSfb; sfb < 13; sfb++) {
            Ipp32s w = sfbShort[sfb + 1] - sfbShort[sfb];
            for (i = 0; i < w; i++) {
                dstPtr[0] = srcPtr[0];
                dstPtr[1] = srcPtr[w];
                dstPtr[2] = srcPtr[2*w];
                dstPtr += 3; srcPtr++;
            }
            srcPtr += 2*w;
            if (sfbShort[sfb] <= nz)
              wl = w;
        }
        ippsCopy_32f(tmp_buf, (*smpl_xr)[ch] + shift, LEN_MDCT_LINE - shift);

        state->com.non_zero[ch] += wl*3;
        if (state->com.non_zero[ch] > LEN_MDCT_LINE)
          state->com.non_zero[ch] = LEN_MDCT_LINE;
    }

    return 1;
}

/******************************************************************************
//  Name:
//    Antialiasing
//
//  Description:
//
//
//
//  Input Arguments:
//    DC - point to sDecoderContext structure
//    gr - number of granule
//    ch - number of channel
//
//  Output Arguments:
//    smpl_re - array of spectrum samples
//
//  Returns:
//    1 - all ok
//
******************************************************************************/
static Ipp32s mp3dec_Antialiasing(MP3Dec *state, Ipp32s gr, Ipp32s ch)
{
    Ipp32s sblim, freqline, i, idx1, idx2;
    Ipp32f bu, bd;

    Ipp32u (*si_blockType)[2] = state->com.si_blockType;
    Ipp32u (*si_mixedBlock)[2] = state->com.si_mixedBlock;
    Ipp32u (*si_winSwitch)[2] = state->com.si_winSwitch;
    samplefloat *smpl_re = state->smpl_re;       // out of huffman
    samplefloat *smpl_ro = state->smpl_ro;       // out of huffman
    Ipp32s nz, freqline_end;

    if (si_winSwitch[gr][ch] && (si_blockType[gr][ch] == 2) &&
        !si_mixedBlock[gr][ch])
        return 1;
    if (si_winSwitch[gr][ch] && (si_blockType[gr][ch] == 2) &&
        si_mixedBlock[gr][ch])
        return 1;
    else
        sblim = 31;

    freqline_end = sblim * 18;
    nz = state->com.non_zero[ch] - 10;
    if (freqline_end > nz)
        freqline_end = nz;

    for (freqline = 0; freqline < freqline_end; freqline += 18) {
        for (i = 0; i < 8; i++) {
            idx1 = freqline + 17 - i;
            idx2 = freqline + 18 + i;

            bu = (*smpl_ro)[ch][idx1];
            bd = (*smpl_ro)[ch][idx2];

            (*smpl_re)[ch][idx1] = (bu * cs[i]) - (bd * ca[i]);
            (*smpl_re)[ch][idx2] = (bd * cs[i]) + (bu * ca[i]);
        }
    }

    state->com.non_zero[ch] += 14;
    if (state->com.non_zero[ch] > LEN_MDCT_LINE)
      state->com.non_zero[ch] = LEN_MDCT_LINE;

    return 1;
}

/******************************************************************************
//  Name:
//    Joint stereo
//
//  Description:
//    MS stereo & intensity streo implementation//
//
//  Input Arguments:
//
//
//  Output Arguments:
//
//
//  Returns:
//    1 - all ok
//
******************************************************************************/

static Ipp32s mp3dec_JointStereo(MP3Dec *state, Ipp32s gr)
{
    const Ipp32f mult = 0.7071067811865475244f;
    Ipp32s isPos[39], found[3];
    Ipp32s *width;
    Ipp32s *scalefPtr, *widthPtr;
    Ipp32f *ptrL, *ptrR;
    Ipp32f li, ri;
    Ipp32f k[2];
    Ipp32s illegal;
    Ipp32s numSfbLong, numSfbShort, numSfb;
    Ipp32s sfbStart, sfbEnd, numBlock;
    Ipp32s i, j, sfb, w;

    Ipp32s blocknumber = state->com.blocknumber;
    Ipp32s blocktypenumber = state->com.blocktypenumber;
    IppMP3FrameHeader *header = &(state->com.header);
    Ipp32s intensity = state->com.intensity;
    Ipp32s ms_stereo = state->com.ms_stereo;
    Ipp32s *s_len = state->com.s_len;
    Ipp32u (*si_blockType)[2] = state->com.si_blockType;
    Ipp32u (*si_mixedBlock)[2] = state->com.si_mixedBlock;
    Ipp32u (*si_sfCompress)[2] = state->com.si_sfCompress;
    sScaleFactors *ScaleFactors = state->com.ScaleFactors;
    samplefloat *smpl_xr = state->smpl_xr; /* out of dequantizer */
    Ipp32s nz = state->com.non_zero[0];
    Ipp32s f20 = 0, f11 = 0;

    if (!intensity) {
        ptrL = &((*smpl_xr)[0][0]);
        ptrR = &((*smpl_xr)[1][0]);

        for (i = 0; i < nz; i++) {
            li = mult * (ptrL[0] + ptrR[0]);
            ri = mult * (ptrL[0] - ptrR[0]);
            ptrL[0] = li;
            ptrR[0] = ri;
            ptrL++; ptrR++;
        }
        return 1;
    }

    scalefPtr = isPos;
    if (si_blockType[gr][1] == 2) {
        if (si_mixedBlock[gr][1]) {
            numSfbLong = sfBandNum[header->id][2][0];
            numSfbShort = sfBandNum[header->id][2][1];
            width = sfBandWidths[header->id][header->samplingFreq].m;
            for (sfb = 0; sfb < numSfbLong; sfb++) {
                *scalefPtr = ScaleFactors[1].l[sfb];
                scalefPtr++;
            }
            for (sfb = 3; sfb < 12; sfb++) {
                for (i = 0; i < 3; i++) {
                    *scalefPtr = ScaleFactors[1].s[i][sfb];
                    scalefPtr++;
                }
            }
        } else {
            numSfbLong = sfBandNum[header->id][1][0];
            numSfbShort = sfBandNum[header->id][1][1];
            width = sfBandWidths[header->id][header->samplingFreq].s;
            for (sfb = 0; sfb < 12; sfb++) {
                for (i = 0; i < 3; i++) {
                    *scalefPtr = ScaleFactors[1].s[i][sfb];
                    scalefPtr++;
                }
            }
        }
        for (i = 0; i < 3; i++) {
            *scalefPtr = 0;
            scalefPtr++;
        }
    } else {
        numSfbLong = sfBandNum[header->id][0][0];
        numSfbShort = sfBandNum[header->id][0][1];
        width = sfBandWidths[header->id][header->samplingFreq].l;
        for (sfb = 0; sfb < 21; sfb++) {
            *scalefPtr = ScaleFactors[1].l[sfb];
            scalefPtr++;
        }
        *scalefPtr = 0;
    }

    if (header->id) {
        illegal = 7;
    } else {
        illegal = 100;
    }

    numSfb = numSfbLong + numSfbShort * 3;
    widthPtr = width + numSfb - 1;
    ptrR = &((*smpl_xr)[1][LEN_MDCT_LINE]);

    found[0] = found[1] = found[2] = 0;
    sfbStart = numSfb;
    sfbEnd = numSfbLong;
    numBlock = 3;
    for (j = 0; j < 2; j++) {
        for (sfb = sfbStart - 1; sfb >= sfbEnd; sfb-=numBlock) {
            for (i = 0; i < numBlock; i++) {
                Ipp32f tmp;
                w = *widthPtr;
                widthPtr--;
                ptrR -= w;
                if (found[i] == 0) {
                    ippsDotProd_32f(ptrR, ptrR, w, &tmp);
                    if (tmp > 0) {
                        isPos[sfb - i] = illegal;
                        found[i] = 1;
                        if (numSfbLong && sfb == 20) f20 = 1;
                        if (numSfbShort && sfb >= 33 && sfb <= 35) f11 = 1;
                    }
                } else {
                    isPos[sfb - i] = illegal;
                }
            }
        }
        found[0] += (found[1] + found[2]);
        sfbStart = sfbEnd;
        sfbEnd = 0;
        numBlock = 1;
    }

    if (header->id == 0) {
        const Ipp8u* ptr = mp3dec_nr_of_sfb[blocknumber][blocktypenumber];

        scalefPtr = isPos;

        for (i = 0; i < 4; i++) {
            Ipp32s num = ptr[i];
            Ipp32s len = s_len[i];

            if (len) {
                Ipp32s tmp = (1 << len) - 1;
                for (sfb = 0; sfb < num; sfb++) {
                    if (*scalefPtr == tmp) {
                        *scalefPtr = illegal;
                    }
                    scalefPtr++;
                }
            } else {
                scalefPtr += num;
            }
        }
    }

    if (si_blockType[gr][1] == 2) {
      if (f11) {
        isPos[numSfb - 1] = isPos[numSfb - 2] = isPos[numSfb - 3] =  0;
      } else {
        isPos[numSfb - 1] =  isPos[numSfb - 4];
        isPos[numSfb - 2] =  isPos[numSfb - 5];
        isPos[numSfb - 3] =  isPos[numSfb - 6];
      }
    } else {
      isPos[numSfb - 1] = f20 ? 0 : isPos[numSfb - 2];
    }

    ptrL = &((*smpl_xr)[0][0]);
    ptrR = &((*smpl_xr)[1][0]);

    for (sfb = 0; sfb < numSfb; sfb++) {
        w = width[sfb];
        if (isPos[sfb] == illegal) {
            if (ms_stereo) {
                for (i = 0; i < w; i++) {
                    li = mult * (ptrL[0] + ptrR[0]);
                    ri = mult * (ptrL[0] - ptrR[0]);
                    ptrL[0] = li;
                    ptrR[0] = ri;
                    ptrL++; ptrR++;
                }
            } else {
                ptrL += w;
                ptrR += w;
            }
        } else {
            if (header->id) {
                if (isPos[sfb] > 6) {
                    isPos[sfb] = 6;
                }
                k[0]  = mpeg1_intensity[isPos[sfb]];
                k[1]  = mpeg1_intensity[6 - isPos[sfb]];
            } else {
                Ipp32s tmp0, tmp1;
                tmp0 = isPos[sfb] & 1;
                tmp1 = (isPos[sfb] + 1) >> 1;

                k[tmp0] = 1;
                k[1-tmp0] = mpeg2_intensity[(si_sfCompress[0][1] & 1)][tmp1];
            }

            for (i = 0; i < w; i++) {
                li = k[0] * ptrL[0];
                ri = k[1] * ptrL[0];
                ptrL[0] = li;
                ptrR[0] = ri;
                ptrL++; ptrR++;
            }
        }
    }
    return 1;
}

/******************************************************************************
//  Name:
//    IMDCT
//
//  Description:
//    inverse modifid cosine tranformation. overlaping and adding.
//
//
//  Input Arguments:
//    DC - point to sDecoderContext structure
//    gr - number of granule
//    ch - number of channel
//
//  Output Arguments:
//    smpl_rw - prepared values for synthesis filterbank
//
//  Returns:
//    1 - all ok
//
******************************************************************************/
static Ipp32s mp3dec_IMDCT(MP3Dec *state, Ipp32s gr, Ipp32s ch)
{
    Ipp32s bt;
    Ipp32s wnd, sb;
    Ipp32s j;

#if !defined(ANDROID)
    IPP_ALIGNED_ARRAY(32, Ipp32f, rawout, 36);
    IPP_ALIGNED_ARRAY(32, Ipp32f, tmp, 36);
    IPP_ALIGNED_ARRAY(32, Ipp32f, tmpBuf, LEN_MDCT_LINE);
#else
    Ipp32f rawout[36], tmp[36], tmpBuf[LEN_MDCT_LINE];
#endif
    Ipp32f *prev_ptr;
    Ipp32f *in, *tmpBufPtr;

    Ipp8u  *mdct_buffer = state->mdct_buffer;
    IppsMDCTInvSpec_32f *pMDCTSpecShort = state->pMDCTSpecShort;
    IppsMDCTInvSpec_32f *pMDCTSpecLong = state->pMDCTSpecLong;
    Ipp32f (*prevblk)[LEN_MDCT_LINE] = state->prevblk;
    Ipp32u (*si_blockType)[2] = state->com.si_blockType;
    Ipp32u (*si_mixedBlock)[2] = state->com.si_mixedBlock;
    Ipp32u (*si_winSwitch)[2] = state->com.si_winSwitch;
    samplefloat *smpl_re = state->smpl_re;       /* out of antialiasing */
    samplefloatrw *smpl_rw = state->smpl_rw;     /* out of imdct */
    Ipp32s nz = state->com.non_zero[ch];
    Ipp32s nimdct = (nz + 17 + 9) * 3641 >> 16;
    Ipp32s maximdct;

    if (nimdct > 32)
        nimdct = 32;
    maximdct = nimdct;

    if (state->dctnum_prev[ch] > maximdct)
        maximdct = state->dctnum_prev[ch];
    state->dctnum_prev[ch] = nimdct;

    prev_ptr = prevblk[ch];
    in = (*smpl_re)[ch];
    tmpBufPtr = tmpBuf;

    for (sb = 0; sb < maximdct; sb++) {
        bt = si_blockType[gr][ch];

        if (si_winSwitch[gr][ch] && si_mixedBlock[gr][ch] && (sb < 2)) {
            bt = 0;
        }

        if (bt != 2) {
            if (sb < nimdct) {
                ippsMDCTInv_32f(in, rawout, pMDCTSpecLong, mdct_buffer);
                ippsMul_32f_I(mp3dec_imdct_table[bt], rawout, 18);
                ippsAdd_32f(prev_ptr, rawout, tmpBufPtr, 18);
                ippsMul_32f(mp3dec_imdct_table[bt] + 18, rawout + 18, prev_ptr, 18);
            } else {
                ippsCopy_32f(prev_ptr, tmpBufPtr, 18);
                ippsZero_32f(prev_ptr, 18);
            }
            in += 18;
        } else {
            if (sb < nimdct) {
                for (wnd = 0; wnd < 3; wnd++) {
                    Ipp32f ttt[6];
                    Ipp32s iii;

                    for (iii = 0; iii < 6; iii++) {
                        ttt[iii] = in[iii*3+wnd];
                    }

                    ippsMDCTInv_32f(ttt, tmp + wnd * 12,
                        pMDCTSpecShort, mdct_buffer);
                }
                ippsMul_32f_I(mp3dec_imdct_table[2], tmp, 36);

                ippsCopy_32f(prev_ptr, tmpBufPtr, 6);
                ippsAdd_32f(prev_ptr + 6, tmp, tmpBufPtr + 6, 6);
                ippsAdd_32f(tmp + 6, tmp + 12, rawout + 12, 6);
                ippsAdd_32f(prev_ptr + 12, rawout + 12, tmpBufPtr + 12, 6);

                ippsAdd_32f(tmp + 18, tmp + 24, prev_ptr, 6);
                ippsCopy_32f(tmp + 30, prev_ptr + 6, 6);
                ippsZero_32f(prev_ptr + 12, 6);
            } else {
                ippsCopy_32f(prev_ptr, tmpBufPtr, 18);
                ippsZero_32f(prev_ptr, 18);
            }
            in += 18;
        }

        if (sb & 1) {
            for (j = 1; j < 18; j += 2) {
                tmpBufPtr[j] = -tmpBufPtr[j];
            }
        }

        prev_ptr += 18;
        tmpBufPtr += 18;
    }

    if (sb < 32)
        ippsZero_32f(tmpBufPtr, 18 * (32 - sb));

    for (j = 0; j < 18; j++) {
        for (sb = 0; sb < 32; sb++) {
            (*smpl_rw)[ch][j][sb] = tmpBuf[sb * 18 + j];
        }
    }

    return 1;
}

/******************************************************************************
//  Name:
//    decode_data_LayerIII
//
//  Description:
//    decode one frame of mpeg 1 layer 3 bitstream and save decoded value
//    to output buffer
//
//  Input Arguments:
//    DC - point to sDecoderContext structure
//
//  Output Arguments:
//    outsamples - decoded values of one frame
//
//  Returns:
//
******************************************************************************/

MP3Status mp3dec_decode_data_LayerIII(MP3Dec *state)
{
    Ipp32s main_data_bits;
    Ipp32s i, gr, ch;
    Ipp32f *pSampl[2];

    IppMP3FrameHeader *header = &(state->com.header);
    sBitsreamBuffer *m_MainData = &(state->com.m_MainData);
    Ipp32f (*GlobalScaleFactor)[2] = state->GlobalScaleFactor;
    Ipp16s *m_pOutSamples = state->com.m_pOutSamples;
    Ipp32u (*si_part23Len)[2] = state->com.si_part23Len;
    Ipp16s (*si_globGain)[2] = state->com.si_globGain;
    samplefloat *smpl_sb = state->smpl_sb;       // out of subband synth
    Ipp32s stereo = state->com.stereo;

    Ipp32s part2_start;

    pSampl[0] = (*smpl_sb)[0];
    pSampl[1] = (*smpl_sb)[1];

    // restores actual scalefactors to the values extracted from the bitstream.
    // Four scalefactors in si_globGain (beginning at an offset of 210)
    // are restored to the GlobalScaleFactor vector.

    ippsCalcSF_16s32f((Ipp16s *)(si_globGain), 210, (Ipp32f *)GlobalScaleFactor, 4);

    if (mp3dec_ReadMainData(&state->com))
        return MP3_NOT_FIND_SYNCWORD;

    main_data_bits =
        (Ipp32s)((m_MainData->pCurrent_dword - m_MainData->pBuffer) * 32 + (32 -
        m_MainData->nBit_offset));

    if (main_data_bits < 0)
        return MP3_NOT_ENOUGH_DATA;

    for (gr = 0; gr < header->id + 1; gr++) {
        for (ch = 0; ch < stereo; ch++) {
            Ipp32s rested_bits;

            // detect start point
            m_MainData->pCurrent_dword = m_MainData->pBuffer + (main_data_bits >> 5);
            m_MainData->dword = BSWAP(m_MainData->pCurrent_dword[0]);
            m_MainData->nBit_offset = 32 - (main_data_bits % 32);
            main_data_bits += si_part23Len[gr][ch];

            part2_start =
                (Ipp32s)((m_MainData->pCurrent_dword - m_MainData->pBuffer) * 32 + 32 -
                m_MainData->nBit_offset);
            state->com.part2_start = part2_start;

            rested_bits =
                (m_MainData->nDataLen -
                (Ipp32s)(m_MainData->pCurrent_dword - m_MainData->pBuffer) * 4) * 32 +
                32 - m_MainData->nBit_offset;
            if (rested_bits < (Ipp32s)si_part23Len[gr][ch])
                return MP3_BAD_STREAM;

            if (header->id)
                mp3dec_GetScaleFactorsL3(&state->com, gr, ch);
            else
                mp3dec_GetScaleFactorsL3_LSF(&state->com, ch);

            if (mp3dec_Huffmancodebits(&state->com, gr, ch))
              return MP3_BAD_STREAM;
            mp3dec_Dequant(state, gr, ch);
        }

        if ((header->mode == 0x01) && (header->modeExt != 0)) {
            if (state->com.non_zero[0] < state->com.non_zero[1])
                state->com.non_zero[0] = state->com.non_zero[1];
            else
                state->com.non_zero[1] = state->com.non_zero[0];
            mp3dec_JointStereo(state, gr);
        }
        for (ch = 0; ch < stereo; ch++) {
            mp3dec_Reordering(state, gr, ch);
            mp3dec_Antialiasing(state, gr, ch);
            mp3dec_IMDCT(state, gr, ch);
            for (i = 0; i < 18; i++) {
              ippsSynthesisFilter_PQMF_MP3_32f((*(state->smpl_rw))[ch][i],
                pSampl[ch] + i * 32,
                state->pPQMFSpec[ch], 1);
            }
        }

        // combines the data from all channels in the working array after the
        // SubBandSynth phase
        // into one joined vector.
        // arguments:
        // pSampl -- points to smpl_sb in global (the working array for the
        // SubBandSynth phase)
        // stereo -- number of channels
        // LEN_MDCT_LINE -- number of data elements in each channel
        // outsamples + ... -- points to the joined (output) vector
        ippsJoin_32f16s_D2L((const Ipp32f **)pSampl, stereo, LEN_MDCT_LINE,
            m_pOutSamples + gr * LEN_MDCT_LINE * stereo);
    }
    return MP3_OK;
}

#endif //UMC_ENABLE_MP3_AUDIO_DECODER
