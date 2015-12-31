/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/GDI+/GDIWindow.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant abstraction for GDI+ windowing system.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Internal dependency : GDI+, Win32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GDIWindow.h"

/////////////////////////////////////////////////////////////////////////////////
// GDIWindowCallbacks implementation
GDIWindowCallbacks::GDIWindowCallbacks( Void * pUserData )
{
    m_pUserData = pUserData;
}
GDIWindowCallbacks::~GDIWindowCallbacks()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GDIWindow implementation
GDIWindow::GDIWindow( const WChar * strTitle, UInt iWidth, UInt iHeight, GDIWindowCallbacks * pCallbacks )
{
    m_pCallbacks = pCallbacks;

    m_hInstance = (HINSTANCE)GetModuleHandle(NULL);

    m_hWindow = NULL;

    m_hWindowMonitor = NULL;
    m_hAdapterMonitor = NULL;

    m_hWindowCursor = NULL;
    m_hWindowMenu = NULL;
    
    m_bClipToSingleAdapter = true;

    m_bActive = true;
    m_bFullScreen = false;
    m_bMinimized = false;
    m_bMaximized = false;
    m_bMinimizedFullscreen = false;
    m_bIgnoreResize = false;
    m_bIsInSizeMove = false;

    m_iWindowedWidth = 0;
    m_iWindowedHeight = 0;
    MemZero( &m_wpWindowed, sizeof(WINDOWPLACEMENT) );
    m_iWindowedStyle = 0;
    m_bWindowedTopMost = false;

    m_iFullScreenWidth = 0;
    m_iFullScreenHeight = 0;

    m_iBackupStyle = 0;
    m_iBackupShowCmd = 0;

    _GDI_CreateWindow( strTitle, iWidth, iHeight );
}
GDIWindow::~GDIWindow()
{
    // nothing to do
}

UInt GDIWindow::GetWidth() const
{
    RECT rectClient;
    GetClientRect( m_hWindow, &rectClient );
    return ( rectClient.right - rectClient.left );
}
UInt GDIWindow::GetHeight() const
{
    RECT rectClient;
    GetClientRect( m_hWindow, &rectClient );
    return ( rectClient.bottom - rectClient.top );
}

Void GDIWindow::Show( Bool bShow )
{
    ShowWindow( m_hWindow, (bShow) ? SW_SHOW : SW_HIDE );
}
Void GDIWindow::Paint()
{
    UpdateWindow( m_hWindow );
}

Void GDIWindow::ShowCursor( Bool bShow ) const
{
    SetCursor( (bShow) ? m_hWindowCursor : NULL );
    ::ShowCursor( (bShow) ? TRUE : FALSE );
}
Void GDIWindow::ClipCursor( Bool bClip ) const
{
    if ( bClip ) {
        RECT rectWindow;
        GetWindowRect( m_hWindow, &rectWindow );
        ::ClipCursor( &rectWindow );
    } else
        ::ClipCursor( NULL );
}

Void GDIWindow::MessageLoop()
{
    HACCEL hAccel = NULL;

    MSG msg;
    while( true ) {
        if ( PeekMessage(&msg, NULL, 0u, 0u, PM_REMOVE) != FALSE ) {
            if ( msg.message == WM_QUIT )
                break;
            if ( hAccel == NULL || TranslateAccelerator(m_hWindow, hAccel, &msg) == 0 ) {
                TranslateMessage( &msg );
		        DispatchMessage( &msg );
            }
        } else
            m_pCallbacks->OnIdleTime();
    }

    if( hAccel != NULL )
        DestroyAcceleratorTable( hAccel );
}
Void GDIWindow::Exit()
{
    PostMessage( m_hWindow, WM_CLOSE, 0, 0 );
}

/////////////////////////////////////////////////////////////////////////////////

