
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

#include "umc_parser_cfg.h"

using namespace UMC;


#define PARSER_FILE_BUFFER 8192 // 8Kb enought for most par files

static const vm_char validSymbols[] = { VM_STRING("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_") };
static const vm_char validDigits[]  = { VM_STRING("0123456789") };

static bool isValidValue(vm_char *symbol, size_t iSize, bool bHaveDot)
{
    size_t iArrSize = sizeof(validDigits)/sizeof(vm_char) - 1;
    if(*symbol == 46 && !bHaveDot)
        return true;
    if(iSize == 0)
    {
        if(*symbol == 45 || *symbol == 43)
            return true;
    }
    for(unsigned int i = 0; i < iArrSize; i++)
    {
        if(*symbol == validDigits[i])
            return true;
    }

    return false;
}

static bool isValidDigit(vm_char *symbol)
{
    size_t iArrSize = sizeof(validDigits)/sizeof(vm_char) - 1;
    for(unsigned int i = 0; i < iArrSize; i++)
    {
        if(*symbol == validDigits[i])
            return true;
    }

    return false;
}

static bool isValidSymbol(vm_char *symbol, size_t iSize)
{
    size_t iArrSize = sizeof(validSymbols)/sizeof(vm_char) - 1;
    for(unsigned int i = 0; i < iArrSize; i++)
    {
        if(*symbol == validSymbols[i])
            return true;
    }
    if(iSize > 0) // Name can't start from digit
        return isValidDigit(symbol);

    return false;
}

ParserCfg::ParserCfg()
{
    m_file        = NULL;
    m_pCVTBuffer  = NULL;
    m_pReadBuffer = NULL;
    m_iBufferLen  = 0;
    m_iCharSize   = 1;
    m_bEOF        = false;
    m_bChecked    = false;
    m_bUnicode    = false;
}

ParserCfg::~ParserCfg()
{
    if(m_pCVTBuffer)
    {
        delete[] m_pCVTBuffer;
        m_pCVTBuffer = NULL;
    }
    if(m_pReadBuffer)
    {
        delete[] m_pReadBuffer;
        m_pReadBuffer = NULL;
    }
    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
        key->Close();
}

void ParserCfg::PrintData()
{
    size_t iSizeMax = 0;
    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        if(key->m_sName.Size() > iSizeMax)
            iSizeMax = key->m_sName.Size();
    }
    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        vm_string_printf(key->m_sName);
        vm_string_printf(VM_STRING("[%d]"), key->m_iArraySize);

        for(Ipp32u i = 0; i < iSizeMax - key->m_sName.Size(); i++)
            vm_string_printf(VM_STRING("."));
         vm_string_printf(VM_STRING(".="));

        for(Ipp32u i = 0; i < key->m_iArraySize; i++)
        {
            switch(key->m_keyType)
            {
            case Key::Boolean:
                if(key->m_bool[i] == true)
                    vm_string_printf(VM_STRING(" true"));
                else
                    vm_string_printf(VM_STRING(" false"));
                break;
            case Key::Integer:
                vm_string_printf(VM_STRING(" %d"), key->m_uint[i]);
                break;
            case Key::Real:
                vm_string_printf(VM_STRING(" %f"), key->m_double[i]);
                break;
            case Key::String:
                vm_string_printf(VM_STRING(" \""));
                vm_string_printf(key->m_string[i]);
                vm_string_printf(VM_STRING("\""));
                break;
            }
            if(i != (key->m_iArraySize - 1))
                vm_string_printf(VM_STRING(","));
        }
        vm_string_printf(VM_STRING("\n"));
    }
}

Ipp32u ParserCfg::GetNumOfkeys()
{
    Ipp32u iKeys = 0;

    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
        iKeys++;

    return iKeys;
}

template<class TYPE>
bool ParserCfg::SetParam(const vm_char *cName, TYPE *value, Ipp32u iArraySize)
{
    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        if(key->m_sName.Size() == vm_string_strlen(cName) && !vm_string_strncmp(key->m_sName, cName, key->m_sName.Size()))
        {
            key->Close();
            m_keys.Remove(&key);
            break;
        }
    }

    Key a(cName, value, iArraySize);
    m_keys.PushBack();
    m_keys.Back() = a;

    return true;
}

