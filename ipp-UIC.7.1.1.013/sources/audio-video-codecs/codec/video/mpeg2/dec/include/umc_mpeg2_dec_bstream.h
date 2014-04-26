/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
//
//
//              Bit Stream operations
*/

#ifndef __UMC_MPEG2_DEC_BSTREAM_H
#define __UMC_MPEG2_DEC_BSTREAM_H

#include "ippdefs.h"

#define KEEP_HISTORY
#define MISMATCH_INTRA
//#define SLICE_THREADING
//#define USE_INTRINSICS
//#define OVERLAY_SUPPORT
//#define MPEG2_USE_REF_IDCT

//#ifdef __ICL
#if (defined( _MSC_VER ) || defined( __ICL )) && (!defined(_WIN32_WCE))
    #define DECLALIGN(x) __declspec(align(x))
    // turn off the "structure was padded due to __declspec(align())" warning
    #pragma warning(disable: 4324)
#else
    #define DECLALIGN(x)
#endif


#define GET_BYTE_PTR(video) \
  video##_curr_ptr + ((video##_bit_offset + 7) >> 3)

#define GET_END_PTR(video) \
  video##_end_ptr

#define GET_START_PTR(video) \
  video##_start_ptr

#define GET_OFFSET(video) \
  (GET_BYTE_PTR(video) - video##_start_ptr)

#define GET_BIT_OFFSET(video) \
  ((video##_curr_ptr - video##_start_ptr)*8 + video##_bit_offset)

#define SET_PTR(video, PTR) {           \
  video##_curr_ptr = (Ipp8u*)(PTR);      \
  video##_bit_offset = 0;                \
}

#define GET_REMAINED_BYTES(video) \
  (GET_END_PTR(video) - GET_BYTE_PTR(video))

#define INIT_BITSTREAM(video, start_ptr, end_ptr) { \
  video##_start_ptr = (Ipp8u*)(start_ptr); \
  video##_end_ptr = (Ipp8u*)(end_ptr); \
  SET_PTR(video, video##_start_ptr); \
}

#define SHOW_BITS_32(video, CODE) { \
  Ipp32u _code = \
  (video##_curr_ptr[0] << 24) | \
  (video##_curr_ptr[1] << 16) | \
  (video##_curr_ptr[2] << 8) | \
  (video##_curr_ptr[3]); \
  CODE = (_code << video##_bit_offset) | (video##_curr_ptr[4] >> (8 - video##_bit_offset)); \
}

#define SHOW_BITS_LONG(video, NUM_BITS, CODE) { \
  Ipp32u _code2; \
  SHOW_BITS_32(video, _code2); \
  CODE = _code2 >> (32 - NUM_BITS); \
}

#define SHOW_HI9BITS(video, CODE) \
  CODE = (((video##_curr_ptr[0] << 24) | (video##_curr_ptr[1] << 16) | (video##_curr_ptr[2] << 8)) << video##_bit_offset);

#define EXPAND_17BITS(video, CODE) \
  CODE |= video##_curr_ptr[2] << (8 + video##_bit_offset);

#define SHOW_HI17BITS(video, CODE) \
  CODE = (((video##_curr_ptr[0] << 24) | (video##_curr_ptr[1] << 16) | (video##_curr_ptr[3]<<8)) << video##_bit_offset);

#define EXPAND_25BITS(video, CODE) \
  CODE |= video##_curr_ptr[3] << video##_bit_offset;

#define SHOW_BITS_SHORT(video, NUM_BITS, CODE) { \
  Ipp32u _code; \
  SHOW_HI9BITS(video, _code); \
  CODE = _code >> (32 - NUM_BITS); \
}

#define SHOW_BITS(video, NUM_BITS, CODE) { \
  Ipp32u _code = \
  (video##_curr_ptr[0] << 24) | \
  (video##_curr_ptr[1] << 16) | \
  (video##_curr_ptr[2] << 8); \
  CODE = (_code << video##_bit_offset) >> (32 - NUM_BITS); \
}

#define SHOW_1BIT(video, CODE) { \
  CODE = (video##_curr_ptr[0] >> (7-video##_bit_offset)) & 1; \
}

#define IS_NEXTBIT1(video) \
  (video##_curr_ptr[0] & (0x80 >> video##_bit_offset))

#define SHOW_TO9BITS(video, NUM_BITS, CODE) { \
  Ipp32u _code = \
  (video##_curr_ptr[0] << 8) | (video##_curr_ptr[1] ); \
  CODE = (_code >> (16 - NUM_BITS - video##_bit_offset)) & ((1<<NUM_BITS)-1); \
}

/*
#define SHOW_TO9BITS(video, NUM_BITS, CODE) { \
  Ipp32u _code = \
  (video##_curr_ptr[0] << 24) | (video##_curr_ptr[1]<<16); \
  CODE = (_code << video##_bit_offset) >> (32 - NUM_BITS); \
}
*/

#define SKIP_TO_END(video) \
  video##_curr_ptr = video##_end_ptr; \
  video##_bit_offset = 0;


#define SKIP_BITS(video, NUM_BITS) \
  video##_bit_offset += NUM_BITS; \
  video##_curr_ptr += (video##_bit_offset >> 3); \
  video##_bit_offset &= 7;

#define GET_BITS(video, NUM_BITS, CODE) { \
  SHOW_BITS(video, NUM_BITS, CODE); \
  SKIP_BITS(video, NUM_BITS); \
}

#define GET_TO9BITS(video, NUM_BITS, CODE) { \
  SHOW_TO9BITS(video, NUM_BITS, CODE); \
  SKIP_BITS(video, NUM_BITS); \
}

#define GET_BITS_LONG(video, NUM_BITS, CODE) { \
  SHOW_BITS_LONG(video, NUM_BITS, CODE); \
  SKIP_BITS(video, NUM_BITS); \
}

#define SKIP_BITS_LONG(video, NUM_BITS) \
  SKIP_BITS(video, NUM_BITS)

#define SKIP_BITS_32(video) \
  SKIP_BITS(video, 32);

#define GET_BITS32(video, CODE) { \
  SHOW_BITS_32(video, CODE) \
  SKIP_BITS_32(video); \
}

#define GET_1BIT(video, CODE) {\
  SHOW_1BIT(video, CODE) \
  SKIP_BITS(video, 1) \
}

#define UNGET_BITS_32(video) \
  video##_curr_ptr -= 4;

#define COPY_BITSTREAM(DST_BS, SRC_BS) \
  DST_BS##_curr_ptr = SRC_BS##_curr_ptr; \
  DST_BS##_bit_offset = SRC_BS##_bit_offset;

/***************************************************************/

#define FIND_START_CODE(video, code) {                          \
  Ipp8u *ptr = GET_BYTE_PTR(video);                             \
  Ipp8u *end_ptr = GET_END_PTR(video) - 3;                      \
  while (ptr < end_ptr && (ptr[0] || ptr[1] || (1 != ptr[2]))) {\
    ptr++;                                                      \
  }                                                             \
  if (ptr < end_ptr) {                                          \
    code = 256 + ptr[3];                                        \
    SET_PTR(video, ptr);                                        \
  } else {                                                      \
    code = (Ipp32u)UMC_ERR_NOT_ENOUGH_DATA;                         \
  }                                                             \
  /*printf("code %x at %5d (%5d) %p\n",code,idx,end,video##_bitstream_current_data);*/ \
}

#define GET_START_CODE(video, code) { \
  FIND_START_CODE(video, code)        \
  if(code != (Ipp32u)UMC_ERR_NOT_ENOUGH_DATA) { \
    SKIP_BITS_32(video);                    \
  }                                         \
}

/***************************************************************/

#define VLC_BAD        0x80
#define VLC_NEXTTABLE  0x40

#ifdef USE_VLC_THRESHOLD

#define DECODE_VLC(value, bitstream, pVLC) \
{ \
  Ipp32s max_bits = pVLC.max_bits; \
  Ipp32u __code; \
  Ipp32s tbl_value; \
  \
  SHOW_BITS_32(bitstream, __code); \
  \
  __code >>= (32 - max_bits); \
  \
  if (__code >= pVLC.threshold_table0 || isOneTable) { \
    Ipp32s bits_table0 = pVLC.bits_table0; \
    tbl_value = pVLC.table0[__code >> (max_bits - bits_table0)]; \
  } else { \
    Ipp32s bits_table1 = pVLC.bits_table1; \
    tbl_value = pVLC.table1[__code & ((1 << bits_table1) - 1)]; \
  } \
  \
  value = tbl_value >> 8; \
  \
  if (tbl_value & (VLC_BAD | VLC_NEXTTABLE)) return UMC_ERR_INVALID_STREAM; \
  \
  tbl_value &= 0x3F; \
  ippiSkipNBits(bitstream->bs_curr_ptr, bitstream->bs_bit_offset, tbl_value); \
}

#else

#define DECODE_VLC(value, bitstream, pVLC) \
{ \
  Ipp32u __code; \
  Ipp32s tbl_value; \
  Ipp32s bits_table0 = pVLC.bits_table0; \
  \
  SHOW_HI9BITS(bitstream, __code); \
  \
  tbl_value = pVLC.table0[__code >> (32 - bits_table0)]; \
  \
  if (tbl_value & (VLC_BAD | VLC_NEXTTABLE)) { \
    Ipp32s max_bits = pVLC.max_bits; \
    Ipp32s bits_table1 = pVLC.bits_table1; \
  \
    if (tbl_value & VLC_BAD) return UMC_ERR_INVALID_STREAM; \
    EXPAND_17BITS(bitstream, __code); \
    tbl_value = pVLC.table1[(__code >> (32 - max_bits)) & ((1 << bits_table1) - 1)]; \
  } \
  \
  value = tbl_value >> 8; \
  \
  tbl_value &= 0x3F; \
  SKIP_BITS(bitstream, tbl_value); \
}

#endif

#define DECODE_VLC_TEST1BIT(value, bitstream, pVLC) \
  if (bitstream->bitstream_INP(current_data[0]) & (1 << bitstream->bs_bit_offset)) { \
    value = 0; \
    ippiSkipNBits(bitstream->bs_curr_ptr, bitstream->bs_bit_offset, 1); \
  } else { \
    DECODE_VLC(value, bitstream, pVLC); \
  }

#define DECODE_VLC_OLD(value, BITSTREAM, pVLC)          \
{                                                       \
  Ipp32s __code;                                        \
  Ipp32s __code_len;                                    \
  Ipp32s __tbl_bits;                                    \
  Ipp32s __tbl_value;                                   \
                                                        \
  __tbl_bits = *pVLC;                                   \
  SHOW_BITS(BITSTREAM, __tbl_bits, __code)              \
  __tbl_value = pVLC[__code + 1];                       \
  __code_len = __tbl_value & 0xff;                      \
  __tbl_value >>= 8;                                    \
                                                        \
  if (__code_len & 0x80)                                \
  {                                                     \
      SKIP_BITS(BITSTREAM, __tbl_bits)                  \
      __tbl_bits = pVLC[__tbl_value];                   \
      SHOW_BITS(BITSTREAM, __tbl_bits, __code)          \
      __tbl_value = pVLC[__code + __tbl_value + 1];     \
      __code_len = __tbl_value & 0xff;                  \
      __tbl_value >>= 8;                                \
  }                                                     \
                                                        \
  value = __tbl_value;                                  \
  SKIP_BITS(BITSTREAM, __tbl_bits - __code_len)         \
}

/***************************************************************/

#ifndef __MPEG2_FUNC__
#define __MPEG2_FUNC__

#define MP2_API( type,name,arg )        extern type name arg;

#define IPPVC_ZIGZAG_SCAN           0
#define IPPVC_ALT_SCAN              1
#define IPPVC_MPEG1_STREAM          2
#define IPPVC_LEAVE_QUANT_UNCHANGED 4
#define IPPVC_LEAVE_SCAN_UNCHANGED  8

#define SCAN_MATRIX_TYPE   const Ipp8u
#define QUANT_MATRIX_TYPE  Ipp8u

struct DecodeIntraSpec_MPEG2 {
  QUANT_MATRIX_TYPE _quantMatrix[64];
  Ipp16s pDstBlock[64];
  Ipp16s idct[64];
  QUANT_MATRIX_TYPE *quantMatrix;
  SCAN_MATRIX_TYPE  *scanMatrix;
  Ipp32s intraVLCFormat;
  Ipp32s intraShiftDC;
};

struct DecodeInterSpec_MPEG2 {
  QUANT_MATRIX_TYPE _quantMatrix[64];
  Ipp16s pDstBlock[64];
  Ipp16s idct[64];
  QUANT_MATRIX_TYPE *quantMatrix;
  SCAN_MATRIX_TYPE  *scanMatrix;
  Ipp32s idxLastNonZero;
  Ipp32s align;
};

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

MP2_API(IppStatus, ownDecodeInter8x8IDCTAdd_MPEG1_1u8u, (
    Ipp8u**                            ppBitStream,
    Ipp32s*                            pBitOffset,
    DecodeInterSpec_MPEG2*         pQuantSpec,
    Ipp32s                             QP,
    Ipp8u*                             pSrcDst,
    Ipp32s                             srcDstStep))

MP2_API(IppStatus, ownDecodeInter8x8IDCTAdd_MPEG2_1u8u, (
    Ipp8u**                            ppBitStream,
    Ipp32s*                            pBitOffset,
    DecodeInterSpec_MPEG2*         pQuantSpec,
    Ipp32s                             QP,
    Ipp8u*                             pSrcDst,
    Ipp32s                             srcDstStep))

/* ///////////////////////////////////////////////////////////////////////////
//  Name:
//    ownDecodeIntra8x8IDCT_MPEG2_1u8u
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

MP2_API(IppStatus, ownDecodeIntra8x8IDCT_MPEG1_1u8u, (
    Ipp8u**                            ppBitStream,
    Ipp32s*                            pBitOffset,
    DecodeIntraSpec_MPEG2*         pQuantSpec,
    Ipp32s                             QP,
    Ipp32s                             blockType,
    Ipp16s*                            pDCPred,
    Ipp8u*                             pDst,
    Ipp32s                             dstStep))

MP2_API(IppStatus, ownDecodeIntra8x8IDCT_MPEG2_1u8u, (
    Ipp8u**                            ppBitStream,
    Ipp32s*                            pBitOffset,
    DecodeIntraSpec_MPEG2*         pQuantSpec,
    Ipp32s                             QP,
    Ipp32s                             blockType,
    Ipp16s*                            pDCPred,
    Ipp8u*                             pDst,
    Ipp32s                             dstStep))

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

MP2_API(IppStatus, ownDecodeIntraInit_MPEG2, (
    const Ipp8u*                 pQuantMatrix,
    Ipp32s                       scan,
    Ipp32s                       intraVLCFormat,
    Ipp32s                       intraShiftDC,
    DecodeIntraSpec_MPEG2*   pSpec))

MP2_API(IppStatus, ownDecodeInterInit_MPEG2, (
    const Ipp8u*                 pQuantMatrix,
    Ipp32s                       flag,
    DecodeInterSpec_MPEG2*   pSpec))

/* ///////////////////////////////////////////////////////////////////////////
//  Name:
//    ownDecodeIntraGetSize_MPEG2
//    ownDecodeInterGetSize_MPEG2
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

MP2_API(IppStatus, ownDecodeIntraGetSize_MPEG2, (
    Ipp32s* pSpecSize))

MP2_API(IppStatus, ownDecodeInterGetSize_MPEG2, (
    Ipp32s* pSpecSize))

#endif /* __MPEG2_FUNC__ */

/***************************************************************/

extern const Ipp16u MPEG2_VLC_TAB1[];
extern const Ipp8u  MPEG2_DCSIZE_TAB[];

#define UHBITS(code, nbits) (((Ipp32u)(code)) >> (32 - (nbits)))
#define SHBITS(code, nbits) (((Ipp32s)(code)) >> (32 - (nbits)))

#define TAB1_OFFSET_10BIT 248
#define TAB1_OFFSET_15BIT 360
#define TAB1_OFFSET_16BIT 408
#define TAB1_OFFSET_8BIT_INTRA 432
#define TAB1_OFFSET_10BIT_INTRA 680

#define UNPACK_VLC1(tab_val, run, val, len) \
{ \
  Ipp32u _tab_val = tab_val; \
  run = _tab_val & 0x1f; \
  len = (_tab_val >> 5) & 0xf; \
  val = (_tab_val >> 9); \
}

#define UNPACK_VLC2(tab_val, run, val, len) \
{ \
  Ipp32u _tab_val = tab_val; \
  run = _tab_val & 0x1f; \
  len = (_tab_val >> 5) & 0xf; \
  val = (_tab_val >> 10); \
}

#define DECODE_DC(val)                                    \
{                                                         \
  const Ipp8u *pTab;                                      \
  Ipp32s dct_dc_size;                                     \
  SHOW_HI9BITS(BS, code);                                 \
  pTab = MPEG2_DCSIZE_TAB + (chromaFlag << 6);            \
  if (code < 0xf8000000) {                                \
    tbl = pTab[UHBITS(code, 5)];                          \
    dct_dc_size = tbl & 0xF;                              \
    len = tbl >> 4;                                       \
    if (dct_dc_size) {                                    \
      EXPAND_17BITS(BS, code);                            \
      code <<= len;                                       \
      val = UHBITS(code, dct_dc_size) - UHBITS(SHBITS(~code, 1), dct_dc_size); \
      SKIP_BITS(BS, len + dct_dc_size);                   \
    } else {                                              \
      val = 0;                                            \
      SKIP_BITS(BS, len);                                 \
    }                                                     \
  } else {                                                \
    tbl = pTab[32 + (UHBITS(code, 10) & 0x1f)];           \
    dct_dc_size = tbl & 0xF;                              \
    len = tbl >> 4;                                       \
    EXPAND_17BITS(BS, code);                              \
    EXPAND_25BITS(BS, code);                              \
    code <<= len;                                         \
    val = UHBITS(code, dct_dc_size) - UHBITS(SHBITS(~code, 1), dct_dc_size); \
    SKIP_BITS(BS, len + dct_dc_size);                     \
  }                                                       \
}

/***************************************************************/

#if (defined(USE_INTRINSICS_W7) || defined(USE_INTRINSICS_A6) || defined(USE_INTRINSICS_PM))&&(!defined(_WIN32_WCE))
#define USE_INTRINSICS
#endif

#define M_EMPTY() _m_empty()

#define __ALIGN8  __declspec (align(8))
#define __ALIGN16 __declspec (align(16))
#define __RESTRICT
#define _mm_loadu      _mm_loadu_si128
#define IPP_INT_PTR(x) ((Ipp32s)(x))

/***************************************************************/

#endif /* __UMC_MPEG2_DEC_BSTREAM_H */
