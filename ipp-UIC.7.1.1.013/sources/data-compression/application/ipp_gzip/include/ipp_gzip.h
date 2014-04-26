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
#ifndef IPP_GZIP_H
#define IPP_GZIP_H

#include <stdio.h>
#include <sys/stat.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#ifdef GZIP_VMTHREADS
    #include <vm_thread.h>
    #include <vm_sys_info.h>
    #include <vm_mutex.h>
#endif
#if !defined _WIN32
    #include <sys/mman.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif
#include "ippcore.h"

/* MS-specific definitions */
#if defined(_WIN32) && _MSC_VER >= 1400 && !defined(_AMD64_)
#define VS2005_PLUS
#else
#undef VS2005_PLUS
#endif

/* File size-specific typedefs */
#if defined(_WIN32)
   typedef __int64 SIZE_TYPE;
   typedef __int64 OFFSET_TYPE;
   typedef struct __stat64 STAT_TYPE;
#else
#if !defined(OSX)
   typedef off64_t SIZE_TYPE;
   typedef off64_t OFFSET_TYPE;
   typedef struct stat64 STAT_TYPE;
#else
   typedef off_t SIZE_TYPE;
   typedef off_t OFFSET_TYPE;
   typedef struct stat64 STAT_TYPE;
#endif
#endif

typedef unsigned char UBYTE;
#ifndef _WIN32
    typedef size_t DWORD;
    typedef unsigned short BOOL;
#endif

extern char* OFFSET_FORMAT;
extern char* CRC_FORMAT;
/*! \brief Ascii text mode selected */
extern short opt_ascii;
/*! \brief Print uncompressed output to stdout */
extern short opt_to_stdout;
/*! \brief Decompress the file(s) */
extern short opt_decompress;
/*! \brief NOT IMPLEMENTED: encrypt the compressed stuff */
extern short opt_encrypt;
extern short opt_force;
extern short opt_help;
extern short opt_pkzip;
extern short opt_list;
extern short opt_license;
extern short opt_force_dynamic;
/*! \brief Don't save/restore original name */
extern short opt_noname;
extern short opt_name;
extern short opt_quiet;
extern short opt_reqursive;
extern short opt_new_suffix;
extern short opt_test;
extern short opt_notime;
extern short opt_verbose;
extern short opt_version;
extern short opt_compress_fast;
extern short opt_compress_best;
extern short opt_lzw_compat;
extern short opt_bits;
extern short opt_rsyncable;
extern short opt_clocks;
extern char *prog_name;
extern char *suffix;
extern int num_threads;
extern short from_stdin;
extern int ret_status;
#define STATUS_OK   0
#define STATUS_WARN 1
#define STATUS_ERR  2

/* Input/output options */
extern short        opt_io_read_method;
extern char         *ptr_read_method;
extern short        opt_io_write_method;
extern char         *ptr_write_method;
extern short        opt_io_read_buffer;
extern size_t       INBUF_SIZE;
extern char         *ptr_read_buffer;
extern short        opt_io_write_buffer;
extern size_t       OUTBUF_SIZE;
extern char         *ptr_write_buffer;
extern short        opt_perf_stat;


extern short opt_dbg_multithreads;
extern char *opt_num_threads_forced;
extern short opt_dbg_slice_size;
extern char *opt_slice_size_array;
extern long opt_slice_size;

extern short opt_debug;

/* File i/o interface */
enum TYPE_GZIP_FILE_MODE {
    GZIP_FILE_READ,
    GZIP_FILE_WRITE
};
typedef enum TYPE_GZIP_FILE_MODE GZIP_FILE_MODE;

enum TYPE_GZIP_IO_METHOD {
   METHOD_LOW = 0,
   METHOD_MMAP = 1
};
typedef enum TYPE_GZIP_IO_METHOD GZIP_IO_METHOD;

typedef int GZIP_FILE_HANDLE;
typedef struct mmap_data {
#if defined(_WIN32)
    HANDLE              map_h_file;
    HANDLE              map_h_map;
#else
    GZIP_FILE_HANDLE     map_h_file;
#endif
    size_t              map_granularity;
    size_t              map_base_len;
    size_t              map_valid_bytes;
    UBYTE               *map_base_addr;
    UBYTE               *map_addr;
    OFFSET_TYPE         map_offset;
    SIZE_TYPE           map_max_size;
    GZIP_FILE_MODE      map_mode;
    int                 map_eof_condition;
} GZIP_MMAP_HANDLE;