bool ParserCfg::SetParam(const vm_char *cName, bool *bValue, Ipp32u iArraySize)
{
    return SetParam<bool>(cName, bValue, iArraySize);
}

bool ParserCfg::SetParam(const vm_char *cName, Ipp32u *iValue, Ipp32u iArraySize)
{
    return SetParam<Ipp32u>(cName, iValue, iArraySize);
}

bool ParserCfg::SetParam(const vm_char *cName, Ipp64f *fValue, Ipp32u iArraySize)
{
    return SetParam<Ipp64f>(cName, fValue, iArraySize);
}

bool ParserCfg::SetParam(const vm_char *cName, DString *sValue, Ipp32u iArraySize)
{
    return SetParam<DString>(cName, sValue, iArraySize);
}

template <class TYPE>
bool ParserCfg::GetParamInt(const vm_char *cName, TYPE *iValue, Ipp32u iArraySize)
{
    Ipp32u iMinArraySize;
    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        if((key->m_keyType == Key::Integer || key->m_keyType == Key::Boolean) && (vm_string_strncmp(key->m_sName, cName, key->m_sName.Size()) == 0))
        {
            iMinArraySize = IPP_MIN(key->m_iArraySize, iArraySize);
            if(key->m_keyType == Key::Integer)
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    iValue[i] = (TYPE)key->m_uint[i];
            }
            else
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    iValue[i] = (TYPE)key->m_bool[i];
            }
            return true;
        }
    }

    return false;
}

template <class TYPE>
bool ParserCfg::GetParamFloat(const vm_char *cName, TYPE *fValue, Ipp32u iArraySize)
{
    Ipp32u iMinArraySize;
    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        if((key->m_keyType == Key::Boolean || key->m_keyType == Key::Integer || key->m_keyType == Key::Real) && (vm_string_strncmp(key->m_sName, cName, key->m_sName.Size()) == 0))
        {
            iMinArraySize = IPP_MIN(key->m_iArraySize, iArraySize);
            if(key->m_keyType == Key::Integer)
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    fValue[i] = (TYPE)key->m_uint[i];
            }
            else if(key->m_keyType == Key::Boolean)
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    fValue[i] = (TYPE)key->m_bool[i];
            }
            else
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    fValue[i] = (TYPE)key->m_double[i];
            }
            return true;
        }
    }

    return false;
}

bool ParserCfg::GetParam(const vm_char *cName, bool *bValue, Ipp32u iArraySize)
{
    Ipp32u iMinArraySize;
    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        if((key->m_keyType == Key::Boolean || key->m_keyType == Key::Integer) && (vm_string_strncmp(key->m_sName, cName, key->m_sName.Size()) == 0))
        {
            iMinArraySize = IPP_MIN(key->m_iArraySize, iArraySize);
            if(key->m_keyType == Key::Boolean)
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    bValue[i] = key->m_bool[i];
            }
            else
            {
                for(Ipp32u i = 0; i < iMinArraySize; i++)
                    bValue[i] = (key->m_uint[i])?true:false;
            }
            return true;
        }
    }

    return false;
}

bool ParserCfg::GetParam(const vm_char *cName, Ipp8u *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp8u>(cName, iValue, iArraySize);
}

bool ParserCfg::GetParam(const vm_char *cName, Ipp8s *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp8s>(cName, iValue, iArraySize);
}

bool ParserCfg::GetParam(const vm_char *cName, Ipp16u *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp16u>(cName, iValue, iArraySize);
}


bool ParserCfg::GetParam(const vm_char *cName, Ipp16s *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp16s>(cName, iValue, iArraySize);
}

bool ParserCfg::GetParam(const vm_char *cName, Ipp32u *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp32u>(cName, iValue, iArraySize);
}

bool ParserCfg::GetParam(const vm_char *cName, Ipp32s *iValue, Ipp32u iArraySize)
{
    return GetParamInt<Ipp32s>(cName, iValue, iArraySize);
}

