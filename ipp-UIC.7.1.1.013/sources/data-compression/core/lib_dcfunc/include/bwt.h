/* //////////////////////////////// "bwt.h" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
*/

#if !defined( __BWT_H__ )

#define __BWT_H__

#include "all.h"

int ForwardBWT( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, Ipp8u *bwtAddBuffer );
int InverseBWT( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, Ipp8u *bwtAddBuffer );

#endif /* __BWT_H__ */
