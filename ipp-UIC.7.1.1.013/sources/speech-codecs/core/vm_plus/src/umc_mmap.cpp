/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_mmap.h"

using namespace UMC;


MMap::MMap(void)
{
    m_address   = NULL;
    m_file_size = 0;
    m_offset    = 0;
    m_sizet     = 0;

    vm_mmap_set_invalid(&m_handle);
}

MMap::~MMap(void)
{
    vm_mmap_close(&m_handle);
}

Status MMap::Init(vm_char *sz_file)
{
    vm_mmap_close(&m_handle);

    m_offset = 0;
    m_sizet = 0;
    m_address = NULL;

    m_file_size = vm_mmap_create(&m_handle, sz_file, FLAG_ATTRIBUTE_READ);

    if (0 == m_file_size)
        return UMC_ERR_FAILED;

    return UMC_OK;
}

Status MMap::Map(Ipp64u st_offset, size_t st_sizet)
{
    void *pv_addr;
    Ipp64u st_align = st_offset % vm_mmap_get_alloc_granularity();

    // check error(s)
    if (!vm_mmap_is_valid(&m_handle))
        return UMC_ERR_NOT_INITIALIZED;
    if (st_offset + st_sizet > m_file_size)
        return UMC_ERR_NOT_ENOUGH_DATA;

    st_sizet += (size_t)st_align;
    st_offset -= st_align;

    // set new window
    pv_addr = vm_mmap_set_view(&m_handle, &st_offset, &st_sizet);
    if (NULL == pv_addr)
    {
        m_offset = 0;
        m_sizet = 0;
        return UMC_ERR_FAILED;
    }

    // save setting(s)
    m_address = pv_addr;
    m_offset = st_offset;
    m_sizet = st_sizet;

    return UMC_OK;
}
