/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_video_data.h"
#include "umc_structures.h"

using namespace UMC;


VideoData::VideoData(void)
{
    m_colorFormat   = NONE;
    m_sampleFormat  = SF_UNSIGNED;
    m_picStructure  = PS_PROGRESSIVE;
    m_iWidth        = 0;
    m_iHeight       = 0;
    m_iPlanes       = 0;
    m_iSAWidth      = 1;
    m_iSAHeight     = 1;
    m_iAlignment    = 1;

    m_pPlaneData    = NULL;
}

VideoData::~VideoData(void)
{
    Close();
}

Status VideoData::Close(void)
{
    ReleaseImage();

    if(m_pPlaneData)
    {
        delete[] m_pPlaneData;
        m_pPlaneData = NULL;
    }

    m_colorFormat   = NONE;
    m_picStructure  = PS_PROGRESSIVE;
    m_iPlanes       = 0;
    m_iWidth        = 0;
    m_iHeight       = 0;
    m_iSAWidth      = 1;
    m_iSAHeight     = 1;
    m_iAlignment    = 1;

    return UMC_OK;
}

Status VideoData::ReleaseImage(void)
{
    for(Ipp32u i = 0; i < m_iPlanes; i++)
        m_pPlaneData[i].m_pPlane = NULL;

    return MediaData::Close();
}

Status VideoData::AllocPlanes(Ipp32u iPlanes)
{
    if(m_pPlaneData)
    {
        delete[] m_pPlaneData;
        m_pPlaneData = NULL;
        m_iPlanes    = 0;
    }

    if(iPlanes)
    {
        m_iPlanes    = iPlanes;
        m_pPlaneData = new PlaneData[m_iPlanes];
        if(NULL == m_pPlaneData)
            return UMC_ERR_ALLOC;
    }

    return UMC_OK;
}

// Initializes image with current values
Status VideoData::Init()
{
    return Init(m_iWidth, m_iHeight, m_colorFormat, 0);
}

// Completely sets image information, without allocation or linking to image memory.
Status VideoData::Init(Ipp32u iWidth, Ipp32u iHeight, ColorFormat format, Ipp32u iBitDepth)
{
    const  sColorFormatInfo* pFormat;
    Status status;

    if(0 == iWidth || 0 == iHeight || NONE == format)
        return UMC_ERR_INVALID_PARAMS;

    pFormat = GetColorFormatInfo(format);
    if(NULL == pFormat)
        return UMC_ERR_INVALID_PARAMS;

    if(iBitDepth == 0 || iBitDepth < pFormat->iMinBitDepth)
      iBitDepth = pFormat->iMinBitDepth;

    // allocate planes
    status = Close();
    if(UMC_OK != status)
        return status;

    status = AllocPlanes(pFormat->iPlanes);
    if(UMC_OK != status)
        return status;

    for(Ipp32u i = 0; i < m_iPlanes; i++)
    {
        m_pPlaneData[i].m_iSampleSize = (iBitDepth+7)>>3;
        m_pPlaneData[i].m_iBitDepth   = iBitDepth;
    }

    m_iWidth  = iWidth;
    m_iHeight = iHeight;

    // set plane info
    m_colorFormat = format;
    status = MapImage();
    if (UMC_OK != status)
        return status;

    return UMC_OK;
}

// Init video data with information from existing data.
Status VideoData::Init(VideoData *pData)
{
    if(this == pData)
        return UMC_ERR_FAILED;

    Close();

    // copy properties but clear external pointers
    *this = *pData;
    m_iBufferSize      = 0;
    m_iDataSize        = 0;
    m_pBufferPointer   = NULL;
    m_pDataPointer     = NULL;
    m_bMemoryAllocated = false;

    for(Ipp32u i = 0; i < m_iPlanes; i++)
        m_pPlaneData[i].m_pPlane = NULL;

    MapImage();

    return UMC_OK;
}

