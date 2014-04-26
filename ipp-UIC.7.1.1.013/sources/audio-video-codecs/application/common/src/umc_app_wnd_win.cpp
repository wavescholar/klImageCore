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
#ifdef UMC_ENABLE_SYS_WIN

// media keys override for VS2005
#if _MSC_VER <= 1400 && !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0500
#endif
#include <windows.h>

#include "vm_types.h"

#include "umc_app_wnd.h"

#include "ippdefs.h"

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SYSCOMMAND: // catch close window button to ensure what render thread will be closed before the window surface.
        if(wParam == SC_CLOSE)
        {
            PostMessage(NULL, msg, wParam, lParam);
            break;
        }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

InterfaceWIN::InterfaceWIN()
{
    m_hWnd      = NULL;
    m_pMenu     = NULL;
    m_wndRect.x = m_wndRect.y = CW_USEDEFAULT;
}

InterfaceWIN::~InterfaceWIN()
{
    WindowClose();
}

bool InterfaceWIN::WindowCreate(vm_char* cAppName, Ipp32u iStyle)
{
    vm_char   *cClassName = VM_STRING("IPPUMCSimplePlayerWinClass");
    WNDCLASSEX wndClass   = {0};
    LONG       lWnd;

    m_iStyle = iStyle;

    wndClass.cbSize         = sizeof(WNDCLASSEX);
    wndClass.style          = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc    = WindowProc;
    wndClass.cbClsExtra     = 0;
    wndClass.cbWndExtra     = 0;
    wndClass.hInstance      = NULL;
    wndClass.hIcon          = NULL;
    wndClass.hCursor        = LoadCursor(0, IDC_ARROW);
    wndClass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName   = NULL;
    wndClass.lpszClassName  = cClassName;
    wndClass.hIconSm        = NULL;

    if(!RegisterClassEx(&wndClass))
        return false;

    switch(m_iStyle)
    {
    case WSTYLE_BORDERLESS:
        lWnd = WS_POPUP;
        break;
    case WSTYLE_FIXED_SIZE:
        lWnd = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        break;
    case WSTYLE_NORMAL:
        lWnd = WS_OVERLAPPEDWINDOW;
        break;
    default:
        return false;
    }

    m_hWnd = CreateWindowEx(0, cClassName, cAppName, lWnd, m_wndRect.x, m_wndRect.y, m_wndRect.width, m_wndRect.height, NULL, NULL, NULL, NULL);
    if(m_hWnd == NULL)
        return false;

    m_bWndCreated = true;
    return true;
}

void InterfaceWIN::WindowClose()
{
    if(!m_bWndCreated)
        return;

    DestroyWindow(m_hWnd);
    m_bWndCreated = false;
    m_hWnd        = NULL;
}

void InterfaceWIN::WindowSetRect(IppiRect wndRect)
{
    if(!m_bWndCreated)
        return;

    m_wndRect = wndRect;
    MoveWindow(m_hWnd, m_wndRect.x, m_wndRect.y, m_wndRect.width, m_wndRect.height, 0);
}

void InterfaceWIN::WindowResize(IppiSize newSize)
{
    if(!m_bWndCreated)
        return;

    ::RECT workArea;
    WINDOWINFO info;
    IppiSize dectSize;
    info.cbSize = sizeof(WINDOWINFO);
    GetWindowInfo(m_hWnd, &info);

    // Get work area (without taskbar and other toolbars)
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    dectSize.width  = workArea.right - workArea.left;
    dectSize.height = workArea.bottom - workArea.top;

    // compensate for window border
    if(info.cxWindowBorders && info.cyWindowBorders)
    {
        newSize.width  = newSize.width + (info.rcClient.left - info.rcWindow.left) + (info.rcWindow.right - info.rcClient.right);
        newSize.height = newSize.height + (info.rcClient.top - info.rcWindow.top) + (info.rcWindow.bottom - info.rcClient.bottom);
    }

    // shift window if too large
    if(newSize.width > dectSize.width)
    {
        info.rcWindow.left = workArea.left;
        newSize.width = dectSize.width;
    }
    else if((info.rcWindow.left + newSize.width) > dectSize.width)
        info.rcWindow.left -= ((newSize.width + info.rcWindow.left) - dectSize.width);
    else if(info.rcWindow.left < workArea.left)
        info.rcWindow.left = workArea.left;

    if(newSize.height > dectSize.height)
    {
        info.rcWindow.top = workArea.top;
        newSize.height = dectSize.height;
    }
    else if((info.rcWindow.top + newSize.height) > dectSize.height)
        info.rcWindow.top -= ((newSize.height + info.rcWindow.top) - dectSize.height);
    else if(info.rcWindow.top < workArea.top)
        info.rcWindow.top = workArea.top;

    m_wndRect.x      = info.rcWindow.left;
    m_wndRect.y      = info.rcWindow.top;
    m_wndRect.width  = newSize.width;
    m_wndRect.height = newSize.height;
    MoveWindow(m_hWnd, m_wndRect.x, m_wndRect.y, m_wndRect.width, m_wndRect.height, 0);
}

