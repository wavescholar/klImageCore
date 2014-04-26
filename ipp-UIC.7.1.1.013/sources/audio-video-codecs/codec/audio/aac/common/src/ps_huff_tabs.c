/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include "ps_dec_settings.h"
/* sbr header contains SBR & PS huff tabs */
#include "sbr_huff_tabs.h"

/*******************************************************************************/
// IPP HUFFMAN TABLES
//

Ipp32s vlcPsTableSizes[]   = { 61, 61, 29, 29, 15, 15, 8, 8, 8, 8 };
Ipp32s vlcPsNumSubTables[] = {3, 3, 3, 4, 2, 2, 1, 1, 1, 1};

static Ipp32s vlcPsSubTablesSizes0[]  = {6, 6, 6};
static Ipp32s vlcPsSubTablesSizes1[]  = {5, 5, 6};
static Ipp32s vlcPsSubTablesSizes2[]  = {6, 6, 6};
static Ipp32s vlcPsSubTablesSizes3[]  = {5, 5, 5, 5};
static Ipp32s vlcPsSubTablesSizes4[]  = {7, 7};
static Ipp32s vlcPsSubTablesSizes5[]  = {7, 7};
static Ipp32s vlcPsSubTablesSizes6[]  = {4};
static Ipp32s vlcPsSubTablesSizes7[]  = {5};
static Ipp32s vlcPsSubTablesSizes8[]  = {5};
static Ipp32s vlcPsSubTablesSizes9[]  = {5};

Ipp32s *vlcPsSubTablesSizes[] = {
  vlcPsSubTablesSizes0, vlcPsSubTablesSizes1, vlcPsSubTablesSizes2,
  vlcPsSubTablesSizes3, vlcPsSubTablesSizes4, vlcPsSubTablesSizes5,
  vlcPsSubTablesSizes6, vlcPsSubTablesSizes7, vlcPsSubTablesSizes8,
  vlcPsSubTablesSizes9
};

// [0]
static IppsVLCTable_32s huff_iid_df_0[] = {
  {-30, 0x1feb4, 18}, {-29, 0x1feb5, 18}, {-28, 0x1fd76, 18},
  {-27, 0x1fd77, 18}, {-26, 0x1fd74, 18}, {-25, 0x1fd75, 18},
  {-24, 0x1fe8a, 18}, {-23, 0x1fe8b, 18}, {-22, 0x1fe88, 18},
  {-21, 0xfe80, 17},  {-20, 0x1feb6, 18}, {-19, 0xfe82, 17},
  {-18, 0xfeb8, 17},  {-17, 0x7f42, 16},  {-16, 0x7fae, 16},
  {-15, 0x3faf, 15},  {-14, 0x1fd1, 14},  {-13, 0x1fe9, 14},
  {-12, 0xfe9, 13},   {-11, 0x7ea, 12},   {-10, 0x7fb, 12},
  {-9, 0x3fb, 11},    {-8, 0x1fb, 10},    {-7, 0x1ff, 10},
  {-6, 0x7c, 8},      {-5, 0x3c, 7},      {-4, 0x1c, 6},
  {-3, 0xc, 5},       {-2, 0, 4},         {-1, 0x1, 3},
  {0, 0x1, 1},        {1, 0x2, 3},        {2, 0x1, 4},
  {3, 0xd, 5},        {4, 0x1d, 6},       {5, 0x3d, 7},
  {6, 0x7d, 8},       {7, 0xfc, 9},       {8, 0x1fc, 10},
  {9, 0x3fc, 11},     {10, 0x3f4, 11},    {11, 0x7eb, 12},
  {12, 0xfea, 13},    {13, 0x1fea, 14},   {14, 0x1fd6, 14},
  {15, 0x3fd0, 15},   {16, 0x7faf, 16},   {17, 0x7f43, 16},
  {18, 0xfeb9, 17},   {19, 0xfe83, 17},   {20, 0x1feb7, 18},
  {21, 0xfe81, 17},   {22, 0x1fe89, 18},  {23, 0x1fe8e, 18},
  {24, 0x1fe8f, 18},  {25, 0x1fe8c, 18},  {26, 0x1fe8d, 18},
  {27, 0x1feb2, 18},  {28, 0x1feb3, 18},  {29, 0x1feb0, 18},
  {30, 0x1feb1, 18},
};

