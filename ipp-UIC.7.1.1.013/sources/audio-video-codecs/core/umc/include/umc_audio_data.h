/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_AUDIO_DATA_H__
#define __UMC_AUDIO_DATA_H__

#include "umc_defs.h"
#include "umc_media_data.h"

#include "ippdefs.h"

namespace UMC
{

class AudioData : public MediaData
{
public:
    DYNAMIC_CAST_DECL(AudioData, MediaData)

    AudioData(void);
    virtual ~AudioData(void);

    // Initialize. Only remembers audio characteristics for future.
    virtual Status Init(Ipp32u iChannels, Ipp32u iSampleFrequency, Ipp32u iBitPerSample);

    // Initialize. Only copy audio characteristics from existing data for future.
    virtual Status Init(AudioData *pData);

    // Allocate buffer for audio data.
    virtual Status Alloc(size_t iLenght);

    // Release audio data and all internal memory. Inherited.
    virtual Status Close(void);

    // Set buffer pointer, assign all pointers. Inherited.
    virtual Status SetBufferPointer(Ipp8u *pBuffer, size_t iSize);

    // Set common Alignment
    Status SetAlignment(Ipp32u iAlignment);

     // Copy actual audio data
    Status Copy(AudioData *pDstData);

     // Copy structures data (just pointers)
    void operator=(const AudioData& par);

public:
    Ipp32u m_iChannels;         // number of audio channels
    Ipp32u m_iSampleFrequency;  // sample rate in Hz
    Ipp32u m_iBitPerSample;     // 0 if compressed
    Ipp32u m_iChannelMask;      // channel mask

protected:
    Ipp32u m_iAlignment;        // default 1

};

}

#endif
