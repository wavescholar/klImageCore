//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2012 Intel Corporation. All Rights Reserved.
//
//
//
#include "rtpdump_file.h"
#include "vm_time.h"

namespace UMC
{
   #define FILE_ID "#!rtpplay1.0 "
   #define FILE_ID_LEN 13

   typedef struct {
      struct vm_timeval start;
      Ipp32u source;
      Ipp16u port;
   } RTPDumpFileHeader;

   RTPDumpFile::RTPDumpFile()
   {
      m_is_info_valid = 0;
      m_file_handle = NULL;
      mReadOnly = 0;
   }

   RTPDumpFile::~RTPDumpFile()
   {
   }

   #define UNIX_TIME_OFFSET (Ipp32u)(2208988800)
   static vm_timeval UTC1900 = {UNIX_TIME_OFFSET,0};

   Status RTPDumpFile::Open(vm_char *p_filename,
                               Ipp32u mode)
   {
      Ipp32s res;
      vm_file   *p_file = NULL;

      if (mode & AFM_CREATE) {
         p_file = vm_file_open(p_filename, VM_STRING("wb"));
      } else {
         p_file = vm_file_open(p_filename, VM_STRING("rb"));
         mReadOnly = 1;
      }
      if (p_file == NULL) {
         return UMC::UMC_ERR_OPEN_FAILED;
      }
      m_file_handle = p_file;

      if (!(mode & AFM_CREATE)) {
         res = ReadHeaderInfo();
         if (res == -1)
               return UMC::UMC_ERR_OPEN_FAILED;
         if (res < 0) {
               if (mode & AFM_NO_CONTENT_WRN) {  // if WAV header is absent return ptr
                  vm_file_fseek((vm_file *) m_file_handle, 0, VM_FILE_SEEK_SET);
                  return UMC::UMC_WRN_INVALID_STREAM;
               }
               vm_file_fclose((vm_file *) m_file_handle);
               return UMC::UMC_ERR_OPEN_FAILED;
         }
      } else {  // write mode
         char tmp[1024];
         RTPDumpFileHeader hdr;
         vm_string_sprintf(tmp,VM_STRING("%s127.0.0.1/20000\n"),FILE_ID);
         vm_file_fputs(tmp,(vm_file *)m_file_handle);
         hdr.port = BSWAP16(20000);
         hdr.source = BIG_ENDIAN_SWAP16(0x0100007F);

         vm_time_gettimeofday(&hdr.start,NULL);
         vm_time_timeradd(&hdr.start,&hdr.start,&UTC1900);

         vm_file_fwrite(&hdr,sizeof(RTPDumpFileHeader),1,(vm_file *)m_file_handle);
         m_data_size = 0;
      }
      return UMC::UMC_OK;
   }

   Ipp32s RTPDumpFile::Read(void *p_data,
                          size_t size)
   {
      Ipp32s n;

      if (m_file_handle == NULL) {
         return UMC::UMC_ERR_NULL_PTR;
      }

      n = (Ipp32s)vm_file_fread(p_data, 1, (Ipp32s)size, (vm_file *) m_file_handle);
      return n;
   }

   Ipp32s RTPDumpFile::Write(void *p_data, size_t size)
   {
      size_t n;

      if (mReadOnly) {  // can't write if the file is RO
         return 0;
      }

      if (m_file_handle == NULL) {
         return UMC::UMC_ERR_NULL_PTR;
      }

      n = vm_file_fwrite(p_data, 1, (Ipp32s)size, (vm_file *) m_file_handle);

      m_data_size += (Ipp32s)size;
      return (Ipp32s)n;
   }

   UMC::Status RTPDumpFile::Close()
   {
      if (m_file_handle == NULL) {
         return UMC::UMC_ERR_NULL_PTR;
      }

      vm_file_fclose((vm_file *) m_file_handle);
      return UMC::UMC_OK;
   }

   Ipp32s RTPDumpFile::ReadHeaderInfo()
   {
      char buffer[1024];
      RTPDumpFileHeader rdHdr;

      if(m_file_handle==NULL) {
         return UMC::UMC_ERR_NULL_PTR;
      }

      if(vm_file_fgets(buffer,1024,(vm_file *)m_file_handle)==NULL) {
         return -1;
      }

      if(vm_string_strncmp(buffer,FILE_ID,FILE_ID_LEN) != 0) {
         return -1;
      }

      if(vm_file_fread(&rdHdr, sizeof(RTPDumpFileHeader), 1, (vm_file *) m_file_handle) != 1) {
         return -1;
      }
      rdHdr.port = BSWAP16(rdHdr.port);
      rdHdr.source = BSWAP32(rdHdr.source);
      return 0;
   }
} /* namespace UMC */
