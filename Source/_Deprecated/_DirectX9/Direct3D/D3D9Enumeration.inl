/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/D3D9Enumeration.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Device Enumeration for Direct3D 9.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// D3D9Enumeration implementation
inline Void D3D9Enumeration::SetDriver( IDirect3D9 * pDriver ) {
    m_pDriver = pDriver;
}

inline Bool D3D9Enumeration::IsEnumDone() const {
    return m_bEnumDone;
}

inline UInt D3D9Enumeration::GetAdapterCount() const {
    return m_iAdapterCount;
}

inline Void D3D9Enumeration::SetEnumCallback( AcceptDevice pfAcceptDevice ) {
    m_pfAcceptDevice = pfAcceptDevice;
}
inline Void D3D9Enumeration::SetResolutionMinMax( UInt iMinWidth, UInt iMinHeight, UInt iMaxWidth, UInt iMaxHeight ) {
    m_iMinWidth = iMinWidth;
    m_iMinHeight = iMinHeight;
    m_iMaxWidth = iMaxWidth;
    m_iMaxHeight = iMaxHeight;
}
inline Void D3D9Enumeration::SetRefreshMinMax( UInt iMin, UInt iMax ) {
    m_iMinRefresh = iMin;
    m_iMaxRefresh = iMax;
}
inline Void D3D9Enumeration::SetMultiSampleMax( UInt iMax ) {
    m_iMaxMultiSamples = iMax;
}

