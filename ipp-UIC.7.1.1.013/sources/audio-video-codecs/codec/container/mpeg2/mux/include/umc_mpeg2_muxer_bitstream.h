/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG2_MUXER_BITSTREAM_H__
#define __UMC_MPEG2_MUXER_BITSTREAM_H__

#include "ippdefs.h"

#ifndef NULL
#define NULL 0
#endif //NULL

namespace UMC
{
    class AdvancedBitStream
    {
    public:
        // Default constructor
        AdvancedBitStream(void);
        // Reset bit stream
        void Reset(void *lpvBuffer, Ipp32s lSize, bool bReset = false);
        // Set bit pointer
        void SetPointer(Ipp32s lBitsFromBegining);
        // Skip bits in stream
        void SkipBits(Ipp32s lSkip);
        // Insert bits in stream
        void AddBits(Ipp32u nBits, Ipp32s lCount);

        Ipp8u *m_lpbBuffer; // pointer to begining of buffer
        Ipp8u *m_lpb; // current position in buffer
        Ipp32s m_lBufferLength; // buffer size
        Ipp8u m_bRegistr; // register
        Ipp32s m_lRemainBits; // available bits in register
    };

}; // end namespace UMC

#endif // __UMC_MPEG2_MUXER_BITSTREAM_H__
