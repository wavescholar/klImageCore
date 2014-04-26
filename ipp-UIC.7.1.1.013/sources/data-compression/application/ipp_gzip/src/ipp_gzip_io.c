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
#include "ipp_gzip.h"

#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#include <limits.h>
#include <sys\utime.h>

#if defined(VS2005_PLUS)
#include <share.h>
#endif
extern SYSTEM_INFO sysInfo;
#else /* Linux */

#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#endif /* ifdef _win32 */

/* Global data */
#if defined(_WIN32)
char *OFFSET_FORMAT = "%016I64X";
#else
char *OFFSET_FORMAT = "%016qX";
#endif
char *CRC_FORMAT = "%08X";
/* Forward declarations */
static GZIP_FILE_HANDLE zopen_low(const char*, GZIP_FILE_MODE);
static GZIP_MMAP_HANDLE *zopen_mmap(GZIP_FILE*, const char*, GZIP_FILE_MODE, SIZE_TYPE);
static OFFSET_TYPE zseek_low(GZIP_FILE*, OFFSET_TYPE, int);
static size_t zread_low(GZIP_FILE*, UBYTE*, size_t);
static size_t zread_mmap(GZIP_FILE*, UBYTE*, size_t);
static size_t zwrite_low(GZIP_FILE*, UBYTE*, size_t);
static size_t zwrite_mmap(GZIP_FILE*, UBYTE*, size_t);
static int zclose_low(GZIP_FILE*);
static int zclose_mmap(GZIP_FILE*);
static int zmap_file(GZIP_MMAP_HANDLE*, OFFSET_TYPE, SIZE_TYPE);

