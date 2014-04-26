/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifdef UMC_ENABLE_MSDK_INTERFACE

#include "vm_types.h"

#include <math.h>
#include <psapi.h>

#include "null_audio_render.h"

#include "codec_selector.h"
#include "pipeline_msdk.h"

#include "mfxmvc.h"
#include "mfxjpeg.h"

#include "ipps.h"

//#define D3D_SURFACES_SUPPORT

#ifdef D3D_SURFACES_SUPPORT
#include <d3d9.h>
#include <dxva2api.h>
#include "msdk_d3d_allocator.h"
#endif

#include "msdk_sysmem_allocator.h"

using namespace UMC;


#define TASK_SWITCH()   vm_time_sleep(0);
#define REPOSITION_AVSYNC_PRECISION 0.99
#define DEF_TIME_TO_SLEEP 5
#define SKIP_FRAME_TOLERENCE 7

static Status SurfaceToVideoData(mfxFrameSurface1 *pSurface, VideoData *pData)
{
    if(!pSurface || !pData)
        return UMC_ERR_NULL_PTR;

    switch(pSurface->Info.FourCC)
    {
    case MFX_FOURCC_NV12:
        pData->Init(pSurface->Info.Width, pSurface->Info.Height, NV12);
        pData->SetPlaneDataPtr(pSurface->Data.Y,  0);
        pData->SetPlaneDataPtr(pSurface->Data.UV, 1);
        pData->SetPlanePitch(pSurface->Data.Pitch, 0);
        pData->SetPlanePitch(pSurface->Data.Pitch, 1);
        break;
    case MFX_FOURCC_YV12:
        pData->Init(pSurface->Info.Width, pSurface->Info.Height, YV12);
        pData->SetPlaneDataPtr(pSurface->Data.Y, 0);
        pData->SetPlaneDataPtr(pSurface->Data.V, 1);
        pData->SetPlaneDataPtr(pSurface->Data.U, 1);
        pData->SetPlanePitch(pSurface->Data.Pitch, 0);
        pData->SetPlanePitch(pSurface->Data.Pitch/2, 1);
        pData->SetPlanePitch(pSurface->Data.Pitch/2, 2);
        break;
    case MFX_FOURCC_YUY2:
        pData->Init(pSurface->Info.Width, pSurface->Info.Height, YUY2);
        pData->SetPlaneDataPtr(pSurface->Data.Y, 0);
        pData->SetPlanePitch(pSurface->Data.Pitch, 0);
        break;
    case MFX_FOURCC_RGB3:
        pData->Init(pSurface->Info.Width, pSurface->Info.Height, RGB);
        pData->SetPlaneDataPtr(pSurface->Data.R, 0);
        pData->SetPlanePitch(pSurface->Data.Pitch, 0);
        break;
    case MFX_FOURCC_RGB4:
        pData->Init(pSurface->Info.Width, pSurface->Info.Height, RGBA);
        pData->SetPlaneDataPtr(pSurface->Data.R, 0);
        pData->SetPlanePitch(pSurface->Data.Pitch, 0);
        break;
    default:
        return UMC_ERR_UNSUPPORTED;
    }

    return UMC_OK;
}

bool getLibName(DString *pLibName)
{
    HANDLE   hCurrent = GetCurrentProcess();
    HMODULE *pModules;
    DWORD    iSize;
    Ipp32u   iModules;
    vm_char  cBuffer[255];
    Ipp32u   iBufferSize = 255;

    if(!EnumProcessModules(hCurrent, NULL, 0, &iSize))
        return false;

    iModules = iSize / sizeof(HMODULE);

    pModules = new HMODULE[iModules];
    if(!pModules)
        return false;

    if(!EnumProcessModules(hCurrent, pModules, iSize, &iSize))
    {
        delete []pModules;
        return false;
    }

    for(Ipp32u i = 0; i < iModules; i++)
    {
        GetModuleFileName(pModules[i], cBuffer, iBufferSize);
        if (_tcsstr(cBuffer, VM_STRING("libmfx")))
        {
            if(pLibName->Size())
                *pLibName += VM_STRING("\n");
            *pLibName += cBuffer;
        }
    }

    delete []pModules;
    return (pLibName->Size())?true:false;
}

PipelineMSDK::PipelineMSDK()
{
    m_pSplitInfo        = NULL;
    m_pDataReader       = NULL;
    m_pSplitter         = NULL;
    m_pAudioDecoder     = NULL;
    m_pDTAudioCodec     = NULL;
    m_pMediaBuffer      = NULL;
    m_pAudioRender      = NULL;
    m_pVideoDecoder     = NULL;
    m_pVideoRender      = NULL;
    m_bStopFlag         = false;
    m_bAudioPlaying     = false;
    m_bVideoPlaying     = false;
    m_bPaused           = false;
    m_bSync             = false;
    m_bExtFrameRate     = false;
    m_pVideoInfo        = NULL;
    m_pAudioInfo        = NULL;
    m_iAudioTrack       = -1;
    m_iVideoTrack       = -1;

    m_iFreq               = vm_time_get_frequency();
    m_bIsMuted            = false;
    m_fCurrentAudioVolume = 0.;

    m_pVideoDecoder       = NULL;
    m_pSurfaces           = NULL;
    m_pAllocator          = NULL;
    m_pAllocatorParams    = NULL;
    m_bD3DAlloc           = false;
    m_bExternalAlloc      = false;
    m_bMVCBufferAllocated = false;

    UMC_SET_ZERO(m_videoParams);
    UMC_SET_ZERO(m_response);
}

PipelineMSDK::~PipelineMSDK()
{
    Close();
}

