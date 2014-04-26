/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//     Intel Integrated Performance Primitives AAC Encode Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel IPP
//  product installation for more information.
//
//  MPEG-4 and AAC are international standards promoted by ISO, IEC, ITU, ETSI
//  and other organizations. Implementations of these standards, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include <math.h>
#include "aac_enc_psychoacoustic_fp.h"
#include "aac_sfb_tables.h"
#include "align.h"

#include "ippcore.h"

#define MIN(a,b)      (((a) < (b)) ? (a) : (b))
#define MAX_TRH_SHORT 100000

/****************************************************************************/
                                    /*x*/      /*y*/
static const float hiPassCoeff[] = {0.43409411784051444f, 0.13181176431897129f};
//static const float hiPassCoeff[] = {-0.4142f, 0.7071f};
//static const float hiPassCoeff[] = {-0.6128f, 0.8064f};

/****************************************************************************/

static void BlockSwitching(sPsychoacousticBlock* pBlock,
                           sPsychoacousticBlockCom* pBlockCom,
                           Ipp32s ch)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, tmpDst, 1024);
  Ipp32f winEnergy[8];
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, tmpDst, 1024);
  static Ipp32f winEnergy[8];
#endif
  Ipp32f max;
  Ipp32s w, attack;

  pBlock->attackIndex = pBlock->lastAttackIndex;

  for (w = 0; w < 3; w++) {
    ippsIIR_32f(pBlockCom->input_data[ch][1] + 128 * w + 576,
                tmpDst, 128, pBlock->IIRfilterState);
    ippsDotProd_32f(tmpDst, tmpDst, 128, &winEnergy[w]);
  }

  ippsIIR_32f(pBlockCom->input_data[ch][1] + 128 * 3 + 576,
              tmpDst, 64, pBlock->IIRfilterState);

  ippsIIR_32f(pBlockCom->input_data[ch][2],
              tmpDst+64, 64, pBlock->IIRfilterState);

  ippsDotProd_32f(tmpDst, tmpDst, 128, &winEnergy[3]);

  for (w = 4; w < 8; w++) {
    ippsIIR_32f(pBlockCom->input_data[ch][2] + 128 * (w - 4) + 64,
                tmpDst, 128, pBlock->IIRfilterState);
    ippsDotProd_32f(tmpDst, tmpDst, 128, &winEnergy[w]);
  }

  attack = 0;
  max = 0;
  pBlock->lastAttackIndex = -1;

  for (w = 0; w < 8; w++) {
    if (winEnergy[w] > pBlock->avWinEnergy * pBlockCom->attackThreshold) {
      attack = 1;
      pBlock->lastAttackIndex = w;
    }
    pBlock->avWinEnergy = (0.7f * pBlock->avWinEnergy) + (0.3f * winEnergy[w]);
    if (max < winEnergy[w]) max = winEnergy[w];
  }

  if (max < 1000000) {
    attack = 0;
  }

  if (attack) {
    pBlock->next_desired_block_type = EIGHT_SHORT_SEQUENCE;
  } else{
    pBlock->next_desired_block_type = ONLY_LONG_SEQUENCE;
  }
}

/****************************************************************************/

Ipp32f sprdngf(Ipp32f b1,
               Ipp32f b2)
{
  Ipp32f tmpx,tmpy,tmpz;

  tmpx = (b2 >= b1) ? 3*(b2-b1) : 1.5f*(b2-b1);

  tmpz = 8 * MIN((tmpx-0.5f)*(tmpx-0.5f) - 2*(tmpx-0.5f), 0);

  tmpy = 15.811389f + 7.5f*(tmpx + 0.474f)-
         17.5f*(Ipp32f)sqrt(1 + (tmpx+0.474f)*(tmpx+0.474f));

  return (tmpy < -100 ? 0 : (Ipp32f)pow(10,(Ipp32f)(tmpz + tmpy)/10));
}

/****************************************************************************/

