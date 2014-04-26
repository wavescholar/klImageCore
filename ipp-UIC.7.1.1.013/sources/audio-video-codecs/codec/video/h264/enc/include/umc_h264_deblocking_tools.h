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

#ifndef __UMC_H264_DEBLOCKING_TOOLS_H
#define __UMC_H264_DEBLOCKING_TOOLS_H

#include "vm_event.h"
#include "vm_thread.h"
#include "umc_structures.h"
#include "umc_h264_video_encoder.h"
#include "umc_h264_core_enc.h"

using namespace UMC;


// Default constructor
Status H264EncoderThreadedDeblockingTools_Create(
    void* state);

// Destructor
void H264EncoderThreadedDeblockingTools_Destroy(
    void* state);

// Initialize object
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264EncoderThreadedDeblockingTools_Initialize(
    void* state,
    H264CoreEncoder<COEFFSTYPE, PIXTYPE> *pEncoder);

// Deblock slice by two thread
void H264EncoderThreadedDeblockingTools_DeblockSliceTwoThreaded(
    Ipp32u uFirstMB,
    Ipp32u unumMBs,
    H264CoreEncoder_DeblockingFunction pDeblocking);

// Deblock slice asynchronous
void H264EncoderThreadedDeblockingTools_WaitEndOfSlice(
    void* state);

void H264EncoderThreadedDeblockingTools_DeblockSliceAsync(
    void* state,
    Ipp32u uFirstMB,
    Ipp32u unumMBs,
    H264CoreEncoder_DeblockingFunction pDeblocking);

// Release object
void H264EncoderThreadedDeblockingTools_Release(
    void* state);

// Additional deblocking thread
Ipp32u VM_THREAD_CALLCONVENTION
H264EncoderThreadedDeblockingTools_DeblockSliceSecondThread(
    void *p);

Ipp32u VM_THREAD_CALLCONVENTION
H264EncoderThreadedDeblockingTools_DeblockSliceAsyncSecondThread(
    void *p);

template<typename COEFFSTYPE, typename PIXTYPE>
struct H264EncoderThreadedDeblockingTools
{
//public:
    Ipp32u m_nMBAFF;                                            // (Ipp32u) MBAFF flag

    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* m_pDecoder;         // (H264CoreEncoder *) pointer to decoder-owner

    vm_thread m_hDeblockingSliceSecondThread;                   // (vm_thread) handle to deblocking slice second thread
    vm_thread m_hDeblockingSliceAsyncSecondThread;              // (vm_thread) handle to deblocking slice second thread

    vm_event m_hBeginFrame;                                     // (vm_event) event to begin of deblocking frame
    vm_event m_hBeginSlice;                                     // (vm_event) event to begin of deblocking slice
    vm_event m_hBeginRow;                                       // (vm_event) event to begin of deblocking row
    vm_event m_hDoneBorder;                                     // (vm_event) event of border macroblock is complete
    vm_event m_hDoneRow;                                        // (vm_event) event of row is complete
    vm_event m_hDoneSlice;                                      // (vm_event) event of slice is complete

    bool m_bQuit;                                               // (bool) quit flag
    Ipp32u m_nFirstMB;                                          // (Ipp32u) first macroblock to deblock
    Ipp32u m_nNumMB;                                            // (Ipp32u) number of macroblock to deblock
    H264CoreEncoder_DeblockingFunction m_pDeblocking;           // (H264CoreEncoder::DeblockingFunction) pointer to current deblocking function
};

#endif // UMC_H264_DEBLOCKING_TOOLS_H
