/* ///////////////////////////////////////////////////////////////////////////// */
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
#ifdef UMC_ENABLE_MPEG4_VIDEO_DECODER

#include <string.h>

#include "vm_debug.h"
#include "umc_mpeg4_video_decoder.h"
#include "umc_video_data.h"
#include "umc_video_processing.h"

using namespace UMC;


Status MPEG4VideoDecoder::AllocateInitFrame(mp4_Frame* pFrame)
{
    Status  status;
    Ipp32s  w, h;
    Ipp8u  *p;

    w = (pFrame->mbPerRow + 2 * MP4_NUM_EXT_MB) << 4;
    h = (pFrame->mbPerCol + 2 * MP4_NUM_EXT_MB) << 4;
    status = m_pMemoryAllocator->Alloc(&pFrame->mid, w * h + (w * h >> 1), UMC_ALLOC_PERSISTENT);
    if (status != UMC_OK)
        return status;
    p = (Ipp8u*)m_pMemoryAllocator->Lock(pFrame->mid);
    ippsSet_8u(0, p, w * h);
    ippsSet_8u(128, p + w * h, w * h >> 1);
    status = m_pMemoryAllocator->Unlock(pFrame->mid);
    if (status != UMC_OK)
        return status;
    pFrame->stepY = w;
    pFrame->stepCb = w >> 1;
    pFrame->stepCr = w >> 1;
    return UMC_OK;
}

void MPEG4VideoDecoder::LockFrame(mp4_Frame* pFrame)
{
    Ipp32u  w, h;

    w = (pFrame->mbPerRow + 2 * MP4_NUM_EXT_MB) << 4;
    h = (pFrame->mbPerCol + 2 * MP4_NUM_EXT_MB) << 4;
    pFrame->apY = (Ipp8u*)m_pMemoryAllocator->Lock(pFrame->mid);
    pFrame->pY = pFrame->apY + w * 16 + 16;
    pFrame->apCb = pFrame->apY + w * h;
    w >>= 1;
    h >>= 1;
    pFrame->pCb = pFrame->apCb + w * 8 + 8;
    pFrame->apCr = pFrame->apCb + w * h;
    pFrame->pCr = pFrame->apCr + w * 8 + 8;
}

Status MPEG4VideoDecoder::AllocateBuffers()
{
    Status  status = UMC_OK;

    // allocate only frame memory, may be extended for whole buffers
    if (m_decInfo->VisualObject.VideoObject.shape == MP4_SHAPE_TYPE_RECTANGULAR) {
        // current frame
        m_decInfo->VisualObject.cFrame.mbPerRow = (m_decInfo->VisualObject.VideoObject.width + 15) >> 4;
        m_decInfo->VisualObject.cFrame.mbPerCol = (m_decInfo->VisualObject.VideoObject.height + 15) >> 4;
        status = AllocateInitFrame(&m_decInfo->VisualObject.cFrame);
        if (status != UMC_OK)
            return status;
        if (m_decInfo->VisualObject.VideoObject.sprite_enable != MP4_SPRITE_STATIC) {
            // ref in past frame
            if (!m_decInfo->noPVOPs) {
                m_decInfo->VisualObject.rFrame.mbPerRow = m_decInfo->VisualObject.cFrame.mbPerRow;
                m_decInfo->VisualObject.rFrame.mbPerCol = m_decInfo->VisualObject.cFrame.mbPerCol;
                status = AllocateInitFrame(&m_decInfo->VisualObject.rFrame);
                if (status != UMC_OK)
                    return status;
            }
            // ref in future frame
            if (!m_decInfo->noBVOPs) {
                m_decInfo->VisualObject.nFrame.mbPerRow = m_decInfo->VisualObject.cFrame.mbPerRow;
                m_decInfo->VisualObject.nFrame.mbPerCol = m_decInfo->VisualObject.cFrame.mbPerCol;
                status = AllocateInitFrame(&m_decInfo->VisualObject.nFrame);
                if (status != UMC_OK)
                    return status;
            }
        } else {
            // data for static sprite
            m_decInfo->VisualObject.sFrame.mbPerRow = (m_decInfo->VisualObject.VideoObject.sprite_width + 15) >> 4;
            m_decInfo->VisualObject.sFrame.mbPerCol = (m_decInfo->VisualObject.VideoObject.sprite_height + 15) >> 4;
            status = AllocateInitFrame(&m_decInfo->VisualObject.sFrame);
            if (status != UMC_OK)
                return status;
        }
    }
    if (mp4_InitVOL(m_decInfo) != MP4_STATUS_OK) {
        return UMC_ERR_ALLOC;
    }
    return status;
}

Status MPEG4VideoDecoder::FreeBuffers()
{
    Status  status = UMC_OK;

    if (m_decInfo->VisualObject.VideoObject.shape == MP4_SHAPE_TYPE_RECTANGULAR) {
        if (m_decInfo->VisualObject.cFrame.mid) {
            status = m_pMemoryAllocator->Free(m_decInfo->VisualObject.cFrame.mid);
            m_decInfo->VisualObject.cFrame.mid = 0;
            if (status != UMC_OK)
                return status;
        }
        if (m_decInfo->VisualObject.VideoObject.sprite_enable != MP4_SPRITE_STATIC) {
            if (m_decInfo->VisualObject.rFrame.mid) {
                if (!m_decInfo->noPVOPs)
                    status = m_pMemoryAllocator->Free(m_decInfo->VisualObject.rFrame.mid);
                m_decInfo->VisualObject.rFrame.mid = 0;
                if (status != UMC_OK)
                    return status;
            }
            if (m_decInfo->VisualObject.nFrame.mid) {
                if (!m_decInfo->noBVOPs)
                    status = m_pMemoryAllocator->Free(m_decInfo->VisualObject.nFrame.mid);
                m_decInfo->VisualObject.nFrame.mid = 0;
                if (status != UMC_OK)
                    return status;
            }
        } else {
            if (m_decInfo->VisualObject.sFrame.mid) {
                status = m_pMemoryAllocator->Free(m_decInfo->VisualObject.sFrame.mid);
                m_decInfo->VisualObject.sFrame.mid = 0;
                if (status != UMC_OK)
                    return status;
            }
        }
    }
    mp4_FreeVOL(m_decInfo);
    return status;
}