//[1]
static IppsVLCTable_32s huff_iid_dt_0[] = {
  {-30, 0x4ed4, 16}, {-29, 0x4ed5, 16}, {-28, 0x4ece, 16},
  {-27, 0x4ecf, 16}, {-26, 0x4ecc, 16}, {-25, 0x4ed6, 16},
  {-24, 0x4ed8, 16}, {-23, 0x4f46, 16}, {-22, 0x4f60, 16},
  {-21, 0x2718, 15}, {-20, 0x2719, 15}, {-19, 0x2764, 15},
  {-18, 0x2765, 15}, {-17, 0x276d, 15}, {-16, 0x27b1, 15},
  {-15, 0x13b7, 14}, {-14, 0x13d6, 14}, {-13, 0x9c7, 13},
  {-12, 0x9e9, 13},  {-11, 0x9ed, 13},  {-10, 0x4ee, 12},
  {-9, 0x4f7, 12},   {-8, 0x278, 11},   {-7, 0x139, 10},
  {-6, 0x9a, 9},     {-5, 0x9f, 9},     {-4, 0x20, 7},
  {-3, 0x11, 6},     {-2, 0xa, 5},      {-1, 0x3, 3},
  {0, 0x1, 1},       {1, 0, 2},         {2, 0xb, 5},
  {3, 0x12, 6},      {4, 0x21, 7},      {5, 0x4c, 8},
  {6, 0x9b, 9},      {7, 0x13a, 10},    {8, 0x279, 11},
  {9, 0x270, 11},    {10, 0x4ef, 12},   {11, 0x4e2, 12},
  {12, 0x9ea, 13},   {13, 0x9d8, 13},   {14, 0x13d7, 14},
  {15, 0x13d0, 14},  {16, 0x27b2, 15},  {17, 0x27a2, 15},
  {18, 0x271a, 15},  {19, 0x271b, 15},  {20, 0x4f66, 16},
  {21, 0x4f67, 16},  {22, 0x4f61, 16},  {23, 0x4f47, 16},
  {24, 0x4ed9, 16},  {25, 0x4ed7, 16},  {26, 0x4ecd, 16},
  {27, 0x4ed2, 16},  {28, 0x4ed3, 16},  {29, 0x4ed0, 16},
  {30, 0x4ed1, 16},
};

// [2]
static IppsVLCTable_32s huff_iid_df_1[] = {
  {-14, 0x1fffb, 17}, {-13, 0x1fffc, 17}, {-12, 0x1fffd, 17},
  {-11, 0x1fffa, 17}, {-10, 0xfffc, 16},  {-9, 0x7ffc, 15},
  {-8, 0x1ffd, 13},   {-7, 0x3fe, 10},    {-6, 0x1fe, 9},
  {-5, 0x7e, 7},      {-4, 0x3c, 6},      {-3, 0x1d, 5},
  {-2, 0xd, 4},       {-1, 0x5, 3},       {0, 0, 1},
  {1, 0x4, 3},        {2, 0xc, 4},        {3, 0x1c, 5},
  {4, 0x3d, 6},       {5, 0x3e, 6},       {6, 0xfe, 8},
  {7, 0x7fe, 11},     {8, 0x1ffc, 13},    {9, 0x3ffc, 14},
  {10, 0x3ffd, 14},   {11, 0x7ffd, 15},   {12, 0x1fffe, 17},
  {13, 0x3fffe, 18},  {14, 0x3ffff, 18},

};