Void GDIWindow::_SwitchDisplay_Prepare( Bool bNewIsWindowed, Bool bHasOldDisplay,
                                        Bool bOldIsWindowed, UInt iOldWidth, UInt iOldHeight )
{
    // Introduction
    m_pCallbacks->OnPause();
    m_bIgnoreResize = true;

    // Prepare
    if ( bNewIsWindowed ) {
        if ( bHasOldDisplay && !bOldIsWindowed ) {
            // Fullscreen => Windowed, save fullscreen state
            m_iFullScreenWidth = iOldWidth;
            m_iFullScreenHeight = iOldHeight;

            // Set windowed style
            m_iBackupStyle = GetWindowLong( m_hWindow, GWL_STYLE );
            SetWindowLong( m_hWindow, GWL_STYLE, m_iWindowedStyle );

            // Restore menus (if any)
            SetMenu( m_hWindow, m_hWindowMenu );
        }
        // Don't hide window before windowed
    } else {
        if ( !bHasOldDisplay || (bHasOldDisplay && bOldIsWindowed) ) {
            // Null/Windowed => Fullscreen, Save windowed state
            MemZero( &m_wpWindowed, sizeof(WINDOWPLACEMENT) );
            m_wpWindowed.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement( m_hWindow, &m_wpWindowed );
            LONG iStyle = GetWindowLong( m_hWindow, GWL_STYLE );
            m_iWindowedStyle = ( iStyle & (~WS_MINIMIZE) & (~WS_MAXIMIZE) );
            LONG iExStyle = GetWindowLong( m_hWindow, GWL_EXSTYLE );
            m_bWindowedTopMost = ( (iExStyle & WS_EX_TOPMOST) != 0 );
            if ( bHasOldDisplay ) {
                m_iWindowedWidth = iOldWidth;
                m_iWindowedHeight = iOldHeight;
            }

            // Set fullscreen style
            m_iBackupStyle = iStyle;
            SetWindowLong( m_hWindow, GWL_STYLE, WS_POPUP | WS_SYSMENU );

            // Remove menus (if any)
            SetMenu( m_hWindow, NULL );
        }

        // Any => Fullscreen, Hide window
        WINDOWPLACEMENT wpFullScreen;
        MemZero( &wpFullScreen, sizeof(WINDOWPLACEMENT) );
        wpFullScreen.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement( m_hWindow, &wpFullScreen );

        // Set ShowCmd
        m_iBackupShowCmd = wpFullScreen.showCmd;
        ShowWindow( m_hWindow, SW_HIDE );

        // Dirty patch : remove WPF_RESTORETOMAXIMIZED placement flag
        if ( (wpFullScreen.flags & WPF_RESTORETOMAXIMIZED) != 0 ) {
            // The window was maximized then minimized. This causes the WPF_RESTORETOMAXIMIZED
            // flag to be set which will cause SW_RESTORE to restore the window from minimized
            // to maximized which isn't what we want ...
            wpFullScreen.flags &= ~WPF_RESTORETOMAXIMIZED;
            wpFullScreen.showCmd = SW_RESTORE;
            SetWindowPlacement( m_hWindow, &wpFullScreen );
        }
    }
}
Void GDIWindow::_SwitchDisplay_Abort( Bool bNewIsWindowed, Bool bHasOldDisplay, Bool bOldIsWindowed )
{
    // Abort
    if ( bNewIsWindowed ) {
        if ( bHasOldDisplay && !bOldIsWindowed ) {
            // Restore fullscreen style
            SetWindowLong( m_hWindow, GWL_STYLE, m_iBackupStyle );
            // Remove back menus
            SetMenu( m_hWindow, NULL );
        }
        // No need to show back
    } else {
        if ( !bHasOldDisplay || (bHasOldDisplay && bOldIsWindowed) ) {
            // Restore windowed style
            SetWindowLong( m_hWindow, GWL_STYLE, m_iBackupStyle );
            // Restore back menus (if any)
            SetMenu( m_hWindow, m_hWindowMenu );
        }
        // Restore ShowCmd
        ShowWindow( m_hWindow, m_iBackupShowCmd );
        // Dirty patch : let the WPF_RESTORETOMAXIMIZED placement flag be gone
    }

    // Conclusion
    m_bIgnoreResize = false;
    m_pCallbacks->OnResume();
}
Bool GDIWindow::_SwitchDisplay_Finalize( Bool bNewIsWindowed, Bool bHasOldDisplay, Bool bOldIsWindowed,
                                         Bool bKeepWindowedSize, UInt iNewWidth, UInt iNewHeight,
                                         HMONITOR hNewAdapterMonitor )
{
    // Update adapter monitor (given by D3D)
    m_hAdapterMonitor = hNewAdapterMonitor;

    // Check for Fullscreen => Windowed case
    if ( bHasOldDisplay && bNewIsWindowed && !bOldIsWindowed ) {
        // Set windowed placement & topmost flag
        SetWindowPlacement( m_hWindow, &m_wpWindowed );
        SetWindowPos( m_hWindow, m_bWindowedTopMost ? HWND_TOPMOST : HWND_NOTOPMOST,
                      0, 0, 0, 0, SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE );
    }

    // Check if we need to resize
    Bool bNeedResize = false;
    if ( bNewIsWindowed && !bKeepWindowedSize ) {
        // Get client rect
        UInt iClientWidth, iClientHeight;
        if ( IsIconic(m_hWindow) ) {
            // Window is minimized, check placement on restore
            WINDOWPLACEMENT wpRestored;
            MemZero( &wpRestored, sizeof(WINDOWPLACEMENT) );
            wpRestored.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement( m_hWindow, &wpRestored );
            if ( (wpRestored.flags & WPF_RESTORETOMAXIMIZED) != 0 && wpRestored.showCmd == SW_SHOWMINIMIZED ) {
                // Temporarily max the window then get client rect
                ShowWindow( m_hWindow, SW_RESTORE );
                RECT rectClient;
                GetClientRect( m_hWindow, &rectClient );
                iClientWidth = ( rectClient.right - rectClient.left );
                iClientHeight = ( rectClient.bottom - rectClient.top );
                ShowWindow( m_hWindow, SW_MINIMIZE );
            } else {
                // Use normal position and substract window frame
                RECT rectFrame = { 0, 0, 0, 0 };
                AdjustWindowRect( &rectFrame, m_iWindowedStyle, FALSE );
                LONG iFrameWidth = ( rectFrame.right - rectFrame.left );
                LONG iFrameHeight = ( rectFrame.bottom - rectFrame.top );
                iClientWidth = (UInt)( wpRestored.rcNormalPosition.right - wpRestored.rcNormalPosition.left - iFrameWidth );
                iClientHeight = (UInt)( wpRestored.rcNormalPosition.bottom - wpRestored.rcNormalPosition.top - iFrameHeight );
            }
        } else {
            // Just get client rect
            RECT rectClient;
            GetClientRect( m_hWindow, &rectClient );
            iClientWidth = ( rectClient.right - rectClient.left );
            iClientHeight = ( rectClient.bottom - rectClient.top );
        }

        // Check if we match
        if ( iClientWidth != iNewWidth || iClientHeight != iNewHeight )
            bNeedResize = true;

        // Check for multiple monitors clipping
        if ( m_bClipToSingleAdapter && !IsIconic(m_hWindow) ) {
            MONITORINFO adaptMonInfos;
            adaptMonInfos.cbSize = sizeof(MONITORINFO);
            GetMonitorInfo( m_hAdapterMonitor, &adaptMonInfos );
            RECT rectWindow;
            GetWindowRect( m_hWindow, &rectWindow );
            // Check if the window rect goes outside the adapter's virtual screen
            if ( rectWindow.left < adaptMonInfos.rcWork.left || rectWindow.right > adaptMonInfos.rcWork.right ||
                 rectWindow.top < adaptMonInfos.rcWork.top || rectWindow.bottom > adaptMonInfos.rcWork.bottom ) {
                // If the window is maximized and on the same monitor as the adapter,
                // the window is already clipped, even when we find an unbounded rect
                if ( m_hAdapterMonitor != m_hWindowMonitor || !IsZoomed(m_hWindow) )
                    bNeedResize = true;
            }
        }
    }

    // Perform the resize if needed
    Bool bRetry = false;
    if ( bNeedResize ) {
        // Restore window, IsIconic first to handle WPF_RESTORETOMAXIMIZED
        if ( IsIconic(m_hWindow) )
            ShowWindow( m_hWindow, SW_RESTORE );
        if ( IsZoomed(m_hWindow) )
            ShowWindow( m_hWindow, SW_RESTORE );

        // Resize
        if ( m_bClipToSingleAdapter ) {
            // Multiple monitors clipping version
            MONITORINFO adaptMonInfos;
            adaptMonInfos.cbSize = sizeof(MONITORINFO);
            GetMonitorInfo( m_hAdapterMonitor, &adaptMonInfos );
            UInt iAdaptMonWidth = ( adaptMonInfos.rcWork.right - adaptMonInfos.rcWork.left );
            UInt iAdaptMonHeight = ( adaptMonInfos.rcWork.bottom - adaptMonInfos.rcWork.top );
            RECT rectResized;
            SetRect( &rectResized, 0, 0, iNewWidth, iNewHeight );
            AdjustWindowRect( &rectResized, GetWindowLong(m_hWindow, GWL_STYLE), FALSE );
            UInt iWinWidth = ( rectResized.right - rectResized.left );
            UInt iWinHeight = ( rectResized.bottom - rectResized.top );
            if ( iWinWidth > iAdaptMonWidth )
                iWinWidth = iAdaptMonWidth;
            if ( iWinHeight > iAdaptMonHeight )
                iWinHeight = iAdaptMonHeight;
            if ( rectResized.left < adaptMonInfos.rcWork.left || rectResized.right > adaptMonInfos.rcWork.right ||
                 rectResized.top < adaptMonInfos.rcWork.top || rectResized.bottom > adaptMonInfos.rcWork.bottom ) {
                 UInt iOffsetX = ( ( iAdaptMonWidth - iWinWidth ) >> 1 );
                 UInt iOffsetY = ( ( iAdaptMonHeight - iWinHeight ) >> 1 );
                 rectResized.left = adaptMonInfos.rcWork.left + iOffsetX;
                 rectResized.right = rectResized.left + iWinWidth;
                 rectResized.top = adaptMonInfos.rcWork.top + iOffsetY;
                 rectResized.bottom = rectResized.top + iWinHeight;
            }
            SetWindowPos( m_hWindow, NULL, rectResized.left, rectResized.top,
                          iWinWidth, iWinHeight, SWP_NOZORDER );
        } else {
            // Standard version
            RECT rectResized;
            SetRect( &rectResized, 0, 0, iNewWidth, iNewHeight );
            AdjustWindowRect( &rectResized, GetWindowLong(m_hWindow, GWL_STYLE), FALSE );
            UInt iWinWidth = ( rectResized.right - rectResized.left );
            UInt iWinHeight = ( rectResized.bottom - rectResized.top );
            SetWindowPos( m_hWindow, NULL, 0, 0,
                          iWinWidth, iWinHeight, SWP_NOMOVE | SWP_NOZORDER );
        }

        // Check if we hit Desktop or WM_GETMINMAXINFO limits and
        // were resized consequently, unnoticed to our willings ...
        RECT rectClient;
        GetClientRect( m_hWindow, &rectClient );
        UInt iClientWidth = ( rectClient.right - rectClient.left );
        UInt iClientHeight = ( rectClient.bottom - rectClient.top );
        if ( iClientWidth != iNewWidth || iClientHeight != iNewHeight )
            bRetry = true;
    }

    // Update states
    m_bFullScreen = !bNewIsWindowed;
    m_bMinimized = ( IsIconic(m_hWindow) != FALSE );
    m_bMaximized = ( IsZoomed(m_hWindow) != FALSE );
    m_bMinimizedFullscreen = ( m_bFullScreen && m_bMinimized );

    // Restore ShowCmd
    if ( !bNewIsWindowed )
        ShowWindow( m_hWindow, m_iBackupShowCmd );

    // Ensure that the display doesn't power down when fullscreen but does when windowed
    if( m_bFullScreen )
        SetThreadExecutionState( ES_DISPLAY_REQUIRED | ES_CONTINUOUS );
    else
        SetThreadExecutionState( ES_CONTINUOUS );

    // Conclusion
    m_bIgnoreResize = false;
    m_pCallbacks->OnResume();

    return bRetry;
}

