/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_mjpeg_pure_splitter.h"
#include "umc_bitstream_reader.h"

#include "ipps.h"

using namespace UMC;


typedef enum _JPEG_MARKER
{
  JM_NONE  = 0,
  JM_SOI   = 0xd8, /* start of image */
  JM_EOI   = 0xd9, /* end of image */

  /* start of frame */
  JM_SOF0  = 0xc0, /* Nondifferential Huffman-coding Baseline DCT */
  JM_SOF1  = 0xc1, /* Nondifferential Huffman-coding Extended sequental DCT */
  JM_SOF2  = 0xc2, /* Nondifferential Huffman-coding Progressive DCT */
  JM_SOF3  = 0xc3, /* Nondifferential Huffman-coding Lossless (sequental) */

  JM_SOF5  = 0xc5, /* Differential Huffman-coding Sequental DCT */
  JM_SOF6  = 0xc6, /* Differential Huffman-coding Progressive DCT */
  JM_SOF7  = 0xc7, /* Differential Lossless */

  JM_SOF9  = 0xc9, /* Nondifferential arithmetic-coding Extended sequental DCT */
  JM_SOFA  = 0xca, /* Nondifferential arithmetic-coding Progressive DCT */
  JM_SOFB  = 0xcb, /* Nondifferential arithmetic-coding Lossless (sequental) */

  JM_SOFD  = 0xcd, /* Differential arithmetic-coding Sequental DCT */
  JM_SOFE  = 0xce, /* Differential arithmetic-coding Progressive DCT */
  JM_SOFF  = 0xcf, /* Differential arithmetic-coding Lossless */

  JM_SOS   = 0xda, /* start of scan */
  JM_DQT   = 0xdb, /* define quantization table(s) */
  JM_DHT   = 0xc4, /* define Huffman table(s) */
  JM_APP0  = 0xe0, /* APP0 */
  JM_APP1  = 0xe1,
  JM_APP2  = 0xe2,
  JM_APP3  = 0xe3,
  JM_APP4  = 0xe4,
  JM_APP5  = 0xe5,
  JM_APP6  = 0xe6,
  JM_APP7  = 0xe7,
  JM_APP8  = 0xe8,
  JM_APP9  = 0xe9,
  JM_APP10 = 0xea,
  JM_APP11 = 0xeb,
  JM_APP12 = 0xec,
  JM_APP13 = 0xed,
  JM_APP14 = 0xee, /* APP14 */
  JM_APP15 = 0xef,
  JM_RST0  = 0xd0, /* restart with modulo 8 counter 0 */
  JM_RST1  = 0xd1, /* restart with modulo 8 counter 1 */
  JM_RST2  = 0xd2, /* restart with modulo 8 counter 2 */
  JM_RST3  = 0xd3, /* restart with modulo 8 counter 3 */
  JM_RST4  = 0xd4, /* restart with modulo 8 counter 4 */
  JM_RST5  = 0xd5, /* restart with modulo 8 counter 5 */
  JM_RST6  = 0xd6, /* restart with modulo 8 counter 6 */
  JM_RST7  = 0xd7, /* restart with modulo 8 counter 7 */
  JM_DRI   = 0xdd, /* define restart interval */
  JM_COM   = 0xfe  /* comment */

} JMARKER;

struct JPEGComponent
{
    Ipp32u iID;
    Ipp32u iHSampling;
    Ipp32u iVSampling;
    Ipp32u iQSelector;

};

struct JPEGSOFHeader
{
    Ipp32u iLength;
    Ipp32u iPrecision;
    Ipp32u iHeight;
    Ipp32u iWidth;
    Ipp32u iComonents;
    JPEGComponent comonent[4];

};

struct JPEGAPP0Header
{
    Ipp16u iLength;

    bool   bJfifDetected;
    Ipp32u iJfifVerMajor;
    Ipp32u iJfifVerMinor;
    Ipp32u iJfifUnits;
    Ipp32u iJfifXDensity;
    Ipp32u iJfifYDensity;
    Ipp32u iJfifThumbWidth;
    Ipp32u iJfifThumbHeight;

    bool   bJfxxDetected;
    Ipp32u iJfxxThumbnailsType;

    bool   bAvi1Detected;
    Ipp32u iAvi1Polarity;
    Ipp32u iAvi1Reserved;
    Ipp32u iAvi1FiledSize1;
    Ipp32u iAvi1FiledSize2;
};

struct JPEGAPP14Header
{
    Ipp16u iLength;

