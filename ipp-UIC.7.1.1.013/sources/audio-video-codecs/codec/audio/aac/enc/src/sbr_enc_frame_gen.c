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
#if defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include <math.h>
#include "aac_status.h"
#include "sbr_settings.h"
#include "sbr_struct.h"
#include "sbr_freq_tabs.h"
#include "sbr_enc_settings.h"
#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"

#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

/********************************************************************/

#define MAX_SLOTS        12

/********************************************************************/

static const sSBRFrameInfoState
sbrFrameInfo_FIXFIX_Env1 = {
  1,
  {0, 16},
  {1},
  0,
  1,
  {0, 16},
  /* empy data */
  1,
  {0},
  1
};

static const sSBRFrameInfoState
sbrFrameInfo_FIXFIX_Env2 = {
  2,
  {0, 8, 16},
  {1, 1},
  0,
  2,
  {0, 8, 16},
  /* empy data */
  1,
  {0},
  1
};

static const sSBRFrameInfoState
sbrFrameInfo_FIXFIX_Env4 = {
  4,
  {0, 4, 8, 12, 16},
  {1, 1, 1, 1},
  0,
  2,
  {0, 8, 16},
  /* empy data */
  1,
  {0},
  1
};

static const sSBRFrameInfoState*
sbr_FIXFIX_FrameInfoTabs[] = {
  &sbrFrameInfo_FIXFIX_Env1,

  &sbrFrameInfo_FIXFIX_Env2,

  &sbrFrameInfo_FIXFIX_Env4
};

/********************************************************************/

static Ipp32s sbrencCalcFrameClass(Ipp32s frameClassOld, Ipp32s tranFlag)
{
  Ipp32s frameClass = FIXFIX;

  switch (frameClassOld) {
  case FIXFIX:
    if (tranFlag)
      frameClass = FIXVAR;// stationary to transient transition
    else
      frameClass = FIXFIX;// when no transients are present, FIXFIX frames are used

    break;

  case FIXVAR:
    if (tranFlag)
      frameClass = VARVAR;// "tight" transients are handeled by VARVAR frames
    else
      frameClass = VARFIX;// "sparse" transients are handeled by [FIXVAR, VARFIX] pairs

    break;

  case VARFIX:
    if (tranFlag)
      frameClass = FIXVAR;
    else
      frameClass = FIXFIX;// transient to stationary transition

    break;

  case VARVAR:
    if (tranFlag)
      frameClass = VARVAR;// "tight" transients are handeled by VARVAR frames
    else
      frameClass = VARFIX;

    break;
  }

  return frameClass;

}

/********************************************************************/
/*              utility functions are used by frame generator       */
/********************************************************************/

static void sbrencAddRE2Buf(Ipp32s* pBuf, Ipp32s* lenBuf, Ipp32s val)
{
  pBuf[*lenBuf] = val;
  (*lenBuf)++;

  return;
}

/********************************************************************/
/*               AddLE2Buf = Add Left Element To Buffer             */
/********************************************************************/

static void sbrencAddLE2Buf(Ipp32s* pBuf, Ipp32s* lenBuf, Ipp32s val)
{
  Ipp32s i;

  for (i = *lenBuf; i > 0; i--) {
    pBuf[i] = pBuf[i - 1];
  }

  pBuf[0] = val;

  (*lenBuf)++;

  return;
}

/********************************************************************/
/*               AddLB2Buf = Add Left Buffer To Buffer              */
/********************************************************************/

static void sbrencAddLB2Buf(Ipp32s *pSrc, Ipp32s lenSrc, Ipp32s *pDst, Ipp32s* lenDst)
{
  Ipp32s i;

  for (i = lenSrc - 1; i >= 0; i--) {

    sbrencAddLE2Buf(pDst, lenDst, pSrc[i]);
  }

  return;
}

