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
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER

#include "umc_h264_deblocking_tools.h"
#include "umc_h264_video_encoder.h"
#include "umc_h264_core_enc.h"

using namespace UMC;


template<typename COEFFSTYPE, typename PIXTYPE>
void H264EncoderThreadedDeblockingTools_Release(void* state)
{
    H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>* tools = (H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>*)state;
    // terminate second thread(s)
    if (vm_thread_is_valid(&tools->m_hDeblockingSliceSecondThread))
    {
        tools->m_bQuit = true;
        vm_event_signal(&tools->m_hBeginRow);

        vm_thread_wait(&tools->m_hDeblockingSliceSecondThread);
        vm_thread_close(&tools->m_hDeblockingSliceSecondThread);
    }
    if (vm_thread_is_valid(&tools->m_hDeblockingSliceAsyncSecondThread))
    {
        tools->m_bQuit = true;
        vm_event_signal(&tools->m_hBeginSlice);

        vm_thread_wait(&tools->m_hDeblockingSliceAsyncSecondThread);
        vm_thread_close(&tools->m_hDeblockingSliceAsyncSecondThread);
    }

    // destroy objects
    if (vm_event_is_valid(&tools->m_hBeginFrame))
        vm_event_destroy(&tools->m_hBeginFrame);
    if (vm_event_is_valid(&tools->m_hBeginSlice))
        vm_event_destroy(&tools->m_hBeginSlice);
    if (vm_event_is_valid(&tools->m_hBeginRow))
        vm_event_destroy(&tools->m_hBeginRow);
    if (vm_event_is_valid(&tools->m_hDoneBorder))
        vm_event_destroy(&tools->m_hDoneBorder);
    if (vm_event_is_valid(&tools->m_hDoneRow))
        vm_event_destroy(&tools->m_hDoneRow);
    if (vm_event_is_valid(&tools->m_hDoneSlice))
        vm_event_destroy(&tools->m_hDoneSlice);

    vm_thread_set_invalid(&tools->m_hDeblockingSliceSecondThread);
    vm_thread_set_invalid(&tools->m_hDeblockingSliceAsyncSecondThread);
    vm_event_set_invalid(&tools->m_hBeginFrame);
    vm_event_set_invalid(&tools->m_hBeginSlice);
    vm_event_set_invalid(&tools->m_hBeginRow);
    vm_event_set_invalid(&tools->m_hDoneBorder);
    vm_event_set_invalid(&tools->m_hDoneRow);
    vm_event_set_invalid(&tools->m_hDoneSlice);

    tools->m_bQuit = false;

}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264EncoderThreadedDeblockingTools_Initialize(void* state, H264CoreEncoder<COEFFSTYPE, PIXTYPE> *pDecoder)
{
    H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>* tools = (H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>*)state;
    vm_status res;

    // release object before initialization
    H264EncoderThreadedDeblockingTools_Release(state);

    // save pointer
    tools->m_pDecoder = pDecoder;

    // create objects
    res = vm_event_init(&tools->m_hBeginFrame, 0, 0);
    if (VM_OK != res)
        return UMC_ERR_INIT;

    res = vm_event_init(&tools->m_hBeginSlice, 0, 0);
    if (VM_OK != res)
        return UMC_ERR_INIT;

    res = vm_event_init(&tools->m_hBeginRow, 0, 0);
    if (VM_OK != res)
        return UMC_ERR_INIT;

    res = vm_event_init(&tools->m_hDoneBorder, 0, 0);
    if (VM_OK != res)
        return UMC_ERR_INIT;

    res = vm_event_init(&tools->m_hDoneRow, 0, 0);
    if (VM_OK != res)
        return UMC_ERR_INIT;

    res = vm_event_init(&tools->m_hDoneSlice, 0, 1);
    if (VM_OK != res)
        return UMC_ERR_INIT;

    // run second thread(s)
    {
        Ipp32s res;

        res = vm_thread_create(
            &tools->m_hDeblockingSliceSecondThread,
            H264EncoderThreadedDeblockingTools_DeblockSliceSecondThread,
            state);
        if (0 == res)
            return UMC_ERR_INIT;

        res = vm_thread_create(
            &tools->m_hDeblockingSliceAsyncSecondThread,
            H264EncoderThreadedDeblockingTools_DeblockSliceAsyncSecondThread,
            state);
        if (0 == res)
            return UMC_ERR_INIT;
    }

    return UMC_OK;

} // Status H264EncoderThreadedDeblockingTools<PIXTYPE, COEFFSTYPE> ::Initialize(H264CoreEncoder *pDecoder)

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264EncoderThreadedDeblockingTools_Create(void* state)
{
    H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>* tools = (H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>*)state;
    vm_thread_set_invalid(&tools->m_hDeblockingSliceSecondThread);
    vm_thread_set_invalid(&tools->m_hDeblockingSliceAsyncSecondThread);
    vm_event_set_invalid(&tools->m_hBeginFrame);
    vm_event_set_invalid(&tools->m_hBeginSlice);
    vm_event_set_invalid(&tools->m_hBeginRow);
    vm_event_set_invalid(&tools->m_hDoneBorder);
    vm_event_set_invalid(&tools->m_hDoneRow);
    vm_event_set_invalid(&tools->m_hDoneSlice);

    tools->m_bQuit = false;
    return UMC_OK;
}

