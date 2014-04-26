/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Frame Processing for multi-frame threading model
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_job.h"
#include "umc_vc1_dec_seq.h"

#include "umc_vc1_dec_time_statistics.h"

#include "umc_vc1_dec_task_store.h"
#include "umc_vc1_dec_task.h"
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_common_blk_order_tbl.h"
#include "umc_vc1_common.h"
#include "umc_vc1_dec_frame_descr.h"
#include "umc_vc1_dec_exception.h"

using namespace UMC;
using namespace UMC::VC1Exceptions;



bool VC1FrameDescriptor::Init(Ipp32u         DescriporID,
                              VC1Context*    pContext,
                              VC1TaskStore*  pStore,
                              bool           IsReorder,
                              Ipp16s*        pResidBuf)
{
    VC1SequenceLayerHeader* seqLayerHeader = pContext->m_seqLayerHeader;
    m_bIsReorder = IsReorder;

    if (!m_pContext)
    {
        Ipp8u* ptr = NULL;
        ptr += align_value<Ipp32u>(sizeof(VC1Context));
        ptr += align_value<Ipp32u>(sizeof(VC1MB)*(pContext->m_seqLayerHeader->heightMB*pContext->m_seqLayerHeader->widthMB));
        ptr += align_value<Ipp32u>(sizeof(VC1PictureLayerHeader)*VC1_MAX_SLICE_NUM);
        ptr += align_value<Ipp32u>(sizeof(VC1DCMBParam)*seqLayerHeader->heightMB*seqLayerHeader->widthMB);
        ptr += align_value<Ipp32u>(sizeof(Ipp16s)*seqLayerHeader->heightMB*seqLayerHeader->widthMB*2*2);
        ptr += align_value<Ipp32u>(sizeof(Ipp8u)*seqLayerHeader->heightMB*seqLayerHeader->widthMB);
        ptr += align_value<Ipp32u>((seqLayerHeader->heightMB*seqLayerHeader->widthMB*VC1_MAX_BITPANE_CHUNCKS));

        // Need to replace with MFX allocator
        if (m_pMemoryAllocator->Alloc(&m_iMemContextID,
                                      (size_t)ptr,
                                      UMC_ALLOC_PERSISTENT,
                                      16) != UMC_OK)
                                      return false;

        m_pContext = (VC1Context*)(m_pMemoryAllocator->Lock(m_iMemContextID));
        memset(m_pContext,0,size_t(ptr));
        ptr = (Ipp8u*)m_pContext;

        ptr += align_value<Ipp32u>(sizeof(VC1Context));
        m_pContext->m_MBs = (VC1MB*)ptr;

        ptr +=  align_value<Ipp32u>(sizeof(VC1MB)*(pContext->m_seqLayerHeader->heightMB*pContext->m_seqLayerHeader->widthMB));
        m_pContext->m_picLayerHeader = (VC1PictureLayerHeader*)ptr;
        m_pContext->m_InitPicLayer = m_pContext->m_picLayerHeader;

        ptr += align_value<Ipp32u>((sizeof(VC1PictureLayerHeader)*VC1_MAX_SLICE_NUM));
        m_pContext->DCACParams = (VC1DCMBParam*)ptr;


        ptr += align_value<Ipp32u>(sizeof(VC1DCMBParam)*seqLayerHeader->heightMB*seqLayerHeader->widthMB);
        m_pContext->savedMV = (Ipp16s*)(ptr);

        ptr += align_value<Ipp32u>(sizeof(Ipp16s)*seqLayerHeader->heightMB*seqLayerHeader->widthMB*2*2);
        m_pContext->savedMVSamePolarity  = ptr;

        ptr += align_value<Ipp32u>(sizeof(Ipp8u)*seqLayerHeader->heightMB*seqLayerHeader->widthMB);
        m_pContext->m_pBitplane.m_databits = ptr;

    }
    Ipp32u buffSize =  (seqLayerHeader->heightMB*VC1_PIXEL_IN_LUMA)*
                        (seqLayerHeader->widthMB*VC1_PIXEL_IN_LUMA)
                        + ((seqLayerHeader->heightMB*VC1_PIXEL_IN_CHROMA)
                        *(seqLayerHeader->widthMB*VC1_PIXEL_IN_CHROMA ))*2;

    //buf size should be divisible by 4
    if(buffSize & 0x00000003)
        buffSize = (buffSize&0xFFFFFFFC) + 4;

    // Need to replace with MFX allocator
    if (m_pMemoryAllocator->Alloc(&m_iInernBufferID,
                                  buffSize,
                                  UMC_ALLOC_PERSISTENT,
                                  16) != UMC_OK)
                                  return false;

    m_pContext->m_pBufferStart = (Ipp8u*)m_pMemoryAllocator->Lock(m_iInernBufferID);
    memset(m_pContext->m_pBufferStart, 0, buffSize);

    // memory for diffs for each FrameDescriptor
    if (!m_pDiffMem)
    {
        if (!pResidBuf)
        {
            if(m_pMemoryAllocator->Alloc(&m_iDiffMemID,
                                         sizeof(Ipp16s)*seqLayerHeader->widthMB*seqLayerHeader->heightMB*8*8*6,
                                         UMC_ALLOC_PERSISTENT, 16) != UMC_OK )
            {
                return false;
            }
            m_pDiffMem = (Ipp16s*)m_pMemoryAllocator->Lock(m_iDiffMemID);
        }
        else
            m_pDiffMem = pResidBuf;

    }

    // Pointers to common pContext
    m_pStore = pStore;
    m_pContext->m_vlcTbl = pContext->m_vlcTbl;
    m_pContext->pRefDist = &pContext->RefDist;
    m_pContext->m_frmBuff.m_pFrames = pContext->m_frmBuff.m_pFrames;
    m_pContext->m_frmBuff.m_iDisplayIndex =  0;
    m_pContext->m_frmBuff.m_iCurrIndex    =  0;
    m_pContext->m_frmBuff.m_iPrevIndex    =  0;
    m_pContext->m_frmBuff.m_iNextIndex    =  1;
    m_pContext->m_frmBuff.m_iICompFieldIndex   =  pContext->m_frmBuff.m_iICompFieldIndex;
    m_pContext->m_seqLayerHeader = pContext->m_seqLayerHeader;
    m_pContext->savedMV_Curr = pContext->savedMV_Curr;
    m_pContext->savedMVSamePolarity_Curr = pContext->savedMVSamePolarity_Curr;
    m_iSelfID = DescriporID;
    return true;
}
void VC1FrameDescriptor::Reset()
{
    m_iFrameCounter = 0;
    m_iRefFramesDst = 0;
    m_bIsReadyToLoad = true;
    m_iSelfID = 0;
    m_iRefFramesDst = 0;
    m_iBFramesDst = 0;
    m_bIsReferenceReady = false;
    m_bIsBReady = false;
    m_bIsReadyToDisplay = false;
    m_bIsSkippedFrame = false;
    m_bIsReadyToProcess = false;
}
void VC1FrameDescriptor::Release()
{
    if(m_pMemoryAllocator)
    {
        if (m_iDiffMemID != 0)
        {
            m_pMemoryAllocator->Unlock(m_iDiffMemID);
            m_pMemoryAllocator->Free(m_iDiffMemID);
            m_iDiffMemID = (MemID)0;
        }

        if (m_iInernBufferID != 0)
        {
            m_pMemoryAllocator->Unlock(m_iInernBufferID);
            m_pMemoryAllocator->Free(m_iInernBufferID);
            m_iInernBufferID = (MemID)0;
        }
        if (m_iMemContextID != 0)
        {
            m_pMemoryAllocator->Unlock(m_iMemContextID);
            m_pMemoryAllocator->Free(m_iMemContextID);
            m_iMemContextID = (MemID)0;
        }

    }

}
void VC1FrameDescriptor::processFrame(Ipp32u*  pOffsets,
                                      Ipp32u*  pValues)
{
    SliceParams slparams;
    memset(&slparams,0,sizeof(SliceParams));
    VC1Task task(0);

    Ipp32u temp_value = 0;
    Ipp32u* bitstream;
    Ipp32s bitoffset = 31;

    Ipp16u heightMB = m_pContext->m_seqLayerHeader->heightMB;

    if ((m_pContext->m_picLayerHeader->FCM != VC1_FieldInterlace)&&
        (m_pContext->m_seqLayerHeader->IsResize))
        heightMB -= 1;

    bool isSecondField = false;
    slparams.MBStartRow = 0;
    slparams.is_continue = 1;
    slparams.MBEndRow = heightMB;

    if (m_pContext->m_picLayerHeader->PTYPE == VC1_SKIPPED_FRAME)
    {
        m_bIsSkippedFrame = true;
        m_bIsReadyToProcess = false;
        SZTables(m_pContext);
        return;
    }
    else
    {
        m_bIsSkippedFrame = false;
    }

    if (m_pContext->m_seqLayerHeader->PROFILE == VC1_PROFILE_ADVANCED)
        DecodePicHeader(m_pContext);
    else
        Decode_PictureLayer(m_pContext);



    slparams.m_pstart = m_pContext->m_bitstream.pBitstream;
    slparams.m_bitOffset = m_pContext->m_bitstream.bitOffset;
    slparams.m_picLayerHeader = m_pContext->m_picLayerHeader;
    slparams.m_vlcTbl = m_pContext->m_vlcTbl;

    if (m_pContext->m_seqLayerHeader->PROFILE != VC1_PROFILE_ADVANCED)
    {
        slparams.MBRowsToDecode = slparams.MBEndRow-slparams.MBStartRow;
        task.m_pSlice = &slparams;
        task.setSliceParams(m_pContext);
        task.m_isFieldReady = true;
        m_pStore->AddSampleTask(&task,m_iSelfID);
        task.m_pSlice = NULL;
        m_pStore->DistributeTasks(m_iSelfID);
        m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].isIC = m_pContext->m_bIntensityCompensation;

        SZTables(m_pContext);
        if (VC1_P_FRAME == m_pContext->m_picLayerHeader->PTYPE)
            CreateComplexICTablesForFrame(m_pContext);
        return;
    }

    if (*(pValues+1) == 0x0B010000)
    {
        bitstream = reinterpret_cast<Ipp32u*>(m_pContext->m_pBufferStart + *(pOffsets+1));
        VC1BitstreamParser::GetNBits(bitstream,bitoffset,32, temp_value);
        bitoffset = 31;
        VC1BitstreamParser::GetNBits(bitstream,bitoffset,9, slparams.MBEndRow);     //SLICE_ADDR
        m_pContext->m_picLayerHeader->is_slice = 1;
    }
    else if(*(pValues+1) == 0x0C010000)
    {
         slparams.m_picLayerHeader->CurrField = 0;
         slparams.m_picLayerHeader->PTYPE = m_pContext->m_picLayerHeader->PTypeField1;
         slparams.m_picLayerHeader->BottomField = (Ipp8u)(1 - m_pContext->m_picLayerHeader->TFF);
         slparams.MBEndRow = heightMB/2;
    }


    slparams.MBRowsToDecode = slparams.MBEndRow-slparams.MBStartRow;
    task.m_pSlice = &slparams;