/////////////////////////////////////////////////////////////////////////////////

Void GDIWindow::_GDI_CreateWindow( const WChar * strTitle, UInt iWidth, UInt iHeight )
{
    DebugAssert( m_hInstance != NULL );
    DebugAssert( m_hWindow == NULL );

    // Load default cursor
    m_hWindowCursor = LoadCursor( NULL, IDC_ARROW );

    // Window region
    RECT rectWindow;
    SetRect( &rectWindow, 0, 0, iWidth, iHeight );
	AdjustWindowRect( &rectWindow, WS_OVERLAPPEDWINDOW, FALSE );

    // Window class
    WNDCLASSEX winClass;
    winClass.cbSize = sizeof(WNDCLASSEX);
    winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
    winClass.hInstance = m_hInstance;
	winClass.style = CS_DBLCLKS;
	winClass.lpfnWndProc = (WNDPROC)_MsgProc_Static;
	winClass.lpszClassName = TEXT("Win32_GDI_WindowClass");
    winClass.lpszMenuName = NULL;
	winClass.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	winClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	winClass.hCursor = m_hWindowCursor;
	winClass.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	RegisterClassEx( &winClass );

    // Window creation
    m_hWindow = CreateWindowEx (
        0, TEXT("Win32_GDI_WindowClass"), strTitle, WS_OVERLAPPEDWINDOW, 32, 128,
        (rectWindow.right - rectWindow.left), (rectWindow.bottom - rectWindow.top),
		NULL, NULL, m_hInstance, (Void*)this
	);
    DebugAssert( m_hWindow != NULL );

    // Save window's monitor
    m_hWindowMonitor = MonitorFromWindow( m_hWindow, MONITOR_DEFAULTTOPRIMARY );
}

