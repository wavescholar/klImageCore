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

#include "umc_config.h"
#ifdef UMC_ENABLE_SYS_GLX

#include "umc_defs.h"
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

#include "umc_video_render.h"

#include "umc_app_wnd.h"

#ifndef XK_0
#define XK_0 0x0030
#define XK_1 0x0031
#define XK_2 0x0032
#define XK_3 0x0033
#define XK_4 0x0034
#define XK_5 0x0035
#define XK_6 0x0036
#define XK_7 0x0037
#define XK_8 0x0038
#define XK_9 0x0039
#endif

#ifndef XK_a
#define XK_a 0x0061
#define XK_b 0x0062
#define XK_c 0x0063
#define XK_d 0x0064
#define XK_e 0x0065
#define XK_f 0x0066
#define XK_g 0x0067
#define XK_h 0x0068
#define XK_i 0x0069
#define XK_j 0x006a
#define XK_k 0x006b
#define XK_l 0x006c
#define XK_m 0x006d
#define XK_n 0x006e
#define XK_o 0x006f
#define XK_p 0x0070
#define XK_q 0x0071
#define XK_r 0x0072
#define XK_s 0x0073
#define XK_t 0x0074
#define XK_u 0x0075
#define XK_v 0x0076
#define XK_w 0x0077
#define XK_x 0x0078
#define XK_y 0x0079
#define XK_z 0x007a
#endif

struct MotifHints
{
    unsigned long   flags;
    unsigned long   functions;
    unsigned long   decorations;
    long            inputMode;
    unsigned long   status;
};

InterfaceX::InterfaceX()
{
    m_display        = NULL;
    m_window         = 0;
    m_pvisinfo       = NULL;
    m_iScreen        = 0;
    m_wndRect.width  = m_wndRect.height = 100;
    m_iPrevPressTime = 0;

    XInitThreads();
}

InterfaceX::~InterfaceX()
{
    WindowClose();
}

bool InterfaceX::WindowCreate(vm_char* cAppName, Ipp32u iStyle)
{
    XSetWindowAttributes attributes;
    int attr[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 0, None };

    m_display = XOpenDisplay(getenv("DISPLAY"));
    if(NULL == m_display)
        return false;

    m_iScreen = DefaultScreen(m_display);

    if (NULL == (m_pvisinfo = glXChooseVisual( m_display, m_iScreen, attr )))
        return false;

    // initial color map for TrueColor and Empty table for Direct Color
    attributes.colormap     = XCreateColormap( m_display, RootWindow(m_display, m_iScreen), m_pvisinfo->visual, AllocNone);
    attributes.event_mask   = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | KeymapStateMask;
    unsigned long valuemask = CWColormap | CWEventMask;

    if (0 == (m_window = XCreateWindow(m_display, RootWindow(m_display, m_iScreen), m_wndRect.x, m_wndRect.y, m_wndRect.width, m_wndRect.height, 2,
        m_pvisinfo->depth, InputOutput, m_pvisinfo->visual, valuemask, &attributes)))
        return false;

    XStoreName(m_display, m_window, cAppName);

    // init interface close message
    m_wmDelete = XInternAtom(m_display, "WM_DELETE_WINDOW", true);
    XSetWMProtocols(m_display, m_window, &m_wmDelete, 1);

    XFlush(m_display);

    m_bWndCreated = true;
    return true;
}

void InterfaceX::WindowClose()
{
    if(!m_bWndCreated)
        return;

    if (NULL != m_pvisinfo)
    {
        XFree(m_pvisinfo);
        m_pvisinfo = NULL;
    }
    XDestroyWindow(m_display, m_window);
    XCloseDisplay(m_display);

    m_bWndCreated = false;
}

void InterfaceX::WindowSetRect(IppiRect wndRect)
{
    if(!m_bWndCreated)
        return;

    m_wndRect = wndRect;
    XMoveResizeWindow(m_display, m_window, m_wndRect.x, m_wndRect.y, m_wndRect.width, m_wndRect.height);
}