Status VideoData::MapImage()
{
    const  sColorFormatInfo *pFormat = NULL;
    Ipp32u i;

    if(m_colorFormat == NONE)
        return UMC_ERR_NOT_INITIALIZED;

    pFormat = GetColorFormatInfo(m_colorFormat);
    if(NULL == pFormat)
        return UMC_ERR_INVALID_PARAMS;

    if(m_iPlanes != pFormat->iPlanes)
    {
        Ipp32u iBitDepth = (m_iPlanes)?m_pPlaneData[0].m_iBitDepth:0;

        if(iBitDepth == 0 || iBitDepth < pFormat->iMinBitDepth)
            iBitDepth = pFormat->iMinBitDepth;

        ReleaseImage();
        AllocPlanes(pFormat->iPlanes);

        for(i = 0; i < m_iPlanes; i++)
        {
            m_pPlaneData[i].m_iSampleSize = (iBitDepth+7)>>3;
            m_pPlaneData[i].m_iBitDepth   = iBitDepth;
        }
    }

    // set correct width & height of planes
    for(i = 0; i < m_iPlanes; i ++)
        MapPlane(i);

    return UMC_OK;
}

Status VideoData::MapPlane(Ipp32u iPlane)
{
    const sColorFormatInfo *pFormat = NULL;
    PlaneData *pPlane;
    Ipp32u iBPP;
    Ipp32u iAlign;

    if(!m_pPlaneData || m_iPlanes <= iPlane)
        return UMC_ERR_FAILED;

    if(m_colorFormat == NONE)
        return UMC_ERR_NOT_INITIALIZED;

    pFormat = GetColorFormatInfo(m_colorFormat);
    if(!pFormat)
        return UMC_ERR_INVALID_PARAMS;

    pPlane  = &m_pPlaneData[iPlane];

    pPlane->m_iWidthDiv  = pFormat->planeFormatInfo[iPlane].iWidthDiv;
    pPlane->m_iHeightDiv = pFormat->planeFormatInfo[iPlane].iHeightDiv;
    pPlane->m_iSamples   = pFormat->planeFormatInfo[iPlane].iChannels;

    if(pPlane->m_iWidthDiv == 0)
    {
        pPlane->m_iWidthDiv  = 1;
        pPlane->m_size.width = m_iWidth;
    }
    else
        pPlane->m_size.width = (m_iWidth + pPlane->m_iWidthDiv - 1)/pPlane->m_iWidthDiv;

    if (pPlane->m_iHeightDiv == 0)
    {
        pPlane->m_iHeightDiv  = 1;
        pPlane->m_size.height = m_iHeight;
    }
    else
        pPlane->m_size.height = (m_iHeight + pPlane->m_iHeightDiv - 1)/pPlane->m_iHeightDiv;

    iBPP   = pPlane->m_iSampleSize * pPlane->m_iSamples;
    iAlign = IPP_MAX(m_iAlignment, iBPP);

    iAlign  = IPP_MAX(iAlign, pFormat->iMinAlign);
    iAlign *= pFormat->planeFormatInfo[iPlane].iAlignMult;

    pPlane->m_iPitch   = align_value<size_t>(pPlane->m_size.width * iBPP, iAlign);
    pPlane->m_iMemSize = pPlane->m_iPitch * pPlane->m_size.height;

    return UMC_OK;
}

// Set common bitdepth for all planes
Status VideoData::SetBitDepth(Ipp32u iBitDepth)
{
    Status status;

    if(!m_pPlaneData || !m_iPlanes)
        return UMC_ERR_NOT_INITIALIZED;

    for(Ipp32u i = 0; i < m_iPlanes; i++)
    {
        status = SetPlaneBitDepth(iBitDepth, i);
        if(status != UMC_OK)
            return status;
    }

    return UMC_OK;
}

