/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Interface/InterfaceManager.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Framework, Interface : InterfaceManager
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// HUDBackboardModel implementation

/////////////////////////////////////////////////////////////////////////////////
// InterfaceManager implementation
inline const GChar * InterfaceManager::GetName() {
    return TEXT("InterfaceManager");
}
inline const GChar * InterfaceManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);"
    );
}

inline GPU2DContext * InterfaceManager::GetHUDContext() const {
    return GUIFn->GetScreenContext();
}

inline GPU2DBitmap * InterfaceManager::GetHUDSkinBitmap() const {
    return m_pSkinBitmap;
}

inline const Rectangle2 & InterfaceManager::GetHUDLocation( HUDImage iHUDImage ) const {
    Assert( iHUDImage < HUD_IMAGE_COUNT );
    return m_arrHUDLocations[iHUDImage];
}

inline GPU2DSolidColorBrush * InterfaceManager::GetHUDColorBrush() const {
    return &m_hColorBrush;
}
inline const Color4 & InterfaceManager::GetHUDColor( HUDColor iHUDColor ) const {
    Assert( iHUDColor < HUD_COLOR_COUNT );
    return m_arrHUDColors[iHUDColor];
}

inline GPU2DSolidColorBrush * InterfaceManager::GetHUDTextBrush() const {
    return &m_hTextBrush;
}
inline GPU2DTextFormat * InterfaceManager::GetHUDTextFormat( HUDFont iHUDFont ) const {
    Assert( iHUDFont < HUD_FONT_COUNT );
    return m_arrTextFormats[iHUDFont];
}

inline HUDDragNDropData * InterfaceManager::GetDragNDropData() {
    return &m_hDragNDrop;
}

