/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_APP_UTILS_H__
#define __UMC_APP_UTILS_H__

#include "umc_defs.h"
#include "umc_string.h"
#include "umc_video_data.h"
#include "umc_data_writer.h"
#include "umc_data_reader.h"

using namespace UMC;


#if defined WINDOWS && defined _UNICODE
#define PRINT_LIB_VERSION(LIB, VER, CVT) \
    VER = ipp##LIB##GetLibVersion(); \
    vm_line_cvt((Ipp8u*)VER->Name, CVT, 255); \
    vm_string_printf(VM_STRING("  %s "), CVT); \
    vm_line_cvt((Ipp8u*)VER->Version, CVT, 255); \
    vm_string_printf(VM_STRING("%s "), CVT); \
    vm_line_cvt((Ipp8u*)VER->BuildDate, CVT, 255); \
    vm_string_printf(VM_STRING("%s \n"), CVT);
#else
#define PRINT_LIB_VERSION(LIB, VER, CVT) \
    VER = ipp##LIB##GetLibVersion(); \
    vm_string_printf(VM_STRING("  %s %s %s \n"), VER->Name, VER->Version, VER->BuildDate);
#endif

IppStatus       InitPreferredCpu(DString sCpu);
ColorFormat     GetPreferredColorFormat(DString sColorFormat);
VideoRenderType GetPreferredVideoRender(DString sVideoRender);
AudioRenderType GetPreferredAudioRender(DString sAudioRender);
VideoStreamType GetPreferredVideoEncoder(DString sCodecName);
AudioStreamType GetPreferredAudioEncoder(DString sCodecName);
AudioStreamType GetPreferredAudioDecoder(DString sCodecName);

Status readVideoFrame(DataReader *pReader, VideoData *pData);
Status writeVideoFrame(DataWriter *pWriter, VideoData *pData);

#endif