// Set bitdepth for specified plane, usually additional or when bitdepth differs for main planes
Status VideoData::SetPlaneBitDepth(Ipp32u iBitDepth, Ipp32u iPlaneNumber)
{
    const sColorFormatInfo *pFormat = NULL;
    Status status;

    if(m_colorFormat == NONE)
        return UMC_ERR_NOT_INITIALIZED;

    pFormat = GetColorFormatInfo(m_colorFormat);
    if(!pFormat)
        return UMC_ERR_INVALID_PARAMS;

    // check if remap is required
    if(pFormat->iPlanes != m_iPlanes)
    {
        status = MapImage();
        if(status != UMC_OK)
            return status;
    }

    if(!m_pPlaneData || m_iPlanes <= iPlaneNumber)
        return UMC_ERR_FAILED;

    m_pPlaneData[iPlaneNumber].m_iBitDepth   = iBitDepth;
    m_pPlaneData[iPlaneNumber].m_iSampleSize = (iBitDepth+7)>>3;

    // remap plane
    return MapPlane(iPlaneNumber);
}

// Set common sample size for all planes
Status VideoData::SetSampleSize(Ipp32u iSampleSize)
{
    Status status;

    if(!m_pPlaneData || !m_iPlanes)
        return UMC_ERR_NOT_INITIALIZED;

    for(Ipp32u i = 0; i < m_iPlanes; i++)
    {
        status = SetPlaneSampleSize(iSampleSize, i);
        if(status != UMC_OK)
            return status;
    }

    return UMC_OK;
}

// Set sample size for specified plane, usually additional or when bitdepth differs for main planes
Status VideoData::SetPlaneSampleSize(Ipp32u iSampleSize, Ipp32u iPlaneNumber)
{
    const sColorFormatInfo *pFormat = NULL;
    Status status;

    if(m_colorFormat == NONE)
        return UMC_ERR_NOT_INITIALIZED;

    pFormat = GetColorFormatInfo(m_colorFormat);
    if(!pFormat)
        return UMC_ERR_INVALID_PARAMS;

    // check if remap is required
    if(pFormat->iPlanes != m_iPlanes)
    {
        status = MapImage();
        if(status != UMC_OK)
            return status;
    }

    if(!m_pPlaneData || m_iPlanes <= iPlaneNumber)
        return UMC_ERR_FAILED;

    m_pPlaneData[iPlaneNumber].m_iSampleSize = iSampleSize;
    if(iSampleSize*8 < m_pPlaneData[iPlaneNumber].m_iBitDepth)
        m_pPlaneData[iPlaneNumber].m_iBitDepth = iSampleSize*8;

    // remap plane
    return MapPlane(iPlaneNumber);
}

Ipp32u VideoData::GetChannelsNumber() const
{
    Ipp32u iChannels = 0;

    if(!m_pPlaneData || m_iPlanes == 0)
        return 0;

    for(Ipp32u i = 0; i < m_iPlanes; i++)
        iChannels += m_pPlaneData[i].m_iSamples;

    return iChannels;
}

Ipp32u VideoData::GetMaxSampleSize() const
{
    Ipp32u iMaxSize = 0;

    if(!m_pPlaneData || m_iPlanes == 0)
        return 0;

    for(Ipp32u i = 0; i < m_iPlanes; i++)
        iMaxSize = IPP_MAX(iMaxSize, m_pPlaneData[i].m_iSampleSize);

    return iMaxSize;
}

// Set common Alignment
Status VideoData::SetAlignment(Ipp32u iAlignment)
{
    if(iAlignment == 0)
        return UMC_ERR_INVALID_PARAMS;

    for(Ipp32u i = 1; i < (1 << 16); i <<= 1)
    {
        if (i & iAlignment)
        {
            m_iAlignment = i;
            break; // stop at last nonzero bit
        }
    }

    if(m_iAlignment != iAlignment)
        return UMC_ERR_INVALID_PARAMS;

    return UMC_OK;
}

