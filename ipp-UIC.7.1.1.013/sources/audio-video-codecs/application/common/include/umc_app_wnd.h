/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2010-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_APP_WND_H__
#define __UMC_APP_WND_H__

#include "umc_config.h"
#include "umc_system_context.h"
#include "umc_app_pipeline.h"

#include "ippi.h"

using namespace UMC;


enum WindowStyle
{
    WSTYLE_NORMAL,
    WSTYLE_FIXED_SIZE,
    WSTYLE_BORDERLESS
};

// base dummy class in case of unsupported system (will work for file and null renders)
class Interface
{
public:
    DYNAMIC_CAST_DECL_BASE(Interface)

    Interface()
    {
        m_wndRect.x     = m_wndRect.y      = 0;
        m_wndRect.width = m_wndRect.height = 0;
        m_pPipeline     = NULL;
        m_iStyle        = WSTYLE_NORMAL;
        m_bFullscreen   = false;
        m_bWndCreated   = false;
    }

    virtual ~Interface()
    {
        WindowClose();
    }

    virtual bool WindowCreate(vm_char* cAppName, Ipp32u iStyle = WSTYLE_NORMAL) { cAppName = cAppName; iStyle = iStyle; return false; };
    virtual void WindowSetRect(IppiRect wndRect) { wndRect = wndRect; return; };
    virtual void WindowResize(IppiSize newSize) { newSize = newSize; return; };
    virtual void WindowSetFullscreen(bool bFullscreen) { bFullscreen = bFullscreen; return; };
    virtual void WindowShow() { return; };
    virtual void WindowHide() { return; };
    virtual void WindowClose() { return; };
    virtual void GetWindowContext(SysRenderContext *pContext) { pContext = pContext; return; };
    virtual void ProcMessages() { return; };
    virtual void AddContextMenu() { return; };

    bool IsWindowExist() { return m_bWndCreated; };

    void WindowSwitchFullscreen()
    {
        WindowSetFullscreen(!m_bFullscreen);
    }

    void AttachPipeline(BasePipeline *pPipeline)
    {
        m_pPipeline = pPipeline;
    }

protected:
    BasePipeline *m_pPipeline;
    IppiRect      m_wndRect;
    Ipp32u        m_iStyle;
    bool          m_bFullscreen;
    bool          m_bWndCreated;
};

#ifdef UMC_ENABLE_SYS_WIN
class InterfaceWIN : public Interface
{
public:
    DYNAMIC_CAST_DECL(InterfaceWIN, Interface)

    InterfaceWIN();
    virtual ~InterfaceWIN();

    virtual bool WindowCreate(vm_char* cAppName, Ipp32u iStyle = WSTYLE_NORMAL);
    virtual void WindowSetRect(IppiRect wndRect);
    virtual void WindowResize(IppiSize newSize);
    virtual void WindowSetFullscreen(bool bFullscreen);
    virtual void WindowShow();
    virtual void WindowHide();
    virtual void WindowClose();
    virtual void GetWindowContext(SysRenderContext *pContext);
    virtual void ProcMessages();

protected:
    HWND   m_hWnd;
    MSG    m_message;
    HMENU *m_pMenu;
};
#endif

#ifdef UMC_ENABLE_SYS_GLX
class InterfaceX : public Interface
{
public:
    DYNAMIC_CAST_DECL(InterfaceX, Interface)

    InterfaceX();
    virtual ~InterfaceX();

    virtual bool WindowCreate(vm_char* cAppName, Ipp32u iStyle = WSTYLE_NORMAL);
    virtual void WindowSetRect(IppiRect wndRect);
    virtual void WindowResize(IppiSize newSize);
    virtual void WindowSetFullscreen(bool bFullscreen);
    virtual void WindowShow();
    virtual void WindowHide();
    virtual void WindowClose();
    virtual void GetWindowContext(SysRenderContext *pContext);
    virtual void ProcMessages();

protected:
    Display*     m_display;
    Window       m_window;
    XVisualInfo* m_pvisinfo;
    Ipp32u       m_iScreen;
    Atom         m_wmDelete;
    Time         m_iPrevPressTime;
};
#endif

#endif
