/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/
#include "umc_config.h"
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include <cstdarg>
#include "umc_h264_task_broker.h"
#include "umc_h264_segment_decoder_mt.h"
#include "umc_h264_heap.h"
#include "umc_h264_task_supplier.h"
#include "umc_h264_frame_list.h"


namespace UMC
{

void PrintInfoStatus(H264DecoderFrameInfo * info)
{
    if (!info)
        return;

    printf("needtoCheck - %d. status - %d\n", info->GetRefAU() != 0, info->GetStatus());
    for (Ipp32u i = 0; i < info->GetSliceCount(); i++)
    {
        printf("slice - %u\n", i);
        H264Slice * pSlice = info->GetSlice(i);
        printf("POC - (%d, %d), \n", info->m_pFrame->m_PicOrderCnt[0], info->m_pFrame->m_PicOrderCnt[1]);
        printf("cur to dec - %d\n", pSlice->m_iCurMBToDec);
        printf("cur to rec - %d\n", pSlice->m_iCurMBToRec);
        printf("cur to deb - %d\n", pSlice->m_iCurMBToDeb);
        printf("dec, rec, deb vacant - %d, %d, %d\n", pSlice->m_bDecVacant, pSlice->m_bRecVacant, pSlice->m_bDebVacant);
        printf("in/out buffers available - %d, %d\n", pSlice->m_CoeffsBuffers.IsInputAvailable(), pSlice->m_CoeffsBuffers.IsOutputAvailable());
        fflush(stdout);
    }
}

static
Ipp32s GetDecodingOffset(H264DecoderFrameInfo * curInfo, Ipp32s &readyCount)
{
    Ipp32s offset = 0;

    H264DecoderFrameInfo * refInfo = curInfo->GetRefAU();
    VM_ASSERT(refInfo);

    if (curInfo->m_pFrame != refInfo->m_pFrame)
    {
        switch(curInfo->m_pFrame->m_PictureStructureForDec)
        {
        case FRM_STRUCTURE:
            if (refInfo->m_pFrame->m_PictureStructureForDec == FLD_STRUCTURE)
            {
                readyCount *= 2;
                offset++;
            }
            else
            {
            }
            break;
        case AFRM_STRUCTURE:
            if (refInfo->m_pFrame->m_PictureStructureForDec == FLD_STRUCTURE)
            {
                readyCount *= 2;
                offset++;
            }
            else
            {
            }
            break;
        case FLD_STRUCTURE:
            switch(refInfo->m_pFrame->m_PictureStructureForDec)
            {
            case FLD_STRUCTURE:
                break;
            case AFRM_STRUCTURE:
            case FRM_STRUCTURE:
                readyCount /= 2;
                offset++;
                break;
            }
            break;
        }
    }

    return offset;
}

TaskBroker::TaskBroker(TaskSupplier * pTaskSupplier)
    : m_pTaskSupplier(pTaskSupplier)
    , m_iConsumerNumber(0)
    , m_FirstAU(0)
    , m_IsShouldQuit(false)
    , m_isExistMainThread(true)
{
    Release();
}

TaskBroker::~TaskBroker()
{
    Release();
}

bool TaskBroker::Init(Ipp32s iConsumerNumber, bool isExistMainThread)
{
    Release();

    m_eWaiting.clear();

    // we keep this variable due some optimizations purposes
    m_iConsumerNumber = iConsumerNumber;
    m_IsShouldQuit = false;
    m_isExistMainThread = isExistMainThread;

    m_eWaiting.resize(m_iConsumerNumber);

    // initilaize event(s)
    for (Ipp32s i = 0; i < m_iConsumerNumber; i += 1)
    {
        if (UMC_OK != m_eWaiting[i].Init(0, 0))
            return false;
    }

    return true;
}

void TaskBroker::Reset()
{
    AutomaticMutex guard(m_mGuard);
    m_FirstAU = 0;
    m_IsShouldQuit = true;

    for (FrameQueue::iterator iter = m_decodingQueue.begin(); iter != m_decodingQueue.end(); ++iter)
    {
        m_pTaskSupplier->UnlockFrameResource(*iter);
    }

    m_decodingQueue.clear();
    m_completedQueue.clear();

    // awake threads
    for (Ipp32s i = 0; i < m_iConsumerNumber; i += 1)
    {
        m_eWaiting[i].Set();
    }
}

void TaskBroker::Release()
{
    Reset();

    AwakeThreads();
}

void TaskBroker::Lock()
{
    m_mGuard.Lock();
    /*if ((m_mGuard.TryLock() != UMC_OK))
    {
        lock_failed++;
        m_mGuard.Lock();
    }*/
}

void TaskBroker::Unlock()
{
    m_mGuard.Unlock();
}

void TaskBroker::AwakeThreads()
{
    m_pTaskSupplier->m_threadGroup.AwakeThreads();
}

bool TaskBroker::PrepareFrame(H264DecoderFrame * pFrame)
{
    if (!pFrame || pFrame->m_iResourceNumber < 0)
    {
        return true;
    }

    if (pFrame->prepared[0] && pFrame->prepared[1])
        return true;

    H264DecoderFrame * resourceHolder = m_pTaskSupplier->IsBusyByFrame(pFrame->m_iResourceNumber);
    if (resourceHolder && resourceHolder != pFrame)
        return false;

    if (!m_pTaskSupplier->LockFrameResource(pFrame))
        return false;

    if (!pFrame->prepared[0] &&
        (pFrame->GetAU(0)->GetStatus() == H264DecoderFrameInfo::STATUS_FILLED || pFrame->GetAU(0)->GetStatus() == H264DecoderFrameInfo::STATUS_STARTED))
    {
        pFrame->prepared[0] = true;
    }

    if (!pFrame->prepared[1] &&
        (pFrame->GetAU(1)->GetStatus() == H264DecoderFrameInfo::STATUS_FILLED || pFrame->GetAU(1)->GetStatus() == H264DecoderFrameInfo::STATUS_STARTED))
    {
        pFrame->prepared[1] = true;
    }

    return true;
}

bool TaskBroker::GetPreparationTask(H264DecoderFrameInfo * info)
{
    H264DecoderFrame * pFrame = info->m_pFrame;

    if (info->m_prepared)
        return false;

    if (info != pFrame->GetAU(0) && pFrame->GetAU(0)->m_prepared != 2)
        return false;

    info->m_prepared = 1;

    Unlock();

    if (info == pFrame->GetAU(0))
    {
        Ipp32s iMBCount = pFrame->totalMBs << (pFrame->m_PictureStructureForDec < FRM_STRUCTURE ? 1 : 0);
        // allocate decoding data
        m_pTaskSupplier->AllocateMBInfo(pFrame->m_iResourceNumber, iMBCount);

        // allocate macroblock intra types
        m_pTaskSupplier->AllocateMBIntraTypes(pFrame->m_iResourceNumber, iMBCount);

        H264DecoderFrameInfo * slicesInfo = pFrame->GetAU(0);
        Ipp32s sliceCount = slicesInfo->GetSliceCount();
        for (Ipp32s i = 0; i < sliceCount; i++)
        {
            H264Slice * pSlice = slicesInfo->GetSlice(i);
            pSlice->InitializeContexts();
            pSlice->m_mbinfo = &m_pTaskSupplier->GetMBInfo(pFrame->m_iResourceNumber);
            pSlice->m_pMBIntraTypes = m_pTaskSupplier->GetIntraTypes(pFrame->m_iResourceNumber);
        }

        // reset frame global data
        if (slicesInfo->IsSliceGroups())
        {
            memset(pFrame->m_mbinfo.mbs, 0, iMBCount*sizeof(H264DecoderMacroblockGlobalInfo));
            memset(pFrame->m_mbinfo.MV[0], 0, iMBCount*sizeof(H264DecoderMacroblockMVs));
            memset(pFrame->m_mbinfo.MV[1], 0, iMBCount*sizeof(H264DecoderMacroblockMVs));
        }
        else
        {
            if (slicesInfo->m_isBExist && slicesInfo->m_isPExist)
            {
                for (Ipp32s i = 0; i < sliceCount; i++)
                {
                    H264Slice * pSlice = slicesInfo->GetSlice(i);
                    if (pSlice->GetSliceHeader()->slice_type == PREDSLICE)
                        memset(pFrame->m_mbinfo.MV[1] + pSlice->GetFirstMBNumber(), 0, pSlice->GetMBCount()*sizeof(H264DecoderMacroblockMVs));
                }
            }
        }

        if (slicesInfo->IsSliceGroups())
        {
            Lock();
            m_pTaskSupplier->SetMBMap(pFrame);
            Unlock();
        }
    }
    else
    {
        Ipp32s sliceCount = info->GetSliceCount();
        for (Ipp32s i = 0; i < sliceCount; i++)
        {
            H264Slice * pSlice = info->GetSlice(i);
            pSlice->InitializeContexts();
            pSlice->m_mbinfo = &m_pTaskSupplier->GetMBInfo(pFrame->m_iResourceNumber);
            pSlice->m_pMBIntraTypes = m_pTaskSupplier->GetIntraTypes(pFrame->m_iResourceNumber);
        }

        if (!info->IsSliceGroups() && info->m_isBExist && info->m_isPExist)
        {
            for (Ipp32s i = 0; i < sliceCount; i++)
            {
                H264Slice * pSlice = info->GetSlice(i);
                if (pSlice->GetSliceHeader()->slice_type == PREDSLICE)
                    memset(pFrame->m_mbinfo.MV[1] + pSlice->GetFirstMBNumber(), 0, pSlice->GetMBCount()*sizeof(H264DecoderMacroblockMVs));
            }
        }
    }

    Lock();
    info->m_prepared = 2;
    return false;
}

bool TaskBroker::AddFrameToDecoding(H264DecoderFrame * frame)
{
    if (frame->IsDecodingStarted() || !IsExistTasks(frame))
        return false;

    VM_ASSERT(frame->IsFrameExist());

    AutomaticMutex guard(m_mGuard);

    m_decodingQueue.push_back(frame);
    frame->StartDecoding();
    return true;
}

void TaskBroker::CompleteFrame(H264DecoderFrame * frame)
{
    if (!frame || m_decodingQueue.empty() || !frame->IsFullFrame())
        return;

    if (!IsFrameCompleted(frame) || frame->IsDecodingCompleted())
        return;

    m_pTaskSupplier->UnlockFrameResource(frame);
    if (frame == m_decodingQueue.front())
    {
        m_decodingQueue.pop_front();
    }
    else
    {
        m_decodingQueue.remove(frame);
    }

    frame->CompleteDecoding();

    if (m_isExistMainThread && m_iConsumerNumber > 1)
        m_completedQueue.push_back(frame);
}

void TaskBroker::SwitchCurrentAU()
{
    if (!m_FirstAU || !m_FirstAU->IsCompleted())
        return;

    while (m_FirstAU)
    {
        if (!IsFrameCompleted(m_FirstAU->m_pFrame))
        {
            if (m_FirstAU->IsCompleted())
            {
                m_FirstAU = m_FirstAU->GetNextAU();
                continue;
            }

            m_FirstAU->SetRefAU(0);

            H264DecoderFrameInfo * temp = m_FirstAU;
            while (temp)
            {
                temp->SetRefAU(0);

                if (temp->IsReference())
                    break;

                temp = temp->GetNextAU();
            }
            break;
        }

        H264DecoderFrameInfo* completed = m_FirstAU;
        m_FirstAU = m_FirstAU->GetNextAU();
        if (m_FirstAU && m_FirstAU->m_pFrame == completed->m_pFrame)
            m_FirstAU = m_FirstAU->GetNextAU();
        CompleteFrame(completed->m_pFrame);
    }

    InitAUs();

    if (m_FirstAU)
        AwakeThreads();
}

void TaskBroker::Start()
{
    AutomaticMutex guard(m_mGuard);

    FrameQueue::iterator iter = m_decodingQueue.begin();
    
    for (; iter != m_decodingQueue.end(); ++iter)
    {
        H264DecoderFrame * frame = *iter;

        if (m_FirstAU && m_FirstAU->m_pFrame == frame)
            break;

        if (IsFrameCompleted(frame))
        {
            CompleteFrame(frame);
            iter = m_decodingQueue.begin();
            if (iter == m_decodingQueue.end()) // avoid ++iter operation
                break;
        }
    }

    InitAUs();
    AwakeThreads();
}

H264DecoderFrameInfo * TaskBroker::FindAU()
{
    FrameQueue::iterator iter = m_decodingQueue.begin();
    FrameQueue::iterator end_iter = m_decodingQueue.end();
    
    for (; iter != end_iter; ++iter)
    {
        H264DecoderFrame * frame = *iter;

        H264DecoderFrameInfo *slicesInfo = frame->GetAU(0);

        if (slicesInfo->GetAllSliceCount())
        {
            if (slicesInfo->IsField())
            {
                if (slicesInfo->GetStatus() == H264DecoderFrameInfo::STATUS_FILLED)
                    return slicesInfo;

                if (frame->GetAU(1)->GetStatus() == H264DecoderFrameInfo::STATUS_FILLED)
                    return frame->GetAU(1);
            }
            else
            {
                if (slicesInfo->GetStatus() == H264DecoderFrameInfo::STATUS_FILLED)
                    return slicesInfo;
            }
        }

        if (UMC::GetAuxiliaryFrame(frame))
        {
            H264DecoderFrame *temp = UMC::GetAuxiliaryFrame(frame);

            H264DecoderFrameInfo *pSliceInfo = temp->GetAU(0);

            if (pSliceInfo->GetAllSliceCount())
            {
                if (pSliceInfo->IsField())
                {
                    if (pSliceInfo->GetStatus() == H264DecoderFrameInfo::STATUS_FILLED)
                        return pSliceInfo;

                    if (temp->GetAU(1)->GetStatus() == H264DecoderFrameInfo::STATUS_FILLED)
                        return temp->GetAU(1);
                }
                else
                {
                    if (pSliceInfo->GetStatus() == H264DecoderFrameInfo::STATUS_FILLED)
                        return pSliceInfo;
                }
            }
        }
    }

    return 0;
}

void TaskBroker::InitAUs()
{
    H264DecoderFrameInfo * pPrev;
    H264DecoderFrameInfo * refAU;
    H264DecoderFrameInfo * refAUAuxiliary;

    if (!m_FirstAU)
    {
        m_FirstAU = FindAU();
        if (!m_FirstAU)
            return;

        if (!PrepareFrame(m_FirstAU->m_pFrame))
        {
            m_FirstAU = 0;
            return;
        }

        m_FirstAU->SetStatus(H264DecoderFrameInfo::STATUS_STARTED);

        pPrev = m_FirstAU;
        m_FirstAU->SetPrevAU(0);
        m_FirstAU->SetNextAU(0);

        refAU = 0;
        refAUAuxiliary = 0;
        m_FirstAU->SetRefAU(0);
        if (m_FirstAU->IsReference())
        {
            if (m_FirstAU->IsAuxiliary())
                refAUAuxiliary = m_FirstAU;
            else
                refAU = m_FirstAU;
        }
    }
    else
    {
        pPrev = m_FirstAU;
        refAUAuxiliary = 0;
        refAU = 0;

        if (pPrev->IsReference())
        {
            if (pPrev->IsAuxiliary())
                refAUAuxiliary = pPrev;
            else
                refAU = pPrev;
        }

        while (pPrev->GetNextAU())
        {
            pPrev = pPrev->GetNextAU();

            if (pPrev->IsReference())
            {
                if (pPrev->IsAuxiliary())
                    refAUAuxiliary = pPrev;
                else
                    refAU = pPrev;
            }
        };
    }

    H264DecoderFrameInfo * pTemp = FindAU();
    for (; pTemp; )
    {
        if (!PrepareFrame(pTemp->m_pFrame))
        {
            pPrev->SetNextAU(0);
            break;
        }

        pTemp->SetStatus(H264DecoderFrameInfo::STATUS_STARTED);
        pTemp->SetNextAU(0);
        pTemp->SetPrevAU(pPrev);

        /*if (pTemp->IsIntraAU())
            pTemp->SetRefAU(0);
        else*/
        if (pTemp->IsAuxiliary())
            pTemp->SetRefAU(refAUAuxiliary);
        else
            pTemp->SetRefAU(refAU);

        if (pTemp->IsReference())
        {
            if (pTemp->IsAuxiliary())
                refAUAuxiliary = pTemp;
            else
                refAU = pTemp;
        }

        pPrev->SetNextAU(pTemp);
        pPrev = pTemp;
        pTemp = FindAU();
    }
}

bool TaskBroker::IsFrameCompleted(H264DecoderFrame * pFrame) const
{
    if (!pFrame)
        return true;

    if (!pFrame->GetAU(0)->IsCompleted())
        return false;

    //pFrame->GetAU(0)->SetStatus(H264DecoderFrameInfo::STATUS_COMPLETED); //quuu
    H264DecoderFrameInfo::FillnessStatus status1 = pFrame->GetAU(1)->GetStatus();

    bool ret;
    switch (status1)
    {
    case H264DecoderFrameInfo::STATUS_NONE:
        ret = true;
        break;
    case H264DecoderFrameInfo::STATUS_NOT_FILLED:
        ret = false;
        break;
    case H264DecoderFrameInfo::STATUS_COMPLETED:
        ret = true;
        break;
    default:
        ret = pFrame->GetAU(1)->IsCompleted();
        break;
    }

    if (ret && !pFrame->IsAuxiliaryFrame())
    {
        ret = IsFrameCompleted(UMC::GetAuxiliaryFrame(pFrame));
    }

    if (ret)
    {
        m_pTaskSupplier->UnlockFrameResource(pFrame);
    }

    return ret;
}

Ipp32s TaskBroker::GetNumberOfSlicesToReconstruct(H264DecoderFrameInfo * info, bool bOnlyReadySlices)
{
    Ipp32s i, iCount = 0;

    Ipp32s sliceCount = info->GetSliceCount();

    for (i = 0; i < sliceCount; i ++)
    {
        H264Slice *pSlice = info->GetSlice(i);

        if (pSlice->m_iMaxMB > pSlice->m_iCurMBToRec)
        {
            // we count up all decoding slices
            if ((false == bOnlyReadySlices) ||
            // or only ready to reconstruct
                ((pSlice->m_bRecVacant) && (pSlice->m_CoeffsBuffers.IsOutputAvailable())))
                iCount += 1;
        }
    }

    return iCount;

} // Ipp32s TaskBroker::GetNumberOfSlicesToReconstruct(void)

bool TaskBroker::IsFrameDeblocked(H264DecoderFrameInfo * info)
{
    // this is guarded function, safe to touch any variable

    Ipp32s i;

    // there is nothing to do
    Ipp32s sliceCount = info->GetSliceCount();
    if (0 == sliceCount)
        return true;

    for (i = 0; i < sliceCount; i += 1)
    {
        H264Slice *pSlice = info->GetSlice(i);

        if ((pSlice) &&
            (false == pSlice->m_bDeblocked))
            return false;
    }

    return true;

} // bool TaskBroker::IsFrameDeblocked(void)

Ipp32s TaskBroker::GetNumberOfSlicesToDecode(H264DecoderFrameInfo * info)
{
    Ipp32s i, iCount = 0;

    Ipp32s sliceCount = info->GetSliceCount();
    for (i = 0; i < sliceCount; i += 1)
    {
        H264Slice *pSlice = info->GetSlice(i);

        if (pSlice->m_iMaxMB > pSlice->m_iCurMBToDec)
            iCount += 1;
    }

    return iCount;

} // Ipp32s TaskBroker::GetNumberOfSlicesToDecode(void)

bool TaskBroker::GetNextTask(H264Task *pTask)
{
    AutomaticMutex guard(m_mGuard);

    pTask->m_taskPreparingGuard = &guard;

    // check error(s)
    if (/*!m_FirstAU ||*/ m_IsShouldQuit)
    {
        return false;
    }

    bool res = GetNextTaskInternal(pTask);
    return res;
} // bool TaskBroker::GetNextTask(H264Task *pTask)

bool TaskBroker::GetNextSlice(H264DecoderFrameInfo * info, H264Task *pTask)
{
    // this is guarded function, safe to touch any variable
    // check error(s)
    if (!info)
        return false;

    if (info->GetRefAU() != 0)
        return false;

    if (GetPreparationTask(info))
        return true;

    if (info->m_prepared != 2)
        return false;

    if (GetNextSliceToDecoding(info, pTask))
        return true;

    // try to get slice to decode
    /*if ((false == GetSliceFromCurrentFrame(0)->IsSliceGroups()) ||
        (0 == pTask->m_iThreadNumber))
    {
        if (GetNextSliceToDecoding(pTask))
            return true;
    }*/

    // try to get slice to deblock
    //if ((false == GetSliceFromCurrentFrame(0)->IsSliceGroups()) ||
      //  (0 == pTask->m_iThreadNumber))
    if (info->IsNeedDeblocking())
        return GetNextSliceToDeblocking(info, pTask);

    return false;
} // bool TaskBroker::GetNextSlice(H264Task *pTask)

void TaskBroker::InitTask(H264DecoderFrameInfo * info, H264Task *pTask, H264Slice *pSlice)
{
    pTask->m_bDone = false;
    pTask->m_bError = false;
    pTask->m_iMaxMB = pSlice->m_iMaxMB;
    pTask->m_pSlice = pSlice;
    pTask->m_pSlicesInfo = info;
}

bool TaskBroker::GetNextSliceToDecoding(H264DecoderFrameInfo * info, H264Task *pTask)
{
    // this is guarded function, safe to touch any variable

    Ipp32s i;
    bool bDoDeblocking;

    // skip some slices, more suitable for first thread
    // and first slice is always reserved for first slice decoder
    /*if (pTask->m_iThreadNumber)
    {
        i = IPP_MAX(1, GetNumberOfSlicesFromCurrentFrame() / m_iConsumerNumber);
        bDoDeblocking = false;
    }
    else
    {
        i = 0;
        bDoDeblocking = true;
    }*/

    i = 0;
    bDoDeblocking = false;

    // find first uncompressed slice
    Ipp32s sliceCount = info->GetSliceCount();
    for (; i < sliceCount; i += 1)
    {
        H264Slice *pSlice = info->GetSlice(i);

        if ((false == pSlice->m_bInProcess) &&
            (false == pSlice->m_bDecoded))
        {
            InitTask(info, pTask, pSlice);
            pTask->m_iFirstMB = pSlice->m_iFirstMB;
            pTask->m_iMBToProcess = IPP_MIN(pSlice->m_iMaxMB - pSlice->m_iFirstMB, pSlice->m_iAvailableMB);
            pTask->m_iTaskID = TASK_PROCESS;
            pTask->m_pBuffer = NULL;
            pTask->pFunction = &H264SegmentDecoderMultiThreaded::ProcessSlice;
            // we can do deblocking only on independent slices or
            // when all previous slices are deblocked
            if (DEBLOCK_FILTER_ON != pSlice->m_SliceHeader.disable_deblocking_filter_idc)
                bDoDeblocking = true;
            pSlice->m_bPrevDeblocked = bDoDeblocking;
            pSlice->m_bInProcess = true;
            pSlice->m_bDecVacant = 0;
            pSlice->m_bRecVacant = 0;
            pSlice->m_bDebVacant = 0;

            return true;
        }
    }

    return false;

} // bool TaskBroker::GetNextSliceToDecoding(H264Task *pTask)

bool TaskBroker::GetNextSliceToDeblocking(H264DecoderFrameInfo * info, H264Task *pTask)
{
    // this is guarded function, safe to touch any variable

    Ipp32s sliceCount = info->GetSliceCount();
    bool bSliceGroups = info->GetSlice(0)->IsSliceGroups();

    // slice group deblocking
    if (bSliceGroups)
    {
        Ipp32s iFirstMB = info->GetSlice(0)->m_iFirstMB;
        bool bNothingToDo = true;

        for (Ipp32s i = 0; i < sliceCount; i += 1)
        {
            H264Slice *pSlice = info->GetSlice(i);

            if (pSlice->m_bInProcess || !pSlice->m_bDecoded)
                return false;
        }

        for (Ipp32s i = 0; i < sliceCount; i += 1)
        {
            H264Slice *pSlice = info->GetSlice(i);

            VM_ASSERT(false == pSlice->m_bInProcess);

            pSlice->m_bInProcess = true;
            pSlice->m_bDebVacant = 0;
            iFirstMB = IPP_MIN(iFirstMB, pSlice->m_iFirstMB);
            if (false == pSlice->m_bDeblocked)
                bNothingToDo = false;
        }

        // we already deblocked
        if (bNothingToDo)
            return false;

        H264Slice *pSlice = info->GetSlice(0);
        InitTask(info, pTask, pSlice);
        pTask->m_iFirstMB = iFirstMB;
        Ipp32s iMBInFrame = (pSlice->m_iMBWidth * pSlice->m_iMBHeight) /
                            ((pSlice->m_SliceHeader.field_pic_flag) ? (2) : (1));
        pTask->m_iMaxMB = iFirstMB + iMBInFrame;
        pTask->m_iMBToProcess = iMBInFrame;

        pTask->m_iTaskID = TASK_DEB_FRAME;
        pTask->m_pBuffer = 0;
        pTask->pFunction = &H264SegmentDecoderMultiThreaded::DeblockSegmentTask;

        return true;
    }
    else
    {
        Ipp32s i;
        bool bPrevDeblocked = true;

        for (i = 0; i < sliceCount; i += 1)
        {
            H264Slice *pSlice = info->GetSlice(i);

            // we can do deblocking only on vacant slices
            if ((false == pSlice->m_bInProcess) &&
                (true == pSlice->m_bDecoded) &&
                (false == pSlice->m_bDeblocked))
            {
                // we can do this only when previous slice was deblocked or
                // deblocking isn't going through slice boundaries
                if ((true == bPrevDeblocked) ||
                    (false == pSlice->DeblockThroughBoundaries()))
                {
                    InitTask(info, pTask, pSlice);
                    pTask->m_iFirstMB = pSlice->m_iFirstMB;
                    pTask->m_iMBToProcess = pSlice->m_iMaxMB - pSlice->m_iFirstMB;
                    pTask->m_iTaskID = TASK_DEB_SLICE;
                    pTask->m_pBuffer = NULL;
                    pTask->pFunction = &H264SegmentDecoderMultiThreaded::DeblockSegmentTask;

                    pSlice->m_bPrevDeblocked = true;
                    pSlice->m_bInProcess = true;
                    pSlice->m_bDebVacant = 0;

                    return true;
                }
            }

            // save previous slices deblocking condition
            if (false == pSlice->m_bDeblocked)
                bPrevDeblocked = false;
        }
    }

    return false;

} // bool TaskBroker::GetNextSliceToDeblocking(H264Task *pTask)

void TaskBroker::AddPerformedTask(H264Task *pTask)
{
    AutomaticMutex guard(m_mGuard);

    H264Slice *pSlice = pTask->m_pSlice;
    H264DecoderFrameInfo * info = pTask->m_pSlicesInfo;
    
    // when whole slice was processed
    if (TASK_PROCESS == pTask->m_iTaskID)
    {
        // it is possible only in "slice group" mode
        if (pTask->m_pSlice->IsSliceGroups())
        {
            pSlice->m_iMaxMB = pTask->m_iMaxMB;
            pSlice->m_iAvailableMB -= pTask->m_iMBToProcess;

            /*
            // correct remain uncompressed macroblock count.
            // we can't relay on slice number cause of field pictures.
            if (pSlice->m_iAvailableMB)
            {
                Ipp32s pos = info->GetPositionByNumber(pSlice->GetSliceNum());
                VM_ASSERT(pos >= 0);
                H264Slice * pNextSlice = info->GetSlice(pos + 1);
                if (pNextSlice)
                {
                    //pNextSlice->m_iAvailableMB = pSlice->m_iAvailableMB;
                }
            }*/
        }

        // slice is deblocked only when deblocking was available
        if (false == pSlice->IsSliceGroups())
        {
            // check condition for frame deblocking
            //if (DEBLOCK_FILTER_ON_NO_SLICE_EDGES == pSlice->m_SliceHeader.disable_deblocking_filter_idc)
                //m_bDoFrameDeblocking = false; // DEBUG : ADB

            if (false == pSlice->m_bDeblocked)
                pSlice->m_bDeblocked = pSlice->m_bPrevDeblocked;
        }
        // slice is decoded
        pSlice->m_bDecoded = true;
        pSlice->m_bDecVacant = 0;
        pSlice->m_bRecVacant = 0;
        pSlice->m_bDebVacant = 1;
        pSlice->m_bInProcess = false;
    }
    else if (TASK_DEB_SLICE == pTask->m_iTaskID)
    {
        pSlice->m_bDebVacant = 1;
        pSlice->m_bDeblocked = 1;
        pSlice->m_bInProcess = false;
    }
    else if (TASK_DEB_FRAME == pTask->m_iTaskID)
    {
        Ipp32s sliceCount = m_FirstAU->GetSliceCount();

        // frame is deblocked
        for (Ipp32s i = 0; i < sliceCount; i += 1)
        {
            H264Slice *pTemp = m_FirstAU->GetSlice(i);

            pTemp->m_bDebVacant = 1;
            pTemp->m_bDeblocked = true;
            pTemp->m_bInProcess = false;
        }
    }
    else
    {
        switch (pTask->m_iTaskID)
        {
        case TASK_DEC:
            {
            VM_ASSERT(pTask->m_iFirstMB == pSlice->m_iCurMBToDec);

            pSlice->m_iCurMBToDec += pTask->m_iMBToProcess;
            // move filled buffer to reconstruct queue
            pSlice->m_CoeffsBuffers.UnLockInputBuffer(pTask->m_WrittenSize);
            pSlice->m_MVsDistortion = pTask->m_mvsDistortion;

            bool isReadyIncrease = (pTask->m_iFirstMB == info->m_iDecMBReady);
            if (isReadyIncrease)
            {
                info->m_iDecMBReady += pTask->m_iMBToProcess;
            }

            // error handling
            if (pTask->m_bError || pSlice->m_bError)
            {
                if (isReadyIncrease)
                    info->m_iDecMBReady = pSlice->m_iMaxMB;
                pSlice->m_iMaxMB = IPP_MIN(pSlice->m_iCurMBToDec, pSlice->m_iMaxMB);
                pSlice->m_bError = true;
            }
            else
            {
                pSlice->m_iMaxMB = pTask->m_iMaxMB;
            }

            if (pSlice->m_iCurMBToDec >= pSlice->m_iMaxMB)
            {
                pSlice->m_bDecVacant = 0;
                if (isReadyIncrease)
                {
                    Ipp32s pos = info->GetPositionByNumber(pSlice->GetSliceNum());
                    if (pos >= 0)
                    {
                        H264Slice * pNextSlice = info->GetSlice(++pos);
                        for (; pNextSlice; pNextSlice = info->GetSlice(++pos))
                        {
                           info->m_iDecMBReady = pNextSlice->m_iCurMBToDec;
                           if (pNextSlice->m_iCurMBToDec < pNextSlice->m_iMaxMB)
                               break;
                        }
                    }
                }
            }
            else
            {
                pSlice->m_bDecVacant = 1;
            }
            }
            break;

        case TASK_REC:
            {
            VM_ASSERT(pTask->m_iFirstMB == pSlice->m_iCurMBToRec);

            pSlice->m_iCurMBToRec += pTask->m_iMBToProcess;

            bool isReadyIncrease = (pTask->m_iFirstMB == info->m_iRecMBReady) && pSlice->m_bDeblocked;
            if (isReadyIncrease)
            {
                info->m_iRecMBReady += pTask->m_iMBToProcess;
            }

            // error handling
            if (pTask->m_bError || pSlice->m_bError)
            {
                if (isReadyIncrease)
                {
                    info->m_iRecMBReady = pSlice->m_iMaxMB;
                }

                pSlice->m_iMaxMB = IPP_MIN(pSlice->m_iCurMBToRec, pSlice->m_iMaxMB);
                pSlice->m_bError = true;
            }

            pSlice->m_CoeffsBuffers.UnLockOutputBuffer();

            if (pSlice->m_iMaxMB <= pSlice->m_iCurMBToRec)
            {
                if (isReadyIncrease)
                {
                    Ipp32s pos = info->GetPositionByNumber(pSlice->GetSliceNum());
                    if (pos >= 0)
                    {
                        H264Slice * pNextSlice = info->GetSlice(pos + 1);
                        if (pNextSlice)
                        {
                            if (pNextSlice->m_bDeblocked)
                                info->m_iRecMBReady = pNextSlice->m_iCurMBToRec;
                            else
                                info->m_iRecMBReady = pNextSlice->m_iCurMBToDeb;

                            info->RemoveSlice(pos);
                        }
                    }
                }

                pSlice->m_bRecVacant = 0;
                pSlice->m_bDecoded = true;

                // check condition for frame deblocking
                //if (DEBLOCK_FILTER_ON_NO_SLICE_EDGES == pSlice->m_SliceHeader.disable_deblocking_filter_idc)
                //    m_bDoFrameDeblocking = false;  // DEBUG : ADB
            }
            else
            {
                pSlice->m_bRecVacant = 1;
            }
            }
            break;

        case TASK_DEC_REC:
            {
            VM_ASSERT(pTask->m_iFirstMB == pSlice->m_iCurMBToDec);
            VM_ASSERT(pTask->m_iFirstMB == pSlice->m_iCurMBToRec);

            pSlice->m_iCurMBToDec += pTask->m_iMBToProcess;
            pSlice->m_iCurMBToRec += pTask->m_iMBToProcess;

            bool isReadyIncreaseDec = (pTask->m_iFirstMB == info->m_iDecMBReady);
            bool isReadyIncreaseRec = (pTask->m_iFirstMB == info->m_iRecMBReady);

            pSlice->m_iMaxMB = pTask->m_iMaxMB;

            if (isReadyIncreaseDec)
                info->m_iDecMBReady += pTask->m_iMBToProcess;

            if (isReadyIncreaseRec && pSlice->m_bDeblocked)
            {
                info->m_iRecMBReady += pTask->m_iMBToProcess;
            }

            // error handling
            if (pTask->m_bError || pSlice->m_bError)
            {
                if (isReadyIncreaseDec)
                    info->m_iDecMBReady = pSlice->m_iMaxMB;

                if (isReadyIncreaseRec && pSlice->m_bDeblocked)
                    info->m_iRecMBReady = pSlice->m_iMaxMB;

                pSlice->m_iMaxMB = IPP_MIN(pSlice->m_iCurMBToDec, pSlice->m_iMaxMB);
                pSlice->m_iCurMBToRec = pSlice->m_iCurMBToDec;
                pSlice->m_bError = true;
            }

            if (pSlice->m_iCurMBToDec >= pSlice->m_iMaxMB)
            {
                VM_ASSERT(pSlice->m_iCurMBToRec >= pSlice->m_iMaxMB);

                if (isReadyIncreaseDec || isReadyIncreaseRec)
                {
                    Ipp32s pos = info->GetPositionByNumber(pSlice->GetSliceNum());
                    VM_ASSERT(pos >= 0);
                    H264Slice * pNextSlice = info->GetSlice(pos + 1);
                    if (pNextSlice)
                    {
                        if (isReadyIncreaseDec)
                        {
                            Ipp32s pos1 = pos;
                            H264Slice * pTmpSlice = info->GetSlice(++pos1);

                            for (; pTmpSlice; pTmpSlice = info->GetSlice(++pos1))
                            {
                               info->m_iDecMBReady = pTmpSlice->m_iCurMBToDec;
                               if (pTmpSlice->m_iCurMBToDec < pTmpSlice->m_iMaxMB)
                                   break;
                            }
                        }

                        if (isReadyIncreaseRec && pSlice->m_bDeblocked)
                        {
                            if (pNextSlice->m_bDeblocked)
                                info->m_iRecMBReady = pNextSlice->m_iCurMBToRec;
                            else
                                info->m_iRecMBReady = pNextSlice->m_iCurMBToDeb;

                            info->RemoveSlice(pos);
                        }
                    }
                }

                pSlice->m_bDecVacant = 0;
                pSlice->m_bRecVacant = 0;
                pSlice->m_bDecoded = true;
            }
            else
            {
                pSlice->m_bDecVacant = 1;
                pSlice->m_bRecVacant = 1;
            }
            }
            break;

        case TASK_DEB:
            {
            VM_ASSERT(pTask->m_iFirstMB == pSlice->m_iCurMBToDeb);

            pSlice->m_iCurMBToDeb += pTask->m_iMBToProcess;

            bool isReadyIncrease = (pTask->m_iFirstMB == info->m_iRecMBReady);
            if (isReadyIncrease)
            {
                info->m_iRecMBReady += pTask->m_iMBToProcess;
            }

            if (pSlice->m_iMaxMB <= pSlice->m_iCurMBToDeb)
            {
                Ipp32s pos = info->GetPositionByNumber(pSlice->GetSliceNum());
                if (isReadyIncrease)
                {
                    VM_ASSERT(pos >= 0);
                    H264Slice * pNextSlice = info->GetSlice(pos + 1);
                    if (pNextSlice)
                    {
                        if (pNextSlice->m_bDeblocked)
                            info->m_iRecMBReady = pNextSlice->m_iCurMBToRec;
                        else
                            info->m_iRecMBReady = pNextSlice->m_iCurMBToDeb;
                    }
                }

                info->RemoveSlice(pos);

                pSlice->m_bDeblocked = true;
                pSlice->m_bInProcess = false;
                pSlice->m_bDecVacant = 0;
                pSlice->m_bRecVacant = 0;
            }
            else
            {
                pSlice->m_bDebVacant = 1;
            }
            }
            break;

        default:
            // illegal case
            VM_ASSERT(false);
            break;
        }
    }

} // void TaskBroker::AddPerformedTask(H264Task *pTask)

#define Check_Status(status)  ((status == H264DecoderFrameInfo::STATUS_FILLED) || (status == H264DecoderFrameInfo::STATUS_STARTED))

Ipp32s TaskBroker::GetNumberOfTasks(bool details)
{
    Ipp32s au_count = 0;

    H264DecoderFrameInfo * temp = m_FirstAU;

    for (; temp ; temp = temp->GetNextAU())
    {
        if (temp->GetStatus() == H264DecoderFrameInfo::STATUS_COMPLETED)
            continue;

        if (details)
        {
            au_count += temp->GetSliceCount();
        }
        else
        {
            if (!temp->IsField())
            {
                au_count++;
            }

            au_count++;
        }
    }

    return au_count;
}

bool TaskBroker::IsEnoughForStartDecoding(bool force)
{
    AutomaticMutex guard(m_mGuard);

    InitAUs();
    Ipp32s au_count = GetNumberOfTasks(false);

    Ipp32s additional_tasks = m_iConsumerNumber;
    if (m_iConsumerNumber > 6)
        additional_tasks -= 1;

    if ((au_count >> 1) >= additional_tasks || (force && au_count))
        return true;

    return false;
}

bool TaskBroker::IsExistTasks(H264DecoderFrame * frame)
{
    H264DecoderFrameInfo *slicesInfo = frame->GetAU(0);

    return Check_Status(slicesInfo->GetStatus());
}

TaskBrokerSingleThread::TaskBrokerSingleThread(TaskSupplier * pTaskSupplier)
    : TaskBroker(pTaskSupplier)
{
}

// Get next working task
bool TaskBrokerSingleThread::GetNextTaskInternal(H264Task *pTask)
{
    while (false == m_IsShouldQuit)
    {
        if (!m_FirstAU)
        {
            if (m_isExistMainThread)
                break;
        }
        else
        {
            if (GetNextSlice(m_FirstAU, pTask))
                return true;

            if (IsFrameCompleted(m_FirstAU->m_pFrame))
            {
                SwitchCurrentAU();

                if (m_isExistMainThread)
                {
                    return false;
                }
            }
            else
            {
                if (m_FirstAU->IsCompleted())
                {
                    SwitchCurrentAU();
                }
            }

            if (GetNextSlice(m_FirstAU, pTask))
                return true;
        }

        break;
    }

    return false;
}

TaskBrokerTwoThread::TaskBrokerTwoThread(TaskSupplier * pTaskSupplier)
    : TaskBrokerSingleThread(pTaskSupplier)
{
}

bool TaskBrokerTwoThread::Init(Ipp32s iConsumerNumber, bool isExistMainThread)
{
    if (!TaskBroker::Init(iConsumerNumber, isExistMainThread))
        return false;

    return true;
}

void TaskBrokerTwoThread::Reset()
{
    AutomaticMutex guard(m_mGuard);

    TaskBrokerSingleThread::Reset();
}

void TaskBrokerTwoThread::Release()
{
    AutomaticMutex guard(m_mGuard);

    TaskBrokerSingleThread::Release();
}

void TaskBrokerTwoThread::CompleteFrame()
{
    if (!m_FirstAU)
        return;

    SwitchCurrentAU();
}

bool TaskBrokerTwoThread::GetNextTaskInternal(H264Task *pTask)
{
    while (false == m_IsShouldQuit)
    {
        if (m_isExistMainThread)
        {
            if (!pTask->m_iThreadNumber && !m_completedQueue.empty())
            {
                CompleteFrame(); // here SwitchAU and awake
                m_completedQueue.clear();
                return false;
            }
        }

        if (!m_FirstAU)
        {
            if (m_isExistMainThread)
            {
                if (!pTask->m_iThreadNumber)
                {
                    SwitchCurrentAU();
                    m_completedQueue.clear();
                    return false;
                }
                if(GetNumberOfTasks(true))
                    AwakeThreads();
            }
        }
        else
        {
            for (H264DecoderFrameInfo *pTemp = m_FirstAU; pTemp; pTemp = pTemp->GetNextAU())
            {
                if (GetNextTaskManySlices(pTemp, pTask))
                {
                    return true;
                }
            }
        }

        break;
    };

    if (!pTask->m_iThreadNumber && m_isExistMainThread) // check for slice groups only. need to review
    {
        m_completedQueue.clear();
    }

    return false;
}

bool TaskBrokerTwoThread::WrapDecodingTask(H264DecoderFrameInfo * info, H264Task *pTask, H264Slice *pSlice)
{
    VM_ASSERT(pSlice);

    // this is guarded function, safe to touch any variable
    if ((1 == pSlice->m_bDecVacant) &&
        (pSlice->m_CoeffsBuffers.IsInputAvailable()))
    {
        pSlice->m_bInProcess = true;
        pSlice->m_bDecVacant = 0;

        pTask->m_taskPreparingGuard->Unlock();
        Ipp32s iMBWidth = pSlice->GetMBRowWidth();

        // error handling
        /*if (0 >= ((signed) pSlice->m_BitStream.BytesLeft()))
        {
            pSlice->m_iMaxMB = pSlice->m_iCurMBToDec;
            return false;
        }*/

        InitTask(info, pTask, pSlice);
        pTask->m_iFirstMB = pSlice->m_iCurMBToDec;
        pTask->m_WrittenSize = 0;
        pTask->m_iMBToProcess = IPP_MIN(pSlice->m_iCurMBToDec -
                                    (pSlice->m_iCurMBToDec % iMBWidth) +
                                    iMBWidth,
                                    pSlice->m_iMaxMB) - pSlice->m_iCurMBToDec;

        pTask->m_iMBToProcess = IPP_MIN(pTask->m_iMBToProcess, pSlice->m_iAvailableMB);
        pTask->m_iTaskID = TASK_DEC;
        pTask->m_pBuffer = (UMC::CoeffsPtrCommon)pSlice->m_CoeffsBuffers.LockInputBuffer();
        pTask->pFunction = &H264SegmentDecoderMultiThreaded::DecodeSegment;

        return true;
    }

    return false;

} // bool TaskBrokerTwoThread::WrapDecodingTask(H264Task *pTask, H264Slice *pSlice)

bool TaskBrokerTwoThread::WrapReconstructTask(H264DecoderFrameInfo * info, H264Task *pTask, H264Slice *pSlice)
{
    VM_ASSERT(pSlice);
    // this is guarded function, safe to touch any variable

    if ((1 == pSlice->m_bRecVacant) &&
        (pSlice->m_CoeffsBuffers.IsOutputAvailable()))
    {
        pSlice->m_bRecVacant = 0;

        Ipp32s iMBWidth = pSlice->GetMBRowWidth();

        InitTask(info, pTask, pSlice);
        pTask->m_taskPreparingGuard->Unlock();

        pTask->m_iFirstMB = pSlice->m_iCurMBToRec;
        pTask->m_iMBToProcess = IPP_MIN(pSlice->m_iCurMBToRec -
                                    (pSlice->m_iCurMBToRec % iMBWidth) +
                                    iMBWidth,
                                    pSlice->m_iMaxMB) - pSlice->m_iCurMBToRec;

        pTask->m_iTaskID = TASK_REC;
        Ipp8u* pointer;
        size_t size;
        pSlice->m_CoeffsBuffers.LockOutputBuffer(pointer, size);
        pTask->m_pBuffer = ((UMC::CoeffsPtrCommon) pointer);
        pTask->pFunction = &H264SegmentDecoderMultiThreaded::ReconstructSegment;

        return true;
    }

    return false;

} // bool TaskBrokerTwoThread::WrapReconstructTask(H264Task *pTaskm H264Slice *pSlice)

bool TaskBrokerTwoThread::WrapDecRecTask(H264DecoderFrameInfo * info, H264Task *pTask, H264Slice *pSlice)
{
    VM_ASSERT(pSlice);
    // this is guarded function, safe to touch any variable

    if ((1 == pSlice->m_bRecVacant) && (1 == pSlice->m_bDecVacant) &&
        (pSlice->m_iCurMBToDec == pSlice->m_iCurMBToRec) &&
        (pSlice->m_CoeffsBuffers.IsInputAvailable()))
    {
        pSlice->m_bRecVacant = 0;
        pSlice->m_bDecVacant = 0;

        pTask->m_taskPreparingGuard->Unlock();
        Ipp32s iMBWidth = pSlice->GetMBRowWidth();

        InitTask(info, pTask, pSlice);
        pTask->m_iFirstMB = pSlice->m_iCurMBToDec;
        pTask->m_WrittenSize = 0;
        pTask->m_iMBToProcess = IPP_MIN(pSlice->m_iCurMBToDec -
                                    (pSlice->m_iCurMBToDec % iMBWidth) +
                                    iMBWidth,
                                    pSlice->m_iMaxMB) - pSlice->m_iCurMBToDec;

        pTask->m_iTaskID = TASK_DEC_REC;
        pTask->m_pBuffer = 0;
        pTask->pFunction = &H264SegmentDecoderMultiThreaded::DecRecSegment;

        return true;
    }

    return false;

} // bool TaskBrokerTwoThread::WrapDecRecTask(H264Task *pTaskm H264Slice *pSlice)

bool TaskBrokerTwoThread::GetDecodingTask(H264DecoderFrameInfo * info, H264Task *pTask)
{
    // this is guarded function, safe to touch any variable
    Ipp32s i;

    H264DecoderFrameInfo * refAU = info->GetRefAU();
    bool is_need_check = refAU != 0;
    Ipp32s readyCount = 0;
    Ipp32s additionalLines = 0;

    if (is_need_check)
    {
        if (refAU->GetStatus() == H264DecoderFrameInfo::STATUS_COMPLETED)
        {
            is_need_check = false;
        }
        else
        {
            readyCount = refAU->m_iDecMBReady;
            additionalLines = GetDecodingOffset(info, readyCount);
        }
    }

    Ipp32s sliceCount = info->GetSliceCount();
    for (i = 0; i < sliceCount; i += 1)
    {
        H264Slice *pSlice = info->GetSlice(i);

        if (pSlice->m_bDecVacant != 1)
            continue;

        if (is_need_check)
        {
            if (pSlice->m_iCurMBToDec + (1 + additionalLines)*pSlice->GetMBRowWidth() > readyCount)
                break;
        }

        if (WrapDecodingTask(info, pTask, pSlice))
            return true;
    }

    return false;

} // bool TaskBrokerTwoThread::GetDecodingTask(H264DecoderFrameInfo * info, H264Task *pTask)

bool TaskBrokerTwoThread::GetReconstructTask(H264DecoderFrameInfo * info, H264Task *pTask)
{
    // this is guarded function, safe to touch any variable

    H264DecoderFrameInfo * refAU = info->GetRefAU();
    bool is_need_check = refAU != 0;
    Ipp32s readyCount = 0;
    Ipp32s additionalLines = 0;

    if (is_need_check)
    {
        if (refAU->GetStatus() == H264DecoderFrameInfo::STATUS_COMPLETED)
        {
            is_need_check = false;
        }
        else
        {
            readyCount = refAU->m_iRecMBReady;
            additionalLines = GetDecodingOffset(info, readyCount);
        }
    }

    Ipp32s i;
    Ipp32s sliceCount = info->GetSliceCount();
    for (i = 0; i < sliceCount; i += 1)
    {
        H264Slice *pSlice = info->GetSlice(i);

        if (pSlice->m_bRecVacant != 1 ||
            !pSlice->m_CoeffsBuffers.IsOutputAvailable())
        {
            continue;
        }

        if (is_need_check)
        {
            Ipp32s k = (( (pSlice->m_MVsDistortion + 15) / 16) + 1); // +2 - (1 - for padding, 2 - current line)
            k += refAU->IsNeedDeblocking() ? 1 : 0;
            if (pSlice->m_iCurMBToRec + (k + additionalLines)*pSlice->GetMBRowWidth() >= readyCount)
                break;
        }

        if (WrapReconstructTask(info, pTask, pSlice))
        {
            return true;
        }
    }

    return false;

} // bool TaskBrokerTwoThread::GetReconstructTask(H264Task *pTask)

bool TaskBrokerTwoThread::GetDecRecTask(H264DecoderFrameInfo * info, H264Task *pTask)
{
    // this is guarded function, safe to touch any variable
    if (info->GetRefAU() || info->GetSliceCount() == 1)
        return false;

    Ipp32s i;
    Ipp32s sliceCount = info->GetSliceCount();
    for (i = 0; i < sliceCount; i += 1)
    {
        H264Slice *pSlice = info->GetSlice(i);

        if (pSlice->m_bRecVacant != 1 || pSlice->m_bDecVacant != 1)
        {
            continue;
        }

        if (WrapDecRecTask(info, pTask, pSlice))
        {
            return true;
        }
    }

    return false;
}

bool TaskBrokerTwoThread::GetDeblockingTask(H264DecoderFrameInfo * info, H264Task *pTask)
{
    // this is guarded function, safe to touch any variable
    Ipp32s i;
    bool bPrevDeblocked = true;

    Ipp32s sliceCount = info->GetSliceCount();
    for (i = 0; i < sliceCount; i += 1)
    {
        H264Slice *pSlice = info->GetSlice(i);

        Ipp32s iMBWidth = pSlice->GetMBRowWidth(); // DEBUG : always deblock two lines !!!
        Ipp32s iAvailableToDeblock;
        Ipp32s iDebUnit = (pSlice->GetSliceHeader()->MbaffFrameFlag) ? (2) : (1);

        iAvailableToDeblock = pSlice->m_iCurMBToRec -
                              pSlice->m_iCurMBToDeb;

        if ((false == pSlice->m_bDeblocked) &&
            ((true == bPrevDeblocked) || (false == pSlice->DeblockThroughBoundaries())) &&
            (1 == pSlice->m_bDebVacant) &&
            ((iAvailableToDeblock > iMBWidth) || (pSlice->m_iCurMBToRec >= pSlice->m_iMaxMB)))
        {
            pSlice->m_bDebVacant = 0;

            pTask->m_taskPreparingGuard->Unlock();
            InitTask(info, pTask, pSlice);
            pTask->m_iFirstMB = pSlice->m_iCurMBToDeb;

            {
                pTask->m_iMBToProcess = IPP_MIN(iMBWidth - (pSlice->m_iCurMBToDeb % iMBWidth),
                                            iAvailableToDeblock);
                pTask->m_iMBToProcess = IPP_MAX(pTask->m_iMBToProcess,
                                            iDebUnit);
                pTask->m_iMBToProcess = align_value<Ipp32s> (pTask->m_iMBToProcess, iDebUnit);
            }

            pTask->m_iTaskID = TASK_DEB;
            pTask->pFunction = &H264SegmentDecoderMultiThreaded::DeblockSegmentTask;

            return true;
        }
        else
        {
            if ((0 >= iAvailableToDeblock) && (pSlice->m_iCurMBToRec >= pSlice->m_iMaxMB))
            {
                pSlice->m_bDeblocked = true;
                bPrevDeblocked = true;
            }
        }

        // save previous slices deblocking condition
        if (false == pSlice->m_bDeblocked || pSlice->m_iCurMBToRec < pSlice->m_iMaxMB)
        {
            bPrevDeblocked = false;
            break; // for mbaff deblocking could be performaed outside boundaries.
        }
    }

    return false;

} // bool TaskBrokerTwoThread::GetDeblockingTask(H264Task *pTask)

bool TaskBrokerTwoThread::GetNextTaskManySlices(H264DecoderFrameInfo * info, H264Task *pTask)
{
    if (!info)
        return false;

    if (GetPreparationTask(info))
        return true;

    if (info->m_prepared != 2)
        return false;

    if (info->IsSliceGroups())
        return GetNextSlice(info, pTask);

    if (info->IsNeedDeblocking())
    {
        if (true == GetDeblockingTask(info, pTask))
            return true;
    }

    // try to get reconstruct task from main queue
    if (GetReconstructTask(info, pTask))
        return true;

    if (GetDecRecTask(info, pTask))
        return true;

    // try to get decoding task from main frame
    if(GetDecodingTask(info, pTask))
        return true;

    return false;
}

void TaskBrokerTwoThread::AddPerformedTask(H264Task *pTask)
{
    AutomaticMutex guard(m_mGuard);

    TaskBrokerSingleThread::AddPerformedTask(pTask);

    if (pTask->m_pSlicesInfo->IsCompleted())
    {
        pTask->m_pSlicesInfo->SetStatus(H264DecoderFrameInfo::STATUS_COMPLETED);
    }

    SwitchCurrentAU();
    AwakeThreads();
} // void TaskBrokerTwoThread::AddPerformedTask(H264Task *pTask)

bool TaskBrokerTwoThread::GetFrameDeblockingTaskThreaded(H264DecoderFrameInfo * , H264Task *)
{
    return false;

} // bool TaskBrokerTwoThread::GetFrameDeblockingTaskThreaded(H264Task *pTask)

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
