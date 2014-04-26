/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MUTEX_H__
#define __UMC_MUTEX_H__

#include "vm_debug.h"
#include "vm_mutex.h"
#include "umc_structures.h"

namespace UMC
{

class Mutex
{
public:
    Mutex(void)
    {
        vm_mutex_set_invalid(&m_handle);
        vm_mutex_init(&m_handle);
    }
    virtual ~Mutex(void)
    {
        if(vm_mutex_is_valid(&m_handle))
            vm_mutex_destroy(&m_handle);
    }

    // Reset mutex
    void Reset(void)
    {
        if(vm_mutex_is_valid(&m_handle))
            vm_mutex_destroy(&m_handle);
        vm_mutex_set_invalid(&m_handle);
        vm_mutex_init(&m_handle);
    }

    // Get ownership of mutex
    void Lock(void)
    {
        if (!vm_mutex_is_valid(&m_handle))
            VM_ASSERT(false);
        else if(VM_OK != vm_mutex_lock(&m_handle))
            VM_ASSERT(false);
    }

    // Release ownership of mutex
    void Unlock(void)
    {
        if(!vm_mutex_is_valid(&m_handle))
            VM_ASSERT(false);
        else if(VM_OK != vm_mutex_unlock(&m_handle))
            VM_ASSERT(false);
    }

    // Try to get ownership of mutex
    Status TryLock(void)
    {
        if(!vm_mutex_is_valid(&m_handle))
            VM_ASSERT(false);

        return vm_mutex_try_lock(&m_handle);
    }

protected:
    vm_mutex m_handle;  // handle to system mutex
};

}

#endif
