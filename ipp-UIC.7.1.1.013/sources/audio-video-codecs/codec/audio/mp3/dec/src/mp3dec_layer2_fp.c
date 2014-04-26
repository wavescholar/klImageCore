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

#include "mp3dec_own_fp.h"
#include "ipps.h"

static Ipp32f mp3dec_C_quant[] = {
    1.33333333333f / 65536.0f,
    1.60000000000f / 65536.0f,
    1.14285714286f / 65536.0f,
    1.77777777777f / 65536.0f,
    1.06666666666f / 65536.0f,
    1.03225806452f / 65536.0f,
    1.01587301587f / 65536.0f,
    1.00787401575f / 65536.0f,
    1.00392156863f / 65536.0f,
    1.00195694716f / 65536.0f,
    1.00097751711f / 65536.0f,
    1.00048851979f / 65536.0f,
    1.00024420024f / 65536.0f,
    1.00012208522f / 65536.0f,
    1.00006103888f / 65536.0f,
    1.00003051851f / 65536.0f,
    1.00001525902f / 65536.0f
};

static Ipp32f mp3dec_D_quant[] = {
  16384 * 65536.0f, 16384 * 65536.0f, 8192 * 65536.0f, 16384 * 65536.0f,
   4096 * 65536.0f,  2048 * 65536.0f, 1024 * 65536.0f,   512 * 65536.0f,
    256 * 65536.0f,   128 * 65536.0f,   64 * 65536.0f,    32 * 65536.0f,
     16 * 65536.0f,     8 * 65536.0f,    4 * 65536.0f,     2 * 65536.0f,
      1 * 65536.0f,
};

Ipp32f mp3dec_scale_values[64] = {
  2.00000000000000f, 1.58740105196820f, 1.25992104989487f,
  1.00000000000000f, 0.79370052598410f, 0.62996052494744f, 0.50000000000000f,
  0.39685026299205f, 0.31498026247372f, 0.25000000000000f, 0.19842513149602f,
  0.15749013123686f, 0.12500000000000f, 0.09921256574801f, 0.07874506561843f,
  0.06250000000000f, 0.04960628287401f, 0.03937253280921f, 0.03125000000000f,
  0.02480314143700f, 0.01968626640461f, 0.01562500000000f, 0.01240157071850f,
  0.00984313320230f, 0.00781250000000f, 0.00620078535925f, 0.00492156660115f,
  0.00390625000000f, 0.00310039267963f, 0.00246078330058f, 0.00195312500000f,
  0.00155019633981f, 0.00123039165029f, 0.00097656250000f, 0.00077509816991f,
  0.00061519582514f, 0.00048828125000f, 0.00038754908495f, 0.00030759791257f,
  0.00024414062500f, 0.00019377454248f, 0.00015379895629f, 0.00012207031250f,
  0.00009688727124f, 0.00007689947814f, 0.00006103515625f, 0.00004844363562f,
  0.00003844973907f, 0.00003051757813f, 0.00002422181781f, 0.00001922486954f,
  0.00001525878906f, 0.00001211090890f, 0.00000961243477f, 0.00000762939453f,
  0.00000605545445f, 0.00000480621738f, 0.00000381469727f, 0.00000302772723f,
  0.00000240310869f, 0.00000190734863f, 0.00000151386361f, 0.00000120155435f,
  1E-20f
};

static Ipp32f mp3dec_rscale_values[64] = {
  5.00000000e-01f, 6.29960533e-01f, 7.93700511e-01f, 1.00000000e+00f,
  1.25992107e+00f, 1.58740102e+00f, 2.00000000e+00f, 2.51984213e+00f,
  3.17480204e+00f, 4.00000000e+00f, 5.03968426e+00f, 6.34960409e+00f,
  8.00000000e+00f, 1.00793685e+01f, 1.26992082e+01f, 1.60000000e+01f,
  2.01587370e+01f, 2.53984163e+01f, 3.20000000e+01f, 4.03174741e+01f,
  5.07968327e+01f, 6.40000000e+01f, 8.06349482e+01f, 1.01593665e+02f,
  1.28000000e+02f, 1.61269896e+02f, 2.03187331e+02f, 2.56000000e+02f,
  3.22539793e+02f, 4.06374662e+02f, 5.12000000e+02f, 6.45079585e+02f,
  8.12749323e+02f, 1.02400000e+03f, 1.29015917e+03f, 1.62549865e+03f,
  2.04800000e+03f, 2.58031834e+03f, 3.25099729e+03f, 4.09600000e+03f,
  5.16063668e+03f, 6.50199458e+03f, 8.19200000e+03f, 1.03212734e+04f,
  1.30039892e+04f, 1.63840000e+04f, 2.06425467e+04f, 2.60079783e+04f,
  3.27680000e+04f, 4.12850935e+04f, 5.20159567e+04f, 6.55360000e+04f,
  8.25701869e+04f, 1.04031913e+05f, 1.31072000e+05f, 1.65140374e+05f,
  2.08063827e+05f, 2.62144000e+05f, 3.30280748e+05f, 4.16127653e+05f,
  5.24288000e+05f, 6.60561496e+05f, 8.32255307e+05f, 1.00000000e+20f,
};