void H264EncoderThreadedDeblockingTools_Destroy(
    void* state)
{
    H264EncoderThreadedDeblockingTools_Release(state);
} // H264EncoderThreadedDeblockingTools<PIXTYPE, COEFFSTYPE> ::~H264EncoderThreadedDeblockingTools<PIXTYPE, COEFFSTYPE> (void)

template<typename COEFFSTYPE, typename PIXTYPE>
void H264EncoderThreadedDeblockingTools_DeblockSliceTwoThreaded(void* state, Ipp32u uFirstMB, Ipp32u unumMBs, H264CoreEncoder_DeblockingFunction pDeblocking)
{
    H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>* tools = (H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u mb_width = tools->m_pDecoder->m_WidthInMBs << tools->m_nMBAFF;
    Ipp32u nBorder = (mb_width / 2) & (~tools->m_nMBAFF);
    Ipp32u i, nCurrMB, nMaxMB;

    // reset border signal to avoid complex logic
    vm_event_reset(&tools->m_hDoneBorder);

    // set maximum MB number
    nMaxMB = uFirstMB + unumMBs;

    // deblock blocks in first row before border
    nCurrMB = uFirstMB;
    for (i = uFirstMB % mb_width;(i < nBorder) && (nCurrMB < nMaxMB);i += 1, nCurrMB += 1)
        (*pDeblocking)(tools->m_pDecoder, nCurrMB);

    if (nCurrMB < nMaxMB)
    {
        // fill thread interaction parameters
        tools->m_nFirstMB = nCurrMB;
        tools->m_nNumMB = nMaxMB - nCurrMB;
        tools->m_pDeblocking = pDeblocking;

        // send signal to second thread
        vm_event_signal(&tools->m_hBeginRow);

        // align macroblock number to new line
        nCurrMB += mb_width - nCurrMB % mb_width;

        while (nCurrMB < nMaxMB)
        {
            // deblock row up to border macroblock(s)
            for (i = 0;(i < (nBorder - 1 - tools->m_nMBAFF)) && (nCurrMB < nMaxMB);i += 1, nCurrMB += 1)
                (*pDeblocking)(tools->m_pDecoder, nCurrMB);

            // wait response from second thread
            vm_event_wait(&tools->m_hDoneBorder);

            if (nCurrMB < nMaxMB)
            {
                // deblock border macroblock(s)
                for (;(i < (nBorder)) && (nCurrMB < nMaxMB);i += 1, nCurrMB += 1)
                    (*pDeblocking)(tools->m_pDecoder, nCurrMB);

                // send signal to second thread
                if (nCurrMB < nMaxMB)
                    vm_event_signal(&tools->m_hBeginRow);

                nCurrMB += mb_width - nBorder;
            }
        }

        // wait for second thread
        vm_event_wait(&tools->m_hDoneRow);
    }

    // set event of slice is done
    vm_event_signal(&tools->m_hDoneSlice);

} // void H264EncoderThreadedDeblockingTools<PIXTYPE, COEFFSTYPE> ::DeblockSliceTwoThreaded(Ipp32u uFirstMB, Ipp32u unumMBs, H264CoreEncoder::DeblockingFunction pDeblocking)

template<typename COEFFSTYPE, typename PIXTYPE>
void H264EncoderThreadedDeblockingTools_DeblockSliceAsync(void* state, Ipp32u uFirstMB, Ipp32u unumMBs, H264CoreEncoder_DeblockingFunction pDeblocking)
{
    H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>* tools = (H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>*)state;
    // fill thread interaction parameters
    tools->m_nFirstMB = uFirstMB;
    tools->m_nNumMB = unumMBs;
    tools->m_pDeblocking = pDeblocking;
    vm_event_signal(&tools->m_hBeginSlice);

} // void H264EncoderThreadedDeblockingTools<PIXTYPE, COEFFSTYPE> ::DeblockSliceAsync(Ipp32u uFirstMB, Ipp32u unumMBs, H264CoreEncoder::DeblockingFunction pDeblocking)

template<typename COEFFSTYPE, typename PIXTYPE>
void H264EncoderThreadedDeblockingTools_WaitEndOfSlice(void* state)
{
    H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>* tools = (H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>*)state;
    vm_event_wait(&tools->m_hDoneSlice);

} // void H264EncoderThreadedDeblockingTools<PIXTYPE, COEFFSTYPE> ::WaitEndOfSlice(void)

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u VM_THREAD_CALLCONVENTION
H264EncoderThreadedDeblockingTools_DeblockSliceSecondThread(void *p)
{
    H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>* tools = (H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>*)p;
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* pDec;

    // check error(s)
    VM_ASSERT(tools);
    if (NULL == tools)
        return 0x0bad;

    pDec = tools->m_pDecoder;

    {
        vm_event *phBeginRow = &(tools->m_hBeginRow);
        vm_event *phDoneRow = &(tools->m_hDoneRow);
        vm_event *phDoneBorder = &(tools->m_hDoneBorder);

        // wait response from main thread
        vm_event_wait(phBeginRow);

        // is it exit ?
        while (false == tools->m_bQuit)
        {
            Ipp32u i, nCurrMB;
            Ipp32u nMaxMB = tools->m_nFirstMB + tools->m_nNumMB;
            H264CoreEncoder_DeblockingFunction pDeblocking = tools->m_pDeblocking;
            Ipp32u mb_width = pDec->m_WidthInMBs << tools->m_nMBAFF;
            Ipp32u nBorder = (mb_width / 2) & (~(tools->m_nMBAFF));

            nCurrMB = tools->m_nFirstMB;

            while (nCurrMB < nMaxMB)
            {
                // wait response from main thread
                if (nCurrMB != tools->m_nFirstMB)
                    vm_event_wait(phBeginRow);

                // deblock border macroblock(s)
                if (nCurrMB % mb_width == nBorder)
                {
                    for (i = 0;i < (1 + tools->m_nMBAFF);i += 1, nCurrMB += 1)
                        (*pDeblocking)(pDec, nCurrMB);

                    i = nBorder + 1 + tools->m_nMBAFF;
                }
                else
                    i = nCurrMB % mb_width;

                // send signal to main thread, even border macroblock wasn't processed
                vm_event_signal(phDoneBorder);

                // deblock all macroblock up to end of row
                for (;(i < mb_width) && (nCurrMB < nMaxMB);i += 1, nCurrMB += 1)
                    (*pDeblocking)(pDec, nCurrMB);

                nCurrMB += nBorder;
            }

            // send signal to main thread
            vm_event_signal(phDoneRow);

            // wait response from main thread
            vm_event_wait(phBeginRow);
        }
    }

    return 1;

} // Ipp32u VM_THREAD_CALLCONVENTION H264EncoderThreadedDeblockingTools<PIXTYPE, COEFFSTYPE> ::DeblockSliceSecondThread(void *p)

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u VM_THREAD_CALLCONVENTION
H264EncoderThreadedDeblockingTools_DeblockSliceAsyncSecondThread(void *p)
{
    H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>* tools = (H264EncoderThreadedDeblockingTools<COEFFSTYPE, PIXTYPE>*)p;
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* pDec;

    // check error(s)
    VM_ASSERT(tools);
    if (NULL == tools)
        return 0x0bad;

    pDec = tools->m_pDecoder;

    {
        vm_event *phBeginSlice = &(tools->m_hBeginSlice);
        vm_event *phDoneSlice = &(tools->m_hDoneSlice);

        // wait response from main thread
        vm_event_wait(phBeginSlice);

        // is it exit ?
        while (false == tools->m_bQuit)
        {
            Ipp32u i;
            H264CoreEncoder_DeblockingFunction pDeblocking = tools->m_pDeblocking;
            Ipp32u nFirstMB = tools->m_nFirstMB;
            Ipp32u nNumMB = tools->m_nNumMB;

            // deblock macroblocks
            for (i = nFirstMB; i < nFirstMB + nNumMB; i++)
                (*pDeblocking)(pDec, i);

            // send signal to main thread
            vm_event_signal(phDoneSlice);

            // wait response from main thread
            vm_event_wait(phBeginSlice);
        }
    }

    return 1;

} // Ipp32u H264EncoderThreadedDeblockingTools<PIXTYPE, COEFFSTYPE> ::DeblockSliceAsyncSecondThread(void *p)

#endif //UMC_ENABLE_H264_VIDEO_ENCODER
