/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __BSTREAM_H
#define __BSTREAM_H

#include "vm_types.h"

#include "ippdefs.h"

typedef struct
{
  Ipp32u* pBuffer;                  //
  Ipp32s  nBufferLen;               //
  Ipp32s  nDataLen;                 //
  Ipp32s  init_nBit_offset;         //
  Ipp32s  nBit_offset;              // 32->1
  Ipp32u  dword;                    //
  Ipp32u* pCurrent_dword;           //

  Ipp32u* saved_pBuffer;            //
  Ipp32s  saved_nBufferLen;         //
  Ipp32s  saved_nDataLen;           //
  Ipp32s  saved_init_nBit_offset;   //
  Ipp32s  saved_nBit_offset;        // 32->1
  Ipp32u  saved_dword;              //
  Ipp32u* saved_pCurrent_dword;     //

} sBitsreamBuffer;

/*******************************************************************/

#ifdef _BIG_ENDIAN_
#define BSWAP(x) (x)
#else
# ifdef __INTEL_COMPILER
#define BSWAP(x) _bswap(x)
#else
#define BSWAP(x) (Ipp32u)(((x) << 24) | (((x)&0xff00) << 8) | (((x) >> 8)&0xff00) | ((x&0xff000000) >> 24));
#endif
#endif

/*******************************************************************/

#define LOAD_DWORD(pBS)                                             \
  (pBS)->dword = BSWAP((pBS)->pCurrent_dword[0]);                   \
  (pBS)->dword &= ~bstream_mask_table[(pBS)->nBit_offset];

#define GET_LOAD_DWORD(pBS)                                         \
  (pBS)->dword = BSWAP((pBS)->pCurrent_dword[0]);                   \
  (pBS)->dword &= bstream_mask_table[(pBS)->nBit_offset];


/*******************************************************************/

#define _OFFSET_PTR(p, n) (((Ipp8u*)(p) - (Ipp8u*)(0)) & ((n)-1))
#define _ALIGN_PTR(p, n) ((Ipp8u*)(p) - _OFFSET_PTR(p, n))

#define INIT_BITSTREAM(pBS, ptr)                                    \
{                                                                   \
  Ipp8u *tmp_ptr = (Ipp8u*)ptr;                                     \
  (pBS)->pBuffer = (Ipp32u *)_ALIGN_PTR(tmp_ptr, 4);                \
  (pBS)->nBit_offset = 32 - ((Ipp32s)_OFFSET_PTR(tmp_ptr, 4) << 3);         \
  (pBS)->init_nBit_offset = (pBS)->nBit_offset;                     \
  (pBS)->pCurrent_dword = (pBS)->pBuffer;                           \
  LOAD_DWORD(pBS)                                                   \
}

#define INIT_BITSTREAM_OFFSET(pBS, ptr, offset)                     \
{                                                                   \
  (pBS)->pBuffer = ptr;                                             \
  (pBS)->nBit_offset = offset;                                      \
  (pBS)->init_nBit_offset = (pBS)->nBit_offset;                     \
  (pBS)->pCurrent_dword = (pBS)->pBuffer;                           \
  LOAD_DWORD(pBS)                                                   \
}

#define GET_INIT_BITSTREAM(pBS, ptr)                                \
{                                                                   \
  Ipp8u *tmp_ptr = (Ipp8u*)ptr;                                     \
  (pBS)->pBuffer = (Ipp32u *)_ALIGN_PTR(tmp_ptr, 4);                \
  (pBS)->nBit_offset = 32 - ((Ipp32s)_OFFSET_PTR(tmp_ptr, 4) << 3);         \
  (pBS)->init_nBit_offset = (pBS)->nBit_offset;                     \
  (pBS)->pCurrent_dword = (pBS)->pBuffer;                           \
  GET_LOAD_DWORD(pBS)                                               \
}

/*******************************************************************/

#define SAVE_BITSTREAM(pBS)                                         \
{                                                                   \
  Ipp32s _nbits = (pBS)->nBit_offset;                               \
  Ipp32u _cw0 = (pBS)->dword;                                       \
  Ipp32u _cw1;                                                      \
  if (_nbits != 32) {                                               \
    _cw1 = (pBS)->pCurrent_dword[0];                                \
    _cw1 = BSWAP(_cw1);                                             \
    _cw1 &= bstream_mask_table[_nbits];                             \
    _cw0 = _cw0 | _cw1;                                             \
    _cw0 = BSWAP(_cw0);                                             \
    (pBS)->pCurrent_dword[0] = _cw0;                                \
  }                                                                 \
}

/*******************************************************************/