/********************************************************************/
/* subdivides dist by calc segments quant to the lengths   */
static void
sbrencCalcSegments(Ipp32s dist0, Ipp32s maxSegment, Ipp32s* nSegments, Ipp32s* segment)
{
  Ipp32s dist, curSegment = 1;

  *nSegments = 1;
  dist = dist0;

  while (dist > MAX_SLOTS) {

    (*nSegments)++;

    curSegment = dist0 / (*nSegments);

    curSegment = (Ipp32s) floor ( curSegment * 0.5f);
    curSegment = IPP_MIN( maxSegment, 2 * curSegment );

    dist = dist0 - ((*nSegments) - 1) * curSegment;
  }

  *segment = curSegment;

  return;
}

/********************************************************************/

static void
sbrencLoadTransientBord(Ipp32s* borders, Ipp32s* lenBord,
                        Ipp32s* freq_res, Ipp32s* lenFreqRes,
                        Ipp32s* minBord, Ipp32s* maxBord, Ipp32s tranPos)
{
  /* reset vectors */
  *lenBord = 0;
  *lenFreqRes = 0;

  /* fill vecBorders: 3 elements [ tranPos + 4, tranPos + 6, tranPos + 10 ] */
  sbrencAddRE2Buf(borders, lenBord, tranPos + 4 );
  sbrencAddRE2Buf(borders, lenBord, tranPos + 6 );
  sbrencAddRE2Buf(borders, lenBord, tranPos + 10);

  /* fill vecFreqRes: 3 elements [ LOW=0, LOW=0, HI=1 ] */
  sbrencAddRE2Buf(freq_res, lenFreqRes, LO );
  sbrencAddRE2Buf(freq_res, lenFreqRes, LO );
  sbrencAddRE2Buf(freq_res, lenFreqRes, HI );

  /* patch */
  *minBord = borders[ 0 ];
  *maxBord = borders[ *lenBord - 1 ];

  return;
}

/********************************************************************/

static void sbrencLoadBordPreProc(Ipp32s* borders, Ipp32s* lenBord,
                                  Ipp32s* freqRes, Ipp32s* lenFreqRes,
                                  Ipp32s minBord, Ipp32s rest)
{
  Ipp32s segment, nSegments;
  Ipp32s bord = minBord;
  Ipp32s j;

  /* fillHelper( GP.aBorders[0], 8 ) */
  sbrencCalcSegments( rest, 8, &nSegments, &segment);

  /*
   * GP.aBorders = cat(fliplr(aBordersFill), GP.aBorders);
   * GP.aFreqRes = cat(ones(length(aBordersFill)), GP.aFreqRes);
   */
  for (j = 0; j <= nSegments - 2; j++) {

    bord = bord - segment;

    sbrencAddLE2Buf(borders, lenBord, bord);

    sbrencAddLE2Buf(freqRes, lenFreqRes, HI);
  }

  return;
}

/********************************************************************/
static void sbrencLoadBordPostProc(Ipp32s tranPos,
                                   Ipp32s* borders, Ipp32s* lenBord,
                                   Ipp32s* freqRes, Ipp32s* lenFreqRes,
                                   Ipp32s* nSegments)
{
  Ipp32s segment = 0;
  Ipp32s bord = 0;
  Ipp32s j = 0;
  Ipp32s maxStep = 0;

  //****
  if (tranPos < 4){
      maxStep = 6;
  } else if (tranPos == 4 || tranPos == 5) {
      maxStep = 4;
  } else {
      maxStep = 8;
  }

  bord = borders[ *lenBord - 1 ];
  bord = 32 - bord;

  /* mandatory patch */
  if ( bord <= 0 ) {

    *nSegments = 1;
    *lenBord = *lenBord - 1;
    *lenFreqRes = *lenFreqRes - 1;

    return;
  }

  /* else if( bord > 0 ) */
  sbrencCalcSegments(bord, maxStep, nSegments, &segment);

  bord = borders[ *lenBord - 1 ];

  for (j = 0; j <= *nSegments - 2; j++) {

    bord += segment;

    sbrencAddRE2Buf(borders, lenBord, bord);

    sbrencAddRE2Buf(freqRes, lenFreqRes, HI);
  }
}

/********************************************************************/

