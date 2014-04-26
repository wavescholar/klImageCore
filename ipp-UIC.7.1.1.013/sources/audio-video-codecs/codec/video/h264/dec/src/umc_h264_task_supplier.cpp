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

#include "memory"

#include "umc_h264_task_supplier.h"
#include "umc_h264_frame_list.h"
#include "umc_automatic_mutex.h"
#include "umc_h264_nal_spl.h"
#include "umc_h264_bitstream.h"

#include "umc_h264_dec_defs_dec.h"
#include "vm_sys_info.h"
#include "umc_h264_segment_decoder_mt.h"

#include "umc_h264_task_broker.h"
#include "umc_video_processing.h"
#include "umc_structures.h"

#include "umc_frame_data.h"


namespace UMC
{

void OnSlideWindow(H264DecoderFrame *pRefFrame, NotifiersChain * notify)
{
    if (!pRefFrame)
        return;

    if (!pRefFrame->IsFrameExist())
    {
        pRefFrame->setWasOutputted();
        pRefFrame->setWasDisplayed();
        return;
    }

    if (!pRefFrame->isShortTermRef() && !pRefFrame->isLongTermRef())
    {
        VM_ASSERT(notify);

        if (notify)
        {
            if (!pRefFrame->IsDecoded())
            {
                pRefFrame->IncrementBusyState();
            }

            void * buf = notify->GetObjHeap()->Allocate ((notifier0<H264DecoderFrame> *) 0);
            notifier_base * notifier = new(buf) notifier0<H264DecoderFrame> (pRefFrame, &H264DecoderFrame::DecrementBusyState);
            notify->AddNotifier(notifier);
        }
    }
}


/****************************************************************************************************/
// Skipping class routine
/****************************************************************************************************/
Skipping::Skipping()
    : m_VideoDecodingSpeed(0)
    , m_SkipCycle(1)
    , m_ModSkipCycle(1)
    , m_PermanentTurnOffDeblocking(0)
    , m_SkipFlag(0)
    , m_NumberOfSkippedFrames(0)
{
}

Skipping::~Skipping()
{
}

void Skipping::Reset()
{
    m_VideoDecodingSpeed = 0;
    m_SkipCycle = 0;
    m_ModSkipCycle = 0;
    m_PermanentTurnOffDeblocking = 0;
    m_NumberOfSkippedFrames = 0;
}

void Skipping::PermanentDisableDeblocking(bool disable)
{
    m_PermanentTurnOffDeblocking = disable ? 3 : 0;
}

bool Skipping::IsShouldSkipDeblocking(H264DecoderFrame * pFrame, Ipp32s field)
{
    return (IS_SKIP_DEBLOCKING_MODE_PREVENTIVE || IS_SKIP_DEBLOCKING_MODE_PERMANENT ||
        (IS_SKIP_DEBLOCKING_MODE_NON_REF && !pFrame->GetAU(field)->IsReference()));
}

bool Skipping::IsShouldSkipFrame(H264DecoderFrame * pFrame, Ipp32s /*field*/)
{
    bool isShouldSkip = false;

    //bool isReference = pFrame->GetAU(field)->IsReference();

    bool isReference0 = pFrame->GetAU(0)->IsReference();
    bool isReference1 = pFrame->GetAU(1)->IsReference();

    bool isReference = isReference0 || isReference1;

    if ((m_VideoDecodingSpeed > 0) && !isReference)
    {
        if ((m_SkipFlag % m_ModSkipCycle) == 0)
        {
            isShouldSkip = true;
        }

        m_SkipFlag++;

        if (m_SkipFlag >= m_SkipCycle)
            m_SkipFlag = 0;
    }

    if (isShouldSkip)
        m_NumberOfSkippedFrames++;

    return isShouldSkip;
}

void Skipping::ChangeVideoDecodingSpeed(Ipp32s & num)
{
    m_VideoDecodingSpeed += num;

    if (m_VideoDecodingSpeed < 0)
        m_VideoDecodingSpeed = 0;
    if (m_VideoDecodingSpeed > 7)
        m_VideoDecodingSpeed = 7;

    num = m_VideoDecodingSpeed;

    Ipp32s deblocking_off = m_PermanentTurnOffDeblocking;

    if (m_VideoDecodingSpeed > 6)
    {
        m_SkipCycle = 1;
        m_ModSkipCycle = 1;
        m_PermanentTurnOffDeblocking = 2;
    }
    else if (m_VideoDecodingSpeed > 5)
    {
        m_SkipCycle = 1;
        m_ModSkipCycle = 1;
        m_PermanentTurnOffDeblocking = 0;
    }
    else if (m_VideoDecodingSpeed > 4)
    {
        m_SkipCycle = 3;
        m_ModSkipCycle = 2;
        m_PermanentTurnOffDeblocking = 1;
    }
    else if (m_VideoDecodingSpeed > 3)
    {
        m_SkipCycle = 3;
        m_ModSkipCycle = 2;
        m_PermanentTurnOffDeblocking = 0;
    }
    else if (m_VideoDecodingSpeed > 2)
    {
        m_SkipCycle = 2;
        m_ModSkipCycle = 2;
        m_PermanentTurnOffDeblocking = 0;
    }
    else if (m_VideoDecodingSpeed > 1)
    {
        m_SkipCycle = 3;
        m_ModSkipCycle = 3;
        m_PermanentTurnOffDeblocking = 0;
    }
    else if (m_VideoDecodingSpeed == 1)
    {
        m_SkipCycle = 4;
        m_ModSkipCycle = 4;
        m_PermanentTurnOffDeblocking = 0;
    }
    else
    {
        m_PermanentTurnOffDeblocking = 0;
    }

    if (deblocking_off == 3)
        m_PermanentTurnOffDeblocking = 3;
}

H264VideoDecoder::SkipInfo Skipping::GetSkipInfo() const
{
    H264VideoDecoder::SkipInfo info;
    info.isDeblockingTurnedOff = (m_VideoDecodingSpeed == 5) || (m_VideoDecodingSpeed == 7);
    info.numberOfSkippedFrames = m_NumberOfSkippedFrames;
    return info;
}

/****************************************************************************************************/
// POCDecoder
/****************************************************************************************************/
POCDecoder::POCDecoder()
{
    Reset();
}

POCDecoder::~POCDecoder()
{
}

void POCDecoder::Reset(Ipp32s IDRFrameNum)
{
    m_PicOrderCnt = 0;
    m_PicOrderCntMsb = 0;
    m_PicOrderCntLsb = 0;
    m_FrameNum = IDRFrameNum;
    m_PrevFrameRefNum = IDRFrameNum;
    m_FrameNumOffset = 0;
    m_TopFieldPOC = 0;
    m_BottomFieldPOC = 0;
}

void POCDecoder::DecodePictureOrderCount(H264Slice *slice, Ipp32s frame_num)
{
    H264SliceHeader *sliceHeader = slice->GetSliceHeader();
    H264SeqParamSet* sps = slice->GetSeqParam();

    Ipp32s uMaxFrameNum = (1<<sps->log2_max_frame_num);

    if (sps->pic_order_cnt_type == 0)
    {
        // pic_order_cnt type 0
        Ipp32s CurrPicOrderCntMsb;
        Ipp32s MaxPicOrderCntLsb = sps->MaxPicOrderCntLsb;

        if ((sliceHeader->pic_order_cnt_lsb < m_PicOrderCntLsb) &&
             ((m_PicOrderCntLsb - sliceHeader->pic_order_cnt_lsb) >= (MaxPicOrderCntLsb >> 1)))
            CurrPicOrderCntMsb = m_PicOrderCntMsb + MaxPicOrderCntLsb;
        else if ((sliceHeader->pic_order_cnt_lsb > m_PicOrderCntLsb) &&
                ((sliceHeader->pic_order_cnt_lsb - m_PicOrderCntLsb) > (MaxPicOrderCntLsb >> 1)))
            CurrPicOrderCntMsb = m_PicOrderCntMsb - MaxPicOrderCntLsb;
        else
            CurrPicOrderCntMsb = m_PicOrderCntMsb;

        if (sliceHeader->nal_ref_idc)
        {
            // reference picture
            m_PicOrderCntMsb = CurrPicOrderCntMsb & (~(MaxPicOrderCntLsb - 1));
            m_PicOrderCntLsb = sliceHeader->pic_order_cnt_lsb;
        }
        m_PicOrderCnt = CurrPicOrderCntMsb + sliceHeader->pic_order_cnt_lsb;
        if (sliceHeader->field_pic_flag == 0)
        {
             m_TopFieldPOC = CurrPicOrderCntMsb + sliceHeader->pic_order_cnt_lsb;
             m_BottomFieldPOC = m_TopFieldPOC + sliceHeader->delta_pic_order_cnt_bottom;
        }

    }    // pic_order_cnt type 0
    else if (sps->pic_order_cnt_type == 1)
    {
        // pic_order_cnt type 1
        Ipp32u i;
        Ipp32u uAbsFrameNum;    // frame # relative to last IDR pic
        Ipp32u uPicOrderCycleCnt = 0;
        Ipp32u uFrameNuminPicOrderCntCycle = 0;
        Ipp32s ExpectedPicOrderCnt = 0;
        Ipp32s ExpectedDeltaPerPicOrderCntCycle;
        Ipp32u uNumRefFramesinPicOrderCntCycle = sps->num_ref_frames_in_pic_order_cnt_cycle;

        if (frame_num < m_FrameNum)
            m_FrameNumOffset += uMaxFrameNum;

        if (uNumRefFramesinPicOrderCntCycle != 0)
            uAbsFrameNum = m_FrameNumOffset + frame_num;
        else
            uAbsFrameNum = 0;

        if ((sliceHeader->nal_ref_idc == false)  && (uAbsFrameNum > 0))
            uAbsFrameNum--;

        if (uAbsFrameNum)
        {
            uPicOrderCycleCnt = (uAbsFrameNum - 1) /
                    uNumRefFramesinPicOrderCntCycle;
            uFrameNuminPicOrderCntCycle = (uAbsFrameNum - 1) %
                    uNumRefFramesinPicOrderCntCycle;
        }

        ExpectedDeltaPerPicOrderCntCycle = 0;
        for (i=0; i < uNumRefFramesinPicOrderCntCycle; i++)
        {
            ExpectedDeltaPerPicOrderCntCycle +=
                sps->poffset_for_ref_frame[i];
        }

        if (uAbsFrameNum)
        {
            ExpectedPicOrderCnt = uPicOrderCycleCnt * ExpectedDeltaPerPicOrderCntCycle;
            for (i=0; i<=uFrameNuminPicOrderCntCycle; i++)
            {
                ExpectedPicOrderCnt +=
                    sps->poffset_for_ref_frame[i];
            }
        }
        else
            ExpectedPicOrderCnt = 0;

        if (sliceHeader->nal_ref_idc == false)
            ExpectedPicOrderCnt += sps->offset_for_non_ref_pic;
        m_PicOrderCnt = ExpectedPicOrderCnt + sliceHeader->delta_pic_order_cnt[0];
        if( sliceHeader->field_pic_flag==0)
        {
            m_TopFieldPOC = ExpectedPicOrderCnt + sliceHeader->delta_pic_order_cnt[ 0 ];
            m_BottomFieldPOC = m_TopFieldPOC +
                sps->offset_for_top_to_bottom_field + sliceHeader->delta_pic_order_cnt[ 1 ];
        }
        else if( ! sliceHeader->bottom_field_flag)
            m_PicOrderCnt = ExpectedPicOrderCnt + sliceHeader->delta_pic_order_cnt[ 0 ];
        else
            m_PicOrderCnt  = ExpectedPicOrderCnt + sps->offset_for_top_to_bottom_field + sliceHeader->delta_pic_order_cnt[ 0 ];
    }    // pic_order_cnt type 1
    else if (sps->pic_order_cnt_type == 2)
    {
        // pic_order_cnt type 2
        Ipp32s iMaxFrameNum = (1<<sps->log2_max_frame_num);
        Ipp32u uAbsFrameNum;    // frame # relative to last IDR pic

        if (frame_num < m_FrameNum)
            m_FrameNumOffset += iMaxFrameNum;
        uAbsFrameNum = frame_num + m_FrameNumOffset;
        m_PicOrderCnt = uAbsFrameNum*2;
        if (sliceHeader->nal_ref_idc == false)
            m_PicOrderCnt--;
            m_TopFieldPOC = m_PicOrderCnt;
            m_BottomFieldPOC = m_PicOrderCnt;

    }    // pic_order_cnt type 2

    if (sliceHeader->nal_ref_idc)
    {
        m_PrevFrameRefNum = frame_num;
    }

    m_FrameNum = frame_num;
}    // decodePictureOrderCount

Ipp32s POCDecoder::DetectFrameNumGap(H264Slice *slice)
{
    H264SeqParamSet* sps = slice->GetSeqParam();

    if (sps->gaps_in_frame_num_value_allowed_flag != 1)
        return 0;

    H264SliceHeader *sliceHeader = slice->GetSliceHeader();

    Ipp32s uMaxFrameNum = (1<<sps->log2_max_frame_num);
    Ipp32s frameNumGap;

    if (sliceHeader->idr_flag)
        return 0;

    // Capture any frame_num gap
    if (sliceHeader->frame_num != m_PrevFrameRefNum &&
        sliceHeader->frame_num != (m_PrevFrameRefNum + 1) % uMaxFrameNum)
    {
        // note this could be negative if frame num wrapped

        if (sliceHeader->frame_num > m_PrevFrameRefNum - 1)
        {
            frameNumGap = (sliceHeader->frame_num - m_PrevFrameRefNum - 1) % uMaxFrameNum;
        }
        else
        {
            frameNumGap = (uMaxFrameNum - (m_PrevFrameRefNum + 1 - sliceHeader->frame_num)) % uMaxFrameNum;
        }
    }
    else
    {
        frameNumGap = 0;
    }

    return frameNumGap;
}

/****************************************************************************************************/
// Resources
/****************************************************************************************************/

LocalResources::LocalResources()
    : next_mb_tables(0), m_ppMBIntraTypes(0), m_piMBIntraProp(0), m_pMBInfo(0), m_numberOfBuffers(0),
    m_pMemoryAllocator(0), m_parsedDataLength(0), m_pParsedData(0), m_midParsedData(0), m_currentResourceIndex(0)
{
    m_paddedParsedDataSize.width = 0;
    m_paddedParsedDataSize.height = 0;
}

LocalResources::~LocalResources()
{
    Close();
}

Status LocalResources::Init(Ipp32s numberOfBuffers, MemoryAllocator *pMemoryAllocator)
{
    Close();

    m_numberOfBuffers = numberOfBuffers;
    m_pMemoryAllocator = pMemoryAllocator;

    m_pMBInfo = new H264DecoderLocalMacroblockDescriptor[numberOfBuffers];
    if (NULL == m_pMBInfo)
        return UMC_ERR_ALLOC;

    m_ppMBIntraTypes = (Ipp32u **) ippMalloc((Ipp32s) (sizeof(Ipp32u *) * numberOfBuffers));
    if (NULL == m_ppMBIntraTypes)
        return UMC_ERR_ALLOC;
    memset(m_ppMBIntraTypes, 0, sizeof(Ipp32u *) * numberOfBuffers);

    // allocate intra MB types array's sizes
    m_piMBIntraProp = (H264IntraTypesProp *) ippMalloc((Ipp32s) (sizeof(H264IntraTypesProp) * numberOfBuffers));
    if (NULL == m_piMBIntraProp)
        return UMC_ERR_ALLOC;
    memset(m_piMBIntraProp, 0, sizeof(H264IntraTypesProp) * numberOfBuffers);

    next_mb_tables = new H264DecoderMBAddr *[numberOfBuffers + 1];

    return UMC_OK;
}

void LocalResources::Reset()
{
    for (Ipp32s i = 0; i < m_numberOfBuffers; i++)
    {
        m_pMBInfo[i].m_isBusy = false;
        m_pMBInfo[i].m_pFrame = 0;
    }

    m_currentResourceIndex = 0;
}

void LocalResources::Close()
{
    if (m_ppMBIntraTypes)
    {
        ippFree(m_ppMBIntraTypes);
        m_ppMBIntraTypes = 0;
    }

    if (m_piMBIntraProp)
    {
        for (Ipp32s i = 0; i < m_numberOfBuffers; i++)
        {
            if (m_piMBIntraProp[i].m_nSize == 0)
                continue;
            m_pMemoryAllocator->Unlock(m_piMBIntraProp[i].m_mid);
            m_pMemoryAllocator->Free(m_piMBIntraProp[i].m_mid);
        }

        ippFree(m_piMBIntraProp);
        m_piMBIntraProp = 0;
    }

    delete[] m_pMBInfo;
    m_pMBInfo = 0;

    delete[] next_mb_tables;
    next_mb_tables = 0;

    DeallocateBuffers();

    m_numberOfBuffers = 0;
    m_pMemoryAllocator = 0;
    m_currentResourceIndex = 0;
}

Ipp32u LocalResources::GetCurrentResourceIndex()
{
    Ipp32s index = m_currentResourceIndex % (m_numberOfBuffers);
    m_currentResourceIndex++;
    return index;
}

bool LocalResources::LockFrameResource(H264DecoderFrame * frame)
{
    Ipp32s number = frame->m_iResourceNumber;
    if (m_pMBInfo[number].m_isBusy)
        return m_pMBInfo[number].m_pFrame == frame;

    m_pMBInfo[number].m_isBusy = true;
    m_pMBInfo[number].m_pFrame = frame;
    return true;
}

void LocalResources::UnlockFrameResource(H264DecoderFrame * frame)
{
    Ipp32s number = frame->m_iResourceNumber;
    if (number < 0 || m_pMBInfo[number].m_pFrame != frame)
        return;

    m_pMBInfo[number].m_isBusy = false;
    m_pMBInfo[number].m_pFrame = 0;
}

H264DecoderFrame * LocalResources::IsBusyByFrame(Ipp32s number)
{
    return m_pMBInfo[number].m_pFrame;
}

bool LocalResources::AllocateMBIntraTypes(Ipp32s iIndex, Ipp32s iMBNumber)
{
    if ((0 == m_ppMBIntraTypes[iIndex]) ||
        (m_piMBIntraProp[iIndex].m_nSize < iMBNumber))
    {
        size_t nSize;
        // delete previously allocated array
        if (m_ppMBIntraTypes[iIndex])
        {
            m_pMemoryAllocator->Unlock(m_piMBIntraProp[iIndex].m_mid);
            m_pMemoryAllocator->Free(m_piMBIntraProp[iIndex].m_mid);
        }
        m_ppMBIntraTypes[iIndex] = NULL;
        m_piMBIntraProp[iIndex].Reset();

        nSize = iMBNumber * NUM_INTRA_TYPE_ELEMENTS * sizeof(IntraType);
        if (UMC_OK != m_pMemoryAllocator->Alloc(&(m_piMBIntraProp[iIndex].m_mid),
                                                nSize,
                                                UMC_ALLOC_PERSISTENT))
            return false;
        m_piMBIntraProp[iIndex].m_nSize = (Ipp32s) iMBNumber;
        m_ppMBIntraTypes[iIndex] = (Ipp32u *) m_pMemoryAllocator->Lock(m_piMBIntraProp[iIndex].m_mid);
    }

    return true;
}

H264DecoderLocalMacroblockDescriptor & LocalResources::GetMBInfo(Ipp32s number)
{
    return m_pMBInfo[number];
}

void LocalResources::AllocateMBInfo(Ipp32s number, Ipp32u iMBCount)
{
    m_pMBInfo[number].Allocate(iMBCount, m_pMemoryAllocator);
}

IntraType * LocalResources::GetIntraTypes(Ipp32s number)
{
    return m_ppMBIntraTypes[number];
}

Status LocalResources::AllocateBuffers(H264SeqParamSet* sps, bool exactSizeRequested)
{
    Status      umcRes = UMC_OK;
    IppiSize desiredPaddedSize;

    IppiSize  size;
    size.width = sps->frame_width_in_mbs * 16;
    size.height = sps->frame_height_in_mbs * 16;

    desiredPaddedSize.width  = (size.width  + 15) & ~15;
    desiredPaddedSize.height = (size.height + 15) & ~15;

    // If our buffer and internal pointers are already set up for this
    // image size, then there's nothing more to do.
    // But if exactSizeRequested, we need to see if our existing
    // buffer is oversized, and perhaps reallocate it.

    if (m_paddedParsedDataSize.width == desiredPaddedSize.width &&
        m_paddedParsedDataSize.height == desiredPaddedSize.height &&
        !exactSizeRequested)
        return umcRes;

    // Determine how much space we need
    Ipp32s     MB_Frame_Width   = desiredPaddedSize.width >> 4;
    Ipp32s     MB_Frame_Height  = desiredPaddedSize.height >> 4;

    Ipp32s     uMBMapSize   = MB_Frame_Width * MB_Frame_Height;
    Ipp32s     next_mb_size = (Ipp32s)(MB_Frame_Width*MB_Frame_Height*sizeof(H264DecoderMBAddr));

    Ipp32s     totalSize = (m_numberOfBuffers + 1)*next_mb_size + uMBMapSize + 128; // 128 used for alignments

    // Reallocate our buffer if its size is not appropriate.
    if (m_parsedDataLength < totalSize ||
        (exactSizeRequested && (m_parsedDataLength != totalSize)))
    {
        DeallocateBuffers();

        if (m_pMemoryAllocator->Alloc(&m_midParsedData,
                                      totalSize,
                                      UMC_ALLOC_PERSISTENT))
            return UMC_ERR_ALLOC;

        m_pParsedData = (Ipp8u *) m_pMemoryAllocator->Lock(m_midParsedData);
        ippsSet_8u(0, m_pParsedData, totalSize);

        m_parsedDataLength = totalSize;
    }

    // Reassign our internal pointers if need be
    if (m_paddedParsedDataSize.width != desiredPaddedSize.width ||
        m_paddedParsedDataSize.height != desiredPaddedSize.height)
    {
        m_paddedParsedDataSize = desiredPaddedSize;

        size_t     offset = 0;

        m_pMBMap = align_pointer<Ipp8u *> (m_pParsedData);
        offset += uMBMapSize;

        if (offset & 0x7)
            offset = (offset + 7) & ~7;
        next_mb_tables[0] = align_pointer<H264DecoderMBAddr *> (m_pParsedData + offset);

        //initialize first table
        for (Ipp32s i = 0; i < uMBMapSize; i++)
            next_mb_tables[0][i] = i + 1; // simple linear scan

        offset += next_mb_size;

        for (Ipp32s i = 1; i <= m_numberOfBuffers; i++)
        {
            if (offset & 0x7)
                offset = (offset + 7) & ~7;

            next_mb_tables[i] = align_pointer<H264DecoderMBAddr *> (m_pParsedData + offset);
            offset += next_mb_size;
        }
    }

    return umcRes;
}

void LocalResources::DeallocateBuffers()
{
    if (m_pParsedData)
    {
        // Free the old buffer.
        m_pMemoryAllocator->Unlock(m_midParsedData);
        m_pMemoryAllocator->Free(m_midParsedData);
        m_pParsedData = 0;
        m_midParsedData = 0;
    }

    m_parsedDataLength = 0;
    m_paddedParsedDataSize.width = 0;
    m_paddedParsedDataSize.height = 0;
}

/****************************************************************************************************/
// SEI_Storer
/****************************************************************************************************/
SEI_Storer::SEI_Storer()
{
    Reset();
}

SEI_Storer::~SEI_Storer()
{
    Close();
}

void SEI_Storer::Init()
{
    Close();
    m_data.resize(MAX_BUFFERED_SIZE);
    m_payloads.resize(START_ELEMENTS);
    m_offset = 0;
    m_lastUsed = 2;
}

void SEI_Storer::Close()
{
    Reset();
    m_data.clear();
    m_payloads.clear();
}

void SEI_Storer::Reset()
{
    m_lastUsed = 2;
    for (Ipp32u i = 0; i < m_payloads.size(); i++)
    {
        m_payloads[i].isUsed = 0;
    }
}

void SEI_Storer::SetFrame(H264DecoderFrame * frame)
{
    VM_ASSERT(frame);
    for (Ipp32u i = 0; i < m_payloads.size(); i++)
    {
        if (m_payloads[i].frame == 0 && m_payloads[i].isUsed)
        {
            m_payloads[i].frame = frame;
        }
    }
}

void SEI_Storer::SetTimestamp(H264DecoderFrame * frame)
{
    VM_ASSERT(frame);
    Ipp64f ts = frame->m_dFrameTime;

    for (Ipp32u i = 0; i < m_payloads.size(); i++)
    {
        if (m_payloads[i].frame == frame)
        {
            m_payloads[i].timestamp = ts;
            if (m_payloads[i].isUsed)
                m_payloads[i].isUsed = m_lastUsed;
        }
    }

    m_lastUsed++;
}

const SEI_Storer::SEI_Message * SEI_Storer::GetPayloadMessage()
{
    SEI_Storer::SEI_Message * msg = 0;

    for (Ipp32u i = 0; i < m_payloads.size(); i++)
    {
        if (m_payloads[i].isUsed > 1)
        {
            if (!msg || msg->isUsed > m_payloads[i].isUsed)
            {
                msg = &m_payloads[i];
            }
        }
    }

    if (msg)
        msg->isUsed = 0;

    return msg;
}

SEI_Storer::SEI_Message* SEI_Storer::AddMessage(UMC::MediaDataEx *nalUnit, SEI_TYPE type)
{
    size_t sz = nalUnit->GetDataSize();

    if (sz > (m_data.size() >> 2))
        return 0;

    if (m_offset + sz > m_data.size())
    {
        m_offset = 0;
    }

    // clear overwriting messages:
    for (Ipp32u i = 0; i < m_payloads.size(); i++)
    {
        if (!m_payloads[i].isUsed)
            continue;

        SEI_Message & mmsg = m_payloads[i];

        if ((m_offset + sz > mmsg.offset) &&
            (m_offset < mmsg.offset + mmsg.msg_size))
        {
            m_payloads[i].isUsed = 0;
            return 0;
        }
    }

    size_t freeSlot = 0;
    for (Ipp32u i = 0; i < m_payloads.size(); i++)
    {
        if (!m_payloads[i].isUsed)
        {
            freeSlot = i;
            break;
        }
    }

    if (m_payloads[freeSlot].isUsed)
    {
        if (m_payloads.size() >= MAX_ELEMENTS)
            return 0;

        m_payloads.push_back(SEI_Message());
        freeSlot = m_payloads.size() - 1;
    }

    m_payloads[freeSlot].msg_size = sz;
    m_payloads[freeSlot].offset = m_offset;
    m_payloads[freeSlot].timestamp = 0;
    m_payloads[freeSlot].frame = 0;
    m_payloads[freeSlot].isUsed = 1;
    m_payloads[freeSlot].data = &(m_data.front()) + m_offset;
    m_payloads[freeSlot].type = type;

    memcpy(&m_data[m_offset], (Ipp8u*)nalUnit->GetDataPointer(), sz);

    m_offset += sz;

    return &m_payloads[freeSlot];
}

/****************************************************************************************************/
// TaskSupplier
/****************************************************************************************************/
TaskSupplier::TaskSupplier()
    : AU_Splitter(&m_Heap, &m_ObjHeap)
    , m_pSegmentDecoder(0)
    , m_iThreadNum(0)
    , m_use_external_framerate(false)
    , m_pDecodedFramesList(0)
    , m_pLastSlice(0)
    , m_pLastDisplayed(0)
    , m_pMemoryAllocator(0)
    , m_pFrameAllocator(0)
    , m_dpbSize(1)
    , m_maxDecFrameBuffering(1)
    , m_DPBSizeEx(0)
    , m_TrickModeSpeed(1)
    , m_pTaskBroker(0)
    , m_pPostProcessing(0)
    , m_DefaultNotifyChain(&m_ObjHeap)
    , m_UIDFrameCounter(0)
    , m_sei_messages(0)
    , m_isInitialized(false)
{
}

TaskSupplier::~TaskSupplier()
{
    Close();
}

Status TaskSupplier::Init(BaseCodecParams *pInit)
{
    VideoDecoderParams *init = DynamicCast<VideoDecoderParams, BaseCodecParams> (pInit);

    if (NULL == init)
        return UMC_ERR_NULL_PTR;
    if(!init->m_pPostProcessor)
        return UMC_ERR_NULL_PTR;

    Close();

    m_dpbSize = 0;
    m_DPBSizeEx = 0;
    m_pCurrentFrame = 0;
    m_pLastFullFrame = 0;

    if (ABSOWN(init->m_fPlaybackRate - 1) > 0.0001)
    {
        m_TrickModeSpeed = 2;
    }
    else
    {
        m_TrickModeSpeed = 1;
    }

    // calculate number of slice decoders.
    // It should be equal to CPU number
    if(!init->m_iThreads)
        init->m_iThreads = vm_sys_info_get_cpu_num();
    if(init->m_iThreads > 16)
        init->m_iThreads = 16;

    m_iThreadNum = init->m_iThreads;

    m_pDecodedFramesList = new H264DBPList();
    if (!m_pDecodedFramesList)
        return UMC_ERR_ALLOC;

    AU_Splitter::Init(init);

    switch(m_iThreadNum)
    {
    case 1:
        m_pTaskBroker = new TaskBrokerSingleThread(this);
        break;
    case 4:
    case 3:
    case 2:
        m_pTaskBroker = new TaskBrokerTwoThread(this);
        break;
    default:
        m_pTaskBroker = new TaskBrokerTwoThread(this);
        break;
    };

    m_pTaskBroker->Init(m_iThreadNum, true);

    // create slice decoder(s)
    m_pSegmentDecoder = new H264SegmentDecoderMultiThreaded *[m_iThreadNum];
    if (NULL == m_pSegmentDecoder)
        return UMC_ERR_ALLOC;
    memset(m_pSegmentDecoder, 0, sizeof(H264SegmentDecoderMultiThreaded *) * m_iThreadNum);

    Ipp32s i;
    for (i = 0; i < m_iThreadNum; i += 1)
    {
        m_pSegmentDecoder[i] = new H264SegmentDecoderMultiThreaded(m_pTaskBroker);
        if (NULL == m_pSegmentDecoder[i])
            return UMC_ERR_ALLOC;
    }

    for (i = 0; i < m_iThreadNum; i += 1)
    {
        if (UMC_OK != m_pSegmentDecoder[i]->Init(i))
            return UMC_ERR_INIT;

        if (!i)
            continue;

        H264Thread * thread = new H264Thread();
        if (thread->Init(i, m_pSegmentDecoder[i]) != UMC_OK)
        {
            delete thread;
            return UMC_ERR_INIT;
        }

        m_threadGroup.AddThread(thread);
    }

    m_pPostProcessing = init->m_pPostProcessor;

    LocalResources::Init(2*(m_iThreadNum), m_pMemoryAllocator);

    m_local_delta_frame_time = 0;
    m_local_frame_time       = 0;
    m_use_external_framerate = 0 < init->m_info.fFramerate;

    if (m_use_external_framerate)
    {
        m_local_delta_frame_time = 1 / init->m_info.fFramerate;
    }

    m_DPBSizeEx = m_iThreadNum + 1;

    m_isInitialized = true;

    return UMC_OK;
}

Status TaskSupplier::PreInit(BaseCodecParams *pInit)
{
    if (m_isInitialized)
        return UMC_OK;

    VideoDecoderParams *init = DynamicCast<VideoDecoderParams, BaseCodecParams> (pInit);

    if (NULL == init)
        return UMC_ERR_NULL_PTR;

    Close();

    m_dpbSize = 0;
    m_DPBSizeEx = 0;
    m_pCurrentFrame = 0;
    m_pLastFullFrame = 0;

    // calculate number of slice decoders.
    // It should be equal to CPU number
    if(!init->m_iThreads)
        init->m_iThreads = vm_sys_info_get_cpu_num();
    if(init->m_iThreads > 16)
        init->m_iThreads = 16;

    m_iThreadNum = init->m_iThreads;

    m_pDecodedFramesList = new H264DBPList();
    if (!m_pDecodedFramesList)
        return UMC_ERR_ALLOC;

    AU_Splitter::Init(init);
    LocalResources::Init(2*m_iThreadNum, m_pMemoryAllocator);

    m_local_delta_frame_time = 0;
    m_local_frame_time       = 0;
    m_use_external_framerate = 0 < init->m_info.fFramerate;

    if (m_use_external_framerate)
    {
        m_local_delta_frame_time = 1 / init->m_info.fFramerate;
    }

    m_DPBSizeEx = m_iThreadNum + 1;

    return UMC_OK;
}

Status  TaskSupplier::SetParams(BaseCodecParams* params)
{
    VideoDecoderParams *pParams = DynamicCast<VideoDecoderParams, BaseCodecParams>(params);

    if (NULL == pParams)
        return UMC_ERR_NULL_PTR;

    if (pParams->m_iTrickModes == 7)
    {
        if (ABSOWN(pParams->m_fPlaybackRate - 1) > 0.0001)
            m_TrickModeSpeed = 2;
        else
            m_TrickModeSpeed = 1;
    }

    return UMC_OK;
}

void TaskSupplier::Close()
{
    if (m_pTaskBroker)
    {
        m_pTaskBroker->Release();
    }

// from reset

    m_threadGroup.Release();

    if (m_pSegmentDecoder)
    {
        for (Ipp32s i = 0; i < m_iThreadNum; i += 1)
        {
            delete m_pSegmentDecoder[i];
            m_pSegmentDecoder[i] = 0;
        }
    }

    if (m_pDecodedFramesList)
    {
        m_pDecodedFramesList->Reset();
    }

    AU_Splitter::Close();

    if (m_pLastSlice)
    {
        m_Heap.Free(m_pLastSlice->m_pSource);
        m_pLastSlice->m_pSource = 0;
        m_pLastSlice->Release();
        m_ObjHeap.FreeObject(m_pLastSlice);
        m_pLastSlice = 0;
    }

    m_Headers.Reset(true);
    Skipping::Reset();
    m_Heap.Reset();

    m_local_frame_time         = 0;

    m_field_index       = 0;
    m_WaitForIDR        = true;

    m_pLastDisplayed = 0;
    m_pCurrentFrame = 0;
    m_pLastFullFrame = 0;

    POCDecoder::Reset();

    delete m_sei_messages;
    m_sei_messages = 0;

// from reset

    LocalResources::Close();

    delete[] m_pSegmentDecoder;
    m_pSegmentDecoder = 0;

    delete m_pTaskBroker;
    m_pTaskBroker = 0;

    m_Headers.Reset();

    delete m_pDecodedFramesList;
    m_pDecodedFramesList = 0;
    m_iThreadNum = 0;

    m_dpbSize = 1;
    m_DPBSizeEx = 1;
    m_maxDecFrameBuffering = 1;

    m_isInitialized = false;
}

void TaskSupplier::Reset()
{
    if (m_pTaskBroker)
        m_pTaskBroker->Reset();

    m_threadGroup.Reset();

    m_DefaultNotifyChain.Notify();
    m_DefaultNotifyChain.Reset();

    if (m_pDecodedFramesList)
    {
        m_pDecodedFramesList->Reset();
    }

    if (m_sei_messages)
        m_sei_messages->Reset();

    AU_Splitter::Reset();

    if (m_pLastSlice)
    {
        m_Heap.Free(m_pLastSlice->m_pSource);
        m_pLastSlice->m_pSource = 0;
        m_pLastSlice->Release();
        m_ObjHeap.FreeObject(m_pLastSlice);
        m_pLastSlice = 0;
    }

    m_Headers.Reset(true);
    Skipping::Reset();
    m_Heap.Reset();

    m_local_frame_time         = 0;

    m_field_index       = 0;
    m_WaitForIDR        = true;

    m_pLastDisplayed = 0;
    m_pCurrentFrame = 0;
    m_pLastFullFrame = 0;

    POCDecoder::Reset();

    LocalResources::Reset();

    if (m_pTaskBroker)
        m_pTaskBroker->Init(m_iThreadNum, true);
}

void TaskSupplier::AfterErrorRestore()
{
    if (m_pTaskBroker)
        m_pTaskBroker->Reset();

    m_threadGroup.Reset();

    if (m_pDecodedFramesList)
    {
        m_pDecodedFramesList->Reset();
    }

    AU_Splitter::Reset();

    Skipping::Reset();
    m_Heap.Reset();
    m_Headers.Reset(true);

    m_field_index       = 0;

    m_pLastDisplayed = 0;
    m_pCurrentFrame = 0;
    m_pLastFullFrame = 0;

    if (m_pTaskBroker)
        m_pTaskBroker->Init(m_iThreadNum, true);
}

Status TaskSupplier::GetInfoFromData(BaseCodecParams* params)
{
    Status umcRes = UMC_OK;
    VideoDecoderParams *lpInfo = DynamicCast<VideoDecoderParams, BaseCodecParams> (params);

    if (!m_isInitialized)
    {
        if (params->m_pData)
        {
            if (!m_pMemoryAllocator)
            {
                // DEBUG : allocate allocator or do it in PreInit !!!
            }
            //m_pTaskSupplier->SetMemoryAllocator(m_pMemoryAllocator);
            PreInit(params);

            if (lpInfo->m_pData && lpInfo->m_pData->GetDataSize())
            {
                umcRes = AddOneFrame(lpInfo->m_pData, 0);
            }

            umcRes = GetInfo(lpInfo);
            Close();
            return umcRes;
        }

        return UMC_ERR_NOT_INITIALIZED;
    }

    if (NULL == lpInfo)
        return UMC_ERR_NULL_PTR;

    return GetInfo(lpInfo);
}

Status TaskSupplier::GetInfo(VideoDecoderParams* lpInfo)
{
    //if (!m_isInitialized)
      //  return UMC_ERR_NOT_INITIALIZED;

    lpInfo->m_pPostProcessor = m_pPostProcessing;

    const H264SeqParamSet *sps = m_Headers.m_SeqParams.GetCurrentHeader();
    if (!sps)
    {
        return UMC_ERR_NOT_ENOUGH_DATA;
    }

    const H264PicParamSet *pps = m_Headers.m_PicParams.GetCurrentHeader();

    lpInfo->m_info.videoInfo.m_iHeight = sps->frame_height_in_mbs * 16 -
        SubHeightC[sps->chroma_format_idc]*(2 - sps->frame_mbs_only_flag) *
        (sps->frame_cropping_rect_top_offset + sps->frame_cropping_rect_bottom_offset);

    lpInfo->m_info.videoInfo.m_iWidth = sps->frame_width_in_mbs * 16 - SubWidthC[sps->chroma_format_idc] *
        (sps->frame_cropping_rect_left_offset + sps->frame_cropping_rect_right_offset);

    if (0.0 < m_local_delta_frame_time)
    {
        lpInfo->m_info.fFramerate = 1.0 / m_local_delta_frame_time;
    }
    else
    {
        lpInfo->m_info.fFramerate = 0;
    }

    lpInfo->m_info.streamType = H264_VIDEO;

    lpInfo->m_info.iProfile = sps->profile_idc;
    lpInfo->m_info.iLevel = sps->level_idc;

    lpInfo->m_iThreads = m_iThreadNum;
    lpInfo->m_info.videoInfo.m_colorFormat = GetUMCColorFormat(sps->chroma_format_idc);

    lpInfo->m_info.iProfile = sps->profile_idc;
    lpInfo->m_info.iLevel = sps->level_idc;

    if (sps->aspect_ratio_idc == 255)
    {
        lpInfo->m_info.videoInfo.m_iSAWidth  = sps->sar_width;
        lpInfo->m_info.videoInfo.m_iSAHeight = sps->sar_height;
    }
    else
    {
        lpInfo->m_info.videoInfo.m_iSAWidth  = SAspectRatio[sps->aspect_ratio_idc][0];
        lpInfo->m_info.videoInfo.m_iSAHeight = SAspectRatio[sps->aspect_ratio_idc][1];
    }

    Ipp32u multiplier = 1 << (6 + sps->bit_rate_scale);
    lpInfo->m_info.iBitrate = sps->bit_rate_value[0] * multiplier;

    if (sps->frame_mbs_only_flag)
        lpInfo->m_info.videoInfo.m_picStructure = PS_PROGRESSIVE;
    else
    {
        if (0 <= sps->offset_for_top_to_bottom_field)
            lpInfo->m_info.videoInfo.m_picStructure = PS_TOP_FIELD_FIRST;
        else
            lpInfo->m_info.videoInfo.m_picStructure = PS_BOTTOM_FIELD_FIRST;
    }

    H264VideoDecoderParams* lpH264Info = DynamicCast<H264VideoDecoderParams, VideoDecoderParams> (lpInfo);
    if (lpH264Info)
    {
        SetDPBSize();
        lpH264Info->m_DPBSize = m_dpbSize + m_DPBSizeEx;

        IppiSize sz;
        sz.width    = sps->frame_width_in_mbs  * 16;
        sz.height   = sps->frame_height_in_mbs * 16;
        lpH264Info->m_fullSize = sz;

        if (pps)
        {
           lpH264Info->m_entropy_coding_type = pps->entropy_coding_mode;
        }

        lpH264Info->m_cropArea.iTop = (Ipp16s)(SubHeightC[sps->chroma_format_idc] * sps->frame_cropping_rect_top_offset * (2 - sps->frame_mbs_only_flag));
        lpH264Info->m_cropArea.iBottom = (Ipp16s)(SubHeightC[sps->chroma_format_idc] * sps->frame_cropping_rect_bottom_offset * (2 - sps->frame_mbs_only_flag));
        lpH264Info->m_cropArea.iLeft = (Ipp16s)(SubWidthC[sps->chroma_format_idc] * sps->frame_cropping_rect_left_offset);
        lpH264Info->m_cropArea.iRight = (Ipp16s)(SubWidthC[sps->chroma_format_idc] * sps->frame_cropping_rect_right_offset);

        lpH264Info->m_auxiliary_format_idc = 0;

        const H264SeqParamSetExtension *sps_ex = m_Headers.m_SeqExParams.GetCurrentHeader();
        if (sps_ex)
        {
            lpH264Info->m_auxiliary_format_idc = sps_ex->aux_format_idc;
        }
    }

    return UMC_OK;
}

H264DecoderFrame *TaskSupplier::GetFreeFrame(void)
{
    AutomaticMutex guard(m_mGuard);

    H264DecoderFrame *pFrame = 0;

    // Traverse list for next disposable frame
    if (m_pDecodedFramesList->countAllFrames() >= m_dpbSize + m_DPBSizeEx)
        pFrame = m_pDecodedFramesList->GetOldestDisposable();

    VM_ASSERT(!pFrame || pFrame->GetBusyState() == 0);

    // Did we find one?
    if (NULL == pFrame)
    {
        if (m_pDecodedFramesList->countAllFrames() >= m_dpbSize + m_DPBSizeEx)
        {
            return 0;
        }

        // Didn't find one. Let's try to insert a new one
        pFrame = new H264DecoderFrameExtension(m_pMemoryAllocator, &m_Heap, &m_ObjHeap);
        if (NULL == pFrame)
            return 0;

        m_pDecodedFramesList->append(pFrame);

        pFrame->m_index = m_pDecodedFramesList->GetFreeIndex();
    }

    pFrame->Reset();

    // Set current as not displayable (yet) and not outputted. Will be
    // updated to displayable after successful decode.
    pFrame->unsetWasOutputted();
    pFrame->unSetisDisplayable();
    pFrame->SetSkipped(false);
    pFrame->SetFrameExistFlag(true);
    pFrame->SetBusyState(BUSY_STATE_NOT_DECODED);

    if (GetAuxiliaryFrame(pFrame))
    {
        GetAuxiliaryFrame(pFrame)->Reset();
    }

    m_UIDFrameCounter++;
    pFrame->m_UID = m_UIDFrameCounter;
    return pFrame;
}

Status TaskSupplier::DecodeSEI(MediaDataEx *nalUnit)
{
    if (m_Headers.m_SeqParams.GetCurrentID() == -1)
        return UMC_OK;

    H264Bitstream bitStream;

    try
    {
        H264MemoryPiece mem;
        mem.SetData(nalUnit);

        H264MemoryPiece * pMem = m_Heap.Allocate(nalUnit->GetDataSize() + 8);
        notifier1<H264_Heap, H264MemoryPiece*> memory_leak_preventing(&m_Heap, &H264_Heap::Free, pMem);

        memset(pMem->GetPointer() + nalUnit->GetDataSize(), 0, 8);

        SwapperBase * swapper = m_pNALSplitter->GetSwapper();
        swapper->SwapMemory(pMem, &mem);

        // Ipp32s nalIndex = nalUnit->GetExData()->index;

        bitStream.Reset((Ipp8u*)pMem->GetPointer(), (Ipp32u)pMem->GetDataSize());

        //bitStream.Reset((Ipp8u*)nalUnit->GetDataPointer() + nalUnit->GetExData()->offsets[nalIndex],
          //  nalUnit->GetExData()->offsets[nalIndex + 1] - nalUnit->GetExData()->offsets[nalIndex]);

        NAL_Unit_Type uNALUnitType;
        Ipp8u uNALStorageIDC;

        bitStream.GetNALUnitType(uNALUnitType, uNALStorageIDC);

        do
        {
            H264SEIPayLoad    m_SEIPayLoads;

            /*Ipp32s target_sps =*/ bitStream.ParseSEI(m_Headers.m_SeqParams,
                m_Headers.m_SeqParams.GetCurrentID(), &m_SEIPayLoads);

            if (m_SEIPayLoads.payLoadType == SEI_USER_DATA_REGISTERED_TYPE)
            {
                m_UserData.PushBack();
                m_UserData.Back() = m_SEIPayLoads;
            }
            else
            {
                H264SEIPayLoad * spl = m_Headers.m_SEIParams.GetHeader(m_SEIPayLoads.payLoadType);
                if (m_Headers.m_SEIParams.AddHeader(&m_SEIPayLoads, false))
                {
                        void * buf = m_ObjHeap.Allocate ((notifier2<Headers, Ipp32s, void*> *)0);
                        notifier_base * notifier = new(buf) notifier2<Headers, Ipp32s, void*>(&m_Headers, &Headers::Signal, 3, spl);
                        m_DefaultNotifyChain.AddNotifier(notifier);
                }
            }

        } while (bitStream.More_RBSP_Data());
    }
    catch(...) { /* nothing to do just catch it */}

    return UMC_OK;
}

Status TaskSupplier::DecodeHeaders(MediaDataEx *nalUnitPar)
{
    Status umcRes = UMC_OK;

    H264Bitstream bitStream;

    MediaDataEx::_MediaDataEx * pMediaEx = nalUnitPar->GetExData();
    MediaData nalUnitTemp;
    nalUnitTemp.SetBufferPointer((Ipp8u*)nalUnitPar->GetDataPointer() + pMediaEx->offsets[pMediaEx->index], pMediaEx->offsets[pMediaEx->index + 1] - pMediaEx->offsets[pMediaEx->index]);
    nalUnitTemp.SetDataSize(pMediaEx->offsets[pMediaEx->index + 1] - pMediaEx->offsets[pMediaEx->index]);

    MediaData * nalUnit = &nalUnitTemp;

    try
    {
        H264MemoryPiece mem;
        mem.SetData(nalUnit);

        H264MemoryPiece * pMem = m_Heap.Allocate(nalUnit->GetDataSize() + 8);
        notifier1<H264_Heap, H264MemoryPiece*> memory_leak_preventing(&m_Heap, &H264_Heap::Free, pMem);

        memset(pMem->GetPointer() + nalUnit->GetDataSize(), 0, 8);

        SwapperBase * swapper = m_pNALSplitter->GetSwapper();
        swapper->SwapMemory(pMem, &mem);

        bitStream.Reset((Ipp8u*)pMem->GetPointer(), (Ipp32u)pMem->GetDataSize());

        NAL_Unit_Type uNALUnitType;
        Ipp8u uNALStorageIDC;

        bitStream.GetNALUnitType(uNALUnitType, uNALStorageIDC);

        switch(uNALUnitType)
        {
        // sequence parameter set
        case NAL_UT_SPS:
            {
                H264SeqParamSet sps;
                umcRes = bitStream.GetSequenceParamSet(&sps);
                if (umcRes == UMC_OK)
                {
                    H264SeqParamSet * temp = m_Headers.m_SeqParams.GetHeader(sps.seq_parameter_set_id);
                    if (m_Headers.m_SeqParams.AddHeader(&sps, false))
                    {
                        void * buf = m_ObjHeap.Allocate ((notifier2<Headers, Ipp32s, void*> *)0);
                        notifier_base * notifier = new(buf) notifier2<Headers, Ipp32s, void*>(&m_Headers, &Headers::Signal, 0, temp);
                        m_DefaultNotifyChain.AddNotifier(notifier);
                    }

                    sps.poffset_for_ref_frame = 0; // avoid twice deleting
                    // DEBUG : (todo - implement copy constructor and assigment operator)

                    // Validate the incoming bitstream's image dimensions.
                    temp = m_Headers.m_SeqParams.GetHeader(sps.seq_parameter_set_id);

                    SetDPBSize();
                    m_pDecodedFramesList->SetDPBSize(m_dpbSize);
                    m_maxDecFrameBuffering = temp->max_dec_frame_buffering ?
                        temp->max_dec_frame_buffering : m_dpbSize;

                    m_pNALSplitter->SetSuggestedSize(CalculateSuggestedSize(&sps));

                    if (!temp->timing_info_present_flag)
                    {
                        temp->num_units_in_tick = 1;
                        temp->time_scale = 30;
                    }
                    if(m_use_external_framerate)
                        temp->time_scale = (Ipp32u)(2*temp->num_units_in_tick / m_local_delta_frame_time);

                    m_local_delta_frame_time = 1 / ((0.5 * temp->time_scale) / temp->num_units_in_tick);

                    if (m_TrickModeSpeed != 1)
                        m_maxDecFrameBuffering = 0;

                    AllocateBuffers(temp, false);
                    if (umcRes != UMC_OK)
                        return umcRes;
                }
                else
                    return UMC_ERR_INVALID_STREAM;
            }
            break;

        case NAL_UT_SPS_EX:
            {
                H264SeqParamSetExtension sps_ex;
                umcRes = bitStream.GetSequenceParamSetExtension(&sps_ex);

                if (umcRes == UMC_OK)
                {
                    H264SeqParamSetExtension * temp = m_Headers.m_SeqExParams.GetHeader(sps_ex.seq_parameter_set_id);
                    if (m_Headers.m_SeqExParams.AddHeader(&sps_ex, false))
                    {
                        void * buf = m_ObjHeap.Allocate ((notifier2<Headers, Ipp32s, void*> *)0);
                        notifier_base * notifier = new(buf) notifier2<Headers, Ipp32s, void*>(&m_Headers, &Headers::Signal, 1, temp);
                        m_DefaultNotifyChain.AddNotifier(notifier);
                    }
                }
                else
                    return UMC_ERR_INVALID_STREAM;
            }
            break;

            // picture parameter set
        case NAL_UT_PPS:
            {
                H264PicParamSet pps;
                // set illegal id
                pps.pic_parameter_set_id = MAX_NUM_PIC_PARAM_SETS;

                // Get id
                umcRes = bitStream.GetPictureParamSetPart1(&pps);
                if (UMC_OK == umcRes)
                {
                    H264SeqParamSet *pRefsps = m_Headers.m_SeqParams.GetHeader(pps.seq_parameter_set_id);

                    if (!pRefsps || pRefsps ->seq_parameter_set_id >= MAX_NUM_SEQ_PARAM_SETS)
                        return UMC_ERR_INVALID_STREAM;

                    // Get rest of pic param set
                    umcRes = bitStream.GetPictureParamSetPart2(&pps, pRefsps);
                    if (UMC_OK == umcRes)
                    {
                        H264PicParamSet * temp = m_Headers.m_PicParams.GetHeader(pps.pic_parameter_set_id);
                        if (m_Headers.m_PicParams.AddHeader(&pps, false))
                        {
                            void * buf = m_ObjHeap.Allocate ((notifier2<Headers, Ipp32s, void*> *)0);
                            notifier_base * notifier = new(buf) notifier2<Headers, Ipp32s, void*>(&m_Headers, &Headers::Signal, 2, temp);
                            m_DefaultNotifyChain.AddNotifier(notifier);
                        }

                        pps.SliceGroupInfo.t3.pSliceGroupIDMap = 0; // avoid twice deleting
                        // DEBUG : (todo - implement copy constructor and assigment operator)

                        m_Headers.m_SeqParams.SetCurrentID(pps.seq_parameter_set_id);
                    }
                }
            }
            break;

        default:
            break;
        }
    }
    catch(const h264_exception & ex)
    {
        return ex.GetStatus();
    }
    catch(...)
    {
        return UMC_ERR_INVALID_STREAM;
    }

    return UMC_OK;

} // Status TaskSupplier::DecodeHeaders(MediaDataEx::_MediaDataEx *pSource, H264MemoryPiece * pMem)

void TaskSupplier::OnSequenceHeader(const H264SeqParamSet * )
{

}

void TaskSupplier::OnPictureHeader(const H264PicParamSet * )
{
}

//////////////////////////////////////////////////////////////////////////////
// ProcessFrameNumGap
//
// A non-sequential frame_num has been detected. If the sequence parameter
// set field gaps_in_frame_num_value_allowed_flag is non-zero then the gap
// is OK and "non-existing" frames will be created to correctly fill the
// gap. Otherwise the gap is an indication of lost frames and the need to
// handle in a reasonable way.
//////////////////////////////////////////////////////////////////////////////
Status TaskSupplier::ProcessFrameNumGap(H264Slice *slice, Ipp32s field)
{
    Status umcRes = UMC_OK;

    Ipp32s frameNumGap = DetectFrameNumGap(slice);

    if (!frameNumGap)
        return UMC_OK;

    if (frameNumGap > m_dpbSize)
    {
        frameNumGap = m_dpbSize;
    }

    H264SliceHeader *sliceHeader = slice->GetSliceHeader();
    H264SeqParamSet* sps = slice->GetSeqParam();

    Ipp32s uMaxFrameNum = (1<<sps->log2_max_frame_num);

    // Fill the frame_num gap with non-existing frames. For each missing
    // frame:
    //  - allocate a frame
    //  - set frame num and pic num
    //  - update FrameNumWrap for all reference frames
    //  - use sliding window frame marking to free oldest reference
    //  - mark the frame as short-term reference
    // The picture part of the generated frames is unimportant -- it will
    // not be used for reference.

    // set to first missing frame. Note that if frame number wrapped during
    // the gap, the first missing frame_num could be larger than the
    // current frame_num. If that happened, FrameNumGap will be negative.
    //VM_ASSERT((Ipp32s)sliceHeader->frame_num > frameNumGap);
    Ipp32s frame_num = sliceHeader->frame_num - frameNumGap;

    while ((frame_num != sliceHeader->frame_num) &&
        (umcRes == UMC_OK))
    {
        // allocate a frame
        // Traverse list for next disposable frame
        H264DecoderFrame *pFrame = GetFreeFrame();

        // Did we find one?
        if (!pFrame)
        {
            return UMC_ERR_NOT_ENOUGH_BUFFER;
        }

        Status res = InitFreeFrame(pFrame, slice);

        if (res != UMC_OK)
        {
            return UMC_ERR_NOT_ENOUGH_BUFFER;
        }

        frameNumGap--;

        if (sps->pic_order_cnt_type != 0)
        {
            Ipp32s tmp1 = sliceHeader->delta_pic_order_cnt[0];
            Ipp32s tmp2 = sliceHeader->delta_pic_order_cnt[1];
            sliceHeader->delta_pic_order_cnt[0] = sliceHeader->delta_pic_order_cnt[1] = 0;

            DecodePictureOrderCount(slice, frame_num);

            sliceHeader->delta_pic_order_cnt[0] = tmp1;
            sliceHeader->delta_pic_order_cnt[1] = tmp2;
        }

        // Set frame num and pic num for the missing frame
        pFrame->setFrameNum(frame_num);
        m_PrevFrameRefNum = frame_num;
        m_FrameNum = frame_num;

        if (sliceHeader->field_pic_flag)
        {
            pFrame->setPicOrderCnt(m_PicOrderCnt,0);
            pFrame->setPicOrderCnt(m_PicOrderCnt,1);
        }
        else
        {
            pFrame->setPicOrderCnt(m_TopFieldPOC, 0);
            pFrame->setPicOrderCnt(m_BottomFieldPOC, 1);
        }

        if (sliceHeader->field_pic_flag == 0)
        {
            pFrame->setPicNum(frame_num, 0);
        }
        else
        {
            pFrame->setPicNum(frame_num*2+1, 0);
            pFrame->setPicNum(frame_num*2+1, 1);
        }

        // Update frameNumWrap and picNum for all decoded frames

        H264DecoderFrame *pFrm;
        H264DecoderFrame * pHead = m_pDecodedFramesList->head();
        for (pFrm = pHead; pFrm; pFrm = pFrm->future())
        {
            // TBD: modify for fields
            pFrm->UpdateFrameNumWrap(frame_num,
                uMaxFrameNum,
                pFrame->m_PictureStructureForRef+
                pFrame->m_bottom_field_flag[field]);
        }

        // sliding window ref pic marking
        SlideWindow(slice, 0);

        pFrame->SetisShortTermRef(0);
        pFrame->SetisShortTermRef(1);

        // next missing frame
        frame_num++;
        if (frame_num >= uMaxFrameNum)
            frame_num = 0;

        // Set current as displayable and was outputted.
        pFrame->SetisDisplayable();
        pFrame->DecrementBusyState(); // == decoded
        pFrame->DecrementBusyState(); // == decoded
        pFrame->SetSkipped(true);
        pFrame->SetFrameExistFlag(false);
        //pFrame->OnDecodingCompleted();

        return UMC_OK;
    }   // while

    return m_pDecodedFramesList->IsDisposableExist() ? UMC_OK : UMC_ERR_NOT_ENOUGH_DATA;
}   // ProcessFrameNumGap

void TaskSupplier::SetDPBSize()
{
    const H264SeqParamSet* sps = m_Headers.m_SeqParams.GetCurrentHeader();

    if (!sps)
        return;

    m_dpbSize = CalculateDPBSize(sps->level_idc, sps->frame_width_in_mbs, sps->frame_height_in_mbs);
    m_dpbSize = IPP_MAX((Ipp32u)m_dpbSize, sps->num_ref_frames);
}

bool TaskSupplier::GetFrameToDisplay(MediaData *dst, bool force)
{
    // Perform output color conversion and video effects, if we didn't
    // already write our output to the application's buffer.
    VideoData *pVData = DynamicCast<VideoData, MediaData> (dst);
    if (!pVData)
        return false;

    m_pLastDisplayed = 0;

    H264DecoderFrame * pFrame = 0;

    do
    {
        pFrame = GetFrameToDisplayInternal(force);
        if (!pFrame || !pFrame->IsDecoded())
        {
            return false;
        }

        PostProcessDisplayFrame(dst, pFrame);

        if (pFrame->IsSkipped())
        {
            pFrame->setWasOutputted();
            pFrame->setWasDisplayed();
        }
    } while (pFrame->IsSkipped());

    m_pLastDisplayed = pFrame;
    pVData->m_iInvalid = pFrame->GetError();

    VideoData data;

    InitColorConverter(pFrame, &data, 0);

    if (m_pPostProcessing->GetFrame(&data, pVData) != UMC_OK)
    {
        pFrame->setWasOutputted();
        pFrame->setWasDisplayed();
        return false;
    }

    pVData->SetDataSize(pVData->GetMappingSize());
    pFrame->setWasOutputted();
    pFrame->setWasDisplayed();

    return true;
}

bool TaskSupplier::IsWantToShowFrame(bool force)
{
    if (m_pDecodedFramesList->countNumDisplayable() > m_maxDecFrameBuffering ||
        force)
    {
        H264DecoderFrame * pTmp = m_pDecodedFramesList->findOldestDisplayable(m_dpbSize);
        return !!pTmp;
    }

    return false;
}

void TaskSupplier::PostProcessDisplayFrame(MediaData *dst, H264DecoderFrame *pFrame)
{
    if (!pFrame || pFrame->post_procces_complete)
        return;

    if (pFrame->m_dFrameTime > -1.0)
        m_local_frame_time = pFrame->m_dFrameTime;
    else
        pFrame->m_dFrameTime = m_local_frame_time;

    m_local_frame_time += m_local_delta_frame_time;
    pFrame->post_procces_complete = true;

    dst->m_fPTSStart = pFrame->m_dFrameTime;
}

H264DecoderFrame *TaskSupplier::GetFrameToDisplayInternal(bool force)
{
    H264DecoderFrame *pFrame = GetAnyFrameToDisplay(force);
    return pFrame;
}

H264DecoderFrame *TaskSupplier::GetAnyFrameToDisplay(bool force)
{
    for (;;)
    {
        // show oldest frame
        if (m_pDecodedFramesList->countNumDisplayable() > m_maxDecFrameBuffering || force)
        {
            H264DecoderFrame *pTmp;

            if (m_maxDecFrameBuffering)
            {
                pTmp = m_pDecodedFramesList->findOldestDisplayable(m_dpbSize);
            }
            else
            {
                pTmp = m_pDecodedFramesList->findFirstDisplayable();
            }

            if (pTmp)
            {
                if (!pTmp->IsFrameExist())
                {
                    pTmp->setWasOutputted();
                    pTmp->setWasDisplayed();
                    continue;
                }
            }

            return pTmp;
        }
        else
            return 0;
    }
}

void TaskSupplier::SetMBMap(H264DecoderFrame *frame)
{
    Ipp32u mbnum, i;
    Ipp32s prevMB;
    Ipp32u uNumMBCols;
    Ipp32u uNumMBRows;
    Ipp32u uNumSliceGroups;
    Ipp32u uNumMapUnits;
    H264Slice * slice = frame->GetAU(0)->GetAnySlice();
    H264PicParamSet *pps = slice->GetPicParam();
    H264SliceHeader * sliceHeader = slice->GetSliceHeader();
    Ipp32s PrevMapUnit[MAX_NUM_SLICE_GROUPS];
    Ipp32s SliceGroup, FirstMB;
    Ipp8u *pMap = NULL;
    bool bSetFromMap = false;

    uNumMBCols = slice->GetSeqParam()->frame_width_in_mbs;
    uNumMBRows = slice->GetSeqParam()->frame_height_in_mbs;

    FirstMB = 0;
    // TBD: update for fields:
    uNumMapUnits = uNumMBCols*uNumMBRows;
    uNumSliceGroups = pps->num_slice_groups;

    VM_ASSERT(frame->m_iResourceNumber >= 0);

    Ipp32s resource = frame->m_iResourceNumber;

    Ipp32s additionalTable = resource + 1;

    if (uNumSliceGroups == 1)
    {
        GetMBInfo(resource).active_next_mb_table = next_mb_tables[0];
    }
    else
    {
        Ipp32s times = frame->m_PictureStructureForDec < FRM_STRUCTURE ? 2 : 1;
        for (Ipp32s j = 0; j < times; j++)
        {
            if (frame->m_PictureStructureForDec < FRM_STRUCTURE)
            {
                if (j)
                {
                    FirstMB = frame->totalMBs;
                    uNumMapUnits <<= 1;
                }
                else
                {
                    uNumMapUnits >>= 1;
                    uNumMBRows >>= 1;
                }
            }

            //since main profile doesn't allow slice groups to be >1 and in baseline no fields (or mbaffs) allowed
            //the following memset is ok.
            // > 1 slice group
            switch (pps->SliceGroupInfo.slice_group_map_type)
            {
            case 0:
                {
                    // interleaved slice groups: run_length for each slice group,
                    // repeated until all MB's are assigned to a slice group
                    Ipp32u NumThisGroup;

                    // Init PrevMapUnit to -1 (none), for first unit of each slice group
                    for (i=0; i<uNumSliceGroups; i++)
                        PrevMapUnit[i] = -1;

                    SliceGroup = 0;
                    NumThisGroup = 0;
                    prevMB = -1;
                    for (mbnum = FirstMB; mbnum < uNumMapUnits; mbnum++)
                    {
                        if (NumThisGroup == pps->SliceGroupInfo.run_length[SliceGroup])
                        {
                            // new slice group
                            PrevMapUnit[SliceGroup] = prevMB;
                            SliceGroup++;
                            if (SliceGroup == (Ipp32s)uNumSliceGroups)
                                SliceGroup = 0;
                            prevMB = PrevMapUnit[SliceGroup];
                            NumThisGroup = 0;
                        }
                        if (prevMB >= 0)
                        {
                            // new
                            next_mb_tables[additionalTable][prevMB] = mbnum;
                        }
                        prevMB = mbnum;
                        NumThisGroup++;
                    }
                }
                GetMBInfo(resource).active_next_mb_table = next_mb_tables[additionalTable];
                break;

            case 1:
                // dispersed
                {
                    Ipp32u row, col;

                    // Init PrevMapUnit to -1 (none), for first unit of each slice group
                    for (i=0; i<uNumSliceGroups; i++)
                        PrevMapUnit[i] = -1;

                    mbnum = FirstMB;
                    for (row = 0; row < uNumMBRows; row++)
                    {
                        SliceGroup = ((row * uNumSliceGroups)/2) % uNumSliceGroups;
                        for (col=0; col<uNumMBCols; col++)
                        {
                            prevMB = PrevMapUnit[SliceGroup];
                            if (prevMB != -1)
                            {
                                next_mb_tables[additionalTable][prevMB]  = mbnum;
                            }
                            PrevMapUnit[SliceGroup] = mbnum;
                            mbnum++;
                            SliceGroup++;
                            if (SliceGroup == (Ipp32s)uNumSliceGroups)
                                SliceGroup = 0;
                        }    // col
                    }    // row
                }

                GetMBInfo(resource).active_next_mb_table = next_mb_tables[additionalTable];
                break;

            case 2:
                {
                    // foreground + leftover: Slice groups are rectangles, any MB not
                    // in a defined rectangle is in the leftover slice group, a MB within
                    // more than one rectangle is in the lower-numbered slice group.

                    // Two steps:
                    // 1. Set m_pMBMap with slice group for all MBs.
                    // 2. Set nextMB fields of MBInfo from m_pMBMap.

                    Ipp32u RectUpper, RectLeft, RectRight, RectLower;
                    Ipp32u RectRows, RectCols;
                    Ipp32u row, col;

                    // First init all as leftover
                    for (mbnum = FirstMB; mbnum<uNumMapUnits; mbnum++)
                        m_pMBMap[mbnum] = (Ipp8u)(uNumSliceGroups - 1);

                    // Next set those in slice group rectangles, from back to front
                    for (SliceGroup = (Ipp32s)(uNumSliceGroups - 2); SliceGroup >= 0; SliceGroup--)
                    {
                        mbnum = pps->SliceGroupInfo.t1.top_left[SliceGroup];
                        RectUpper = pps->SliceGroupInfo.t1.top_left[SliceGroup] / uNumMBCols;
                        RectLeft = pps->SliceGroupInfo.t1.top_left[SliceGroup] % uNumMBCols;
                        RectLower = pps->SliceGroupInfo.t1.bottom_right[SliceGroup] / uNumMBCols;
                        RectRight = pps->SliceGroupInfo.t1.bottom_right[SliceGroup] % uNumMBCols;
                        RectRows = RectLower - RectUpper + 1;
                        RectCols = RectRight - RectLeft + 1;

                        for (row = 0; row < RectRows; row++)
                        {
                            for (col=0; col < RectCols; col++)
                            {
                                m_pMBMap[mbnum + col] = (Ipp8u)SliceGroup;
                            }    // col

                            mbnum += uNumMBCols;
                        }    // row
                    }    // SliceGroup
                }
                GetMBInfo(resource).active_next_mb_table = next_mb_tables[additionalTable];

                pMap = m_pMBMap;
                bSetFromMap = true;        // to cause step 2 to occur below
                break;
            case 3:
                {
                    // Box-out, clockwise or counterclockwise. Result is two slice groups,
                    // group 0 included by the box, group 1 excluded.

                    // Two steps:
                    // 1. Set m_pMBMap with slice group for all MBs.
                    // 2. Set nextMB fields of MBInfo from m_pMBMap.

                    Ipp32u x, y, leftBound, topBound, rightBound, bottomBound;
                    Ipp32s xDir, yDir;
                    Ipp32u mba;
                    Ipp32u dir_flag = pps->SliceGroupInfo.t2.slice_group_change_direction_flag;
                    Ipp32u uNumInGroup0;
                    Ipp32u uGroup0Count = 0;

                    SliceGroup = 1;        // excluded group

                    uNumInGroup0 = IPP_MIN(pps->SliceGroupInfo.t2.slice_group_change_rate *
                                    sliceHeader->slice_group_change_cycle, uNumMapUnits - FirstMB);

                    uNumInGroup0 = IPP_MIN(uNumInGroup0, uNumMapUnits);

                    if (uNumInGroup0 == uNumMapUnits)
                    {
                        // all units in group 0
                        SliceGroup = 0;
                        uGroup0Count = uNumInGroup0;    // to skip box out
                    }

                    // First init all
                    for (mbnum = FirstMB; mbnum < uNumMapUnits; mbnum++)
                        m_pMBMap[mbnum] = (Ipp8u)SliceGroup;

                    // Next the box-out algorithm to change included MBs to group 0

                    // start at center
                    x = (uNumMBCols - dir_flag)>>1;
                    y = (uNumMBRows - dir_flag)>>1;
                    leftBound = rightBound = x;
                    topBound = bottomBound = y;
                    xDir = dir_flag - 1;
                    yDir = dir_flag;

                    // expand out from center until group 0 includes the required number
                    // of units
                    while (uGroup0Count < uNumInGroup0)
                    {
                        mba = x + y*uNumMBCols;
                        if (m_pMBMap[mba + FirstMB] == 1)
                        {
                            // add MB to group 0
                            m_pMBMap[mba + FirstMB] = 0;
                            uGroup0Count++;
                        }
                        if (x == leftBound && xDir == -1)
                        {
                            if (leftBound > 0)
                            {
                                leftBound--;
                                x--;
                            }
                            xDir = 0;
                            yDir = dir_flag*2 - 1;
                        }
                        else if (x == rightBound && xDir == 1)
                        {
                            if (rightBound < uNumMBCols - 1)
                            {
                                rightBound++;
                                x++;
                            }
                            xDir = 0;
                            yDir = 1 - dir_flag*2;
                        }
                        else if (y == topBound && yDir == -1)
                        {
                            if (topBound > 0)
                            {
                                topBound--;
                                y--;
                            }
                            xDir = 1 - dir_flag*2;
                            yDir = 0;
                        }
                        else if (y == bottomBound && yDir == 1)
                        {
                            if (bottomBound < uNumMBRows - 1)
                            {
                                bottomBound++;
                                y++;
                            }
                            xDir = dir_flag*2 - 1;
                            yDir = 0;
                        }
                        else
                        {
                            x += xDir;
                            y += yDir;
                        }
                    }    // while
                }

                GetMBInfo(resource).active_next_mb_table = next_mb_tables[additionalTable];

                pMap = m_pMBMap;
                bSetFromMap = true;        // to cause step 2 to occur below
                break;
            case 4:
                // raster-scan: 2 slice groups. Both groups contain units ordered
                // by raster-scan, so initializing nextMB for simple raster-scan
                // ordering is all that is required.
                GetMBInfo(resource).active_next_mb_table = next_mb_tables[0];
                break;
            case 5:
                // wipe: 2 slice groups, the vertical version of case 4. Init
                // nextMB by processing the 2 groups as two rectangles (left
                // and right); to allow for the break between groups occurring
                // not at a column boundary, the rectangles also have an upper
                // and lower half (same heights both rectangles) that may vary
                // in width from one another by one macroblock, for example:
                //  L L L L R R R R R
                //  L L L L R R R R R
                //  L L L R R R R R R
                //  L L L R R R R R R
                {
                    Ipp32u uNumInGroup0;
                    Ipp32u uNumInLGroup;
                    Ipp32s SGWidth;
                    Ipp32s NumUpperRows;
                    Ipp32s NumRows;
                    Ipp32s row, col;
                    Ipp32s iMBNum;

                    uNumInGroup0 = IPP_MIN(pps->SliceGroupInfo.t2.slice_group_change_rate *
                                    sliceHeader->slice_group_change_cycle, uNumMapUnits - FirstMB);
                    if (uNumInGroup0 >= uNumMapUnits)
                    {
                        // all units in group 0
                        uNumInGroup0 = uNumMapUnits;
                    }
                    if (pps->SliceGroupInfo.t2.slice_group_change_direction_flag == 0)
                        uNumInLGroup = uNumInGroup0;
                    else
                        uNumInLGroup = uNumMapUnits - uNumInGroup0;

                    if (uNumInLGroup > 0)
                    {
                        // left group
                        NumUpperRows = uNumInLGroup % uNumMBRows;
                        NumRows = uNumMBRows;
                        SGWidth = uNumInLGroup / uNumMBRows;        // lower width, left
                        if (NumUpperRows)
                        {
                            SGWidth++;            // upper width, left

                            // zero-width lower case
                            if (SGWidth == 1)
                                NumRows = NumUpperRows;
                        }
                        iMBNum = FirstMB;

                        for (row = 0; row < NumRows; row++)
                        {
                            col = 0;
                            while (col < SGWidth-1)
                            {
                                next_mb_tables[additionalTable][iMBNum + col] = (iMBNum + col + 1);
                                col++;
                            }    // col

                            // next for last MB on row
                            next_mb_tables[additionalTable][iMBNum + col] = (iMBNum + uNumMBCols);
                            iMBNum += uNumMBCols;

                            // time to switch to lower?
                            NumUpperRows--;
                            if (NumUpperRows == 0)
                                SGWidth--;
                        }    // row
                    }    // left group

                    if (uNumInLGroup < uNumMapUnits)
                    {
                        // right group
                        NumUpperRows = uNumInLGroup % uNumMBRows;
                        NumRows = uNumMBRows;
                        // lower width, right:
                        SGWidth = uNumMBCols - uNumInLGroup / uNumMBRows;
                        if (NumUpperRows)
                            SGWidth--;            // upper width, right
                        if (SGWidth > 0)
                        {
                            // first MB is on first row
                            iMBNum = uNumMBCols - SGWidth;
                        }
                        else
                        {
                            // zero-width upper case
                            SGWidth = 1;
                            iMBNum = (NumUpperRows + 1)*uNumMBCols - 1;
                            NumRows = uNumMBRows - NumUpperRows;
                            NumUpperRows = 0;
                        }

                        for (row = 0; row < NumRows; row++)
                        {
                            col = 0;
                            while (col < SGWidth-1)
                            {
                                next_mb_tables[additionalTable][iMBNum + col] = (iMBNum + col + 1);
                                col++;
                            }    // col

                            // next for last MB on row
                            next_mb_tables[additionalTable][iMBNum + col] = (iMBNum + uNumMBCols);

                            // time to switch to lower?
                            NumUpperRows--;
                            if (NumUpperRows == 0)
                            {
                                SGWidth++;
                                // fix next for last MB on row
                                next_mb_tables[additionalTable][iMBNum + col]= (iMBNum+uNumMBCols - 1);
                                iMBNum--;
                            }

                            iMBNum += uNumMBCols;

                        }    // row
                    }    // right group
                }

                GetMBInfo(resource).active_next_mb_table = next_mb_tables[additionalTable];
                break;
            case 6:
                // explicit map read from bitstream, contains slice group id for
                // each map unit
                GetMBInfo(resource).active_next_mb_table = next_mb_tables[additionalTable];
                pMap = pps->SliceGroupInfo.t3.pSliceGroupIDMap;
                bSetFromMap = true;
                break;
            default:
                // can't happen
                VM_ASSERT(0);

            }    // switch map type

            if (bSetFromMap)
            {
                // Set the nextMB MBInfo field of a set of macroblocks depending upon
                // the slice group information in the map, to create an ordered
                // (raster-scan) linked list of MBs for each slice group. The first MB
                // of each group will be identified by the first slice header for each
                // group.

                // For each map unit get assigned slice group from the map
                // For all except the first unit in each
                // slice group, set the next field of the previous MB in that
                // slice group.

                // Init PrevMapUnit to -1 (none), for first unit of each slice group
                for (i=0; i<uNumSliceGroups; i++)
                    PrevMapUnit[i] = -1;

                for (mbnum=FirstMB; mbnum<uNumMapUnits; mbnum++)
                {
                    SliceGroup = pMap[mbnum];
                    prevMB = PrevMapUnit[SliceGroup];
                    if (prevMB != -1)
                    {
                        next_mb_tables[additionalTable][prevMB] = mbnum;
                    }
                    PrevMapUnit[SliceGroup] = mbnum;
                }
            }
        }    // >1 slice group
    }
}    // setMBMap

void TaskSupplier::SlideWindow(H264Slice * pSlice, Ipp32s field_index)
{
    Ipp32u NumShortTermRefs, NumLongTermRefs;
    H264SeqParamSet* sps = pSlice->GetSeqParam();

    // find out how many active reference frames currently in decoded
    // frames buffer
    m_pDecodedFramesList->countActiveRefs(NumShortTermRefs, NumLongTermRefs);
    while (NumShortTermRefs > 0 &&
        (NumShortTermRefs + NumLongTermRefs >= sps->num_ref_frames) &&
        !field_index)
    {
        // mark oldest short-term reference as unused
        VM_ASSERT(NumShortTermRefs > 0);

        H264DecoderFrame * pFrame = m_pDecodedFramesList->freeOldestShortTermRef();

        if (!pFrame)
            break;

        NumShortTermRefs--;

        OnSlideWindow(pFrame, &m_DefaultNotifyChain);
    }
}

//////////////////////////////////////////////////////////////////////////////
// updateRefPicMarking
//  Called at the completion of decoding a frame to update the marking of the
//  reference pictures in the decoded frames buffer.
//////////////////////////////////////////////////////////////////////////////
Status TaskSupplier::UpdateRefPicMarking(H264DecoderFrame * pFrame, H264Slice * pSlice, Ipp32s field_index)
{
    Status umcRes = UMC_OK;
    Ipp32u arpmmf_idx;
    Ipp32s picNum;
    Ipp32s LongTermFrameIdx;
    bool bCurrentisST = true;

    H264SliceHeader const * sliceHeader = pSlice->GetSliceHeader();

    if (pFrame->m_bIDRFlag)
    {
        // mark all reference pictures as unused
        m_pDecodedFramesList->removeAllRef(&m_DefaultNotifyChain);

        if (sliceHeader->long_term_reference_flag)
        {
            pFrame->SetisLongTermRef(field_index);
            pFrame->setLongTermFrameIdx(0);
        }
        else
        {
            pFrame->SetisShortTermRef(field_index);
        }

        bCurrentisST = false;
    }
    else
    {
        AdaptiveMarkingInfo* pAdaptiveMarkingInfo = pSlice->GetAdaptiveMarkingInfo();
        // adaptive ref pic marking
        if (pAdaptiveMarkingInfo && pAdaptiveMarkingInfo->num_entries > 0)
        {
            for (arpmmf_idx=0; arpmmf_idx<pAdaptiveMarkingInfo->num_entries;
                 arpmmf_idx++)
            {
                H264DecoderFrame * pRefFrame = 0;

                switch (pAdaptiveMarkingInfo->mmco[arpmmf_idx])
                {
                case 1:
                    // mark a short-term picture as unused for reference
                    // Value is difference_of_pic_nums_minus1
                    picNum = pFrame->PicNum(field_index) -
                        (pAdaptiveMarkingInfo->value[arpmmf_idx*2] + 1);
                    pRefFrame = m_pDecodedFramesList->freeShortTermRef(picNum);
                    break;
                case 2:
                    // mark a long-term picture as unused for reference
                    // value is long_term_pic_num
                    picNum = pAdaptiveMarkingInfo->value[arpmmf_idx*2];
                    pRefFrame = m_pDecodedFramesList->freeLongTermRef(picNum);
                    break;
                case 3:
                    // Assign a long-term frame idx to a short-term picture
                    // Value is difference_of_pic_nums_minus1 followed by
                    // long_term_frame_idx. Only this case uses 2 value entries.
                    picNum = pFrame->PicNum(field_index) -
                        (pAdaptiveMarkingInfo->value[arpmmf_idx*2] + 1);
                    LongTermFrameIdx = pAdaptiveMarkingInfo->value[arpmmf_idx*2+1];

                    pRefFrame = m_pDecodedFramesList->findShortTermPic(picNum, 0);

                    // First free any existing LT reference with the LT idx
                    pRefFrame = m_pDecodedFramesList->freeLongTermRefIdx(LongTermFrameIdx, pRefFrame);

                    m_pDecodedFramesList->changeSTtoLTRef(picNum, LongTermFrameIdx);
                    break;
                case 4:
                    {
                    // Specify max long term frame idx
                    // Value is max_long_term_frame_idx_plus1
                    // Set to "no long-term frame indices" (-1) when value == 0.
                    Ipp32s m_MaxLongTermFrameIdx = pAdaptiveMarkingInfo->value[arpmmf_idx*2] - 1;

                    // Mark any long-term reference frames with a larger LT idx
                    // as unused for reference.
                    m_pDecodedFramesList->freeOldLongTermRef(m_MaxLongTermFrameIdx, &m_DefaultNotifyChain);
                    }
                    break;
                case 5:
                    // Mark all as unused for reference
                    // no value
                    m_WaitForIDR = false;
                    m_pDecodedFramesList->removeAllRef(&m_DefaultNotifyChain);
                    m_pDecodedFramesList->IncreaseRefPicListResetCount(pFrame);
                    // set "previous" picture order count vars for future

                    if (pFrame->m_PictureStructureForDec < 0)
                    {
                        pFrame->setPicOrderCnt(0, field_index);
                        pFrame->setPicNum(0, field_index);
                    }
                    else
                    {
                        Ipp32s poc = pFrame->PicOrderCnt(0, 3);
                        pFrame->setPicOrderCnt(pFrame->PicOrderCnt(0, 1) - poc, 0);
                        pFrame->setPicOrderCnt(pFrame->PicOrderCnt(1, 1) - poc, 1);
                        pFrame->setPicNum(0, 0);
                        pFrame->setPicNum(0, 1);
                    }

                    POCDecoder::Reset(0);
                    // set frame_num to zero for this picture, for correct
                    // FrameNumWrap
                    pFrame->setFrameNum(0);
                    break;
                case 6:
                    // Assign long term frame idx to current picture
                    // Value is long_term_frame_idx
                    LongTermFrameIdx = pAdaptiveMarkingInfo->value[arpmmf_idx*2];

                    // First free any existing LT reference with the LT idx
                    pRefFrame = m_pDecodedFramesList->freeLongTermRefIdx(LongTermFrameIdx, pFrame);

                    // Mark current
                    pFrame->SetisLongTermRef(field_index);
                    pFrame->setLongTermFrameIdx(LongTermFrameIdx);
                    bCurrentisST = false;
                    break;
                case 0:
                default:
                    // invalid mmco command in bitstream
                    VM_ASSERT(0);
                    umcRes = UMC_ERR_INVALID_STREAM;
                }    // switch

                OnSlideWindow(pRefFrame, &m_DefaultNotifyChain);
            }    // for arpmmf_idx
        }
    }    // not IDR picture

    if (bCurrentisST)
    { // set current as
        if (sliceHeader->field_pic_flag && field_index)
        {
        }
        else
        {
            SlideWindow(pSlice, field_index);
        }

        pFrame->SetisShortTermRef(field_index);
    }

    return umcRes;
}    // updateRefPicMarking

void TaskSupplier::PreventDPBFullness()
{
    try
    {
        for (;;)
        {
            // force Display or ... delete long term
            const H264SeqParamSet* sps = m_Headers.m_SeqParams.GetCurrentHeader();
            if (sps)
            {
                Ipp32u NumShortTermRefs, NumLongTermRefs;

                // find out how many active reference frames currently in decoded
                // frames buffer
                m_pDecodedFramesList->countActiveRefs(NumShortTermRefs, NumLongTermRefs);

                if (NumLongTermRefs == sps->num_ref_frames)
                {
                    H264DecoderFrame *pFrame = m_pDecodedFramesList->findOldestLongTermRef();
                    if (pFrame)
                    {
                        pFrame->unSetisLongTermRef(0);
                        pFrame->unSetisLongTermRef(1);
                        pFrame->Reset();
                    }
                }

                if (m_pDecodedFramesList->IsDisposableExist())
                    break;

                while (NumShortTermRefs > 0 &&
                    (NumShortTermRefs + NumLongTermRefs >= sps->num_ref_frames))
                {
                    H264DecoderFrame * pFrame = m_pDecodedFramesList->freeOldestShortTermRef();

                    if (!pFrame)
                        break;

                    NumShortTermRefs--;
                };

                if (m_pDecodedFramesList->IsDisposableExist())
                    break;
            }

            H264DecoderFrame *pCurr = m_pDecodedFramesList->head();
            while (pCurr)
            {
                if (pCurr->IsDecoded())
                {
                    pCurr->SetBusyState(0);
                }

                pCurr = pCurr->future();
            }

            break;
        }
    }
    catch(...) {}

    if (!m_pDecodedFramesList->IsDisposableExist())
        AfterErrorRestore();
}

Status TaskSupplier::CompleteDecodedFrames(H264DecoderFrame ** decoded)
{
    bool existCompleted = false;

    if (decoded)
    {
        *decoded = 0;
    }

    for (;;) //add all ready to decoding
    {
        bool isOneToAdd = true;
        H264DecoderFrame * frameToAdd = 0;

        for (H264DecoderFrame * frame = m_pDecodedFramesList->head(); frame; frame = frame->future())
        {
            if (frame->IsFrameExist() && !frame->IsDecoded())
            {
                if (!frame->IsDecodingStarted())// && pFrame->IsFullFrame())
                {
                    if (frameToAdd)
                    {
                        isOneToAdd = false;
                        if (frameToAdd->m_UID < frame->m_UID) // add first with min UID
                            continue;
                    }

                    frameToAdd = frame;
                }

                if (!frame->IsDecodingCompleted())
                {
                    continue;
                }

                frame->OnDecodingCompleted();
                existCompleted = true;
            }
        }

        if (frameToAdd)
        {
            if (m_pTaskBroker->AddFrameToDecoding(frameToAdd))
            {
                if (decoded)
                {
                    *decoded = frameToAdd;
                }
            }
            else
                break;
        }

        if (isOneToAdd)
            break;
    }

    return existCompleted ? UMC_OK : UMC_ERR_NOT_ENOUGH_DATA;
}

Status TaskSupplier::AddSource(MediaData * pSource, MediaData *dst)
{
    Status umcRes = UMC_OK;

    umcRes = AddOneFrame(pSource, dst); // construct frame

    if (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes)
    {
        if (m_pDecodedFramesList->IsDisposableExist())
        {
            return UMC_WRN_INFO_NOT_READY;
        }

        if (m_pDecodedFramesList->IsAlmostDisposableExist())
        {
            return UMC_WRN_INFO_NOT_READY;
        }

        if (!m_pTaskBroker->IsEnoughForStartDecoding(true))
        {
            if (CompleteDecodedFrames(0) == UMC_OK)
                return UMC_WRN_INFO_NOT_READY;

            if (!m_DefaultNotifyChain.IsEmpty())
            {
                m_DefaultNotifyChain.Notify();
                return UMC_WRN_INFO_NOT_READY;
            }

            if (GetFrameToDisplayInternal(true))
                return UMC_ERR_NOT_ENOUGH_BUFFER;

            PreventDPBFullness();
            return UMC_WRN_INFO_NOT_READY;
        }

        return UMC_WRN_INFO_NOT_READY;
    }

    return umcRes;
}

Status TaskSupplier::GetFrame(MediaData * pSource, MediaData *dst)
{
    Status umcRes = AddSource(pSource, dst);

    if (umcRes == UMC_WRN_REPOSITION_INPROGRESS)
        umcRes = UMC_OK;

    if (UMC_OK != umcRes && UMC_ERR_SYNC != umcRes && UMC_ERR_NOT_ENOUGH_BUFFER != umcRes && UMC_ERR_NOT_ENOUGH_DATA != umcRes && umcRes != UMC_WRN_INFO_NOT_READY)
        return umcRes;

    if (!dst)
        return umcRes;

    bool force = (umcRes == UMC_WRN_INFO_NOT_READY) || (umcRes == UMC_ERR_NOT_ENOUGH_BUFFER) || !pSource;

    if (umcRes == UMC_ERR_NOT_ENOUGH_BUFFER)
        return UMC_ERR_NOT_ENOUGH_BUFFER;

    if (!pSource)
    {
        do
        {
            umcRes = RunDecoding(force);
        } while (umcRes == UMC_OK);

        return umcRes;
    }
    else
    {
        return RunDecoding(force);
    }
}

Status TaskSupplier::AddOneFrame(MediaData * pSource, MediaData *dst)
{
    Status umsRes = UMC_OK;

    m_pLastFullFrame = 0;

    if (m_pLastSlice)
    {
        Status sts = AddSlice(m_pLastSlice, pSource);
        if (sts == UMC_ERR_NOT_ENOUGH_BUFFER)
        {
            return sts;
        }

        if (sts == UMC_OK)
            return sts;
    }

    bool is_header_readed = false;

    do
    {
        if (!dst && is_header_readed)
        {
            Ipp32s iCode = m_pNALSplitter->CheckNalUnitType(pSource);
            switch (iCode)
            {
            case NAL_UT_IDR_SLICE:
            case NAL_UT_SLICE:
            case NAL_UT_AUXILIARY:

            case NAL_UT_DPA: //ignore it
            case NAL_UT_DPB:
            case NAL_UT_DPC:

            case NAL_UT_SEI:
                return UMC_OK;
            }
        }

        MediaDataEx *nalUnit = m_pNALSplitter->GetNalUnits(pSource);

        if (!nalUnit && pSource)
        {
            Ipp32u flags = pSource->m_iFlags;

            if (!(flags & MDF_NOT_FULL_FRAME))
            {
                VM_ASSERT(!m_pLastSlice);
                return AddSlice(0, 0);
            }

            return is_header_readed & !dst ? UMC_OK : UMC_ERR_SYNC;
        }

        if (!nalUnit)
        {
            if (!pSource)
                return AddSlice(0, 0);

            return UMC_ERR_NOT_ENOUGH_DATA;
        }

        MediaDataEx::_MediaDataEx* pMediaDataEx = nalUnit->GetExData();

        for (Ipp32s i = 0; i < (Ipp32s)pMediaDataEx->count; i++, pMediaDataEx->index ++)
        {
            if (!dst)
            {
                switch ((NAL_Unit_Type)pMediaDataEx->values[i]) // skip data at DecodeHeader mode
                {
                case NAL_UT_IDR_SLICE:
                case NAL_UT_SLICE:
                case NAL_UT_AUXILIARY:

                case NAL_UT_DPA: //ignore it
                case NAL_UT_DPB:
                case NAL_UT_DPC:

                case NAL_UT_SEI:
                    if (!is_header_readed)
                    {
                        continue;
                    }
                    else
                    {
                        return UMC_OK;
                    }
                default:
                    break;
                }
            }

            switch ((NAL_Unit_Type)pMediaDataEx->values[i])
            {
            case NAL_UT_IDR_SLICE:
            case NAL_UT_SLICE:
            case NAL_UT_AUXILIARY:
                {
                H264Slice * pSlice = DecodeSliceHeader(nalUnit);
                if (pSlice)
                {
                    Status sts = AddSlice(pSlice, pSource);
                    if (sts == UMC_ERR_NOT_ENOUGH_BUFFER)
                    {
                        return sts;
                    }

                    if (sts == UMC_OK)
                        return sts;
                }
                }
                break;

            case NAL_UT_SPS:
            case NAL_UT_PPS:
            case NAL_UT_SPS_EX:
            case NAL_UNIT_SUBSET_SPS:
            case NAL_UNIT_PREFIX:
                umsRes = DecodeHeaders(nalUnit);
                if (umsRes != UMC_OK)
                    return umsRes;
                is_header_readed = true;
                break;

            case NAL_UT_SEI:
                DecodeSEI(nalUnit);
                break;
            case NAL_UT_AUD:
                {
                    Status sts = AddSlice(0, pSource);
                    if (sts == UMC_OK)
                    {
                        return sts;
                    }
                }
                break;

            case NAL_UT_DPA: //ignore it
            case NAL_UT_DPB:
            case NAL_UT_DPC:
            case NAL_UT_FD:
            case NAL_UT_UNSPECIFIED:
                break;

            case NAL_END_OF_STREAM:
            case NAL_END_OF_SEQ:
                {
                    Status sts = AddSlice(0, pSource);

                    m_WaitForIDR = true;
                    if (sts == UMC_OK)
                    {
                        return sts;
                    }
                }
                break;

            default:
                break;
            };
        }

    } while ((pSource) && (MINIMAL_DATA_SIZE < pSource->GetDataSize()));

    if (!pSource)
    {
        return AddSlice(0, 0);
    }
    else
    {
        Ipp32u flags = pSource->m_iFlags;

        if (!(flags & MDF_NOT_FULL_FRAME))
        {
            return AddSlice(0, 0);
        }
    }

    return UMC_ERR_NOT_ENOUGH_DATA;
}

static
bool IsFieldOfOneFrame(H264DecoderFrame *pFrame, H264Slice * pSlice1, H264Slice *pSlice2)
{
    if (!pFrame)
        return false;

    if (pFrame && pFrame->GetAU(0)->GetStatus() > H264DecoderFrameInfo::STATUS_NOT_FILLED
        && pFrame->GetAU(1)->GetStatus() > H264DecoderFrameInfo::STATUS_NOT_FILLED)
        return false;

    if ((pSlice1->GetSliceHeader()->nal_ref_idc && !pSlice2->GetSliceHeader()->nal_ref_idc) ||
        (!pSlice1->GetSliceHeader()->nal_ref_idc && pSlice2->GetSliceHeader()->nal_ref_idc))
        return false;

    if (pSlice1->GetSliceHeader()->field_pic_flag != pSlice2->GetSliceHeader()->field_pic_flag)
        return false;

//    if (pSlice1->GetSliceHeader()->frame_num != pSlice2->GetSliceHeader()->frame_num)
  //      return false;

    if (pSlice1->GetSliceHeader()->bottom_field_flag == pSlice2->GetSliceHeader()->bottom_field_flag)
        return false;

    return true;
}

H264Slice * TaskSupplier::DecodeSliceHeader(MediaDataEx *nalUnit)
{
    H264Slice * pSlice = m_ObjHeap.AllocateObject((H264Slice*)0);
    pSlice->SetHeap(&m_ObjHeap);

    H264MemoryPiece memCopy;
    memCopy.SetData(nalUnit);

    H264MemoryPiece * pMemCopy = &memCopy;

    pMemCopy->SetDataSize(nalUnit->GetDataSize());
    pMemCopy->SetTime(nalUnit->m_fPTSStart);

    H264MemoryPiece * pMem = m_Heap.Allocate(nalUnit->GetDataSize() + 8);
    notifier1<H264_Heap, H264MemoryPiece*> memory_leak_preventing(&m_Heap, &H264_Heap::Free, pMem);
    memset(pMem->GetPointer() + nalUnit->GetDataSize(), 0, 8);

    SwapperBase * swapper = m_pNALSplitter->GetSwapper();
    swapper->SwapMemory(pMem, pMemCopy);

    Ipp32s pps_pid = pSlice->RetrievePicParamSetNumber(pMem->GetPointer(), pMem->GetSize());
    if (pps_pid == -1)
    {
        m_ObjHeap.FreeObject(pSlice);
        return 0;
    }

    H264SEIPayLoad * spl = m_Headers.m_SEIParams.GetHeader(SEI_RECOVERY_POINT_TYPE);

    if (m_WaitForIDR)
    {
        if (pSlice->GetSliceHeader()->slice_type != INTRASLICE && !spl)
        {
            m_ObjHeap.FreeObject(pSlice);
            return 0;
        }
    }

    pSlice->m_pPicParamSet = m_Headers.m_PicParams.GetHeader(pps_pid);
    if (!pSlice->m_pPicParamSet)
    {
        m_ObjHeap.FreeObject(pSlice);
        return 0;
    }

    Ipp32s seq_parameter_set_id = pSlice->m_pPicParamSet->seq_parameter_set_id;

    if (pSlice->m_SliceHeader.nal_unit_type == NAL_UNIT_SLICE_SCALABLE)
        seq_parameter_set_id += MAX_NUM_SEQ_PARAM_SETS;

    pSlice->m_pSeqParamSet = m_Headers.m_SeqParams.GetHeader(seq_parameter_set_id);
    if (!pSlice->m_pSeqParamSet)
    {
        m_ObjHeap.FreeObject(pSlice);
        return 0;
    }

    pSlice->m_pSeqParamSetEx = m_Headers.m_SeqExParams.GetHeader(seq_parameter_set_id);
    pSlice->m_pCurrentFrame = m_pCurrentFrame;

    m_Headers.m_SeqParams.SetCurrentID(pSlice->m_pPicParamSet->seq_parameter_set_id);
    m_Headers.m_PicParams.SetCurrentID(pSlice->m_pPicParamSet->pic_parameter_set_id);

    pSlice->m_pSource = pMem;
    pSlice->m_dTime = pMem->GetTime();

    if (!pSlice->Reset(pMem->GetPointer(), pMem->GetDataSize(), m_iThreadNum))
    {
        // should ignored slice
        m_ObjHeap.FreeObject(pSlice);
        return 0;
    }

    if (spl && (pSlice->GetSliceHeader()->slice_type != INTRASLICE))
    {
        m_Headers.Signal(3, spl);
    }

    m_WaitForIDR = false;
    memory_leak_preventing.ClearNotification();

    return pSlice;
}

Status TaskSupplier::AddSlice(H264Slice * pSlice, MediaData * pSource)
{
    m_pLastSlice = 0;

    if (!pSlice) // complete frame
    {
        if (!m_pCurrentFrame)
            return UMC_ERR_NOT_ENOUGH_DATA;

        CompleteFrame(m_pCurrentFrame, m_field_index);

        if (m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE)
        {
            if (!pSource)
            {
                if (ProcessNonPairedField(m_pCurrentFrame))
                {
                    OnFullFrame(m_pCurrentFrame);
                    m_pCurrentFrame = 0;
                    return UMC_OK;
                }
            }

            if (m_field_index)
            {
                OnFullFrame(m_pCurrentFrame);
                m_pCurrentFrame = 0;
                return UMC_OK;
            }

            return UMC_ERR_NOT_ENOUGH_DATA;
        }

        OnFullFrame(m_pCurrentFrame);
        m_pCurrentFrame = 0;
        return UMC_OK;
    }

    H264DecoderFrame * pFrame = m_pCurrentFrame;

    if (pSlice->IsAuxiliary())
    {
        pFrame = UMC::GetAuxiliaryFrame(pFrame);
        pSlice->m_pCurrentFrame = pFrame;

        if (pFrame && pFrame->GetAU(0)->GetSliceCount() == 0)
        {
            pFrame = 0;
        }
    }

    if (pFrame)
    {
        m_field_index = (pFrame->GetAU(1)->GetStatus() != H264DecoderFrameInfo::STATUS_NONE
            && pFrame->GetAU(1)->GetSliceCount() != 0);
        H264Slice * pFirstFrameSlice = pFrame->GetAU(m_field_index)->GetAnySlice();
        VM_ASSERT(pFirstFrameSlice);

        if ((false == IsPictureTheSame(pFirstFrameSlice, pSlice)))
        {
            bool isField = pFirstFrameSlice->IsField();

            CompleteFrame(m_pCurrentFrame, m_field_index);

            if (pSlice->IsField())
            {
                if (IsFieldOfOneFrame(pFrame, pFirstFrameSlice, pSlice))
                {
                    m_field_index = 1;
                    InitFrame(pFrame, pSlice);
                }
                else
                {
                    ProcessNonPairedField(pFrame);
                    m_field_index = 0;

                    OnFullFrame(m_pCurrentFrame);
                    m_pCurrentFrame = 0;
                    m_pLastSlice = pSlice;
                    return UMC_OK;
                }
            }
            else
            {
                if (isField) // only one field
                {
                    ProcessNonPairedField(m_pCurrentFrame);
                }

                m_field_index = 0;

                OnFullFrame(m_pCurrentFrame);
                m_pCurrentFrame = 0;
                m_pLastSlice = pSlice;
                return UMC_OK;
            }
        }
    }
    else
    {
        if (DetectFrameNumGap(pSlice))
        {
            m_pCurrentFrame = 0;
            m_pLastSlice = pSlice;
            return ProcessFrameNumGap(pSlice, m_field_index);
        }

        pFrame = AddFrame(pSlice);
        if (!pFrame)
        {
            m_pCurrentFrame = 0;
            m_pLastSlice = pSlice;
            return UMC_ERR_NOT_ENOUGH_BUFFER;
        }

        m_pCurrentFrame = pFrame;
    }

    // set IDR for auxiliary
    //

    if (m_pCurrentFrame && m_pCurrentFrame->IsAuxiliaryFrame())
    {
        m_pCurrentFrame = m_pCurrentFrame->primary_picture;
    }

    // Init refPicList
    AddSliceToFrame(pFrame, pSlice);

    if (pSlice->GetSliceHeader()->slice_type != INTRASLICE)
    {
        Ipp32u NumShortTermRefs, NumLongTermRefs;
        m_pDecodedFramesList->countActiveRefs(NumShortTermRefs, NumLongTermRefs);
        if (NumShortTermRefs + NumLongTermRefs == 0)
            AddFakeReferenceFrame(pSlice);
    }

    pSlice->UpdateReferenceList(m_pDecodedFramesList);
    return UMC_ERR_NOT_ENOUGH_DATA;
}

void TaskSupplier::AddFakeReferenceFrame(H264Slice * pSlice)
{
    H264DecoderFrame *pFrame = GetFreeFrame();
    if (!pFrame)
        return;

    Status umcRes = InitFreeFrame(pFrame, pSlice);
    if (umcRes != UMC_OK)
    {
        return;
    }

    umcRes = AllocateFrameData(pFrame, pFrame->lumaSize(), pFrame->m_bpp_luma, pFrame->m_bpp_chroma, pFrame->GetColorFormat());
    if (umcRes != UMC_OK)
    {
        return;
    }

    Ipp32s frame_num = pSlice->GetSliceHeader()->frame_num;
    if (pSlice->GetSliceHeader()->field_pic_flag == 0)
    {
        pFrame->setPicNum(frame_num, 0);
    }
    else
    {
        pFrame->setPicNum(frame_num*2+1, 0);
        pFrame->setPicNum(frame_num*2+1, 1);
    }

    pFrame->SetisShortTermRef(0);
    pFrame->SetisShortTermRef(1);

    pFrame->SetSkipped(true);
    pFrame->SetFrameExistFlag(false);
    pFrame->SetisDisplayable();

    pFrame->DefaultFill(2, false);

    H264SliceHeader* sliceHeader = pSlice->GetSliceHeader();
    if (pSlice->GetSeqParam()->pic_order_cnt_type != 0)
    {
        Ipp32s tmp1 = sliceHeader->delta_pic_order_cnt[0];
        Ipp32s tmp2 = sliceHeader->delta_pic_order_cnt[1];
        sliceHeader->delta_pic_order_cnt[0] = sliceHeader->delta_pic_order_cnt[1] = 0;

        DecodePictureOrderCount(pSlice, frame_num);

        sliceHeader->delta_pic_order_cnt[0] = tmp1;
        sliceHeader->delta_pic_order_cnt[1] = tmp2;
    }

    if (sliceHeader->field_pic_flag)
    {
        pFrame->setPicOrderCnt(m_PicOrderCnt,0);
        pFrame->setPicOrderCnt(m_PicOrderCnt,1);
    }
    else
    {
        pFrame->setPicOrderCnt(m_TopFieldPOC, 0);
        pFrame->setPicOrderCnt(m_BottomFieldPOC, 1);
    }

    // mark generated frame as short-term reference
    {
        // reset frame global data
        H264DecoderMacroblockGlobalInfo *pMBInfo = pFrame->m_mbinfo.mbs;
        memset(pMBInfo, 0, pFrame->totalMBs*sizeof(H264DecoderMacroblockGlobalInfo));
    }
}

bool TaskSupplier::ProcessNonPairedField(H264DecoderFrame * pFrame)
{
    if (pFrame && pFrame->GetAU(1)->GetStatus() == H264DecoderFrameInfo::STATUS_NOT_FILLED)
    {
        pFrame->setPicOrderCnt(pFrame->PicOrderCnt(0, 1), 1);
        pFrame->GetAU(1)->SetStatus(H264DecoderFrameInfo::STATUS_NONE);
        pFrame->m_bottom_field_flag[1] = !pFrame->m_bottom_field_flag[0];

        pFrame->m_isShortTermRef[1] = pFrame->m_isShortTermRef[0];
        pFrame->m_isLongTermRef[1] = pFrame->m_isLongTermRef[0];

        H264Slice * pSlice = pFrame->GetAU(0)->GetAnySlice();
        pFrame->setPicNum(pSlice->GetSliceHeader()->frame_num*2 + 1, 1);

        Ipp32s isBottom = pSlice->IsBottomField() ? 0 : 1;
        pFrame->DefaultFill(isBottom, false);
        return true;
    }

    return false;
}

H264DecoderFrame * TaskSupplier::GetLastFullFrame()
{
    return m_pLastFullFrame;
}

void TaskSupplier::OnFullFrame(H264DecoderFrame * pFrame)
{
    m_pLastFullFrame = pFrame;
    pFrame->SetFullFrame(true);
}

void TaskSupplier::CompleteFrame(H264DecoderFrame * pFrame, Ipp32s field)
{
    if (!pFrame)
        return;

    CompleteFrame(UMC::GetAuxiliaryFrame(pFrame), field);

    H264DecoderFrameInfo * slicesInfo = pFrame->GetAU(field);

    if (slicesInfo->GetStatus() > H264DecoderFrameInfo::STATUS_NOT_FILLED)
        return;

    bool is_auxiliary_exist = slicesInfo->GetAnySlice()->GetSeqParamEx() &&
        (slicesInfo->GetAnySlice()->GetSeqParamEx()->aux_format_idc != 0);

    if (is_auxiliary_exist)
    {
        if (!pFrame->IsAuxiliaryFrame())
            DBPUpdate(pFrame, field);
    }
    else
        DBPUpdate(pFrame, field);

    if (!field)
    {
        pFrame->m_iResourceNumber = LocalResources::GetCurrentResourceIndex();
    }

    pFrame->MoveToNotifiersChain(m_DefaultNotifyChain);

    // skipping algorithm
    {
        if ((slicesInfo->IsField() && field || !slicesInfo->IsField()) &&
            IsShouldSkipFrame(pFrame, field))
        {
            if (slicesInfo->IsField())
            {
                pFrame->GetAU(0)->SetStatus(H264DecoderFrameInfo::STATUS_COMPLETED);
                pFrame->GetAU(1)->SetStatus(H264DecoderFrameInfo::STATUS_COMPLETED);
            }
            else
            {
                pFrame->GetAU(0)->SetStatus(H264DecoderFrameInfo::STATUS_COMPLETED);
            }

            pFrame->unSetisShortTermRef(0);
            pFrame->unSetisShortTermRef(1);
            pFrame->unSetisLongTermRef(0);
            pFrame->unSetisLongTermRef(1);
            pFrame->SetSkipped(true);
            pFrame->OnDecodingCompleted();
            return;
        }
        else
        {
            if (IsShouldSkipDeblocking(pFrame, field))
            {
                pFrame->GetAU(field)->SkipDeblocking();
            }
        }
    }

    if (!slicesInfo->GetSlice(0)->IsSliceGroups())
    {
        Ipp32s count = slicesInfo->GetSliceCount();

        H264Slice * pFirstSlice = 0;
        for (Ipp32s j = 0; j < count; j ++)
        {
            H264Slice * pSlice = slicesInfo->GetSlice(j);
            if (!pFirstSlice || pSlice->m_iFirstMB < pFirstSlice->m_iFirstMB)
            {
                pFirstSlice = pSlice;
            }
        }

        if (pFirstSlice->m_iFirstMB)
        {
            m_pSegmentDecoder[0]->RestoreErrorRect(0, pFirstSlice->m_iFirstMB, pFirstSlice);
        }

        for (Ipp32s i = 0; i < count; i ++)
        {
            H264Slice * pCurSlice = slicesInfo->GetSlice(i);

    #define MAX_MB_NUMBER 0x7fffffff

            Ipp32s minFirst = MAX_MB_NUMBER;
            for (Ipp32s j = 0; j < count; j ++)
            {
                H264Slice * pSlice = slicesInfo->GetSlice(j);
                if (pSlice->m_iFirstMB > pCurSlice->m_iFirstMB && minFirst > pSlice->m_iFirstMB)
                {
                    minFirst = pSlice->m_iFirstMB;
                }
            }

            if (minFirst != MAX_MB_NUMBER)
            {
                pCurSlice->m_iMaxMB = minFirst;
            }
        }
    }

    if (!field)
    {
        H264SeqParamSet *pSeqParam = slicesInfo->GetSlice(0)->GetSeqParam();

        pFrame->totalMBs = pSeqParam->frame_width_in_mbs * pSeqParam->frame_height_in_mbs;
        if (pFrame->m_PictureStructureForDec < FRM_STRUCTURE)
            pFrame->totalMBs /= 2;
    }

    slicesInfo->SetStatus(H264DecoderFrameInfo::STATUS_FILLED);
}

Status TaskSupplier::InitFreeFrame(H264DecoderFrame * pFrame, H264Slice *pSlice)
{
    Status umcRes = UMC_OK;
    H264SeqParamSet *pSeqParam = pSlice->GetSeqParam();

    Ipp32s iMBCount = pSeqParam->frame_width_in_mbs * pSeqParam->frame_height_in_mbs;
    pFrame->totalMBs = iMBCount;

    pFrame->m_FrameType = SliceTypeToFrameType(pSlice->GetSliceHeader()->slice_type);
    pFrame->m_dFrameTime = pSlice->m_dTime;
    pFrame->m_crop_left = SubWidthC[pSeqParam->chroma_format_idc] * pSeqParam->frame_cropping_rect_left_offset;
    pFrame->m_crop_right = SubWidthC[pSeqParam->chroma_format_idc] * pSeqParam->frame_cropping_rect_right_offset;
    pFrame->m_crop_top = SubHeightC[pSeqParam->chroma_format_idc] * pSeqParam->frame_cropping_rect_top_offset * (2 - pSeqParam->frame_mbs_only_flag);
    pFrame->m_crop_bottom = SubHeightC[pSeqParam->chroma_format_idc] * pSeqParam->frame_cropping_rect_bottom_offset * (2 - pSeqParam->frame_mbs_only_flag);
    pFrame->m_crop_flag = pSeqParam->frame_cropping_flag;

    pFrame->setFrameNum(pSlice->GetSliceHeader()->frame_num);

    if (pSeqParam->aspect_ratio_idc == 255)
    {
        pFrame->m_aspect_width  = pSeqParam->sar_width;
        pFrame->m_aspect_height = pSeqParam->sar_height;
    }
    else
    {
        pFrame->m_aspect_width  = SAspectRatio[pSeqParam->aspect_ratio_idc][0];
        pFrame->m_aspect_height = SAspectRatio[pSeqParam->aspect_ratio_idc][1];
    }

    if (pSlice->GetSliceHeader()->field_pic_flag)
    {
        pFrame->m_bottom_field_flag[0] = pSlice->GetSliceHeader()->bottom_field_flag;
        pFrame->m_bottom_field_flag[1] = !pSlice->GetSliceHeader()->bottom_field_flag;

        pFrame->m_PictureStructureForRef =
        pFrame->m_PictureStructureForDec = FLD_STRUCTURE;
    }
    else
    {
        pFrame->m_bottom_field_flag[0] = 0;
        pFrame->m_bottom_field_flag[1] = 1;

        if (pSlice->m_SliceHeader.MbaffFrameFlag)
        {
            pFrame->m_PictureStructureForRef =
            pFrame->m_PictureStructureForDec = AFRM_STRUCTURE;
        }
        else
        {
            pFrame->m_PictureStructureForRef =
            pFrame->m_PictureStructureForDec = FRM_STRUCTURE;
        }
    }

    Ipp32s chroma_format_idc = pFrame->IsAuxiliaryFrame() ? 0 : pSeqParam->chroma_format_idc;

    Ipp8u bit_depth_luma, bit_depth_chroma;
    if (pFrame->IsAuxiliaryFrame())
    {
        bit_depth_luma = pSlice->GetSeqParamEx()->bit_depth_aux;
        bit_depth_chroma = 8;
    } else {
        bit_depth_luma = pSeqParam->bit_depth_luma;
        bit_depth_chroma = pSeqParam->bit_depth_chroma;
    }
    
    Ipp32s iMBWidth = pSeqParam->frame_width_in_mbs;
    Ipp32s iMBHeight = pSeqParam->frame_height_in_mbs;
    IppiSize dimensions = {iMBWidth * 16, iMBHeight * 16};

    ColorFormat cf = GetUMCColorFormat(chroma_format_idc);

    if (pSlice->GetSeqParamEx() && pSlice->GetSeqParamEx()->aux_format_idc)
    {
        cf = ConvertColorFormatToAlpha(cf);
    }

    //if (cf == YUV420)
      //  cf = NV12;

    VideoData info;
    info.Init(dimensions.width, dimensions.height, cf);
    info.SetPlaneBitDepth(bit_depth_luma, 0);
    info.SetPlaneBitDepth(bit_depth_chroma, 1);
    info.SetPlaneBitDepth(bit_depth_chroma, 2);

    pFrame->Init(&info);

    return umcRes;
}

Status TaskSupplier::AllocateFrameData(H264DecoderFrame * pFrame, IppiSize dimensions, Ipp32s bit_depth_luma, Ipp32s bit_depth_chroma, ColorFormat color_format)
{
    VideoData info;
    info.Init(dimensions.width, dimensions.height, color_format);
    info.SetPlaneBitDepth(bit_depth_luma, 0);
    info.SetPlaneBitDepth(bit_depth_chroma, 1);
    info.SetPlaneBitDepth(bit_depth_chroma, 2);

    FrameMemID frmMID;
    Status sts = m_pFrameAllocator->Alloc(&frmMID, &info, 0);

    if (sts != UMC_OK)
    {
        throw h264_exception(UMC_ERR_ALLOC);
    }

    const FrameData *frmData = m_pFrameAllocator->Lock(frmMID);

    if (!frmData)
        throw h264_exception(UMC_ERR_LOCK);

    pFrame->allocate(frmData, &info);

    Status umcRes = pFrame->allocateParsedFrameData();

    return umcRes;
}

H264DecoderFrame * TaskSupplier::AddFrame(H264Slice *pSlice)
{
    if (!pSlice)
        return 0;

    H264DecoderFrame *pFrame = 0;

    m_pLastFullFrame = 0;

    if (pSlice->IsAuxiliary())
    {
        VM_ASSERT(!m_pCurrentFrame->IsAuxiliaryFrame());
        ((H264DecoderFrameExtension *)m_pCurrentFrame)->AllocateAuxiliary();
        ((H264DecoderFrameExtension *)m_pCurrentFrame)->FillInfoToAuxiliary();
        pFrame = UMC::GetAuxiliaryFrame(m_pCurrentFrame);
    }
    else
    {
        pFrame = GetFreeFrame();
    }

    if (!pFrame)
    {
        return 0;
    }

    Status umcRes = InitFreeFrame(pFrame, pSlice);
    if (umcRes != UMC_OK)
    {
        return 0;
    }

    umcRes = AllocateFrameData(pFrame, pFrame->lumaSize(), pFrame->m_bpp_luma, pFrame->m_bpp_chroma, pFrame->GetColorFormat());
    if (umcRes != UMC_OK)
    {
        return 0;
    }

    if(!m_UserData.IsEmpty())
    {
        pFrame->m_UserData.Clear();
        pFrame->m_pCurrentUD = NULL;

        for(List<H264SEIPayLoad>::Iterator pSEI = m_UserData.ItrFront(); pSEI != m_UserData.ItrBackBound(); ++pSEI)
        {
            pFrame->m_UserData.PushBack();
            pFrame->m_UserData.Back() = *pSEI;
        }
        m_UserData.Clear();
    }

    if (m_sei_messages)
        m_sei_messages->SetFrame(pFrame);

    H264SEIPayLoad * payload = m_Headers.m_SEIParams.GetHeader(SEI_PIC_TIMING_TYPE);
    if (payload && pSlice->GetSeqParam()->pic_struct_present_flag)
    {
        pFrame->m_displayPictureStruct = payload->SEI_messages.pic_timing.pic_struct;
    }
    else
    {
        pFrame->m_displayPictureStruct = (pFrame->m_PictureStructureForDec == FRM_STRUCTURE) ? DPS_FRAME : DPS_TOP_BOTTOM;
    }

    if (pSlice->IsAuxiliary())
    {
        for (H264DecoderFrame *pTmp = m_pDecodedFramesList->head(); pTmp; pTmp = pTmp->future())
        {
            H264DecoderFrameExtension * frame = DynamicCast<H264DecoderFrameExtension, H264DecoderFrame>(pTmp);
            VM_ASSERT(frame);
            frame->FillInfoToAuxiliary();
        }
    }

    m_field_index = 0;

    if (pSlice->IsField())
    {
        pFrame->GetAU(1)->SetStatus(H264DecoderFrameInfo::STATUS_NOT_FILLED);
    }

    //fill chroma planes in case of 4:0:0
    if (pFrame->m_chroma_format == 0)
    {
        pFrame->DefaultFill(2, true);
    }

    InitFrame(pFrame, pSlice);
    return pFrame;
} // H264DecoderFrame * TaskSupplier::AddFrame(H264Slice *pSlice)

void TaskSupplier::InitFrame(H264DecoderFrame * pFrame, H264Slice *pSlice)
{
    H264SliceHeader *sliceHeader = pSlice->GetSliceHeader();
    if (sliceHeader->idr_flag)
    {
        POCDecoder::Reset(sliceHeader->frame_num);
    }

    DecodePictureOrderCount(pSlice, sliceHeader->frame_num);

    pFrame->m_bIDRFlag = sliceHeader->idr_flag != 0;

    if (pFrame->m_bIDRFlag)
    {
        m_pDecodedFramesList->IncreaseRefPicListResetCount(pFrame);
    }

    pFrame->setFrameNum(sliceHeader->frame_num);

    if (sliceHeader->field_pic_flag == 0)
        pFrame->setPicNum(sliceHeader->frame_num, 0);
    else
        pFrame->setPicNum(sliceHeader->frame_num*2+1, m_field_index);

    //transfer previosly calculated PicOrdeCnts into current Frame
    if (pFrame->m_PictureStructureForRef < FRM_STRUCTURE)
    {
        pFrame->setPicOrderCnt(m_PicOrderCnt, m_field_index);
        if (!m_field_index) // temporally set same POC for second field
            pFrame->setPicOrderCnt(m_PicOrderCnt, 1);
    }
    else
    {
        pFrame->setPicOrderCnt(m_TopFieldPOC, 0);
        pFrame->setPicOrderCnt(m_BottomFieldPOC, 1);
    }

    pFrame->InitRefPicListResetCount(m_field_index);
} // void TaskSupplier::InitFrame(H264DecoderFrame * pFrame, H264Slice *pSlice)

void TaskSupplier::AddSliceToFrame(H264DecoderFrame *pFrame, H264Slice *pSlice)
{
    if (pFrame->m_FrameType < SliceTypeToFrameType(pSlice->GetSliceHeader()->slice_type))
        pFrame->m_FrameType = SliceTypeToFrameType(pSlice->GetSliceHeader()->slice_type);

    H264DecoderFrameInfo * au_info = pFrame->GetAU(m_field_index);
    Ipp32s iSliceNumber = au_info->GetSliceCount() + 1;

    if (m_field_index)
    {
        iSliceNumber += pFrame->m_TopSliceCount;
    }
    else
    {
        pFrame->m_TopSliceCount++;
    }

    pFrame->m_iNumberOfSlices++;

    pSlice->SetSliceNumber(iSliceNumber);
    pSlice->m_pCurrentFrame = pFrame;
    au_info->AddSlice(pSlice);
}

void TaskSupplier::DBPUpdate(H264DecoderFrame * pFrame, Ipp32s field)
{
    H264Slice * pSlice = pFrame->GetAU(field)->GetSlice(0);
    if (!pSlice->IsReference())
        return;

    UpdateRefPicMarking(pFrame, pSlice, field);

    if (GetAuxiliaryFrame(pFrame))
    {
        // store marking results to auxiliary frames
        H264DecoderFrame *pHead = m_pDecodedFramesList->head();

        for (H264DecoderFrame *pTmp = pHead; pTmp; pTmp = pTmp->future())
        {
            H264DecoderFrameExtension * frame = DynamicCast<H264DecoderFrameExtension, H264DecoderFrame>(pTmp);
            VM_ASSERT(frame);
            frame->FillInfoToAuxiliary();
        }
    }
}


Status TaskSupplier::IsNeedRunDecoding(H264DecoderFrame ** decoded)
{
    CompleteDecodedFrames(decoded);

    return RunDecoding_1();
}

H264DecoderFrame * TaskSupplier::FindSurface(FrameMemID id)
{
    AutomaticMutex guard(m_mGuard);

    H264DecoderFrame *pFrame = m_pDecodedFramesList->head();
    for (; pFrame; pFrame = pFrame->future())
    {
        if (pFrame->GetFrameData()->GetFrameMID() == id)
            return pFrame;
    }

    return 0;
}

Status TaskSupplier::RunDecoding_1()
{
    H264DecoderFrame *pFrame = m_pDecodedFramesList->head();

    for (; pFrame; pFrame = pFrame->future())
    {
        if (!pFrame->IsDecodingCompleted())
        {
            break;
        }
    }

    m_pTaskBroker->Start();

    if (!pFrame)
        return UMC_OK;

    return UMC_OK;
}

Status TaskSupplier::RunDecoding(bool force, H264DecoderFrame ** )
{
    Status umcRes = UMC_OK;

    CompleteDecodedFrames(0);
    m_pTaskBroker->Start();

    if (!m_pTaskBroker->IsEnoughForStartDecoding(force))
    {
        CompleteDecodedFrames(0);
        return UMC_ERR_NOT_ENOUGH_DATA;
    }

    do
    {
        umcRes = m_pSegmentDecoder[0]->ProcessSegment();
    } while (umcRes == UMC_OK);

    CompleteDecodedFrames(0);
    if (umcRes == UMC_ERR_NOT_ENOUGH_DATA)
        return UMC_OK;

    return umcRes;
}

void TaskSupplier::InitColorConverter(H264DecoderFrame *source, VideoData * videoData, Ipp8u force_field)
{
    if (!videoData || ! source)
        return;

    // set correct size in color converter
    Ipp32s crop_left   = source->m_crop_left;
    Ipp32s crop_right  = source->m_crop_right;
    Ipp32s crop_top    = source->m_crop_top;
    Ipp32s crop_bottom = source->m_crop_bottom;
    Ipp32u width  = source->lumaSize().width;
    Ipp32u height = source->lumaSize().height;
    Ipp32u pitch;

    if (force_field)
    {
        height >>= 1;
        crop_top >>= 1;
        crop_bottom >>= 1;
    }

    videoData->Init(width,  height, source->GetColorFormat());

    if (source->m_pYPlane)
    {
        videoData->SetPlaneBitDepth(source->m_bpp_luma, 0);
        videoData->SetPlaneDataPtr((void*)source->m_pYPlane, 0);
    }
    else
    {
        videoData->SetPlaneDataPtr((void*)1, 0);
    }

    if (source->m_pUPlane)
    {
        videoData->SetPlaneBitDepth(source->m_bpp_chroma, 1);
        videoData->SetPlaneBitDepth(source->m_bpp_chroma, 2);
        videoData->SetPlaneDataPtr((void*)source->m_pUPlane, 1);
        videoData->SetPlaneDataPtr((void*)source->m_pVPlane, 2);
    }
    else
    {
        if (source->m_pUVPlane) // NV12
        {
            videoData->SetPlaneBitDepth(source->m_bpp_chroma, 1);
            videoData->SetPlaneDataPtr((void*)source->m_pUVPlane, 1);
        }
        else
        {
            videoData->SetPlaneDataPtr((void*)1, 1);
            videoData->SetPlaneDataPtr((void*)1, 2);
        }
    }

    pitch = source->pitch_luma()*((source->m_bpp_luma+7)/8);
    videoData->SetPlanePitch(pitch, 0);

    pitch = source->pitch_chroma()*((source->m_bpp_chroma+7)/8);
    videoData->SetPlanePitch(pitch, 1);
    videoData->SetPlanePitch(pitch, 2);

    H264DecoderFrame *auxiliary = GetAuxiliaryFrame(source);
    if (auxiliary)
    {
        pitch = auxiliary->pitch_luma()*((auxiliary->m_bpp_luma+7)/8);
        videoData->SetPlaneBitDepth(auxiliary->m_bpp_luma, 3);
        videoData->SetPlaneDataPtr((void*)auxiliary->m_pYPlane, 3);
        videoData->SetPlanePitch(pitch, 3);
    }

    switch (source->m_displayPictureStruct)
    {
    case DPS_BOTTOM:
    case DPS_BOTTOM_TOP:
    case DPS_BOTTOM_TOP_BOTTOM:
        videoData->m_picStructure = PS_BOTTOM_FIELD_FIRST;
        break;
    case DPS_TOP_BOTTOM:
    case DPS_TOP:
    case DPS_TOP_BOTTOM_TOP:
        videoData->m_picStructure = PS_TOP_FIELD_FIRST;
        break;

    case DPS_FRAME:
    case DPS_FRAME_DOUBLING:
    case DPS_FRAME_TRIPLING:
    default:
        videoData->m_picStructure = PS_PROGRESSIVE;
        break;
    }

    videoData->SetDataSize(videoData->GetMappingSize());
    videoData->m_frameType = source->m_FrameType;
    videoData->m_iSAWidth  = source->m_aspect_width;
    videoData->m_iSAHeight = source->m_aspect_height;

    m_LastNonCropDecodedFrame = *videoData;

    if (crop_left | crop_right | crop_top | crop_bottom)
    {
        UMC::RECT SrcCropArea;
        SrcCropArea.iLeft = (Ipp16s)(crop_left);
        SrcCropArea.iTop  = (Ipp16s)(crop_top);
        SrcCropArea.iRight = (Ipp16s)(width - crop_right);
        SrcCropArea.iBottom = (Ipp16s)(height - crop_bottom);

        videoData->Crop(SrcCropArea);
    }

    videoData->m_fPTSStart = source->m_dFrameTime;
}

Status TaskSupplier::GetUserData(MediaData * pUD)
{
    if(!pUD)
        return UMC_ERR_NULL_PTR;

    if (!m_pLastDisplayed)
        return UMC_ERR_NOT_ENOUGH_DATA;

    if(!m_pLastDisplayed->m_UserData.IsEmpty() && m_pLastDisplayed->m_pCurrentUD != m_pLastDisplayed->m_UserData.ItrBackBound())
    {
        if(!m_pLastDisplayed->m_pCurrentUD)
            m_pLastDisplayed->m_pCurrentUD = m_pLastDisplayed->m_UserData.ItrFront();

        pUD->m_fPTSStart = m_pLastDisplayed->m_dFrameTime;
        pUD->SetBufferPointer(m_pLastDisplayed->m_pCurrentUD->user_data, m_pLastDisplayed->m_pCurrentUD->payLoadSize);
        pUD->SetDataSize(m_pLastDisplayed->m_pCurrentUD->payLoadSize);

        ++m_pLastDisplayed->m_pCurrentUD;
        return UMC_OK;
    }

    return UMC_ERR_NOT_ENOUGH_DATA;
}

bool TaskSupplier::IsShouldSuspendDisplay()
{
    AutomaticMutex guard(m_mGuard);

    if (m_pDecodedFramesList->GetDisposable() || m_pDecodedFramesList->countAllFrames() < m_dpbSize + m_DPBSizeEx)
        return false;

    return true;
}

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
