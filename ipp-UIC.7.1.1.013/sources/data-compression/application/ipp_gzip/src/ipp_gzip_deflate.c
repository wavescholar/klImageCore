/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives Data Compression Domain-based GZIP
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
*/
#ifndef GZIP_DEFLATE_OLD

   #include <stdio.h>
   #include <sys/stat.h>
   #include <ippcore.h>
   #include <ippdc.h>
   #include <fcntl.h>
   #include <string.h>
   #include <stdlib.h>
   #include <ipps.h>
   #ifndef _WIN32
      #include <unistd.h>
   #endif

   #include "ipp_gzip.h"
   #include "ipp_gzip_deflate.h"

   #ifdef GZIP_VMTHREADS
      #define CRC_BUFSIZE (64*1024)

struct crc_params {
   char         *filename;
   int          max_chunk;
   SIZE_TYPE    chunk_size;
   unsigned int *crc;
   unsigned int *crc_array;
};
   #endif

static const unsigned char rev_code_table[256] = {
   0, 128,  64, 192,  32, 160,  96, 224,  16, 144,  80, 208,  48, 176, 112, 240,
   8, 136,  72, 200,  40, 168, 104, 232,  24, 152,  88, 216,  56, 184, 120, 248,
   4, 132,  68, 196,  36, 164, 100, 228,  20, 148,  84, 212,  52, 180, 116, 244,
   12,140,  76, 204,  44, 172, 108, 236,  28, 156,  92, 220,  60, 188, 124, 252,
   2, 130,  66, 194,  34, 162,  98, 226,  18, 146,  82, 210,  50, 178, 114, 242,
   10,138,  74, 202,  42, 170, 106, 234,  26, 154,  90, 218,  58, 186, 122, 250,
   6, 134,  70, 198,  38, 166, 102, 230,  22, 150,  86, 214,  54, 182, 118, 246,
   14,142,  78, 206,  46, 174, 110, 238,  30, 158,  94, 222,  62, 190, 126, 254,
   1, 129,  65, 193,  33, 161,  97, 225,  17, 145,  81, 209,  49, 177, 113, 241,
   9, 137,  73, 201,  41, 169, 105, 233,  25, 153,  89, 217,  57, 185, 121, 249,
   5, 133,  69, 197,  37, 165, 101, 229,  21, 149,  85, 213,  53, 181, 117, 245,
   13,141,  77, 205,  45, 173, 109, 237,  29, 157,  93, 221,  61, 189, 125, 253,
   3, 131,  67, 195,  35, 163,  99, 227,  19, 147,  83, 211,  51, 179, 115, 243,
   11,139,  75, 203,  43, 171, 107, 235,  27, 155,  91, 219,  59, 187, 123, 251,
   7, 135,  71, 199,  39, 167, 103, 231,  23, 151,  87, 215,  55, 183, 119, 247,
   15,143,  79, 207,  47, 175, 111, 239,  31, 159,  95, 223,  63, 191, 127, 255
};

struct def_state {
   Ipp16u               buf_bits;
   unsigned int         buf_valid;
   UBYTE*               in_buf;
   UBYTE*               out_buf;
   const UBYTE*         in_ptr;
   UBYTE*               win_ptr;
   UBYTE*               store_ptr;
   char*                file_name;
   unsigned int         in_len;
   unsigned int         out_len;
   SIZE_TYPE            file_size;
   int level;
#ifdef GZIP_VMTHREADS
   unsigned int         chunk_num;
   unsigned int         chunk_max;
   SIZE_TYPE            chunk_size;
   int                  status;
   GZIP_FILE            *fd_out;
   GZIP_FILE            *fd_in;
#endif
};
static void send_bits(struct def_state* state, unsigned int value, unsigned int length) {
   unsigned int len = length;
   unsigned short val = (unsigned short)value;

   if(state->buf_valid > (unsigned int)(BUF_SIZE - len)) {
      state->buf_bits = (unsigned short)(state->buf_bits | (val << state->buf_valid));
      PUT_SHORT(state, state->buf_bits);
      state->buf_bits = (unsigned short)(val >> (BUF_SIZE - state->buf_valid));
      state->buf_valid += len - BUF_SIZE;
   } else {
      state->buf_bits = (unsigned short)(state->buf_bits | (value << state->buf_valid));
      state->buf_valid += len;
   }
}
static void buf_flush(struct def_state* state) {
   if(state->buf_valid > 8) {
      PUT_SHORT(state, state->buf_bits);
   } else if(state->buf_valid > 0) {
      put_byte(state, (UBYTE)state->buf_bits);
   }
   state->buf_bits = 0;
   state->buf_valid = 0;
}
static unsigned short code_rev(int code, int len){
   return(unsigned short)(((rev_code_table[code & 255] << 8) | rev_code_table[(code >> 8) & 255]) >> (16 - len));
}

