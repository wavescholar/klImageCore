/* //////////////////////////////// "gitscheme.h" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
//
//            Sample of IPP DC ( data compression ) domain functions usage
//
*/

#if !defined( __GIT_SCHEME__ )

#define __GIT_SCHEME__

#include "all.h"

typedef struct _git_state gitState;

struct _git_state {
    Ipp8u             * mainstream;
    int                 blocksize;
    Ipp8u             * pBwtAddBuffer;
    int                 bwtBufferSize;
    Ipp8u             * pGITState;
    int                 gitStateSize;
    IppGITStrategyHint  strategyHint;
};

int Compress2StateInitAlloc( gitState **ppState, int blocksize, IppGITStrategyHint strategyHint );
int Decompress2StateInitAlloc( gitState **ppState, int blocksize, IppGITStrategyHint strategyHint );

int Compress2( Ipp8u **ppSrc, int SrcLen, Ipp8u **ppDst, int *pDstLen, gitState *pState );
int Decompress2( Ipp8u **ppSrc, int SrcLen, Ipp8u **ppDst, int *pDstLen, gitState *pState );

void Free2State( gitState *pState );

#endif /* __BWT_SCHEME__ */
