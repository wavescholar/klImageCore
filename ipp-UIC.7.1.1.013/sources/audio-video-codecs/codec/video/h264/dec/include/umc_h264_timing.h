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

#ifndef __UMC_H264_TIMING_H
#define __UMC_H264_TIMING_H

//#define USE_DETAILED_H264_TIMING

#include "vm_time.h"

namespace UMC
{

#ifdef USE_DETAILED_H264_TIMING

#define GET_TIMING_TICKS vm_time_get_tick()

class TimingInfo
{
public:
    TimingInfo()
    {
        Reset();
    }

    void Reset()
    {
        vm_tick start_tick, end_tick;
        start_tick = GET_TIMING_TICKS;
        end_tick = GET_TIMING_TICKS;

        decode_time = 0;
        reconstruction_time = 0;
        deblocking_time = 0;

        m_compensative_const = 0;//end_tick - start_tick;
    }

    vm_tick decode_time;
    vm_tick reconstruction_time;
    vm_tick deblocking_time;

    vm_tick m_compensative_const;
};

extern TimingInfo* clsTimingInfo;

#define INIT_TIMING \
    if (!clsTimingInfo) \
        clsTimingInfo = new TimingInfo; \
    else \
        clsTimingInfo->Reset(); \


#define START_TICK      vm_tick start_tick = GET_TIMING_TICKS;

#define END_TICK(x)         \
{\
    vm_tick end_tick = GET_TIMING_TICKS;\
    clsTimingInfo->x += (end_tick - start_tick) - clsTimingInfo->m_compensative_const;\
}

#define START_TICK1      vm_tick start_tick1 = GET_TIMING_TICKS;
#define END_TICK1(x)         \
{\
    vm_tick end_tick = GET_TIMING_TICKS;\
    clsTimingInfo->x += (end_tick - start_tick1) - clsTimingInfo->m_compensative_const;\
}

//#define ENABLE_OPTIONAL
#if ENABLE_OPTIONAL
    #define START_TICK_OPTIONAL
    #define END_TICK_OPTIONAL(x)
#else
    #define START_TICK_OPTIONAL START_TICK
    #define END_TICK_OPTIONAL(x)  END_TICK(x)

#endif

#else

#define INIT_TIMING
#define START_TICK
#define START_TICK1
#define END_TICK(x)
#define END_TICK1(x)

#endif

} // namespace UMC

#endif // __UMC_H264_TIMING_H
