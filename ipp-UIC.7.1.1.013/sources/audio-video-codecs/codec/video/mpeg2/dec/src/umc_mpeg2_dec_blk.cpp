/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_VIDEO_DECODER

#include "umc_mpeg2_dec_defs.h"

static QUANT_MATRIX_TYPE Qmatrix[64] =
{
    16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,
};

#define UHBITS(code, nbits) (((Ipp32u)(code)) >> (32 - (nbits)))
#define SHBITS(code, nbits) (((Ipp32s)(code)) >> (32 - (nbits)))

#define TAB1_OFFSET_10BIT 248
#define TAB1_OFFSET_15BIT 360
#define TAB1_OFFSET_16BIT 408
#define TAB1_OFFSET_8BIT_INTRA 432
#define TAB1_OFFSET_10BIT_INTRA 680

const Ipp16u MPEG2_VLC_TAB1[] =
{
  0xae2, 0xae2,
  0x6e9, 0x6e9,
  0x12e0, 0x12e0,
  0x6e8, 0x6e8,
  0x6c7, 0x6c7, 0x6c7, 0x6c7,
  0x6c6, 0x6c6, 0x6c6, 0x6c6,
  0xac1, 0xac1, 0xac1, 0xac1,
  0x6c5, 0x6c5, 0x6c5, 0x6c5,
  0x70d,
  0x1b00,
  0x70c,
  0x70b,
  0xb03,
  0xf01,
  0x1700,
  0x70a,
  0xea0, 0xea0, 0xea0, 0xea0, 0xea0, 0xea0, 0xea0, 0xea0, // 0010 10xx
  0x6a4, 0x6a4, 0x6a4, 0x6a4, 0x6a4, 0x6a4, 0x6a4, 0x6a4, // 0011 00xx
  0x6a3, 0x6a3, 0x6a3, 0x6a3, 0x6a3, 0x6a3, 0x6a3, 0x6a3, // 0011 10xx
  0xa80, 0xa80, 0xa80, 0xa80, 0xa80, 0xa80, 0xa80, 0xa80,
  0xa80, 0xa80, 0xa80, 0xa80, 0xa80, 0xa80, 0xa80, 0xa80, // 0100 0xxx
  0x682, 0x682, 0x682, 0x682, 0x682, 0x682, 0x682, 0x682,
  0x682, 0x682, 0x682, 0x682, 0x682, 0x682, 0x682, 0x682, // 0101 0xxx
  0x661, 0x661, 0x661, 0x661, 0x661, 0x661, 0x661, 0x661,
  0x661, 0x661, 0x661, 0x661, 0x661, 0x661, 0x661, 0x661,
  0x661, 0x661, 0x661, 0x661, 0x661, 0x661, 0x661, 0x661,
  0x661, 0x661, 0x661, 0x661, 0x661, 0x661, 0x661, 0x661, // 0110 xxxx
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, 0x640, // 264

  /* 13-bit code */
  0xbaa, 0xba9, 0xfa5, 0x13a3,
  0x17a2, 0x1fa1, 0x1ba1, 0x3fa0,
  0x3ba0, 0x37a0, 0x33a0, 0x7ba,
  0x7b9, 0x7b8, 0x7b7, 0x7b6,
  0x2f80, 0x2f80, 0xb88, 0xb88,
  0xf84, 0xf84, 0x2b80, 0x2b80,
  0x1382, 0x1382, 0xb87, 0xb87,
  0x795, 0x795, 0x794, 0x794,
  0x2780, 0x2780, 0x793, 0x793,
  0x792, 0x792, 0x1781, 0x1781,
  0xf83, 0xf83, 0x2380, 0x2380,
  0xb86, 0xb86, 0x791, 0x791, // 312

  /* 10-bit code  */
  0x750, 0x750, 0x750, 0x750, 0x750, 0x750, 0x750, 0x750,
  0xb45, 0xb45, 0xb45, 0xb45, 0xb45, 0xb45, 0xb45, 0xb45,
  0x1f40, 0x1f40, 0x1f40, 0x1f40, 0x1f40, 0x1f40, 0x1f40, 0x1f40,
  0xf42, 0xf42, 0xf42, 0xf42, 0xf42, 0xf42, 0xf42, 0xf42,
  0x1341, 0x1341, 0x1341, 0x1341, 0x1341, 0x1341, 0x1341, 0x1341,
  0x74f, 0x74f, 0x74f, 0x74f, 0x74f, 0x74f, 0x74f, 0x74f,
  0x74e, 0x74e, 0x74e, 0x74e, 0x74e, 0x74e, 0x74e, 0x74e,
  0xb44, 0xb44, 0xb44, 0xb44, 0xb44, 0xb44, 0xb44, 0xb44, // 376

  /* 15-bit code */
  0xa3e0, 0x9fe0, 0x9be0, 0x97e0,
  0x93e0, 0x8fe0, 0x8be0, 0x87e0,
  0x83e0, 0x3be1, 0x37e1, 0x33e1,
  0x2fe1, 0x2be1, 0x27e1, 0x23e1,
  0x7fc0, 0x7fc0, 0x7bc0, 0x7bc0,
  0x77c0, 0x77c0, 0x73c0, 0x73c0,
  0x6fc0, 0x6fc0, 0x6bc0, 0x6bc0,
  0x67c0, 0x67c0, 0x63c0, 0x63c0,
  0x5fc0, 0x5fc0, 0x5bc0, 0x5bc0,
  0x57c0, 0x57c0, 0x53c0, 0x53c0,
  0x4fc0, 0x4fc0, 0x4bc0, 0x4bc0,
  0x47c0, 0x47c0, 0x43c0, 0x43c0, // 424
  /* 16-bit code */
  0x4a01, 0x4601, 0x4201, 0x3e01,
  0xe06, 0xa10, 0xa0f, 0xa0e,
  0xa0d, 0xa0c, 0xa0b, 0x61f,
  0x61e, 0x61d, 0x61c, 0x61b, // 440

  /* 8-bit code for B15 */
  0x6e7, 0x6e7, 0x6e8, 0x6e8,
  0x6e6, 0x6e6, 0xae2, 0xae2,
  0x1ec0, 0x1ec0, 0x1ec0, 0x1ec0,
  0x1ac0, 0x1ac0, 0x1ac0, 0x1ac0,
  0x6c4, 0x6c4, 0x6c4, 0x6c4,
  0x6c5, 0x6c5, 0x6c5, 0x6c5,
  0x1701, 0x70b, 0x2f00, 0x2b00,
  0x70d, 0x70c, 0xb03, 0x1301,
  0x6a2, 0x6a2, 0x6a2, 0x6a2,
  0x6a2, 0x6a2, 0x6a2, 0x6a2,
  0xaa1, 0xaa1, 0xaa1, 0xaa1,
  0xaa1, 0xaa1, 0xaa1, 0xaa1,
  0x6a3, 0x6a3, 0x6a3, 0x6a3,
  0x6a3, 0x6a3, 0x6a3, 0x6a3,
  0x661, 0x661, 0x661, 0x661,
  0x661, 0x661, 0x661, 0x661,
  0x661, 0x661, 0x661, 0x661,
  0x661, 0x661, 0x661, 0x661,
  0x661, 0x661, 0x661, 0x661,
  0x661, 0x661, 0x661, 0x661,
  0x661, 0x661, 0x661, 0x661,
  0x661, 0x661, 0x661, 0x661,
  0xffff, 0xffff, 0xffff, 0xffff,
  0xffff, 0xffff, 0xffff, 0xffff,
  0xffff, 0xffff, 0xffff, 0xffff,
  0xffff, 0xffff, 0xffff, 0xffff,
  0xe80, 0xe80, 0xe80, 0xe80,
  0xe80, 0xe80, 0xe80, 0xe80,
  0xe80, 0xe80, 0xe80, 0xe80,
  0xe80, 0xe80, 0xe80, 0xe80,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0x640, 0x640, 0x640, 0x640,
  0xa60, 0xa60, 0xa60, 0xa60,
  0xa60, 0xa60, 0xa60, 0xa60,
  0xa60, 0xa60, 0xa60, 0xa60,
  0xa60, 0xa60, 0xa60, 0xa60,
  0xa60, 0xa60, 0xa60, 0xa60,
  0xa60, 0xa60, 0xa60, 0xa60,
  0xa60, 0xa60, 0xa60, 0xa60,
  0xa60, 0xa60, 0xa60, 0xa60,
  0x12a0, 0x12a0, 0x12a0, 0x12a0,
  0x12a0, 0x12a0, 0x12a0, 0x12a0,
  0x16a0, 0x16a0, 0x16a0, 0x16a0,
  0x16a0, 0x16a0, 0x16a0, 0x16a0,
  0x6e9, 0x6e9, 0xee1, 0xee1,
  0x6ea, 0x6ea, 0x22e0, 0x22e0,
  0x26e0, 0x26e0, 0x3300, 0x3700,
  0xf02, 0xb04, 0x3b00, 0x3f00, // 688
  /* 10-bit code for B15 */
  0xb25, 0xb25, 0x72e, 0x72e,
  0x1342, 0x750, 0x72f, 0x72f,
};

