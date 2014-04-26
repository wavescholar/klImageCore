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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#ifndef _WIN32
   #include <unistd.h>
   #ifdef __MACH__
      #include <sys/types.h>
      #include <sys/sysctl.h>
   #endif
#endif

#include <ippcore.h>

#include "ipp_gzip.h"

/*! \brief In Windows possible filename separator can be \ */
#define FNAME_SEPARATOR_1 '\\'
/*! \brief or / */
#define FNAME_SEPARATOR_2 '/'
/*! \brief Defines max pointer of two */
#define maxptr(a, b) ((a > b)? a : b)

static char *gzip_suffixes[] = {
   ".gz", ".taz", ".tgz", ".z"
};

/**
 * Convert codepage to dos-866 for non-english languages
 */
char* char_to_oem(char* input){
#if defined _WIN32
   char *output;
   output = malloc(strlen(input)+1);
   CharToOem(input, output);
   return output;
#else
   return input;
#endif
}
void char_to_oem_free(char* input){
#if defined _WIN32
   free(input);
#endif
}
void print_stat(float stat_clocks, float stat_ratio, char* filename, char* out_filename, short flag_multi) {
   char *conv_filename, *conv_outfile;

   conv_filename = char_to_oem(filename);
   if(opt_perf_stat) {
      fprintf(stderr, "%s: %s:   ", prog_name, conv_filename);
      if(opt_decompress)
         fprintf(stderr, "inflating -- ");
      else
         fprintf(stderr, "deflating -- ");
      print_io_methods();
      if(flag_multi)
         fprintf(stderr, " MT -- ");
      else
         fprintf(stderr, " ST -- ");
      fprintf(stderr, "%6.1f clocks per input symbol\n", stat_clocks);
   }
   if(opt_verbose && !from_stdin && !opt_to_stdout) {
      conv_outfile = char_to_oem(out_filename);
      fprintf(stderr, "%s: %s:  %6.2f%% -- replaced with %s\n", prog_name, conv_filename, stat_ratio, conv_outfile);
      char_to_oem_free(conv_outfile);
   }
   char_to_oem_free(conv_filename);
}
/**
 * Prints current program name on stderr
 */
void print_progname()
{
   fprintf(stderr, "%s: ", prog_name);
}
/**
 * Produces "progname: text" message on stderr
 * @param text
 */
void warn_message(char *filename, char *text)
{
   char *conv_filename;
#if defined(_WIN32)
   DWORD last_error = GetLastError();
#endif
   conv_filename = char_to_oem(filename);
   print_progname();
#if defined(_WIN32)
   if (last_error != ERROR_SUCCESS)
       fprintf(stderr, "%s:   %s (GetLastError code %d)\n", conv_filename, text, last_error);
   else
       fprintf(stderr, "%s:   %s\n", conv_filename, text);
#else
   fprintf(stderr, "%s:   %s\n", conv_filename, text);
#endif
   char_to_oem_free(conv_filename);
}
/**
 * Program name separation function
 * @param full_name
 *
 * @return char*
 */
char *get_program_name(char *full_name)
{
   char *ptr1, *ptr2;

   ptr1 = strrchr(full_name, FNAME_SEPARATOR_1);
   ptr2 = strrchr(full_name, FNAME_SEPARATOR_2);
   if(!ptr1 && !ptr2)
      return full_name;
   return maxptr(ptr1, ptr2)+1;
}
/**
 * Error during file i/o occurred
 * @param filename
 */
void error_file(char *filename)
{
   char* conv_filename = char_to_oem(filename);
   print_progname();
   perror((const char*)conv_filename);
   char_to_oem_free(conv_filename);
   ret_status = STATUS_ERR;
}
/* Couple of memory management functions to simplify memory handling */
MEM_ENTRY* zalloc_mem_head(void)
{
   MEM_ENTRY* pnew_list = malloc(sizeof(MEM_ENTRY));
   pnew_list->p_allocated_mem = pnew_list->p_next = NULL;
   return pnew_list;
}
void* zalloc_mem(size_t size, MEM_ENTRY* p_mem_list)
{
   MEM_ENTRY* p_local = p_mem_list;
   while(p_local->p_next != NULL)
      p_local = p_local->p_next; /* Find the tail */
   if(p_local->p_allocated_mem == NULL) {
      p_local->p_allocated_mem = malloc(size);
      return p_local->p_allocated_mem;
   } else {
      MEM_ENTRY* p_new_entry = malloc(sizeof(MEM_ENTRY));
      p_new_entry->p_next = NULL;
      p_new_entry->p_allocated_mem = malloc(size);
      p_local->p_next = p_new_entry;
      return p_new_entry->p_allocated_mem;
   }
}
void zfree_all(MEM_ENTRY* p_list)
{
   while(p_list != NULL) {
      MEM_ENTRY* p_next = p_list->p_next;
      if(p_list->p_allocated_mem != NULL)
         free(p_list->p_allocated_mem);
      free(p_list);
      p_list = p_next;
   }
}

