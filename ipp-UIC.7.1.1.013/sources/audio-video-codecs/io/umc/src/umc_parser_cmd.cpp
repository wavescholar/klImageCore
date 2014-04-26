
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

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "umc_parser_cmd.h"

using namespace UMC;


static const vm_char validSymbols[]     = { VM_STRING("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\0") };
static const vm_char validSymbolsExt[]  = { VM_STRING("-_\0") };
static const vm_char validDigits[]      = { VM_STRING("0123456789\0") };

static bool isValid(vm_char *pSymbol, const vm_char *pValidString)
{
    size_t iArrSize = vm_string_strlen(pValidString);
    for(unsigned int i = 0; i < iArrSize; i++)
    {
        if(*pSymbol == pValidString[i])
            return true;
    }

    return false;
}

ParserCmd::ParserCmd()
{
    m_iBufferLen       = 0;
    m_iKeysCount       = 0;
    m_iEmptyKeysCount  = 0;
    m_bHelpPrinted     = false;
    m_bErrorResilience = false;
}

ParserCmd::~ParserCmd()
{
    Reset();
}

void ParserCmd::Reset()
{
    for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
        pKey->Free();
}

bool ParserCmd::IsKeyExist(const vm_char *cName, const vm_char *cLongName)
{
    for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
    {
        if((pKey->m_sName.Size() && vm_string_strlen(cName) == pKey->m_sName.Size() && !vm_string_strncmp(pKey->m_sName, cName, pKey->m_sName.Size())) ||
            (pKey->m_sLongName.Size() && vm_string_strlen(cLongName) == pKey->m_sLongName.Size() && !vm_string_strncmp(pKey->m_sLongName, cLongName, pKey->m_sLongName.Size())))
                return true;
    }

    return false;
}

void ParserCmd::PrintData()
{
    size_t iSizeMax = 0;
    size_t iLongSizeMax = 0;
    for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
    {
        if(pKey->m_sName.Size() > iSizeMax)
            iSizeMax = pKey->m_sName.Size();
        if(pKey->m_sLongName.Size() > iLongSizeMax)
            iLongSizeMax = pKey->m_sLongName.Size();
    }
    for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
    {
        if(pKey->m_sName.Size())
        {
            vm_string_printf(VM_STRING("-"));
            vm_string_printf(pKey->m_sName);
        }
        for(Ipp32u i = 0; i < iSizeMax - pKey->m_sName.Size() + 1; i++)
            vm_string_printf(VM_STRING(" "));

        if(pKey->m_sLongName.Size())
        {
            vm_string_printf(VM_STRING("--"));
            vm_string_printf(pKey->m_sLongName);
        }
        else
            vm_string_printf(VM_STRING("  "));
        for(Ipp32u i = 0; i < iLongSizeMax - pKey->m_sLongName.Size(); i++)
            vm_string_printf(VM_STRING(" "));

        vm_string_printf(VM_STRING(" [%d] ="), pKey->m_iArraySize);

        if(pKey->m_bAllocated)
        {
            for(Ipp32u i = 0; i < pKey->m_iArraySize; i++)
            {
                switch(pKey->m_keyType)
                {
                case Boolean:
                    if(pKey->m_bool[i] == true)
                        vm_string_printf(VM_STRING(" true"));
                    else
                        vm_string_printf(VM_STRING(" false"));
                    break;
                case Integer:
                    vm_string_printf(VM_STRING(" %d"), pKey->m_uint[i]);
                    break;
                case Real:
                    vm_string_printf(VM_STRING(" %f"), pKey->m_double[i]);
                    break;
                case String:
                    vm_string_printf(VM_STRING(" \""));
                    vm_string_printf(pKey->m_string[i]);
                    vm_string_printf(VM_STRING("\""));
                    break;
                default:
                    break;
                }
                if(i != (pKey->m_iArraySize - 1))
                    vm_string_printf(VM_STRING(","));
            }
        }
        else
            vm_string_printf(VM_STRING(" unallocated"));
        vm_string_printf(VM_STRING("\n"));
    }
}