struct ipp_gzip_file {
    GZIP_FILE_MODE      file_mode;
    GZIP_IO_METHOD      file_io_method;
    int                 file_eof_condition;
    SIZE_TYPE           file_bytes_out;
    SIZE_TYPE           file_bytes_in;
    SIZE_TYPE           file_uncompr_size;
    SIZE_TYPE           file_size;
    GZIP_MMAP_HANDLE    *file_p_map_handle;
    GZIP_FILE_HANDLE    file_h_file;
    OFFSET_TYPE         file_offset;
    OFFSET_TYPE         file_cur_pos;
};

typedef struct ipp_gzip_file GZIP_FILE;

typedef struct finalize_params {
   OFFSET_TYPE       initial_offset;
   GZIP_FILE         *file_ptr;
   GZIP_FILE         *in_file_ptr;
   int               status;
} GZIP_FINALIZE_DATA;
/* Dynamic memory structure to simplify mem freeing after several calls to malloc */
struct mem_entry {
   void*             p_allocated_mem;
   struct mem_entry* p_next;
};
typedef struct mem_entry MEM_ENTRY;
/* Function prototypes to deal with dynamic memory */
MEM_ENTRY* zalloc_mem_head(void);
void* zalloc_mem(size_t, MEM_ENTRY*);
void zfree_all(MEM_ENTRY*);

#if defined (__MACH__)
    #define ZOPEN zzopen
#else
    #define ZOPEN zzopen
#endif

GZIP_FILE*  ZOPEN(const char*, GZIP_FILE_MODE, int, SIZE_TYPE);
SIZE_TYPE   zget_bytes_out(GZIP_FILE*);
SIZE_TYPE   zget_bytes_in(GZIP_FILE*);
void        zset_uncompressed_size(GZIP_FILE*, SIZE_TYPE);
void        zset_max_file_len(GZIP_FILE*, SIZE_TYPE);
int         zextend_file(GZIP_FILE*, SIZE_TYPE);
GZIP_MMAP_HANDLE
            *zdup_mmap(GZIP_FILE*, OFFSET_TYPE);
size_t      zwrite_dup_mmap(GZIP_MMAP_HANDLE*, UBYTE*, size_t);
size_t      zread_dup_mmap(GZIP_MMAP_HANDLE*, UBYTE*, size_t);
void        zclose_dup_mmap(GZIP_MMAP_HANDLE*);
OFFSET_TYPE zseek_mmap(GZIP_MMAP_HANDLE*, OFFSET_TYPE, SIZE_TYPE);

int         zclose(GZIP_FILE*);                             /* Close file function */
size_t      zread(GZIP_FILE*, UBYTE*, size_t);            /* Read specified number of bytes to specified address */
size_t      zwrite(GZIP_FILE*, UBYTE*, size_t);           /* Write buffer */
OFFSET_TYPE zseek(GZIP_FILE*, OFFSET_TYPE, int);                   /* Move file pointer to specified position */
int         zeof(GZIP_FILE*);                               /* Check if EOF */
OFFSET_TYPE ztell(GZIP_FILE*);                             /* Return current file position */
void        zcopytime(char*, char*, long);                 /* Copy date/time information from one file to another */
GZIP_FILE*  zopen_stdin(void);                           /* Open standard input stream */
GZIP_FILE*  zopen_stdout(void);                          /* Open standard output stream */
SIZE_TYPE   zgetsize(GZIP_FILE*);                        /* Get file size */
int         zfilemap(GZIP_FILE*, DWORD);
void        ztruncate(GZIP_FILE*);
#if !defined(_WIN32)
mode_t      zget_permbits(GZIP_FILE*);
int         zset_permbits(GZIP_FILE*, mode_t);
#endif

BOOL        zextend(GZIP_FILE*, DWORD);