const Ipp8u MPEG2_DCSIZE_TAB[128] = {
  0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
  0x30, 0x30, 0x30, 0x30, 0x33, 0x33, 0x33, 0x33,
  0x34, 0x34, 0x34, 0x34, 0x45, 0x45, 0x56, 0x00,
  0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67,
  0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67,
  0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
  0x89, 0x89, 0x89, 0x89, 0x9a, 0x9a, 0x9b, 0x9b,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
  0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x55, 0x00,
  0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
  0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
  0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
  0x88, 0x88, 0x88, 0x88, 0x99, 0x99, 0xaa, 0xab
};

static DECLALIGN(16) SCAN_MATRIX_TYPE scan_tbl[2][64] =
{
  {
     0, 1, 8, 16,9, 2, 3, 10,
     17,24,32,25,18,11,4, 5,
     12,19,26,33,40,48,41,34,
     27,20,13, 6, 7,14,21,28,
     35,42,49,56,57,50,43,36,
     29,22,15,23,30,37,44,51,
     58,59,52,45,38,31,39,46,
     53,60,61,54,47,55,62,63
  },
  {
     0, 8, 16,24,1, 9, 2, 10,
     17,25,32,40,48,56,57,49,
     41,33,26,18, 3,11,4, 12,
     19,27,34,42,50,58,35,43,
     51,59,20,28,5 ,13,6 ,14,
     21,29,36,44,52,60,37,45,
     53,61,22,30,7 ,15,23,31,
     38,46,54,62,39,47,55,63
  }
};


