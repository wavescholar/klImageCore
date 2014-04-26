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

void InverseTransform(Ipp32s before_downmix,
                      AC3Dec *state)
{
  Ipp32s ch = 0;
  Ipp32s nChannel = 0;
  Ipp32s blksw = state->audblk.blksw;

  if (before_downmix) {
    nChannel = state->bsi.nfchans;
    if ((state->bsi.lfeon) && (state->outlfeon)) {
      ippsMDCTInv_32f(state->coeffs[5],
                      state->samples[5],
                      state->allocation_imdct.pMDCTSpecLong,
                      state->allocation_imdct.pBufferLong);
    }
  } else {
    nChannel = state->nChannelOut;
    if ((state->bsi.lfeon) && (state->outlfeon)) {
      ippsMDCTInv_32f(state->temp[nChannel-1],
                      state->samples[nChannel-1],
                      state->allocation_imdct.pMDCTSpecLong,
                      state->allocation_imdct.pBufferLong);
      state->temp[nChannel-1] = state->samples[nChannel-1];
    }
    nChannel -= state->outlfeon;
  }

  for (ch = 0; ch < nChannel; ch++) {
    Ipp32f *inPtr;
    Ipp32s test;

    if (before_downmix) {
      inPtr = state->coeffs[ch];
      test = (blksw >> (state->bsi.nfchans - ch - 1)) & 1;
    } else {
      inPtr = state->temp[ch];
      test = blksw & 1;
    }

    if (test) {
      Ipp32f tmp_buf[64];
      Ipp32s i;

      ippsDeinterleave_32f(inPtr, 2, 128,
                           state->ShortBuff);

      ippsMDCTInv_32f(state->ShortBuff[0],
                      state->samples[ch],
                      state->allocation_imdct.pMDCTSpecShort,
                      state->allocation_imdct.pBufferShort);

      ippsMDCTInv_32f(state->ShortBuff[1],
                      &(state->samples[ch][256]),
                      state->allocation_imdct.pMDCTSpecShort,
                      state->allocation_imdct.pBufferShort);

      for (i = 192; i < 256; i++) {
        tmp_buf[i - 192] =
          -0.5f * state->samples[ch][i];
      }

      for (i = 63; i >= 0; i--) {
        state->samples[ch][i + 64] =
          0.5f * state->samples[ch][i];
      }

      for (i = 0; i < 64; i++) {
        state->samples[ch][i] =
        tmp_buf[i];
      }

      for (i = 256; i < 256+128; i++) {
        state->samples[ch][i] =
          0.5f * state->samples[ch][i + 64];
      }
    } else {
      ippsMDCTInv_32f(inPtr,
                      state->samples[ch],
                      state->allocation_imdct.pMDCTSpecLong,
                      state->allocation_imdct.pBufferLong);
    }
    state->temp[ch] = state->samples[ch];
  }
}

/********************************************************************/

void WindowingOverlap(Ipp16s* pOut,
                      AC3Dec *state)
{
  Ipp32s i = 0;
  Ipp32f *pDataTemp[6];

  if ((!state->bsi.lfeon) && (state->outlfeon)) {
    ippsZero_32f(state->temp[state->nChannelOut - 1], 128);
    ippsZero_32f(state->temp[state->nChannelOut - 1]+256, 128);
  }

  for (i = 0; i < state->nChannelOut; i++) {
    ippsFlip_32f(state->temp[i], state->temp[i]+128, 128);
    ippsFlip_32f(state->temp[i]+256, state->temp[i]+128+256, 128);

    ippsAddProduct_32f(WindowTable, state->temp[i],
      state->delay[i], 256);

    pDataTemp[i] = state->delay[i];
  }

  if (state->gainScale != 1) {
    for (i = 0; i < state->nChannelOut; i++) {
      ippsMulC_32f_I(state->gainScale, state->delay[i], 256);
    }
  }

  ippsJoin_32f16s_D2L((const Ipp32f **)pDataTemp, state->nChannelOut,
                       256, (Ipp16s*)pOut);

  for (i = 0; i < state->nChannelOut; i++) {
    ippsMul_32f(WindowTable + 256, state->temp[i] + 256, state->delay[i], 256);
  }
}

/********************************************************************/

#endif //UMC_ENABLE_AC3_AUDIO_DECODER