Ipp32s mp3dec_decode_data_LayerII(MP3Dec *state)
{
  Ipp32s (*sample)[32][36] = state->com.sample;
  Ipp16s (*scalefactor)[3][32] = state->com.scalefactor;
  Ipp16s (*allocation)[32] = state->com.allocation;
  const Ipp8u* alloc_table = state->com.alloc_table;
  samplefloatrw *smpl_rw = state->smpl_rw;     // out of imdct
  Ipp32s i, ii, ch, sb;
  Ipp32f scale;
  Ipp32s sblimit = state->com.sblimit;
  Ipp32s stereo = state->com.stereo;

  for (ch = 0; ch < stereo; ch++) {
    for (sb = 0; sb < sblimit; sb++) {
      if (allocation[ch][sb] != 0) {
      for (ii = 0; ii < 3; ii++) {
        Ipp32s idx = alloc_table[sb * 16 + allocation[ch][sb]];
        Ipp32s x = mp3_numbits[idx];
        Ipp32s xor_coef;
        Ipp32s *sample_ptr = &sample[ch][sb][0];

        xor_coef = (1 << (x - 1));

        scale = mp3dec_C_quant[idx] * mp3dec_scale_values[scalefactor[ch][ii][sb]];
        for (i = 0; i < 12; i++) {
          (*smpl_rw)[ch][ii*12+i][sb] =
            ((Ipp32f)((sample_ptr[ii*12+i] ^ xor_coef) << (32 - x)));

          // Dequantize the sample
          (*smpl_rw)[ch][ii*12+i][sb] += mp3dec_D_quant[idx];
          (*smpl_rw)[ch][ii*12+i][sb] *= scale;
        }
      }
      } else {
        for (i = 0; i < 36; i++) {
          (*smpl_rw)[ch][i][sb] = 0;
        }
      }
    }  // for sb

    for (sb = sblimit; sb < 32; sb++) {
      for (i = 0; i < 36; i++) {
        (*smpl_rw)[ch][i][sb] = 0;
      }
    }
  }    // for ch

  return 1;
}

void mp3dec_dequantize_sample_mc_l2(MP3Dec *state)
{
  Ipp16s (*allocation)[32] = state->com.allocation;
  MP3Dec_com *state_com = &(state->com);
  Ipp16s (*bit_alloc)[32] = state_com->allocation;
  Ipp32s (*sample)[32][36] = state_com->sample;
  samplefloatrw *smpl_rw = state->smpl_rw;
  const Ipp8u* alloc_table = state->com.alloc_table;
  Ipp32s stereo = state_com->stereo;
  Ipp32s ch_start = stereo;
  Ipp32s channels = stereo + state_com->mc_channel;

  Ipp32s i, j, k, sbgr;
  Ipp32s sblimit = state_com->sblimit;

  for (i = 0; i < sblimit; i++) {
    sbgr = mp3_mc_sb_group[i];

    for (k = ch_start; k < channels; k++)
      if (bit_alloc[k][i]) {
        Ipp32s dequant_needed = 0;
        if (state_com->mc_dyn_cross_on == 0) {
          dequant_needed = 1;
        }
        else {
          if (state_com->mc_dyn_cross_bits == 4  &&
            ((state_com->mc_dyn_cross_mode[sbgr]==0) ||
            (state_com->mc_dyn_cross_mode[sbgr]==1  && k!=4) ||
            (state_com->mc_dyn_cross_mode[sbgr]==2  && k!=3) ||
            (state_com->mc_dyn_cross_mode[sbgr]==3  && k!=2) ||
            (state_com->mc_dyn_cross_mode[sbgr]==8  && k!=4) ||
            (state_com->mc_dyn_cross_mode[sbgr]==9  && k!=3) ||
            (state_com->mc_dyn_cross_mode[sbgr]==10 && k!=4)))
          {
            dequant_needed = 1;
          } else if (state_com->mc_dyn_cross_bits != 4 ||
            ((state_com->mc_dyn_cross_mode[sbgr]==4  && k==2) ||
            (state_com->mc_dyn_cross_mode[sbgr]==5  && k==3) ||
            (state_com->mc_dyn_cross_mode[sbgr]==6  && k==4) ||
            (state_com->mc_dyn_cross_mode[sbgr]==11 && k==2) ||
            (state_com->mc_dyn_cross_mode[sbgr]==12 && k==2) ||
            (state_com->mc_dyn_cross_mode[sbgr]==13 && k==3) ||
            (state_com->mc_dyn_cross_mode[sbgr]==14 && k==2)))
          {
            if (!((state_com->mc_dyn_cross_bits == 3 &&
              ((state_com->mc_dyn_cross_mode[sbgr] == 1 && k==3) ||
              (state_com->mc_dyn_cross_mode[sbgr] == 2 && k==2) ||
              state_com->mc_dyn_cross_mode[sbgr] == 3 ||
              (state_com->mc_dyn_cross_mode[sbgr] == 4 && k==3)
              )) ||
              (state_com->mc_dyn_cross_bits == 1 && state_com->mc_dyn_cross_mode[sbgr] == 1 && k == 2) ||
              (state_com->mc_header.surround == 3 && state_com->mc_dyn_second_stereo[sbgr] == 1 &&
              ((state_com->mc_header.center != 0 && k == 4) || (state_com->mc_header.center == 0 && k == 3))))) {
              dequant_needed = 1;
            }
          }
        }
        if (dequant_needed) {
          Ipp32s idx = alloc_table[i * 16 + allocation[k][i]];
          Ipp32s x = mp3_numbits[idx];
          Ipp32s xor_coef;
          Ipp32s *sample_ptr = &sample[k][i][0];
          Ipp32f scale;
          Ipp32f tmp[36];

          xor_coef = (1 << (x - 1));

          scale = mp3dec_C_quant[idx];

          ippsXorC_32u_I(xor_coef, (Ipp32u *)sample_ptr, 36);
          ippsLShiftC_32s_I(32 - x, sample_ptr, 36);
          ippsConvert_32s32f_Sfs(sample_ptr, tmp, 36, 0);
          ippsAddC_32f_I(mp3dec_D_quant[idx], tmp, 36);
          for (j = 0; j < 36; j++)
            (*smpl_rw)[k][j][i] = tmp[j] * scale;
        }
      }
      else {
        for (j = 0; j < 36; j++)
          (*smpl_rw)[k][j][i] = 0.0f;
      }
  }
  for (i = sblimit; i < 32; i++)
    for (k = ch_start; k < channels; k++) {
      for (j = 0; j < 36; j++)
        (*smpl_rw)[k][j][i] = 0.0f;
    }
}