void ParserCmd::PrintHelp()
{
    List<Key>::Iterator pKey;
    List<DString> groups;
    List<DString>::Iterator pGroup;
    size_t   iMaxSizeShort = 0;
    size_t   iMaxSizeLong  = 0;
    DString  sFDesc;
    vm_char *pDesc;
    bool     bNewGroup;
    Ipp32u   i, j;

    for(pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
    {
        bNewGroup = true;
        for(pGroup = groups.ItrFront(); pGroup != groups.ItrBackBound(); ++pGroup)
        {
            if(*pGroup == pKey->m_sGroupName)
                bNewGroup = false;
        }
        if(bNewGroup)
        {
            groups.PushBack();
            groups.Back() = pKey->m_sGroupName;
        }
    }

    for(pGroup = groups.ItrFront(); pGroup != groups.ItrBackBound(); ++pGroup)
    {
        iMaxSizeShort = iMaxSizeLong = 0;
        for(pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
        {
            if(pKey->m_sGroupName == *pGroup)
            {
                iMaxSizeShort = ((iMaxSizeShort < pKey->m_sName.Size())?pKey->m_sName.Size():iMaxSizeShort);
                iMaxSizeLong  = ((iMaxSizeLong < pKey->m_sLongName.Size())?pKey->m_sLongName.Size():iMaxSizeLong);
            }
        }
        if(iMaxSizeShort)
            iMaxSizeShort += 3;
        if(iMaxSizeLong)
            iMaxSizeLong  += 4;

        bNewGroup = true;
        for(pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
        {
            if(pKey->m_sGroupName != *pGroup)
                continue;

            if(pKey->m_sGroupName.Size() && bNewGroup)
            {
                vm_string_printf(VM_STRING("\n%s:\n"), (vm_char*)pKey->m_sGroupName);
                bNewGroup = false;
            }

            if(iMaxSizeShort)
            {
                if(pKey->m_sName.Size())
                {
                    vm_string_printf(VM_STRING("  -%s"), (vm_char*)pKey->m_sName);
                    for(i = 0; i < iMaxSizeShort - pKey->m_sName.Size() - 3; i++)
                        vm_string_printf(VM_STRING(" "));
                }
                else
                {
                    for(i = 0; i < iMaxSizeShort; i++)
                        vm_string_printf(VM_STRING(" "));
                }
            }

            if(iMaxSizeLong)
            {
                if(pKey->m_sLongName.Size())
                {
                    vm_string_printf(VM_STRING("  --%s"), (vm_char*)pKey->m_sLongName);
                    for(i = 0; i < iMaxSizeLong - pKey->m_sLongName.Size() - 4; i++)
                        vm_string_printf(VM_STRING(" "));
                }
                else
                {
                    for(i = 0; i < iMaxSizeLong; i++)
                        vm_string_printf(VM_STRING(" "));
                }
            }

            sFDesc.Clear();
            pDesc = pKey->m_sDescription;
            i = 0;
            while(pDesc[i] != '\0')
            {
                if(pDesc[i] == '\n')
                {
                    sFDesc.Append(pDesc, i+1);
                    pDesc += i+1; i = 0;

                    for(j = 0; j < iMaxSizeShort + iMaxSizeLong + 2; j++)
                        sFDesc += VM_STRING(" ");
                }
                else
                    i++;
            }
            if(sFDesc.Size())
            {
                sFDesc.Append(pDesc, i);
                sFDesc += VM_STRING("\n");
            }
            else
                sFDesc = pKey->m_sDescription;
            vm_string_printf(VM_STRING("  %s\n"), (vm_char*)sFDesc);
        }
    }

    m_bHelpPrinted = true;
}

void ParserCmd::AddKey(const vm_char *cName, const vm_char *cLongName, const vm_char *cDescription, KeyType keyType, Ipp32u iArraySize, bool bAllowEmpty, DString sGroupName)
{
    AddKey(cName, cLongName, VM_STRING(""), cDescription, keyType, iArraySize, bAllowEmpty, sGroupName);
}

void ParserCmd::AddKey(const vm_char *cName, const vm_char *cLongName, const vm_char *cCfgName, const vm_char *cDescription, KeyType keyType, Ipp32u iArraySize, bool bAllowEmpty, DString sGroupName)
{
    Ipp32u iSize = (Ipp32u)vm_string_strlen(cName);
    Ipp32u iLongSize = (Ipp32u)vm_string_strlen(cLongName);
    Ipp32u i;

    if(!iSize && !iLongSize && !bAllowEmpty) // only empty keys can be without names
        return;
    if(IsKeyExist(cName, cLongName))
        return;

    for(i = 0; i < iSize; i++)
    {
        if(!isValid((vm_char*)&cName[i], validSymbols))
            return;
    }

    for(i = 0; i < iLongSize; i++)
    {
        if(!isValid((vm_char*)&cLongName[i], validSymbols) && !isValid((vm_char*)&cLongName[i], validSymbolsExt) && !isValid((vm_char*)&cLongName[i], validDigits))
            return;
    }

    if(keyType == Boolean)
        iArraySize = 1;

    Key pKey(cName, cLongName, cCfgName, cDescription, keyType, iArraySize, bAllowEmpty, sGroupName);

    m_keys.PushBack();
    m_keys.Back() = pKey;
    m_iKeysCount++;
    if(bAllowEmpty)
        m_iEmptyKeysCount++;
}

template <class TYPE>
Ipp32u ParserCmd::GetParamInt(const vm_char *cName, const vm_char *cLongName, TYPE *iValue, Ipp32u iArraySize)
{
    Ipp32u iMinArraySize;
    for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
    {
        if((pKey->m_keyType != Integer && pKey->m_keyType != Boolean) || pKey->m_sName.Size() != vm_string_strlen(cName) || pKey->m_sLongName.Size() != vm_string_strlen(cLongName))
            continue;
        if(pKey->m_bAllocated && (!vm_string_strncmp(pKey->m_sName, cName, pKey->m_sName.Size()) && !vm_string_strncmp(pKey->m_sLongName, cLongName, pKey->m_sLongName.Size())))
        {
            if(!iValue)
                return pKey->m_iArraySize;
            iMinArraySize = IPP_MIN(pKey->m_iArraySize, iArraySize);
            if(pKey->m_keyType == Integer)
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    iValue[i] = (TYPE)pKey->m_uint[i];
            }
            else
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    iValue[i] = (TYPE)pKey->m_bool[i];
            }
            return iMinArraySize;
        }
    }

    return 0;
}

template <class TYPE>
Ipp32u ParserCmd::GetParamFloat(const vm_char *cName, const vm_char *cLongName, TYPE *fValue, Ipp32u iArraySize)
{
    Ipp32u iMinArraySize;
    for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
    {
        if((pKey->m_keyType != Boolean && pKey->m_keyType != Integer && pKey->m_keyType != Real) || pKey->m_sName.Size() != vm_string_strlen(cName) || pKey->m_sLongName.Size() != vm_string_strlen(cLongName))
            continue;
        if(pKey->m_bAllocated && (!vm_string_strncmp(pKey->m_sName, cName, pKey->m_sName.Size()) && !vm_string_strncmp(pKey->m_sLongName, cLongName, pKey->m_sLongName.Size())))
        {
            if(!fValue)
                return pKey->m_iArraySize;
            iMinArraySize = IPP_MIN(pKey->m_iArraySize, iArraySize);
            if(pKey->m_keyType == Integer)
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    fValue[i] = (TYPE)pKey->m_uint[i];
            }
            else if(pKey->m_keyType == Boolean)
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    fValue[i] = (TYPE)pKey->m_bool[i];
            }
            else
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    fValue[i] = (TYPE)pKey->m_double[i];
            }
            return iMinArraySize;
        }
    }

    return 0;
}