static void sbrencSplitGrid(Ipp32s* borders, Ipp32s* lenBord,
                            Ipp32s* freqRes,
                            Ipp32s tran, /* tran = tranPos + 4 */
                            Ipp32s* bordersNext, Ipp32s* lenBordNext,
                            Ipp32s* freqResNext, Ipp32s* lenFreqResNext,
                            Ipp32s* pIndxTran, Ipp32s* pIndxSplt,
                            Ipp32s* pIndxTranNext, Ipp32s* pIdxFillNext,
                            Ipp32s nSegments)
{
  Ipp32s iSplit = 0;
  Ipp32s iTran  = EMPTY_MAPPING;
  Ipp32s i, j;
  Ipp32s iTranNext;

  *lenBordNext = 0;
  *lenFreqResNext = 0;

  while( (borders[iSplit] < MAX_SLOTS + 4) && (iSplit < *lenBord) ) iSplit++;

  for (i = 0; i < *lenBord; i++) {
    if (borders[i] >= tran) {
      iTran = i;
      break;
    }
  }

  for (j = 0, i = iSplit; i < *lenBord; i++, j++) {

    bordersNext[j] = borders[i] - 16;

    freqResNext[j] = freqRes[i];

    (*lenBordNext)++;
    (*lenFreqResNext)++;
  }

  /* mandatory patch */
  iTranNext = EMPTY_MAPPING;
  if (iTran != EMPTY_MAPPING) {
    iTranNext = iTran - iSplit;
  }

  /* I can't stand it */
  *pIdxFillNext = *lenBord - (nSegments - 1) - iSplit;

  //---------------------
  // return result
  *pIndxTran = iTran;
  *pIndxSplt = iSplit;

  *pIndxTranNext = iTranNext;
  //--------------------

  return;
}

/********************************************************************/

static void
sbrencConflictResolution (Ipp32s *borders, Ipp32s *lenBord,
                          Ipp32s minBord,
                          Ipp32s *freqRes, Ipp32s *lenFreqRes,

                          Ipp32s *bordersNext, Ipp32s *lenBordNext,
                          Ipp32s *freqResNext, Ipp32s *lenFreqResNext,

                          Ipp32s iFillNext,
                          Ipp32s dmin, Ipp32s dmax,

                          Ipp32s *nLeftBord)
{
  Ipp32s nBordNext, maxBordNext, i;
  Ipp32s middleBord;

  /* be careful: transfer */
  if (iFillNext >= 1) {

    *lenBordNext   = iFillNext;
    *lenFreqResNext = iFillNext;
  }

  nBordNext = *lenBordNext;

  maxBordNext = bordersNext[nBordNext - 1];

  middleBord = minBord - maxBordNext;

  while (middleBord < 0) {

    nBordNext--;
    maxBordNext = bordersNext[nBordNext - 1];
    middleBord = minBord - maxBordNext;
  }

  *lenBordNext = nBordNext;
  *lenFreqResNext = nBordNext;

  *nLeftBord = nBordNext - 1;

  //----------------------------
  if (middleBord <= dmax) {

    if (middleBord >= dmin) {

       sbrencAddLB2Buf(bordersNext, *lenBordNext, borders, lenBord);

       sbrencAddLB2Buf(freqResNext, *lenFreqResNext, freqRes, lenFreqRes);

    } else { /* middleBord > dmin */

      if (*lenBordNext > 1) {

        sbrencAddLB2Buf(bordersNext, *lenBordNext-1, borders, lenBord);

        sbrencAddLB2Buf(freqResNext, *lenFreqResNext - 1, freqRes, lenFreqRes);

        nLeftBord--;
      } else {

        /* lenBord ==== lenFreqRes */
        for (i = 0; i < *lenBord - 1; i++) {
          borders[i] = borders[i + 1];
          freqRes[i] = freqRes[i + 1];
        }

        (*lenBord)--;
        (*lenFreqRes)--;

        sbrencAddLB2Buf(bordersNext, *lenBordNext, borders, lenBord);

        sbrencAddLB2Buf(freqResNext, *lenFreqResNext, freqRes, lenFreqRes);

      } /*  */
    } /*  */
  } else { /* (middleBord > dmax) */

    sbrencLoadBordPreProc(borders, lenBord, freqRes, lenFreqRes, minBord, middleBord);

    sbrencAddLB2Buf(bordersNext, *lenBordNext, borders, lenBord);

    sbrencAddLB2Buf(freqResNext, *lenFreqResNext, freqRes, lenFreqRes);
  }

  return;
}

