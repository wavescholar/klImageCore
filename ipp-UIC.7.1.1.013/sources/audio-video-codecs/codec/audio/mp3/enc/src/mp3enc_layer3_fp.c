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

#include "mp3enc_own_fp.h"

#include "ippcore.h"

/******************************************************************************
//  Name:
//    mdct_init
//
//  Description:
//    Initialize coefficients of mdct windows
//
//  Input Arguments:
//     - pointer to encoder context
//
//  Output Arguments:
//
//  Returns:
//    -
//
******************************************************************************/

MP3Status mp3enc_mdctInit(MP3Enc *state, Ipp8u *mem, Ipp32s *size_all)
{
    Ipp32s i;
    Ipp32f c[8] =
      { -0.6f, -0.535f, -0.33f, -0.185f, -0.095f, -0.041f, -0.0142f, -0.0037f };
    Ipp32f tmp;
    Ipp32f *cs, *ca;
    Ipp32f (*mdct_win)[36];
    Ipp32s size, ts, ts1, size_buf, size_init, size_buf1, size_init1;
    Ipp8u *mem_init;

    size = 0;

    ippsMDCTFwdGetSize_32f(36, &ts, &size_init, &size_buf);
    ippsMDCTFwdGetSize_32f(12, &ts1, &size_init1, &size_buf1);
    if (size_buf < size_buf1) size_buf = size_buf1;
    if (size_init < size_init1) size_init = size_init1;

    size += ts + ts1 + size_buf;

    if (mem) {
      mem_init = 0;
      if (size_init) {
        mem_init = (Ipp8u*)ippMalloc(size_init);
        if (!mem_init)
          return MP3_ALLOC;
      }

      state->pMDCTSpec36 = (IppsMDCTFwdSpec_32f*)mem;
      ippsMDCTFwdInit_32f(&(state->pMDCTSpec36), 36, mem, mem_init);
      mem += ts;
      state->pMDCTSpec12 = (IppsMDCTFwdSpec_32f*)mem;
      ippsMDCTFwdInit_32f(&(state->pMDCTSpec12), 12, mem, mem_init);
      mem += ts1;
      state->pMDCTbuf = mem;
      mem += size_buf;
      if(mem_init)
        ippFree(mem_init);
    }

    if(state) {
      cs = state->cs;
      ca = state->ca;
      mdct_win = state->mdct_win;

      for (i = 0; i < 8; i++) {
        tmp = (Ipp32f)sqrt(1.0 + c[i] * c[i]);
        ca[i] = c[i] / tmp;
        cs[i] = 1.0f / tmp;
      }

  /*
  * type 0
  */
      for (i = 0; i < 36; i++)
        state->mdct_win[0][i] = (Ipp32f)sin(PI / 36 * (i + 0.5));
  /*
  * type 1
  */
      for (i = 0; i < 18; i++)
        mdct_win[1][i] = (Ipp32f)sin(PI / 36 * (i + 0.5));
      for (i = 18; i < 24; i++)
        mdct_win[1][i] = 1.0;
      for (i = 24; i < 30; i++)
        mdct_win[1][i] = (Ipp32f)sin(PI / 12 * (i + 0.5 - 18));
      for (i = 30; i < 36; i++)
        mdct_win[1][i] = 0.0;
  /*
  * type 3
  */
      for (i = 0; i < 6; i++)
        mdct_win[3][i] = 0.0;
      for (i = 6; i < 12; i++)
        mdct_win[3][i] = (Ipp32f)sin(PI / 12 * (i + 0.5 - 6));
      for (i = 12; i < 18; i++)
        mdct_win[3][i] = 1.0;
      for (i = 18; i < 36; i++)
        mdct_win[3][i] = (Ipp32f)sin(PI / 36 * (i + 0.5));
  /*
  * type 2
  */
      for (i = 0; i < 12; i++)
        mdct_win[2][i] = (Ipp32f)sin(PI / 12 * (i + 0.5));
      for (i = 12; i < 36; i++)
        mdct_win[2][i] = 0.0;

      ippsZero_32f(&state->fbout_data[0][0][0][0],2*4*18*32);
    }

    if(size_all)
      *size_all = size;

    return MP3_OK;
}