#ifdef SLICE_INFO
    Ipp32s slice_counter = 0;
    printf("Slice number %d\n", slice_counter);
    printf("Number MB rows to decode  =%d\n", slparams.MBRowsToDecode);
    ++slice_counter;
#endif
    task.setSliceParams(m_pContext);

    task.m_isFieldReady = true;
    m_pStore->AddSampleTask(&task,m_iSelfID);

    pOffsets++;
    pValues++;

    while (*pOffsets)
    {
        task.m_isFirstInSecondSlice = false;
        if (*(pValues) == 0x0C010000)
        {
            isSecondField = true;
            task.m_isFirstInSecondSlice = true;
            m_pContext->m_bitstream.pBitstream = reinterpret_cast<Ipp32u*>(m_pContext->m_pBufferStart + *pOffsets);
            m_pContext->m_bitstream.pBitstream += 1; // skip start code
            m_pContext->m_bitstream.bitOffset = 31;
            //m_pContext->m_picLayerHeader = m_pContext->m_InitPicLayer + 1;
            ++m_pContext->m_picLayerHeader;
            *m_pContext->m_picLayerHeader = *m_pContext->m_InitPicLayer;

            m_pContext->m_picLayerHeader->BottomField = (Ipp8u)m_pContext->m_InitPicLayer->TFF;
            m_pContext->m_picLayerHeader->PTYPE = m_pContext->m_InitPicLayer->PTypeField2;
            m_pContext->m_picLayerHeader->CurrField = 1;
            m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].RANGE_MAPY  = m_pContext->m_seqLayerHeader->RANGE_MAPY;
            m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].RANGE_MAPUV = m_pContext->m_seqLayerHeader->RANGE_MAPUV;

            m_pContext->m_picLayerHeader->is_slice = 0;
            DecodePicHeader(m_pContext);

            //VC1BitstreamParser::GetNBits(m_pContext->m_pbs,m_pContext->m_bitOffset,32, temp_value);

            slparams.MBStartRow = heightMB/2;

            if (*(pOffsets+1) && *(pValues+1) == 0x0B010000)
            {
                bitstream = reinterpret_cast<Ipp32u*>(m_pContext->m_pBufferStart + *(pOffsets+1));
                VC1BitstreamParser::GetNBits(bitstream,bitoffset,32, temp_value);
                bitoffset = 31;
                VC1BitstreamParser::GetNBits(bitstream,bitoffset,9, slparams.MBEndRow);
            } else
                slparams.MBEndRow = heightMB;

            slparams.m_picLayerHeader = m_pContext->m_picLayerHeader;
            slparams.m_vlcTbl = m_pContext->m_vlcTbl;
            slparams.m_pstart = m_pContext->m_bitstream.pBitstream;
            slparams.m_bitOffset = m_pContext->m_bitstream.bitOffset;
            slparams.MBRowsToDecode = slparams.MBEndRow-slparams.MBStartRow;
            task.m_pSlice = &slparams;
            task.setSliceParams(m_pContext);
            if (isSecondField)
                task.m_isFieldReady = false;
            else
                task.m_isFieldReady = true;
            m_pStore->AddSampleTask(&task,m_iSelfID);
            slparams.MBStartRow = slparams.MBEndRow;
            ++pOffsets;
            ++pValues;
        }
        else if (*(pValues) == 0x0B010000)
        {
            m_pContext->m_bitstream.pBitstream = reinterpret_cast<Ipp32u*>(m_pContext->m_pBufferStart + *pOffsets);
            VC1BitstreamParser::GetNBits(m_pContext->m_bitstream.pBitstream,m_pContext->m_bitstream.bitOffset,32, temp_value);
            m_pContext->m_bitstream.bitOffset = 31;

            VC1BitstreamParser::GetNBits(m_pContext->m_bitstream.pBitstream,m_pContext->m_bitstream.bitOffset,9, slparams.MBStartRow);     //SLICE_ADDR
            VC1BitstreamParser::GetNBits(m_pContext->m_bitstream.pBitstream,m_pContext->m_bitstream.bitOffset,1, temp_value);            //PIC_HEADER_FLAG


            if (temp_value == 1)                //PIC_HEADER_FLAG
            {
                ++m_pContext->m_picLayerHeader;
                if (isSecondField)
                    m_pContext->m_picLayerHeader->CurrField = 1;
                else
                    m_pContext->m_picLayerHeader->CurrField = 0;
                DecodePictureHeader_Adv(m_pContext);
                DecodePicHeader(m_pContext);
            }
            m_pContext->m_picLayerHeader->is_slice = 1;

            if (*(pOffsets+1) && *(pValues+1) == 0x0B010000)
            {
                bitstream = reinterpret_cast<Ipp32u*>(m_pContext->m_pBufferStart + *(pOffsets+1));
                VC1BitstreamParser::GetNBits(bitstream,bitoffset,32, temp_value);
                bitoffset = 31;
                VC1BitstreamParser::GetNBits(bitstream,bitoffset,9, slparams.MBEndRow);

            }
            else if(*(pValues+1) == 0x0C010000)
                slparams.MBEndRow = heightMB/2;
            else
                slparams.MBEndRow = heightMB;

            slparams.m_picLayerHeader = m_pContext->m_picLayerHeader;
            slparams.m_vlcTbl = m_pContext->m_vlcTbl;
            slparams.m_pstart = m_pContext->m_bitstream.pBitstream;
            slparams.m_bitOffset = m_pContext->m_bitstream.bitOffset;
            slparams.MBRowsToDecode = slparams.MBEndRow-slparams.MBStartRow;
            task.m_pSlice = &slparams;
            task.setSliceParams(m_pContext);
            if (isSecondField)
                task.m_isFieldReady = false;
            else
                task.m_isFieldReady = true;
            m_pStore->AddSampleTask(&task,m_iSelfID);
#ifdef SLICE_INFO
            printf("Slice number %d\n", slice_counter);
            printf("Number MB rows to decode  =%d\n", slparams.MBRowsToDecode);
            ++slice_counter;
#endif
            slparams.MBStartRow = slparams.MBEndRow;

            ++pOffsets;
            ++pValues;
        }
        else
        {
            pOffsets++;
            pValues++;
        }
    }
    SZTables(m_pContext);
    if ((VC1_B_FRAME != m_pContext->m_picLayerHeader->PTYPE)&&
        (VC1_BI_FRAME != m_pContext->m_picLayerHeader->PTYPE))

    {
        if ((m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].ICFieldMask)||
            ((m_pContext->m_frmBuff.m_iPrevIndex > -1)&&
            (m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].ICFieldMask)))
        {
            if (m_pContext->m_picLayerHeader->FCM ==  VC1_FieldInterlace)
                CreateComplexICTablesForFields(m_pContext);
            else
                CreateComplexICTablesForFrame(m_pContext);
        }
    }


     // Intensity compensation for frame
    m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].isIC = m_pContext->m_bIntensityCompensation;
    task.m_pSlice = NULL;
