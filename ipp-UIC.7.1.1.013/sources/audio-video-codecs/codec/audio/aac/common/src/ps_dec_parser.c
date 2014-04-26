/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include "aac_status.h"
/* parametric stereo */
#include "ps_dec_struct.h"
#include "sbr_huff_tabs.h"
#include "ps_dec_parser.h"
/* debug */
#include "vm_debug.h"

#include "ipps.h"

/* ************************************************************************** */

#define OWN_MIN_MAX(indx, minIndx, maxIndx) IPP_MIN( IPP_MAX(indx, minIndx), maxIndx)

#define MOD_IDX (8)

/* ************************************************************************** */

void ps_header_fill_default( sPSDecComState* pPSItem )
{
  pPSItem->bs_enable_iid = 0;
  pPSItem->bs_iid_mode   = 0;
  pPSItem->bs_enable_icc = 0;
  pPSItem->bs_icc_mode   = 0;
  pPSItem->bs_enable_ext = 0;
  pPSItem->bs_enable_ipdopd = 0;

  pPSItem->flag_HAconfig     = CONFIG_HA_BAND1020;
  pPSItem->flag_HAconfigPrev = pPSItem->flag_HAconfig;


  return;
}

/* ************************************************************************** */

#define EXIT_ERR_PS_PARSER             \
{                                      \
     ps_header_fill_default( pPSDec ); \
     return PS_ERR_PARSER;             \
}

/* ************************************************************************** */

static Ipp16s ps_huff_dec(void *t_huff, sBitsreamBuffer * pBS)
{
  Ipp16s index = 0;
  Ipp8u  *pSrc;
  Ipp32s bitoffset;

  pSrc = (Ipp8u *)pBS->pCurrent_dword + ((32 - pBS->nBit_offset) >> 3);
  bitoffset = (32 - pBS->nBit_offset) & 0x7;

  ippsVLCDecodeOne_1u16s(&pSrc, &bitoffset, &index,
    (IppsVLCDecodeSpec_32s *) t_huff);

  pBS->pCurrent_dword = (Ipp32u *)(pSrc - ((size_t)(pSrc) & 3));
  pBS->dword = BSWAP(pBS->pCurrent_dword[0]);
  pBS->nBit_offset =
    (Ipp32s)(32 - ((pSrc - (Ipp8u *)pBS->pCurrent_dword) << 3) - bitoffset);

  return index;
}

/* ************************************************************************** */

static
Ipp32s ps_extension_parser(sBitsreamBuffer* pBS,
                           sPSDecComState*  pPSDec,
                           Ipp32s ps_ext_id,
                           Ipp32s num_bits_left)
{
  Ipp32s tabNumIpdOpdBands[] = { 5, 11, 17,  5, 11, 17 };
  void*  pHuffTab = NULL;

  Ipp32s cnt = 0, bytes = 0, empty_bits = 0, env = 0;
  Ipp32s b = 0, nIpdOpdBands = 0;
  Ipp32s reserved_ps = 0, indx = 0;

  switch( ps_ext_id ){
    case PS_EXTENSION_VER0:

      GET_BITS(pBS, pPSDec->bs_enable_ipdopd, 1, Ipp32s);

      if( pPSDec->bs_enable_ipdopd ){

        nIpdOpdBands = tabNumIpdOpdBands[pPSDec->bs_iid_mode];

        for( env = 0; env < pPSDec->nEnv; env++){

          GET_BITS(pBS, pPSDec->bs_ipd_dt[env], 1, Ipp32s);
          // ipd_data
          indx     = ( pPSDec->bs_ipd_dt[env] ) ? 7 : 6;
          pHuffTab = pPSDec->psHuffTables[ indx ];

          for (b = 0; b < nIpdOpdBands; b++) {
            pPSDec->indxIpd[env][b] = ps_huff_dec(pHuffTab, pBS);
          }

          GET_BITS(pBS, pPSDec->bs_opd_dt[env], 1, Ipp32s);
          // opd_data
          indx = ( pPSDec->bs_opd_dt[env] ) ? 9 : 8;
          pHuffTab = pPSDec->psHuffTables[ indx ];

          for (b = 0; b < nIpdOpdBands; b++) {
            pPSDec->indxOpd[env][b] = ps_huff_dec(pHuffTab, pBS);
          }
        }// for (env = 0; env ...
      }// if( pPSDec->bs_enable_ipdopd )

      GET_BITS(pBS, reserved_ps, 1, Ipp32s);

      break;

    default: // version of PS extension > V0. doesn't support
      cnt = num_bits_left >> 3; /* bits2bytes */
      for (bytes=0; bytes<cnt; bytes++){
        GET_BITS(pBS, empty_bits, 8, Ipp32s);
      }
      break;
  }

  pPSDec->nIpdOpdBands = nIpdOpdBands;

  return PS_NO_ERR;
}

