/* //////////////////////////////// "mtf.h" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
*/

#if !defined( __MTF_H__ )

#define __MTF_H__

#include "all.h"

int ForwardMTF( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppMTFState_8u * pMTFState );
int InverseMTF( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppMTFState_8u * pMTFState );

#endif /* __MTF_H__ */