#if !defined (__INTEL_COMPILER) && defined( _MSC_VER)
#pragma optimize( "", off )
#endif

void mp3dec_mc_denormalize_sample_l2(MP3Dec *state)
{
  MP3Dec_com *state_com = &(state->com);
  Ipp32s stereo = state_com->stereo;
  Ipp32s ch_start = stereo;
  Ipp32s channels = stereo + state_com->mc_channel;
  Ipp32s i, j, sbgr, bl;
  Ipp32s gr;

  Ipp32s sblimit = state_com->sblimit;
  samplefloatrw *smpl_rw = state->smpl_rw;
  Ipp16s (*scalefactor)[3][32] = state->com.scalefactor;

  for (gr = 0; gr < 3; gr++) {
    for (i = 0; i < sblimit; i++) {
      sbgr = mp3_mc_sb_group[i];
      if (state_com->mc_dyn_cross_on == 0)
        for (j = ch_start; j < channels; j++)
          for (bl = 0; bl < 12; bl++)
            (*smpl_rw)[j][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[j][gr][i]];
      else if (state_com->mc_dyn_cross_bits == 0) {
        if (state_com->mc_header.surround == 3) {
          for (bl = 0; bl < 12; bl++)
            (*smpl_rw)[ch_start][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[ch_start][gr][i]];
          if (state_com->mc_dyn_second_stereo[sbgr] == 0)
            for (bl = 0; bl < 12; bl++)
              (*smpl_rw)[ch_start+1][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[ch_start+1][gr][i]];
          else
            for (bl = 0; bl < 12; bl++)
              (*smpl_rw)[ch_start+1][gr*12+bl][i] = ((*smpl_rw)[ch_start][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[ch_start][gr][i]])
              * mp3dec_scale_values[scalefactor[ch_start+1][gr][i]];
        }
      }
      else if (state_com->mc_dyn_cross_bits == 1) {
        switch (state_com->mc_dyn_cross_mode[sbgr]) {
            case 0 :
              for (bl = 0; bl < 12; bl++)
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
              if (state_com->mc_header.surround == 3) {
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                if (state_com->mc_dyn_second_stereo[sbgr] == 0)
                  for (bl = 0; bl < 12; bl++)
                    (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
                else
                  for (bl = 0; bl < 12; bl++)
                    (*smpl_rw)[4][gr*12+bl][i] = ((*smpl_rw)[3][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[3][gr][i]])
                    * mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 1 :
              if (state_com->mc_tc_alloc[sbgr] == 0)
              {
                if (state_com->mc_dyn_cross_LR)
                  for (bl = 0; bl < 12; bl++)
                    (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                    * mp3dec_scale_values[scalefactor[2][gr][i]];
                else          /* C,S from L0 */
                  for (bl = 0; bl < 12; bl++)
                    (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                    * mp3dec_scale_values[scalefactor[2][gr][i]];
              }
              if (state_com->mc_tc_alloc[sbgr] == 1)
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              if (state_com->mc_tc_alloc[sbgr] == 2)
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              if (state_com->mc_header.surround == 3) {
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                if (state_com->mc_dyn_second_stereo[sbgr] == 0)
                  for (bl = 0; bl < 12; bl++)
                    (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
                else
                  for (bl = 0; bl < 12; bl++)
                    (*smpl_rw)[4][gr*12+bl][i] = ((*smpl_rw)[3][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[3][gr][i]])
                    * mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
        }
      }
      else if (state_com->mc_dyn_cross_bits == 3) {
        switch (state_com->mc_dyn_cross_mode[sbgr]) {
            case 0 :
              for (j = ch_start; j < channels; j++)
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[j][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[j][gr][i]];
              break;
            case 1 :
              for (bl = 0; bl < 12; bl++)
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
              if (state_com->mc_header.surround == 2 ||
                state_com->mc_tc_alloc[sbgr] == 4 ||
                state_com->mc_tc_alloc[sbgr] == 5 ||
                (state_com->mc_tc_alloc[sbgr] != 3 && state_com->mc_dyn_cross_LR))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[3][gr*12+bl][i]  = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[3][gr][i]];
              else
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[3][gr*12+bl][i]  = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[3][gr][i]];
              break;
            case 2 :
              for (bl = 0; bl < 12; bl++)
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
              if (state_com->mc_header.surround == 2 ||
                state_com->mc_tc_alloc[sbgr] == 1 ||
                state_com->mc_tc_alloc[sbgr] == 5 ||
                (state_com->mc_tc_alloc[sbgr] != 2 && !state_com->mc_dyn_cross_LR))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              break;
            case 3 :
              if (state_com->mc_header.surround == 2 ||
                state_com->mc_tc_alloc[sbgr] == 1 ||
                state_com->mc_tc_alloc[sbgr] == 5 ||
                (state_com->mc_tc_alloc[sbgr] != 2 && !state_com->mc_dyn_cross_LR))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              if (state_com->mc_header.surround == 2 ||
                state_com->mc_tc_alloc[sbgr] == 4 ||
                state_com->mc_tc_alloc[sbgr] == 5 ||
                (state_com->mc_tc_alloc[sbgr] != 3 && state_com->mc_dyn_cross_LR))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[3][gr*12+bl][i]  = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[3][gr][i]];
              else
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[3][gr*12+bl][i]  = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[3][gr][i]];
              break;
            case 4 :
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[3][gr*12+bl][i] = (*smpl_rw)[2][gr*12+bl][i];
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
              }
              break;
        }
      }
      else if (state_com->mc_dyn_cross_bits == 4)  {
        switch (state_com->mc_dyn_cross_mode[sbgr]) {
            case 0 :
              for (j = ch_start; j < channels; j++)
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[j][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[j][gr][i]];
              break;
            case 1 :
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i]  = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                * mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 2 :
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[2][gr*12+bl][i] = (*smpl_rw)[2][gr*12+bl][i] * mp3dec_scale_values[scalefactor[2][gr][i]];
                (*smpl_rw)[3][gr*12+bl][i]  = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                * mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 3 :
              if ((state_com->mc_tc_alloc[sbgr] == 1) || (state_com->mc_tc_alloc[sbgr] == 7))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else if ((state_com->mc_tc_alloc[sbgr] == 2) || (state_com->mc_tc_alloc[sbgr] == 6))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else if (state_com->mc_dyn_cross_LR)
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];

              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 4 :
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
                (*smpl_rw)[3][gr*12+bl][i]  = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                * mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i]  = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                * mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 5 :
              if ((state_com->mc_tc_alloc[sbgr] == 1) || (state_com->mc_tc_alloc[sbgr] == 7))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else if ((state_com->mc_tc_alloc[sbgr] == 2) || (state_com->mc_tc_alloc[sbgr] == 6))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else if (state_com->mc_dyn_cross_LR)
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];

              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i]  = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                * mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 6 :
              if ((state_com->mc_tc_alloc[sbgr] == 1) || (state_com->mc_tc_alloc[sbgr] == 7))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else if ((state_com->mc_tc_alloc[sbgr] == 2) || (state_com->mc_tc_alloc[sbgr] == 6))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else if (state_com->mc_dyn_cross_LR)
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];

              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[3][gr*12+bl][i]  = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                * mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 7 :
              if ((state_com->mc_tc_alloc[sbgr] == 1) || (state_com->mc_tc_alloc[sbgr] == 7))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else if ((state_com->mc_tc_alloc[sbgr] == 2) || (state_com->mc_tc_alloc[sbgr] == 6))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else if (state_com->mc_dyn_cross_LR)
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[3][gr*12+bl][i]  = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                * mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i]  = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                * mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 8 :
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i]  = (*smpl_rw)[3][gr*12+bl][i];
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 9 :
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[3][gr*12+bl][i] = (*smpl_rw)[2][gr*12+bl][i];
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 10 :
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[4][gr*12+bl][i]  = (*smpl_rw)[2][gr*12+bl][i];
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 11 :
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[3][gr*12+bl][i]  = (*smpl_rw)[2][gr*12+bl][i];
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i]  = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                * mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 12 :
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[4][gr*12+bl][i]  = (*smpl_rw)[2][gr*12+bl][i];
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
                (*smpl_rw)[3][gr*12+bl][i]  = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                * mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 13:
              if ((state_com->mc_tc_alloc[sbgr] == 1) || (state_com->mc_tc_alloc[sbgr] == 7))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else if ((state_com->mc_tc_alloc[sbgr] == 2) || (state_com->mc_tc_alloc[sbgr] == 6))
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else if (state_com->mc_dyn_cross_LR)
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[1][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[1][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              else
                for (bl = 0; bl < 12; bl++)
                  (*smpl_rw)[2][gr*12+bl][i] = ((*smpl_rw)[0][gr*12+bl][i] * mp3dec_rscale_values[scalefactor[0][gr][i]])
                  * mp3dec_scale_values[scalefactor[2][gr][i]];
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[4][gr*12+bl][i]  = (*smpl_rw)[3][gr*12+bl][i];
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
            case 14 :
              for (bl = 0; bl < 12; bl++) {
                (*smpl_rw)[4][gr*12+bl][i]  = (*smpl_rw)[2][gr*12+bl][i];
                (*smpl_rw)[3][gr*12+bl][i]  = (*smpl_rw)[2][gr*12+bl][i];
                (*smpl_rw)[2][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[2][gr][i]];
                (*smpl_rw)[3][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[3][gr][i]];
                (*smpl_rw)[4][gr*12+bl][i] *= mp3dec_scale_values[scalefactor[4][gr][i]];
              }
              break;
        }
      }
    }
  }
}