Void GDIWindow::_GDI_CheckWindowSizeChange()
{
    if ( m_bIgnoreResize || m_bFullScreen )
        return;

    RECT rectClient;
    GetClientRect( m_hWindow, &rectClient );
    m_pCallbacks->OnSizeChange( (rectClient.right - rectClient.left), (rectClient.bottom - rectClient.top) );
}
Void GDIWindow::_GDI_CheckWindowMonitorChange()
{
    if ( m_bIgnoreResize || m_bFullScreen )
        return;

    m_hWindowMonitor = MonitorFromWindow( m_hWindow, MONITOR_DEFAULTTOPRIMARY );
    if ( m_hWindowMonitor != m_hAdapterMonitor )
        m_pCallbacks->OnMonitorChange( m_hWindowMonitor );
}

LRESULT CALLBACK GDIWindow::_MsgProc_Static( HWND hWindow, UInt iMsg, WPARAM iWParam, LPARAM iLParam )
{
    GDIWindow * pThis = NULL;
    if ( iMsg == WM_CREATE ) {
	    // Window was just created, retrieve this from CreateWindowEx's lParam
	    pThis = (GDIWindow*)( ((LPCREATESTRUCT)iLParam)->lpCreateParams );
	    // Save it to the hwnd's user data ...
	    SetWindowLong( hWindow, GWL_USERDATA, (Long)pThis );
    } else {
	    // Retrieve our instance pointer
	    pThis = (GDIWindow*)GetWindowLong( hWindow, GWL_USERDATA );
    }
    if ( pThis != NULL )
	    return pThis->_MsgProc_Virtual( hWindow, iMsg, iWParam, iLParam );
    else
	    return DefWindowProc( hWindow, iMsg, iWParam, iLParam );
}
LRESULT GDIWindow::_MsgProc_Virtual( HWND hWindow, UInt iMsg, WPARAM iWParam, LPARAM iLParam )
{
    Bool bProcessDone = false;

    switch( iMsg ) {
        // Keyboard messages
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN: {
                // Handle key press
                KeyCode iKey = VKToKeyCode[iWParam & 0xff];
                bProcessDone = m_pCallbacks->OnKeyPress( iKey );
            } break;
        case WM_SYSKEYUP:
        case WM_KEYUP: {
                // Handle key release
                KeyCode iKey = VKToKeyCode[iWParam & 0xff];
                bProcessDone = m_pCallbacks->OnKeyRelease( iKey );
            } break;
        // Mouse messages
        case WM_MOUSEMOVE: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                m_pCallbacks->OnMouseMove( iX, iY );
            } break;
        case WM_MOUSEWHEEL: {
                POINT pt; // WM_MOUSEWHEEL needs screen->client conversion
                pt.x = (UInt)(LOWORD(iLParam));
                pt.y = (UInt)(HIWORD(iLParam));
                ScreenToClient( m_hWindow, &pt );
                Int iWheelDelta = (Int)( (Short)(HIWORD(iWParam)) ) / WHEEL_DELTA;
                bProcessDone = m_pCallbacks->OnMouseWheel( (UInt)pt.x, (UInt)pt.y, iWheelDelta );
            } break;
        case WM_LBUTTONDOWN: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                bProcessDone = m_pCallbacks->OnMousePress( iX, iY, KEYCODE_MOUSELEFT );
            } break;
        case WM_RBUTTONDOWN: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                bProcessDone = m_pCallbacks->OnMousePress( iX, iY, KEYCODE_MOUSERIGHT );
            } break;
        case WM_MBUTTONDOWN: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                bProcessDone = m_pCallbacks->OnMousePress( iX, iY, KEYCODE_MOUSEMIDDLE );
            } break;
        case WM_XBUTTONDOWN: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                KeyCode iButton = VKToKeyCode[iWParam & 0xff];
                bProcessDone = m_pCallbacks->OnMousePress( iX, iY, iButton );
            } break;
        case WM_LBUTTONUP: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                bProcessDone = m_pCallbacks->OnMouseRelease( iX, iY, KEYCODE_MOUSELEFT );
            } break;
        case WM_RBUTTONUP: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                bProcessDone = m_pCallbacks->OnMouseRelease( iX, iY, KEYCODE_MOUSERIGHT );
            } break;
        case WM_MBUTTONUP: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                bProcessDone = m_pCallbacks->OnMouseRelease( iX, iY, KEYCODE_MOUSEMIDDLE );
            } break;
        case WM_XBUTTONUP: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                KeyCode iButton = VKToKeyCode[iWParam & 0xff];
                bProcessDone = m_pCallbacks->OnMouseRelease( iX, iY, iButton );
            } break;
        case WM_LBUTTONDBLCLK: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                bProcessDone = m_pCallbacks->OnMouseDblClick( iX, iY, KEYCODE_MOUSELEFT );
            } break;
        case WM_RBUTTONDBLCLK: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                bProcessDone = m_pCallbacks->OnMouseDblClick( iX, iY, KEYCODE_MOUSERIGHT );
            } break;
        case WM_MBUTTONDBLCLK: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                bProcessDone = m_pCallbacks->OnMouseDblClick( iX, iY, KEYCODE_MOUSEMIDDLE );
            } break;
        case WM_XBUTTONDBLCLK: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                KeyCode iButton = VKToKeyCode[iWParam & 0xff];
                bProcessDone = m_pCallbacks->OnMouseDblClick( iX, iY, iButton );
            } break;
        // Activation messages
        case WM_ACTIVATEAPP: {
                if ( !m_bActive && iWParam == TRUE ) {
                    m_bActive = true;
                    // Enable controller inputs here ... //
                    if ( m_bMinimizedFullscreen ) {
                        // Fullscreen => Windowed (while minimized)
                        m_pCallbacks->OnResume();
                        m_bMinimizedFullscreen = false;
                    }
                    // Disable shortcut keys here //
                } else if ( m_bActive && iWParam == FALSE ) {
                    m_bActive = false;
                    // Disable controller inputs here ... //
                    if ( m_bFullScreen ) {
                        // Fullscreen => Minimized
                        ::ClipCursor( NULL ); // unclip cursor
                        m_pCallbacks->OnPause();
                        m_bMinimizedFullscreen = true;
                    }
                    // Enable shortcut keys here //
                }
            } break;
        case WM_ACTIVATE: {
                if ( LOWORD(iWParam) == WA_ACTIVE && HIWORD(iWParam) == 0 )
                    m_pCallbacks->OnWindowFocus();
            } break;
        // Paint message
        case WM_PAINT: {
                m_pCallbacks->OnWindowDraw();
            } break;
        // Moving / Sizing messages
        case WM_ENTERSIZEMOVE: {
                m_pCallbacks->OnPause();
                m_bIsInSizeMove = true;
            } break;
        case WM_EXITSIZEMOVE: {
                m_pCallbacks->OnResume();
                m_bIsInSizeMove = false;
                _GDI_CheckWindowSizeChange();
                _GDI_CheckWindowMonitorChange();
            } break;
        case WM_MOVE: {
                UInt iX = (UInt)(LOWORD(iLParam));
                UInt iY = (UInt)(HIWORD(iLParam));
                m_pCallbacks->OnWindowMove( iX, iY );
            } break;
        case WM_SIZE: {
                UInt iWidth = (UInt)(LOWORD(iLParam));
                UInt iHeight = (UInt)(HIWORD(iLParam));
                m_pCallbacks->OnWindowResize( iWidth, iHeight );
                if ( iWParam == SIZE_MINIMIZED ) {
                    m_pCallbacks->OnPause();
                    m_bMinimized = true;
                    m_bMaximized = false;
                } else {
                    RECT rcClient;
                    GetClientRect( m_hWindow, &rcClient );
                    if ( rcClient.top == 0 && rcClient.bottom == 0 ) {
                        // Taskbar min-max spamming case, causes SIZE_RESTORED when minimized
                    } else if ( iWParam == SIZE_MAXIMIZED ) {
                        if ( m_bMinimized )
                            m_pCallbacks->OnResume();
                        m_bMinimized = false;
                        m_bMaximized = true;
                        _GDI_CheckWindowSizeChange();
                        _GDI_CheckWindowMonitorChange();
                    } else if ( iWParam == SIZE_RESTORED ) {
                        if ( m_bMaximized ) {
                            m_bMaximized = false;
                            _GDI_CheckWindowSizeChange();
                            _GDI_CheckWindowMonitorChange();
                        } else if ( m_bMinimized ) {
                            m_pCallbacks->OnResume();
                            m_bMinimized = false;
                            _GDI_CheckWindowSizeChange();
                            _GDI_CheckWindowMonitorChange();
                        } else if ( m_bIsInSizeMove ) {
                            // User is dragging edges, don't update yet
                        } else {
                            // This is an instant resize (SetWindowPos etc ...)
                            _GDI_CheckWindowSizeChange();
                            _GDI_CheckWindowMonitorChange();
                        }
                    }
                }
            } break;
        // Menu messages
        case WM_ENTERMENULOOP: {
                m_pCallbacks->OnPause();
            } break;
        case WM_EXITMENULOOP: {
                m_pCallbacks->OnResume();
            } break;
        case WM_MENUCHAR: {
                // A menu is active and the user presses an unbinded key
                return MAKELRESULT( 0, MNC_CLOSE );
            } break;
        case WM_NCHITTEST: {
                // Prevent the user from selecting the menu in fullscreen mode
                if ( m_bFullScreen )
                    return HTCLIENT;
            } break;
        // Misc messages
        case WM_SETCURSOR: {
                if ( m_bActive && m_bFullScreen ) {
                    // Let us handle the cursor in fullscreen
                    return 1;
                }
            } break;
        case WM_GETMINMAXINFO: {
                // Retrieve Min-Max infos
                ( (MINMAXINFO*)iLParam )->ptMinTrackSize.x = GDIWINDOW_MIN_WIDTH;
                ( (MINMAXINFO*)iLParam )->ptMinTrackSize.y = GDIWINDOW_MIN_HEIGHT;
            } break;
        case WM_ERASEBKGND: {
                // Don't clear background
                return 1;
            } break;
        case WM_SYSCOMMAND: {
                UInt iSysCmd = (iWParam & 0xFFF0);
	            // Prevent screensaver or monitor powersave mode from starting
	            if ( iSysCmd == SC_SCREENSAVE || iSysCmd == SC_MONITORPOWER )
		            return 0;
                // Prevent moving / sizing / menu in fullscreen
                if ( m_bFullScreen ) {
                    if ( iSysCmd == SC_MOVE || iSysCmd == SC_SIZE ||
                         iSysCmd == SC_MAXIMIZE || iSysCmd == SC_KEYMENU )
		                return 0;
                }
            } break;
        // Exit sequence
        case WM_CLOSE: {
                DestroyWindow( m_hWindow );
                m_hWindow = NULL;
                UnregisterClass( TEXT("Win32_GDI_WindowClass"), NULL );
                return 0;
            } break;
        case WM_DESTROY: {
	            PostQuitMessage( 0 );
            } break;
        case WM_QUIT: {
                // Message loop exit-case, never goes here
            } break;
        default: break;
    }
    if ( bProcessDone )
        return 0;
    return DefWindowProc( hWindow, iMsg, iWParam, iLParam );
}

