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

#ifndef REF_DECODER_COMPATIBLE

/********************************************************************/

static void CountCplCoeffCom(Ipp32f *stream_coeffs_cpl,
                             Ipp32f *stream_coeffs,
                             _AudBlk *audblk,
                             Ipp32f dpfac,
                             Ipp32f epfac,
                             Ipp32s testBsi,
                             Ipp32s ch)
{
  Ipp32s i, phsflg, ind;
  Ipp32f cpl_coord = 0;
  Ipp32s bnd = 0;
  Ipp32s bnd1 = 0;

  for (i = audblk->cplstrtmant; i < audblk->cplendmant; i += 12) {
    if (!audblk->cplbndstrc[bnd]) {
      phsflg = audblk->phsflg[bnd1];

      ind = 15 + (Ipp32s)(audblk->phscor[bnd1] * dpfac + 0.5) -
                 (Ipp32s)(audblk->phscor[bnd1] * epfac + 0.5);

      cpl_coord = audblk->cplcoord[ch][bnd1] * phsCorTab[ind];
      if      (cpl_coord >  1) cpl_coord =  1;
      else if (cpl_coord < -1) cpl_coord = -1;

      bnd1++;
    }

    if (testBsi && phsflg) {
      ippsMulC_32f(stream_coeffs_cpl+i, -cpl_coord, stream_coeffs+i, 12);
    } else {
      ippsMulC_32f(stream_coeffs_cpl+i, cpl_coord, stream_coeffs+i, 12);
    }
    bnd++;
  }
}

/********************************************************************/
#else
/********************************************************************/

static void CountCplCoeff00(Ipp32f *stream_coeffs_cpl,
                            Ipp32f *stream_coeffs,
                            _AudBlk *audblk,
                            Ipp32s ch)
{
  Ipp32s i;
  Ipp32f cpl_coord = 0;
  Ipp32s bnd = 0;

  for (i = audblk->cplstrtmant; i < audblk->cplendmant; i += 12) {
    Ipp32s j;
    Ipp32f *src = stream_coeffs_cpl+i;
    Ipp32f *dst = stream_coeffs+i;

    cpl_coord = audblk->cplcoord[ch][bnd];

    for (j = 0; j < 12; j++) {
      dst[j] = src[j] * cpl_coord;
    }

    bnd++;
  }
}

/********************************************************************/

static void CountCplCoeff01(Ipp32f *stream_coeffs_cpl,
                            Ipp32f *stream_coeffs,
                            _AudBlk *audblk,
                            Ipp32s ch)
{
  Ipp32s i, ind;
  Ipp32f cpl_coord = 0;
  Ipp32s bnd = 0;

  for (i = audblk->cplstrtmant; i < audblk->cplendmant; i += 12) {
    Ipp32s j;
    Ipp32f *src = stream_coeffs_cpl+i;
    Ipp32f *dst = stream_coeffs+i;

    ind = 15 - audblk->phscor[bnd];

    cpl_coord = audblk->cplcoord[ch][bnd] * phsCorTab[ind];
    if      (cpl_coord >  8) cpl_coord =  8;
    else if (cpl_coord < -8) cpl_coord = -8;

    for (j = 0; j < 12; j++) {
      dst[j] = src[j] * cpl_coord;
    }
    bnd++;
  }
}

/********************************************************************/

static void CountCplCoeff0Com(Ipp32f *stream_coeffs_cpl,
                              Ipp32f *stream_coeffs,
                              _AudBlk *audblk,
                              Ipp32f epfac,
                              Ipp32s ch)
{
  Ipp32s i, ind;
  Ipp32f cpl_coord = 0;
  Ipp32s bnd = 0;

  for (i = audblk->cplstrtmant; i < audblk->cplendmant; i += 12) {
    Ipp32s j;
    Ipp32f *src = stream_coeffs_cpl+i;
    Ipp32f *dst = stream_coeffs+i;

    ind = 15 - (Ipp32s)(audblk->phscor[bnd] * epfac + 0.5);

    cpl_coord = audblk->cplcoord[ch][bnd] * phsCorTab[ind];
    if      (cpl_coord >  8) cpl_coord =  8;
    else if (cpl_coord < -8) cpl_coord = -8;

    for (j = 0; j < 12; j++) {
      dst[j] = src[j] * cpl_coord;
    }
    bnd++;
  }
}


