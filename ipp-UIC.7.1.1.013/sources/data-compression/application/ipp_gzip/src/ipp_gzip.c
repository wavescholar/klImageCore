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
#ifdef _WIN32
   #include <windows.h>
#else
   #include <dirent.h>
   #include <unistd.h>
#endif


#ifdef GZIP_VMTHREADS
   #include <vm_thread.h>
   #include <vm_sys_info.h>
   #include <vm_semaphore.h>
#endif

#include "version.h"
#include "ipp_gzip.h"
#include "ipps.h"
#include "ippdc.h"

/* Global variables */
short opt_ascii = 0;
short opt_to_stdout = 0;
short opt_decompress = 0;
short opt_encrypt = 0;
short opt_force = 0;
short opt_help = 0;
short opt_pkzip = 0;
short opt_list = 0;
short opt_license = 0;
short opt_noname = 0;
short opt_name = 0;
short opt_quiet = 0;
short opt_reqursive = 0;
short opt_new_suffix = 0;
short opt_test = 0;
short opt_notime = 0;
short opt_verbose = 0;
short opt_version = 0;
short opt_compress_fast = 0;
short opt_compress_best = 0;
short opt_lzw_compat = 0;
short opt_bits = 0;
short opt_rsyncable = 0;
short opt_force_dynamic = 0;
short opt_clocks = 0;
char *prog_name = NULL;
char *suffix = NULL;

short opt_dbg_multithreads = 0;
char *opt_num_threads_forced = NULL;
short opt_dbg_slice_size = 0;
char *opt_slice_size_array = NULL;
long opt_slice_size = 0;
short from_stdin = 0;
short opt_perf_stat = 0;

/* Input/output options storage */
short opt_io_read_method = METHOD_LOW;
char *ptr_read_method = NULL;
short opt_io_write_method = METHOD_LOW;
char *ptr_write_method = NULL;
short opt_io_read_buffer = 64;
size_t INBUF_SIZE = 64 * 1024;
char *ptr_read_buffer = NULL;
short opt_io_write_buffer = 64;
size_t OUTBUF_SIZE = 64 * 1024;
char *ptr_write_buffer = NULL;
short opt_debug = 0;

#ifdef _WIN32
SYSTEM_INFO sysInfo;
#else
size_t sys_page_size;
#endif

