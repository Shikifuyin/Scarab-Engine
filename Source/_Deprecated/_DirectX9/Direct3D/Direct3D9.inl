/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Direct3D9.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant abstraction for DirectX rendering.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Direct3D9Renderer implementation
inline UInt Direct3D9Renderer::GetVSBaseSampler() const {
    return D3DVERTEXTEXTURESAMPLER0;
}
inline UInt Direct3D9Renderer::GetVSMaxSamplers() const {
    return m_iMaxVSSamplers;
}
inline UInt Direct3D9Renderer::GetPSBaseSampler() const {
    return 0;
}
inline UInt Direct3D9Renderer::GetPSMaxSamplers() const {
    return m_iMaxPSSamplers;
}
inline UInt Direct3D9Renderer::GetMaxTextureUnits() const {
    return m_iMaxTextureUnits;
}

inline Bool Direct3D9Renderer::IsFullScreen() const {
    return ( m_devParameters.d3dPresentParams.Windowed == 0 );
}
inline UInt Direct3D9Renderer::GetBackBufferWidth() const {
    return m_devParameters.d3dPresentParams.BackBufferWidth;
}
inline UInt Direct3D9Renderer::GetBackBufferHeight() const {
    return m_devParameters.d3dPresentParams.BackBufferHeight;
}
inline TextureFormat Direct3D9Renderer::GetBackBufferFormat() const {
    return Direct3D9ConvertFormat(m_devParameters.d3dPresentParams.BackBufferFormat);
}
inline TextureFormat Direct3D9Renderer::GetDepthStencilFormat() const {
    return Direct3D9ConvertFormat(m_devParameters.d3dPresentParams.AutoDepthStencilFormat);
}
inline UInt Direct3D9Renderer::GetMultiSampleCount() const {
    return m_devParameters.d3dPresentParams.MultiSampleQuality;
}

/////////////////////////////////////////////////////////////////////////////////

inline Bool Direct3D9Renderer::_D3D9_CanResetDevice( const D3D9DeviceParameters & newParameters,
                                                     const D3D9DeviceParameters & oldParameters ) const {
    return ( newParameters.iAdapterOrdinal == oldParameters.iAdapterOrdinal &&
             newParameters.d3dDeviceType == oldParameters.d3dDeviceType &&
             newParameters.dwBehaviour == oldParameters.dwBehaviour );
}