#define ADD_PREDICT(sb, j, ch, idx, out)                          \
{                                                                 \
  Ipp32s i;                                                       \
  Ipp32f tmp;                                                     \
                                                                  \
  tmp = 0.0f;                                                     \
  for (i = 0; i < state_com->mc_predsi[sb][idx]; i++)             \
  tmp +=                                                          \
    pred_buf[ch][sb][9+j-i-state_com->mc_delay_comp[sb][idx]] *   \
    ((Ipp32s)state_com->mc_pred_coeff[sb][idx][i] - 127) *        \
    0.03125f;                                                     \
  out += tmp;                                                     \
}

void mp3dec_mc_dematricing(MP3Dec *state)
{
  MP3Dec_com *state_com = &(state->com);
  Ipp32s ii, j, k, tc_alloc, sbgr = 0;
  Ipp32f tmp_sample, tmp_sample1, surround_sample;
  samplefloatrw *smpl_rw = state->smpl_rw;     // out of imdct
  Ipp32f (*pred_buf)[8][36+9] = state->mc_pred_buf;

  if (state->com.mc_prediction_on){
    for (k=0; k<2; k++)
      for (ii=0; ii<8; ii++) {
        for (j = 0; j < 9; j++)
          state->mc_pred_buf[k][ii][j] = state->mc_pred_buf[k][ii][j+36];
        for (j=0;j<36;j++)
          state->mc_pred_buf[k][ii][9+j] = (*smpl_rw)[k][j][ii];
      }
  }

  for(k = 0; k < 32; k ++) {
    if (state_com->mc_prediction_on && k < 8 && state_com->mc_prediction[k]) {
      if ((state_com->mc_header.surround == 2) && (state_com->mc_header.center != 0)) {
        switch (state_com->mc_dyn_cross_mode[k])
        {
        case 0:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 0, 2, (*smpl_rw)[3][j][k]);
            ADD_PREDICT(k, j, 1, 3, (*smpl_rw)[3][j][k]);
            ADD_PREDICT(k, j, 0, 4, (*smpl_rw)[4][j][k]);
            ADD_PREDICT(k, j, 1, 5, (*smpl_rw)[4][j][k]);
          }
          break;

        case 1:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 0, 2, (*smpl_rw)[3][j][k]);
            ADD_PREDICT(k, j, 1, 3, (*smpl_rw)[3][j][k]);
          }
          break;

        case 2:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 0, 2, (*smpl_rw)[4][j][k]);
            ADD_PREDICT(k, j, 1, 3, (*smpl_rw)[4][j][k]);
          }
          break;

        case 3:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[3][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[3][j][k]);
            ADD_PREDICT(k, j, 0, 2, (*smpl_rw)[4][j][k]);
            ADD_PREDICT(k, j, 1, 3, (*smpl_rw)[4][j][k]);
          }
          break;

        case 4:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[2][j][k]);
          }
          break;

        case 5:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[3][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[3][j][k]);
          }
          break;

        case 6:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[4][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[4][j][k]);
          }
          break;

        case 8:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[2][j][k]);
          }
          break;

        case 9:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[4][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[4][j][k]);
          }
          break;

        case 10:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[3][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[3][j][k]);
          }
          break;

        }
      }
      else if ((state_com->mc_header.surround == 1 && state_com->mc_header.center != 0) ||
        (state_com->mc_header.surround == 2 && state_com->mc_header.center == 0))
      {
        switch(state_com->mc_dyn_cross_mode[k])
        {
        case 0:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 0, 2, (*smpl_rw)[3][j][k]);
            ADD_PREDICT(k, j, 1, 3, (*smpl_rw)[3][j][k]);
          }
          break;

        case 1:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[2][j][k]);
          }
          break;

        case 2:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[3][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[3][j][k]);
          }
          break;

        }
      }
      else if (((state_com->mc_header.surround == 0 || state_com->mc_header.surround == 3) && state_com->mc_header.center != 0) ||
        (state_com->mc_header.surround == 1 && state_com->mc_header.center == 0))
      {
        switch(state_com->mc_dyn_cross_mode[k])
        {
        case 0:
          for (j=0; j<36; j++) {
            ADD_PREDICT(k, j, 0, 0, (*smpl_rw)[2][j][k]);
            ADD_PREDICT(k, j, 1, 1, (*smpl_rw)[2][j][k]);
          }
          break;

        }
      }
    }
  }

  for(k = 0; k < 32; k ++) {
    if(state_com->mc_tc_sbgr_select == 1)
      tc_alloc = state_com->mc_tc_allocation;
    else {
      sbgr = mp3_mc_sb_group[k];
      tc_alloc = state_com->mc_tc_alloc[sbgr];
    }
    if (state_com->mc_header.dematrix_procedure != 3)
    {
      if ((state_com->mc_header.surround == 2) && (state_com->mc_header.center != 0)) {
        switch(tc_alloc) {
      case 0:
        if (state_com->mc_header.dematrix_procedure == 2) {
          for (j=0; j<36; j++) {
            surround_sample = ((*smpl_rw)[3][j][k] + (*smpl_rw)[4][j][k]) * 0.5f;
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] -
              (*smpl_rw)[2][j][k] + surround_sample;
            (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] -
              (*smpl_rw)[2][j][k] - surround_sample;
          }
        }
        else {
          for (j=0; j<36; j++) {
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] -
              (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
            (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] -
              (*smpl_rw)[2][j][k] - (*smpl_rw)[4][j][k];
          }
        }
        break;

      case 1:
        if (state_com->mc_header.dematrix_procedure == 2) {
          for (j=0; j<36; j++) {
            surround_sample = ((*smpl_rw)[3][j][k] + (*smpl_rw)[4][j][k]) * 0.5f;
            tmp_sample = (*smpl_rw)[2][j][k];
            (*smpl_rw)[2][j][k] = (*smpl_rw)[0][j][k] -
              (*smpl_rw)[2][j][k] + surround_sample;
            (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] -
              (*smpl_rw)[2][j][k] - surround_sample;
            (*smpl_rw)[0][j][k] = tmp_sample;
          }
        }
        else {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[2][j][k];
            (*smpl_rw)[2][j][k] = (*smpl_rw)[0][j][k] -
              (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
            (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] -
              (*smpl_rw)[2][j][k] - (*smpl_rw)[4][j][k];
            (*smpl_rw)[0][j][k] = tmp_sample;
          }
        }
        break;

      case 2:
        if (state_com->mc_header.dematrix_procedure == 2) {
          for (j=0; j<36; j++) {
            surround_sample = ((*smpl_rw)[3][j][k] + (*smpl_rw)[4][j][k]) * 0.5f;
            tmp_sample = (*smpl_rw)[2][j][k];
            (*smpl_rw)[2][j][k] = (*smpl_rw)[1][j][k] -
              (*smpl_rw)[2][j][k] - surround_sample;
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] -
              (*smpl_rw)[2][j][k] + surround_sample;
            (*smpl_rw)[1][j][k] = tmp_sample;
          }
        }
        else {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[2][j][k];
            (*smpl_rw)[2][j][k] = (*smpl_rw)[1][j][k] -
              (*smpl_rw)[2][j][k] - (*smpl_rw)[4][j][k];
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (
              *smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
            (*smpl_rw)[1][j][k] = tmp_sample;
          }
        }
        break;

      case 3:
        if (state_com->mc_header.dematrix_procedure == 2) {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[3][j][k];
            (*smpl_rw)[3][j][k] = -2.0f * ((*smpl_rw)[0][j][k] -
              (*smpl_rw)[2][j][k] -  (*smpl_rw)[3][j][k]) - (*smpl_rw)[4][j][k];
            (*smpl_rw)[1][j][k] = (*smpl_rw)[0][j][k] + (*smpl_rw)[1][j][k] -
              2.0f * (*smpl_rw)[2][j][k] - tmp_sample;
            (*smpl_rw)[0][j][k] = tmp_sample;
          }
        }
        else {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[3][j][k];
            (*smpl_rw)[3][j][k] = (*smpl_rw)[0][j][k] -
              (*smpl_rw)[3][j][k] - (*smpl_rw)[2][j][k];
            (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] -
              (*smpl_rw)[2][j][k] - (*smpl_rw)[4][j][k];
            (*smpl_rw)[0][j][k] = tmp_sample;
          }
        }
        break;

      case 4:
        if (state_com->mc_header.dematrix_procedure == 2) {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[4][j][k];
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] + (*smpl_rw)[1][j][k] -
              2.0f * (*smpl_rw)[2][j][k] - (*smpl_rw)[4][j][k];
            (*smpl_rw)[4][j][k] = 2.0f * (*smpl_rw)[1][j][k] -
              2.0f * ((*smpl_rw)[2][j][k] + (*smpl_rw)[4][j][k]) - (*smpl_rw)[3][j][k];
            (*smpl_rw)[1][j][k] = tmp_sample;
          }
        }
        else {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[4][j][k];
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
            (*smpl_rw)[4][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[4][j][k] - (*smpl_rw)[2][j][k];
            (*smpl_rw)[1][j][k] = tmp_sample;
          }
        }
        break;

      case 5:
        if (state_com->mc_header.dematrix_procedure == 2) {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[3][j][k];
            (*smpl_rw)[3][j][k] = 0.5f * ((*smpl_rw)[1][j][k] -
              (*smpl_rw)[0][j][k] + (*smpl_rw)[3][j][k] - (*smpl_rw)[4][j][k]);
            (*smpl_rw)[0][j][k] = tmp_sample;
            (*smpl_rw)[1][j][k] = (*smpl_rw)[4][j][k];
            (*smpl_rw)[4][j][k] = (*smpl_rw)[3][j][k];
          }
        }
        else {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[3][j][k];
            (*smpl_rw)[3][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[3][j][k] - (*smpl_rw)[2][j][k];
            (*smpl_rw)[0][j][k] = tmp_sample;
            tmp_sample = (*smpl_rw)[4][j][k];
            (*smpl_rw)[4][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[4][j][k] - (*smpl_rw)[2][j][k];
            (*smpl_rw)[1][j][k] = tmp_sample;
          }
        }
        break;

      case 6:
        if (state_com->mc_header.dematrix_procedure == 2) {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[2][j][k];
            tmp_sample1 = (*smpl_rw)[3][j][k];
            (*smpl_rw)[3][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] + (*smpl_rw)[3][j][k] - (*smpl_rw)[4][j][k];
            (*smpl_rw)[2][j][k] = 0.5f * ((*smpl_rw)[0][j][k] + (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k] - tmp_sample1);
            (*smpl_rw)[1][j][k] = tmp_sample;
            (*smpl_rw)[0][j][k] = tmp_sample1;
          }
        }
        else {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[2][j][k];
            (*smpl_rw)[2][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[4][j][k];
            (*smpl_rw)[1][j][k] = tmp_sample;
            tmp_sample = (*smpl_rw)[3][j][k];
            (*smpl_rw)[3][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[3][j][k] - (*smpl_rw)[2][j][k];
            (*smpl_rw)[0][j][k] = tmp_sample;
          }
        }
        break;

      case 7:
        if (state_com->mc_header.dematrix_procedure == 2) {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[2][j][k];
            tmp_sample1 = (*smpl_rw)[4][j][k];
            (*smpl_rw)[4][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[0][j][k] + (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k] - (*smpl_rw)[4][j][k];
            (*smpl_rw)[2][j][k] = 0.5f * ((*smpl_rw)[0][j][k] + (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k] - tmp_sample1);
            (*smpl_rw)[0][j][k] = tmp_sample;
            (*smpl_rw)[1][j][k] = tmp_sample1;
          }
        }
        else {
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[2][j][k];
            (*smpl_rw)[2][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
            (*smpl_rw)[0][j][k] = tmp_sample;
            tmp_sample = (*smpl_rw)[4][j][k];
            (*smpl_rw)[4][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[4][j][k] - (*smpl_rw)[2][j][k];
            (*smpl_rw)[1][j][k] = tmp_sample;
          }
        }
        break;

        }
      }
      else if (state_com->mc_header.surround == 1 && state_com->mc_header.center != 0) {
        switch(tc_alloc) {
      case 0:
        if (state_com->mc_header.dematrix_procedure == 2)
          for (j=0; j<36; j++) {
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] + (*smpl_rw)[3][j][k];
          }
        else
          for (j=0; j<36; j++) {
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
            (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
          }
          break;

      case 1:
        if (state_com->mc_header.dematrix_procedure == 2)
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[2][j][k];
            (*smpl_rw)[2][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] + (*smpl_rw)[3][j][k];
            (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
            (*smpl_rw)[0][j][k] = tmp_sample;
          }
        else
          for (j=0; j<36; j++) {
            tmp_sample = (*smpl_rw)[2][j][k];
            (*smpl_rw)[2][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
            (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
            (*smpl_rw)[0][j][k] = tmp_sample;
          }
          break;

      case 2:
        for (j=0; j<36; j++) {
          tmp_sample = (*smpl_rw)[2][j][k];
          (*smpl_rw)[2][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
          if (state_com->mc_header.dematrix_procedure == 2)
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] + (*smpl_rw)[3][j][k];
          else
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
          (*smpl_rw)[1][j][k] = tmp_sample;
        }
        break;

      case 3:
        for (j=0; j<36; j++) {
          tmp_sample = (*smpl_rw)[3][j][k];
          if (state_com->mc_header.dematrix_procedure == 2)
            (*smpl_rw)[3][j][k] = -(*smpl_rw)[0][j][k] + (*smpl_rw)[2][j][k] + (*smpl_rw)[3][j][k];
          else
            (*smpl_rw)[3][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
          (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
          (*smpl_rw)[0][j][k] = tmp_sample;
        }
        break;

      case 4:
        for (j=0; j<36; j++) {
          tmp_sample = (*smpl_rw)[3][j][k];
          (*smpl_rw)[3][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
          if (state_com->mc_header.dematrix_procedure == 2)
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] + (*smpl_rw)[3][j][k];
          else
            (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k] - (*smpl_rw)[3][j][k];
          (*smpl_rw)[1][j][k] = tmp_sample;
        }
        break;

      case 5:
        for (j=0; j<36; j++) {
          tmp_sample = (*smpl_rw)[2][j][k];
          tmp_sample1 = (*smpl_rw)[3][j][k];
          (*smpl_rw)[2][j][k] = 0.5f * ((*smpl_rw)[0][j][k] + (*smpl_rw)[1][j][k] - tmp_sample - tmp_sample1);
          (*smpl_rw)[3][j][k] = 0.5f * ((*smpl_rw)[1][j][k] - (*smpl_rw)[0][j][k] + tmp_sample - tmp_sample1);
          (*smpl_rw)[0][j][k] = tmp_sample;
          (*smpl_rw)[1][j][k] = tmp_sample1;
        }
        break;
        }
      }
      else if (state_com->mc_header.surround == 1 || state_com->mc_header.center != 0) {
        switch(tc_alloc) {
      case 0:
        for (j=0; j<36; j++) {
          (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k];
          (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k];
        }
        break;

      case 1:
        for (j=0; j<36; j++) {
          tmp_sample = (*smpl_rw)[2][j][k];
          (*smpl_rw)[2][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k];
          (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k];
          (*smpl_rw)[0][j][k] = tmp_sample;
        }
        break;

      case 2:
        for (j=0; j<36; j++) {
          tmp_sample = (*smpl_rw)[2][j][k];
          (*smpl_rw)[2][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[2][j][k];
          (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k];
          (*smpl_rw)[1][j][k] = tmp_sample;
        }
        break;
        }
      }
      else if (state_com->mc_header.surround == 2) {
        switch(tc_alloc) {
      case 0:
        for (j=0; j<36; j++) {
          (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k];
          (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[3][j][k];
        }
        break;

      case 1:
        for (j=0; j<36; j++) {
          tmp_sample = (*smpl_rw)[2][j][k];
          (*smpl_rw)[1][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[3][j][k];
          (*smpl_rw)[2][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k];
          (*smpl_rw)[0][j][k] = tmp_sample;
        }
        break;

      case 2:
        for (j=0; j<36; j++) {
          tmp_sample = (*smpl_rw)[3][j][k];
          (*smpl_rw)[0][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k];
          (*smpl_rw)[3][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[3][j][k];
          (*smpl_rw)[1][j][k] = tmp_sample;
        }
        break;

      case 3:
        for (j=0; j<36; j++) {
          tmp_sample = (*smpl_rw)[2][j][k];
          (*smpl_rw)[2][j][k] = (*smpl_rw)[0][j][k] - (*smpl_rw)[2][j][k];
          (*smpl_rw)[0][j][k] = tmp_sample;
          tmp_sample = (*smpl_rw)[3][j][k];
          (*smpl_rw)[3][j][k] = (*smpl_rw)[1][j][k] - (*smpl_rw)[3][j][k];
          (*smpl_rw)[1][j][k] = tmp_sample;
        }
        break;
        }
      }
    }
  }
}

#if !defined (__INTEL_COMPILER) && defined( _MSC_VER)
#pragma optimize( "", on )
#endif

void mp3dec_mc_denormalizing(MP3Dec *state)
{
  MP3Dec_com *state_com = &(state->com);
  Ipp32f m1;
  Ipp32f m2;
  Ipp32f m3;
  Ipp32s stereo = state_com->stereo;
  Ipp32s channels = stereo + state_com->mc_channel;
  samplefloatrw *smpl_rw = state->smpl_rw;

  switch (state_com->mc_header.dematrix_procedure) {
    case 0:
    case 2:
      m1 = 1.0f + 1.414213562f;
      m2 = m1 * 1.414213562f;
      m3 = m1 * 1.414213562f;
      break;
    case 1:
      m1 = 1.5f + 0.5f * 1.414213562f;
      m2 = m1 * 1.414213562f;
      m3 = m1 * 2.0f;
      break;
    default:
    case 3:
      m1 = 1.0f;
      m2 = 1.0f;
      m3 = 1.0f;
      break;
  }

  if (state_com->mc_header.dematrix_procedure != 3 ) {
    ippsMulC_32f_I(m1, &(*smpl_rw)[0][0][0], stereo * 36 * 32);

    if (state_com->mc_header.dematrix_procedure != 1)
    {
      if (state_com->mc_header.surround == 3) {
        if (state_com->mc_header.center != 0)
          ippsMulC_32f_I(m2, &(*smpl_rw)[2][0][0], 36 * 32);
      }
      else {
        ippsMulC_32f_I(m2, &(*smpl_rw)[2][0][0], 36 * 32 * (channels - 2));
      }
    }
    else
    {
      if (state_com->mc_header.surround == 3) {
        if (state_com->mc_header.center != 0)
          ippsMulC_32f_I(m2, &(*smpl_rw)[2][0][0], 36 * 32);
      }
      else if (state_com->mc_channel == 3) {
        ippsMulC_32f_I(m2, &(*smpl_rw)[2][0][0], 36 * 32);
        ippsMulC_32f_I(m3, &(*smpl_rw)[3][0][0], 36 * 32 * 2);
      }
      else if (state_com->mc_channel == 2)
      {
        if (state_com->mc_header.surround == 2){
          ippsMulC_32f_I(m3, &(*smpl_rw)[2][0][0], 36 * 32 * 2);
        }
        else  {
          ippsMulC_32f_I(m2, &(*smpl_rw)[2][0][0], 36 * 32);
          ippsMulC_32f_I(m3, &(*smpl_rw)[3][0][0], 36 * 32);
        }
      }
      else
      {
        if (state_com->mc_header.center == 0){
          ippsMulC_32f_I(m3, &(*smpl_rw)[2][0][0], 36 * 32);
        }
        else
          ippsMulC_32f_I(m2, &(*smpl_rw)[2][0][0], 36 * 32);
      }
    }
  }
}

void mp3dec_mc_lfe_l2(MP3Dec *state, Ipp32f *out)
{
  Ipp32s idx = state->com.alloc_table[state->com.mc_lfe_alloc];
  Ipp32s x = mp3_numbits[idx];
  Ipp32s xor_coef;
  Ipp32f scale;

  xor_coef = (1 << (x - 1));

  scale = mp3dec_C_quant[idx] * mp3dec_scale_values[state->com.mc_lfe_scf];

  ippsXorC_32u_I(xor_coef, (Ipp32u *)state->com.mc_lfe_spl, 12);
  ippsLShiftC_32s_I(32 - x, state->com.mc_lfe_spl, 12);
  ippsConvert_32s32f_Sfs(state->com.mc_lfe_spl, out, 12, 0);
  ippsAddC_32f_I(mp3dec_D_quant[idx], out, 12);
  ippsMulC_32f_I(scale, out, 12);
}

#endif //UMC_ENABLE_MP3_AUDIO_DECODER
