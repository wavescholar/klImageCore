/* //////////////////////////////// "bwtscheme.h" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
*/

#if !defined( __BWT_SCHEME__ )

#define __BWT_SCHEME__

#include "all.h"

typedef struct _bwt_state bwtState;

struct _bwt_state {
    Ipp8u             * mainstream;
    int                 blocksize;
    Ipp8u             * pBwtAddBuffer;
    int                 bwtBufferSize;
    Ipp8u             * pHuffState;
    int                 huffStateSize;
    Ipp8u             * pMTFState;
    int                 mtfStateSize;
};

int Compress1StateInitAlloc( bwtState **ppState, int BlockSize );
int Decompress1StateInitAlloc( bwtState **ppState, int BlockSize );
int Compress1( Ipp8u **ppSrc, int SrcLen, Ipp8u ** ppDst, int *pDstLen, bwtState *pState );
int Decompress1( Ipp8u **pSrc, int SrcLen, Ipp8u ** ppDst, int *pDstLen, bwtState *pState );
void Free1State( bwtState *pState );

#endif /* __BWT_SCHEME__ */
