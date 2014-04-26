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

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <ipps.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#include <ippcore.h>
#include <ippdc.h>

#include "ipp_gzip.h"

#define CODE_LEN 2048
#define MINSIZE(a, b) ((a<b)? a: b)

static const unsigned short order[19] = {
   16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

static const UBYTE rev_code_table[256] = {
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

typedef struct {
   UBYTE op;           /* operation, extra bits, table bits */
   UBYTE bits;         /* bits in this part of the code */
   unsigned short val;         /* offset in table or code value */
} table_codes;

struct inf_state {
   unsigned int   buf_bits;
   unsigned int   buf_len;
   UBYTE          *in_buf;
   UBYTE          *out_buf;
   UBYTE          *in_ptr;
   UBYTE          *out_ptr;
   char           *file_name;
   unsigned int   in_len;
   unsigned int   out_len;
   unsigned int   total_out;
   unsigned int   crc;
   int            mode;
   int            final;
};

static int stored_block(struct inf_state* state, table_codes* codes, GZIP_FILE *fd_in, GZIP_FILE *fd_out) {
   unsigned int len, nlen;

   if(state->in_len < 4) {
      ippsMove_8u(state->in_ptr, state->in_buf, state->in_len);
      state->in_len += (unsigned int)zread(fd_in, state->in_buf + state->in_len, INBUF_SIZE - state->in_len);
      state->in_ptr = state->in_buf;
   }

   NEEDBITS(16);
   len = state->buf_bits;
   DROPBITS(16);
   NEEDBITS(16);
   nlen = state->buf_bits;
   DROPBITS(16);

   if(len != (nlen^0xffff)) {
      warn_message(state->file_name, "stored block length mismatch");
      ret_status = STATUS_ERR;
      return ST_ERROR;
   }

   if(len != 0) {
      if(state->out_len < OUTBUF_SIZE) {
         state->total_out += (unsigned int)OUTBUF_SIZE - state->out_len;
         ippsCRC32_8u(state->out_buf, (int)(OUTBUF_SIZE - state->out_len), &state->crc);
         if(!opt_test) {
            zwrite(fd_out, state->out_buf, OUTBUF_SIZE - state->out_len);
         }
         if((OUTBUF_SIZE - state->out_len) >= WIN_SIZE) {
            ippsCopy_8u(state->out_buf + (OUTBUF_SIZE - state->out_len) - WIN_SIZE,
                        (Ipp8u*)(((IppInflateState*)codes)->pWindow), WIN_SIZE);
         } else {
            ippsMove_8u(((IppInflateState*)codes)->pWindow + (OUTBUF_SIZE - state->out_len),
                        (Ipp8u*)(((IppInflateState*)codes)->pWindow), WIN_SIZE - ((unsigned int)OUTBUF_SIZE - state->out_len));
            ippsCopy_8u(state->out_buf,
                        (Ipp8u*)(((IppInflateState*)codes)->pWindow + WIN_SIZE - ((unsigned int)OUTBUF_SIZE - state->out_len)),
                        (unsigned int)OUTBUF_SIZE - state->out_len);
         }
         state->out_ptr = state->out_buf;
         state->out_len = (unsigned int)OUTBUF_SIZE;
      }

      if(state->in_len < len) {
         ippsMove_8u(state->in_ptr, state->in_buf, state->in_len);
         state->in_len += (unsigned int)zread(fd_in, state->in_buf + state->in_len, (unsigned int)INBUF_SIZE - state->in_len);
         state->in_ptr = state->in_buf;
      }

      state->total_out += len;
      ippsCRC32_8u(state->in_ptr, (int)len, &state->crc);
      if(!opt_test) {
         zwrite(fd_out, state->in_ptr, len);
      }
      state->in_len -= len;
      if(len >= ((IppInflateState*)codes)->winSize) {
         ippsCopy_8u(state->in_ptr + len - WIN_SIZE, (Ipp8u*)(((IppInflateState*)codes)->pWindow), WIN_SIZE);
      } else {
         ippsMove_8u(((IppInflateState*)codes)->pWindow + len, (Ipp8u*)(((IppInflateState*)codes)->pWindow), WIN_SIZE - len);
         ippsCopy_8u(state->in_ptr, (Ipp8u*)(((IppInflateState*)codes)->pWindow + WIN_SIZE - len), len);
      }
      state->in_ptr += len;
   }

   if(state->final == 1)
      return ST_STREAM_END;
   ret_status = STATUS_OK;
   return ST_OK;
}
static int huff_block(struct inf_state* state, table_codes* codes, GZIP_FILE *fd_in, GZIP_FILE *fd_out) {
   int status;
   Ipp16u clens[19], code_lens[320];
   int clens_freq[8], clens_bnd[8], srv_code[128], srv_len[128];
   int hlit, hdist, hclen, len, step, ctmp, ltmp, i, k;

   if(state->mode == DYNAMIC_BLOCK) {
      if(state->in_len < 1000) {
         ippsMove_8u(state->in_ptr, state->in_buf, state->in_len);
         state->in_len += (unsigned int)zread(fd_in, state->in_buf + state->in_len, (unsigned int)INBUF_SIZE - state->in_len);
         state->in_ptr = state->in_buf;
      }

      clens_freq[0] = clens_freq[1] = clens_freq[2] = clens_freq[3] =
                                                      clens_freq[4] = clens_freq[5] = clens_freq[6] = clens_freq[7] = 0;

      NEEDBITS(14);
      hlit = (state->buf_bits & 0x1f) + 257;
      hdist = ((state->buf_bits >> 5) & 0x1f) + 1;
      hclen = ((state->buf_bits >> 10) & 0x0f) + 4;
      DROPBITS(14);

      for(i=0; i<hclen; i++) {
         NEEDBITS(3);
         clens[order[i]] = (Ipp16u)(state->buf_bits & 0x7);
         clens_freq[state->buf_bits & 0x7]++;
         DROPBITS(3);
      }
      for(i=hclen; i<19; i++)
         clens[order[i]] = 0;

      clens_bnd[0] = clens_bnd[1] = 0;
      clens_bnd[2] = clens_freq[1] << 1;
      for(i=3; i<8; i++ ) {
         clens_bnd[i] = (clens_bnd[i - 1] + clens_freq[i - 1]) << 1;
      }
      for(i=0; i<19; i++) {
         len = clens[i];
         if(len) {
            step = 1 << len;
            k = rev_code_table[clens_bnd[len]++] >> (8 - len);
            for( ; k < 128; k += step ) {
               srv_code[k] = i;
               srv_len[k] = len;
            }
         }
      }
      i=0;
      while(i<(hlit + hdist)) {
         NEEDBITS(14);
         ctmp = srv_code[state->buf_bits & 0x7f];
         ltmp = srv_len[state->buf_bits & 0x7f];
         DROPBITS(ltmp);
         if(ctmp < 16)
            code_lens[i++] = (Ipp16u)ctmp;
         else {
            if(16 == ctmp) {
               ctmp = code_lens[i - 1];
               ltmp = (state->buf_bits & 3) + 3;
               DROPBITS(2);
            } else if(17 == ctmp) {
               ctmp = 0;
               ltmp = (state->buf_bits & 7) + 3;
               DROPBITS(3);
            } else {
               ctmp = 0;
               ltmp = (state->buf_bits & 0x7f) + 11;
               DROPBITS(7);
            }
            while(ltmp--)
               code_lens[i++] = (Ipp16u)ctmp;
         }
      }
      if(i != (hlit + hdist)) {
         warn_message(state->file_name, "lenth/distance codes mismatch");
         ret_status = STATUS_ERR;
         return ST_ERROR;
      }

      status = ippsInflateBuildHuffTable(code_lens, hlit, hdist, (IppInflateState*)codes);
      if(status != ST_OK) {
         ret_status = STATUS_ERR;
         return status;
      }
   }
   if(state->mode == STATIC_BLOCK || state->mode == DYNAMIC_BLOCK) {
      i = (int)ippLEN;
      do { /* LZ77 decoding cycle */
         status = ippsInflate_8u(&state->in_ptr, &state->in_len, &state->buf_bits, &state->buf_len, 0,
                                 &state->out_ptr, &state->out_len, (unsigned int)OUTBUF_SIZE - state->out_len, (IppInflateMode*)&i, (IppInflateState*)codes);
         if(status != ST_OK) {
            ret_status = STATUS_ERR;
            return status;
         }

         if(state->in_len == 0) {
            state->in_len = (unsigned int)zread(fd_in, state->in_buf, (unsigned int)INBUF_SIZE);
            state->in_ptr = state->in_buf;
         }
         if(state->out_len == 0) {
            state->total_out += (unsigned int)OUTBUF_SIZE;
            ippsCRC32_8u(state->out_buf, (int)OUTBUF_SIZE, &state->crc);
            if(!opt_test) {
               zwrite(fd_out, state->out_buf, OUTBUF_SIZE);
            }
            ippsCopy_8u(state->out_buf + (unsigned int)OUTBUF_SIZE - ((IppInflateState*)codes)->winSize, (Ipp8u*)((IppInflateState*)codes)->pWindow, ((IppInflateState*)codes)->winSize);
            state->out_ptr = state->out_buf;
            state->out_len = (unsigned int)OUTBUF_SIZE;
         }
      } while(i != ippTYPE);
   }
   ret_status = STATUS_OK;
   if(state->final == 1)
      return ST_STREAM_END;
   return ST_OK;
}
static int process(struct inf_state* state, GZIP_FILE *fd_in, GZIP_FILE *fd_out) {
   unsigned int out_cnt;
   int status = ST_OK;
   MEM_ENTRY *p_mem;
   table_codes *codes;

   p_mem = zalloc_mem_head();
   codes = (table_codes*)zalloc_mem(CODE_LEN*sizeof(table_codes), p_mem);
   ((IppInflateState*)codes)->pWindow = zalloc_mem(WIN_SIZE, p_mem);
   ((IppInflateState*)codes)->winSize = WIN_SIZE;
   ((IppInflateState*)codes)->tableBufferSize = 2048;
   state->buf_bits = 0;
   state->buf_len = 0;
   state->total_out = 0;
   state->crc = 0;
#if defined(_WIN32)
#pragma warning(disable:4127)
#endif
   while(1) {
      NEEDBITS(3);
      if(state->in_len < 3) {
         ippsMove_8u(state->in_ptr, state->in_buf, state->in_len);
         state->in_len += (unsigned int)zread(fd_in, state->in_buf + state->in_len, (unsigned int)INBUF_SIZE - state->in_len);
         state->in_ptr = state->in_buf;
      }
      state->final = (state->buf_bits & 1);

      switch(((state->buf_bits >> 1) & 3)) {
      case 0:
         state->mode = STORED_BLOCK;
         break;
      case 1:
         state->mode = STATIC_BLOCK;
         ((IppInflateState*)codes)->tableType = 0;
         break;
      case 2:
         state->mode = DYNAMIC_BLOCK;
         break;
      default:
         state->mode = UNKNOWN_BLOCK;
         warn_message(state->file_name, "unknown deflate method");
         status = ST_ERROR;
         ret_status = STATUS_ERR;
         break;
      }
      DROPBITS(3);

      if(status >= ST_OK) {
          if(state->mode == STORED_BLOCK) {
              BYTEBITS();
              status = stored_block(state, codes, fd_in, fd_out);
          }
         else if(state->mode == STATIC_BLOCK || state->mode == DYNAMIC_BLOCK)
            status = huff_block(state, codes, fd_in, fd_out);
      }
      if(status == ST_STREAM_END) {
         status = ST_OK;
         out_cnt = (unsigned int)OUTBUF_SIZE - state->out_len;
         state->total_out += out_cnt;
         state->out_ptr = state->out_buf;
         ippsCRC32_8u(state->out_buf, out_cnt, &state->crc);
         if(!opt_test)
            zwrite(fd_out, state->out_buf, out_cnt);
         break;
      } else if(status == ST_ERROR) {
         ret_status = STATUS_ERR;
         break;
      }
   }
   zfree_all(p_mem);
   return status;
}

void inflate_st(char *filename){
   GZIP_FILE      *fd_in, *fd_out;
   Ipp64u         start_clocks = 0;
   MEM_ENTRY      *p_mem;
   char           flag_byte, method, *orig_fname, *basename;
   UBYTE          *comment_field, *ptr;
   unsigned int   incnt;
   SIZE_TYPE      uncompressed_size = 0;
   long           timestamp = 0;
   int            status = ST_OK, fd_out_flag = 0;
   struct inf_state* state;
   char           stdin_name[] = "stdin";
   int            ipp_gzip_style = 0; /* Not ipp_gzip archive by default */
   short          save_write_method = opt_io_write_method;
   int            temp_resname = 0;

   p_mem = zalloc_mem_head();
   state = (struct inf_state*)zalloc_mem(sizeof(struct inf_state), p_mem);
   state->in_buf = state->in_ptr = (UBYTE*)zalloc_mem(INBUF_SIZE, p_mem);
   state->out_buf = state->out_buf = (UBYTE*)zalloc_mem(OUTBUF_SIZE, p_mem);

   /* DEBUG */
   state->total_out = 0L;
   state->crc = 0L;
   /* Open input file */
   if(from_stdin) {      /* Inflating stdin */
      filename = stdin_name;
      fd_in = zopen_stdin();
   } else {
      if((fd_in = ZOPEN(filename, GZIP_FILE_READ, opt_io_read_method, 0)) == NULL) {
         error_file(filename);
         zfree_all(p_mem);
         ret_status = STATUS_ERR;
         return;
      }
   }
   /* Read archive header */
   incnt = (unsigned int)zread(fd_in, state->in_buf, (unsigned int)INBUF_SIZE);
   while(1) {
      if(incnt == -1) {
         status = ST_ERROR;
         error_file(filename);
         ret_status = STATUS_ERR;
         break;
      }
      if(state->in_buf[0] != GZIP_HDR_1 || state->in_buf[1] != GZIP_HDR_2) {
         status = ST_ERROR;
         warn_message(filename, "not in gzip format");
         ret_status = STATUS_ERR;
         break;
      }
      method = state->in_buf[2];
      flag_byte = state->in_buf[3];
      if(method != DEFLATE) {
         status = ST_ERROR;
         warn_message(filename, "unsupported compress method");
         ret_status = STATUS_ERR;
         break;
      }
      state->in_ptr = &state->in_buf[4];
      timestamp = GET_LONG(&state->in_buf[4]);
      state->in_ptr += 6;
      if((flag_byte & FNAME) && *state->in_ptr) {        /* If gzip flag says, that original filename exists */
         orig_fname = (char*)zalloc_mem(strlen((const char*)state->in_ptr)+1, p_mem);
         ptr = (UBYTE *)orig_fname;
         while(*ptr++ = GET_BYTE(state->in_ptr)); /* Copy original filename */
      } else {
         orig_fname = NULL;
      }
      if(opt_list) {
         status = ST_STREAM_END;
         show_list(filename, orig_fname, fd_in, timestamp);
         break;
      }
      /* Look for comment field */
      if((flag_byte & FCOMMENT) && *state->in_ptr) {
         comment_field = (UBYTE*)zalloc_mem(strlen((const char*)state->in_ptr)+1, p_mem);
         ptr = comment_field;
         while(*ptr++ = GET_BYTE(state->in_ptr)); /* Copy comment */
      }
      else
         comment_field = NULL;
      /* Check if our archive have ipp_gzip-style comment */
      if(comment_field)
         ipp_gzip_style = check_ipp_gzip_comment(comment_field);
      else
         ipp_gzip_style = 0;
      if(opt_perf_stat)
         start_clocks = ippGetCpuClocks();

      /* Create output file */
      if(!opt_to_stdout) {
         if(!opt_name || (flag_byte & FNAME)==0) {       /* Use don't want to restore original name */
            basename = (char*)zalloc_mem(strlen((const char*)filename)+6, p_mem);
            if(!get_nosuf_name(filename, basename)) {
              strcat(basename, ".gtmp");
              temp_resname = 1;
            }
         } else {
            size_t str_size;
            str_size = strlen((const char*)filename);
            basename = (char*)zalloc_mem(str_size+strlen(orig_fname)+1, p_mem);
            restore_output_name(filename, orig_fname, basename);
         }
      }
      if(!opt_test) {
         if(!opt_to_stdout) {
            if(!ipp_gzip_style) { /* We cannot use file mapping, since we don't know REAL uncompressed size. GZIP
                                  keeps only 2^32 remainder of it */
               opt_io_write_method = METHOD_LOW;
               uncompressed_size = 0;
            } else { /* We will read uncompressed size from comment field */
#if defined(VS2005_PLUS)
               if(1 != sscanf_s((const char*)comment_field+1, OFFSET_FORMAT, &uncompressed_size)) {
#else
               if(1 != sscanf((const char*)comment_field+1, OFFSET_FORMAT, &uncompressed_size)) {
#endif
                  opt_io_write_method = METHOD_LOW; /* Couldn't read uncompressed size */
                  uncompressed_size = 0;
               }
            }
            if((fd_out = out_open(basename, uncompressed_size)) == NULL) {
               status = ST_ERROR;
               error_file(filename);
               ret_status = STATUS_ERR;
               break;
            }
            fd_out_flag = 1;
            zset_uncompressed_size(fd_out, uncompressed_size);
         } else
            fd_out = zopen_stdout();
      }
      break;
   }
   if(status != ST_OK) {
      if(fd_out_flag)
         zclose(fd_out);
      zclose(fd_in);
      zfree_all(p_mem);
      opt_io_write_method = save_write_method;
      return;
   }

   /* Inflate */
   state->out_ptr = state->out_buf;
   state->out_len = (unsigned int)OUTBUF_SIZE;
   state->in_len = incnt - (unsigned int)(state->in_ptr - state->in_buf);
   state->buf_bits = 0;
   state->buf_len = 0;
   state->file_name = filename;
   if(!opt_test)
      status = process(state, fd_in, fd_out);
   else
      status = process(state, fd_in, NULL);
   if(status == ST_ERROR) {
      if(!opt_test)
         zclose(fd_out);
      zclose(fd_in);
      zfree_all(p_mem);
      opt_io_write_method = save_write_method;
      ret_status = STATUS_ERR;
      return;
   }

   /* Check CRC */
   BYTEBITS();
   NEEDBITS(32);
   if(state->buf_bits != state->crc) {
       warn_message(filename, "crc error");
       status = ST_ERROR;
   }
   DROPBITS(16);
   DROPBITS(16);
   state->buf_bits = 0; /* NEEDBITS has SHR statement. SHR on 32 bits do nothing on some architectures */
   NEEDBITS(32);
   if(state->total_out != state->buf_bits) {  /* Check compressed file size */
      warn_message(filename, "length error");
      status = ST_ERROR;
   }
   if(status != ST_ERROR && opt_test && opt_verbose && state->in_len == 0)
      warn_message(filename, "OK");
   if(state->in_len > 0 && status != ST_ERROR)
      warn_message(filename, "decompression OK, trailing garbage ignored");

   /* Print decompression statistics */
   if(status != ST_ERROR && !opt_test && (opt_perf_stat || opt_verbose)) {
      if(zget_bytes_out(fd_out) != 0) {
         print_stat((float)(ippGetCpuClocks()-start_clocks)/zget_bytes_in(fd_in),
            (float)(100*(1.-(float)zget_bytes_in(fd_in)/zget_bytes_out(fd_out))),
                    filename, basename, 0);
      } else
         print_stat((float)(ippGetCpuClocks()-start_clocks)/zget_bytes_in(fd_in), 0,  filename, basename, 0);
   }
#if !defined(_WIN32)
   /* On Linux copy permission bits */
   if(!opt_test)
      zset_permbits(fd_out, zget_permbits(fd_in));
#endif
   zclose(fd_in);
   if(!opt_test)
      zclose(fd_out);
   if(!opt_notime && !opt_to_stdout)          /* Need to restore original file creation time. */
      zcopytime(filename, basename, timestamp);
   if(status != ST_ERROR && !opt_to_stdout && !from_stdin && !opt_test) {
      if(UNLINK(filename) != 0)
         warn_message(filename, "cannot unlink");
      if(temp_resname)
         rename(basename, filename);
   }
   zfree_all(p_mem);
   opt_io_write_method = save_write_method;
}

int check_ipp_gzip_comment(const UBYTE* p_comment)
{
   Ipp32u ccrc = 0;
   int cfl = (int)strlen((const char*)p_comment);
   UBYTE cxor;

   ippsCRC32_8u(p_comment, cfl-1, &ccrc);
   cxor = (UBYTE)((ccrc & 0xFF) ^ ((ccrc >> 8) & 0xFF) ^ ((ccrc >> 16) & 0xFF) ^ ((ccrc >> 24) & 0xFF));
   if(cxor == 0) cxor++;
   return cxor==*(p_comment+cfl-1);
}
