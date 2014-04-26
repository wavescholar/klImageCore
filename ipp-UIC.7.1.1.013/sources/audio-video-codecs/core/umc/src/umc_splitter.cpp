/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_splitter.h"

using namespace UMC;


Splitter::Splitter()
{
    m_pDataReader = NULL;
}

SystemStreamType Splitter::GetStreamType(DataReader *pDR)
{
    Status status = UMC_OK;
    Ipp32u iLongCode;

    if(!pDR)
        return UNDEF_STREAM;
    else
        pDR->Reset();

    status = pDR->Check32u(&iLongCode, 0);
    if (UMC_OK != status)
        return UNDEF_STREAM;

    // possibly jpeg
    if((iLongCode&0xFF000000) == 0xFF000000)
    {
        Ipp8u  iByte;
        Ipp32u iOffset = 1;
        do
        {
            status = pDR->Check8u(&iByte, iOffset);
            if (UMC_OK != status)
                return UNDEF_STREAM;
            iOffset++;
        } while(iByte == 0xff);

        // check for StartOfImage marker
        if(iByte == 0xd8)
            return MJPEG_STREAM;
    }

    // it can be either avs or mpeg4 format
    if(iLongCode == 0x000001B0)
    {
        Ipp8u iByte;

        // the header of avs standard is 18 bytes long.
        // the one of mpeg4 standard is only one byte long.
        status = pDR->Check8u(&iByte, 5);
        if(UMC_OK != status)
            return UNDEF_STREAM;

        if(iByte)
            return AVS_PURE_VIDEO_STREAM;
    }

    if(iLongCode == 0x80000001)
    {
        Ipp8u iByte;

        // it is known bug of avs reference encoder - it adds extra 0x080 byte at the beginning.
        status = pDR->Check8u(&iByte, 4);
        if(UMC_OK != status)
            return UNDEF_STREAM;

        if(iByte == 0x0B0)
            return AVS_PURE_VIDEO_STREAM;
    }

    if((iLongCode & 0x1f) == 7 || (iLongCode == 0x00000001))
    {
        Ipp32u iDWord;

        status = pDR->Check32u(&iDWord, 4);

        if(iDWord == 'moov')
            return MP4_ATOM_STREAM;

        return H264_PURE_VIDEO_STREAM;
    }

    if(iLongCode == 0x0000010F || (iLongCode&0xFF) == 0xC5)
        return VC1_PURE_VIDEO_STREAM;

    if(iLongCode == 0x3026b275)
        return ASF_STREAM;

    if(iLongCode == 'RIFF')
    {
        Ipp32u iDWord;

        status = pDR->Check32u(&iDWord, 8);
        if (iDWord == 'AVI ')
            return AVI_STREAM;
        else if(iDWord == 'WAVE')
            return WAVE_STREAM;
    }

    if(iLongCode == 0x464c5601) // "FLV 0x01"  FLV version1
        return FLV_STREAM;

    if(iLongCode == 'DKIF')
    {
        Ipp32u iDWord;

        status = pDR->Check32u(&iDWord, 8);
        if(iDWord == 'VP80' || iDWord == 'I420')
            return IVF_STREAM;
    }

    if(iLongCode == 0x4f676753)
        return OGG_STREAM;

    // get next 4 bytes
    status = pDR->Check32u(&iLongCode, 4);
    if (UMC_OK != status)
        return UNDEF_STREAM;

    if(iLongCode == 'moov')
        return MP4_ATOM_STREAM;

    if(iLongCode == 'ftyp')
    {
        Ipp32u iDWord;

        status = pDR->Check32u(&iDWord, 8);
        if(UMC_OK != status)
            return UNDEF_STREAM;

        // mp42
        if(iDWord == 'mp42' ||
            iDWord == 'mp41' ||
            iDWord == 'isom' ||
            iDWord == 'MSNV' ||
            iDWord == 'M4V ' ||
            iDWord == 'M4A ' ||
            iDWord == '3gp6' ||
            iDWord == '3gp4' ||
            iDWord == '3gp5' ||
            iDWord == 'avc1' ||
            iDWord == 'avs2' ||
            iDWord == 'qt  ')
        {
            return MP4_ATOM_STREAM;
        }
    }

    return MPEGx_SYSTEM_STREAM;
}