void MPEG4VideoDecoder::LockBuffers()
{
    if (m_decInfo->VisualObject.VideoObject.shape == MP4_SHAPE_TYPE_RECTANGULAR) {
        LockFrame(&m_decInfo->VisualObject.cFrame);
        if (m_decInfo->VisualObject.VideoObject.sprite_enable != MP4_SPRITE_STATIC) {
            LockFrame(&m_decInfo->VisualObject.rFrame);
            LockFrame(&m_decInfo->VisualObject.nFrame);
        } else {
            LockFrame(&m_decInfo->VisualObject.sFrame);
        }
    }
}

Status MPEG4VideoDecoder::UnlockBuffers()
{
    Status status = UMC_OK;

    if (m_decInfo->VisualObject.VideoObject.shape == MP4_SHAPE_TYPE_RECTANGULAR) {
        status = m_pMemoryAllocator->Unlock(m_decInfo->VisualObject.cFrame.mid);
        if (status != UMC_OK)
            return status;
        if (m_decInfo->VisualObject.VideoObject.sprite_enable != MP4_SPRITE_STATIC) {
            status = m_pMemoryAllocator->Unlock(m_decInfo->VisualObject.rFrame.mid);
            if (status != UMC_OK)
                return status;
            status = m_pMemoryAllocator->Unlock(m_decInfo->VisualObject.nFrame.mid);
            if (status != UMC_OK)
                return status;
        } else {
            status = m_pMemoryAllocator->Unlock(m_decInfo->VisualObject.sFrame.mid);
            if (status != UMC_OK)
                return status;
        }
    }
    return status;
}

Status MPEG4VideoDecoder::Init(BaseCodecParams *lpInit)
{
    Status  status = UMC_OK;
    VideoDecoderParams *pParam = DynamicCast<VideoDecoderParams, BaseCodecParams> (lpInit);

    Close();
    if (NULL == m_decInfo)
        return UMC_ERR_ALLOC;
    if (NULL == pParam)
        return UMC_ERR_NULL_PTR;
    if (NULL == pParam->m_pPostProcessor)
        return UMC_ERR_NULL_PTR;

    try
    {
        // create default memory allocator if not exist
        status = BaseCodec::Init(lpInit);
        if (status != UMC_OK)
            return status;
        m_time_reset = 0;
        m_dec_time_fr = 0.0;
        m_dec_time_base = -1.0;
        m_dec_time_prev = 0.0;
        m_dec_time_frinc = (pParam->m_info.fFramerate > 0) ? 1.0 / pParam->m_info.fFramerate : 0.0;
        m_is_skipped_b = m_skipped_fr = m_b_prev = 0;
        m_Param = *pParam;
        m_IsInit = m_IsInitBase = false;
        memset(m_decInfo, 0, sizeof(mp4_Info));
        m_decInfo->stopOnErr = 0;
        m_decInfo->strictSyntaxCheck = 0;
        m_decInfo->VisualObject.verid = 1;
        m_DeringingProcPlane[0] = m_DeringingProcPlane[1] = m_DeringingProcPlane[2] = false;
        m_DeblockingProcPlane[0] = m_DeblockingProcPlane[1] = m_DeblockingProcPlane[2] = false;
        m_DeblockingTHR1 = 2;
        m_DeblockingTHR2 = 6;
        ppFrame0.mid = ppFrame1.mid = 0;
        if (NULL != pParam->m_pData) {
            if (pParam->m_pData->m_fPTSStart >= 0.0)
                m_dec_time_base = pParam->m_pData->m_fPTSStart;
            m_decInfo->buflen = pParam->m_pData->GetDataSize();
            if (m_decInfo->buflen > 0) {
                m_decInfo->bufptr = m_decInfo->buffer = reinterpret_cast<Ipp8u *> (pParam->m_pData->GetDataPointer());
                m_decInfo->bitoff = 0;
                status = InsideInit();
                //pParam->m_pData->MoveDataPointer(((size_t)m_decInfo->bufptr - (size_t)m_decInfo->buffer));
                size_t stDecidedData;
                if ((size_t)m_decInfo->bufptr - (size_t)m_decInfo->buffer < m_decInfo->buflen)
                    stDecidedData = m_decInfo->buflen - ((size_t)m_decInfo->bufptr - (size_t)m_decInfo->buffer);
                else
                    stDecidedData = 0;
                pParam->m_pData->MoveDataPointer(pParam->m_pData->GetDataSize() - stDecidedData);
            }
        }
        if (UMC_OK == status) {
            m_pPostProcessor = pParam->m_pPostProcessor;
            // set VOP size if it is unknown in splitter
            if (pParam->m_info.videoInfo.m_iWidth == 0 || pParam->m_info.videoInfo.m_iHeight == 0) {
                if (m_decInfo->VisualObject.VideoObject.width == 0 && m_decInfo->VisualObject.VideoObject.height== 0)
                    return UMC_ERR_INIT;
                else
                {
                    pParam->m_info.videoInfo.m_iWidth  = m_Param.m_info.videoInfo.m_iWidth  = m_decInfo->VisualObject.VideoObject.width;
                    pParam->m_info.videoInfo.m_iHeight = m_Param.m_info.videoInfo.m_iHeight = m_decInfo->VisualObject.VideoObject.height;
                }
            }
            m_ClipInfo = m_Param.m_info;
            m_IsInitBase = true;
        }
    }
    catch(...)
    {
        return UMC_ERR_INVALID_STREAM;
    }

    return status;
}