// [3]
static IppsVLCTable_32s huff_iid_dt_1[] = {

  {-14, 0x7fff9, 19}, {-13, 0x7fffa, 19}, {-12, 0x7fffb, 19},
  {-11, 0xffff8, 20}, {-10, 0xffff9, 20}, {-9, 0xffffa, 20},
  {-8, 0x1fffd, 17},  {-7, 0x7ffe, 15},   {-6, 0xffe, 12},
  {-5, 0x3fe, 10},    {-4, 0xfe, 8},      {-3, 0x3e, 6},
  {-2, 0xe, 4},       {-1, 0x2, 2},       {0, 0, 1},
  {1, 0x6, 3},        {2, 0x1e, 5},       {3, 0x7e, 7},
  {4, 0x1fe, 9},      {5, 0x7fe, 11},     {6, 0x1ffe, 13},
  {7, 0x3ffe, 14},    {8, 0x1fffc, 17},   {9, 0x7fff8, 19},
  {10, 0xffffb, 20},  {11, 0xffffc, 20},  {12, 0xffffd, 20},
  {13, 0xffffe, 20},  {14, 0xfffff, 20},
};

// [4]
static IppsVLCTable_32s huff_icc_df[] = {
  {-7, 0x3fff, 14}, {-6, 0x3ffe, 14}, {-5, 0xffe, 12},
  {-4, 0x3fe, 10},  {-3, 0x7e, 7},    {-2, 0x1e, 5},
  {-1, 0x6, 3},     {0, 0, 1},        {1, 0x2, 2},
  {2, 0xe, 4},      {3, 0x3e, 6},     {4, 0xfe, 8},
  {5, 0x1fe, 9},    {6, 0x7fe, 11},   {7, 0x1ffe, 13},

};

// [5]
static IppsVLCTable_32s huff_icc_dt[] = {
  {-7, 0x3ffe, 14}, {-6, 0x1ffe, 13}, {-5, 0x7fe, 11},
  {-4, 0x1fe, 9},   {-3, 0x7e, 7},    {-2, 0x1e, 5},
  {-1, 0x6, 3},     {0, 0, 1},        {1, 0x2, 2},
  {2, 0xe, 4},      {3, 0x3e, 6},     {4, 0xfe, 8},
  {5, 0x3fe, 10},   {6, 0xffe, 12},   {7, 0x3fff, 14},
};

// [6]
static IppsVLCTable_32s huff_ipd_df[] = {
  {0, 0x1, 1}, {1, 0, 3},   {2, 0x6, 4},
  {3, 0x4, 4}, {4, 0x2, 4}, {5, 0x3, 4},
  {6, 0x5, 4}, {7, 0x7, 4},
};

// [7]
static IppsVLCTable_32s huff_ipd_dt[] = {
  {0, 0x1, 1}, {1, 0x2, 3}, {2, 0x2, 4},
  {3, 0x3, 5}, {4, 0x2, 5}, {5, 0, 4},
  {6, 0x3, 4}, {7, 0x3, 3},
};

// [8]
static IppsVLCTable_32s huff_opd_df[] = {
  {0, 0x1, 1}, {1, 0x1, 3}, {2, 0x6, 4},
  {3, 0x4, 4}, {4, 0xf, 5}, {5, 0xe, 5},
  {6, 0x5, 4}, {7, 0, 3},
};

// [9]
static IppsVLCTable_32s huff_opd_dt[] = {
  {0, 0x1, 1}, {1, 0x2, 3}, {2, 0x1, 4},
  {3, 0x7, 5}, {4, 0x6, 5}, {5, 0, 4},
  {6, 0x2, 4}, {7, 0x3, 3},
};

/*******************************************************************************/

IppsVLCTable_32s* vlcPsBooks[] = {
  huff_iid_df_0, //0
  huff_iid_dt_0, //1
  huff_iid_df_1, //2
  huff_iid_dt_1, //3

  huff_icc_df,   //4
  huff_icc_dt,   //5

  huff_ipd_df,   //6
  huff_ipd_dt,   //7

  huff_opd_df,   //8
  huff_opd_dt,   //9
};

/*******************************************************************************/

/* EOF */

#endif //UMC_ENABLE_XXX

