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
#if defined (UMC_ENABLE_AC3_AUDIO_DECODER)

#include "ac3_dec.h"
#include "ac3_dec_own_fp.h"
#include "ipps.h"

/********************************************************************/

#define DB11           7509 * 256

#define COMP_CUSTOM_A  0
#define COMP_CUSTOM_D  1
#define COMP_LINE      2
#define COMP_RF        3

static const Ipp16s NFRONT[8] = { 2, 1, 2, 3, 2, 3, 2, 3 };

static const Ipp16s NREAR[8] = { 0, 0, 0, 0, 1, 1, 2, 2 };

//#define m6dB  0.5000345f    /* -6.0 dB */
//#define m3dB  0.7071312f    /* -3.0 dB */
//#define m45dB 0.594292158f  /* -4.5 dB */

#define m6dB  0.5f          /* -6.0 dB */
#define m3dB  0.707106781f  /* -3.0 dB */
#define m45dB 0.594603557f  /* -4.5 dB */

const Ipp32f cmixlev_lut[4] = {
  m3dB, m45dB, m6dB, m45dB
};

const Ipp32f smixlev_lut[4] = {
  m3dB, m6dB, 0, m6dB
};

static const Ipp32f karaokeTable[4][18] = {
  {1.00000000f,  0.00000000f,  0.00000000f,  0.70710677f,
   0.70710677f,  0.00000000f,  0.00000000f,  0.00000000f,
   0.00000000f,  1.00000000f,  1.00000000f,  1.00000000f,
   0.00000000f,  0.00000000f,  0.00000000f,  1.00000000f,
   0.00000000f,  0.00000000f},

  {1.00000000f,  0.70713121f,  0.00000000f,  0.70710677f,
   0.70710677f,  0.70710677f,  0.70710677f,  0.00000000f,
   0.00000000f,  1.00000000f,  1.00000000f,  1.00000000f,
   0.00000000f,  0.00000000f,  0.00000000f,  1.00000000f,
   1.00000000f,  0.00000000f},

  {1.00000000f,  0.00000000f,  0.70713121f,  0.70710677f,
   0.70710677f,  0.00000000f,  0.00000000f,  0.70710677f,
   0.70710677f,  1.00000000f,  1.00000000f,  1.00000000f,
   0.00000000f,  0.00000000f,  0.00000000f,  1.00000000f,
   0.00000000f,  1.00000000f},

  {1.00000000f,  0.70713121f,  0.70713121f,  0.70710677f,
   0.70710677f,  1.00000000f,  0.00000000f,  0.00000000f,
   1.00000000f,  1.00000000f,  1.00000000f, -1.00000000f,
   0.00000000f,  1.00000000f,  1.00000000f,  1.00000000f,
   0.00000000f,  0.00000000f},
};

/********************************************************************/

static Ipp32f drc(Ipp32s compre,
                  Ipp32s compr,
                  Ipp32s dynrng,
                  Ipp32s downmix,
                  Ipp32f dialnorm,
                  AC3Dec *state)
{
  Ipp32s tmp;
  Ipp32f gain;

  if (state->out_compmod == COMP_RF) {
    if (compre) {
      if (compr)        gain = (Ipp32f)compr + DB11;
      else              gain = (Ipp32f)dynrng + DB11;
    } else if (downmix) gain = (Ipp32f)dynrng - DB11;
    else gain = (Ipp32f)dynrng;
  } else if (state->out_compmod == COMP_LINE) {
    if (dynrng > 0)     gain = (Ipp32f)dynrng * state->drc_scaleLow;
    else if (downmix)   gain = (Ipp32f)dynrng;
    else                gain = (Ipp32f)dynrng * state->drc_scaleHigh;
  } else {
    if (dynrng > 0)     gain = (Ipp32f)dynrng * state->drc_scaleLow;
    else                gain = (Ipp32f)dynrng * state->drc_scaleHigh;
    if (downmix)        gain = gain - DB11;
  }

  tmp = (Ipp32s)(gain);
  gain = ((Ipp32f)((tmp & 0xFFFFF) + 1048576)) /
         ((Ipp32f)(1 << (20 - (tmp >> 20))));

  if (state->out_compmod != COMP_CUSTOM_A)
    gain *= dialnorm;

  return gain;
}

/********************************************************************/

