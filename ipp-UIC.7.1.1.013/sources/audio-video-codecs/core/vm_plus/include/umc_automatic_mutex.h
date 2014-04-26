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

#ifndef __UMC_AUTOMATIC_MUTEX_H__
#define __UMC_AUTOMATIC_MUTEX_H__

#include "umc_mutex.h"

namespace UMC
{

class AutomaticMutex
{
public:
    AutomaticMutex(Mutex &mutex)
    {
        m_pMutex   = &mutex;
        m_iCounter = 0;
        Lock();
    }
    virtual ~AutomaticMutex(void)
    {
        Unlock();
    }

    void Lock()
    {
        m_pMutex->Lock();
        m_iCounter++;
    }

    void Unlock()
    {
        if(m_iCounter)
        {
            m_iCounter--;
            m_pMutex->Unlock();
        }
    }

protected:
    Mutex  *m_pMutex;
    Ipp32u  m_iCounter;

private:
    AutomaticMutex & operator = (AutomaticMutex &)
    {
        return *this;
    }
};

}

#endif
