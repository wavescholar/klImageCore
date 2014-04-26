/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_thread.h"

using namespace UMC;


Thread::Thread(void)
{
    vm_thread_set_invalid(&m_Thread);
}

Thread::~Thread()
{
    Close();
}

Status Thread::Create(vm_thread_callback func, void *arg)
{
    Status umcRes = UMC_OK;

    if (0 == vm_thread_create(&m_Thread, func, arg))
        umcRes = UMC_ERR_FAILED;

    return umcRes;
}

Status Thread::SetPriority(vm_thread_priority priority)
{
    if (vm_thread_set_priority(&m_Thread, priority))
        return UMC_OK;
    else
        return UMC_ERR_FAILED;
}

#if defined WINDOWS
Status Thread::SetExceptionReaction(vm_thread_callback)
{
    return UMC_OK;
}
#endif
