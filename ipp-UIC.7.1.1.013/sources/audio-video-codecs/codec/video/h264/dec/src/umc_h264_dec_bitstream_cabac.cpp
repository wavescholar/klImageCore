/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/
#include "umc_config.h"
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include "umc_h264_video_decoder.h"
#include "umc_h264_bitstream.h"
#include "umc_h264_bitstream_inlines.h"
#include "umc_h264_dec_internal_cabac.h"
#include "vm_types.h"

using namespace UMC;

namespace UMC_H264_DECODER
{

template <class T> inline
T Clip3(T Min, T Max, T Value)
{
    if (Value < Min)
        return Min;
    else if (Value > Max)
        return Max;
    else
        return Value;

} //T Clip3(T Min, T Max, T Value)

static
void InitializeContext(CABAC_CONTEXT *pContext, Ipp16s m, Ipp16s n, Ipp32s SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                       , Ipp32s ctxIdx
#endif
#endif
                       )
{
    Ipp32s preCtxState;

    preCtxState = Clip3(1, 126, ((m * SliceQPy) >> 4) + n);
    if (preCtxState <= 63)
    {
        pContext->pStateIdxAndVal = Ipp8u((63 - preCtxState) * 2);
    }
    else
    {
        pContext->pStateIdxAndVal = Ipp8u((preCtxState - 64) * 2 + 1);
    }
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
    if(cabac_bits==NULL) cabac_bits=fopen(__CABAC_FILE__,"w+t");
    if(cabac_bits)
        fprintf(cabac_bits,"ctx_ini %d %d %d\n",ctxIdx,pContext->pStateIdx,pContext->valMPS);
#endif
#endif

} // void InitializeContext(CABAC_CONTEXT *pContext, Ipp8s m, Ipp8s n, Ipp8u SliceQPy)

struct INITIALIZE_VALUES
{
    Ipp16s m;
    Ipp16s n;
};

// See table 9-12 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_0_10[11] =
{
    {20, -15},
    {2, 54},
    {3, 74},
    {20, -15},
    {2, 54},
    {3, 74},
    {-28, 127},
    {-23, 104},
    {-6, 53},
    {-1, 54},
    {7, 51},
};

// See table 9-17 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_60_69[10] =
{
    {0, 41},
    {0, 63},
    {0, 63},
    {0, 63},
    {-9, 83},
    {4, 86},
    {0, 97},
    {-7, 72},
    {13, 41},
    {3, 62},
};

// See table 9-18 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_70_104_intra[35] =
{
    {0, 11},
    {1, 55},
    {0, 69},
    {-17, 127},
    {-13, 102},
    {0, 82},
    {-7, 74},
    {-21, 107},
    {-27, 127},
    {-31, 127},
    {-24, 127},
    {-18, 95},
    {-27, 127},
    {-21, 114},
    {-30, 127},
    {-17, 123},
    {-12, 115},
    {-16, 122},
    {-11, 115},
    {-12, 63},
    {-2, 68},
    {-15, 84},
    {-13, 104},
    {-3, 70},
    {-8, 93},
    {-10, 90},
    {-30, 127},
    {-1, 74},
    {-6, 97},
    {-7, 91},
    {-20, 127},
    {-4, 56},
    {-5, 82},
    {-7, 76},
    {-22, 125},
};

// See table 9-19 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_105_165_intra[61] =
{
    {-7, 93},
    {-11, 87},
    {-3, 77},
    {-5, 71},
    {-4, 63},
    {-4, 68},
    {-12, 84},
    {-7, 62},
    {-7, 65},
    {8, 61},
    {5, 56},
    {-2, 66},
    {1, 64},
    {0, 61},
    {-2, 78},
    {1, 50},
    {7, 52},
    {10, 35},
    {0, 44},
    {11, 38},
    {1, 45},
    {0, 46},
    {5, 44},
    {31, 17},
    {1, 51},
    {7, 50},
    {28, 19},
    {16, 33},
    {14, 62},
    {-13, 108},
    {-15, 100},
    {-13, 101},
    {-13, 91},
    {-12, 94},
    {-10, 88},
    {-16, 84},
    {-10, 86},
    {-7, 83},
    {-13, 87},
    {-19, 94},
    {1, 70},
    {0, 72},
    {-5, 74},
    {18, 59},
    {-8, 102},
    {-15, 100},
    {0, 95},
    {-4, 75},
    {2, 72},
    {-11, 75},
    {-3, 71},
    {15, 46},
    {-13, 69},
    {0, 62},
    {0, 65},
    {21, 37},
    {-15, 72},
    {9, 57},
    {16, 54},
    {0, 62},
    {12, 72},
};

// See table 9-20 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_166_226_intra[61] =
{
    {24, 0},
    {15, 9},
    {8, 25},
    {13, 18},
    {15, 9},
    {13, 19},
    {10, 37},
    {12, 18},
    {6, 29},
    {20, 33},
    {15, 30},
    {4, 45},
    {1, 58},
    {0, 62},
    {7, 61},
    {12, 38},
    {11, 45},
    {15, 39},
    {11, 42},
    {13, 44},
    {16, 45},
    {12, 41},
    {10, 49},
    {30, 34},
    {18, 42},
    {10, 55},
    {17, 51},
    {17, 46},
    {0, 89},
    {26, -19},
    {22, -17},
    {26, -17},
    {30, -25},
    {28, -20},
    {33, -23},
    {37, -27},
    {33, -23},
    {40, -28},
    {38, -17},
    {33, -11},
    {40, -15},
    {41, -6},
    {38, 1},
    {41, 17},
    {30, -6},
    {27, 3},
    {26, 22},
    {37, -16},
    {35, -4},
    {38, -8},
    {38, -3},
    {37, 3},
    {38, 5},
    {42, 0},
    {35, 16},
    {39, 22},
    {14, 48},
    {27, 37},
    {21, 60},
    {12, 68},
    {2, 97},
};

// See table 9-21 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_227_275_intra[49] =
{
    {-3, 71},
    {-6, 42},
    {-5, 50},
    {-3, 54},
    {-2, 62},
    {0, 58},
    {1, 63},
    {-2, 72},
    {-1, 74},
    {-9, 91},
    {-5, 67},
    {-5, 27},
    {-3, 39},
    {-2, 44},
    {0, 46},
    {-16, 64},
    {-8, 68},
    {-10, 78},
    {-6, 77},
    {-10, 86},
    {-12, 92},
    {-15, 55},
    {-10, 60},
    {-6, 62},
    {-4, 65},
    {-12, 73},
    {-8, 76},
    {-7, 80},
    {-9, 88},
    {-17, 110},
    {-11, 97},
    {-20, 84},
    {-11, 79},
    {-6, 73},
    {-4, 74},
    {-13, 86},
    {-13, 96},
    {-11, 97},
    {-19, 117},
    {-8, 78},
    {-5, 33},
    {-4, 48},
    {-2, 53},
    {-3, 62},
    {-13, 71},
    {-10, 79},
    {-12, 86},
    {-13, 90},
    {-14, 97},
};

// See table 9-22 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_277_337_intra[61] =
{
    {-6, 93},
    {-6, 84},
    {-8, 79},
    {0, 66},
    {-1, 71},
    {0, 62},
    {-2, 60},
    {-2, 59},
    {-5, 75},
    {-3, 62},
    {-4, 58},
    {-9, 66},
    {-1, 79},
    {0, 71},
    {3, 68},
    {10, 44},
    {-7, 62},
    {15, 36},
    {14, 40},
    {16, 27},
    {12, 29},
    {1, 44},
    {20, 36},
    {18, 32},
    {5, 42},
    {1, 48},
    {10, 62},
    {17, 46},
    {9, 64},
    {-12, 104},
    {-11, 97},
    {-16, 96},
    {-7, 88},
    {-8, 85},
    {-7, 85},
    {-9, 85},
    {-13, 88},
    {4, 66},
    {-3, 77},
    {-3, 76},
    {-6, 76},
    {10, 58},
    {-1, 76},
    {-1, 83},
    {-7, 99},
    {-14, 95},
    {2, 95},
    {0, 76},
    {-5, 74},
    {0, 70},
    {-11, 75},
    {1, 68},
    {0, 65},
    {-14, 73},
    {3, 62},
    {4, 62},
    {-1, 68},
    {-13, 75},
    {11, 55},
    {5, 64},
    {12, 70},
};

// See table 9-23 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_338_398_intra[61] =
{
    {15, 6},
    {6, 19},
    {7, 16},
    {12, 14},
    {18, 13},
    {13, 11},
    {13, 15},
    {15, 16},
    {12, 23},
    {13, 23},
    {15, 20},
    {14, 26},
    {14, 44},
    {17, 40},
    {17, 47},
    {24, 17},
    {21, 21},
    {25, 22},
    {31, 27},
    {22, 29},
    {19, 35},
    {14, 50},
    {10, 57},
    {7, 63},
    {-2, 77},
    {-4, 82},
    {-3, 94},
    {9, 69},
    {-12, 109},
    {36, -35},
    {36, -34},
    {32, -26},
    {37, -30},
    {44, -32},
    {34, -18},
    {34, -15},
    {40, -15},
    {33, -7},
    {35, -5},
    {33, 0},
    {38, 2},
    {33, 13},
    {23, 35},
    {13, 58},
    {29, -3},
    {26, 0},
    {22, 30},
    {31, -7},
    {35, -15},
    {34, -3},
    {34, 3},
    {36, -1},
    {34, 5},
    {32, 11},
    {35, 5},
    {34, 12},
    {39, 11},
    {30, 29},
    {34, 26},
    {29, 39},
    {19, 66},
};

static
INITIALIZE_VALUES M_and_N_for_ctxIdx_399_401_intra[3] =
{
    {31,21},
    {31,31},
    {25,50},
};

static
INITIALIZE_VALUES M_and_N_for_ctxIdx_402_459_intra[58] =
{
    {-17,120},
    {-20,112},
    {-18,114},
    {-11, 85},
    {-15, 92},
    {-14, 89},
    {-26, 71},
    {-15, 81},
    {-14, 80},
    {  0, 68},
    {-14, 70},
    {-24, 56},
    {-23, 68},
    {-24, 50},
    {-11, 74},
    { 23,-13},
    { 26,-13},
    { 40,-15},
    { 49,-14},
    { 44,  3},
    { 45,  6},
    { 44, 34},
    { 33, 54},
    { 19, 82},
    { -3, 75},
    { -1, 23},
    {  1, 34},
    {  1, 43},
    {  0, 54},
    { -2, 55},
    {  0, 61},
    {  1, 64},
    {  0, 68},
    { -9, 92},
    {-14,106},
    {-13, 97},
    {-15, 90},
    {-12, 90},
    {-18, 88},
    {-10, 73},
    { -9, 79},
    {-14, 86},
    {-10, 73},
    {-10, 70},
    {-10, 69},
    { -5, 66},
    { -9, 64},
    { -5, 58},
    {  2, 59},
    { 21,-10},
    { 24,-11},
    { 28, -8},
    { 28, -1},
    { 29,  3},
    { 29,  9},
    { 35, 20},
    { 29, 36},
    { 14, 67},
};

// See table 9-13 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_11_23[13][3] =
{
    {{23, 33}, {22, 25}, {29, 16}},
    {{23, 2}, {34, 0}, {25, 0}},
    {{21, 0}, {16, 0}, {14, 0}},
    {{1, 9}, {-2, 9}, {-10, 51}},
    {{0, 49}, {4, 41}, {-3, 62}},
    {{-37, 118}, {-29, 118}, {-27, 99}},
    {{5, 57}, {2, 65}, {26, 16}},
    {{-13, 78}, {-6, 71}, {-4, 85}},
    {{-11, 65}, {-13, 79}, {-24, 102}},
    {{1, 62}, {5, 52}, {5, 57}},
    {{12, 49}, {9, 50}, {6, 57}},
    {{-4, 73}, {-3, 70}, {-17, 73}},
    {{17, 50}, {10, 54}, {14, 57}},
};

// See table 9-14 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_24_39[16][3] =
{
    {{18, 64}, {26, 34}, {20, 40}},
    {{9, 43}, {19, 22}, {20, 10}},
    {{29, 0}, {40, 0}, {29, 0}},
    {{26, 67}, {57, 2}, {54, 0}},
    {{16, 90}, {41, 36}, {37, 42}},
    {{9, 104}, {26, 69}, {12, 97}},
    {{-46, 127}, {-45, 127}, {-32, 127}},
    {{-20, 104}, {-15, 101}, {-22, 117}},
    {{1, 67}, {-4, 76}, {-2, 74}},
    {{-13, 78}, {-6, 71}, {-4, 85}},
    {{-11, 65}, {-13, 79}, {-24, 102}},
    {{1, 62}, {5, 52}, {5, 57}},
    {{-6, 86}, {6, 69}, {-6, 93}},
    {{-17, 95}, {-13, 90}, {-14, 88}},
    {{-6, 61}, {0, 52}, {-6, 44}},
    {{9, 45}, {8, 43}, {4, 55}},
};

// See table 9-15 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_40_53[14][3] =
{
    {{-3, 69}, {-2, 69}, {-11, 89}},
    {{-6, 81}, {-5, 82}, {-15, 103}},
    {{-11, 96}, {-10, 96}, {-21, 116}},
    {{6, 55}, {2, 59}, {19, 57}},
    {{7, 67}, {2, 75}, {20, 58}},
    {{-5, 86}, {-3, 87}, {4, 84}},
    {{2, 88}, {-3, 100}, {6, 96}},
    {{0, 58}, {1, 56}, {1, 63}},
    {{-3, 76}, {-3, 74}, {-5, 85}},
    {{-10, 94}, {-6, 85}, {-13, 106}},
    {{5, 54}, {0, 59}, {5, 63}},
    {{4, 69}, {-3, 81}, {6, 75}},
    {{-3, 81}, {-7, 86}, {-3, 90}},
    {{0, 88}, {-5, 95}, {-1, 101}},
};

// See table 9-16 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_54_59[6][3] =
{
    {{-7, 67}, {-1, 66}, {3, 55}},
    {{-5, 74}, {-1, 77}, {-4, 79}},
    {{-4, 74}, {1, 70}, {-2, 75}},
    {{-5, 80}, {-2, 86}, {-12, 97}},
    {{-7, 72}, {-5, 72}, {-7, 50}},
    {{1, 58}, {0, 61}, {1, 60}},
};

// See table 9-18 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_70_104_inter[35][3] =
{
    {{0, 45}, {13, 15}, {7, 34}},
    {{-4, 78}, {7, 51}, {-9, 88}},
    {{-3, 96}, {2, 80}, {-20, 127}},
    {{-27, 126}, {-39, 127}, {-36, 127}},
    {{-28, 98}, {-18, 91}, {-17, 91}},
    {{-25, 101}, {-17, 96}, {-14, 95}},
    {{-23, 67}, {-26, 81}, {-25, 84}},
    {{-28, 82}, {-35, 98}, {-25, 86}},
    {{-20, 94}, {-24, 102}, {-12, 89}},
    {{-16, 83}, {-23, 97}, {-17, 91}},
    {{-22, 110}, {-27, 119}, {-31, 127}},
    {{-21, 91}, {-24, 99}, {-14, 76}},
    {{-18, 102}, {-21, 110}, {-18, 103}},
    {{-13, 93}, {-18, 102}, {-13, 90}},
    {{-29, 127}, {-36, 127}, {-37, 127}},
    {{-7, 92}, {0, 80}, {11, 80}},
    {{-5, 89}, {-5, 89}, {5, 76}},
    {{-7, 96}, {-7, 94}, {2, 84}},
    {{-13, 108}, {-4, 92}, {5, 78}},
    {{-3, 46}, {0, 39}, {-6, 55}},
    {{-1, 65}, {0, 65}, {4, 61}},
    {{-1, 57}, {-15, 84}, {-14, 83}},
    {{-9, 93}, {-35, 127}, {-37, 127}},
    {{-3, 74}, {-2, 73}, {-5, 79}},
    {{-9, 92}, {-12, 104}, {-11, 104}},
    {{-8, 87}, {-9, 91}, {-11, 91}},
    {{-23, 126}, {-31, 127}, {-30, 127}},
    {{5, 54}, {3, 55}, {0, 65}},
    {{6, 60}, {7, 56}, {-2, 79}},
    {{6, 59}, {7, 55}, {0, 72}},
    {{6, 69}, {8, 61}, {-4, 92}},
    {{-1, 48}, {-3, 53}, {-6, 56}},
    {{0, 68}, {0, 68}, {3, 68}},
    {{-4, 69}, {-7, 74}, {-8, 71}},
    {{-8, 88}, {-9, 88}, {-13, 98}},
};

// See table 9-19 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_105_165_inter[61][3] =
{
    {{-2, 85}, {-13, 103}, {-4, 86}},
    {{-6, 78}, {-13, 91}, {-12, 88}},
    {{-1, 75}, {-9, 89}, {-5, 82}},
    {{-7, 77}, {-14, 92}, {-3, 72}},
    {{2, 54}, {-8, 76}, {-4, 67}},
    {{5, 50}, {-12, 87}, {-8, 72}},
    {{-3, 68}, {-23, 110}, {-16, 89}},
    {{1, 50}, {-24, 105}, {-9, 69}},
    {{6, 42}, {-10, 78}, {-1, 59}},
    {{-4, 81}, {-20, 112}, {5, 66}},
    {{1, 63}, {-17, 99}, {4, 57}},
    {{-4, 70}, {-78, 127}, {-4, 71}},
    {{0, 67}, {-70, 127}, {-2, 71}},
    {{2, 57}, {-50, 127}, {2, 58}},
    {{-2, 76}, {-46, 127}, {-1, 74}},
    {{11, 35}, {-4, 66}, {-4, 44}},
    {{4, 64}, {-5, 78}, {-1, 69}},
    {{1, 61}, {-4, 71}, {0, 62}},
    {{11, 35}, {-8, 72}, {-7, 51}},
    {{18, 25}, {2, 59}, {-4, 47}},
    {{12, 24}, {-1, 55}, {-6, 42}},
    {{13, 29}, {-7, 70}, {-3, 41}},
    {{13, 36}, {-6, 75}, {-6, 53}},
    {{-10, 93}, {-8, 89}, {8, 76}},
    {{-7, 73}, {-34, 119}, {-9, 78}},
    {{-2, 73}, {-3, 75}, {-11, 83}},
    {{13, 46}, {32, 20}, {9, 52}},
    {{9, 49}, {30, 22}, {0, 67}},
    {{-7, 100}, {-44, 127}, {-5, 90}},
    {{9, 53}, {0, 54}, {1, 67}},
    {{2, 53}, {-5, 61}, {-15, 72}},
    {{5, 53}, {0, 58}, {-5, 75}},
    {{-2, 61}, {-1, 60}, {-8, 80}},
    {{0, 56}, {-3, 61}, {-21, 83}},
    {{0, 56}, {-8, 67}, {-21, 64}},
    {{-13, 63}, {-25, 84}, {-13, 31}},
    {{-5, 60}, {-14, 74}, {-25, 64}},
    {{-1, 62}, {-5, 65}, {-29, 94}},
    {{4, 57}, {5, 52}, {9, 75}},
    {{-6, 69}, {2, 57}, {17, 63}},
    {{4, 57}, {0, 61}, {-8, 74}},
    {{14, 39}, {-9, 69}, {-5, 35}},
    {{4, 51}, {-11, 70}, {-2, 27}},
    {{13, 68}, {18, 55}, {13, 91}},
    {{3, 64}, {-4, 71}, {3, 65}},
    {{1, 61}, {0, 58}, {-7, 69}},
    {{9, 63}, {7, 61}, {8, 77}},
    {{7, 50}, {9, 41}, {-10, 66}},
    {{16, 39}, {18, 25}, {3, 62}},
    {{5, 44}, {9, 32}, {-3, 68}},
    {{4, 52}, {5, 43}, {-20, 81}},
    {{11, 48}, {9, 47}, {0, 30}},
    {{-5, 60}, {0, 44}, {1, 7}},
    {{-1, 59}, {0, 51}, {-3, 23}},
    {{0, 59}, {2, 46},{-21, 74}},
    {{22, 33}, {19, 38}, {16, 66}},
    {{5, 44}, {-4, 66}, {-23, 124}},
    {{14, 43}, {15, 38}, {17, 37}},
    {{-1, 78}, {12, 42}, {44, -18}},
    {{0, 60}, {9, 34}, {50, -34}},
    {{9, 69}, {0, 89}, {-22, 127}},
};

// See table 9-20 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_166_226_inter[61][3] =
{
    {{11, 28}, {4, 45}, {4, 39}},
    {{2, 40}, {10, 28}, {0, 42}},
    {{3, 44}, {10, 31}, {7, 34}},
    {{0, 49}, {33, -11}, {11, 29}},
    {{0, 46}, {52, -43}, {8, 31}},
    {{2, 44}, {18, 15}, {6, 37}},
    {{2, 51}, {28, 0}, {7, 42}},
    {{0, 47}, {35, -22}, {3, 40}},
    {{4, 39}, {38, -25}, {8, 33}},
    {{2, 62}, {34, 0}, {13, 43}},
    {{6, 46}, {39, -18}, {13, 36}},
    {{0, 54}, {32, -12}, {4, 47}},
    {{3, 54}, {102, -94}, {3, 55}},
    {{2, 58}, {0, 0}, {2, 58}},
    {{4, 63}, {56, -15}, {6, 60}},
    {{6, 51}, {33, -4}, {8, 44}},
    {{6, 57}, {29, 10}, {11, 44}},
    {{7, 53}, {37, -5}, {14, 42}},
    {{6, 52}, {51, -29}, {7, 48}},
    {{6, 55}, {39, -9}, {4, 56}},
    {{11, 45}, {52, -34}, {4, 52}},
    {{14, 36}, {69, -58}, {13, 37}},
    {{8, 53}, {67, -63}, {9, 49}},
    {{-1, 82}, {44, -5}, {19, 58}},
    {{7, 55}, {32, 7}, {10, 48}},
    {{-3, 78}, {55, -29}, {12, 45}},
    {{15, 46}, {32, 1}, {0, 69}},
    {{22, 31}, {0, 0}, {20, 33}},
    {{-1, 84}, {27, 36}, {8, 63}},
    {{25, 7}, {33, -25}, {35, -18}},
    {{30, -7}, {34, -30}, {33, -25}},
    {{28, 3}, {36, -28}, {28, -3}},
    {{28, 4}, {38, -28}, {24, 10}},
    {{32, 0}, {38, -27}, {27, 0}},
    {{34, -1}, {34, -18}, {34, -14}},
    {{30, 6}, {35, -16}, {52, -44}},
    {{30, 6}, {34, -14}, {39, -24}},
    {{32, 9}, {32, -8}, {19, 17}},
    {{31, 19}, {37, -6}, {31, 25}},
    {{26, 27}, {35, 0}, {36, 29}},
    {{26, 30}, {30, 10}, {24, 33}},
    {{37, 20}, {28, 18}, {34, 15}},
    {{28, 34}, {26, 25}, {30, 20}},
    {{17, 70}, {29, 41}, {22, 73}},
    {{1, 67}, {0, 75}, {20, 34}},
    {{5, 59}, {2, 72}, {19, 31}},
    {{9, 67}, {8, 77}, {27, 44}},
    {{16, 30}, {14, 35}, {19, 16}},
    {{18, 32}, {18, 31}, {15, 36}},
    {{18, 35}, {17, 35}, {15, 36}},
    {{22, 29}, {21, 30}, {21, 28}},
    {{24, 31}, {17, 45}, {25, 21}},
    {{23, 38}, {20, 42}, {30, 20}},
    {{18, 43}, {18, 45}, {31, 12}},
    {{20, 41}, {27, 26}, {27, 16}},
    {{11, 63}, {16, 54}, {24, 42}},
    {{9, 59}, {7, 66}, {0, 93}},
    {{9, 64}, {16, 56}, {14, 56}},
    {{-1, 94}, {11, 73}, {15, 57}},
    {{-2, 89}, {10, 67}, {26, 38}},
    {{-9, 108}, {-10, 116}, {-24, 127}},
};

// See table 9-21 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_227_275_inter[49][3] =
{
    {{-6, 76}, {-23, 112}, {-24, 115}},
    {{-2, 44}, {-15, 71}, {-22, 82}},
    {{0, 45}, {-7, 61}, {-9, 62}},
    {{0, 52}, {0, 53}, {0, 53}},
    {{-3, 64}, {-5, 66}, {0, 59}},
    {{-2, 59}, {-11, 77}, {-14, 85}},
    {{-4, 70}, {-9, 80}, {-13, 89}},
    {{-4, 75}, {-9, 84}, {-13, 94}},
    {{-8, 82}, {-10, 87}, {-11, 92}},
    {{-17, 102}, {-34, 127}, {-29, 127}},
    {{-9, 77}, {-21, 101}, {-21, 100}},
    {{3, 24}, {-3, 39}, {-14, 57}},
    {{0, 42}, {-5, 53}, {-12, 67}},
    {{0, 48}, {-7, 61}, {-11, 71}},
    {{0, 55}, {-11, 75}, {-10, 77}},
    {{-6, 59}, {-15, 77}, {-21, 85}},
    {{-7, 71}, {-17, 91}, {-16, 88}},
    {{-12, 83}, {-25, 107}, {-23, 104}},
    {{-11, 87}, {-25, 111}, {-15, 98}},
    {{-30, 119}, {-28, 122}, {-37, 127}},
    {{1, 58}, {-11, 76}, {-10, 82}},
    {{-3, 29}, {-10, 44}, {-8, 48}},
    {{-1, 36}, {-10, 52}, {-8, 61}},
    {{1, 38}, {-10, 57}, {-8, 66}},
    {{2, 43}, {-9, 58}, {-7, 70}},
    {{-6, 55}, {-16, 72}, {-14, 75}},
    {{0, 58}, {-7, 69}, {-10, 79}},
    {{0, 64}, {-4, 69}, {-9, 83}},
    {{-3, 74}, {-5, 74}, {-12, 92}},
    {{-10, 90}, {-9, 86}, {-18, 108}},
    {{0, 70}, {2, 66}, {-4, 79}},
    {{-4, 29}, {-9, 34}, {-22, 69}},
    {{5, 31}, {1, 32}, {-16, 75}},
    {{7, 42}, {11, 31}, {-2, 58}},
    {{1, 59}, {5, 52}, {1, 58}},
    {{-2, 58}, {-2, 55}, {-13, 78}},
    {{-3, 72}, {-2, 67}, {-9, 83}},
    {{-3, 81}, {0, 73}, {-4, 81}},
    {{-11, 97}, {-8, 89}, {-13, 99}},
    {{0, 58}, {3, 52}, {-13, 81}},
    {{8, 5}, {7, 4}, {-6, 38}},
    {{10, 14}, {10, 8}, {-13, 62}},
    {{14, 18}, {17, 8}, {-6, 58}},
    {{13, 27}, {16, 19}, {-2, 59}},
    {{2, 40}, {3, 37}, {-16, 73}},
    {{0, 58}, {-1, 61}, {-10, 76}},
    {{-3, 70}, {-5, 73}, {-13, 86}},
    {{-6, 79}, {-1, 70}, {-9, 83}},
    {{-8, 85}, {-4, 78}, {-10, 87}},
};

// See table 9-22 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_277_337_inter[61][3] =
{
    {{-13, 106}, {-21, 126}, {-22, 127}},
    {{-16, 106}, {-23, 124}, {-25, 127}},
    {{-10, 87}, {-20, 110}, {-25, 120}},
    {{-21, 114}, {-26, 126}, {-27, 127}},
    {{-18, 110}, {-25, 124}, {-19, 114}},
    {{-14, 98}, {-17, 105}, {-23, 117}},
    {{-22, 110}, {-27, 121}, {-25, 118}},
    {{-21, 106}, {-27, 117}, {-26, 117}},
    {{-18, 103}, {-17, 102}, {-24, 113}},
    {{-21, 107}, {-26, 117}, {-28, 118}},
    {{-23, 108}, {-27, 116}, {-31, 120}},
    {{-26, 112}, {-33, 122}, {-37, 124}},
    {{-10, 96}, {-10, 95}, {-10, 94}},
    {{-12, 95}, {-14, 100}, {-15, 102}},
    {{-5, 91}, {-8, 95}, {-10, 99}},
    {{-9, 93}, {-17, 111}, {-13, 106}},
    {{-22, 94}, {-28, 114}, {-50, 127}},
    {{-5, 86}, {-6, 89}, {-5, 92}},
    {{9, 67}, {-2, 80}, {17, 57}},
    {{-4, 80}, {-4, 82}, {-5, 86}},
    {{-10, 85}, {-9, 85}, {-13, 94}},
    {{-1, 70}, {-8, 81}, {-12, 91}},
    {{7, 60}, {-1, 72}, {-2, 77}},
    {{9, 58}, {5, 64}, {0, 71}},
    {{5, 61}, {1, 67}, {-1, 73}},
    {{12, 50}, {9, 56}, {4, 64}},
    {{15, 50}, {0, 69}, {-7, 81}},
    {{18, 49}, {1, 69}, {5, 64}},
    {{17, 54}, {7, 69}, {15, 57}},
    {{10, 41}, {-7, 69}, {1, 67}},
    {{7, 46}, {-6, 67}, {0, 68}},
    {{-1, 51}, {-16, 77}, {-10, 67}},
    {{7, 49}, {-2, 64}, {1, 68}},
    {{8, 52}, {2, 61}, {0, 77}},
    {{9, 41}, {-6, 67}, {2, 64}},
    {{6, 47}, {-3, 64}, {0, 68}},
    {{2, 55}, {2, 57}, {-5, 78}},
    {{13, 41}, {-3, 65}, {7, 55}},
    {{10, 44}, {-3, 66}, {5, 59}},
    {{6, 50}, {0, 62}, {2, 65}},
    {{5, 53}, {9, 51}, {14, 54}},
    {{13, 49}, {-1, 66}, {15, 44}},
    {{4, 63}, {-2, 71}, {5, 60}},
    {{6, 64}, {-2, 75}, {2, 70}},
    {{-2, 69}, {-1, 70}, {-2, 76}},
    {{-2, 59}, {-9, 72}, {-18, 86}},
    {{6, 70}, {14, 60}, {12, 70}},
    {{10, 44}, {16, 37}, {5, 64}},
    {{9, 31}, {0, 47}, {-12, 70}},
    {{12, 43}, {18, 35}, {11, 55}},
    {{3, 53}, {11, 37}, {5, 56}},
    {{14, 34}, {12, 41}, {0, 69}},
    {{10, 38}, {10, 41}, {2, 65}},
    {{-3, 52}, {2, 48}, {-6, 74}},
    {{13, 40}, {12, 41}, {5, 54}},
    {{17, 32}, {13, 41}, {7, 54}},
    {{7, 44}, {0, 59}, {-6, 76}},
    {{7, 38}, {3, 50}, {-11, 82}},
    {{13, 50}, {19, 40}, {-2, 77}},
    {{10, 57}, {3, 66}, {-2, 77}},
    {{26, 43}, {18, 50}, {25, 42}},
};

// See table 9-23 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_338_398_inter[61][3] =
{
    {{14, 11}, {19, -6}, {17, -13}},
    {{11, 14}, {18, -6}, {16, -9}},
    {{9, 11}, {14, 0}, {17, -12}},
    {{18, 11}, {26, -12}, {27, -21}},
    {{21, 9}, {31, -16}, {37, -30}},
    {{23, -2}, {33, -25}, {41, -40}},
    {{32, -15}, {33, -22}, {42, -41}},
    {{32, -15}, {37, -28}, {48, -47}},
    {{34, -21}, {39, -30}, {39, -32}},
    {{39, -23}, {42, -30}, {46, -40}},
    {{42, -33}, {47, -42}, {52, -51}},
    {{41, -31}, {45, -36}, {46, -41}},
    {{46, -28}, {49, -34}, {52, -39}},
    {{38, -12}, {41, -17}, {43, -19}},
    {{21, 29}, {32, 9}, {32, 11}},
    {{45, -24}, {69, -71}, {61, -55}},
    {{53, -45}, {63, -63}, {56, -46}},
    {{48, -26}, {66, -64}, {62, -50}},
    {{65, -43}, {77, -74}, {81, -67}},
    {{43, -19}, {54, -39}, {45, -20}},
    {{39, -10}, {52, -35}, {35, -2}},
    {{30, 9}, {41, -10}, {28, 15}},
    {{18, 26}, {36, 0}, {34, 1}},
    {{20, 27}, {40, -1}, {39, 1}},
    {{0, 57}, {30, 14}, {30, 17}},
    {{-14, 82}, {28, 26}, {20, 38}},
    {{-5, 75}, {23, 37}, {18, 45}},
    {{-19, 97}, {12, 55}, {15, 54}},
    {{-35, 125}, {11, 65}, {0, 79}},
    {{27, 0}, {37, -33}, {36, -16}},
    {{28, 0}, {39, -36}, {37, -14}},
    {{31, -4}, {40, -37}, {37, -17}},
    {{27, 6}, {38, -30}, {32, 1}},
    {{34, 8}, {46, -33}, {34, 15}},
    {{30, 10}, {42, -30}, {29, 15}},
    {{24, 22}, {40, -24}, {24, 25}},
    {{33, 19}, {49, -29}, {34, 22}},
    {{22, 32}, {38, -12}, {31, 16}},
    {{26, 31}, {40, -10}, {35, 18}},
    {{21, 41}, {38, -3}, {31, 28}},
    {{26, 44}, {46, -5}, {33, 41}},
    {{23, 47}, {31, 20}, {36, 28}},
    {{16, 65}, {29, 30}, {27, 47}},
    {{14, 71}, {25, 44}, {21, 62}},
    {{8, 60}, {12, 48}, {18, 31}},
    {{6, 63}, {11, 49}, {19, 26}},
    {{17, 65}, {26, 45}, {36, 24}},
    {{21, 24}, {22, 22}, {24, 23}},
    {{23, 20}, {23, 22}, {27, 16}},
    {{26, 23}, {27, 21}, {24, 30}},
    {{27, 32}, {33, 20}, {31, 29}},
    {{28, 23}, {26, 28}, {22, 41}},
    {{28, 24}, {30, 24}, {22, 42}},
    {{23, 40}, {27, 34}, {16, 60}},
    {{24, 32}, {18, 42}, {15, 52}},
    {{28, 29}, {25, 39}, {14, 60}},
    {{23, 42}, {18, 50}, {3, 78}},
    {{19, 57}, {12, 70}, {-16, 123}},
    {{22, 53}, {21, 54}, {21, 53}},
    {{22, 61}, {14, 71}, {22, 56}},
    {{11, 86}, {11, 83}, {25, 61}},
};

static
INITIALIZE_VALUES M_and_N_for_ctxIdx_399_401_inter[3][3] =
{
    {{12, 40}, {25, 32}, {21, 33}},
    {{11, 51}, {21, 49}, {19, 50}},
    {{14, 59}, {21, 54}, {17, 61}},
};

static
INITIALIZE_VALUES M_and_N_for_ctxIdx_402_459_inter[58][3] =
{
    {{-4, 79}, {-5, 85}, {-3, 78}},
    {{-7, 71}, {-6, 81}, {-8, 74}},
    {{-5, 69}, {-10,77}, {-9, 72}},
    {{-9, 70}, {-7, 81}, {-10,72}},
    {{-8 ,66}, {-17,80}, {-18,75}},
    {{-10,68}, {-18,73}, {-12,71}},
    {{-19,73}, {-4, 74}, {-11,63}},
    {{-12,69}, {-10,83}, {-5, 70}},
    {{-16,70}, {-9, 71}, {-17,75}},
    {{-15,67}, {-9, 67}, {-14,72}},
    {{-20,62}, {-1, 61}, {-16,67}},
    {{-19,70}, {-8, 66}, {-8, 53}},
    {{-16,66}, {-14,66}, {-14,59}},
    {{-22,65}, {0,  59}, {-9, 52}},
    {{-20,63}, {2,  59}, {-11,68}},
    {{9,  -2}, {17,-10}, {9,  -2}},
    {{26, -9}, {32,-13}, {30,-10}},
    {{33, -9}, {42, -9}, {31, -4}},
    {{39, -7}, {49, -5}, {33, -1}},
    {{41, -2}, {53,  0}, {33,  7}},
    {{45,  3}, {64,  3}, {31, 12}},
    {{49,  9}, {68, 10}, {37, 23}},
    {{45, 27}, {66, 27}, {31, 38}},
    {{36, 59}, {47, 57}, {20, 64}},
    {{-6, 66}, {-5, 71}, {-9, 71}},
    {{-7, 35}, {0,  24}, {-7, 37}},
    {{-7, 42}, {-1, 36}, {-8, 44}},
    {{-8, 45}, {-2, 42}, {-11,49}},
    {{-5, 48}, {-2, 52}, {-10,56}},
    {{-12,56}, {-9, 57}, {-12,59}},
    {{-6, 60}, {-6, 63}, {-8, 63}},
    {{-5, 62}, {-4, 65}, {-9, 67}},
    {{-8, 66}, {-4, 67}, {-6, 68}},
    {{-8, 76}, {-7, 82}, {-10,79}},
    {{-5, 85}, {-3, 81}, {-3, 78}},
    {{-6, 81}, {-3, 76}, {-8, 74}},
    {{-10,77}, {-7, 72}, {-9, 72}},
    {{-7, 81}, {-6, 78}, {-10,72}},
    {{-17,80}, {-12,72}, {-18,75}},
    {{-18,73}, {-14,68}, {-12,71}},
    {{-4, 74}, {-3, 70}, {-11,63}},
    {{-10,83}, {-6, 76}, {-5, 70}},
    {{-9, 71}, {-5, 66}, {-17,75}},
    {{-9, 67}, {-5, 62}, {-14,72}},
    {{-1, 61}, {0,  57}, {-16,67}},
    {{-8, 66}, {-4, 61}, {-8, 53}},
    {{-14,66}, {-9, 60}, {-14,59}},
    {{0,  59}, {1,  54}, {-9, 52}},
    {{2,  59}, {2,  58}, {-11,68}},
    {{21,-13}, {17,-10}, {9,  -2}},
    {{33,-14}, {32,-13}, {30,-10}},
    {{39, -7}, {42, -9}, {31, -4}},
    {{46, -2}, {49, -5}, {33, -1}},
    {{51,  2}, {53,  0}, {33,  7}},
    {{60,  6}, {64,  3}, {31, 12}},
    {{61, 17}, {68, 10}, {37, 23}},
    {{55, 34}, {66, 27}, {31, 38}},
    {{42, 62}, {47, 57}, {20, 64}},
};

// See table G-6 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_1024_1026_intra[3] =
{
    {-14, 138}, {-22, 140},  {-11, 99},
};

static
INITIALIZE_VALUES M_and_N_for_ctxIdx_1024_1026_inter[3] =
{
    {0, 75}, {2, 65},  {2, 59},
};

// See table G-7 of H.264 standard
static
INITIALIZE_VALUES M_and_N_for_ctxIdx_1027_1030[4] =
{
    {-6, 67}, {-6, 67}, { -23, 104 } ,  { -35,  106 }
};

} // namespace UMC_H264_DECODER