/********************************************************************/

static void CountCplCoeff10(Ipp32f *stream_coeffs_cpl,
                            Ipp32f *stream_coeffs,
                            _AudBlk *audblk,
                            Ipp32s ch)
{
  Ipp32s i, ind;
  Ipp32f cpl_coord = 0;
  Ipp32s bnd = 0;

  for (i = audblk->cplstrtmant; i < audblk->cplendmant; i += 12) {
    Ipp32s j;
    Ipp32f *src = stream_coeffs_cpl+i;
    Ipp32f *dst = stream_coeffs+i;

    ind = 15 + audblk->phscor[bnd];

    cpl_coord = audblk->cplcoord[ch][bnd] * phsCorTab[ind];
    if      (cpl_coord >  8) cpl_coord =  8;
    else if (cpl_coord < -8) cpl_coord = -8;

    for (j = 0; j < 12; j++) {
      dst[j] = src[j] * cpl_coord;
    }
    bnd++;
  }
}


/********************************************************************/

static void CountCplCoeff1Com(Ipp32f *stream_coeffs_cpl,
                              Ipp32f *stream_coeffs,
                              _AudBlk *audblk,
                              Ipp32f epfac,
                              Ipp32s ch)
{
  Ipp32s i, ind;
  Ipp32f cpl_coord = 0;
  Ipp32s bnd = 0;

  for (i = audblk->cplstrtmant; i < audblk->cplendmant; i += 12) {
    Ipp32s j;
    Ipp32f *src = stream_coeffs_cpl+i;
    Ipp32f *dst = stream_coeffs+i;

    ind = 15 + audblk->phscor[bnd] -
          (Ipp32s)(audblk->phscor[bnd] * epfac + 0.5);

    cpl_coord = audblk->cplcoord[ch][bnd] * phsCorTab[ind];
    if      (cpl_coord >  8) cpl_coord =  8;
    else if (cpl_coord < -8) cpl_coord = -8;

    for (j = 0; j < 12; j++) {
      dst[j] = src[j] * cpl_coord;
    }
    bnd++;
  }
}


/********************************************************************/

static void CountCplCoeffCom0(Ipp32f *stream_coeffs_cpl,
                             Ipp32f *stream_coeffs,
                             _AudBlk *audblk,
                             Ipp32f dpfac,
                             Ipp32s ch)
{
  Ipp32s i, ind;
  Ipp32f cpl_coord = 0;
  Ipp32s bnd = 0;

  for (i = audblk->cplstrtmant; i < audblk->cplendmant; i += 12) {
    Ipp32s j;
    Ipp32f *src = stream_coeffs_cpl+i;
    Ipp32f *dst = stream_coeffs+i;

    ind = 15 + (Ipp32s)(audblk->phscor[bnd] * dpfac + 0.5);

    cpl_coord = audblk->cplcoord[ch][bnd] * phsCorTab[ind];
    if      (cpl_coord >  8) cpl_coord =  8;
    else if (cpl_coord < -8) cpl_coord = -8;

    for (j = 0; j < 12; j++) {
      dst[j] = src[j] * cpl_coord;
    }
    bnd++;
  }
}


/********************************************************************/

static void CountCplCoeffCom1(Ipp32f *stream_coeffs_cpl,
                             Ipp32f *stream_coeffs,
                             _AudBlk *audblk,
                             Ipp32f dpfac,
                             Ipp32s ch)
{
  Ipp32s i, ind;
  Ipp32f cpl_coord = 0;
  Ipp32s bnd = 0;

  for (i = audblk->cplstrtmant; i < audblk->cplendmant; i += 12) {
    Ipp32s j;
    Ipp32f *src = stream_coeffs_cpl+i;
    Ipp32f *dst = stream_coeffs+i;

    ind = 15 + (Ipp32s)(audblk->phscor[bnd] * dpfac + 0.5) -
                audblk->phscor[bnd];

    cpl_coord = audblk->cplcoord[ch][bnd] * phsCorTab[ind];
    if      (cpl_coord >  8) cpl_coord =  8;
    else if (cpl_coord < -8) cpl_coord = -8;

    for (j = 0; j < 12; j++) {
      dst[j] = src[j] * cpl_coord;
    }
    bnd++;
  }
}


