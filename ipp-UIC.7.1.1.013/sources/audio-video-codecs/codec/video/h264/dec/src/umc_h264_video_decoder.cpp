/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/
#include "umc_config.h"
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include "umc_h264_video_decoder.h"
#include "umc_video_data.h"
#include "umc_h264_bitstream.h"

#include "umc_memory_allocator.h"
#include "umc_default_frame_allocator.h"
#include "umc_video_processing.h"

#include "umc_h264_task_supplier.h"
#include "umc_h264_timing.h"
#include "vm_sys_info.h"

namespace UMC
{

#ifdef USE_DETAILED_H264_TIMING
    TimingInfo* clsTimingInfo;
#endif

//////////////////////////////////////////////////////////////////////////////
// H264Decoder constructor
//////////////////////////////////////////////////////////////////////////////
H264VideoDecoder::H264VideoDecoder ()
{
    m_pTaskSupplier       = NULL;
    m_pFrameAllocator     = NULL;
    m_IsInitialized       = false;

    // allocator tools
    INIT_TIMING;
}

//////////////////////////////////////////////////////////////////////////////
// Start_Sequence: This method should be called if there are significant
//                 changes in the input format in the compressed bit stream.
//                 This method must be called between construction
//                 of the H264Decoder object and the first time a decompress
//                 is done.
//////////////////////////////////////////////////////////////////////////////
Status H264VideoDecoder::Init(BaseCodecParams *pInit)
{
    Status umcRes = UMC_OK;
    VideoDecoderParams *init = DynamicCast<VideoDecoderParams, BaseCodecParams> (pInit);

    // check error(s)
    if(NULL == init)
        return UMC_ERR_NULL_PTR;
    if(!init->m_pPostProcessor)
        return UMC_ERR_NULL_PTR;

    // release object before initialization
    Close();

    // initialize memory control tools
    umcRes = VideoDecoder::Init(pInit);
    if (UMC_OK != umcRes)
        return umcRes;

    m_pPostProcessor = init->m_pPostProcessor;

    m_pTaskSupplier = new TaskSupplier();
    m_pFrameAllocator = new DefaultFrameAllocator();

    m_pTaskSupplier->SetFrameAllocator(m_pFrameAllocator);
    m_pTaskSupplier->SetMemoryAllocator(m_pMemoryAllocator);

    try
    {
        m_pTaskSupplier->Init(init);

        if (init->m_pData && init->m_pData->GetDataSize())
            umcRes = m_pTaskSupplier->GetFrame((MediaData *)init->m_pData, 0);
    }
    catch(...)
    {
        return UMC_ERR_FAILED;
    }

    if (UMC_ERR_NOT_ENOUGH_DATA == umcRes || UMC_ERR_NULL_PTR == umcRes)
        umcRes = UMC_OK;

    m_IsInitialized = (umcRes == UMC_OK);

    return umcRes;
}

Status  H264VideoDecoder::SetParams(BaseCodecParams* params)
{
    if (!m_IsInitialized)
        return UMC_ERR_NOT_INITIALIZED;

    VideoDecoderParams *pParams = DynamicCast<VideoDecoderParams, BaseCodecParams>(params);

    if (NULL == pParams)
        return UMC_ERR_NULL_PTR;

    m_pTaskSupplier->SetParams(pParams);

    return VideoDecoder::SetParams(params);
}

Status H264VideoDecoder::Close()
{
    // release memory control tools
    try
    {
        if(m_pTaskSupplier != NULL)
        {
            delete m_pTaskSupplier;
            m_pTaskSupplier = NULL;
        }
        if(m_pFrameAllocator != NULL)
        {
            delete m_pFrameAllocator;
            m_pFrameAllocator = NULL;
        }

        VideoDecoder::Close();
    }
    catch(...) {}

    m_IsInitialized = false;

    return UMC_OK;
}

Status  H264VideoDecoder::Reset()
{
    Status umcRes = UMC_OK;

    if (!m_IsInitialized)
        return UMC_ERR_NOT_INITIALIZED;

#ifdef USE_SEI
    m_FrameProcessingStage = NORMAL_FRAME_PROCESSING;
#endif

    m_pTaskSupplier->Reset();

    return umcRes;
}

//////////////////////////////////////////////////////////////////////////////
// H264Decoder Destructor
//////////////////////////////////////////////////////////////////////////////
H264VideoDecoder::~H264VideoDecoder(void)
{
    Close();
}

Status H264VideoDecoder::GetFrame(MediaData* src, MediaData* dst)
{
    Status umcRes = UMC_OK;

    try
    {
        if (!m_IsInitialized)
            return UMC_ERR_NOT_INITIALIZED;

        if (!dst)
            return UMC_ERR_NULL_PTR;

        dst->SetDataSize(0);

        // initialize bit stream data
        if (src && MINIMAL_DATA_SIZE > src->GetDataSize())
        {
            src->MoveDataPointer((Ipp32s) src->GetDataSize());
            return UMC_ERR_NOT_ENOUGH_DATA;
        }

        if (!src)
        {
            return GetFrameInternal(src, dst);

            // disable all errors - error's silent mode
            //if (UMC_OK != umcRes && UMC_ERR_NOT_ENOUGH_DATA != umcRes && UMC_ERR_NOT_ENOUGH_BUFFER != umcRes)
              //  return umcRes;
        }

        src->m_iFlags = MDF_NOT_FULL_FRAME;

        umcRes = GetFrameInternal(src, dst);
        return umcRes;
    }
    catch(const h264_exception &ex)
    {
        m_pTaskSupplier->AfterErrorRestore();
        return ex.GetStatus();
    }
    catch(...)
    {
        m_pTaskSupplier->AfterErrorRestore();
        return UMC_ERR_INVALID_STREAM;
    }

} // Status H264VideoDecoder::GetFrame(MediaData* src, MediaData* dst)

Status H264VideoDecoder::GetFrameInternal(MediaData* src, MediaData* dst)
{
    bool force = (src == 0);

    Status umcRes;
    do
    {
        umcRes = m_pTaskSupplier->GetFrame(src, dst);

        if (umcRes == UMC_ERR_NOT_ENOUGH_BUFFER)
        {
            umcRes = UMC_ERR_NOT_ENOUGH_DATA;
            force = true;
        }

        if (UMC_WRN_INVALID_STREAM == umcRes)
        {
            umcRes = UMC_ERR_NOT_ENOUGH_DATA;
        }

        if (UMC_OK != umcRes && UMC_ERR_NOT_ENOUGH_DATA != umcRes)
        {
            return umcRes;
        }

        if (dst->GetDataSize())
            return UMC_OK;

        umcRes = m_pTaskSupplier->GetFrameToDisplay(dst, force) ? UMC_OK : UMC_ERR_NOT_ENOUGH_DATA;

    } while ((UMC_ERR_NOT_ENOUGH_DATA == umcRes) &&
            (src && dst) &&
            (MINIMAL_DATA_SIZE < src->GetDataSize()));

    return umcRes;
}

Status H264VideoDecoder::GetInfo(BaseCodecParams* params)
{
    Status umcRes = UMC_OK;
    VideoDecoderParams *lpInfo = DynamicCast<VideoDecoderParams, BaseCodecParams> (params);

    if (!m_IsInitialized)
    {
        if (params->m_pData)
        {
            umcRes = VideoDecoder::Init(params);
            if (UMC_OK != umcRes)
                return umcRes;

            m_pTaskSupplier = new TaskSupplier();
            m_pTaskSupplier->SetMemoryAllocator(m_pMemoryAllocator);
            m_pTaskSupplier->PreInit(params);

            if (lpInfo->m_pData && lpInfo->m_pData->GetDataSize())
            {
                umcRes = m_pTaskSupplier->GetFrame((MediaData *) lpInfo->m_pData, 0);
            }

            umcRes = m_pTaskSupplier->GetInfo(lpInfo);
            Close();
            return umcRes;
        }

        return UMC_ERR_NOT_INITIALIZED;
    }

    if (NULL == lpInfo)
        return UMC_ERR_NULL_PTR;

    if (UMC_OK == umcRes)
    {
        return m_pTaskSupplier->GetInfo(lpInfo);
    }

    return umcRes;
}

Status  H264VideoDecoder::ChangeVideoDecodingSpeed(Ipp32s& num)
{
    if (!m_IsInitialized)
        return UMC_ERR_NOT_INITIALIZED;

    m_pTaskSupplier->ChangeVideoDecodingSpeed(num);
    return UMC_OK;
}

H264VideoDecoder::SkipInfo H264VideoDecoder::GetSkipInfo() const
{
    if (!m_IsInitialized)
    {
        H264VideoDecoder::SkipInfo defSkip = {false, 0};

        return defSkip;
    }

    return m_pTaskSupplier->GetSkipInfo();
}

Status H264VideoDecoder::GetUserData(MediaData * pUD)
{
    if (!m_IsInitialized)
        return UMC_ERR_NOT_INITIALIZED;

    if (!pUD)
        return UMC_ERR_NULL_PTR;

    return m_pTaskSupplier->GetUserData(pUD);
}

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
