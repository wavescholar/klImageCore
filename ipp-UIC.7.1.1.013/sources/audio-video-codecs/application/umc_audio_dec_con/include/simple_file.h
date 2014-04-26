/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SIMPLE_FILE_H__
#define __SIMPLE_FILE_H__

#include "audio_file.h"
#include "vm_types.h"

class SimpleFile: public UMC::AudioFile
{
public:
  SimpleFile();
  virtual ~SimpleFile();

  virtual UMC::Status Open(vm_char* p_filename, Ipp32u mode);
  virtual UMC::Status Close();
  virtual size_t Write(void * p_data, size_t size);
  virtual size_t Read(void * p_data, size_t size);

protected:
  void *  m_file_handle;
  Ipp32u  m_open_mode;
  Ipp32s  m_is_first_time;
};

#endif // __SIMPLE_FILE_H__
