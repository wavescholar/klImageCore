/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AUDIO_FILE_H__
#define __AUDIO_FILE_H__

#include "vm_types.h"
#include "umc_structures.h"

namespace UMC
{
    class AudioFile
    {
    public:
        enum
        {
            AFM_DEFAULT        = 0x0000,
            AFM_CREATE         = 0x0001,
            AFM_READ           = 0x0002,
            AFM_WRITE          = 0x0004,
            AFM_APPEND         = 0x000D, // CREATE+WRITE+APPEND
            AFM_NO_CONTENT_WRN = 0xF000
        };

    public:
        virtual Status Open(vm_char* p_filename, Ipp32u mode = AFM_DEFAULT) = 0;
        virtual Status Close() = 0;
        virtual Ipp32s Write(void* p_data, size_t size) = 0;
        virtual Ipp32s Read(void* p_data, size_t size) = 0;
    };

} /* namespace UMC */

#endif // __AUDIO_FILE_H__
