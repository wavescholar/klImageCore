/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#include <math.h>

#include "umc_config.h"
#include "umc_app_utils.h"
#include "functions_common.h"

#include "ippcore.h"
#include "ippdc.h"

using namespace UMC;


IppStatus InitPreferredCpu(DString sCpu)
{
    if(sCpu.Size())
    {
        if(!sCpu.Compare(VM_STRING("SSE"), false))
            return ippInitCpu(ippCpuSSE);
        else if(!sCpu.Compare(VM_STRING("SSE2"), false))
            return ippInitCpu(ippCpuSSE2);
        else if(!sCpu.Compare(VM_STRING("SSE3"), false))
            return ippInitCpu(ippCpuSSE3);
        else if(!sCpu.Compare(VM_STRING("SSSE3"), false))
            return ippInitCpu(ippCpuSSSE3);
        else if(!sCpu.Compare(VM_STRING("SSE41"), false))
            return ippInitCpu(ippCpuSSE41);
        else if(!sCpu.Compare(VM_STRING("SSE42"), false))
            return ippInitCpu(ippCpuSSE42);
        else if(!sCpu.Compare(VM_STRING("AVX"), false))
            return ippInitCpu(ippCpuAVX);
        else if(!sCpu.Compare(VM_STRING("AES"), false))
            return ippInitCpu(ippCpuAES);
        else if(!sCpu.Compare(VM_STRING("AVX2"), false))
            return ippInitCpu(ippCpuAVX2);
    }
    return ippInit();
}

ColorFormat GetPreferredColorFormat(DString sColorFormat)
{
    if(sColorFormat.Size())
    {
        if(!sColorFormat.Compare(VM_STRING("gray"), false))
            return GRAY;
        else if(!sColorFormat.Compare(VM_STRING("yv12"), false))
            return YV12;
        else if(!sColorFormat.Compare(VM_STRING("nv12"), false))
            return NV12;
        else if(!sColorFormat.Compare(VM_STRING("yuy2"), false))
            return YUY2;
        else if(!sColorFormat.Compare(VM_STRING("uyvy"), false))
            return UYVY;
        else if(!sColorFormat.Compare(VM_STRING("yuv420"), false))
            return YUV420;
        else if(!sColorFormat.Compare(VM_STRING("yuv422"), false))
            return YUV422;
        else if(!sColorFormat.Compare(VM_STRING("yuv444"), false))
            return YUV444;
        else if(!sColorFormat.Compare(VM_STRING("rgb"), false))
            return RGB;
        else if(!sColorFormat.Compare(VM_STRING("rgba"), false))
            return RGBA;
        else if(!sColorFormat.Compare(VM_STRING("bgr"), false))
            return BGR;
        else if(!sColorFormat.Compare(VM_STRING("bgra"), false))
            return BGRA;
        else if(!sColorFormat.Compare(VM_STRING("bgr565"), false))
            return BGR565;
        else if(!sColorFormat.Compare(VM_STRING("bgr555"), false))
            return BGR555;
        else if(!sColorFormat.Compare(VM_STRING("bgr444"), false))
            return BGR444;
    }
    return NONE;
}

VideoRenderType GetPreferredVideoRender(DString sVideoRender)
{
    if(sVideoRender.Size()) // render was specified
    {
        if(sVideoRender == VM_STRING("null"))
            return NULL_VIDEO_RENDER;
#ifdef UMC_ENABLE_OPENGL_VIDEO_RENDER
        else if(sVideoRender == VM_STRING("opengl"))
            return OPENGL_VIDEO_RENDER;
#endif
#ifdef UMC_ENABLE_GDI_VIDEO_RENDER
        else if(sVideoRender == VM_STRING("gdi"))
            return GDI_VIDEO_RENDER;
#endif
#ifdef UMC_ENABLE_FW_VIDEO_RENDER
        else if(sVideoRender == VM_STRING("fw"))
            return FW_VIDEO_RENDER;
#endif
    }
    else // render was not specified, select automatically
    {
#if defined UMC_ENABLE_OPENGL_VIDEO_RENDER
        return OPENGL_VIDEO_RENDER;
#elif defined UMC_ENABLE_GDI_VIDEO_RENDER
        return GDI_VIDEO_RENDER;
#else
        return NULL_VIDEO_RENDER;
#endif
    }
    return DEF_VIDEO_RENDER;
}

