/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

//
//    This file contains class covering OSS Linux devices interface
//

#ifndef __OSS_DEV_H__
#define __OSS_DEV_H__

#ifdef UMC_ENABLE_OSS_AUDIO_RENDER

#include "linux_dev.h"

namespace UMC
{
    class OSSDev
    {
    public:
        OSSDev():
          m_uiBytesPlayed(0),
          m_uiBitsPerSample(0),
          m_uiNumOfChannels(0),
          m_uiFreq(0)
        {}

        virtual ~OSSDev() {}

        Status Init(const Ipp32u uiBitsPerSample,
                    const Ipp32u uiNumOfChannels,
                    const Ipp32u uiFreq);

        Status RenderData(Ipp8u* pbData, Ipp32u uiDataSize);
        Status Post();
        Status Reset();
        Status GetBytesPlayed(Ipp32u& ruiBytesPlayed);

        Status SetBitsPerSample(const Ipp32u uiBitsPerSample);
        Status SetNumOfChannels(const Ipp32u uiNumOfChannels);
        Status SetFreq(const Ipp32u uiFreq);

    protected:
        LinuxDev m_Device;
        Ipp32u m_uiBytesPlayed;
        Ipp32u m_uiBitsPerSample;
        Ipp32u m_uiNumOfChannels;
        Ipp32u m_uiFreq;
    };
}; // namespace UMC

#endif // UMC_ENABLE_OSS_AUDIO_RENDER

#endif // __OSS_DEV_H__

