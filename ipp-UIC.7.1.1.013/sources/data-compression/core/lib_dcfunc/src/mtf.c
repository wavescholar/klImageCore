/*//////////////////////////////// "mtf.c" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//     higher level functions of IPP DC ( data compression ) domain for
//     Move-To-Front Transformation usage
*/

#include "mtf.h"

/*************************************************************************************************
 * Function:
 * int ForwardMTF( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppMTFState_8u * pMTFState )
 *
 * Function for realization of forward MTF
 * Gets the pSrc as a source vector of length SrcLen, does forward MTF and
 * writes result to pDst ( and length of pDst in pDstLen )
 *
 * Main Parameters:
 *          pSrc         - pointer to Source buffer,
 *          pDst         - pointer to Destination buffer,
 *          SrcLen       - Length of source buffer,
 *          pDstLen      - Length of destination buffer
 *          pMTFState    - pointer to MTF encoding/decoding state structure
 *
 * Returns:
 *    ippStsNullPtrErr  If One or several pointer(s) is 0
 *    ippStsSizeErr     Length of the source vector is less or equal zero
 *    ippStsNoErr       No errors
 *
 ************************************************************************************************/
int ForwardMTF( Ipp8u *pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppMTFState_8u * pMTFState ) {

    /* variables */
    IppStatus  st = ippStsNoErr;

    /* checks the pointer is not null */
    if( pSrc == 0 || pDst == 0 )
        return ippStsNullPtrErr;

   /* allocates the memory needed for MTF transformation structure */
    if( ippStsNoErr != ( st = ippsMTFInit_8u( pMTFState ) ) ) {
        fprintf( stderr, "Error while trying to init the mtf state. Exiting.\n" );
        return st;
    }

    /* does the forward MTF transformation */
    if( ippStsNoErr != ( st = ippsMTFFwd_8u( pSrc, pDst, SrcLen, pMTFState) ) ) {
        fprintf(stderr, "Error <%d> while trying do forward MTF. Exiting.\n", st);
        return st;
    }

    *pDstLen = SrcLen;

    return ippStsNoErr; /* No errors */
} /* ForwardMTF() */

/*************************************************************************************************
 * Function:
 * IppStatus InverseMTF( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppMTFState_8u * pMTFState)
 *
 * Function for realization of backward MTF
 * Gets the pSrc as a source vector of length SrcLen, does backward MTF and
 * writes result to pDst ( and length of pDst in pDstLen )
 *
 * Main Parameters:
 *          pSrc         - pointer to Source buffer,
 *          pDst         - pointer to Destination buffer,
 *          SrcLen       - Length of source buffer,
 *          pDstLen      - Length of destination buffer
 *          pMTFState    - pointer to MTF encoding/decoding state structure
 *
 * Returns:
 *    ippStsNullPtrErr  If One or several pointer(s) is 0
 *    ippStsSizeErr     Length of the source vector is less or equal zero
 *    ippStsNoErr       No errors
 *
 ************************************************************************************************/
int InverseMTF( Ipp8u *pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, IppMTFState_8u * pMTFState ) {

    /* variables */
    IppStatus  st = ippStsNoErr;

    /* checks the pointer is not null */
    if( pSrc == 0 || pDst == 0 )
        return ippStsNullPtrErr;

    /* allocates the memory needed for MTF transformation structure */
    if( ippStsNoErr != ( st = ippsMTFInit_8u( pMTFState ) ) ) {
        fprintf( stderr, "Error while trying to init the mtf state. Exiting.\n" );
        return st;
    }

    /* does the backward MTF transformation */
    if( ippStsNoErr != ( st = ippsMTFInv_8u( pSrc, pDst, SrcLen, pMTFState) ) ) {
        fprintf(stderr, "Error <%d> while trying do backward MTF. Exiting.\n", st);
        return st;
    }

    *pDstLen = SrcLen;

    return ippStsNoErr; /* No errors */
} /* InverseMTF() */
