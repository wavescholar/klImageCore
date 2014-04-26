/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

//
//    This file contains class covering Linux /dev situated devices interface
//

#ifndef __LINUX_DEV_H__
#define __LINUX_DEV_H__

#ifdef LINUX

#include "umc_structures.h"

namespace UMC
{

class LinuxDev
{
public:
    LinuxDev():m_iHandle(-1), m_iMode(0) {}
    virtual ~LinuxDev() { Close(); }

    Status Init(const vm_char* szDevName, Ipp32s iMode);
    Status Read(Ipp8u* pbData, Ipp32u uiDataSize,
                Ipp32u* puiReadSize);
    Status Write(Ipp8u* pbBuffer, Ipp32u uiBufSize,
                    Ipp32u* puiWroteSize);
    Status Ioctl(Ipp32s iReqCode);
    Status Ioctl(Ipp32s iReqCode, void* uiParam);
    void Close();

protected:
    Ipp32s m_iHandle;
    Ipp32s m_iMode;
};

};  //  namespace UMC

#endif

#endif
