/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_MP3_AUDIO_ENCODER)

#include "mp3enc_own_fp.h"

static Ipp32f mp3_scale_values[64] = {
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

static Ipp32f mp3_rscale_values[64] = {
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

static const Ipp32f mpa_tab_a_l1[14] = {
  0.750000000f, 0.875000000f, 0.937500000f,
  0.968750000f, 0.984375000f, 0.992187500f,
  0.996093750f, 0.998046875f, 0.999023438f,
  0.999511719f, 0.999755859f, 0.999877930f,
  0.999938965f, 0.999969482f};

static const Ipp32f mpa_tab_b_l1[14] = {
  -0.250000000f, -0.125000000f, -0.062500000f,
  -0.031250000f, -0.015625000f, -0.007812500f,
  -0.003906250f, -0.001953125f, -0.000976563f,
  -0.000488281f, -0.000244141f, -0.000122070f,
  -0.000061035f, -0.000030518f};

static const Ipp32f mpa_tab_snr_l1[18] = {
   0.00f,  7.00f, 16.00f,
  25.28f, 31.59f, 37.75f,
  43.84f, 49.89f, 55.93f,
  61.96f, 67.98f, 74.01f,
  80.03f, 86.05f, 92.01f, 98.01f,
};

static const Ipp32f mpa_tab_a_l2[17] = {
  0.750000000f, 0.625000000f, 0.875000000f,
  0.562500000f, 0.937500000f, 0.968750000f,
  0.984375000f, 0.992187500f, 0.996093750f,
  0.998046875f, 0.999023438f, 0.999511719f,
  0.999755859f, 0.999877930f, 0.999938965f,
  0.999969482f, 0.999984741f
};

static const Ipp32f mpa_tab_b_l2[17] = {
  -0.250000000f, -0.375000000f, -0.125000000f,
  -0.437500000f, -0.062500000f, -0.031250000f,
  -0.015625000f, -0.007812500f, -0.003906250f,
  -0.001953125f, -0.000976563f, -0.000488281f,
  -0.000244141f, -0.000122070f, -0.000061035f,
  -0.000030518f, -0.000015259f
};

static const Ipp32f mpa_tab_snr_l2[18] = {
   0.00f,  7.00f, 11.00f,
  16.00f, 20.84f, 25.28f,
  31.59f, 37.75f, 43.84f,
  49.89f, 55.93f, 61.96f,
  67.98f, 74.01f, 80.03f,
  86.05f, 92.01f, 98.01f
};

static const Ipp32s mp3enc_pattern_l2[5][5] = {
  {0x123, 0x122, 0x122, 0x133, 0x123},
  {0x113, 0x111, 0x111, 0x444, 0x113},
  {0x111, 0x111, 0x111, 0x333, 0x113},
  {0x222, 0x222, 0x222, 0x333, 0x123},
  {0x123, 0x122, 0x122, 0x133, 0x123}
};

static const Ipp32s mp3enc_sfs_per_scfsi[] = {3, 2, 1, 2};

static void mp3enc_bit_allocation_l1(MP3Enc *state)
{
  VM_ALIGN32_DECL(Ipp32f) mnr[2][32];
  VM_ALIGN32_DECL(Ipp8u) used[2][32];
  Ipp32f (*smr)[32] = state->pa_snr;
  Ipp16s (*allocation)[32] = state->com.allocation;
  Ipp32s adb = state->com.slot_size << 3;

  Ipp32s i, k, smpl_bits, scale_bits, min_sb, min_ch, oth_ch;
  Ipp32s bspl, bscf, bbal;
  Ipp32f smal;
  Ipp32s stereo  = state->com.stereo;
  Ipp32s sblimit = state->com.sblimit_real;
  Ipp32s jsbound = 32;
  Ipp32s banc = 32;
  Ipp32s berr = 16;
  Ipp32s len = stereo << 5;

  bbal = 4 * ((jsbound * stereo) + (32 - jsbound));
  adb -= bbal + berr + banc;

  ippsSubCRev_32f(&smr[0][0], mpa_tab_snr_l1[0], &mnr[0][0], len);
  ippsZero_16s(&allocation[0][0], len);
  ippsZero_8u(&used[0][0], len);

  bspl = bscf = 0;

  do {
    smal = mnr[0][0] + 1;
    min_sb = -1;
    min_ch = -1;

    for(k = 0; k < stereo; k++) {
      for(i = 0; i < sblimit; i++) {
        if (used[k][i] != 2 && mnr[k][i] < smal) {
          smal = mnr[k][i];
          min_sb = i;
          min_ch = k;
        }
      }
    }

    if (min_sb > -1) {
      if (used[min_ch][min_sb]) {
        smpl_bits = 12;
        scale_bits = 0;
      } else {
        smpl_bits = 24;
        scale_bits = 6;
      }
      if (min_sb >= jsbound)
        scale_bits *= stereo;

      if (adb >= bspl + bscf + scale_bits + smpl_bits) {
        bspl += smpl_bits;
        bscf += scale_bits;
        allocation[min_ch][min_sb]++;
        used[min_ch][min_sb] = 1;
        mnr[min_ch][min_sb] = mpa_tab_snr_l1[allocation[min_ch][min_sb]] -
          smr[min_ch][min_sb];
        if (allocation[min_ch][min_sb] ==  14)
          used[min_ch][min_sb] = 2;
      }
      else
        used[min_ch][min_sb] = 2;

      if(stereo == 2 && min_sb >= jsbound) {
        oth_ch = 1 - min_ch;
        allocation[oth_ch][min_sb] = allocation[min_ch][min_sb];
        used[oth_ch][min_sb] = used[min_ch][min_sb];
        mnr[oth_ch][min_sb] = mpa_tab_snr_l1[allocation[oth_ch][min_sb]] -
          smr[oth_ch][min_sb];
      }
    }
  } while(min_sb > -1);
}

Ipp32s mp3enc_quantization_l1(MP3Enc *state)
{
  VM_ALIGN32_DECL(Ipp32f) fbout[12];
  Ipp32s gr, ch, sb;
  Ipp32s stereo = state->com.stereo;
  Ipp32s index, bits_per_sample;
  Ipp32s sblimit = state->com.sblimit_real;
  Ipp32f min, max, tmp, cmul, cadd, fxor_coef;
  Ipp32s xor_coef;

  mp3enc_bit_allocation_l1(state);

  for (ch = 0; ch < stereo; ch++) {
    for (sb = 0; sb < sblimit; sb++) {
      if (state->com.allocation[ch][sb]) {
        max = 0;
        bits_per_sample = state->com.allocation[ch][sb] + 1;

        for (gr = 0; gr < 12; gr++) {
          tmp = fbout[gr] = state->fbout_data[0][ch][gr][sb];
        }
        ippsMinMax_32f(fbout, 12, &min, &max);
        if (max < -min)
          max = -min;

        index = 0;
        for (gr = 62; gr > 0; gr--){
          if (max < mp3_scale_values[gr]) {
            index = gr;
            break;
          }
        }
        state->com.scalefactor_l1[ch][sb] = (Ipp16s)index;

        xor_coef = (1 << (bits_per_sample - 1));
        fxor_coef = (Ipp32f)xor_coef;

        cmul = mp3_rscale_values[index] * mpa_tab_a_l1[bits_per_sample - 2];
        cadd = mpa_tab_b_l1[bits_per_sample - 2];

        ippsMulC_32f_I(cmul, fbout, 12);
        ippsAddC_32f_I(cadd, fbout, 12);
        for (gr = 0; gr < 12; gr++) {
          tmp = fbout[gr];

          if (tmp >= 0) {
            state->com.sample[ch][sb][gr] = ((Ipp32s)(tmp * fxor_coef)) | xor_coef;
          } else {
            state->com.sample[ch][sb][gr] = ((Ipp32s)((tmp + 1.0f) * fxor_coef));
          }
        }
      }
    }
  }
  return 1;
}

static void mp3enc_scale_factor_calc_l2(MP3Enc *state)
{
  VM_ALIGN32_DECL(Ipp32f) fbout[12];
  Ipp32s j, t;
  Ipp32s ch, sb, index;
  Ipp32f max;
  Ipp32s stereo = state->com.stereo;
  Ipp32s sblimit_real = state->com.sblimit_real;

  if (state->com.mc_header.lfe && state->com.mc_lfe_alloc) {
    //ippsCopy_32f(state->mc_lfe_buf, fbout, 12);
    //ippsAbs_32f_I(fbout, 12);
    ippsAbs_32f(state->mc_lfe_buf, fbout, 12);
    ippsMax_32f(fbout, 12, &max);
    index = 0;
    for (j = 62; j > 0; j--) {
      if (max < mp3_scale_values[j]) {
        index = j;
        break;
      }
    }
    state->com.mc_lfe_scf = index;
  }

  for (ch = 0; ch < stereo + state->com.mc_channel; ch++) {
    for (t = 0; t < 3; t++) {
      for (sb = 0; sb < sblimit_real; sb++){
        for (j = 0; j < 12; j++)
          fbout[j] = fabsf(state->fbout_data[ch][0][t * 12 + j][sb]);
        ippsMax_32f(fbout, 12, &max);

        index = 0;
        for (j = 62; j > 0; j--) {
          if (max < mp3_scale_values[j]) {
            index = j;
            break;
          }
        }
        state->com.scalefactor[ch][t][sb] = (Ipp16s)index;
      }
      for (sb = sblimit_real; sb < 32; sb++)
        state->com.scalefactor[ch][t][sb] = 63;
    }
  }
}

static void mp3enc_transmission_pattern_l2(MP3Enc *state)
{
  Ipp16s (*sf)[3][32] = state->com.scalefactor;
  Ipp16u (*si)[32] = state->com.scfsi;
  Ipp32s dscf[2];
  Ipp32s cls[2], i, j, k;
  Ipp32s stereo = state->com.stereo;
  Ipp32s sblimit = state->com.sblimit;

  for(k = 0; k < stereo + state->com.mc_channel; k++)
    for(i = 0; i < sblimit; i++) {
      dscf[0] =  (sf[k][0][i]-sf[k][1][i]);
      dscf[1] =  (sf[k][1][i]-sf[k][2][i]);
      for(j = 0; j < 2; j++) {
        if (dscf[j] <= -3) cls[j] = 0;
        else if (dscf[j] > -3 && dscf[j] < 0) cls[j] = 1;
        else if (dscf[j] == 0) cls[j] = 2;
        else if (dscf[j] > 0 && dscf[j] < 3) cls[j] = 3;
        else cls[j] = 4;
      }
      switch (mp3enc_pattern_l2[cls[0]][cls[1]]) {
        case 0x123 :    si[k][i] = 0;
                        break;
        case 0x122 :    si[k][i] = 3;
                        sf[k][2][i] = sf[k][1][i];
                        break;
        case 0x133 :    si[k][i] = 3;
                        sf[k][1][i] = sf[k][2][i];
                        break;
        case 0x113 :    si[k][i] = 1;
                        sf[k][1][i] = sf[k][0][i];
                        break;
        case 0x111 :    si[k][i] = 2;
                        sf[k][1][i] = sf[k][2][i] = sf[k][0][i];
                        break;
        case 0x222 :    si[k][i] = 2;
                        sf[k][0][i] = sf[k][2][i] = sf[k][1][i];
                        break;
        case 0x333 :    si[k][i] = 2;
                        sf[k][0][i] = sf[k][1][i] = sf[k][2][i];
                        break;
        case 0x444 :    si[k][i] = 2;
                        if (sf[k][0][i] > sf[k][2][i])
                             sf[k][0][i] = sf[k][2][i];
                        sf[k][1][i] = sf[k][2][i] = sf[k][0][i];
      }
    }
}

static Ipp32s mp3enc_bits_js_l2(MP3Enc *state)
{
  Ipp32s i, sb, ch, ba;
  Ipp16u (*scfsi)[32] = state->com.scfsi;
  Ipp32f (*smr)[32] = state->pa_snr;
  Ipp32s stereo  = state->com.stereo;
  Ipp32s sblimit = state->com.sblimit;
  Ipp32s sblimit_real = state->com.sblimit_real;
  Ipp32s jsbound = state->com.jsbound;
  Ipp32s req_bits = 0, bbal = 0, berr = 16, banc = 32;
  Ipp32s maxAlloc, sel_bits, sc_bits, smp_bits;

  Ipp32s *nbal_alloc_table = state->com.nbal_alloc_table;
  Ipp8u *alloc_table = state->com.alloc_table;

  for(i = 0; i < jsbound; i++)
    bbal += nbal_alloc_table[i] << (stereo - 1);

  for(i = jsbound; i < sblimit; i++)
    bbal += nbal_alloc_table[i];

  req_bits = banc + bbal + berr;

  for(sb = 0; sb < sblimit_real; sb++)
    for(ch = 0; ch < ((sb < jsbound) ? stereo : 1); ch++) {
      maxAlloc = (1 << nbal_alloc_table[sb]) - 1;
      sel_bits = sc_bits = smp_bits = 0;
      for(ba = 0; ba < maxAlloc - 1; ba++)
        if(mpa_tab_snr_l2[alloc_table[sb * 16 + ba] + ((ba > 0) ? 1 : 0)] >= smr[ch][sb])
          break;
      if(stereo == 2 && sb >= jsbound)
        for(; ba < maxAlloc - 1; ba++)
          if(mpa_tab_snr_l2[alloc_table[sb * 16 + ba] + ((ba > 0) ? 1 : 0)] >= smr[1 - ch][sb])
            break;
      if(ba > 0) {
        Ipp32s nb = mp3_cls_quant[alloc_table[sb * 16 + ba]];
        smp_bits = (nb < 0) ? -12 * nb : 36 * nb;

        sel_bits = 2;
        sc_bits  = 6 * mp3enc_sfs_per_scfsi[scfsi[ch][sb]];
        if(stereo == 2 && sb >= jsbound) {
          sel_bits += 2;
          sc_bits  += 6 * mp3enc_sfs_per_scfsi[scfsi[1-ch][sb]];
        }
        req_bits += smp_bits + sel_bits + sc_bits;
      }
    }
    return req_bits;
}
static void mp3enc_bit_allocation_l2(MP3Enc *state)
{
  VM_ALIGN32_DECL(Ipp32f) mnr[NUM_CHANNELS][32];
  VM_ALIGN32_DECL(Ipp8u) used[NUM_CHANNELS][32];
  Ipp16u (*scfsi)[32] = state->com.scfsi;
  Ipp32f (*smr)[32] = state->pa_snr;
  Ipp16s (*allocation)[32] = state->com.allocation;
  Ipp32f smal;
  Ipp32s adb = (state->com.slot_size << 3);

  Ipp32s i, min_ch, min_sb, oth_ch, k, kk, increment, scale, seli, ba, tb;
  Ipp32s bspl, bscf, bsel, bbal;
  Ipp32s stereo  = state->com.stereo;
  Ipp32s mc_channel = state->com.mc_channel;
  Ipp32s mc_sblimit = state->com.mc_sblimit;
  Ipp32s mc_channel_conf = state->com.mc_channel_conf;
  Ipp32s sblimit = state->com.sblimit;
  Ipp32s sblimit_real = state->com.sblimit_real;
  Ipp32s jsbound = state->com.jsbound;
  Ipp32s len = stereo << 5;
  Ipp32s mc_len = mc_channel << 5;
  Ipp32s mode, mode_ext;
  Ipp32s rq_db;

  Ipp32s banc = 32+16;
  Ipp32s berr = 16;

  Ipp32s *nbal_alloc_table = state->com.nbal_alloc_table;
  Ipp8u  *alloc_table = state->com.alloc_table;
  if (mc_channel) {
    berr += 16;
    banc += 16+3;
  }
  if (state->com.mc_header.lfe) {
    banc += 70;
  }

  mode = MPA_LR_STEREO;
  if(state->com.stereo_mode_param == MPA_JOINT_STEREO) {
    mode_ext = 0;
    jsbound = sblimit;
    if((rq_db = mp3enc_bits_js_l2(state)) > adb) {
      mode = MPA_JOINT_STEREO;
      mode_ext = 4;
      do {
        mode_ext--;
        jsbound = 4 + mode_ext * 4;
        state->com.jsbound = jsbound;
        rq_db = mp3enc_bits_js_l2(state);
      } while( (rq_db > adb) && (mode_ext > 0));
      state->com.jsbound = jsbound;
      state->com.stereo_mode_ext = mode_ext;
    }
  }
  state->com.stereo_mode = mode;
  bbal = 0;

  for(i = 0; i < jsbound; i++)
    bbal += nbal_alloc_table[i] << (stereo - 1);

  for(i = jsbound; i < sblimit; i++)
    bbal += nbal_alloc_table[i];

  if (mc_channel) {
     for(i = 0; i < mc_sblimit; i++) {
        bbal += nbal_alloc_table[i] * mc_channel;
     }

     switch ( mc_channel_conf ) {
     case 320 :
     case 310 :
        tb = 3; break;
     case 220 :
     case 300 :
     case 302 : 
     case 210 :
        tb = 2; break;
     default :
        tb = 0;
     }

     if (state->com.mc_tc_sbgr_select == 0)
        bbal += 12 * tb;
     else
        bbal += tb;
  }

  adb -= bbal + berr + banc;

  ippsSubCRev_32f(&smr[0][0], mpa_tab_snr_l2[0], &mnr[0][0], len + mc_len);
  ippsZero_16s(&allocation[0][0], len + mc_len);
  ippsZero_8u(&used[0][0], len + mc_len);

  bspl = bscf = bsel = 0;

  do  {
    smal = mnr[0][0] + 1;
    min_sb = -1;
    min_ch = -1;

    for(k = 0; k < stereo + mc_channel; k++) {
      for(i = 0; i < sblimit_real; i++) {
        kk = mp3enc_mc_trans_channel (&state->com, mp3_mc_sb_group[i], k);
        if ((i >= 12) && (state->com.mc_header.center == 3) && (kk == 2))
          used[kk][i] = 2;
        if (used[kk][i] != 2 && smal > mnr[kk][i]) {
          smal = mnr[kk][i];
          min_sb = i;
          min_ch = kk;
        }
      }
    }

      if(min_sb > -1) {
        Ipp32s nba = allocation[min_ch][min_sb];
        Ipp32s nb1 = mp3_cls_quant[alloc_table[(min_sb << 4) + nba + 1]];
        Ipp32s nb0 = mp3_cls_quant[alloc_table[(min_sb << 4) + nba]];

        increment = (nb1 < 0) ? (-12 * nb1) : (36 * nb1);
        if (used[min_ch][min_sb])
          increment -= (nb0 < 0) ? (-12 * nb0): (36 * nb0);

        oth_ch = 1 - min_ch;
        if (used[min_ch][min_sb]) scale = seli = 0;
        else {
          seli = 2;
          scale = 6 * mp3enc_sfs_per_scfsi[scfsi[min_ch][min_sb]];
          if(stereo == 2 && min_sb >= jsbound) {
            seli += 2;
            scale += 6 * mp3enc_sfs_per_scfsi[scfsi[oth_ch][min_sb]];
          }
        }

        if (adb >= bspl + bscf + bsel + seli + scale + increment) {
          ba = ++allocation[min_ch][min_sb];

          bspl += increment;
          bscf += scale;
          bsel += seli;
          used[min_ch][min_sb] = 1;
          mnr[min_ch][min_sb] = mpa_tab_snr_l2[alloc_table[min_sb * 16 + ba] + 1] -
              smr[min_ch][min_sb];
          if (ba >= (1 << nbal_alloc_table[min_sb]) - 1)
            used[min_ch][min_sb] = 2;
        }
        else
          used[min_ch][min_sb] = 2;

        if(min_sb >= jsbound && stereo == 2) {
          ba = allocation[oth_ch][min_sb] = allocation[min_ch][min_sb];
          used[oth_ch][min_sb] = used[min_ch][min_sb];
          mnr[oth_ch][min_sb] = mpa_tab_snr_l2[alloc_table[min_sb * 16 + ba] + 1] -
              smr[oth_ch][min_sb];
        }
      }
  } while(min_sb > -1);

}

static void mp3enc_subband_quantization_l2(MP3Enc *state)
{
  VM_ALIGN32_DECL(Ipp32f) fbout[36];
  Ipp32s i, j, k, kk, s, n, qnt, bal, xor_coef, ind;
  Ipp32s stereo  = state->com.stereo;
  Ipp32s sblimit = state->com.sblimit;
  Ipp32s jsbound = state->com.jsbound;
  Ipp32f d, fxor_coef, rscale;
  Ipp32s d1,d2;
  Ipp16s (*sf)[3][32] = state->com.scalefactor;
  Ipp16s (*allocation)[32] = state->com.allocation;

  for (k = 0; k < stereo + state->com.mc_channel; k++) {
    Ipp32s limit;
    if (k == 1)
      limit = IPP_MIN(jsbound, sblimit);
    else
      limit = sblimit;

    for (i = 0; i < limit; i++)
      if ((bal = allocation[k][i]) != 0) {
        qnt = state->com.alloc_table[i * 16 + bal];
        n = mp3_numbits[qnt] - 1;
        xor_coef = 1 << n;
        fxor_coef = (Ipp32f)xor_coef;

        kk = k;
        if (k == 0 && i >= jsbound)
          kk = 2;

        ind = 0;
        for (s = 0; s < 3; s++) {
          for (j = 0; j < 12; j++) {
            fbout[ind++] = state->fbout_data[kk][0][s * 12 + j][i];
          }
        }
        ind = 0;
        for (s = 0; s < 3; s++) {
          rscale = mp3_rscale_values[sf[kk][s][i]] * mpa_tab_a_l2[qnt];
          ippsMulC_32f_I(rscale, fbout + ind, 12);
          ippsAddC_32f_I(mpa_tab_b_l2[qnt], fbout + ind, 12);
          ind += 12;
        }

        for (j = 0; j < 36; j++) {
          d = fbout[j];
          
          d1 = (Ipp32s)(d * fxor_coef) | xor_coef;
          d2 = (Ipp32s)((d + 1.0f) * fxor_coef);
          state->com.sample[k][i][j] = (d >= 0) ? d1 : d2;
          //if (d >= 0) {
          //  state->com.sample[k][i][j] = (Ipp32s)(d * fxor_coef) | xor_coef;
          //} else {
          //  state->com.sample[k][i][j] = (Ipp32s)((d + 1.0f) * fxor_coef);
          //}
        }
      }
  }
}

static void mp3enc_lfe_quantization_l2(MP3Enc *state)
{
  Ipp32s j, n, qnt, xor_coef;
  Ipp32f d, fxor_coef, rscale;
  Ipp32s d1,d2;

  if (state->com.mc_header.lfe && state->com.mc_lfe_alloc)
  {
    qnt = 5;
    n = mp3_numbits[qnt] - 1;
    xor_coef = 1 << n;
    fxor_coef = (Ipp32f)xor_coef;

    rscale = mp3_rscale_values[state->com.mc_lfe_scf] * mpa_tab_a_l2[qnt];
    ippsMulC_32f_I(rscale, state->mc_lfe_buf, 12);
    ippsAddC_32f_I(mpa_tab_b_l2[qnt], state->mc_lfe_buf, 12);

    for (j = 0; j < 12; j++) {
      d = state->mc_lfe_buf[j];

      d1 = (Ipp32s)(d * fxor_coef) | xor_coef;
      d2 = (Ipp32s)((d + 1.0f) * fxor_coef);
      state->com.mc_lfe_spl[j] = (d >= 0) ? d1 : d2;
      //if (d >= 0) {
      //  state->com.mc_lfe_spl[j] = (Ipp32s)(d * fxor_coef) | xor_coef;
      //} else {
      //  state->com.mc_lfe_spl[j] = (Ipp32s)((d + 1.0f) * fxor_coef);
      //}
    }
  }
}

static void mp3enc_join_LR_l2(MP3Enc *state)
{
  Ipp32s sb, j;
  Ipp32s sblimit_real = state->com.sblimit_real;

  for (sb = 0; sb < sblimit_real; sb++)
    for (j = 0; j < 36; j++)
      state->fbout_data[2][0][j][sb] = 0.5f * (state->fbout_data[0][0][j][sb] +
        state->fbout_data[1][0][j][sb]);
}

Ipp32s mp3enc_quantization_l2(MP3Enc *state)
{
  if(state->com.stereo_mode_param == MPA_JOINT_STEREO) {
    mp3enc_join_LR_l2(state);
  }

  if (state->com.mc_header.lfe) {
    state->com.mc_lfe_alloc = 4;
  }

  mp3enc_scale_factor_calc_l2(state);

  mp3enc_transmission_pattern_l2(state);

  mp3enc_bit_allocation_l2(state);

  mp3enc_lfe_quantization_l2(state);
  mp3enc_subband_quantization_l2(state);

  return 1;
}

#endif //UMC_ENABLE_MP3_AUDIO_ENCODER