/* Local variables */
int num_threads = 1;
#ifdef GZIP_VMTHREADS
static vm_semaphore *semaphore;
static void VM_THREAD_CALLCONVENTION process_file_st(void*);
#else
static void process_file_st(char*);
#endif
static void process_dir(char*, int);
static void process_file_mt(char*);
static int is_dir(char*);
/* Program statuses */
int ret_status = STATUS_OK;
int main(int argc, char *argv[]) {
   int i;
   int file_arg_start;
   int file_count;
   int is_dir_flag;
#ifdef GZIP_VMTHREADS
   int dbg_multithreads, real_files = 0;
   vm_thread* thread_vec;
#endif

   prog_name = get_program_name(argv[0]);
   /* Simple option parser */
   file_arg_start = get_options(argc, argv);
   file_count = argc - file_arg_start;
   if(opt_help) {
      help();
      exit(0);
   }
   if(opt_ascii && !opt_quiet) {
      fprintf(stderr, "%s: 'ascii' is ignored in this version\n", prog_name);
   }
   if(opt_encrypt && !opt_quiet) {
      fprintf(stderr, "%s: 'encrypt' is ignored in this version\n", prog_name);
   }
   if(opt_pkzip && !opt_quiet) {
      fprintf(stderr, "%s: 'pkzip' is ignored in this version\n", prog_name);
   }
   /* Connect to proper processor-type-related code if ipp_gzip is linked statically */
   ippStaticInit();
   if(opt_version) {
      print_version();
      exit(0);
   }
   /* Decode input/output options */
#ifdef _WIN32
   GetSystemInfo(&sysInfo);            /* Get OS file map granularity */
#else
   sys_page_size = (size_t)sysconf(_SC_PAGESIZE);
#endif
   if(ptr_read_buffer != NULL) {      /* Read buffer size is specified */
      opt_io_read_buffer = (short)atoi(ptr_read_buffer);
      if(opt_io_read_buffer == 0 || opt_io_read_buffer > 1024)    /* If something wrong is specified */
         opt_io_read_buffer = 64;
      INBUF_SIZE = opt_io_read_buffer * 1024;
   }
   if(ptr_write_buffer != NULL) {
      opt_io_write_buffer = (short)atoi(ptr_write_buffer);
      if(opt_io_write_buffer == 0 || opt_io_write_buffer > 1024)
         opt_io_write_buffer = 64;
      OUTBUF_SIZE = opt_io_write_buffer * 1024;
   }
   if(ptr_read_method != NULL) {
      opt_io_read_method = (short)atoi(ptr_read_method);
      if(opt_io_read_method < METHOD_LOW)
         opt_io_read_method = METHOD_LOW;         /* Low/level by default */
      if(opt_io_read_method > METHOD_MMAP)
         opt_io_read_method = METHOD_LOW;         /* Low/level by default */
      if(opt_io_read_method == METHOD_MMAP) {
#ifdef _WIN32
         /* Align buffer size to system granularity boundary */
         INBUF_SIZE = (INBUF_SIZE/sysInfo.dwAllocationGranularity+1)*sysInfo.dwAllocationGranularity;
#else
         INBUF_SIZE = (INBUF_SIZE/sys_page_size + 1) * sys_page_size;
#endif /* _WIN32 */
      }
   }
   if(ptr_write_method != NULL) {
      opt_io_write_method = (short)atoi(ptr_write_method);
      if(opt_io_write_method < METHOD_LOW)
         opt_io_write_method = METHOD_LOW;        /* low-level by default */
       if (opt_io_write_method > METHOD_MMAP)
         opt_io_write_method = METHOD_LOW;        /* low-level by default */
       if(opt_io_write_method == METHOD_MMAP) {
#ifdef _WIN32
         /* Align buffer size to system granularity boundary */
         OUTBUF_SIZE = (OUTBUF_SIZE/sysInfo.dwAllocationGranularity+1)*sysInfo.dwAllocationGranularity;
#else
         OUTBUF_SIZE = (OUTBUF_SIZE/sys_page_size + 1) * sys_page_size;
#endif
      }
   }
   if(file_count == 0) {
      if(!opt_force && !opt_test && !opt_list) {
         fprintf(stderr, "%s:   compressed data not written to a terminal. Use -f to force compression.\n", prog_name);
         fprintf(stderr, "For help, type: %s -h\n", prog_name);
         exit(0);
      }
      from_stdin = 1;
      opt_to_stdout = 1;
      opt_noname = 1;
      opt_notime = 1;
   }

   /* Determine number of available threads. Depending on it we'll choose between single thread and multi thread processing */
   if(!from_stdin && !opt_to_stdout) {
#ifdef GZIP_VMTHREADS
      if(opt_dbg_multithreads) {
         dbg_multithreads = atoi(opt_num_threads_forced);
         if(dbg_multithreads > MAX_THREADS) {
            if(!opt_decompress)
               fprintf(stderr, "%s:   too many threads requested; limited to %d\n", prog_name, MAX_THREADS);
            num_threads = MAX_THREADS;
         } else if(dbg_multithreads < 1) {
            fprintf(stderr, "%s:   too few threads requested; set to amount of cpus\n", prog_name);
            num_threads = vm_sys_info_get_cpu_num();
         } else
            num_threads = dbg_multithreads;
      } else
         num_threads = vm_sys_info_get_cpu_num();

      if(num_threads > 1) {
         semaphore = (vm_semaphore*)malloc(sizeof(vm_semaphore));
         vm_semaphore_set_invalid(semaphore);
         vm_semaphore_init(semaphore, num_threads);
         if(file_count > 1) {
            thread_vec = (vm_thread*)malloc(file_count * sizeof(vm_thread));
            real_files = 0;
            for(i=0; i<file_count; i++) {
               is_dir_flag = is_dir(argv[file_arg_start+i]);
               switch(is_dir_flag) {
               case 0:
                  vm_semaphore_wait(semaphore);
                  vm_thread_set_invalid(&thread_vec[real_files]);
                  vm_thread_create(&thread_vec[real_files], (vm_thread_callback)process_file_st, (void*)argv[file_arg_start+i]);
                  real_files++;
                  break;
               case 1:
                  process_dir(argv[file_arg_start+i], num_threads);
                  break;
               default:
                  break;
               }
            }
            /* Wait for all threads finish */
            for(i=0; i<real_files; i++) {
               vm_thread_wait(&thread_vec[i]);
               vm_thread_close(&thread_vec[i]);
            }
            free(thread_vec);
         } else {
            is_dir_flag = is_dir(argv[file_arg_start]);
            switch(is_dir_flag) {
            case 0:
               process_file_mt((void*)argv[file_arg_start]);
               break;
            case 1:
               process_dir(argv[file_arg_start], num_threads);
               break;
            default:
               break;
            }
         }
         vm_semaphore_destroy(semaphore);
         free(semaphore);
      } else {
         for(i=0; i<file_count; i++) {
            is_dir_flag = is_dir(argv[file_arg_start+i]);
            switch(is_dir_flag) {
            case 0:
               process_file_st((void*)argv[file_arg_start+i]);
               break;
            case 1:
               process_dir(argv[file_arg_start + i], 1);
               break;
            default:
               break;
            }
         }
      }
#else
      for(i=0; i<file_count; i++) {
         is_dir_flag = is_dir(argv[file_arg_start+i]);
         switch(is_dir_flag) {
         case 0:
            process_file_st(argv[file_arg_start + i]);
            break;
         case 1:
            process_dir(argv[file_arg_start + i], 1);
            break;
         default:
            break;
         }
      }
#endif
   } else {
      if(!from_stdin) {
         if(file_count > 1)
            fprintf(stderr, "%s:   only one file allowed in stdout mode\nfirst file will be process\n", prog_name);
         for(i=0; i<file_count; i++) {
            is_dir_flag = is_dir(argv[file_arg_start+i]);
            switch(is_dir_flag) {
            case 0:
               process_file_st(argv[file_arg_start + i]);
               i = file_count;
               break;
            default:
               break;
            }
         }
      } else {
         process_file_st(argv[file_arg_start]);
      }
   }
   return ret_status;
}

