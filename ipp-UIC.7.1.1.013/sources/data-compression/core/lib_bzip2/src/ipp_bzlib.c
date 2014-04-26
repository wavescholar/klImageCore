/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives Ipp_bzip2 Sample
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
*/

#include "ippcore.h"
#include "ippdc.h"
#include "ipps.h"
#include "bzlib_private.h"

#if defined(_OPENMP)
#include <omp.h>
#endif

#ifndef IPP_NOCPP
extern int link_to_ipp_libbz2;
void init_link_to_ipp_libbz2() {
  link_to_ipp_libbz2 = 1;
} /* init_link_to_ipp_libbz2 */
#endif

/* ==================================
             Encode block
   ================================== */

#define BZ_E_M_IDLE      -1
#define BZ_E_M_RUNNING   BZ_RUN
#define BZ_E_M_FLUSHING  BZ_FLUSH
#define BZ_E_M_FINISHING BZ_FINISH

#define BZ_E_D_OUTPUT    1
#define BZ_E_D_INPUT     2

#define PTR_EXCHANGE(PTR_SRC, PTR_DST) \
{ Ipp8u *ptr_tmp_src = PTR_SRC; PTR_SRC = PTR_DST; PTR_DST = ptr_tmp_src; }

#define FLUSH_CODE_BUF_TO_OUT \
  for( ; code_len >= 8; code_len -= 8, code <<= 8 ) out[out_len++] = (Ipp8u)(code >> 24)

#define WR_TO_OUT(VAL, LEN) \
{ \
  FLUSH_CODE_BUF_TO_OUT; \
  code     |= (VAL) << (32 - code_len - (LEN)); \
  code_len += (LEN); \
}

typedef struct {
  Ipp8u*       in;
  Ipp8u*       out;
  Ipp8u*       tmp_buf;
  Ipp8u*       shared_state;
  unsigned int block_crc;
  unsigned int in_len;
  unsigned int out_len;
  int          out_rem_bits;
} mt_e_state;

typedef struct {
  bz_stream*   strm;
  mt_e_state*  mt_states;
  Ipp8u*       int_next_in;
  Ipp8u*       int_org_in;
  Ipp8u*       int_next_out;
  unsigned int avail_in_expect;
  unsigned int int_avail_in;
  unsigned int int_avail_out;
  int          block_size;
  unsigned int combined_crc;
  unsigned int code;
  int          code_len;
  int          chunks;
  int          cur_chunk;
  int          direction; /* 2 conditions: INPUT, OUTPUT */
  int          mode;      /* 4 conditions: IDLE, RUNNING, FLUSHING, FINISHING */
} int_e_state;

static void* default_bzalloc( void* opaque, int items, int size )
{
  opaque = opaque;
  return ippsMalloc_8u( items * size );
} /* default_bzalloc */

static void default_bzfree( void* opaque, void* addr )
{
  opaque = opaque;
  ippsFree( addr );
} /* default_bzfree */

static int init_e_block_state( int_e_state* s )
{
  mt_e_state* mt_s = s->mt_states;
  int i;

  s->int_next_in   = s->int_org_in;
  mt_s[0].in       = s->int_org_in;
  s->int_avail_in  = s->block_size - 19;
  s->direction     = BZ_E_D_INPUT;
  s->mode          = BZ_E_M_RUNNING;
  s->cur_chunk     = 0;
  for( i = 0; i < s->chunks; i++ ) {
    mt_s[i].block_crc = 0;
    if( ippStsNoErr != ippsEncodeRLEInit_BZ2_8u( (IppRLEState_BZ2*)mt_s[i].shared_state ) ) return BZ_PARAM_ERROR;
  } /* for */
  return BZ_OK;
} /* init_e_block_state */

