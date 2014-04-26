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
#include "ac3_dec_tables.h"
#include "ipps.h"

/********************************************************************/

#define AC3_MAX(a, b, c) \
  c = (a); if (c < (b)) c = (b);

#define COUNT_MASK                                 \
  if (psd < dbknee) mask += ((dbknee - psd) >> 2); \
  if (hth[bin] > mask) mask = hth[bin];            \
  mask += deltba[bin];                             \
  mask -= snroffset;                               \
  if (mask < 0) mask = 0;                          \
  mask &= 0x1fe0;                                  \
  mask += floor;

/********************************************************************/

static void baComputeBap(Ipp32s sampleStart,
                         Ipp32s sampleEnd,
                         Ipp32s fgain,
                         Ipp32s snroffset,
                         Ipp32s fastleak,
                         Ipp32s slowleak,
                         Ipp32s fscod,
                         Ipp32s *exps,
                         Ipp32s *deltba,
                         Ipp32s *bap,
                         _AudBlk *audblk)
{
  Ipp32s bin, lastbin;
  Ipp32s bndstrt, bndstrtdelta;
  Ipp32s bndend;
  Ipp32s lowcomp = 0;
  Ipp32s begin = 0;

  Ipp32s sdecay;
  Ipp32s fdecay;
  Ipp32s sgain;
  Ipp32s dbknee;
  Ipp32s floor;
  Ipp16s *hth = HTH[fscod];
  Ipp8u  *TAB = BAPTAB + 2300;
  Ipp32s mask, psd;
  Ipp32s *pbap;
  Ipp32s *pexps;
  Ipp32s i, bndsize;


  sdecay = SLOWDEC[audblk->sdcycod];
  fdecay = FASTDEC[audblk->fdcycod];
  sgain = SLOWGAIN[audblk->sgaincod];
  dbknee = DBPBTAB[audblk->dbpbcod];
  floor = FLOORTAB[audblk->floorcod];

  snroffset += floor;

  bndstrtdelta = bndstrt = MASKTAB[sampleStart];
  bndend = (MASKTAB[sampleEnd - 1] + 1);

  deltba -= bndstrt;

  if (bndstrt == 0) {   /* For fbw and lfe channels */
    /* Note 1) For fbw bndend is more then 37 */
    /*      2) For the first 27 bands the width of the band is equel to 1 */
    if (exps[0] == exps[1] + 2) {
      lowcomp = 384;
    } else {
      lowcomp = 0;
    }

    bin = 0;
    psd = (3072 - (exps[bin] << 7));
    mask = (psd - fgain - lowcomp);
    COUNT_MASK
    bap[bin] = TAB[(psd - mask) >> 5];

    if (exps[1] == exps[2] + 2) {
      lowcomp = 384;
    } else if (lowcomp != 0) {
      if (exps[1] < exps[2]) {
        lowcomp -= 64;
      }
    }

    bin = 1;
    psd = (3072 - (exps[bin] << 7));
    mask = (psd - fgain - lowcomp);
    COUNT_MASK
    bap[bin] = TAB[(psd - mask) >> 5];

    begin = 7;

    for (bin = 2; bin < 7; bin++) {
      if (exps[bin] == exps[bin + 1] + 2) {
        lowcomp = 384;
        psd = (3072 - (exps[bin] << 7));
        mask = (psd - fgain - lowcomp);
        COUNT_MASK
        bap[bin] = TAB[(psd - mask) >> 5];

        begin = bin + 1;
        break;
      } else if (exps[bin] < exps[bin + 1]) {
        lowcomp -= 64;
        if (lowcomp < 0) lowcomp = 0;
        psd = (3072 - (exps[bin] << 7));
        mask = (psd - fgain - lowcomp);
        COUNT_MASK
        bap[bin] = TAB[(psd - mask) >> 5];

      } else {
        psd = (3072 - (exps[bin] << 7));
        mask = (psd - fgain - lowcomp);
        COUNT_MASK
        bap[bin] = TAB[(psd - mask) >> 5];

        begin = bin + 1;
        break;
      }
    }

    fastleak = psd - fgain;
    slowleak = psd - sgain;

    for (bin = begin; bin < 7; bin++) {
      psd = (3072 - (exps[bin] << 7));
      AC3_MAX(fastleak - fdecay, psd - fgain, fastleak)
      AC3_MAX(slowleak - sdecay, psd - sgain, slowleak)

      if (exps[bin] == exps[bin + 1] + 2) {
        lowcomp = 384;
      } else if (lowcomp > 0) {
        if (exps[bin] < exps[bin + 1]) {
          lowcomp -= 64;
        }
      }

      AC3_MAX(fastleak - lowcomp, slowleak, mask)
      COUNT_MASK
      bap[bin] = TAB[(psd - mask) >> 5];
    }

    if (bndend == 7) return;

    for (bin = 7; bin < 20; bin++) {
      psd = (3072 - (exps[bin] << 7));
      AC3_MAX(fastleak - fdecay, psd - fgain, fastleak)
      AC3_MAX(slowleak - sdecay, psd - sgain, slowleak)

      if (exps[bin] == exps[bin + 1] + 2) {
        lowcomp = 320;
      } else if (lowcomp > 0) {
        if (exps[bin] < exps[bin + 1]) {
          lowcomp -= 64;
        }
      }

      AC3_MAX(fastleak - lowcomp, slowleak, mask)
      COUNT_MASK
      bap[bin] = TAB[(psd - mask) >> 5];
    }

    for (bin = 20; bin < 22; bin++) {
      psd = (3072 - (exps[bin] << 7));
      AC3_MAX(fastleak - fdecay, psd - fgain, fastleak)
      AC3_MAX(slowleak - sdecay, psd - sgain, slowleak)

      lowcomp -= 128;
      if (lowcomp < 0) lowcomp = 0;

      AC3_MAX(fastleak - lowcomp, slowleak, mask)
      COUNT_MASK
      bap[bin] = TAB[(psd - mask) >> 5];

    }
    bndstrt = 22;
    sampleStart = BNDTAB[bndstrt];
  }

  pbap   = bap + sampleStart;
  pexps = exps + sampleStart;

  for (bin = bndstrt; bin < bndend; bin++) {
    Ipp32s tmp_psd[24];

    tmp_psd[0] = psd = (3072 - ((*pexps) << 7));
    pexps++;

    lastbin = BNDTAB[bin] + BNDSZ[bin];
    if (lastbin > sampleEnd) lastbin = sampleEnd;
    bndsize = lastbin - sampleStart;

    sampleStart += bndsize;

    for (i = 1; i < bndsize; i++) {
      Ipp32s diff, psd1;
      tmp_psd[i] = psd1 = (3072 - ((*pexps) << 7));
      diff = psd - psd1;
      if (diff < -510) {
        psd = psd1;
      } else if (diff < 0) {
        psd = psd1 + LATAB[((-diff) >> 1)];
      } else if (diff < 511) {
        psd = psd + LATAB[((diff) >> 1)];
      }
      pexps++;
    }

    AC3_MAX(fastleak - fdecay, psd - fgain, fastleak)
    AC3_MAX(slowleak - sdecay, psd - sgain, slowleak)
    AC3_MAX(fastleak, slowleak, mask)
    COUNT_MASK
    for (i = 0; i < bndsize; i++) {
      *pbap = TAB[(tmp_psd[i] - mask) >> 5];
      pbap++;
    }
  }
}

