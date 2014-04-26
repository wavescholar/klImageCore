/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __WAV_FILE_H__
#define __WAV_FILE_H__

#include "audio_file.h"
#include "vm_types.h"

namespace UMC
{
    class WavFile: public AudioFile
    {
    public:
        struct Info
        {
            Ipp32u format_tag;      /// 1 = PCM
            Ipp32u sample_rate;     /// f.e. 44100
            Ipp32u resolution;      ///  8, 16, 32 bits, etc
            Ipp32u channels_number;
            Ipp32u channel_mask;
        };
        WavFile();
        ~WavFile();

        virtual Status Open(vm_char* p_filename, Ipp32u mode);
        virtual Status Close();
        virtual Ipp32s Write(void * p_data, size_t size); // number of bytes written
        virtual Ipp32s Read(void * p_data, size_t size);  // number of bytes read

        virtual Status SetInfo(Info* p_info);
        virtual Status GetInfo(Info* p_info);

    protected:
        void*       m_file_handle;
        Ipp32u      m_open_mode;
        Ipp32u      m_is_info_valid;
        Ipp32u      m_data_size;
        Info        m_info;
        Ipp32s      mReadOnly;    // not zero if the file is opened for reading only

        Ipp32s      ReadHeaderInfo();
    };
} /* namespace UMC */

#endif // __WAV_FILE_H__