/********************************************************************/

static void CountCplCoeffCom(Ipp32f *stream_coeffs_cpl,
                             Ipp32f *stream_coeffs,
                             _AudBlk *audblk,
                             Ipp32f dpfac,
                             Ipp32f epfac,
                             Ipp32s ch)
{
  Ipp32s i, ind;
  Ipp32f cpl_coord = 0;
  Ipp32s bnd = 0;

  for (i = audblk->cplstrtmant; i < audblk->cplendmant; i += 12) {
    Ipp32s j;
    Ipp32f *src = stream_coeffs_cpl+i;
    Ipp32f *dst = stream_coeffs+i;

    ind = 15 + (Ipp32s)(audblk->phscor[bnd] * dpfac + 0.5) -
               (Ipp32s)(audblk->phscor[bnd] * epfac + 0.5);

    cpl_coord = audblk->cplcoord[ch][bnd] * phsCorTab[ind];
    if      (cpl_coord >  8) cpl_coord =  8;
    else if (cpl_coord < -8) cpl_coord = -8;

    for (j = 0; j < 12; j++) {
      dst[j] = src[j] * cpl_coord;
    }
    bnd++;
  }
}

/********************************************************************/
#endif
/********************************************************************/

void uncoupleChannel(AC3Dec *state,
                     Ipp32s ch)
{
  _AudBlk *audblk = &(state->audblk);
  Ipp32f *stream_coeffs = state->coeffs[ch];
  Ipp32f *stream_coeffs_cpl = state->cplChannel;
  Ipp32f dpfac = phscorFac[state->out_acmod][state->bsi.acmod][state->bsi.surmixlev][ch];
  Ipp32f epfac = phscorFac[audblk->phsoutmod][state->bsi.acmod][state->bsi.surmixlev][ch];
#ifndef REF_DECODER_COMPATIBLE
  Ipp32s testBsi = 0;

  if (state->bsi.acmod == 0x02 && ch == 1 && audblk->phsflginu)
    testBsi = 1;

  CountCplCoeffCom(stream_coeffs_cpl, stream_coeffs, audblk,
                       dpfac, epfac, testBsi, ch);
#else
  if (dpfac == epfac) {
    CountCplCoeff00(stream_coeffs_cpl, stream_coeffs, audblk, ch);
  } else if (dpfac == 0) {
    if (epfac == 1) {
      CountCplCoeff01(stream_coeffs_cpl, stream_coeffs, audblk, ch);
    } else {
      CountCplCoeff0Com(stream_coeffs_cpl, stream_coeffs, audblk,
                        epfac, ch);
    }
  } else if (dpfac == 1) {
    if (epfac == 0) {
      CountCplCoeff10(stream_coeffs_cpl, stream_coeffs, audblk, ch);
    } else {
      CountCplCoeff1Com(stream_coeffs_cpl, stream_coeffs, audblk,
                        epfac, ch);
    }
  } else {
    if (epfac == 0) {
      CountCplCoeffCom0(stream_coeffs_cpl, stream_coeffs, audblk,
                        dpfac, ch);
    } else if (epfac == 1) {
      CountCplCoeffCom1(stream_coeffs_cpl, stream_coeffs, audblk,
                        dpfac, ch);
    } else {
      CountCplCoeffCom(stream_coeffs_cpl, stream_coeffs, audblk,
                    dpfac, epfac, ch);
    }
  }
#endif
}

/********************************************************************/

#endif //UMC_ENABLE_AC3_AUDIO_DECODER





