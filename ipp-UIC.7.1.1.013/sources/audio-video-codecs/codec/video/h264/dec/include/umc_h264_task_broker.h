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

#ifndef __UMC_H264_TASK_BROKER_H
#define __UMC_H264_TASK_BROKER_H

#include <vector>
#include <list>

#include "umc_h264_dec_defs_dec.h"
#include "umc_h264_slice_decoding.h"
#include "umc_h264_heap.h"

namespace UMC
{
class H264DecoderFrameList;

class TaskBroker
{
public:
    TaskBroker(TaskSupplier * pTaskSupplier);

    virtual bool Init(Ipp32s iConsumerNumber, bool isExistMainThread);
    virtual ~TaskBroker();

    virtual bool AddFrameToDecoding(H264DecoderFrame * pFrame);

    virtual bool IsEnoughForStartDecoding(bool force);
    bool IsExistTasks(H264DecoderFrame * frame);

    // Get next working task
    virtual bool GetNextTask(H264Task *pTask);

    virtual void Reset();
    virtual void Release();

    // Task was performed
    virtual void AddPerformedTask(H264Task *pTask);

    virtual void Start();

    virtual bool PrepareFrame(H264DecoderFrame * pFrame);

    void Lock();
    void Unlock();

    TaskSupplier * m_pTaskSupplier;

protected:
    Ipp32s GetNumberOfTasks(bool details);
    bool IsFrameCompleted(H264DecoderFrame * pFrame) const;

    virtual bool GetNextTaskInternal(H264Task *pTask) = 0;

    bool GetNextSlice(H264DecoderFrameInfo * info, H264Task *pTask);
    bool GetNextSliceToDecoding(H264DecoderFrameInfo * info, H264Task *pTask);

    // Get number of slices to reconstruct
    Ipp32s GetNumberOfSlicesToReconstruct(H264DecoderFrameInfo * info, bool bOnlyReadySlices = false);

    // Get next available slice to deblocking
    bool GetNextSliceToDeblocking(H264DecoderFrameInfo * info, H264Task *pTask);

    bool GetPreparationTask(H264DecoderFrameInfo * info);

    // Get number of slices to decode
    Ipp32s GetNumberOfSlicesToDecode(H264DecoderFrameInfo * info);

    // Check current frame condition
    bool IsFrameDeblocked(H264DecoderFrameInfo * info);

    void InitTask(H264DecoderFrameInfo * info, H264Task *pTask, H264Slice *pSlice);

    void InitAUs();
    H264DecoderFrameInfo * FindAU();
    void SwitchCurrentAU();
    virtual void CompleteFrame(H264DecoderFrame * frame);

    Ipp32s m_iConsumerNumber;

    H264DecoderFrameInfo * m_FirstAU;

    std::vector<Event> m_eWaiting;                          // waiting threads events

    bool m_IsShouldQuit;
    
    virtual void AwakeThreads();

    typedef std::list<H264DecoderFrame *> FrameQueue;
    FrameQueue m_decodingQueue;
    FrameQueue m_completedQueue;

    Mutex m_mGuard;

    bool m_isExistMainThread;
};

class TaskBrokerSingleThread : public TaskBroker
{
public:
    TaskBrokerSingleThread(TaskSupplier * pTaskSupplier);

    // Get next working task
    virtual bool GetNextTaskInternal(H264Task *pTask);
};

class TaskBrokerTwoThread : public TaskBrokerSingleThread
{
public:

    TaskBrokerTwoThread(TaskSupplier * pTaskSupplier);

    virtual bool Init(Ipp32s iConsumerNumber, bool isExistMainThread);

    virtual bool GetNextTaskManySlices(H264DecoderFrameInfo * info, H264Task *pTask);

    // Get next working task
    virtual bool GetNextTaskInternal(H264Task *pTask);

    virtual void Release();
    virtual void Reset();

    virtual void AddPerformedTask(H264Task *pTask);

private:

    bool WrapDecodingTask(H264DecoderFrameInfo * info, H264Task *pTask, H264Slice *pSlice);
    bool WrapReconstructTask(H264DecoderFrameInfo * info, H264Task *pTask, H264Slice *pSlice);
    bool WrapDecRecTask(H264DecoderFrameInfo * info, H264Task *pTask, H264Slice *pSlice);

    bool GetDecRecTask(H264DecoderFrameInfo * info, H264Task *pTask);
    bool GetDecodingTask(H264DecoderFrameInfo * info, H264Task *pTask);
    bool GetReconstructTask(H264DecoderFrameInfo * info, H264Task *pTask);
    bool GetDeblockingTask(H264DecoderFrameInfo * info, H264Task *pTask);
    bool GetFrameDeblockingTaskThreaded(H264DecoderFrameInfo * info, H264Task *pTask);
#if defined (__ICL)
#pragma warning(disable:1125)
#endif
    void CompleteFrame();
};

} // namespace UMC

#endif // __UMC_H264_TASK_BROKER_H