#define PUT_BITS(pBS, cw, n)                                        \
{                                                                   \
  Ipp32s _nbits = (pBS)->nBit_offset;                               \
  Ipp32u _cw0 = (pBS)->dword;                                       \
  Ipp32u _cw;                                                       \
  _cw = (cw) & bstream_mask_table[(n)];                             \
  if (_nbits <= (n)) {                                              \
    _cw0 = _cw0 | (_cw >> ((n) - _nbits));                          \
    _cw0 = BSWAP(_cw0);                                             \
    (pBS)->pCurrent_dword[0] = _cw0;                                \
    (pBS)->pCurrent_dword++;                                        \
    _nbits = 32 - ((n) - _nbits);                                   \
    if (_nbits != 32) {                                             \
      _cw0 = _cw << _nbits;                                         \
    } else {                                                        \
      _cw0 = 0;                                                     \
    }                                                               \
  } else {                                                          \
    _nbits -= (n);                                                  \
    _cw0 = _cw0 | (_cw << _nbits);                                  \
  }                                                                 \
  (pBS)->nBit_offset = _nbits;                                      \
  (pBS)->dword = _cw0;                                              \
}

/*******************************************************************/

#define GET_BITS(pBS, res_value, nbits, type)                       \
{                                                                   \
  Ipp32s  tmp_bit_number;                                           \
  Ipp32u value;                                                     \
  Ipp32u current_dword;                                             \
  Ipp32u blen = (nbits);                                            \
                                                                    \
  tmp_bit_number = (pBS)->nBit_offset - blen;                       \
                                                                    \
  current_dword = (pBS)->dword;                                     \
  if (tmp_bit_number > 0) {                                         \
    value = current_dword << (32 - (pBS)->nBit_offset);             \
    value >>= (32 - blen);                                          \
    (pBS)->nBit_offset = tmp_bit_number;                            \
  } else if (tmp_bit_number == 0) {                                 \
    value = current_dword << (32 - (pBS)->nBit_offset);             \
    value >>= (32 - blen);                                          \
    (pBS)->pCurrent_dword++;                                        \
    current_dword = BSWAP((pBS)->pCurrent_dword[0]);                \
    (pBS)->dword = current_dword;                                   \
    (pBS)->nBit_offset = 32;                                        \
  } else {                                                          \
    tmp_bit_number = blen - (pBS)->nBit_offset;                     \
    value = (current_dword << (32 - (pBS)->nBit_offset));           \
    value >>= (32 - (pBS)->nBit_offset);                            \
    (pBS)->pCurrent_dword++;                                        \
    current_dword = BSWAP((pBS)->pCurrent_dword[0]);                \
    (pBS)->dword = current_dword;                                   \
    value <<= tmp_bit_number;                                       \
    value += (current_dword >> (32 - tmp_bit_number));              \
    (pBS)->nBit_offset = (32 - tmp_bit_number);                     \
  }                                                                 \
  (res_value) = (type)value;                                        \
}

/*******************************************************************/

#define GET_BITS_COUNT(pBS, size)                                   \
  size =(Ipp32s)((((pBS)->pCurrent_dword) - ((pBS)->pBuffer)) * 32 +         \
          (pBS)->init_nBit_offset - ((pBS)->nBit_offset));

/*******************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

extern Ipp32u bstream_mask_table[33];

Ipp32u GetNumProcessedByte(sBitsreamBuffer* pBS);
void   Byte_alignment(sBitsreamBuffer* pBS);
Ipp32u Getbits(sBitsreamBuffer* pBS, Ipp32u len);
void   Putbits(sBitsreamBuffer* pBS, Ipp32u value, Ipp32s len);
void   bs_save(sBitsreamBuffer* pBS);
void   bs_restore(sBitsreamBuffer* pBS);
void bs_copy(sBitsreamBuffer* src, sBitsreamBuffer* dst);

void bs_CRC_reset(Ipp32u *crc);
void bs_CRC_update(Ipp32u *ptr, Ipp32s offset, Ipp32s len, Ipp32u *crc);
void bs_CRC_update_bs(sBitsreamBuffer *bs, Ipp32s len, Ipp32u *crc);
void bs_CRC_update_ptr(Ipp8u *ptr, Ipp32s len, Ipp32u *crc);
void bs_CRC_update_imm(Ipp32u val, Ipp32s len, Ipp32u *crc);
void bs_CRC_update_zero(Ipp32s len, Ipp32u *crc);

/***********************************************************************

                Alternative bitstream function(s)

***********************************************************************/

Ipp32u get_bits( Ipp8u **pp_bitstream, Ipp32s *p_offset, Ipp32s num_bits);
void byte_alignment(Ipp8u **pp_bitstream, Ipp32s *p_offset);

Ipp32s SwapBuffer(Ipp8u *pBuffer, Ipp32s len_buffer);

#ifdef  __cplusplus
}
#endif

#endif//__BSTREAM_H