Status PipelineMSDK::Init(BasePipelineParams *pBaseParams)
{
    MSDKPipelineParams *pParams = DynamicCast<MSDKPipelineParams, BasePipelineParams>(pBaseParams);
    Status umcRes = UMC_OK;

    if(!pParams)
        return UMC_ERR_NULL_PTR;

    m_params = *pParams;

    Close();

    umcRes = InitDataReader(&m_params, &m_pDataReader);
    if(UMC_OK != umcRes)
        return umcRes;

    umcRes = InitSplitter(&m_params, m_pDataReader, &m_pSplitter);
    if(UMC_OK != umcRes)
        return umcRes;

    umcRes = m_pSplitter->GetInfo(&m_pSplitInfo);
    if(UMC_OK != umcRes)
        return umcRes;

    for(Ipp32u i = 0; i < m_pSplitInfo->m_iTracks; i++)
    {
        if(m_pSplitInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO)
        {
            if(m_pSplitInfo->m_ppTrackInfo[i]->m_bEnabled && (m_params.m_iSelectedVideoID == (Ipp32s)m_pSplitInfo->m_ppTrackInfo[i]->m_iPID))
                m_iVideoTrack = i;
        }
        else if(m_pSplitInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_AUDIO)
        {
            if(m_pSplitInfo->m_ppTrackInfo[i]->m_bEnabled && (m_params.m_iSelectedAudioID == (Ipp32s)m_pSplitInfo->m_ppTrackInfo[i]->m_iPID))
                m_iAudioTrack = i;
        }
    }

    for(Ipp32u i = 0; i < m_pSplitInfo->m_iTracks; i++)
    {
        if(m_pSplitInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO)
        {
            if(m_iVideoTrack != -1 && m_iVideoTrack != (Ipp32s)i)
                continue;
            if(m_pSplitInfo->m_ppTrackInfo[i]->m_bEnabled)
            {
                m_pVideoInfo  = (VideoStreamInfo*)m_pSplitInfo->m_ppTrackInfo[i]->m_pStreamInfo;
                m_iVideoTrack = i;
            }
        }
        else if(m_pSplitInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_AUDIO)
        {
            if(m_iAudioTrack != -1 && m_iAudioTrack != (Ipp32s)i)
                continue;
            if(m_pSplitInfo->m_ppTrackInfo[i]->m_bEnabled)
            {
                m_pAudioInfo  = (AudioStreamInfo*)m_pSplitInfo->m_ppTrackInfo[i]->m_pStreamInfo;
                m_iAudioTrack = i;
            }
        }
    }

    if(m_pAudioInfo)
    {
        // Get splitter some time to read out at least one audio frame
        vm_time_sleep(0);

        umcRes = InitDTAudioDecoder(&m_params, m_pAudioInfo, m_pSplitInfo->m_ppTrackInfo[m_iAudioTrack]->m_pHeader, &m_pMediaBuffer, &m_pAudioDecoder, &m_pDTAudioCodec);
        if(UMC_ERR_INVALID_STREAM == umcRes || UMC_ERR_UNSUPPORTED == umcRes)
        {
            m_pSplitter->SetTrackState(m_iAudioTrack, TRACK_DISABLED); // disable unsupported track
            m_pAudioInfo  = NULL;
            m_iAudioTrack = -1;
            umcRes = UMC_OK;
        }
        else if(UMC_OK != umcRes)
            return umcRes;

        if(m_pAudioInfo)
        {
            umcRes = InitAudioRender(&m_params, m_pAudioInfo, &m_pAudioRender);
            if(UMC_OK != umcRes)
            {
                vm_string_printf(VM_STRING("Audio render initialization problem, status = %d\n"), umcRes);
                return umcRes;
            }
            m_params.m_iPrefAudioRender = m_pAudioRender->GetRenderType();
        }
    }

    if(m_pVideoInfo)
    {
        mfxFrameAllocRequest request;
        mfxBitstream         bs;
        mfxStatus            sts         = MFX_ERR_NONE;
        mfxVersion           version     = {3, 1}; // mjpeg, mvc requirement
        mfxIMPL              impl        = MFX_IMPL_AUTO;
        mfxU16               iSurfNumber = 0;
        MediaData            frameHeader;
        DString              sLibName;

        UMC_SET_ZERO(bs);
        UMC_SET_ZERO(request);

        if(m_params.m_iMsdkImpl == 1)
            impl = MFX_IMPL_SOFTWARE;
        else if(m_params.m_iMsdkImpl == 2)
            impl = MFX_IMPL_HARDWARE;

        VideoColorConversionParams vccParams;
        VideoDeinterlaceParams      vdParams;
        VideoResizeParams           vrParams;
        VideoAnaglyphParams         vaParams;

        vm_string_printf(VM_STRING("\n"));
        switch(m_pVideoInfo->streamType)
        {
        case MPEG1_VIDEO:
        case MPEG2_VIDEO:
            m_videoParams.mfx.CodecId = MFX_CODEC_MPEG2;
            break;

        case H264_VIDEO:
            m_videoParams.mfx.CodecId = MFX_CODEC_AVC;
            break;

        case VC1_VIDEO:
        case WMV_VIDEO:
            m_videoParams.mfx.CodecId = MFX_CODEC_VC1;
            break;

        case MJPEG_VIDEO:
            m_videoParams.mfx.CodecId = MFX_CODEC_JPEG;
            break;

        default:
            vm_string_printf(VM_STRING("mfxlib: unsupported codec\n"));
            m_pSplitter->SetTrackState(m_iVideoTrack, TRACK_DISABLED); // disable unsupported track
            m_pVideoInfo = NULL;
            umcRes = UMC_OK;
        }

        sts = m_session.Init(impl, &version);
        if(sts != MFX_ERR_NONE)
            return UMC_ERR_FAILED;

        if(getLibName(&sLibName))
            vm_string_printf(VM_STRING("mfxlib: %s\n"), (vm_char*)sLibName);

        sts = MFXQueryIMPL(m_session, &impl);
        if(sts != MFX_ERR_NONE)
            return UMC_ERR_FAILED;

        vm_string_printf(VM_STRING("mfxlib: "));
        switch(impl&0xff)
        {
        case MFX_IMPL_SOFTWARE:
            vm_string_printf(VM_STRING("software "));
            break;
        case MFX_IMPL_HARDWARE:
            vm_string_printf(VM_STRING("hardware (def. dev.) "));
            break;
        case MFX_IMPL_HARDWARE2:
            vm_string_printf(VM_STRING("hardware (dev. 2) "));
            break;
        case MFX_IMPL_HARDWARE3:
            vm_string_printf(VM_STRING("hardware (dev. 3) "));
            break;
        case MFX_IMPL_HARDWARE4:
            vm_string_printf(VM_STRING("hardware (dev. 4) "));
            break;
        }
        if((impl&0xff00) & MFX_IMPL_VIA_D3D9)
            vm_string_printf(VM_STRING("via D3D9 "));
        else if((impl&0xff00) & MFX_IMPL_VIA_D3D11)
            vm_string_printf(VM_STRING("via D3D11 "));

        sts = MFXQueryVersion(m_session, &version);
        if(sts != MFX_ERR_NONE)
            return UMC_ERR_FAILED;

        vm_string_printf(VM_STRING("v%d.%d\n"), version.Major, version.Minor);

        // create decoder
        m_pVideoDecoder = new MFXVideoDECODE(m_session);
        if(!m_pVideoDecoder)
            return UMC_ERR_ALLOC;

        m_pSplitter->CheckNextData(&frameHeader, m_iVideoTrack); // usual m_pHeader may be not enougth for mfx, so we check out full frame

        bs.Data       = (mfxU8*)frameHeader.GetDataPointer();
        bs.MaxLength  = (Ipp32u)frameHeader.GetDataSize();
        bs.DataLength = (Ipp32u)frameHeader.GetDataSize();
        bs.DataOffset = 0;

        sts = m_pVideoDecoder->DecodeHeader(&bs, &m_videoParams);
        if(sts == MFX_WRN_PARTIAL_ACCELERATION)
            vm_string_printf(VM_STRING("mfxlib: partial acceleration\n"), version.Major, version.Minor);
        else if(sts != MFX_ERR_NONE)
            return UMC_ERR_FAILED;

        // check extension data
        if(m_pSplitInfo->m_ppTrackInfo[m_iVideoTrack]->m_pHeaderExt && m_params.m_bMsdkMvc)
        {
            if(m_pVideoInfo->streamSubtype == MULTIVIEW_VIDEO)
            {
                mfxExtMVCSeqDesc *pMVCDesc = new mfxExtMVCSeqDesc();
                if(!pMVCDesc)
                    return UMC_ERR_ALLOC;

                pMVCDesc->Header.BufferId = MFX_EXTBUFF_MVC_SEQ_DESC;
                pMVCDesc->Header.BufferSz = sizeof(mfxExtMVCSeqDesc);

                m_videoParams.ExtParam = new mfxExtBuffer*[1];
                if(!m_videoParams.ExtParam)
                {
                    delete pMVCDesc;
                    return UMC_ERR_ALLOC;
                }

                m_videoParams.ExtParam[0] = (mfxExtBuffer*)pMVCDesc;
                m_videoParams.NumExtParam = 1;

                bs.Data       = (mfxU8*)m_pSplitInfo->m_ppTrackInfo[m_iVideoTrack]->m_pHeaderExt->GetDataPointer();
                bs.MaxLength  = (Ipp32u)m_pSplitInfo->m_ppTrackInfo[m_iVideoTrack]->m_pHeaderExt->GetDataSize();
                bs.DataLength = (Ipp32u)m_pSplitInfo->m_ppTrackInfo[m_iVideoTrack]->m_pHeaderExt->GetDataSize();
                bs.DataOffset = 0;

                sts = m_pVideoDecoder->DecodeHeader(&bs, &m_videoParams);
                if(MFX_ERR_NOT_ENOUGH_BUFFER == sts)
                {
                    mfxExtMVCSeqDesc* pMVCBuffer = (mfxExtMVCSeqDesc*) m_videoParams.ExtParam[0];
                    if(!pMVCBuffer)
                        return UMC_ERR_ALLOC;

                    pMVCBuffer->View = new mfxMVCViewDependency[pMVCBuffer->NumView];
                    if(!pMVCBuffer->View)
                        return UMC_ERR_ALLOC;

                    for(Ipp32u i = 0; i < pMVCBuffer->NumView; ++i)
                        UMC_SET_ZERO(pMVCBuffer->View[i]);

                    pMVCBuffer->NumViewAlloc = pMVCBuffer->NumView;

                    pMVCBuffer->ViewId = new mfxU16[pMVCBuffer->NumViewId];
                    if(!pMVCBuffer->ViewId)
                        return UMC_ERR_ALLOC;

                    for(Ipp32u i = 0; i < pMVCBuffer->NumViewId; ++i)
                        UMC_SET_ZERO(pMVCBuffer->ViewId[i]);

                    pMVCBuffer->NumViewIdAlloc = pMVCBuffer->NumViewId;

                    pMVCBuffer->OP = new mfxMVCOperationPoint[pMVCBuffer->NumOP];
                    if(!pMVCBuffer->OP)
                        return UMC_ERR_ALLOC;

                    for(Ipp32u i = 0; i < pMVCBuffer->NumOP; ++i)
                        UMC_SET_ZERO(pMVCBuffer->OP[i]);

                    pMVCBuffer->NumOPAlloc = pMVCBuffer->NumOP;

                    m_bMVCBufferAllocated = true;
                }
                else if(sts != MFX_ERR_NONE && sts != MFX_WRN_PARTIAL_ACCELERATION)
                    return UMC_ERR_FAILED;

                sts = m_pVideoDecoder->DecodeHeader(&bs, &m_videoParams);
                if(MFX_ERR_NOT_ENOUGH_BUFFER == sts)
                    return UMC_ERR_ALLOC;
                else if(sts != MFX_ERR_NONE && sts != MFX_WRN_PARTIAL_ACCELERATION)
                    return UMC_ERR_FAILED;

                vm_string_printf(VM_STRING("mfxlib: multiview enabled\n"), version.Major, version.Minor);
            }
            else if(m_pVideoInfo->streamSubtype == SCALABLE_VIDEO)
                vm_string_printf(VM_STRING("Warning: stream scalability data is not supported\n"));
        }

        if(bs.PicStruct == MFX_PICSTRUCT_FIELD_TFF || bs.PicStruct == MFX_PICSTRUCT_FIELD_BFF)
        {
            m_videoParams.mfx.FrameInfo.CropH *= 2;
            m_videoParams.mfx.FrameInfo.Height = align_value<mfxU16>(m_videoParams.mfx.FrameInfo.CropH, 16);
            m_videoParams.mfx.FrameInfo.PicStruct = bs.PicStruct;
        }

        m_params.m_dstVideoData.Init(&m_pVideoInfo->videoInfo);

        switch(m_videoParams.mfx.FrameInfo.FourCC)
        {
        case MFX_FOURCC_NV12:
            m_params.m_dstVideoData.m_colorFormat = NV12;
            break;
        case MFX_FOURCC_YV12:
            m_params.m_dstVideoData.m_colorFormat = YV12;
            break;
        case MFX_FOURCC_YUY2:
            m_params.m_dstVideoData.m_colorFormat = YUY2;
            break;
        case MFX_FOURCC_RGB3:
            m_params.m_dstVideoData.m_colorFormat = RGB;
            break;
        case MFX_FOURCC_RGB4:
            m_params.m_dstVideoData.m_colorFormat = RGBA;
            break;
        }

        m_videoParams.IOPattern  = (mfxU16)(m_bD3DAlloc ? MFX_IOPATTERN_OUT_VIDEO_MEMORY : MFX_IOPATTERN_OUT_SYSTEM_MEMORY);
        m_videoParams.AsyncDepth = 1;

        sts = m_pVideoDecoder->Query(&m_videoParams, &m_videoParams);
        if(sts != MFX_ERR_NONE && sts != MFX_WRN_PARTIAL_ACCELERATION)
            return UMC_ERR_FAILED;

        sts = m_pVideoDecoder->QueryIOSurf(&m_videoParams, &request);
        if(sts != MFX_ERR_NONE && sts != MFX_WRN_PARTIAL_ACCELERATION)
            return UMC_ERR_FAILED;

        iSurfNumber = IPP_MAX(request.NumFrameSuggested, 1);
        request.NumFrameMin       = iSurfNumber;
        request.NumFrameSuggested = iSurfNumber;

        memcpy(&(request.Info), &(m_videoParams.mfx.FrameInfo), sizeof(mfxFrameInfo));
        request.Type  = MFX_MEMTYPE_EXTERNAL_FRAME | MFX_MEMTYPE_FROM_DECODE; 
        request.Type |=  m_bD3DAlloc ? MFX_MEMTYPE_VIDEO_MEMORY_DECODER_TARGET : MFX_MEMTYPE_SYSTEM_MEMORY; 

        m_pAllocator = new SysMemFrameAllocator();
        if(!m_pAllocator)
            return UMC_ERR_ALLOC;

        sts = m_pAllocator->Init(m_pAllocatorParams);
        if(sts != MFX_ERR_NONE)
            return UMC_ERR_FAILED;

        sts = m_pAllocator->Alloc(m_pAllocator->pthis, &request, &m_response);
        if(sts != MFX_ERR_NONE)
            return UMC_ERR_FAILED;

        iSurfNumber = m_response.NumFrameActual;
        m_pSurfaces = new mfxFrameSurface1[iSurfNumber];
        if(!m_pSurfaces)
            return UMC_ERR_ALLOC;

        for(Ipp32u i = 0; i < iSurfNumber; i++)
        {
            memset(&(m_pSurfaces[i]), 0, sizeof(mfxFrameSurface1));
            memcpy(&(m_pSurfaces[i].Info), &(m_videoParams.mfx.FrameInfo), sizeof(mfxFrameInfo));

            sts = m_pAllocator->Lock(m_pAllocator->pthis, m_response.mids[i], &(m_pSurfaces[i].Data));
            if(sts != MFX_ERR_NONE)
                return UMC_ERR_FAILED;
        }

        sts = m_pVideoDecoder->Init(&m_videoParams);
        if(sts != MFX_ERR_NONE && sts != MFX_WRN_PARTIAL_ACCELERATION)
            return UMC_ERR_FAILED;

        vdParams.m_bEnabled          = m_params.m_bDeinterlace;
        vdParams.m_deinterlacingType = m_params.m_deinterlacingType;
        vrParams.m_interpolationType = m_params.m_interpolationType;
        vaParams.m_bEnabled          = m_params.m_bAnaglyph;

        m_videoProcessor.AddFilter(FILTER_DEINTERLACER,    (BaseCodecParams*)&vdParams);
        m_videoProcessor.AddFilter(FILTER_COLOR_CONVERTER, (BaseCodecParams*)&vccParams);
        m_videoProcessor.AddFilter(FILTER_ANAGLYPHER,      (BaseCodecParams*)&vaParams);
        m_videoProcessor.AddFilter(FILTER_RESIZER,         (BaseCodecParams*)&vrParams);

        if(m_params.m_fFramerate == 0)
            m_params.m_fFramerate = (Ipp32f)m_pVideoInfo->fFramerate;
        else
            m_bExtFrameRate = true;

        if(m_params.m_selectedSize.width && m_params.m_selectedSize.height)
        {
            m_params.m_dstVideoData.m_iWidth  = m_params.m_selectedSize.width;
            m_params.m_dstVideoData.m_iHeight = m_params.m_selectedSize.height;
        }
        if(m_params.m_bKeepAspect)
        {
            Ipp32f fScale = IPP_MIN((Ipp32f)m_params.m_dstVideoData.m_iWidth/m_pVideoInfo->videoInfo.m_iWidth, (Ipp32f)m_params.m_dstVideoData.m_iHeight/m_pVideoInfo->videoInfo.m_iHeight);
            m_params.m_dstVideoData.m_iWidth  = (Ipp32u)UMC_ROUND_32U(fScale*m_pVideoInfo->videoInfo.m_iWidth);
            m_params.m_dstVideoData.m_iHeight = (Ipp32u)UMC_ROUND_32U(fScale*m_pVideoInfo->videoInfo.m_iHeight);

            // correct DAR
            if(m_pVideoInfo->videoInfo.m_iSAWidth != m_pVideoInfo->videoInfo.m_iSAHeight)
            {
                fScale = (Ipp32f)m_pVideoInfo->videoInfo.m_iSAWidth/m_pVideoInfo->videoInfo.m_iSAHeight;
                if(fScale > 1)
                    m_params.m_dstVideoData.m_iWidth = UMC_ROUND_32U(m_params.m_dstVideoData.m_iWidth*fScale);
                else
                    m_params.m_dstVideoData.m_iHeight = UMC_ROUND_32U(m_params.m_dstVideoData.m_iHeight/fScale);
                m_params.m_dstVideoData.m_iSAWidth = m_params.m_dstVideoData.m_iSAHeight = 1;
            }
        }

        if(m_params.m_selectedFormat != NONE)
            m_params.m_dstVideoData.m_colorFormat = m_params.m_selectedFormat;

        if(m_params.m_bDeinterlace)
            m_params.m_dstVideoData.m_picStructure = PS_PROGRESSIVE;
        else
        {
            if(m_params.m_dstVideoData.m_picStructure == PS_TOP_FIELD)
                m_params.m_dstVideoData.m_picStructure = PS_TOP_FIELD_FIRST;
            else if(m_params.m_dstVideoData.m_picStructure == PS_BOTTOM_FIELD)
                m_params.m_dstVideoData.m_picStructure = PS_BOTTOM_FIELD_FIRST;
        }

        umcRes = InitVideoRender(&m_params, m_pVideoInfo, &m_pVideoRender);
        if(UMC_OK != umcRes)
            return umcRes;

        m_params.m_iPrefVideoRender = m_pVideoRender->GetRenderType();
    }

    umcRes = m_StepEvent.Init(1,1);
    if(UMC_OK == umcRes && m_pVideoInfo)
    {
        if(m_params.m_bStep)
            umcRes = m_StepEvent.Reset();
        else
            umcRes = m_StepEvent.Set();
    }
    else if(UMC_OK != umcRes)
        return umcRes;

    m_bSync      = m_params.m_bSync;
    m_bStopFlag  = false;

    m_stat.Reset();
    m_stat.fFrameRate = m_params.m_fFramerate;

    m_params.m_iSelectedVideoID = m_iVideoTrack;
    m_params.m_iSelectedAudioID = m_iAudioTrack;

    m_pSplitter->Run();

    if(m_pAudioInfo && m_pVideoInfo)
        m_stat.fDuration = ((m_pVideoInfo->fDuration < m_pAudioInfo->fDuration) ? m_pAudioInfo->fDuration: m_pVideoInfo->fDuration);
    else if(m_pVideoInfo)
        m_stat.fDuration = m_pVideoInfo->fDuration;
    else if(m_pAudioInfo)
    {
        if(m_params.m_iAudioRenderFlags & FLAG_AREN_HIDDEN)
            Mute();
        m_stat.fDuration = m_pAudioInfo->fDuration;
    }

    m_videoWait.Init(0, 0);

    return umcRes;
}

