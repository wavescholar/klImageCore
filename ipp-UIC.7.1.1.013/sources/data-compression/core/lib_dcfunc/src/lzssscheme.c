/*//////////////////////////////// "lzssscheme.c" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
//     higher level functions of IPP DC ( data compression ) domain for
//     LZSS encoding/decoding usage
*/

#include "lzssscheme.h"

int CompressLZSS( Ipp8u *pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen ) {

    IppStatus         st;
    IppLZSSState_8u * pLZSSState = NULL;
    Ipp8u           * pTSrc      = pSrc,
                    * pTDst      = pDst;
    int               srcLen     = SrcLen,
                      dstLen     = *pDstLen;

    /* inits the lzss state structure for decoding */
    if( ippStsNoErr != ( st = ippsEncodeLZSSInitAlloc_8u( &pLZSSState ) ) ) {
        fprintf( stderr, "Error while encode lz77.exiting...\n");
        return st;
    }

    do {
        /* encode source vector pSrc to destination pDst and returns len of encoded vector to dstLen*/
        st = ippsEncodeLZSS_8u( &pTSrc, &srcLen, &pTDst, &dstLen, pLZSSState );

        /* if error is not due to the insufficies output buffer */
        if( st != ippStsNoErr && st != ippStsDstSizeLessExpected ) {
            fprintf( stderr, "Error while encode lz77.exiting...\n");
            return st;
        }
    } while( st != ippStsNoErr );

    if( ippStsNoErr != ( st = ippsEncodeLZSSFlush_8u(&pTDst, &dstLen, pLZSSState) ) ) {
        fprintf( stderr, "Error <%d> while flushing LZSS endings. Exiting.\n", st );
    }
    *pDstLen -= dstLen;

    /* frees the space, allocated for pLZSSState state structure */
    ippsLZSSFree_8u( pLZSSState );

    return 0;
} /* CompressLZSS() */

int DecompressLZSS( Ipp8u *pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen ) {

    IppStatus         st;
    IppLZSSState_8u * pLZSSState = NULL;
    Ipp8u           * pTSrc      = pSrc,
                    * pTDst      = pDst;
    int               srcLen     = SrcLen,
                      dstLen     = *pDstLen,
                      tDstLen    = 0;

    /* inits the lzss state structure for decoding */
    if( ippStsNoErr != ( st = ippsDecodeLZSSInitAlloc_8u( &pLZSSState ) ) ) {
        fprintf( stderr, "Error while initalloc 4 decode lz77.exiting...\n");
        return st;
    }

    do {
        dstLen = GZIPBLOCKSIZE; /* decode using a small blocks of GZIPBLOCKSIZE length */
        tDstLen += dstLen;

        /* decode source vector pSrc to destination pDst and returns len of decoded vector to dstLen*/
        st = ippsDecodeLZSS_8u( &pTSrc, &srcLen, &pTDst, &dstLen, pLZSSState );

        /* if error is not due to the insufficies output buffer */
        if( st != ippStsNoErr && st != ippStsDstSizeLessExpected ) {
            fprintf( stderr, "Error %d while decode lz77.exiting...\n", st);
            return st;
        }

        tDstLen -= dstLen;

    } while( st != ippStsNoErr );

    *pDstLen = tDstLen;
    dstLen =  tDstLen;

    /* frees the space, allocated for pLZSSState state structure */
    ippsLZSSFree_8u( pLZSSState );
    return 0;
} /* DecompressLZSS() */