/*
    print_version()
        simply prints copyright message and current program version number
*/
void print_version() {
   const IppLibraryVersion* version;

   fprintf(stderr, "%s\n%s\n\n", COPYRIGHT_MESSAGE, CURRENT_VERSION);
   version = ippGetLibVersion();
   fprintf(stderr, "Core library: %s v%s (%s)\n", version->Name, version->Version, version->BuildDate);
   version = ippsGetLibVersion();
   fprintf(stderr, "  SP library: %s v%s (%s)\n", version->Name, version->Version, version->BuildDate);
   version = ippdcGetLibVersion();
   fprintf(stderr, "  DC library: %s v%s (%s)\n", version->Name, version->Version, version->BuildDate);

   if(!strcmp(version->targetCpu, "px"))
      fprintf(stderr, "  px: optimization for all IA-32 processors \n");
   else if(!strcmp(version->targetCpu, "mx"))
      fprintf(stderr, "  mx: optimization for all processors with Intel(R) 64 instructions\n");
   else if(!strcmp(version->targetCpu, "v8"))
      fprintf(stderr, "  v8: optimization for Intel(R) Core(TM) with SSSE3 \n");
   else if(!strcmp(version->targetCpu, "u8"))
      fprintf(stderr, "  u8: optimization for Intel(R) Core(TM) with SSSE3 and Intel(R) 64 instructions \n");
   else if(!strcmp(version->targetCpu, "p8"))
      fprintf(stderr, "  p8: optimization for Intel(R) Core(TM) with SSE4.1 \n");
   else if(!strcmp(version->targetCpu, "y8"))
      fprintf(stderr, "  y8: optimization for Intel(R) Core(TM) with SSE4.1 and Intel(R) 64 instructions \n");
}

