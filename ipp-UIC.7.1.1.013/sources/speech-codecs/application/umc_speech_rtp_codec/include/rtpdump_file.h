/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __RTPDUMP_FILE_H__
#define __RTPDUMP_FILE_H__

#include "audio_file.h"
#include "vm_types.h"

namespace UMC
{
   class RTPDumpFile: public AudioFile
   {
   public:
      typedef struct {
         Ipp16u pckLen;
         Ipp16u WholeLength;
         Ipp32u RecordOffsetMS;
      } RTPDumpFilePacketHeader;

      RTPDumpFile();
      ~RTPDumpFile();

      // Open file
      virtual Status Open(vm_char* p_filename, Ipp32u mode);
      // Close file
      virtual Status Close();
      // Write data to the file
      virtual Ipp32s Write(void * p_data, size_t size); // number of bytes written
      // Read data to the file
      virtual Ipp32s Read(void * p_data, size_t size);  // number of bytes read
   protected:
      void*       m_file_handle;
      Ipp32u      m_is_info_valid;
      Ipp32u      m_data_size;
      Ipp32s      mReadOnly;    // not zero if the file is opened for reading only

      Ipp32s      ReadHeaderInfo();
   };
} /* namespace UMC */

#endif // __RTPDUMP_FILE_H__
