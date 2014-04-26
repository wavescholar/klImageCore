/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Slice for threading (base on h264)
//
*/

#ifndef __UMC_VC1_DEC_THREAD_H_
#define __UMC_VC1_DEC_THREAD_H_

#include "vm_types.h"
#include "umc_structures.h"
#include "umc_vc1_dec_job.h"

#include <new>

namespace UMC
{

#pragma pack(1)

class VideoAccelerator;
class VC1TaskStore;
class VC1ThreadDecoder
{
    friend class VC1VideoDecoder;
public:
    // Default constructor
    VC1ThreadDecoder();
    // Destructor
    virtual ~VC1ThreadDecoder();

    void* operator new(size_t size, void* p)
    {
        if (!p)
            throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
        return new(p) Ipp8u[size];
    };

    // external memory management. No need to delete memory
    void operator delete(void *p)
    {
        //Anyway its incorrect when we trying free null pointer
        if (!p)
            throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
    };

    void operator delete(void *, void *) 
    {
        // delete for system exceptions case
        throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
    };

    // Initialize slice decoder
    virtual Status Init(VC1Context* pContext,
                        Ipp32s iNumber,
                        VC1TaskStore* pTaskStore,
                        VideoAccelerator* va,
                        MemoryAllocator* pMemoryAllocator,
                        VC1TaskProcessor* pExternalProcessor);

    // Start decoding asynchronously
    Status StartProcessing(void);
    // Wait for end of asynchronous decoding
    Status WaitForEndOfProcessing(void);

    // wait (if thread already worked) and stop thread
    Status WaitAndStop(void);

    // Decode picture segment
    virtual Status process(void);
    virtual Status processMainThread(void);
    Ipp32s  getThreadID()
    {
        return this->m_iNumber;
    }

protected:
    // Release slice decoder
    void Release(void);

    //
    // Threading tools
    //

    // Starting routine for decoding thread
    static
    Ipp32u DecodingThreadRoutine(void *p);

    vm_thread m_hThread;                                        // (vm_thread) handle to asynchronously working thread
    vm_event m_hStartProcessing;                                // (vm_event) event to start asynchronous processing
    vm_event m_hDoneProcessing;                                 // (vm_event) event to show end of processing

    Ipp32s m_iNumber;                                           // (Ipp32s) ordinal number of decoder


    volatile
    bool m_bQuit;                                               // (bool) quit flag for additional thread(s)
    Status m_Status;                                            // (Status) async return value
    bool m_bStartDecoding;

private:
    MemoryAllocator*   m_pMemoryAllocator; // (MemoryAllocator*) pointer to memory allocator
    VC1TaskProcessorUMC   m_TaskProcessor;
    VC1TaskProcessor*  m_pJobSlice;
    VC1TaskStore*      m_pStore;
};

#pragma pack()

} // namespace UMC

#endif // __umc_vc1_dec_thread_H
