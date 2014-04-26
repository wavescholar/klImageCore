/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "umc_scene_analyzer_frame.h"
#include "umc_structures.h"
#include "ippcore.h"

namespace UMC
{

enum
{
    SA_STEP_ALIGN               = 64
};

SceneAnalyzerPicture::SceneAnalyzerPicture(void)
{
    memset(&m_pPic, 0, sizeof(m_pPic));
    m_picSize = 0;
    m_picStep = 0;
    m_picDim.width = m_picDim.height = 0;
    m_mbDim.width = m_mbDim.height = 0;
    m_bChangeDetected = true;

    m_pSadBuffer = (Ipp16u *) 0;

    m_pSliceInfo = (UMC_SCENE_INFO *) 0;
    m_numSliceInfo = 0;
    m_colorFormat = YUV420;
} // SceneAnalyzerPicture::SceneAnalyzerPicture(void)

SceneAnalyzerPicture::~SceneAnalyzerPicture(void)
{
    // deinitialize the object
    Close();

    // release the buffer
    ReleaseBuffer();
    ReleaseSliceInfo();

} // SceneAnalyzerPicture::~SceneAnalyzerPicture(void)

void SceneAnalyzerPicture::Close(void)
{
    // deallocate the buffer
    if (m_pSadBuffer)
    {
        ippFree(m_pSadBuffer);
    }

    m_picStep = 0;
    m_picDim.width = m_picDim.height = 0;
    m_mbDim.width = m_mbDim.height = 0;
    m_bChangeDetected = true;

    m_pSadBuffer = (Ipp16u *) 0;

    m_numSliceInfo = 0;

} // void SceneAnalyzerPicture::Close(void)

Status SceneAnalyzerPicture::Init(Ipp32s srcWidth, Ipp32s srcHeight,
                                  ColorFormat colorFormat)
{
    size_t picStep, picSize;
    IppiSize picDim = {srcWidth, srcHeight};
    Status umcRes;

    // check error
    if ((0 == srcWidth) || (0 == srcHeight))
        return UMC_ERR_INVALID_PARAMS;

    // close the object before the initialization
    Close();

    // allocate temporal SAD buffer
    m_pSadBuffer = (Ipp16u*)ippMalloc(SA_ESTIMATION_WIDTH*sizeof(Ipp16u));
    if (NULL == m_pSadBuffer)
    {
        return UMC_ERR_ALLOC;
    }

    // calculate step
    picStep = align_value<size_t> (srcWidth, SA_STEP_ALIGN);

    // calculate picture size
    switch (colorFormat)
    {
    case YV12:
    case NV12:
    case YUV420:
        picSize = (picStep * srcHeight * 3) / 2;
        break;

    case YUV422:
        picSize = picStep * srcHeight * 2;
        break;

    case YUV444:
        picSize = picStep * srcHeight * 3;
        break;

    case GRAY:
        picSize = picStep * srcHeight;
        break;

    default:
        return UMC_ERR_INVALID_PARAMS;
    }

    // allocate the buffer
    umcRes = AllocateBuffer(picSize);
    if (UMC_OK != umcRes)
        return umcRes;

    // set the chroma pointers
    switch (colorFormat)
    {
    case YV12:
    case NV12:
    case YUV420:
    case YUV422:
        m_pPic[1] = m_pPic[0] + picStep * srcHeight;
        m_pPic[2] = m_pPic[1] + picStep / 2;
        break;

    case YUV444:
        m_pPic[1] = m_pPic[0] + picStep * srcHeight;
        m_pPic[2] = m_pPic[1] + picStep * srcHeight;
        break;

    case GRAY:
        break;
    }

    // save parameters
    m_picStep = picStep;
    m_picDim = picDim;
    m_mbDim.width = picDim.width / 4;
    m_mbDim.height = picDim.height / 4;
    m_colorFormat = colorFormat;

    // allocate slice info items
    umcRes = AllocateSliceInfo(m_mbDim.height);
    if (UMC_OK != umcRes)
        return umcRes;

    return UMC_OK;

} // Status SceneAnalyzerPicture::Init(Ipp32s srcWidth, Ipp32s srcHeight,

Status SceneAnalyzerPicture::SetPointer(const Ipp8u *pPic, size_t picStep,
                                        Ipp32s srcWidth, Ipp32s srcHeight)
{
    IppiSize picDim = {srcWidth, srcHeight};

    // close the object before the initialization
    Close();

    // release the allocated buffer
    ReleaseBuffer();

    // save parameters
    m_pPic[0] = pPic;
    m_picStep = picStep;
    m_picDim = picDim;
    m_mbDim.width = picDim.width / 4;
    m_mbDim.height = picDim.height / 4;
    m_colorFormat = GRAY;

    return UMC_OK;

} // Status SceneAnalyzerPicture::SetPointer(const Ipp8u *pPic, size_t picStep,

Status SceneAnalyzerPicture::AllocateBuffer(size_t bufSize)
{
    // check size of allocated buffer
    if (m_picSize >= bufSize)
        return UMC_OK;

    // release the buffer before allocation of new one
    ReleaseBuffer();

    // allocate new buffer
    m_pPic[0] = (Ipp8u*)ippMalloc((int) bufSize);
    if (NULL == m_pPic[0])
    {
        return UMC_ERR_ALLOC;
    }

    // save allocate size
    m_picSize = bufSize;

    return UMC_OK;

} // Status SceneAnalyzerPicture::AllocateBuffer(size_t bufSize)

void SceneAnalyzerPicture::ReleaseBuffer(void)
{
    // we identify the allocated buffer by its size.
    // Note, that the buffer can be set to the object externally.
    if ((m_picSize) && (m_pPic[0]))
    {
        ippFree((void *) m_pPic[0]);
    }

    memset(&m_pPic, 0, sizeof(m_pPic));
    m_picSize = 0;

} // void SceneAnalyzerPicture::ReleaseBuffer(void)

Status SceneAnalyzerPicture::AllocateSliceInfo(size_t numSliceInfo)
{
    // check number of already allocated buffers
    if (m_numSliceInfo >= numSliceInfo)
    {
        return UMC_OK;
    }

    // release the allocated slice info strucures before allocation
    ReleaseSliceInfo();

    // allocate new slice info structures
    m_pSliceInfo = (UMC_SCENE_INFO *) ippMalloc((int) (sizeof(UMC_SCENE_INFO) * numSliceInfo));
    if (NULL == m_pSliceInfo)
    {
        return UMC_ERR_ALLOC;
    }

    // save allocate size
    m_numSliceInfo = numSliceInfo;

    return UMC_OK;

} // Status SceneAnalyzerPicture::AllocateSliceInfo(size_t numSliceInfo)

void SceneAnalyzerPicture::ReleaseSliceInfo(void)
{
    if (m_pSliceInfo)
    {
        ippFree(m_pSliceInfo);
    }

    m_pSliceInfo = (UMC_SCENE_INFO *) 0;
    m_numSliceInfo = 0;

} // void SceneAnalyzerPicture::ReleaseSliceInfo(void)


SceneAnalyzerFrame::SceneAnalyzerFrame(void)
{
    m_frameStructure = PS_PROGRESSIVE;
    m_frameEstimation = PS_PROGRESSIVE;
    m_frameType = NONE_PICTURE;
} // SceneAnalyzerFrame::SceneAnalyzerFrame(void)

SceneAnalyzerFrame::~SceneAnalyzerFrame(void)
{

} // SceneAnalyzerFrame::~SceneAnalyzerFrame(void)

static
IppiSize GetScaledImageSize(IppiSize srcDim)
{
    IppiSize dstDim;

    // try to scale to something ~300 X ~200
    switch (srcDim.width)
    {
    case 640:
    case 704:
    case 720:
    case 768:
        dstDim.width = srcDim.width / 2;
        dstDim.height = srcDim.height / 2;
        break;

    case 960:
        dstDim.width = srcDim.width / 3;
        dstDim.height = srcDim.height / 3;
        break;

    case 1280:
        dstDim.width = srcDim.width / 4;
        dstDim.height = srcDim.height / 3;
        break;

    case 1440:
        dstDim.width = srcDim.width / 4;
        dstDim.height = srcDim.height / 4;
        break;

    case 1920:
        dstDim.width = srcDim.width / 6;
        dstDim.height = srcDim.height / 4;
        break;

    default:
        // too small image,
        // do not down scale
        if (352 >= srcDim.width)
        {
            dstDim = srcDim;
        }
        else
        {
            dstDim.width = 320;
            dstDim.height = 240;
        }
    }

    // align destination dimensions
    dstDim.width = dstDim.width & -4;
    dstDim.height = dstDim.height & -4;

    return dstDim;

} // IppiSize GetScaledImageSize(IppiSize srcDim)

Status SceneAnalyzerFrame::SetSource(VideoData *pSrc, PictureStructure interlaceType)
{
    VideoData::PlaneData *pPlaneInfo;
    Status umcRes;
    Ipp32u i;

    // check error(s)
    if (NULL == pSrc)
    {
        return UMC_ERR_NULL_PTR;
    }

    // check color format
    if (false == CheckSupportedColorFormat(pSrc))
    {
        return UMC_ERR_INVALID_PARAMS;
    }

    // get plane parameters
    pPlaneInfo = pSrc->GetPtrToPlane(0);
    if(!pPlaneInfo)
        return UMC_ERR_FAILED;
    if(8 != pPlaneInfo->m_iBitDepth)
        return UMC_ERR_INVALID_PARAMS;

    // set frame & fields pointers
    umcRes = SceneAnalyzerPicture::Init(pPlaneInfo->m_size.width,
                                        pPlaneInfo->m_size.height,
                                        pSrc->m_colorFormat);
    if (UMC_OK != umcRes)
        return umcRes;
    if (PS_PROGRESSIVE != interlaceType)
    {
        umcRes = m_fields[0].SetPointer(m_pPic[0],
                                        m_picStep * 2,
                                        m_picDim.width,
                                        m_picDim.height / 2);
        if (UMC_OK != umcRes)
            return umcRes;
        umcRes = m_fields[1].SetPointer(m_pPic[0] + m_picStep,
                                        m_picStep * 2,
                                        m_picDim.width,
                                        m_picDim.height / 2);
        if (UMC_OK != umcRes)
            return umcRes;
    }
    // copy picture
    for (i = 0; i < pSrc->GetPlanesNumber(); i += 1)
    {
        // get plane parameters
        pPlaneInfo = pSrc->GetPtrToPlane(i);
        if(!pPlaneInfo)
            return UMC_ERR_FAILED;

        // copy data
        ippiCopy_8u_C1R(pPlaneInfo->m_pPlane,
                        (int) pPlaneInfo->m_iPitch,
                        (Ipp8u *) m_pPic[i],
                        (int) m_picStep,
                        pPlaneInfo->m_size);
    }

    // initialize the scaled picture
    {
        IppiSize srcDim = {m_picDim.width, m_picDim.height / 2};
        IppiRect srcRoi = {0, 0, srcDim.width, srcDim.height};
        IppiSize dstDim = GetScaledImageSize(m_picDim);
        Ipp8u*   pBuffer;
        int      bufferSize;
        IppiRect RectSrc = srcRoi;
        IppiRect RectDst;

        // initialize the scaled image
        umcRes = m_scaledPic.Init(dstDim.width, dstDim.height, GRAY);
        if (UMC_OK != umcRes)
            return umcRes;

        ippiGetResizeRect(RectSrc, &RectDst,
                          ((double) dstDim.width) / ((double) srcDim.width),
                          ((double) dstDim.height) / ((double) srcDim.height),
                          0, 0, IPPI_INTER_CUBIC);
        if(ippStsNoErr != ippiResizeGetBufSize(RectSrc, RectDst, 1, IPPI_INTER_CUBIC, &bufferSize))
            return UMC_ERR_UNSUPPORTED;

        pBuffer = (Ipp8u*)ippMalloc(bufferSize);
        if(bufferSize <= 0)
            return UMC_ERR_NULL_PTR;

        // convert the picture
        ippiResizeSqrPixel_8u_C1R(m_pPic[0],
                          srcDim,
                          (int) m_picStep * 2,
                          RectSrc,
                          (Ipp8u *) m_scaledPic.m_pPic[0],
                          (int) m_scaledPic.m_picStep,
                          RectDst,
                          ((double) dstDim.width) / ((double) srcDim.width),
                          ((double) dstDim.height) / ((double) srcDim.height),
                          0,
                          0,
                          IPPI_INTER_CUBIC,
                          pBuffer);

        if(bufferSize > 0)
            ippFree(pBuffer);
    }

    // save frame structure
    m_frameStructure = interlaceType;
    m_frameEstimation = interlaceType;

    return UMC_OK;

} // Status SceneAnalyzerFrame::SetSource(VideoData *pSrc, InterlaceType interlaceType)

} // namespace UMC