    bool   bAdobeDetected;
    Ipp32u iAdobeVersion;
    Ipp32u iAdobeFlags0;
    Ipp32u iAdobeFlags1;
    Ipp32u iAdobeTransform;
};


static Ipp32u getNextMarker(DataReader *pReader)
{
    Status status;
    Ipp8u  iByte;
    Ipp32u iCount = 0;

    while(!pReader->IsEOS())
    {
        UMC_CHECK_FUNC(status, pReader->Get8u(&iByte));

        if(iByte != 0xff)
        {
            do
            {
                iCount++;
                UMC_CHECK_FUNC(status, pReader->Get8u(&iByte));

            } while(iByte != 0xff);
        }

        do
        {
            UMC_CHECK_FUNC(status, pReader->Get8u(&iByte));

        } while(iByte == 0xff);

        if(iByte != 0)
            return iByte;
    }

    return 0;
}

static Status parseSOF(DataReader *pReader, JPEGSOFHeader *pHeader)
{
    BitstreamReader bs;
    bs.Init(pReader);

    pHeader->iLength    = bs.GetBits(16);
    pHeader->iPrecision = bs.GetBits(8);
    pHeader->iHeight    = bs.GetBits(16);
    pHeader->iWidth     = bs.GetBits(16);
    pHeader->iComonents = bs.GetBits(8);

    for(Ipp32u i = 0; i < pHeader->iComonents; i++)
    {
        pHeader->comonent[i].iID        = bs.GetBits(8);
        pHeader->comonent[i].iHSampling = bs.GetBits(4);
        pHeader->comonent[i].iVSampling = bs.GetBits(4);
        pHeader->comonent[i].iQSelector = bs.GetBits(8);
    }
    bs.AlignReader();

    return UMC_OK;
}

static Status parseAPP0(DataReader *pReader, JPEGAPP0Header *pHeader)
{
    BitstreamReader bs;

    Ipp8u  iMarker[5];
    size_t iSize = 5;

    pReader->Get16uSwap(&pHeader->iLength);

    pReader->CacheData(&iMarker[0], iSize);
    if(iMarker[0] == 'J' && iMarker[1] == 'F' && iMarker[2] == 'I' && iMarker[3] == 'F' && iMarker[4] == 0)
    {
        pReader->MovePosition(5);
        bs.Init(pReader);

        pHeader->bJfifDetected    = true;
        pHeader->iJfifVerMajor    = bs.GetBits(8);
        pHeader->iJfifVerMinor    = bs.GetBits(8);
        pHeader->iJfifUnits       = bs.GetBits(8);
        pHeader->iJfifXDensity    = bs.GetBits(16);
        pHeader->iJfifYDensity    = bs.GetBits(16);
        pHeader->iJfifThumbWidth  = bs.GetBits(8);
        pHeader->iJfifThumbHeight = bs.GetBits(8);

        bs.AlignReader();
    }

    pReader->CacheData(&iMarker[0], iSize);
    if(iMarker[0] == 'J' && iMarker[1] == 'F' && iMarker[2] == 'X' && iMarker[3] == 'X' && iMarker[4] == 0)
    {
        pReader->MovePosition(5);
        bs.Init(pReader);

        pHeader->bJfxxDetected       = true;
        pHeader->iJfxxThumbnailsType = bs.GetBits(8);

        bs.AlignReader();
    }

    pReader->CacheData(&iMarker[0], iSize);
    if(iMarker[0] == 'A' && iMarker[1] == 'V' && iMarker[2] == 'I' && iMarker[3] == '1')
    {
        pReader->MovePosition(4);
        bs.Init(pReader);

        pHeader->bAvi1Detected   = true;
        pHeader->iAvi1Polarity   = bs.GetBits(8);
        pHeader->iAvi1Reserved   = bs.GetBits(8);
        pHeader->iAvi1FiledSize1 = bs.GetBits(32);
        pHeader->iAvi1FiledSize2 = bs.GetBits(32);

        bs.AlignReader();
    }

    return UMC_OK;
}

static Status parseAPP14(DataReader *pReader, JPEGAPP14Header *pHeader)
{
    BitstreamReader bs;

    Ipp8u  iMarker[5];
    size_t iSize = 5;

    pReader->Get16uSwap(&pHeader->iLength);

    pReader->CacheData(&iMarker[0], iSize);
    if(iMarker[0] == 'a' && iMarker[1] == 'd' && iMarker[2] == 'o' && iMarker[3] == 'b' && iMarker[4] == 'e')
    {
        pReader->MovePosition(5);
        bs.Init(pReader);

        pHeader->bAdobeDetected  = true;
        pHeader->iAdobeVersion   = bs.GetBits(16);
        pHeader->iAdobeFlags0    = bs.GetBits(16);
        pHeader->iAdobeFlags1    = bs.GetBits(16);
        pHeader->iAdobeTransform = bs.GetBits(8);

        bs.AlignReader();
    }

    return UMC_OK;
}