Ipp8u default_intra_quantizer_matrix[64] =
{
   8, 16, 16, 19, 16, 19, 22, 22,
  22, 22, 22, 22, 26, 24, 26, 27,
  27, 27, 26, 26, 26, 26, 27, 27,
  27, 29, 29, 29, 34, 34, 34, 29,
  29, 29, 27, 27, 29, 29, 32, 32,
  34, 34, 37, 38, 37, 35, 35, 34,
  35, 38, 38, 40, 40, 40, 48, 48,
  46, 46, 56, 56, 58, 69, 69, 83
};

/***************************************************************/

#define DEF_VARS()                                                  \
  SCAN_MATRIX_TYPE *scanMatrix = pQuantSpec->scanMatrix;            \
  QUANT_MATRIX_TYPE *pQuantMatrix = pQuantSpec->quantMatrix;        \
  DEF_BLOCK(pDstBlock);                                             \
  Ipp8u *BS_curr_ptr;                                               \
  Ipp32s BS_bit_offset;                                             \
  Ipp32s i, j, k;                                                   \
  Ipp32s val, run, len, sign;                                       \
  Ipp32u tbl;                                                       \
  Ipp32u code

/***************************************************************/

/* ///////////////////////////////////////////////////////////////////////////
//  Name:
//    ownDecodeInter8x8IDCTAdd_MPEG2_1u8u
//
//  Purpose:
//    Performs VLC decoding of DCT coefficients for one inter 8x8 block,
//    dequantization of coefficients, inverse DCT and addition of resulted
//    8x8 block to destination.
//
//  Parameters:
//    ppBitStream      Pointer to the pointer to the current byte in
//                     the bitstream, it is updated after block decoding.
//    pBitOffset       Pointer to the bit position in the byte pointed by
//                     *ppBitStream, it is updated after block decoding.
//                     Must be in the range [0, 7].
//    pQuantSpec       Pointer to the structure DecodeInterSpec_MPEG2
//    QP               Quantization parameter.
//    pSrcDst          Pointer to the 8x8 block in the destination image
//    srcDstStep       Step through the destination image
//
//  Returns:
//    ippStsNoErr        No error.
//    ippStsNullPtrErr   One of the specified pointers is NULL.
//    ippStsVLCErr       An illegal code is detected through the
//                       stream processing.
*/

MP2_FUNC(IppStatus, ownDecodeInter8x8IDCTAdd_MPEG2_1u8u, (
    Ipp8u**                            BitStream_curr_ptr,
    Ipp32s*                            BitStream_bit_offset,
    DecodeInterSpec_MPEG2*         pQuantSpec,
    Ipp32s                             quant,
    Ipp8u*                             pSrcDst,
    Ipp32s                             srcDstStep))
{
  DEF_VARS();
  DEF_BLOCK(idct);
  Ipp32s mask;
  Ipp32s q;

  COPY_BITSTREAM(BS, *BitStream)
  SHOW_HI9BITS(BS, code);

  if (pQuantMatrix) {
    if (code & 0x80000000) { /* first 2-bit code */
      val = quant*pQuantMatrix[0];
      val = (val + val + val) >> 5;
      sign = SHBITS(code + code, 1);
      APPLY_SIGN(val, sign);
      if ((code & 0x30000000) == 0x20000000) { /* end */
        SKIP_BITS(BS, 4);
        COPY_BITSTREAM(*BitStream, BS)
        IDCT_INTER1(val, idct, pSrcDst, srcDstStep);
        return ippStsOk;
      }
      for (k = 0; k < 64; k++) {
        pDstBlock[k] = 0;
      }
      pDstBlock[0] = (Ipp16s)val;
      mask = 1 ^ val;
      SKIP_BITS(BS, 2);
      SHOW_HI9BITS(BS, code);
      i = 0;
    } else {
      for (k = 0; k < 64; k++) {
        pDstBlock[k] = 0;
      }
      i = -1;
      mask = 1;
    }

    for (;;) {
      if ((code & 0xc0000000) == 0x80000000) {
        break;
      } else if (code >= 0x08000000) {
        tbl = MPEG2_VLC_TAB1[UHBITS(code - 0x08000000, 8)];
common:
        i++;
        UNPACK_VLC1(tbl, run, val, len)

        i += run;
        i &= 63; // just in case
        j = scanMatrix[i];

        q = pQuantMatrix[j];
        val = val * quant;
        val = (val * q) >> 5;
        sign = SHBITS(code << len, 1);
        APPLY_SIGN(val, sign);
        SKIP_BITS(BS, (len+1));
        pDstBlock[j] = (Ipp16s)val;
        mask ^= val;
        SHOW_HI9BITS(BS, code);
        continue;
      } else if (code >= 0x04000000) {
        EXPAND_17BITS(BS, code);
        i += 1 + UHBITS(code << 6, 6);
        i &= 63;
        j = scanMatrix[i];

        EXPAND_25BITS(BS, code);
        code <<= 12;

        q = quant*pQuantMatrix[j];
        val = 2 * (SHBITS(code, 12) + SHBITS(code, 1)) + 1;
        val = (val * q) / 32;
        SAT(val)
        pDstBlock[j] = (Ipp16s)val;
        mask ^= val;

        SKIP_BITS(BS, 24);
        SHOW_HI9BITS(BS, code);
        continue;
      } else {
        EXPAND_17BITS(BS, code);
        if (code >= 0x00800000) {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_10BIT + UHBITS(code, 13)];
          goto common;
        } else if (code >= 0x00200000) {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_15BIT + UHBITS(code, 15)];
          goto common;
        } else {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_16BIT + UHBITS(code, 16)];
          if (code < 16)
            return ippStsVLCErr;
          code <<= 16;
          SKIP_BITS(BS, 16);
          goto common;
        }
      }
    }
  } else {
    Ipp32s q_mul1 = quant << 3;
    //pQuantMatrix = Qmatrix;
    if (code & 0x80000000) { /* first 2-bit code */
      val = q_mul1 + q_mul1 + (q_mul1 &~ 15);
      val >>= 4;
      sign = SHBITS(code + code, 1);
      APPLY_SIGN(val, sign);
      if ((code & 0x30000000) == 0x20000000) { /* end */
        SKIP_BITS(BS, 4);
        COPY_BITSTREAM(*BitStream, BS)
        IDCT_INTER1(val, idct, pSrcDst, srcDstStep);
        return ippStsOk;
      }
      for (k = 0; k < 64; k++) {
        pDstBlock[k] = 0;
      }
      pDstBlock[0] = (Ipp16s)val;
      mask = 1 ^ val;
      SKIP_BITS(BS, 2);
      SHOW_HI9BITS(BS, code);
      i = 0;
    } else {
      for (k = 0; k < 64; k++) {
        pDstBlock[k] = 0;
      }
      i = -1;
      mask = 1;
    }

    for (;;) {
      if ((code & 0xc0000000) == 0x80000000) {
        break;
      } else if (code >= 0x08000000) {
        tbl = MPEG2_VLC_TAB1[UHBITS(code - 0x08000000, 8)];
common2:

        i++;
        UNPACK_VLC1(tbl, run, val, len)

        i += run;
        i &= 63; // just in case

        j = scanMatrix[i];

        val = (val*q_mul1) >> 4;
        sign = SHBITS(code << len, 1);
        APPLY_SIGN(val, sign);
        SKIP_BITS(BS, (len+1));
        pDstBlock[j] = (Ipp16s)val;
        mask ^= val;
        SHOW_HI9BITS(BS, code);
        continue;
      } else if (code >= 0x04000000) {
        EXPAND_17BITS(BS, code);
        i += 1 + UHBITS(code << 6, 6);
        i &= 63;
        j = scanMatrix[i];

        EXPAND_25BITS(BS, code);
        code <<= 12;
        val = 2 * (SHBITS(code, 12) + SHBITS(code, 1)) + 1;
        val = (val * quant) / 2;
        SAT(val)
        pDstBlock[j] = (Ipp16s)val;
        mask ^= val;

        SKIP_BITS(BS, 24);
        SHOW_HI9BITS(BS, code);
        continue;
      } else {
        EXPAND_17BITS(BS, code);
        if (code >= 0x00800000) {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_10BIT + UHBITS(code, 13)];
          goto common2;
        } else if (code >= 0x00200000) {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_15BIT + UHBITS(code, 15)];
          goto common2;
        } else {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_16BIT + UHBITS(code, 16)];
          if (code < 16)
            return ippStsVLCErr;
          code <<= 16;
          SKIP_BITS(BS, 16);
          goto common2;
        }
      }
    }
  }

  pDstBlock[63] ^= mask & 1;
  SKIP_BITS(BS, 2);
  COPY_BITSTREAM(*BitStream, BS)

  IDCT_INTER(pDstBlock, i, idct, pSrcDst, srcDstStep);

  return ippStsOk;
}