/* Global function prototypes */
char* char_to_oem(char*);
void  char_to_oem_free(char*);
void  print_stat(float, float, char*, char*, short);
void  print_version(void);
int   get_options(int, char **);
void  help(void);
void  warn_message(char*, char*);
char  *get_program_name(char*);
void  decompress(char*);
void  compress(char*);
#ifdef GZIP_VMTHREADS
void  deflate_mt(char*);     /* Deflate multithreaded version */
void  inflate_mt(char*);
#endif
void  deflate_st(char*);     /* Deflate singlethreaded version */
void  inflate_st(char*);
void  error_file(char*);     /* Print file error message and exit */
void  create_output_name(char *, char *);
UBYTE *create_gzip_hdr(UBYTE*, char*, STAT_TYPE*, int);
int   check_ipp_gzip_comment(const UBYTE*);
void  get_base_name(char*, char*);
int   get_nosuf_name(char*, char*);
void  restore_output_name(char*, char*, char*);
void  print_ipp_status(IppStatus);
void  print_progname(void);
GZIP_FILE *out_open(char*, SIZE_TYPE);
void  show_list(char*, char*, GZIP_FILE*,long);
char  *mk_chunk_name(char*, int, char*);
int   remove_tmp_file(char*, int);
int   get_cpu_count(void);
int   known_suffix(char*);
int   is_file(int);
void  error_system(char*);
void  print_io_methods(void);

/* Global definitions */
/*! \brief Max threads to allow */
#define MAX_THREADS 32
/*! \brief Minimum file length to use chunks */
#define MIN_LENGTH_TO_SLICE (1024 * 256)
/*! \brief GZIP magic byte 1 */
#define GZIP_HDR_1 0x1F
/*! \brief GZIP magic byte 2 */
#define GZIP_HDR_2 0x8B
/*! \brief DEFLATE byte in header*/
#define DEFLATE 0x08
/* Flag byte: definition of bits */
/*! \brief Compressed file is text */
#define FTEXT 0x01
/*! \brief CRC16 is stored */
#define FHCRC16 0x02
/*! \brief Extra fields are present */
#define FEXTRA 0x04
/*! \brief Filename stored in gzip header */
#define FNAME 0x08
/*! \brief Comment field is present. In multi-chunk mode comment contains chunk params */
#define FCOMMENT 0x10
/*! \brief Extra flag: max compression */
#define XFL_BEST 0x02
/*! \brief Extra flag: best compression */
#define XFL_FAST 0x04
/*! \brief NTFS-based file system */
#define STORED_BUF_SIZE (1024*70)
/*! \brief Buffer size for stored block saving */
#ifdef _WIN32
#define OS_NTFS 0x0B
#define OS_BYTE OS_NTFS
#else
#define OS_UNIX 0x03
#define OS_BYTE OS_UNIX
#endif
/*! \brief Standard GZIP header size */
#define GZIP_HEADER_SIZE 10
/*! \brief Specific IPP GZIP comment field header */
#define COMMENT_MAGIC_HEADER 0xC0
/* Size of comment field */
#define COMMENT_OFFSET_FRM_SZ (16)
#define COMMENT_CRC_FRM_SZ (8)
#define COMMENT_SIZE(chunks) ((chunks-1)*(COMMENT_OFFSET_FRM_SZ+COMMENT_CRC_FRM_SZ) + 4 + COMMENT_OFFSET_FRM_SZ)
#ifdef _WIN32
    #define MAX_NAME _MAX_PATH
#else
    #include <limits.h>
    #define MAX_NAME PATH_MAX
#endif

/*! \brief Put one byte to output buffer */
#define PUT_BYTE(o, b) *o++ = (UBYTE)(b)
/*! \brief Put long (4-byte) value to output buffer */
#define PUT_LONG(o, L) { PUT_BYTE(o, L & 0xFF); PUT_BYTE(o, (L>>8)&0xFF); PUT_BYTE(o, (L>>16)&0xFF); PUT_BYTE(o, (L>>24)&0xFF); }

/*! \brief Get one byte from input buffer */
#define GET_BYTE(i) (*i++)
/*! \brief Get long (4-byte value) from input buffer */
/* #define GET_LONG(i) (GET_BYTE(i) | (GET_BYTE(i) << 8) | (GET_BYTE(i) << 16) | (GET_BYTE(i) << 24))    - Doesn't work correctly on Linux */
#define GET_LONG(i) ((*i) | (*(i+1) << 8) | (*(i+2) << 16) | (*(i+3) << 24))
/*! \brief Check IPP DC status. If not ippStsNoErr - complain */
#define CHECK_STATUS(status) if (status != ippStsNoErr) print_ipp_status(status);
/*! \brief Get High and Low DWORDS of argument */
#define HI_DWORD(arg) ((DWORD)(arg>>32))
#define LO_DWORD(arg) ((DWORD)(arg & UINT_MAX))

#if defined(_WIN32)
#define SYSSTAT _stat64
#else
#define SYSSTAT   stat64
#define FSTAT     fstat64
#endif

#if defined(VS2005_PLUS)