/**
 * Constructs output filename from input filename and possible
 * new suffix if specified by command line option
 * @param i_name - input file name
 * @param ou_name - output file name
 */
void create_output_name(char *i_name, char *ou_name)
{
   strcpy(ou_name, i_name);
   if(!opt_new_suffix)                /* If new suffix is not specified */
      strcat(ou_name, ".gz");
   else
      strcat(ou_name, suffix);        /* Add new suffix specified in command line */
}
/**
 * Creates standard gzip-like header and returns pointer to end
 * of header (before possible comment field)
 * @param out - pointer to output buffer
 * @param filename - input filename
 * @param istat - file statistics structure
 * @param comment_needed - 1 if we need to put comment into
 *                       header
 *
 * @return char*
 */
UBYTE *create_gzip_hdr(UBYTE *out, char *filename, STAT_TYPE *istat, int comment_needed)
{
   UBYTE flag_byte, xfl_byte;

   *out++ = GZIP_HDR_1;
   *out++ = GZIP_HDR_2;
   *out++ = DEFLATE;
   flag_byte = 0;
   if(!opt_noname) {
      flag_byte |= FNAME;
   }
   if(comment_needed) {
      flag_byte |= FCOMMENT;
   }
   *out++ = flag_byte;
   PUT_LONG(out, istat->st_mtime);

   xfl_byte = 0;       /* Extra flags */
   if(opt_compress_fast) {
      xfl_byte |= XFL_FAST;
   }
   if(opt_compress_best) {
      xfl_byte |= XFL_BEST;
   }
   *out++ = xfl_byte;
   *out++ = OS_BYTE;
   if(!opt_noname) {              /* We must put original filename into the header */
      while(*out++ = *filename++)
         ;
   }
   return out;
}

/*! \brief Contains options description */
struct option_descr {
   char *long_name;               /**< Long option name */
   char short_name;               /**< Option character */
   short *flag_to_set;     /**< Integer flag to set if option encountered */
   char **modifier;               /**< Additional string to option */
   char *description;             /**< Explanation for help() function */
};
/*! \brief Array containing all available option descriptions */
struct option_descr all_options[] = {
/*    { "ascii",      'a', &opt_ascii, NULL,           "make text mode ASCII (NOT IMPLEMENTED)" }, */
   { "to-stdout",  'c', &opt_to_stdout, NULL,   "output to stdout"},
   { "stdout",     'c', &opt_to_stdout, NULL,      "       the same"},
   { "decompress", 'd', &opt_decompress, NULL, "decompress"},
   { "uncompress", 'd', &opt_decompress, NULL, "       the same"},
/*    { "encrypt",    'e', &opt_encrypt, NULL,       "encrypt (NOT IMPLEMENTED)" }, */
   { "force",      'f', &opt_force, NULL,           "force output file overwrite"},
   { "help",       'h', &opt_help, NULL,             "print this text"},
/*    { "pkzip",      'k', &opt_pkzip, NULL,           "force PKZIP compatibility (NOT IMPLEMENTED)" }, */
   { "list",       'l', &opt_list, NULL,             "list content of gzip archive"},
/*    { "license",    'L', &opt_license, NULL,       "print license message" }, */
   { "no-name",    'n', &opt_noname, NULL,        "don't save/restore original filename/time"},
   { "name",       'N', &opt_name, NULL,             "save/restore original filename/time"},
/*    { "quiet",      'q', &opt_quiet, NULL,           "quiet/silent mode" }, */
/*    { "silent",     'q', &opt_quiet, NULL,          "       the same" }, */
   { "recursive",  'r', &opt_reqursive, NULL,   "recurse into directories"},
   { "suffix",     'S', &opt_new_suffix, &suffix,     "make new suffix instead of .gz"},
   { "test",       't', &opt_test, NULL,             "test archive integrity"},
   { "no-time",    'T', &opt_notime, NULL,        "don't save/restore file date/timestamp"},
   { "verbose",    'v', &opt_verbose, NULL,       "print some intermediate information"},
   { "version",    'V', &opt_version, NULL,       "print current version number"},
   { "fast",       '1', &opt_compress_fast, NULL,    "use faster compression method"},
   { "best",       '9', &opt_compress_best, NULL,    "use best compression method"},
/*    { "lzw",        'Z', &opt_lzw_compat, NULL,        "enable LZW compatibility (NOT IMPLEMENTED" }, */
/*    { "bits",       'b', &opt_bits, NULL,             "make number of bits per source byte" }, */
/*    { "rsyncable",  'R', &opt_rsyncable, NULL,   "make rsync-enabled archive" } */

