/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2003 - 2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_SYSTEM_CONTEXT_H__
#define __UMC_SYSTEM_CONTEXT_H__

#include "umc_config.h"
#include "umc_dynamic_cast.h"
#include "umc_string.h"

#if defined UMC_ENABLE_SYS_WIN
#include <windows.h>
#elif defined UMC_ENABLE_SYS_GLX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

namespace UMC
{

class SysRenderContext
{
public:
    DYNAMIC_CAST_DECL_BASE(SysRenderContext)

    SysRenderContext()
    {
#if defined UMC_ENABLE_SYS_WIN
        m_hWnd = NULL;
#elif defined UMC_ENABLE_SYS_GLX
        m_pVisualInfo = NULL;
        m_pDisplay    = NULL;
        m_iScreen     = 0;
#endif
    }
    virtual ~SysRenderContext() {}

#if defined UMC_ENABLE_SYS_WIN
    HWND m_hWnd;
#elif defined UMC_ENABLE_SYS_GLX
    XVisualInfo* m_pVisualInfo;
    Display*     m_pDisplay;
    Window       m_window;
    Ipp32u       m_iScreen;
#endif
};

}

#endif

