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
#if defined(GZIP_VMTHREADS) && !defined(GZIP_INFLATE_OLD)

   #include <stdio.h>
   #include <fcntl.h>
   #ifdef _WIN32
      #include <sys/stat.h>
      #include <windows.h>
      #include <io.h>
   #else
      #include <unistd.h>
   #endif
   #include <string.h>
   #include <stdlib.h>
   #include <time.h>

   #include <ippcore.h>
   #include <ippdc.h>
   #include <ipps.h>

   #include "vm_semaphore.h"

   #include "ipp_gzip.h"

static const unsigned short order[19] = {
   16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

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

static int extra_trail = 2;

typedef enum {
   error = -1,
   stored = 0,
   fixed = 1,
   dynamic = 2
} inf_mode;

typedef struct {
   UBYTE          op;           /* operation, extra bits, table bits */
   UBYTE          bits;         /* bits in this part of the code */
   unsigned short val;         /* offset in table or code value */
} table_codes;

struct inf_state_mt {
   unsigned int buf_bits;
   unsigned int buf_len;
   char* filename;
   UBYTE *in_buf;
   UBYTE *out_buf;
   UBYTE *in_ptr;
   UBYTE *out_ptr;
   SIZE_TYPE in_len;
   SIZE_TYPE out_len;
   SIZE_TYPE total_out;
   SIZE_TYPE in_size;
   SIZE_TYPE out_size;
   SIZE_TYPE in_left;
   inf_mode mode;
   unsigned int crc;
   int final;
};

static void chunk_warn_message(char *filename, int chunk, char *text) {
   char* conv_filename = char_to_oem(filename);
   print_progname();
   fprintf(stderr, "%s:   %s in chunk %d\n", conv_filename, text, chunk);
   char_to_oem_free(conv_filename);
}

/*! \brief Choose minimal of two values */
   #define MINSIZE(a, b) ((a<b)? a: b)

struct ic_params {
   int          chunk;
   int          max_chunk;
   Ipp32u       from_crc;
   Ipp32u       to_crc;
   OFFSET_TYPE  from_pos;
   OFFSET_TYPE  to_pos;
   size_t       in_buffer_size;
   size_t       out_buffer_size;
   char         *filename;
   UBYTE        *chunk0_inptr;
   UBYTE        *chunk0_inbuf;
   GZIP_FILE    *chunk0_fd_in;
   GZIP_FILE    *tmp_fd;
   int          *err_status;
   SIZE_TYPE    target_file_size;
   vm_semaphore *semaphore;
};

static void VM_THREAD_CALLCONVENTION inflate_chunk(void*);

struct allocatable_data {
   GZIP_FILE   *fd_in;
   UBYTE       *inbuf;
   char        *orig_fname;
   char        *result_fname;
   OFFSET_TYPE *offset_array;
   Ipp32u      *crc_array;
   UBYTE       *comment_field;
};

/**
 * Multi-threaded version of inflate function
 *
 * @param filename
 */

void inflate_mt(char *filename)
{
   MEM_ENTRY   *p_mem;
   size_t      incnt;
   UBYTE       flag_byte, method;
   UBYTE       *inptr;
   UBYTE       *ptr;
   size_t      comment_field_length;
   int         chunks;
   int         i;
   STAT_TYPE   file_info;
   long        timestamp = 0;
   size_t      insize, outsize;
   GZIP_FILE   *fd_out = NULL;
   GZIP_FILE   *fd_in;
   int         error_found;
   GZIP_FILE   **tmp_fd_vector = NULL;          /* Array of temporary files to be used in low-level i/o */
   Ipp64u      start_clocks;
   struct ic_params
                *vm_params;
   vm_thread   *thread_vec;
   SIZE_TYPE   target_file_size = 0;
   vm_semaphore *semaphore;
   UBYTE        *comment_field;
   Ipp32u       *crc_array;
   UBYTE        *inbuf;
   OFFSET_TYPE  *offset_array;
   char         *orig_fname, *result_fname;
   int           temp_resname = 0;

   if(vm_sys_info_get_cpu_num() == 1) {
      inflate_st(filename);        /* No alternates */
      return;
   }
   /* Get input file statistics */
   if(SYSSTAT(filename, &file_info) == -1) {
      warn_message(filename, "cannot get file statistics");
      error_file(filename);
      ret_status = STATUS_ERR;
      return;
   }
   /* Allocate and initialize data structure for pointers */
   comment_field = NULL;
   crc_array = NULL;
   inbuf = NULL;
   offset_array = NULL;
   orig_fname = NULL;
   result_fname = NULL;
   /* Open input file */
   if( (fd_in = ZOPEN(filename, GZIP_FILE_READ, opt_io_read_method, 0)) == NULL ) {
      warn_message(filename, "cannot open input file for sharing");
      error_file(filename);
      ret_status = STATUS_ERR;
      return;
   }
   insize = INBUF_SIZE;
   outsize = OUTBUF_SIZE;
   p_mem = zalloc_mem_head();
   inbuf = (UBYTE*)zalloc_mem(insize, p_mem);
   /* Read header, original filename, comment field */
   incnt = zread(fd_in, inbuf, insize);
   if( incnt == -1 ) {
      warn_message(filename, "cannot read header");
      error_file(filename);
      zclose(fd_in);
      ret_status = STATUS_ERR;
      return;
   }
   /* Check GZIP header */
   if( inbuf[0] != GZIP_HDR_1 || inbuf[1] != GZIP_HDR_2 ) {
      warn_message(filename, "not in gzip format");
      zfree_all(p_mem);
      ret_status = STATUS_ERR;
      return;
   }
   method = inbuf[2];
   if( method != DEFLATE ) {
      warn_message(filename, "unknown compress method");
      zfree_all(p_mem);
      ret_status = STATUS_ERR;
      return;
   }
   flag_byte = inbuf[3];
   /* Get time stamp and original filename if exists */
   inptr = inbuf + 4;
   timestamp = GET_LONG(inptr); inptr += 4;
   GET_BYTE(inptr);    /* Skip extra flags byte */
   GET_BYTE(inptr);    /* Skip OS flag byte */
   if( (flag_byte & FNAME) && *inptr ) {        /* If gzip flag says, that original filename exists */
      orig_fname = (char*)zalloc_mem(strlen((const char*)inptr)+1, p_mem);
      ptr = (UBYTE *)orig_fname;
      while( (*ptr++ = GET_BYTE(inptr))); /* Copy original filename */
   }
   if(opt_list) {         /* We want just to show archive content */
      show_list(filename, orig_fname, fd_in, timestamp);
      zclose(fd_in);
      zfree_all(p_mem);
      return;
   }
   /* Look for comment field */
   comment_field_length = 0;
   if( (flag_byte & FCOMMENT) && *inptr ) {
      comment_field_length = strlen((const char*)inptr);
      comment_field = (UBYTE *)zalloc_mem(comment_field_length+1, p_mem);
      ptr = comment_field;
      while( (*ptr++ = GET_BYTE(inptr)) ) /* Copy comment */
         ;
   }
   if( (flag_byte & FCOMMENT) == 0                 /* If no comment exists */
       || (*(comment_field)) != COMMENT_MAGIC_HEADER ) {   /* or it is not a multi-chunk comment */
      zclose(fd_in);
      zfree_all(p_mem);
      inflate_st(filename);
      return;
   }
   /* Additionally check comment field with CRC */
   if (!check_ipp_gzip_comment(comment_field)) { /* Not a IPP_GZIP comment */
      zclose(fd_in);
      zfree_all(p_mem);
      inflate_st(filename);
      return;
   }
   if(opt_perf_stat)
      start_clocks = ippGetCpuClocks();
   /* Comment field is OK for multi-threading inflate */
   /* Get uncompressed file length */
   SYSSCANF((const char*)comment_field+1, OFFSET_FORMAT, &target_file_size);
   SYSSCANF((const char*)comment_field+1+COMMENT_OFFSET_FRM_SZ, "%1X", &chunks);
   if(comment_field_length != (size_t)(COMMENT_SIZE(chunks)-1)) {          /* Suspicious comment field */
      zclose(fd_in);
      zfree_all(p_mem);
      inflate_st(filename);
      return;
   }
   /* Restore chunk offset and chunk CRC arrays */
   offset_array = (OFFSET_TYPE *)zalloc_mem(sizeof(OFFSET_TYPE)*(chunks-1), p_mem);
   crc_array = (Ipp32u *)zalloc_mem(sizeof(long)*(chunks-1), p_mem);
   ptr = comment_field+2+COMMENT_OFFSET_FRM_SZ;
   for( i=0; i<chunks-1; i++ ) {
      if(1 != SYSSCANF((const char*)ptr, OFFSET_FORMAT, offset_array+i)) { /* cannot read */
         zclose(fd_in);
         zfree_all(p_mem);
         inflate_st(filename);
         ret_status = STATUS_ERR;
         return;
      }
      ptr += COMMENT_OFFSET_FRM_SZ;
   }
   for( i=0; i<chunks-1; i++ ) {
      if(1 != SYSSCANF((const char*)ptr, CRC_FORMAT, crc_array+i)) {
         zclose(fd_in);
         zfree_all(p_mem);
         inflate_st(filename);
         ret_status = STATUS_ERR;
         return;
      }
      ptr += COMMENT_CRC_FRM_SZ;
   }
   if(!opt_test) {
      /* Create output file */
      if(!opt_name || (flag_byte & FNAME)==0) {       /* Use don't want to restore original name */
         result_fname = (char*)zalloc_mem(strlen((const char*)filename)+6, p_mem);
         if(!get_nosuf_name(filename, result_fname)) {
            strcat(result_fname, ".gtmp");
            temp_resname = 1;
         }
      } else {
         size_t str_size;
         str_size = strlen((const char*)filename);
         result_fname = (char*)zalloc_mem(str_size+strlen(orig_fname)+1, p_mem);
         restore_output_name(filename, orig_fname, result_fname);
      }
      if(!opt_to_stdout) {
         if( (fd_out = out_open(result_fname, target_file_size)) == NULL ) {
            warn_message(filename, "cannot open output file");
            error_file(result_fname);
            zclose(fd_in);
            zfree_all(p_mem);
            inflate_st(filename);
            ret_status = STATUS_ERR;
            return;
         }
#if !defined(_WIN32)
         if(!opt_test)
            zset_permbits(fd_out, file_info.st_mode);
#endif
      } else
         fd_out = zopen_stdout();
      if(opt_io_write_method == METHOD_LOW)
         tmp_fd_vector = (GZIP_FILE**)zalloc_mem(sizeof(GZIP_FILE*) * chunks, p_mem);
   }
   error_found = 0;

   /* Here we need to use correct number of threads. Number of threads created must not be greater than number of processors available */
   vm_params = (struct ic_params*)zalloc_mem(chunks * sizeof(struct ic_params), p_mem);
   thread_vec = (vm_thread*)zalloc_mem(chunks * sizeof(vm_thread), p_mem);
   semaphore = (vm_semaphore*)zalloc_mem(sizeof(vm_semaphore), p_mem);
   vm_semaphore_set_invalid(semaphore);
   if(opt_dbg_multithreads) {
      int dbg_multithreads = atoi(opt_num_threads_forced);
      if(dbg_multithreads > chunks) {
         warn_message(filename, "too many threads declarated; limited to amount of chunks");
         vm_semaphore_init(semaphore, chunks);
      } else if(dbg_multithreads < 1)
         vm_semaphore_init(semaphore, vm_sys_info_get_cpu_num());
      else
         vm_semaphore_init(semaphore, dbg_multithreads);
   } else
      vm_semaphore_init(semaphore, vm_sys_info_get_cpu_num());

   for(i=0; i<chunks; i++) {
      if( i == 0 ) {                   /* If initial chunk */
         vm_params[i].from_crc = 0L;
         vm_params[i].to_crc = crc_array[0];
         vm_params[i].from_pos = (long)(inptr-inbuf);
         vm_params[i].to_pos = offset_array[0];
      } else if( i == chunks-1 ) {     /* If last chunk */
         vm_params[i].from_crc = crc_array[i-1];
         vm_params[i].to_crc = 0L;
         vm_params[i].from_pos = offset_array[i-1];
         vm_params[i].to_pos = file_info.st_size;
      } else {
         vm_params[i].from_crc = crc_array[i-1];
         vm_params[i].to_crc = crc_array[i];
         vm_params[i].from_pos = offset_array[i-1];
         vm_params[i].to_pos = offset_array[i];
      }
      if(opt_io_write_method == METHOD_LOW && !opt_test) {
         if(i > 0) {
            char tmp_filename[MAX_NAME];
            if( (tmp_fd_vector[i] = ZOPEN(mk_chunk_name(filename, i, tmp_filename), GZIP_FILE_WRITE,
                        opt_io_write_method, target_file_size)) == NULL ) {
               warn_message(filename, "cannot create temporary file");
               zclose(fd_in);
               zfree_all(p_mem);
               ret_status = STATUS_ERR;
               return;
            }
         } else {
            tmp_fd_vector[i] = fd_out;
         }
         vm_params[i].tmp_fd = tmp_fd_vector[i];
      }
      vm_params[i].in_buffer_size = insize;
      vm_params[i].out_buffer_size = outsize;
      vm_params[i].filename = filename;
      vm_params[i].chunk0_inptr = inptr;
      vm_params[i].chunk0_inbuf = inbuf;
      vm_params[i].chunk0_fd_in = fd_in;
      vm_params[i].err_status = &error_found;
      vm_params[i].chunk = i;
      vm_params[i].max_chunk = chunks;
      vm_params[i].target_file_size = target_file_size;
      vm_params[i].semaphore = semaphore;
      if(opt_io_write_method == METHOD_MMAP && !opt_test)
         vm_params[i].tmp_fd = fd_out;  /* In MMAP we write to the same output file in parallel */

      vm_semaphore_wait(semaphore);
      vm_thread_set_invalid(&thread_vec[i]);
      vm_thread_create(&thread_vec[i], (vm_thread_callback)inflate_chunk, &vm_params[i]);
   }

   for(i=0; i<chunks; i++) {
      vm_thread_wait(&thread_vec[i]);
      vm_thread_close(&thread_vec[i]);
   }
   vm_semaphore_destroy(semaphore);

   if(opt_io_write_method == METHOD_LOW) {
      /* Allocate lengthy buffer for tmp file re-read */
      inbuf = (UBYTE*)zalloc_mem(INBUF_SIZE, p_mem);
      insize = INBUF_SIZE;
   }
   if(!opt_test) {
      if(opt_io_write_method == METHOD_LOW) {
         for(i=1; i<chunks; i++) {
            if( zseek(tmp_fd_vector[i], (OFFSET_TYPE)0, 0)) {
               warn_message(filename, "cannot rewind temporary file"); abort();
            }
            while( !zeof(tmp_fd_vector[i]) ) {
               incnt = zread(tmp_fd_vector[i], inbuf, insize);
               zwrite(fd_out, inbuf, incnt);
            }
            zclose(tmp_fd_vector[i]);
            remove_tmp_file(filename, i);
         }
      } /* opt_io_write_method == METHOD_LOW*/
   } /* !opt_test */
   /* Free all */
   if(!opt_notime && !opt_test)          /* Need to restore original file creation time */
      zcopytime(filename, result_fname, timestamp);
   if(!error_found && opt_test && opt_verbose) {
      warn_message(filename, "OK");
      ret_status = STATUS_OK;
   }

   /* Print decompression statistics */
   if(!error_found && !opt_test && (opt_perf_stat || opt_verbose)) {
      print_stat((float)((ippGetCpuClocks()-start_clocks)/fd_in->file_size),
          (float)(100*(1.-(float)fd_in->file_size/zget_bytes_out(fd_out))), filename, result_fname, 1);
   }
   if(!opt_test) {
      zclose(fd_out);
   }
   zclose(fd_in);
   if(!opt_to_stdout && filename != NULL && !opt_test && error_found!=1 ) {
       if(UNLINK(filename) != 0) {
         warn_message(filename, "cannot unlink");
         ret_status = STATUS_WARN;
       }
       if(temp_resname)
         rename(result_fname, filename);
   }
   zfree_all(p_mem);
}

static int block_type(struct inf_state_mt* state, table_codes* codes, GZIP_FILE *fd_in)
{
   int status = ST_OK;
   unsigned int d_len;
   SIZE_TYPE read_size;

#if defined(_WIN32)
#pragma warning(disable:4127)
#endif
   while(1) {
      if(state->in_len < 3) {
         if(state->in_left == 0)
            return ST_ERROR;
         read_size = MINSIZE(state->in_size - state->in_len, state->in_left);
         ippsMove_8u(state->in_ptr, state->in_buf, (int)state->in_len);
         d_len = (unsigned int)zread(fd_in, state->in_buf + state->in_len, (size_t)read_size);
         state->in_len += d_len;
         state->in_ptr = state->in_buf;
         state->in_left -= d_len;
      }
      NEEDBITS(3);
      state->final = (state->buf_bits & 1);

      switch(((state->buf_bits >> 1) & 3)) {
      case 0:
         state->mode = stored;
         break;
      case 1:
         state->mode = fixed;
         ((IppInflateState*)codes)->tableType = 0;
         break;
      case 2:
         state->mode = dynamic;
         break;
      default:
         state->mode = error;
         status = ST_ERROR;
         break;
      }
      DROPBITS(3);
      break;
   }
   return status;
}

/**
 * Stored block reader
 */
static int stored_block(struct inf_state_mt* state, table_codes* codes, GZIP_FILE *fd_in, GZIP_FILE *tmp_fd,
                        GZIP_MMAP_HANDLE *p_view)
{
   unsigned int len, nlen, d_len, read_size;

   if((state->in_len + (state->buf_len / 8)) < 4) { /* If not enough input data to get stored block header */
      if(state->in_left == 0)
         return ST_ERROR;
      read_size = (unsigned int)MINSIZE(state->in_size - state->in_len, state->in_left);
      ippsMove_8u(state->in_ptr, state->in_buf, (int)state->in_len);
      d_len = (unsigned int)zread(fd_in, state->in_buf + state->in_len, read_size);
      state->in_len += d_len;
      state->in_ptr = state->in_buf;
      state->in_left -= d_len;
   }

   NEEDBITS(16);
   len = state->buf_bits;
   DROPBITS(16);
   NEEDBITS(16);
   nlen = state->buf_bits;
   DROPBITS(16);

   if(len != (nlen^0xffff)) {
      warn_message(state->filename, "store block length mismatch");
      return ST_ERROR;
   }

   if(len != 0) {
      if(state->out_len < state->out_size) {
         ippsCRC32_8u(state->out_buf, (int)(state->out_size - state->out_len), &state->crc);
         if(!opt_test) {
            size_t write_size = (size_t)(state->out_size - state->out_len);
            if(opt_io_write_method == METHOD_MMAP) {
//               if(zwriteview(p_view, state->out_buf, state->out_size - state->out_len) != (state->out_size - state->out_len)) {
               if(zwrite_dup_mmap(p_view, state->out_buf, write_size) != write_size) {
               return ST_ERROR;
         }
            } else {    /* METHOD == LOW */
               if(zwrite(tmp_fd, state->out_buf, write_size) != write_size) {
                  return ST_ERROR;
               }
            }
         }
         if((state->out_size - state->out_len) >= ((IppInflateState*)codes)->winSize) {
            ippsCopy_8u(state->out_buf + (state->out_size - state->out_len) - ((IppInflateState*)codes)->winSize,
                        (Ipp8u*)(((IppInflateState*)codes)->pWindow), ((IppInflateState*)codes)->winSize);
         } else {
            ippsMove_8u(((IppInflateState*)codes)->pWindow + (state->out_size - state->out_len),
                        (Ipp8u*)(((IppInflateState*)codes)->pWindow), (int)(((IppInflateState*)codes)->winSize - (state->out_size - state->out_len)));
            ippsCopy_8u(state->out_buf,
                        (Ipp8u*)(((IppInflateState*)codes)->pWindow + ((IppInflateState*)codes)->winSize - (state->out_size - state->out_len)),
                        (int)(state->out_size - state->out_len));
         }
         state->out_ptr = state->out_buf;
         state->out_len = state->out_size;
      }

      if(state->in_len < len) {
         if(state->in_left == 0)
            return ST_ERROR;
         read_size = (unsigned int)MINSIZE(state->in_size - state->in_len, state->in_left);
         ippsMove_8u(state->in_ptr, state->in_buf, (int)state->in_len);
         d_len = (unsigned int)zread(fd_in, state->in_buf + state->in_len, read_size);
         state->in_len += d_len;
         state->in_ptr = state->in_buf;
         state->in_left -= d_len;
      }

      ippsCRC32_8u(state->in_ptr, (int)len, &state->crc);
      if(!opt_test) {
         if(opt_io_write_method == METHOD_MMAP) {
            if(zwrite_dup_mmap(p_view, state->in_ptr, len) != len) {
               return ST_ERROR;
      }
         } else {    /* METHOD == LOW */
            if(zwrite(tmp_fd, state->in_ptr, len) != len) {
               return ST_ERROR;
            }
         }
      }
      if(len >= ((IppInflateState*)codes)->winSize) {
         ippsCopy_8u(state->in_ptr + len - ((IppInflateState*)codes)->winSize,
                     (Ipp8u*)(((IppInflateState*)codes)->pWindow), ((IppInflateState*)codes)->winSize);
      } else {
         ippsMove_8u(((IppInflateState*)codes)->pWindow + len,
                     (Ipp8u*)(((IppInflateState*)codes)->pWindow), ((IppInflateState*)codes)->winSize - len);
         ippsCopy_8u(state->in_ptr,
                     (Ipp8u*)(((IppInflateState*)codes)->pWindow + ((IppInflateState*)codes)->winSize - len), len);
      }
      state->in_len -= len;
      state->in_ptr += len;
   }

   if(state->final == 1)
      return ST_STREAM_END;
   return ST_OK;
}

/**
 * Huffman decoder
 */

static int huff_block(struct inf_state_mt* state, table_codes* codes, GZIP_FILE *fd_in, GZIP_FILE *tmp_fd,
                      GZIP_MMAP_HANDLE *p_view)
{
   int status;
   unsigned short clens[19], code_lens[320];
   int clens_freq[8], clens_bnd[8], srv_code[128], srv_len[128];
   int hlit, hdist, hclen, len, step, ctmp, ltmp, i, k;
   unsigned int read_size, d_len;


   if(state->mode == dynamic) {
      if(state->in_len < 1000 && state->in_left > 0) {
         if(state->in_left == 0)
            return ST_ERROR;
         read_size = (unsigned int)MINSIZE(state->in_size - state->in_len, state->in_left);
         ippsMove_8u(state->in_ptr, state->in_buf, (int)state->in_len);
         d_len = (unsigned int)zread(fd_in, state->in_buf + state->in_len, read_size);
         state->in_len += d_len;
         state->in_ptr = state->in_buf;
         state->in_left -= d_len;
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
         clens[order[i]] = (unsigned short)(state->buf_bits & 0x7);
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
            code_lens[i++] = (unsigned short)ctmp;
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
               code_lens[i++] = (unsigned short)ctmp;
         }
      }
      if(i < (hlit + hdist)) {
         warn_message(state->filename, "too few lenth/distance codes");
         return 0;
      }
      if(i > (hlit + hdist)) {
         warn_message(state->filename, "too much lenth/distance codes");
         return 0;
      }

      status = ippsInflateBuildHuffTable(code_lens, hlit, hdist, (IppInflateState*)codes);
      if(status!=ippStsNoErr)
         return status;
   }
   if(state->mode == fixed || state->mode == dynamic) {
      i = (int)ippLEN;
      do { /* LZ77 decoding cycle */
          unsigned int in_len = (unsigned int)state->in_len;
          unsigned int out_len = (unsigned int)state->out_len;
         status = ippsInflate_8u(&state->in_ptr, &in_len, &state->buf_bits, &state->buf_len,
                     0, &state->out_ptr, &out_len, (unsigned int)(state->out_size - state->out_len),
                     (IppInflateMode*)&i, (IppInflateState*)codes);
            state->in_len = (SIZE_TYPE)in_len;
            state->out_len = (SIZE_TYPE)out_len;
         if(status != ippStsNoErr)
            return status;

         if(state->in_len == 0) {
            if(state->in_left == 0)
               break;
            read_size = (unsigned int)MINSIZE(state->in_size, state->in_left);
            state->in_len = (unsigned int)zread(fd_in, state->in_buf, read_size);
            state->in_ptr = state->in_buf;
            state->in_left -= state->in_len;
         }
         if(state->out_len == 0) {
            ippsCRC32_8u(state->out_buf, (int)state->out_size, &state->crc);
            if(!opt_test) {
               if(opt_io_write_method == METHOD_MMAP) {
                  if(zwrite_dup_mmap(p_view, state->out_buf, (size_t)state->out_size) != (size_t)state->out_size) {
                     status = ST_ERROR;
                     break;
                  }
               } else {    /* METHOD == LOW */
                  if(zwrite(tmp_fd, state->out_buf, (size_t)state->out_size) != (size_t)state->out_size) {
                  status = ST_ERROR;
                  break;
               }
            }
            }
            ippsCopy_8u(state->out_buf + state->out_size - ((IppInflateState*)codes)->winSize,
                        (Ipp8u*)(((IppInflateState*)codes)->pWindow), ((IppInflateState*)codes)->winSize);
            state->out_ptr = state->out_buf;
            state->out_len = state->out_size;
         }
      } while(i != ippTYPE);
   }
   if(state->final == 1)
      return ST_STREAM_END;
   return ST_OK;
}

