/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

//
//      This file contains class covering OSS Linux devices interface
//

#include "umc_config.h"

#ifdef UMC_ENABLE_OSS_AUDIO_RENDER

#include <sys/soundcard.h>
#include "fcntl.h"
#include "oss_dev.h"
#include "vm_debug.h"

using namespace UMC;


Status OSSDev::Init(const Ipp32u uiBitsPerSample,
                              const Ipp32u uiNumOfChannels,
                              const Ipp32u uiFreq)
{
    Status umcRes = UMC_OK;

    if (UMC_OK == umcRes)
    {   umcRes = m_Device.Init(VM_STRING("/dev/dsp"), O_WRONLY);    }

    if (UMC_OK == umcRes)
    {
        m_uiBitsPerSample = uiBitsPerSample;
        m_uiNumOfChannels = uiNumOfChannels;
        m_uiFreq = uiFreq;
        umcRes = Reset();
    }

    return umcRes;
}

Status OSSDev::RenderData(Ipp8u* pbData, Ipp32u uiDataSize)
{
    m_uiBytesPlayed += uiDataSize;
    return m_Device.Write(pbData, uiDataSize, NULL);
}

Status OSSDev::SetBitsPerSample(const Ipp32u uiBitsPerSample)
{
    Status umcRes = UMC_OK;
    Ipp32u uiFormat;

    switch (uiBitsPerSample)
    {
    case 16:
        uiFormat = AFMT_S16_LE;
        break;
    case 8:
        uiFormat = AFMT_S8;
        break;
    default:
        umcRes = UMC_ERR_UNSUPPORTED;
    }

    Ipp32u uiFormatSet = uiFormat;
    if (UMC_OK == umcRes)
    {   umcRes = m_Device.Ioctl(SNDCTL_DSP_SETFMT, (void *)&uiFormatSet); }

    if (UMC_OK == umcRes && uiFormat != uiFormatSet)
    {
        umcRes = UMC_ERR_UNSUPPORTED;
    }
    return umcRes;
}

Status OSSDev::SetNumOfChannels(const Ipp32u uiNumOfChannels)
{
    Status umcRes = UMC_OK;
    Ipp32u uiNumOfChannelsSet = uiNumOfChannels;

    if (UMC_OK == umcRes)
    {
        umcRes = m_Device.Ioctl(SNDCTL_DSP_CHANNELS,
                                (void *)&uiNumOfChannelsSet);
    }

    if (UMC_OK == umcRes && uiNumOfChannels != uiNumOfChannelsSet)
    {
        umcRes = UMC_ERR_UNSUPPORTED;
    }
    return umcRes;
}

Status
OSSDev::SetFreq(const Ipp32u uiFreq)
{
    Status umcRes = UMC_OK;
    Ipp32u uiFreqSet = uiFreq;

    if (UMC_OK == umcRes)
    {   umcRes = m_Device.Ioctl(SNDCTL_DSP_SPEED, (void *)&uiFreqSet);    }

    if (UMC_OK == umcRes && uiFreq != uiFreqSet)
    {
        umcRes = UMC_ERR_UNSUPPORTED;
    }
    return umcRes;
}

Status OSSDev::Post()
{
    Status umcRes = m_Device.Ioctl(SNDCTL_DSP_POST);

    return umcRes;
}

Status OSSDev::Reset()
{
    Status umcRes = m_Device.Ioctl(SNDCTL_DSP_RESET);

    if (UMC_OK == umcRes)
    {   umcRes = SetBitsPerSample(m_uiBitsPerSample);   }

    if (UMC_OK == umcRes)
    {   umcRes = SetNumOfChannels(m_uiNumOfChannels);   }

    if (UMC_OK == umcRes)
    {   umcRes = SetFreq(m_uiFreq);    }

    m_uiBytesPlayed = 0;
    return umcRes;
}

Status OSSDev::GetBytesPlayed(Ipp32u& ruiBytesPlayed)
{
    Status umcRes = UMC_OK;

    count_info Info;
    Info.bytes = 0;
    umcRes = m_Device.Ioctl(SNDCTL_DSP_GETODELAY, (void *)&Info);

    if (UMC_OK == umcRes)
    {   ruiBytesPlayed = m_uiBytesPlayed - Info.bytes;    }
    return umcRes;
}

#endif // UMC_ENABLE_OSS_AUDIO_RENDER
