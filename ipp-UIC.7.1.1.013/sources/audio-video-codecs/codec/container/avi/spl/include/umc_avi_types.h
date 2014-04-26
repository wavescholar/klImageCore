/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_AVI_TYPES_H__
#define __UMC_AVI_TYPES_H__

#include "ippdefs.h"
#include "umc_structures.h"

typedef Ipp32u tFOURCC;
#define uimcFOURCC DO_FOURCC

// standard avi FOURCCs
#define AVI_FOURCC_WRONG        uimcFOURCC( 0,   0,   0,   0 )
#define AVI_FOURCC_ANY_         uimcFOURCC(0xFF,0xFF,0xFF,0xFF)
#define AVI_FOURCC_RIFF         uimcFOURCC('R', 'I', 'F', 'F')
#define AVI_FOURCC_AVI_         uimcFOURCC('A', 'V', 'I', ' ')
#define AVI_FOURCC_AVIX         uimcFOURCC('A', 'V', 'I', 'X')
#define AVI_FOURCC_LIST         uimcFOURCC('L', 'I', 'S', 'T')
#define AVI_FOURCC_JUNK         uimcFOURCC('J', 'U', 'N', 'K')
#define AVI_FOURCC_MOVI         uimcFOURCC('m', 'o', 'v', 'i')
#define AVI_FOURCC_STRMHDR      uimcFOURCC('s', 't', 'r', 'h')
#define AVI_FOURCC_STRMFMT      uimcFOURCC('s', 't', 'r', 'f')
#define AVI_FOURCC_IDX1         uimcFOURCC('i', 'd', 'x', '1')
#define AVI_FOURCC_INDX         uimcFOURCC('i', 'n', 'd', 'x')
#define AVI_FOURCC_HDRL         uimcFOURCC('h', 'd', 'r', 'l')
#define AVI_FOURCC_AVIH         uimcFOURCC('a', 'v', 'i', 'h')
#define AVI_FOURCC_STRL         uimcFOURCC('s', 't', 'r', 'l')
#define AVI_FOURCC_STRH         uimcFOURCC('s', 't', 'r', 'h')
#define AVI_FOURCC_STRF         uimcFOURCC('s', 't', 'r', 'f')
#define AVI_FOURCC_REC_         uimcFOURCC('r', 'e', 'c', ' ')
#define AVI_FOURCC_00DB         uimcFOURCC('0', '0', 'd', 'b')
#define AVI_FOURCC_00DC         uimcFOURCC('0', '0', 'd', 'c')
#define AVI_FOURCC_00WB         uimcFOURCC('0', '0', 'w', 'b')
#define AVI_FOURCC_DB           uimcFOURCC( 0,   0,  'd', 'b')
#define AVI_FOURCC_DC           uimcFOURCC( 0,   0,  'd', 'c')
#define AVI_FOURCC_WB           uimcFOURCC( 0,   0,  'w', 'b')
#define AVI_FOURCC_vids         uimcFOURCC('v', 'i', 'd', 's')
#define AVI_FOURCC_auds         uimcFOURCC('a', 'u', 'd', 's')
#define AVI_FOURCC_iavs         uimcFOURCC('i', 'a', 'v', 's')
#define AVI_FOURCC_ivas         uimcFOURCC('i', 'v', 'a', 's')