void psy_long_window(sPsychoacousticBlock* pBlock,
                     sPsychoacousticBlockCom* pBlockCom,
                     Ipp32s ch)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, rsqr_long, 1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, e_b, MAX_PPT_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, ecb, MAX_PPT_LONG);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, rsqr_long, 1024);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, e_b, MAX_PPT_LONG);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, ecb, MAX_PPT_LONG);
#endif
  Ipp32f        *r;
  Ipp32f        *nb,*nb_l;
  Ipp32f        *noiseThr;
  Ipp32s        b, num_ptt, sb;

  r = (Ipp32f*)pBlock->r[0];

  ippsZero_32f(r + pBlockCom->non_zero_line_long, 1024 - pBlockCom->non_zero_line_long);

  ippsSqr_32f((Ipp32f*)r, rsqr_long, 1024);

  num_ptt = pBlockCom->longWindow->num_ptt;

  for (b = 0; b < num_ptt; b++) {
    Ipp32s w_high = pBlockCom->longWindow->w_high[b];
    Ipp32s w_low = pBlockCom->longWindow->w_low[b];
    Ipp32f *tmp_rsqr = &rsqr_long[w_low];
    Ipp32s len = w_high - w_low + 1;

    ippsSum_32f(tmp_rsqr, len, &e_b[b], ippAlgHintNone);
  }

  for (b = 0; b < num_ptt; b++) {
    Ipp32f *tmp_ptr = (Ipp32f*)pBlockCom->sprdngf_long + b * num_ptt;

    ippsDotProd_32f(e_b, tmp_ptr, num_ptt, &ecb[b]);
  }

  nb   = pBlock->nb_long[pBlockCom->nb_curr_index];
  nb_l = pBlock->nb_long[pBlockCom->nb_prev_index];

  ippsMulC_32f_I(ATTENUATION, ecb, num_ptt);
  ippsMul_32f((Ipp32f*)pBlockCom->rnorm_long, ecb, (Ipp32f*)nb, num_ptt);

  if (pBlock->block_type != LONG_STOP_SEQUENCE) {
    ippsMulC_32f(nb_l, 0.01f, ecb, num_ptt);
    ippsMaxEvery_32f_I(nb, ecb, num_ptt);
    ippsMulC_32f_I(2, nb_l, num_ptt);
    ippsMinEvery_32f_I(ecb, nb_l, num_ptt);
  } else {
    ippsCopy_32f(nb, nb_l, num_ptt);
  }

  ippsMaxEvery_32f_I(pBlockCom->longWindow->qsthr, nb, num_ptt);
  ippsMaxEvery_32f_I(pBlockCom->longWindow->qsthr, nb_l, num_ptt >> 1);

  noiseThr = &pBlockCom->noiseThr[ch][0];

  for (sb = 0; sb < pBlockCom->num_sfb_long; sb++) {
    Ipp32s start = pBlockCom->aacenc_p2sb_l[sb].bu;
    Ipp32s end = pBlockCom->aacenc_p2sb_l[sb].bo;

    noiseThr[sb] = pBlockCom->aacenc_p2sb_l[sb].w1 * nb_l[start] +
                    pBlockCom->aacenc_p2sb_l[sb].w2 * nb_l[end];

    for (b = start + 1; b < end; b++) {
      noiseThr[sb] += nb_l[b];
    }
    if ((noiseThr[sb] > 1.0e9f) && (sb < 11)) {
      noiseThr[sb] = 1.0e9f;
    }
  }
}

/****************************************************************************/

void psy_short_window(sPsychoacousticBlock* pBlock,
                      sPsychoacousticBlockCom* pBlockCom,
                      Ipp32s ch,
                      Ipp32s lastBlockType)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, rsqr_short, 128);
  IPP_ALIGNED_ARRAY(32, Ipp32f, e_b, MAX_PPT_SHORT);
  IPP_ALIGNED_ARRAY(32, Ipp32f, ecb, MAX_PPT_SHORT);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, rsqr_short, 128);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, e_b, MAX_PPT_SHORT);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, ecb, MAX_PPT_SHORT);
