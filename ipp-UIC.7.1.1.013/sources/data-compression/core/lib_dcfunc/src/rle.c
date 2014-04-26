/*//////////////////////////////// "rle.c" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//     higher level functions of IPP DC ( data compression ) domain for
//     RLE encoding/decoding usage
*/

#include "rle.h"

/*************************************************************************************************
 * Function:
 * int EncodeRLE( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen )
 *
 * Function for realization of RLE Encoding
 * Gets the pSrc as a source vector of length SrcLen, does RLE Encoding and
 * writes result to pDst ( and length of pDst in pDstLen )
 *
 * Main Parameters:
 *          pSrc         - pointer to Source buffer,
 *          pDst         - pointer to Destination buffer,
 *          SrcLen       - Length of source buffer,
 *          pDstLen      - Length of destination buffer
 *
 * Returns:
 *    ippStsNullPtrErr  If One or several pointer(s) is 0
 *    ippStsSizeErr     Length of the source vector is less or equal zero
 *    ippStsNoErr       No errors
 *
 ************************************************************************************************/
int EncodeRLE( Ipp8u *pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen ) {
    /* variables */
    IppStatus st = ippStsNoErr;
    Ipp8u * pSrcOrig = pSrc;

    /* checks the pointer is not null */
    if( pSrc == 0 || pDst == 0 )
        return ippStsNullPtrErr;

    /* do the RLE encoding using ippdc primitive */
    st = ippsEncodeRLE_8u( &pSrcOrig, &SrcLen, pDst, pDstLen );

    if( st != ippStsNoErr && st != ippStsDstSizeLessExpected ) {
        fprintf( stderr, "Error <%d> while trying RLE encoding. Exiting.\n", st );
    }
    return ippStsNoErr; /* No errors */
} /* EncodeRLE() */

/*************************************************************************************************
 * Function:
 * int DecodeRLE( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen )
 *
 * Function for realization of RLE Decoding
 * Gets the pSrc as a source vector of length SrcLen, does RLE decoding and
 * writes result to pDst ( and length of pDst in pDstLen )
 *
 * Main Parameters:
 *          pSrc         - pointer to Source buffer,
 *          pDst         - pointer to Destination buffer,
 *          SrcLen       - Length of source buffer,
 *          pDstLen      - Length of destination buffer
 *
 * Returns:
 *    ippStsNullPtrErr  If One or several pointer(s) is 0
 *    ippStsSizeErr     Length of the source vector is less or equal zero
 *    ippStsNoErr       No errors
 *
 ************************************************************************************************/
int DecodeRLE( Ipp8u *pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen ) {
    /* variables */
    IppStatus  st = ippStsNoErr;
    Ipp8u * pSrcOrig = pSrc;

    /* checks the pointer is not null */
    if( 0 == pSrc || 0 == pDst )
        return ippStsNullPtrErr;

    /* does the RLE decoding */
    st = ippsDecodeRLE_8u( &pSrcOrig, &SrcLen, pDst, pDstLen );

    /* check the returned status of RLE decoding */
    if ( st != ippStsNoErr && st != ippStsDstSizeLessExpected )  {
        fprintf(stderr, "Error <%d> while trying RLE decoding. Exiting.\n", st);
        return st;
    }
    return ippStsNoErr; /* No errors */
} /* DecodeRLE() */
