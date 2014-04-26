/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SBR_DEC_TABS_FP_H__
#define __SBR_DEC_TABS_FP_H__

/* SBR */
#include "sbr_huff_tabs.h"
/* PS */
#include "ps_dec_struct.h"

/********************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

  /********************************************************************/
  /*                       SPECTRAL BAND REPLICATION                  */
  /********************************************************************/
/* QMF WINDOW */
extern Ipp32f  SBR_TABLE_QMF_WINDOW_640[];
extern Ipp32f  SBR_TABLE_QMF_WINDOW_320[];
/* TABLE is used by HFadjustment */
extern Ipp32f  SBR_TABLE_FI_NOISE_RE[];
extern Ipp32f  SBR_TABLE_FI_NOISE_IM[];
extern Ipp32f* SBR_TABLE_V[2];

  /********************************************************************/
  /*                   PARAMETRIC STEREO PART                         */
  /********************************************************************/
extern const Ipp32s tabResBand1020[];
extern const Ipp32s tabResBand34[];

extern const Ipp32f tabQuantIidStd[];
extern const Ipp32f tabQuantIidFine[];
extern const Ipp32f tabScaleIidStd[];
extern const Ipp32f tabScaleIidFine[];

extern const Ipp32f tabQuantRHO[];

extern const Ipp32f* pCoefTabs[13];

/********************************************************************/
// configuration set [band1020, band24]

extern const sTDSpec setConfBand20;
extern const sTDSpec setConfBand34;

#ifdef  __cplusplus
}
#endif

/********************************************************************/

#endif/*__SBR_DEC_TABS_FP_H__ */