STATISTICS_START_TIME(m_timeStatistics->alg_StartTime);
    m_pStore->DistributeTasks(m_iSelfID);
STATISTICS_END_TIME(m_timeStatistics->alg_StartTime,
                    m_timeStatistics->alg_EndTime,
                    m_timeStatistics->alg_TotalTime);

}

Status VC1FrameDescriptor::preProcData(Ipp8u*                 pbufferStart,
                                       Ipp32u                 bufferSize,
                                       Ipp64u                 frameCount,
                                       bool                   isWMV)
{
    Status vc1Sts = UMC_OK;

    Ipp32u Ptype;
    m_iFrameCounter = frameCount;
    m_pContext->m_FrameSize = bufferSize;
    if (isWMV)
        bufferSize += 8;

    ippsCopy_8u(pbufferStart,m_pContext->m_pBufferStart,(bufferSize & 0xFFFFFFF8) + 8); // (bufferSize & 0xFFFFFFF8) + 8 - skip frames
    m_pContext->m_bitstream.pBitstream = (Ipp32u*)m_pContext->m_pBufferStart;

    if ((m_pContext->m_seqLayerHeader->PROFILE == VC1_PROFILE_ADVANCED)||(!isWMV))
        m_pContext->m_bitstream.pBitstream += 1;

    m_pContext->m_bitstream.bitOffset = 31;

    m_pContext->m_picLayerHeader = m_pContext->m_InitPicLayer;
    m_bIsSpecialBSkipFrame = false;
    if (m_pContext->m_seqLayerHeader->PROFILE == VC1_PROFILE_ADVANCED)
    {
        m_pContext->m_bNeedToUseCompBuffer = 0;
        GetNextPicHeader_Adv(m_pContext);
        Ptype = m_pContext->m_picLayerHeader->PTYPE|m_pContext->m_picLayerHeader->PTypeField1;
        vc1Sts = SetDisplayIndex_Adv(Ptype);
        // skipping tools
        if (m_pStore->IsNeedSkipFrame(Ptype))
        {
            if ((Ptype == VC1_B_FRAME) || (Ptype == VC1_BI_FRAME))
                m_bIsSpecialBSkipFrame = true;
            m_pContext->m_picLayerHeader->PTYPE = Ptype = VC1_SKIPPED_FRAME;
        }
    }
    else
    {
        if (!isWMV)
            m_pContext->m_bitstream.pBitstream = (Ipp32u*)m_pContext->m_pBufferStart + 2;
        GetNextPicHeader(m_pContext, isWMV);
        // skipping tools
        vc1Sts = SetDisplayIndex(m_pContext->m_picLayerHeader->PTYPE);
        if (m_pStore->IsNeedSkipFrame(m_pContext->m_picLayerHeader->PTYPE))
        {
            if ((m_pContext->m_picLayerHeader->PTYPE == VC1_B_FRAME) || (m_pContext->m_picLayerHeader->PTYPE == VC1_BI_FRAME))
                m_bIsSpecialBSkipFrame = true;
            m_pContext->m_picLayerHeader->PTYPE = VC1_SKIPPED_FRAME;
        }
    }
    m_pContext->m_bIntensityCompensation = 0;


    m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].m_bIsExpanded = 0;
    return vc1Sts;
}
Status VC1FrameDescriptor::SetDisplayIndex_Adv(Ipp32u                 PTYPE)
{
    Status vc1Sts = VC1_OK;

    switch(PTYPE)
    {
    case VC1_I_FRAME:
        {
            m_bIsWarningStream = false;
            m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetPrevIndex();
            m_pContext->m_frmBuff.m_iDisplayIndex = m_pStore->GetDisplayIndex();
            m_pContext->m_frmBuff.m_iCurrIndex = m_pStore->GetRefInternalIndex();
        }
        break;
    case VC1_P_FRAME:
        {
            m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetPrevIndex();
            if (m_pContext->m_frmBuff.m_iPrevIndex == -1)
            {
                m_bIsWarningStream = true;

                m_pContext->m_frmBuff.m_iPrevIndex = 0;
                m_pContext->m_frmBuff.m_iDisplayIndex = 0;
                memset(m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_pAllocatedMemory,
                    128,
                    m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_AllocatedMemorySize);

                //vc1Sts = UMC_WRN_INVALID_STREAM;
                throw vc1_exception(invalid_stream);
            }
            else
            {
                m_bIsWarningStream = false;
                m_pContext->m_frmBuff.m_iDisplayIndex = m_pStore->GetDisplayIndex();
                m_pContext->m_frmBuff.m_iCurrIndex = m_pStore->GetRefInternalIndex();
                *m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].pRANGE_MAPY =
                                                 m_pContext->m_picLayerHeader->RANGEREDFRM - 1;
            }
            m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].TFF = m_pContext->m_picLayerHeader->TFF;
        }
        break;
    case VC1_B_FRAME:
        {
            if (!m_bIsSpecialBSkipFrame)
            {
                m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetNextIndex();
                m_pContext->m_frmBuff.m_iNextIndex = m_pStore->GetPrevIndex();
            }
            else
            {
                m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetPrevIndex();
                m_pContext->m_frmBuff.m_iNextIndex = m_pStore->GetNextIndex();
            }

            if ((m_pContext->m_frmBuff.m_iNextIndex == -1)||(m_pContext->m_frmBuff.m_iPrevIndex == -1))
            {
                m_bIsWarningStream = true;

                if(m_pContext->m_frmBuff.m_iNextIndex == -1)
                {
                    m_pContext->m_frmBuff.m_iNextIndex = 0;
                    m_pContext->m_frmBuff.m_iPrevIndex = 1;
                    m_pContext->m_frmBuff.m_iCurrIndex = 2;
                    m_pContext->m_frmBuff.m_iDisplayIndex = 2;

                    memset(m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iNextIndex].m_pAllocatedMemory,
                        128,
                        m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iNextIndex].m_AllocatedMemorySize);

                    memset(m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_pAllocatedMemory,
                        128,
                        m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_AllocatedMemorySize);


                }
                else
                {
                    m_pContext->m_frmBuff.m_iPrevIndex = 0;
                    m_pContext->m_frmBuff.m_iCurrIndex = 2;
                    m_pContext->m_frmBuff.m_iDisplayIndex = 2;
                    memset(m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_pAllocatedMemory,
                        128,
                        m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_AllocatedMemorySize);
                }
                //vc1Sts = UMC_WRN_INVALID_STREAM;
                throw vc1_exception(invalid_stream);
            }
            else
            {
                m_bIsWarningStream = false;
                m_pContext->m_frmBuff.m_iCurrIndex = m_pStore->GetInternalIndex();
                m_pContext->m_frmBuff.m_iDisplayIndex = m_pContext->m_frmBuff.m_iCurrIndex;

                //vc1Sts = VC1_NOT_ENOUGH_DATA;
            }
        }
        break;
    case VC1_BI_FRAME:
        {
            m_bIsWarningStream = false;
            if (!m_bIsSpecialBSkipFrame)
            {
                m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetNextIndex();
                m_pContext->m_frmBuff.m_iNextIndex = m_pStore->GetPrevIndex();
            }
            else
            {
                m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetPrevIndex();
                m_pContext->m_frmBuff.m_iNextIndex = m_pStore->GetNextIndex();
            }
            //m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetNextIndex();
            //m_pContext->m_frmBuff.m_iNextIndex = m_pStore->GetPrevIndex();
            m_pContext->m_frmBuff.m_iCurrIndex = m_pStore->GetInternalIndex();
            m_pContext->m_frmBuff.m_iDisplayIndex = m_pContext->m_frmBuff.m_iCurrIndex;
           // vc1Sts = VC1_NOT_ENOUGH_DATA ;
        }
        break;
    case VC1_SKIPPED_FRAME:
        {
            m_bIsWarningStream = false;
            m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetPrevIndex();
            m_pContext->m_frmBuff.m_iDisplayIndex = m_pStore->GetDisplayIndex();
            m_pContext->m_frmBuff.m_iCurrIndex = m_pStore->GetRefInternalIndex();
        }
        break;
    default:
        break;
    }
    if (m_pContext->m_frmBuff.m_iDisplayIndex == -1)
        m_pContext->m_frmBuff.m_iDisplayIndex = m_pContext->m_frmBuff.m_iCurrIndex;

    m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].FCM = m_pContext->m_picLayerHeader->FCM;

    m_pContext->LumaTable[0] =  m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].LumaTable[0];
    m_pContext->LumaTable[1] =  m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].LumaTable[1];
    m_pContext->LumaTable[2] =  m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].LumaTable[2];
    m_pContext->LumaTable[3] =  m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].LumaTable[3];

    m_pContext->ChromaTable[0] = m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].ChromaTable[0];
    m_pContext->ChromaTable[1] = m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].ChromaTable[1];
    m_pContext->ChromaTable[2] = m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].ChromaTable[2];
    m_pContext->ChromaTable[3] = m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].ChromaTable[3];

    m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].ICFieldMask = 0;
    return vc1Sts;
}


