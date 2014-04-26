/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __PS_DEC_PARSER_H__
#define __PS_DEC_PARSER_H__

/* BITSTREAM */
#include "bstream.h"
/* PARAMETRIC STEREO */
#include "ps_dec_struct.h"

#ifdef  __cplusplus
extern  "C" {
#endif

  void ps_header_fill_default( sPSDecComState* pPSItem );

  Ipp32s ps_dec_parser( sBitsreamBuffer * BS, sPSDecComState* pPSItem);

#ifdef  __cplusplus
}
#endif

#endif/*__PS_DEC_PARSER_H__ */
/* EOF */