/*---------------------------------------------------*/
int BZ_API(BZ2_bzCompressInit) ( bz_stream* strm, int blockSize100k, int verbosity, int workFactor )
{
  if( NULL == strm ) return BZ_PARAM_ERROR;
  if( (blockSize100k < 1) || (blockSize100k > 9) ) return BZ_PARAM_ERROR;
  if( (1 != sizeof(char)) || (2 != sizeof(short)) || (4 != sizeof(int)) ) return BZ_CONFIG_ERROR;
  if( NULL == strm->bzalloc ) strm->bzalloc = default_bzalloc;
  if( NULL == strm->bzfree ) strm->bzfree = default_bzfree;
  verbosity  = verbosity;
  workFactor = workFactor;
  { /* block */
    int_e_state *s;
    mt_e_state*  mt_s;
    int          block_size = blockSize100k * 100000;
    int          i, tmp, max_state_size;

#if defined(IPP_IT_SORT)
    if( ippStsNoErr != ippsBWTFwdGetBufSize_SelectSort_8u( block_size, &max_state_size,
                                                           ippBWTItohTanakaUnlimSort ) ) return BZ_CONFIG_ERROR;
#elif defined (IPP_SFX_SORT)
    if( ippStsNoErr != ippsBWTFwdGetBufSize_SelectSort_8u( block_size, &max_state_size,
                                                           ippBWTSuffixSort ) ) return BZ_CONFIG_ERROR;
#else
    if( ippStsNoErr != ippsBWTFwdGetBufSize_SelectSort_8u( block_size, &max_state_size,
                                                           ippBWTAutoSort ) ) return BZ_CONFIG_ERROR;
#endif
    if( ippStsNoErr != ippsEncodeHuffGetSize_BZ2_16u8u( block_size, &tmp ) ) return BZ_CONFIG_ERROR;
    max_state_size = IPP_MAX(max_state_size, tmp);
    if( ippStsNoErr != ippsMTFGetSize_8u( &tmp ) ) return BZ_CONFIG_ERROR;
    max_state_size = IPP_MAX(max_state_size, tmp);
    if( ippStsNoErr != ippsRLEGetSize_BZ2_8u( &tmp ) ) return BZ_CONFIG_ERROR;
    max_state_size = IPP_MAX(max_state_size, tmp);

#if defined(_OPENMP)
    tmp = omp_get_max_threads();
#else
    tmp = 1;
#endif

    s = (int_e_state*)BZALLOC( sizeof(int_e_state) +
                               tmp * (max_state_size + sizeof(mt_e_state) +
                               block_size * (sizeof(Ipp8u) * 2 + sizeof(Ipp16u))) );
    if( NULL == s ) return BZ_MEM_ERROR;
    strm->state          = s;
    s->strm              = strm;
    s->chunks            = tmp;
    s->mt_states = mt_s  = (mt_e_state*)((Ipp8u*)s + sizeof(int_e_state));

    s->int_org_in        = (Ipp8u*)mt_s + sizeof(mt_e_state) * tmp;
    mt_s[0].out          = s->int_org_in + block_size * tmp;
    mt_s[0].tmp_buf      = mt_s[0].out + block_size;
    mt_s[0].shared_state = mt_s[0].out + (sizeof(Ipp16u) + sizeof(Ipp8u)) * block_size * tmp;
    for( i = 1; i < tmp; i++ ) {
      mt_s[i].out          = mt_s[i - 1].tmp_buf + block_size * sizeof(Ipp16u);
      mt_s[i].tmp_buf      = mt_s[i].out + block_size;
      mt_s[i].shared_state = mt_s[i - 1].shared_state + max_state_size;
    } /* for */
    strm->total_in_lo32  = strm->total_in_hi32  = 0;
    strm->total_out_lo32 = strm->total_out_hi32 = 0;

    s->block_size   = block_size;
    s->code         = 0x425a6830 + blockSize100k;
    s->code_len     = 32;
    s->combined_crc = 0;
    if( BZ_OK != init_e_block_state( s ) ) return BZ_CONFIG_ERROR;
  } /* block */
  return BZ_OK;
} /* BZ2_bzCompressInit */

static int encode_block( int_e_state* s, int cur_chunk )
{
  mt_e_state* mt_s    = s->mt_states + cur_chunk;
  Ipp8u* src          = mt_s->in;
  Ipp8u* dst          = mt_s->tmp_buf;
  Ipp8u* out          = mt_s->out;
  Ipp8u* shared_state = mt_s->shared_state;
  int    block_size   = s->block_size;
  int    src_len      = mt_s->in_len;
  int    dst_len      = block_size;
  unsigned int code   = 0;
  int    code_len     = 0;
  int    out_len      = 0;
  int    ret          = BZ_PARAM_ERROR;
  int    frq_tbl[258];
  Ipp8u  in_use[256];
  int    i, tmp, dict_size;

  do {
    if( 0 == cur_chunk ) {
      code     = s->code;
      code_len = s->code_len;
    } /* if */
    if( 0 == src_len ) { ret = BZ_OK; break; }
    if( ippStsNoErr > ippsRLEGetInUseTable_8u( in_use, (IppRLEState_BZ2*)shared_state ) ) break;
    if( ippStsNoErr != ippsReduceDictionary_8u_I( in_use, src, src_len, &dict_size ) ) break;
    WR_TO_OUT(0x314159, 24);
    WR_TO_OUT(0x265359, 24);
    { /* block */
      unsigned int block_crc = mt_s->block_crc;
      WR_TO_OUT(block_crc >> 16,    16);
      WR_TO_OUT(block_crc & 0xffff, 16);
    } /* block */
    { /* BWT */
      int idx;
      if( 1 == dict_size ) {
        if( ippStsNoErr > ippsCopy_8u( src, dst, src_len ) ) break;
        idx = 0;
      } /* if */
      else {
#if defined(IPP_IT_SORT)
        if( ippStsNoErr != ippsBWTFwd_SelectSort_8u( src, dst, src_len, &idx, (Ipp8u*)shared_state,
                                                     ippBWTItohTanakaUnlimSort ) ) break;
#elif defined (IPP_SFX_SORT)
        if( ippStsNoErr != ippsBWTFwd_SelectSort_8u( src, dst, src_len, &idx, (Ipp8u*)shared_state,
                                                     ippBWTSuffixSort ) ) break;
#else
        if( ippStsNoErr != ippsBWTFwd_SelectSort_8u( src, dst, src_len, &idx, (Ipp8u*)shared_state,
                                                     ippBWTAutoSort ) ) break;
#endif
      } /* else */
      PTR_EXCHANGE(src, dst);
      WR_TO_OUT(0, 1);
      WR_TO_OUT(idx, 24);
    } /* BWT */
    { /* block */
      unsigned short buf[16];
      unsigned short in_use_16;

      for( i = 0; i < 16; i++ ) buf[i] = 0;
      for( i = in_use_16 = 0; i < 256; i++ ) {
        tmp        = i >> 4;
        buf[tmp]   = (buf[tmp] << 1) | in_use[i];
        in_use_16 |= in_use[i] << (15 - tmp);
      } /* for */
      WR_TO_OUT(in_use_16, 16);
      for( i = 0; i < 16; i++ ) {
        if( buf[i] ) WR_TO_OUT(buf[i], 16);
      } /* for */
    } /* block */
    /* MTF */
    if( ippStsNoErr != ippsMTFInit_8u( (IppMTFState_8u*)shared_state ) ) break;
    if( ippStsNoErr != ippsMTFFwd_8u( src, dst, src_len, (IppMTFState_8u*)shared_state ) ) break;
    PTR_EXCHANGE(src, dst);
    { /* The 1-2 coding */
      Ipp8u* tmp_src = src;
      dst_len        = block_size;
      for( i = 0; i < 258; i++ ) frq_tbl[i] = 0;
      if( ippStsNoErr != ippsEncodeZ1Z2_BZ2_8u16u( &tmp_src, &src_len, (Ipp16u*)dst, &dst_len, frq_tbl ) ) break;
      dict_size                 += 2;
      ((Ipp16u*)dst)[dst_len++]  = (Ipp16u)(dict_size - 1);
      frq_tbl[dict_size - 1]     = 1;
      PTR_EXCHANGE(src, dst);
      src_len = dst_len;
    } /* The 1-2 coding */
    if( ippStsNoErr != ippsEncodeHuffInit_BZ2_16u8u( dict_size, frq_tbl, (Ipp16u*)src, src_len,
                                                     (IppEncodeHuffState_BZ2*)shared_state ) ) break;
    tmp = block_size - out_len;
    if( ippStsNoErr != ippsPackHuffContext_BZ2_16u8u( &code, &code_len, out + out_len, &tmp,
                                                      (IppEncodeHuffState_BZ2*)shared_state ) ) break;
    out_len += tmp;
    { /* block */
      Ipp16u* tmp_src = (Ipp16u*)src;

      tmp = block_size;
      if( ippStsNoErr != ippsEncodeHuff_BZ2_16u8u( &code, &code_len, &tmp_src, &src_len, out + out_len, &tmp,
                                                   (IppEncodeHuffState_BZ2*)shared_state ) ) break;
      out_len += tmp;
    } /* block */
    ret = BZ_OK;
  } while( 0 );
  FLUSH_CODE_BUF_TO_OUT;
  if( code_len ) out[out_len]  = (Ipp8u)(code >> 24);
  mt_s->out_len      = out_len;
  mt_s->out_rem_bits = code_len;
  return ret;
} /* encode_block */