/* ///////////////////////////////////////////////////////////////////////////
//  Name:
//    ownDecodeIntra8x8IDCT_MPEG2_1u8u
//    ownDecodeIntra8x8IDCT_MPEG1_1u8u
//
//  Purpose:
//    Performs VLC decoding of DCT coefficients for one intra 8x8 block,
//    dequantization of coefficients, inverse DCT and storing of resulted
//    8x8 block to destination.
//
//  Parameters:
//    ppBitStream      Pointer to the pointer to the current byte in
//                     the bitstream, it is updated after block decoding.
//    pBitOffset       Pointer to the bit position in the byte pointed by
//                     *ppBitStream, it is updated after block decoding.
//                     Must be in the range [0, 7].
//    pQuantSpec       Pointer to the structure DecodeIntraSpec_MPEG2
//    QP               Quantization parameter.
//    blockType        Indicates the type of block, takes one of the following
//                     values:
//                         IPPVC_BLOCK_LUMA - for luma blocks,
//                         IPPVC_BLOCK_CHROMA - for chroma blocks
//                     And in case of MPEG1 D-type block, IPPVC_BLOCK_MPEG1_DTYPE
//                     have to be added to IPPVC_BLOCK_LUMA or IPPVC_BLOCK_CHROMA.
//    pDCPred          Pointer to the value to be added to the DC coefficient
//    pDst             Pointer to the 8x8 block in the destination image
//    dstStep          Step through the destination image
//
//  Returns:
//    ippStsNoErr        No error.
//    ippStsNullPtrErr   One of the specified pointers is NULL.
//    ippStsVLCErr       An illegal code is detected through the
//                       stream processing.
*/

