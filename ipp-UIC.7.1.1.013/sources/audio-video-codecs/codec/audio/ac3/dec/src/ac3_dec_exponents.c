/* ////////////////////////// ac3dec_exponents.cpp ////////////////////////// */
/*
//
//              INTeL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AC3_AUDIO_DECODER)

#include "ac3_dec.h"
#include "ac3_dec_own_fp.h"
#include "ac3_dec_tables.h"

/********************************************************************/

static Ipp32s expUnpackCh(Ipp32s TypeCh,
                          Ipp32s ExpStr,
                          Ipp32s nGrps,
                          Ipp32s Exp0,
                          Ipp32s *source,
                          Ipp32s *dest)
{
  Ipp32s i;
  Ipp32s exp_acc;
  Ipp32s cbErrors = 0;  /* error counter */
  Ipp32s pkexp = 0;

  if (ExpStr == EXP_REUSE)
    return cbErrors;

/*
 * Handle the initial absolute exponent
 */
  exp_acc = Exp0;

/*
 * In the case of a fbw channel then the initial absolute values is also an
 * exponent
 */
  if (TypeCh != CH_CPL) {
    *dest = exp_acc;
    dest++;
  }

/*
 * Loop through the groups and fill the dest array appropriately
 */

  switch (ExpStr) {
    case EXP_D45:
      for (i = 0; i < nGrps; i++) {
        pkexp = source[i];
        pkexp = UNGRP5[pkexp];
        exp_acc = (((pkexp >> 12) & 0x000f) - 2) + exp_acc;
        dest[0] = dest[1] = dest[2] = dest[3] = exp_acc;
        exp_acc = (((pkexp >> 8) & 0x000f) - 2) + exp_acc;
        dest[4] = dest[5] = dest[6] = dest[7] = exp_acc;
        exp_acc = (((pkexp >> 4) & 0x000f) - 2) + exp_acc;
        dest[8] = dest[9] = dest[10] = dest[11] = exp_acc;
        dest += 12;
      }
      break;

    case EXP_D25:
      for (i = 0; i < nGrps; i++) {
        pkexp = source[i];
        pkexp = UNGRP5[pkexp];
        exp_acc = (((pkexp >> 12) & 0x000f) - 2) + exp_acc;
        dest[0] = dest[1] = exp_acc;
        exp_acc = (((pkexp >> 8) & 0x000f) - 2) + exp_acc;
        dest[2] = dest[3] = exp_acc;
        exp_acc = (((pkexp >> 4) & 0x000f) - 2) + exp_acc;
        dest[4] = dest[5] = exp_acc;
        dest += 6;
      }
      break;

    case EXP_D15:
      for (i = 0; i < nGrps; i++) {
        pkexp = source[i];
        pkexp = UNGRP5[pkexp];
        exp_acc = (((pkexp >> 12) & 0x000f) - 2) + exp_acc;
        dest[0] = exp_acc;
        exp_acc = (((pkexp >> 8) & 0x000f) - 2) + exp_acc;
        dest[1] = exp_acc;
        exp_acc = (((pkexp >> 4) & 0x000f) - 2) + exp_acc;
        dest[2] = exp_acc;
        dest += 3;
      }
      break;

    default:
      break;
  }

  return cbErrors;      /* return # exponent unpacking errors */
}

/********************************************************************/

Ipp32s DecodeExponents(AC3Dec *state)
{
  Ipp32s i;
  Ipp32u cbErrors = 0;
  _AudBlk *audblk = &(state->audblk);

  for (i = 0; i < state->bsi.nfchans; i++) {
    cbErrors +=
      expUnpackCh(CH_FBW, audblk->chexpstr[i], audblk->nchgrps[i],
                  audblk->exps[i][0], &audblk->exps[i][1],
                  state->fbw_exp[i]);
  }

  if (audblk->cplinu) {
    cbErrors +=
      expUnpackCh(CH_CPL, audblk->cplexpstr, audblk->ncplgrps,
                  (audblk->cplabsexp << 1),
                  &audblk->cplexps[0],
                  &state->cpl_exp[audblk->cplstrtmant]);
  }

  if (state->bsi.lfeon) {
    cbErrors +=
      expUnpackCh(CH_LFE, audblk->lfeexpstr, 2, audblk->lfeexps[0],
                  &audblk->lfeexps[1], state->lfe_exp);
  }

  return cbErrors;      /* return # unpacking errors */
}

/********************************************************************/

#endif //UMC_ENABLE_AC3_AUDIO_DECODER