static int encode_mt( int_e_state* s )
{
  mt_e_state*  mt_s   = s->mt_states;
  int          chunks = s->cur_chunk;
  int          i;

  { /* block */
    int ret = BZ_OK;
    if( chunks > 1 ) {
#if defined(_OPENMP)
      int usr_limit_num_threads;

      if( ippStsNoErr != ippGetNumThreads( &usr_limit_num_threads ) ) usr_limit_num_threads = 1;
#pragma omp parallel for num_threads(usr_limit_num_threads) reduction(|: ret)
#endif
      for( i = 0; i < chunks; i++ ) ret = encode_block( s, i );
    } /* if */
    else ret = encode_block( s, 0 );

    if( BZ_OK != ret ) return BZ_PARAM_ERROR;
  } /* block */
  { /* block */
    Ipp8u*       out          = mt_s[0].out;
    int          out_len      = mt_s[0].out_len;
    int          out_rem_bits = mt_s[0].out_rem_bits;
    unsigned int combined_crc = s->combined_crc;
    unsigned int code;
    int          code_len;

    for( i = 1; i < chunks; i++ ) {
      int len_bits = mt_s[i].out_len * 8 + mt_s[i].out_rem_bits;
      if( ippStsNoErr != ippsCopy_1u( (Ipp8u*)mt_s[i].out, 0, (Ipp8u*)out + out_len, out_rem_bits,
                                      len_bits ) ) return BZ_PARAM_ERROR;
      len_bits     += out_rem_bits;
      out_rem_bits  = len_bits &  7;
      out_len      += len_bits >> 3;
    } /* for */
    for( i = 0; i < chunks; i++ ) {
      if( s->mt_states[i].in_len > 0 ) {
        combined_crc = ((combined_crc << 1) | (combined_crc >> 31)) ^ s->mt_states[i].block_crc;
      } /* if */
    } /* for */
    code     = ((unsigned int)out[out_len] & (-1 <<(8 - out_rem_bits))) << 24;
    code_len = out_rem_bits;
    if( BZ_E_M_FINISHING == s->mode ) {
      WR_TO_OUT(0x177245, 24);
      WR_TO_OUT(0x385090, 24);
      WR_TO_OUT(combined_crc >> 16,    16);
      WR_TO_OUT(combined_crc & 0xffff, 16);
      for( ; code_len > 0; code_len -= 8, code <<= 8 ) out[out_len++] = (Ipp8u)(code >> 24);
      code_len = 0;
    } /* if */
    s->combined_crc  = combined_crc;
    s->int_next_out  = out;
    s->int_avail_out = out_len;
    s->code          = code;
    s->code_len      = code_len;
  } /* block */
  return BZ_OK;
} /* encode_mt */