Status PipelineMSDK::Close()
{
    m_MutAccess.Lock();

    if(!m_bStopFlag)
    {
        m_bStopFlag = true;
        m_StepEvent.Set();
        m_videoWait.Set();

        if(m_VideoThread.IsValid())
            m_VideoThread.Wait();

        if(m_SyncThread.IsValid())
            m_SyncThread.Wait();

        if(m_AudioThread.IsValid())
            m_AudioThread.Wait();

        if(NULL != m_pSplitter)
            m_pSplitter->Stop();
    }

    UMC_DELETE(m_pVideoRender);
    if(m_pAudioRender)
    {
        m_pAudioRender->Stop();
        UMC_DELETE(m_pAudioRender);
    }
    UMC_DELETE(m_pDTAudioCodec);
    UMC_DELETE(m_pMediaBuffer);
    UMC_DELETE(m_pAudioDecoder);
    UMC_DELETE(m_pSplitter);
    UMC_DELETE(m_pDataReader);

    UMC_DELETE(m_pVideoDecoder);
    if(m_videoParams.ExtParam)
    {
        for(Ipp32u i = 0; i < m_videoParams.NumExtParam; i++)
        {
            if(m_videoParams.ExtParam[i]->BufferId == MFX_EXTBUFF_MVC_SEQ_DESC)
            {
                mfxExtMVCSeqDesc* pMVCBuffer = (mfxExtMVCSeqDesc*)m_videoParams.ExtParam[i];
                UMC_DELETE(pMVCBuffer->View);
                UMC_DELETE(pMVCBuffer->ViewId);
                UMC_DELETE(pMVCBuffer->OP);
            }

            UMC_DELETE(m_videoParams.ExtParam[i]);
        }
        UMC_DELETE(m_videoParams.ExtParam);
    }
    UMC_DELETE(m_pSurfaces);
    if(m_pAllocator)
    {
        m_pAllocator->Free(m_pAllocator->pthis, &m_response);
        UMC_DELETE(m_pAllocator);
        UMC_DELETE(m_pAllocatorParams);
    }
    m_session.Close();

    m_MutAccess.Unlock();
    return UMC_OK;
}