/********************************************************************/

Ipp32s BitAllocation(AC3Dec* state)
{
  Ipp32s i, tmp;
  Ipp32s snroffset;
  _AudBlk *audblk = &(state->audblk);

/*
 * if all the SNR offset constants are zero then the whole block is zero
 */

  tmp = audblk->csnroffst;
  for (i = 0; i < state->bsi.nfchans; i++) {
    tmp = tmp | audblk->fsnroffst[i];
  }

  if (audblk->cplinu)
    tmp = tmp | audblk->cplfsnroffst;

  if (state->bsi.lfeon)
    tmp = tmp | audblk->lfefsnroffst;

  if (!tmp) {
    ippsZero_8u((Ipp8u *)state->fbw_bap, sizeof(Ipp32s) * 256 * 5);
    ippsZero_8u((Ipp8u *)state->cpl_bap, sizeof(Ipp32s) * 256);
    ippsZero_8u((Ipp8u *)state->lfe_bap, sizeof(Ipp32s) * 7);
    return 0;
  }

  snroffset = (audblk->csnroffst - 15) << 6;

  for (i = 0; i < state->bsi.nfchans ; i++) {
    if (audblk->bitAllocation[i]) {
      baComputeBap(0, audblk->endmant[i], FASTGAIN[audblk->fgaincod[i]],
                   snroffset + (audblk->fsnroffst[i] << 2), 0, 0,
                   state->syncinfo.fscod, state->fbw_exp[i], state->deltba[i],
                   state->fbw_bap[i], audblk);
    }
  }

  if (audblk->cplinu && audblk->CplBitAllocation) {
    Ipp32s fastleak = ((audblk->cplfleak << 8) + 768);
    Ipp32s slowleak = ((audblk->cplsleak << 8) + 768);

    baComputeBap(audblk->cplstrtmant, audblk->cplendmant,
                 FASTGAIN[audblk->cplfgaincod],
                 snroffset + (audblk->cplfsnroffst << 2), fastleak, slowleak,
                 state->syncinfo.fscod, state->cpl_exp, state->cpldeltba,
                 state->cpl_bap, audblk);
  }

  if (state->bsi.lfeon && audblk->LfeBitAllocation) {
    state->lfe_exp[7] = state->lfe_exp[6];
    baComputeBap(0, 7, FASTGAIN[audblk->lfefgaincod],
                 snroffset + (audblk->lfefsnroffst << 2), 0, 0,
                 state->syncinfo.fscod, state->lfe_exp, state->lfedeltba,
                 state->lfe_bap, audblk);
  }

  return 1;
}

/********************************************************************/

#endif //UMC_ENABLE_AC3_AUDIO_DECODER