/**
 * uncompress chunk in parallel thread
 * @param chunk - current chunk number
 * @param from_crc - initial CRC32 value
 * @param to_crc - final CRC32 value
 * @param from_pos - initial file position of compressed chunk
 * @param to_pos - final file position
 * @param in_buffer_size - input buffer size
 * @param out_buffer_size - output buffer size
 * @param filename - input file name
 * @param chunk0_inptr - current ptr in outer buffer to be used
 *                     in chunk #0
 * @param chunk0_inbuf - input outer buffer to be used in chunk
 *                     #0
 * @param chunk0_fd_in - input file descriptor to be used in
 *                     chunk #0
 *
 * @return int - 1 (success) or 0 (failure)
 */
static void VM_THREAD_CALLCONVENTION inflate_chunk(void *params)
{
   GZIP_FILE        *fd_in;
   unsigned int     read_size;

   struct ic_params *pic = (struct ic_params*)params;
   int              chunk = pic->chunk;
   Ipp32u           from_crc = pic->from_crc;
   Ipp32u           to_crc = pic->to_crc;
   OFFSET_TYPE      from_pos = pic->from_pos;
   OFFSET_TYPE      to_pos = pic->to_pos;
   size_t           in_buffer_size = pic->in_buffer_size;
   size_t           out_buffer_size = pic->out_buffer_size;
   char             *filename = pic->filename;
   UBYTE            *chunk0_inptr = pic->chunk0_inptr;
   UBYTE            *chunk0_inbuf = pic->chunk0_inbuf;
   GZIP_FILE        *chunk0_fd_in = pic->chunk0_fd_in;
   GZIP_FILE        *tmp_fd = pic->tmp_fd;
   int              *err_status = pic->err_status;
   SIZE_TYPE        target_file_size = pic->target_file_size;
   int              max_chunks = pic->max_chunk;
   vm_semaphore     *semaphore = pic->semaphore;

   SIZE_TYPE        target_file_position;
   table_codes      codes[2048];
   struct inf_state_mt* state;
   int              status = ST_OK;
   MEM_ENTRY        *p_mem;
   GZIP_MMAP_HANDLE *p_view;

   p_mem = zalloc_mem_head();
   state = (struct inf_state_mt*)zalloc_mem(sizeof(struct inf_state_mt), p_mem);
   memset(state, 0, sizeof(struct inf_state_mt));
   state->in_size = (unsigned int)in_buffer_size;
   state->out_size = (unsigned int)out_buffer_size;
   state->filename = filename;

   if(chunk == 0) {           /* initial chunk */
      state->in_buf = chunk0_inbuf;
      state->in_ptr = chunk0_inptr;
      fd_in = chunk0_fd_in;
      if(opt_io_write_method == METHOD_MMAP && !opt_test) {
         if((p_view = zdup_mmap(tmp_fd, 0)) == NULL) {
            warn_message(filename, "cannot open view for address 0");
            zfree_all(p_mem);
            ret_status = STATUS_ERR;
            return;
         }
      }
   } else {
      /* Open input file in parallel thread */
      if((fd_in = ZOPEN(filename, GZIP_FILE_READ, opt_io_read_method, 0)) == NULL) {
         warn_message(filename, "cannot open input file in inflate_chunk");
         error_file(filename);
         zfree_all(p_mem);
         ret_status = STATUS_ERR;
         return;
      }
      /* Allocate input buffer */
      state->in_buf = (UBYTE*)zalloc_mem(in_buffer_size, p_mem);
      state->in_ptr = state->in_buf;
      /* This is not chunk#0, so we need to seek to desired position and to read input data */
      if( zseek(fd_in, from_pos, SEEK_SET)==0) {
         warn_message(filename, "cannot seek in inflate_chunk");
         error_file(filename);
         if(chunk) {
            zfree_all(p_mem);
            zclose(fd_in);
         }
         ret_status = STATUS_ERR;
         return;
      }
      /* For writing to the same file in parallel mode we need to set file position for output and lock target region of output file */
      target_file_position = (target_file_size/max_chunks)*chunk;
      if(opt_io_write_method == METHOD_MMAP && !opt_test) {
         if((p_view = zdup_mmap(tmp_fd, target_file_position)) == NULL) {
            zfree_all(p_mem);
            zclose(fd_in);
            ret_status = STATUS_ERR;
            return;
         }
      }
   }
   /* Create output buffer */
   state->out_buf = (UBYTE*)zalloc_mem(out_buffer_size, p_mem);
   state->out_ptr = state->out_buf;
   state->out_len = (unsigned int)out_buffer_size;
   state->in_left = to_pos - from_pos;
   ((IppInflateState*)codes)->winSize = 32768;
   ((IppInflateState*)codes)->tableBufferSize = 2048 * sizeof(state->buf_bits) - sizeof(IppInflateState);
   ((IppInflateState*)codes)->pWindow = (UBYTE*)zalloc_mem(((IppInflateState*)codes)->winSize, p_mem);
   /* Determine how much to read */
   read_size = (unsigned int)MINSIZE((unsigned int)in_buffer_size, state->in_left);
   if(chunk) {       /* If not initial chunk - read */
      state->in_len = (unsigned int)zread(fd_in, state->in_buf, read_size);
   } else
      state->in_len = (int)MINSIZE(to_pos-from_pos, (long)(in_buffer_size-from_pos));
   state->crc = from_crc;
   state->in_left -= state->in_len;
   while(1) { /* decoding loop */
      status = block_type(state, codes, fd_in);
      if(state->mode == stored) {
          BYTEBITS();
         if(!opt_test) {
            if(opt_io_write_method == METHOD_MMAP)
               status = stored_block(state, codes, fd_in, NULL, p_view);
            else    /* METHOD == LOW */
               status = stored_block(state, codes, fd_in, tmp_fd, NULL);
         } else {
            status = stored_block(state, codes, fd_in, NULL, NULL);
         }
      } else if(state->mode == dynamic || state->mode == fixed)
         if(!opt_test) {
            if(opt_io_write_method == METHOD_MMAP)
               status = huff_block(state, codes, fd_in, NULL, p_view);
            else    /* METHOD == LOW */
               status = huff_block(state, codes, fd_in, tmp_fd, NULL);
         } else {
            status = huff_block(state, codes, fd_in, NULL, NULL);
         }
      if((state->in_left == 0 && state->in_len == 0 && status == ST_OK) || status == ST_STREAM_END) {
         ippsCRC32_8u(state->out_buf, (int)(out_buffer_size - state->out_len), &state->crc);
         if(!opt_test) {
            if(opt_io_write_method == METHOD_MMAP) {
               size_t write_size = (size_t)(out_buffer_size - state->out_len);
               if(zwrite_dup_mmap(p_view, state->out_buf, write_size) != write_size) {
                  warn_message(filename, "error writing to temporary map file");
                  if(chunk) {
                     zclose(fd_in);
                  }
                  zclose_dup_mmap(p_view);
                  zfree_all(p_mem);
                  *err_status = 1;
                  ret_status = STATUS_ERR;
                  return;
               }
            }
            else
            {    /* METHOD == LOW */
               if(zwrite(tmp_fd, state->out_buf, (size_t)(out_buffer_size-state->out_len)) !=
                            (size_t)(out_buffer_size - state->out_len)) {
                  warn_message(filename, "error writing to temporary file");
                  if(chunk) {
                     zclose(fd_in);
                  }
                  zclose(tmp_fd);
                  *err_status = 1;
                  zfree_all(p_mem);
                  ret_status = STATUS_ERR;
                  return;
               }
            }
            state->out_ptr = state->out_buf;
            state->out_len = (unsigned int)out_buffer_size;
         }
         break;
      } else if(status == ST_ERROR) {
         warn_message(filename, "inflate error");
         if(!opt_test) {
            if(opt_io_write_method == METHOD_MMAP)
               zclose_dup_mmap(p_view);
            else
               zclose(tmp_fd);
         }
         if(chunk) {
            zclose(fd_in);
         }
         zfree_all(p_mem);
         return;
      }
   }

   if(opt_io_write_method == METHOD_MMAP && !opt_test)
      zclose_dup_mmap(p_view);
   if(to_crc != 0) {
      if( state->crc != to_crc ) {        /* CRC error for current chunk */
         chunk_warn_message(filename, chunk, "crc error");
         *err_status = 1;
         if(chunk) {
            zclose(fd_in);
         }
         zfree_all(p_mem);
         ret_status = STATUS_ERR;
         return;
      }
   } else {
       BYTEBITS();
       NEEDBITS(32);
       if(state->buf_bits != state->crc) {
           chunk_warn_message(filename, chunk, "crc error");
           *err_status = 1;
           if(chunk) {
               zclose(fd_in);
           }
           zfree_all(p_mem);
           ret_status = STATUS_ERR;
           return;
      }
       DROPBITS(16);
       DROPBITS(16);
       state->buf_bits = 0;
      if (state->in_len > 4) {
          chunk_warn_message(filename, chunk, "decompression OK, trailing garbage ignored");
          *err_status = extra_trail;
          ret_status = STATUS_WARN;
      }
   }
/* End of input buffer processing */
   if(chunk) {
      zclose(fd_in);
   }
   vm_semaphore_post(semaphore);
   zfree_all(p_mem);
   ret_status = STATUS_OK;
   return;
}

#endif /* GZIP_VMTHREADS */
