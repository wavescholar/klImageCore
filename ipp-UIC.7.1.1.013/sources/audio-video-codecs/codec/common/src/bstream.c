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

#ifdef UMC_BSTREAM
#undef UMC_BSTREAM
#endif

#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)
  #define UMC_BSTREAM
#elif defined UMC_ENABLE_MP3_AUDIO_DECODER
  #define UMC_BSTREAM
#elif defined UMC_ENABLE_AAC_AUDIO_ENCODER
  #define UMC_BSTREAM
#elif defined UMC_ENABLE_MP3_AUDIO_ENCODER
  #define UMC_BSTREAM
#elif defined UMC_ENABLE_MP4_SPLITTER
  #define UMC_BSTREAM
#endif

#ifdef UMC_BSTREAM

#include "bstream.h"

Ipp32u bstream_mask_table[33] = {
  0x0,
  0x01,         0x03,       0x07,       0x0F,
  0x01F,        0x03F,      0x07F,      0x0FF,
  0x01FF,       0x03FF,     0x07FF,     0x0FFF,
  0x01FFF,      0x03FFF,    0x07FFF,    0x0FFFF,
  0x01FFFF,     0x03FFFF,   0x07FFFF,   0x0FFFFF,
  0x01FFFFF,    0x03FFFFF,  0x07FFFFF,  0x0FFFFFF,
  0x01FFFFFF,   0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF,
  0x1FFFFFFF,   0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF,
};

/*******************************************************************/

Ipp32u GetNumProcessedByte(sBitsreamBuffer* pBS)
{
  Ipp32u ret;

  ret = (Ipp32u)((pBS->pCurrent_dword - pBS->pBuffer) * sizeof(*pBS->pBuffer));

  ret += (32 - pBS->nBit_offset) / 8;
  return ret;
}

void Byte_alignment(sBitsreamBuffer* pBS)
{
  Ipp32s i;

  i = pBS->nBit_offset % 8;
  pBS->nBit_offset -= i;

  if (pBS->nBit_offset == 0) {
    pBS->nBit_offset = 32;
    pBS->pCurrent_dword++;
    pBS->dword = BSWAP(pBS->pCurrent_dword[0]);
  }
}

Ipp32u Getbits(sBitsreamBuffer* pBS,
               Ipp32u len)
{
  Ipp32s tmp_bit_number;
  Ipp32u value;
  Ipp32u current_dword;

  tmp_bit_number = pBS->nBit_offset - len;

  current_dword = BSWAP(pBS->pCurrent_dword[0]);
  if (tmp_bit_number > 0) {
    value = current_dword << (32 - pBS->nBit_offset);
    value >>= (32 - len);
    pBS->nBit_offset = tmp_bit_number;
  } else if (tmp_bit_number == 0) {
    value = current_dword << (32 - pBS->nBit_offset);
    value >>= (32 - len);
    pBS->pCurrent_dword++;
    pBS->nBit_offset = 32;
  } else {
    tmp_bit_number = len - pBS->nBit_offset;
    value = (current_dword << (32 - pBS->nBit_offset));
    value >>= (32 - pBS->nBit_offset);
    pBS->pCurrent_dword++;
    current_dword = BSWAP(pBS->pCurrent_dword[0]);
    value <<= tmp_bit_number;
    value += (current_dword >> (32 - tmp_bit_number));
    pBS->nBit_offset = (32 - tmp_bit_number);
  }
  return value;
}

void Putbits(sBitsreamBuffer *pBS,
             Ipp32u value,
             Ipp32s len)
{
  Ipp32s tmp_bit_number;
  Ipp32u tmp_data;

  tmp_bit_number = pBS->nBit_offset - len;

  if (tmp_bit_number > 0) {
    tmp_data = (pBS->pCurrent_dword[0] >> pBS->nBit_offset);
    tmp_data <<= len;
    tmp_data += value;
    tmp_data <<= tmp_bit_number;
    pBS->pCurrent_dword[0] = tmp_data;
    pBS->nBit_offset = tmp_bit_number;
  } else if (tmp_bit_number == 0) {
    tmp_data = (pBS->pCurrent_dword[0] >> pBS->nBit_offset);
    tmp_data <<= len;
    tmp_data += value;
    pBS->pCurrent_dword[0] = tmp_data;
    pBS->pCurrent_dword++;
    pBS->nBit_offset = 32;
  } else {
    tmp_bit_number = len - pBS->nBit_offset;
    tmp_data = (pBS->pCurrent_dword[0] >> pBS->nBit_offset);
    tmp_data <<= pBS->nBit_offset;
    tmp_data += (value >> tmp_bit_number);
    pBS->pCurrent_dword[0] = tmp_data;
    pBS->pCurrent_dword++;
    pBS->nBit_offset = 32 - tmp_bit_number;
    tmp_data = (value << pBS->nBit_offset);
    pBS->pCurrent_dword[0] = tmp_data;
  }
}

