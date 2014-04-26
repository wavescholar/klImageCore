/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MMAP_H__
#define __UMC_MMAP_H__

#include "ippdefs.h"
#include "vm_debug.h"
#include "vm_mmap.h"
#include "umc_structures.h"

namespace UMC
{

class MMap
{
public:
    MMap(void);
    virtual ~MMap(void);

    // Initialize object
    Status Init(vm_char *sz_file);
    // Map memory
    Status Map(Ipp64u st_offset, size_t st_sizet);
    // Get addres of mapping
    void *GetAddr(void);
    // Get offset of mapping
    Ipp64u GetOffset(void);
    // Get size of mapping
    Ipp64u GetSize(void);
    // Get size of mapped file
    Ipp64u GetFileSize(void);

protected:
    vm_mmap m_handle;           // handle to system mmap object
    void *m_address;            // addres of mapped window
    Ipp64u m_file_size;         // file size
    Ipp64u m_offset;            // offset of mapping
    Ipp64u m_sizet;             // size of window
};

inline
void *MMap::GetAddr(void)
{
    return m_address;
}

inline
Ipp64u MMap::GetOffset(void)
{
    return m_offset;
}

inline
Ipp64u MMap::GetSize(void)
{
    return m_sizet;
}

inline
Ipp64u MMap::GetFileSize(void)
{
    return m_file_size;
}

}
#endif
