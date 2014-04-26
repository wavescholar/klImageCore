/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#include <stdio.h>
#include <memory.h>
#include "simple_file.h"

SimpleFile::SimpleFile()
{
  m_is_first_time = 1;
  m_file_handle = NULL;
}

SimpleFile::~SimpleFile()
{
}

Ipp32s SimpleFile::Open(vm_char* p_filename, Ipp32u mode)
{
  vm_file* p_file = NULL;

  if (mode & AFM_CREATE) {
    p_file = vm_file_open(p_filename, VM_STRING("wb"));
  } else {
    p_file = vm_file_open(p_filename, VM_STRING("rb"));
  }

  if (p_file == NULL)
  {
    return -1;
  }

  m_file_handle = p_file;

  return 0;
}

size_t SimpleFile::Read(void * p_data, size_t size)
{
  size_t      n;

  if (m_file_handle == NULL)
  {
    return 0;
  }

  n = vm_file_read(p_data,1,size,(vm_file*)m_file_handle);

  return (Ipp32s)n;
}

size_t SimpleFile::Write(void * p_data, size_t size)
{
  size_t      n;

  if (m_file_handle == NULL)
  {
    return 0;
  }

  n = vm_file_write(p_data,1,size,(vm_file*)m_file_handle);

  return n;
}

Ipp32s SimpleFile::Close()
{
  if (m_file_handle == NULL)
  {
    return -1;
  }

  vm_file_close((vm_file*)m_file_handle);
  return 0;
}