Ipp32u ParserCmd::GetParam(const vm_char *cName, const vm_char *cLongName, bool *bValue, Ipp32u iArraySize)
{
    Ipp32u iMinArraySize;
    for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
    {
        if((pKey->m_keyType != Boolean && pKey->m_keyType != Integer) || pKey->m_sName.Size() != vm_string_strlen(cName) || pKey->m_sLongName.Size() != vm_string_strlen(cLongName))
            continue;
        if(pKey->m_bAllocated && (!vm_string_strncmp(pKey->m_sName, cName, pKey->m_sName.Size()) && !vm_string_strncmp(pKey->m_sLongName, cLongName, pKey->m_sLongName.Size())))
        {
            if(!bValue)
                return pKey->m_iArraySize;
            iMinArraySize = IPP_MIN(pKey->m_iArraySize, iArraySize);
            if(pKey->m_keyType == Boolean)
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    bValue[i] = pKey->m_bool[i];
            }
            else
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    bValue[i] = (pKey->m_uint[i])?true:false;
            }
            return iMinArraySize;
        }
    }

    return 0;
}

Ipp32u ParserCmd::GetParam(const vm_char *cName, const vm_char *cLongName, Ipp8u *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp8u>(cName, cLongName, iValue, iArraySize);
}

Ipp32u ParserCmd::GetParam(const vm_char *cName, const vm_char *cLongName, Ipp8s *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp8s>(cName, cLongName, iValue, iArraySize);
}