static int is_dir(char* filename) {
   STAT_TYPE file_info;
   if(SYSSTAT(filename, &file_info) == -1) {
      error_file(filename);
      return -1;
   }
   /* Check if directory file is specified */
#if defined(VS2005_PLUS)
   if((file_info.st_mode & _S_IFDIR)) {
#else
   if((file_info.st_mode & S_IFDIR)) {
#endif
      if(!opt_reqursive || from_stdin || opt_to_stdout) {
         warn_message(filename, "directory ignored");
         return 2;
      }
      return 1;
   }
   return 0;
}

/*
    process_file(char *filename)
        function to process compress/decompress/list/test on a file
*/
#ifdef GZIP_VMTHREADS
static void VM_THREAD_CALLCONVENTION process_file_st(void *param)
{
   char* filename = (char*)param;
#else
static void process_file_st(char *filename) {
#endif
   Ipp64u clocks = 0;
   if(opt_clocks && !opt_to_stdout && !opt_list)
      clocks = ippGetCpuClocks();
   if(opt_decompress | opt_list | opt_test) {
      inflate_st(filename);
   } else
      deflate_st(filename);
   if(opt_clocks && !opt_to_stdout && !opt_list) {
      clocks = ippGetCpuClocks() - clocks;
      printf("clocks=%llu\n", clocks);
   }
#ifdef GZIP_VMTHREADS
   if(num_threads == 1)
      return;
   vm_semaphore_post(semaphore);
#endif
}

#ifdef GZIP_VMTHREADS
static void process_file_mt(char *filename) {
   Ipp64u clocks = 0;
   if(opt_clocks && !opt_to_stdout && !opt_list)
      clocks = ippGetCpuClocks();
   if(opt_decompress | opt_list | opt_test) {
      inflate_mt(filename);
   } else
      deflate_mt(filename);
   if(opt_clocks && !opt_to_stdout && !opt_list) {
      clocks = ippGetCpuClocks() - clocks;
      printf("clocks=%llu\n", clocks);
   }
}
#endif

/**
 * Process specified directory in single-thread.
 * Windows NT has no POSIX-like directory processing functions,
 * so it will be done using Windows API.
 * In Linux we'll use POSIX functions
 * @param dirname, num_treads
 *
 * @return int
 */
static void process_dir(char *dirname, int num_threads) {
#ifdef GZIP_VMTHREADS
   vm_thread* thread_vec_dir = NULL;
#ifdef _WIN32
   int file_count = 0, i = 0, real_files = 0;
   unsigned int full_filename_length = 0;
   char *filename_ptr = NULL;
#endif
#endif
#ifdef _WIN32
   WIN32_FIND_DATA find_file_data;
   HANDLE find_handle;
   char *full_filename = NULL, *valid_search_name;
   int next_file;
   size_t search_name_len;

   search_name_len = strlen(dirname)+3;
   valid_search_name = (char*)malloc(search_name_len);
#if defined(VS2005_PLUS)
   strcpy_s(valid_search_name, search_name_len, dirname);
   strcat_s(valid_search_name, search_name_len, "\\*");
#else
   strcpy(valid_search_name, dirname);
   strcat(valid_search_name, "\\*");
#endif
   find_handle = FindFirstFile(valid_search_name, &find_file_data);
   if(find_handle == INVALID_HANDLE_VALUE) {           /* No files in directory */
      free(valid_search_name);
      return;
   }
#ifdef GZIP_VMTHREADS
   if(num_threads > 1) {
      next_file = TRUE;
      while(next_file) {
         file_count++;
         next_file = FindNextFile(find_handle, &find_file_data);
         full_filename_length += (unsigned int)(strlen(dirname)+strlen(find_file_data.cFileName)+2);
      }
      file_count-=2;
      full_filename = (char*)malloc(full_filename_length);
      filename_ptr = full_filename;
      thread_vec_dir = (vm_thread*)malloc(file_count * sizeof(vm_thread));
      find_handle = FindFirstFile(valid_search_name, &find_file_data);
   }
#endif
   next_file = TRUE;
   while(next_file) {
      if(strcmp(".", find_file_data.cFileName) != 0 && strcmp("..", find_file_data.cFileName) != 0) {
          size_t full_name_len;

            full_name_len = (strlen(dirname)+strlen(find_file_data.cFileName))+2;
          if(num_threads <= 1)
            full_filename = (char*)malloc(full_name_len);
#if defined(VS2005_PLUS)
         strcpy_s(full_filename, full_name_len, dirname);
         strcat_s(full_filename, full_name_len, "\\");
         strcat_s(full_filename, full_name_len, find_file_data.cFileName);
#else
         strcpy(full_filename, dirname);
         strcat(full_filename, "\\");
         strcat(full_filename, find_file_data.cFileName);
#endif
         if(!is_dir(full_filename)) {
#ifdef GZIP_VMTHREADS
            if(num_threads > 1 && file_count > 1) {
               vm_semaphore_wait(semaphore);
               vm_thread_set_invalid(&thread_vec_dir[real_files]);
               vm_thread_create(&thread_vec_dir[real_files], (vm_thread_callback)process_file_st, (void*)full_filename);
               full_filename += strlen(dirname)+strlen(find_file_data.cFileName)+2;
               real_files++;
            } else
               process_file_st(full_filename);
#else
            process_file_st(full_filename);
#endif
         } else
            process_dir(full_filename, num_threads);
         if(num_threads <= 1) {
            free(full_filename);
            full_filename = NULL;
         }
      }
      next_file = FindNextFile(find_handle, &find_file_data);
   }

#ifdef GZIP_VMTHREADS
   if(num_threads > 1 && file_count > 1) {
      for(i=0; i<real_files; i++) {
         vm_thread_wait(&thread_vec_dir[i]);
         vm_thread_close(&thread_vec_dir[i]);
      }
      free(thread_vec_dir);
      thread_vec_dir = NULL;
      free(filename_ptr);
      filename_ptr = NULL;
   }
#endif

   if(GetLastError() == ERROR_NO_MORE_FILES) {
      FindClose(find_handle);
      free(valid_search_name);
      if(full_filename) free(full_filename);
#ifdef GZIP_VMTHREADS
      if(thread_vec_dir) free(thread_vec_dir);
#endif
      return;
   }
   warn_message(dirname, "directory read failure");
   FindClose(find_handle);
   free(valid_search_name);
#else  /* Non-Win32 version, i.e. Linux */
   DIR *dir_ptr;
   struct dirent *entry_ptr;
   char *full_filename;
   int file_count = 0, full_filename_length = 0, i = 0, real_files = 0;
   char *filename_ptr;

   if((dir_ptr = opendir(dirname)) == NULL) {
      perror("opendir failure");
      return;
   }
   while((entry_ptr = readdir(dir_ptr)) != NULL) {
      file_count++;
      full_filename_length += strlen(dirname)+strlen(entry_ptr->d_name)+2;
   }
   file_count-=2;
   filename_ptr = full_filename = (char*)malloc(full_filename_length);
   if((dir_ptr = opendir(dirname)) == NULL) {
      perror("opendir failure");
      return;
   }
   while((entry_ptr = readdir(dir_ptr)) != NULL) {
      if(strcmp(".", entry_ptr->d_name) == 0 || strcmp("..", entry_ptr->d_name) == 0)
         continue;
      strcpy(full_filename, dirname);
      strcat(full_filename, "/");
      strcat(full_filename, entry_ptr->d_name);
      full_filename += strlen(dirname)+strlen(entry_ptr->d_name)+2;
   }
   if((dir_ptr = opendir(dirname)) == NULL) {
      perror("opendir failure");
      return;
   }
   full_filename = filename_ptr;

#ifdef GZIP_VMTHREADS
   if(num_threads > 1 && file_count > 1)
      thread_vec_dir = (vm_thread*)malloc(file_count*sizeof(vm_thread));
#endif
   for(i=0; i<file_count; i++) {
      if(!is_dir(full_filename)) {
#ifdef GZIP_VMTHREADS
         if(num_threads > 1 && file_count > 1) {
            vm_semaphore_wait(semaphore);
            vm_thread_set_invalid(&thread_vec_dir[real_files]);
            vm_thread_create(&thread_vec_dir[real_files], (vm_thread_callback)process_file_st, (void*)full_filename);
            real_files++;
         } else {
            process_file_st(full_filename);
         }
#else
         process_file_st(full_filename);
#endif
      } else {
         process_dir(full_filename, num_threads);
      }
      full_filename += strlen(full_filename)+1;
   }

#ifdef GZIP_VMTHREADS
   if(num_threads > 1 && file_count > 1) {
      for(i=0; i<real_files; i++) {
         vm_thread_wait(&thread_vec_dir[i]);
         vm_thread_close(&thread_vec_dir[i]);
      }
      free(thread_vec_dir);
   }
#endif
   free(filename_ptr);
   closedir(dir_ptr);
#endif
    if(full_filename) free(full_filename);
#ifdef GZIP_VMTHREADS
    if(thread_vec_dir) free(thread_vec_dir);
#endif
    return;
}