KeyCode GDIWindow::VKToKeyCode[256] =
{
    KEYCODE_NULL, // unassigned 0x00

    KEYCODE_MOUSELEFT,   // VK_LBUTTON
    KEYCODE_MOUSERIGHT,  // VK_RBUTTON
    KEYCODE_NULL,        // VK_CANCEL
    KEYCODE_MOUSEMIDDLE, // VK_MBUTTON

    KEYCODE_MOUSEX1, // VK_XBUTTON1
    KEYCODE_MOUSEX2, // VK_XBUTTON2

    KEYCODE_NULL, // unassigned 0x07

    KEYCODE_BACKSPACE, // VK_BACK
    KEYCODE_TAB,       // VK_TAB

    KEYCODE_NULL, KEYCODE_NULL, // unassigned 0x0a - 0x0b

    KEYCODE_CLRSCR, // VK_CLEAR
    KEYCODE_ENTER,  // VK_RETURN

    KEYCODE_NULL, KEYCODE_NULL, // unassigned 0x0e - 0x0f

    KEYCODE_SHIFT,    // VK_SHIFT
    KEYCODE_CTRL,     // VK_CONTROL
    KEYCODE_ALT,      // VK_MENU
    KEYCODE_PAUSE,    // VK_PAUSE
    KEYCODE_CAPSLOCK, // VK_CAPITAL

    // Chinese/Japan/Korean keys
    KEYCODE_NULL, // VK_KANA, VK_HANGEUL, VK_HANGUL
    KEYCODE_NULL, // unassigned 0x16
    KEYCODE_NULL, // VK_JUNJA
    KEYCODE_NULL, // VK_FINAL
    KEYCODE_NULL, // VK_HANJA, VK_KANJI

    KEYCODE_NULL, // unassigned 0x1a

    // Esc key
    KEYCODE_ESCAPE, // VK_ESCAPE

    KEYCODE_NULL, // VK_CONVERT
    KEYCODE_NULL, // VK_NONCONVERT
    KEYCODE_NULL, // VK_ACCEPT
    KEYCODE_NULL, // VK_MODECHANGE

    // Special keys
    KEYCODE_SPACE,    // VK_SPACE
    KEYCODE_PAGEUP,   // VK_PRIOR
    KEYCODE_PAGEDOWN, // VK_NEXT
    KEYCODE_END,      // VK_END
    KEYCODE_HOME,     // VK_HOME
    KEYCODE_LEFT,     // VK_LEFT
    KEYCODE_UP,       // VK_UP
    KEYCODE_RIGHT,    // VK_RIGHT
    KEYCODE_DOWN,     // VK_DOWN
    KEYCODE_NULL,     // VK_SELECT
    KEYCODE_NULL,     // VK_PRINT
    KEYCODE_NULL,     // VK_EXECUTE
    KEYCODE_PRNTSCR,  // VK_SNAPSHOT
    KEYCODE_INSERT,   // VK_INSERT
    KEYCODE_DELETE,   // VK_DELETE
    KEYCODE_NULL,     // VK_HELP

    // Ascii numbers
    KEYCODE_0, KEYCODE_1, KEYCODE_2, KEYCODE_3, //
    KEYCODE_4, KEYCODE_5, KEYCODE_6, KEYCODE_7, // Ascii 0x30 - 0x39
    KEYCODE_8, KEYCODE_9,                       //

    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL,               //
    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, // unassigned 0x3a - 0x40

    // Ascii characters
    KEYCODE_A, KEYCODE_B, KEYCODE_C, KEYCODE_D, //
    KEYCODE_E, KEYCODE_F, KEYCODE_G, KEYCODE_H, //
    KEYCODE_I, KEYCODE_J, KEYCODE_K, KEYCODE_L, //
    KEYCODE_M, KEYCODE_N, KEYCODE_O, KEYCODE_P, // Ascii 0x41 - 0x5a
    KEYCODE_Q, KEYCODE_R, KEYCODE_S, KEYCODE_T, //
    KEYCODE_U, KEYCODE_V, KEYCODE_W, KEYCODE_X, //
    KEYCODE_Y, KEYCODE_Z,                       //

    KEYCODE_NULL, // VK_LWIN
    KEYCODE_NULL, // VK_RWIN
    KEYCODE_NULL, // VK_APPS

    KEYCODE_NULL, // unassigned 0x5e

    KEYCODE_NULL, // VK_SLEEP

    // Numeric pad keys
    KEYCODE_NUMPAD_0,   // VK_NUMPAD0
    KEYCODE_NUMPAD_1,   // VK_NUMPAD1
    KEYCODE_NUMPAD_2,   // VK_NUMPAD2
    KEYCODE_NUMPAD_3,   // VK_NUMPAD3
    KEYCODE_NUMPAD_4,   // VK_NUMPAD4
    KEYCODE_NUMPAD_5,   // VK_NUMPAD5
    KEYCODE_NUMPAD_6,   // VK_NUMPAD6
    KEYCODE_NUMPAD_7,   // VK_NUMPAD7
    KEYCODE_NUMPAD_8,   // VK_NUMPAD8
    KEYCODE_NUMPAD_9,   // VK_NUMPAD9
    KEYCODE_NUMPAD_MUL, // VK_MULTIPLY
    KEYCODE_NUMPAD_ADD, // VK_ADD
    KEYCODE_NULL,       // VK_SEPARATOR
    KEYCODE_NUMPAD_SUB, // VK_SUBTRACT
    KEYCODE_NUMPAD_DOT, // VK_DECIMAL
    KEYCODE_NUMPAD_DIV, // VK_DIVIDE

    // Function keys
    KEYCODE_F1, KEYCODE_F2, KEYCODE_F3, KEYCODE_F4,     //
    KEYCODE_F5, KEYCODE_F6, KEYCODE_F7, KEYCODE_F8,     //
    KEYCODE_F9, KEYCODE_F10, KEYCODE_F11, KEYCODE_F12,  //
    KEYCODE_F13, KEYCODE_F14, KEYCODE_F15, KEYCODE_F16, // VK_F1 - VK_F24
    KEYCODE_F17, KEYCODE_F18, KEYCODE_F19, KEYCODE_F20, //
    KEYCODE_F21, KEYCODE_F22, KEYCODE_F23, KEYCODE_F24, //

    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, //
    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, // unassigned 0x88 - 0x8f

    KEYCODE_NUMLOCK,    // VK_NUMLOCK
    KEYCODE_SCROLLLOCK, // VK_SCROLL

    // NEC PC-9800 keys
    KEYCODE_NULL, // VK_OEM_NEC_EQUAL, VK_OEM_FJ_JISHO for Fujitsu/OASYS

    // Fujitsu/OASYS keys
    KEYCODE_NULL, // VK_OEM_FJ_MASSHOU
    KEYCODE_NULL, // VK_OEM_FJ_TOUROKU
    KEYCODE_NULL, // VK_OEM_FJ_LOYA
    KEYCODE_NULL, // VK_OEM_FJ_ROYA

    KEYCODE_NULL,                                           //
    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, // unassigned 0x97 - 0x9f
    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, //

    // Modifier keys
    KEYCODE_LSHIFT, // VK_LSHIFT
    KEYCODE_RSHIFT, // VK_RSHIFT
    KEYCODE_LCTRL,  // VK_LCONTROL
    KEYCODE_RCTRL,  // VK_RCONTROL
    KEYCODE_LALT,   // VK_LMENU
    KEYCODE_RALT,   // VK_RMENU

    // Multimedia keys
    KEYCODE_NULL, // VK_BROWSER_BACK
    KEYCODE_NULL, // VK_BROWSER_FORWARD
    KEYCODE_NULL, // VK_BROWSER_REFRESH
    KEYCODE_NULL, // VK_BROWSER_STOP
    KEYCODE_NULL, // VK_BROWSER_SEARCH
    KEYCODE_NULL, // VK_BROWSER_FAVORITES
    KEYCODE_NULL, // VK_BROWSER_HOME
    KEYCODE_NULL, // VK_VOLUME_MUTE
    KEYCODE_NULL, // VK_VOLUME_DOWN
    KEYCODE_NULL, // VK_VOLUME_UP
    KEYCODE_NULL, // VK_MEDIA_NEXT_TRACK
    KEYCODE_NULL, // VK_MEDIA_PREV_TRACK
    KEYCODE_NULL, // VK_MEDIA_STOP
    KEYCODE_NULL, // VK_MEDIA_PLAY_PAUSE
    KEYCODE_NULL, // VK_LAUNCH_MAIL
    KEYCODE_NULL, // VK_LAUNCH_MEDIA_SELECT
    KEYCODE_NULL, // VK_LAUNCH_APP1
    KEYCODE_NULL, // VK_LAUNCH_APP2

    KEYCODE_NULL, KEYCODE_NULL, // unassigned 0xb8 - 0xb9

    // OEM keys
    KEYCODE_OEM_1,      // VK_OEM_1
    KEYCODE_OEM_PLUS,   // VK_OEM_PLUS
    KEYCODE_OEM_COMMA,  // VK_OEM_COMMA
    KEYCODE_OEM_MINUS,  // VK_OEM_MINUS
    KEYCODE_OEM_PERIOD, // VK_OEM_PERIOD
    KEYCODE_OEM_2,      // VK_OEM_2
    KEYCODE_OEM_3,      // VK_OEM_3

    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, //
    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, //
    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, //
    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, // unassigned 0xc1 - 0xda
    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, //
    KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, KEYCODE_NULL, //
    KEYCODE_NULL, KEYCODE_NULL,                             //

    KEYCODE_OEM_4, // VK_OEM_4
    KEYCODE_OEM_5, // VK_OEM_5
    KEYCODE_OEM_6, // VK_OEM_6
    KEYCODE_OEM_7, // VK_OEM_7
    KEYCODE_OEM_8, // VK_OEM_8

    KEYCODE_NULL, // unassigned 0xe0

    // Enhanced keyboards
    KEYCODE_NULL,    // VK_OEM_AX
    KEYCODE_OEM_102, // VK_OEM_102
    KEYCODE_NULL,    // VK_ICO_HELP
    KEYCODE_NULL,    // VK_ICO_00
    KEYCODE_NULL,    // VK_PROCESSKEY
    KEYCODE_NULL,    // VK_ICO_CLEAR
    KEYCODE_NULL,    // VK_PACKET

    KEYCODE_NULL, // unassigned 0xe8

    // Nokia / Ericsson
    KEYCODE_NULL, // VK_OEM_RESET
    KEYCODE_NULL, // VK_OEM_JUMP
    KEYCODE_NULL, // VK_OEM_PA1
    KEYCODE_NULL, // VK_OEM_PA2
    KEYCODE_NULL, // VK_OEM_PA3
    KEYCODE_NULL, // VK_OEM_WSCTRL
    KEYCODE_NULL, // VK_OEM_CUSEL
    KEYCODE_NULL, // VK_OEM_ATTN
    KEYCODE_NULL, // VK_OEM_FINISH
    KEYCODE_NULL, // VK_OEM_COPY
    KEYCODE_NULL, // VK_OEM_AUTO
    KEYCODE_NULL, // VK_OEM_ENLW
    KEYCODE_NULL, // VK_OEM_BACKTAB
    KEYCODE_NULL, // VK_ATTN
    KEYCODE_NULL, // VK_CRSEL
    KEYCODE_NULL, // VK_EXSEL
    KEYCODE_NULL, // VK_EREOF
    KEYCODE_NULL, // VK_PLAY
    KEYCODE_NULL, // VK_ZOOM
    KEYCODE_NULL, // VK_NONAME
    KEYCODE_NULL, // VK_PA1
    KEYCODE_NULL, // VK_OEM_CLEAR

    KEYCODE_NULL  // unassigned 0xff
};