// Allocates memory according to existing information in VideoData and given alignment
// If image memory was already allocated it is released.
// return UMC_OK on success, UMC_ERR_INVALID_STREAM if image is improperly described
Status VideoData::Alloc()
{
    Status status;
    Ipp8u *pPtr;
    size_t iSize;

    ReleaseImage();

    // remap image in case of parameters change
    status = MapImage();
    if (UMC_OK != status)
        return status;

    iSize = GetMappingSize();
    if(0 == iSize)
        return UMC_ERR_INVALID_STREAM;

    status = MediaData::Alloc(iSize + m_iAlignment - 1);
    if(status != UMC_OK)
        return UMC_ERR_ALLOC;

    pPtr = m_pBufferPointer;
    for(Ipp32u i = 0; i < m_iPlanes; i++)
    {
        m_pPlaneData[i].m_pPlane = pPtr;
        pPtr += m_pPlaneData[i].m_iMemSize;
    }

    return status;
}

// Links to provided image memory
// Image must be described before
Status VideoData::SetBufferPointer(Ipp8u *pBuffer, size_t iSize)
{
    Ipp8u     *pABuf  = align_pointer<Ipp8u *>(pBuffer, m_iAlignment);
    Ipp8u     *pPtr   = pABuf;
    size_t     iMemSize;

    if(!m_pPlaneData)
    {
        SetDataSize(0);
        return UMC_ERR_FAILED;
    }

    iMemSize = GetMappingSize();
    if(iSize < iMemSize)
    {
        SetDataSize(0);
        return UMC_ERR_NOT_ENOUGH_BUFFER;
    }

    for(Ipp32u i = 0; i < m_iPlanes; i++)
    {
        m_pPlaneData[i].m_pPlane = pPtr;
        pPtr += m_pPlaneData[i].m_iMemSize;
    }

    MediaData::SetBufferPointer(pBuffer, iSize);
    MediaData::SetDataSize(iMemSize + (pABuf - pBuffer));
    MediaData::MoveDataPointer((Ipp64s)(pABuf - pBuffer));

    return UMC_OK;
}

// Returns required image memory size according to alignment and current image description
size_t VideoData::GetMappingSize()
{
    size_t     iSize  = 0;

    if(!m_pPlaneData)
        return 0;

    for(Ipp32u i = 0; i < m_iPlanes; i++)
        iSize += m_pPlaneData[i].m_iMemSize;

    return iSize;
}

// Set pointer for specified plane. Should be used for additional planes,
// or when image layout is different.
Status VideoData::SetPlaneDataPtr(void *pBuffer, Ipp32u iPlaneNumber)
{
    if(!m_pPlaneData || m_iPlanes <= iPlaneNumber)
        return UMC_ERR_FAILED;

    m_pPlaneData[iPlaneNumber].m_pPlane = (Ipp8u*)pBuffer;

    return UMC_OK;
}

// Set pitch for specified plane. Should be used for additional planes,
// or when image layout is different.
Status VideoData::SetPlanePitch(size_t iPitch, Ipp32u iPlaneNumber)
{
    if(!m_pPlaneData || m_iPlanes <= iPlaneNumber)
        return UMC_ERR_FAILED;

    m_pPlaneData[iPlaneNumber].m_iPitch   = iPitch;
    m_pPlaneData[iPlaneNumber].m_iMemSize = iPitch * m_pPlaneData[iPlaneNumber].m_size.height;

    return UMC_OK;
}