Status PipelineMSDK::Start()
{
    Status umcRes = UMC_OK;

    m_MutAccess.Lock();

    if(!IsPlaying())
    {
        m_bStopFlag = false;
        m_bAudioPlaying = false;
        m_bVideoPlaying = false;
        m_stat.Reset();

        if (UMC_OK == umcRes && m_pVideoInfo)
            umcRes = m_VideoThread.Create(VideoThreadProc, this);

        if (UMC_OK == umcRes && m_pAudioInfo)
            umcRes = m_AudioThread.Create(AudioThreadProc, this);

        if (UMC_OK == umcRes && (NULL != m_pVideoRender))
            umcRes = m_SyncThread.Create(SyncThreadProc, this);

        if (UMC_OK == umcRes)
        {
            vm_time_sleep(1);

            m_SyncThread.SetPriority(VM_THREAD_PRIORITY_HIGHEST);
            m_AudioThread.SetPriority(VM_THREAD_PRIORITY_HIGH);
            m_VideoThread.SetPriority(VM_THREAD_PRIORITY_NORMAL);
        }

        if (UMC_OK != umcRes)
            Stop();
    }

    m_MutAccess.Unlock();
    return umcRes;
}

Status PipelineMSDK::Stop()
{
    Status umcRes = UMC_OK;
    m_MutAccess.Lock();

    m_bStopFlag = true;
    m_StepEvent.Set();
    m_videoWait.Set();

    if(m_VideoThread.IsValid())
        m_VideoThread.Wait();

    if(m_SyncThread.IsValid())
        m_SyncThread.Wait();

    if(m_AudioThread.IsValid())
        m_AudioThread.Wait();

    //Splitter can be stopped only after the video thread has finished
    if(NULL != m_pSplitter)
        umcRes = m_pSplitter->Stop();

    m_MutAccess.Unlock();
    return umcRes;
}