#define UNLINK(file) _unlink(file)
#define FILENO(file) _fileno(file)
#define ISATTY(file) _isatty(file)
#define SETMODE(file, mode) _setmode(file, mode)
#define SYSOPEN _sopen_s
#define SYSCLOSE _close
#define SYSREAD _read
#define SYSWRITE _write
#define SYSLSEEK _lseeki64
#define SYSTELL _telli64
#define SYSSCANF sscanf_s

#else /* Linux & VS < VS2005 */

#define UNLINK(file) unlink(file)
#define FILENO(file) fileno(file)
#define ISATTY(file) isatty(file)
#define SETMODE(file, mode) setmode(file, mode)

#if defined(_WIN32)
#define SYSOPEN open
#else
#if !defined(OSX)
#define SYSOPEN open64
#else
#define SYSOPEN open
#endif
#endif

#define SYSCLOSE close
#define SYSREAD read
#define SYSWRITE write
#if !defined(_WIN32)
#if !defined(OSX)
#define SYSLSEEK lseek64
#else
#define SYSLSEEK lseek
#endif
#else
#define SYSLSEEK _lseeki64
#endif
#define SYSSCANF sscanf

#ifdef _WIN32
#define SYSTELL _telli64
#else
#define SYSTELL(f) lseek(f, 0, SEEK_CUR)
#endif

#endif

#ifdef _WIN32

#if defined(VS2005_PLUS)

#define READ_MODE_BITS  (_O_RDONLY | _O_SEQUENTIAL | _O_BINARY)
#define WRITE_MODE_BITS (_O_CREAT | _O_BINARY | _O_RDWR | _O_SEQUENTIAL | _O_TRUNC)

#else

#define READ_MODE_BITS  (O_RDONLY | O_SEQUENTIAL | O_BINARY)
#define WRITE_MODE_BITS (O_CREAT | O_BINARY | O_RDWR | O_SEQUENTIAL | O_TRUNC)

#endif
#define SYSUNMAP(p,l) UnmapViewOfFile(p)
#define SYSCLOSE_FD_OR_HANDLE(fd, h) CloseHandle(h)
#else
#define READ_MODE_BITS  (O_RDONLY)
#define WRITE_MODE_BITS (O_CREAT | O_RDWR | O_TRUNC)
#define SYSUNMAP(p,l) munmap(p, l)
#define SYSCLOSE_FD_OR_HANDLE(fd, h) SYSCLOSE(fd)
#endif

/*! \brief Choose minimal of two values */
#define MINSIZE(a, b) ((a<b)? a: b)
#define MAXSIZE(a, b) ((a>b)? a: b)

#ifndef GZIP_DEFLATE_OLD

#define ST_ERROR -1
#define ST_OK 0
#define ST_STREAM_END 2

#define UNKNOWN_BLOCK -1
#define STORED_BLOCK 0
#define STATIC_BLOCK 1
#define DYNAMIC_BLOCK 2
#define BUF_SIZE 8*2
#define STORED_MAX 1024*64-1
#define WIN_SIZE 1024*32
#define LIT_SIZE 1024*16
#define LEN_CODE_LEN 286
#define OFT_CODE_LEN 30
#define SRV_CODE_LEN 20

#define NEEDBITS(NUM) \
do { \
    if (state->buf_len >= (NUM)) break; \
    if (state->in_len == 0) { \
        state->in_len = (unsigned int)zread(fd_in, state->in_buf, (unsigned int)INBUF_SIZE); \
        state->in_ptr = state->in_buf; \
    } \
    state->buf_bits |= (Ipp32u)(*state->in_ptr << state->buf_len); \
    state->in_ptr++; \
    state->buf_len += 8; \
    state->in_len--; \
} while(1);


#define DROPBITS(NUM) \
  state->buf_bits >>= (NUM); \
  state->buf_len -= (NUM);

#define BYTEBITS() \
    do { \
        int have_bits = state->buf_len % 8; \
        int align_bits = 8 - have_bits; \
        if (state->buf_len < 8) { \
            state->buf_len = 0; \
            break; \
        } \
        if (have_bits == 0) break; \
        state->buf_bits >>= have_bits; \
        state->buf_bits |= (Ipp32u)(*state->in_ptr++ << 8); \
        state->in_len--; \
        state->buf_len += align_bits; \
    } while(0)

#endif

#if !defined(_WIN32)
extern size_t sys_page_size;
#endif
#endif

