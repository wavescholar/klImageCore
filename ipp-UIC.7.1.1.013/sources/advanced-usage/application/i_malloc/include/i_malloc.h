/* ////////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory 
//  of your Intel(R) IPP product installation for more information.
//
//
*/

/**
 * This header file describes how memory allocation can be replaced
 * in those Intel(R) libraries which support this feature. It contains
 * all declarations required by an application developer to replace
 * the memory allocation.
 *
 * Intel(R) libraries supporting this feature only use the following
 * functions to allocate or free memory:
 * - malloc
 * - calloc
 * - realloc
 * - free
 * and call those functions via globally visibible function pointers:
 * - i_malloc
 * - i_calloc
 * - i_realloc
 * - i_free
 *
 * C++ new and delete operators are changed to also use these function
 * pointers, if they occur in a library at all. No Intel(R) library
 * supporting memory allocation replacement will allocate memory
 * before it is invoked explicitly by the application for the first time.
 *
 * Therefore an application can safely set these function pointers
 * at the very beginning of its execution to some other replacement
 * functions. The function pointers must remain valid while Intel(R)
 * libraries are in use.
 *
 * Setting these pointers is optional because the copies contained in
 * Intel(R) libraries point to the standard C library functions by
 * default.
 *
 * On Windows(R) data exported by a DLL and data contained in a static
 * library are accessed differently. To support mixing static
 * libraries and DLLs, the function pointers exist in two sets with
 * different names so that the application can override both sets in
 * the same source file without running into name conflicts.
 *
 * Here is an example:
\verbatim

   #include <i_malloc.h>
   #include <my_malloc.h>

   int main( int argc, int argv )
   {
       // override normal pointers
       i_malloc = my_malloc;
       i_calloc = my_calloc;
       i_realloc = my_realloc;
       i_free = my_free;

   #ifdef _WIN32_
       // also override pointers used by DLLs
       i_malloc_dll = my_malloc;
       i_calloc_dll = my_calloc;
       i_realloc_dll = my_realloc;
       i_free_dll = my_free;
   #endif

       // now start using Intel(R) libraries
   }
\endverbatim
 */

#ifndef _I_MALLOC_H_
#define _I_MALLOC_H_

#include <stdlib.h>     /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs for all four function pointers */
typedef void * (* i_malloc_t)(size_t size);
typedef void * (* i_calloc_t)(size_t nmemb, size_t size);
typedef void * (* i_realloc_t)(void *ptr, size_t size);
typedef void (* i_free_t)(void *ptr);

#ifdef WIN32

# ifdef INTEL_DLL_EXPORTS
#  define INTEL_API_DEF __declspec(dllexport)
# else
#  define INTEL_API_DEF __declspec(dllimport)
#endif

/* function pointers as used and exported by a DLL */
extern INTEL_API_DEF i_malloc_t i_malloc_dll;
extern INTEL_API_DEF i_calloc_t i_calloc_dll;
extern INTEL_API_DEF i_realloc_t i_realloc_dll;
extern INTEL_API_DEF i_free_t i_free_dll;

#else /* WIN32 */

# define INTEL_API_DEF

#endif /* WIN32 */

/* normal function pointers for static libraries */
extern i_malloc_t i_malloc;
extern i_calloc_t i_calloc;
extern i_realloc_t i_realloc;
extern i_free_t i_free;

#ifdef __cplusplus
}
#endif

#endif /* _I_MALLOC_H_ */
