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

/*
    This is common interface for configuration files parser.

    You can parse files with ParseFile() method or lines with ParseLine() method.
    For ParseFile() each parameter must be on new line, line break symbols must not be placed inside strigns.
    For ParseLine() parameters must be separated with semicolon, semicolon must not be placed inside strings.

    For possible valid symbols see validSymbols[] table.

    GetParam() - get parsed values, returns false if parameter was not found.
        Name         define a parameter which value needs to be returned.
        *pValue      dst array.
        ArraySize    size of dst array.
*/

#ifndef __UMC_PARSER_FILE_H__
#define __UMC_PARSER_FILE_H__

#include "vm_file.h"
#include "umc_list.h"
#include "umc_string.h"

namespace UMC
{

class ParserCfg
{
public:
    ParserCfg();
    ~ParserCfg();

    bool    ParseFile(const vm_char *cFilename);
    bool    ParseLine(const vm_char *cLine);
    bool    SetParam(const vm_char *cName, bool    *bValue, Ipp32u iArraySize);
    bool    SetParam(const vm_char *cName, Ipp32u  *iValue, Ipp32u iArraySize);
    bool    SetParam(const vm_char *cName, Ipp64f  *fValue, Ipp32u iArraySize);
    bool    SetParam(const vm_char *cName, DString *sValue, Ipp32u iArraySize);
    bool    GetParam(const vm_char *cName, bool    *bValue, Ipp32u iArraySize);
    bool    GetParam(const vm_char *cName, Ipp8s   *iValue, Ipp32u iArraySize);
    bool    GetParam(const vm_char *cName, Ipp8u   *iValue, Ipp32u iArraySize);
    bool    GetParam(const vm_char *cName, Ipp16s  *iValue, Ipp32u iArraySize);
    bool    GetParam(const vm_char *cName, Ipp16u  *iValue, Ipp32u iArraySize);
    bool    GetParam(const vm_char *cName, Ipp32s  *iValue, Ipp32u iArraySize);
    bool    GetParam(const vm_char *cName, Ipp32u  *iValue, Ipp32u iArraySize);
    bool    GetParam(const vm_char *cName, Ipp32f  *fValue, Ipp32u iArraySize);
    bool    GetParam(const vm_char *cName, Ipp64f  *fValue, Ipp32u iArraySize);
    bool    GetParam(const vm_char *cName, DString *sValue, Ipp32u iArraySize);
    Ipp32u  GetNumOfkeys();
    void    PrintData(); // print all parsed data

protected:
    class Key
    {
    public:
        typedef enum
        {
            Boolean,
            Real,
            Integer,
            String
        } KeyType;

        Key() {};

        Key(const vm_char *cName, bool    *bValue, Ipp32u iArraySize)
        {
            m_sName = cName;
            m_keyType = Boolean;
            m_iArraySize = iArraySize;
            m_bool = new bool[iArraySize];
            m_bAllocated = true;
            for(Ipp32u i = 0; i < iArraySize; i++)
                m_bool[i] = bValue[i];
        };
        Key(const vm_char *cName, Ipp64f  *fValue, Ipp32u iArraySize)
        {
            m_sName = cName;
            m_keyType = Real;
            m_iArraySize = iArraySize;
            m_double = new double[iArraySize];
            m_bAllocated = true;
            for(Ipp32u i = 0; i < iArraySize; i++)
                m_double[i] = fValue[i];
        };
        Key(const vm_char *cName, Ipp32u  *iValue, Ipp32u iArraySize)
        {
            m_sName = cName;
            m_keyType = Integer;
            m_iArraySize = iArraySize;
            m_uint = new unsigned int[iArraySize];
            m_bAllocated = true;
            for(Ipp32u i = 0; i < iArraySize; i++)
                m_uint[i] = iValue[i];
        };
        Key(const vm_char *cName, DString *sValue, Ipp32u iArraySize)
        {
            m_sName = cName;
            m_keyType = String;
            m_iArraySize = iArraySize;
            m_string = new DString[iArraySize];
            m_bAllocated = true;
            for(Ipp32u i = 0; i < iArraySize; i++)
                m_string[i] = sValue[i];
        };

        void Close() 
        {
            if(m_bAllocated)
            {
                switch(m_keyType)
                {
                case Boolean:
                    delete[] m_bool;
                    break;
                case Integer:
                    delete[] m_uint;
                    break;
                case Real:
                    delete[] m_double;
                    break;
                case String:
                    delete[] m_string;
                    break;
                }
                m_bAllocated = false;
            }
        };

        DString m_sName;
        Ipp32u  m_iArraySize;
        KeyType m_keyType;
        bool    m_bAllocated;

        union
        {
            bool    *m_bool;
            Ipp32u  *m_uint;
            Ipp64f  *m_double;
            DString *m_string;
        };
    };

    List<Key> m_keys;
    vm_file  *m_file;
    vm_char  *m_pReadBuffer;
    vm_char  *m_pCVTBuffer;
    vm_char  *m_pBuffer;
    Ipp32u    m_iBufferLen;
    bool      m_bEOF;
    Ipp32u    m_iCharSize;
    bool      m_bUnicode;
    bool      m_bChecked;

    bool NextSymbol(vm_char **ppBuffer = NULL, Ipp32u *pBufferLen = NULL);
    bool NextLine(vm_char **ppBuffer = NULL, Ipp32u *pBufferLen = NULL);
    bool NextPair(vm_char **ppBuffer = NULL, Ipp32u *pBufferLen = NULL);
    bool SkipSpaces(vm_char **ppBuffer = NULL, Ipp32u *pBufferLen = NULL);
    bool IsKeyExist(const vm_char *key);
    bool ParseAsDigit(vm_char *name, vm_char *value);
    bool ParseAsString(vm_char *name, vm_char *value);
    template<class TYPE>
    bool SetParam(const vm_char *name, TYPE  *value, Ipp32u array_size);
    template<class TYPE>
    bool GetParamInt(const vm_char *name, TYPE  *value, Ipp32u array_size);
    template<class TYPE>
    bool GetParamFloat(const vm_char *name, TYPE  *value, Ipp32u array_size);
private:
    ParserCfg(const ParserCfg&) {}
    const ParserCfg& operator=(const ParserCfg&) { return *this; }
};

}
#endif
