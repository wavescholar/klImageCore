/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_PENDULUM_H__
#define __UMC_PENDULUM_H__

#include "umc_structures.h"
#include "vm_event.h"

namespace UMC
{

class Pendulum
{
public:
    Pendulum(void);
    virtual ~Pendulum(void);

    // Initialize
    Status Init(bool bSignaled = true);

    // Set pendulum to specific state
    Status Reset(bool bSignaled);

    // Set pendulum to signaled state (synchronized)
    Status Set(void);
    // Infinitely wait until event is signaled
    Status Wait(void);

protected:
    // Release object
    void Release(void);

    vm_event m_hHigh;       // event to waiting high state
    vm_event m_hLow;        // event to waiting low state
};

}
#endif