/********************************************************************/

static void
sbrencCalcSBRGrid(sSBRGrid* pGridState,
                  Ipp32s frameClass,

                  Ipp32s *borders, Ipp32s lenBord,
                  Ipp32s *freqRes, Ipp32s lenFreqRes,
                  Ipp32s iSplit, Ipp32s iTran, Ipp32s nLeftBord)
{
  Ipp32s i, n, absBordLead, absBordTrail, nRightBord, env;
  Ipp32s absBord    = 0;
  Ipp32s bs_pointer = 0;
  Ipp32s rel_bord   = 0;

  Ipp32s *bs_freq_res    = pGridState->bs_freq_res;
  Ipp32s *bs_freq_res_LR = pGridState->bs_freq_res_LR;

  Ipp32s *bs_rel_bord    = pGridState->bs_rel_bord;
  Ipp32s *bs_rel_bord_0  = pGridState->bs_rel_bord_0;
  Ipp32s *bs_rel_bord_1  = pGridState->bs_rel_bord_1;

  /* set all length to ZERO */
  Ipp32s len_bs_rel_bord   = 0;
  Ipp32s len_bs_rel_bord_1 = 0;
  Ipp32s len_bs_rel_bord_0 = 0;

  switch (frameClass) {
//-----------------------
  case FIXVAR:

    absBord = borders[iSplit];
    i = iSplit;

    while (i >= 1) {
      rel_bord = borders[i] - borders[i - 1];
      sbrencAddRE2Buf(bs_rel_bord, &len_bs_rel_bord, rel_bord);
      i--;
    }

    n = len_bs_rel_bord;

    for (env = 0; env < iSplit; env++) {
      bs_freq_res[env] = freqRes[iSplit - 1 - env];
    }

    bs_freq_res[iSplit] = 1;

    /* pointer: */
    if (iSplit >= iTran && iTran != EMPTY_MAPPING) {
      bs_pointer = iSplit - iTran + 1;
    }

    pGridState->bs_abs_bord = absBord;
    pGridState->n = n;

    break;
//-----------------------
  case VARFIX:

    absBord = borders[0];

    for (i = 1; i < lenBord; i++) {
      rel_bord = borders[i] - borders[i - 1];
      sbrencAddRE2Buf(bs_rel_bord, &len_bs_rel_bord, rel_bord);
    }

    n = len_bs_rel_bord;

    for( env = 0; env < lenFreqRes; env++){
      bs_freq_res[env] = freqRes[env];
    }

    if (iTran >= 0 && iTran != EMPTY_MAPPING) {
      bs_pointer = iTran + 1;
    }

    pGridState->bs_abs_bord = absBord;
    pGridState->n = n;

    break;
//-----------------------
  case VARVAR:

    {
      lenBord = iSplit + 1;
      lenFreqRes = iSplit + 1;

      absBordLead = borders[0];
      absBordTrail = borders[lenBord - 1];

      nRightBord = lenBord - nLeftBord - 2;

      for (i = 1; i <= nLeftBord; i++) {
        rel_bord = borders[i] - borders[i - 1];
        sbrencAddRE2Buf(bs_rel_bord_0, &len_bs_rel_bord_0, rel_bord);
      }

      i = lenBord - 1;

      while (i >= lenBord - nRightBord) {
        rel_bord = borders[i] - borders[i - 1];
        sbrencAddRE2Buf(bs_rel_bord_1, &len_bs_rel_bord_1, rel_bord);
        i--;
      }

      if (iSplit >= iTran && iTran != EMPTY_MAPPING) {
        bs_pointer = iSplit - iTran + 1;
      }

      for (env = 0; env < lenBord - 1; env++) {
        bs_freq_res_LR[env] = freqRes[env];
      }
    }

    pGridState->bs_abs_bord_0 = absBordLead;
    pGridState->bs_abs_bord_1 = absBordTrail;
    pGridState->bs_num_rel_0  = nLeftBord;
    pGridState->bs_num_rel_1  = nRightBord;

    break;
//-----------------------
  default:
    /* do nothing */
    break;
//-----------------------
  } //END switch (frameClass) {

  pGridState->frameClass = frameClass;
  pGridState->bs_pointer = bs_pointer;

  return;
}