Ipp32u ParserCmd::GetParam(const vm_char *cName, const vm_char *cLongName, Ipp16u *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp16u>(cName, cLongName, iValue, iArraySize);
}

Ipp32u ParserCmd::GetParam(const vm_char *cName, const vm_char *cLongName, Ipp16s *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp16s>(cName, cLongName, iValue, iArraySize);
}

Ipp32u ParserCmd::GetParam(const vm_char *cName, const vm_char *cLongName, Ipp32u *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp32u>(cName, cLongName, iValue, iArraySize);
}

Ipp32u ParserCmd::GetParam(const vm_char *cName, const vm_char *cLongName, Ipp32s *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp32s>(cName, cLongName, iValue, iArraySize);
}

Ipp32u ParserCmd::GetParam(const vm_char *cName, const vm_char *cLongName, Ipp32f *fValue, Ipp32u iArraySize)
{
    return GetParamFloat<Ipp32f>(cName, cLongName, fValue, iArraySize);
}

Ipp32u ParserCmd::GetParam(const vm_char *cName, const vm_char *cLongName, Ipp64f *fValue, Ipp32u iArraySize)
{
    return GetParamFloat<Ipp64f>(cName, cLongName, fValue, iArraySize);
}

Ipp32u ParserCmd::GetParam(const vm_char *cName, const vm_char *cLongName, DString *sValue, Ipp32u iArraySize)
{
    Ipp32u iMinArraySize;
    for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
    {
        if(pKey->m_keyType != String || pKey->m_sName.Size() != vm_string_strlen(cName) || pKey->m_sLongName.Size() != vm_string_strlen(cLongName))
            continue;
        if(pKey->m_bAllocated && (!vm_string_strncmp(pKey->m_sName, cName, pKey->m_sName.Size()) && !vm_string_strncmp(pKey->m_sLongName, cLongName, pKey->m_sLongName.Size())))
        {
            if(!sValue)
                return pKey->m_iArraySize;
            iMinArraySize = IPP_MIN(pKey->m_iArraySize, iArraySize);
            for(Ipp32u i = 0; i < iMinArraySize; i++)
                sValue[i] = pKey->m_string[i];
            return iMinArraySize;
        }
    }

    return 0;
}

Ipp32u ParserCmd::GetParamSize(const vm_char *cName, const vm_char *cLongName)
{
    for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
    {
        if(pKey->m_sName.Size() != vm_string_strlen(cName) || pKey->m_sLongName.Size() != vm_string_strlen(cLongName))
            continue;
        if(pKey->m_bAllocated && (!vm_string_strncmp(pKey->m_sName, cName, pKey->m_sName.Size()) && !vm_string_strncmp(pKey->m_sLongName, cLongName, pKey->m_sLongName.Size())))
            return pKey->m_iArraySize;
    }

    return 0;
}

ParserCmd::Key* ParserCmd::FindNextKey(const vm_char *cName, bool bLong, bool bEmpty)
{
    vm_char *cKeyName;
    Ipp32u   iNameSize;
    Ipp32u   iMaxSize = 0;
    Key*     retKey = 0;

    if(bEmpty)
    {
        for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
        {
            if(pKey->m_bAllocated)
                continue;
            if(pKey->m_bAllowEmpty)
            {
                retKey = pKey;
                break;
            }
        }
    }
    else
    {
        for(List<Key>::Iterator pKey = m_keys.ItrFront(); pKey != m_keys.ItrBackBound(); ++pKey)
        {
            if(pKey->m_bAllocated)
                continue;
            cKeyName  = ((bLong)?pKey->m_sLongName:pKey->m_sName);
            iNameSize = (Ipp32u)vm_string_strlen(cKeyName);
            if(!vm_string_strncmp(cKeyName, cName, iNameSize) && iNameSize)
            {
                if(iMaxSize <= iNameSize)
                {
                    iMaxSize = iNameSize;
                    retKey = pKey;
                }
            }
        }
    }

    m_pBuffer += iMaxSize;
    m_iBufferLen -= iMaxSize;

    return retKey;
}

bool ParserCmd::NextSymbol()
{
    if(m_iBufferLen > 0)
    {
        m_iBufferLen--;
        m_pBuffer++;
    }
    if(m_iBufferLen == 0)
        return false;

    return true;
}

bool ParserCmd::SkipSpaces()
{
    while(*m_pBuffer == 32 || *m_pBuffer == 9 || *m_pBuffer == 0)
    {
        if(!NextSymbol())
            return false;
    }

    return true;
}

