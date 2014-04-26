/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

/*
    This file contains class covering Linux /dev situated devices interface
*/

#ifdef LINUX

#include "linux_dev.h"
#include "vm_debug.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <vm_debug.h>

using namespace UMC;

Status LinuxDev::Init(const vm_char* szDevName, Ipp32s iMode)
{
    Status umcRes = UMC_OK;

    VM_ASSERT(NULL != szDevName);
    Close();

    m_iHandle = open(szDevName, iMode);
    if (-1 == m_iHandle) {
        umcRes = UMC_ERR_OPEN_FAILED;
    }
    else
    {   m_iMode = iMode;    }
    return umcRes;
}

Status LinuxDev::Read(Ipp8u* pbData, Ipp32u uiDataSize, Ipp32u* puiReadSize)
{
    Status umcRes = UMC_OK;
    VM_ASSERT(-1 != m_iHandle);
    VM_ASSERT(NULL != pbData);
    VM_ASSERT(0 != uiDataSize);

    if (NULL != puiReadSize)
    {   *puiReadSize = 0;   }

    ssize_t readRes = read(m_iHandle, pbData, uiDataSize);

    switch (readRes) {
    case 0:
        umcRes = UMC_ERR_END_OF_STREAM;
        break;
    case -1:
        umcRes = UMC_ERR_FAILED;
        break;
    default:
        if (NULL != puiReadSize)
        {   *puiReadSize = readRes; }
    }
    return umcRes;
}

Status LinuxDev::Write(Ipp8u* pbBuffer, Ipp32u uiBufSize, Ipp32u* puiWroteSize)
{
    Status umcRes = UMC_OK;
    VM_ASSERT(-1 != m_iHandle);
    VM_ASSERT(NULL != pbBuffer);
    VM_ASSERT(0 != uiBufSize);

    if (NULL != puiWroteSize)
    {   *puiWroteSize = 0;   }

    ssize_t readRes = write(m_iHandle, pbBuffer, uiBufSize);

    if (-1 == readRes) {
        umcRes = UMC_ERR_FAILED;
    } else
        if (NULL != puiWroteSize)
        {   *puiWroteSize = readRes; }
    return umcRes;
}

Status LinuxDev::Ioctl(Ipp32s iReqCode)
{
    Status umcRes = UMC_OK;
    VM_ASSERT(-1 != m_iHandle);

    Ipp32s iRes = ioctl(m_iHandle, iReqCode);
    if (-1 == iRes) {   umcRes = UMC_ERR_FAILED;    }
    return umcRes;
}

Status LinuxDev::Ioctl(Ipp32s iReqCode, void* uiParam)
{
    Status umcRes = UMC_OK;
    VM_ASSERT(-1 != m_iHandle);

    Ipp32s iRes = ioctl(m_iHandle, iReqCode, uiParam);
    if (-1 == iRes) {   umcRes = UMC_ERR_FAILED;    }
    return umcRes;
}

void LinuxDev::Close()
{
    if (-1 != m_iHandle) {
        close(m_iHandle);
        m_iHandle = -1;
        m_iMode = 0;
    }
}

#endif