Status MPEG4VideoDecoder::InsideInit()
{
    Status  status;
    Ipp32u  code, h_vo_found = 0, h_vos_found = 0;

    m_buffered_frame = m_Param.m_iFlags & FLAG_VDEC_REORDER;
    if (m_Param.m_info.streamSubtype == MPEG4_VIDEO_DIVX5)
    {
        m_decInfo->ftype = 2;
        m_decInfo->ftype_f = 1;
    }
    else if (m_Param.m_info.streamSubtype == MPEG4_VIDEO_QTIME)
    {
        m_decInfo->ftype = 1;
        m_decInfo->ftype_f = 0;
    } else { // UNDEF_VIDEO_SUBTYPE
        m_decInfo->ftype = 0;
        m_decInfo->ftype_f = 0;
    }
    m_decInfo->num_threads = m_Param.m_iThreads;
    for (;;) {
        if (m_decInfo->strictSyntaxCheck) {
            if (!mp4_SeekStartCodeOrShortPtr(m_decInfo)) {
                return UMC_ERR_SYNC;
            }
            // check short_video_start_marker
            if (mp4_IsShortCode(m_decInfo)) {
                if (mp4_Parse_VideoObject(m_decInfo) != MP4_STATUS_OK)
                    return UMC_ERR_INVALID_STREAM;
            } else {
                if (mp4_IsStartCodeValue(m_decInfo, MP4_VISUAL_OBJECT_SEQUENCE_SC, MP4_VISUAL_OBJECT_SEQUENCE_SC)) {
                    m_decInfo->bufptr ++;
                    if (mp4_Parse_VisualObjectSequence(m_decInfo) != MP4_STATUS_OK)
                        return UMC_ERR_INVALID_STREAM;
                } else {
                    return UMC_ERR_INVALID_STREAM;
                }
                if (!mp4_SeekStartCodePtr(m_decInfo)) {
                    return UMC_ERR_INVALID_STREAM;
                }
                if (mp4_IsStartCodeValue(m_decInfo, MP4_VISUAL_OBJECT_SC, MP4_VISUAL_OBJECT_SC)) {
                    m_decInfo->bufptr ++;
                    if (mp4_Parse_VisualObject(m_decInfo) != MP4_STATUS_OK)
                        return UMC_ERR_INVALID_STREAM;
                } else {
                    return UMC_ERR_INVALID_STREAM;
                }
                if (!mp4_SeekStartCodePtr(m_decInfo)) {
                    return UMC_ERR_INVALID_STREAM;
                }
                if (mp4_IsStartCodeValue(m_decInfo, MP4_VIDEO_OBJECT_MIN_SC,  MP4_VIDEO_OBJECT_MAX_SC)) {
                    m_decInfo->bufptr ++;
                    if (mp4_Parse_VideoObject(m_decInfo) != MP4_STATUS_OK)
                        return UMC_ERR_INVALID_STREAM;
                } else {
                    return UMC_ERR_INVALID_STREAM;
                }
            }
        } else {
            if (!mp4_SeekStartCodeOrShortPtr(m_decInfo)) {
                return UMC_ERR_SYNC;
            }
            // check short_video_start_marker
            if (mp4_IsShortCode(m_decInfo)) {
              if ((mp4_Parse_VideoObject(m_decInfo)) != MP4_STATUS_OK)
                return UMC_ERR_INVALID_STREAM;
              break;
            }
            code = mp4_GetBits(m_decInfo, 8);
            if (!h_vos_found && code == MP4_VISUAL_OBJECT_SEQUENCE_SC) {
                h_vos_found = 1;
                if ((mp4_Parse_VisualObjectSequence(m_decInfo)) != MP4_STATUS_OK)
                    return UMC_ERR_INVALID_STREAM;
            }
            if (!h_vo_found && code == MP4_VISUAL_OBJECT_SC) {
                h_vo_found = 1;
                if ((mp4_Parse_VisualObject(m_decInfo)) != MP4_STATUS_OK)
                    return UMC_ERR_INVALID_STREAM;
            }
            // some streams can start with video_object_layer
            if ((Ipp32s)code >= MP4_VIDEO_OBJECT_LAYER_MIN_SC && code <= MP4_VIDEO_OBJECT_LAYER_MAX_SC) {
                m_decInfo->bufptr -= 4;
                if ((mp4_Parse_VideoObject(m_decInfo)) != MP4_STATUS_OK)
                    return UMC_ERR_INVALID_STREAM;
                break;
            }
        }
    }

    status = AllocateBuffers();
    if (status != UMC_OK)
        return status;

    // set aspect ratio info
    switch (m_decInfo->VisualObject.VideoObject.aspect_ratio_info)
    {
        case MP4_ASPECT_RATIO_FORBIDDEN:
        case MP4_ASPECT_RATIO_1_1:
            m_Param.m_info.videoInfo.m_iSAWidth  = 1;
            m_Param.m_info.videoInfo.m_iSAHeight = 1;
            break;

        case MP4_ASPECT_RATIO_12_11:
            m_Param.m_info.videoInfo.m_iSAWidth  = 12;
            m_Param.m_info.videoInfo.m_iSAHeight = 11;
            break;

        case MP4_ASPECT_RATIO_10_11:
            m_Param.m_info.videoInfo.m_iSAWidth  = 10;
            m_Param.m_info.videoInfo.m_iSAHeight = 11;
            break;

        case MP4_ASPECT_RATIO_16_11:
            m_Param.m_info.videoInfo.m_iSAWidth  = 16;
            m_Param.m_info.videoInfo.m_iSAHeight = 11;
            break;

        case MP4_ASPECT_RATIO_40_33:
            m_Param.m_info.videoInfo.m_iSAWidth  = 40;
            m_Param.m_info.videoInfo.m_iSAHeight = 33;
            break;

        default:
            m_Param.m_info.videoInfo.m_iSAWidth  = m_decInfo->VisualObject.VideoObject.aspect_ratio_info_par_width;
            m_Param.m_info.videoInfo.m_iSAHeight = m_decInfo->VisualObject.VideoObject.aspect_ratio_info_par_height;
    }

    // set profile/level info
    m_Param.m_info.iProfile = m_decInfo->profile_and_level_indication >> 4;
    m_Param.m_info.iLevel = m_decInfo->profile_and_level_indication & 15;
    if (m_Param.m_info.iProfile == MPEG4_PROFILE_SIMPLE)
        if (m_Param.m_info.iLevel == 8)
            m_Param.m_info.iLevel = MPEG4_LEVEL_0;

    if (m_Param.m_info.iProfile == MPEG4_PROFILE_ADVANCED_SIMPLE)
    {
        if (m_Param.m_info.iLevel == 7)
            m_Param.m_info.iLevel = MPEG4_LEVEL_3B;
        if (m_Param.m_info.iLevel > 7) {
            m_Param.m_info.iProfile = MPEG4_PROFILE_FGS;
            m_Param.m_info.iLevel -= 8;
        }
    }
    m_IsInit = true;
    return UMC_OK;
}

