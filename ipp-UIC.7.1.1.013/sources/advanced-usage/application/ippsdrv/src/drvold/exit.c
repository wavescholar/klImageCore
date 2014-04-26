
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1998-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include <ntddk.h>



void __cdecl exit(int status)
{
  return;
} // exit()


void* __cdecl malloc(size_t size)
{
  void* ptr = NULL;
  ptr = ExAllocatePool(NonPagedPool,(ULONG)size);
  return ptr;
} // malloc()


void __cdecl free(void* ptr)
{
  ExFreePool(ptr);
  return;
} // free()


void* __cdecl realloc( void *memblock, size_t size )
{
 void* ptr = NULL;
 return ptr;
}

void* __cdecl calloc( size_t num, size_t size )
{
  void* ptr = NULL;
  return ptr;
}