GZIP_FILE *zzopen(const char *filename, GZIP_FILE_MODE mode, int file_io_method, SIZE_TYPE initial_size)
{
   GZIP_FILE *new_ptr;
   STAT_TYPE istat;

   if(mode != GZIP_FILE_READ && mode != GZIP_FILE_WRITE)
      return NULL;

   new_ptr = (GZIP_FILE*)malloc(sizeof(GZIP_FILE));

   if(NULL == new_ptr)
      return NULL;
   if(GZIP_FILE_READ == mode) {
      SYSSTAT(filename, &istat);
      initial_size = istat.st_size;
   }
   switch(file_io_method) {
      case METHOD_LOW:
         new_ptr->file_h_file = zopen_low(filename, mode);
         new_ptr->file_io_method = METHOD_LOW;
         new_ptr->file_p_map_handle = NULL;
         break;
      case METHOD_MMAP:
         new_ptr->file_p_map_handle = zopen_mmap(new_ptr, filename, mode, initial_size);
         if(NULL == new_ptr->file_p_map_handle) {
            free(new_ptr);
            return NULL;
         }
         new_ptr->file_io_method = METHOD_MMAP;
         new_ptr->file_h_file = 0;
         break;
      default:
         free(new_ptr);
         return NULL;
   }
   new_ptr->file_size = new_ptr->file_uncompr_size = initial_size;

   new_ptr->file_bytes_in = new_ptr->file_bytes_out = 0;
   new_ptr->file_cur_pos = new_ptr->file_offset = 0;
   new_ptr->file_eof_condition = 0;
   new_ptr->file_mode = mode;

   return new_ptr;
}
OFFSET_TYPE zseek(GZIP_FILE *file_ptr, OFFSET_TYPE how_much, int direction)
{
   if(NULL == file_ptr)
      return 0;
   if(file_ptr->file_io_method == METHOD_LOW)
      return zseek_low(file_ptr, how_much, direction);
   else {
      OFFSET_TYPE status, desired_pos;
      if(direction == SEEK_END) {
         if(how_much > 0)
            desired_pos = file_ptr->file_size - how_much;
         else
            desired_pos = file_ptr->file_size + how_much;
      }
      else
         desired_pos = how_much;
      status = zseek_mmap(file_ptr->file_p_map_handle, desired_pos, file_ptr->file_size);
      if(status == desired_pos)
         file_ptr->file_cur_pos = desired_pos;
      return status;
   }
}
size_t zread(GZIP_FILE *file_ptr, UBYTE *p_buffer, size_t buf_size)
{
   if(NULL == file_ptr)
      return 0;
   if(file_ptr->file_io_method == METHOD_LOW)
      return zread_low(file_ptr, p_buffer, buf_size);
   else
      return zread_mmap(file_ptr, p_buffer, buf_size);
}
size_t zwrite(GZIP_FILE *file_ptr, UBYTE *p_buffer, size_t buf_size)
{
   if(NULL == file_ptr)
      return 0;
   if(file_ptr->file_io_method == METHOD_LOW)
      return zwrite_low(file_ptr, p_buffer, buf_size);
   else
      return zwrite_mmap(file_ptr, p_buffer, buf_size);
}
int zclose(GZIP_FILE *file_ptr)
{
   if(NULL == file_ptr)
      return 0;
   if(file_ptr->file_io_method == METHOD_LOW)
      return zclose_low(file_ptr);
   else
      return zclose_mmap(file_ptr);
}
/* mmap existing and opened file one more time */
GZIP_MMAP_HANDLE *zdup_mmap(GZIP_FILE *file_ptr, OFFSET_TYPE offset)
{
   GZIP_MMAP_HANDLE *p_map_handle;

   if(NULL == file_ptr)
      return NULL;
   p_map_handle = (GZIP_MMAP_HANDLE*)malloc(sizeof(GZIP_MMAP_HANDLE));
   if(NULL == p_map_handle)
      return NULL;
   memset(p_map_handle, 0, sizeof(GZIP_MMAP_HANDLE));
   p_map_handle->map_mode = GZIP_FILE_WRITE;
   p_map_handle->map_h_file = file_ptr->file_p_map_handle->map_h_file;
   p_map_handle->map_max_size = file_ptr->file_size;
#if defined(_WIN32)
   p_map_handle->map_granularity = sysInfo.dwAllocationGranularity;
   p_map_handle->map_h_map = CreateFileMapping(p_map_handle->map_h_file, NULL, PAGE_READWRITE, 0, 0, NULL);
   if(p_map_handle->map_h_map == NULL) {
      error_system("Cannot duplicate map object");
      free(p_map_handle);
      return NULL;
   }
#else
   p_map_handle->map_granularity = sys_page_size;
#endif
   if(zseek_mmap(p_map_handle, offset, p_map_handle->map_max_size) != offset) {
      error_system("Cannot seek duplicated object");
#if defined(_WIN32)
      CloseHandle(p_map_handle->map_h_map);
#else
#endif
      free(p_map_handle);
      return NULL;
   }
   p_map_handle->map_offset = offset;
   return p_map_handle;
}
SIZE_TYPE zget_bytes_in(GZIP_FILE *file_ptr)
{
   return file_ptr->file_bytes_in;
}
SIZE_TYPE zget_bytes_out(GZIP_FILE *file_ptr)
{
   return file_ptr->file_bytes_out;
}
GZIP_FILE* zopen_stdout()
{
   GZIP_FILE *new_ptr;
   new_ptr = (GZIP_FILE*)malloc(sizeof(GZIP_FILE));
   new_ptr->file_bytes_in = 0;
   new_ptr->file_bytes_out = 0;
   new_ptr->file_mode = GZIP_FILE_WRITE;
   new_ptr->file_io_method = METHOD_LOW;
   new_ptr->file_h_file = FILENO(stdout);
   new_ptr->file_p_map_handle = NULL;
#ifdef _WIN32
#if defined(VS2005_PLUS)
   SETMODE(new_ptr->file_h_file, _O_BINARY);
#else
   SETMODE(new_ptr->file_h_file, O_BINARY);
#endif
#endif
   return new_ptr;
}
GZIP_FILE* zopen_stdin()
{
   GZIP_FILE *new_ptr;
   new_ptr = malloc(sizeof(GZIP_FILE));
   new_ptr->file_bytes_in = 0;
   new_ptr->file_bytes_out = 0;
   new_ptr->file_eof_condition = 0;
   new_ptr->file_mode = GZIP_FILE_READ;
   new_ptr->file_h_file = FILENO(stdin);
   new_ptr->file_io_method = METHOD_LOW;
   new_ptr->file_p_map_handle = NULL;
#ifdef _WIN32
#if defined(VS2005_PLUS)
   SETMODE(new_ptr->file_h_file, _O_BINARY);
#else
   SETMODE(new_ptr->file_h_file, O_BINARY);
#endif
#endif
   return new_ptr;
}
void ztruncate(GZIP_FILE *file_ptr)
{
   if(file_ptr->file_io_method == METHOD_LOW)
      return;
   if(file_ptr->file_p_map_handle != NULL && file_ptr->file_p_map_handle->map_base_addr != NULL) {
      SYSUNMAP(file_ptr->file_p_map_handle->map_base_addr, file_ptr->file_p_map_handle->map_base_len);
   #ifdef _WIN32
      CloseHandle(file_ptr->file_p_map_handle->map_h_map);            /* We don't need view window anymore */
      file_ptr->file_p_map_handle->map_h_map = 0;
   #endif
      file_ptr->file_p_map_handle->map_base_addr = NULL;
      }
#ifdef _WIN32
   {
      LARGE_INTEGER tot_size;
      tot_size.QuadPart = file_ptr->file_cur_pos;
      SetFilePointerEx(file_ptr->file_p_map_handle->map_h_file, tot_size, NULL, FILE_BEGIN);
      SetEndOfFile(file_ptr->file_p_map_handle->map_h_file);
   }
#else
#if defined(OSX)
   ftruncate(file_ptr->file_p_map_handle->map_h_file, file_ptr->file_cur_pos);
#else
   ftruncate64(file_ptr->file_p_map_handle->map_h_file, file_ptr->file_cur_pos);
#endif
#endif
   file_ptr->file_size = file_ptr->file_cur_pos;
}
int zeof(GZIP_FILE *file)
{
   return file->file_eof_condition;
}
OFFSET_TYPE ztell(GZIP_FILE *file)
{
   if(file->file_io_method == METHOD_LOW) {
      return SYSTELL(file->file_h_file);
   }
   else
      return file->file_cur_pos;
}
void zset_uncompressed_size(GZIP_FILE *file_ptr, SIZE_TYPE new_size)
{
   file_ptr->file_uncompr_size = new_size;
}
void zset_max_file_len(GZIP_FILE *file_ptr, SIZE_TYPE max_size)
{
   file_ptr->file_size = file_ptr->file_uncompr_size = max_size;
   if(NULL != file_ptr->file_p_map_handle) {
      file_ptr->file_p_map_handle->map_max_size = max_size;
   }
}
void zcopytime(char *infilename, char *outfilename, long timestamp)
{
   STAT_TYPE ifstat;
#ifdef _WIN32
   struct __utimbuf64 utim;
#else
   struct timeval file_times[2];
#endif
   if(infilename == NULL) {       /* Inflating from stdin */
#if defined(_WIN32)
      if(_fstat64(FILENO(stdin), &ifstat) != 0) {
#else
      if(fstat64(FILENO(stdin), &ifstat) != 0) {
#endif
         error_file("stdin");
         return;
      }
   } else {
      if(SYSSTAT(infilename, &ifstat)) {
         error_file(infilename);
         return;             /* Couldn't get input file information */
      }
   }
   if(!opt_notime && timestamp)
      ifstat.st_mtime = timestamp;
#ifdef _WIN32
   utim.actime = ifstat.st_atime;
   utim.modtime = ifstat.st_mtime;
   _utime64(outfilename, &utim);
#else
   file_times[0].tv_sec = ifstat.st_atime;
   file_times[1].tv_sec = ifstat.st_mtime;
   utimes(outfilename, file_times);
#endif
}
void zclose_dup_mmap(GZIP_MMAP_HANDLE *p_map_handle)
{
   SYSUNMAP(p_map_handle->map_base_addr, p_map_handle->map_base_len);
#if defined(_WIN32)
   CloseHandle(p_map_handle->map_h_map);
   p_map_handle->map_h_map = 0;
#endif
   free(p_map_handle);
}
static size_t ztransfer_data(GZIP_FILE* file_ptr, GZIP_MMAP_HANDLE *p_map_handle, GZIP_FILE_MODE mode,
                      UBYTE *p_buffer, size_t transfer_size)
{
   size_t byte_count = 0;
   size_t to_transfer = transfer_size;

   while(to_transfer > 0) {
      if(p_map_handle->map_valid_bytes > 0) {
         if(to_transfer <= p_map_handle->map_valid_bytes) {
            if(GZIP_FILE_WRITE == mode) {
               memcpy(p_map_handle->map_addr, p_buffer, to_transfer);
            } else {
               memcpy(p_buffer, p_map_handle->map_addr, to_transfer);
            }
            p_map_handle->map_addr += to_transfer;
            p_map_handle->map_valid_bytes -= to_transfer;
            byte_count += to_transfer;
            if(NULL != file_ptr) {
               if(GZIP_FILE_WRITE == mode)
                  file_ptr->file_bytes_out += byte_count;
               else
                  file_ptr->file_bytes_in += byte_count;
               file_ptr->file_cur_pos += to_transfer;
               if(file_ptr->file_cur_pos >= file_ptr->file_size)
                  file_ptr->file_eof_condition = 1;
            } else
               p_map_handle->map_offset += to_transfer;
            return byte_count;
         } else {
            size_t chunk_size = p_map_handle->map_valid_bytes;

            if(GZIP_FILE_WRITE == mode)
               memcpy(p_map_handle->map_addr, p_buffer, chunk_size);
            else
               memcpy(p_buffer, p_map_handle->map_addr, chunk_size);
            p_map_handle->map_valid_bytes = 0;
            p_map_handle->map_addr += chunk_size;
            p_buffer += chunk_size;
            byte_count += chunk_size;
            to_transfer -= chunk_size;
            if(NULL != file_ptr) {
               if(GZIP_FILE_WRITE == mode)
                  file_ptr->file_bytes_out += chunk_size;
               else
                  file_ptr->file_bytes_in += chunk_size;
               file_ptr->file_cur_pos += chunk_size;
               if(file_ptr->file_cur_pos >= file_ptr->file_size) {
                  file_ptr->file_eof_condition = 1;
                  return byte_count;
               }
            } else {
               p_map_handle->map_offset += chunk_size;
               if(p_map_handle->map_eof_condition)
                  return byte_count;
            }
         }
      }
      if(NULL != file_ptr)
         zseek_mmap(p_map_handle, file_ptr->file_cur_pos, file_ptr->file_size);
      else
         zseek_mmap(p_map_handle, p_map_handle->map_offset, p_map_handle->map_max_size);
      if(0 == p_map_handle->map_valid_bytes)
         return byte_count;
   }
   return byte_count;
}
size_t zwrite_dup_mmap(GZIP_MMAP_HANDLE *p_map_handle, UBYTE *p_buffer, size_t write_size)
{
   return ztransfer_data(NULL, p_map_handle, GZIP_FILE_WRITE, p_buffer, write_size);
}
size_t zread_dup_mmap(GZIP_MMAP_HANDLE *p_map_handle, UBYTE *p_buffer, size_t read_size)
{
   return ztransfer_data(NULL, p_map_handle, GZIP_FILE_READ, p_buffer, read_size);
}
int zextend_file(GZIP_FILE *file_ptr, SIZE_TYPE new_size)
{
#if defined(_WIN32)
   LARGE_INTEGER truncate_size;
#endif
   if(file_ptr->file_mode == GZIP_FILE_READ || file_ptr->file_io_method == METHOD_LOW)
      return 1; /* No manual extension required */
#if defined(_WIN32)
   truncate_size.QuadPart = new_size;
   if (!UnmapViewOfFile(file_ptr->file_p_map_handle->map_base_addr)) {
      error_system("zextend_file:unmap failure");
      return 0;
   }
   file_ptr->file_p_map_handle->map_base_addr = NULL;
   if(!CloseHandle(file_ptr->file_p_map_handle->map_h_map)) {
      error_system("zextend_file:closehandle failure");
      return 0;
   }
   file_ptr->file_p_map_handle->map_h_map = 0;
   if(!SetFilePointerEx(file_ptr->file_p_map_handle->map_h_file, truncate_size, NULL, FILE_BEGIN))
      return 0;
   if(!SetEndOfFile(file_ptr->file_p_map_handle->map_h_file))
      return 0;
#else
#if defined(OSX)
   if(ftruncate(file_ptr->file_p_map_handle->map_h_file, new_size) == -1)
#else
   if(ftruncate64(file_ptr->file_p_map_handle->map_h_file, new_size) == -1)
#endif
      return 0;
#endif
   return 1;
}
/*
 *
 * Local i/o functions with regards to OS
 *
 */
static GZIP_FILE_HANDLE zopen_low(const char *filename, GZIP_FILE_MODE mode)
{
   GZIP_FILE_HANDLE new_handle;
   int mode_bits;

   mode_bits = (GZIP_FILE_READ==mode)? READ_MODE_BITS : WRITE_MODE_BITS;
#if defined(VS2005_PLUS)
   if(SYSOPEN(&new_handle, filename, mode_bits, _SH_DENYNO, _S_IWRITE | _S_IREAD))
      return 0;
#else
   if((new_handle = SYSOPEN(filename, mode_bits, S_IWRITE | S_IREAD)) == -1)
      return 0;
#endif
   return new_handle;
}
static OFFSET_TYPE zseek_low(GZIP_FILE *file_ptr, OFFSET_TYPE offset, int direction)
{
   return SYSLSEEK(file_ptr->file_h_file, offset, direction);
}
static size_t zread_low(GZIP_FILE *file_ptr, UBYTE *p_buffer, size_t read_size)
{
   size_t to_read = read_size;
   size_t incnt;

   while((incnt=SYSREAD(file_ptr->file_h_file, p_buffer, (unsigned int)to_read)) > 0) {
      to_read -= incnt;
      if(to_read == 0)
         break;
      p_buffer += incnt;
   }
   if(incnt == -1) {
      perror("Cannot read from file");
      return 0;
   }
   file_ptr->file_eof_condition = (incnt == 0)? 1 : 0;
   file_ptr->file_bytes_in += (read_size-to_read);
   return read_size-to_read;
}
static size_t zwrite_low(GZIP_FILE *file_ptr, UBYTE *p_buffer, size_t write_size)
{
   size_t outcnt;
   STAT_TYPE ostat;

   outcnt = SYSWRITE(file_ptr->file_h_file, p_buffer, (unsigned int)write_size);
   file_ptr->file_bytes_out += outcnt;
#if defined(_WIN32)
   _fstat64(file_ptr->file_h_file, &ostat);
#else
   fstat64(file_ptr->file_h_file, &ostat);
#endif
   file_ptr->file_size = ostat.st_size;
   return outcnt;
}
static int zclose_low(GZIP_FILE *file_ptr)
{
   int close_status;

   close_status = SYSCLOSE(file_ptr->file_h_file);
   free(file_ptr);
   return close_status;
}
static GZIP_MMAP_HANDLE *zopen_mmap(GZIP_FILE* file_ptr, const char *filename, GZIP_FILE_MODE mode,
                                    SIZE_TYPE initial_size)
{
   GZIP_MMAP_HANDLE *p_map_handle;
#if defined(_WIN32)
   DWORD file_mode_bits;
   DWORD file_share_bits;
   DWORD file_creation_disp;
   DWORD file_attrs;
#else
   size_t   loc_sys_page_size;
#endif

   file_ptr = file_ptr;
   p_map_handle = (GZIP_MMAP_HANDLE*)malloc(sizeof(GZIP_MMAP_HANDLE));
   p_map_handle->map_mode = mode;
   p_map_handle->map_eof_condition = 0;
#if defined(_WIN32)
   if(GZIP_FILE_READ == mode){
         file_mode_bits = GENERIC_READ;
         file_share_bits = FILE_SHARE_READ;
         file_creation_disp = OPEN_EXISTING;
         file_attrs = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN;
   }
   else {
         file_mode_bits = FILE_ALL_ACCESS;
         file_share_bits = FILE_SHARE_WRITE;
         file_creation_disp = CREATE_ALWAYS;
         file_attrs = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN;
   }
   p_map_handle->map_h_file = CreateFile(filename, file_mode_bits, file_share_bits, NULL,
                                    file_creation_disp, file_attrs, NULL);
   if(p_map_handle->map_h_file == INVALID_HANDLE_VALUE) {
      error_system("Cannot create file");
      free(p_map_handle);
      return NULL;
   }
   if(GZIP_FILE_READ == mode)
      p_map_handle->map_h_map = CreateFileMapping(p_map_handle->map_h_file, NULL, PAGE_READONLY, 0, 0, NULL);
   else
      p_map_handle->map_h_map = CreateFileMapping(p_map_handle->map_h_file, NULL, PAGE_READWRITE,
                                             HI_DWORD(MAXSIZE(initial_size, OUTBUF_SIZE)),
                                             LO_DWORD(MAXSIZE(initial_size, OUTBUF_SIZE)), NULL);
   if(p_map_handle->map_h_map == NULL) {
      error_system("Cannot create map object");
      CloseHandle(p_map_handle->map_h_file);
      free(p_map_handle);
      return NULL;
   }
   p_map_handle->map_granularity = sysInfo.dwAllocationGranularity;
#else /* Linux */
   if(GZIP_FILE_READ == mode) {
      if((p_map_handle->map_h_file = SYSOPEN(filename, READ_MODE_BITS, S_IWRITE | S_IREAD)) < 0) {
         free(p_map_handle);
         return NULL;
      }
   } else {
      if((p_map_handle->map_h_file = SYSOPEN(filename, WRITE_MODE_BITS, S_IWRITE | S_IREAD)) < 0) {
         free(p_map_handle);
         return NULL;
      }
      if(initial_size > 0) {
#if defined(OSX)
         if (ftruncate(p_map_handle->map_h_file, initial_size) < 0) {
#else
         if (ftruncate64(p_map_handle->map_h_file, initial_size) < 0) {
#endif
            SYSCLOSE(p_map_handle->map_h_file);
            free(p_map_handle);
            return NULL;
         }
      }
   }
   p_map_handle->map_granularity = sys_page_size;
#endif
   p_map_handle->map_addr = p_map_handle->map_base_addr = NULL;
   p_map_handle->map_valid_bytes = p_map_handle->map_base_len = 0;
   p_map_handle->map_offset = 0;
   p_map_handle->map_max_size = initial_size;
   return p_map_handle;
}
static int zmap_file(GZIP_MMAP_HANDLE *p_map_handle, OFFSET_TYPE offset, SIZE_TYPE file_size)
{
   size_t map_size;
   size_t buf_size;

   buf_size = (GZIP_FILE_READ==p_map_handle->map_mode)? INBUF_SIZE : OUTBUF_SIZE;
   if((offset+buf_size) > file_size)
      map_size = (size_t)(file_size - offset);
   else
      map_size = buf_size;
#if defined(_WIN32)
   p_map_handle->map_base_addr = MapViewOfFile(p_map_handle->map_h_map,
      (GZIP_FILE_READ==p_map_handle->map_mode)? FILE_MAP_READ : FILE_MAP_ALL_ACCESS,
        HI_DWORD(offset), LO_DWORD(offset), map_size);
   if(p_map_handle->map_base_addr == NULL) {
      error_system("zmap_file failed to map");
      return 0;
   }
#else
#if defined(OSX)
   p_map_handle->map_base_addr = mmap(NULL, map_size, (GZIP_FILE_READ==p_map_handle->map_mode)?
                                        PROT_READ : PROT_READ|PROT_WRITE, MAP_SHARED, p_map_handle->map_h_file, offset);
#else
   p_map_handle->map_base_addr = mmap64(NULL, map_size, (GZIP_FILE_READ==p_map_handle->map_mode)?
                                        PROT_READ : PROT_READ|PROT_WRITE, MAP_SHARED, p_map_handle->map_h_file, offset);
#endif
   if(p_map_handle == (GZIP_MMAP_HANDLE*)(-1))
      return 0;
#endif
   p_map_handle->map_base_len = map_size;
   return 1;
}
OFFSET_TYPE zseek_mmap(GZIP_MMAP_HANDLE *p_map_handle, OFFSET_TYPE offset, SIZE_TYPE max_size)
{
   if(p_map_handle->map_addr) { /* file was previously mapped */
      if(p_map_handle->map_valid_bytes) {
         if(offset >= p_map_handle->map_offset && offset <= (p_map_handle->map_offset+p_map_handle->map_valid_bytes)) {
            size_t offset_dif = (size_t)(offset - p_map_handle->map_offset);
            p_map_handle->map_valid_bytes -= offset_dif;
            p_map_handle->map_addr = p_map_handle->map_base_addr + offset_dif;
            return offset;
         }
      }
      /* Specific case when OFFSET == max_size */
      if (offset == max_size) {
         p_map_handle->map_valid_bytes = 0;
         SYSUNMAP(p_map_handle->map_base_addr, p_map_handle->map_base_len);
         p_map_handle->map_base_addr = p_map_handle->map_addr = NULL;
         p_map_handle->map_eof_condition = 1;
         return offset;
      }
      SYSUNMAP(p_map_handle->map_base_addr, p_map_handle->map_base_len);
      p_map_handle->map_base_addr = p_map_handle->map_addr = NULL;
   }
   if(p_map_handle->map_addr == NULL) {
      size_t map_size;
      size_t offset_dif;
      OFFSET_TYPE granular_offset = offset - (offset % p_map_handle->map_granularity);

      if(granular_offset > p_map_handle->map_max_size)
         return 0;
      if(!zmap_file(p_map_handle, granular_offset, p_map_handle->map_max_size))
         return 0;
      p_map_handle->map_offset = offset;
      offset_dif = (size_t)(offset - granular_offset);
      map_size = p_map_handle->map_base_len - offset_dif;
      if((p_map_handle->map_offset + map_size) > p_map_handle->map_max_size)
         p_map_handle->map_valid_bytes = map_size - (size_t)(p_map_handle->map_max_size - p_map_handle->map_offset);
      else
         p_map_handle->map_valid_bytes = map_size;
      p_map_handle->map_addr = p_map_handle->map_base_addr + offset_dif;
   }
   return offset;
}

static size_t zread_mmap(GZIP_FILE *file_ptr, UBYTE *p_buffer, size_t read_size)
{
   return ztransfer_data(file_ptr, file_ptr->file_p_map_handle, GZIP_FILE_READ, p_buffer, read_size);
}
static int zclose_mmap(GZIP_FILE *file_ptr)
{
   GZIP_MMAP_HANDLE *p_map_handle = file_ptr->file_p_map_handle;

#if defined(_WIN32)
   if ((p_map_handle->map_base_addr) != NULL &&
      !SYSUNMAP(p_map_handle->map_base_addr, p_map_handle->map_base_len))
      error_system("zclose_mmap:unmap failure");
   if(p_map_handle->map_h_map && !CloseHandle(p_map_handle->map_h_map))
      warn_message("mapping handle", "cannot close handle");
   if(p_map_handle->map_h_file) {
         if(!CloseHandle(p_map_handle->map_h_file))
            warn_message("mapped file", "cannot close");
         p_map_handle->map_h_file = 0;
   }
#else
   if(p_map_handle->map_h_file) {
      SYSCLOSE(p_map_handle->map_h_file);
      p_map_handle->map_h_file = 0;
   }
#endif
   free(p_map_handle);
   return 1;
}
static size_t zwrite_mmap(GZIP_FILE *file_ptr, UBYTE *p_buffer, size_t write_size)
{
   return ztransfer_data(file_ptr, file_ptr->file_p_map_handle, GZIP_FILE_WRITE, p_buffer, write_size);
}
#if !defined(_WIN32)
mode_t zget_permbits(GZIP_FILE *file_ptr)
{
   STAT_TYPE istat;
   GZIP_FILE_HANDLE fd;
   fd = (file_ptr->file_h_file)? file_ptr->file_h_file : file_ptr->file_p_map_handle->map_h_file;
   FSTAT(fd, &istat);
   return istat.st_mode;
}
int zset_permbits(GZIP_FILE *file_ptr, mode_t perm_bits)
{
   GZIP_FILE_HANDLE fd;
   fd = (file_ptr->file_h_file)? file_ptr->file_h_file : file_ptr->file_p_map_handle->map_h_file;
   return (fchmod(fd, perm_bits) != 0)? 0 : 1;
}
#endif
