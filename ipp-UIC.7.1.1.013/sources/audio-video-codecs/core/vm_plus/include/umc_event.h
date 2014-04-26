/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_EVENT_H__
#define __UMC_EVENT_H__

#include "vm_debug.h"
#include "vm_event.h"
#include "umc_structures.h"

namespace UMC
{

class Event
{
public:
    Event(void);
    virtual ~Event(void);

    // Initialize event
    Status Init(Ipp32s iManual, Ipp32s iState);
    // Set event to signaled state
    Status Set(void);
    // Set event to non-signaled state
    Status Reset(void);
    // Pulse event (should not be used)
    Status Pulse(void);
    // Wait until event is signaled
    Status Wait(Ipp32u msec);
    // Infinitely wait until event is signaled
    Status Wait(void);

protected:
    vm_event m_handle;
};

inline
Status Event::Set(void)
{
    return vm_event_signal(&m_handle);
}

inline
Status Event::Reset(void)
{
    return vm_event_reset(&m_handle);
}

inline
Status Event::Pulse(void)
{
    return vm_event_pulse(&m_handle);
}

inline
Status Event::Wait(Ipp32u msec)
{
    return vm_event_timed_wait(&m_handle, msec);
}

inline
Status Event::Wait(void)
{
    return vm_event_wait(&m_handle);
}

}
#endif