MP2_FUNC(IppStatus, ownDecodeIntra8x8IDCT_MPEG2_1u8u, (
    Ipp8u**                            BitStream_curr_ptr,
    Ipp32s*                            BitStream_bit_offset,
    DecodeIntraSpec_MPEG2*         pQuantSpec,
    Ipp32s                             quant,
    Ipp32s                             chromaFlag,
    Ipp16s*                            dct_dc_past,
    Ipp8u*                             pDst,
    Ipp32s                             dstStep))
{
  Ipp32s intra_vlc_format = pQuantSpec->intraVLCFormat;
  Ipp32s intra_dc_shift = pQuantSpec->intraShiftDC;
  DEF_VARS();
  Ipp32s mask;

  COPY_BITSTREAM(BS, *BitStream)

  /* DC */
  DECODE_DC(val)
  val += *dct_dc_past;
  *dct_dc_past = (Ipp16s)val;
  val <<= intra_dc_shift;
#ifdef MISMATCH_INTRA
  mask = ~val;
#endif
  i = 0;

  SHOW_HI9BITS(BS, code);

  if (intra_vlc_format) {
    if ((code & 0xf0000000) == 0x60000000) { /* end */
      SKIP_BITS(BS, 4);
      COPY_BITSTREAM(*BitStream, BS)
      IDCT_INTRA1(val, idct, pDst, dstStep);
      return ippStsOk;
    }
    for (k = 0; k < 64; k++) {
      pDstBlock[k] = 0;
    }
    pDstBlock[0] = (Ipp16s)val;
    for (;;) {
      if ((code & 0xf0000000) == 0x60000000) {
        break;
      } else if (code >= 0x08000000) {
        tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_8BIT_INTRA + UHBITS(code, 8)];
      } else if (code >= 0x04000000) {
          EXPAND_17BITS(BS, code);
          i += 1 + UHBITS (code << 6, 6);
          i &= 63;
          j = scanMatrix[i];

          EXPAND_25BITS(BS, code);
          code <<= 12;
          val = SHBITS(code, 12);
          val = (val * (quant*pQuantMatrix[j])) / 16;
          SAT(val)
          pDstBlock[j] = (Ipp16s)val;
#ifdef MISMATCH_INTRA
          mask ^= val;
#endif
          SKIP_BITS(BS, 24);
          SHOW_HI9BITS(BS, code);
          continue;
      } else {
        EXPAND_17BITS(BS, code);
        if (code >= 0x02000000) {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_10BIT_INTRA + UHBITS(code, 10)];
        } else if (code >= 0x00800000) {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_10BIT + UHBITS(code, 13)];
        } else if (code >= 0x00200000) {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_15BIT + UHBITS(code, 15)];
        } else {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_16BIT + UHBITS(code, 16)];
          if (code < 16)
            return ippStsVLCErr;
          code <<= 16;
          SKIP_BITS(BS, 16);
        }
      }

      i++;
      UNPACK_VLC2(tbl, run, val, len)

      i += run;
      i &= 63;
      j = scanMatrix[i];
      val = (val*(quant*pQuantMatrix[j])) >> 4;
      sign = SHBITS(code << len, 1);
      APPLY_SIGN(val, sign);
      SKIP_BITS(BS, (len + 1));
      pDstBlock[j] = (Ipp16s)val;
#ifdef MISMATCH_INTRA
      mask ^= val;
#endif
      SHOW_HI9BITS(BS, code);
    }
#ifdef MISMATCH_INTRA
    pDstBlock[63] ^= mask & 1;
#endif
    SKIP_BITS(BS, 4);
    COPY_BITSTREAM(*BitStream, BS)
  } else {
    for (k = 0; k < 64; k++) {
      pDstBlock[k] = 0;
    }
    pDstBlock[0] = (Ipp16s)val;
    for (;;) {
      if ((code & 0xc0000000) == 0x80000000) {
        break;
      } else if (code >= 0x08000000) {
        tbl = MPEG2_VLC_TAB1[UHBITS(code - 0x08000000, 8)];
      } else if (code >= 0x04000000) {
        EXPAND_17BITS(BS, code);
        i += 1 + UHBITS (code << 6, 6);
        i &= 63;
        j = scanMatrix[i];

        EXPAND_25BITS(BS, code);
        code <<= 12;
        val = SHBITS(code, 12);
        val = (val * (quant*pQuantMatrix[j])) / 16;
        SAT(val)
        pDstBlock[j] = (Ipp16s)val;
#ifdef MISMATCH_INTRA
        mask ^= val;
#endif

        SKIP_BITS(BS, 24);
        SHOW_HI9BITS(BS, code);
        continue;
      } else {
        EXPAND_17BITS(BS, code);

        if (code >= 0x00800000) {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_10BIT + UHBITS(code, 13)];
        } else if (code >= 0x00200000) {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_15BIT + UHBITS(code, 15)];
        } else {
          tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_16BIT + UHBITS(code, 16)];
          if (code < 16)
            return ippStsVLCErr;
          code <<= 16;
          SKIP_BITS(BS, 16);
        }
      }

      i++;
      UNPACK_VLC2(tbl, run, val, len);

      i += run;
      i &= 63;
      j = scanMatrix[i];
      val = (val*(quant*pQuantMatrix[j])) >> 4;
      sign = SHBITS(code << len, 1);
      APPLY_SIGN(val, sign);
      SKIP_BITS(BS, (len + 1));
      pDstBlock[j] = (Ipp16s)val;
#ifdef MISMATCH_INTRA
      mask ^= val;
#endif
      SHOW_HI9BITS(BS, code);
    }
#ifdef MISMATCH_INTRA
    pDstBlock[63] ^= mask & 1;
#endif
    SKIP_BITS(BS, 2);
    COPY_BITSTREAM(*BitStream, BS)
  }

  IDCT_INTRA(pDstBlock, i, idct, pDst, dstStep);

  return ippStsOk;
}

/********************************************************************/

