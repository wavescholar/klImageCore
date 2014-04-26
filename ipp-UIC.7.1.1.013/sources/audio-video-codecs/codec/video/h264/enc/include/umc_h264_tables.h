//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#ifndef UMC_H264_TABLES_H
#define UMC_H264_TABLES_H

#include "umc_h264_defs.h"


extern const Ipp8u EdgePelCountTable[52];
extern const Ipp8u EdgePelDiffTable[52];

// Division by 6
extern const Ipp32s QP_DIV_6[88];
extern const Ipp32s QP_MOD_6[88];

// Maximum pixel values according to (the bit depth - 8).
extern const Ipp16s MAX_PIX_VALUE[5];
/////////////////////////////////////////////////////////
// offsets for 4x4 blocks
extern const Ipp8u xoff[16];
extern const Ipp8u yoff[16];
extern const Ipp8s xyoff_[16][2];

// Offset for 8x8 blocks
extern const Ipp8u xoff8[4];
extern const Ipp8u yoff8[4];

// Offset for 16x16 block
extern const Ipp8u yoff16[1];

//////////////////////////////////////////////////////////
// scan matrices
//extern  const Ipp8u single_scan[16];
//extern  const Ipp8u dec_single_scan[16];

// bit index mask used in RLE
extern const Ipp16u bit_index_mask[16];

//////////////////////////////////////////////////////////
// Mapping from luma QP to chroma QP
extern const Ipp8u QPtoChromaQP[52];

extern const Ipp32u CBP4x4Mask[32];
extern const Ipp32u CBP8x8Mask[12];

// CBP4x4 bits to map 8x8 block to corresponding luma 4x4 blocks
#define CBP4x4_FROM8x8_0    0x000f  // luma 0,1,2,3
#define CBP4x4_FROM8x8_1    0x00f0  // luma 4,5,6,7
#define CBP4x4_FROM8x8_2    0x0f00  // luma 8,9,10,11
#define CBP4x4_FROM8x8_3    0xf000  // luma 12,13,14,15

////////////////////////////////////////////////////////
// Mappings from block number in loop to 8x8 block number
extern const Ipp8u subblock_block_ss[24];
extern const Ipp8u subblock_block_ds[32];

// Mapping from 8x8 block number to 4x4 block number
extern const Ipp8u block_subblock_mapping_[16];

// Mapping from block number in loop to 8x8 block number
extern const Ipp8u subblock_block_mapping[16];

// chroma block re-mapping
extern const Ipp8u subblock_block_chroma[8];

// Mapping from chroma block number to luma block number where the vector
// to be used for chroma can be found
extern const Ipp8u chroma_block_address[4];

// Encoder tables for coefficient encoding
extern const Ipp8u enc_levrun_inter[16];
extern const Ipp8u enc_levrun_intra[8];
extern const Ipp8u enc_ntab_inter[10][4];
extern const Ipp8u enc_ntab_intra[5][9];
extern const Ipp8u enc_ntab_cDC[2][2];
extern const Ipp8u enc_levrun_cDC[4];

////////////////////////////////////////////////////////////////////
// Translation from block number in chroma loop to
// actual chroma block number
extern const Ipp8u block_trans[8];

/////////////////////////////////////////////////////////////////////
// RD multiplier (Lagrange factor)
extern const Ipp16u rd_quant[52];
extern const Ipp16u rd_quant_intra[52];
extern const Ipp16u rd_quant_intra_min[52];

// Table used to prevent single or 'expensive' coefficients are coded
extern const Ipp8u coeff_importance[16];
extern const Ipp8u coeff_importance8x8[64];
extern const Ipp32s COEFF_BLOCK_THRESHOLD;
extern const Ipp32s COEFF_MB_THRESHOLD ;

// Tables used for finding if a block is on the edge
// of a macroblock
extern const Ipp8u left_edge_tab4_[4];
extern const Ipp8u top_edge_tab4_[4];
extern const Ipp8u right_edge_tab4[4];
extern const Ipp8u left_edge_tab16_[16];
extern const Ipp8u top_edge_tab16_[16];
extern const Ipp8u right_edge_tab16[16];
extern const Ipp8u left_edge_tab16_8x4[16];
extern const Ipp8u top_edge_tab16_8x4[16];
extern const Ipp8u right_edge_tab16_8x4[16];
extern const Ipp8u left_edge_tab16_4x8[16];
extern const Ipp8u top_edge_tab16_4x8[16];
extern const Ipp8u right_edge_tab16_4x8[16];

extern const Ipp8u above_right_avail_8x4[16];
extern const Ipp8u above_right_avail_4x8[16];
extern const Ipp8u above_right_avail_4x4_[16];
extern const Ipp8u above_right_avail_4x4_lin[16];
extern const Ipp8u intra4x4_below_left_avail[16];

// chroma vector mapping
extern  const Ipp8u c2x2m[4][4];

// empty threshold table
extern const Ipp32s EmptyThreshold[52];

// encode Shortcut thresh tables
extern const Ipp32u DirectBSkipMEThres[52];
extern const Ipp32u PSkipMEThres[52];
extern const Ipp32s BestOf5EarlyExitThres[52];

//////////////////////////////////////////////////////////
// scan matrices
extern Ipp32s dec_single_scan[2][16];
extern const Ipp32s dec_single_scan_p[4];
extern const Ipp32s dec_single_scan_p422[8];
extern Ipp16s enc_single_scan[2][16];
extern const Ipp32s dec_single_scan_8x8[2][64];
extern Ipp16s enc_single_scan_8x8[2][64];

extern const Ipp8u SubWidthC[4];
extern const Ipp8u SubHeightC[4];

// gc_Zeroes is a constant array of zeroes.  It is useful for the decoder's
// reconstruction functions, to quickly emulate a non-coded block or subblock.
// The "gc_" prefix indicates this is a global, constant table.
// Because it's constant, you can use it in contexts expecting other
// than an Ipp32s, via an appropriate cast.

extern const Ipp32s gc_Zeroes[8*8];

// MB part widths and heights according to mb_type.
extern const Ipp8u MbPartWidth[NUMBER_OF_MBTYPES];
extern const Ipp8u MbPartHeight[NUMBER_OF_MBTYPES];

extern const IppiSize size16x16;
extern const IppiSize size16x8;
extern const IppiSize size8x16;
extern const IppiSize size8x8;
extern const IppiSize size8x4;
extern const IppiSize size4x8;
extern const IppiSize size4x4;
extern const IppiSize size4x2;
extern const IppiSize size2x4;
extern const IppiSize size2x2;

//Default scaling matrices
extern const Ipp8u DefaultScalingList4x4[2][16];
extern const Ipp8u DefaultScalingList8x8[2][64];
//Flat scaling matrices
extern const Ipp8u FlatScalingList4x4[16];
extern const Ipp8u FlatScalingList8x8[64];

#define BITSFORMV_OFFSET ((MAX_MV_INT + 1) * 4 * 3)
#define BITSMAX 60
extern Ipp16s glob_RDQM[52][BITSMAX];
extern Ipp8u BitsForMV [BITSFORMV_OFFSET + 1 + BITSFORMV_OFFSET];

extern const Ipp32s lambda_sq[87];
extern const Ipp8u transTbl[2][128];

extern const Ipp8u IntraMBTypeOffset[5];

#endif // UMC_H264_TABLES_H