static void gen_codes(freq_table* freq, code_table* code, freq_table* srv_freq,
                      short* code_max, int* pbound, int bound_len, int len ){
   freq_table freq_copy[286];
   freq_table *low_idx, *hi_idx, *low_bound, *hi_bound, *max_idx;
   freq_table *low_bound_stk[32], *hi_bound_stk[32];
   int middle, stk_cnt = 1, s_len;
   int counts[286];
   int i, j, max_len = 0, cnt;
   int min = bound_len, bound, prev = -1, curr;
   short max = 0;
   unsigned short frq, idx;

   for( j = len - 1, i = 0; i < j; ) {
      while((j > i) && (freq[j].frq == 0) ) j--;
      while((j > i) && (freq[i].frq  > 0) ) i++;
      SWAP(freq_table, freq[i], freq[j]);
   }
   if( i == 0 ) {
      freq[0].frq = 1;
      len = 2;
   } else
      len = i + 1;

   s_len = len;
   low_idx = freq;
   hi_idx = freq + s_len - 1;
   do {
      if(s_len <= 9) {
         for(; hi_idx > low_idx; hi_idx--) {
            for(max_idx = low_idx, low_bound = low_idx + 1; low_bound <= hi_idx; low_bound++)
               max_idx = (low_bound->frq > max_idx->frq)?(low_bound):(max_idx);
            SWAP(freq_table, *hi_idx, *max_idx);
         }
         hi_idx = hi_bound_stk[--stk_cnt];
         low_idx = low_bound_stk[stk_cnt];
      } else {
         s_len >>= 1;
         if( low_idx->frq > (low_idx + s_len)->frq )
            SWAP(freq_table, *low_idx, *(low_idx + s_len));
         if( low_idx->frq > hi_idx->frq )
            SWAP(freq_table, *low_idx, *hi_idx);
         if( (low_idx + s_len)->frq > hi_idx->frq )
            SWAP(freq_table, *(low_idx + s_len), *hi_idx);
         low_bound = low_idx + 1;
         hi_bound = hi_idx;
         middle   = (low_idx + s_len)->frq;
#if defined(_MSC_VER)
#pragma warning(disable:4127)
#endif
         while(1) {
            while((hi_bound > low_bound) && (low_bound->frq <= middle)) low_bound++;
            while((hi_bound > low_bound) && (hi_bound->frq >  middle)) hi_bound--;
            if(low_bound == hi_bound)
               break;
            SWAP(freq_table, *low_bound, *hi_bound);
         }
         do {
            low_bound--;
         } while((low_bound->frq == middle) && (low_bound > low_idx));

         if((low_bound - low_idx) < (hi_idx - hi_bound)) {
            if(low_bound == low_idx) {
               low_idx = hi_bound;
            } else {
               low_bound_stk[stk_cnt] = hi_bound;
               hi_bound_stk[stk_cnt++] = hi_idx;
               hi_idx = low_bound;
            }
         } else {
            if(hi_bound == hi_idx) {
               hi_idx = low_bound;
            } else {
               low_bound_stk[stk_cnt] = low_idx;
               hi_bound_stk[stk_cnt++] = low_bound;
               low_idx = hi_bound;
            }
         }
      }
      s_len = (int)(hi_idx - low_idx + 1);
   } while(stk_cnt);

   for(i = 0; i < len; i++) {
      max = (short)(((short)freq[i].idx > max)?(freq[i].idx):(max));
      freq_copy[i] = freq[i];
   }
   *code_max = (short)(max + 1);
   if(len > 2) {
      for(i = 0; i < len-1; i++) {
         frq = (unsigned short)(freq[i].frq + freq[i+1].frq);
         idx = freq[i+1].idx;
         for(j = i + 2; (j < len) && (frq >= freq[j].frq); j++)
            freq[j-1] = freq[j];
         freq[i].frq = freq[j-1].idx = idx;
         freq[j-1].frq = frq;
      }
      for(i = 0; i < 286; i++)
         counts[i] = 0;
      for(i = len-2; i >= 0; i--) {
         code[freq[i].frq].len++;
         max_len = code[freq[i].idx].len = code[freq[i].frq].len;
         counts[max_len] += 2;
         counts[max_len-1] -= 1;
      }
      if(max_len > bound_len) {
         for(i = max_len; i > bound_len; i--) {
            for(cnt = counts[i]; cnt > 0; cnt -= 2, counts[i-1]++) {
               for(j = bound_len - 1; (j > 0) && (counts[j] == 0); j--);
               min = (j < min)?(j):(min);
               counts[j] -= 1;
               counts[j + 1] += 2;
            }
         }
         for(i = bound_len, j = 0; i >= min; i--) {
            for(cnt = counts[i]; cnt > 0; cnt--)
               code[freq_copy[j++].idx].len = (unsigned short)i;
         }
      }
      for(i = 2, pbound[1] = 0; i <= bound_len; i++)
         pbound[i] = (pbound[i-1] + counts[i-1]) << 1;
   } else {
      for(i = 0; i < len; i++)
         code[freq[i].idx].len = 1;
      pbound[1] = 0;
   }

   if(srv_freq) {
      cnt = i = 0;
      len = *code_max;

      for(i = 0; i < len;) {
         if((curr = code[i].len) != 0) {
            if( curr == prev ) {
               bound = (len < (i+6))?(len):(i+6);
               for(cnt = i; (i < bound) && ((curr = code[i].len) == prev); i++)
                  code[i].code = code_rev(pbound[code[i].len]++, code[i].len);
               if((cnt = i-cnt) >= 3)
                  srv_freq[16].frq++;
               else {
                  srv_freq[prev].frq = (unsigned short)(cnt + srv_freq[prev].frq);
               }
            } else {
               prev = curr;
               code[i].code = code_rev(pbound[code[i].len]++, code[i].len);
               srv_freq[curr].frq++;
               i++;
            }
         } else {
            bound = (len < (i+138))?(len):(i+138);
            prev  = 0;
            for(cnt = i; (i < bound) && ((curr = code[i].len) == 0); i++);
            if((cnt = i - cnt) >= 3) {
               if(cnt >= 11)
                  srv_freq[18].frq++;
               else
                  srv_freq[17].frq++;
            } else {
               srv_freq[0].frq = (unsigned short)(cnt + srv_freq[0].frq);
            }
         }
      }
   }
}
static void build_trees(freq_table* len_freq, freq_table* oft_freq, code_table* len_code,
                        code_table* oft_code, code_table* srv_code,
                        short* len_max, short* oft_max, short* srv_max){
   int i;
   freq_table srv_freq[20];
   int bounds[16];

   for(i=0;i<286;i++) {
      len_freq[i].idx = (unsigned short)i;
      len_code[i].code = len_code[i].len = 0;
   }
   for(i=0;i<30;i++) {
      oft_freq[i].idx = (unsigned short)i;
      oft_code[i].code = oft_code[i].len = 0;
   }
   for(i = 0; i < 20; i++) {
      srv_freq[i].idx = (unsigned short)i;
      srv_freq[i].frq = 0;
      srv_code[i].code = srv_code[i].len = 0;
   }

   gen_codes(len_freq, len_code, srv_freq, len_max, bounds, 15, 286);
   gen_codes(oft_freq, oft_code, srv_freq, oft_max, bounds, 15, 30);
   gen_codes(srv_freq, srv_code, NULL, srv_max, bounds, 7, 20);
   for(i = 0; i < *srv_max; i++) {
      srv_code[i].code = code_rev(bounds[srv_code[i].len]++, srv_code[i].len);
   }
   for(*srv_max = 18; *srv_max >= 3; (*srv_max)--) {
      if(srv_code[srv_order[*srv_max]].len) break;
   }
}
static void write_tree(struct def_state* state, code_table* code, short code_max, code_table* srv_code) {
   int i, cnt, bound, tmp_len, prev = -1, curr;
   unsigned int tmp_code;

   for(i = 0; i < code_max;) {
      if((curr = code[i].len) != 0) {
         if( curr == prev ) {
            bound = IPP_MIN(code_max, (i + 6));
            for( cnt = i; (i < bound) && ((curr = code[i].len) == prev); i++ );
            if( (cnt = i - cnt) >= 3 ) {
               send_bits(state, srv_code[16].code, srv_code[16].len);
               send_bits(state, (cnt - 3), 2);
            } else {
               tmp_code = srv_code[prev].code;
               tmp_len  = srv_code[prev].len;
               while( cnt-- ) {
                  send_bits(state, tmp_code, tmp_len);
               }
            }
         } else {
            prev = curr;
            send_bits(state, srv_code[curr].code, srv_code[curr].len);
            i++;
         }
      } else {
         bound = IPP_MIN(code_max, (i + 138));
         prev  = 0;
         for( cnt = i; (i < bound) && ((curr = code[i].len) == 0); i++ );
         if( (cnt = i - cnt) >= 3 ) {
            if( cnt >= 11 ) {
               send_bits(state, srv_code[18].code, srv_code[18].len);
               send_bits(state, (cnt - 11), 7);
            } else {
               send_bits(state, srv_code[17].code, srv_code[17].len);
               send_bits(state, (cnt - 3), 3);
            }
         } else {
            tmp_code = srv_code[0].code;
            tmp_len  = srv_code[0].len;
            while(cnt--) {
               send_bits(state, tmp_code, tmp_len);
            }
         }
      }
   }
}
static int get_tree_len(code_table* code, short code_max, code_table* srv_code) {
   int i, cnt, bound, tmp_len, prev = -1, curr;
   int tree_size = 0;

   for(i = 0; i < code_max;) {
      if((curr = code[i].len) != 0) {
         if( curr == prev ) {
            bound = IPP_MIN(code_max, (i + 6));
            for( cnt = i; (i < bound) && ((curr = code[i].len) == prev); i++ );
            if( (cnt = i - cnt) >= 3 ) {
               tree_size += (srv_code[16].len + 2);
            } else {
               tmp_len  = srv_code[prev].len;
               while( cnt-- ) {
                  tree_size += tmp_len;
               }
            }
         } else {
            prev = curr;
            tree_size += srv_code[curr].len;
            i++;
         }
      } else {
         bound = IPP_MIN(code_max, (i + 138));
         prev  = 0;
         for( cnt = i; (i < bound) && ((curr = code[i].len) == 0); i++ );
         if( (cnt = i - cnt) >= 3 ) {
            if( cnt >= 11 ) {
               tree_size += (srv_code[18].len + 7);
            } else {
               tree_size += (srv_code[17].len + 3);
            }
         } else {
            tmp_len  = srv_code[0].len;
            while(cnt--) {
               tree_size += tmp_len;
            }
         }
      }
   }
   return tree_size;
}
static block_est get_type(const UBYTE* l_buf, const unsigned short* d_buf, code_table* len_code, code_table* oft_code,
                          code_table* srv_code, unsigned int lit_size, unsigned int stored_len, short len_max, short oft_max, short srv_max){
   unsigned int fixed_len = 3;
   unsigned int dynamic_len = 17 + 3*srv_max;
   int length, idx;
   unsigned int i, n, offset;
   block_est block;
   short can_store = 1;
   code_table len_code_ext[256];
   code_table oft_code_ext[256];

   if(stored_len > STORED_MAX)
      can_store = 0;
   else
      stored_len = stored_len*8+40;

   for(i=0; i<256; i++) {
      idx = l_code_tab[i];
      length = len_code[idx + 257].len;
      len_code_ext[i].code = (unsigned short)(len_code[idx + 257].code | (len_excode_table[i].code << length));
      len_code_ext[i].len = (unsigned short)(length + len_excode_table[i].len);

      idx = d_code_tab[i];
      length = oft_code[idx].len;
      oft_code_ext[i].code = (unsigned short)(oft_code[idx].code | (oft_excode_table[i].code << length));
      oft_code_ext[i].len = (unsigned short)(length + oft_excode_table[i].len);
   }

   for(i=0; i<lit_size; i++) {
      if((offset = d_buf[i]) != 0) {
         length = l_buf[i]+3;
         fixed_len += l_table[length].len;
         dynamic_len += len_code_ext[length-3].len;
         if(offset > 256) {
            for(n=0; offset >= o_table_h[n+1].base; n++) ;
            idx = d_code_tab[256 + ((offset - 1) >> 7)];
            fixed_len += 5 + o_table_h[n].len;
            dynamic_len += oft_code[idx].len;
            dynamic_len += d_extra_tab[idx];
         } else {
            fixed_len += o_table_l[offset-1].len;
            dynamic_len += oft_code_ext[offset - 1].len;
         }
      } else {
         length = l_buf[i];
         fixed_len += c_table[length].len;
         dynamic_len += len_code[length].len;
      }
   }

   block.type = DYNAMIC_BLOCK;
   block.size = dynamic_len>>3;
   if(dynamic_len >= fixed_len) {
      block.type = STATIC_BLOCK;
      block.size = fixed_len>>3;
   }
   if(fixed_len >= stored_len && dynamic_len >= stored_len && can_store) {
      block.type = STORED_BLOCK;
      block.size = stored_len>>3;
   }

   if(block.type == DYNAMIC_BLOCK) {
      dynamic_len += get_tree_len(len_code, (short)(len_max-1), srv_code);
      dynamic_len += get_tree_len(oft_code, (short)(oft_max-1), srv_code);
      if(dynamic_len >= fixed_len) {
         block.type = STATIC_BLOCK;
         block.size = fixed_len>>3;
      }
      if(fixed_len >= stored_len && dynamic_len >= stored_len && can_store) {
         block.type = STORED_BLOCK;
         block.size = stored_len>>3;
      }
   }
   return block;
}