MP2_FUNC(IppStatus, ownDecodeInter8x8IDCTAdd_MPEG1_1u8u, (
    Ipp8u**                            BitStream_curr_ptr,
    Ipp32s*                            BitStream_bit_offset,
    DecodeInterSpec_MPEG2*         pQuantSpec,
    Ipp32s                             quant,
    Ipp8u*                             pSrcDst,
    Ipp32s                             srcDstStep))
{
  DEF_VARS();
  DEF_BLOCK(idct);

  COPY_BITSTREAM(BS, *BitStream)

  if (!pQuantMatrix) pQuantMatrix = Qmatrix;

  SHOW_HI9BITS(BS, code);

  if (code & 0x80000000) { /* first 2-bit code */
    val = (3*quant*pQuantMatrix[0]) >> 4;
    val = (val - 1) | 1;
    sign = SHBITS(code + code, 1);
    APPLY_SIGN(val, sign);

    for (k = 0; k < 64; k++) {
      pDstBlock[k] = 0;
    }
    pDstBlock[0] = (Ipp16s)val;

    SKIP_BITS(BS, 2);
    SHOW_HI9BITS(BS, code);
    i = 0;
  } else {
    for (k = 0; k < 64; k++) {
      pDstBlock[k] = 0;
    }
    i = -1;
  }

  for (;;) {
    if ((code & 0xc0000000) == 0x80000000) {
      break;
    } else if (code >= 0x08000000) {
      tbl = MPEG2_VLC_TAB1[UHBITS(code - 0x08000000, 8)];
common:
      i++;
      UNPACK_VLC1(tbl, run, val, len)

      i += run;
      i &= 63;
      j = scanMatrix[i];
      val = (val * (quant*pQuantMatrix[j])) >> 4;
      val = (val - 1) | 1;
      sign = SHBITS(code << len, 1);
      APPLY_SIGN(val, sign);

      SKIP_BITS(BS, (len + 1));
      pDstBlock[j] = (Ipp16s)val;
      SHOW_HI9BITS(BS, code);
      continue;
    } else if (code >= 0x04000000) {
      EXPAND_17BITS(BS, code);
      i += 1 + UHBITS (code << 6, 6);
      i &= 63;
      j = scanMatrix[i];

      SKIP_BITS(BS, 12);
      GET_TO9BITS(BS, 8, val);
      val = (Ipp8s)val;
      if (!(val & 0x7f))
      {
        Ipp32s val2;
        GET_TO9BITS(BS, 8, val2);
        val = 2*val + val2;
      }
      sign = SHBITS(val, 1);
      val = 2*(val + sign) + 1;
      val = (val * (quant*pQuantMatrix[j])) / 16;
      val = (val + ~sign) | 1;

      SAT(val)
      pDstBlock[j] = (Ipp16s)val;

      SHOW_HI9BITS(BS, code);
      continue;
    } else {
      EXPAND_17BITS(BS, code);
      if (code >= 0x00800000) {
        tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_10BIT + UHBITS(code, 13)];
        goto common;
      } else if (code >= 0x00200000) {
        tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_15BIT + UHBITS(code, 15)];
        goto common;
      } else {
        tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_16BIT + UHBITS(code, 16)];
        if (code < 16)
          return ippStsVLCErr;
        code <<= 16;
        SKIP_BITS(BS, 16);
        goto common;
      }
    }
  }

  SKIP_BITS(BS, 2);
  COPY_BITSTREAM(*BitStream, BS)

  IDCT_INTER(pDstBlock, i, idct, pSrcDst, srcDstStep);

  return ippStsOk;
}

MP2_FUNC(IppStatus, ownDecodeIntra8x8IDCT_MPEG1_1u8u, (
    Ipp8u**                            BitStream_curr_ptr,
    Ipp32s*                            BitStream_bit_offset,
    DecodeIntraSpec_MPEG2*         pQuantSpec,
    Ipp32s                             quant,
    Ipp32s                             blockType,
    Ipp16s*                            dct_dc_past,
    Ipp8u*                             pDst,
    Ipp32s                             dstStep))
{
  DEF_VARS();
  Ipp32s chromaFlag = blockType & 1;

  for (k = 0; k < 64; k++) {
    pDstBlock[k] = 0;
  }

  COPY_BITSTREAM(BS, *BitStream)

  /* DC */
  DECODE_DC(val)
  val += *dct_dc_past;
  *dct_dc_past = (Ipp16s)val;
  pDstBlock[0] = (Ipp16s)(val << 3);

  if (blockType & 4) { // D-type picture, DC coef only
    COPY_BITSTREAM(*BitStream, BS)
    //IDCT_INTRA(pDstBlock, 0, idct, pDst, dstStep);
    ippiDCT8x8Inv_AANTransposed_16s8u_C1R(pDstBlock, pDst, dstStep, 0);
    return ippStsOk;
  }

  i = 0;
  SHOW_HI9BITS(BS, code);

  for (;;) {
    if ((code & 0xc0000000) == 0x80000000) {
      break;
    } else if (code >= 0x08000000) {
      tbl = MPEG2_VLC_TAB1[UHBITS(code - 0x08000000, 8)];
    } else if (code >= 0x04000000) {
      EXPAND_17BITS(BS, code);
      i += 1 + UHBITS(code << 6, 6);
      i &= 63;
      j = scanMatrix[i];

      SKIP_BITS(BS, 12);
      GET_TO9BITS(BS, 8, val);
      val = (Ipp8s)val;
      if (!(val & (Ipp8s)0x7f))
      {
        Ipp16s base = (val & 0x80) ? -256 : 0;
        GET_TO9BITS(BS, 8, val);
        val = (Ipp16s)(val+base);
      }
      sign = SHBITS(val, 1);
      val = (val * (quant*pQuantMatrix[j])) / 8;
      val = (val + ~sign) | 1;

      SAT(val)
      pDstBlock[j] = (Ipp16s)val;

      SHOW_HI9BITS(BS, code);
      continue;
    } else {

      EXPAND_17BITS(BS, code);
      if (code >= 0x00800000) {
        tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_10BIT + UHBITS(code, 13)];
      } else if (code >= 0x00200000) {
        tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_15BIT + UHBITS(code, 15)];
      } else {
        tbl = MPEG2_VLC_TAB1[TAB1_OFFSET_16BIT + UHBITS(code, 16)];
        if (code < 16)
          return ippStsVLCErr;
        code <<= 16;
        SKIP_BITS(BS, 16);
      }
    }
    i++;
    UNPACK_VLC2(tbl, run, val, len)
    i += run;
    i &= 63;
    j = scanMatrix[i];
    val = (val*(quant*pQuantMatrix[j])) >> 3;
    sign = SHBITS(code << len, 1);
    val = (val-1) | 1;
    APPLY_SIGN(val, sign);

    SKIP_BITS(BS, (len + 1));
    pDstBlock[j] = (Ipp16s)val;
    SHOW_HI9BITS(BS, code);
  }

  SKIP_BITS(BS, 2);
  COPY_BITSTREAM(*BitStream, BS)

  IDCT_INTRA(pDstBlock, i, idct, pDst, dstStep);

  return ippStsOk;
}