#endif
  Ipp32f        *r;
  Ipp32s        win_counter;
  Ipp32f        *nb, *nb_s;
  Ipp32f        *noiseThr;
  Ipp32s        b, num_ptt, sb;

  for (win_counter = 0; win_counter < 8; win_counter++) {

    r = &pBlock->r[0][win_counter*128];

    ippsZero_32f(r + pBlockCom->non_zero_line_short, 128 - pBlockCom->non_zero_line_short);

    ippsSqr_32f(r, rsqr_short, 128);

    num_ptt = pBlockCom->shortWindow->num_ptt;

    for (b = 0; b < num_ptt; b++) {
      Ipp32s w_high = pBlockCom->shortWindow->w_high[b];
      Ipp32s w_low = pBlockCom->shortWindow->w_low[b];
      Ipp32f *tmp_rsqr = &rsqr_short[w_low];
      Ipp32s len = w_high - w_low + 1;

      ippsSum_32f(tmp_rsqr, len, &e_b[b], ippAlgHintNone);
    }

    for (b = 0; b < num_ptt; b++) {
      Ipp32f *tmp_ptr = (Ipp32f*)pBlockCom->sprdngf_short + b * num_ptt;

      ippsDotProd_32f(e_b, tmp_ptr, num_ptt, &ecb[b]);
    }

    nb = pBlock->nb_short[win_counter];
    if (win_counter == 0) {
      nb_s = pBlock->nb_short[7];
    } else {
      nb_s = pBlock->nb_short[win_counter - 1];
    }

    ippsMulC_32f_I(ATTENUATION, ecb, num_ptt);
    ippsMul_32f(pBlockCom->rnorm_short, ecb, nb, num_ptt);

    if ((lastBlockType != LONG_START_SEQUENCE) || (win_counter != 0)) {
      ippsMulC_32f(nb_s, 0.01f, ecb, num_ptt);
      ippsMaxEvery_32f_I(nb, ecb, num_ptt);
      ippsMulC_32f_I(2, nb_s, num_ptt);
      ippsMinEvery_32f_I(ecb, nb_s, num_ptt);
    } else {
      ippsCopy_32f(nb, nb_s, num_ptt);
    }

    ippsMaxEvery_32f_I(pBlockCom->shortWindow->qsthr, nb, num_ptt);
    ippsMaxEvery_32f_I(pBlockCom->shortWindow->qsthr, nb_s, num_ptt>>1);

    noiseThr = &pBlockCom->noiseThr[ch][MAX_SFB_SHORT * win_counter];

    for (sb = 0; sb < pBlockCom->num_sfb_short; sb++) {
      Ipp32s start = pBlockCom->aacenc_p2sb_s[sb].bu;
      Ipp32s end = pBlockCom->aacenc_p2sb_s[sb].bo;

      noiseThr[sb] = pBlockCom->aacenc_p2sb_s[sb].w1 * nb_s[start] +
                      pBlockCom->aacenc_p2sb_s[sb].w2 * nb_s[end];

      for (b = start + 1; b < end; b++) {
        noiseThr[sb] += nb_s[b];
      }

      /* pre-echo control */
      if (win_counter == pBlock->attackIndex) {
        noiseThr[sb] *= 0.01f;
        if (noiseThr[sb] > MAX_TRH_SHORT)
          noiseThr[sb] = MAX_TRH_SHORT;
      }

      if ((noiseThr[sb] > 1.0e9f) && (sb < 2)) {
        noiseThr[sb] = 1.0e9f;
      }
    }
  }
}

/****************************************************************************/

