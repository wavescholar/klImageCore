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
    This is common interface for console commands parser.

    To use this interface you need to add keys first with AddKey() method
    AddKey uses several parameters:
        ShortKeyName must be one symbol long and can be accessed by single dash "-"
        LongKeyName  can use multiple symbols and can be accessed by double dash "--"
        CfgName      special name which can be used to fill values in Configuration Parser and can be accessed with Config key type
        KeyType      allocate key as bool (bool keys don't use values), integer, double, DString or special Config
        ArraySize    how many values separated by space should be parsed for each key. 1 is default value, 0 - count size until next key symbol or eob. Boolean keys are restricted to size 1
        AllowEmpty   allows value to be passed without use of key name. Must be used with caution to avoid ambiguity.
        Group        assign a group to a key, to output keys in group order

    For possible valid symbols see validSymbols[] and validSymbolsExt[] tables.

    Init(argv, argc, pCfg) - parse command line, returns error code (0 - no error, 1 - unknown or improper key present, 2 - value parsing error)
        pCfg pointer to ParserCfg object for CfgName value translation, NULL by default.

    InitRes(argv, argc, pCfg) - parse command line in error resilience mode which will ignore unknown keys and parsing errors
    thus allowing to use multiple initializations with new keys (e.g. if you want to use different sets of keys for different modes).
    InitRes does not support empty keys.

    Reset() - frees internal allocated memory. Used by Init().

    GetParam() - get parsed values, returns minimal array size of key and dst buffer, 0 means that key was not found.
        ShortKeyName and LongKeyName define a key which value needs to be returned.
        *pValue      dst array.
        ArraySize    size of dst array.

    GetParamSize() - returns key array size
*/

#ifndef __UMC_PARSER_CMD_H__
#define __UMC_PARSER_CMD_H__

#include "umc_list.h"
#include "umc_string.h"
#include "umc_parser_cfg.h"


namespace UMC
{

typedef enum
{
    Boolean,
    Real,
    Integer,
    String,
    Config
} KeyType;

class ParserCmd
{
public:
    ParserCmd();
    ~ParserCmd();

    Ipp32s Init(vm_char *argv[], int argc, ParserCfg *pCfg = NULL);
    Ipp32s InitRes(vm_char *argv[], int argc, ParserCfg *pCfg = NULL);
    void   Reset();
    void AddKey(const vm_char *cName, const vm_char *cLongName, const vm_char *cDescription, KeyType keyType, Ipp32u iArraySize = 1, bool bAllowEmpty = 0, DString sGroupName = 0);
    void AddKey(const vm_char *cName, const vm_char *cLongName, const vm_char *cCfgName, const vm_char *cDescription, KeyType keyType, Ipp32u iArraySize = 1, bool bAllowEmpty = 0, DString sGroupName = 0);
    Ipp32u GetParamSize(const vm_char *cName, const vm_char *cLongName);
    Ipp32u GetParam(const vm_char *cName, const vm_char *cLongName, bool    *bValue, Ipp32u iArraySize);
    Ipp32u GetParam(const vm_char *cName, const vm_char *cLongName, Ipp8s   *iValue, Ipp32u iArraySize);
    Ipp32u GetParam(const vm_char *cName, const vm_char *cLongName, Ipp8u   *iValue, Ipp32u iArraySize);
    Ipp32u GetParam(const vm_char *cName, const vm_char *cLongName, Ipp16s  *iValue, Ipp32u iArraySize);
    Ipp32u GetParam(const vm_char *cName, const vm_char *cLongName, Ipp16u  *iValue, Ipp32u iArraySize);
    Ipp32u GetParam(const vm_char *cName, const vm_char *cLongName, Ipp32s  *iValue, Ipp32u iArraySize);
    Ipp32u GetParam(const vm_char *cName, const vm_char *cLongName, Ipp32u  *iValue, Ipp32u iArraySize);
    Ipp32u GetParam(const vm_char *cName, const vm_char *cLongName, Ipp32f  *fValue, Ipp32u iArraySize);
    Ipp32u GetParam(const vm_char *cName, const vm_char *cLongName, Ipp64f  *fValue, Ipp32u iArraySize);
    Ipp32u GetParam(const vm_char *cName, const vm_char *cLongName, DString *sValue, Ipp32u iArraySize);
    void PrintHelp(); // print keys and description
    void PrintData(); // print all parsed data

public:
    DString m_sProgName;
    DString m_sProgNameShort;
    bool      m_bHelpPrinted;

protected:
    class Key
    {
    public:
        Key() {};
        Key(const vm_char *cName, const vm_char *cLongName, const vm_char *cCfgName, const vm_char *cDescription, KeyType keyType, Ipp32u iArraySize, bool bAllowEmpty, DString sGroupName)
        {
            m_bTaken       = m_bAllocated = false;
            m_bAllowEmpty  = bAllowEmpty;
            m_keyType      = keyType;
            m_sName        = cName;
            m_sLongName    = cLongName;
            m_sCfgName     = cCfgName;
            m_sDescription = cDescription;
            m_iArraySize   = iArraySize;
            m_sGroupName   = sGroupName;
        };

        void Alloc()
        {
            switch(m_keyType)
            {
            case Boolean:
                m_bool   = new bool[m_iArraySize];
                break;
            case Integer:
                m_uint   = new Ipp32u[m_iArraySize];
                break;
            case Real:
                m_double = new Ipp64f[m_iArraySize];
                break;
            case Config:
            case String:
                m_string = new DString[m_iArraySize];
                break;
            }
            m_bAllocated = true;
        };

        void Free() 
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
                case Config:
                case String:
                    delete[] m_string;
                    break;
                }
                m_bAllocated = false;
            }
        };

        DString m_sName;
        DString m_sLongName;
        DString m_sCfgName;
        DString m_sDescription;
        KeyType m_keyType;
        Ipp32u  m_iArraySize;
        DString m_sGroupName;
        bool    m_bAllowEmpty;
        bool    m_bAllocated;
        bool    m_bTaken;

        union
        {
            bool    *m_bool;
            Ipp32u  *m_uint;
            Ipp64f  *m_double;
            DString *m_string;
        };
    };

    List<Key> m_keys;
    vm_char *m_pBuffer;
    Ipp32u   m_iBufferLen;
    Ipp32u   m_iKeysCount;
    Ipp32u   m_iEmptyKeysCount;
    bool     m_bErrorResilience;

    bool NextSymbol();
    bool SkipSpaces();
    bool IsKeyExist(const vm_char *cName, const vm_char *cLongName);
    void EstimateSize(Key *pKey);
    bool ParseInt(Key *key);
    bool ParseFloat(Key *key);
    bool ParseString(Key *key);
    Key* FindNextKey(const vm_char *cName, bool bLong, bool bEmpty);
    template<class TYPE>
    Ipp32u GetParamInt(const vm_char *cName, const vm_char *cLongName, TYPE  *value, Ipp32u iArraySize);
    template<class TYPE>
    Ipp32u GetParamFloat(const vm_char *cName, const vm_char *cLongName, TYPE  *value, Ipp32u iArraySize);
};

}
#endif
