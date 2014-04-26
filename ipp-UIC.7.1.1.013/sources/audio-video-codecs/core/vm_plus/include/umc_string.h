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

#ifndef __UMC_STRING_H__
#define __UMC_STRING_H__

#include "vm_strings.h"
#include "ippdefs.h"

namespace UMC
{

// vm_char based string class with dynamic memory allocations
class DString
{
public:
    DString();
    DString(const vm_char *pSrc);
    DString(const DString &src);

    ~DString();

    void Clear(); // reset string buffer

    // replace current string with a new string
    size_t Replace(const vm_char* pSrc, size_t iSrcSize);

    // add new string to the end of the current string
    size_t Append(const vm_char* pSrc, size_t iSrcSize);

    Ipp32s Compare(const vm_char *pSrc, bool bCaseSensitive = true);

    size_t Trim();
    size_t TrimLeft();
    size_t TrimRight();

    size_t                  Size() const { return m_iLen; }
    operator       vm_char*     ()       { return m_pData; }
    operator const vm_char*     () const { return m_pData; }

    DString& operator=(const vm_char* pSrc)
    {
        Replace(pSrc, (Ipp32u)vm_string_strlen(pSrc));
        return *this;
    }

    DString& operator=(const DString &str)
    {
        Replace(str.m_pData, str.m_iLen);
        return *this;
    }

    DString operator + (const DString &right)
    {
        Append(right.m_pData, right.m_iLen);
        return *this;
    }
    DString operator + (const vm_char *right)
    {
        Append(right, (Ipp32u)vm_string_strlen(right));
        return *this;
    }
    DString operator + (const vm_char right)
    {
        Append(&right, 1);
        return *this;
    }

    void operator += (const DString &right)
    {
        Append(right.m_pData, right.m_iLen);
    }
    void operator += (const vm_char *right)
    {
        Append(right, (Ipp32u)vm_string_strlen(right));
    }
    void operator += (const vm_char right)
    {
        Append(&right, 1);
    }

    bool operator == (const vm_char *right)
    {
        if(!Compare(right))
            return true;
        return false;
    }

    bool operator == (const DString &right)
    {
        if(!Compare(right.m_pData))
            return true;
        return false;
    }

    bool operator != (const vm_char *right)
    {
        if(Compare(right))
            return true;
        return false;
    }

    bool operator != (const DString &right)
    {
        if(Compare(right.m_pData))
            return true;
        return false;
    }

protected:
    vm_char *m_pData; // string buffer
    size_t   m_iSize; // buffer size
    size_t   m_iLen;  // string length
};

}

#endif
