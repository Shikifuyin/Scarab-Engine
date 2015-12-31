/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/GDI+/GDIWindow.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_GDIPLUS_GDIWINDOW_H
#define SCARAB_THIRDPARTY_GDIPLUS_GDIWINDOW_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Forwards/ForwardWin32.h"

#include "../System/Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GDIWINDOW_MIN_WIDTH 200
#define GDIWINDOW_MIN_HEIGHT 200

/////////////////////////////////////////////////////////////////////////////////
// The GDIWindowCallbacks class
class GDIWindowCallbacks
{
public:
    GDIWindowCallbacks( Void * pUserData = NULL );
    virtual ~GDIWindowCallbacks();

    inline Void SetUserData( Void * pUserData );

    // Main loop
    virtual Void OnIdleTime() = 0;

    virtual Void OnPause() = 0;
    virtual Void OnResume() = 0;

    // Display
    virtual Void OnSizeChange( UInt iNewWidth, UInt iNewHeight ) = 0;
    virtual Void OnMonitorChange( Void * pMonitor ) = 0;

    // Window
    virtual Void OnWindowFocus() = 0;

    virtual Void OnWindowMove( UInt iX, UInt iY ) = 0;
    virtual Void OnWindowResize( UInt iWidth, UInt iHeight ) = 0;

    virtual Void OnWindowDraw() = 0;

    // Keyboard
    virtual Bool OnKeyPress( KeyCode iKey ) = 0;
    virtual Bool OnKeyRelease( KeyCode iKey ) = 0;

    // Mouse
    virtual Void OnMouseMove( UInt iX, UInt iY ) = 0;
    virtual Bool OnMousePress( UInt iX, UInt iY, KeyCode iKey ) = 0;
    virtual Bool OnMouseRelease( UInt iX, UInt iY, KeyCode iKey ) = 0;
    virtual Bool OnMouseDblClick( UInt iX, UInt iY, KeyCode iKey ) = 0;
    virtual Bool OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta ) = 0;
protected:
    Void * m_pUserData;
};

/////////////////////////////////////////////////////////////////////////////////
// The GDIWindow class
class GDIWindow
{
public:
    GDIWindow( const WChar * strTitle, UInt iWidth, UInt iHeight, GDIWindowCallbacks * pCallbacks );
    ~GDIWindow();

    // Getters
    UInt GetWidth() const;
    UInt GetHeight() const;

    inline Bool IsActive() const;
    inline Bool IsFullScreen() const;
    inline Bool IsMinimized() const;
    inline Bool IsMaximized() const;
    inline Bool IgnoreResize() const;
    inline Bool IsInSizeMove() const;

    // Window methods
    Void Show( Bool bShow );
    Void Paint();

    // Cursor methods
    Void ShowCursor( Bool bShow ) const;
    Void ClipCursor( Bool bClip ) const;

    // Window lifetime
    Void MessageLoop();
    Void Exit();

public: // internal : Direct3D helpers
    // accessors
    inline HWND _GetHWND() const;
    inline HCURSOR _GetHCURSOR() const;
    // SwitchDisplay transition
    Void _SwitchDisplay_Prepare( Bool bNewIsWindowed, Bool bHasOldDisplay,
                                 Bool bOldIsWindowed, UInt iOldWidth, UInt iOldHeight );
    Void _SwitchDisplay_Abort( Bool bNewIsWindowed, Bool bHasOldDisplay, Bool bOldIsWindowed );
    Bool _SwitchDisplay_Finalize( Bool bNewIsWindowed, Bool bHasOldDisplay, Bool bOldIsWindowed,
                                  Bool bKeepWindowedSize, UInt iNewWidth, UInt iNewHeight,
                                  HMONITOR hNewAdapterMonitor );
    inline UInt _SwitchDisplay_PreviousWidth( Bool bWindowed ) const;
    inline UInt _SwitchDisplay_PreviousHeight( Bool bWindowed ) const;

private:
    // GDI Interface
    Void _GDI_CreateWindow( const WChar * strTitle, UInt iWidth, UInt iHeight );

    Void _GDI_CheckWindowSizeChange();
    Void _GDI_CheckWindowMonitorChange();

    // Message callback
    static LRESULT CALLBACK _MsgProc_Static( HWND hWindow, UInt iMsg, WPARAM iWParam, LPARAM iLParam );
    LRESULT _MsgProc_Virtual( HWND hWindow, UInt iMsg, WPARAM iWParam, LPARAM iLParam );
    static KeyCode VKToKeyCode[256];

    // Callbacks
    GDIWindowCallbacks * m_pCallbacks;

    // GDI data
    HINSTANCE m_hInstance;
    HWND m_hWindow;
    HMONITOR m_hWindowMonitor;
    HMONITOR m_hAdapterMonitor;
    HCURSOR m_hWindowCursor;
    HMENU m_hWindowMenu;
    Bool m_bClipToSingleAdapter;

    // General state
    Bool m_bActive;
    Bool m_bFullScreen;
    Bool m_bMinimized, m_bMaximized;
    Bool m_bMinimizedFullscreen;
    Bool m_bIgnoreResize;
    Bool m_bIsInSizeMove;

    // Windowed state
    UInt m_iWindowedWidth, m_iWindowedHeight;
    WINDOWPLACEMENT m_wpWindowed;
    DWord m_iWindowedStyle;
    Bool m_bWindowedTopMost;

    // FullScreen state
    UInt m_iFullScreenWidth, m_iFullScreenHeight;

    // Backups
    DWord m_iBackupStyle;
    UInt m_iBackupShowCmd;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GDIWindow.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_GDIPLUS_GDIWINDOW_H

