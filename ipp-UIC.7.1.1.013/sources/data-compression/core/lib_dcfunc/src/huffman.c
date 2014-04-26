/*//////////////////////////////// "huffman.c" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//     higher level functions of IPP DC ( data compression ) domain for
//     Huffman encoding/decoding usage
*/

#include "huffman.h"

/*************************************************************************************************
 * Function:
 * int EncodeHuffman( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppHuffState_8u *pHuffState )
 *
 * Function for realization of Huffman Encoding
 * Gets the pSrc as a source vector of length SrcLen, does Huffman encoding and
 * writes result to pDst ( and length of pDst in pDstLen )
 *
 * Main Parameters:
 *          pSrc         - pointer to Source buffer,
 *          pDst         - pointer to Destination buffer,
 *          SrcLen       - Length of source buffer,
 *          pDstLen      - Length of destination buffer
 *          pHuffState   - pointer to State structure for Huffman encoding/decoding
 *
 * Returns:
 *    ippStsNullPtrErr  If One or several pointer(s) is 0
 *    ippStsSizeErr     Length of the source vector is less or equal zero
 *    ippStsNoErr       No errors
 *
 ************************************************************************************************/
int EncodeHuffman( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppHuffState_8u *pHuffState ) {

    IppStatus  st = ippStsNoErr;
    int     freqTable[256];     /* array of characters frequencies */
    int     codeLenTable[256];  /* array of characters codelengths */
    int     i           =  0;
    int     maxDstLen   = *pDstLen;
    Ipp8u * pDstTmp     =  pDst;
    int     blockSize   =  0;
    int     dstLen      =  0;

    /* checks the pointers is not null */
    if( pSrc == 0 || pDst == 0 ) return ippStsNullPtrErr;

    /* fills the array to reach uniform distribution of characters frequencies */
    for( i = 0; i < 256; i++ ) freqTable[i] = 0;

    /* stores statistics of input characters from pSrc vector */
    for( i = 0; i < SrcLen; i++ ) freqTable[(int)(pSrc[i])]++;

    /* init structure for huffman encoding */
    if( ippStsNoErr != ippsEncodeHuffInitAlloc_8u( freqTable, &pHuffState ) ) {
        fprintf( stderr, "Error while trying to init the frequency table for huffman. Exiting.\n" );
        return st;
    }

    /* gets the array of characters codelengths to store it in pDst vector */
    if( ippStsNoErr != ippsHuffGetLenCodeTable_8u( codeLenTable, pHuffState ) ) {
        fprintf( stderr, "Error while trying to get the code length 4 huffman. Exiting.\n" );
        return st;
    }

    dstLen = maxDstLen;

    /* stores array of characters codelengths in the beginning of pDst vector */
    if( ippStsNoErr != ippsHuffLenCodeTablePack_8u( codeLenTable, pDstTmp, &dstLen ) ) {
        fprintf( stderr, "Error <%d> while trying to flush code lengths. Exiting.\n", st );
        return st;
    }

    /* slides vectors, calculates lengths */
    pDstTmp   += dstLen;
    blockSize  = dstLen;
    dstLen     = maxDstLen;

    /* does the huffman encoding of pSrc vector to pDst vector */
    if( ippStsNoErr != ippsEncodeHuff_8u( pSrc, SrcLen, pDstTmp, &dstLen, pHuffState ) ) {
        fprintf( stderr, "Error <%d> while trying to encode huffman. Exiting.\n", st );
        return st;
    }

    /* slides vectors, calculates lengths */
    pDstTmp   += dstLen;
    blockSize += dstLen;
    dstLen     = maxDstLen;

    /* flushes the reminder of Huffman encoding to the end of pDst vector */
    if( ippStsNoErr != ippsEncodeHuffFinal_8u( pDstTmp, &dstLen, pHuffState ) ) {
        fprintf( stderr, "Error <%d> while trying to finalize ecoding huffman. Exiting.\n", st);
        return st;
    }

    /* calculates length of pDst vector */
    blockSize  += dstLen;
    *pDstLen    = blockSize;

    return 0;
} /* EncodeHuffman() */

/************************************************************************************************
 * Function:
 * int DecodeHuffman( Ipp8u *pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppHuffState_8u *pHuffState )
 *
 * Function for realization of Huffman Decoding
 * Gets the pSrc as a source vector of length SrcLen, does Huffman decoding and
 * writes result to pDst ( and length of pDst in pDstLen )
 *
 * Main Parameters:
 *          pSrc         - pointer to Source buffer,
 *          pDst         - pointer to Destination buffer,
 *          SrcLen       - Length of source buffer,
 *          pDstLen      - Length of destination buffer
 *          pHuffState   - pointer to State structure for Huffman encoding/decoding
 *
 * Returns:
 *    ippStsNullPtrErr  If One or several pointer(s) is 0
 *    ippStsSizeErr     Length of the source vector is less or equal zero
 *    ippStsNoErr       No errors
 *
 *************************************************************************************************/
int DecodeHuffman( Ipp8u *pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppHuffState_8u *pHuffState ) {

    IppStatus   st;
    int         dstLen = *pDstLen,
                codeLenTable[256];  /* to store table for code lengths */
    Ipp8u       * pTSrc = pSrc,     /* init additional pointers to preserve pointer pSrc */
                * pTDst = pDst;     /* init additional pointers to preserve pointer pDst */

    /* checks the pointers is not null */
    if( pSrc == 0 || pDst == 0) return ippStsNullPtrErr;

    /* unpacks table stored in the beginning of pSrc vector */
    if( ippStsNoErr != ( st = ippsHuffLenCodeTableUnpack_8u( pTSrc, &dstLen, codeLenTable ) ) ) {
        fprintf(stderr, "Error <%d> while decoding Huffman (Unpack Table)\n", st);
        return st;
    }

    /* slide pointer to the beginning of encoded data (passes the packed array of codes lengths )*/
    pTSrc += dstLen;

    /* init structure for huffman decoding */
    if( ippStsNoErr != ( st = ippsDecodeHuffInitAlloc_8u( codeLenTable, &pHuffState ) ) ) {
        fprintf(stderr, "Error <%d> while decoding Huffman (Init)\n", st);
        return st;
    }

    /* decodes by huffman using ippdc function */
    if( ippStsNoErr != ( st = ippsDecodeHuff_8u( pTSrc, SrcLen - dstLen, pTDst, pDstLen, pHuffState ) ) ) {
        fprintf(stderr, "Error <%d> while decoding HUFFMAN\n", st);
        return st;
    }

    return 0;
} /* DecodeHuffman() */
