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

#ifndef __CMDLINE_H__
#define __CMDLINE_H__


#include "list_.h"
#include "fixedstring.h"

class CommandLine
{
public:
    CommandLine() {};

    bool AddKey(const char *key, const char *description, bool    &value, int array_size, bool    def_value, int group);
    bool AddKey(const char *key, const char *description, int     &value, int array_size, int     def_value, int group);
    bool AddKey(const char *key, const char *description, double  &value, int array_size, double  def_value, int group);
    bool AddKey(const char *key, const char *description, StringA &value, int array_size, StringA def_value, int group);

    bool Parse(char *argv[], int argc, int inGroup, int outGroup);

    void HelpMessage(const char* format_str[]);

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

        StringA     m_name;
        StringA     m_description;
        bool        m_present;
        int         m_size;
        int         m_length;
        int         m_group;
        KeyType     m_type;

        union
        {
            bool    *m_bool;
            double  *m_double;
            int     *m_int;
            StringA *m_string;
        };

        Key():
          m_present(false), m_length(0), m_type(Integer){};

        Key(const char *key, const char *description, bool    &value, int array_size, bool    def_value, int group):
          m_name(key), m_description(description), m_present(false), m_size(array_size), m_length(0), m_group(group), m_type(Boolean), m_bool  (&value)
          {
            if(array_size)
            {
              for(int i = 0; i < array_size; i++)
                m_bool[i]   = def_value;
            }
            else
              *m_bool = def_value;
          };
        Key(const char *key, const char *description, double  &value, int array_size, double  def_value, int group):
          m_name(key), m_description(description), m_present(false), m_size(array_size), m_length(0), m_group(group), m_type(Real   ), m_double(&value)
          { for(int i = 0; i < array_size; i++) m_double[i] = def_value; };
        Key(const char *key, const char *description, int     &value, int array_size, int     def_value, int group):
          m_name(key), m_description(description), m_present(false), m_size(array_size), m_length(0), m_group(group), m_type(Integer), m_int   (&value)
          { for(int i = 0; i < array_size; i++) m_int[i]    = def_value; };
        Key(const char *key, const char *description, StringA &value, int array_size, StringA def_value, int group):
          m_name(key), m_description(description), m_present(false), m_size(array_size), m_length(0), m_group(group), m_type(String ), m_string(&value)
          { for(int i = 0; i < array_size; i++) m_string[i] = def_value; };
    };

    List<Key> m_keys;

    bool IsKeyExist (const char *key, int group);
    Key* FindNextKey(const char *str, int inGroup, int outGroup);
};

#endif