/******************************************************************************
//  Name:
//    mdct
//
//  Description:
//    Perform windowing and appling of mdct.
//
//  Input Arguments:
//             - pointer to encoder context
//    in         - input samples
//   block_type  - type of block
//
//  Output Arguments:
//    out        - samples in frequency domain
//
//  Returns:
//    1 - all ok
******************************************************************************/

Ipp32s mp3enc_mdctInSubband(MP3Enc *state,
                            Ipp32f *in,
                            Ipp32f *out,
                            Ipp32s block_type)
{
    Ipp32s i, j;
    Ipp32f *tout = out;

    if (block_type == 2) {
      IPP_ALIGNED_ARRAY(32, Ipp32f, tmp_in, 36);
      IPP_ALIGNED_ARRAY(32, Ipp32f, tmp_out, 36);
      Ipp32f *tmp_out2[3];
      Ipp32f **tmp_out3;

      tmp_out2[0] = tmp_out;
      tmp_out2[1] = tmp_out + 6;
      tmp_out2[2] = tmp_out + 12;

      tmp_out3 = &(tmp_out2[0]);

      for (i = 0; i < state->com.lowpass_maxline; i++) {
        ippsMul_32f(in + 6, state->mdct_win[block_type], tmp_in, 12);
        ippsMul_32f(in + 12, state->mdct_win[block_type], tmp_in + 12, 12);
        ippsMul_32f(in + 18, state->mdct_win[block_type], tmp_in + 24, 12);

        ippsMDCTFwd_32f(tmp_in, tmp_out, state->pMDCTSpec12, state->pMDCTbuf);
        ippsMDCTFwd_32f(tmp_in + 12, tmp_out + 6, state->pMDCTSpec12, state->pMDCTbuf);
        ippsMDCTFwd_32f(tmp_in + 24, tmp_out + 12, state->pMDCTSpec12, state->pMDCTbuf);

        ippsMulC_32f_I(2.f / 12, tmp_out, 18);
        ippsInterleave_32f((const Ipp32f **)tmp_out3, 3, 6, out);
        in += 36;
        out += 18;
      }
    } else {
      Ipp32f  *ptr_out = out;

      for (i = 0; i < state->com.lowpass_maxline; i++) {

        for (j = 0; j < 36; j++)
          in[j] *= state->mdct_win[block_type][j];

        ippsMDCTFwd_32f(in, out, state->pMDCTSpec36, state->pMDCTbuf);

        in += 36;
        out += 18;
      }
      ippsMulC_32f_I(2.f / 36, ptr_out, 18 * state->com.lowpass_maxline);
    }

    if (state->com.lowpass_maxline < 32) {
        ippsZero_32f(tout + state->com.lowpass_maxline * 18, 18 * (32 - state->com.lowpass_maxline));
    }

    return 1;
}

/******************************************************************************
//  Name:
//    mdctBlock
//
//  Description:
//    The output of the filterbank is the input to the subdivision using the MDCT.
//    18 consecutive output values of one granule and 18 output values of the granule before
//    are assembled to one block of 36 samples for each subbands.
//
//  Input Arguments:
//             - pointer to encoder context
//
//  Output Arguments:
//    -
//
//  Returns:
//    1 - all ok.
******************************************************************************/