   /* Debugging options */
   { "num-threads", 'm', &opt_dbg_multithreads, &opt_num_threads_forced, "DEBUG: set number of threads to create"},
   { "force-dynamic", 'b', &opt_force_dynamic, NULL, "force dynamic Huffman coding"},
   { "min-size",       'j',    &opt_dbg_slice_size,    &opt_slice_size_array,
      "DEBUG: set minimum input file length to slice\nInput/Output options:"},
   { "DEBUG",          'D', &opt_debug, NULL, "DEBUG mode: prints some debug info"},
   /* Input/output options */
   { "write-method",   'w',    &opt_io_write_method,   &ptr_write_method,  "I/O: write method to use -w 0 (low/level) -w 1 (mmap)"},
   { "read-method",    'y',    &opt_io_read_method,    &ptr_read_method,   "I/O: read method to use -y 0 (low/level) -y 1 (mmap)"},
   { "read-buffer",    'u',    &opt_io_read_buffer,    &ptr_read_buffer,   "I/O: read buffer size in Kbytes"},
   { "write-buffer",   'i',    &opt_io_write_buffer,   &ptr_write_buffer,  "I/O: write buffer size in Kbytes"},
   { "stat",           's',    &opt_perf_stat,         NULL,               "display performance statistics"},
   { "clocks",           'C',    &opt_clocks,         NULL,               "display CPU clocks"}
};
/**
 * Prints list of options currently available
 */
void help()
{
   int i;

   fprintf(stderr, "Usage: %s [OPTION]... [FILE]...\nCompress or uncompress FILEs (by default, compress FILES in-place)\n\n", prog_name);
   fprintf(stderr, "Mandatory arguments to long options are mandatory for short options too.\n");
   for(i=0; i<sizeof(all_options)/sizeof(struct option_descr); i++) {
      fprintf(stderr, "\t-%c, --%s\t%s\n", all_options[i].short_name, all_options[i].long_name, all_options[i].description);
   }
}

static char unknown_option[] = "Unknown option '%s', use '-h' for help\n";  /**< Unknown option message format string */

/**
 * Processes all command line options, returns index of first
 * filename in command line filename argument
 * @param argc
 * @param argv
 *
 * @return int
 */
int get_options(int argc, char *argv[])
{
   int i, j;
   char *ptr;
   unsigned short option_ok = 0;

   for(i=1; i<argc; i++) {
      ptr = argv[i];
      if(*ptr == '-' && *(ptr+1) == '-') {           /* long option name found */
         option_ok = 0;
         for(j=0; j<sizeof(all_options)/sizeof(struct option_descr); j++) {
            if(strcmp(all_options[j].long_name, ptr+2) == 0) {
               (*all_options[j].flag_to_set)++;
               if(all_options[j].modifier) {
                  *all_options[j].modifier = argv[++i];
               }
               option_ok = 1;
               break;
            }
         }
         if(!option_ok) {
            fprintf(stderr, unknown_option, argv[i]);
         }
      } else if(*ptr == '-') {                       /* short option name */
         for(j=0; j<sizeof(all_options)/sizeof(struct option_descr); j++) {
            option_ok = 0;
            if(*(ptr+1) == all_options[j].short_name) {
               (*all_options[j].flag_to_set)++;
               if(all_options[j].modifier) {
                  *all_options[j].modifier = argv[++i];
               }
               option_ok = 1;
               break;
            }
         }
         if(!option_ok) {
            fprintf(stderr, unknown_option, argv[i]);
         }
      } else {                                    /* start of filenames */
         return i;
      }
   }
   return argc;        /* no filename found */
}
/**
 * Get basename (no paths, no .gz extension) from filename.
 * Store basename in "basename"
 * @param filename
 * @param basename
 */
