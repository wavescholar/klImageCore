/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_event.h"

using namespace UMC;


Event::Event(void)
{
    vm_event_set_invalid(&m_handle);
}

Event::~Event(void)
{
    if (vm_event_is_valid(&m_handle))
    {
        vm_event_signal(&m_handle);
        vm_event_destroy(&m_handle);
    }
}

Status Event::Init(Ipp32s iManual, Ipp32s iState)
{
    Status umcRes = UMC_OK;

    if (vm_event_is_valid(&m_handle))
    {
        umcRes = vm_event_signal(&m_handle);
        vm_event_destroy(&m_handle);
    }

    if (UMC_OK == umcRes)
        umcRes = vm_event_init(&m_handle, iManual, iState);

    return umcRes;
}