Status PipelineMSDK::Pause()
{
    Status umcRes = UMC_OK;
    m_MutAccess.Lock();

    m_bPaused = true;

    if (!m_AudioThread.IsValid())
        umcRes = m_StepEvent.Pulse();
    else
        umcRes = m_pAudioRender->Pause(true);

    m_MutAccess.Unlock();
    return umcRes;
}

Status PipelineMSDK::Resume()
{
    Status umcRes = UMC_OK;
    m_MutAccess.Lock();

    if (NULL != m_pAudioRender)
        umcRes = m_pAudioRender->Pause(false);

    if (UMC_OK == umcRes)
    {
        m_bPaused = false;
        umcRes = m_StepEvent.Set();
    }

    m_MutAccess.Unlock();
    return umcRes;
}

Status PipelineMSDK::SetSpeed(Ipp32f fSpeed)
{
    fSpeed = fSpeed;
    return UMC_ERR_NOT_IMPLEMENTED;
}

bool PipelineMSDK::IsPlaying()
{
    return m_SyncThread.IsValid() || m_AudioThread.IsValid() || m_VideoThread.IsValid();
}

void PipelineMSDK::GetStat(Statistic &stat)
{
    m_MutAccess.Lock();
    stat = m_stat;
    m_MutAccess.Unlock();
}

void PipelineMSDK::WaitForStop()
{
    //always synchronized stream finishing with SyncProc
    if(m_SyncThread.IsValid())
        m_SyncThread.Wait();
    else
    {
        //there is an exception: pure audio files
        //SyncProc is absent in this case
        if(m_AudioThread.IsValid())
            m_AudioThread.Wait();
    }
}

Ipp64u PipelineMSDK::GetStreamSize()
{
    Ipp64u stRes = 0;
    if (NULL != m_pDataReader)
        stRes = m_pDataReader->GetSize();
    return stRes;
}

Ipp64f PipelineMSDK::GetPosition()
{
    return -1;
}

Ipp64f PipelineMSDK::SetPosition(Ipp64f fPos)
{
    fPos = fPos;
    return -1;
}