Status MPEG4VideoDecoder::GetFrame(MediaData* in, MediaData* out)
{
    Status  status = UMC_OK;
    Ipp64f  pts = -1.0;

#ifdef ENABLE_OPENMP
    m_decInfo->mTreadPriority = vm_get_current_thread_priority();
#endif

    if (!out)
        return UMC_ERR_NULL_PTR;
    out->m_iInvalid = 1;
    if (!m_IsInitBase)
        return UMC_ERR_NOT_INITIALIZED;
    if (in) {
        m_decInfo->bitoff = 0;
        m_decInfo->bufptr = m_decInfo->buffer = (Ipp8u *)in->GetDataPointer();
        m_decInfo->buflen = in->GetDataSize();
    }
    if (!m_IsInit) {
        if (in == NULL)
            return UMC_ERR_NOT_ENOUGH_DATA;
        m_Param.m_pData = in;
        status = InsideInit();
        if (status != UMC_OK) {
            in->MoveDataPointer((Ipp32s)(in->GetDataSize() - m_decInfo->buflen));
            if (status == UMC_ERR_INVALID_STREAM || status == UMC_ERR_SYNC)
                return UMC_ERR_NOT_ENOUGH_DATA;
            else
                return status;
        }
    }
    LockBuffers();
    try {
    for (;;) {
        if (in == NULL) {
            // show last frame (it can be only if (m_Param.lFlags & FLAG_VDEC_REORDER))
            if (!m_buffered_frame) {
                UnlockBuffers();
                return UMC_ERR_NOT_ENOUGH_DATA;
            }
            m_buffered_frame = false;
            m_decInfo->VisualObject.vFrame = m_decInfo->VisualObject.VideoObject.prevPlaneIsB ?
                                            &m_decInfo->VisualObject.nFrame : &m_decInfo->VisualObject.cFrame;
        } else {
            // Seeking the VOP start_code, and then begin the vop decoding
            if (m_decInfo->VisualObject.VideoObject.short_video_header) {
                if (!mp4_SeekShortVideoStartMarker(m_decInfo)) {
                    status = UMC_ERR_SYNC;
                    break;
                }
            } else {
                for (;;) {
                    if (!mp4_SeekStartCodePtr(m_decInfo)) {
                        status = UMC_ERR_SYNC;
                        break;
                    }
                    int code = m_decInfo->bufptr[0];
                    m_decInfo->bufptr ++;
                    // parse repeated VOS, VO and VOL headers because stream may be glued from different streams
                    if (code == MP4_VISUAL_OBJECT_SEQUENCE_SC) {
                        if (mp4_Parse_VisualObjectSequence(m_decInfo) != MP4_STATUS_OK) {
                            status = UMC_ERR_INVALID_STREAM;
                            break;
                        }
                    } else if (code == MP4_VISUAL_OBJECT_SC) {
                        if (mp4_Parse_VisualObject(m_decInfo) != MP4_STATUS_OK) {
                            status = UMC_ERR_INVALID_STREAM;
                            break;
                        }
                    } else if (code >= MP4_VIDEO_OBJECT_LAYER_MIN_SC && code <= MP4_VIDEO_OBJECT_LAYER_MAX_SC) {
                        // in repeated headers check only VOL header
                        m_decInfo->bufptr -= 4;
                        // save parameters which can affect on reinit
                        Ipp32s interlaced = m_decInfo->VisualObject.VideoObject.interlaced;
                        Ipp32s data_partitioned = m_decInfo->VisualObject.VideoObject.data_partitioned;
                        Ipp32s sprite_enable = m_decInfo->VisualObject.VideoObject.sprite_enable;
                        Ipp32s width = m_decInfo->VisualObject.VideoObject.width;
                        Ipp32s height = m_decInfo->VisualObject.VideoObject.height;
                        if (mp4_Parse_VideoObject(m_decInfo) != MP4_STATUS_OK) {
                            status = UMC_ERR_INVALID_STREAM;
                            break;
                        }
                        // realloc if something was changed
                        if (interlaced != m_decInfo->VisualObject.VideoObject.interlaced ||
                            data_partitioned != m_decInfo->VisualObject.VideoObject.data_partitioned ||
                            sprite_enable != m_decInfo->VisualObject.VideoObject.sprite_enable ||
                            width != m_decInfo->VisualObject.VideoObject.width ||
                            height != m_decInfo->VisualObject.VideoObject.height)
                        {
                            if (m_decInfo->strictSyntaxCheck)
                            {
                                status = UMC_ERR_INVALID_STREAM;
                                break;
                            }

                            if (width != m_decInfo->VisualObject.VideoObject.width ||
                                height != m_decInfo->VisualObject.VideoObject.height)
                            {
                                m_Param.m_info.videoInfo.m_iWidth = m_decInfo->VisualObject.VideoObject.width;
                                m_Param.m_info.videoInfo.m_iHeight = m_decInfo->VisualObject.VideoObject.height;
                            }

                            UnlockBuffers();
                            status = FreeBuffers();
                            if (status != UMC_OK)
                                break;

                            status = AllocateBuffers();
                            if (status != UMC_OK)
                                break;

                            LockBuffers();

                            // free buffers for MPEG-4 post-processing
                            if (ppFrame0.mid)
                            {
                                status = m_pMemoryAllocator->Free(ppFrame0.mid);
                                ppFrame0.mid = 0;
                                if (status != UMC_OK)
                                    break;
                            }

                            if (ppFrame1.mid)
                            {
                                status = m_pMemoryAllocator->Free(ppFrame1.mid);
                                ppFrame1.mid = 0;
                                if (status != UMC_OK)
                                    break;
                            }
                        }
                        // reinit quant matrix
                        ippiQuantInvIntraInit_MPEG4(m_decInfo->VisualObject.VideoObject.quant_type ? m_decInfo->VisualObject.VideoObject.intra_quant_mat : NULL, m_decInfo->VisualObject.VideoObject.QuantInvIntraSpec, 8);
                        ippiQuantInvInterInit_MPEG4(m_decInfo->VisualObject.VideoObject.quant_type ? m_decInfo->VisualObject.VideoObject.nonintra_quant_mat : NULL, m_decInfo->VisualObject.VideoObject.QuantInvInterSpec, 8);
                    }
                    else if (code == MP4_GROUP_OF_VOP_SC)
                    {
                        if (mp4_Parse_GroupOfVideoObjectPlane(m_decInfo) != MP4_STATUS_OK)
                        {
                            status = UMC_ERR_INVALID_STREAM;
                            break;
                        }
                    }
                    else if (m_decInfo->bufptr[-1] == MP4_VIDEO_OBJECT_PLANE_SC)
                    {
                        break;
                    }
                }
                if (status != UMC_OK)
                    break;
            }

            // parse VOP header
            if ((mp4_Parse_VideoObjectPlane(m_decInfo)) != MP4_STATUS_OK)
            {
                //status = UMC_WRN_INVALID_STREAM;
                status = UMC_OK;
                break;
            }

            if (m_IsReset && m_decInfo->VisualObject.VideoObject.VideoObjectPlane.coding_type != MP4_VOP_TYPE_I)
            {
                //UnlockBuffers();
                //return UMC_ERR_NOT_ENOUGH_DATA;
                m_decInfo->VisualObject.vFrame = NULL;
                status = UMC_ERR_NOT_ENOUGH_DATA;
                break;
            }

            if (m_decInfo->VisualObject.VideoObject.VideoObjectPlane.coding_type == MP4_VOP_TYPE_B)
            {
                if (0 < m_is_skipped_b && !m_b_prev)
                {
                    m_is_skipped_b --;
                    m_skipped_fr ++;
                    m_b_prev = 1;
                    m_decInfo->bufptr = m_decInfo->buffer+ m_decInfo->buflen;
                    status = UMC_ERR_NOT_ENOUGH_DATA;
                    break;
                } else
                    m_b_prev = 0;
            }

            // decode VOP
            if ((mp4_DecodeVideoObjectPlane(m_decInfo)) != MP4_STATUS_OK)
            {
                status = UMC_WRN_INVALID_STREAM;
            }

            // after reset it is need to skip first B-frames
            if (m_decInfo->VisualObject.VideoObject.VOPindex < 2 && m_decInfo->VisualObject.VideoObject.VideoObjectPlane.coding_type == MP4_VOP_TYPE_B)
            {
                status = UMC_ERR_NOT_ENOUGH_DATA;
                break;
            }

            // do not count not_coded P frames with same vop_time as reference (in AVI)

            if (m_decInfo->VisualObject.VideoObject.VideoObjectPlane.coded ||
                (m_decInfo->VisualObject.rFrame.time != m_decInfo->VisualObject.cFrame.time &&
                 m_decInfo->VisualObject.nFrame.time != m_decInfo->VisualObject.cFrame.time))
            {
                    m_decInfo->VisualObject.VideoObject.VOPindex++;
            }
            else
            {
                status = UMC_ERR_NOT_ENOUGH_DATA;
                break;
            }
            if (m_IsReset && m_decInfo->VisualObject.VideoObject.VideoObjectPlane.coding_type == MP4_VOP_TYPE_I)
            {
                m_time_reset = (Ipp32s)m_decInfo->VisualObject.cFrame.time;
                m_decInfo->VisualObject.vFrame = NULL;
                m_IsReset = false;
            }
            if ((m_Param.m_iFlags & FLAG_VDEC_REORDER) && (m_decInfo->VisualObject.vFrame == NULL))
            {
                // buffer first frame in VDEC_REORDER mode
                status = UMC_ERR_NOT_ENOUGH_DATA;
                break;
            }
            if (!(m_Param.m_iFlags & FLAG_VDEC_REORDER))
                m_decInfo->VisualObject.vFrame = &m_decInfo->VisualObject.cFrame;
        }
        break;
    }
    } catch(...) {
        status = UMC_ERR_INVALID_STREAM;
    }
    if (in)
        if (in->m_fPTSStart != -1.0 && m_dec_time_base == -1.0)
            m_dec_time_base = in->m_fPTSStart;
    if ((m_dec_time_frinc > 0.0) || (m_decInfo->ftype == 1)) {
        if (m_Param.m_iFlags & FLAG_VDEC_REORDER)
            pts = m_dec_time_prev;
        if (in) {
            if (in->m_fPTSStart != -1.0) {
                // take right PTS for I-, P- frames
                m_dec_time_prev = in->m_fPTSStart;
            } else {
                // when PB...  are in one AVI chunk, first PTS from in->GetTime() is right and second is -1.0
                m_dec_time_prev += m_dec_time_frinc;
            }
        }
        if (!(m_Param.m_iFlags & FLAG_VDEC_REORDER))
            pts = m_dec_time_prev;
    }
    else
    {
        bool  extPTS = false;
        if (in)
            if (in->m_fPTSStart != -1.0)
                extPTS = true;
        if (extPTS)
        {
            pts = in->m_fPTSStart;
        }
        else
        {
            {
                if (m_decInfo->VisualObject.vFrame)
                {
                    pts = (Ipp64f)(m_decInfo->VisualObject.vFrame->time - m_time_reset) / m_decInfo->VisualObject.VideoObject.vop_time_increment_resolution;
                }
                else
                {
                    pts = 0.0;
                }
            }

            if (m_dec_time_base != -1.0)
                pts += m_dec_time_base;
        }
    }


    if ((UMC_OK == status || UMC_WRN_INVALID_STREAM == status) && (m_decInfo->VisualObject.vFrame != NULL)) {
        try {
        FrameType ft;
        if (m_decInfo->VisualObject.VideoObject.sprite_enable == MP4_SPRITE_STATIC)
            ft = I_PICTURE;
        else
            ft = (m_decInfo->VisualObject.vFrame->type) == MP4_VOP_TYPE_I ? I_PICTURE :
                 (m_decInfo->VisualObject.vFrame->type) == MP4_VOP_TYPE_B ? B_PICTURE :
                                                                            P_PICTURE;
        mp4_Frame rendFrame = *m_decInfo->VisualObject.vFrame;
        bool     ppDeblocking = m_DeblockingProcPlane[0] || m_DeblockingProcPlane[1] || m_DeblockingProcPlane[2];
        bool     ppDeringing = m_DeringingProcPlane[0] || m_DeringingProcPlane[1] || m_DeringingProcPlane[2];

        if (ppDeblocking || ppDeringing) {
            if (!ppFrame0.mid) {
                ppFrame0.mbPerRow = m_decInfo->VisualObject.vFrame->mbPerRow;
                ppFrame0.mbPerCol = m_decInfo->VisualObject.vFrame->mbPerCol;
                status = AllocateInitFrame(&ppFrame0);
                if (status != UMC_OK)
                    return status;
            }
            if (!ppFrame1.mid) {
                ppFrame1.mbPerRow = m_decInfo->VisualObject.vFrame->mbPerRow;
                ppFrame1.mbPerCol = m_decInfo->VisualObject.vFrame->mbPerCol;
                status = AllocateInitFrame(&ppFrame1);
                if (status != UMC_OK)
                    return status;
            }
            if (ppFrame0.mid)
                LockFrame(&ppFrame0);
            if (ppFrame1.mid)
                LockFrame(&ppFrame1);
            PostProcess(&rendFrame);
        }
        //if (m_LastDecodedFrame.m_colorFormat != YUV420) {
            m_lastDecodedFrame.Init(m_decInfo->VisualObject.VideoObject.width, m_decInfo->VisualObject.VideoObject.height, YUV420);
        //}
        m_lastDecodedFrame.m_frameType = ft;
        m_lastDecodedFrame.m_fPTSStart = pts;
        m_lastDecodedFrame.SetPlaneDataPtr(rendFrame.pY, 0);
        m_lastDecodedFrame.SetPlaneDataPtr(rendFrame.pCb, 1);
        m_lastDecodedFrame.SetPlaneDataPtr(rendFrame.pCr, 2);
        m_lastDecodedFrame.SetPlanePitch(rendFrame.stepY, 0);
        m_lastDecodedFrame.SetPlanePitch(rendFrame.stepCb, 1);
        m_lastDecodedFrame.SetPlanePitch(rendFrame.stepCr, 2);
        if(m_Param.m_info.videoInfo.m_picStructure == PS_PROGRESSIVE)
            m_lastDecodedFrame.m_picStructure = PS_PROGRESSIVE;
        else if(m_Param.m_info.videoInfo.m_picStructure == PS_BOTTOM_FIELD_FIRST)
            m_lastDecodedFrame.m_picStructure = PS_BOTTOM_FIELD_FIRST;
        else if(m_Param.m_info.videoInfo.m_picStructure == PS_TOP_FIELD_FIRST)
            m_lastDecodedFrame.m_picStructure = PS_TOP_FIELD_FIRST;

        m_pPostProcessor->GetFrame(&m_lastDecodedFrame, out);
        if (ppDeblocking || ppDeringing) {
            if (ppFrame0.mid)
                status = m_pMemoryAllocator->Unlock(ppFrame0.mid);
            if (ppFrame1.mid)
                status = m_pMemoryAllocator->Unlock(ppFrame1.mid);
        }
        out->m_iInvalid = 0;
        } catch(...) {
            status = UMC_ERR_INVALID_STREAM;
        }
    }
    if (in) {
        size_t stDecidedData;
        if ((size_t)m_decInfo->bufptr - (size_t)m_decInfo->buffer < m_decInfo->buflen)
            stDecidedData = m_decInfo->buflen - ((size_t)m_decInfo->bufptr - (size_t)m_decInfo->buffer);
        else
            stDecidedData = 0;
        in->MoveDataPointer(in->GetDataSize() - stDecidedData);
        // can't calculate time for the next frame
        in->m_fPTSStart = -1;
    }
    // set interlaced info
    if (!m_decInfo->VisualObject.VideoObject.interlaced)
        m_Param.m_info.videoInfo.m_picStructure = PS_PROGRESSIVE;
    else
        m_Param.m_info.videoInfo.m_picStructure = ((m_decInfo->VisualObject.VideoObject.VideoObjectPlane.top_field_first) ? PS_TOP_FIELD_FIRST : PS_BOTTOM_FIELD_FIRST);
    UnlockBuffers();
    return status;
}