void InterfaceX::WindowResize(IppiSize newSize)
{
    if(!m_bWndCreated)
        return;

    m_wndRect.width  = newSize.width;
    m_wndRect.height = newSize.height;
    XMoveResizeWindow(m_display, m_window, m_wndRect.x, m_wndRect.y, m_wndRect.width, m_wndRect.height);
    XFlush(m_display);
}

void InterfaceX::WindowSetFullscreen(bool bFullscreen)
{
#ifndef OSX // X11 fullscreen event doesn't work properly on Mac
    XEvent event;

    if(!m_bWndCreated)
        return;

    if(bFullscreen == m_bFullscreen)
        return;

    event.xclient.type         = ClientMessage;
    event.xclient.display      = m_display;
    event.xclient.window       = m_window;
    event.xclient.serial       = 0;
    event.xclient.send_event   = True;
    event.xclient.format       = 32;
    event.xclient.message_type = XInternAtom(m_display, "_NET_WM_STATE", true);

    event.xclient.data.l[1]    = XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", true);
    event.xclient.data.l[2]    = 0;
    event.xclient.data.l[3]    = 0;

    if(bFullscreen)
    {
        event.xclient.data.l[0] = 1;
        m_bFullscreen = true;
    }
    else
    {
        event.xclient.data.l[0] = 0;
        m_bFullscreen = false;
    }

    XSendEvent(m_display, RootWindow(m_display, m_iScreen), False, SubstructureRedirectMask | SubstructureNotifyMask, &event);

    XFlush(m_display);
#endif
}

void InterfaceX::WindowShow()
{
    if(!m_bWndCreated)
        return;

    XMapRaised(m_display, m_window);
    XFlush(m_display);
}

void InterfaceX::WindowHide()
{
    if(!m_bWndCreated)
        return;

    XUnmapWindow(m_display, m_window);
    XFlush(m_display);
}

void InterfaceX::GetWindowContext(SysRenderContext *pContext)
{
    if(!m_bWndCreated)
        return;

    if(!pContext)
        return;

    pContext->m_pDisplay     = m_display;
    pContext->m_window       = m_window;
    pContext->m_iScreen      = m_iScreen;
    pContext->m_pVisualInfo  = m_pvisinfo;
}

void InterfaceX::ProcMessages()
{
    XEvent         event;
    XComposeStatus cmps;
    KeySym         keyval;
    char           cBuffer[4];

    if(!m_bWndCreated)
        return;

    while (XEventsQueued(m_display, QueuedAlready))
    {
        XNextEvent(m_display, &event);

        switch(event.type)
        {
        case ClientMessage:
            if((unsigned long)event.xclient.data.l[0] == m_wmDelete)
                m_pPipeline->Stop();
            break;
        case ButtonPress:
            if(event.xbutton.button == Button1)
            {
                if(event.xbutton.time - m_iPrevPressTime < 200)
                {
                    WindowSwitchFullscreen();
                    m_iPrevPressTime = 0;
                }
                else
                    m_iPrevPressTime = event.xbutton.time;
            }
            break;
        case KeyPress:
            XLookupString(&event.xkey, cBuffer, 4, &keyval, &cmps);
            switch(keyval)
            {
            case XK_Escape: // quit program if escape pressed
                m_pPipeline->Stop();
                break;

            case XK_space:
                if(!m_pPipeline->IsPaused())
                    m_pPipeline->Pause();
                else
                    m_pPipeline->Resume();
                break;

            case XK_m:
                m_pPipeline->Mute();
                break;

            case XK_r:
                m_pPipeline->Start();
                break;

            case XK_s:
                m_pPipeline->Stop();
                break;

            case XK_f:
                WindowSwitchFullscreen();
                break;

            case MappingNotify:
                XRefreshKeyboardMapping(&event.xmapping);
                break;
            }
            break;
        }
    }
}

#endif