void bs_save(sBitsreamBuffer *pBS)
{
  pBS->saved_nBit_offset = pBS->nBit_offset;
  pBS->saved_init_nBit_offset = pBS->init_nBit_offset;
  pBS->saved_nBufferLen = pBS->nBufferLen;
  pBS->saved_nDataLen = pBS->nDataLen;
  pBS->saved_pBuffer = pBS->pBuffer;
  pBS->saved_pCurrent_dword = pBS->pCurrent_dword;
  pBS->saved_dword = pBS->dword;
}

void bs_restore(sBitsreamBuffer* pBS)
{
  pBS->nBit_offset = pBS->saved_nBit_offset;
  pBS->init_nBit_offset = pBS->saved_init_nBit_offset;
  pBS->nBufferLen = pBS->saved_nBufferLen;
  pBS->nDataLen = pBS->saved_nDataLen;
  pBS->pBuffer = pBS->saved_pBuffer;
  pBS->pCurrent_dword = pBS->saved_pCurrent_dword;
  pBS->dword = pBS->saved_dword;
}

void bs_copy(sBitsreamBuffer* src,
             sBitsreamBuffer* dst)
{
  dst->nBit_offset = src->nBit_offset;
  dst->init_nBit_offset = src->init_nBit_offset;
  dst->nBufferLen = src->nBufferLen;
  dst->nDataLen = src->nDataLen;
  dst->pBuffer = src->pBuffer;
  dst->pCurrent_dword = src->pCurrent_dword;
  dst->dword = src->dword;
}

/***********************************************************************

                Alternative bitstream function(s)

***********************************************************************/
/*
#define BITS_IN_BYTE 8
#define BITS_IN_INT 32

Ipp32u
get_bits( Ipp8u **pp_bitstream, Ipp32s *p_offset, Ipp32s num_bits)
{
    Ipp8u *p_bitstream = *pp_bitstream;
    Ipp32u tmp;
    Ipp32u factor = *p_offset + num_bits;
    Ipp32u mask = ((Ipp32u)( ~0 )) >> *p_offset;

    tmp = ( (Ipp32u)( p_bitstream[0] << 3 * BITS_IN_BYTE ) |
            (Ipp32u)( p_bitstream[1] << 2 * BITS_IN_BYTE ) |
            (Ipp32u)( p_bitstream[2] << 1 * BITS_IN_BYTE ) |
            (Ipp32u)( p_bitstream[3] )) & mask;
    tmp = tmp >> ( BITS_IN_INT - factor );
    *pp_bitstream += ( factor >> 3 );
    *p_offset = factor & 0x07;
    return tmp;
}
*/

Ipp32u get_bits(Ipp8u **pp_bs,
                Ipp32s *p_offset,
                Ipp32s n)
{
  Ipp32u data;
  Ipp32u tmp;

  data = 0;
  data += pp_bs[0][0];
  data <<= 8;
  data += pp_bs[0][1];
  data <<= 8;
  data += pp_bs[0][2];
  data <<= 8;
  data += pp_bs[0][3];

  tmp = (Ipp32u)pp_bs[0][4];
  data <<= p_offset[0];
  tmp >>= (8 - p_offset[0]);
  data += tmp;

  p_offset[0] += n;
  pp_bs[0] += (p_offset[0] / 8);
  p_offset[0] %= 8;

  data >>= (32 - n);
  return data;
}

void byte_alignment(Ipp8u **pp_bitstream,
                    Ipp32s *p_offset)
{
  if (*p_offset & 0x7) {
    (*pp_bitstream)++;
    *p_offset = 0;
  }
}

Ipp32s SwapBuffer(Ipp8u *pBuffer,
                  Ipp32s len_buffer)
{
  Ipp32s i;
  Ipp32s len;
  Ipp32s temp;
  Ipp32s *pSwpBuffer = (Ipp32s *)pBuffer;

  len = len_buffer >> 2;
  for (i = 0; i < len; i++) {
    temp = pSwpBuffer[i];
    pSwpBuffer[i] = BSWAP(temp);
  }

  return (len_buffer % (sizeof(Ipp32u)));
}