AACStatus InitPsychoacousticCom(sPsychoacousticBlockCom* pBlock,
                                Ipp8u* mem,
                                Ipp32s sf_index,
                                Ipp32s ns_mode,
                                Ipp32s *size_all)
{
  Ipp8u  *pBufInit;
  Ipp32s num_ptt, b, bb;
  Ipp32s sizeSpecLong, sizeInit, sizeWork;
  Ipp32s sizeSpecShort, sizeInitShort, sizeWorkShort;
  Ipp32f *bval;

  if (pBlock) {
    ippsZero_8u((Ipp8u*)pBlock, sizeof(sPsychoacousticBlockCom));

    pBlock->iblen_long  = 1024;
    pBlock->iblen_short = 128;

    pBlock->nb_curr_index = 1;
    pBlock->nb_prev_index = 0;

    pBlock->aacenc_p2sb_l = aacenc_p2sb_l[sf_index];
    pBlock->aacenc_p2sb_s = aacenc_p2sb_s[sf_index];
    pBlock->sfb_offset_long  = sfb_tables[sf_index].sfb_offset_long_window;
    pBlock->sfb_offset_short = sfb_tables[sf_index].sfb_offset_short_window;

    pBlock->longWindow    = &psy_partition_tables_long[sf_index];
    pBlock->shortWindow   = &psy_partition_tables_short[sf_index];

    num_ptt = pBlock->longWindow->num_ptt;
    bval = pBlock->longWindow->bval;

    for (b = 0; b < num_ptt; b++) {
      Ipp32f *tmp_ptr = pBlock->sprdngf_long + b * num_ptt;
      Ipp32f tmp = 0;

      for (bb = 0; bb < num_ptt; bb++) {
        tmp_ptr[bb] = sprdngf(bval[bb], bval[b]);
        tmp += tmp_ptr[bb];
      }
      pBlock->rnorm_long[b] = 1/tmp;
    }

    num_ptt = pBlock->shortWindow->num_ptt;
    bval = pBlock->shortWindow->bval;

    for (b = 0; b < num_ptt; b++) {
      Ipp32f *tmp_ptr = pBlock->sprdngf_short + b * num_ptt;
      Ipp32f tmp = 0;

      for (bb = 0; bb < num_ptt; bb++) {
        tmp_ptr[bb] = sprdngf(bval[bb], bval[b]);
        tmp += tmp_ptr[bb];
      }
      pBlock->rnorm_short[b] = 1/tmp;
    }
  }

  if (ippsFFTGetSize_R_32f(11, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
                           &sizeSpecLong, &sizeInit, &sizeWork) != ippStsNoErr) {
    return AAC_ALLOC;
  }

  if (ippsFFTGetSize_R_32f(8, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
                           &sizeSpecShort, &sizeInitShort, &sizeWorkShort) != ippStsNoErr) {
    return AAC_ALLOC;
  }

  if (sizeInit < sizeInitShort)
    sizeInit = sizeInitShort;

  if (sizeWork < sizeWorkShort)
    sizeWork = sizeWorkShort;

  *size_all = sizeSpecLong + sizeSpecShort + sizeWork + sizeInit;

  if (pBlock) {
    pBlock->pBuffer = mem + sizeSpecLong + sizeSpecShort;
    pBufInit = pBlock->pBuffer + sizeWork;

    pBlock->pFFTSpecLong = (IppsFFTSpec_R_32f*)mem;
    if (ippsFFTInit_R_32f(&(pBlock->pFFTSpecLong), 11, IPP_FFT_NODIV_BY_ANY,
                          ippAlgHintFast, mem, pBufInit) != ippStsOk) {
      return AAC_ALLOC;
    }

    pBlock->pFFTSpecShort = (IppsFFTSpec_R_32f*)(mem + sizeSpecLong);
    if (ippsFFTInit_R_32f(&(pBlock->pFFTSpecShort), 8, IPP_FFT_NODIV_BY_ANY,
                          ippAlgHintFast, mem + sizeSpecLong, pBufInit) != ippStsOk) {
      return AAC_ALLOC;
    }

    pBlock->ns_mode = ns_mode;
  }

  return AAC_OK;
}

/****************************************************************************/