/*******************************************************/

/* ///////////////////////////////////////////////////////////////////////////
//  Name:
//    ownDecodeIntraInit_MPEG2
//    ownDecodeInterInit_MPEG2
//
//  Purpose:
//    Initialized IppiDecodeIntraSpec_16s(DecodeInterSpec_MPEG2) structure.
//    If pQuantMatrix is NULL this means default quantization matrix.
//
//  Parameters:
//    pQuantMatrix   Pointer to the quantization matrix size of 64.
//    scan           Type of the scan, takes one of the following values:
//                       IPPVC_SCAN_ZIGZAG, indicating the classical zigzag scan,
//                       IPPVC_SCAN_VERTICAL - alternate-vertical scan
//    intraVLCFormat 0 or 1, defines one of two VLC tables for decoding intra blocks
//    intraShiftDC   Integer value for shifting intra DC coefficient after VLC decoding
//    pSpec          Pointer to the structure IppiDecodeIntraSpec_16s or
//                   DecodeInterSpec_MPEG2 which will initialized.
//
//  Returns:
//    ippStsNoErr        No error.
//    ippStsNullPtrErr   Indicates an error when pointer pSpec is NULL.
*/

MP2_FUNC(IppStatus, ownDecodeIntraInit_MPEG2, (
    const Ipp8u*                 pQuantMatrix,
    Ipp32s                       flag,
    Ipp32s                       intraVLCFormat,
    Ipp32s                       intraShiftDC,
    DecodeIntraSpec_MPEG2*   pSpec))
{
  SCAN_MATRIX_TYPE *scan = scan_tbl[flag & 1]; // IPPVC_ALT_SCAN == 1
  pSpec->intraVLCFormat = intraVLCFormat;
  pSpec->intraShiftDC = intraShiftDC;
  if (!(flag & IPPVC_LEAVE_SCAN_UNCHANGED)) {
    pSpec->scanMatrix = scan;
  }
  if (!(flag & IPPVC_LEAVE_QUANT_UNCHANGED)) {
    Ipp32s i;
    if (!pQuantMatrix) pQuantMatrix = default_intra_quantizer_matrix;
    pSpec->quantMatrix = pSpec->_quantMatrix;
    for (i = 0; i < 64; i++) {
      pSpec->quantMatrix[scan[i]] = pQuantMatrix[i];
    }
  }
  return ippStsNoErr;
}

MP2_FUNC(IppStatus, ownDecodeInterInit_MPEG2, (
    const Ipp8u*                 pQuantMatrix,
    Ipp32s                       flag,
    DecodeInterSpec_MPEG2*   pSpec))
{
  SCAN_MATRIX_TYPE *scan = scan_tbl[flag & 1];
  if (!(flag & IPPVC_LEAVE_SCAN_UNCHANGED)) {
    pSpec->scanMatrix = scan;
  }
  if (!(flag & IPPVC_LEAVE_QUANT_UNCHANGED)) {
    if (pQuantMatrix) {
      Ipp32s i, mask = 0;
      pSpec->quantMatrix = pSpec->_quantMatrix;
      for (i = 0; i < 64; i++) {
        pSpec->quantMatrix[scan[i]] = pQuantMatrix[i];
        mask |= pQuantMatrix[i];
      }
      if (mask == 16) { // default matrix
        pSpec->quantMatrix = NULL;
      }
    } else {
      pSpec->quantMatrix = NULL;
    }
  }
  return ippStsNoErr;
}

/* ///////////////////////////////////////////////////////////////////////////
//  Name:
//    ippiDecodeIntraGetSize_MPEG2
//    ippiDecodeInterGetSize_MPEG2
//
//  Purpose:
//    Return size of DecodeIntraSpec_MPEG2 or DecodeInterSpec_MPEG2.
//
//  Parameters:
//    pSpecSize Pointer to the resulting size of the structure
//    DecodeIntraSpec_MPEG2 or DecodeInterSpec_MPEG2.
//
//  Returns:
//    ippStsNoErr        No error.
//    ippStsNullPtrErr   Indicates an error when pointer pSpecSize is NULL.
*/