static Ipp16u bs_CRC_table[256] = {
  0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
  0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
  0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
  0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
  0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
  0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
  0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
  0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
  0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
  0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
  0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
  0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
  0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
  0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
  0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
  0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
  0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
  0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
  0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
  0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
  0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
  0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
  0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
  0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
  0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
  0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
  0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
  0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
  0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
  0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
  0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
  0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};

#define CRC_UPDATE8(d, c)\
  c = bs_CRC_table[(((c)>>8) ^ (d)) & 0xff] ^ ((c) << 8);

#define CRC_UPDATE32(_data, crc)         \
{                                        \
  Ipp32u d = _data;                      \
  Ipp32u c = *crc;                       \
  Ipp32u d0 = (d >> 24);                 \
  Ipp32u d1 = (d >> 16) & 0xff;          \
  Ipp32u d2 = (d >> 8) & 0xff;           \
  Ipp32u d3 = d & 0xff;                  \
  CRC_UPDATE8(d0, c)                     \
  CRC_UPDATE8(d1, c)                     \
  CRC_UPDATE8(d2, c)                     \
  CRC_UPDATE8(d3, c)                     \
  *crc = c & 0xffff;                     \
}

#define CRC_UPDATE(_data, _length, crc)  \
{                                        \
  Ipp32u d = _data;                      \
  Ipp32u length = _length;               \
  Ipp32u c = *crc << 16;                 \
                                         \
  d <<= 32 - length;                     \
                                         \
  while(length--) {                      \
    c = (c << 1) ^                       \
    (((Ipp32s)(d ^ c) >> 31)             \
    & 0x80050000);                       \
    d <<= 1;                             \
  }                                      \
  *crc = c >> 16;                        \
}

#define CRC_UPDATE_ZERO(_length, crc)    \
{                                        \
  Ipp32s i, length = _length;            \
  Ipp32u c = *crc;                       \
                                         \
  for(i = 0; i < length - 7; i += 8)     \
    c = bs_CRC_table[(c>>8) & 0xff] ^    \
      (c << 8);                          \
  c <<= 16;                              \
                                         \
  while(i < length) {                    \
    i++;                                 \
    c = (c << 1) ^                       \
    (((Ipp32s)(c) >> 31)                 \
    & 0x80050000);                       \
  }                                      \
  *crc = c >> 16;                        \
}

void bs_CRC_reset(Ipp32u *crc) {
  *crc = 0xffff;
}

void bs_CRC_update(Ipp32u *ptr, Ipp32s offset, Ipp32s len, Ipp32u *crc) {
  Ipp32s i;
  Ipp32u data;
  Ipp32s nbits;

  if (offset == 0) {
    ptr++;
  } else if (offset != 32) {
    nbits = offset;
    if (nbits > len)
      nbits = len;
    data = *ptr++;
    data = BSWAP(data);
    data = ((data << (32 - offset)) >> (32 - nbits));
    data &= ((1 << nbits) - 1);
    CRC_UPDATE(data, nbits, crc)
      len -= nbits;
  }

  for (i = 0; i < len - 31; i += 32) {
    data = *ptr++;
    data = BSWAP(data);
    CRC_UPDATE32(data, crc)
  }
  if (i < len) {
    data = *ptr++;
    data = BSWAP(data);
    nbits = len - i;
    data = data >> (32 - nbits);
    data &= ((1 << nbits) - 1);
    CRC_UPDATE(data, nbits, crc)
  }
}

void bs_CRC_update_bs(sBitsreamBuffer *bs, Ipp32s len, Ipp32u *crc) {
  bs_CRC_update(bs->pCurrent_dword, bs->nBit_offset, len, crc);
}

void bs_CRC_update_ptr(Ipp8u *ptr, Ipp32s len, Ipp32u *crc) {
  Ipp32u *pCurrent_dword = (Ipp32u *)_ALIGN_PTR(ptr, 4);
  Ipp32s nBit_offset = 32 - ((Ipp32s)_OFFSET_PTR(ptr, 4) << 3);

  bs_CRC_update(pCurrent_dword, nBit_offset, len, crc);
}

void bs_CRC_update_imm(Ipp32u val, Ipp32s len, Ipp32u *crc) {
  CRC_UPDATE(val, len, crc)
}

void bs_CRC_update_zero(Ipp32s len, Ipp32u *crc) {
  CRC_UPDATE_ZERO(len, crc)
}

#endif //UMC_ENABLE_XXX