bool ParserCfg::GetParam(const vm_char *cName, Ipp32f *fValue, Ipp32u iArraySize)
{
    return GetParamFloat<Ipp32f>(cName, fValue, iArraySize);
}

bool ParserCfg::GetParam(const vm_char *cName, Ipp64f *fValue, Ipp32u iArraySize)
{
    return GetParamFloat<Ipp64f>(cName, fValue, iArraySize);
}

bool ParserCfg::GetParam(const vm_char *cName, DString *sValue, Ipp32u iArraySize)
{
    Ipp32u iMinArraySize;
    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        if(key->m_keyType == Key::String && (vm_string_strncmp(key->m_sName, cName, key->m_sName.Size()) == 0))
        {
            iMinArraySize = IPP_MIN(key->m_iArraySize, iArraySize);
            for(Ipp32u i = 0; i < iMinArraySize; i++)
                sValue[i] = key->m_string[i];
            return true;
        }
    }

    return false;
}

bool ParserCfg::IsKeyExist(const vm_char *str)
{
    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        if(vm_string_strlen(str) == key->m_sName.Size() && !vm_string_strncmp(key->m_sName, str, key->m_sName.Size()))
            return true;
    }

    return false;
}

bool ParserCfg::NextSymbol(vm_char **ppBuffer, Ipp32u *pBufferLen)
{
    if(ppBuffer)
    {
        if(*pBufferLen > 0)
        {
            (*pBufferLen)--;
            (*ppBuffer)++;
        }
        if(*pBufferLen == 0)
            return false;
    }
    else
    {
        if(m_iBufferLen > 0)
        {
            m_iBufferLen--;
            m_pBuffer++;
        }
        if(!m_bEOF && m_iBufferLen == 0)
        {
            Ipp32s iCount;
            if(NULL == m_pReadBuffer)
                m_pReadBuffer = (vm_char*)new Ipp8u[PARSER_FILE_BUFFER];

            iCount = (Ipp32s)vm_file_fread(m_pReadBuffer, 1, PARSER_FILE_BUFFER, m_file);
            if(iCount != PARSER_FILE_BUFFER)
                m_bEOF = true;
            if(iCount == 0)
                return false;

            m_pBuffer = m_pReadBuffer;

#ifdef WINDOWS
            if(!m_bChecked && iCount > 4)
            {
                Ipp8u *pStartCode = (Ipp8u*)m_pBuffer;
                if(pStartCode[0] == 0xEF && pStartCode[1] == 0xBB && pStartCode[2] == 0xBF) // UTF-8
                {
                    pStartCode += 3;
                    iCount     -= 3;
                }
                else if(pStartCode[0] == 0xFF && pStartCode[1] == 0xFE) // UTF-16LE
                {
                    m_iCharSize = 2;
                    pStartCode += 2;
                    iCount     -= 2;
                    m_bUnicode = true;
                }
                else if(pStartCode[0] == 0xFE && pStartCode[1] == 0xFF) // UTF-16BE
                    return false;
                else if(pStartCode[0] == 0xFF && pStartCode[1] == 0xFE && pStartCode[0] == 0x00 && pStartCode[1] == 0x00) // UTF-32LE
                    return false;
                else if(pStartCode[0] == 0x00 && pStartCode[1] == 0x00 && pStartCode[0] == 0xFE && pStartCode[1] == 0xFF) // UTF-32BE
                    return false;

                m_pBuffer  = (vm_char*)pStartCode;
                m_bChecked = true;
            }
#endif
            m_iBufferLen = iCount;

#ifdef WINDOWS
#ifdef _UNICODE
            if(!m_bUnicode)
            {   // ASCII to unicode
                if(NULL == m_pCVTBuffer)
                    m_pCVTBuffer = (vm_char*)new Ipp8u[PARSER_FILE_BUFFER*2];

                vm_line_to_unicode((Ipp8u*)m_pBuffer, m_iBufferLen, (Ipp8u*)m_pCVTBuffer, PARSER_FILE_BUFFER*2);
                m_pBuffer    = m_pCVTBuffer;
            }
            else
                m_iBufferLen = iCount/m_iCharSize + iCount%m_iCharSize;
#else
            if(m_bUnicode)
            {   // Unicode to ASCII
                if(NULL == m_pCVTBuffer)
                    m_pCVTBuffer = (vm_char*)new Ipp8u[PARSER_FILE_BUFFER/m_iCharSize];
                m_iBufferLen = iCount/m_iCharSize + iCount%m_iCharSize;

                vm_line_to_ascii((Ipp8u*)m_pBuffer, m_iBufferLen, (Ipp8u*)m_pCVTBuffer, PARSER_FILE_BUFFER/m_iCharSize);
                m_pBuffer = m_pCVTBuffer;
            }
#endif
#endif
        }
        if(m_iBufferLen == 0)
            return false;
    }
    return true;
}

