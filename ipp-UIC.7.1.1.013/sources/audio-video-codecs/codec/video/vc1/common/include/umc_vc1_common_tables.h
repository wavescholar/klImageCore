/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, misc tables
//
*/

#ifndef __UMC_VC1_COMMON_TABLES_H__
#define __UMC_VC1_COMMON_TABLES_H__

#include "ippdefs.h"

//VC-1 Table 58: Escape mode 3 level codeword size conservative code-table (used for 1 <= PQUANT <= 7 or if
//VOPDQUANT is present)
//1 <= PQUANT <= 7
//ESCLVLSZ VLC    Level codeword size
//  001                 1
//  010                 2
//  011                 3
//  100                 4
//  101                 5
//  110                 6
//  111                 7
//  00000               8
//  00001               9
//  00010               10
//  00011               11

//it is not used, because esier get FLC 3 or 5 bits length
extern const Ipp32s VC1_EscapeMode3Conservative_tbl[];

//VC-1 Table 59: Escape mode 3 level codeword size efficient code-table (used for 8 <= PQUANT <= 31 and if
//VOPDQUANT is absent)
//8 <= PQUANT <= 31
//ESCLVLSZ VLC    Level codeword size
//  1                   2
//  01                  3
//  001                 4
//  0001                5
//  00001               6
//  000001              7
//  000000              8

//it is not used for while,
extern const Ipp32s VC1_EscapeMode3Efficient_tbl[];


//VC-1 Table 68: IMODE Codetable
//CODING MODE    CODEWORD
//Raw            0000
//Norm-2        10
//Diff-2        001
//Norm-6        11
//Diff-6        0001
//Rowskip        010
//Colskip        011

extern const Ipp32s VC1_Bitplane_IMODE_tbl[];

//VC-1 Table 81: Code table for 3x2 and 2x3 tiles
extern const Ipp32s VC1_BitplaneTaledbitsTbl[];

//VC-1 Table 39
extern const Ipp32s VC1_BFraction_tbl[];

#endif //__umc_vc1_common_tables_H__
