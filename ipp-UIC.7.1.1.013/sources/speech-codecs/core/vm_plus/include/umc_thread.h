/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_THREAD_H__
#define __UMC_THREAD_H__

#include "vm_debug.h"
#include "vm_thread.h"
#include "umc_structures.h"

namespace UMC
{

class Thread
{
public:
    Thread(void);
    virtual ~Thread(void);

    // Check thread status
    bool IsValid(void);
    // Create new thread
    Status Create(vm_thread_callback func, void *arg);
    // Wait until thread does exit
    void Wait(void);
    // Set thread priority
    Status SetPriority(vm_thread_priority priority);
    // Close thread object
    void Close(void);

#if defined WINDOWS
    // Set reaction on exception, if exception is caught(VM_THREADCATCHCRASH define)
    Status SetExceptionReaction(vm_thread_callback func);
#endif

protected:
    vm_thread m_Thread;                     // handle to system thread
};

inline
bool Thread::IsValid(void)
{
    return vm_thread_is_valid(&m_Thread) ? true : false;
}

inline
void Thread::Wait(void)
{
    vm_thread_wait(&m_Thread);
}

inline
void Thread::Close(void)
{
    vm_thread_close(&m_Thread);
}

}
#endif