/* ************************************************************************** */

static Ipp32s ownInterpolation(Ipp32s* pSrcDst, Ipp32s len)
{
  Ipp32s i = 0;

  len <<= 1;
  for (i = len-1; i > 0; i--) {
    pSrcDst[i] = pSrcDst[i >> 1];
  }

  return 0;//OK
}

/* ************************************************************************** */

static Ipp32s
ownDeltaDecode(  Ipp32s *pIndxCur,
                 Ipp32s *pIndxPrev,

                 Ipp32s flag,
                 Ipp32s dtdf,
                 Ipp32s nBands,

                 Ipp32s offset,
                 Ipp32s minIndx,
                 Ipp32s maxIndx)
{
  Ipp32s band = 0, startBand = 0;
  Ipp32s* pIndx1 = NULL;

  /* [1] passive delta decode */
  if( !flag ){
    ippsZero_32s(pIndxCur, nBands*offset);
    return 0;//OK
  }

  /* [2] active delta decode */
  /* tuning */
  pIndx1    = (dtdf) ? pIndxPrev : pIndxCur;
  startBand = (dtdf) ? 0 : 1;

  for (band = startBand; band < nBands; band++) {
    pIndxCur[band] += pIndx1[ dtdf ? band*offset : band-1];
  }

  /* patch */
  for(band = 0; band < nBands; band++){
    pIndxCur[band] = OWN_MIN_MAX(pIndxCur[band],minIndx,maxIndx);
  }

  /* [4] interpolation */
  if (2 == offset) {
    ownInterpolation(pIndxCur, nBands);
  }

  return 0;//OK
}

/* ************************************************************************** */

static Ipp32s
ownDeltaDecodeMod(Ipp32s *pIndxCur,
                  Ipp32s *pIndxPrev,
                  Ipp32s flag,
                  Ipp32s dtdf,

                  Ipp32s nBands,
                  Ipp32s offset)
{
  Ipp32s band = 0, startBand = 0;
  Ipp32s* pIndx1 = NULL;

  /* passive delta decode mod */
  if( !flag ){
    ippsZero_32s(pIndxCur, nBands * offset);
    return 0;
  }

  /* tuning */
  pIndx1      = (dtdf) ? pIndxPrev : pIndxCur;
  startBand   = (dtdf) ? 0 : 1;
  pIndxCur[0] = (dtdf) ? pIndxCur[0] : pIndxCur[0] % MOD_IDX;

  /* decode */
  for( band = startBand; band < nBands; band++ ){
     pIndxCur[band] += pIndx1[ dtdf ? band*offset : band-1];
     pIndxCur[band] %= MOD_IDX;
  }

  /* [4] interpolation */
  if (2 == offset) {
    ownInterpolation(pIndxCur, nBands);
  }

  return 0;//OK
}

/* ************************************************************************** */

