//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#include "umc_config.h"
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER

#include "umc_h264_tables.h"

const Ipp8u EdgePelCountTable [52] =
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,4,4
};

const Ipp8u EdgePelDiffTable [52] =
{
    0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,10,10,11,12,13,14,15,16,17,18,19,20,21
// QP/2
    //0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,16,16,17,17,18,18,19,19,20,20,21,21,22,22,23,23,24,24,25
// QP/3
    //0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8,9,9,9,10,10,10,11,11,11,12,12,12,13,13,13,14,14,14,15,15,15,16,16,16,17
// QP/4
    //0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,11,12,12,12,12
};

const Ipp32s QP_DIV_6[88] = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9,
    10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13,
    13, 13, 14, 14, 14, 14
};

const Ipp32s QP_MOD_6[88] = {
    0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5,
    0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5,
    0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3
};

const Ipp16s MAX_PIX_VALUE[5] = { 255, 511, 1023, 2047, 4095};
const Ipp8u block_subblock_mapping_[16] =
    {0,1,4,5,2,3,6,7,8,9,12,13,10,11,14,15};

const Ipp8u xoff[16] = {0,4,0,4,8,12,8,12,0,4,0, 4, 8,12, 8,12};
const Ipp8u yoff[16] = {0,0,4,4,0, 0,4,4, 8,8,12,12,8, 8,12,12};

// Offset for 8x8 blocks
const Ipp8u xoff8[4] = {0,8,0,8};
const Ipp8u yoff8[4] = {0,0,8,8};

// Offset for 16x16 block
const Ipp8u yoff16[1] = {0};

// Offsets to advance from one luma subblock to the next, using 8x8
// block ordering of subblocks (ie, subblocks 0..3 are the subblocks
// in 8x8 block 0. Table is indexed by current subblock, containing a
// pair of values for each. The first value is the x offset to be added,
// the second value is the pitch multiplier to use to add the y offset.
const Ipp8s xyoff_[16][2] = {
    {4,0},{-4,4},{4,0},{4,-4},
    {4,0},{-4,4},{4,0},{-12,4},
    {4,0},{-4,4},{4,0},{4,-4},
    {4,0},{-4,4},{4,0},{-12,4}
};

//////////////////////////////////////////////////////////
// scan matrices, for Run Length Encoding
const Ipp16u bit_index_mask[16] = {
    0xfffe, 0xfffd, 0xfffb, 0xfff7,
    0xffef, 0xffdf, 0xffbf, 0xff7f,
    0xfeff, 0xfdff, 0xfbff, 0xf7ff,
    0xefff, 0xdfff, 0xbfff, 0x7fff
};

// chroma QP mapping
const Ipp8u QPtoChromaQP[52] =
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,29,30,31,32,32,33,
    34,34,35,35,36,36,37,37,37,38,38,38,39,39,39,39};

// 4x4 coded block CBP flags/masks
const Ipp32u CBP4x4Mask[32] =
{
    0x00000001, 0x00000002, 0x00000004, 0x00000008,
    0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800,
    0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000,
    0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000,
};

// 8x8 coded block CBP flags/masks
const Ipp32u CBP8x8Mask[12] =
{
    0x00000f,
    0x0000f0,
    0x000f00,
    0x00f000,
    0x010000, 0x020000, 0x040000, 0x080000,
    0x100000, 0x200000, 0x400000, 0x800000,
};

////////////////////////////////////////////////////////
// Mappings from block number in loop to 8x8 block number
const Ipp8u subblock_block_ss[24] =
    {0,0,1,1,0,0,1,1,2,2,3,3,2,2,3,3, /*luma*/
    4,4,4,4,5,5,5,5/*chroma*/};
const Ipp8u subblock_block_ds[32] =
    {0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,2,2,2,2,3,3,3,3};

// Mapping from block number in loop to 8x8 block number
const Ipp8u subblock_block_mapping[16] =
    {0,0,1,1,0,0,1,1,2,2,3,3,2,2,3,3};

// chroma block re-mapping
const Ipp8u subblock_block_chroma[8] = {4,4,4,4,5,5,5,5};