MJPEGSplitter::MJPEGSplitter()
{
    m_pDataReader = NULL;
    m_iResilience = 0; // 0 - stop on error, 1 - skip unit on error, 2 - ignore error
    m_iFrames     = 0;
}

MJPEGSplitter::~MJPEGSplitter()
{
    Close();
}

Status MJPEGSplitter::Init(SplitterParams *pParams)
{
    if(!pParams)
        return UMC_OK;

    m_pDataReader = pParams->m_pDataReader;

    m_info.m_iFlags     = pParams->m_iFlags;
    m_info.m_systemType = MJPEG_STREAM;
    m_info.m_iTracks    = 1;
    m_info.m_fDuration  = 0;

    m_info.m_ppTrackInfo = new TrackInfo*[1];
    if(!m_info.m_ppTrackInfo)
        return UMC_ERR_ALLOC;

    m_info.m_ppTrackInfo[0] = new TrackInfo;
    if(m_info.m_ppTrackInfo[0] == NULL)
        return UMC_ERR_ALLOC;

    m_info.m_ppTrackInfo[0]->m_pStreamInfo = new VideoStreamInfo;
    if(m_info.m_ppTrackInfo[0]->m_pStreamInfo == NULL)
        return UMC_ERR_ALLOC;

    m_info.m_ppTrackInfo[0]->m_bEnabled = true;
    m_info.m_ppTrackInfo[0]->m_type     = TRACK_MJPEG;

    VideoStreamInfo *pInfo = (VideoStreamInfo*)m_info.m_ppTrackInfo[0]->m_pStreamInfo;
    bool             bStop = false;
    JPEGSOFHeader    sofHeader;
    JPEGAPP0Header   app0Header;
    JPEGAPP14Header  app14Header;

    UMC_SET_ZERO(sofHeader);
    UMC_SET_ZERO(app0Header);
    UMC_SET_ZERO(app14Header);

    // scan for data
    while(!m_pDataReader->IsEOS() && !bStop)
    {
        Ipp32u iMarker = getNextMarker(m_pDataReader);

        switch(iMarker)
        {
        case JM_SOF0:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_NDIFF_HUFF_BASELINE;
            break;
        case JM_SOF1:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_NDIFF_HUFF_EXTENDED;
            break;
        case JM_SOF2:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_NDIFF_HUFF_PROGRESSIVE;
            break;
        case JM_SOF3:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_NDIFF_HUFF_LOSSLESS;
            break;

        case JM_SOF5:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_DIFF_HUFF_EXTENDED;
            break;
        case JM_SOF6:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_DIFF_HUFF_PROGRESSIVE;
            break;
        case JM_SOF7:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_DIFF_HUFF_LOSSLESS;
            break;

        case JM_SOF9:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_NDIFF_ARITH_EXTENDED;
            break;
        case JM_SOFA:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_NDIFF_ARITH_PROGRESSIVE;
            break;
        case JM_SOFB:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_NDIFF_ARITH_LOSSLESS;
            break;

        case JM_SOFD:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_DIFF_ARITH_EXTENDED;
            break;
        case JM_SOFE:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_DIFF_ARITH_PROGRESSIVE;
            break;
        case JM_SOFF:
            parseSOF(m_pDataReader, &sofHeader);
            pInfo->iProfile = JPEG_PROFILE_DIFF_ARITH_LOSSLESS;
            break;

        case JM_APP0:
            parseAPP0(m_pDataReader, &app0Header);
            break;

        case JM_APP14:
            parseAPP14(m_pDataReader, &app14Header);
            break;

        case JM_EOI:
        case JM_SOS:
            bStop = true;
            break;
        default:
            continue;
        }
    }

    // copy header into init buffer
    size_t iHeaderSize = (size_t)m_pDataReader->GetPosition();
    m_info.m_ppTrackInfo[0]->m_pHeader = new MediaData;
    m_info.m_ppTrackInfo[0]->m_pHeader->Alloc(iHeaderSize);
    m_pDataReader->SetPosition(0);
    m_pDataReader->CacheData(m_info.m_ppTrackInfo[0]->m_pHeader->GetBufferPointer(), iHeaderSize, 0);
    m_info.m_ppTrackInfo[0]->m_pHeader->SetDataSize(iHeaderSize);

    // detect JPEG color space
    pInfo->videoInfo.m_colorFormat = NONE;

    if(app14Header.bAdobeDetected)
    {
        switch(app14Header.iAdobeTransform)
        {
        case 0:
            switch(sofHeader.iComonents)
            {
            case 3:
                pInfo->videoInfo.m_colorFormat = RGB;
                break;
            case 4:
                pInfo->videoInfo.m_colorFormat = CMYK;
                break;
            default:
                break;
            }
        break;

        default:
            break;
        }
    }

    if(sofHeader.iComonents == 1)
        pInfo->videoInfo.m_colorFormat = GRAY;
    else if(sofHeader.iComonents == 3)
    {
        if(sofHeader.comonent[0].iHSampling == 1 && sofHeader.comonent[0].iVSampling == 1 &&
           sofHeader.comonent[1].iHSampling == 1 && sofHeader.comonent[1].iVSampling == 1 &&
           sofHeader.comonent[2].iHSampling == 1 && sofHeader.comonent[2].iVSampling == 1)
           pInfo->videoInfo.m_colorFormat = YUV444;
        else if(sofHeader.comonent[0].iHSampling == 2 && sofHeader.comonent[0].iVSampling == 1 &&
                sofHeader.comonent[1].iHSampling == 1 && sofHeader.comonent[1].iVSampling == 1 &&
                sofHeader.comonent[2].iHSampling == 1 && sofHeader.comonent[2].iVSampling == 1)
           pInfo->videoInfo.m_colorFormat = YUV422;
        else if(sofHeader.comonent[0].iHSampling == 2 && sofHeader.comonent[0].iVSampling == 2 &&
                sofHeader.comonent[1].iHSampling == 1 && sofHeader.comonent[1].iVSampling == 1 &&
                sofHeader.comonent[2].iHSampling == 1 && sofHeader.comonent[2].iVSampling == 1)
           pInfo->videoInfo.m_colorFormat = YUV420;
        else
            return UMC_ERR_UNSUPPORTED;
    }
    else if(sofHeader.iComonents == 4)
    {
        if(sofHeader.comonent[0].iHSampling == 1 && sofHeader.comonent[0].iVSampling == 1 &&
           sofHeader.comonent[1].iHSampling == 1 && sofHeader.comonent[1].iVSampling == 1 &&
           sofHeader.comonent[2].iHSampling == 1 && sofHeader.comonent[2].iVSampling == 1 &&
           sofHeader.comonent[3].iHSampling == 1 && sofHeader.comonent[3].iVSampling == 1)
           pInfo->videoInfo.m_colorFormat = YUV444A;
        else if(sofHeader.comonent[0].iHSampling == 2 && sofHeader.comonent[0].iVSampling == 1 &&
                sofHeader.comonent[1].iHSampling == 1 && sofHeader.comonent[1].iVSampling == 1 &&
                sofHeader.comonent[2].iHSampling == 1 && sofHeader.comonent[2].iVSampling == 1 &&
                sofHeader.comonent[3].iHSampling == 2 && sofHeader.comonent[3].iVSampling == 1)
           pInfo->videoInfo.m_colorFormat = YUV422A;
        else if(sofHeader.comonent[0].iHSampling == 2 && sofHeader.comonent[0].iVSampling == 2 &&
                sofHeader.comonent[1].iHSampling == 1 && sofHeader.comonent[1].iVSampling == 1 &&
                sofHeader.comonent[2].iHSampling == 1 && sofHeader.comonent[2].iVSampling == 1 &&
                sofHeader.comonent[3].iHSampling == 2 && sofHeader.comonent[3].iVSampling == 2)
           pInfo->videoInfo.m_colorFormat = YUV420A;
        else
            return UMC_ERR_UNSUPPORTED;
    }
    else
        return UMC_ERR_UNSUPPORTED;

    pInfo->streamType = MJPEG_VIDEO;
    pInfo->videoInfo.m_iWidth  = sofHeader.iWidth;
    pInfo->videoInfo.m_iHeight = sofHeader.iHeight;

    if(app0Header.bAvi1Detected)
    {
        if(app0Header.iAvi1Polarity == 1)
        {
            pInfo->videoInfo.m_iHeight *= 2;
            pInfo->videoInfo.m_picStructure = PS_TOP_FIELD;
        }
        else if(app0Header.iAvi1Polarity == 2)
        {
            pInfo->videoInfo.m_iHeight *= 2;
            pInfo->videoInfo.m_picStructure = PS_BOTTOM_FIELD;
        }
    }

    return UMC_OK;
}