void PipelineMSDK::SyncProc()
{
    vm_tick t1 = 0, t2 = 0, t2_prev = 0;
    Ipp64f flip_time = 0.001;
    Ipp64f flip_times[3] = {0.001,0.001,0.001};
    Status umcRes = UMC_OK;
    bool bNoAudioAnyMore = (NULL == m_pAudioRender);
    bool bNoVideoAnyMore = (NULL == m_pVideoRender);
    bool bNullAudioRender = false;
    AudioRender* pStoppedAudioRender = NULL;
    Ipp64f prevVideoPts = 0;
    NULLAudioRenderParams renderParams;

    // Wait until video and audio decoding threads will pass some data to
    // the renders - we can't start synchronization without it
    while (!m_bStopFlag &&
          ((NULL != m_pVideoRender && !m_bVideoPlaying) ||
           (NULL != m_pAudioRender && !m_bAudioPlaying)))
    {   vm_time_sleep(1);   }

    m_stat.iFramesRendered = 0;
    // Continue video data rendering until no more data in audio and video
    // render left
    for (Ipp32s frame_num = 0, skip_window = 0;
         UMC_OK == umcRes && (!bNoAudioAnyMore || !bNoVideoAnyMore) && !m_bStopFlag;
         frame_num++)
    {
        m_stat.iFrameNum = frame_num + m_stat.iSkippedNum;

        // Check next frame PTS if any
        if (!bNoVideoAnyMore)
        {
            Status get_fr_sts = UMC_OK;
            do
            {
                VM_ASSERT(NULL != m_pVideoRender);
                get_fr_sts = m_pVideoRender->GetRenderFrame(&(m_stat.fFrameTime));
                if (UMC_OK != get_fr_sts)
                    vm_time_sleep(DEF_TIME_TO_SLEEP);
            } while (get_fr_sts == UMC_ERR_TIMEOUT && !m_bStopFlag);

            if (m_stat.fFrameTime == -1.0 || get_fr_sts != UMC_OK)
            {
               bNoVideoAnyMore = true;
            }
        }
        if(frame_num == 0)
        {
            prevVideoPts = m_stat.fFrameInitTime = m_stat.fFrameTime;
            if(m_stat.fDuration > 0)
                m_stat.fFullDuration = m_stat.fDuration + m_stat.fFrameInitTime;
        }

        // If we have no more audio but some video or if we have no audio at all
        if (m_bSync && ((NULL == m_pAudioRender ) ||
                        (NULL == pStoppedAudioRender &&
                         m_bAudioPlaying &&
                         bNoAudioAnyMore &&
                        !bNoVideoAnyMore)))
        {
            pStoppedAudioRender = m_pAudioRender;
            m_pAudioRender = new NULLAudioRender(m_stat.fFrameTime);
            if (NULL == m_pAudioRender)
            {
                //  signal error, stop everything
                m_bStopFlag = true;
                umcRes = UMC_ERR_ALLOC;
            }
            else
            {
                // Start time counting
                m_pAudioRender->Pause(false);
                m_bAudioPlaying = true;
                bNullAudioRender = true;
            }
        }


        TASK_SWITCH();

        Ipp64f ft = m_stat.fFrameTime - flip_time;


        // Let's synchronize video to audio if there is some data in the audio
        // render or NULLAudioRender is used
        if (!bNoAudioAnyMore || bNullAudioRender)
        {
            VM_ASSERT(NULL != m_pAudioRender);
            VM_ASSERT(NULL != m_pVideoDecoder);

            Ipp64f dfAudioTime = m_pAudioRender->GetTime();
            dfAudioTime += m_pAudioRender->GetDelay();

            // Wait for the next video frame display time if we have one
            if (!bNoVideoAnyMore && m_bSync)
            {
                if(m_params.m_bPrintPTS)
                    printf("PTS V-A: %.4f - %.4f = %.4f", ft, dfAudioTime, dfAudioTime - ft);

                if(prevVideoPts > ft + 1.0 || prevVideoPts + 1.0 < ft) //PTS jump
                {
                    if(abs(dfAudioTime - ft) > 1.0)
                    {
                        // try to syncronize video and audio after PTS jump
                        if (!bNullAudioRender)
                        {
                            volatile Ipp64f st1;
                            Ipp32u   n = 0;
                            for (st1 = dfAudioTime;
                                st1 > 0 && (abs(st1 - ft) > 1.0) && !m_bStopFlag && n < 100; n++)
                            {
                                vm_time_sleep(DEF_TIME_TO_SLEEP);
                                st1 = m_pAudioRender->GetTime();
                                dfAudioTime = st1;
                            }
                        }
                        else
                        {

                            renderParams.m_InitPTS = m_stat.fFrameTime;
                            m_pAudioRender->SetParams(&renderParams);
                            m_pAudioRender->Pause(false);
                        }
                        if(m_params.m_bPrintPTS)
                            printf(" - pts jump");
                     }
                }

                if (ft > dfAudioTime)
                {
                    skip_window = 0;
                    //umcRes = m_pVideoDecoder->ResetSkipCount();
                    //if (UMC_ERR_NOT_IMPLEMENTED == umcRes)
                    //{    umcRes = UMC_OK;    }

                    volatile Ipp64f st1;
                    Ipp32u   n = 0;

                    for (st1 = dfAudioTime; st1 >= dfAudioTime && ft > st1 && !m_bStopFlag && n < 100 ;n++)
                    {
                        Ipp32f a=0;
                        vm_time_sleep(IPP_MAX(0,IPP_MIN(1,(Ipp32s)((ft-st1)*1000))));
                        st1 = m_pAudioRender->GetTime();
                        a = (Ipp32f) m_pAudioRender->GetDelay();
                        st1 += a;

                        if(m_bPaused)
                        {
                            m_pVideoRender->ShowLastFrame();
                        }
                    }
                }
                else if (ft < dfAudioTime &&
                         (dfAudioTime - ft > (0.7/m_params.m_fFramerate)))
                {
                    if (++skip_window >= SKIP_FRAME_TOLERENCE)
                    {
                        skip_window = 0;
                        //umcRes = m_pVideoDecoder->SkipVideoFrame(1);
                        //if (UMC_ERR_NOT_IMPLEMENTED == umcRes)
                        //{   umcRes = UMC_OK;   }
                    }
                    if(m_params.m_bPrintPTS)
                        printf(" - skip attempt (%d).", skip_window);
                }
                if(m_params.m_bPrintPTS)
                    printf("\n");
            }

            // Stop synchronization efforts and play the rest of the video on
            // maximum speed if we have no more audo samples
            if (-1.0 == dfAudioTime)
            {   bNoAudioAnyMore = true; }
        }
        prevVideoPts = m_stat.fFrameTime;

        // It's time to display next video frame
        if (UMC_OK == umcRes && !bNoVideoAnyMore)
        {
            t1 = vm_time_get_tick();
            umcRes = m_pVideoRender->RenderFrame();
            t2 = vm_time_get_tick();
            m_stat.iFramesRendered++;
            m_videoWait.Set();
        }

        // Update Statistic structure and frame display statistic
        if (UMC_OK == umcRes)
        {
//            m_stat.iSkippedNum = m_pVideoDecoder->GetNumOfSkippedFrames();

            Ipp64f this_flip_time =
                    (Ipp64f)(t2-t1)/(Ipp64f)m_iFreq;

            flip_times[0] = flip_times[1];
            flip_times[1] = flip_times[2];
            flip_times[2] = this_flip_time;

            flip_time = (flip_times[0] + flip_times[1] + flip_times[2]) / 3;

            while (VM_TIMEOUT == m_StepEvent.Wait(500))
            {
                m_pVideoRender->ShowLastFrame();
                if (m_bStopFlag)
                {   break;  }
            }

            // ignore the first frame (might be a long wait to be synchronized
            if (1 < m_stat.iFrameNum)
            {
                m_stat.fRenderTime += (Ipp64f)(t2-t2_prev) /
      (Ipp64f)m_iFreq;
                m_stat.fRenderRate =
                    (Ipp64f)(m_stat.iFrameNum - 1) / m_stat.fRenderTime;
            }
            t2_prev = t2;

            TASK_SWITCH();
        }

    }

    UMC_DELETE(pStoppedAudioRender);
}

mfxU16 GetFreeSurfaceIndex(mfxFrameSurface1* pSurfacesPool, mfxU16 nPoolSize)
{    
    if (pSurfacesPool)
    {
        for (mfxU16 i = 0; i < nPoolSize; i++)
        {
            if (0 == pSurfacesPool[i].Data.Locked)
            {       
                return i;
            }
        }
    }   

    return 0xFFFF;
}

