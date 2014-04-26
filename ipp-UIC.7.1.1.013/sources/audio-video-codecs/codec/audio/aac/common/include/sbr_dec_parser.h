/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SBR_DEC_PARSER_H__
#define __SBR_DEC_PARSER_H__

#include "sbr_dec_struct.h"
#include "bstream.h"

#ifdef  __cplusplus
extern  "C" {
#endif

  Ipp32s  sbr_grid(sBitsreamBuffer * BS, Ipp32s* bs_frame_class, Ipp32s* bs_pointer, Ipp32s* freqRes,
                   Ipp32s* bordersEnv, Ipp32s* bordersNoise, Ipp32s* nEnv, Ipp32s* nNoiseEnv, Ipp32s* status);

  void    sbr_grid_coupling(sSBRDecComState * pSbr);

  Ipp32s  sbr_dtdf(sBitsreamBuffer * BS, Ipp32s* bs_df_env, Ipp32s* bs_df_noise, Ipp32s nEnv, Ipp32s nNoiseEnv);

  Ipp32s  sbr_invf(Ipp32s ch, sBitsreamBuffer * BS, sSBRDecComState * pSbr);

  Ipp32s  sbr_envelope(Ipp32s ch, Ipp32s bs_coupling, Ipp32s bs_amp_res,
                       sBitsreamBuffer * BS, sSBRDecComState * pDst);

  Ipp32s  sbr_noise(sBitsreamBuffer* BS, Ipp16s* vNoise, Ipp32s* vSize, Ipp32s* bs_df_noise,
                    void* sbrHuffTables[10], Ipp32s ch, Ipp32s bs_coupling, Ipp32s nNoiseEnv, Ipp32s NQ);

  Ipp32s  sbr_sinusoidal_coding(sBitsreamBuffer * BS, Ipp32s* pDst, Ipp32s len);

  Ipp32s  sbrEnvNoiseDec(sSBRDecComState * pSbr, Ipp32s ch);

#ifdef  __cplusplus
}
#endif

#endif/*__SBR_DEC_PARSER_H__ */
