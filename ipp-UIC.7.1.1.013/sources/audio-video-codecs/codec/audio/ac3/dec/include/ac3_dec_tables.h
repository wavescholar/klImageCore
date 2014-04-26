/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AC3_DEC_TABLES_H__
#define __AC3_DEC_TABLES_H__

#include "ippdefs.h"

typedef struct frmsize_s {
  Ipp16u bit_rate;
  Ipp16u frm_size[3];
} frmsize_t;

extern const frmsize_t FRAMESIZECODE[];
extern const Ipp32s SAMPLING_RATE[];
extern const Ipp16u NFCHANS[];
extern Ipp16s UNGRP5[];

extern Ipp16s SLOWDEC[];
extern Ipp16s FASTDEC[];
extern Ipp16s SLOWGAIN[];
extern Ipp16s DBPBTAB[];

extern Ipp16s FLOORTAB[];
extern Ipp16s FASTGAIN[];
extern Ipp16s BNDTAB[];
extern Ipp16s BNDSZ[];
extern Ipp16s MASKTAB[];
extern Ipp16s LATAB[];
extern Ipp16s HTH[][50];
extern Ipp8u BAPTAB[];

extern Ipp32f phscorFac[8][8][4][5];
extern Ipp32f phsCorTab[];
extern Ipp32f WindowTable[];

#endif /* __AC3DEC_TABLES_H__ */

