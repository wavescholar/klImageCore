/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __SEARCH_H
#define __SEARCH_H

#include "aac_enc_own.h"

#define  MAX_NON_ESC_VALUE  12

#ifdef  __cplusplus
extern "C" {
#endif

Ipp32s best_codebooks_search(sEnc_individual_channel_stream* pStream,
                             Ipp16s *px_quant_unsigned,
                             Ipp16s *px_quant_signed,
                             Ipp16s *px_quant_unsigned_pred,
                             Ipp16s *px_quant_signed_pred);

void tree_build(Ipp32s sfb_bit_len[MAX_SFB][12],
                Ipp32s cb_trace[MAX_SFB][12],
                Ipp32s max_sfb,
                Ipp32s len_esc_value);
#if 1
void bit_count(sEnc_individual_channel_stream* pStream,
               Ipp16s *px_quant_unsigned,
               Ipp16s *px_quant_signed,
               Ipp32s *sfb_offset,
               Ipp32s sfb_bit_len[MAX_SFB][12]);
#endif
#ifdef  __cplusplus
}
#endif

#endif//__SEARCH_H
