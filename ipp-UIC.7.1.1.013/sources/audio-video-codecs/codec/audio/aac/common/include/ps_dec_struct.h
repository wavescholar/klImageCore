/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __PS_DEC_STRUCT_H__
#define __PS_DEC_STRUCT_H__

#include "ippdefs.h"
#include "ippac.h"
/* PARAMETRIC STEREO */
#include "ps_dec_settings.h"

typedef struct{

  Ipp32s  nGroup;
  Ipp32s  nBin;
  Ipp32s  decay_cutoff;
  Ipp32s  thres;

  /* delay */
  Ipp32s firstDelayGroup;
  Ipp32s firstDelaySb;

  /* tables */
  Ipp32s* pStartBordTab;
  Ipp32s* pStopBordTab;
  Ipp32s* pBinTab;
  Ipp32s* pPostBinTab;

} sTDSpec;

typedef struct{

  /* header */
  Ipp32s bs_enable_ps_header;
  Ipp32s bs_enable_iid;
  Ipp32s bs_iid_mode;
  Ipp32s bs_enable_icc;
  Ipp32s bs_icc_mode;
  Ipp32s bs_enable_ext;

  Ipp32s bs_enable_ipdopd;

  /* bs_frame_class */
  Ipp32s bs_frame_class;
  Ipp32s bs_num_env_idx;

  /* num bands */
  Ipp32s nEnv;
  Ipp32s nIidBands;
  Ipp32s nIccBands;
  Ipp32s nIpdOpdBands;
  Ipp32s iid_quant;

  /* freq resolution */
  Ipp32s freq_res_iid;
  Ipp32s freq_res_ipd;
  Ipp32s freq_res_icc;

  /* hybrid analysis configuration */
  Ipp32s flag_HAconfig;
  Ipp32s flag_HAconfigPrev;

  /* mixing strategy = [0, 1] = [Ra, Rb] */
  Ipp32s mix_strategy;

  /* decorrelated */
  Ipp32s delayLenIndx;

  Ipp32s bufBordPos[MAX_PS_ENV];

  /* flags of PS parameters */
  Ipp32s bs_iid_dt[MAX_PS_ENV];
  Ipp32s bs_icc_dt[MAX_PS_ENV];
  Ipp32s bs_ipd_dt[ MAX_PS_ENV ];
  Ipp32s bs_opd_dt[ MAX_PS_ENV ];

  /* mapped */
  Ipp32s indxIpdMapped[5][17];
  Ipp32s indxIpdMapped_1[17];
  Ipp32s indxIpdMapped_2[17];

  Ipp32s indxOpdMapped[5][17];
  Ipp32s indxOpdMapped_1[17];
  Ipp32s indxOpdMapped_2[17];

  Ipp32s indxIidMapped[5][34];
  Ipp32s indxIccMapped[5][34];

  /* transient detection [0, 1] = [20, 34] band */
  sTDSpec tdSpec[2];

  /* huffman diff index */
  Ipp32s indxIid[MAX_PS_ENV][34];
  Ipp32s indxIcc[MAX_PS_ENV][34];
  Ipp32s indxIpd[MAX_PS_ENV][17];
  Ipp32s indxOpd[MAX_PS_ENV][17];

  Ipp32s indxIidPrev[34];
  Ipp32s indxIccPrev[34];
  Ipp32s indxIpdPrev[17];
  Ipp32s indxOpdPrev[17];

  /* huffman */
  IppsVLCDecodeSpec_32s* psHuffTables[10];

  /* decode mode */
  Ipp32s modePS;

} sPSDecComState;

#endif             /* __PS_DEC_STRUCT_H__ */


