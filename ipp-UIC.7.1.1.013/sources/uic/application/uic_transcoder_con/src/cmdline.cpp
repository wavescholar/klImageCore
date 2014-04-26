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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cmdline.h"
#include "genalg.h"
#include "ippimage.h"


inline bool IsKeyPrefix(char a)  { return (a == '/' || a == '-'); }


inline void ConcatenateArgs(char *argv[], int argc, StringA &cmdLine)
{
    for(int i = 1; i < argc; i++)
    {
        cmdLine = cmdLine + argv[i];
        cmdLine = cmdLine + " ";
    }
    cmdLine.CutTail(1);
}

inline void SkipSpaces  (const char* &curr) { while(isspace(*curr)) curr++; }

inline bool IsSpaceOrEnd(int value) { return (isspace(value) || (!value)); }


inline unsigned int PassDigits(const char* &curr, StringA &str)
{
    unsigned int n = 0;
    while(isdigit(*curr)) { str = str + (*curr); curr++; n++; }
    return n;
}

inline bool Parse(const char* &curr, bool &value)
{
    switch(*curr)
    {
    case '+': value = true;  break;
    case '-': value = false; break;
    default:
        return false;
    }

    curr++;

    return true;
}

inline bool Parse(const char* &curr, int &value)
{
    StringA str;

    if(*curr == '+' || *curr == '-')
    {
        str = str + (*curr);
        curr++;
    }

    if(isspace(*curr)) return false;

    if(!PassDigits(curr, str)) return false;

    if(!IsSpaceOrEnd(*curr)) return false;

    value = atoi(str);

    return true;
}

inline bool Parse(const char* &curr, double &value)
{
    StringA str;

    if(*curr == '+' || *curr == '-')
    {
        str = str + (*curr);
        curr++;
    }

    if(isspace(*curr)) return false;

    unsigned int nOfMantDigits = PassDigits(curr, str);

    if(*curr == '.')
    {
        str = str + (*curr);
        curr++;
        if(!(*curr)) goto convert;
    }

    nOfMantDigits += PassDigits(curr, str);

    if(!nOfMantDigits) return false;

    if(*curr == 'e' || *curr == 'E')
    {
        str = str + (*curr);
        curr++;
        if(*curr == '+' || *curr == '-')
        {
            str = str + (*curr);
            curr++;
        }
        if(!PassDigits(curr, str)) return false;
    }

    if(!IsSpaceOrEnd(*curr)) return false;

convert:
    value = atof(str);

    return true;
}

inline void Parse(const char* &curr, StringA &str)
{
    str.Clear();

    while (*curr)
    {
        if(isspace(*curr) && IsKeyPrefix(curr[1])) break;
        str = str + (*curr);
        curr++;
    }
}

bool CommandLine::IsKeyExist(const char *str, int group)
{
    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        if(group == key->m_group && strlen(str) == key->m_name.Size() && !strncmp(key->m_name, str, key->m_name.Size())) return true;
    }

    return false;
}

bool CommandLine::AddKey(const char *key, const char *description, bool   &value, int array_size, bool def_value, int group)
{
    if(IsKeyExist(key, group)) return false;

    Key a(key, description, value, array_size, def_value, group);

    m_keys.PushBack();
    m_keys.Back() = a;

    return true;
}

bool CommandLine::AddKey(const char *key, const char *description, int    &value, int array_size, int def_value, int group)
{
    if(IsKeyExist(key, group)) return false;

    Key a(key, description, value, array_size, def_value, group);

    m_keys.PushBack();
    m_keys.Back() = a;

    return true;
}

bool CommandLine::AddKey(const char *key, const char *description, double &value, int array_size, double def_value, int group)
{
    if(IsKeyExist(key, group)) return false;

    Key a(key, description, value, array_size, def_value, group);

    m_keys.PushBack();
    m_keys.Back() = a;

    return true;
}

bool CommandLine::AddKey(const char *key, const char *description, StringA &value, int array_size, StringA def_value, int group)
{
    if(IsKeyExist(key, group)) return false;

    Key a(key, description, value, array_size, def_value, group);

    m_keys.PushBack();
    m_keys.Back() = a;

    return true;
}