using namespace UMC_H264_DECODER;

namespace UMC
{

void H264Bitstream::InitializeContextVariablesIntra_CABAC(Ipp32s SliceQPy)
{
    Ipp32s l;

    SliceQPy = IPP_MAX(0, SliceQPy);

    // See subclause 9.3.1.1 of H.264 standard

    // Initialize context(s) for mb_type (SI & I slices)
    for (l = 0; l <= 10; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_0_10[l - 0].m,
                          M_and_N_for_ctxIdx_0_10[l - 0].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initialize context(s) for mb_qp_delta &
    // intra_chroma_pred_mode & prev_intra4x4_pred_mode_flag &
    // rem_intra4x4_pred_mode
    for (l = 60; l <= 69; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_60_69[l - 60].m,
                          M_and_N_for_ctxIdx_60_69[l - 60].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initialize context(s) for mb_field_decoding_flag &
    // coded_block_pattern(luma) & coded_block_pattern(chroma) &
    // coded_block_flag (SI & I slices)
    for (l = 70; l <= 104; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_70_104_intra[l - 70].m,
                          M_and_N_for_ctxIdx_70_104_intra[l - 70].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initalize context(s) for significant_coeff_flag[] (frame coded)
    for (l = 105; l <= 165; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_105_165_intra[l - 105].m,
                          M_and_N_for_ctxIdx_105_165_intra[l - 105].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initalize context(s) for last_significant_coeff_flag[] (frame coded)
    for (l = 166; l <= 226; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_166_226_intra[l - 166].m,
                          M_and_N_for_ctxIdx_166_226_intra[l - 166].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initalize context(s) for coeff_abs_level_minus1[]
    for (l = 227; l <= 275; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_227_275_intra[l - 227].m,
                          M_and_N_for_ctxIdx_227_275_intra[l - 227].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // ctxIdx equal to 276 is associated the end_of_slice_flag
    // Initial values associated with ctxIdx equal to 276
    // are specified to be pStateIdx = 63 and valMPS = 0
    context_array[276].pStateIdxAndVal = 63 * 2;

    // Initalize context(s) for significant_coeff_flag[] (field coded)
    for (l = 277; l <= 337; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_277_337_intra[l - 277].m,
                          M_and_N_for_ctxIdx_277_337_intra[l - 277].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initalize context(s) for last_significant_coeff_flag[] (field coded)
    for (l = 338; l <= 398; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_338_398_intra[l - 338].m,
                          M_and_N_for_ctxIdx_338_398_intra[l - 338].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    for (l = 399; l <= 401; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_399_401_intra[l - 399].m,
                          M_and_N_for_ctxIdx_399_401_intra[l - 399].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    for (l = 402; l <= 459; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_402_459_intra[l - 402].m,
                          M_and_N_for_ctxIdx_402_459_intra[l - 402].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    for (l = 460; l <= 462; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_1024_1026_intra[l - 460].m,
                          M_and_N_for_ctxIdx_1024_1026_intra[l - 460].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
            , l
#endif
#endif
            );
    }

} // void H264Bitstream::InitializeContextVariablesIntra_CABAC(Ipp32s SliceQPy)

void H264Bitstream::InitializeContextVariablesInter_CABAC(Ipp32s SliceQPy,
                                                          Ipp32s cabac_init_idc)
{
    Ipp32s l;

    // See subclause 9.3.1.1 of H.264 standard
    SliceQPy = IPP_MAX(0, SliceQPy);

    // Initialize context(s) for mb_skip_flag & mb_type (P & SP slices)
    // & sub_mb_pred (P & SP slices)
    for (l = 11; l <= 23; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_11_23[l - 11][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_11_23[l - 11][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initialize context(s) for mb_skip_flag & mb_type (B slices)
    // & sub_mb_pred (B slices)
    for (l = 24; l <= 39; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_24_39[l - 24][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_24_39[l - 24][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initialize context(s) for mvd_10 & mvd_11
    for (l = 40; l <= 53; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_40_53[l - 40][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_40_53[l - 40][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initialize context(s) for ref_idx_10 & ref_idx_11
    for (l = 54; l <= 59; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_54_59[l - 54][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_54_59[l - 54][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initialize context(s) for mb_qp_delta &
    // intra_chroma_pred_mode & prev_intra4x4_pred_mode_flag &
    // rem_intra4x4_pred_mode
    for (l = 60; l <= 69; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_60_69[l - 60].m,
                          M_and_N_for_ctxIdx_60_69[l - 60].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initalize context(s) for mb_field_decoding_flag &
    // coded_block_pattern(luma) & coded_block_pattern(chroma) &
    // coded_block_flag (P, SP & B slices)
    for (l = 70; l <= 104; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_70_104_inter[l - 70][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_70_104_inter[l - 70][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initalize context(s) for significant_coeff_flag[] (frame coded)
    for (l = 105; l <= 165; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_105_165_inter[l - 105][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_105_165_inter[l - 105][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initalize context(s) for last_significant_coeff_flag[] (frame coded)
    for (l = 166; l <= 226; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_166_226_inter[l - 166][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_166_226_inter[l - 166][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initalize context(s) for coeff_abs_level_minus1[]
    for (l = 227; l <= 275; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_227_275_inter[l - 227][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_227_275_inter[l - 227][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initalize context(s) for significant_coeff_flag[] (field coded)
    for (l = 277; l <= 337; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_277_337_inter[l - 277][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_277_337_inter[l - 277][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    // Initalize context(s) for last_significant_coeff_flag[] (field coded)
    for (l = 338; l <= 398; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_338_398_inter[l - 338][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_338_398_inter[l - 338][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    for (l = 399; l <= 401; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_399_401_inter[l - 399][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_399_401_inter[l - 399][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    for (l = 402; l <= 459; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_402_459_inter[l - 402][cabac_init_idc].m,
                          M_and_N_for_ctxIdx_402_459_inter[l - 402][cabac_init_idc].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
                          , l
#endif
#endif
                          );
    }

    for (l = 460; l <= 462; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_1024_1026_inter[l - 460].m,
                          M_and_N_for_ctxIdx_1024_1026_inter[l - 460].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
            , l
#endif
#endif
            );
    }

    for (l = 463; l <= 466; l += 1)
    {
        InitializeContext(context_array + l,
                          M_and_N_for_ctxIdx_1027_1030[l - 463].m,
                          M_and_N_for_ctxIdx_1027_1030[l - 463].n,
                          SliceQPy
#ifdef STORE_CABAC_BITS
#ifdef CABAC_CONTEXTS_COMP
            , l
#endif
#endif
            );
    }

} // void H264Bitstream::InitializeContextVariablesInter_CABAC(Ipp32s SliceQPy,

void H264Bitstream::InitializeDecodingEngine_CABAC()
{
    // See subclause 9.3.1.2 of H.264 standard

    AlignPointerRight();

    m_lcodIRange = 0x01fe;

    m_lcodIOffset = GetBits(9);

#if (CABAC_MAGIC_BITS > 0)
    m_lcodIRange = m_lcodIRange << CABAC_MAGIC_BITS;
    m_lcodIOffset = m_lcodIOffset << CABAC_MAGIC_BITS;
    {
        Ipp32u nBits;

        m_iMagicBits = (m_bitOffset % 16) + 1;
        nBits = GetBits(m_iMagicBits);
        m_lcodIOffset |= nBits << (16 - m_iMagicBits);

        m_pMagicBits = ((Ipp16u *) m_pbs) + ((15 == m_bitOffset) ? (1) : (0));
    }
#endif // (CABAC_MAGIC_BITS > 0)

} // void H264Bitstream::InitializeDecodingEngine_CABAC(void)

void H264Bitstream::TerminateDecode_CABAC(void)
{
#if (CABAC_MAGIC_BITS > 0)
    // restore source pointer
    m_pbs = (Ipp32u *) (((size_t) m_pMagicBits) & -0x04);
    m_bitOffset = (((size_t) m_pMagicBits) & 0x02) ? (15) : (31);
    // return prereaded bits
    ippiUngetNBits(m_pbs, m_bitOffset, m_iMagicBits);
    m_iMagicBits = 0;
    m_lcodIOffset &= -0x10000;
#endif
    AlignPointerRight();

} // void H264Bitstream::TerminateDecode_CABAC(void)

#ifdef STORE_CABAC_BITS
FILE *cabac_bits;
Ipp32s sym_cnt;
#endif

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