AudioRenderType GetPreferredAudioRender(DString sAudioRender)
{
    if(sAudioRender.Size())
    {
        if(sAudioRender == VM_STRING("null"))
            return NULL_AUDIO_RENDER;
#ifdef UMC_ENABLE_DSOUND_AUDIO_RENDER
        else if(sAudioRender == VM_STRING("dsound"))
            return DSOUND_AUDIO_RENDER;
#endif
#ifdef UMC_ENABLE_WINMM_AUDIO_RENDER
        else if(sAudioRender == VM_STRING("winmm"))
            return WINMM_AUDIO_RENDER;
#endif
#ifdef UMC_ENABLE_OSS_AUDIO_RENDER
        else if(sAudioRender == VM_STRING("oss"))
            return OSS_AUDIO_RENDER;
#endif
#ifdef UMC_ENABLE_FW_AUDIO_RENDER
        else if(sAudioRender == VM_STRING("fw"))
            return FW_AUDIO_RENDER;
#endif
    }
    else
    {
#if defined UMC_ENABLE_WINMM_AUDIO_RENDER
        return WINMM_AUDIO_RENDER;
#elif defined UMC_ENABLE_DSOUND_AUDIO_RENDER
        return DSOUND_AUDIO_RENDER;
#elif defined UMC_ENABLE_OSS_AUDIO_RENDER
        return OSS_AUDIO_RENDER;
#else
        return NULL_AUDIO_RENDER;
#endif
    }
    return DEF_AUDIO_RENDER;
}

VideoStreamType GetPreferredVideoEncoder(DString sCodecName)
{
    if(!vm_string_stricmp(sCodecName, VM_STRING("H264")) || !vm_string_stricmp(sCodecName, VM_STRING("H.264")))
        return H264_VIDEO;
    else if(!vm_string_stricmp(sCodecName, VM_STRING("MPEG2")))
        return MPEG2_VIDEO;
    else if(!vm_string_stricmp(sCodecName, VM_STRING("MPEG4")))
        return MPEG4_VIDEO;
    else if(!vm_string_stricmp(sCodecName, VM_STRING("VC1")))
        return VC1_VIDEO;
    else
        return UNDEF_VIDEO;
}

AudioStreamType GetPreferredAudioEncoder(DString sCodecName)
{
    if(!vm_string_stricmp(sCodecName, VM_STRING("AAC")))
        return AAC_AUDIO;
    else if(!vm_string_stricmp(sCodecName, VM_STRING("MP3")))
        return MPEG2_AUDIO;
    else
        return UNDEF_AUDIO;
}

AudioStreamType GetPreferredAudioDecoder(DString sCodecName)
{
    if(!vm_string_stricmp(sCodecName, VM_STRING("AAC")))
        return AAC_AUDIO;
    else if(!vm_string_stricmp(sCodecName, VM_STRING("AC3")))
        return AC3_AUDIO;
    else if(!vm_string_stricmp(sCodecName, VM_STRING("MP3")))
        return MPEG2_AUDIO;
    else
        return UNDEF_AUDIO;
}

Status readVideoFrame(DataReader *pReader, VideoData *pData)
{
    Status status;
    VideoData::PlaneData *pPlane;
    size_t iLenght;
    Ipp32u i;
    Ipp32s j;

    for(i = 0; i < pData->GetPlanesNumber(); i++)
    {
        pPlane = pData->GetPtrToPlane(i);
        iLenght = pPlane->m_size.width * pPlane->m_iSamples * pPlane->m_iSampleSize;

        for(j = 0; j < pPlane->m_size.height; j++)
        {
            status = pReader->GetData(pPlane->m_pPlane + pPlane->m_iPitch*j, iLenght);
            if(status != UMC_OK)
                return status;
        }
    }

    return UMC_OK;
}

Status writeVideoFrame(DataWriter *pWriter, VideoData *pData)
{
    Status status;
    VideoData::PlaneData *pPlane;
    size_t iLenght;
    Ipp32u i;
    Ipp32s j;

    for(i = 0; i < pData->GetPlanesNumber(); i++)
    {
        pPlane = pData->GetPtrToPlane(i);
        iLenght = pPlane->m_size.width * pPlane->m_iSamples * pPlane->m_iSampleSize;

        for(j = 0; j < pPlane->m_size.height; j++)
        {
            status = pWriter->PutData(pPlane->m_pPlane + pPlane->m_iPitch*j, iLenght);
            if(status != UMC_OK)
                return status;
        }
    }

    return UMC_OK;
}