void InterfaceWIN::WindowSetFullscreen(bool bFullscreen)
{
    if(!m_bWndCreated || m_iStyle == WSTYLE_FIXED_SIZE)
        return;

    if(bFullscreen == m_bFullscreen)
        return;

    if(!bFullscreen)
    {
        ShowWindow(m_hWnd, SW_RESTORE);
        SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        SetWindowPos(m_hWnd, HWND_NOTOPMOST, m_wndRect.x, m_wndRect.y, m_wndRect.width, m_wndRect.height, SWP_FRAMECHANGED);
        m_bFullscreen = false;
    }
    else
    {
        WINDOWINFO info;
        info.cbSize = sizeof(WINDOWINFO);
        GetWindowInfo(m_hWnd, &info);
        m_wndRect.x = info.rcWindow.left;
        m_wndRect.y = info.rcWindow.top;
        m_wndRect.width  = info.rcWindow.right - info.rcWindow.left;
        m_wndRect.height = info.rcWindow.bottom - info.rcWindow.top;

        SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos(m_hWnd, HWND_TOPMOST, m_wndRect.x, m_wndRect.y, m_wndRect.width, m_wndRect.height, SWP_FRAMECHANGED);
        ShowWindow(m_hWnd, SW_MAXIMIZE);
        m_bFullscreen = true;
    }
}

void InterfaceWIN::WindowShow()
{
    if(!m_bWndCreated)
        return;

    ShowWindow(m_hWnd, SW_SHOW);
}

void InterfaceWIN::WindowHide()
{
    if(!m_bWndCreated)
        return;

    ShowWindow(m_hWnd, SW_HIDE);
}

void InterfaceWIN::GetWindowContext(SysRenderContext *pContext)
{
    if(!m_bWndCreated)
        return;

    if(!pContext)
        return;

    pContext->m_hWnd = m_hWnd;
}

void InterfaceWIN::ProcMessages()
{
    if(!m_bWndCreated)
        return;

    if(PeekMessage(&m_message, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&m_message);
        DispatchMessage(&m_message);

        switch(m_message.message)
        {
        case WM_SYSCOMMAND:
            if(m_message.wParam == SC_CLOSE)
                m_pPipeline->Stop();
            return;

        case WM_QUIT:
            m_pPipeline->Stop();
            return;

        case WM_KEYDOWN:
            switch(m_message.wParam)
            {
            case VK_ESCAPE:
                m_pPipeline->Stop();
                return;

            case VK_MEDIA_PLAY_PAUSE:
            case VK_SPACE:
                if(!m_pPipeline->IsPaused())
                    m_pPipeline->Pause();
                else
                    m_pPipeline->Resume();
                return;

            case VK_MEDIA_STOP:
            case VK_S:
                m_pPipeline->Stop();
                return;

            case VK_VOLUME_MUTE:
            case VK_M:
                m_pPipeline->Mute();
                return;

            case VK_F:
                WindowSwitchFullscreen();
                return;
/*
            case VK_RIGHT:
                m_pPipeline->Step();
                return;

            case VK_LEFT:
                m_pPipeline->Step();
                return;*/
            }
            return;

        case WM_LBUTTONDBLCLK:
            WindowSwitchFullscreen();
            return;

        case WM_RBUTTONUP:
            //GetWindowInfo(m_hWnd, &winInfo);
            //res = TrackPopupMenuEx(hMenu, 0, winInfo.rcClient.left + (message.lParam & 0xFFFF), winInfo.rcClient.top + (message.lParam >> 16), hWnd, NULL);
            return;
        }
    }
}

#endif