void get_base_name(char *filename, char *basename)
{
   char *name_start;
   char *ptr;

   ptr = strrchr((const char*)filename, FNAME_SEPARATOR_1);
   name_start = maxptr(ptr, strrchr((const char*)filename, FNAME_SEPARATOR_2));
   if(name_start == NULL) {   /* i.e. no path separators in the name */
      name_start = filename;
   } else {
      name_start++;
   }
   *basename = '\0';
   for(ptr=name_start; *ptr; ptr++) {
      if(strcmp((const char*)ptr, ".gz") == 0) {
         *basename = '\0';
         return;
      } else if(strcmp((const char*)ptr, ".tgz") == 0 || strcmp((const char*)ptr, ".taz")==0) {
         strcpy(basename, ".tar");
         return;
      }
      *basename++ = *ptr;
   }
   *basename = '\0';
   return;
}
/**
 * Returns filename without suffix.
 * Returns 0 if name has no suffix.
 * @param filename
 * @param nosuf_name
 *
 * @return int
 */
int get_nosuf_name(char *filename, char *nosuf_name)
{
   size_t str_size = strlen(filename);
   int i;

   strcpy(nosuf_name, filename);

   for(i = str_size; i > 0; i--) {
     if(nosuf_name[i] == '.')
     {
        nosuf_name[i] = '\0';
        return 1;
     }

     if(nosuf_name[i] == '/' || nosuf_name[i] == '\\')
       return 0;
   }
   return 0;
}
/**
 * Restore basename for output file
 * Take input filename and substitute it with filename stored in
 * gzip archive
 * @param filename
 * @param orig_name
 * @param mod_name
 */
void restore_output_name(char *filename, char *orig_name, char *mod_name)
{
   char *fname_separator;
   char *ptr;

   strcpy(mod_name, filename);
   ptr = strrchr((const char*)mod_name, FNAME_SEPARATOR_1);
   fname_separator = maxptr(ptr, strrchr((const char*)mod_name, FNAME_SEPARATOR_2));
   if(fname_separator == NULL) {      /* No directory name is specified */
      strcpy(mod_name, orig_name);
   } else {
      fname_separator++;
      *fname_separator = '\0';        /* Cut filename */
      strcat(mod_name, orig_name);
   }
}
/**
 *
 * @param template_name
 * @param chunk
 * @param output_name
 *
 * @return char*
 */
char *mk_chunk_name(char *template_name, int chunk, char *output_name)
{
#ifndef _WIN32
   char basename[PATH_MAX];
   get_base_name(template_name, basename);

   sprintf(output_name, "/tmp/%s.%02d", basename, chunk);
#else
   sprintf(output_name, "%s.%02d", template_name, chunk);
#endif
   return output_name;
}
int remove_tmp_file(char *template_name, int chunk)
{
   int status;
   char tmp_name[MAX_NAME];

   status = UNLINK(mk_chunk_name(template_name, chunk, tmp_name));
   if(status == -1)
      perror("Remove file");
   return status;
}
/**
 * Print status of last call to IPP library
 * @param status
 */
void print_ipp_status(IppStatus status)
{
   fprintf(stderr, "IPP status: %s\n", ippGetStatusString(status));
}
/**
 * Create output file.
 * @param filename
 *
 * @return FILE* - file stream of open file. -1 if not open.
 */