bool ParserCfg::NextLine(vm_char **ppBuffer, Ipp32u *pBufferLen)
{
    vm_char *pBuffer = ((ppBuffer)?(*ppBuffer):(m_pBuffer));
    while(*pBuffer != 10 && *pBuffer != 13) // skip till eol symbols
    {
        if(!NextSymbol(ppBuffer, pBufferLen))
            return false;
        pBuffer = ((ppBuffer)?(*ppBuffer):(m_pBuffer));
    }
    while(*pBuffer == 10 || *pBuffer == 13) // skip eol symbols
    {
        if(!NextSymbol(ppBuffer, pBufferLen))
            return false;
        pBuffer = ((ppBuffer)?(*ppBuffer):(m_pBuffer));
    }

    return true;
}

bool ParserCfg::NextPair(vm_char **ppBuffer, Ipp32u *pBufferLen)
{
    vm_char *pBuffer = ((ppBuffer)?(*ppBuffer):(m_pBuffer));
    while(*pBuffer != 59) // skip till semicolon symbols
    {
        if(!NextSymbol(ppBuffer, pBufferLen))
            return false;
        pBuffer = ((ppBuffer)?(*ppBuffer):(m_pBuffer));
    }
    while(*pBuffer == 59) // skip semicolon symbols
    {
        if(!NextSymbol(ppBuffer, pBufferLen))
            return false;
        pBuffer = ((ppBuffer)?(*ppBuffer):(m_pBuffer));
    }

    return true;
}

bool ParserCfg::SkipSpaces(vm_char **ppBuffer, Ipp32u *pBufferLen)
{
    vm_char *pBuffer = ((ppBuffer)?(*ppBuffer):(m_pBuffer));
    while(*pBuffer == 32 || *pBuffer == 9)
    {
        if(!NextSymbol(ppBuffer, pBufferLen))
            return false;
        pBuffer = ((ppBuffer)?(*ppBuffer):(m_pBuffer));
    }

    return true;
}

