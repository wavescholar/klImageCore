/* ////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
//
//
*/

#include "umc_string.h"

using namespace UMC;


DString::DString()
{
    m_pData = NULL;
    m_iSize = 0;
    m_iLen  = 0;
    Replace((const vm_char*)&(VM_STRING("")), 0); // initialize with zero-length string to avoid typecast memory problems
}

DString::DString(const vm_char *pSrc)
{
    m_pData = NULL;
    m_iSize = 0;
    m_iLen  = 0;
    if(!pSrc)
        Replace((const vm_char*)&(VM_STRING("")), 0);
    else
        Replace(pSrc, (Ipp32u)vm_string_strlen(pSrc));
}

DString::DString(const DString &src)
{
    m_pData = NULL;
    m_iSize = 0;
    m_iLen  = 0;
    Replace(src.m_pData, src.m_iLen);
}

DString::~DString()
{
    if(m_pData)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
    m_iSize = 0;
    m_iLen  = 0;
}

void DString::Clear()
{
    if(m_pData)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
    m_iSize = 0;
    m_iLen  = 0;
    Replace((const vm_char*)&(VM_STRING("")), 0);
}

size_t DString::Replace(const vm_char* pSrc, size_t iSrcSize)
{
    if(pSrc && m_pData != pSrc)
    {
        if(m_iSize < iSrcSize + 1)
        {
            if(m_pData)
                delete[] m_pData;
            m_iSize = sizeof(vm_char)*(iSrcSize + 1);
            m_pData = (vm_char*)new vm_char[m_iSize];
        }
        m_iLen = iSrcSize;
        memcpy(m_pData, pSrc, iSrcSize);
        m_pData[m_iLen] = VM_STRING('\0');

        return iSrcSize;
    }
    return 0;
}

size_t DString::Append(const vm_char* pSrc, size_t iSrcSize)
{
    if(pSrc)
    {
        if(m_iSize < iSrcSize + m_iLen + 1)
        {
            m_iSize = sizeof(vm_char)*(iSrcSize + m_iLen + 1);
            vm_char *pData = (vm_char*)new vm_char[m_iSize];
            if(m_pData)
            {
                memcpy(pData, m_pData, m_iLen);
                delete[] m_pData;
            }
            m_pData = pData;
        }
        memcpy(&m_pData[m_iLen], pSrc, iSrcSize);
        m_iLen += iSrcSize;
        m_pData[m_iLen] = VM_STRING('\0');
        return iSrcSize;
    }
    return 0;
}

Ipp32s DString::Compare(const vm_char *pSrc, bool bCaseSensitive)
{
    if(!pSrc || !m_pData)
        return -1;

    if(bCaseSensitive)
        return vm_string_strcmp(m_pData, pSrc);
    else
        return vm_string_stricmp(m_pData, pSrc);
}

size_t DString::Trim()
{
    return TrimRight() + TrimLeft();
}

size_t DString::TrimLeft()
{
    size_t iSpaces = 0;
    size_t i;

    if(!m_iLen)
        return 0;

    for(i = 0; i < m_iLen; i++)
    {
        if(m_pData[i] != VM_STRING(' '))
            break;

        iSpaces++;
    }

    if(iSpaces)
    {
        for(i = iSpaces; i < m_iLen; i++)
            m_pData[i - iSpaces] = m_pData[i];

        m_iLen -= iSpaces;
        m_pData[m_iLen] = VM_STRING('\0');
    }

    return iSpaces;
}

size_t DString::TrimRight()
{
    size_t iSpaces = 0;

    if(!m_iLen)
        return 0;

    for(size_t i = m_iLen - 1; ; i--)
    {
        if(m_pData[i] != VM_STRING(' '))
            break;

        iSpaces++;
        if(!i)
            break;
    }
    m_iLen = m_iLen - iSpaces;
    m_pData[m_iLen] = VM_STRING('\0');

    return iSpaces;
}

