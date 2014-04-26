/*//////////////////////////////// "bwt.c" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//     higher level functions of IPP DC ( data compression ) domain for
//     BWT forward/backward transform
*/

#include "bwt.h"

/*************************************************************************************************
 * Function:
 * int ForwardBWT( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, Ipp8u *addBuff )
 *
 * Function for realization of forward BWT
 * Gets the pSrc as a source vector of length SrcLen, does BWT and writes result to pDst
 * ( and length of pDst in pDstLen )
 *
 * Main Parameters:
 *          pSrc         - pointer to Source buffer,
 *          pDst         - pointer to Destination buffer,
 *          SrcLen       - Length of source buffer,
 *          pDstLen      - Length of destination buffer
 *          addBuff      - Additional buffer for BWT transform
 *
 * Returns:
 *    ippStsNullPtrErr  If One or several pointer(s) is 0
 *    ippStsSizeErr     Length of the source vector is less or equal zero
 *    ippStsNoErr       No errors
 *
 ************************************************************************************************/
int ForwardBWT( Ipp8u *pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, Ipp8u *addBuff ) {

    IppStatus st;           /* to store and analyze return statuses of ippdc function */
    int firstMeetAt = 0;    /* to store the first occurrence index */

    /* checks the pointer is not null */
    if( pSrc == 0 || pDst == 0 )
        return ippStsNullPtrErr;

    /* checks the pointer is not null */
    if( addBuff == 0 )
        return ippStsNullPtrErr;

    /* size of  vector after bwt is 4 bytes longer, due to the
       first 4 bytes contain the 'first occurrence index' value */
    *pDstLen = SrcLen + sizeof(int);

    /* does the forward BWT using ippdc function */
    if( ippStsNoErr != ( st = ippsBWTFwd_8u( pSrc, pDst + sizeof(int), SrcLen, &firstMeetAt, addBuff ) ) ) {
        fprintf( stderr, "Error <%d> while trying forward BWT. Exiting.\n", st );
        return st;
    }

    /* store the first occurrence index in first 4 bytes of pSrc vector */
    ((int*)pDst)[0] = firstMeetAt;

    return 0; /* No errors */
} /* ForwardBWT() */

/*************************************************************************************************
 * Function:
 * int InverseBWT( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, Ipp8u *addBuff )
 *
 * Function for realization of backward BWT
 * Gets the pSrc as a source vector of length SrcLen, does BWT and writes result to pDst
 * ( and length of pDst in pDstLen )
 *
 * Main Parameters:
 *          pSrc         - pointer to Source buffer,
 *          pDst         - pointer to Destination buffer,
 *          SrcLen       - Length of source buffer,
 *          pDstLen      - Length of destination buffer
 *          addBuff      - Additional buffer for BWT transform
 *
 * Returns:
 *    ippStsNullPtrErr  If One or several pointer(s) is 0
 *    ippStsSizeErr     Length of the source vector is less or equal zero
 *    ippStsNoErr       No errors
 *
 ************************************************************************************************/
int InverseBWT( Ipp8u * pSrc, int SrcLen, Ipp8u *pDst, int *pDstLen, Ipp8u *addBuff ) {

    IppStatus st;           /* to store and analyze return statuses of ippdc function */
    int bwtFirstShot = 0;   /* to store the first occurrence index */

    /* checks the pointer is not null */
    if( addBuff == 0)
        return ippStsNullPtrErr;

    /* checks the pointer is not null */
    if( pSrc == 0 || pDst == 0 )
        return ippStsNullPtrErr;

    /* reads first four bytes to get the first occurrence index for BWT */
    bwtFirstShot = ((int*)pSrc)[0];

    /* resulting length is 4 bytes shorter */
    *pDstLen = SrcLen - sizeof(int);

    /* does the backward BWT using ippdc function */
    if( ippStsNoErr != ( st = ippsBWTInv_8u( pSrc + ( sizeof(int) ), pDst, SrcLen - sizeof(int), bwtFirstShot, addBuff ) ) ) {
        fprintf(stderr, "Error <%d> while trying do BWT. Exiting.\n", st);
        return st;
    }

    return 0; /* No errors */
} /* InverseBWT() */
