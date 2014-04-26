
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
#define __DRIVER__
#include "ippsexp.h"



void __cdecl exit(int status)
{
  TRACE(("IPPSEXP.SYS: entering exit\n"));
  return;
} // exit()

void* __cdecl realloc( void *memblock, size_t size )
{
 void* ptr = NULL;

  TRACE(("IPPSEXP.SYS: entering realloc\n"));

return ptr;
}

void* __cdecl calloc( size_t num, size_t size )
{
  void* ptr = NULL;

  TRACE(("IPPSEXP.SYS: entering calloc\n"));

return ptr;
}


void* __cdecl malloc(size_t size)
{
  void* ptr = NULL;

  TRACE(("IPPSEXP.SYS: entering malloc\n"));

  ptr = ExAllocatePool(NonPagedPool,(ULONG)size);

  TRACE(("IPPSEXP.SYS: allocate %d bytes at 0x%08X\n",size,ptr));

  TRACE(("IPPSEXP.SYS: leave malloc\n"));

  return ptr;
} // malloc()


void __cdecl free(void* ptr)
{
  TRACE(("IPPSEXP.SYS: entering free\n"));

  ExFreePool(ptr);

  TRACE(("IPPSEXP.SYS: free memory block at 0x%08X\n",ptr));

  TRACE(("IPPSEXP.SYS: leave free\n"));

  return;
} // free()