Status MJPEGSplitter::Close()
{
    UMC_DELETE(m_info.m_ppTrackInfo[0]->m_pHeader);
    UMC_DELETE(m_info.m_ppTrackInfo[0]->m_pStreamInfo);
    UMC_DELETE(m_info.m_ppTrackInfo[0]);
    UMC_DELETE_ARR(m_info.m_ppTrackInfo);

    return UMC_OK;
}

Status MJPEGSplitter::GetNextData(MediaData *pData, Ipp32u )
{
    size_t iFragmentSize = 0;
    Ipp64u iFragmentPos = 0;
    bool bStarted = false;
    bool bFinished = false;

    if(!pData)
        return UMC_ERR_NULL_PTR;

    // scan for frame
    while(!m_pDataReader->IsEOS())
    {
        Ipp32u iMarker = getNextMarker(m_pDataReader);

        if(iMarker == JM_SOI)
        {
            iFragmentPos  = m_pDataReader->GetPosition() - 2;
            bStarted = true;
        }
        else if(iMarker == JM_EOI)
        {
            if(!bStarted) // start code have not found yet
                continue;
            iFragmentSize = (size_t)(m_pDataReader->GetPosition() - iFragmentPos);
            bFinished = true;
            break;
        }
    }

    if(!bStarted || !bFinished)
        return UMC_ERR_END_OF_STREAM;

    size_t iReadSize = iFragmentSize;

    if(m_data.GetBufferSize() < iReadSize)
        m_data.Alloc(iReadSize);

    m_pDataReader->SetPosition(iFragmentPos);
    m_pDataReader->GetData(m_data.GetBufferPointer(), iReadSize);
    m_data.SetDataSize(iReadSize);

    VideoStreamInfo *pInfo = (VideoStreamInfo*)m_info.m_ppTrackInfo[0]->m_pStreamInfo;
    m_data.m_fPTSStart = m_iFrames/pInfo->fFramerate; // set frame PST from default framerate value
    if(pInfo->videoInfo.m_picStructure == PS_TOP_FIELD || pInfo->videoInfo.m_picStructure == PS_BOTTOM_FIELD)
        m_data.m_fPTSStart /= 2;
    m_iFrames++;

    *pData = m_data;

    return UMC_OK;
}