#if 0
MP2_FUNC(IppStatus, ippiDecodeIntraGetSize_MPEG2, (
    Ipp32s* pSpecSize))
{
  return ippStsNoErr;
}

MP2_FUNC(IppStatus, ippiDecodeInterGetSize_MPEG2, (
    Ipp32s* pSpecSize))
{
  return ippStsNoErr;
}
#endif

/* /////////////////////////////////////////////////////////////////////////// */

#ifdef USE_INTRINSICS

#include <emmintrin.h>

#define RND_VAL  8

DECLALIGN(16) static Ipp16s gg[] = {
  0x2000, 0x2000, 0x2000, 0x2000,
  0x2c62, 0x25a0, 0x1924, 0x08d4,
  0x29cf, 0x1151, 0xeeaf, 0xd631,
  0x25a0, 0xf72c, 0xd39e, 0xe6dc,
  RND_VAL, RND_VAL, RND_VAL, RND_VAL
};

/*static*/ void Pack8x8(Ipp16s* x, Ipp8u* y, Ipp32s step)
{
  __m128i *px = (__m128i*)x;
  __m128i zero;

  zero = _mm_setzero_si128();

  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(*px, zero));
  px++;
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(*px, zero));
  px++;
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(*px, zero));
  px++;
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(*px, zero));
  px++;
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(*px, zero));
  px++;
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(*px, zero));
  px++;
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(*px, zero));
  px++;
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(*px, zero));
}

/*static*/ void IDCTAdd_1x1to8x8(Ipp32s val, Ipp8u* y, Ipp32s step)
{
  __m128i v, zero;

  zero = _mm_setzero_si128();

  v = _mm_cvtsi32_si128((val + 4) >> 3);
  v = _mm_shufflelo_epi16(v, 0);
  v = _mm_unpacklo_epi64(v, v);

  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(v, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(v, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(v, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(v, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(v, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(v, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(v, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(v, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
}

/*static*/ void IDCTAdd_1x4to8x8(const Ipp16s* x, Ipp8u* y, Ipp32s step)
{
  __m128i s0, s1, g0, g1, t0, t1;
  __m128i r0, r1, r2, r3, zero;

  //s0 = _mm_loadl_epi64((__m128i*)x);
  s0 = _mm_unpacklo_epi16(_mm_loadl_epi64((__m128i*)x    ), _mm_loadl_epi64((__m128i*)x + 1));
  s1 = _mm_unpacklo_epi16(_mm_loadl_epi64((__m128i*)x + 2), _mm_loadl_epi64((__m128i*)x + 3));
  s0 = _mm_unpacklo_epi32(s0, s1);
  s0 = _mm_slli_epi16(s0, 4);

  s0 = _mm_unpacklo_epi16(s0, s0);
  s1 = _mm_unpackhi_epi16(s0, s0);
  s0 = _mm_unpacklo_epi16(s0, s0);
  g0 = _mm_load_si128((__m128i*)gg);
  g1 = _mm_load_si128((__m128i*)gg + 1);

  s0 = _mm_mulhi_epi16(s0, g0);
  s1 = _mm_mulhi_epi16(s1, g1);
  s0 = _mm_add_epi16(s0, s1);

  s0 = _mm_add_epi16(s0, _mm_loadl_epi64((__m128i*)gg + 2)); // round
  s1 = _mm_unpackhi_epi64(s0, s0);
  t0 = _mm_add_epi16(s0, s1);
  t1 = _mm_sub_epi16(s0, s1);
  t0 = _mm_srai_epi16(t0, 4);
  t1 = _mm_srai_epi16(t1, 4);

  zero = _mm_setzero_si128();

  t0 = _mm_unpacklo_epi16(t0, t0);
  t1 = _mm_unpacklo_epi16(t1, t1);

  r0 = _mm_unpacklo_epi16(t0, t0);
  r2 = _mm_unpackhi_epi16(t0, t0);
  r1 = _mm_unpackhi_epi16(r0, r0);
  r0 = _mm_unpacklo_epi16(r0, r0);
  r3 = _mm_unpackhi_epi16(r2, r2);
  r2 = _mm_unpacklo_epi16(r2, r2);

  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(r0, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(r1, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(r2, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(r3, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;

  r0 = _mm_unpacklo_epi16(t1, t1);
  r2 = _mm_unpackhi_epi16(t1, t1);
  r1 = _mm_unpackhi_epi16(r0, r0);
  r0 = _mm_unpacklo_epi16(r0, r0);
  r3 = _mm_unpackhi_epi16(r2, r2);
  r2 = _mm_unpacklo_epi16(r2, r2);

  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(r3, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(r2, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(r1, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
  y += step;
  _mm_storel_epi64((__m128i*)y, _mm_packus_epi16(_mm_add_epi16(r0, _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)y), zero)), zero));
}

#else

/*static*/ void IDCTAdd_1x1to8x8(Ipp32s val, Ipp8u* y, Ipp32s step) {
  Ipp32s i, j;
  val = (val + 4) >> 3;
  for (j = 0; j < 8; j++) {
    for (i = 0; i < 8; i++) {
      Ipp32s r = y[i] + val;
      if (r < 0) r = 0;
      if (r > 255) r = 255;
      y[i] = (Ipp8u)r;
    }
    y += step;
  }
}

#endif

#endif // UMC_ENABLE_MPEG2_VIDEO_DECODER