static int copy_to_int_e_buf( int_e_state* s )
{
  bz_stream*  strm             = s->strm;
  mt_e_state* mt_s             = s->mt_states;
  Ipp8u*      tmp_next_in      = (Ipp8u*)strm->next_in;
  int         tmp_avail_in     = (int)strm->avail_in;
  int         tmp_int_avail_in = (int)s->int_avail_in;
  int         cur_chunk        = s->cur_chunk;
  int         ret              = BZ_PARAM_ERROR;

  do {
    if( ippStsNoErr > ippsEncodeRLE_BZ2_8u( (Ipp8u**)&strm->next_in, (int*)&strm->avail_in,
                                            s->int_next_in, &tmp_int_avail_in,
                                            (IppRLEState_BZ2*)mt_s[cur_chunk].shared_state ) ) break;
    tmp_avail_in      -= (int)strm->avail_in;
    s->avail_in_expect = strm->avail_in;
    s->int_next_in    += tmp_int_avail_in;
    s->int_avail_in   -= tmp_int_avail_in;
    if( ippStsNoErr != ippsCRC32_BZ2_8u( tmp_next_in, tmp_avail_in, &mt_s[cur_chunk].block_crc ) ) break;
    if( (s->int_avail_in == 0) && (cur_chunk < s->chunks) ) {
      int ext_len = 1;

      if( ippStsNoErr != ippsEncodeRLEFlush_BZ2_8u( s->int_next_in, &ext_len, (IppRLEState_BZ2*)mt_s[cur_chunk].shared_state ) ) break;
      mt_s[cur_chunk].in_len = (s->block_size - 19 + ext_len);
      s->int_next_in        += ext_len;
      if( ++cur_chunk < s->chunks ) {
        s->int_avail_in    = (s->block_size - 19);
        mt_s[cur_chunk].in = s->int_next_in;
      } /* if */
    } /* if */
    strm->total_in_lo32 += (unsigned int)tmp_avail_in;
    if( strm->total_in_lo32 < (unsigned int)tmp_avail_in ) strm->total_in_hi32++;
    ret = BZ_OK;
  } while( 0 );
  s->cur_chunk = cur_chunk;
  return ret;
} /* copy_to_int_e_buf */

static int copy_from_int_e_buf( int_e_state* s )
{
  bz_stream* strm = s->strm;
  int tmp         = IPP_MIN( (int)strm->avail_out, (int)s->int_avail_out );

  if( ippStsNoErr != ippsCopy_8u( (Ipp8u*)s->int_next_out, (Ipp8u*)strm->next_out, tmp ) ) return BZ_PARAM_ERROR;
  s->int_next_out  += tmp;
  s->int_avail_out -= tmp;
  strm->next_out   += tmp;
  strm->avail_out  -= tmp;
  strm->total_out_lo32 += (unsigned int)tmp;
  if( strm->total_out_lo32 < (unsigned int)tmp ) strm->total_out_hi32++;
  return BZ_OK;
} /* copy_from_int_e_buf */

static int encode_run( int_e_state* s )
{
  bz_stream* strm = s->strm;
  int        ret  = BZ_PARAM_ERROR;

  for( ;; ) {
    if( BZ_E_D_INPUT == s->direction ) {
      if( 0 == strm->avail_in ) break;
      if( BZ_OK != copy_to_int_e_buf( s ) ) break;
      if( 0 != s->int_avail_in ) { ret = BZ_OK; break; }
      s->direction = BZ_E_D_OUTPUT;
      if( BZ_OK != encode_mt( s ) ) break;
    } /* if */
    if( BZ_E_D_OUTPUT == s->direction ) {
      if( 0 == strm->avail_out ) break;
      if( BZ_OK != copy_from_int_e_buf( s ) ) break;
      if( 0 != s->int_avail_out ) { ret = BZ_OK; break; }
      if( BZ_OK != init_e_block_state( s ) ) break;
      if( 0 == strm->avail_in ) { ret = BZ_OK; break; }
    } /* if */
  } /* for */
  return ret;
} /* encode_run */

static int flush_rle( int_e_state* s )
{
  int tmp_int_avail_in = s->int_avail_in + 19;

  if( ippStsNoErr != ippsEncodeRLEFlush_BZ2_8u( s->int_next_in, &tmp_int_avail_in,
                                                (IppRLEState_BZ2*)s->mt_states[s->cur_chunk].shared_state ) ) return BZ_PARAM_ERROR;
  s->mt_states[s->cur_chunk++].in_len = s->block_size - 19 - s->int_avail_in + tmp_int_avail_in;
  return BZ_OK;
} /* flush_rle */

static int encode_flush_finish( int_e_state* s )
{
  bz_stream* strm = s->strm;
  int        ret  = BZ_SEQUENCE_ERROR;

  if( strm->avail_in == s->avail_in_expect ) {
    for( ;; ) {
      if( BZ_E_D_INPUT == s->direction ) {
        if( 0 != strm->avail_in ) {
          if( BZ_OK != copy_to_int_e_buf( s ) ) break;
        } /* if */
        if( 0 == s->avail_in_expect ) {
          if( BZ_OK != flush_rle( s ) ) break;
        } /* if */
        s->direction = BZ_E_D_OUTPUT;
        if( BZ_OK != encode_mt( s ) ) break;
      } /* if */
      if( BZ_E_D_OUTPUT == s->direction ) {
        if( 0 == strm->avail_out ) break;
        if( BZ_OK != copy_from_int_e_buf( s ) ) break;
        if( 0 != s->int_avail_out ) { ret = BZ_OK; break; }
        if( BZ_OK != init_e_block_state( s ) ) break;
        if( 0 == s->avail_in_expect ) { ret = BZ_OK; break; }
      } /* if */
    } /* for */
  } /* if */
  return ret;
} /* encode_flush_finish */