void  Downmix(Ipp32s dynrng,
              Ipp32s dynrng2,
              Ipp32s before_imdct,
              AC3Dec *state)
{
  Ipp32s downmix, i;
  Ipp32f cmixgain;  /* center channel mix gain */
  Ipp32f smixgain;  /* surround channel mix gain */
  Ipp32f gain = .0f;
  Ipp16s infront, inrear, outfront, outrear;
  Ipp32f *samples[7];
  Ipp32f *samL  = NULL;
  Ipp32f *samC  = NULL;
  Ipp32f *samR  = NULL;
  Ipp32f *samSL = NULL ;
  Ipp32f *samSR = NULL;
  Ipp32f *samS  = NULL;
  Ipp32f *samLFE= NULL;
  Ipp32f *karaokeSL= NULL;

  if (before_imdct) {
    for (i = 0; i < 7; i++) {
      samples[i] = state->coeffs[i];
    }
  } else {
    for (i = 0; i < 7; i++) {
      samples[i] = state->samples[i];
    }
  }

/* init in_channel */
  switch (state->bsi.acmod) {
  case ACMOD_0:
    samL = samples[0];
    samR = samples[1];

    samC = samples[2];
    samSL = samples[3];
    samSR = samples[4];
    samS = samples[6];
    break;

  case ACMOD_10:
    samC = samples[0];

    samL = samples[1];
    samR = samples[2];
    samSL = samples[3];
    samSR = samples[4];
    samS = samples[6];

    break;
  case ACMOD_20:
    samL = samples[0];
    samR = samples[1];

    samC = samples[2];
    samSL = samples[3];
    samSR = samples[4];
    samS = samples[6];

    break;
  case ACMOD_30:
    samL = samples[0];
    samC = samples[1];
    samR = samples[2];

    samSL = samples[3];
    samSR = samples[4];
    samS = samples[6];

    break;
  case ACMOD_21:
    samL = samples[0];
    samR = samples[1];
    samS = samples[2];

    samC = samples[3];
    samSL = samples[4];
    samSR = samples[6];

    break;
  case ACMOD_31:
    samL = samples[0];
    samC = samples[1];
    samR = samples[2];
    samS = samples[3];

    samSL = samples[4];
    samSR = samples[6];

    break;
  case ACMOD_22:
    samL = samples[0];
    samR = samples[1];
    samSL = samples[2];
    samSR = samples[3];

    samC = samples[4];
    samS = samples[6];

    break;
  case ACMOD_32:
    samL = samples[0];
    samC = samples[1];
    samR = samples[2];
    samSL = samples[3];
    samSR = samples[4];

    samS = samples[6];
    break;

  default:
    break;
  }
  samLFE = samples[5];

/*
 * Set up downmix parameters
 */
  infront = NFRONT[state->bsi.acmod];
  inrear = NREAR[state->bsi.acmod];
  outfront = NFRONT[state->out_acmod];
  outrear = NREAR[state->out_acmod];
  cmixgain = cmixlev_lut[state->bsi.cmixlev];
  smixgain = smixlev_lut[state->bsi.surmixlev];
  downmix = 0;

  if (inrear == 1) {
    karaokeSL = samS;
  } else {
    karaokeSL = samSL;
  }

  if (before_imdct) {
    if (state->bsi.acmod == ACMOD_10) {
      ippsZero_32f(samL, 256);
      ippsZero_32f(samR, 256);
    }

    if ((infront == 2) && (outfront != 2)) {
      ippsZero_32f(samC, 256);
    }

    if (inrear == 0) {
      if (outrear == 2) {
        ippsZero_32f(samSL, 256);
        ippsZero_32f(samSR, 256);
      } else if (outrear == 1) {
        ippsZero_32f(samS, 256);
      }
    } else if (inrear == 1) {
      if (outrear == 2) {
        ippsZero_32f(samSL, 256);
        ippsZero_32f(samSR, 256);
      }
    }

    if (state->bsi.acmod == ACMOD_0) {   /* 1+1 mode, dual independent mono channels present */
      gain = drc(state->bsi.compre, state->bsi.compr, dynrng, 0,
                state->bsi.dialnorm, state);
      ippsMulC_32f_I(gain, samL, 256);
      gain = drc(state->bsi.compr2e, state->bsi.compr2, dynrng2, 0,
                state->bsi.dialnorm2, state);
      ippsMulC_32f_I(gain, samR, 256);

      if (outfront == 1) {        /* 1 front loudspeaker (center) */
        if (state->dualmonomode == DUAL_LEFTMONO) {
          ippsCopy_32f(samL, samC, 256);
        } else if (state->dualmonomode == DUAL_RGHTMONO) {
          ippsCopy_32f(samR, samC, 256);
        } else {
          ippsAdd_32f(samL, samR, samC, 256);
          ippsMulC_32f_I(m6dB, samC, 256);        /* 0.501187233 = -6dB */
        }
      } else if (outfront == 2) {
        if (state->dualmonomode == DUAL_LEFTMONO) {
          ippsMulC_32f_I(m3dB, samL, 256);        /* 0.707945 = -3dB */
          ippsCopy_32f(samL, samR, 256);
        } else if (state->dualmonomode == DUAL_RGHTMONO) {
          ippsMulC_32f_I(m3dB, samR, 256);        /* 0.707945 = -3dB */
          ippsCopy_32f(samR, samL, 256);
        } else if (state->dualmonomode == DUAL_MIXMONO) {
          ippsAdd_32f_I(samR, samL, 256);
          ippsMulC_32f_I(m6dB, samL, 256);        /* 0.501187233 = -6dB */
          ippsCopy_32f(samL, samR, 256);
        }
      } else {
        if (state->dualmonomode == DUAL_LEFTMONO) {
          ippsCopy_32f(samL, samC, 256);
          ippsZero_32f(samL, 256);
          ippsZero_32f(samR, 256);
        } else if (state->dualmonomode == DUAL_RGHTMONO) {
          ippsCopy_32f(samR, samC, 256);
          ippsZero_32f(samL, 256);
          ippsZero_32f(samR, 256);
        } else if (state->dualmonomode == DUAL_MIXMONO) {
          ippsAdd_32f(samL, samR, samC, 256);
          ippsMulC_32f_I(m6dB, samC, 256);        /* 0.501187233 = -6dB */
          ippsZero_32f(samL, 256);
          ippsZero_32f(samR, 256);
        }
      }
    } else if (state->bsi.karaokeMode) {
      Ipp32s karaokeCapable = state->karaokeCapable;
      downmix = 1;

      if (outfront == 1) {
        if (karaokeCapable >= 0) {
          Ipp32f c0 = karaokeTable[karaokeCapable][0];
          Ipp32f c1 = karaokeTable[karaokeCapable][1];
          Ipp32f c2 = karaokeTable[karaokeCapable][2];

          ippsMulC_32f_I(c0, samC, 256);
          ippsAddProductC_32f(karaokeSL, c1, samC, 256);
          ippsAddProductC_32f(samSR, c2, samC, 256);
        } else {
          ippsMulC_32f_I(cmixgain * m3dB * 2, samC, 256);
          ippsAddProductC_32f(karaokeSL, m3dB * smixgain, samC, 256);
          ippsAddProductC_32f(samSR, m3dB * smixgain, samC, 256);
        }
        ippsAdd_32f_I(samR, samL, 256);
        ippsAddProductC_32f(samL, m3dB, samC, 256);
      } else if (outfront == 2) {
        if (karaokeCapable >= 0) {
          Ipp32f c0 = karaokeTable[karaokeCapable][3];
          Ipp32f c1 = karaokeTable[karaokeCapable][4];
          Ipp32f c2 = karaokeTable[karaokeCapable][5];
          Ipp32f c3 = karaokeTable[karaokeCapable][6];
          Ipp32f c4 = karaokeTable[karaokeCapable][7];
          Ipp32f c5 = karaokeTable[karaokeCapable][8];

          ippsAddProductC_32f(samC, c0, samL, 256);
          ippsAddProductC_32f(samC, c1, samR, 256);

          ippsAddProductC_32f(karaokeSL, c2, samL, 256);
          ippsAddProductC_32f(karaokeSL, c3, samR, 256);

          ippsAddProductC_32f(samSR, c4, samL, 256);
          ippsAddProductC_32f(samSR, c5, samR, 256);

        } else {
          ippsAddProductC_32f(samC, cmixgain, samL, 256);
          ippsAddProductC_32f(samC, cmixgain, samR, 256);
          if (inrear == 1) {
            ippsAddProductC_32f(samS, smixgain * m3dB, samL, 256);
            ippsAddProductC_32f(samS, smixgain * m3dB, samR, 256);
          } else if (inrear == 2) {
            ippsAddProductC_32f(samSL, smixgain, samL, 256);
            ippsAddProductC_32f(samSR, smixgain, samR, 256);
          }
        }
      } else if (outfront == 3) {
        if (karaokeCapable >= 0) {
          Ipp32f c0 = karaokeTable[karaokeCapable][9];
          Ipp32f c1 = karaokeTable[karaokeCapable][10];
          Ipp32f c2 = karaokeTable[karaokeCapable][11];
          Ipp32f c3 = karaokeTable[karaokeCapable][12];
          Ipp32f c4 = karaokeTable[karaokeCapable][13];
          Ipp32f c5 = karaokeTable[karaokeCapable][14];
          Ipp32f c6 = karaokeTable[karaokeCapable][15];
          Ipp32f c7 = karaokeTable[karaokeCapable][16];
          Ipp32f c8 = karaokeTable[karaokeCapable][17];

          if (c0 >= 0) {
            ippsAddProductC_32f(samC, c3, samL, 256);
          }
          else         ippsAddProductC_32f(samC, c3, samR, 256);

          if (c1 >= 0) ippsAddProductC_32f(karaokeSL, c4, samL, 256);
          else         ippsAddProductC_32f(karaokeSL, c4, samR, 256);

          if (c2 >= 0) ippsAddProductC_32f(samSR, c5, samL, 256);
          else         ippsAddProductC_32f(samSR, c5, samR, 256);

          ippsMulC_32f_I(c6, samC, 256);
          ippsAddProductC_32f(karaokeSL, c7, samC, 256);
          ippsAddProductC_32f(samSR, c8, samC, 256);
        } else {
          if (inrear == 1) {
            ippsAddProductC_32f(samS, smixgain, samC, 256);
          } else if (inrear == 2) {
            ippsAddProductC_32f(samSL, smixgain, samL, 256);
            ippsAddProductC_32f(samSR, smixgain, samR, 256);
          }
        }
      }
      ippsZero_32f(karaokeSL, 256);
      ippsZero_32f(samSR, 256);
    } else {
      if (state->out_acmod == ACMOD_0) { /* Dolby Surround compatible */
        if (infront != 2) {
          ippsMulC_32f_I(m3dB, samC, 256);
          ippsAdd_32f_I(samC, samL, 256);
          ippsAdd_32f_I(samC, samR, 256);
          if (infront != 1) downmix = 1;
        }
        if (inrear == 1) {
          ippsMulC_32f_I(m3dB, samS, 256);
          ippsSub_32f_I(samS, samL, 256);
          ippsAdd_32f_I(samS, samR, 256);
          downmix = 1;
        } else if (inrear == 2) {
          ippsAdd_32f_I(samSR, samSL, 256);
          ippsMulC_32f_I(m3dB, samSL, 256);
          ippsSub_32f_I(samSL, samL, 256);
          ippsAdd_32f_I(samSL, samR, 256);
          downmix = 1;
        }
      } else if (state->out_acmod == ACMOD_10) { /* center only */
        if (infront == 3) {
          ippsMulC_32f_I(2 * cmixgain * m3dB, samC, 256);
          downmix = 1;
        }
        if (infront != 1) {
          ippsAdd_32f_I(samL, samR, 256);
          ippsAddProductC_32f(samR, m3dB, samC, 256);
          downmix = 1;
        }
        if (inrear == 1) {
          ippsAddProductC_32f(samS, smixgain * m3dB, samC, 256);
          if (smixgain > 0) downmix = 1;
        } else if (inrear == 2) {
          ippsAdd_32f(samSL, samSR, samSL, 256);
          ippsAddProductC_32f(samSL, smixgain * m3dB, samC, 256);
          if (smixgain > 0) downmix = 1;
        }
      } else {    /* more than center output requested */
        if (outfront == 2) {
          if (infront == 1) {
            ippsMulC_32f(samC, m3dB, samL, 256);
            ippsCopy_32f(samL, samR, 256);
          } else if (infront == 3) {
            ippsMulC_32f_I(cmixgain, samC, 256);
            ippsAdd_32f_I(samC, samL, 256);
            ippsAdd_32f_I(samC, samR, 256);
            downmix = 1;
          }
        }
        if (inrear == 1) {        /* single surround channel coded */
          if (outrear == 0) {     /* no surround loudspeakers */
            ippsMulC_32f_I(smixgain * m3dB, samS, 256);
            ippsAdd_32f_I(samS, samL, 256);
            ippsAdd_32f_I(samS, samR, 256);
            if (smixgain > 0) downmix = 1;
          } else if (outrear == 2) {      /* two surround loudspeaker channels */
            ippsMulC_32f(samS, m3dB, samSL, 256);
            ippsCopy_32f(samSL, samSR, 256);
          }
        } else if (inrear == 2) { /* two surround channels encoded */
          if (outrear == 0) {
            ippsAddProductC_32f(samSL, smixgain, samL, 256);
            ippsAddProductC_32f(samSR, smixgain, samR, 256);
            if (smixgain > 0) downmix = 1;
          } else if (outrear == 1) {
            ippsAdd_32f_I(samSR, samSL, 256);
            ippsMulC_32f(samSL, m3dB, samS, 256);
            downmix = 1;
          }
        }
      }
    }
  } else {
    if (state->bsi.acmod == ACMOD_10) {
      ippsZero_32f(samL, 128);
      ippsZero_32f(samL+256, 128);
      ippsZero_32f(samR, 128);
      ippsZero_32f(samR+256, 128);
    }

    if ((infront == 2) && (outfront != 2)) {
      ippsZero_32f(samC, 128);
      ippsZero_32f(samC+256, 128);
    }

    if (inrear == 0) {
      if (outrear == 2) {
        ippsZero_32f(samSL, 128);
        ippsZero_32f(samSL+256, 128);
        ippsZero_32f(samSR, 128);
        ippsZero_32f(samSR+256, 128);
      } else if (outrear == 1) {
        ippsZero_32f(samS, 128);
        ippsZero_32f(samS+256, 128);
      }
    } else if (inrear == 1) {
      if (outrear == 2) {
        ippsZero_32f(samSL, 128);
        ippsZero_32f(samSL+256, 128);
        ippsZero_32f(samSR, 128);
        ippsZero_32f(samSR+256, 128);
      }
    }

    if (state->bsi.acmod == ACMOD_0) {   /* 1+1 mode, dual independent mono channels present */
      gain = drc(state->bsi.compre, state->bsi.compr, dynrng, 0,
                state->bsi.dialnorm, state);
      ippsMulC_32f_I(gain, samL, 128);
      ippsMulC_32f_I(gain, samL+256, 128);
      gain = drc(state->bsi.compr2e, state->bsi.compr2, dynrng2, 0,
                state->bsi.dialnorm2, state);
      ippsMulC_32f_I(gain, samR, 128);
      ippsMulC_32f_I(gain, samR+256, 128);

      if (outfront == 1) {        /* 1 front loudspeaker (center) */
        if (state->dualmonomode == DUAL_LEFTMONO) {
          ippsCopy_32f(samL, samC, 128);
          ippsCopy_32f(samL+256, samC+256, 128);
        } else if (state->dualmonomode == DUAL_RGHTMONO) {
          ippsCopy_32f(samR, samC, 128);
          ippsCopy_32f(samR+256, samC+256, 128);
        } else {
          ippsAdd_32f(samL, samR, samC, 128);
          ippsAdd_32f(samL+256, samR+256, samC+256, 128);
          ippsMulC_32f_I(m6dB, samC, 128);        /* 0.501187233 = -6dB */
          ippsMulC_32f_I(m6dB, samC+256, 128);    /* 0.501187233 = -6dB */
        }
      } else if (outfront == 2) {
        if (state->dualmonomode == DUAL_LEFTMONO) {
          ippsMulC_32f_I(m3dB, samL, 128);        /* 0.707945 = -3dB */
          ippsMulC_32f_I(m3dB, samL+256, 128);    /* 0.707945 = -3dB */
          ippsCopy_32f(samL, samR, 128);
          ippsCopy_32f(samL+256, samR+256, 128);
        } else if (state->dualmonomode == DUAL_RGHTMONO) {
          ippsMulC_32f_I(m3dB, samR, 128);        /* 0.707945 = -3dB */
          ippsMulC_32f_I(m3dB, samR+256, 128);    /* 0.707945 = -3dB */
          ippsCopy_32f(samR, samL, 128);
          ippsCopy_32f(samR+256, samL+256, 128);
        } else if (state->dualmonomode == DUAL_MIXMONO) {
          ippsAdd_32f_I(samR, samL, 128);
          ippsAdd_32f_I(samR+256, samL+256, 128);
          ippsMulC_32f_I(m6dB, samL, 128);        /* 0.501187233 = -6dB */
          ippsMulC_32f_I(m6dB, samL+256, 128);    /* 0.501187233 = -6dB */
          ippsCopy_32f(samL, samR, 128);
          ippsCopy_32f(samL+256, samR+256, 128);
        }
      } else {
        if (state->dualmonomode == DUAL_LEFTMONO) {
          ippsCopy_32f(samL, samC, 128);
          ippsCopy_32f(samL+256, samC+256, 128);
          ippsZero_32f(samL, 128);
          ippsZero_32f(samL+256, 128);
          ippsZero_32f(samR, 128);
          ippsZero_32f(samR+256, 128);
        } else if (state->dualmonomode == DUAL_RGHTMONO) {
          ippsCopy_32f(samR, samC, 128);
          ippsCopy_32f(samR+256, samC+256, 128);
          ippsZero_32f(samL, 128);
          ippsZero_32f(samL+256, 128);
          ippsZero_32f(samR, 128);
          ippsZero_32f(samR+256, 128);
        } else if (state->dualmonomode == DUAL_MIXMONO) {
          ippsAdd_32f(samL, samR, samC, 128);
          ippsAdd_32f(samL+256, samR+256, samC+256, 128);
          ippsMulC_32f_I(m6dB, samC, 128);        /* 0.501187233 = -6dB */
          ippsMulC_32f_I(m6dB, samC+256, 128);    /* 0.501187233 = -6dB */
          ippsZero_32f(samL, 128);
          ippsZero_32f(samL+256, 128);
          ippsZero_32f(samR, 128);
          ippsZero_32f(samR+256, 128);
        }
      }
    } else if (state->bsi.karaokeMode) {
      Ipp32s karaokeCapable = state->karaokeCapable;
      downmix = 1;

      if (outfront == 1) {
        if (karaokeCapable >= 0) {
          Ipp32f c0 = karaokeTable[karaokeCapable][0];
          Ipp32f c1 = karaokeTable[karaokeCapable][1];
          Ipp32f c2 = karaokeTable[karaokeCapable][2];

          ippsMulC_32f_I(c0, samC, 128);
          ippsMulC_32f_I(c0, samC+256, 128);
          ippsAddProductC_32f(karaokeSL, c1, samC, 128);
          ippsAddProductC_32f(karaokeSL+256, c1, samC+256, 128);
          ippsAddProductC_32f(samSR, c2, samC, 128);
          ippsAddProductC_32f(samSR+256, c2, samC+256, 128);
        } else {
          ippsMulC_32f_I(cmixgain * m3dB * 2, samC, 128);
          ippsMulC_32f_I(cmixgain * m3dB * 2, samC+256, 128);
          ippsAddProductC_32f(karaokeSL, m3dB * smixgain, samC, 128);
          ippsAddProductC_32f(karaokeSL+256, m3dB * smixgain, samC+256, 128);
          ippsAddProductC_32f(samSR, m3dB * smixgain, samC, 128);
          ippsAddProductC_32f(samSR+256, m3dB * smixgain, samC+256, 128);
        }
        ippsAdd_32f_I(samR, samL, 128);
        ippsAdd_32f_I(samR+256, samL+256, 128);
        ippsAddProductC_32f(samL, m3dB, samC, 128);
        ippsAddProductC_32f(samL+256, m3dB, samC+256, 128);
      } else if (outfront == 2) {
        if (karaokeCapable >= 0) {
          Ipp32f c0 = karaokeTable[karaokeCapable][3];
          Ipp32f c1 = karaokeTable[karaokeCapable][4];
          Ipp32f c2 = karaokeTable[karaokeCapable][5];
          Ipp32f c3 = karaokeTable[karaokeCapable][6];
          Ipp32f c4 = karaokeTable[karaokeCapable][7];
          Ipp32f c5 = karaokeTable[karaokeCapable][8];

          ippsAddProductC_32f(samC, c0, samL, 128);
          ippsAddProductC_32f(samC+256, c0, samL+256, 128);
          ippsAddProductC_32f(samC, c1, samR, 128);
          ippsAddProductC_32f(samC+256, c1, samR+256, 128);

          ippsAddProductC_32f(karaokeSL, c2, samL, 128);
          ippsAddProductC_32f(karaokeSL+256, c2, samL+256, 128);
          ippsAddProductC_32f(karaokeSL, c3, samR, 128);
          ippsAddProductC_32f(karaokeSL+256, c3, samR+256, 128);

          ippsAddProductC_32f(samSR, c4, samL, 128);
          ippsAddProductC_32f(samSR+256, c4, samL+256, 128);
          ippsAddProductC_32f(samSR, c5, samR, 128);
          ippsAddProductC_32f(samSR+256, c5, samR+256, 128);

        } else {
          ippsAddProductC_32f(samC, cmixgain, samL, 128);
          ippsAddProductC_32f(samC+256, cmixgain, samL+256, 128);
          ippsAddProductC_32f(samC, cmixgain, samR, 128);
          ippsAddProductC_32f(samC+256, cmixgain, samR+256, 128);
          if (inrear == 1) {
            ippsAddProductC_32f(samS, smixgain * m3dB, samL, 128);
            ippsAddProductC_32f(samS+256, smixgain * m3dB, samL+256, 128);
            ippsAddProductC_32f(samS, smixgain * m3dB, samR, 128);
            ippsAddProductC_32f(samS+256, smixgain * m3dB, samR+256, 128);
          } else if (inrear == 2) {
            ippsAddProductC_32f(samSL, smixgain, samL, 128);
            ippsAddProductC_32f(samSL+256, smixgain, samL+256, 128);
            ippsAddProductC_32f(samSR, smixgain, samR, 128);
            ippsAddProductC_32f(samSR+256, smixgain, samR+256, 128);
          }
        }
      } else if (outfront == 3) {
        if (karaokeCapable >= 0) {
          Ipp32f c0 = karaokeTable[karaokeCapable][9];
          Ipp32f c1 = karaokeTable[karaokeCapable][10];
          Ipp32f c2 = karaokeTable[karaokeCapable][11];
          Ipp32f c3 = karaokeTable[karaokeCapable][12];
          Ipp32f c4 = karaokeTable[karaokeCapable][13];
          Ipp32f c5 = karaokeTable[karaokeCapable][14];
          Ipp32f c6 = karaokeTable[karaokeCapable][15];
          Ipp32f c7 = karaokeTable[karaokeCapable][16];
          Ipp32f c8 = karaokeTable[karaokeCapable][17];

          if (c0 >= 0) {
            ippsAddProductC_32f(samC, c3, samL, 128);
            ippsAddProductC_32f(samC+256, c3, samL+256, 128);
          } else {
            ippsAddProductC_32f(samC, c3, samR, 128);
            ippsAddProductC_32f(samC+256, c3, samR+256, 128);
          }

          if (c1 >= 0) {
            ippsAddProductC_32f(karaokeSL, c4, samL, 128);
            ippsAddProductC_32f(karaokeSL+256, c4, samL+256, 128);
          } else {
            ippsAddProductC_32f(karaokeSL, c4, samR, 128);
            ippsAddProductC_32f(karaokeSL+256, c4, samR+256, 128);
          }

          if (c2 >= 0) {
            ippsAddProductC_32f(samSR, c5, samL, 128);
            ippsAddProductC_32f(samSR+256, c5, samL+256, 128);
          } else {
            ippsAddProductC_32f(samSR, c5, samR, 128);
            ippsAddProductC_32f(samSR+256, c5, samR+256, 128);
          }

          ippsMulC_32f_I(c6, samC, 128);
          ippsMulC_32f_I(c6, samC+256, 128);
          ippsAddProductC_32f(karaokeSL, c7, samC, 128);
          ippsAddProductC_32f(karaokeSL+256, c7, samC+256, 128);
          ippsAddProductC_32f(samSR, c8, samC, 128);
          ippsAddProductC_32f(samSR+256, c8, samC+256, 128);
        } else {
          if (inrear == 1) {
            ippsAddProductC_32f(samS, smixgain, samC, 128);
            ippsAddProductC_32f(samS+256, smixgain, samC+256, 128);
          } else if (inrear == 2) {
            ippsAddProductC_32f(samSL, smixgain, samL, 128);
            ippsAddProductC_32f(samSL+256, smixgain, samL+256, 128);
            ippsAddProductC_32f(samSR, smixgain, samR, 128);
            ippsAddProductC_32f(samSR+256, smixgain, samR+256, 128);
          }
        }
      }
      ippsZero_32f(karaokeSL, 128);
      ippsZero_32f(karaokeSL+256, 128);
      ippsZero_32f(samSR, 128);
      ippsZero_32f(samSR+256, 128);
    } else {
      if (state->out_acmod == ACMOD_0) { /* Dolby Surround compatible */
        if (infront != 2) {
          ippsMulC_32f_I(m3dB, samC, 128);
          ippsMulC_32f_I(m3dB, samC+256, 128);
          ippsAdd_32f_I(samC, samL, 128);
          ippsAdd_32f_I(samC+256, samL+256, 128);
          ippsAdd_32f_I(samC, samR, 128);
          ippsAdd_32f_I(samC+256, samR+256, 128);
          if (infront != 1) downmix = 1;
        }
        if (inrear == 1) {
          ippsMulC_32f_I(m3dB, samS, 128);
          ippsMulC_32f_I(m3dB, samS+256, 128);
          ippsSub_32f_I(samS, samL, 128);
          ippsSub_32f_I(samS+256, samL+256, 128);
          ippsAdd_32f_I(samS, samR, 128);
          ippsAdd_32f_I(samS+256, samR+256, 128);
          downmix = 1;
        } else if (inrear == 2) {
          ippsAdd_32f_I(samSR, samSL, 128);
          ippsAdd_32f_I(samSR+256, samSL+256, 128);
          ippsMulC_32f_I(m3dB, samSL, 128);
          ippsMulC_32f_I(m3dB, samSL+256, 128);
          ippsSub_32f_I(samSL, samL, 128);
          ippsSub_32f_I(samSL+256, samL+256, 128);
          ippsAdd_32f_I(samSL, samR, 128);
          ippsAdd_32f_I(samSL+256, samR+256, 128);
          downmix = 1;
        }
      } else if (state->out_acmod == ACMOD_10) { /* center only */
        if (infront == 3) {
          ippsMulC_32f_I(2 * cmixgain * m3dB, samC, 128);
          ippsMulC_32f_I(2 * cmixgain * m3dB, samC+256, 128);
          downmix = 1;
        }
        if (infront != 1) {
          ippsAdd_32f_I(samL, samR, 128);
          ippsAdd_32f_I(samL+256, samR+256, 128);
          ippsAddProductC_32f(samR, m3dB, samC, 128);
          ippsAddProductC_32f(samR+256, m3dB, samC+256, 128);
          downmix = 1;
        }
        if (inrear == 1) {
          ippsAddProductC_32f(samS, smixgain * m3dB, samC, 128);
          ippsAddProductC_32f(samS+256, smixgain * m3dB, samC+256, 128);
          if (smixgain > 0) downmix = 1;
        } else if (inrear == 2) {
          ippsAdd_32f(samSL, samSR, samSL, 128);
          ippsAdd_32f(samSL+256, samSR+256, samSL+256, 128);
          ippsAddProductC_32f(samSL, smixgain * m3dB, samC, 128);
          ippsAddProductC_32f(samSL+256, smixgain * m3dB, samC+256, 128);
          if (smixgain > 0) downmix = 1;
        }
      } else {    /* more than center output requested */
        if (outfront == 2) {
          if (infront == 1) {
            ippsMulC_32f(samC, m3dB, samL, 128);
            ippsMulC_32f(samC+256, m3dB, samL+256, 128);
            ippsCopy_32f(samL, samR, 128);
            ippsCopy_32f(samL+256, samR+256, 128);
          } else if (infront == 3) {
            ippsMulC_32f_I(cmixgain, samC, 128);
            ippsMulC_32f_I(cmixgain, samC+256, 128);
            ippsAdd_32f_I(samC, samL, 128);
            ippsAdd_32f_I(samC+256, samL+256, 128);
            ippsAdd_32f_I(samC, samR, 128);
            ippsAdd_32f_I(samC+256, samR+256, 128);
            downmix = 1;
          }
        }
        if (inrear == 1) {        /* single surround channel coded */
          if (outrear == 0) {     /* no surround loudspeakers */
            ippsMulC_32f_I(smixgain * m3dB, samS, 128);
            ippsMulC_32f_I(smixgain * m3dB, samS+256, 128);
            ippsAdd_32f_I(samS, samL, 128);
            ippsAdd_32f_I(samS+256, samL+256, 128);
            ippsAdd_32f_I(samS, samR, 128);
            ippsAdd_32f_I(samS+256, samR+256, 128);
            if (smixgain > 0) downmix = 1;
          } else if (outrear == 2) {      /* two surround loudspeaker channels */
            ippsMulC_32f(samS, m3dB, samSL, 128);
            ippsMulC_32f(samS+256, m3dB, samSL+256, 128);
            ippsCopy_32f(samSL, samSR, 128);
            ippsCopy_32f(samSL+256, samSR+256, 128);
          }
        } else if (inrear == 2) { /* two surround channels encoded */
          if (outrear == 0) {
            ippsAddProductC_32f(samSL, smixgain, samL, 128);
            ippsAddProductC_32f(samSL+256, smixgain, samL+256, 128);
            ippsAddProductC_32f(samSR, smixgain, samR, 128);
            ippsAddProductC_32f(samSR+256, smixgain, samR+256, 128);
            if (smixgain > 0) downmix = 1;
          } else if (outrear == 1) {
            ippsAdd_32f_I(samSR, samSL, 128);
            ippsAdd_32f_I(samSR+256, samSL+256, 128);
            ippsMulC_32f(samSL, m3dB, samS, 128);
            ippsMulC_32f(samSL+256, m3dB, samS+256, 128);
            downmix = 1;
          }
        }
      }
    }
  }

 /* write out_channel */
  switch (state->out_acmod) {
  case ACMOD_0:
    state->temp[0] = samL;
    state->temp[1] = samR;

    if (state->outlfeon)
      state->temp[2] = samLFE;
    break;
  case ACMOD_10:
    state->temp[0] = samC;

    if (state->outlfeon)
      state->temp[1] = samLFE;
    break;

  case ACMOD_20:
    state->temp[0] = samL;
    state->temp[1] = samR;

    if (state->outlfeon)
      state->temp[2] = samLFE;
    break;

  case ACMOD_30:
    state->temp[0] = samL;
    state->temp[1] = samC;
    state->temp[2] = samR;

    if (state->outlfeon)
      state->temp[3] = samLFE;
    break;
  case ACMOD_21:
    state->temp[0] = samL;
    state->temp[1] = samR;
    state->temp[2] = samS;

    if (state->outlfeon)
      state->temp[3] = samLFE;
    break;

  case ACMOD_31:
    state->temp[0] = samL;
    state->temp[1] = samC;
    state->temp[2] = samR;
    state->temp[3] = samS;

    if (state->outlfeon)
      state->temp[4] = samLFE;
    break;

  case ACMOD_22:
    state->temp[0] = samL;
    state->temp[1] = samR;
    state->temp[2] = samSL;
    state->temp[3] = samSR;
    if (state->outlfeon)
      state->temp[4] = samLFE;
    break;

  case ACMOD_32:
    state->temp[0] = samL;
    state->temp[1] = samC;
    state->temp[2] = samR;
    state->temp[3] = samSL;
    state->temp[4] = samSR;

    if (state->outlfeon)
      state->temp[5] = samLFE;
    break;

  default:
    break;
  }

  if (state->bsi.acmod != ACMOD_0) {
    Ipp32s i;
    gain = drc(state->bsi.compre, state->bsi.compr, dynrng,
               downmix, state->bsi.dialnorm, state);
    if (before_imdct) {
      for (i = 0; i < (state->nChannelOut - state->outlfeon); i++) {
        ippsMulC_32f_I(gain, state->temp[i], 256);
      }
    } else {
      for (i = 0; i < (state->nChannelOut - state->outlfeon); i++) {
        ippsMulC_32f_I(gain, state->temp[i], 128);
        ippsMulC_32f_I(gain, state->temp[i]+256, 128);
      }
    }
  }

  if ((state->bsi.lfeon) && (state->outlfeon)) {
    if (before_imdct) {
      ippsMulC_32f_I(gain, state->temp[state->nChannelOut - 1], 256);
    } else {
      ippsMulC_32f_I(gain, state->temp[state->nChannelOut - 1], 128);
      ippsMulC_32f_I(gain, state->temp[state->nChannelOut - 1]+256, 128);
    }
  }
}

/********************************************************************/

#endif //UMC_ENABLE_AC3_AUDIO_DECODER


