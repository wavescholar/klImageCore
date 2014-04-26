/*//////////////////////////////// "bwtscheme.c" /////////////////////////////
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

#include "bwtscheme.h"

Ipp8u *pTmpB;

/* macro to exchange 2 pointers */
#define EXCHANGEBUFFERS(PSRC, PDST) \
pTmpB=PSRC; PSRC=PDST; PDST=pTmpB

/* Allocates memory nad inits the bwtState **ppState structure */
int Compress1StateInitAlloc( bwtState **ppState, int blocksize ) {

    IppStatus st;
    int totalSize   = 0;    /* total size of all buffers */
    int bwtSize     = 0;    /* size of additional buffer for BWT transform */
    int mtfSize     = 0;    /* size of state structure for MTF transform */
    int hufSize     = 0;    /* size of state structure for Huffman encoding */

    /* allocation of additional buffer for BWT */
    if( ippStsNoErr != ( st = ippsBWTFwdGetSize_8u( (blocksize<<1), &bwtSize ) ) )
        return st;

    totalSize += bwtSize;

    if( ippStsNoErr != ( st = ippsMTFGetSize_8u( &mtfSize ) ) )
        return st;

    totalSize += mtfSize;

    if( ippStsNoErr != ( st = ippsHuffGetSize_8u( &hufSize ) ) )
        return st;

    totalSize += hufSize;

    /* allocates memory for the structure itself */
    (*ppState)                  = (bwtState *)ippsMalloc_8u(sizeof(bwtState));

    /* allocates memory for the additional buffers and state structures
       as a one big piece of memory and then just inits a pointers as a
       shift inside this memory piece */
    (*ppState)->mtfStateSize    = mtfSize;
    (*ppState)->huffStateSize   = hufSize;
    (*ppState)->bwtBufferSize   = bwtSize;
    (*ppState)->mainstream      = (Ipp8u *)ippsMalloc_8u(sizeof(Ipp8u)*totalSize);
    (*ppState)->pBwtAddBuffer   = (*ppState)->mainstream;
    (*ppState)->pMTFState       = (*ppState)->pBwtAddBuffer + bwtSize;
    (*ppState)->pHuffState      = (*ppState)->pMTFState + mtfSize;
    (*ppState)->blocksize       = blocksize;

    if( ippStsNoErr != ( st = ippsMTFInit_8u( (IppMTFState_8u *)((*ppState)->pMTFState) ) ) )
        return st;

    return ippStsNoErr;
} /* Compress1StateInitAlloc() */

int Decompress1StateInitAlloc( bwtState **ppState, int blocksize ) {

    IppStatus st;
    int totalSize   = 0;    /* total size of all buffers */
    int bwtSize     = 0;    /* size of additional buffer for BWT transform */
    int mtfSize     = 0;    /* size of state structure for MTF transform */
    int hufSize     = 0;    /* size of state structure for Huffman encoding */


    /* gets size of additional buffer for BWT */
    if( ippStsNoErr != ( st = ippsBWTInvGetSize_8u( (blocksize<<1), &bwtSize ) ) )
        return st;

    totalSize += bwtSize;

    /* gets size of state structure for MTF */
    if( ippStsNoErr != ( st = ippsMTFGetSize_8u( &mtfSize ) ) )
        return st;

    totalSize += mtfSize;

    /* gets size of state structure for Huffman Encoding */
    if( ippStsNoErr != ( st = ippsHuffGetSize_8u( &hufSize ) ) )
        return st;

    totalSize += hufSize;

    /* allocates memory for the structure itself */
    (*ppState) = (bwtState *)ippsMalloc_8u(sizeof(bwtState));

    /* allocates memory for the additional buffers and state structures
       as a one big piece of memory and then just inits a pointers as a
       shift inside this memory piece */
    (*ppState)->mtfStateSize    = mtfSize;
    (*ppState)->huffStateSize   = hufSize;
    (*ppState)->bwtBufferSize   = bwtSize;
    (*ppState)->blocksize       = blocksize;
    (*ppState)->mainstream      = (Ipp8u *)ippsMalloc_8u( sizeof(Ipp8u) * totalSize + 1);
    (*ppState)->pBwtAddBuffer   = (*ppState)->mainstream;
    (*ppState)->pMTFState       = ((*ppState)->pBwtAddBuffer + bwtSize);
    (*ppState)->pHuffState      = ((*ppState)->pMTFState + mtfSize);

    if( ippStsNoErr != ( st = ippsMTFInit_8u( (IppMTFState_8u *)((*ppState)->pMTFState) ) ) )
        return st;

    return ippStsNoErr;
} /* Decompress1StateInitAlloc() */

