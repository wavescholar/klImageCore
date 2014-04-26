/*//////////////////////////////// "git.c" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//     higher level functions of IPP DC ( data compression ) domain for
//     GIT encoding/decoding
*/

#include "git.h"

/*************************************************************************************************
 * Function:
 * int EncodeGIT( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen,
 *                         IppGITState_8u * pGITState, IppGITStrategyHint strategyHint )
 *
 * Function for realization of General Interval Transform
 * Gets the pSrc as a source vector of length SrcLen, does GIT and
 * writes result to pDst ( and length of pDst in pDstLen )
 *
 * Main Parameters:
 *          pSrc         - pointer to Source buffer,
 *          pDst         - pointer to Destination buffer,
 *          SrcLen       - Length of source buffer,
 *          pDstLen      - Length of destination buffer
 *          pGITState    - State structure for General Interval Transform
 *          strategyHint - Strategy hint for General Interval Transform
 *
 * Returns:
 *    ippStsNullPtrErr  If One or several pointer(s) is NULL
 *    ippStsSizeErr     Length of the source vector is less or equal zero
 *    ippStsNoErr       No errors
 *
 ************************************************************************************************/
int EncodeGIT( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppGITState_8u * pGITState, IppGITStrategyHint strategyHint ) {

    IppStatus st;

    /* just cast a ippdc function */
    if( ippStsNoErr != ( st = ippsEncodeGIT_8u( pSrc, SrcLen, pDst, pDstLen, strategyHint, pGITState ) ) ) {
        fprintf( stderr, "Error <%d> while trying GIT (main). Exiting.\n", st );
        return st;
    }
    return 0;
} /* EncodeGIT() */

/*************************************************************************************************
 * Function:
 * int DecodeGIT( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen
 *                         IppGITState_8u * pGITState, IppGITStrategyHint strategyHint )
 *
 * Function for realization of backward General Interval Transform
 * Gets the pSrc as a source vector of length SrcLen, does GIT and
 * writes result to pDst ( and length of pDst in pDstLen )
 *
 * Main Parameters:
 *          pSrc         - Pointer to Source buffer
 *          pDst         - Pointer to Destination buffer
 *          SrcLen       - Length of Source buffer
 *          pDstLen      - Length of Destination buffer
 *          pGITState    - State structure for General Interval Transform
 *          strategyHint - Strategy hint for General Interval Transform
 *
 * Returns:
 *    ippStsNullPtrErr  If One or several pointer(s) is NULL
 *    ippStsSizeErr     Length of the source vector is less or equal zero
 *    ippStsNoErr       No errors
 *
 ************************************************************************************************/
int DecodeGIT( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppGITState_8u * pGITState, IppGITStrategyHint strategyHint ) {

    IppStatus st;

    /* just cast a ippdc function */
    if( ippStsNoErr != ( st = ippsDecodeGIT_8u( pSrc, SrcLen, pDst, pDstLen, strategyHint, pGITState ) ) ) {
        fprintf( stderr, "Error <%d> while trying decode GIT. Exiting.\n", st );
        return st;
    }
    return 0;
} /* DecodeGIT() */
