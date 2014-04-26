/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_thread.h"
#include "vm_thread.h"
#include "vm_event.h"
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_dec_task_store.h"
#include "umc_vc1_dec_job.h"



namespace UMC
{

VC1ThreadDecoder::VC1ThreadDecoder()
{
    m_pMemoryAllocator = NULL;
    m_pStore = NULL;
    m_pJobSlice = NULL;
    vm_thread_set_invalid(&m_hThread);
    vm_event_set_invalid(&m_hStartProcessing);
    vm_event_set_invalid(&m_hDoneProcessing);

    m_bQuit = false;
    m_bStartDecoding = false;

}

VC1ThreadDecoder::~VC1ThreadDecoder(void)
{
    Release();

}

void VC1ThreadDecoder::Release(void)
{
    if (m_pJobSlice)
        m_pJobSlice->~VC1TaskProcessor();

    if (m_pJobSlice != &m_TaskProcessor)
    {
        delete m_pJobSlice;
        m_pJobSlice = NULL;
    }


    // threading tools
    if (vm_thread_is_valid(&m_hThread))
    {
        m_bQuit = true;
        vm_event_signal(&m_hStartProcessing);

        vm_thread_wait(&m_hThread);
        vm_thread_close(&m_hThread);
    }

    vm_event_destroy(&m_hStartProcessing);
    vm_event_destroy(&m_hDoneProcessing);

    m_bQuit = false;

}

Status VC1ThreadDecoder::Init(VC1Context* pContext,
                              Ipp32s iNumber,
                              VC1TaskStore* pTaskStore,
                              VideoAccelerator* va,
                              MemoryAllocator* pMemoryAllocator,
                              VC1TaskProcessor* pExternalProcessor)
{
//    static Ipp32u mask[4] = {2,32,64,128};
#ifdef WINDOWS
    static Ipp32u mask[4] = {2,1,2,1};
#endif
//    static Ipp32u proc_mask[4] = {1,2,1,2};
    vm_status vmRes;

    // release object before initialization
    Release();

    // save thread number(s)
    m_iNumber = iNumber;
    m_pMemoryAllocator = pMemoryAllocator;

    // save pointer to TaskStore
    m_pStore = pTaskStore;
     m_pJobSlice = &m_TaskProcessor;


    if (NULL == m_pJobSlice)
        return UMC_ERR_ALLOC;

    if (!pExternalProcessor)
    {
        if (UMC_OK != m_TaskProcessor.Init(pContext,iNumber,pTaskStore,m_pMemoryAllocator))
            return UMC_ERR_INIT;
    }

    // threading tools
    m_bQuit = false;
    if (iNumber)
    {
        // initialize working events
        vmRes = vm_event_init(&m_hStartProcessing, 0, 0);
        if (VM_OK != vmRes)
            return UMC_ERR_INIT;
        vmRes = vm_event_init(&m_hDoneProcessing, 0, 0);
        if (VM_OK != vmRes)
            return UMC_ERR_INIT;

        // start decoding thread
        {
            Ipp32u res;
            res = vm_thread_create(&m_hThread, (vm_thread_callback)DecodingThreadRoutine, this);
            if (0 == res)
                return UMC_ERR_INIT;
        }
    }

#ifdef WINDOWS
            //SetThreadIdealProcessor(m_hThread.handle,2);
            SetThreadAffinityMask(m_hThread.handle,mask[iNumber]);
            //SetProcessAffinityMask(m_hThread.handle,mask[iNumber]);
#endif

    return UMC_OK;

}

Status VC1ThreadDecoder::StartProcessing(void)
{
    m_bStartDecoding = true;

    if (0 == vm_event_is_valid(&m_hStartProcessing))
        return UMC_ERR_FAILED;

    vm_event_reset(&m_hDoneProcessing);
    vm_event_signal(&m_hStartProcessing);

    return UMC_OK;

}

Status VC1ThreadDecoder::WaitForEndOfProcessing(void)
{

    if (0 == vm_event_is_valid(&m_hDoneProcessing))
        return UMC_ERR_FAILED;

    m_pStore->WakeUP();

    if (m_bStartDecoding)
        vm_event_wait(&m_hDoneProcessing);

    m_bStartDecoding = false;

    return m_Status;

}

Status VC1ThreadDecoder::WaitAndStop(void)
{
    if (0 == vm_event_is_valid(&m_hDoneProcessing))
        return UMC_ERR_FAILED;

    if (0 == vm_event_is_valid(&m_hStartProcessing))
        return UMC_ERR_FAILED;

    m_pStore->WakeUP();

    if (m_bStartDecoding)
        vm_event_wait(&m_hDoneProcessing);

    m_bStartDecoding = false;

    return UMC_OK;

}

Status VC1ThreadDecoder::process(void)
{
    return m_pJobSlice->process();
}
Status VC1ThreadDecoder::processMainThread(void)
{
    return m_pJobSlice->processMainThread();
}

Ipp32u VC1ThreadDecoder::DecodingThreadRoutine(void *p)
{
    VC1ThreadDecoder *pObj = (VC1ThreadDecoder *) p;

    // check error(s)
    if (NULL == p)
        return 0x0baad;

    {
        vm_event (&hStartProcessing)(pObj->m_hStartProcessing);
        vm_event (&hDoneProcessing)(pObj->m_hDoneProcessing);

        // wait for begin decoding
        vm_event_wait(&hStartProcessing);

        while (false == pObj->m_bQuit)
        {
            pObj->m_Status = pObj->process();

            // set done event
            vm_event_signal(&hDoneProcessing);

            // wait for begin decoding
            vm_event_wait(&hStartProcessing);
        }
    }

    return 0x0264dec0 + pObj->m_iNumber;

}

} // namespace UMC
#endif