// Mapping from chroma block number to luma block number where the vector
// to be used for chroma can be found
const Ipp8u chroma_block_address[4] = {0,2,8,10};


// Table used to prevent single or 'expensive' coefficients are coded
const Ipp8u coeff_importance[16] = {3,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0};
const Ipp8u coeff_importance8x8[64] = {3,3,3,3,2,2,2,2,2,2,2,2,1,1,1,1,
                                       1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
                                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const Ipp32s COEFF_BLOCK_THRESHOLD = 3;
const Ipp32s COEFF_MB_THRESHOLD = 5;
// The purpose of the thresholds is to prevent that single or
// 'expensive' coefficients are coded. With 4x4 transform there is
// larger chance that a single coefficient in a 8x8 or 16x16 block
// may be nonzero. A single small (level=1) coefficient in a 8x8 block
// will cost in bits: 3 or more bits for the coefficient, 4 bits for
// EOBs for the 4x4 blocks, possibly also more bits for CBP.  Hence
// the total 'cost' of that single coefficient will typically be 10-12
// bits which in a RD consideration is too much to justify the Distortion
// improvement.  The action below is to watch such 'single' coefficients
// and set the reconstructed block equal to the prediction according to
// a given criterium.  The action is taken only for inter blocks. Action
// is taken only for luma blocks. Notice that this is a pure encoder issue
// and hence does not have any implication on the standard.

// i22 is a parameter set in dct4 and accumulated for each 8x8 block.
// If level=1 for a coefficient, i22 is increased by a number depending
// on RUN for that coefficient.  The numbers are (see also dct4):
// 3,2,2,1,1,1,0,0,... when RUN equals 0,1,2,3,4,5,6,  etc.
// If level >1 i22 is increased by 9 (or any number above 3).
// The threshold is set to 3.  This means for example:
//    1: If there is one coefficient with (RUN,level)=(0,1) in a
//       8x8 block this coefficient is discarded.
//    2: If there are two coefficients with (RUN,level)=(1,1) and
//       (4,1) the coefficients are also discarded

// i33 is the accumulation of i22 over a whole macroblock.  If i33 is
// 5 or less for the whole MB, all nonzero coefficients are discarded
// for the MB and the reconstructed block is set equal to the prediction.
//
// Search for i22 and i33 to see how the thresholds are used

// Encoder tables for coefficient encoding
const Ipp8u enc_levrun_inter[16] =
    {4,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0};

const Ipp8u enc_levrun_intra[8] =
    {9,3,1,1,1,0,0,0};

const Ipp8u enc_ntab_inter[10][4] = {
    { 1, 7,15,29},
    { 3,17, 0, 0},
    { 5,19, 0, 0},
    { 9, 0, 0, 0},
    {11, 0, 0, 0},
    {13, 0, 0, 0},
    {21, 0, 0, 0},
    {23, 0, 0, 0},
    {25, 0, 0, 0},
    {27, 0, 0, 0},
};

const Ipp8u enc_ntab_intra[5][9] = {
    { 1, 5, 9,11,13,23,25,27,29},
    { 3,19,21, 0, 0, 0, 0, 0, 0},
    { 7, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0},
    {17, 0, 0, 0, 0, 0, 0, 0, 0}
};

const Ipp8u enc_ntab_cDC[2][2] = { {1,3}, {5,0} };
const Ipp8u enc_levrun_cDC[4] = { 2,1,0,0 };

////////////////////////////////////////////////////////////////////
// Translation from block number in chroma loop to
// actual chroma block number
const Ipp8u block_trans[8] = {16,17,18,19,20,21,22,23};

/////////////////////////////////////////////////////////////////////
// RD multiplier (Lagrange factor)

// Smoothed P frame version
const Ipp16u rd_quant[52] = {
     13,  13,  13,  14,
     18,  21,  23,  26,
     28,  29,  29,  29,
     29,  29,  29,  32,
     34,  36,  38,  39,
     39,  40,  42,  43,
     43,  44,  46,  48,
     50,  52,  56,  62,
     74,  86,  86,  99,
    111, 111, 119, 133,
    148, 153, 165, 176,
    188, 202, 232, 248,
    314, 347, 384, 453
};

const Ipp16u rd_quant_intra[52] = {
     1, 1, 1, 1,
     1, 1, 1, 1,
     1, 1, 1, 1,
     1, 1, 2, 2,
     7, 12, 14, 26,
     46, 54, 65, 74,
     95, 103, 115, 123,
     182, 224, 275, 314,
     409, 423, 423, 516,
     588, 692, 898, 1021,
     1125, 1365, 1410, 2006,
     2590, 2590, 5395, 5395,
     7185, 9268, 11951, 14010
};

const Ipp16u rd_quant_intra_min[52] = {
    25, 44, 49, 49,
    113, 144, 106, 188,
    129, 135, 263, 247,
    290, 151, 121, 175,
    234, 329, 356, 484,
    501, 529, 685, 714,
    795, 719, 649, 761,
    804, 990, 1210, 1206,
    1805, 1744, 1488, 1731,
    1404, 1138, 2060, 2768,
    3139, 3294, 3284, 3628,
    4008, 4205, 4645, 7605,
    8485, 28887, 32000, 32000
};

// empty threshold tables

// All 4x4 blocks of a 16x16 block with a SAD below this
// QP-dependent threshold will be classified as empty.
const Ipp32s EmptyThreshold[52] = {
    3, 4, 20, 26,
    31, 41, 47, 59,
    71, 77, 86, 95,
    103, 104, 106, 107,
    127, 134, 141, 167,
    185, 198, 205, 212,
    234, 294, 304, 314,
    325, 335, 346, 357,
    409, 461, 540, 653,
    741, 765, 790, 815,
    840, 1045, 1259, 1340,
    1556, 1772, 2006, 2069,
    2102, 2134, 2569, 3290
};

const IppiSize size16x16={16,16};
const IppiSize size16x8={16,8};
const IppiSize size8x16={8,16};
const IppiSize size8x8={8,8};
const IppiSize size8x4={8,4};
const IppiSize size4x8={4,8};
const IppiSize size4x4={4,4};
const IppiSize size4x2={4,2};
const IppiSize size2x4={2,4};
const IppiSize size2x2={2,2};

// Tuned for Stuart & MIB

const Ipp32u DirectBSkipMEThres[52] =
{
    4, 4, 5, 13,
    14, 14, 21, 26,
    26, 29, 31, 33,
    35, 35, 37, 37,
    39, 47, 47, 47,
    82, 88, 106, 131,
    141, 186, 214, 214,
    214, 221, 262, 310,
    332, 332, 343, 510,
    510, 601, 780, 860,
    1043, 1225, 1306, 1533,
    1533, 1635, 1859, 1859,
    2045, 2180, 2180, 2251
};

// Not Tuned!!!

const Ipp32u PSkipMEThres[52] =
{
    1, 1, 1, 1,
    1, 2, 3, 3,
    5, 7, 9, 12,
    16, 16, 19, 23,
    28, 35, 43, 52,
    63, 76, 86, 97,
    110, 107, 116, 125,
    134, 147, 167, 189,
    215, 265, 310, 362,
    424, 496, 563, 639,
    725, 756, 901, 1079,
    1299, 1573, 1914, 2341,
    2876, 5356, 6866, 8800
};

const Ipp32s BestOf5EarlyExitThres[52] =
{
    38, 38, 38, 38,
    38, 38, 38, 38,
    38, 46, 46, 46,
    50, 50, 50, 50,
    60, 60, 60, 68,
    68, 68, 68, 68,
    68, 71, 95, 95,
    95, 107, 153, 176,
    176, 176, 202, 209,
    209, 284, 294, 314,
    359, 371, 383, 383,
    396, 452, 516, 533,
    628, 739, 841, 1020
};


// Tables used for finding if a luma block is on the edge
// of a macroblock. JVT CD block order
// tab4, indexed by 8x8 block
const Ipp8u left_edge_tab4_[4]       = {1,0,1,0};
const Ipp8u top_edge_tab4_[4]        = {1,1,0,0};
const Ipp8u right_edge_tab4[4]      = {0,1,0,1};
// tab16, indexed by 4x4 subblock
const Ipp8u left_edge_tab16_[16]     = {1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0};
const Ipp8u top_edge_tab16_[16]      = {1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0};
const Ipp8u right_edge_tab16[16]    = {0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1};
// 8x4 and 4x8 tables, indexed by [8x8block*4 + subblock]
const Ipp8u left_edge_tab16_8x4[16]     = {1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0};
const Ipp8u top_edge_tab16_8x4[16]      = {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0};
const Ipp8u right_edge_tab16_8x4[16]    = {0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0};
const Ipp8u left_edge_tab16_4x8[16]     = {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
const Ipp8u top_edge_tab16_4x8[16]      = {1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0};
const Ipp8u right_edge_tab16_4x8[16]    = {0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0};

// Tables for MV prediction to find if upper right predictor is
// available, indexed by [8x8block*4 + subblock]
const Ipp8u above_right_avail_8x4[16] = {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
const Ipp8u above_right_avail_4x8[16] = {0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0};

// Table for 4x4 intra prediction to find if a subblock can use predictors
// from above right. Also used for motion vector prediction availability.
// JVT CD block order.
const Ipp8u above_right_avail_4x4_[16] = {1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0};
const Ipp8u above_right_avail_4x4_lin[16] = {
    1,1,1,1,
    1,0,1,0,
    1,1,1,0,
    1,0,1,0
};

// Table for 4x4 intra prediction to find if a subblock can use predictors
// from below left. JVT CD block order.
const Ipp8u intra4x4_below_left_avail[16] = {1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0};

// chroma vector mapping
const Ipp8u c2x2m[4][4] = {{0,0,1,1},{0,0,1,1},{2,2,3,3},{2,2,3,3}};

//////////////////////////////////////////////////////////
// scan matrices
Ipp32s dec_single_scan[2][16] = {
    {0,1,4,8,5,2,3,6,9,12,13,10,7,11,14,15},
    {0,4,1,8,12,5,9,13,2,6,10,14,3,7,11,15}
};

const Ipp32s dec_single_scan_p[4] = {0,1,2,3};
const Ipp32s dec_single_scan_p422[8] = {0,2,1,4,6,3,5,7};

Ipp16s enc_single_scan[2][16] = {
    {0,1,5,6,2,4,7,12,3,8,11,13,9,10,14,15},
    {0,2,8,12,1,5,9,13,3,6,10,14,4,7,11,15}
};

const Ipp32s dec_single_scan_8x8[2][64] = {
    {0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5, 12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6, 7, 14, 21, 28, 35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51, 58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63},
    {0, 8, 16, 1, 9, 24, 32, 17, 2, 25, 40, 48, 56, 33, 10, 3, 18, 41, 49, 57, 26, 11, 4, 19, 34, 42, 50, 58, 27, 12, 5, 20, 35, 43, 51, 59, 28, 13, 6, 21, 36, 44, 52, 60, 29, 14, 22, 37, 45, 53, 61, 30, 7, 15, 38, 46, 54, 62, 23, 31, 39, 47, 55, 63}
};

Ipp16s enc_single_scan_8x8[2][64] = {
    { 0, 1, 5, 6, 14, 15, 27, 28, 2, 4, 7, 13, 16, 26, 29, 42, 3, 8, 12, 17, 25, 30, 41, 43, 9, 11, 18, 24, 31, 40, 44, 53, 10, 19, 23, 32, 39, 45, 52, 54, 20, 22, 33, 38, 46, 51, 55, 60, 21, 34, 37, 47, 50, 56, 59, 61, 35, 36, 48, 49, 57, 58, 62, 63},
    { 0, 3, 8, 15, 22, 30, 38, 52, 1, 4, 14, 21, 29, 37, 45, 53, 2, 7, 16, 23, 31, 39, 46, 58, 5, 9, 20, 28, 36, 44, 51, 59, 6, 13, 24, 32, 40, 47, 54, 60, 10, 17, 25, 33, 41, 48, 55, 61, 11, 18, 26, 34, 42, 49, 56, 62, 12, 19, 27, 35, 43, 50, 57, 63}
};

const Ipp32s gc_Zeroes[8*8] = { 0 };

const Ipp8u MbPartWidth[NUMBER_OF_MBTYPES] = {
    16, // MBTYPE_INTRA
    16, // MBTYPE_INTRA_16x16 = 1,
    16, // MBTYPE_PCM = 2,             // Raw Pixel Coding, qualifies as a INTRA type...
    16, // MBTYPE_INTER = 3,           // 16x16
    16, // MBTYPE_INTER_16x8 = 4,
     8, // MBTYPE_INTER_8x16 = 5,
     8, // MBTYPE_INTER_8x8 = 6,
     8, // MBTYPE_INTER_8x8_REF0 = 7,  // same as MBTYPE_INTER_8x8, with all RefIdx=0
    16, // MBTYPE_FORWARD = 8,
    16, // MBTYPE_BACKWARD = 9,
    16, // MBTYPE_SKIPPED = 10,
    16, // MBTYPE_DIRECT = 11,
    16, // MBTYPE_BIDIR = 12,
    16, // MBTYPE_FWD_FWD_16x8 = 13,
     8, // MBTYPE_FWD_FWD_8x16 = 14,
    16, // MBTYPE_BWD_BWD_16x8 = 15,
     8, // MBTYPE_BWD_BWD_8x16 = 16,
    16, // MBTYPE_FWD_BWD_16x8 = 17,
     8, // MBTYPE_FWD_BWD_8x16 = 18,
    16, // MBTYPE_BWD_FWD_16x8 = 19,
     8, // MBTYPE_BWD_FWD_8x16 = 20,
    16, // MBTYPE_BIDIR_FWD_16x8 = 21,
     8, // MBTYPE_BIDIR_FWD_8x16 = 22,
    16, // MBTYPE_BIDIR_BWD_16x8 = 23,
     8, // MBTYPE_BIDIR_BWD_8x16 = 24,
    16, // MBTYPE_FWD_BIDIR_16x8 = 25,
     8, // MBTYPE_FWD_BIDIR_8x16 = 26,
    16, // MBTYPE_BWD_BIDIR_16x8 = 27,
     8, // MBTYPE_BWD_BIDIR_8x16 = 28,
    16, // MBTYPE_BIDIR_BIDIR_16x8 = 29,
     8, // MBTYPE_BIDIR_BIDIR_8x16 = 30,
     8  // MBTYPE_B_8x8 = 31,
};

const Ipp8u MbPartHeight[NUMBER_OF_MBTYPES] = {
    16, // MBTYPE_INTRA
    16, // MBTYPE_INTRA_16x16 = 1,
    16, // MBTYPE_PCM = 2,             // Raw Pixel Coding, qualifies as a INTRA type...
    16, // MBTYPE_INTER = 3,           // 16x16
     8, // MBTYPE_INTER_16x8 = 4,
    16, // MBTYPE_INTER_8x16 = 5,
     8, // MBTYPE_INTER_8x8 = 6,
     8, // MBTYPE_INTER_8x8_REF0 = 7,  // same as MBTYPE_INTER_8x8, with all RefIdx=0
    16, // MBTYPE_FORWARD = 8,
    16, // MBTYPE_BACKWARD = 9,
    16, // MBTYPE_SKIPPED = 10,
    16, // MBTYPE_DIRECT = 11,
    16, // MBTYPE_BIDIR = 12,
     8, // MBTYPE_FWD_FWD_16x8 = 13,
    16, // MBTYPE_FWD_FWD_8x16 = 14,
     8, // MBTYPE_BWD_BWD_16x8 = 15,
    16, // MBTYPE_BWD_BWD_8x16 = 16,
     8, // MBTYPE_FWD_BWD_16x8 = 17,
    16, // MBTYPE_FWD_BWD_8x16 = 18,
     8, // MBTYPE_BWD_FWD_16x8 = 19,
    16, // MBTYPE_BWD_FWD_8x16 = 20,
     8, // MBTYPE_BIDIR_FWD_16x8 = 21,
    16, // MBTYPE_BIDIR_FWD_8x16 = 22,
     8, // MBTYPE_BIDIR_BWD_16x8 = 23,
    16, // MBTYPE_BIDIR_BWD_8x16 = 24,
     8, // MBTYPE_FWD_BIDIR_16x8 = 25,
    16, // MBTYPE_FWD_BIDIR_8x16 = 26,
     8, // MBTYPE_BWD_BIDIR_16x8 = 27,
    16, // MBTYPE_BWD_BIDIR_8x16 = 28,
     8, // MBTYPE_BIDIR_BIDIR_16x8 = 29,
    16, // MBTYPE_BIDIR_BIDIR_8x16 = 30,
     8  // MBTYPE_B_8x8 = 31,
};

const Ipp8u DefaultScalingList4x4[2][16] = {
    {6,13,20,28,13,20,28,32,20,28,32,37,28,32,37,42},
    {10,14,20,24,14,20,24,27,20,24,27,30,24,27,30,34}
};

const Ipp8u FlatScalingList4x4[16] = {
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16
};

const Ipp8u DefaultScalingList8x8[2][64] = {
    { 6,10,13,16,18,23,25,27,10,11,16,18,23,25,27,29,
     13,16,18,23,25,27,29,31,16,18,23,25,27,29,31,33,
     18,23,25,27,29,31,33,36,23,25,27,29,31,33,36,38,
     25,27,29,31,33,36,38,40,27,29,31,33,36,38,40,42},
    { 9,13,15,17,19,21,22,24,13,13,17,19,21,22,24,25,
     15,17,19,21,22,24,25,27,17,19,21,22,24,25,27,28,
     19,21,22,24,25,27,28,30,21,22,24,25,27,28,30,32,
     22,24,25,27,28,30,32,33,24,25,27,28,30,32,33,35}
};

const Ipp8u FlatScalingList8x8[64] = {
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16
};

const Ipp8u SubWidthC[4]  = { 1, 2, 2, 1 };
const Ipp8u SubHeightC[4] = { 1, 2, 1, 1 };

Ipp8u BitsForMV [BITSFORMV_OFFSET + 1 + BITSFORMV_OFFSET];

static const Ipp8u s_BitsForMV[128] = {
         1, 3, 5, 5, 7, 7, 7, 7, 9, 9, 9, 9, 9, 9, 9, 9,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
        13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
        15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
        15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15
};

class BitsForMV_Initializer
{
public:
    BitsForMV_Initializer ()
    {
        for (Ipp32s qp = 0; qp < 52; qp ++)
            for (Ipp32s mvl = 0; mvl <= BITSFORMV_OFFSET; mvl ++) {
                if (mvl < 128)
                    BitsForMV[BITSFORMV_OFFSET + mvl] = BitsForMV[BITSFORMV_OFFSET - mvl] = s_BitsForMV[mvl];
                else
                    BitsForMV[BITSFORMV_OFFSET + mvl] = BitsForMV[BITSFORMV_OFFSET - mvl] = s_BitsForMV[mvl >> 7] + 14;
            }
    }
};

BitsForMV_Initializer initBitsForMV;


Ipp16s glob_RDQM[52][BITSMAX];

static const Ipp32s qp_lambda[40] = // pow(2, qp/6)
{
    1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 6, 6, 7, 8, 9, 10, 11, 13, 14, 16, 18, 20, 23, 25, 29, 32, 36, 40, 45, 51, 57, 64, 72, 81, 91
};

class RDQM_Initializer
{
public:
    RDQM_Initializer ()
    {
        for (Ipp32s qp = 0; qp < 52; qp ++)
            for (Ipp32s bits = 0; bits < BITSMAX; bits ++)
                glob_RDQM[qp][bits] = (Ipp16s)(qp_lambda[MAX(0, qp - 12)] * bits);
    }
};

RDQM_Initializer initRDQM;

#if 0
/* int( (pow(2.0,MAX(0,(iQP-12))/3.0)*0.85) + 0.5) */
const Ipp32s lambda_sq[87] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 3, 3, 4, 5, 7, 9, 11, 14, 17, 22, 27,
    34, 43, 54, 69, 86, 109, 137, 173, 218, 274,
    345, 435, 548, 691, 870, 1097, 1382, 1741,
    2193, 2763, 3482, 4387, 5527, 6963, 8773,
    11053, 13926, 17546, 22107, 27853, 35092,
    44214, 55706, 70185, 88427, 111411, 140369,
    176854, 222822, 280739, 353709, 445645, 561477,
    707417, 891290, 1122955, 1414834, 1782579,
    2245909, 2829668, 3565158, 4491818, 5659336,
    7130317, 8983636, 11318672, 14260634, 17967272,
    22637345,
};
#endif

#if 0
/* int( (pow(2.0,iQP)*0.85*2) + 0.5) // lambda*32*/
const int lambda_sq[87] = {
    2,     2,     3,     3,     4,     5,     7,     9,
   11,    14,    17,    22,    27,    34,    43,    54,
   69,    86,   109,   137,   173,   218,   274,   345,
  435,   548,   691,   870,  1097,  1382,  1741,  2193,
 2763,  3482,  4387,  5527,  6963,  8773, 11053, 13926,
17546, 22107, 27853, 35092, 44214, 55706, 70185, 88427,
111411, 140369, 176854, 222822, 280739, 353709, 445645, 561477,
707417, 891290, 1122955, 1414834, 1782579, 2245909, 2829668, 3565158,
4491818, 5659336, 7130317, 8983636, 11318672, 14260634, 17967272, 22637345,
28521267, 35934545, 45274690, 57042534, 71869090, 90549379, 114085069, 143738180,
181098758, 228170138, 287476359, 362197516, 456340275, 574952719, 724395033
};
#endif

//lambda*32
const int lambda_sq[87] = {
    2,     2,     3,     3,     4,     5,     7,     9,
   11,    14,    17,    22,    27,    34,    43,    54,
   69,    86,   109,   137,   160,   224,   288,   352,
  448,   544,   704,   864,  1088,  1376,  1728,  2208,
 2752,  3488,  4384,  5536,  6976,  8768, 11040, 13920,
17536, 22112, 27840, 35104, 44224, 55712, 70176, 88416,
111424, 140384, 176864, 222816, 280736, 353696, 445632, 561472,
707424, 891296, 1122944, 1414848, 1782592, 2245920, 2829664, 3565152,
4491808, 5659328, 7130304, 8983648, 11318688, 14260640, 17967264, 22637344,
28521280, 35934560, 45274688, 57042528, 71869088, 90549376, 114085056, 143738176,
181098752, 228170144, 287476352, 362197504, 456340288, 574952704, 724395040,
};

/* CABAC trans tables: state (MPS and LPS ) + valMPS in 6th bit */
const Ipp8u transTbl[2][128] = {
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
     15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
     27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
     39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
     51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
     62, 63,  0, 64, 65, 66, 66, 68, 68, 69, 70, 71,
     72, 73, 73, 75, 75, 76, 77, 77, 79, 79, 80, 80,
     82, 82, 83, 83, 85, 85, 86, 86, 87, 88, 88, 89,
     90, 90, 91, 91, 92, 93, 93, 94, 94, 94, 95, 96,
     96, 97, 97, 97, 98, 98, 99, 99, 99, 100, 100,
     100, 101, 101, 101, 102, 102, 127
    },
    { 64, 0, 1, 2, 2, 4, 4, 5, 6, 7, 8, 9, 9, 11, 11,
      12, 13, 13, 15, 15, 16, 16, 18, 18, 19, 19, 21,
      21, 22, 22, 23, 24, 24, 25, 26, 26, 27, 27, 28,
      29, 29, 30, 30, 30, 31, 32, 32, 33, 33, 33, 34,
      34, 35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38,
      63, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
      76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
      88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
      100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
      110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
      120, 121, 122, 123, 124, 125, 126, 126, 127
     }
};

// Intra MB_Type Offset by Slice Type
const Ipp8u IntraMBTypeOffset[5] = {
    5,  // PREDSLICE
    23, // BPREDSLICE
    0,  // INTRASLICE
    5,  // S_PREDSLICE
    1   // S_INTRASLICE
};

#endif //UMC_ENABLE_H264_VIDEO_ENCODER