void ParserCmd::EstimateSize(Key *pKey)
{
    vm_char *pPtr = m_pBuffer;
    Ipp32u   iLen = m_iBufferLen;
    Ipp32u   iSize;

    if(pKey->m_iArraySize != 0)
        return;

    do
    {
        iSize = (Ipp32u)vm_string_strlen(pPtr) + 1;
        pPtr += iSize;
        iLen -= iSize;
        pKey->m_iArraySize++;

    } while(pPtr[0] != '-' && iLen != 0);
}

bool ParserCmd::ParseInt(Key *pKey)
{
    Ipp32u iSize;
    vm_char* pBuf;
    Ipp32u   iBufLen;
    bool bError = false;

    if(!pKey->m_iArraySize) // dynamic size
    {
        EstimateSize(pKey);
    }
    pKey->Alloc();

    for(Ipp32u i = 0; i < pKey->m_iArraySize; i++)
    {
        if(m_iBufferLen == 0)
        {
            bError = true;
            break;
        }
        iSize = (Ipp32u)vm_string_strlen(m_pBuffer) + 1;
        iBufLen = iSize - 1;
        pBuf = m_pBuffer;

        if(*pBuf == 45 || *pBuf == 43)
        {
            pBuf++;
            iBufLen--;
        }

        while(iBufLen)
        {
            if(!isValid(pBuf, validDigits))
            {
                bError = true;
                break;
            }
            pBuf++;
            iBufLen--;
        }
        if(bError)
            break;

        pKey->m_uint[i] = (Ipp32u)vm_string_atoll(m_pBuffer);
        m_pBuffer += iSize;
        m_iBufferLen -= iSize;
    }

    if(bError)
    {
        pKey->Free();
        return false;
    }
    return true;
}

bool ParserCmd::ParseFloat(Key *pKey)
{
    Ipp32u iSize;
    vm_char* pBuf;
    Ipp32u   iBufLen;
    bool bError = false;
    bool bHaveSign = false;
    bool bHaveDot = false;

    if(!pKey->m_iArraySize) // dynamic size
    {
        EstimateSize(pKey);
    }
    pKey->Alloc();

    for(Ipp32u i = 0; i < pKey->m_iArraySize; i++)
    {
        if(m_iBufferLen == 0)
        {
            bError = true;
            break;
        }
        iSize = (Ipp32u)vm_string_strlen(m_pBuffer) + 1;
        iBufLen = iSize - 1;
        pBuf = m_pBuffer;
        bHaveSign = false;
        bHaveDot = false;

        while(iBufLen)
        {
            if(*pBuf == 45 || *pBuf == 43)
            {
                if(bHaveSign)
                {
                    bError = true;
                    break;
                }
                bHaveSign = true;
                pBuf++;
                iBufLen--;
            }

            if(*pBuf == 46)
            {
                if(bHaveDot)
                {
                    bError = true;
                    break;
                }
                bHaveDot = true;
                pBuf++;
                iBufLen--;
            }

            if(!isValid(pBuf, validDigits))
            {
                bError = true;
                break;
            }
            pBuf++;
            iBufLen--;
        }
        if(bError)
            break;

        pKey->m_double[i] = vm_string_atof(m_pBuffer);
        m_pBuffer += iSize;
        m_iBufferLen -= iSize;
    }

    if(bError)
    {
        pKey->Free();
        return false;
    }
    return true;
}

bool ParserCmd::ParseString(Key *pKey)
{
    Ipp32u iSize;
    bool bError = false;

    if(!pKey->m_iArraySize) // dynamic size
    {
        EstimateSize(pKey);
    }
    pKey->Alloc();

    for(Ipp32u i = 0; i < pKey->m_iArraySize; i++)
    {
        if(m_iBufferLen == 0)
        {
            bError = true;
            break;
        }
        iSize = (Ipp32u)vm_string_strlen(m_pBuffer) + 1;
        pKey->m_string[i] = m_pBuffer;
        m_pBuffer += iSize;
        m_iBufferLen -= iSize;
    }

    if(bError)
    {
        pKey->Free();
        return false;
    }
    return true;
}

Ipp32s ParserCmd::InitRes(vm_char *argv[], int argc, ParserCfg *pCfg)
{
    Ipp32s iStatus;

    m_bErrorResilience = true;
    iStatus = Init(argv, argc, pCfg);
    m_bErrorResilience = false;

    return iStatus;
}