CommandLine::Key* CommandLine::FindNextKey(const char *str, int inGroup, int outGroup)
{
    Key* retKey = 0;
    int size = 0;

    for(List<Key>::Iterator key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        if((inGroup == IT_UNKNOWN && outGroup == IT_UNKNOWN)
          || key->m_group == IT_UNKNOWN || key->m_group == outGroup || key->m_group == inGroup)
        {
            if(!strncmp(key->m_name, str, key->m_name.Size()))
            {
                if(size <= (int)(key->m_name.Size()))
                {
                  size = key->m_name.Size();
                  retKey = key;
                }
            }
        }
    }

    return retKey;
}

bool CommandLine::Parse(char *argv[], int argc, int inGroup, int outGroup)
{
    if(argc == 1) return true;

    StringA cmdLine;

    ConcatenateArgs(argv, argc, cmdLine);

    StringA str;

    const char *curr = cmdLine;
    const char *pos;

    while(IsKeyPrefix(*curr))
    {
        curr++;

        Key* key = FindNextKey(curr, inGroup, outGroup);

        if(key)
        {
            if(!key->m_present)
            {
                if(key->m_group == outGroup || key->m_group == inGroup)
                {
                    key->m_present = true;
                    pos = curr;
                    curr += key->m_name.Size();

                    SkipSpaces(curr);

                    if(*curr == '=') curr++;
                    bool result = true;

                    if(key->m_size)
                    {
                      for(int i = 0; i < key->m_size; i++)
                      {
                        SkipSpaces(curr);

                        switch(key->m_type)
                        {
                        case Key::Boolean: result = ::Parse(curr, key->m_bool[i]  ); break;
                        case Key::Integer: result = ::Parse(curr, key->m_int[i]   ); break;
                        case Key::Real:    result = ::Parse(curr, key->m_double[i]); break;
                        case Key::String:           ::Parse(curr, key->m_string[i]); break;
                        }

                        if(!(*curr) || IsKeyPrefix(*curr))
                          break;
                      }
                    }
                    else
                    {
                      SkipSpaces(curr);

                      switch(key->m_type)
                      {
                      case Key::Boolean: *key->m_bool = true; break;
                      case Key::Integer: return false;        break;
                      case Key::Real:    return false;        break;
                      case Key::String:  return false;        break;
                      }
                    }

                    SkipSpaces(curr);
                    key->m_length = curr - pos;

                    if(!result) return false;
                }
            }
            else
                curr += key->m_length;
        }
        else
            return false;

        while(*curr && !IsKeyPrefix(*curr))
            curr++;
    }

    if(*curr) return false;
    return true;
}

void CommandLine::HelpMessage(const char* format_str[])
{
    printf("\nSupported input formats:\n");
    printf("  .bmp; .dcm; .jp2; .jpg; .jxr .png;\n");

    printf("\nSupported output formats:\n");
    printf("  .bmp; .jp2; .jpg; .jxr .png;\n");

    printf("\nCommand line options:\n");

    unsigned int maxSize = 0;
    int           first_loop;

    List<Key>::Iterator key;

    for(key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
    {
        unsigned int size = key->m_name.Size();
        if(size > maxSize) maxSize = size;
    }

    for(int i = 0; i < IT_END; i++)
    {
        first_loop = 1;
        for(key = m_keys.ItrFront(); key != m_keys.ItrBackBound(); ++key)
        {
            if(key->m_group == i)
            {
                if(first_loop && i != IT_UNKNOWN)
                {
                    printf("\n  ");
                    printf(format_str[i]);
                    printf(" options:\n");
                    first_loop = 0;
                }

                printf("  -%s  ", (const char*)key->m_name);
                for(unsigned int i = 0; i < maxSize - key->m_name.Size(); i++)
                    printf(" ");
                printf("%s\n", (const char*)key->m_description);
            }
        }
    }
    printf("\n");
}