/* top level function for compression */
int Compress1( Ipp8u **ppSrc, int SrcLen, Ipp8u **ppDst, int *pDstLen, bwtState *pState ) {

    int st;

    /* initializes local variables from main state structure fields */
    IppHuffState_8u * pHuffState    = (IppHuffState_8u *)(pState->pHuffState);
    IppMTFState_8u  * pMTFState     = (IppMTFState_8u *)(pState->pMTFState);
    Ipp8u           * pBwtAddBuffer =  pState->pBwtAddBuffer;
    Ipp8u           * pSrc          = *ppSrc;
    Ipp8u           * pDst          = *ppDst;
    int               bwtLen        = *pDstLen,
                      mtfLen        = *pDstLen,
                      rleLen        = *pDstLen,
                      rleLenZ       = *pDstLen;

    /* does the Run-Length Encoding (see file rle.c for implementation details) */
    if( ippStsNoErr != ( st = EncodeRLE( pSrc, SrcLen, pDst, &rleLenZ ) ) ) {
        fprintf(stderr, "Error <%d> while trying to encode RLE. Exiting.\n", st );
        return st;
    }

    /* swaps two pointers */
    EXCHANGEBUFFERS( pSrc, pDst );

    /* does forward Burrows-Wheller transform (see file bwt.c for implementation details) */
    if( ippStsNoErr != ( st = ForwardBWT( pSrc, rleLenZ, pDst, &bwtLen, pBwtAddBuffer ) ) ) {
        fprintf(stderr, "Error <%d> while trying forward BWT Transformation. Exiting.\n", st );
        return st;
    }

    /* swaps two pointers */
    EXCHANGEBUFFERS( pSrc, pDst );

    /* does forward Move-To-Front transform (see file mtf.c for implementation details) */
    if( ippStsNoErr != ( st = ForwardMTF( pSrc, bwtLen, pDst, &mtfLen, pMTFState ) ) ) {
        fprintf(stderr, "Error <%d> while trying forward MTF Transformation. Exiting.\n", st );
        return st;
    }

    /* swaps two pointers */
    EXCHANGEBUFFERS( pSrc, pDst );

    /* does the Run-Length Encoding (see file rle.c for implementation details) */
    if( ippStsNoErr != ( st = EncodeRLE( pSrc, mtfLen, pDst, &rleLen ) ) ) {
        fprintf(stderr, "Error <%d> while trying to encode RLE. Exiting.\n", st );
        return st;
    }

    /* swaps two pointers */
    EXCHANGEBUFFERS( pSrc, pDst );

    /* does the Huffman Encoding (see file huffman.c for implementation details) */
    if( ippStsNoErr != ( st = EncodeHuffman( pSrc, rleLen, pDst, pDstLen, pHuffState ) ) ) {
        fprintf(stderr, "Error <%d> while trying to encode Huffman. Exiting.\n", st );
        return st;
    }

    *ppSrc = pSrc;
    *ppDst = pDst;

    return st;
} /* Compress1() */

/* top level function for decompression */
int Decompress1( Ipp8u **ppSrc, int SrcLen, Ipp8u **ppDst, int *pDstLen, bwtState *pState ) {

    int st;
    IppHuffState_8u * pHuffState    = (IppHuffState_8u *)(pState->pHuffState);
    IppMTFState_8u  * pMTFState     = (IppMTFState_8u  *)(pState->pMTFState);
    Ipp8u           * pBwtAddBuffer = pState->pBwtAddBuffer;
    Ipp8u           * pSrc          = *ppSrc;
    Ipp8u           * pDst          = *ppDst;
    int               rleLen        = *pDstLen,
                      rleLenZ       = *pDstLen,
                      mtfLen        = *pDstLen,
                      hufLen        = *pDstLen;

    /* does the Huffman Decoding (see file huffman.c for implementation details) */
    if( ippStsNoErr != ( st = DecodeHuffman( pSrc, SrcLen, pDst, &hufLen, pHuffState ) ) ) {
        fprintf(stderr, "Error <%d> while trying to decode Huffman. Exiting.\n", st );
        return st;
    }

    /* swaps two pointers */
    EXCHANGEBUFFERS( pSrc, pDst );

    /* does the Run-Length Decoding (see file rle.c for implementation details) */
    if( ippStsNoErr != ( st = DecodeRLE( pSrc, hufLen, pDst, &rleLen ) ) ) {
        fprintf(stderr, "Error <%d> while trying to decode RLE. Exiting.\n", st );
        return st;
    }

    /* swaps two pointers */
    EXCHANGEBUFFERS( pSrc, pDst );

    /* does backward Move-To-Front transform (see file mtf.c for implementation details) */
    if( ippStsNoErr != ( st = InverseMTF( pSrc, rleLen, pDst, &mtfLen, pMTFState ) ) ) {
        fprintf(stderr, "Error <%d> while trying backward MTF Transformation. Exiting.\n", st );
        return st;
    }

    /* swaps two pointers */
    EXCHANGEBUFFERS( pSrc, pDst );

    /* does backward Burrows-Wheller transform (see file bwt.c for implementation details) */
    if( ippStsNoErr != ( st = InverseBWT( pSrc, mtfLen, pDst, &rleLenZ, pBwtAddBuffer ) ) ) {
        fprintf(stderr, "Error <%d> while trying backward BWT Transformation. Exiting.\n", st );
        return st;
    }

    /* swaps two pointers */
    EXCHANGEBUFFERS( pSrc, pDst );

    /* does the Run-Length Decoding (see file rle.c for implementation details) */
    if( ippStsNoErr != ( st = DecodeRLE( pSrc, rleLenZ, pDst, pDstLen ) ) ) {
        fprintf(stderr, "Error <%d> while trying to decode RLE. Exiting.\n", st );
        return st;
    }

    *ppSrc = pSrc;
    *ppDst = pDst;

    return st;
} /* Decompress1() */ 

/* frees all the fields of bwtState structure */
void Free1State( bwtState *pState ) {
    ippsFree(pState->mainstream);
    ippsFree(pState);
} /* Free1State() */