// dv50 FOURCCs
#define AVI_FOURCC_DV50         uimcFOURCC('D', 'V', '5', '0')
#define AVI_FOURCC_dv50         uimcFOURCC('d', 'v', '5', '0')
// dvsd(50) FOURCCs (525/60 or 625/50 systems)
#define AVI_FOURCC_dvsd         uimcFOURCC('d', 'v', 's', 'd')
#define AVI_FOURCC_DVSD         uimcFOURCC('D', 'V', 'S', 'D')
#define AVI_FOURCC_dsvd         uimcFOURCC('d', 's', 'v', 'd')
#define AVI_FOURCC_DSVD         uimcFOURCC('D', 'S', 'V', 'D')
#define AVI_FOURCC_dvsl         uimcFOURCC('d', 'v', 's', 'l')
#define AVI_FOURCC_DVSL         uimcFOURCC('D', 'V', 'S', 'L')
#define AVI_FOURCC_vidv         uimcFOURCC('v', 'i', 'd', 'v')
#define AVI_FOURCC_VIDV         uimcFOURCC('V', 'I', 'D', 'V')
#define AVI_FOURCC_sldv         uimcFOURCC('s', 'l', 'd', 'v')
#define AVI_FOURCC_SLDV         uimcFOURCC('S', 'L', 'D', 'V')
// dvhd(100) FOURCCs (1125/60 or 1250/50 systems)
#define AVI_FOURCC_dvhp         uimcFOURCC('d', 'v', 'h', 'p')
#define AVI_FOURCC_DVHP         uimcFOURCC('D', 'V', 'H', 'P')
#define AVI_FOURCC_dvhd         uimcFOURCC('d', 'v', 'h', 'd')
#define AVI_FOURCC_DVHD         uimcFOURCC('D', 'V', 'H', 'D')
#define AVI_FOURCC_dvh1         uimcFOURCC('d', 'v', 'h', '1')
#define AVI_FOURCC_DVH1         uimcFOURCC('D', 'V', 'H', '1')
// h264 FOURCCs
#define AVI_FOURCC_ilv4         uimcFOURCC('i', 'l', 'v', '4')
#define AVI_FOURCC_ILV4         uimcFOURCC('I', 'L', 'V', '4')
#define AVI_FOURCC_vssh         uimcFOURCC('v', 's', 's', 'h')
#define AVI_FOURCC_VSSH         uimcFOURCC('V', 'S', 'S', 'H')
#define AVI_FOURCC_H264         uimcFOURCC('H', '2', '6', '4')
// wmv FOURCCs
#define AVI_FOURCC_WMV3         uimcFOURCC('W', 'M', 'V', '3')
// mpeg4 video FOURCCs
#define AVI_FOURCC_divx         uimcFOURCC('d', 'i', 'v', 'x')
#define AVI_FOURCC_DIVX         uimcFOURCC('D', 'I', 'V', 'X')
#define AVI_FOURCC_dx50         uimcFOURCC('d', 'x', '5', '0')
#define AVI_FOURCC_DX50         uimcFOURCC('D', 'X', '5', '0')
#define AVI_FOURCC_div3         uimcFOURCC('d', 'i', 'v', '3')
#define AVI_FOURCC_DIV3         uimcFOURCC('D', 'I', 'V', '3')
#define AVI_FOURCC_div4         uimcFOURCC('d', 'i', 'v', '4')
#define AVI_FOURCC_DIV4         uimcFOURCC('D', 'I', 'V', '4')
#define AVI_FOURCC_div5         uimcFOURCC('d', 'i', 'v', '5')
#define AVI_FOURCC_DIV5         uimcFOURCC('D', 'I', 'V', '5')
#define AVI_FOURCC_xvid         uimcFOURCC('x', 'v', 'i', 'd')
#define AVI_FOURCC_XVID         uimcFOURCC('X', 'V', 'I', 'D')
#define AVI_FOURCC_mp4v         uimcFOURCC('m', 'p', '4', 'v')
#define AVI_FOURCC_MP4V         uimcFOURCC('M', 'P', '4', 'V')
// mjpeg video FOURCCs
#define AVI_FOURCC_mjpx         uimcFOURCC('m', 'j', 'p', 'x')
#define AVI_FOURCC_MJPX         uimcFOURCC('M', 'J', 'P', 'X')
#define AVI_FOURCC_mjpg         uimcFOURCC('m', 'j', 'p', 'g')
#define AVI_FOURCC_MJPG         uimcFOURCC('M', 'J', 'P', 'G')
#define AVI_FOURCC_dmb1         uimcFOURCC('d', 'm', 'b', '1')
// h261 video FOURCCs
#define AVI_FOURCC_h261         uimcFOURCC('h', '2', '6', '1')
#define AVI_FOURCC_H261         uimcFOURCC('H', '2', '6', '1')
// h263 video FOURCCs
#define AVI_FOURCC_h263         uimcFOURCC('h', '2', '6', '3')
#define AVI_FOURCC_H263         uimcFOURCC('H', '2', '6', '3')
#define AVI_FOURCC_i263         uimcFOURCC('i', '2', '6', '3')
#define AVI_FOURCC_I263         uimcFOURCC('I', '2', '6', '3')
#define AVI_FOURCC_m263         uimcFOURCC('m', '2', '6', '3')
#define AVI_FOURCC_M263         uimcFOURCC('M', '2', '6', '3')
#define AVI_FOURCC_vivo         uimcFOURCC('v', 'i', 'v', 'o')
#define AVI_FOURCC_VIVO         uimcFOURCC('V', 'I', 'V', 'O')
// uncompressed video FOURCCs
#define AVI_FOURCC_DIB          uimcFOURCC('D', 'I', 'B', ' ')
#define AVI_FOURCC_IYUV         uimcFOURCC('I', 'Y', 'U', 'V')

