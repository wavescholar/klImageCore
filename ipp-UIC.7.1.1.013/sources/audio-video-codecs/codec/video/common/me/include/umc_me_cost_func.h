/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
//                     motion estimation
//
*/
#ifndef _UMC_ME_COST_FUNC_H_
#define _UMC_ME_COST_FUNC_H_

#include "umc_me.h"
#include "ippvc.h"

//#define USE_OPTIMIZED_SATD
#ifdef USE_OPTIMIZED_SATD
#include "emmintrin.h"
#endif

namespace UMC
{

inline void VarMeanIntra(const Ipp8u* pblock, Ipp32s step, Ipp32s *vardiff, Ipp32s *meandiff, Ipp32s &cost)
{
    ippiVarSum8x8_8u32s_C1R(pblock             , step, &vardiff[0], &meandiff[0]);
    ippiVarSum8x8_8u32s_C1R(pblock + 8         , step, &vardiff[1], &meandiff[1]);
    ippiVarSum8x8_8u32s_C1R(pblock + 8*step    , step, &vardiff[2], &meandiff[2]);
    ippiVarSum8x8_8u32s_C1R(pblock + 8*step + 8, step, &vardiff[3], &meandiff[3]);
    cost = vardiff[0] + vardiff[1] + vardiff[2] + vardiff[3] +
        (abs(meandiff[0]) + abs(meandiff[1]) + abs(meandiff[2]) + abs(meandiff[3]))/8;
}

inline void VarMeanInter(Ipp16s *pDiff, Ipp32s step, Ipp32s* vardiff, Ipp32s* meandiff, Ipp32s &cost)
{
    ippiVarSum8x8_16s32s_C1R(pDiff         , step, &vardiff[0], &meandiff[0]);
    ippiVarSum8x8_16s32s_C1R(pDiff+8       , step, &vardiff[1], &meandiff[1]);
    ippiVarSum8x8_16s32s_C1R(pDiff+4*step  , step, &vardiff[2], &meandiff[2]);
    ippiVarSum8x8_16s32s_C1R(pDiff+4*step+8, step, &vardiff[3], &meandiff[3]);
    cost = vardiff[0] + vardiff[1] + vardiff[2] + vardiff[3] +
        (abs(meandiff[0]) + abs(meandiff[1]) + abs(meandiff[2]) + abs(meandiff[3]))/8;
}

inline void VarMeanIntraBlock(const Ipp8u* pblock, Ipp32s step, Ipp32s &vardiff, Ipp32s &meandiff, Ipp32s &cost)
{
    ippiVarSum8x8_8u32s_C1R(pblock             , step, &vardiff, &meandiff);
    cost = vardiff + abs(meandiff)/8;
}

inline void VarMeanInterBlock(Ipp16s *pDiff, Ipp32s step, Ipp32s &vardiff, Ipp32s &meandiff, Ipp32s &cost)
{
    ippiVarSum8x8_16s32s_C1R(pDiff         , step, &vardiff, &meandiff);
    cost = vardiff + abs(meandiff)/8;
}

inline void VarMean16x16(Ipp16s *pDiff, Ipp32s step, Ipp32s *vardiff, Ipp32s *meandiff)
{
    ippiVarSum8x8_16s32s_C1R(pDiff         , step, &vardiff[0], &meandiff[0]);
    ippiVarSum8x8_16s32s_C1R(pDiff+8       , step, &vardiff[1], &meandiff[1]);
    ippiVarSum8x8_16s32s_C1R(pDiff+4*step  , step, &vardiff[2], &meandiff[2]);
    ippiVarSum8x8_16s32s_C1R(pDiff+4*step+8, step, &vardiff[3], &meandiff[3]);
}

inline void VarMean8x8(Ipp16s *pDiff, Ipp32s step, Ipp32s &vardiff, Ipp32s &meandiff)
{
    ippiVarSum8x8_16s32s_C1R(pDiff         , step, &vardiff, &meandiff);
}

void SAD8x8(const Ipp8u*  pSrc, Ipp32s  srcStep, const Ipp8u*  pRef, Ipp32s  refStep, Ipp32s* pSAD, Ipp32s  mcType)
{
    Ipp32s sum;
    *pSAD = 0;
    for(int i=0; i<8; i+=4)
    {
        for(int j=0; j<8; j+=4)
        {
            ippiSAD4x4_8u32s(pSrc+i*srcStep+j, srcStep,  pRef+i*refStep+j, refStep, &sum, mcType);
            *pSAD += sum;
        }
    }
    return;
}

void SAD4x4Blocks2x2(const   Ipp8u*  pSrc, Ipp32s  srcStep, const   Ipp8u*  pRef, Ipp32s  refStep, Ipp16u*  pDstSAD)
{
    int i,j;
    for(i=0; i<4; i++) pDstSAD[i]=0;

    for(i=0; i<4; i++)
    {
        for(j=0; j<4; j++)
        {
            int index = (2&i)+(2&j)/2;
            pDstSAD[index] = pDstSAD[index] + (Ipp16u)(abs( (int)*(pSrc+i*srcStep+j) - (int)*(pRef+i*refStep+j)));
        }
    }
    return;
}

//ipp functions:

//perform fast hadamard transformation of block
template<typename T, Ipp32s size> void HadamardFwdFast(const T* pSrc, Ipp32s srcStep, Ipp16s* pDst)
{
    Ipp32s a[size], b[size], temp[size*size], *pTemp;
    Ipp32s j;

#if defined(WINDOWS) && defined(__ICL)
#pragma warning(disable:280)
#endif
    switch(size)
    {
    case 4:
        pTemp = temp;
        for(j=0;j<4;j++) {
            a[0] = pSrc[0] + pSrc[1];
            a[1] = pSrc[0] - pSrc[1];
            a[2] = pSrc[2] + pSrc[3];
            a[3] = pSrc[2] - pSrc[3];
            pSrc = pSrc + (srcStep/sizeof(T));

            pTemp[0] = a[0] + a[2];
            pTemp[1] = a[1] + a[3];
            pTemp[2] = a[0] - a[2];
            pTemp[3] = a[1] - a[3];
            pTemp = pTemp + 4;
        }
        pTemp = temp;
        for(j=0;j<4;j++) {
            a[0] = pTemp[0*4] + pTemp[1*4];
            a[1] = pTemp[0*4] - pTemp[1*4];
            a[2] = pTemp[2*4] + pTemp[3*4];
            a[3] = pTemp[2*4] - pTemp[3*4];
            pTemp++;

            pDst[0*4] = (Ipp16s)(a[0] + a[2]);
            pDst[1*4] = (Ipp16s)(a[1] + a[3]);
            pDst[2*4] = (Ipp16s)(a[0] - a[2]);
            pDst[3*4] = (Ipp16s)(a[1] - a[3]);
            pDst++;
        }
        break;
    case 8:
        pTemp = temp;
        for(j=0;j<8;j++) {
            a[0] = pSrc[0] + pSrc[1];
            a[1] = pSrc[2] + pSrc[3];
            a[2] = pSrc[4] + pSrc[5];
            a[3] = pSrc[6] + pSrc[7];
            a[4] = pSrc[0] - pSrc[1];
            a[5] = pSrc[2] - pSrc[3];
            a[6] = pSrc[4] - pSrc[5];
            a[7] = pSrc[6] - pSrc[7];
            pSrc = pSrc + (srcStep/sizeof(T));

            b[0] = a[0] + a[1];
            b[1] = a[2] + a[3];
            b[2] = a[4] + a[5];
            b[3] = a[6] + a[7];
            b[4] = a[0] - a[1];
            b[5] = a[2] - a[3];
            b[6] = a[4] - a[5];
            b[7] = a[6] - a[7];

            pTemp[0] = b[0] + b[1];
            pTemp[1] = b[2] + b[3];
            pTemp[2] = b[4] + b[5];
            pTemp[3] = b[6] + b[7];
            pTemp[4] = b[0] - b[1];
            pTemp[5] = b[2] - b[3];
            pTemp[6] = b[4] - b[5];
            pTemp[7] = b[6] - b[7];
            pTemp = pTemp + 8;
        }
        pTemp = temp;
        for(j=0;j<8;j++) {
            a[0] = pTemp[0*8] + pTemp[1*8];
            a[1] = pTemp[2*8] + pTemp[3*8];
            a[2] = pTemp[4*8] + pTemp[5*8];
            a[3] = pTemp[6*8] + pTemp[7*8];
            a[4] = pTemp[0*8] - pTemp[1*8];
            a[5] = pTemp[2*8] - pTemp[3*8];
            a[6] = pTemp[4*8] - pTemp[5*8];
            a[7] = pTemp[6*8] - pTemp[7*8];
            pTemp++;

            b[0] = a[0] + a[1];
            b[1] = a[2] + a[3];
            b[2] = a[4] + a[5];
            b[3] = a[6] + a[7];
            b[4] = a[0] - a[1];
            b[5] = a[2] - a[3];
            b[6] = a[4] - a[5];
            b[7] = a[6] - a[7];

            pDst[0*8] = (Ipp16s)(b[0] + b[1]);
            pDst[1*8] = (Ipp16s)(b[2] + b[3]);
            pDst[2*8] = (Ipp16s)(b[4] + b[5]);
            pDst[3*8] = (Ipp16s)(b[6] + b[7]);
            pDst[4*8] = (Ipp16s)(b[0] - b[1]);
            pDst[5*8] = (Ipp16s)(b[2] - b[3]);
            pDst[6*8] = (Ipp16s)(b[4] - b[5]);
            pDst[7*8] = (Ipp16s)(b[6] - b[7]);
            pDst++;
        }
    break;

    default:
        VM_ASSERT(0);
    }

}

//find SATD of two 8x8 blocks
#ifdef USE_OPTIMIZED_SATD
Ipp32u SAT8x8D(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step)
{
    __declspec(align(32)) Ipp16s diff[8][8];
    Ipp32u satd = 0;
    ippiSub8x8_8u16s_C1R(pSrc1, src1Step, pSrc2, src2Step, &diff[0][0], 16);


    __declspec(align(32)) __m128i  _p_0, _p_1, _p_2, _p_3, _p_4, _p_5, _p_6, _p_7, _b_2, _b_3, _b_6, _b_7, _p_t, _p_s;
    Ipp32s  s;
    __declspec(align(32)) Ipp16s tmp[8][8];

    _p_0 = _mm_load_si128((__m128i*)(diff[0]));
    _p_4 = _mm_sub_epi16(_p_0, *(__m128i*)(diff[4]));
    _p_0 = _mm_add_epi16(_p_0, *(__m128i*)(diff[4]));
    _p_1 = _mm_load_si128((__m128i*)(diff[1]));
    _p_5 = _mm_sub_epi16(_p_1, *(__m128i*)(diff[5]));
    _p_1 = _mm_add_epi16(_p_1, *(__m128i*)(diff[5]));
    _p_2 = _mm_load_si128((__m128i*)(diff[2]));
    _p_6 = _mm_sub_epi16(_p_2, *(__m128i*)(diff[6]));
    _p_2 = _mm_add_epi16(_p_2, *(__m128i*)(diff[6]));
    _p_3 = _mm_load_si128((__m128i*)(diff[3]));
    _p_7 = _mm_sub_epi16(_p_3, *(__m128i*)(diff[7]));
    _p_3 = _mm_add_epi16(_p_3, *(__m128i*)(diff[7]));

    _b_2 = _mm_sub_epi16(_p_0, _p_2);
    _p_0 = _mm_add_epi16(_p_0, _p_2);
    _b_3 = _mm_sub_epi16(_p_1, _p_3);
    _p_1 = _mm_add_epi16(_p_1, _p_3);
    _b_6 = _mm_sub_epi16(_p_4, _p_6);
    _p_4 = _mm_add_epi16(_p_4, _p_6);
    _b_7 = _mm_sub_epi16(_p_5, _p_7);
    _p_5 = _mm_add_epi16(_p_5, _p_7);

    _p_s = _mm_sub_epi16(_p_0, _p_1);
    _p_0 = _mm_add_epi16(_p_0, _p_1);
    _mm_store_si128((__m128i*)diff[1], _p_s);
    _mm_store_si128((__m128i*)diff[0], _p_0);
    _p_s = _mm_sub_epi16(_b_2, _b_3);
    _b_2 = _mm_add_epi16(_b_2, _b_3);
    _mm_store_si128((__m128i*)diff[3], _p_s);
    _mm_store_si128((__m128i*)diff[2], _b_2);
    _p_s = _mm_sub_epi16(_p_4, _p_5);
    _p_4 = _mm_add_epi16(_p_4, _p_5);
    _mm_store_si128((__m128i*)diff[5], _p_s);
    _mm_store_si128((__m128i*)diff[4], _p_4);
    _p_s = _mm_sub_epi16(_b_6, _b_7);
    _b_6 = _mm_add_epi16(_b_6, _b_7);
    _mm_store_si128((__m128i*)diff[7], _p_s);
    _mm_store_si128((__m128i*)diff[6], _b_6);

    _p_0 = _mm_loadl_epi64((__m128i*)(&diff[0][0]));
    _p_1 = _mm_loadl_epi64((__m128i*)(&diff[1][0]));
    _p_2 = _mm_loadl_epi64((__m128i*)(&diff[2][0]));
    _p_3 = _mm_loadl_epi64((__m128i*)(&diff[3][0]));
    _p_0 = _mm_unpacklo_epi16(_p_0, _p_1);
    _p_2 = _mm_unpacklo_epi16(_p_2, _p_3);
    _p_1 = _mm_unpackhi_epi32(_p_0, _p_2);
    _p_0 = _mm_unpacklo_epi32(_p_0, _p_2);
    _mm_storel_epi64((__m128i*)&tmp[0][0], _p_0);
    _mm_storel_epi64((__m128i*)&tmp[2][0], _p_1);
    _p_0 = _mm_srli_si128(_p_0, 8);
    _p_1 = _mm_srli_si128(_p_1, 8);
    _mm_storel_epi64((__m128i*)&tmp[1][0], _p_0);
    _mm_storel_epi64((__m128i*)&tmp[3][0], _p_1);

    _p_0 = _mm_loadl_epi64((__m128i*)(&diff[0][4]));
    _p_1 = _mm_loadl_epi64((__m128i*)(&diff[1][4]));
    _p_2 = _mm_loadl_epi64((__m128i*)(&diff[2][4]));
    _p_3 = _mm_loadl_epi64((__m128i*)(&diff[3][4]));
    _p_0 = _mm_unpacklo_epi16(_p_0, _p_1);
    _p_2 = _mm_unpacklo_epi16(_p_2, _p_3);
    _p_1 = _mm_unpackhi_epi32(_p_0, _p_2);
    _p_0 = _mm_unpacklo_epi32(_p_0, _p_2);
    _mm_storel_epi64((__m128i*)&tmp[4][0], _p_0);
    _mm_storel_epi64((__m128i*)&tmp[6][0], _p_1);
    _p_0 = _mm_srli_si128(_p_0, 8);
    _p_1 = _mm_srli_si128(_p_1, 8);
    _mm_storel_epi64((__m128i*)&tmp[5][0], _p_0);
    _mm_storel_epi64((__m128i*)&tmp[7][0], _p_1);

    _p_0 = _mm_loadl_epi64((__m128i*)(&diff[4][0]));
    _p_1 = _mm_loadl_epi64((__m128i*)(&diff[5][0]));
    _p_2 = _mm_loadl_epi64((__m128i*)(&diff[6][0]));
    _p_3 = _mm_loadl_epi64((__m128i*)(&diff[7][0]));
    _p_0 = _mm_unpacklo_epi16(_p_0, _p_1);
    _p_2 = _mm_unpacklo_epi16(_p_2, _p_3);
    _p_1 = _mm_unpackhi_epi32(_p_0, _p_2);
    _p_0 = _mm_unpacklo_epi32(_p_0, _p_2);
    _mm_storel_epi64((__m128i*)&tmp[0][4], _p_0);
    _mm_storel_epi64((__m128i*)&tmp[2][4], _p_1);
    _p_0 = _mm_srli_si128(_p_0, 8);
    _p_1 = _mm_srli_si128(_p_1, 8);
    _mm_storel_epi64((__m128i*)&tmp[1][4], _p_0);
    _mm_storel_epi64((__m128i*)&tmp[3][4], _p_1);

    _p_0 = _mm_loadl_epi64((__m128i*)(&diff[4][4]));
    _p_1 = _mm_loadl_epi64((__m128i*)(&diff[5][4]));
    _p_2 = _mm_loadl_epi64((__m128i*)(&diff[6][4]));
    _p_3 = _mm_loadl_epi64((__m128i*)(&diff[7][4]));
    _p_0 = _mm_unpacklo_epi16(_p_0, _p_1);
    _p_2 = _mm_unpacklo_epi16(_p_2, _p_3);
    _p_1 = _mm_unpackhi_epi32(_p_0, _p_2);
    _p_0 = _mm_unpacklo_epi32(_p_0, _p_2);
    _mm_storel_epi64((__m128i*)&tmp[4][4], _p_0);
    _mm_storel_epi64((__m128i*)&tmp[6][4], _p_1);
    _p_0 = _mm_srli_si128(_p_0, 8);
    _p_1 = _mm_srli_si128(_p_1, 8);
    _mm_storel_epi64((__m128i*)&tmp[5][4], _p_0);
    _mm_storel_epi64((__m128i*)&tmp[7][4], _p_1);

    _p_0 = _mm_load_si128((__m128i*)(tmp[0]));
    _p_4 = _mm_sub_epi16(_p_0, *(__m128i*)(tmp[4]));
    _p_0 = _mm_add_epi16(_p_0, *(__m128i*)(tmp[4]));
    _p_1 = _mm_load_si128((__m128i*)(tmp[1]));
    _p_5 = _mm_sub_epi16(_p_1, *(__m128i*)(tmp[5]));
    _p_1 = _mm_add_epi16(_p_1, *(__m128i*)(tmp[5]));
    _p_2 = _mm_load_si128((__m128i*)(tmp[2]));
    _p_6 = _mm_sub_epi16(_p_2, *(__m128i*)(tmp[6]));
    _p_2 = _mm_add_epi16(_p_2, *(__m128i*)(tmp[6]));
    _p_3 = _mm_load_si128((__m128i*)(tmp[3]));
    _p_7 = _mm_sub_epi16(_p_3, *(__m128i*)(tmp[7]));
    _p_3 = _mm_add_epi16(_p_3, *(__m128i*)(tmp[7]));

    _b_2 = _mm_sub_epi16(_p_0, _p_2);
    _p_0 = _mm_add_epi16(_p_0, _p_2);
    _b_3 = _mm_sub_epi16(_p_1, _p_3);
    _p_1 = _mm_add_epi16(_p_1, _p_3);
    _b_6 = _mm_sub_epi16(_p_4, _p_6);
    _p_4 = _mm_add_epi16(_p_4, _p_6);
    _b_7 = _mm_sub_epi16(_p_5, _p_7);
    _p_5 = _mm_add_epi16(_p_5, _p_7);

    _p_s = _mm_sub_epi16(_p_0, _p_1);
    _p_t = _mm_srai_epi16(_p_s, 15);
    _p_s = _mm_xor_si128(_p_s, _p_t);
    _p_s = _mm_sub_epi16(_p_s, _p_t);
    _p_0 = _mm_add_epi16(_p_0, _p_1);
    _p_t = _mm_srai_epi16(_p_0, 15);
    _p_0 = _mm_xor_si128(_p_0, _p_t);
    _p_0 = _mm_sub_epi16(_p_0, _p_t);
    _p_0 = _mm_add_epi16(_p_0, _p_s);

    _p_s = _mm_sub_epi16(_b_2, _b_3);
    _p_t = _mm_srai_epi16(_p_s, 15);
    _p_s = _mm_xor_si128(_p_s, _p_t);
    _p_s = _mm_sub_epi16(_p_s, _p_t);
    _p_0 = _mm_add_epi16(_p_0, _p_s);
    _b_2 = _mm_add_epi16(_b_2, _b_3);
    _p_t = _mm_srai_epi16(_b_2, 15);
    _b_2 = _mm_xor_si128(_b_2, _p_t);
    _b_2 = _mm_sub_epi16(_b_2, _p_t);
    _p_0 = _mm_add_epi16(_p_0, _b_2);

    _p_s = _mm_sub_epi16(_p_4, _p_5);
    _p_t = _mm_srai_epi16(_p_s, 15);
    _p_s = _mm_xor_si128(_p_s, _p_t);
    _p_s = _mm_sub_epi16(_p_s, _p_t);
    _p_0 = _mm_add_epi16(_p_0, _p_s);
    _p_4 = _mm_add_epi16(_p_4, _p_5);
    _p_t = _mm_srai_epi16(_p_4, 15);
    _p_4 = _mm_xor_si128(_p_4, _p_t);
    _p_4 = _mm_sub_epi16(_p_4, _p_t);
    _p_0 = _mm_add_epi16(_p_0, _p_4);

    _p_s = _mm_sub_epi16(_b_6, _b_7);
    _p_t = _mm_srai_epi16(_p_s, 15);
    _p_s = _mm_xor_si128(_p_s, _p_t);
    _p_s = _mm_sub_epi16(_p_s, _p_t);
    _p_0 = _mm_add_epi16(_p_0, _p_s);
    _b_6 = _mm_add_epi16(_b_6, _b_7);
    _p_t = _mm_srai_epi16(_b_6, 15);
    _b_6 = _mm_xor_si128(_b_6, _p_t);
    _b_6 = _mm_sub_epi16(_b_6, _p_t);
    _p_0 = _mm_add_epi16(_p_0, _b_6);

    _p_t = _mm_srli_si128(_p_0, 8);
    _p_0 = _mm_add_epi16(_p_0, _p_t);
    _p_t = _mm_srli_si128(_p_0, 4);
    _p_0 = _mm_add_epi16(_p_0, _p_t);
    s = _mm_cvtsi128_si32(_p_0);
    satd += (s >> 16) + (Ipp16s)s;

    return satd;
}
#endif


}//namespace UMC
#endif