bool ParserCfg::ParseAsDigit(vm_char *name, vm_char *value)
{
    DString sDigit;
    DString sArray;
    Ipp32u iArraySize = 0;
    bool bHaveDigit   = false;
    bool bHaveComma   = false;
    bool bHaveDot     = false;
    bool bFloatArray  = false;
    bool bNotNumber   = false;
    bool bFinish      = false;
    const vm_char cArraySeparator = 0x00;

    vm_char *pTmpBuffer    = value;
    Ipp32u   iTmpBufferLen = (Ipp32u)vm_string_strlen(value);

    // parse as digit
    while(iTmpBufferLen > 0 && !bNotNumber && !bFinish)
    {
        bHaveDigit = false;
        bHaveDot = false;
        sDigit.Clear();

        if(!SkipSpaces(&pTmpBuffer, &iTmpBufferLen))
            bFinish = true;
        while(!bHaveDigit)
        {
            if(isValidValue(pTmpBuffer, sDigit.Size(), bHaveDot) && !bFinish)
            {
                if(*pTmpBuffer == 46)
                    bFloatArray = bHaveDot = true;
                sDigit = sDigit + *pTmpBuffer;
                bFinish = !NextSymbol(&pTmpBuffer, &iTmpBufferLen);
            }
            else
            {
                if(sDigit.Size())
                {
                    if(*pTmpBuffer == 35) // check for comment field
                        bFinish = true;
                    if(*pTmpBuffer != 32 && *pTmpBuffer != 9 && *pTmpBuffer != 44 && *pTmpBuffer != 35 && !bFinish) // only spaces and "," are allowed for digit break
                    {
                        bFinish = true;
                        bNotNumber = true;
                        break;
                    }
                    bHaveComma = false;
                    bHaveDigit = true;
                    iArraySize++;
                    sArray = sArray + sDigit + cArraySeparator;
                }
                else // not digit
                {
                    bFinish = true;
                    bNotNumber = true;
                    break;
                }
            }
        }

        if(!SkipSpaces(&pTmpBuffer, &iTmpBufferLen))
            bFinish = true;
        if(bHaveDigit && !bFinish)
        {
            if(*pTmpBuffer == 35) // check for comment field
                break;
            if(bHaveComma) // double comma
            {
                bNotNumber = true;
                bFinish = true;
                break;
            }
            if(*pTmpBuffer == 44) // only "," is allowed as array separator
            {
                bFinish = !NextSymbol(&pTmpBuffer, &iTmpBufferLen);
                bHaveComma = true;
            }
            else
            {
                bNotNumber = true;
                bFinish = true;
                break;
            }
        }
    }

    if(bHaveComma || bNotNumber || !iArraySize) // empty comma
        return false;

    if(bFloatArray)
    {
        Ipp64f  *fOutArray = new Ipp64f[iArraySize];
        vm_char *pBuffer = sArray;
        for(Ipp32u i = 0; i < iArraySize; i++)
        {
            fOutArray[i] = vm_string_atof(pBuffer);
            pBuffer += (vm_string_strlen(pBuffer) + 1);
        }

        Key a(name, fOutArray, iArraySize);

        m_keys.PushBack();
        m_keys.Back() = a;

        delete[] fOutArray;
    }
    else
    {
        Ipp32u  *iOutArray = new Ipp32u[iArraySize];
        vm_char *pBuffer = sArray;
        for(Ipp32u i = 0; i < iArraySize; i++)
        {
            iOutArray[i] = vm_string_atoi(pBuffer);
            pBuffer += (vm_string_strlen(pBuffer) + 1);
        }

        Key a(name, iOutArray, iArraySize);

        m_keys.PushBack();
        m_keys.Back() = a;

        delete[] iOutArray;
    }

    return true;
}