void PipelineMSDK::VideoProc()
{
    Status     status = UMC_OK;
    MediaData  inData;
    VideoData  outMfxData;
    VideoData  outData;
    bool       bRenderLocked = false;
    mfxStatus         mfxStatus = MFX_ERR_NONE;
    mfxBitstream      bs;
    mfxBitstream     *pBS = &bs;
    mfxSyncPoint      syncp;
    mfxFrameSurface1 *pOutSurface = NULL;
    mfxU16            iIndex = 0;

    UMC_SET_ZERO(bs);

    outData.Init(&m_params.m_dstVideoData);

    for(; !m_bStopFlag && UMC_OK == status;)
    {
        // Wait for the free buffer in the video render
        do
        {
            status = m_pVideoRender->LockInputBuffer(&outData);
            if(UMC_OK == status)   // Be aware that video render surface was locked and not released yet
            {
                bRenderLocked = true;
                break;
            }
            // there is only one legal error return value, all other are incorrect.
            else if(!m_bStopFlag && (UMC_ERR_NOT_ENOUGH_BUFFER == status ||UMC_ERR_TIMEOUT == status))
                m_videoWait.Wait();
            else
            {
                vm_string_printf(VM_STRING("Error in video render\n"));
                break;
            }
        } while(!m_bStopFlag);

        // Repeat decode procedure until the decoder will agree to decompress at least one frame
        do
        {
            // Get some more data from the the splitter if we've decoded all data from the previous buffer
            if(pBS && (pBS->DataLength <= 4 || mfxStatus == MFX_ERR_MORE_DATA) && mfxStatus != MFX_ERR_MORE_SURFACE)
            {
                do
                {
                    status = m_pSplitter->GetNextData(&inData, m_iVideoTrack);
                } while(status == UMC_ERR_NOT_ENOUGH_DATA && !m_bStopFlag);
                if(status == UMC_ERR_END_OF_STREAM)
                {
                    pBS = NULL; // Ok, here is no more data in the splitter. Let's extract the rest of decoded data from the decoder
                    status = UMC_OK;
                }
                else if(status == UMC_WRN_REPOSITION_INPROGRESS)
                    status = UMC_OK;
                else if(status != UMC_OK)
                {
                    vm_string_printf(VM_STRING("Error in splitter\n"));
                    break;
                }
                else
                {
                    // convert MediaData to mfxBitstream
                    pBS->TimeStamp  = 0;//(mfxU64)pDataIn->m_fPTSStart*1000;
                    pBS->Data       = (mfxU8*)inData.GetDataPointer();
                    pBS->MaxLength  = (mfxU32)inData.GetDataSize();
                    pBS->DataLength = (mfxU32)inData.GetDataSize();
                    pBS->DataOffset = 0;
                }
            }

            if(MFX_ERR_MORE_SURFACE == mfxStatus || MFX_ERR_NONE == mfxStatus)
            {
                iIndex = GetFreeSurfaceIndex(m_pSurfaces, m_response.NumFrameActual); // find new working surface
                if (0xFFFF == iIndex)
                {
                    vm_string_printf(VM_STRING("Error in GetFreeSurfaceIndex\n"));
                    return;
                }
            }

            mfxStatus = m_pVideoDecoder->DecodeFrameAsync(pBS, &(m_pSurfaces[iIndex]), &pOutSurface, &syncp);
            // ignore warnings if output is available, 
            // if no output and no action required just repeat the same call
            if(MFX_ERR_NONE < mfxStatus && syncp) 
                mfxStatus = MFX_ERR_NONE;
            if(MFX_ERR_NONE == mfxStatus)
                mfxStatus = m_session.SyncOperation(syncp, 60000);
            if (MFX_WRN_DEVICE_BUSY == mfxStatus)
                Sleep(1); // just wait and then repeat the same call to DecodeFrameAsync
        } while (((MFX_ERR_MORE_DATA == mfxStatus && pBS) || MFX_ERR_MORE_SURFACE == mfxStatus || mfxStatus == MFX_WRN_VIDEO_PARAM_CHANGED || mfxStatus == MFX_WRN_DEVICE_BUSY) && !m_bStopFlag);

        if(UMC_OK != status || mfxStatus != MFX_ERR_NONE || m_bStopFlag)
            break;

        status = SurfaceToVideoData(pOutSurface, &outMfxData);
        if(status != UMC_OK)
            return;
        outMfxData.m_fPTSStart = m_stat.iFramesDecoded/m_stat.fFrameRate;

        // copy to render buffer
        status = m_videoProcessor.GetFrame(&outMfxData, &outData);
        if(status != UMC_OK)
        {
            vm_string_printf(VM_STRING("Error in m_videoProcessor\n"));
            break;
        }
        if((Ipp64s)pOutSurface->Data.TimeStamp == -1)
            outData.m_fPTSStart = -1;

        if(m_bExtFrameRate)
            outData.m_fPTSStart = m_stat.iFramesDecoded/m_stat.fFrameRate;
        m_stat.iFramesDecoded++;

        // Unlock video render surface
        if (bRenderLocked)
        {
            status = m_pVideoRender->UnLockInputBuffer(&outData);
            bRenderLocked = false;
        }
        m_bVideoPlaying = true;
    }

    if(m_bVideoPlaying == false)
        m_bStopFlag = true;

    if (bRenderLocked)
    {
        outData.m_fPTSStart = -1;
        outData.m_frameType = NONE_PICTURE;
        status = m_pVideoRender->UnLockInputBuffer(&outData, UMC_ERR_END_OF_STREAM);
    }

    m_pVideoRender->Stop();
}

template<class typeSource, class typeMedium>
Status LockInputBuffer(typeSource *pSource, typeMedium *pMedium, bool *pbStop)
{
    Status umcRes;

    do
    {
        umcRes = pSource->LockInputBuffer(pMedium);
        if (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes)
            vm_time_sleep(DEF_TIME_TO_SLEEP);

    } while ((false == *pbStop) &&
             (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes));

    return umcRes;

} // Status LockInputBuffer(typeSource *pSource, typeMedium *pMedium, bool *pbStop)

template<class typeDestination, class typeMedium>
Status LockOutputBuffer(typeDestination *pDestination, typeMedium *pMedium, bool *pbStop)
{
    Status umcRes;

    do
    {
        umcRes = pDestination->LockOutputBuffer(pMedium);
        if (UMC_ERR_NOT_ENOUGH_DATA == umcRes)
            vm_time_sleep(DEF_TIME_TO_SLEEP);

    } while ((false == *pbStop) &&
             (UMC_ERR_NOT_ENOUGH_DATA == umcRes));

    return umcRes;

} // Status LockOutputBuffer(typeDestination *pDestination, typeMedium *pMedium, bool *pbStop)