static Ipp32s map34To20Idx (Ipp32s *pIndx, Ipp32s nBands)
{
  pIndx[0]  = (2*pIndx[0]+pIndx[1])/3;
  pIndx[1]  = (pIndx[1]+2*pIndx[2])/3;
  pIndx[2]  = (2*pIndx[3]+pIndx[4])/3;
  pIndx[3]  = (pIndx[4]+2*pIndx[5])/3;
  pIndx[4]  = (pIndx[6]+pIndx[7]) >> 1;
  pIndx[5]  = (pIndx[8]+pIndx[9]) >> 1;
  pIndx[6]  = pIndx[10];
  pIndx[7]  = pIndx[11];
  pIndx[8]  = (pIndx[12]+pIndx[13]) >> 1;
  pIndx[9]  = (pIndx[14]+pIndx[15]) >> 1;
  pIndx[10] = pIndx[16];

  if( 17 == nBands ) return 0;//OK

  pIndx[11] = pIndx[17];
  pIndx[12] = pIndx[18];
  pIndx[13] = pIndx[19];
  pIndx[14] = (pIndx[20]+pIndx[21])>>1;
  pIndx[15] = (pIndx[22]+pIndx[23])>>1;
  pIndx[16] = (pIndx[24]+pIndx[25])>>1;
  pIndx[17] = (pIndx[26]+pIndx[27])>>1;
  pIndx[18] = (pIndx[28]+pIndx[29]+pIndx[30]+pIndx[31])>>2;
  pIndx[19] = (pIndx[32]+pIndx[33])>>1;


  return 0;//OK
}

/* ************************************************************************** */

static Ipp32s map20To34Idx (Ipp32s *pIndx, Ipp32s nBands)
{
  int bufT[34];

  bufT[0] = pIndx[0];
  bufT[1] = (pIndx[0] + pIndx[1]) >> 1;
  bufT[2] = pIndx[1];
  bufT[3] = pIndx[2];
  bufT[4] = (pIndx[2] + pIndx[3]) >> 1;
  bufT[5] = pIndx[3];
  bufT[6] = pIndx[4];
  bufT[7] = pIndx[4];
  bufT[8] = pIndx[5];
  bufT[9] = pIndx[5];
  bufT[10] = pIndx[6];
  bufT[11] = pIndx[7];
  bufT[12] = pIndx[8];
  bufT[13] = pIndx[8];
  bufT[14] = pIndx[9];
  bufT[15] = pIndx[9];
  bufT[16] = pIndx[10];

  if( 17 == nBands ) return 0;//OK

  bufT[17] = pIndx[11];
  bufT[18] = pIndx[12];
  bufT[19] = pIndx[13];
  bufT[20] = pIndx[14];
  bufT[21] = pIndx[14];
  bufT[22] = pIndx[15];
  bufT[23] = pIndx[15];
  bufT[24] = pIndx[16];
  bufT[25] = pIndx[16];
  bufT[26] = pIndx[17];
  bufT[27] = pIndx[17];
  bufT[28] = pIndx[18];
  bufT[29] = pIndx[18];
  bufT[30] = pIndx[18];
  bufT[31] = pIndx[18];
  bufT[32] = pIndx[19];
  bufT[33] = pIndx[19];

  ippsCopy_32s(bufT, pIndx, nBands);

  return 0;//OK
}

/* ************************************************************************** */

static Ipp32s ownIndxUpdate(Ipp32s* pSrc, Ipp32s* pDst, Ipp32s len, Ipp32s flag)
{

  if( flag ){
    ippsCopy_32s(pSrc, pDst, len);
  } else {
    ippsZero_32s(pDst, len);
  }

  return 0;//OK
}

/* ************************************************************************** */