Status MPEG4VideoDecoder::Close(void)
{
    m_IsInitBase = false;
    if (m_IsInit)
    {
        m_IsInit = false;
        try {
            if (ppFrame0.mid)
            {
                m_pMemoryAllocator->Free(ppFrame0.mid);
                ppFrame0.mid = 0;
            }

            if (ppFrame1.mid)
            {
                m_pMemoryAllocator->Free(ppFrame1.mid);
                ppFrame1.mid = 0;
            }

            FreeBuffers();
            // close default memory allocator if exist
            BaseCodec::Close();
        } catch(...) {
        }
    }
    return UMC_OK;
}

MPEG4VideoDecoder::MPEG4VideoDecoder(void)
{
    m_IsInit = m_IsInitBase = false;
    m_decInfo = new mp4_Info;
    m_IsReset = false;
}

MPEG4VideoDecoder::~MPEG4VideoDecoder(void)
{
    Close();
    delete m_decInfo;
}

Status  MPEG4VideoDecoder::ResetSkipCount()
{
    if (!m_IsInitBase)
        return UMC_ERR_NOT_INITIALIZED;
    m_is_skipped_b = m_skipped_fr = m_b_prev = 0;
    return UMC_OK;
}

Status  MPEG4VideoDecoder::SkipVideoFrame(Ipp32s count)
{
    if (!m_IsInitBase)
        return UMC_ERR_NOT_INITIALIZED;
    if (count < 0) {
        m_is_skipped_b = 0;
        return UMC_OK;
    }
    m_is_skipped_b += count;
    return UMC_OK;
}

