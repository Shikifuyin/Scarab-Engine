/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/GDI+/GDIWindow.inl
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
// GDIWindowCallbacks implementation
inline Void GDIWindowCallbacks::SetUserData( Void * pUserData ) {
    m_pUserData = pUserData;
}

/////////////////////////////////////////////////////////////////////////////////
// GDIWindow implementation
inline Bool GDIWindow::IsActive() const {
    return m_bActive;
}
inline Bool GDIWindow::IsFullScreen() const {
    return m_bFullScreen;
}
inline Bool GDIWindow::IsMinimized() const {
    return m_bMinimized;
}
inline Bool GDIWindow::IsMaximized() const {
    return m_bMaximized;
}
inline Bool GDIWindow::IgnoreResize() const {
    return m_bIgnoreResize;
}
inline Bool GDIWindow::IsInSizeMove() const {
    return m_bIsInSizeMove;
}

/////////////////////////////////////////////////////////////////////////////////

inline HWND GDIWindow::_GetHWND() const {
    return m_hWindow;
}
inline HCURSOR GDIWindow::_GetHCURSOR() const {
    return m_hWindowCursor;
}

inline UInt GDIWindow::_SwitchDisplay_PreviousWidth( Bool bWindowed ) const {
    return bWindowed ? m_iWindowedWidth : m_iFullScreenWidth;
}
inline UInt GDIWindow::_SwitchDisplay_PreviousHeight( Bool bWindowed ) const {
    return bWindowed ? m_iWindowedHeight : m_iFullScreenHeight;
}