/*---------------------------------------------------*/
int BZ_API(BZ2_bzCompress) ( bz_stream *strm, int action )
{
  int_e_state* s;

  if( NULL == strm ) return BZ_PARAM_ERROR;
  s = (int_e_state*)strm->state;
  if( NULL == s ) return BZ_PARAM_ERROR;
  if( strm != s->strm ) return BZ_PARAM_ERROR;
  for( ;; ) {
    switch( s->mode ) {
      case BZ_E_M_RUNNING:
        if( BZ_RUN == action ) {
          if( BZ_OK == encode_run( s ) ) return BZ_RUN_OK;
        } /* if */
        else if( (BZ_FLUSH == action) || (BZ_FINISH == action) ) {
          s->avail_in_expect = strm->avail_in;
          s->mode            = action;
          break;
        } /* else if */
        return BZ_PARAM_ERROR;
      case BZ_E_M_FLUSHING:
        if( BZ_FLUSH != action ) return BZ_SEQUENCE_ERROR;
        if( BZ_OK != encode_flush_finish( s ) ) return BZ_SEQUENCE_ERROR;
        if( (s->avail_in_expect > 0) || (s->int_avail_out > 0) ) return BZ_FLUSH_OK;
        s->mode = BZ_E_M_RUNNING;
        if( strm->avail_in > 0 ) break;
        return BZ_RUN_OK;
      case BZ_E_M_FINISHING:
        if( BZ_FINISH != action ) return BZ_SEQUENCE_ERROR;
        if( BZ_OK != encode_flush_finish( s ) ) return BZ_SEQUENCE_ERROR;
        if( (s->avail_in_expect > 0) || (s->int_avail_out > 0) ) return BZ_FINISH_OK;
        s->mode = BZ_E_M_IDLE;
        return BZ_STREAM_END;
      default:
        return BZ_SEQUENCE_ERROR;
    } /* switch */
  } /* for */
  return BZ_OK;
} /* BZ2_bzCompress */

/*---------------------------------------------------*/
int BZ_API(BZ2_bzCompressEnd) ( bz_stream *strm )
{
  if( NULL == strm )        return BZ_PARAM_ERROR;
  if( NULL == strm->state ) return BZ_PARAM_ERROR;
  BZFREE( strm->state );
  strm->state = NULL;
  return BZ_OK;
} /* BZ2_bzCompressEnd */

/* ==================================
             Decode block
   ================================== */

#define BZ_D_M_IDLE              0
#define BZ_D_M_MAGIC             1
#define BZ_D_M_HDR_1             2
#define BZ_D_M_HDR_2             3
#define BZ_D_M_CRC_1             4
#define BZ_D_M_CRC_2             5
#define BZ_D_M_INDEX             6
#define BZ_D_M_MAPPING           7
#define BZ_D_M_IN_USE            8
#define BZ_D_M_HUFF_1            9
#define BZ_D_M_HUFF_2           10
#define BZ_D_M_DECODE           11
#define BZ_D_M_OUTPUT           12
#define BZ_D_M_EOB              13
#define BZ_D_M_HDR_LAST_BLOCK   14
#define BZ_D_M_CRC_1_LAST_BLOCK 15
#define BZ_D_M_CRC_2_LAST_BLOCK 16

typedef struct {
  Ipp8u*        in;
  Ipp8u*        out;
  Ipp8u*        shared_state;
  unsigned int  in_len;
  unsigned int  out_len;
  unsigned int  stored_block_crc;
  int           bwt_index;
  int           dict_size;
  unsigned char in_use[256];
} mt_d_state;

typedef struct {
  bz_stream*     strm;
  mt_d_state*    mt_states;
  unsigned int   code;
  int            code_len;
  int            mode;
  int            last_block;
  int            block_size;
  unsigned int   block_crc;
  unsigned int   combined_crc;
  unsigned int   stored_combined_crc;
  unsigned short in_use_16;
  int            chunks;
  int            cur_chunk;
  int            tmp_chunk;
} int_d_state;

#define RD_BYTE_FROM_IN code |= ((unsigned int)*(next_in++)) << (24 - code_len)
#define RD_FROM_IN(LEN_LE_24) \
for( ; (code_len < (LEN_LE_24)) && (avail_in > 0); code_len += 8, avail_in-- ) { \
  RD_BYTE_FROM_IN; \
}
#define RD_FROM_IN_WITH_CHK(LEN_LE_24) \
RD_FROM_IN(LEN_LE_24) \
if( code_len < (LEN_LE_24) ) break
#define SHL_CODE(LEN_L_32) code <<= (LEN_L_32); code_len -= (LEN_L_32)
#define SHL_CODE_AND_NEXT_MODE(LEN_L_32) SHL_CODE(LEN_L_32); mode++

/*---------------------------------------------------*/

static int init_d_block_state( int_d_state* s )
{
  mt_d_state* mt_s = s->mt_states;
  int         i;

  s->cur_chunk = 0;
  for( i = 0; i < s->chunks; i++ ) {
    mt_s[i].in_len = 0;
    mt_s[i].out    = mt_s[i].in + sizeof(Ipp16u) * s->block_size;
  } /* for */
  return BZ_OK;
} /* init_d_block_state */