/***************************************************************************/

static Ipp32s sbrencGetStatic_FIXFIX_Tabs(sSBRFrameInfoState* pFIState, Ipp32s nEnv )
{
  Ipp32s indx = 0;
#if !defined(ANDROID)
  const Ipp32s mapTab[] = { 0, 0, 1, 1, 2 };
#else
  static const Ipp32s mapTab[] = { 0, 0, 1, 1, 2 };
#endif
  Ipp8u* pTab = NULL;

  if ( nEnv < 1 || nEnv > 4 )
    return -1; // error

  indx = mapTab[ nEnv ];

  pTab = (Ipp8u*)sbr_FIXFIX_FrameInfoTabs[ indx ];

  ippsCopy_8u(pTab, (Ipp8u*)pFIState, sizeof (sSBRFrameInfoState));

  return 0;//OK
}

/********************************************************************/

static void
sbrencSBRGrid2FrameInfo (sSBRGrid* pGridState, sSBRFrameInfoState* pFIState)
{
  Ipp32s nEnv = 0, border = 0;
  Ipp32s i, k;
  Ipp32s bs_pointer;
  sSBRGrid* in = pGridState;
  sSBRFrameInfoState* out = pFIState;

  Ipp32s frameClass = in->frameClass;

//---------------------------------------------------------
  switch (frameClass) {

  case FIXFIX:
    sbrencGetStatic_FIXFIX_Tabs(out, out->nEnv);

    break;

  case FIXVAR:
  case VARFIX:

    nEnv = in->n + 1;

    out->nEnv = nEnv;

    border = in->bs_abs_bord;

    if (nEnv == 1)
      out->nNoiseEnv = 1;
    else
      out->nNoiseEnv = 2;

    break;

  default:
    /* do nothing */
    break;
  }

//---------------------------------------------------------

  switch (frameClass) {
  case FIXVAR:

    out->bordersEnv[0] = 0;

    out->bordersEnv[nEnv] = border;

    for (k = 0, i = nEnv - 1; k < nEnv - 1; k++, i--) {
      border -= in->bs_rel_bord[k];

      out->bordersEnv[i] = border;
    }

    bs_pointer = in->bs_pointer;

    if (bs_pointer == 0) {

      out->shortEnv = 0;
    } else {

      out->shortEnv = nEnv + 1 - bs_pointer;
    }

    for (k = 0, i = nEnv - 1; k < nEnv; k++, i--) {
      out->freqRes[i] = in->bs_freq_res[k];
    }

    if (bs_pointer == 0 || bs_pointer == 1) {
      out->bordersNoise[1] = out->bordersEnv[nEnv - 1];
    } else {
      out->bordersNoise[1] = out->bordersEnv[out->shortEnv];
    }

    break;

  case VARFIX:
    out->bordersEnv[0] = border;

    for (k = 0; k < nEnv - 1; k++) {
      border += in->bs_rel_bord[k];
      out->bordersEnv[k + 1] = border;
    }

    out->bordersEnv[nEnv] = 16;

    bs_pointer = in->bs_pointer;

    if (bs_pointer == 0 || bs_pointer == 1) {
      out->shortEnv = 0;
    } else {
      out->shortEnv = bs_pointer - 1;
    }

    for (k = 0; k < nEnv; k++) {
      out->freqRes[k] = in->bs_freq_res[k];
    }

    switch (bs_pointer) {
    case 0:
      out->bordersNoise[1] = out->bordersEnv[1];
      break;
    case 1:
      out->bordersNoise[1] = out->bordersEnv[nEnv - 1];
      break;
    default:
      out->bordersNoise[1] = out->bordersEnv[out->shortEnv];
      break;
    }
    break;

  case VARVAR:
    nEnv = in->bs_num_rel_0 + in->bs_num_rel_1 + 1;

    out->nEnv = nEnv;

    out->bordersEnv[0] = border = in->bs_abs_bord_0;

    for (k = 0, i = 1; k < in->bs_num_rel_0; k++, i++) {
      border += in->bs_rel_bord_0[k];
      out->bordersEnv[i] = border;
    }

    border = in->bs_abs_bord_1;

    out->bordersEnv[nEnv] = border;

    for (k = 0, i = nEnv - 1; k < in->bs_num_rel_1; k++, i--) {
      border -= in->bs_rel_bord_1[k];
      out->bordersEnv[i] = border;
    }

    bs_pointer = in->bs_pointer;

    if (bs_pointer == 0) {
      out->shortEnv = 0;
    } else {
      out->shortEnv = nEnv + 1 - bs_pointer;
    }

    for (k = 0; k < nEnv; k++) {
      out->freqRes[k] = in->bs_freq_res_LR[k];
    }

    if (nEnv == 1) {
      out->nNoiseEnv = 1;
      out->bordersNoise[0] = in->bs_abs_bord_0;
      out->bordersNoise[1] = in->bs_abs_bord_1;
    } else {
      out->nNoiseEnv = 2;
      out->bordersNoise[0] = in->bs_abs_bord_0;

      if (bs_pointer == 0 || bs_pointer == 1) {
        out->bordersNoise[1] = out->bordersEnv[nEnv - 1];
      } else {
        out->bordersNoise[1] = out->bordersEnv[out->shortEnv];
      }

      out->bordersNoise[2] = in->bs_abs_bord_1;
    }
    break;

  default:
    /* do nothing */
    break;
  }

  if (frameClass == VARFIX || frameClass == FIXVAR) {

    out->bordersNoise[0] = out->bordersEnv[0];

    if (nEnv == 1) {
      out->bordersNoise[1] = out->bordersEnv[nEnv];
    } else {
      out->bordersNoise[2] = out->bordersEnv[nEnv];
    }
  }

  return;
}