Status MJPEGSplitter::CheckNextData(MediaData *pData, Ipp32u )
{
    size_t iFragmentSize = 0;
    Ipp64u iStartPos = 0;
    Ipp64u iFragmentPos = 0;
    bool bStarted = false;
    bool bFinished = false;

    if(!pData)
        return UMC_ERR_NULL_PTR;

    iStartPos = m_pDataReader->GetPosition();

    // scan for frame
    while(!m_pDataReader->IsEOS())
    {
        Ipp32u iMarker = getNextMarker(m_pDataReader);

        if(iMarker == JM_SOI)
        {
            iFragmentPos  = m_pDataReader->GetPosition() - 2;
            bStarted = true;
        }
        else if(iMarker == JM_EOI)
        {
            if(!bStarted) // start code have not found yet
                continue;
            iFragmentSize = (size_t)(m_pDataReader->GetPosition() - iFragmentPos);
            bFinished = true;
            break;
        }
    }

    if(!bStarted || !bFinished)
        return UMC_ERR_END_OF_STREAM;

    size_t iReadSize = iFragmentSize;

    if(m_data.GetBufferSize() < iReadSize)
        m_data.Alloc(iReadSize);

    m_pDataReader->SetPosition(iFragmentPos);
    m_pDataReader->GetData(m_data.GetBufferPointer(), iReadSize);
    m_data.SetDataSize(iReadSize);

    m_pDataReader->SetPosition(iStartPos);

    VideoStreamInfo *pInfo = (VideoStreamInfo*)m_info.m_ppTrackInfo[0]->m_pStreamInfo;
    m_data.m_fPTSStart = m_iFrames/pInfo->fFramerate; // set frame PST from default framerate value
    if(pInfo->videoInfo.m_picStructure == PS_TOP_FIELD || pInfo->videoInfo.m_picStructure == PS_BOTTOM_FIELD)
        m_data.m_fPTSStart /= 2;
    *pData = m_data;

    return UMC_OK;
}
