/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"

#include "raw.h"
#include "string.h"

using namespace UIC;


IM_ERROR ReadImageRAW(BaseStreamInput& in, PARAMS_RAW& param, CIppImage& image)
{
    BaseStream::TSize cnt;
    int row_size;

    if(param.res_width == 0 || param.res_height == 0)
        return IE_PARAMS;

    if(!strcmp(param.color_format, "gray"))
    {
        image.Color(IC_GRAY);
        if(!strcmp(param.pixel_format, "8u"))
            image.Alloc(param.res_width, param.res_height, 1, 8);
        else if(!strcmp(param.pixel_format, "16u"))
            image.Alloc(param.res_width, param.res_height, 1, 16);
        else if(!strcmp(param.pixel_format, "16s"))
            image.Alloc(param.res_width, param.res_height, 1, 16);
        else if(!strcmp(param.pixel_format, "32s"))
            image.Alloc(param.res_width, param.res_height, 1, 32);
        else if(!strcmp(param.pixel_format, "32f"))
            image.Alloc(param.res_width, param.res_height, 1, 32);
        else
            return IE_PARAMS;
    }
    else if(!strcmp(param.color_format, "graya"))
    {
        image.Color(IC_GRAYA);
        if(!strcmp(param.pixel_format, "8u"))
            image.Alloc(param.res_width, param.res_height, 2, 8);
        else if(!strcmp(param.pixel_format, "16u"))
            image.Alloc(param.res_width, param.res_height, 2, 16);
        else if(!strcmp(param.pixel_format, "16s"))
            image.Alloc(param.res_width, param.res_height, 2, 16);
        else if(!strcmp(param.pixel_format, "32s"))
            image.Alloc(param.res_width, param.res_height, 2, 32);
        else if(!strcmp(param.pixel_format, "32f"))
            image.Alloc(param.res_width, param.res_height, 2, 32);
        else
            return IE_PARAMS;
    }
    else if(!strcmp(param.color_format, "rgb") || !strcmp(param.color_format, "bgr"))
    {
        if(!strcmp(param.color_format, "rgb"))
            image.Color(IC_RGB);
        else
            image.Color(IC_BGR);
        if(!strcmp(param.pixel_format, "8u"))
            image.Alloc(param.res_width, param.res_height, 3, 8);
        else if(!strcmp(param.pixel_format, "16u"))
            image.Alloc(param.res_width, param.res_height, 3, 16);
        else if(!strcmp(param.pixel_format, "16s"))
            image.Alloc(param.res_width, param.res_height, 3, 16);
        else if(!strcmp(param.pixel_format, "32s"))
            image.Alloc(param.res_width, param.res_height, 3, 32);
        else if(!strcmp(param.pixel_format, "32f"))
            image.Alloc(param.res_width, param.res_height, 3, 32);
        else
            return IE_PARAMS;
    }
    else if(!strcmp(param.color_format, "rgba") || !strcmp(param.color_format, "bgra"))
    {
        if(!strcmp(param.color_format, "rgba"))
            image.Color(IC_RGBA);
        else
            image.Color(IC_BGRA);
        if(!strcmp(param.pixel_format, "8u"))
            image.Alloc(param.res_width, param.res_height, 4, 8);
        else if(!strcmp(param.pixel_format, "16u"))
            image.Alloc(param.res_width, param.res_height, 4, 16);
        else if(!strcmp(param.pixel_format, "16s"))
            image.Alloc(param.res_width, param.res_height, 4, 16);
        else if(!strcmp(param.pixel_format, "32s"))
            image.Alloc(param.res_width, param.res_height, 4, 32);
        else if(!strcmp(param.pixel_format, "32f"))
            image.Alloc(param.res_width, param.res_height, 4, 32);
        else
            return IE_PARAMS;
    }
    else if(!strcmp(param.color_format, "cmyk"))
    {
        image.Color(IC_CMYK);
        if(!strcmp(param.pixel_format, "8u"))
            image.Alloc(param.res_width, param.res_height, 4, 8);
        else if(!strcmp(param.pixel_format, "16u"))
            image.Alloc(param.res_width, param.res_height, 4, 16);
        else if(!strcmp(param.pixel_format, "16s"))
            image.Alloc(param.res_width, param.res_height, 4, 16);
        else if(!strcmp(param.pixel_format, "32s"))
            image.Alloc(param.res_width, param.res_height, 4, 32);
        else if(!strcmp(param.pixel_format, "32f"))
            image.Alloc(param.res_width, param.res_height, 4, 32);
        else
            return IE_PARAMS;
    }
    else if(!strcmp(param.color_format, "cmyka"))
    {
        image.Color(IC_CMYKA);
        if(!strcmp(param.pixel_format, "8u"))
            image.Alloc(param.res_width, param.res_height, 5, 8);
        else if(!strcmp(param.pixel_format, "16u"))
            image.Alloc(param.res_width, param.res_height, 5, 16);
        else if(!strcmp(param.pixel_format, "16s"))
            image.Alloc(param.res_width, param.res_height, 5, 16);
        else if(!strcmp(param.pixel_format, "32s"))
            image.Alloc(param.res_width, param.res_height, 5, 32);
        else if(!strcmp(param.pixel_format, "32f"))
            image.Alloc(param.res_width, param.res_height, 5, 32);
        else
            return IE_PARAMS;
    }
    else
        return IE_PARAMS;

    if(!strcmp(param.pixel_format, "8u"))
        image.Format(IF_UNSIGNED);
    else if(!strcmp(param.pixel_format, "16u"))
        image.Format(IF_UNSIGNED);
    else if(!strcmp(param.pixel_format, "16s"))
        image.Format(IF_SIGNED);
    else if(!strcmp(param.pixel_format, "32s"))
        image.Format(IF_SIGNED);
    else if(!strcmp(param.pixel_format, "32f"))
        image.Format(IF_FLOAT);

    if(param.data_offset)
    {
        BaseStream::TOffset offset = param.data_offset;
        in.Seek(offset, BaseStream::Beginning);
    }

    row_size = image.Width()*image.NChannels()*(image.Precision()/8);
    for(int i = 0; i < image.Height(); i++)
    {
        in.Read(image.DataPtr() + image.Step()*i, row_size, cnt);
        if(cnt != (BaseStream::TSize)row_size)
            return IE_RDATA;
    }

    return IE_OK;
}


IM_ERROR SaveImageRAW(CIppImage& image, PARAMS_RAW& param, BaseStreamOutput& out)
{
    BaseStream::TSize cnt;
    param = param;
    int row_size = image.Width()*image.NChannels()*(image.Precision()/8);

    for(int i = 0; i < image.Height(); i++)
    {
        out.Write(image.DataPtr() + i*image.Step(), row_size, cnt);
        if(cnt != (BaseStream::TSize)row_size)
            return IE_WDATA;
    }

    return IE_OK;
}