static Ipp32s ps_delta_decode_phase_params( sPSDecComState* pState )
{
  Ipp32s env;
  Ipp32s nGridSteps = pState->iid_quant ? 15 : 7;

  /* [1] delta decoding */
  for( env = 0; env < pState->nEnv; env++ ){
    Ipp32s *pIndxIidPrev = NULL;
    Ipp32s *pIndxIccPrev = NULL;
    Ipp32s *pIndxIpdPrev = NULL;
    Ipp32s *pIndxOpdPrev = NULL;
    Ipp32s* pIndxCur     = NULL;
    Ipp32s* pIndxPrev    = NULL;
    Ipp32s param, flag, dfdt, nBands, offset, minIndx, maxIndx;

    if( 0 == env ){
      pIndxIidPrev = pState->indxIidPrev;
      pIndxIccPrev = pState->indxIccPrev;
      pIndxIpdPrev = pState->indxIpdPrev;
      pIndxOpdPrev = pState->indxOpdPrev;
    } else {
      pIndxIidPrev = pState->indxIid[env-1];
      pIndxIccPrev = pState->indxIcc[env-1];
      pIndxIpdPrev = pState->indxIpd[env-1];
      pIndxOpdPrev = pState->indxOpd[env-1];
    }

    /* first tuning */
    flag = pState->bs_enable_iid;
    dfdt = pState->bs_iid_dt[env];
    nBands = pState->nIidBands;
    offset = (pState->freq_res_iid) ? 1 : 2;
    minIndx= -nGridSteps;
    maxIndx=  nGridSteps;
    pIndxCur = pState->indxIid[env];
    pIndxPrev= pIndxIidPrev;

    for(param = 0; param < 2; param++){
      ownDeltaDecode(pIndxCur, pIndxPrev, flag, dfdt, nBands, offset, minIndx, maxIndx);

      /* second tuning */
      flag = pState->bs_enable_icc;
      dfdt = pState->bs_icc_dt[env];
      nBands = pState->nIccBands;
      offset = (pState->freq_res_icc) ? 1 : 2;
      minIndx= 0;
      maxIndx= 7;
      pIndxCur = pState->indxIcc[env];
      pIndxPrev= pIndxIccPrev;
    }

    /* third tuning */
    flag = pState->bs_enable_ipdopd;
    dfdt = pState->bs_ipd_dt[env];
    nBands = pState->nIpdOpdBands; //tabNumIpdOpdBands[ pState->freq_res_ipd ];
    offset = (pState->freq_res_ipd) ? 1 : 2;
    pIndxCur = pState->indxIpd[env];
    pIndxPrev= pIndxIpdPrev;

    for(param = 0; param < 2; param++){
      ownDeltaDecodeMod(pIndxCur, pIndxPrev, flag, dfdt, nBands, offset);
      /* fourth tuning */
      dfdt = pState->bs_opd_dt[env];
      pIndxCur = pState->indxOpd[env];
      pIndxPrev= pIndxOpdPrev;
    }
  } // end for(env = 0;

  /* [2] case nEnv == 0, we use info from prev frame */
  if( 0 == pState->nEnv ){
    /* IID */
    ownIndxUpdate(pState->indxIidPrev, pState->indxIid[0], 34, pState->bs_enable_iid);
    /* ICC */
    ownIndxUpdate(pState->indxIccPrev, pState->indxIcc[0], 34, pState->bs_enable_icc);
    /* IPD */
    ownIndxUpdate(pState->indxIpdPrev, pState->indxIpd[0], 17, pState->bs_enable_ipdopd);
    /* OPD */
    ownIndxUpdate(pState->indxOpdPrev, pState->indxOpd[0], 17, pState->bs_enable_ipdopd);

    pState->nEnv++;
  }

  /* patch */
#if 1
  if( 0 == pState->bs_frame_class ){
    pState->bufBordPos[0] = 0;
    pState->bufBordPos[ pState->nEnv ] = 32;
    for( env = 1; env < pState->nEnv; env++ ){
      pState->bufBordPos[env] = env * 32 / pState->nEnv;
    }
  }
#endif

  /* [5] - borders. class FIX or VAR */
  if( pState->bs_frame_class ){

    /* dublicating */
    if( pState->bufBordPos[pState->nEnv] < 32 ){
      /* IID */
      ippsCopy_32s(pState->indxIid[pState->nEnv - 1], pState->indxIid[pState->nEnv], 34);
      /* ICC */
      ippsCopy_32s(pState->indxIcc[pState->nEnv - 1], pState->indxIcc[pState->nEnv], 34);
      /* IPD */
      ippsCopy_32s(pState->indxIpd[pState->nEnv - 1], pState->indxIpd[pState->nEnv], 17);
      /* OPD */
      ippsCopy_32s(pState->indxOpd[pState->nEnv - 1], pState->indxOpd[pState->nEnv], 17);

      /* update */
      pState->nEnv++;
      pState->bufBordPos[pState->nEnv] = 32;
    }

    /* patch, from ref code */
    for( env = 1; env < pState->nEnv; env++ ){
      Ipp32s thres = 0;

      thres = 32 - ( pState->nEnv - env );
      if( pState->bufBordPos[env] > thres ){
        pState->bufBordPos[env] = thres;
      } else {
        thres = pState->bufBordPos[env - 1] + 1;
        if( pState->bufBordPos[env] < thres ){
          pState->bufBordPos[env] = thres;
        }
      }// if()
    }
  }// if( class )

  /* [6] preprocessing mapping */
  for( env = 0; env < pState->nEnv; env++ ){
    /* IID */
    ippsCopy_32s(pState->indxIid[env], pState->indxIidMapped[env], 34);
    /* ICC */
    ippsCopy_32s(pState->indxIcc[env], pState->indxIccMapped[env], 34);
    /* IPD */
    ippsCopy_32s(pState->indxIpd[env], pState->indxIpdMapped[env], 17);
    /* OPD */
    ippsCopy_32s(pState->indxOpd[env], pState->indxOpdMapped[env], 17);
  }

  /* [7] */
  if ( PS_ENABLE_BL == pState->modePS ){
    for( env = 0; env < pState->nEnv; env++ ){
      if( 2 == pState->freq_res_iid ){
        map34To20Idx( pState->indxIidMapped[env], 34 );
      }
      if( 2 == pState->freq_res_icc ){
        map34To20Idx( pState->indxIccMapped[env], 34 );
      }
      /* IPD - DISABLE */
      ippsZero_32s(pState->indxIpdMapped[env], 17);
      /* OPD - DISABLE */
      ippsZero_32s(pState->indxOpdMapped[env], 17);
    }
  } else if ( CONFIG_HA_BAND34 == pState->flag_HAconfig ){
    for( env = 0; env < pState->nEnv; env++ ){
      if( 2 > pState->freq_res_iid ){
        map20To34Idx( pState->indxIidMapped[env], 34 );
      }
      if( 2 > pState->freq_res_icc ){
        map20To34Idx( pState->indxIccMapped[env], 34 );
      }
      if( 2 > pState->freq_res_ipd ){
        map20To34Idx(pState->indxIpdMapped[env], 17);
        map20To34Idx(pState->indxOpdMapped[env], 17);
      }
    }
  }//f ( PS_ENABLE_BL == pState->modePS ){

  /* [8] active update of previous data */
  /* IID */
  ippsCopy_32s(pState->indxIid[pState->nEnv - 1], pState->indxIidPrev, 34);
  /* ICC */
  ippsCopy_32s(pState->indxIcc[pState->nEnv - 1], pState->indxIccPrev, 34);
  /* IPD */
  ippsCopy_32s(pState->indxIpd[pState->nEnv - 1], pState->indxIpdPrev, 17);
  /* OPD */
  ippsCopy_32s(pState->indxOpd[pState->nEnv - 1], pState->indxOpdPrev, 17);


  return 0;//OK
}