Ipp32s mp3enc_mdctBlock(MP3Enc *state, int gr_cnt)
{
    IPP_ALIGNED_ARRAY(32, Ipp32f, in, 1152);
    Ipp32s ch, gr, bnd, k, j;
    Ipp32f bu, bd;

    Ipp32f *ptr_fbout;
    Ipp32f *cs, *ca;
    Ipp32s stereo;
    Ipp32s grnum;

    stereo = state->com.stereo;
    grnum = state->com.grnum;
    cs = state->cs;
    ca = state->ca;

//    for (gr = 1; gr <= grnum; gr++)
    gr = gr_cnt + 1;
    {
      for (ch = 0; ch < stereo; ch++) {
        ptr_fbout = &((*(state->fbout[gr]))[ch][0][0]);
        for (k = 33; k < LEN_MDCT_LINE; k += 32)
            for (j = 0; j < 32; j += 2, k += 2)
                ptr_fbout[k] = -ptr_fbout[k];
        }
    }

//    for (gr = 0; gr < grnum; gr++)
    gr = gr_cnt;
    {
      for (ch = 0; ch < stereo; ch++) {
        for (bnd = 0, j = 0; bnd < 32; bnd++) {
          for (k = 0; k < 18; k++) {
            in[j + k] = (*(state->fbout[gr]))[ch][k][bnd];
            in[j + k + 18] = (*(state->fbout[gr + 1]))[ch][k][bnd];
          }
          j += 36;
        }

        mp3enc_mdctInSubband(state, in, state->mdct_out[gr][ch], state->com.si_blockType[gr][ch]);

      }
      for (ch = 0; ch < stereo; ch++) {
        if (state->com.si_blockType[gr][ch] != 2) {
          Ipp32s  idx1, idx2;

          j = 0;
          for (bnd = 0; bnd < 31; bnd++) {
            for (k = 0; k < 8; k++) {
              idx1 = j + 17 - k;
              idx2 = j + 18 + k;
              bu =
                state->mdct_out[gr][ch][idx1] * cs[k] + state->mdct_out[gr][ch][idx2] * ca[k];
              bd =
                state->mdct_out[gr][ch][idx2] * cs[k] - state->mdct_out[gr][ch][idx1] * ca[k];
              state->mdct_out[gr][ch][idx1] = bu;
              state->mdct_out[gr][ch][idx2] = bd;
            }
            j += 18;
          }

          ippsZero_32f(state->mdct_out[gr][ch] + state->non_zero_line[0],
                       LEN_MDCT_LINE - state->non_zero_line[0]);
        } else {
          const Ipp32u *sfb_short = mp3enc_sfBandIndex[state->com.header.id][state->com.header.samplingFreq].s;
          Ipp32f *mdct_out = state->mdct_out[gr][ch];
          Ipp32s i, sfb, start, end, line, window;

          ippsZero_32f(state->mdct_out[gr][ch] + 3 * state->non_zero_line[1],
                       LEN_MDCT_LINE - 3 * state->non_zero_line[1]);

          i = 0;
          for (sfb = 0; sfb < 13; sfb++) {
            start = sfb_short[sfb];
            end = sfb_short[sfb + 1];

            for (window = 0; window < 3; window++) {
              for (line = start; line < end; line += 2) {
                in[i++] = mdct_out[line*3  +window];
                in[i++] = mdct_out[line*3+3+window];
              }
            }
          }
          ippsCopy_32f(in, mdct_out, LEN_MDCT_LINE);
        }
      }
      if (state->com.stereo_mode == MPA_MS_STEREO) {
        const Ipp32f mult = 0.7071067811865475244f;
        Ipp32f *ptrL = &(state->mdct_out[gr][0][0]);
        Ipp32f *ptrR = &(state->mdct_out[gr][1][0]);
        Ipp32f li, ri;
        for (k = 0; k < LEN_MDCT_LINE; k++) {
          li = mult * (ptrL[0] + ptrR[0]);
          ri = mult * (ptrL[0] - ptrR[0]);
          ptrL[0] = li;
          ptrR[0] = ri;
          ptrL++; ptrR++;
        }
      }
    }

    if (gr_cnt == grnum - 1) {
      state->fbout_prev += grnum;
      if (state->fbout_prev > 3)
          state->fbout_prev -= 4;

      for (j = 0; j < 4; j++) {
          Ipp32s ind;
          ind = state->fbout_prev + j;
          if (ind > 3) ind -= 4;
          state->fbout[j] = &state->fbout_data[ind];
      }
    }

    return 1;
}

#endif //UMC_ENABLE_MP3_AUDIO_ENCODER