Ipp32s ParserCmd::Init(vm_char *argv[], int argc, ParserCfg *pCfg)
{
    Key   *pKey;
    bool   bHaveKey      = false;
    bool   bBoolSequence = false;
    bool   bFinish       = false;
    Ipp32s iError        = 0;
    Ipp32s i;

    Reset();

    if(!m_sProgName.Size())
    {
        m_sProgName = argv[0];
        for(i = (Ipp32s)vm_string_strlen(argv[0]) - 1; i >= 0; i--)
        {
            if(argv[0][i] == '\\' || argv[0][i] == '/')
            {
                m_sProgNameShort = &argv[0][i+1];
                break;
            }
        }
        if(!m_sProgNameShort.Size())
            m_sProgNameShort = argv[0];
    }

    if(argc == 1 || m_keys.IsEmpty())
        return 0; // nothing to parse

    m_pBuffer = argv[1];

    for(i = 1; i < argc; i++)
        m_iBufferLen += ((Ipp32u)vm_string_strlen(argv[i]) + 1);

    while(m_iBufferLen > 0)
    {
        pKey = 0;
        bHaveKey = false;
        bBoolSequence = false;

        if(!SkipSpaces())
            bFinish = true;

        if(bFinish)
            break;

        if(*m_pBuffer == '-') // key prefix
        {
            bFinish = !NextSymbol();

            if(*m_pBuffer == '-') // long key prefix
            {
                bFinish = !NextSymbol();
                pKey = FindNextKey(m_pBuffer, true, false);
                if(!pKey)
                {
                    iError = 1;
                    if(m_bErrorResilience)
                    {
                        NextSymbol();
                        continue;
                    }
                    else
                        break;
                }
                if(pKey->m_keyType == Boolean)
                {
                    bBoolSequence = true;
                    pKey->Alloc();
                    pKey->m_bool[0] = true;
                }
            }
            else
            {
                pKey = FindNextKey(m_pBuffer, false, false);
                if(!pKey)
                {
                    iError = 1;
                    if(m_bErrorResilience)
                    {
                        NextSymbol();
                        continue;
                    }
                    else
                        break;
                }
                while(pKey->m_keyType == Boolean) // collect bool sequence
                {
                    bBoolSequence = true;
                    pKey->Alloc();
                    pKey->m_bool[0] = true;
                    if(pCfg && pKey->m_sCfgName)
                        pCfg->SetParam(pKey->m_sCfgName, pKey->m_bool, pKey->m_iArraySize);

                    if(!isValid(m_pBuffer, validSymbols))
                        break;
                    pKey = FindNextKey(m_pBuffer, false, false);
                    if(!pKey || pKey->m_keyType != Boolean) // found key but not boolean
                    {
                        iError = 1;
                        break;
                    }
                }
            }
        }
        else if(!m_bErrorResilience) // empty key
        {
            pKey = FindNextKey(m_pBuffer, false, true);
            if(!pKey)
            {
                iError = 1;
                break;
            }
        }
        if(!m_bErrorResilience && iError)
            break;

        if(bBoolSequence)
            continue;

        if(!SkipSpaces())
            bFinish = true;

        if(pKey)
        {
            switch(pKey->m_keyType)
            {
            case Integer:
                if(!ParseInt(pKey))
                    iError = 2;
                else if(pCfg && pKey->m_sCfgName.Size())
                    pCfg->SetParam(pKey->m_sCfgName, pKey->m_uint, pKey->m_iArraySize);
                break;
            case Real:
                if(!ParseFloat(pKey))
                    iError = 2;
                else if(pCfg && pKey->m_sCfgName.Size())
                    pCfg->SetParam(pKey->m_sCfgName, pKey->m_double, pKey->m_iArraySize);
                break;
            case String:
                if(!ParseString(pKey))
                    iError = 2;
                else if(pCfg && pKey->m_sCfgName.Size())
                    pCfg->SetParam(pKey->m_sCfgName, pKey->m_string, pKey->m_iArraySize);
                break;
            case Config:
                if(!pCfg || !ParseString(pKey) || !pCfg->ParseLine((const vm_char*)(*pKey->m_string)))
                    iError = 2;
                pKey->Free();
                break;
            default:
                iError = -1;
            }
        }
        else if(m_bErrorResilience)
        {
            NextSymbol();
            continue;
        }

        if(!m_bErrorResilience && iError)
            break;
    }

    return iError;
}