GZIP_FILE *out_open(char *filename, SIZE_TYPE initial_size)
{
   int answer;
   char* conv_name;
   STAT_TYPE chk_stat;

   if(SYSSTAT(filename, &chk_stat) == 0) {
      if(!opt_force) {
         conv_name = char_to_oem(filename);
         fprintf(stderr, "%s: %s already exists; do you wish to overwrite (y or n)? ", prog_name, conv_name);
         char_to_oem_free(conv_name);
         answer = fgetc(stdin);
         if(tolower(answer) != 'y') {
            fprintf(stderr, "        not overwritten\n");
            return NULL;
         }
      }
      if (UNLINK(filename) != 0) {
         fprintf(stderr, "        cannot delete\n");
         return NULL;
      }
   }
   return ZOPEN(filename, GZIP_FILE_WRITE, opt_io_write_method, initial_size);
}
static char *list_title = "compressed        uncompressed  ratio uncompressed_name";
static char *list_title_verbose = "method  crc      date   time           compressed      uncompressed  ratio uncompressed_name";
static char *year="JanFebMarAprMayJunJulAugSepOctNovDec";
static short list_header = 0;
void show_list(char *filename, char *orig_name, GZIP_FILE *file, long time_stamp)
{
   Ipp32u      file_crc;
   SIZE_TYPE   file_len;
   UBYTE       file_tail[8];
   UBYTE       *tail_ptr;
   STAT_TYPE   file_info;
   char        *orig_fname, *conv_name;
#if defined(VS2005_PLUS)
   struct tm ltime;
#else
   struct tm *ltime;
#endif

   if(SYSSTAT((const char*)filename, &file_info) == (-1L)) {
      return;
   }
   if(zseek(file, -8, SEEK_END) == 0) {
      error_file(filename);
      return;
   }
   if(zread(file, file_tail, 8) != 8) {
      error_file(filename);
      return;
   }
   tail_ptr = file_tail;
   file_crc = GET_LONG(tail_ptr); tail_ptr += 4;
   file_len = GET_LONG(tail_ptr); tail_ptr += 4;
   if(orig_name!=NULL)
      orig_fname = orig_name;
   else
      orig_fname = filename;
   conv_name = char_to_oem(orig_fname);
   if(opt_verbose) {
#if defined(VS2005_PLUS)
      _localtime32_s(&ltime, (const __time32_t*)&time_stamp);
#else
      ltime = localtime((const time_t*)&time_stamp);
#endif
      if(!list_header) {
         fprintf(stdout, "%s\n", list_title_verbose);
         list_header = 1;
      }
#if defined(VS2005_PLUS)
      fprintf(stdout, "deflate %08x %3.3s %02d %02d:%02d %19lld%20lld%6.1f%% %s\n", file_crc, year+ltime.tm_mon*3,
              ltime.tm_mday, ltime.tm_hour, ltime.tm_min, file_info.st_size,
              file_len, 100*(1.-(float)file_info.st_size/file_len), conv_name);
#else
      fprintf(stdout, "defla %08x %3.3s %02d %02d:%02d %19lld%20lld %6.1f%% %s\n",
              file_crc, year+ltime->tm_mon*3, ltime->tm_mday, ltime->tm_hour, ltime->tm_min,
              file_info.st_size, file_len, 100*(1.-(float)file_info.st_size/file_len), char_to_oem(orig_fname));
#endif
   } else {
      if(!list_header) {
         fprintf(stdout, "%64s\n", list_title);
         list_header = 1;
      }
      fprintf(stdout, "%19llu%20llu %6.1f%% %s\n", file_info.st_size, file_len,
                                            100*(1.-(float)file_info.st_size/file_len), conv_name);
   }
   char_to_oem_free(conv_name);
}

/**
 * return 1 if 'filename' already has one of the GZIP suffixes
 * @param filename
 *
 * @return int
 */
int known_suffix(char *filename)
{
   int i;
   char *ptr, *conv_name;

   for(i=0; i<sizeof(gzip_suffixes)/sizeof(char*); i++) {
      size_t str_size;
      ptr = strstr(filename, gzip_suffixes[i]);
      if(ptr != NULL) {
         str_size = strlen(ptr);
         if(str_size == strlen(gzip_suffixes[i])) {
            conv_name = char_to_oem(filename);
            fprintf(stderr, "%s: %s already has %s suffix -- unchanged\n", prog_name, conv_name, gzip_suffixes[i]);
            char_to_oem_free(conv_name);
            return 1;
         }
      }
   }
   return 0;
}
/**
 * return 1 if "fd" is a regular file
 *
 * @param fd
 *
 * @return int
 */
int is_file(int fd)
{
   STAT_TYPE file_info;
#if defined(_WIN32)
   if(_fstat64(fd, &file_info) == 0)
      return file_info.st_mode & _S_IFREG;
#else
   if(fstat64(fd, &file_info) == 0)
      return S_ISREG(file_info.st_mode);
#endif
   return 0;
}
/**
 * Print Windows system error code
 * @param text
 */
void error_system(char *text) {
   print_progname();
#ifdef _WIN32
   fprintf(stderr, "system error code for '%s' %lu\n", text, GetLastError());
#else
   fprintf(stderr, "system error code for '%s' %d\n", text, errno);
#endif
   ret_status = STATUS_ERR;
}
/**
 * Print out used i/o method to stderr
 */
void print_io_methods()
{
   char *p_m_read, *p_m_write;
   switch(opt_io_read_method) {
   case METHOD_LOW:
      p_m_read = "RL";
      break;
   case METHOD_MMAP:
      p_m_read = "RM";
      break;
   default:
      p_m_read = "RU";
      break;
   }
   switch(opt_io_write_method) {
   case METHOD_LOW:
      p_m_write = "WL";
      break;
   case METHOD_MMAP:
      p_m_write = "WM";
      break;
   default:
      p_m_write = "WU";
      break;
   }
   fprintf(stderr, "%s/%s", p_m_read, p_m_write);
}