void InitPsychoacoustic(sPsychoacousticBlockCom* pBlockCom,
                        sPsychoacousticBlock* pBlock)
{
  Ipp32s k;
#if !defined(ANDROID)
  Ipp32f iirTap[4];
#else
  static Ipp32f iirTap[4];
#endif

  pBlock->block_type = ONLY_LONG_SEQUENCE;
  pBlock->desired_block_type = ONLY_LONG_SEQUENCE;
  pBlock->bitsToPECoeff = 0.35f;
  pBlock->scalefactorDataBits = 100;
  pBlock->PEtoNeededPECoeff = 1;

  for (k = 0; k < 2; k++) {
    ippsSet_32f(1, pBlock->r[k], 1024);
  }

  ippsCopy_32f(pBlockCom->longWindow->qsthr, pBlock->nb_long[0],
               pBlockCom->longWindow->num_ptt);

  ippsCopy_32f(pBlockCom->longWindow->qsthr, pBlock->nb_long[1],
               pBlockCom->longWindow->num_ptt);

  pBlock->avWinEnergy = 0;
  pBlock->attackIndex = 0;
  pBlock->lastAttackIndex = 0;

  iirTap[0] = hiPassCoeff[0];
  iirTap[1] = -hiPassCoeff[0];
  iirTap[2] = 1;
  iirTap[3] = hiPassCoeff[1];

  ippsIIRInitAlloc_32f(&pBlock->IIRfilterState, iirTap, 1, NULL);
}

/****************************************************************************/

void Psychoacoustic(sPsychoacousticBlock** pBlock,
                    sPsychoacousticBlockCom* pBlockCom,
                    Ipp32f **mdct_line,
                    Ipp32s *window_shape,
                    Ipp32s *prev_window_shape,
                    Ipp32s stereo_mode,
                    Ipp32s numCh)
{
  Ipp32s lastBlockType[2];
  Ipp32s ch;

  for (ch = 0; ch < numCh; ch++) {
    BlockSwitching(pBlock[ch], pBlockCom, ch);
  }

  if (numCh == 2) {
    if (stereo_mode != AAC_LR_STEREO) {
      if (pBlock[0]->next_desired_block_type !=
          pBlock[1]->next_desired_block_type) {
        pBlock[0]->next_desired_block_type =
        pBlock[1]->next_desired_block_type = EIGHT_SHORT_SEQUENCE;
      }
    }
  }

  for (ch = 0; ch < numCh; ch++) {
    lastBlockType[ch] = pBlock[ch]->block_type;
    if ((pBlock[ch]->block_type == EIGHT_SHORT_SEQUENCE) ||
        (pBlock[ch]->block_type == LONG_START_SEQUENCE)) {
      if ((pBlock[ch]->desired_block_type == ONLY_LONG_SEQUENCE) &&
          (pBlock[ch]->next_desired_block_type == ONLY_LONG_SEQUENCE)) {
        pBlock[ch]->block_type = LONG_STOP_SEQUENCE;
      } else {
        pBlock[ch]->block_type = EIGHT_SHORT_SEQUENCE;
      }
    } else if (pBlock[ch]->next_desired_block_type == EIGHT_SHORT_SEQUENCE) {
      pBlock[ch]->block_type = LONG_START_SEQUENCE;
    } else {
      pBlock[ch]->block_type = ONLY_LONG_SEQUENCE;
    }
    pBlock[ch]->desired_block_type = pBlock[ch]->next_desired_block_type;
  }

  for (ch = 0; ch < numCh; ch++) {
    window_shape[ch] = 1;

    if ((pBlock[ch]->block_type == LONG_START_SEQUENCE) ||
        (pBlock[ch]->block_type == EIGHT_SHORT_SEQUENCE))
      window_shape[ch] = 0;

    FilterbankEnc(pBlockCom->filterbank_block,
                  (Ipp32f*)pBlockCom->input_data[ch][0],
                  (Ipp32f*)pBlockCom->input_data[ch][1],
                  pBlock[ch]->block_type, window_shape[ch],
                  prev_window_shape[ch],
                  (Ipp32f*)pBlock[ch]->r[0], 0);

     mdct_line[ch] = (Ipp32f*)pBlock[ch]->r[0];
  }

  for (ch = 0; ch < numCh; ch++) {
    if (pBlock[ch]->block_type == EIGHT_SHORT_SEQUENCE) {
      psy_short_window(pBlock[ch], pBlockCom, ch, lastBlockType[ch]);
    } else {
      psy_long_window(pBlock[ch], pBlockCom, ch);
    }
  }
}

/****************************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

