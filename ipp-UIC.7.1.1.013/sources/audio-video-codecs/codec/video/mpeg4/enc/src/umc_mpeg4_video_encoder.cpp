/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER

#include "umc_mpeg4_video_encoder.h"
#include "umc_video_data.h"
#include "vm_debug.h"
#include "vm_time.h"

namespace UMC
{

MPEG4EncoderParams::MPEG4EncoderParams() {
    ippsSet_8u(0, (Ipp8u*)(&m_Param), sizeof(m_Param));
    m_Param.quantIVOP = 4;
    m_Param.quantPVOP = 4;
    m_Param.quantBVOP = 6;
    m_Param.IVOPdist = 300;
    m_Param.PVOPsearchWidth = -15;
    m_Param.PVOPsearchHeight = 15;
    m_Param.BVOPsearchWidthForw = 15;
    m_Param.BVOPsearchHeightForw = 15;
    m_Param.BVOPsearchWidthBack = 15;
    m_Param.BVOPsearchHeightBack = 15;
    m_Param.MEalgorithm = 1;
    m_Param.MEaccuracy = 2;
    m_Param.obmc_disable = 1;
    m_Param.RoundingControl = 1;
    m_Param.SceneChangeThreshold = 45;
    m_Param.bsBuffer = (Ipp8u*)1;
    m_Param.bsBuffSize = 1; // encoder will not allocate buffer
    m_Param.padType = 0; // set 1 for QuickTime(tm) and 2 for DivX (tm) v. >= 5
    m_Param.TimeResolution = 30;
    m_Param.TimeIncrement = 1;
}

MPEG4VideoEncoder::MPEG4VideoEncoder()
{
    m_IsInit = false;
}

MPEG4VideoEncoder::~MPEG4VideoEncoder()
{
    Close();
}

Status MPEG4VideoEncoder::AllocateBuffers()
{
    Status status = UMC_OK;
    int    i;

    // allocate only frame memory, may be extended for whole buffers
    for (i = 0; i < mp4enc->mPlanes; i ++)
        mp4enc->mFrame[i].mid = 0;
    for (i = 0; i < mp4enc->mPlanes; i ++) {
        status = m_pMemoryAllocator->Alloc(&mp4enc->mFrame[i].mid, mp4enc->mLumaPlaneSize + mp4enc->mChromaPlaneSize + mp4enc->mChromaPlaneSize, UMC_ALLOC_PERSISTENT);
        if (status != UMC_OK)
            return status;
    }
    return status;
}

Status MPEG4VideoEncoder::FreeBuffers()
{
    Status status = UMC_OK;
    int    i;

    for (i = 0; i < mp4enc->mPlanes; i ++) {
        if (mp4enc->mFrame[i].mid)
            status = m_pMemoryAllocator->Free(mp4enc->mFrame[i].mid);
        if (status != UMC_OK)
            return status;
    }
    return status;
}

void MPEG4VideoEncoder::LockBuffers()
{
    int    i;

    for (i = 0; i < mp4enc->mPlanes; i ++) {
        mp4enc->mFrame[i].ap = (Ipp8u*)m_pMemoryAllocator->Lock(mp4enc->mFrame[i].mid);
        mp4enc->mFrame[i].pY = mp4enc->mFrame[i].ap + mp4enc->mExpandSizeA + mp4enc->mExpandSizeA * mp4enc->mStepLuma;
        mp4enc->mFrame[i].pU = mp4enc->mFrame[i].ap + mp4enc->mLumaPlaneSize + (mp4enc->mExpandSizeA >> 1) + (mp4enc->mExpandSizeA >> 1) * mp4enc->mStepChroma;
        mp4enc->mFrame[i].pV = mp4enc->mFrame[i].ap + mp4enc->mLumaPlaneSize + mp4enc->mChromaPlaneSize + (mp4enc->mExpandSizeA >> 1) + (mp4enc->mExpandSizeA >> 1) * mp4enc->mStepChroma;
    }
}

Status MPEG4VideoEncoder::UnlockBuffers()
{
    Status status = UMC_OK;
    int    i;

    for (i = 0; i < mp4enc->mPlanes; i ++) {
        status = m_pMemoryAllocator->Unlock(mp4enc->mFrame[i].mid);
        if (status != UMC_OK)
            return status;
    }
    return status;
}

Status MPEG4VideoEncoder::Reset()
{
    return Init(&m_Param);
    //return UMC_ERR_NOT_IMPLEMENTED;
}

Status MPEG4VideoEncoder::SetParams(BaseCodecParams* baseParams)
{
    VideoEncoderParams *bParam = DynamicCast<VideoEncoderParams, BaseCodecParams>(baseParams);

    if (bParam == NULL)
        return UMC_ERR_UNSUPPORTED;
    // only BitRate and FrameRate could be changed
    if (bParam->m_info.iBitrate == m_Param.m_info.iBitrate && bParam->m_info.fFramerate == m_Param.m_info.fFramerate)
        return UMC_ERR_UNSUPPORTED;
    m_Param.m_info.iBitrate = bParam->m_info.iBitrate;
    m_Param.m_info.fFramerate = bParam->m_info.fFramerate;
    mp4enc->ResetRC(m_Param.m_info.iBitrate, m_Param.m_info.fFramerate);
    return UMC_OK;
    //return UMC_ERR_NOT_IMPLEMENTED;
}

Status MPEG4VideoEncoder::Init(BaseCodecParams* init)
{
    MPEG4EncoderParams *pParam = DynamicCast<MPEG4EncoderParams, BaseCodecParams>(init);
    VideoEncoderParams *vParam = DynamicCast<VideoEncoderParams, BaseCodecParams>(init);

    if (vParam == NULL && pParam == NULL)
        return UMC_ERR_NULL_PTR;
    if (m_IsInit)
        Close();
    mp4enc = new MPEG4_ENC::VideoEncoderMPEG4;
    if (!mp4enc)
        return UMC_ERR_ALLOC;
    if (pParam == NULL) {
        // default params are in constructor
        m_Param.m_Param.Width = vParam->m_info.videoInfo.m_iWidth;
        m_Param.m_Param.Height = vParam->m_info.videoInfo.m_iHeight;
        if (vParam->m_info.iBitrate <= 0) {
            m_Param.m_Param.RateControl = 0;
            m_Param.m_Param.BitRate = 0;
        } else {
            m_Param.m_Param.RateControl = 1;
            m_Param.m_Param.BitRate = vParam->m_info.iBitrate;
        }
        m_Param.m_info.fFramerate = vParam->m_info.fFramerate;
        if (vParam->m_info.fFramerate > 0 && (vParam->m_info.fFramerate == (Ipp32s)vParam->m_info.fFramerate)) {
            m_Param.m_Param.TimeResolution = (Ipp32s)vParam->m_info.fFramerate;
            m_Param.m_Param.TimeIncrement = 1;
        } else {
            if (vParam->m_info.fFramerate >= 23.976 && vParam->m_info.fFramerate < 24) {
                m_Param.m_Param.TimeResolution = 24000;
                m_Param.m_Param.TimeIncrement = 1001;
            } else if (vParam->m_info.fFramerate >= 29.97 && vParam->m_info.fFramerate < 30) {
                m_Param.m_Param.TimeResolution = 30000;
                m_Param.m_Param.TimeIncrement = 1001;
            } else {
                m_Param.m_Param.TimeResolution = 30;
                m_Param.m_Param.TimeIncrement = 1;
                m_Param.m_info.fFramerate = 30;
            }
        }
        m_Param.m_info.videoInfo.m_iWidth = vParam->m_info.videoInfo.m_iWidth;
        m_Param.m_info.videoInfo.m_iHeight = vParam->m_info.videoInfo.m_iHeight;
        //m_Param.numFramesToEncode = vParam->numFramesToEncode;
        m_Param.m_info.iBitrate = vParam->m_info.iBitrate;
    } else {
        m_Param = *pParam;
        // override MPEG-4 params if base params are valid
        if (m_Param.m_info.videoInfo.m_iWidth && m_Param.m_info.videoInfo.m_iHeight) {
            m_Param.m_Param.Width = m_Param.m_info.videoInfo.m_iWidth;
            m_Param.m_Param.Height = m_Param.m_info.videoInfo.m_iHeight;
        }
        if (m_Param.m_info.fFramerate > 0) {
            if (m_Param.m_info.fFramerate == (Ipp32s)m_Param.m_info.fFramerate) {
                m_Param.m_Param.TimeResolution = (Ipp32s)m_Param.m_info.fFramerate;
                m_Param.m_Param.TimeIncrement = 1;
            } else {
                if (m_Param.m_info.fFramerate >= 23.976 && m_Param.m_info.fFramerate < 24) {
                    m_Param.m_Param.TimeResolution = 24000;
                    m_Param.m_Param.TimeIncrement = 1001;
                } else if (m_Param.m_info.fFramerate >= 29.97 && m_Param.m_info.fFramerate < 30) {
                    m_Param.m_Param.TimeResolution = 30000;
                    m_Param.m_Param.TimeIncrement = 1001;
                }
            }
        } else
            m_Param.m_info.fFramerate = m_Param.m_Param.TimeResolution / m_Param.m_Param.TimeIncrement;
        if (m_Param.m_info.iBitrate > 0) {
            if (m_Param.m_Param.RateControl == 0)
                m_Param.m_Param.RateControl = 1;
            m_Param.m_Param.BitRate = m_Param.m_info.iBitrate;
        }
    }
    m_Param.m_Param.numThreads = vParam->m_iThreads;
    m_Param.m_Param.profile_and_level = (Ipp8u)((vParam->m_info.iProfile << 4) + (vParam->m_info.iLevel & 15));
    m_Param.m_Param.aspect_ratio_width = (Ipp8u)m_Param.m_info.videoInfo.m_iSAWidth;
    m_Param.m_Param.aspect_ratio_height = (Ipp8u)m_Param.m_info.videoInfo.m_iSAHeight;
    Ipp32s  mp4status = mp4enc->Init(&m_Param.m_Param);
    if (mp4status == MPEG4_ENC::MP4_STS_ERR_PARAM)
        return UMC_ERR_INIT;
    if (mp4status == MPEG4_ENC::MP4_STS_ERR_NOMEM)
        return UMC_ERR_ALLOC;
    m_FrameCount = 0;
    if (m_Param.m_Param.BVOPdist) {
        bTime = new Ipp64f [m_Param.m_Param.BVOPdist];
        if (!bTime)
            return UMC_ERR_ALLOC;
    }
    bTimePos = 0;
    gTime = 0.0;
    iTime = 1.0 / vParam->m_info.fFramerate;
    // create default memory allocator if not exist
    Status status = BaseCodec::Init(init);
    if (status != UMC_OK)
        return status;
    status = AllocateBuffers();
    if (status != UMC_OK)
        return status;
    m_Param.m_iSuggestedOutputSize = m_Param.m_info.videoInfo.m_iWidth * m_Param.m_info.videoInfo.m_iHeight;
    m_IsInit = true;
    return UMC_OK;
}

Status MPEG4VideoEncoder::GetInfo(BaseCodecParams* baseParams)
{
    MPEG4EncoderParams* mp4Params = DynamicCast<MPEG4EncoderParams, BaseCodecParams>(baseParams);
    VideoEncoderParams *encParams = DynamicCast<VideoEncoderParams, BaseCodecParams>(baseParams);

    if (!m_IsInit)
        return UMC_ERR_NOT_INITIALIZED;
    m_Param.m_info.streamType = MPEG4_VIDEO;
    m_Param.m_iQuality = 100 - m_Param.m_Param.quantPVOP * 100 / 33;
    if (mp4Params) {
        *mp4Params = m_Param;
    } else if (encParams) {
        *encParams = m_Param;
    } else if (baseParams) {
        *baseParams = m_Param;
    } else
        return UMC_ERR_NULL_PTR;
    return UMC_OK;
}

Status MPEG4VideoEncoder::Close()
{
    if (!m_IsInit)
        return UMC_ERR_NOT_INITIALIZED;
    FreeBuffers();
    // close default memory allocator if exist
    BaseCodec::Close();
    if (m_Param.m_Param.BVOPdist)
        delete [] bTime;
    mp4enc->Close();
    delete mp4enc;
    m_IsInit = false;
    return UMC_OK;
}

Status MPEG4VideoEncoder::GetFrame(MediaData* pIn, MediaData* pOut)
{
    if (!m_IsInit)
        return UMC_ERR_NOT_INITIALIZED;
    VideoData* pVideoDataIn = DynamicCast<VideoData, MediaData> (pIn);
    if (!pOut)
        return UMC_ERR_NULL_PTR;
    mp4enc->InitBuffer((Ipp8u*)pOut->GetDataPointer() + pOut->GetDataSize(), (Ipp32s)((Ipp8u*)pOut->GetBufferPointer() - (Ipp8u*)pOut->GetDataPointer() + pOut->GetBufferSize() - pOut->GetDataSize()));
    if (m_FrameCount == 0)
        mp4enc->EncodeHeader();
    LockBuffers();
    if (pIn && ((m_Param.m_Param.sprite_enable != MP4_SPRITE_STATIC) || (m_FrameCount == 0))) {
        // copy YUV to internal frame
        IppiSize  roi;
        Ipp8u    *pY, *pU, *pV;
        Ipp32s    stepL, stepC;

        mp4enc->GetCurrentFrameInfo(&pY, &pU, &pV, &stepL, &stepC);
        roi.width = m_Param.m_info.videoInfo.m_iWidth;
        roi.height = m_Param.m_info.videoInfo.m_iHeight;
        ippiCopy_8u_C1R((Ipp8u*)pVideoDataIn->GetPlaneDataPtr(0), (int)pVideoDataIn->GetPlanePitch(0), pY, stepL, roi);
        roi.width >>= 1;
        roi.height >>= 1;
        ippiCopy_8u_C1R((Ipp8u*)pVideoDataIn->GetPlaneDataPtr(1), (int)pVideoDataIn->GetPlanePitch(1), pU, stepC, roi);
        ippiCopy_8u_C1R((Ipp8u*)pVideoDataIn->GetPlaneDataPtr(2), (int)pVideoDataIn->GetPlanePitch(2), pV, stepC, roi);
    }
    Ipp32s  sts = mp4enc->EncodeFrame(pIn == NULL);
    Ipp64f  pts = gTime;
    if (pIn) {
        pIn->SetDataSize(0);
        if (pIn->m_fPTSStart >= 0)
            pts = pIn->m_fPTSStart;
    }
    if (sts == MPEG4_ENC::MP4_STS_BUFFERED) {
        //pOut->SetDataSize(0);
        pOut->m_fPTSStart = -1;
        if (pIn) {
            bTime[bTimePos] = pts;
            bTimePos ++;
            if (bTimePos >= m_Param.m_Param.BVOPdist)
                bTimePos = 0;
        }
    } else {
        if (sts != MPEG4_ENC::MP4_STS_NODATA)
            m_FrameCount ++;
        pOut->SetDataSize(mp4enc->GetBufferFullness() + pOut->GetDataSize());
        if (mp4enc->GetFrameType() != MPEG4_ENC::MP4_VOP_TYPE_B) {
            pOut->m_fPTSStart = pts;
        } else {
            pOut->m_fPTSStart = bTime[bTimePos];
            bTime[bTimePos] = pts;
            bTimePos ++;
            if (bTimePos >= m_Param.m_Param.BVOPdist)
                bTimePos = 0;
        }
        switch (mp4enc->GetFrameType()) {
          case MPEG4_ENC::MP4_VOP_TYPE_I:
            pOut->m_frameType = I_PICTURE;
            break;
          case MPEG4_ENC::MP4_VOP_TYPE_P:
            pOut->m_frameType = P_PICTURE;
            break;
          case MPEG4_ENC::MP4_VOP_TYPE_B:
            pOut->m_frameType = B_PICTURE;
            break;
          default:
            pOut->m_frameType = NONE_PICTURE;
            break;
        }
    }
    gTime += iTime;
    UnlockBuffers();
    //return (sts == MPEG4_ENC::MP4_STS_BUFFERED) ? UMC_ERR_NOT_ENOUGH_DATA : (sts == MPEG4_ENC::MP4_STS_NODATA) ? UMC_ERR_END_OF_STREAM : UMC_OK;
    return (sts == MPEG4_ENC::MP4_STS_NODATA) ? UMC_ERR_END_OF_STREAM : UMC_OK;
    //return UMC_OK;
}

Status MPEG4EncoderParams::ReadParams(ParserCfg *par)
{
    DString   sIntraQMatrixFileName, sNonIntraQMatrixFileName;
    vm_file  *InputFile;
    Ipp32s    iSpriteRect[4];
    Ipp32s    iNumFramesToEncode;
    Ipp32s    i, j, k;

    UMC_CHECK(par, UMC_ERR_NULL_PTR);
    if(!par->GetNumOfkeys())
        return UMC_ERR_NOT_INITIALIZED;

    par->GetParam(VM_STRING("iTimeResolution"),          &m_Param.TimeResolution,  1);
    par->GetParam(VM_STRING("iTimeIncrement"),           &m_Param.TimeIncrement,  1);
    par->GetParam(VM_STRING("iQuantType"),               &m_Param.quant_type,  1);
    par->GetParam(VM_STRING("iQuantIVOP"),               &m_Param.quantIVOP,  1);
    par->GetParam(VM_STRING("iQuantPVOP"),               &m_Param.quantPVOP,  1);
    par->GetParam(VM_STRING("iQuantBVOP"),               &m_Param.quantBVOP,  1);
    par->GetParam(VM_STRING("sIntraQMatrixFileName"),    &sIntraQMatrixFileName,  1);
    par->GetParam(VM_STRING("sNonIntraQMatrixFileName"), &sNonIntraQMatrixFileName,  1);
    par->GetParam(VM_STRING("bShortVideoHeader"),        &m_Param.short_video_header,  1);
    par->GetParam(VM_STRING("iIVOPdist"),                &m_Param.IVOPdist,  1);
    par->GetParam(VM_STRING("iBVOPdist"),                &m_Param.BVOPdist,  1);
    par->GetParam(VM_STRING("iPVOPsearchWidth"),         &m_Param.PVOPsearchWidth,  1);
    par->GetParam(VM_STRING("iPVOPsearchHeight"),        &m_Param.PVOPsearchHeight,  1);
    par->GetParam(VM_STRING("iBVOPsearchWidthForw"),     &m_Param.BVOPsearchWidthForw,  1);
    par->GetParam(VM_STRING("iBVOPsearchHeightForw"),    &m_Param.BVOPsearchHeightForw,  1);
    par->GetParam(VM_STRING("iBVOPsearchWidthBack"),     &m_Param.BVOPsearchWidthBack,  1);
    par->GetParam(VM_STRING("iBVOPsearchHeightBack"),    &m_Param.BVOPsearchHeightBack,  1);
    par->GetParam(VM_STRING("iMEAlgorithm"),             &m_Param.MEalgorithm,  1);
    par->GetParam(VM_STRING("iMEAccuracy"),              &m_Param.MEaccuracy,  1);
    par->GetParam(VM_STRING("iME4mv"),                   &m_Param.ME4mv,  1);
    par->GetParam(VM_STRING("bOBMCDisable"),             &m_Param.obmc_disable,  1);
    par->GetParam(VM_STRING("bRoundingControl"),         &m_Param.RoundingControl,  1);
    par->GetParam(VM_STRING("bCalcPSNR"),                &m_Param.calcPSNR,  1);
    par->GetParam(VM_STRING("iRateControl"),             &m_Param.RateControl,  1);
    par->GetParam(VM_STRING("iSceneChangeThreshold"),    &m_Param.SceneChangeThreshold,  1);
    par->GetParam(VM_STRING("bInsertGOV"),               &m_Param.insertGOV,  1);
    par->GetParam(VM_STRING("bRepeatHeaders"),           &m_Param.repeatHeaders,  1);
    par->GetParam(VM_STRING("bResync"),                  &m_Param.resync,  1);
    par->GetParam(VM_STRING("iVideoPacketLenght"),       &m_Param.VideoPacketLenght,  1);
    par->GetParam(VM_STRING("iDataPartitioned"),         &m_Param.data_partitioned,  1);
    par->GetParam(VM_STRING("iReversibleVLC"),           &m_Param.reversible_vlc,  1);
    par->GetParam(VM_STRING("bInterlaced"),              &m_Param.interlaced,  1);
    par->GetParam(VM_STRING("bTopFieldFirst"),           &m_Param.top_field_first,  1);
    par->GetParam(VM_STRING("bAlternateVerticalScan"),   &m_Param.alternate_vertical_scan_flag,  1);
    par->GetParam(VM_STRING("iInterlacedME"),            &m_Param.interlacedME,  1);
    par->GetParam(VM_STRING("iSpriteEnable"),            &m_Param.sprite_enable,  1);

    m_Param.BitRate = m_info.iBitrate;
    if(!m_Param.RateControl)
        m_info.iBitrate = 0;

    if (m_Param.sprite_enable) {
        par->GetParam(VM_STRING("bNoOfSpriteWarping"),       &m_Param.no_of_sprite_warping_points,  1);
        par->GetParam(VM_STRING("bSpriteWarpingAccuracy"),   &m_Param.sprite_warping_accuracy,  1);
    }

    if (m_Param.sprite_enable == IPPVC_SPRITE_STATIC) {
        par->GetParam(VM_STRING("iFramesLimit"),             &iNumFramesToEncode,  1);
        par->GetParam(VM_STRING("iSpriteBrightnessChange"),  &m_Param.sprite_brightness_change,  1);
        par->GetParam(VM_STRING("iSpriteRect"),              &iSpriteRect[0],  4);
        m_Param.sprite_left_coordinate = iSpriteRect[0];
        m_Param.sprite_top_coordinate = iSpriteRect[1];
        m_Param.sprite_width = iSpriteRect[2];
        m_Param.sprite_height = iSpriteRect[3];

        if (iNumFramesToEncode < 1)
            iNumFramesToEncode = 1;
        if (m_Param.no_of_sprite_warping_points < 0 || m_Param.no_of_sprite_warping_points > 3)
            m_Param.no_of_sprite_warping_points = 0;
        /*
        if (m_Param.no_of_sprite_warping_points > 0) {
            m_Param.warping_mv_code_du = new Ipp32s [m_Param.no_of_sprite_warping_points * numFramesToEncode];
            m_Param.warping_mv_code_dv = new Ipp32s [m_Param.no_of_sprite_warping_points * numFramesToEncode];
            m_Param.brightness_change_factor = new Ipp32s [numFramesToEncode];
            for (i = 0; i < (Ipp32s)numFramesToEncode; i ++) {
                for (j = 0; j < m_Param.no_of_sprite_warping_points; j ++) {
                    vm_file_fscanf(InputFile, VM_STRING("%d"), &m_Param.warping_mv_code_du[i*m_Param.no_of_sprite_warping_points+j]);
                    vm_file_fscanf(InputFile, VM_STRING("%d"), &m_Param.warping_mv_code_dv[i*m_Param.no_of_sprite_warping_points+j]);
                }
                if (m_Param.sprite_brightness_change)
                    vm_file_fscanf(InputFile, VM_STRING("%d"), &m_Param.brightness_change_factor[i]);
            }
        }
        */
    }

    // read quant matrix
    if(sIntraQMatrixFileName.Size()) {
        InputFile = vm_file_open((vm_char*)sIntraQMatrixFileName, VM_STRING("rt"));
        if (!InputFile)
            return UMC_ERR_FAILED;
        else {
            m_Param.load_intra_quant_mat = 1;
            for (i = 0; i < 64; i++) {
                k = vm_file_fscanf(InputFile, VM_STRING("%d"), &j);
                if (k <= 0 || j < 1 || j > 255 )
                    break;
                m_Param.intra_quant_mat[i] = (Ipp8u)j;
            }
            m_Param.load_intra_quant_mat_len = i;
            if (m_Param.load_intra_quant_mat_len < 2) {
                m_Param.load_intra_quant_mat = 0;
                m_Param.load_intra_quant_mat_len = 0;
            }
        }
        vm_file_fclose(InputFile);
        m_Param.quant_type = 1;
    }

    if(sNonIntraQMatrixFileName.Size()) {
        InputFile = vm_file_open((vm_char*)sNonIntraQMatrixFileName, VM_STRING("rt"));
        if (!InputFile)
            return UMC_ERR_FAILED;
        else {
            m_Param.load_nonintra_quant_mat = 1;
            for (i = 0; i < 64; i++) {
                k = vm_file_fscanf(InputFile, VM_STRING("%d"), &j);
                if (k <= 0 || j < 1 || j > 255 )
                    break;
                m_Param.nonintra_quant_mat[i] = (Ipp8u)j;
            }
            m_Param.load_nonintra_quant_mat_len = i;
            if (m_Param.load_nonintra_quant_mat_len < 2) {
                m_Param.load_nonintra_quant_mat = 0;
                m_Param.load_nonintra_quant_mat_len = 0;
            }
        }
        vm_file_fclose(InputFile);
        m_Param.quant_type = 1;
    }

    if (m_Param.sprite_enable == IPPVC_SPRITE_STATIC) {
        m_info.videoInfo.m_iWidth = m_Param.sprite_width;
        m_info.videoInfo.m_iHeight = m_Param.sprite_height;
    }

    return UMC_OK;
}

} // namespace UMC

#endif //defined (UMC_ENABLE_MPEG4_VIDEO_ENCODER)
