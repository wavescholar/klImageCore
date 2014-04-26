/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __BASESTREAMIN_H__
#define __BASESTREAMIN_H__

#include "jpegbase.h"
#include "basestream.h"


class BaseStreamInputFwd : public BaseStream
{
public:
    BaseStreamInputFwd() {}
    virtual ~BaseStreamInputFwd() {}

    virtual TStatus Read(void* buf, TSize size, TSize &cnt) = 0;
    virtual TStatus SeekFwd (TSize offset) = 0;
};


class CBaseStreamInput : public BaseStreamInputFwd
{
public:
    CBaseStreamInput()          {}
    virtual ~CBaseStreamInput() {}

    virtual TStatus Seek(TOffset offset, SeekOrigin origin) = 0;
    virtual TStatus SeekFwd(TSize   offset)  { return Seek((TOffset)offset, Current); }
};

#endif // __BASESTREAMIN_H__