bool ParserCfg::ParseAsString(vm_char *name, vm_char *value)
{
    DString sString;
    DString sArray;
    Ipp32u iArraySize = 0;
    Ipp32u iSpacesCount = 0;
    bool bHaveString  = false;
    bool bHaveQuotes  = false;
    bool bBoolArray   = true;
    bool bFinish      = false;
    bool bError       = false;
    vm_char cCurrentQuote = (vm_char)-1;
    const vm_char cArraySeparator = 0x00;
    const vm_char cTrue[]  = { VM_STRING("true") };
    const vm_char cFalse[] = { VM_STRING("false") };

    vm_char *pTmpBuffer    = value;
    Ipp32u   iTmpBufferLen = (Ipp32u)vm_string_strlen(value);

    // parse as string
    while(!bFinish)
    {
        bHaveQuotes = false;
        bHaveString = false;
        sString.Clear();

        if(iTmpBufferLen == 0)
            bFinish = true;
        if(!SkipSpaces(&pTmpBuffer, &iTmpBufferLen))
            bFinish = true;
        while(!bHaveString)
        {
            if(cCurrentQuote == -1 && (*pTmpBuffer == 34 || *pTmpBuffer == 39)) // remember open quote type
                cCurrentQuote = *pTmpBuffer;
            if(*pTmpBuffer != cCurrentQuote && *pTmpBuffer != 44 && *pTmpBuffer != 35 && !bFinish) // check for break symbols
            {
                if(*pTmpBuffer == 32 || *pTmpBuffer == 9) // count spaces for trimming
                    iSpacesCount++;
                else
                    iSpacesCount = 0;
                sString = sString + *pTmpBuffer;
                bFinish = !NextSymbol(&pTmpBuffer, &iTmpBufferLen);
            }
            else
            {
                if(*pTmpBuffer == cCurrentQuote) // Start quote section
                {
                    if(sString.Size() != 0) // Quotes can not be placed in the middle of the value
                    {
                        bError  = true;
                        bFinish = true;
                        break;
                    }
                    bFinish = !NextSymbol(&pTmpBuffer, &iTmpBufferLen); // skip open quote
                    bBoolArray = false; // quoted boolean is treated as string
                    while(*pTmpBuffer != cCurrentQuote && !bFinish)
                    {
                        sString = sString + *pTmpBuffer;
                        bFinish = !NextSymbol(&pTmpBuffer, &iTmpBufferLen);
                        if(bFinish) // eol without quote close
                            bError = true;
                    }
                    bFinish = !NextSymbol(&pTmpBuffer, &iTmpBufferLen); // skip close quote
                    cCurrentQuote = (vm_char)-1;
                }
                else if(*pTmpBuffer == 35) // check for comment field
                    bFinish = true;

                bHaveString = true;
                iArraySize++;
                if(iSpacesCount) // cut tail spaces
                {
                    sString.TrimRight();
                    iSpacesCount = 0;
                }
                if(bBoolArray) // check for boolean style
                {
                    if(vm_string_stricmp(sString, cTrue) != 0 && vm_string_stricmp(sString, cFalse) != 0)
                        bBoolArray = false;
                }
                sArray = sArray + sString + cArraySeparator;
            }
        }

        if(!SkipSpaces(&pTmpBuffer, &iTmpBufferLen))
            bFinish = true;
        if(bHaveString && !bFinish)
        {
            if(*pTmpBuffer == 35) // check for comment field
                break;
            if(*pTmpBuffer == 44) // only "," is allowed as array separator
                bFinish = !NextSymbol(&pTmpBuffer, &iTmpBufferLen);
            else
            {
                bError = true;
                break;
            }
        }
    }

    if(bError) // if error save raw string
    {
        sString = value;

        Key a(name, &sString, 1);

        m_keys.PushBack();
        m_keys.Back() = a;
    }
    else
    {
        if(!iArraySize)
            return false;

        if(bBoolArray)
        {
            bool *bOutArray = new bool[iArraySize];
            vm_char *pBuffer = sArray;
            for(Ipp32u i = 0; i < iArraySize; i++)
            {
                if(vm_string_stricmp(pBuffer, cTrue) == 0)
                    bOutArray[i] = true;
                else
                    bOutArray[i] = false;
                pBuffer += (vm_string_strlen(pBuffer) + 1);
            }

            Key a(name, bOutArray, iArraySize);

            m_keys.PushBack();
            m_keys.Back() = a;

            delete[] bOutArray;
        }
        else
        {
            DString *sOutArray = new DString[iArraySize];
            vm_char *pBuffer = sArray;
            for(Ipp32u i = 0; i < iArraySize; i++)
            {
                sOutArray[i] = pBuffer;
                pBuffer += (vm_string_strlen(pBuffer) + 1);
            }

            Key a(name, sOutArray, iArraySize);

            m_keys.PushBack();
            m_keys.Back() = a;

            delete[] sOutArray;
        }
    }

    return true;
}