Status VideoData::Crop(UMC::RECT CropArea)
{
    int left = CropArea.iLeft;
    int top = CropArea.iTop;
    int right = CropArea.iRight;
    int bottom = CropArea.iBottom;
    if (!right) right = m_iWidth;
    if (!bottom) bottom = m_iHeight;
    int w = right - left;
    int h = bottom - top;
    int k;

    UMC_CHECK(w > 0, UMC_ERR_INVALID_PARAMS);
    UMC_CHECK(h > 0, UMC_ERR_INVALID_PARAMS);
    UMC_CHECK(left >= 0, UMC_ERR_INVALID_PARAMS);
    UMC_CHECK(top >= 0, UMC_ERR_INVALID_PARAMS);
    UMC_CHECK(right <= (int)m_iWidth, UMC_ERR_INVALID_PARAMS);
    UMC_CHECK(bottom <= (int)m_iHeight, UMC_ERR_INVALID_PARAMS);

    for (k = 0; k < (int)m_iPlanes; k++)
    {
        int wDiv = (m_pPlaneData[k].m_size.width) ? m_iWidth/m_pPlaneData[k].m_size.width : 1;
        int hDiv = (m_pPlaneData[k].m_size.height) ? m_iHeight/m_pPlaneData[k].m_size.height : 1;
        m_pPlaneData[k].m_pPlane += (top / hDiv) * m_pPlaneData[k].m_iPitch +
            (left / wDiv) * m_pPlaneData[k].m_iSamples * m_pPlaneData[k].m_iSampleSize;
        m_pPlaneData[k].m_size.width  = w / wDiv;
        m_pPlaneData[k].m_size.height = h / hDiv;
    }
    m_iWidth = w;
    m_iHeight = h;

    return UMC_OK;
}

Status VideoData::Copy(VideoData *pDstData)
{
    if(!pDstData)
        return UMC_ERR_NULL_PTR;

    if(this == pDstData)
        return UMC_OK;

    // check data compatibility
    if(m_iWidth != pDstData->m_iWidth || m_iHeight != pDstData->m_iHeight)
        return UMC_ERR_INVALID_PARAMS;
    if(m_colorFormat != pDstData->m_colorFormat)
        return UMC_ERR_INVALID_PARAMS;
    if(m_iPlanes != pDstData->m_iPlanes)
        return UMC_ERR_INVALID_PARAMS;

    VideoData::PlaneData *pSrc;
    VideoData::PlaneData *pDst;
    IppiSize    size;
    Ipp32u      iPlanes = m_iPlanes;
    IppStatus   status  = ippStsNoErr;

    for(Ipp32u i = 0; i < iPlanes; i++)
    {
        pSrc = GetPtrToPlane(i);
        pDst = pDstData->GetPtrToPlane(i);

        if(!pSrc->m_pPlane || !pDst->m_pPlane)
            return UMC_ERR_INVALID_PARAMS;
        if(pSrc->m_iSampleSize != pDst->m_iSampleSize)
            return UMC_ERR_INVALID_PARAMS;
        if(pSrc->m_iSamples != pDst->m_iSamples)
            return UMC_ERR_INVALID_PARAMS;

        size.width  = pSrc->m_size.width * pSrc->m_iSamples * pSrc->m_iSampleSize;
        size.height = pSrc->m_size.height;

        status = ippiCopy_8u_C1R(pSrc->m_pPlane, (Ipp32s)pSrc->m_iPitch, pDst->m_pPlane, (Ipp32s)pDst->m_iPitch, size);
        if(status != ippStsNoErr)
            return UMC_ERR_FAILED;
    }

    return UMC_OK;
}

VideoData& VideoData::operator=(const VideoData &par)
{
    ReleaseImage();

    if(m_iPlanes != par.m_iPlanes)
        AllocPlanes(par.m_iPlanes);

    MediaData::operator=(par);

    if(par.m_iPlanes > 0)
        memcpy(m_pPlaneData, par.m_pPlaneData, par.m_iPlanes*sizeof(PlaneData));

    m_iWidth        = par.m_iWidth;
    m_iHeight       = par.m_iHeight;
    m_sampleFormat  = par.m_sampleFormat;
    m_colorFormat   = par.m_colorFormat;
    m_picStructure  = par.m_picStructure;
    m_iSAWidth      = par.m_iSAWidth;
    m_iSAHeight     = par.m_iSAHeight;
    m_iAlignment    = par.m_iAlignment;

    return *this;
}