#ifndef WAVE_FORMAT_MPEGLAYER3
#define WAVE_FORMAT_MPEGLAYER3 0x0055  /*  ISO/MPEG Layer3 Format Tag */
#endif //WAVE_FORMAT_MPEGLAYER3

#ifndef WAVE_FORMAT_ALAW
#define WAVE_FORMAT_ALAW 0x0006
#endif //WAVE_FORMAT_ALAW

#ifndef WAVE_FORMAT_MULAW
#define WAVE_FORMAT_MULAW 0x0007
#endif //WAVE_FORMAT_MULAW

#ifndef WAVE_FORMAT_AAC
#define WAVE_FORMAT_AAC 0x5473
#endif //WAVE_FORMAT_AAC

#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM     1
#endif //WAVE_FORMAT_PCM

#ifndef WAVE_FORMAT_DVM
#define WAVE_FORMAT_DVM     0x2000
#endif //WAVE_FORMAT_DVM

#ifndef AVIIF_LIST
#define AVIIF_LIST       0x00000001
#define AVIIF_KEYFRAME   0x00000010
#define AVIIF_FIRSTPART  0x00000020
#define AVIIF_LASTPART   0x00000040
#define AVIIF_NO_TIME    0x00000100
#endif // AVIIF_LIST

#define AVI_INDEX_OF_INDEXES       0x00
#define AVI_INDEX_OF_CHUNKS        0x01
#define AVI_INDEX_OF_TIMED_CHUNKS  0x02
#define AVI_INDEX_OF_SUB_2FIELD    0x03

    struct MainAviHeader
    {
        Ipp32u uiMicroSecPerFrame;
        Ipp32u uiMaxBytesPerSec;
        Ipp32u uiReserved1;
        Ipp32u uiFlags;
        Ipp32u uiTotalFrames;
        Ipp32u uiInitialFrames;
        Ipp32u uiStreams;
        Ipp32u uiSuggestedBufferSize;
        Ipp32u uiWidth;
        Ipp32u uiHeight;
        Ipp32u uiReserved[4];
        inline void SwapBigEndian()
        {
            uiMicroSecPerFrame  = BIG_ENDIAN_SWAP32(uiMicroSecPerFrame);
            uiMaxBytesPerSec    = BIG_ENDIAN_SWAP32(uiMaxBytesPerSec);
            uiReserved1         = BIG_ENDIAN_SWAP32(uiReserved1);
            uiFlags             = BIG_ENDIAN_SWAP32(uiFlags);
            uiTotalFrames       = BIG_ENDIAN_SWAP32(uiTotalFrames);
            uiInitialFrames     = BIG_ENDIAN_SWAP32(uiInitialFrames);
            uiStreams           = BIG_ENDIAN_SWAP32(uiStreams);
            uiSuggestedBufferSize = BIG_ENDIAN_SWAP32(uiSuggestedBufferSize);
            uiWidth             = BIG_ENDIAN_SWAP32(uiWidth);
            uiHeight            = BIG_ENDIAN_SWAP32(uiHeight);
        }
    };

    struct AviSplStreamHeader
    {
        Ipp32u fccType;
        Ipp32u fccHandler;
        Ipp32u uiFlags;
        Ipp32u uiPriority;
        Ipp32u uiInitialFrames;
        Ipp32u uiScale;
        Ipp32u uiRate;
        Ipp32u uiStart;
        Ipp32u uiLength;
        Ipp32u uiSuggestedBufferSize;
        Ipp32u uiQuality;
        Ipp32u uiSampleSize;
        UMC::RECT rcFrame;
        inline void SwapBigEndian()
        {
            fccType             = BIG_ENDIAN_SWAP32(fccType);
            fccHandler          = BIG_ENDIAN_SWAP32(fccHandler);
            uiFlags             = BIG_ENDIAN_SWAP32(uiFlags);
            uiPriority          = BIG_ENDIAN_SWAP32(uiPriority);
            uiInitialFrames     = BIG_ENDIAN_SWAP32(uiInitialFrames);
            uiScale             = BIG_ENDIAN_SWAP32(uiScale);
            uiRate              = BIG_ENDIAN_SWAP32(uiRate);
            uiStart             = BIG_ENDIAN_SWAP32(uiStart);
            uiLength            = BIG_ENDIAN_SWAP32(uiLength);
            uiSuggestedBufferSize = BIG_ENDIAN_SWAP32(uiSuggestedBufferSize);
            uiQuality           = BIG_ENDIAN_SWAP32(uiQuality);
            uiSampleSize        = BIG_ENDIAN_SWAP32(uiSampleSize);
            rcFrame.SwapBigEndian();
        }
    };

    struct BitmapInfoHeader
    {
        Ipp32u biSize;
        Ipp32u biWidth;
        Ipp32u biHeight;
        Ipp16u biPlanes;
        Ipp16u biBitCount;
        Ipp32u biCompression;
        Ipp32u biSizeImage;
        Ipp32s biXPelsPerMeter;
        Ipp32s biYPelsPerMeter;
        Ipp32u biClrUsed;
        Ipp32u biClrImportant;
        inline void SwapBigEndian()
        {
            biSize          = BIG_ENDIAN_SWAP32(biSize);
            biWidth         = BIG_ENDIAN_SWAP32(biWidth);
            biHeight        = BIG_ENDIAN_SWAP32(biHeight);
            biPlanes        = BIG_ENDIAN_SWAP16(biPlanes);
            biBitCount      = BIG_ENDIAN_SWAP16(biBitCount);
            biCompression   = BIG_ENDIAN_SWAP32(biCompression);
            biSizeImage     = BIG_ENDIAN_SWAP32(biSizeImage);
            biXPelsPerMeter = BIG_ENDIAN_SWAP32(biXPelsPerMeter);
            biYPelsPerMeter = BIG_ENDIAN_SWAP32(biYPelsPerMeter);
            biClrUsed       = BIG_ENDIAN_SWAP32(biClrUsed);
            biClrImportant  = BIG_ENDIAN_SWAP32(biClrImportant);
        }
    };

    struct WaveFormatEx
    {
        Ipp16u wFormatTag;
        Ipp16u nChannels;
        Ipp32u nSamplesPerSec;
        Ipp32u nAvgBytesPerSec;
        Ipp16u nBlockAlign;
        Ipp16u wBitsPerSample;
        Ipp16u cbSize;
        inline void SwapBigEndian()
        {
            wFormatTag      = BIG_ENDIAN_SWAP16(wFormatTag);
            nChannels       = BIG_ENDIAN_SWAP16(nChannels);
            nSamplesPerSec  = BIG_ENDIAN_SWAP32(nSamplesPerSec);
            nAvgBytesPerSec = BIG_ENDIAN_SWAP32(nAvgBytesPerSec);
            nBlockAlign     = BIG_ENDIAN_SWAP16(nBlockAlign);
            wBitsPerSample  = BIG_ENDIAN_SWAP16(wBitsPerSample);
            cbSize          = BIG_ENDIAN_SWAP16(cbSize);
        }
    };

    struct DvInfo {
        Ipp32u uiDVAAuxSrc;
        Ipp32u uiDVAAuxCtl;
        Ipp32u uiDVAAuxSrc1;
        Ipp32u uiDVAAuxCtl1;
        Ipp32u uiDVVAuxSrc;
        Ipp32u uiDVVAuxCtl;
        Ipp32u uiDVReserved[2];
    };

    struct AviTrack
    {
        AviTrack(void)
        {
            m_uiMaxSampleSize = 0;
            m_pStreamFormat = NULL;
            m_uiStreamFormatSize = 0;
            m_pIndex = NULL;
            m_uiIndexSize = 0;
            memset(&m_StreamHeader, 0, sizeof(m_StreamHeader));
        }

        size_t m_uiMaxSampleSize;
        AviSplStreamHeader m_StreamHeader;
        Ipp8u *m_pStreamFormat;
        Ipp32u m_uiStreamFormatSize;
        Ipp8u *m_pIndex;
        Ipp32u m_uiIndexSize;
    };

#endif  //  __UMC_AVI_TYPES_H__