Ipp32u MPEG4VideoDecoder::GetNumOfSkippedFrames(void)
{
    return m_skipped_fr;
}

Status MPEG4VideoDecoder::Reset(void)
{
    if (!m_IsInitBase)
        return UMC_ERR_NOT_INITIALIZED;
    if (m_IsInit) {
        mp4_ResetVOL(m_decInfo);
        m_IsReset = true;
        m_dec_time_base = -1.0;
        m_dec_time_prev = 0.0;
    }
    return UMC_OK;
}

Status MPEG4VideoDecoder::GetPerformance(Ipp64f *perf)
{
    return UMC_ERR_NOT_IMPLEMENTED;
}

//mp4_Frame* MPEG4VideoDecoder::GetCurrentFramePtr(void)
//{
//  return m_decInfo->VisualObject.vFrame;
//}

Status MPEG4VideoDecoder::GetInfo(BaseCodecParams* info)
{
    VideoDecoderParams *pParams;
    if (!m_IsInitBase)
        return UMC_ERR_NOT_INITIALIZED;
    if (info == NULL)
        return UMC_ERR_NULL_PTR;
    pParams = DynamicCast<VideoDecoderParams, BaseCodecParams> (info);
    if (NULL != pParams) {
        *pParams = m_Param;
    } else {
        if (!m_IsInit)
            return UMC_ERR_NOT_INITIALIZED;
    }
    return UMC_OK;
}