int BZ_API(BZ2_bzDecompressInit) ( bz_stream* strm, int verbosity, int small )
{
  if( NULL == strm ) return BZ_PARAM_ERROR;
  if( (1 != sizeof(char)) || (2 != sizeof(short)) || (4 != sizeof(int)) ) return BZ_CONFIG_ERROR;
  if( NULL == strm->bzalloc ) strm->bzalloc = default_bzalloc;
  if( NULL == strm->bzfree ) strm->bzfree = default_bzfree;
  verbosity = verbosity;
  small     = small;
  { /* block */
    int_d_state *s = (int_d_state*)BZALLOC( sizeof(int_d_state) );
    if( NULL == s ) return BZ_MEM_ERROR;

    strm->total_in_lo32  = 0;
    strm->total_in_hi32  = 0;
    strm->total_out_lo32 = 0;
    strm->total_out_hi32 = 0;
    strm->state          = s;
    s->mt_states = 0;

    s->strm         =  strm;
    s->mode         =  BZ_D_M_MAGIC;
    s->last_block   =  0;
    s->code         =  0;
    s->code_len     =  0;
    s->combined_crc =  0;
    s->block_crc    =  0;
  } /* block */
  return BZ_OK;
} /* BZ2_bzDecompressInit */

static int decode_block( int_d_state *s, int cur_chunk )
{
    mt_d_state* mt_s    = s->mt_states + cur_chunk;
    Ipp8u* src          = mt_s->in;
    Ipp8u* dst          = mt_s->out;
    Ipp8u* shared_state = mt_s->shared_state;
    int    dst_len      = s->block_size;
    int    ret          = BZ_DATA_ERROR;

    do {
      if( ippStsNoErr != ippsDecodeBlock_BZ2_16u8u( (Ipp16u*)src, mt_s->in_len, dst, &dst_len,
                                                    mt_s->bwt_index, mt_s->dict_size - 2, mt_s->in_use,
                                                    (Ipp8u*)shared_state ) ) break;
      ret = BZ_OK;
    } while( 0 );
    mt_s->out_len = dst_len;
    return ret;
} /* decode_block */

static int copy_from_int_d_buf( int_d_state *s )
{
  bz_stream*   strm   = s->strm;
  mt_d_state*  mt_s_0 = s->mt_states;
  mt_d_state*  mt_s;
  int          tmp_avail_out;

  while( (s->tmp_chunk < s->cur_chunk) && (strm->avail_out) ) {
    mt_s          = mt_s_0 + s->tmp_chunk;
    tmp_avail_out = strm->avail_out;

    if( mt_s->out_len ) {
      if( ippStsNoErr > ippsDecodeRLEState_BZ2_8u( (Ipp8u**)&mt_s->out, &mt_s->out_len,
                                                   (Ipp8u**)&strm->next_out, &strm->avail_out,
                                                   (IppRLEState_BZ2*)mt_s_0->shared_state ) ) return BZ_DATA_ERROR;
    } /* if */
    else {
      if( ippStsNoErr > ippsDecodeRLEStateFlush_BZ2_8u( (IppRLEState_BZ2*)mt_s_0->shared_state,
                                                          (Ipp8u**)&strm->next_out, &strm->avail_out ) ) return BZ_DATA_ERROR;
    } /* else */
    if( (tmp_avail_out -= strm->avail_out) > 0 ) {
      strm->total_out_lo32 += (unsigned int)tmp_avail_out;
      if( strm->total_out_lo32 < (unsigned int)tmp_avail_out ) strm->total_out_hi32++;
      if( ippStsNoErr != ippsCRC32_BZ2_8u( (Ipp8u*)strm->next_out - tmp_avail_out, tmp_avail_out,
                                           &s->block_crc ) ) return BZ_DATA_ERROR;
    } /* if */
    if( 0 == strm->avail_out ) return BZ_OK;
    if( 0 == mt_s->out_len ) {
      if( s->block_crc != mt_s->stored_block_crc ) return BZ_DATA_ERROR;
      s->combined_crc = ((s->combined_crc << 1) | (s->combined_crc >> 31)) ^ s->block_crc;
      s->block_crc = 0;
      s->tmp_chunk++;
    } /* if */
  } /* while */
  return BZ_OK;
} /* copy_from_int_d_buf */