/* ************************************************************************** */

Ipp32s ps_dec_parser( sBitsreamBuffer * pBS, sPSDecComState* pPSDec)
{
  /* tables */
  Ipp32s tabNumEnv[2][4] = { { 0, 1, 2, 4 }, { 1, 2, 3, 4 } };
  Ipp32s tabNumIidBands[]= { 10, 20, 34, 10, 20, 34 };
  Ipp32s tabNumIccBands[]= { 10, 20, 34, 10, 20, 34, 0, 0}; // 2 last elements - reserved

  Ipp32s nIidBands = 0, nIccBands = 0;

  Ipp32s nEnv = 0, env = 0, b = 0;
  Ipp32s iid_quant  = 0;// default

  Ipp32s cnt = 0, esc_cnt = 0;
  Ipp32s num_bits_left = 0;
  Ipp32s ps_ext_id  = 0;
  Ipp32s empty_bits = 0;
  Ipp32s decodeBitsStart = 0, decodeBitsStop = 0, error = 0;

  void*   pHuffTab = NULL;
  Ipp32s  indx = 0;

  /* header */
  GET_BITS(pBS, pPSDec->bs_enable_ps_header, 1, Ipp32s);
  if( pPSDec->bs_enable_ps_header ){

    GET_BITS(pBS, pPSDec->bs_enable_iid, 1, Ipp32s);
    if(pPSDec->bs_enable_iid){
      GET_BITS(pBS, pPSDec->bs_iid_mode, 3, Ipp32s);
    }
    /* protection: iid_mode = [0, 5] */
    if( pPSDec->bs_iid_mode < 0 || pPSDec->bs_iid_mode > 5 ){
      EXIT_ERR_PS_PARSER
    }

    GET_BITS(pBS, pPSDec->bs_enable_icc, 1, Ipp32s);
    if( pPSDec->bs_enable_icc ){
      GET_BITS(pBS, pPSDec->bs_icc_mode, 3, Ipp32s);
    }
    /* protection: icc_mode = [0, 5] */
    if( pPSDec->bs_icc_mode < 0 || pPSDec->bs_icc_mode > 5 ){
      EXIT_ERR_PS_PARSER
    }

    GET_BITS(pBS, pPSDec->bs_enable_ext, 1, Ipp32s);
  }

  /* frame info */
  GET_BITS(pBS, pPSDec->bs_frame_class, 1, Ipp32s);
  GET_BITS(pBS, pPSDec->bs_num_env_idx, 2, Ipp32s);

  nEnv = tabNumEnv[pPSDec->bs_frame_class][pPSDec->bs_num_env_idx];
  pPSDec->nEnv = nEnv;

  if( pPSDec->bs_frame_class ){
    pPSDec->bufBordPos[0] = 0;
    for(env = 0; env < nEnv; env++){
      GET_BITS(pBS, pPSDec->bufBordPos[env + 1], 5, Ipp32s);
      pPSDec->bufBordPos[env + 1] += 1;
    }
  } else {
    for(env = 0; env < nEnv; env++){
      pPSDec->bufBordPos[ nEnv ] = 32 / nEnv * env;
    }
  }
  pPSDec->bufBordPos[ nEnv ] = 32;

  /* correct selection of quant */
  iid_quant    = (pPSDec->bs_iid_mode < 3) ? 0 : 1;
  nIidBands    = tabNumIidBands[pPSDec->bs_iid_mode];
  nIccBands    = tabNumIccBands[pPSDec->bs_icc_mode];

  /* patch, tuning freq_resolution */
  if (pPSDec->bs_iid_mode > 2){
    pPSDec->freq_res_iid = pPSDec->bs_iid_mode - 3;
  }else{
    pPSDec->freq_res_iid = pPSDec->bs_iid_mode;
  }

  /* the same result */
  pPSDec->freq_res_ipd =  pPSDec->freq_res_iid;

  if (pPSDec->bs_icc_mode > 2){
    pPSDec->freq_res_icc = pPSDec->bs_icc_mode-3;
    pPSDec->mix_strategy = PS_MIX_RB; //Rb
  }else{
    pPSDec->freq_res_icc = pPSDec->bs_icc_mode;
    pPSDec->mix_strategy = PS_MIX_RA; //Ra
  }

  /* select correct HA config (not clear from ISO spec) */
  // flag of HA has been changed if IID or ICC has been sent
  if( pPSDec->bs_enable_iid || pPSDec->bs_enable_icc ){
    pPSDec->flag_HAconfig  = (pPSDec->bs_enable_iid && (34 == nIidBands));
    pPSDec->flag_HAconfig |= (pPSDec->bs_enable_icc && (34 == nIccBands));
    pPSDec->flag_HAconfig  = (pPSDec->flag_HAconfig) ? CONFIG_HA_BAND34 : CONFIG_HA_BAND1020;
  }

  /* IID_DATA */
  if( pPSDec->bs_enable_iid ){
    for( env = 0; env < nEnv; env++ ){
      GET_BITS(pBS, pPSDec->bs_iid_dt[env], 1, Ipp32s);

      indx = ( pPSDec->bs_iid_dt[env] ) ? 1 : 0;
      indx = ( iid_quant ) ? indx : indx + 2;
      pHuffTab = pPSDec->psHuffTables[indx];

      for (b = 0; b < nIidBands; b++){
        pPSDec->indxIid[env][b] = ps_huff_dec(pHuffTab, pBS);
      }
    }
  }

  /* ICC_DATA */
  if( pPSDec->bs_enable_icc ){
    for( env = 0; env < nEnv; env++ ){
      GET_BITS(pBS, pPSDec->bs_icc_dt[env], 1, Ipp32s);

      indx = ( pPSDec->bs_icc_dt[env] ) ? 5 : 4;
      pHuffTab = pPSDec->psHuffTables[indx];

      for( b = 0; b < nIccBands; b++){
        pPSDec->indxIcc[env][b] = ps_huff_dec(pHuffTab, pBS);
      }
    }
  }

  /* extension */
  if(pPSDec->bs_enable_ext){
    GET_BITS(pBS, cnt, 4, Ipp32s); // ps_extension_size
    if( 15 == cnt ){
      GET_BITS(pBS, esc_cnt, 8, Ipp32s);
      cnt += esc_cnt;
    }

    num_bits_left = 8*cnt;
    while ( num_bits_left > 7 ) {
      GET_BITS(pBS, ps_ext_id, 2, Ipp32s);
      num_bits_left -= 2;

      GET_BITS_COUNT(pBS, decodeBitsStart);
      error = ps_extension_parser(pBS, pPSDec, ps_ext_id, num_bits_left);

      if( error ) return PS_ERR_PARSER;

      GET_BITS_COUNT(pBS, decodeBitsStop);

      num_bits_left -= (decodeBitsStop - decodeBitsStart);
    }

    // fill elements
    if( num_bits_left < 0 ) return PS_ERR_PARSER;

    /* read fill bits for byte alignment */
    GET_BITS(pBS, empty_bits, num_bits_left, Ipp32s);
  }

  /* save params */
  pPSDec->iid_quant = iid_quant;
  pPSDec->nIidBands = nIidBands;
  pPSDec->nIccBands = nIccBands;

  /* delta decode of phase params */
  ps_delta_decode_phase_params( pPSDec );

  /* tuning mode of PSDec */
  if( PS_ENABLE_BL == pPSDec->modePS ){
    pPSDec->mix_strategy  = PS_MIX_RA;// Ra
    pPSDec->flag_HAconfig = CONFIG_HA_BAND1020;
  } //else if UNRESTRICTED

  return 0;
}

/* EOF */

#endif //UMC_ENABLE_XXX