Status MPEG4VideoDecoder::SetDeblockingParams(bool procPlane0, bool procPlane1, bool procPlane2, Ipp32s THR1, Ipp32s THR2)
{
    m_DeblockingProcPlane[0] = procPlane0;
    m_DeblockingProcPlane[1] = procPlane1;
    m_DeblockingProcPlane[2] = procPlane2;
    m_DeblockingTHR1 = THR1;
    m_DeblockingTHR2 = THR2;
    return UMC_OK;
}

Status MPEG4VideoDecoder::SetDeringingParams(bool procPlane0, bool procPlane1, bool procPlane2)
{
    m_DeringingProcPlane[0] = procPlane0;
    m_DeringingProcPlane[1] = procPlane1;
    m_DeringingProcPlane[2] = procPlane2;
    return UMC_OK;
}

Status MPEG4VideoDecoder::PostProcess(mp4_Frame *inout)
{
    int      w, h, i, j, k, QP, width, height;
    IppiSize size;
    Ipp8u    *pSrc[3], *pDst[3];
    int      srcPitch[3], dstPitch[3], threshold[6];
    QP = m_decInfo->VisualObject.VideoObject.short_video_header ? m_decInfo->VisualObject.VideoObject.VideoObjectPlaneH263.vop_quant : m_decInfo->VisualObject.VideoObject.VideoObjectPlane.quant;
    width = (m_decInfo->VisualObject.VideoObject.width + 7) & (~7);
    height = (m_decInfo->VisualObject.VideoObject.height + 7) & (~7);
    if (m_DeblockingProcPlane[0] || m_DeblockingProcPlane[1] || m_DeblockingProcPlane[2]) {
        pSrc[0] = inout->pY;
        srcPitch[0] = inout->stepY;
        pDst[0] = ppFrame0.pY;
        dstPitch[0] = ppFrame0.stepY;
        pSrc[1] = inout->pCb;
        srcPitch[1] = inout->stepCb;
        pDst[1] = ppFrame0.pCb;
        dstPitch[1] = ppFrame0.stepCb;
        pSrc[2] = inout->pCr;
        srcPitch[2] = inout->stepCr;
        pDst[2] = ppFrame0.pCr;
        dstPitch[2] = ppFrame0.stepCr;
        for (k = 0; k < 3; k ++) {
            if (m_DeblockingProcPlane[k]) {
                size.height = 8;
                if (k == 0) {
                    size.width = width;
                    h = height >> 3;
                    w = width >> 3;
                } else {
                    size.width = width >> 1;
                    h = height >> 4;
                    w = width >> 4;
                }
                for (i = 0; i < h; i ++) {
                    ippiCopy_8u_C1R(pSrc[k], srcPitch[k], pDst[k], dstPitch[k], size);
                    if (i > 0) {
                        for (j = 0; j < w; j ++)
                            ippiFilterDeblocking8x8HorEdge_MPEG4_8u_C1IR(pDst[k] + 8 * j, dstPitch[k], QP, m_DeblockingTHR1, m_DeblockingTHR2);
                        for (j = 1; j < w; j ++)
                            ippiFilterDeblocking8x8VerEdge_MPEG4_8u_C1IR(pDst[k] - 8 * dstPitch[k] + 8 * j, dstPitch[k], QP, m_DeblockingTHR1, m_DeblockingTHR2);
                    }
                    if (i == h - 1) {
                        for (j = 1; j < w; j ++)
                            ippiFilterDeblocking8x8VerEdge_MPEG4_8u_C1IR(pDst[k] + 8 * j, dstPitch[k], QP, m_DeblockingTHR1, m_DeblockingTHR2);
                    }
                    pSrc[k] += srcPitch[k] * 8;
                    pDst[k] += dstPitch[k] * 8;
                }
            } else {
                if (k == 0) {
                    size.width = width;
                    size.height = height;
                } else {
                    size.width = width >> 1;
                    size.height = height >> 1;
                }
                ippiCopy_8u_C1R(pSrc[k], srcPitch[k], pDst[k], dstPitch[k], size);
            }
        }
        *inout = ppFrame0;
    }
    if (m_DeringingProcPlane[0] || m_DeringingProcPlane[1] || m_DeringingProcPlane[2]) {
        pSrc[0] = inout->pY;
        srcPitch[0] = inout->stepY;
        pDst[0] = ppFrame1.pY;
        dstPitch[0] = ppFrame1.stepY;
        if (!m_DeringingProcPlane[0]) {
            size.width = width;
            size.height = height;
            ippiCopy_8u_C1R(pSrc[0], srcPitch[0], pDst[0], dstPitch[0], size);
        }
        pSrc[1] = inout->pCb;
        srcPitch[1] = inout->stepCb;
        pDst[1] = ppFrame1.pCb;
        dstPitch[1] = ppFrame1.stepCb;
        if (!m_DeringingProcPlane[1]) {
            size.width = width >> 1;
            size.height = height >> 1;
            ippiCopy_8u_C1R(pSrc[1], srcPitch[1], pDst[1], dstPitch[1], size);
        }
        pSrc[2] = inout->pCr;
        srcPitch[2] = inout->stepCr;
        pDst[2] = ppFrame1.pCr;
        dstPitch[2] = ppFrame1.stepCr;
        if (!m_DeringingProcPlane[2]) {
            size.width = width >> 1;
            size.height = height >> 1;
            ippiCopy_8u_C1R(pSrc[2], srcPitch[2], pDst[2], dstPitch[2], size);
        }
        h = inout->mbPerCol;
        w = inout->mbPerRow;
        for (i = 0; i < h; i ++) {
            for (j = 0; j < w; j ++) {
                ippiFilterDeringingThreshold_MPEG4_8u_P3R(pSrc[0]+ 16 * j, srcPitch[0], pSrc[1] + 8 * j, srcPitch[1], pSrc[2] + 8 * j, srcPitch[2], threshold);
                // copy border macroblocks
                if (i == 0 || i == h - 1 || j == 0 || j == w - 1) {
                    if (m_DeringingProcPlane[0])
                        ippiCopy16x16_8u_C1R(pSrc[0] + 16 * j, srcPitch[0], pDst[0] + 16 * j, dstPitch[0]);
                    if (m_DeringingProcPlane[1])
                        ippiCopy8x8_8u_C1R(pSrc[1] + 8 * j, srcPitch[1], pDst[1] + 8 * j, dstPitch[1]);
                    if (m_DeringingProcPlane[2])
                        ippiCopy8x8_8u_C1R(pSrc[2] + 8 * j, srcPitch[2], pDst[2] + 8 * j, dstPitch[2]);
                }
                if (m_DeringingProcPlane[0]) {
                    if (i != 0 && j != 0)
                        ippiFilterDeringingSmooth8x8_MPEG4_8u_C1R(pSrc[0] + 16 * j, srcPitch[0], pDst[0] + 16 * j, dstPitch[0], QP, threshold[0]);
                    if (i != 0 && j != w - 1)
                        ippiFilterDeringingSmooth8x8_MPEG4_8u_C1R(pSrc[0] + 16 * j + 8, srcPitch[0], pDst[0] + 16 * j + 8, dstPitch[0], QP, threshold[1]);
                    if (i != h - 1 && j != 0)
                        ippiFilterDeringingSmooth8x8_MPEG4_8u_C1R(pSrc[0] + 16 * j + 8 * srcPitch[0], srcPitch[0], pDst[0] + 16 * j + 8 * dstPitch[0], dstPitch[0], QP, threshold[2]);
                    if (i != h - 1 && j != w - 1)
                        ippiFilterDeringingSmooth8x8_MPEG4_8u_C1R(pSrc[0] + 16 * j + 8 * srcPitch[0] + 8, srcPitch[0], pDst[0] + 16 * j + 8 * dstPitch[0] + 8, dstPitch[0], QP, threshold[3]);
                }
                if (i != 0 && j != 0 && i != h - 1 && j != w - 1) {
                    if (m_DeringingProcPlane[1])
                        ippiFilterDeringingSmooth8x8_MPEG4_8u_C1R(pSrc[1] + 8 * j, srcPitch[1], pDst[1] + 8 * j, dstPitch[1], QP, threshold[4]);
                    if (m_DeringingProcPlane[2])
                        ippiFilterDeringingSmooth8x8_MPEG4_8u_C1R(pSrc[2] + 8 * j, srcPitch[2], pDst[2] + 8 * j, dstPitch[2], QP, threshold[5]);
                }
            }
            pSrc[0] += srcPitch[0] * 16;
            pDst[0] += dstPitch[0] * 16;
            pSrc[1] += srcPitch[1] * 8;
            pDst[1] += dstPitch[1] * 8;
            pSrc[2] += srcPitch[2] * 8;
            pDst[2] += dstPitch[2] * 8;
        }
        *inout = ppFrame1;
    }
    return UMC_OK;
}

#endif