/********************************************************************/

Ipp32s sbrencFrameGenerator(sSBREnc_SCE_State* pState, Ipp32s splitFlag)
{
  sSBRFrameInfoState* pFIState = &(pState->sbrFIState);
  sSBRGrid* pGridState = &(pState->sbrGrid);

  //sSBRGridRaw* pGRawNext = &( pState->sbrGridRawNext );
  //sSBRGridRaw* pGRaw     = &( pState->sbrGridRaw );

  /* current info */
  Ipp32s* borders = pState->sbrGridRaw.bufBorders;
  Ipp32s* freqRes = pState->sbrGridRaw.bufFreqRes;

  Ipp32s* lenBord   = &(pState->sbrGridRaw.lenBord);
  Ipp32s* lenFreqRes = &(pState->sbrGridRaw.lenFreqRes);

  /* next info */
  Ipp32s* bordersNext = pState->sbrGridRawNext.bufBorders;
  Ipp32s* freqResNext = pState->sbrGridRawNext.bufFreqRes;

  Ipp32s* lenBordNext   = &(pState->sbrGridRawNext.lenBord);
  Ipp32s* lenFreqResNext = &(pState->sbrGridRawNext.lenFreqRes);

  Ipp32s* iTranNext = &(pState->sbrGridRawNext.iTran);
  Ipp32s* iFillNext = &(pState->sbrGridRawNext.iFill);

  Ipp32s frameClass = FIXFIX;
  Ipp32s minBord = 0, maxBord = 0;
  Ipp32s tranFlag  = pState->sbrTransientState.tranFlag;
  Ipp32s tranPos   = pState->sbrTransientState.tranPos;
  Ipp32s numEnv;

  Ipp32s tran = 0;
  Ipp32s nSegments = 0;

  Ipp32s iSplit = 0;
  Ipp32s iTran  = 0;

  Ipp32s nLeftBord;

  /* CODE */

  frameClass = sbrencCalcFrameClass(pState->sbrGrid.frameClassOld, tranFlag);
  /* update oldFrameClass */
  pState->sbrGrid.frameClassOld = frameClass;

  if (tranFlag) {

    tran = tranPos + 4;

    sbrencLoadTransientBord(borders, lenBord, freqRes, lenFreqRes,
                            &minBord, &maxBord, tranPos);
  }
//---------------------

  switch ( frameClass ) {
//-----------------------
  case FIXFIX:

    /* patch */
    if( 0 == splitFlag )
      numEnv = 1;
    else
      numEnv = 2;

    pFIState->nEnv  = numEnv;
    pGridState->frameClass = frameClass;

    break;
//-----------------------
  case FIXVAR:

    //if( tranPos > 8 ) {
      sbrencLoadBordPreProc(borders, lenBord, freqRes, lenFreqRes, minBord, minBord);
    //}

    //if( tranPos < 10 ) {
      sbrencLoadBordPostProc(tranPos, borders, lenBord, freqRes, lenFreqRes, &nSegments);
    //}

    sbrencSplitGrid(borders, lenBord,
                    freqRes,
                    tran,
                    bordersNext, lenBordNext,
                    freqResNext, lenFreqResNext,

                    &iTran, &iSplit,
                    iTranNext, iFillNext,

                    nSegments);

    sbrencCalcSBRGrid(pGridState, frameClass,

                      borders, *lenBord,
                      freqRes, *lenFreqRes,
                      iSplit, iTran, EMPTY_MAPPING);

    break;
//-----------------------
  case VARFIX:

    sbrencCalcSBRGrid(pGridState, frameClass,

                      bordersNext, *lenBordNext,
                      freqResNext, *lenFreqResNext,
                      EMPTY_MAPPING, iTran, EMPTY_MAPPING);

    break;
//-----------------------
  case VARVAR:

    sbrencConflictResolution (borders, lenBord,
                              minBord,
                              freqRes, lenFreqRes,

                              bordersNext, lenBordNext,
                              freqResNext, lenFreqResNext,

                              *iFillNext, 4, 12,

                              &nLeftBord);

    sbrencLoadBordPostProc(tranPos,
                           borders, lenBord,
                           freqRes, lenFreqRes,
                           &nSegments);

    sbrencSplitGrid(borders, lenBord,
                    freqRes,
                    tran,
                    bordersNext, lenBordNext,
                    freqResNext, lenFreqResNext,

                    &iTran, &iSplit,
                    iTranNext, iFillNext,

                    nSegments);

    sbrencCalcSBRGrid(pGridState, frameClass,
                      borders, *lenBord,
                      freqRes, *lenFreqRes,
                      iSplit, iTran, nLeftBord);

    break;
//-----------------------
  } /* END of SWITCH */

//  printf("\nlen_bord = %i\n", *lenBord);

  sbrencSBRGrid2FrameInfo (pGridState, pFIState);

    /* AYA */
  //-------------------------------------------------
   /* debug */
#ifdef SBR_NEED_LOG
  {
    Ipp32s i;

    fprintf(logFile, "SbrFrameInfo\n");

    fprintf(logFile, "frameClass = %i\n", frameClass);

    fprintf(logFile, "envelopes = %i\n", pFIState->nEnv);
    fprintf(logFile, "shortEnv = %i\n", pFIState->shortEnv);
    fprintf(logFile, "nNoiseEnvelopes = %i\n", pFIState->nNoiseEnv);

    fprintf(logFile, "borders v\n");
    for(i=0; i<pFIState->nEnv; i++){
      fprintf(logFile, "borders[%i] = %i  ", i, pFIState->bordersEnv[i]);
    }

    fprintf(logFile, "\nfreqRes v\n");
    for(i=0; i<pFIState->nEnv; i++){
      fprintf(logFile, "freqRes[%i] = %i  ", i, pFIState->freqRes[i]);
    }

    fprintf(logFile, "\nbordersNoise v\n");
    for(i=0; i<pFIState->nNoiseEnv; i++){
      fprintf(logFile, "bordersNoise[%i] = %i  ", i, pFIState->bordersNoise[i]);
    }

    fprintf(logFile, "\n");

  }
#endif
  //-------------------------------------------------

  return 0; //OK

}

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