/*---------------------------------------------------*/
int BZ_API(BZ2_bzDecompress) ( bz_stream *strm )
{
  if( NULL == strm )           return BZ_PARAM_ERROR;
  if( NULL == strm->state )    return BZ_PARAM_ERROR;
  if( NULL == strm->next_in )  return BZ_PARAM_ERROR;
  if( NULL == strm->next_out ) return BZ_PARAM_ERROR;
  { /* block */
    int_d_state* s            = (int_d_state*)strm->state;
    mt_d_state*  mt_s         = s->mt_states + s->cur_chunk;
    Ipp8u*       next_in      = (Ipp8u*)strm->next_in;
    int          avail_in     = (int)strm->avail_in;
    int          tmp_avail_in = avail_in;
    unsigned int code         = s->code;
    int          code_len     = s->code_len;
    int          cur_chunk    = s->cur_chunk;
    int          mode         = s->mode;
    int          ret          = BZ_OK;
    int          i, tmp;

    preswitch:
    switch( mode ) {
    case BZ_D_M_HDR_1:
      RD_FROM_IN_WITH_CHK(24);
      if( 0x314159 != (code >> (unsigned)8) ) {
        if( 0x177245 != (code >> (unsigned)8) ) { ret = BZ_DATA_ERROR; break; }
        SHL_CODE(24);
        s->last_block = 1;
        mode          = BZ_D_M_HDR_LAST_BLOCK;
        goto preswitch;
      } /* if */
      SHL_CODE_AND_NEXT_MODE(24);
    case BZ_D_M_HDR_2:
      RD_FROM_IN_WITH_CHK(24);
      if( 0x265359 != (code >> (unsigned)8) ) { ret = BZ_DATA_ERROR; break; }
      SHL_CODE_AND_NEXT_MODE(24);
    case BZ_D_M_CRC_1:
      RD_FROM_IN_WITH_CHK(16);
      mt_s->stored_block_crc = code & 0xFFFF0000;
      SHL_CODE_AND_NEXT_MODE(16);
    case BZ_D_M_CRC_2:
      RD_FROM_IN_WITH_CHK(16+1);
      mt_s->stored_block_crc |= code >> (unsigned)16;
      SHL_CODE_AND_NEXT_MODE(16+1);
    case BZ_D_M_INDEX:
      RD_FROM_IN_WITH_CHK(24);
      mt_s->bwt_index = code >> (unsigned)8;
      if( (0 > mt_s->bwt_index) || (s->block_size + 10 < mt_s->bwt_index) ) { ret = BZ_DATA_ERROR; break; }
      SHL_CODE_AND_NEXT_MODE(24);
    case BZ_D_M_MAPPING:
      RD_FROM_IN_WITH_CHK(16);
      s->in_use_16 = (unsigned short)(code >> (unsigned)16);
      if( ippStsNoErr != ippsZero_8u( (Ipp8u*)mt_s->in_use, 256 ) ) { ret = BZ_DATA_ERROR; break; }
      mt_s->dict_size = 0;
      SHL_CODE_AND_NEXT_MODE(16);
    case BZ_D_M_IN_USE:
      { /* block */
        int j;
        for( i = 0; i < 16; i++ ) {
          if( s->in_use_16 & (1 << (15 - i)) ) {
            RD_FROM_IN_WITH_CHK(16);
            for( j = 0; j < 16; j++ ) {
              tmp                      = (code >> (unsigned)(31 - j)) & 1;
              mt_s->in_use[i * 16 + j] = (unsigned char)tmp;
              mt_s->dict_size         += tmp;
            } /* for */
            s->in_use_16 &= (1 << (15 - i)) ^ -1;
            SHL_CODE(16);
          } /* if */
        } /* for */
      } /* block */
      if( s->in_use_16 ) break;
      mt_s->dict_size += 2;
      if( ippStsNoErr != ippsDecodeHuffInit_BZ2_8u16u( mt_s->dict_size,
                                                       (IppDecodeHuffState_BZ2*)mt_s->shared_state) ) {
        ret = BZ_DATA_ERROR;
        break;
      } /* if */
      mode++;
    case BZ_D_M_HUFF_1:
      if( 0 == avail_in ) break;
      { /* block */
        IppStatus st = ippsUnpackHuffContext_BZ2_8u16u( &code, &code_len, (Ipp8u**)&next_in, (int*)&avail_in,
                                                        (IppDecodeHuffState_BZ2*)mt_s->shared_state );
        if( ippStsSrcSizeLessExpected == st ) break;
        if( ippStsNoErr != st ) { ret = BZ_DATA_ERROR; break; }
      } /* block */
      mode++;
    case BZ_D_M_HUFF_2:
      if( 0 == avail_in ) break;
      { /* block */
        int in_size = s->block_size - mt_s->in_len;
        IppStatus st = ippsDecodeHuff_BZ2_8u16u( &code, &code_len, (Ipp8u**)&next_in, (int*)&avail_in,
                                                 (Ipp16u*)mt_s->in + mt_s->in_len, &in_size,
                                                 (IppDecodeHuffState_BZ2*)mt_s->shared_state );
        mt_s->in_len += in_size;
        if( ippStsSrcSizeLessExpected == st ) break;
        if( ippStsNoErr != st ) { ret = BZ_DATA_ERROR; break; }
        cur_chunk++; mt_s++;
        if( cur_chunk < s->chunks ) {
          mode = BZ_D_M_HDR_1;
          goto preswitch;
        } /* if */
        else mode++;
      } /* block */
    case BZ_D_M_DECODE:
      { /* block */
        if( cur_chunk > 1 ) {
#if defined(_OPENMP)
          int usr_limit_num_threads;

          if( ippStsNoErr != ippGetNumThreads( &usr_limit_num_threads ) ) usr_limit_num_threads = 1;
#pragma omp parallel for num_threads(usr_limit_num_threads) reduction(|: ret)
#endif
          for( i = 0; i < cur_chunk; i++ ) ret = decode_block( s, i );
        } /* if */
        else ret = decode_block( s, 0 );
        if( BZ_OK != ret ) { ret = BZ_PARAM_ERROR; break; }
        if( ippStsNoErr != ippsDecodeRLEStateInit_BZ2_8u( (IppRLEState_BZ2*)s->mt_states->shared_state ) ) {
          ret = BZ_DATA_ERROR; break;
        } /* if */
        s->tmp_chunk = 0;
        mode++;
      } /* block */
    case BZ_D_M_OUTPUT:
      s->cur_chunk = cur_chunk;
      if( BZ_OK != (ret = copy_from_int_d_buf( s )) ) { ret = BZ_DATA_ERROR; break; }
      if( s->tmp_chunk < s->cur_chunk ) break;
    case BZ_D_M_EOB:
      if( s->last_block ) {
        if( s->stored_combined_crc != s->combined_crc ) { ret = BZ_DATA_ERROR; break; }
        mode = BZ_D_M_IDLE;
        ret = BZ_STREAM_END; break;
      } /* if */
      else {
        if( BZ_OK != (ret = init_d_block_state( s )) ) { ret = BZ_DATA_ERROR; break; }
        mode = BZ_D_M_HDR_1;
        cur_chunk = 0;
        mt_s = s->mt_states;
        goto preswitch;
      } /* else */
      break;
    case BZ_D_M_HDR_LAST_BLOCK:
      RD_FROM_IN_WITH_CHK(24);
      if( 0x385090 != (code >> (unsigned)8) ) { ret = BZ_DATA_ERROR; break; }
      SHL_CODE_AND_NEXT_MODE(24);
    case BZ_D_M_CRC_1_LAST_BLOCK:
      RD_FROM_IN_WITH_CHK(16);
      s->stored_combined_crc = code & 0xFFFF0000;
      SHL_CODE_AND_NEXT_MODE(16);
    case BZ_D_M_CRC_2_LAST_BLOCK:
      RD_FROM_IN_WITH_CHK(16);
      s->stored_combined_crc |= code >> (unsigned)16;
      SHL_CODE(16);
      if( cur_chunk > 0 ) mode = BZ_D_M_DECODE;
      else mode = BZ_D_M_EOB;
      goto preswitch;
    case BZ_D_M_MAGIC:
      { /* block */
        int max_state_size, block_size;

        RD_FROM_IN(24);
        if( (code_len + avail_in * 8) < 32 ) break;
        if( 0x425A68 /*"BZh"*/ != (code >> (unsigned)8) ) { ret = BZ_DATA_ERROR_MAGIC; break; }
        SHL_CODE(24);
        RD_BYTE_FROM_IN;
        avail_in     -= 1;
        s->block_size = block_size = ((code >> (unsigned)24) - 0x30/*'0'*/) * 100000;
        code        <<= 8;
        if( (100000 > block_size) || (900000 < block_size) ) { ret = BZ_DATA_ERROR_MAGIC; break; }
        if( ippStsNoErr != ippsDecodeBlockGetSize_BZ2_8u( block_size, &max_state_size ) ) { ret = BZ_DATA_ERROR; break; }
        if( ippStsNoErr != ippsDecodeHuffGetSize_BZ2_8u16u( block_size, &tmp ) ) { ret = BZ_DATA_ERROR; break; }
        max_state_size = IPP_MAX(max_state_size, tmp);
        if( ippStsNoErr != ippsRLEGetSize_BZ2_8u( &tmp ) ) { ret = BZ_DATA_ERROR; break; }
        max_state_size = IPP_MAX(max_state_size, tmp);
#if defined(_OPENMP)
        tmp = omp_get_max_threads();
#else
        tmp = 1;
#endif
        mt_s = s->mt_states = BZALLOC( tmp * (block_size * (sizeof(Ipp16u) + sizeof(Ipp8u)) +
                                              sizeof(mt_d_state) + max_state_size) );
        if( NULL == mt_s ) { ret = BZ_MEM_ERROR; break; }
        mt_s[0].shared_state = (Ipp8u*)mt_s + sizeof(mt_d_state) * tmp;
        mt_s[0].in           = mt_s[0].shared_state + max_state_size;
        mt_s[0].out          = mt_s[0].in + sizeof(Ipp16u) * block_size;
        mt_s[0].in_len       = 0;
        for( i = 1; i < tmp; i++ ) {
          mt_s[i].shared_state = mt_s[i - 1].out + sizeof(Ipp8u) * block_size;
          mt_s[i].in           = mt_s[i].shared_state + max_state_size;
          mt_s[i].out          = mt_s[i].in + sizeof(Ipp16u) * block_size;
          mt_s[i].in_len       = 0;
        } /* for */
      } /* block */
      s->chunks = tmp;
      cur_chunk = 0;
      mode++;
      goto preswitch;
    case BZ_D_M_IDLE:
      ret = BZ_SEQUENCE_ERROR; break;
    default:
      ret = BZ_DATA_ERROR; break;
    } /* switch */
    tmp_avail_in        -= avail_in;
    strm->total_in_lo32 += (unsigned int)tmp_avail_in;
    if( strm->total_in_lo32 < (unsigned int)tmp_avail_in ) strm->total_in_hi32++;
    strm->next_in  = (char*)next_in;
    strm->avail_in = avail_in;
    s->code        = code;
    s->code_len    = code_len;
    s->cur_chunk   = cur_chunk;
    s->mode        = mode;
    return ret;
  } /* block */
} /* BZ2_bzDecompress */

/*---------------------------------------------------*/
int BZ_API(BZ2_bzDecompressEnd)  ( bz_stream *strm )
{
  if( NULL == strm )                                   return BZ_PARAM_ERROR;
  if( NULL == strm->state )                            return BZ_PARAM_ERROR;
#if 0
    /* 
    * This should return BZ_PARAM_ERROR only if there were BZ2_bzDecompress 
    * calls after BZ2_bzDecompressInit.  Just commenting out for now. 
    */ 
  if( NULL == ((int_d_state*)strm->state)->mt_states ) return BZ_PARAM_ERROR;
#endif
  if ( NULL != ((int_d_state*)strm->state)->mt_states )
    BZFREE( ((int_d_state*)strm->state)->mt_states );
  BZFREE( strm->state );
  strm->state = NULL;
  return BZ_OK;
} /* BZ2_bzDecompressEnd */
