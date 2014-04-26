/*//////////////////////////////// "gitscheme.c" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
//
//     higher level functions of IPP DC ( data compression ) domain for
//     BWT, MTF, RLE and Huffman encoding/decoding usage
*/

#include "gitscheme.h"

Ipp8u *pTmp;
/* macro for exchanging two variables-pointers*/
#define EXCHANGEBUFFERS(PSRC, PDST) pTmp = PSRC; PSRC = PDST; PDST = pTmp

int Compress2StateInitAlloc( gitState **ppState, int blocksize, IppGITStrategyHint strategyHint ) {

    IppStatus st;
    int totalSize = 0,
        bwtSize   = 0,
        gitSize   = 0,
        maxsize   = blocksize << 1;

    IppGITState_8u *pGState = NULL;

    /* allocation of additional buffer for BWT */
    if( ippStsNoErr != ( st = ippsBWTFwdGetSize_8u( maxsize, &bwtSize ) ) )
        return st;

    totalSize += bwtSize;

    if( ippStsNoErr != ( st = ippsEncodeGITGetSize_8u( maxsize, maxsize, &gitSize ) ) )
        return st;

    totalSize += gitSize;

    (*ppState)                = (gitState *)ippsMalloc_8u(sizeof(gitState));
    (*ppState)->mainstream    = (Ipp8u *)ippsMalloc_8u( sizeof(Ipp8u) * totalSize );
    (*ppState)->pBwtAddBuffer = (*ppState)->mainstream;
    (*ppState)->pGITState     = (*ppState)->mainstream + bwtSize;

    (*ppState)->bwtBufferSize = bwtSize;
    (*ppState)->gitStateSize  = gitSize;
    (*ppState)->strategyHint  = strategyHint;
    (*ppState)->blocksize     = blocksize;
    pGState = (IppGITState_8u *)((*ppState)->pGITState);
    if( ippStsNoErr != ( st = ippsEncodeGITInit_8u( maxsize, maxsize, pGState ) ) )
        return st;

    return ippStsNoErr;
} /* Compress2StateInitAlloc() */

int Decompress2StateInitAlloc( gitState **ppState, int blocksize, IppGITStrategyHint strategyHint ) {

    IppStatus st;
    int totalSize = 0,
        bwtSize   = 0,
        gitSize   = 0;
    IppGITState_8u *pGState = NULL;

    /* allocation of additional buffer for BWT */
    if( ippStsNoErr != ( st = ippsBWTInvGetSize_8u( blocksize << 1, &bwtSize ) ) )
        return st;

    totalSize += bwtSize;

    if( ippStsNoErr != ( st = ippsDecodeGITGetSize_8u( blocksize, &gitSize ) ) )
        return st;

    totalSize += gitSize;

    (*ppState)                  = (gitState *)ippsMalloc_8u(sizeof(gitState));
    (*ppState)->mainstream      = (Ipp8u *)ippsMalloc_8u( sizeof(Ipp8u) * totalSize);
    (*ppState)->pBwtAddBuffer   = (*ppState)->mainstream;
    (*ppState)->pGITState       = ((*ppState)->pBwtAddBuffer + bwtSize);
    (*ppState)->bwtBufferSize   =  bwtSize;
    (*ppState)->gitStateSize    =  gitSize;
    (*ppState)->strategyHint    =  strategyHint;
    (*ppState)->blocksize       =  blocksize;

    pGState = (IppGITState_8u *)((*ppState)->pGITState);
    if( ippStsNoErr != ( st = ippsDecodeGITInit_8u( blocksize << 1, pGState ) ) )
        return st;

    return ippStsNoErr;
} /* Decompress2StateInitAlloc() */

int Compress2( Ipp8u **ppSrc, int SrcLen, Ipp8u **ppDst, int *pDstLen, gitState *pState ) {
    int     st;
    int         bwtLen              = *pDstLen;
    Ipp8u *     pBwtAddBuffer       = pState->pBwtAddBuffer;
    Ipp8u *     pSrc                = *ppSrc;
    Ipp8u *     pDst                = *ppDst;
    IppGITState_8u * pGITState      = (IppGITState_8u *)pState->pGITState;
    IppGITStrategyHint strategyHint = pState->strategyHint;

    if( ippStsNoErr != ( st = ForwardBWT( pSrc, SrcLen, pDst, &bwtLen, pBwtAddBuffer ) ) ) {
        fprintf(stderr, "Error <%d> while trying forward BWT Transformation. Exiting.\n", st );
        return st;
    }

    EXCHANGEBUFFERS(pSrc, pDst);

    if( ippStsNoErr != ( st = EncodeGIT( pSrc, bwtLen, pDst, pDstLen, pGITState, strategyHint ) ) ) {
        fprintf(stderr, "Error <%d> while trying to encode GIT. Exiting.\n", st );
        return st;
    }

    *ppSrc = pSrc;
    *ppDst = pDst;
    return st;
} /* Compress2() */

int Decompress2( Ipp8u **ppSrc, int SrcLen, Ipp8u **ppDst, int *pDstLen, gitState *pState ) {
    int st;
    int gitLen                      = *pDstLen;
    Ipp8u * pBwtAddBuffer           = pState->pBwtAddBuffer;
    IppGITState_8u  * pGITState     = (IppGITState_8u *)pState->pGITState;
    IppGITStrategyHint strategyHint = pState->strategyHint;
    Ipp8u *pSrc                     = *ppSrc;
    Ipp8u *pDst                     = *ppDst;

    if( ippStsNoErr != ( st = DecodeGIT( pSrc, SrcLen, pDst, &gitLen, pGITState, strategyHint ) ) ) {
        fprintf(stderr, "Error <%d> while trying to decode GIT. Exiting.\n", st );
        return st;
    }

    EXCHANGEBUFFERS(pSrc, pDst);

    if( ippStsNoErr != ( st = InverseBWT( pSrc, gitLen, pDst, pDstLen, pBwtAddBuffer ) ) ) {
        fprintf(stderr, "Error <%d> while trying backward BWT Transformation. Exiting.\n", st );
        return st;
    }

    *ppSrc = pSrc;
    *ppDst = pDst;

    return st;
} /* Decompress2() */

void Free2State( gitState *pState ) {
    ippsFree(pState->mainstream);
    ippsFree(pState);
} /* Free2State() */