void PipelineMSDK::AudioProc()
{
    //LOG  (VM_STRING("AudioProc,+"));
    Status umcRes = UMC_OK;
    MediaData ComprData;
    AudioData  UncomprData;
    bool bSplitterIsEmpty = false;
    Ipp64f dfStartTime = 0;
    Ipp32u uiComprSize = 0;
    Ipp32u uiShift = 0;

    // check error(s)
    VM_ASSERT(NULL != m_pSplitter);

    // Continue passing data from the splitter to decoder and from
    // decoder to the render
    UncomprData = m_pAudioInfo->audioInfo;

    while ((false == m_bStopFlag) &&
           (false == bSplitterIsEmpty))
    {
        // 1st step: obtain data from splitter
        do
        {
            umcRes = m_pSplitter->GetNextData(&ComprData, m_iAudioTrack);
            if ((UMC_ERR_NOT_ENOUGH_DATA == umcRes) || (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes))
                vm_time_sleep(5);
        } while ((false == m_bStopFlag) && ((UMC_ERR_NOT_ENOUGH_DATA == umcRes) || (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes)));
        // check error(s) & end of stream
        if (UMC_ERR_END_OF_STREAM == umcRes)
        {
            bSplitterIsEmpty = true;
            ComprData.SetDataSize(0);
        }
        else if (UMC_OK != umcRes)
            break;
        // save data size and data time
        uiComprSize = (Ipp32u) ComprData.GetDataSize();
        dfStartTime = ComprData.m_fPTSStart;

        // decode data and pass them to renderer
        uiShift = 0;
        do
        {
            // 2nd step: compressed data should be passed to the decoder first
            if (m_pDTAudioCodec)
            {
                MediaData buff;

                // get decoder's internal buffer
                umcRes = m_pDTAudioCodec->LockInputBuffer(&buff);
                // check error(s)
                if (UMC_OK != umcRes)
                    break;

                // Copy compressed data to the decoder's buffer
                if (UMC_OK == umcRes)
                {
                    Ipp32u uiDataToCopy = IPP_MIN((Ipp32u) buff.GetBufferSize(), uiComprSize);

                    memcpy(buff.GetDataPointer(),
                           (Ipp8u*)ComprData.GetDataPointer() + uiShift,
                           uiDataToCopy);

                    buff.SetDataSize(uiDataToCopy);
                    buff.m_fPTSStart = dfStartTime;

                    umcRes = m_pDTAudioCodec->UnLockInputBuffer(&buff, (bSplitterIsEmpty) ? (UMC_ERR_END_OF_STREAM) : (UMC_OK));
                    // check error(s)
                    if (UMC_OK != umcRes)
                        break;


                    uiShift += uiDataToCopy;
                    uiComprSize -= uiDataToCopy;
                }
            }

            do
            {
                // wait until audio renderer will free enough internal buffers
                umcRes = LockInputBuffer(m_pAudioRender, &UncomprData, (bool *) &m_bStopFlag);
                // check error(s)
                if (UMC_OK != umcRes)
                {
                   if (!m_bStopFlag)
                      vm_string_printf(VM_STRING("Error in audio render\n"));

                    TASK_SWITCH();
                    break;
                }

                // move decoded data to the renderer

                // brunch for compressed data
                if (m_pDTAudioCodec)
                {
                    UncomprData.SetDataSize(0);
                    vm_tick ullDecTime = vm_time_get_tick();
                    umcRes = m_pDTAudioCodec->GetFrame(&UncomprData);
                    ullDecTime = vm_time_get_tick() - ullDecTime;

                    if (UMC_OK != umcRes)
                        break;
                }
                // branch for PCM data
                else
                {
                    Ipp64f dfStart = 0.0;
                    Ipp64f dfEnd = 0.0;

                    if (0 == uiComprSize)
                        break;

                    Ipp32u uiDataToCopy = (Ipp32u) IPP_MIN(uiComprSize, UncomprData.GetBufferSize());

                    memcpy(UncomprData.GetDataPointer(),
                           ((Ipp8u*)ComprData.GetDataPointer()) + uiShift,
                           uiDataToCopy);
                    UncomprData.SetDataSize(uiDataToCopy);

                    dfStart = ComprData.m_fPTSStart;
                    dfEnd   = ComprData.m_fPTSEnd;
                    Ipp64f dfNorm = (dfEnd - dfStart) / (uiShift + uiComprSize);
                    dfStart += dfNorm * uiShift;
                    dfEnd = dfStart + dfNorm * uiDataToCopy;
                    UncomprData.m_fPTSStart = dfStart;
                    UncomprData.m_fPTSEnd   = dfEnd;

                    uiShift += uiDataToCopy;
                    uiComprSize -= uiDataToCopy;
                }

                // call finalizing function
                if (UncomprData.GetDataSize())
                {
                    umcRes = m_pAudioRender->UnLockInputBuffer(&UncomprData);
                    // check error(s)
                    TASK_SWITCH();
                    if (UMC_OK != umcRes)
                        break;
                    // open SyncProc() only after render starts
                    if (-1. != m_pAudioRender->GetTime())
                        m_bAudioPlaying = true;
                }
            } while (false == m_bStopFlag);
            // check after-cicle error(s)
            if ((UMC_OK != umcRes) &&
                (UMC_ERR_NOT_ENOUGH_DATA != umcRes) &&
                (UMC_ERR_SYNC != umcRes))
                break;

            umcRes = UMC_OK;

        } while ((false == m_bStopFlag) && (0 != uiComprSize));
        // check after-cicle error(s)
        if (UMC_OK != umcRes)
            break;
    }

    // send end of stream to renderer
    if(!m_bStopFlag)
    {
        // wait until audio renderer will free enough intermal buffers
        do
        {
            umcRes = m_pAudioRender->LockInputBuffer(&UncomprData);
            if (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes)
                vm_time_sleep(DEF_TIME_TO_SLEEP);

        } while ((false == m_bStopFlag) &&
                 (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes));
        // check error(s)
        if (UMC_OK == umcRes)
        {
            UncomprData.SetDataSize(0);
            UncomprData.m_fPTSStart = 0;
            umcRes = m_pAudioRender->UnLockInputBuffer(&UncomprData, UMC_ERR_END_OF_STREAM);
        }
    }
}

Ipp32u VM_THREAD_CALLCONVENTION PipelineMSDK::SyncThreadProc(void* pvParam)
{
    VM_ASSERT(NULL != pvParam);
    ((PipelineMSDK*)pvParam)->SyncProc();
    return 0;
}

Ipp32u VM_THREAD_CALLCONVENTION PipelineMSDK::VideoThreadProc(void* pvParam)
{
    VM_ASSERT(NULL != pvParam);
    ((PipelineMSDK*)pvParam)->VideoProc();
    return 0;
}

Ipp32u VM_THREAD_CALLCONVENTION PipelineMSDK::AudioThreadProc(void* pvParam)
{
    VM_ASSERT(NULL != pvParam);
    ((PipelineMSDK*)pvParam)->AudioProc();
    return 0;
}

#endif
