/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/
#ifndef __ALIGN_H__
#define __ALIGN_H__

#include "ippdefs.h"

#if defined(_MSC_VER)
#pragma warning(disable:4505) // warning C4505: unreferenced local function has been removed
#endif

#define __ALIGN_BYTES 32
#define __ALIGNED(a) (((a) + __ALIGN_BYTES - 1) & ~(__ALIGN_BYTES - 1))

#ifdef __INTEL_COMPILER

#define __ALIGN __declspec(align(__ALIGN_BYTES))

#else

#define __ALIGN
#endif

#if defined(__ICC) || defined( __ICL ) || defined ( __ECL )
  #define __INLINE static __inline
#elif defined( __GNUC__ )
  #define __INLINE static __inline__
#else
  #define __INLINE static
#endif


#if (defined (INTEL64) || defined(linux64) || defined(linux32e)) && !defined(_WIN32_WCE)
__INLINE
Ipp64s IPP_INT_PTR( const void* ptr )  {
    union {
        void*   Ptr;
        Ipp64s  Int;
    } dd;
    dd.Ptr = (void*)ptr;
    return dd.Int;
}
__INLINE
Ipp64u IPP_UINT_PTR( const void* ptr )  {
    union {
        void*    Ptr;
        Ipp64u   Int;
    } dd;
    dd.Ptr = (void*)ptr;
    return dd.Int;
}
#elif (defined(IA32) || defined(linux32)) && !defined(_WIN32_WCE)
__INLINE
Ipp32s IPP_INT_PTR( const void* ptr )  {
    union {
        void*   Ptr;
        Ipp32s  Int;
    } dd;
    dd.Ptr = (void*)ptr;
    return dd.Int;
}

__INLINE
Ipp32u IPP_UINT_PTR( const void* ptr )  {
    union {
        void*   Ptr;
        Ipp32u  Int;
    } dd;
    dd.Ptr = (void*)ptr;
    return dd.Int;
}
#else
  #define IPP_INT_PTR( ptr )  ( (long)(ptr) )
  #define IPP_UINT_PTR( ptr ) ( (Ipp32u long)(ptr) )
#endif

#define IPP_BYTES_TO_ALIGN(ptr, align) ((-(IPP_INT_PTR(ptr)&((align)-1)))&((align)-1))

#define IPP_ALIGNED_PTR(ptr, align) (void*)( (Ipp8s*)(ptr) + (IPP_BYTES_TO_ALIGN( ptr, align )) )

#define IPP_MALLOC_ALIGNED_BYTES  32

#define IPP_MALLOC_ALIGNED_0BYTES   0
#define IPP_MALLOC_ALIGNED_1BYTES   1
#define IPP_MALLOC_ALIGNED_8BYTES   8
#define IPP_MALLOC_ALIGNED_16BYTES 16
#define IPP_MALLOC_ALIGNED_32BYTES 32

#define IPP_ALIGNED_ARRAY(align,arrtype,arrname,arrlength)\
 arrtype arrname##AlignedArrBuff[(arrlength)+IPP_MALLOC_ALIGNED_##align##BYTES/sizeof(arrtype)];\
 arrtype *arrname = (arrtype*)IPP_ALIGNED_PTR(arrname##AlignedArrBuff,align)

#endif