#ifdef GZIP_VMTHREADS
static void VM_THREAD_CALLCONVENTION process(void* p_state)
{
   struct def_state *state = p_state;
   GZIP_FILE *fd_out = state->fd_out;
   GZIP_FILE *fd_in = state->fd_in;
   unsigned int chunk_end = 0;
#else
static int process(struct def_state* state, GZIP_FILE *fd_in, GZIP_FILE *fd_out) {
#endif
   MEM_ENTRY *p_mem;
   int *hash_head, *hash_prev;
   code_table *cur_len_code, *cur_oft_code;
   UBYTE* l_buf;
   unsigned short* d_buf;
   unsigned int last_lit = 0, arrow = 0, n, win_arrow, eof = 0;
   unsigned int stored_len, flush = 0, final = 0, crc = 0, dst_len;
   size_t hash_size = WIN_SIZE;
   short len_max, oft_max, srv_max;
   int temp, status = 0;
   block_est block_stat;
   freq_table len_freq[286];
   freq_table oft_freq[30];
   code_table len_code[286];
   code_table oft_code[30];
   code_table srv_code[20];

   p_mem = zalloc_mem_head();
   hash_prev = (int*)zalloc_mem(sizeof(int)*hash_size, p_mem);
   hash_head = (int*)zalloc_mem(sizeof(int)*hash_size, p_mem);
   l_buf = (UBYTE*)zalloc_mem(LIT_SIZE, p_mem);
   d_buf = (unsigned short*)zalloc_mem(LIT_SIZE*sizeof(unsigned short), p_mem);

   ippsSet_32s(-(int)hash_size, hash_head, (int)hash_size);
#ifdef GZIP_VMTHREADS
   if(state->chunk_size != -1) {
      state->in_len = (unsigned int)zread(fd_in, state->in_buf,
          ((size_t)state->chunk_size<INBUF_SIZE)?(size_t)state->chunk_size:INBUF_SIZE);
      state->chunk_size -= state->in_len;
   } else {
      state->in_len = (unsigned int)zread(fd_in, state->in_buf, INBUF_SIZE);
      ippsCRC32_8u(state->in_buf, state->in_len, &crc);
   }
#else
   state->in_len = (unsigned int)zread(fd_in, state->in_buf, INBUF_SIZE);
   ippsCRC32_8u(state->in_buf, state->in_len, &crc);
#endif
   if(from_stdin) {
      state->file_size += state->in_len;
      if(state->in_len == 0) { /* If stdin is empty we create empty gzip file */
         state->out_buf[state->out_len++] = 3;
         ippsSet_8u(0, state->out_buf + state->out_len, 9);
         state->out_len += 9;
         zwrite(fd_out, state->out_buf, state->out_len);
         /* Free all */
         zfree_all(p_mem);
#ifdef GZIP_VMTHREADS
         state->status = status;
         return;
#else
         return status;
#endif
      }
   }
   state->buf_bits = 0;
   state->buf_valid = 0;
   win_arrow = WIN_SIZE;

   while(1) {
      for(n = 0; n < 286; n++) len_freq[n].frq = 0;
      for(n = 0; n < 30; n++) oft_freq[n].frq = 0;
      len_freq[256].frq = 1;
      len_max = oft_max = srv_max = 0;
      stored_len = 0;
      dst_len = LIT_SIZE - 1;
      last_lit = 0;

      while(1) {
#ifdef GZIP_VMTHREADS
         if(state->chunk_size != -1)
            eof = (state->chunk_size <= 0)?1:0;
         else
            eof = zeof(fd_in);
#else
         eof = zeof(fd_in);
#endif
         if(state->in_len < 300 && !eof) {
            ippsMove_8u(state->win_ptr + win_arrow - WIN_SIZE*2, state->store_ptr, WIN_SIZE*2);
            ippsMove_8u(state->in_ptr, state->in_buf, state->in_len);
#ifdef GZIP_VMTHREADS
            if(state->chunk_size != -1) {
               temp = (unsigned int)zread(fd_in, state->in_buf + state->in_len,
                   ((size_t)state->chunk_size<(INBUF_SIZE - state->in_len))?(size_t)state->chunk_size:(INBUF_SIZE - state->in_len));
               state->chunk_size -= temp;
            } else {
               temp = (unsigned int)zread(fd_in, state->in_buf + state->in_len, INBUF_SIZE - state->in_len);
               ippsCRC32_8u(state->in_buf+state->in_len, temp, &crc);
            }
#else
            temp = (int)zread(fd_in, state->in_buf + state->in_len, INBUF_SIZE - state->in_len);
            ippsCRC32_8u(state->in_buf+state->in_len, temp, &crc);
#endif
            if(from_stdin)
               state->file_size += temp;
            state->in_len += temp;
            state->in_ptr = state->in_buf;
            win_arrow = WIN_SIZE;
         }

#ifdef GZIP_VMTHREADS
         if(state->chunk_size != -1) {
            if(eof) {
               if(state->chunk_num == state->chunk_max)
                  flush = 3;
               else
                  flush = 2;
            }
         } else
            if(eof) flush = 3;
#else
         if(eof) flush = 3;
#endif
         temp = state->in_len;
         if((arrow+(unsigned)temp) > 0x7FFFFFFF) { /* 2GB */
            unsigned int norm_level = (unsigned int)0x7fffffff + 1 - (2*WIN_SIZE);
            arrow -= norm_level;
            ippsSubC_32s_ISfs( (Ipp32s)norm_level, (Ipp32s*)hash_prev, WIN_SIZE, 0 );
            ippsSubC_32s_ISfs( (Ipp32s)norm_level, (Ipp32s*)hash_head, (int)hash_size, 0 );
         }
         status = ippsDeflateLZ77_8u(&state->in_ptr, (Ipp32u*)&state->in_len, &arrow, state->win_ptr + win_arrow, WIN_SIZE,
                                     hash_head, hash_prev, (int)hash_size, (IppDeflateFreqTable*)len_freq, (IppDeflateFreqTable*)oft_freq,
                                     l_buf + last_lit, d_buf + last_lit, &dst_len, state->level, (IppLZ77Flush)flush);

         if(status) {
            status = ST_ERROR;
            warn_message(state->file_name, "lz77 deflate error");
            break;
         }
         stored_len += temp - state->in_len;
         win_arrow += temp - state->in_len;
         last_lit = LIT_SIZE - dst_len - 1;

         if(eof && state->in_len == 0) {
#ifdef GZIP_VMTHREADS
            if(state->chunk_size != -1) {
               if(state->chunk_num == state->chunk_max)
                  final = 1;
               else
                  chunk_end = 1;
            } else
               final = 1;
#else
            final = 1;
#endif
            break;
         }
         if(dst_len <= 0)
            break;
      }
      if(status == ST_ERROR) break;

      build_trees(len_freq, oft_freq, len_code, oft_code, srv_code, &len_max, &oft_max, &srv_max);
      block_stat = get_type(l_buf, d_buf, len_code, oft_code, srv_code, last_lit, stored_len, len_max, oft_max, srv_max);

      if(final) block_stat.size += 8;
      if(block_stat.size+state->out_len >= OUTBUF_SIZE-1000) {
         zwrite(fd_out, state->out_buf, state->out_len);
         state->out_len = 0;
      }

      if(STORED_BLOCK == block_stat.type) {
         send_bits(state, (STORED_BLOCK<<1)+final, 3);
         buf_flush(state);
         send_bits(state, stored_len, 16);
         send_bits(state, stored_len^0xffff, 16);
         buf_flush(state);

         ippsCopy_8u(state->in_ptr - stored_len, state->out_buf + state->out_len, stored_len);
         state->out_len += stored_len;
      } else {
         if(STATIC_BLOCK == block_stat.type) {
            send_bits(state, (STATIC_BLOCK<<1)+final, 3);
            cur_len_code = (code_table*)static_len_code;
            cur_oft_code = (code_table*)static_oft_code;
         } else {
            send_bits(state, (DYNAMIC_BLOCK<<1)+final, 3);
            srv_max++;
            send_bits(state, len_max - 257, 5);
            send_bits(state, oft_max - 1, 5);
            send_bits(state, srv_max - 4, 4);
            for(temp = 0; temp < srv_max; temp++)
               send_bits(state, srv_code[srv_order[temp]].len, 3);
            write_tree(state, len_code, len_max, srv_code);
            write_tree(state, oft_code, oft_max, srv_code);
            cur_len_code = len_code;
            cur_oft_code = oft_code;
         }
         status = ippsDeflateHuff_8u(l_buf, d_buf, last_lit, &state->buf_bits, &state->buf_valid,
                                     (IppDeflateHuffCode*)cur_len_code, (IppDeflateHuffCode*)cur_oft_code,
                                     state->out_buf, (Ipp32u*)&state->out_len);
         if(status) {
            status = ST_ERROR;
            warn_message(state->file_name, "huffman deflate error");
            break;
         }
         if(state->out_len > OUTBUF_SIZE) {
            status = ST_ERROR;
            warn_message(state->file_name, "out buffer overflow");
            break;
         }
         send_bits(state, cur_len_code[256].code, cur_len_code[256].len);
      }
#ifdef GZIP_VMTHREADS
      if(final || chunk_end) {
         if(flush == 2 && state->chunk_size != -1) { /* Empty stored block to align data to a byte boundary */
            send_bits(state, (STORED_BLOCK<<1)+final, 3);
            buf_flush(state);
            send_bits(state, 0x0000, 16);
            send_bits(state, 0xffff, 16);
            buf_flush(state);
         }
#else
      if(final) {
#endif
         buf_flush(state);
#ifdef GZIP_VMTHREADS
         if(state->chunk_size != -1) {
            zwrite(fd_out, state->out_buf, state->out_len);
            if(state->chunk_num > 0)
               ztruncate(fd_out);
            zclose(fd_in);
         } else {
#endif
            put_byte(state, (UBYTE)(crc & 0xff));
            put_byte(state, (UBYTE)((crc >> 8) & 0xff));
            put_byte(state, (UBYTE)((crc >> 16) & 0xff));
            put_byte(state, (UBYTE)((crc >> 24) & 0xff));
            put_byte(state, (UBYTE)(state->file_size & 0xff));
            put_byte(state, (UBYTE)((state->file_size >> 8) & 0xff));
            put_byte(state, (UBYTE)((state->file_size >> 16) & 0xff));
            put_byte(state, (UBYTE)((state->file_size >> 24) & 0xff));
            zwrite(fd_out, state->out_buf, state->out_len);
#ifdef GZIP_VMTHREADS
         }
#endif
         break;
      }
   }
   zfree_all(p_mem);
#ifdef GZIP_VMTHREADS
   state->status = status;
#else
   return status;
#endif
}

void deflate_st(char *filename)
{
   STAT_TYPE    file_info;
   GZIP_FILE    *fd_in, *fd_out;
   Ipp64u       start_clocks = 0;
   char         out_filename[MAX_NAME], base_name[MAX_NAME];
   unsigned char *general_buffer, *general_ptr;
   unsigned int general_size;
   struct def_state* pstate;
   int          status = 0;
   char         stdin_name[] = "stdin";

   if(!from_stdin) {
      /* Skip bogus files */
      if(strcmp(".", filename) == 0 || strcmp("..", filename) == 0)
         return;
      if(known_suffix(filename))
         return;
   }


   if(!from_stdin) {
      if(SYSSTAT(filename, &file_info) == -1) {
         error_file(filename);
         return;
      }
   }   /* !from_stdin */
   /* Start CPU clocking if required */
   if(opt_perf_stat)
      start_clocks = ippGetCpuClocks();
   /* Open input file */
   if(from_stdin) {
      filename = stdin_name;
      fd_in = zopen_stdin();
   } else
      fd_in = ZOPEN(filename, GZIP_FILE_READ, opt_io_read_method, 0);
   if(fd_in == NULL) {                  /* Cannot open input file for sharing for some reason */
      error_file(filename);
      return;
   }

   /* Allocate input/output buffers */
   general_size = sizeof(struct def_state) + (unsigned int)INBUF_SIZE + (unsigned int)OUTBUF_SIZE + WIN_SIZE*2;
   general_ptr = general_buffer = malloc(general_size);
   pstate = (struct def_state*)general_ptr;
   pstate->out_buf = (general_ptr += sizeof(struct def_state));
   pstate->store_ptr = (general_ptr += OUTBUF_SIZE);
   pstate->win_ptr = (general_ptr += WIN_SIZE);
   pstate->in_ptr = pstate->in_buf = (general_ptr += WIN_SIZE);
   pstate->out_len = 0;

   if(!from_stdin)
      pstate->file_size = file_info.st_size;
   else
      pstate->file_size = 0;
#ifdef GZIP_VMTHREADS
   pstate->chunk_size = -1;
#endif

   if(!opt_noname)
      get_base_name(filename, base_name);
   pstate->out_len = (unsigned int)(create_gzip_hdr(pstate->out_buf, base_name, &file_info, 0) - pstate->out_buf);   /* Last 0 means that we need no comment here */

   if(!opt_to_stdout) {
      create_output_name(filename, out_filename);
      fd_out = out_open(out_filename, pstate->file_size+OUTBUF_SIZE); /* + OUTBUF_SIZE in case of what result file may be greater than source */
      if(fd_out == NULL) {
         error_file(out_filename);
         zclose(fd_in);
         free(general_buffer);
         return;
      }
//      fd_out->uncompressed_size = pstate->file_size+(SIZE_TYPE)OUTBUF_SIZE; /* + OUTBUF_SIZE in case of what result file may be greater than source */
   } else
      fd_out = zopen_stdout();

   if(pstate->file_size == 0 && !from_stdin) { /* Special case specific for GZIP */
      pstate->out_buf[pstate->out_len++] = 3;
      ippsSet_8u(0, pstate->out_buf + pstate->out_len, 9);
      pstate->out_len += 9;
      zwrite(fd_out, pstate->out_buf, pstate->out_len);
      if(opt_perf_stat || opt_verbose)
         print_stat(0, (float)(100*(1.-zget_bytes_out(fd_out))), filename, out_filename, 0);
      zclose(fd_in);
      if(fd_out->file_io_method == METHOD_MMAP) zextend_file(fd_out, fd_out->file_bytes_out);
      zclose(fd_out);
      free(general_buffer);
      if(!opt_to_stdout && filename != NULL)
         UNLINK(filename);
      return;
   }

   pstate->level = 6;
   if(opt_compress_best)
      pstate->level = 9;
   if(opt_compress_fast)
      pstate->level = 1;

   pstate->file_name = filename;
#ifdef GZIP_VMTHREADS
   pstate->fd_in = fd_in;
   pstate->fd_out = fd_out;
   process(pstate);
   status = pstate->status;
#else
   status = process(pstate, fd_in, fd_out);
#endif
   if(status != ST_ERROR && opt_io_write_method == METHOD_MMAP)
      ztruncate(fd_out);
   if(status != ST_ERROR && (opt_perf_stat || opt_verbose)) {
      if(!from_stdin && !opt_to_stdout) {
         print_stat((float)((ippGetCpuClocks()-start_clocks)/zget_bytes_in(fd_in)),
             (float)(100*(1.-(float)zget_bytes_out(fd_out)/zget_bytes_in(fd_in))), filename, out_filename, 0);
      } else
         print_stat(0, 0, filename, out_filename, 0);
   }
#if defined(_WIN32)
   zclose(fd_out);
   zclose(fd_in);
#endif
   if(status != ST_ERROR && !opt_to_stdout && !from_stdin) {
#if !defined(WIN32)
      /* For Linux: copy input file permission bits to destination file */
      zset_permbits(fd_out, zget_permbits(fd_in));
      zclose(fd_out);
      zclose(fd_in);
#endif
      if(UNLINK(filename) != 0)
         warn_message(filename, "cannot unlink");
   }
   free(general_buffer);
   return;
}

   #ifdef GZIP_VMTHREADS
/**
 * Computes input file CRC in separated thread
 * @param filename  - input file name
 * @param max_chunk - max number of chunks to split
 * @param chunk_size - size of each chunk
 */
static void VM_THREAD_CALLCONVENTION compute_crc(void *params)
{
   GZIP_FILE    *fd;
   UBYTE        buffer[CRC_BUFSIZE];
   char         *filename;
   SIZE_TYPE    incnt, bytes_for_crc, total_bytes, chunk_size;
   size_t       bytes_to_read;
   unsigned int *crc, *crc_array;
   int          chunk_no, max_chunk;
   struct       crc_params *param_ptr;

   param_ptr = params;
   filename = param_ptr->filename;
   max_chunk = param_ptr->max_chunk;
   chunk_size = param_ptr->chunk_size;
   crc = param_ptr->crc;
   crc_array = param_ptr->crc_array;
   if((fd = ZOPEN(filename, GZIP_FILE_READ, opt_io_read_method, 0)) == NULL) {
      warn_message(filename, "cannot open file for CRC computing");
      return;
   }
   zseek(fd, 0, SEEK_SET); /* Necessary to start mapping */
   chunk_no = 0;
   bytes_for_crc = chunk_size;
   total_bytes = 0;
   *crc = 0;
   while(!zeof(fd)) {
      bytes_to_read = (bytes_for_crc < sizeof(buffer))? (size_t)bytes_for_crc : sizeof(buffer);
      incnt = (SIZE_TYPE)zread(fd, buffer, bytes_to_read);
      bytes_for_crc -= incnt;
      total_bytes += incnt;
      ippsCRC32_8u(buffer, (int)incnt, crc);
      if(chunk_no < max_chunk) {
         if(bytes_for_crc == 0) {
            crc_array[chunk_no++] = *crc;
            bytes_for_crc = chunk_size;
         }
      } else {
         if(bytes_for_crc == 0) {
            if(!zeof(fd)) {
               bytes_for_crc = chunk_size;
               continue;
            }
         }
      }
   }
   zclose(fd);
   return;
}

#if defined(_WIN32)
extern SYSTEM_INFO sysInfo;
#endif
static void VM_THREAD_CALLCONVENTION finalize_proc(void* p_data)
{
   GZIP_FINALIZE_DATA *ptr = (GZIP_FINALIZE_DATA*)p_data;
   size_t in_cnt;
   UBYTE *p_buffer;
   GZIP_MMAP_HANDLE *p_view_in, *p_view_out;

   p_view_in = zdup_mmap(ptr->in_file_ptr, 0);
   p_view_out = zdup_mmap(ptr->file_ptr, ptr->initial_offset);
   p_buffer = (UBYTE*)malloc(INBUF_SIZE);
   while((in_cnt = zread_dup_mmap(p_view_in, p_buffer, INBUF_SIZE)) > 0) {
      zwrite_dup_mmap(p_view_out, p_buffer, in_cnt);
   }
   free(p_buffer);
   zclose_dup_mmap(p_view_in);
   zclose_dup_mmap(p_view_out);
}

void deflate_mt(char *filename)
{
   STAT_TYPE      file_info;
   GZIP_FILE      *fd_out;          /* File descriptor for output */
   GZIP_FILE      **fd_out_vector; /* Vector of temporary file descriptors */
   GZIP_FILE      **fd_in_vector; /* Vector of temporary file descriptors */
   Ipp64u         start_clocks;
   unsigned int   chunks, comment_size; /* Number of chunks to slice file to */
   SIZE_TYPE      chunk_size, chunk_rem;
   MEM_ENTRY*     p_mem;
   char           out_filename[MAX_NAME], base_name[MAX_NAME];
   UBYTE          tail_buf[8], *tail_ptr, *ptr, comment_crc_xored, *inbuf;
   UBYTE          *zip_header, *zip_hdr_ptr;      /* buffer to store zip header */
   UBYTE          *real_comment;  /* Pointer to comment buffer */
   OFFSET_TYPE    comment_offset;            /* Offset from beginning of file to comment field */
   unsigned int   i, comment_crc, incnt, compress_level;
   unsigned int   *crc_array;   /* CRC value array */
   OFFSET_TYPE    *offsets;       /* Binary offset array */
   unsigned int   crc;          /* Accumulated CRC */
   int            status_write, status = 0;
   struct         crc_params compute_crc_params;
   vm_thread      *thread_vec;
   struct def_state *state;
   int            cbl;    /* Comment field remaining length */
   SIZE_TYPE      tot_file_len = 0;
   SIZE_TYPE      final_len = 0;
   GZIP_FINALIZE_DATA
                  *finalize_vec;
#if defined(VS2005_PLUS)
   int            nchar;
#endif

   if(known_suffix(filename))
      return;

   if(SYSSTAT(filename, &file_info) == -1) {
      error_file(filename);
      return;
   }
   if(opt_dbg_slice_size && (file_info.st_size < atoi(opt_slice_size_array) || file_info.st_size < 100)) {       /* "-j minsize" option found */
      deflate_st(filename);
      return;
   } else if(!opt_dbg_slice_size && file_info.st_size < MIN_LENGTH_TO_SLICE) { /* If file is too small, process it with no chunks */
      deflate_st(filename);
      return;
   }

   if(opt_dbg_multithreads) {
      int dbg_multithreads = atoi(opt_num_threads_forced);
      if(dbg_multithreads > MAX_THREADS)
         chunks = MAX_THREADS;
      else if(dbg_multithreads < 1)
         chunks = vm_sys_info_get_cpu_num();
      else
         chunks = dbg_multithreads;
   } else
      chunks = vm_sys_info_get_cpu_num();

   if(chunks == 1 || chunks > (unsigned int)file_info.st_size) { /* Single thread environment */
      deflate_st(filename);
      return;
   }

   if(opt_perf_stat)
      start_clocks = ippGetCpuClocks();

   chunk_rem = file_info.st_size % chunks;
   chunk_size = file_info.st_size / chunks;

   comment_size = COMMENT_SIZE(chunks);   /* Size of comment field */
   create_output_name(filename, out_filename);
   if(METHOD_MMAP == opt_io_write_method)
      final_len = file_info.st_size + OUTBUF_SIZE*3; /* We'll be writing in parallel, so file len must be maximal */
   else
      final_len = chunk_size + OUTBUF_SIZE;
   if((fd_out = out_open(out_filename, final_len)) == NULL) {
      error_file(out_filename);
      return;
   }
#if !defined(_WIN32)
   /* On Linux copy permission bits */
   zset_permbits(fd_out, file_info.st_mode);
#endif

   zset_uncompressed_size(fd_out, file_info.st_size + OUTBUF_SIZE);

   p_mem = zalloc_mem_head();
   real_comment = (UBYTE*)zalloc_mem(comment_size, p_mem);
   offsets = (OFFSET_TYPE*)zalloc_mem(sizeof(OFFSET_TYPE)*(chunks-1), p_mem);
   crc_array = (unsigned int*)zalloc_mem(sizeof(unsigned int)*(chunks-1), p_mem);
   zip_hdr_ptr = zip_header = (UBYTE*)zalloc_mem(GZIP_HEADER_SIZE + strlen(filename) + 1 + comment_size, p_mem);
   fd_out_vector = (GZIP_FILE**)zalloc_mem(sizeof(GZIP_FILE*)* chunks, p_mem);
   fd_in_vector = (GZIP_FILE**)zalloc_mem(sizeof(GZIP_FILE*) * chunks, p_mem);
   state = (struct def_state*)zalloc_mem(sizeof(struct def_state)*chunks, p_mem);

   if(!opt_noname)
      get_base_name(filename, base_name);
   zip_hdr_ptr = create_gzip_hdr(zip_hdr_ptr, base_name, &file_info, 1);           /* Create gzip header structure with comment */

   comment_offset = (long)(zip_hdr_ptr - zip_header);

   /* Fake comment field for now */
   for(i=0; i<comment_size-1; i++) {
      PUT_BYTE(zip_hdr_ptr, 'U');
   }
   PUT_BYTE(zip_hdr_ptr, 0);
   /* Write header to file */
   status_write = (int)zwrite(fd_out, zip_header, zip_hdr_ptr - zip_header);
   if(status_write == -1) {
      error_file(out_filename);
      zfree_all(p_mem);
      zclose(fd_out);
      return;
   }

   /* Modify thread number to +1 to use 1 additional thread for CRC computing */
   for(i=0; i<chunks; i++) {
      char tmp_name[MAX_NAME];
      if(i > 0) {   /* Thread #1 using fd_out as compressed data output */
         if((fd_in_vector[i] = ZOPEN(filename, GZIP_FILE_READ, opt_io_read_method, 0)) == NULL) {
            error_file(filename);
            zfree_all(p_mem);
            return;
         }
         if(zseek(fd_in_vector[i], (OFFSET_TYPE)(i*chunk_size), SEEK_SET) == 0) {
            warn_message(filename, "cannot seek");
            zfree_all(p_mem);
            zclose(fd_in_vector[i]);
            return;
         }
         if((fd_out_vector[i] = ZOPEN(mk_chunk_name(filename, i, tmp_name), GZIP_FILE_WRITE, 
                opt_io_write_method, (file_info.st_size/chunks)+(SIZE_TYPE)OUTBUF_SIZE*2)) == NULL) {
            warn_message(filename, "cannot create temporary file");
            zfree_all(p_mem);
            return;
         }
      } else {
         if((fd_in_vector[i] = ZOPEN(filename, GZIP_FILE_READ, opt_io_read_method, 0)) == NULL) {
            error_file(filename);
            zfree_all(p_mem);
            return;
         }
         fd_out_vector[i] = fd_out;
      }
   }

   compress_level = 6;
   if(opt_compress_best)
      compress_level = 9;
   if(opt_compress_fast)
      compress_level = 1;

   thread_vec = (vm_thread*)malloc((chunks+1) * sizeof(vm_thread));
   compute_crc_params.filename = filename;
   compute_crc_params.max_chunk = chunks-1;
   compute_crc_params.chunk_size = chunk_size;
   compute_crc_params.crc = &crc;
   compute_crc_params.crc_array = crc_array;
   vm_thread_set_invalid(&thread_vec[0]);
   vm_thread_create(&thread_vec[0], (vm_thread_callback)compute_crc, &compute_crc_params);
   /* Create data structure and start processing threads */
   for(i=1; i<chunks+1; i++) {
       UBYTE* ptr = zalloc_mem(OUTBUF_SIZE+WIN_SIZE*2+INBUF_SIZE, p_mem);
        state[i-1].out_buf = ptr;
        ptr += OUTBUF_SIZE;
      state[i-1].store_ptr = ptr;
      ptr += WIN_SIZE;
      state[i-1].win_ptr = ptr;
      ptr += WIN_SIZE;
      state[i-1].in_ptr = state[i-1].in_buf = ptr;
      state[i-1].out_len = 0;
      state[i-1].file_name = filename;
      state[i-1].chunk_num = i-1;
      state[i-1].chunk_max = chunks-1;
      state[i-1].chunk_size = (i!=chunks)?chunk_size:(chunk_size + chunk_rem);
      state[i-1].fd_in = fd_in_vector[i-1];
      state[i-1].fd_out = fd_out_vector[i-1];
      state[i-1].level = compress_level;
      vm_thread_set_invalid(&thread_vec[i]);
      vm_thread_create(&thread_vec[i], (vm_thread_callback)process, &state[i-1]);
   }
   /* Wait when all threads finish */
   for(i=0; i<chunks+1; i++) {
      vm_thread_wait(&thread_vec[i]);
      if(status != ST_ERROR)
         status = state[i].status;
      vm_thread_close(&thread_vec[i]);
   }
   free(thread_vec);
   if(status == ST_ERROR) {   /* Some of thread(s) got problem */
      zfree_all(p_mem);
      return;
   }
   /* Build offset table, compute total file length */
   offsets[0] = ztell(fd_out);
   if (opt_io_write_method == METHOD_MMAP)
        tot_file_len += offsets[0];
   for(i=1; i<chunks-1; i++) {
      OFFSET_TYPE loc_len = ztell(fd_out_vector[i]);
      offsets[i] = offsets[i-1] + loc_len;
      if (opt_io_write_method == METHOD_MMAP)
        tot_file_len += loc_len;
   }
   if(opt_io_write_method == METHOD_MMAP) {
        tot_file_len += ztell(fd_out_vector[chunks-1]);
        tot_file_len += 8; /* Here we have total compressed file len, which can be used for parallel write, if mmap is used */
      zset_max_file_len(fd_out, tot_file_len);
   }
   inbuf = (UBYTE*) zalloc_mem(INBUF_SIZE, p_mem);
   /* Prepare offset/crc arrays */
   /* Comment field (BTW, unused in orig gzip) has the form:
        0x0000  COMMENT_MAGIC_HEADER (0xC0)
        # update for huge files
        0x0001  Decompressed file size (8 bytes in Hex form)
        0x0009  Number of chunks (2..127)
        0x000A  Offset to chunk 1
        0x0012  Crc of Chunk 0
        0x001A  Offset to chunk 2
        0x0022  Crc of chunk 1
        ...
        0xNNNN  Zero byte (0x00)
    */
   ptr = real_comment;
   *ptr++ = COMMENT_MAGIC_HEADER;
   cbl = comment_size-1;
#if defined(VS2005_PLUS)
   nchar = sprintf_s((char*)ptr, cbl, OFFSET_FORMAT, file_info.st_size);
   ptr += nchar;
   cbl -= nchar;
   nchar = sprintf_s((char*)ptr, cbl, "%1X", chunks);
   ptr += nchar;
   cbl -= nchar;
#else
   ptr += sprintf((char*)ptr, OFFSET_FORMAT, file_info.st_size);
   ptr += sprintf((char*)ptr, "%1X", chunks);
#endif
   for(i=0; i<chunks-1; i++) {
#if defined(VS2005_PLUS)
       nchar = sprintf_s((char*)ptr, cbl, OFFSET_FORMAT, offsets[i]);
      ptr += nchar;
   cbl -= nchar;
#else
      ptr += sprintf((char*)ptr, OFFSET_FORMAT, offsets[i]);
#endif
   }
   for(i=0; i<chunks-1; i++) {
#if defined(VS2005_PLUS)
       nchar = sprintf_s((char*)ptr, cbl, "%08X", crc_array[i]);
      ptr += nchar;
   cbl -= nchar;
#else
      ptr += sprintf((char*)ptr, "%08X", crc_array[i]);
#endif
   }
   comment_crc = 0;
   ippsCRC32_8u(real_comment, (int)(ptr - real_comment), &comment_crc);
   comment_crc_xored = (UBYTE)((comment_crc & 0xFF) ^ ((comment_crc >> 8) & 0xFF) ^ ((comment_crc >> 16) & 0xFF) ^ ((comment_crc >> 24) & 0xFF));
   if(comment_crc_xored == 0)
      comment_crc_xored = 1;              /* Avoid extra nulls in asciz field */
   *ptr++ = comment_crc_xored;
   real_comment[comment_size-1] = '\0';
   /*
    Move temporary files content to destination file
    */
   if(opt_io_write_method == METHOD_LOW) {
      for(i=1; i<chunks; i++) {
         if(zseek(fd_out_vector[i], (OFFSET_TYPE)0, SEEK_SET)) {
            warn_message(filename, "cannot rewind temporary file");
            zfree_all(p_mem);
            return;
         }
         inbuf = zalloc_mem(INBUF_SIZE*2, p_mem); /* Don't do that in real life (with regular malloc).
                                                  inbuf is an already allocated pointer */
         while(!zeof(fd_out_vector[i])) {
             size_t readsize = INBUF_SIZE*2;
            if((incnt = (unsigned int)zread(fd_out_vector[i], inbuf, readsize)) == -1) {
               warn_message(filename, "cannot read from tmp file");
               zfree_all(p_mem);
               return;
            }
            zwrite(fd_out, inbuf, incnt);
         }
         zclose(fd_out_vector[i]);
         remove_tmp_file(filename, i);
      }
   }
   else {
      /* With mmap i/o we can write destination file content in parallel using threads */
      /* Prepare tasks for writing threads */
      thread_vec = (vm_thread*)zalloc_mem(sizeof(vm_thread)*(chunks-1), p_mem);
      finalize_vec = (GZIP_FINALIZE_DATA*)zalloc_mem(sizeof(GZIP_FINALIZE_DATA)*(chunks-1), p_mem);
      for(i=0; i<chunks-1; i++) {
         finalize_vec[i].initial_offset = offsets[i];
         finalize_vec[i].file_ptr = fd_out;
         finalize_vec[i].status = ST_OK;
         finalize_vec[i].in_file_ptr = fd_out_vector[i+1];

         vm_thread_set_invalid(thread_vec+i);
         vm_thread_create(thread_vec+i, (vm_thread_callback)finalize_proc, &finalize_vec[i]);
      }
      /* Wait for all threads */
      status = ST_OK;
      for(i=0; i<chunks-1; i++) {
         vm_thread_wait(&thread_vec[i]);
         if(status != ST_ERROR)
            status = finalize_vec[i].status;
         vm_thread_close(&thread_vec[i]);
         zclose(fd_out_vector[i+1]);
         remove_tmp_file(filename, i+1);
      }
      /* Seek to the end of output file - 8 */
      zseek(fd_out, 8, SEEK_END);
   }
   /* Finish file with CRC and total file length */
   tail_ptr = tail_buf;
   PUT_LONG(tail_ptr, crc);
   PUT_LONG(tail_ptr, (unsigned long)file_info.st_size);
   zwrite(fd_out, tail_buf, 8);

   if(zseek(fd_out, comment_offset, 0) == (-1L))
      warn_message(filename, "final seek failed");
   else
      zwrite(fd_out, real_comment, comment_size);

   if (!zextend_file(fd_out, tot_file_len))
  {
      error_system("cannot truncate file");
      warn_message(out_filename, "failed");
        zfree_all(p_mem);
        return;
  }

  if(opt_perf_stat || opt_verbose) {
      print_stat((float)((ippGetCpuClocks()-start_clocks)/file_info.st_size),
         (float)(100*(1.-(float)zget_bytes_out(fd_out)/file_info.st_size)),
                 filename, out_filename, 1);
  }
  if(!opt_to_stdout && filename != NULL) {
      if(UNLINK(filename) != 0)
         warn_message(filename, "cannot unlink");
  }
  zclose(fd_out);
  zfree_all(p_mem);
  return;
}
   #endif
#endif

