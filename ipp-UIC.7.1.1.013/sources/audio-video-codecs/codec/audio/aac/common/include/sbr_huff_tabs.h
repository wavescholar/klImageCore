/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SBR_HUFF_TABS_H__
#define __SBR_HUFF_TABS_H__

#include "ippdc.h"
#include "ippac.h"

#define NUM_SBR_HUFF_TABS 10

#define HUFF_NOISE_COMPRESS 0

#define HUFF_ENV_COMPRESS   1

#ifdef  __cplusplus
extern "C" {
#endif

/* SBR */
extern Ipp32s vlcSbrTableSizes[];
extern Ipp32s vlcSbrNumSubTables[];
extern Ipp32s *vlcSbrSubTablesSizes[];
extern IppsVLCTable_32s *vlcSbrBooks[];

/* PS */
extern Ipp32s vlcPsTableSizes[];
extern Ipp32s vlcPsNumSubTables[];
extern Ipp32s *vlcPsSubTablesSizes[];
extern IppsVLCTable_32s *vlcPsBooks[];
IppStatus ownInitSBREncHuffTabs(IppsVLCEncodeSpec_32s** ppSpec, Ipp32s *sizeAll);

Ipp32s ownVLCCountBits_16s32s(Ipp16s delta, IppsVLCEncodeSpec_32s* pHuffTab);

Ipp32s sbrencSetEnvHuffTabs(Ipp32s  bs_amp_res,
                            Ipp16s* LAV,
                            Ipp32s* bs_env_start_bits,

                            IppsVLCEncodeSpec_32s** pTimeHuffTab,
                            IppsVLCEncodeSpec_32s** pFreqHuffTab,
                            IppsVLCEncodeSpec_32s*  sbrHuffTabs[NUM_SBR_HUFF_TABS],

                            Ipp32s  typeCompress); /* [1] - envelope, [0] - noise */

#ifdef  __cplusplus
}
#endif

#endif/*__SBR_HUFF_TABS_H__ */