bool ParserCfg::ParseFile(const vm_char *filename)
{
    DString sName;
    DString sValue;
    bool bHaveName = false;
    bool bHaveEq   = false;
    bool bFinish   = false;

#if (PARSER_FILE_BUFFER%4)
    // buffer size must be multiple of 4
    return false;
#endif

    m_file = vm_file_open(filename, VM_STRING("rt"));
    if(m_file == NULL)
        return false;
    NextSymbol();

    while(!m_bEOF || m_iBufferLen > 0)
    {
        bHaveName = false;
        bHaveEq   = false;
        sName.Clear();
        sValue.Clear();

        if(!SkipSpaces())
            bFinish = true;
        //check name
        while(!bHaveName) // read name
        {
            if(isValidSymbol(m_pBuffer, sName.Size()) && !bFinish) // only letters and digits allowed
            {
                sName = sName + *m_pBuffer;
                bFinish = !NextSymbol();
            }
            else
            {
                if(sName.Size())
                {
                    if(*m_pBuffer != 32 && *m_pBuffer != 9 && *m_pBuffer != 61) // only spaces and "=" are allowed for name break
                    {
                        NextLine();
                        break;
                    }
                    if(IsKeyExist(sName)) // ignore duplicated name
                    {
                        NextLine();
                        break;
                    }
                    bHaveName = true;
                }
                else // looks like garbage, skipping line
                {
                    NextLine();
                    break;
                }
            }
        }

        if(!SkipSpaces())
            bFinish = true;
        //check eq
        if(bHaveName && !bHaveEq)
        {
            if(*m_pBuffer == 61) // only "=" is allowed as operator
            {
                bFinish = !NextSymbol();
                bHaveEq = true;
            }
            else
                NextLine();
        }

        if(!SkipSpaces())
            bFinish = true;
        //check value
        while(bHaveName && bHaveEq)
        {
            if(*m_pBuffer != 10 && *m_pBuffer != 13 && !bFinish) // check for line break
            {
                sValue = sValue + *m_pBuffer;
                bFinish = !NextSymbol();
            }
            else
            {
                if(sValue.Size())
                {
                    if(!ParseAsDigit(sName, sValue))
                        ParseAsString(sName, sValue);
                }
                else
                    ParseAsString(sName, sValue); // save as empty string
                break;
            }
        }
    }

    vm_file_close(m_file);
    return true;
}

bool ParserCfg::ParseLine(const vm_char *cLine)
{
    DString sName;
    DString sValue;
    vm_char*  pBuffer = (vm_char*)cLine;
    Ipp32u    iStrLen = (Ipp32u)vm_string_strlen(cLine);
    bool bHaveName = false;
    bool bHaveEq   = false;
    bool bFinish   = false;

    while(iStrLen > 0)
    {
        bHaveName = false;
        bHaveEq   = false;
        sName.Clear();
        sValue.Clear();

        if(!SkipSpaces(&pBuffer, &iStrLen))
            bFinish = true;
        //check name
        while(!bHaveName) // read name
        {
            if(isValidSymbol(pBuffer, sName.Size()) && !bFinish) // only letters and digits allowed
            {
                sName = sName + *pBuffer;
                bFinish = !NextSymbol(&pBuffer, &iStrLen);
            }
            else
            {
                if(sName.Size())
                {
                    if(*pBuffer != 32 && *pBuffer != 9 && *pBuffer != 61) // only spaces and "=" are allowed for name break
                    {
                        NextPair(&pBuffer, &iStrLen);
                        break;
                    }
                    if(IsKeyExist(sName)) // ignore duplicated name
                    {
                        NextPair(&pBuffer, &iStrLen);
                        break;
                    }
                    bHaveName = true;
                }
                else // looks like garbage, skipping line
                {
                    NextPair(&pBuffer, &iStrLen);
                    break;
                }
            }
        }

        if(!SkipSpaces(&pBuffer, &iStrLen))
            bFinish = true;
        //check eq
        if(bHaveName && !bHaveEq)
        {
            if(*pBuffer == 61) // only "=" is allowed as operator
            {
                bFinish = !NextSymbol(&pBuffer, &iStrLen);
                bHaveEq = true;
            }
            else
                NextPair(&pBuffer, &iStrLen);
        }

        if(!SkipSpaces(&pBuffer, &iStrLen))
            bFinish = true;
        //check value
        while(bHaveName && bHaveEq)
        {
            if(*pBuffer != 59 && !bFinish) // check for semicolon
            {
                sValue = sValue + *pBuffer;
                bFinish = !NextSymbol(&pBuffer, &iStrLen);
            }
            else
            {
                if(sValue.Size())
                {
                    if(!ParseAsDigit(sName, sValue))
                        ParseAsString(sName, sValue);
                }
                else
                    ParseAsString(sName, sValue); // save as empty string
                break;
            }
        }
    }

    return true;
}