Status VC1FrameDescriptor::SetDisplayIndex(Ipp32u  PTYPE)

{
    Status vc1Sts = VC1_OK;

    switch(PTYPE)
    {
    case VC1_I_FRAME:
        {
            m_bIsWarningStream = false;
            m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetPrevIndex();
            m_pContext->m_frmBuff.m_iDisplayIndex = m_pStore->GetDisplayIndex();
            m_pContext->m_frmBuff.m_iCurrIndex = m_pStore->GetRefInternalIndex();

            *m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].pRANGE_MAPY =
                m_pContext->m_picLayerHeader->RANGEREDFRM - 1;

        }
        break;
    case VC1_BI_FRAME:
        {
            m_bIsWarningStream = false;
            m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetNextIndex();
            m_pContext->m_frmBuff.m_iNextIndex = m_pStore->GetPrevIndex();
            m_pContext->m_frmBuff.m_iCurrIndex = m_pStore->GetInternalIndex();
            m_pContext->m_frmBuff.m_iDisplayIndex = m_pContext->m_frmBuff.m_iCurrIndex;

            *m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].pRANGE_MAPY =
                m_pContext->m_picLayerHeader->RANGEREDFRM - 1;
        }
        break;
    case VC1_P_FRAME:
        {
            m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetPrevIndex();

            if (m_pContext->m_frmBuff.m_iPrevIndex == -1)
            {
                m_pContext->m_frmBuff.m_iPrevIndex = 0;
                m_pContext->m_frmBuff.m_iDisplayIndex = 0;
                memset(m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_pAllocatedMemory,
                    128,
                    m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_AllocatedMemorySize);

                m_bIsWarningStream = true;
                throw vc1_exception(invalid_stream);
                //vc1Sts = UMC_WRN_INVALID_STREAM;
            }
            else
            {
                m_pContext->m_frmBuff.m_iDisplayIndex = m_pStore->GetDisplayIndex();
                m_pContext->m_frmBuff.m_iCurrIndex = m_pStore->GetRefInternalIndex();
                *m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].pRANGE_MAPY =
                                                 m_pContext->m_picLayerHeader->RANGEREDFRM - 1;
                m_bIsWarningStream = false;
            }
        }
        break;
    case VC1_B_FRAME:
        {
            m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetNextIndex();
            m_pContext->m_frmBuff.m_iNextIndex = m_pStore->GetPrevIndex();

            if ((m_pContext->m_frmBuff.m_iNextIndex == -1)||(m_pContext->m_frmBuff.m_iPrevIndex == -1))
            {
                m_bIsWarningStream = true;
                if(m_pContext->m_frmBuff.m_iNextIndex == -1)
                {
                    m_pContext->m_frmBuff.m_iNextIndex = 0;
                    m_pContext->m_frmBuff.m_iPrevIndex = 1;
                    memset(m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iNextIndex].m_pAllocatedMemory,
                        128,
                        m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iNextIndex].m_AllocatedMemorySize);

                    memset(m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_pAllocatedMemory,
                        128,
                        m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_AllocatedMemorySize);

                }
                else
                {
                    m_pContext->m_frmBuff.m_iPrevIndex = 1 - m_pContext->m_frmBuff.m_iNextIndex;
                    memset(m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_pAllocatedMemory,
                        128,
                        m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iPrevIndex].m_AllocatedMemorySize);
                }
                throw vc1_exception(invalid_stream);
                //vc1Sts = UMC_WRN_INVALID_STREAM;
            }
            else
            {
                m_bIsWarningStream = false;
                m_pContext->m_frmBuff.m_iCurrIndex = m_pStore->GetInternalIndex();
                m_pContext->m_frmBuff.m_iDisplayIndex = m_pContext->m_frmBuff.m_iCurrIndex;
                if (*m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iNextIndex].pRANGE_MAPY ==
                    (m_pContext->m_picLayerHeader->RANGEREDFRM - 1))
                    *m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].pRANGE_MAPY =
                    m_pContext->m_picLayerHeader->RANGEREDFRM - 1;
                else
                    *m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].pRANGE_MAPY =
                    *m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iNextIndex].pRANGE_MAPY;

               // vc1Sts = VC1_NOT_ENOUGH_DATA;
            }
        }
        break;
    case VC1_SKIPPED_FRAME:
        {
            m_bIsWarningStream = false;
            m_pContext->m_frmBuff.m_iPrevIndex = m_pStore->GetPrevIndex();
            m_pContext->m_frmBuff.m_iDisplayIndex = m_pStore->GetDisplayIndex();
            m_pContext->m_frmBuff.m_iCurrIndex = m_pStore->GetRefInternalIndex();
            *m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].pRANGE_MAPY =
                m_pContext->m_picLayerHeader->RANGEREDFRM - 1;
        }
        break;
    }

    m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].FCM = m_pContext->m_picLayerHeader->FCM;
    m_pContext->LumaTable[0] =  m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].LumaTable[0];
    m_pContext->LumaTable[1] =  m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].LumaTable[1];
    m_pContext->ChromaTable[0] = m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].ChromaTable[0];
    m_pContext->ChromaTable[1] = m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].ChromaTable[1];

    m_pContext->m_frmBuff.m_pFrames[m_pContext->m_frmBuff.m_iCurrIndex].ICFieldMask = 0;

    if (m_pContext->m_frmBuff.m_iDisplayIndex == -1)
        m_pContext->m_frmBuff.m_iDisplayIndex = m_pContext->m_frmBuff.m_iCurrIndex;
    return vc1Sts;
}
#endif
