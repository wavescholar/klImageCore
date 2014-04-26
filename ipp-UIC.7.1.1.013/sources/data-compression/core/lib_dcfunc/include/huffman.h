/* //////////////////////////////// "huffman.h" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
*/

#if !defined( __HUFFMAN_H__ )

#define __HUFFMAN_H__

#include "all.h"

int EncodeHuffman( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppHuffState_8u *pHuffState );
int DecodeHuffman( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppHuffState_8u *pHuffState );

#endif /* __HUFFMAN_H__ */
