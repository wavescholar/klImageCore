/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AACCMN_HUFF_H
#define __AACCMN_HUFF_H

#include "ippdefs.h"

#ifdef  __cplusplus
extern "C" {
#endif

Ipp32u aac_huff_decode_sf(Ipp8u** pp_bs, Ipp32s* p_offset);

#ifdef  __cplusplus
}
#endif

#endif//__AACCMN_HUFF_H
