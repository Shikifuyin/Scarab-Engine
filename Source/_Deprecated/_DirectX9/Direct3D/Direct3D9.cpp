/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Direct3D9.cpp
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
// Internal dependency : Direct3D9, DirectX
#define D3D_DEBUG_INFO // Direct3D debug flag

#define WIN32_LEAN_AND_MEAN
#include <d3d9.h>

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Direct3D9.h"

/////////////////////////////////////////////////////////////////////////////////
// Direct3D9Renderer implementation
Direct3D9Renderer::Direct3D9Renderer( const Direct3D9Input & hInput, UInt iWidth, UInt iHeight,
                                      TextureFormat iColorFormat, TextureFormat iDepthStencilFormat, UInt iMultiSampleCount ):
    m_eDevices()
{
    m_pfOnDestroyResources = hInput.pfOnDestroyResources;
    m_pfOnRecreateResources = hInput.pfOnRecreateResources;
    m_pResourceUserData = hInput.pResourceUserData;
    m_pfOnShaderVersionUpdate = hInput.pfOnShaderVersionUpdate;
    m_pShaderVersionUserData = hInput.pShaderVersionUserData;

    // Window
    m_pWindow = hInput.pWindow;

    // Driver
    m_pDriver = Direct3DCreate9( D3D_SDK_VERSION );
    DebugAssert( m_pDriver != NULL );
    m_eDevices.SetDriver( m_pDriver );
    m_eDevices.SetEnumCallback( _AcceptMinimalDevice );
    m_eDevices.SetResolutionMinMax( 640, 480, 1280, 1024 );
    m_eDevices.SetRefreshMinMax( 60, 90 );
    m_eDevices.SetMultiSampleMax( 0 );
    m_eDevices.Enumerate();
    MemZero( &m_devParameters, sizeof(D3D9DeviceParameters) );

    // Device
    m_pDevice = NULL;
    m_devState = D3D9DEVICE_NULL;
    MemZero( &m_devCaps, sizeof(D3DCAPS9) );
    MemZero( &m_devBackBufferDesc, sizeof(D3DSURFACE_DESC) );
    MemZero( &m_devViewport, sizeof(D3DVIEWPORT9) );

    D3D9DeviceParameters bestParameters;
    MemZero( &bestParameters, sizeof(D3D9DeviceParameters) );
    bestParameters.iAdapterOrdinal = D3DADAPTER_DEFAULT;
    bestParameters.d3dAdapterFormat = D3DFMT_A8R8G8B8;
    bestParameters.d3dDeviceType = D3DDEVTYPE_HAL;
    bestParameters.dwBehaviour = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    bestParameters.d3dPresentParams.BackBufferWidth = iWidth;
    bestParameters.d3dPresentParams.BackBufferHeight = iHeight;
    bestParameters.d3dPresentParams.BackBufferFormat = Direct3D9TextureFormat[iColorFormat];
    bestParameters.d3dPresentParams.BackBufferCount = 1;
    bestParameters.d3dPresentParams.Windowed = m_pWindow->IsFullScreen() ? FALSE : TRUE;
    bestParameters.d3dPresentParams.AutoDepthStencilFormat = Direct3D9TextureFormat[iDepthStencilFormat];
    bestParameters.d3dPresentParams.EnableAutoDepthStencil = 
        ( iDepthStencilFormat != TEXTURE_FMT_UNDEFINED ) ? TRUE : FALSE;
    bestParameters.d3dPresentParams.MultiSampleType = (D3DMULTISAMPLE_TYPE)( iMultiSampleCount );
    bestParameters.d3dPresentParams.MultiSampleQuality = iMultiSampleCount;
    bestParameters.d3dPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    D3D9DeviceMatchOptions matchOptions;
    matchOptions.iAdapterOrdinal = DEVMATCH_CLOSEST;
    matchOptions.iAdapterFormat = DEVMATCH_CLOSEST;
    matchOptions.iDeviceType = DEVMATCH_CLOSEST;
    matchOptions.iVertexProcessing = DEVMATCH_CLOSEST;
    matchOptions.iResolution = DEVMATCH_CLOSEST;
    matchOptions.iBackBufferFormat = DEVMATCH_CLOSEST;
    matchOptions.iBackBufferCount = DEVMATCH_PRESERVE;
    matchOptions.iWindowed = DEVMATCH_PRESERVE;
    matchOptions.iRefreshRate = DEVMATCH_IGNORE;
    matchOptions.iDepthStencilFormat = DEVMATCH_CLOSEST;
    matchOptions.iMultiSample = DEVMATCH_CLOSEST;
    matchOptions.iPresentFlags = DEVMATCH_IGNORE;
    matchOptions.iPresentInterval = DEVMATCH_PRESERVE;
    matchOptions.iSwapEffect = DEVMATCH_IGNORE;

    D3D9DeviceParameters validParameters;
    Bool bOk = m_eDevices.MatchValidDevice( &validParameters, matchOptions, bestParameters );
    DebugAssert( bOk );

    bOk = _D3D9_SwitchDevice( &validParameters, false );
    DebugAssert( bOk );

    // D3D9 cursor
    _D3D9_SetWindowCursor();
    _D3D9_UpdateCursorState();
}
Direct3D9Renderer::~Direct3D9Renderer()
{
    // D3D9 cursor
    // nothing to do

    // Device
    if ( m_pDevice != NULL )
        _D3D9_DestroyDevice();

    // Driver
    m_pDriver->Release();
    m_pDriver = NULL;
}

Void Direct3D9Renderer::ToggleFullScreen()
{
    DebugAssert( m_pDevice != NULL );

    // Backup old parameters
    D3D9DeviceParameters oldParameters;
    MemCopy( &oldParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );

    // Create new parameters
    D3D9DeviceParameters newParameters;
    MemCopy( &newParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
    newParameters.d3dPresentParams.Windowed = !( newParameters.d3dPresentParams.Windowed );

    // Match options
    D3D9DeviceMatchOptions matchOptions;
    matchOptions.iAdapterOrdinal = DEVMATCH_PRESERVE;
    matchOptions.iAdapterFormat = DEVMATCH_CLOSEST;
    matchOptions.iDeviceType = DEVMATCH_PRESERVE;
    matchOptions.iVertexProcessing = DEVMATCH_PRESERVE;
    matchOptions.iBackBufferFormat = DEVMATCH_CLOSEST;
    matchOptions.iBackBufferCount = DEVMATCH_PRESERVE;
    matchOptions.iWindowed = DEVMATCH_PRESERVE;
    matchOptions.iRefreshRate = DEVMATCH_IGNORE;
    matchOptions.iDepthStencilFormat = DEVMATCH_CLOSEST;
    matchOptions.iMultiSample = DEVMATCH_CLOSEST;
    matchOptions.iPresentFlags = DEVMATCH_PRESERVE;
    matchOptions.iPresentInterval = DEVMATCH_PRESERVE;
    matchOptions.iSwapEffect = DEVMATCH_PRESERVE;

    // Get previous state
    Bool bWindowed = ( newParameters.d3dPresentParams.Windowed != FALSE );
    UInt iWidth = m_pWindow->_SwitchDisplay_PreviousWidth( bWindowed );
    UInt iHeight = m_pWindow->_SwitchDisplay_PreviousHeight( bWindowed );
    if ( iWidth > 0 && iHeight > 0 ) {
        matchOptions.iResolution = DEVMATCH_CLOSEST;
        newParameters.d3dPresentParams.BackBufferWidth = iWidth;
        newParameters.d3dPresentParams.BackBufferHeight = iHeight;
    } else
        matchOptions.iResolution = DEVMATCH_IGNORE;

    // Match valid parameters
    D3D9DeviceParameters validParameters;
    Bool bOk = m_eDevices.MatchValidDevice( &validParameters, matchOptions, newParameters );
    DebugAssert( bOk );

    // Switch to new valid parameters
    bOk = _D3D9_SwitchDevice( &validParameters, false );
    if ( !bOk ) {
        // Failed, switch back
        bOk = _D3D9_SwitchDevice( &oldParameters, false );
        DebugAssert( bOk );
    }
}
Void Direct3D9Renderer::SwitchMonitor( Void * pMonitor )
{
    DebugAssert( m_pDevice != NULL );

    HMONITOR hWindowMonitor = (HMONITOR)pMonitor;
    UInt iNewAdapter = _D3D9_AdapterFromMonitor( hWindowMonitor );
    if ( iNewAdapter == INVALID_OFFSET )
        return;

    // Create new parameters
    D3D9DeviceParameters newParameters;
    MemCopy( &newParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
    newParameters.iAdapterOrdinal = iNewAdapter;

    // Match options
    D3D9DeviceMatchOptions matchOptions;
    matchOptions.iAdapterOrdinal = DEVMATCH_PRESERVE;
    matchOptions.iAdapterFormat = DEVMATCH_CLOSEST;
    matchOptions.iDeviceType = DEVMATCH_PRESERVE;
    matchOptions.iVertexProcessing = DEVMATCH_PRESERVE;
    matchOptions.iResolution = DEVMATCH_CLOSEST;
    matchOptions.iBackBufferFormat = DEVMATCH_CLOSEST;
    matchOptions.iBackBufferCount = DEVMATCH_PRESERVE;
    matchOptions.iWindowed = DEVMATCH_PRESERVE;
    matchOptions.iRefreshRate = DEVMATCH_IGNORE;
    matchOptions.iDepthStencilFormat = DEVMATCH_CLOSEST;
    matchOptions.iMultiSample = DEVMATCH_CLOSEST;
    matchOptions.iPresentFlags = DEVMATCH_PRESERVE;
    matchOptions.iPresentInterval = DEVMATCH_PRESERVE;
    matchOptions.iSwapEffect = DEVMATCH_PRESERVE;

    // Match valid parameters
    D3D9DeviceParameters validParameters;
    Bool bOk = m_eDevices.MatchValidDevice( &validParameters, matchOptions, newParameters );
    DebugAssert( bOk );

    // Switch to new valid parameters
    bOk = _D3D9_SwitchDevice( &validParameters, false );
    DebugAssert( bOk );
}

Void Direct3D9Renderer::SwitchResolution( UInt iBackBufferWidth, UInt iBackBufferHeight )
{
    DebugAssert( m_pDevice != NULL );

    if ( m_pWindow->IsFullScreen() ) {
        // Create new parameters
        D3D9DeviceParameters newParameters;
        MemCopy( &newParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
        newParameters.d3dPresentParams.BackBufferWidth = iBackBufferWidth;
        newParameters.d3dPresentParams.BackBufferHeight = iBackBufferHeight;

        // Match options
        D3D9DeviceMatchOptions matchOptions;
        matchOptions.iAdapterOrdinal = DEVMATCH_PRESERVE;
        matchOptions.iAdapterFormat = DEVMATCH_PRESERVE;
        matchOptions.iDeviceType = DEVMATCH_PRESERVE;
        matchOptions.iVertexProcessing = DEVMATCH_PRESERVE;
        matchOptions.iResolution = DEVMATCH_CLOSEST;
        matchOptions.iBackBufferFormat = DEVMATCH_PRESERVE;
        matchOptions.iBackBufferCount = DEVMATCH_PRESERVE;
        matchOptions.iWindowed = DEVMATCH_PRESERVE;
        matchOptions.iRefreshRate = DEVMATCH_PRESERVE;
        matchOptions.iDepthStencilFormat = DEVMATCH_PRESERVE;
        matchOptions.iMultiSample = DEVMATCH_PRESERVE;
        matchOptions.iPresentFlags = DEVMATCH_PRESERVE;
        matchOptions.iPresentInterval = DEVMATCH_PRESERVE;
        matchOptions.iSwapEffect = DEVMATCH_PRESERVE;

        // Match valid parameters
        D3D9DeviceParameters validParameters;
        Bool bOk = m_eDevices.MatchValidDevice( &validParameters, matchOptions, newParameters );
        DebugAssert( bOk );

        // Switch to valid parameters
        bOk = _D3D9_SwitchDevice( &validParameters, false );
        DebugAssert( bOk );
    } else {
        if ( m_pWindow->IgnoreResize() )
            return;
        
        // Create new parameters
        D3D9DeviceParameters newParameters;
        MemCopy( &newParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
        newParameters.d3dPresentParams.BackBufferWidth = iBackBufferWidth;
        newParameters.d3dPresentParams.BackBufferHeight = iBackBufferHeight;

        // Switch to new parameters
        Bool bOk = _D3D9_SwitchDevice( &newParameters, false );
        DebugAssert( bOk );
    }
}
Void Direct3D9Renderer::SwitchBackBufferFormat( TextureFormat iBackBufferFormat )
{
    DebugAssert( m_pDevice != NULL );

    // Create new parameters
    D3D9DeviceParameters newParameters;
    MemCopy( &newParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
    newParameters.d3dPresentParams.BackBufferFormat = Direct3D9TextureFormat[iBackBufferFormat];

    // Match options
    D3D9DeviceMatchOptions matchOptions;
    matchOptions.iAdapterOrdinal = DEVMATCH_PRESERVE;
    matchOptions.iAdapterFormat = DEVMATCH_CLOSEST;
    matchOptions.iDeviceType = DEVMATCH_PRESERVE;
    matchOptions.iVertexProcessing = DEVMATCH_PRESERVE;
    matchOptions.iResolution = DEVMATCH_PRESERVE;
    matchOptions.iBackBufferFormat = DEVMATCH_CLOSEST;
    matchOptions.iBackBufferCount = DEVMATCH_PRESERVE;
    matchOptions.iWindowed = DEVMATCH_PRESERVE;
    matchOptions.iRefreshRate = DEVMATCH_PRESERVE;
    matchOptions.iDepthStencilFormat = DEVMATCH_PRESERVE;
    matchOptions.iMultiSample = DEVMATCH_PRESERVE;
    matchOptions.iPresentFlags = DEVMATCH_PRESERVE;
    matchOptions.iPresentInterval = DEVMATCH_PRESERVE;
    matchOptions.iSwapEffect = DEVMATCH_PRESERVE;

    // Match valid parameters
    D3D9DeviceParameters validParameters;
    Bool bOk = m_eDevices.MatchValidDevice( &validParameters, matchOptions, newParameters );
    DebugAssert( bOk );

    // Switch to valid parameters
    bOk = _D3D9_SwitchDevice( &validParameters, false );
    DebugAssert( bOk );
}
Void Direct3D9Renderer::SwitchDepthStencilFormat( TextureFormat iDepthStencilFormat )
{
    DebugAssert( m_pDevice != NULL );

    // Create new parameters
    D3D9DeviceParameters newParameters;
    MemCopy( &newParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
    newParameters.d3dPresentParams.AutoDepthStencilFormat = Direct3D9TextureFormat[iDepthStencilFormat];
    newParameters.d3dPresentParams.EnableAutoDepthStencil =
        ( iDepthStencilFormat != TEXTURE_FMT_UNDEFINED ) ? TRUE : FALSE;

    // Match options
    D3D9DeviceMatchOptions matchOptions;
    matchOptions.iAdapterOrdinal = DEVMATCH_PRESERVE;
    matchOptions.iAdapterFormat = DEVMATCH_PRESERVE;
    matchOptions.iDeviceType = DEVMATCH_PRESERVE;
    matchOptions.iVertexProcessing = DEVMATCH_PRESERVE;
    matchOptions.iResolution = DEVMATCH_PRESERVE;
    matchOptions.iBackBufferFormat = DEVMATCH_PRESERVE;
    matchOptions.iBackBufferCount = DEVMATCH_PRESERVE;
    matchOptions.iWindowed = DEVMATCH_PRESERVE;
    matchOptions.iRefreshRate = DEVMATCH_PRESERVE;
    matchOptions.iDepthStencilFormat = DEVMATCH_CLOSEST;
    matchOptions.iMultiSample = DEVMATCH_PRESERVE;
    matchOptions.iPresentFlags = DEVMATCH_PRESERVE;
    matchOptions.iPresentInterval = DEVMATCH_PRESERVE;
    matchOptions.iSwapEffect = DEVMATCH_PRESERVE;

    // Match valid parameters
    D3D9DeviceParameters validParameters;
    Bool bOk = m_eDevices.MatchValidDevice( &validParameters, matchOptions, newParameters );
    DebugAssert( bOk );

    // Switch to valid parameters
    bOk = _D3D9_SwitchDevice( &validParameters, false );
    DebugAssert( bOk );
}
Void Direct3D9Renderer::SwitchMultiSampleCount( UInt iMultiSampleCount )
{
    DebugAssert( m_pDevice != NULL );

    // Create new parameters
    D3D9DeviceParameters newParameters;
    MemCopy( &newParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
    newParameters.d3dPresentParams.MultiSampleType = (D3DMULTISAMPLE_TYPE)iMultiSampleCount;
    newParameters.d3dPresentParams.MultiSampleQuality = iMultiSampleCount;

    // Match options
    D3D9DeviceMatchOptions matchOptions;
    matchOptions.iAdapterOrdinal = DEVMATCH_PRESERVE;
    matchOptions.iAdapterFormat = DEVMATCH_PRESERVE;
    matchOptions.iDeviceType = DEVMATCH_PRESERVE;
    matchOptions.iVertexProcessing = DEVMATCH_PRESERVE;
    matchOptions.iResolution = DEVMATCH_PRESERVE;
    matchOptions.iBackBufferFormat = DEVMATCH_PRESERVE;
    matchOptions.iBackBufferCount = DEVMATCH_PRESERVE;
    matchOptions.iWindowed = DEVMATCH_PRESERVE;
    matchOptions.iRefreshRate = DEVMATCH_PRESERVE;
    matchOptions.iDepthStencilFormat = DEVMATCH_PRESERVE;
    matchOptions.iMultiSample = DEVMATCH_CLOSEST;
    matchOptions.iPresentFlags = DEVMATCH_PRESERVE;
    matchOptions.iPresentInterval = DEVMATCH_PRESERVE;
    matchOptions.iSwapEffect = DEVMATCH_PRESERVE;

    // Match valid parameters
    D3D9DeviceParameters validParameters;
    Bool bOk = m_eDevices.MatchValidDevice( &validParameters, matchOptions, newParameters );
    DebugAssert( bOk );

    // Switch to valid parameters
    bOk = _D3D9_SwitchDevice( &validParameters, false );
    DebugAssert( bOk );
}
Void Direct3D9Renderer::SwitchDisplay( UInt iBackBufferWidth, UInt iBackBufferHeight, TextureFormat iBackBufferFormat )
{
    DebugAssert( m_pDevice != NULL );

    // Create new parameters
    D3D9DeviceParameters newParameters;
    MemCopy( &newParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
    if ( m_pWindow->IsFullScreen() || !(m_pWindow->IgnoreResize()) ) {
        newParameters.d3dPresentParams.BackBufferWidth = iBackBufferWidth;
        newParameters.d3dPresentParams.BackBufferHeight = iBackBufferHeight;
    }
    newParameters.d3dPresentParams.BackBufferFormat = Direct3D9TextureFormat[iBackBufferFormat];

    // Match options
    D3D9DeviceMatchOptions matchOptions;
    matchOptions.iAdapterOrdinal = DEVMATCH_PRESERVE;
    matchOptions.iAdapterFormat = DEVMATCH_CLOSEST;
    matchOptions.iDeviceType = DEVMATCH_PRESERVE;
    matchOptions.iVertexProcessing = DEVMATCH_PRESERVE;
    matchOptions.iResolution = DEVMATCH_CLOSEST;
    matchOptions.iBackBufferFormat = DEVMATCH_CLOSEST;
    matchOptions.iBackBufferCount = DEVMATCH_PRESERVE;
    matchOptions.iWindowed = DEVMATCH_PRESERVE;
    matchOptions.iRefreshRate = DEVMATCH_PRESERVE;
    matchOptions.iDepthStencilFormat = DEVMATCH_PRESERVE;
    matchOptions.iMultiSample = DEVMATCH_PRESERVE;
    matchOptions.iPresentFlags = DEVMATCH_PRESERVE;
    matchOptions.iPresentInterval = DEVMATCH_PRESERVE;
    matchOptions.iSwapEffect = DEVMATCH_PRESERVE;

    // Match valid parameters
    D3D9DeviceParameters validParameters;
    Bool bOk = m_eDevices.MatchValidDevice( &validParameters, matchOptions, newParameters );
    DebugAssert( bOk );

    // Switch to valid parameters
    bOk = _D3D9_SwitchDevice( &validParameters, false );
    DebugAssert( bOk );
}
Void Direct3D9Renderer::SwitchDevice( UInt iBackBufferWidth, UInt iBackBufferHeight, TextureFormat iBackBufferFormat,
                                      TextureFormat iDepthStencilFormat, UInt iMultiSampleCount )
{
    DebugAssert( m_pDevice != NULL );

    // Create new parameters
    D3D9DeviceParameters newParameters;
    MemCopy( &newParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
    if ( m_pWindow->IsFullScreen() || !(m_pWindow->IgnoreResize()) ) {
        newParameters.d3dPresentParams.BackBufferWidth = iBackBufferWidth;
        newParameters.d3dPresentParams.BackBufferHeight = iBackBufferHeight;
    }
    newParameters.d3dPresentParams.BackBufferFormat = Direct3D9TextureFormat[iBackBufferFormat];
    newParameters.d3dPresentParams.AutoDepthStencilFormat = Direct3D9TextureFormat[iDepthStencilFormat];
    newParameters.d3dPresentParams.EnableAutoDepthStencil =
        ( iDepthStencilFormat != TEXTURE_FMT_UNDEFINED ) ? TRUE : FALSE;
    newParameters.d3dPresentParams.MultiSampleType = (D3DMULTISAMPLE_TYPE)iMultiSampleCount;
    newParameters.d3dPresentParams.MultiSampleQuality = iMultiSampleCount;

    // Match options
    D3D9DeviceMatchOptions matchOptions;
    matchOptions.iAdapterOrdinal = DEVMATCH_PRESERVE;
    matchOptions.iAdapterFormat = DEVMATCH_CLOSEST;
    matchOptions.iDeviceType = DEVMATCH_PRESERVE;
    matchOptions.iVertexProcessing = DEVMATCH_PRESERVE;
    matchOptions.iResolution = DEVMATCH_CLOSEST;
    matchOptions.iBackBufferFormat = DEVMATCH_CLOSEST;
    matchOptions.iBackBufferCount = DEVMATCH_PRESERVE;
    matchOptions.iWindowed = DEVMATCH_PRESERVE;
    matchOptions.iRefreshRate = DEVMATCH_PRESERVE;
    matchOptions.iDepthStencilFormat = DEVMATCH_CLOSEST;
    matchOptions.iMultiSample = DEVMATCH_CLOSEST;
    matchOptions.iPresentFlags = DEVMATCH_PRESERVE;
    matchOptions.iPresentInterval = DEVMATCH_PRESERVE;
    matchOptions.iSwapEffect = DEVMATCH_PRESERVE;

    // Match valid parameters
    D3D9DeviceParameters validParameters;
    Bool bOk = m_eDevices.MatchValidDevice( &validParameters, matchOptions, newParameters );
    DebugAssert( bOk );

    // Switch to valid parameters
    bOk = _D3D9_SwitchDevice( &validParameters, false );
    DebugAssert( bOk );
}

Void Direct3D9Renderer::SetCullingState( Bool bCullEnabled, CullingTest iCullTest, CullingConvention iCullConvention )
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes;
    if ( bCullEnabled ) {
        DWord dwCullMode;
        if ( iCullTest == CULLING_BACK )
            dwCullMode = Direct3D9CullOrder[1 - iCullConvention];
        else // CULLING_FRONT
            dwCullMode = Direct3D9CullOrder[iCullConvention];

        if ( m_CurrentRS.CullMode != dwCullMode ) {
            m_CurrentRS.CullMode = dwCullMode;
            hRes = m_pDevice->SetRenderState( D3DRS_CULLMODE, dwCullMode );
            DebugAssert( hRes == D3D_OK );
        }
    } else {
        if ( m_CurrentRS.CullMode != D3DCULL_NONE ) {
            m_CurrentRS.CullMode = D3DCULL_NONE;
            hRes = m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
            DebugAssert( hRes == D3D_OK );
        }
    }
}
Void Direct3D9Renderer::SetDepthState( Bool bDepthEnabled, Bool bDepthWritable, DepthTestMode iDepthTest )
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes;
    if ( bDepthEnabled ) {
        if ( m_CurrentRS.DepthEnable != D3DZB_TRUE ) {
            m_CurrentRS.DepthEnable = D3DZB_TRUE;
            hRes = m_pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
            DebugAssert( hRes == D3D_OK );
        }
        DWord dwDepthTest = Direct3D9DepthTest[iDepthTest];
        if ( m_CurrentRS.DepthTest != dwDepthTest ) {
            m_CurrentRS.DepthTest = dwDepthTest;
            hRes = m_pDevice->SetRenderState( D3DRS_ZFUNC, dwDepthTest );
            DebugAssert( hRes == D3D_OK );
        }
    } else {
        if ( m_CurrentRS.DepthEnable != D3DZB_FALSE ) {
            m_CurrentRS.DepthEnable = D3DZB_FALSE;
            hRes = m_pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
            DebugAssert( hRes == D3D_OK );
        }
    }

    if ( bDepthWritable ) {
        if ( m_CurrentRS.DepthWritable != TRUE ) {
            m_CurrentRS.DepthWritable = TRUE;
            hRes = m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
            DebugAssert( hRes == D3D_OK );
        }
    } else {
        if ( m_CurrentRS.DepthWritable != FALSE ) {
            m_CurrentRS.DepthWritable = FALSE;
            hRes = m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
            DebugAssert( hRes == D3D_OK );
        }
    }
}
Void Direct3D9Renderer::SetStencilState( Bool bStencilEnabled, StencilTestMode iStencilTest, StencilOperation iOnStencilFail,
                                         StencilOperation iOnDepthFail, StencilOperation iOnDepthSuccess,
                                         DWord dwStencilRef, DWord dwStencilMask, DWord dwStencilWriteMask )
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes;
    if ( bStencilEnabled ) {
        if ( m_CurrentRS.StencilEnable != TRUE ) {
            m_CurrentRS.StencilEnable = TRUE;
            hRes = m_pDevice->SetRenderState( D3DRS_STENCILENABLE, TRUE );
            DebugAssert( hRes == D3D_OK );
        }
        DWord dwStencilTest = Direct3D9StencilTest[iStencilTest];
        if ( m_CurrentRS.StencilTest != dwStencilTest ) {
            m_CurrentRS.StencilTest = dwStencilTest;
            hRes = m_pDevice->SetRenderState( D3DRS_STENCILFUNC, dwStencilTest );
            DebugAssert( hRes == D3D_OK );
        }
        DWord dwOnStencilFail = Direct3D9StencilOp[iOnStencilFail];
        if ( m_CurrentRS.StencilOnFail != dwOnStencilFail ) {
            m_CurrentRS.StencilOnFail = dwOnStencilFail;
            hRes = m_pDevice->SetRenderState( D3DRS_STENCILFAIL, dwOnStencilFail );
            DebugAssert( hRes == D3D_OK );
        }
        DWord dwOnDepthFail = Direct3D9StencilOp[iOnDepthFail];
        if ( m_CurrentRS.StencilOnDepthFail != dwOnDepthFail ) {
            m_CurrentRS.StencilOnDepthFail = dwOnDepthFail;
            hRes = m_pDevice->SetRenderState( D3DRS_STENCILZFAIL, dwOnDepthFail );
            DebugAssert( hRes == D3D_OK );
        }
        DWord dwOnDepthSuccess = Direct3D9StencilOp[iOnDepthSuccess];
        if ( m_CurrentRS.StencilOnDepthPass != dwOnDepthSuccess ) {
            m_CurrentRS.StencilOnDepthPass = dwOnDepthSuccess;
            hRes = m_pDevice->SetRenderState( D3DRS_STENCILPASS, dwOnDepthSuccess );
            DebugAssert( hRes == D3D_OK );
        }
        if ( m_CurrentRS.StencilRef != dwStencilRef ) {
            m_CurrentRS.StencilRef = dwStencilRef;
            hRes = m_pDevice->SetRenderState( D3DRS_STENCILREF, dwStencilRef );
            DebugAssert( hRes == D3D_OK );
        }
        if ( m_CurrentRS.StencilMask != dwStencilMask ) {
            m_CurrentRS.StencilMask = dwStencilMask;
            hRes = m_pDevice->SetRenderState( D3DRS_STENCILMASK, dwStencilMask );
            DebugAssert( hRes == D3D_OK );
        }
        if ( m_CurrentRS.StencilWriteMask != dwStencilWriteMask ) {
            m_CurrentRS.StencilWriteMask = dwStencilWriteMask;
            hRes = m_pDevice->SetRenderState( D3DRS_STENCILWRITEMASK, dwStencilWriteMask );
            DebugAssert( hRes == D3D_OK );
        }
    } else {
        if ( m_CurrentRS.StencilEnable != FALSE ) {
            m_CurrentRS.StencilEnable = FALSE;
            hRes = m_pDevice->SetRenderState( D3DRS_STENCILENABLE, FALSE );
            DebugAssert( hRes == D3D_OK );
        }
    }
}
Void Direct3D9Renderer::SetBlendingState( Bool bBlendEnabled, BlendingSrcMode iBlendSrc, BlendingDstMode iBlendDst, DWord dwConstColor,
                                          Bool bBlendTestEnabled, BlendingTestMode iBlendTest, DWord dwBlendTestRef )
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes;
    if ( bBlendEnabled ) {
        DWord dwBlendSrc = Direct3D9BlendSource[iBlendSrc];
        DWord dwSrcConstColor = 0;
        if ( dwBlendSrc == 0 ) {
            switch( iBlendSrc ) {
                case BLENDING_SRC_CONST_COLOR:        dwBlendSrc = D3DBLEND_BLENDFACTOR;    dwSrcConstColor = dwConstColor; break;
                case BLENDING_SRC_CONST_COLOR_INVERT: dwBlendSrc = D3DBLEND_INVBLENDFACTOR; dwSrcConstColor = dwConstColor; break;
                case BLENDING_SRC_CONST_ALPHA:        dwBlendSrc = D3DBLEND_BLENDFACTOR;    dwSrcConstColor = (dwConstColor & 0xff000000) >> 24;
                                                      dwSrcConstColor = D3DCOLOR_ARGB( dwSrcConstColor, dwSrcConstColor,
                                                                                       dwSrcConstColor, dwSrcConstColor ); break;
                case BLENDING_SRC_CONST_ALPHA_INVERT: dwBlendSrc = D3DBLEND_INVBLENDFACTOR; dwSrcConstColor = (dwConstColor & 0xff000000) >> 24;
                                                      dwSrcConstColor = D3DCOLOR_ARGB( dwSrcConstColor, dwSrcConstColor,
                                                                                       dwSrcConstColor, dwSrcConstColor ); break;
                default: DebugAssert( false ); break;
            }
        }
        DWord dwBlendDst = Direct3D9BlendDestination[iBlendDst];
        DWord dwDstConstColor = 0;
        if ( dwBlendDst == 0 ) {
            switch( iBlendDst ) {
                case BLENDING_DST_CONST_COLOR:        dwBlendDst = D3DBLEND_BLENDFACTOR;    dwDstConstColor = dwConstColor; break;
                case BLENDING_DST_CONST_COLOR_INVERT: dwBlendDst = D3DBLEND_INVBLENDFACTOR; dwDstConstColor = dwConstColor; break;
                case BLENDING_DST_CONST_ALPHA:        dwBlendDst = D3DBLEND_BLENDFACTOR;    dwDstConstColor = (dwConstColor & 0xff000000) >> 24;
                                                      dwDstConstColor = D3DCOLOR_ARGB( dwDstConstColor, dwDstConstColor,
                                                                                       dwDstConstColor, dwDstConstColor ); break;
                case BLENDING_DST_CONST_ALPHA_INVERT: dwBlendDst = D3DBLEND_INVBLENDFACTOR; dwDstConstColor = (dwConstColor & 0xff000000) >> 24;
                                                      dwDstConstColor = D3DCOLOR_ARGB( dwDstConstColor, dwDstConstColor,
                                                                                       dwDstConstColor, dwDstConstColor ); break;
                default: DebugAssert( false ); break;
            }
        }

        if ( dwBlendSrc == D3DBLEND_BLENDFACTOR || dwBlendSrc == D3DBLEND_INVBLENDFACTOR ) {
            if ( dwBlendDst == D3DBLEND_BLENDFACTOR || dwBlendDst == D3DBLEND_INVBLENDFACTOR ) {
                // Direct3D9 does not fully support const_alpha / const_alpha_invert
                DebugAssert( dwSrcConstColor == dwDstConstColor );
            }
        }

        if ( m_CurrentRS.BlendEnable != TRUE ) {
            m_CurrentRS.BlendEnable = TRUE;
            hRes = m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
            DebugAssert( hRes == D3D_OK );
        }
        if ( m_CurrentRS.BlendSrc != dwBlendSrc ) {
            m_CurrentRS.BlendSrc = dwBlendSrc;
            hRes = m_pDevice->SetRenderState( D3DRS_SRCBLEND, dwBlendSrc );
            DebugAssert( hRes == D3D_OK );
        }
        if ( m_CurrentRS.BlendDst != dwBlendDst ) {
            m_CurrentRS.BlendDst = dwBlendDst;
            hRes = m_pDevice->SetRenderState( D3DRS_DESTBLEND, dwBlendDst );
            DebugAssert( hRes == D3D_OK );
        }
        if ( m_CurrentRS.BlendFactor != dwSrcConstColor ) {
            m_CurrentRS.BlendFactor = dwSrcConstColor;
            hRes = m_pDevice->SetRenderState( D3DRS_BLENDFACTOR, dwSrcConstColor );
            DebugAssert( hRes == D3D_OK );
        }
    } else {
        if ( m_CurrentRS.BlendEnable != FALSE ) {
            m_CurrentRS.BlendEnable = FALSE;
            hRes = m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
            DebugAssert( hRes == D3D_OK );
        }
    }

    if ( bBlendTestEnabled ) {
        if ( m_CurrentRS.BlendTestEnable != TRUE ) {
            m_CurrentRS.BlendTestEnable = TRUE;
            hRes = m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
            DebugAssert( hRes == D3D_OK );
        }
        DWord dwBlendTest = Direct3D9BlendTest[iBlendTest];
        if ( m_CurrentRS.BlendTest != dwBlendTest ) {
            m_CurrentRS.BlendTest = dwBlendTest;
            hRes = m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, dwBlendTest );
            DebugAssert( hRes == D3D_OK );
        }
        if ( m_CurrentRS.BlendTestRef != dwBlendTestRef ) {
            m_CurrentRS.BlendTestRef = dwBlendTestRef;
            hRes = m_pDevice->SetRenderState( D3DRS_ALPHAREF, dwBlendTestRef );
            DebugAssert( hRes == D3D_OK );
        }
    } else {
        if ( m_CurrentRS.BlendTestEnable != FALSE ) {
            m_CurrentRS.BlendTestEnable = FALSE;
            hRes = m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
            DebugAssert( hRes == D3D_OK );
        }
    }
}
Void Direct3D9Renderer::SetPolygonState( Bool bWireframeEnabled, Bool bOffsetFills, Float fSlopeScale, Float fDepthBias )
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes;
    if ( bWireframeEnabled ) {
        if ( m_CurrentRS.FillMode != D3DFILL_WIREFRAME ) {
            m_CurrentRS.FillMode = D3DFILL_WIREFRAME;
            hRes = m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
            DebugAssert( hRes == D3D_OK );
        }
    } else {
        if ( m_CurrentRS.FillMode != D3DFILL_SOLID ) {
            m_CurrentRS.FillMode = D3DFILL_SOLID;
            hRes = m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
            DebugAssert( hRes == D3D_OK );
        }
    }

    // Direct3D9 does not support polygon offset for points or lines.
    if ( bOffsetFills ) {
        FloatConverter fConv;

        fConv.f = fSlopeScale;
        DWord dwSlopeScale = fConv.i;
        if ( m_CurrentRS.SlopeScaleDepthBias != dwSlopeScale ) {
            m_CurrentRS.SlopeScaleDepthBias = dwSlopeScale;
            hRes = m_pDevice->SetRenderState( D3DRS_SLOPESCALEDEPTHBIAS, dwSlopeScale );
            DebugAssert( hRes == D3D_OK );
        }
        // divisor = 2^n for n bits of precision in the depth buffer
        // For now, n is allways 24 (TEXTURE_FMT_D24S8).
        fConv.f = fDepthBias / 16777216.0f;
        DWord dwDepthBias = fConv.i;
        if ( m_CurrentRS.DepthBias != dwDepthBias ) {
            m_CurrentRS.DepthBias = dwDepthBias;
            hRes = m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, dwDepthBias );
            DebugAssert( hRes == D3D_OK );
        }
    } else {
        if ( m_CurrentRS.SlopeScaleDepthBias != 0 ) {
            m_CurrentRS.SlopeScaleDepthBias = 0;
            hRes = m_pDevice->SetRenderState( D3DRS_SLOPESCALEDEPTHBIAS, 0 );
            DebugAssert( hRes == D3D_OK );
        }
        if ( m_CurrentRS.DepthBias != 0 ) {
            m_CurrentRS.DepthBias = 0;
            hRes = m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, 0 );
            DebugAssert( hRes == D3D_OK );
        }
    }
}

Void Direct3D9Renderer::SetSamplerWrapState( UInt iSamplerUnit, ShaderSamplingType iType, const ShaderSamplingWrapMode * arrWrapMode,
                                                                                          DWord dwBorderColor )
{
    HRESULT hRes;

    D3D9SamplerState * pCurSS = NULL;
    if ( iSamplerUnit >= D3DVERTEXTEXTURESAMPLER0 ) {
        // Vertex sampler
        DebugAssert( (iSamplerUnit - D3DVERTEXTEXTURESAMPLER0) < m_iMaxVSSamplers );
        pCurSS = ( m_arrCurrentVSState + (iSamplerUnit - D3DVERTEXTEXTURESAMPLER0) );
    } else {
        // Pixel sampler
        DebugAssert( iSamplerUnit < m_iMaxPSSamplers );
        pCurSS = ( m_arrCurrentPSState + iSamplerUnit );
    }

    DWord dwWrap;
    switch( iType ) {
        case SHADERSAMPLING_TYPE_1D:
            dwWrap = Direct3D9WrapMode[ arrWrapMode[0] ];
            if ( pCurSS->Wrap[0] != dwWrap ) {
                pCurSS->Wrap[0] = dwWrap;
                hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_ADDRESSU, dwWrap );
                DebugAssert( hRes == D3D_OK );
            }
            if ( pCurSS->Wrap[1] != dwWrap ) {
                pCurSS->Wrap[1] = dwWrap;
                hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_ADDRESSV, dwWrap );
                DebugAssert( hRes == D3D_OK );
            }
            break;
        case SHADERSAMPLING_TYPE_2D:
            dwWrap = Direct3D9WrapMode[ arrWrapMode[0] ];
            if ( pCurSS->Wrap[0] != dwWrap ) {
                pCurSS->Wrap[0] = dwWrap;
                hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_ADDRESSU, dwWrap );
                DebugAssert( hRes == D3D_OK );
            }
            dwWrap = Direct3D9WrapMode[ arrWrapMode[1] ];
            if ( pCurSS->Wrap[1] != dwWrap ) {
                pCurSS->Wrap[1] = dwWrap;
                hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_ADDRESSV, dwWrap );
                DebugAssert( hRes == D3D_OK );
            }
            break;
        case SHADERSAMPLING_TYPE_3D:
            dwWrap = Direct3D9WrapMode[ arrWrapMode[0] ];
            if ( pCurSS->Wrap[0] != dwWrap ) {
                pCurSS->Wrap[0] = dwWrap;
                hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_ADDRESSU, dwWrap );
                DebugAssert( hRes == D3D_OK );
            }
            dwWrap = Direct3D9WrapMode[ arrWrapMode[1] ];
            if ( pCurSS->Wrap[1] != dwWrap ) {
                pCurSS->Wrap[1] = dwWrap;
                hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_ADDRESSV, dwWrap );
                DebugAssert( hRes == D3D_OK );
            }
            dwWrap = Direct3D9WrapMode[ arrWrapMode[2] ];
            if ( pCurSS->Wrap[2] != dwWrap ) {
                pCurSS->Wrap[2] = dwWrap;
                hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_ADDRESSW, dwWrap );
                DebugAssert( hRes == D3D_OK );
            }
            break;
        case SHADERSAMPLING_TYPE_CUBE:
            dwWrap = Direct3D9WrapMode[ arrWrapMode[0] ];
            if ( pCurSS->Wrap[0] != dwWrap ) {
                pCurSS->Wrap[0] = dwWrap;
                hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_ADDRESSU, dwWrap );
                DebugAssert( hRes == D3D_OK );
            }
            dwWrap = Direct3D9WrapMode[ arrWrapMode[1] ];
            if ( pCurSS->Wrap[1] != dwWrap ) {
                pCurSS->Wrap[1] = dwWrap;
                hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_ADDRESSV, dwWrap );
                DebugAssert( hRes == D3D_OK );
            }
            break;
        default: DebugAssert( false ); break;
    }

    D3DCOLOR d3dBorderColor = (D3DCOLOR)dwBorderColor;
    if ( pCurSS->BorderColor != d3dBorderColor ) {
        pCurSS->BorderColor = d3dBorderColor;
        hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_BORDERCOLOR, d3dBorderColor );
        DebugAssert( hRes == D3D_OK );
    }
}
Void Direct3D9Renderer::SetSamplerFilterState( UInt iSamplerUnit, ShaderSamplingFilter iFilter, Float fLODBias, Float fAnisotropy )
{
    HRESULT hRes;

    static const Float fMaxAnisotropy = (Float)SHADERPROG_MAX_ANISOTROPY;
    if ( fAnisotropy < 1.0f || fAnisotropy > fMaxAnisotropy )
        fAnisotropy = 1.0f;

    D3D9SamplerState * pCurSS = NULL;
    if ( iSamplerUnit >= D3DVERTEXTEXTURESAMPLER0 ) {
        // Vertex sampler
        DebugAssert( (iSamplerUnit - D3DVERTEXTEXTURESAMPLER0) < m_iMaxVSSamplers );
        pCurSS = ( m_arrCurrentVSState + (iSamplerUnit - D3DVERTEXTEXTURESAMPLER0) );
    } else {
        // Pixel sampler
        DebugAssert( iSamplerUnit < m_iMaxPSSamplers );
        pCurSS = ( m_arrCurrentPSState + iSamplerUnit );
    }

    if ( iFilter == SHADERSAMPLING_FILTER_NEAREST ) {
        if ( pCurSS->MagFilter != D3DTEXF_POINT ) {
            pCurSS->MagFilter = D3DTEXF_POINT;
            hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
            DebugAssert( hRes == D3D_OK );
        }
    } else {
        if ( pCurSS->MagFilter != D3DTEXF_LINEAR ) {
            pCurSS->MagFilter = D3DTEXF_LINEAR;
            hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
            DebugAssert( hRes == D3D_OK );
        }
    }
    if ( fAnisotropy > 1.0f && fAnisotropy <= fMaxAnisotropy ) {
        if ( pCurSS->MinFilter != D3DTEXF_ANISOTROPIC ) {
            pCurSS->MinFilter = D3DTEXF_ANISOTROPIC;
            hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );
            DebugAssert( hRes == D3D_OK );
        }
    } else {
        DWord dwMinFilter = Direct3D9MinFilter[iFilter];
        if ( pCurSS->MinFilter != dwMinFilter ) {
            pCurSS->MinFilter = dwMinFilter;
            hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_MINFILTER, dwMinFilter );
            DebugAssert( hRes == D3D_OK );
        }
    }
    DWord dwMipFilter = Direct3D9MipFilter[iFilter];
    if ( pCurSS->MipFilter != dwMipFilter ) {
        pCurSS->MipFilter = dwMipFilter;
        hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_MIPFILTER, dwMipFilter );
        DebugAssert( hRes == D3D_OK );
    }

    if ( pCurSS->LodBias != fLODBias ) {
        pCurSS->LodBias = fLODBias;
        hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_MIPMAPLODBIAS, *( (DWord*)&fLODBias ) );
        DebugAssert( hRes == D3D_OK );
    }

    if ( pCurSS->Anisotropy != fAnisotropy ) {
        pCurSS->Anisotropy = fAnisotropy;
        hRes = m_pDevice->SetSamplerState( iSamplerUnit, D3DSAMP_MAXANISOTROPY, (DWord)fAnisotropy );
        DebugAssert( hRes == D3D_OK );
    }
}

Void Direct3D9Renderer::GetViewport( UInt * outX, UInt * outY, UInt * outWidth, UInt * outHeight ) const
{
    DebugAssert( m_pDevice != NULL );

    // DirectX Viewport :
    // - Left-Hand coordinates
    // - Origin in Upper-Left corner
    // - Y-Axis points downward, X-Axis points rightward
    // => No transformation needed
    *outX = (UInt)( m_devViewport.X );
    *outY = (UInt)( m_devViewport.Y );
    *outWidth = (UInt)( m_devViewport.Width );
    *outHeight = (UInt)( m_devViewport.Height );
}
Void Direct3D9Renderer::GetDepthRange( Float * outDepthMin, Float * outDepthMax ) const
{
    DebugAssert( m_pDevice != NULL );

    *outDepthMin = m_devViewport.MinZ;
    *outDepthMax = m_devViewport.MaxZ;
}

Void Direct3D9Renderer::MoveViewport( UInt iX, UInt iY )
{
    DebugAssert( m_pDevice != NULL );

    m_devViewport.X = (DWord)( iX );
    m_devViewport.Y = (DWord)( iY );

    HRESULT hRes = m_pDevice->SetViewport( &m_devViewport );
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::ResizeViewport( UInt iWidth, UInt iHeight )
{
    DebugAssert( m_pDevice != NULL );

    m_devViewport.Width = (DWord)( iWidth );
    m_devViewport.Height = (DWord)( iHeight );

    HRESULT hRes = m_pDevice->SetViewport( &m_devViewport );
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::SetViewport( UInt iX, UInt iY, UInt iWidth, UInt iHeight )
{
    DebugAssert( m_pDevice != NULL );

    m_devViewport.X = (DWord)( iX );
    m_devViewport.Y = (DWord)( iY );
    m_devViewport.Width = (DWord)( iWidth );
    m_devViewport.Height = (DWord)( iHeight );

    HRESULT hRes = m_pDevice->SetViewport( &m_devViewport );
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::SetDepthRange( Float fDepthMin, Float fDepthMax )
{
    DebugAssert( m_pDevice != NULL );

    m_devViewport.MinZ = fDepthMin;
    m_devViewport.MaxZ = fDepthMax;

    HRESULT hRes = m_pDevice->SetViewport( &m_devViewport );
    DebugAssert( hRes == D3D_OK );
}

Void Direct3D9Renderer::ClearColorBuffer( DWord dwClearColor )
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes = m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, dwClearColor, 1.0f, 0 );
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::ClearDepthBuffer( Float fClearDepth )
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes = m_pDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, fClearDepth, 0 );
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::ClearStencilBuffer( DWord dwClearStencil )
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes = m_pDevice->Clear( 0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, dwClearStencil );
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::ClearBuffers( DWord dwClearColor, Float fClearDepth, DWord dwClearStencil )
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes = m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
                                     dwClearColor, fClearDepth, dwClearStencil );
    DebugAssert( hRes == D3D_OK );
}

Void Direct3D9Renderer::ClearColorBuffer( DWord dwClearColor, UInt iX, UInt iY, UInt iWidth, UInt iHeight )
{
    DebugAssert( m_pDevice != NULL );

    D3DRECT d3dRect;
    d3dRect.x1 = (LONG)iX;
    d3dRect.y1 = (LONG)iY;
    d3dRect.x2 = (LONG)(iX + iWidth - 1);
    d3dRect.y2 = (LONG)(iY + iHeight - 1);
    HRESULT hRes = m_pDevice->Clear( 1, &d3dRect, D3DCLEAR_TARGET, dwClearColor, 1.0f, 0 );
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::ClearDepthBuffer( Float fClearDepth, UInt iX, UInt iY, UInt iWidth, UInt iHeight )
{
    DebugAssert( m_pDevice != NULL );

    D3DRECT d3dRect;
    d3dRect.x1 = (LONG)iX;
    d3dRect.y1 = (LONG)iY;
    d3dRect.x2 = (LONG)(iX + iWidth - 1);
    d3dRect.y2 = (LONG)(iY + iHeight - 1);
    HRESULT hRes = m_pDevice->Clear( 1, &d3dRect, D3DCLEAR_ZBUFFER, 0, fClearDepth, 0 );
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::ClearStencilBuffer( DWord dwClearStencil, UInt iX, UInt iY, UInt iWidth, UInt iHeight )
{
    DebugAssert( m_pDevice != NULL );

    D3DRECT d3dRect;
    d3dRect.x1 = (LONG)iX;
    d3dRect.y1 = (LONG)iY;
    d3dRect.x2 = (LONG)(iX + iWidth - 1);
    d3dRect.y2 = (LONG)(iY + iHeight - 1);
    HRESULT hRes = m_pDevice->Clear( 1, &d3dRect, D3DCLEAR_STENCIL, 0, 1.0f, dwClearStencil );
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::ClearBuffers( DWord dwClearColor, Float fClearDepth, DWord dwClearStencil,
                                      UInt iX, UInt iY, UInt iWidth, UInt iHeight )
{
    DebugAssert( m_pDevice != NULL );

    D3DRECT d3dRect;
    d3dRect.x1 = (LONG)iX;
    d3dRect.y1 = (LONG)iY;
    d3dRect.x2 = (LONG)(iX + iWidth - 1);
    d3dRect.y2 = (LONG)(iY + iHeight - 1);
    HRESULT hRes = m_pDevice->Clear( 1, &d3dRect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
                                     dwClearColor, fClearDepth, dwClearStencil );
    DebugAssert( hRes == D3D_OK );
}

Void Direct3D9Renderer::SetColorMask( Bool bAllowRed, Bool bAllowGreen, Bool bAllowBlue, Bool bAllowAlpha )
{
    DebugAssert( m_pDevice != NULL );

    DWord dwMask = 0;
    if (bAllowRed)   dwMask |= D3DCOLORWRITEENABLE_RED;
    if (bAllowGreen) dwMask |= D3DCOLORWRITEENABLE_GREEN;
    if (bAllowBlue)  dwMask |= D3DCOLORWRITEENABLE_BLUE;
    if (bAllowAlpha) dwMask |= D3DCOLORWRITEENABLE_ALPHA;
    HRESULT hRes = m_pDevice->SetRenderState( D3DRS_COLORWRITEENABLE, dwMask );
    DebugAssert( hRes == D3D_OK );
}

Bool Direct3D9Renderer::QueryRendering()
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes = m_pDevice->TestCooperativeLevel();

    // Device OK case
    if ( hRes == D3D_OK ) {
        m_devState = D3D9DEVICE_OK;
        return true;
    }

    // Device LOST-Wait case
    if ( hRes == D3DERR_DEVICELOST ) {
        if ( m_devState == D3D9DEVICE_OK ) {
            // Just noticed LOST
            _DestroyResources();
        }
        m_devState = D3D9DEVICE_LOST;
        return false;
    }

    // Device LOST-Reset case
    if ( hRes == D3DERR_DEVICENOTRESET ) {
        if ( m_devState == D3D9DEVICE_OK ) {
            // Just noticed LOST
            _DestroyResources();
        }
        m_devState = D3D9DEVICE_LOST;

        // Reset the device
        D3D9DeviceParameters curParameters;
        MemCopy( &curParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
        _D3D9_ResetDevice( &curParameters );

        // Reset OK case
        if ( m_devState == D3D9DEVICE_OK ) {
            _RecreateResources();
            return true;
        }

        // Still LOST case
        if ( m_devState == D3D9DEVICE_LOST )
            return false;

        // Reset FAILED case
        Bool bOk = _D3D9_SwitchDevice( &curParameters, true );
        DebugAssert( bOk );
        return bOk;
    }

    // All remaining D3DERR_* cases
    if ( m_devState == D3D9DEVICE_OK ) {
        // Just noticed error
        _DestroyResources();
    }
    m_devState = D3D9DEVICE_RESETFAILED;

    // Try matching desktop format
    if ( !(m_pWindow->IsFullScreen()) ) {
        D3DDISPLAYMODE d3dAdapterDesktopDisplayMode;
        hRes = m_pDriver->GetAdapterDisplayMode( m_devParameters.iAdapterOrdinal, &d3dAdapterDesktopDisplayMode );
        DebugAssert( hRes == D3D_OK );
        if ( m_devParameters.d3dAdapterFormat != d3dAdapterDesktopDisplayMode.Format ) {
            D3D9DeviceMatchOptions matchOptions;
            matchOptions.iAdapterOrdinal = DEVMATCH_PRESERVE;
            matchOptions.iAdapterFormat = DEVMATCH_CLOSEST;
            matchOptions.iDeviceType = DEVMATCH_PRESERVE;
            matchOptions.iVertexProcessing = DEVMATCH_PRESERVE;
            matchOptions.iResolution = DEVMATCH_CLOSEST;
            matchOptions.iBackBufferFormat = DEVMATCH_CLOSEST;
            matchOptions.iBackBufferCount = DEVMATCH_PRESERVE;
            matchOptions.iWindowed = DEVMATCH_PRESERVE;
            matchOptions.iRefreshRate = DEVMATCH_PRESERVE;
            matchOptions.iDepthStencilFormat = DEVMATCH_PRESERVE;
            matchOptions.iMultiSample = DEVMATCH_PRESERVE;
            matchOptions.iPresentFlags = DEVMATCH_PRESERVE;
            matchOptions.iPresentInterval = DEVMATCH_PRESERVE;
            matchOptions.iSwapEffect = DEVMATCH_PRESERVE;

            D3D9DeviceParameters bestParameters;
            MemCopy( &bestParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
            bestParameters.d3dAdapterFormat = d3dAdapterDesktopDisplayMode.Format;
            bestParameters.d3dPresentParams.BackBufferFormat = d3dAdapterDesktopDisplayMode.Format;

            D3D9DeviceParameters validParameters;
            Bool bOk = m_eDevices.MatchValidDevice( &validParameters, matchOptions, bestParameters );
            DebugAssert( bOk );

            bOk = _D3D9_SwitchDevice( &validParameters, false );
            DebugAssert( bOk );
            return bOk;
        }
    }

    // Reset the device
    D3D9DeviceParameters curParameters;
    MemCopy( &curParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );
    _D3D9_ResetDevice( &curParameters );

    // Reset OK case
    if ( m_devState == D3D9DEVICE_OK ) {
        _RecreateResources();
        return true;
    }

    // Still LOST case
    if ( m_devState == D3D9DEVICE_LOST )
        return false;

    // Reset FAILED case
    Bool bOk = _D3D9_SwitchDevice( &curParameters, true );
    DebugAssert( bOk );
    return bOk;
}
Void Direct3D9Renderer::PresentBackBuffer()
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes = m_pDevice->Present( NULL, NULL, NULL, NULL );

    // Device OK case
    if ( hRes == D3D_OK )
        return;

    // Device LOST case
    if ( hRes == D3DERR_DEVICELOST ) {
        if ( m_devState == D3D9DEVICE_OK ) {
            // Just noticed LOST
            _DestroyResources();
        }
        m_devState = D3D9DEVICE_LOST;
        return;
    }

    // D3DERR_DEVICEREMOVED case
    _D3D9_DestroyDevice();
    DebugAssert( false );
}

Void Direct3D9Renderer::DrawBegin()
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes = m_pDevice->BeginScene();
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::DrawEnd()
{
    DebugAssert( m_pDevice != NULL );

    HRESULT hRes = m_pDevice->EndScene();
    DebugAssert( hRes == D3D_OK );
}

Void Direct3D9Renderer::DrawPrimitive( MeshType iPrimitiveType, UInt iPrimitiveCount, UInt iVertexCount,
                                       Bool bIndexed, UInt iVertexOffset, UInt iIndexOffset )
{
    DebugAssert( m_pDevice != NULL );

    if ( iVertexCount == 0 || iPrimitiveCount == 0 )
        return;

    D3DPRIMITIVETYPE d3dPrimitiveType = Direct3D9PrimitiveType[iPrimitiveType];

    HRESULT hRes;
    if ( bIndexed )
        hRes = m_pDevice->DrawIndexedPrimitive( d3dPrimitiveType, 0, iVertexOffset, iVertexCount,
                                                iIndexOffset, iPrimitiveCount );
    else
        hRes = m_pDevice->DrawPrimitive( d3dPrimitiveType, iVertexOffset, iPrimitiveCount );
    DebugAssert( hRes == D3D_OK );
}

Void Direct3D9Renderer::SetCursorImage( UInt iWidth, UInt iHeight, const DWord * pImageARGB,
                                        UInt iHotSpotX, UInt iHotSpotY )
{
    DebugAssert( m_pDevice != NULL );

    if ( pImageARGB == NULL ) {
        _D3D9_SetWindowCursor();
        return;
    }

    m_iCursorWidth = iWidth;
    m_iCursorHeight = iHeight;
    m_pCursorImage = pImageARGB;
    m_iCursorHotSpotX = iHotSpotX;
    m_iCursorHotSpotY = iHotSpotY;
    _D3D9_CreateCursor();
}
Void Direct3D9Renderer::ShowCursor( Bool bShow )
{
    DebugAssert( m_pDevice != NULL );
    m_pDevice->ShowCursor( (bShow) ? TRUE : FALSE );
}
Void Direct3D9Renderer::WarpCursor( UInt iX, UInt iY )
{
    DebugAssert( m_pDevice != NULL );
    m_pDevice->SetCursorPosition( iX, iY, 0 );
}

/////////////////////////////////////////////////////////////////////////////////

Bool Direct3D9Renderer::_AcceptMinimalDevice( D3DCAPS9 * pCaps, D3DFORMAT /*d3dAdapterFormat*/,
                                              D3DFORMAT /*d3dBackBufferFormat*/, Bool /*bWindowed*/ )
{
    // Default adapter
    if ( pCaps->AdapterOrdinal != D3DADAPTER_DEFAULT )
        return false;

    // HAL Device
    if ( pCaps->DeviceType != D3DDEVTYPE_HAL )
        return false;

    // Hardware T&L
    if ( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 )
        return false;

    // Pure device
    if ( (pCaps->DevCaps & D3DDEVCAPS_PUREDEVICE) == 0 )
        return false;

    // Minimal VS 3.0
    if ( pCaps->VertexShaderVersion < D3DVS_VERSION(3,0) )
        return false;

    // Minimal PS 3.0
    if ( pCaps->PixelShaderVersion < D3DPS_VERSION(3,0) )
        return false;

    return true;
}

UInt Direct3D9Renderer::_D3D9_AdapterFromMonitor( HMONITOR hMonitor ) const
{
    for( UInt iAdapter = 0; iAdapter < m_eDevices.GetAdapterCount(); ++iAdapter ) {
        const D3D9AdapterInfo * pAdapter = m_eDevices.GetAdapter( iAdapter );
        HMONITOR hCurMonitor = m_pDriver->GetAdapterMonitor( pAdapter->iAdapterOrdinal );
        if ( hCurMonitor == hMonitor )
            return pAdapter->iAdapterOrdinal;
    }
    return INVALID_OFFSET;
}

Void Direct3D9Renderer::_D3D9_CreateDevice( D3D9DeviceParameters * pParameters )
{
    DebugAssert( m_pDevice == NULL );

    // Create the device
    HRESULT hRes = m_pDriver->CreateDevice (
        pParameters->iAdapterOrdinal, pParameters->d3dDeviceType, m_pWindow->_GetHWND(),
        pParameters->dwBehaviour, &(pParameters->d3dPresentParams), &m_pDevice
    );

    // Update state
    if ( hRes == D3D_OK )
        m_devState = D3D9DEVICE_OK;
    else if ( hRes == D3DERR_DEVICELOST ) {
        m_devState = D3D9DEVICE_LOST;
    } else {
        m_devState = D3D9DEVICE_CREATEFAILED;
        DebugAssert( false );
        return;
    }

    // Save device parameters
    MemCopy( &m_devParameters, pParameters, sizeof(D3D9DeviceParameters) );

    // Save caps
    hRes = m_pDevice->GetDeviceCaps( &m_devCaps );
    DebugAssert( hRes == D3D_OK );

    // Save backbuffer's descriptor
    IDirect3DSurface9 * pBackBuffer = NULL;
    hRes = m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    DebugAssert( hRes == D3D_OK );

    MemZero( &m_devBackBufferDesc, sizeof(D3DSURFACE_DESC) );
    hRes = pBackBuffer->GetDesc( &m_devBackBufferDesc );
    DebugAssert( hRes == D3D_OK );

    pBackBuffer->Release();
    pBackBuffer = NULL;

    // Default viewport
    m_devViewport.X = 0;
    m_devViewport.Y = 0;
    m_devViewport.Width = m_devBackBufferDesc.Width;
    m_devViewport.Height = m_devBackBufferDesc.Height;
    m_devViewport.MinZ = 0.0f;
    m_devViewport.MaxZ = 1.0f;
    hRes = m_pDevice->SetViewport( &m_devViewport );
    DebugAssert( hRes == D3D_OK );

    // Update shaders
    _UpdateShaderVersions();

    // RenderStates & SamplerStates
    _ApplyDefaultRenderStates();
    _ApplyDefaultSamplerStates();
}
Void Direct3D9Renderer::_D3D9_ResetDevice( D3D9DeviceParameters * pParameters )
{
    DebugAssert( m_pDevice != NULL );

    // Reset the device
    HRESULT hRes = m_pDevice->Reset( &(pParameters->d3dPresentParams) );

    // Update state
    if ( hRes == D3D_OK )
        m_devState = D3D9DEVICE_OK;
    else if ( hRes == D3DERR_DEVICELOST ) {
        m_devState = D3D9DEVICE_LOST;
    } else {
        m_devState = D3D9DEVICE_RESETFAILED;
        DebugAssert( false );
        return;
    }

    // Update device parameters
    MemCopy( &m_devParameters, pParameters, sizeof(D3D9DeviceParameters) );

    // Update caps (not needed)
    hRes = m_pDevice->GetDeviceCaps( &m_devCaps );
    DebugAssert( hRes == D3D_OK );

    // Update backbuffer's descriptor
    IDirect3DSurface9 * pBackBuffer = NULL;
    hRes = m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    DebugAssert( hRes == D3D_OK );

    MemZero( &m_devBackBufferDesc, sizeof(D3DSURFACE_DESC) );
    hRes = pBackBuffer->GetDesc( &m_devBackBufferDesc );
    DebugAssert( hRes == D3D_OK );

    pBackBuffer->Release();
    pBackBuffer = NULL;

    // Default viewport
    m_devViewport.X = 0;
    m_devViewport.Y = 0;
    m_devViewport.Width = m_devBackBufferDesc.Width;
    m_devViewport.Height = m_devBackBufferDesc.Height;
    m_devViewport.MinZ = 0.0f;
    m_devViewport.MaxZ = 1.0f;
    hRes = m_pDevice->SetViewport( &m_devViewport );
    DebugAssert( hRes == D3D_OK );

    // Update shaders (not needed)
    _UpdateShaderVersions();

    // RenderStates & SamplerStates
    _ApplyDefaultRenderStates();
    _ApplyDefaultSamplerStates();
}
Void Direct3D9Renderer::_D3D9_DestroyDevice()
{
    DebugAssert( m_pDevice != NULL );

    // Destroy the device
    m_pDevice->Release();
    m_pDevice = NULL;

    // Update state
    m_devState = D3D9DEVICE_NULL;

    // Erase device parameters
    MemZero( &m_devParameters, sizeof(D3D9DeviceParameters) );

    // Erase caps
    MemZero( &m_devCaps, sizeof(D3DCAPS9) );

    // Erase backbuffer descriptor
    MemZero( &m_devBackBufferDesc, sizeof(D3DSURFACE_DESC) );

    // Erase viewport
    MemZero( &m_devViewport, sizeof(D3DVIEWPORT9) );

    // Update shaders
    _UpdateShaderVersions();
}
Bool Direct3D9Renderer::_D3D9_SwitchDevice( D3D9DeviceParameters * pNewParameters, Bool bForceRecreate )
{
    // Save old device parameters
    D3D9DeviceParameters oldParameters;
    MemCopy( &oldParameters, &m_devParameters, sizeof(D3D9DeviceParameters) );

    // Check some stuff beforehand
    Bool bDeviceExisted = ( m_pDevice != NULL );
    Bool bOldIsWindowed = false;
    UInt iOldWidth = 0;
    UInt iOldHeight = 0;
    if ( bDeviceExisted ) {
        bOldIsWindowed = ( oldParameters.d3dPresentParams.Windowed != FALSE );
        iOldWidth = oldParameters.d3dPresentParams.BackBufferWidth;
        iOldHeight = oldParameters.d3dPresentParams.BackBufferHeight;
    }
    Bool bNewIsWindowed = ( pNewParameters->d3dPresentParams.Windowed != FALSE );
    UInt iNewWidth = pNewParameters->d3dPresentParams.BackBufferWidth;
    UInt iNewHeight = pNewParameters->d3dPresentParams.BackBufferHeight;
    Bool bKeepWindowedSize = false;
    if ( iNewWidth == 0 && iNewHeight == 0 ) {
        RECT rectClient;
        GetClientRect( m_pWindow->_GetHWND(), &rectClient );
        iNewWidth = ( rectClient.right - rectClient.left );
        iNewHeight = ( rectClient.bottom - rectClient.top );
        pNewParameters->d3dPresentParams.BackBufferWidth = iNewWidth;
        pNewParameters->d3dPresentParams.BackBufferHeight = iNewHeight;
        bKeepWindowedSize = true;
    }

    // Destroy resources
    if ( bDeviceExisted )
        _DestroyResources();

    // Prepare the window for display switch
    m_pWindow->_SwitchDisplay_Prepare( bNewIsWindowed, bDeviceExisted,
                                       bOldIsWindowed, iOldWidth, iOldHeight );

    // Reset / Recreate
    if ( bDeviceExisted && !bForceRecreate && _D3D9_CanResetDevice(*pNewParameters, oldParameters) ) {
        // Try to reset
        _D3D9_ResetDevice( pNewParameters );

        // Failed, try to recreate
        if ( m_devState != D3D9DEVICE_OK && m_devState != D3D9DEVICE_LOST ) {
            m_pWindow->_SwitchDisplay_Abort( bNewIsWindowed, bDeviceExisted, bOldIsWindowed );

            return _D3D9_SwitchDevice( pNewParameters, true );
        }
    } else {
        // Try to recreate
        if ( bDeviceExisted )
            _D3D9_DestroyDevice();
        _D3D9_CreateDevice( pNewParameters );

        // Failed
        if ( m_devState != D3D9DEVICE_OK && m_devState != D3D9DEVICE_LOST ) {
            m_pWindow->_SwitchDisplay_Abort( bNewIsWindowed, bDeviceExisted, bOldIsWindowed );

            _D3D9_DestroyDevice();
            return false;
        }
    }

    // Update backbuffer size who might have changed
    iNewWidth = pNewParameters->d3dPresentParams.BackBufferWidth;
    iNewHeight = pNewParameters->d3dPresentParams.BackBufferHeight;

    // Finalize
    HMONITOR hNewAdapterMonitor = m_pDriver->GetAdapterMonitor( pNewParameters->iAdapterOrdinal );
    Bool bRetry = m_pWindow->_SwitchDisplay_Finalize ( bNewIsWindowed, bDeviceExisted, bOldIsWindowed, bKeepWindowedSize,
                                                       iNewWidth, iNewHeight, hNewAdapterMonitor );

    // Retry if asked
    if ( bRetry ) {
        pNewParameters->d3dPresentParams.BackBufferWidth = 0;
        pNewParameters->d3dPresentParams.BackBufferHeight = 0;

        return _D3D9_SwitchDevice( pNewParameters, false );
    }

    // Rebuild resources
    if ( bDeviceExisted )
        _RecreateResources();

    // Update cursor state
    _D3D9_UpdateCursorState();

    // Success
    return true;
}

Void Direct3D9Renderer::_D3D9_SetWindowCursor()
{
    static COLORREF s_arrMasks[1024];
    static COLORREF s_arrColors[1024];
    static DWord s_pImageARGB[1024];

    BOOL bRes;
    Int iRes;

    HCURSOR hCursor = m_pWindow->_GetHCURSOR();
    DebugAssert( hCursor != NULL );

    // Cursor infos
    ICONINFO iconInfos;
    MemZero( &iconInfos, sizeof(ICONINFO) );
    bRes = GetIconInfo( hCursor, &iconInfos );
    DebugAssert( bRes == TRUE );
    BITMAP iconBMP;
    iRes = GetObject( (HGDIOBJ)( iconInfos.hbmMask ), sizeof(BITMAP), (Void*)( &iconBMP ) );
    DebugAssert( iRes != 0 );
    Bool bMonochrome = ( iconInfos.hbmColor == NULL );
    DWord iCursorWidth = iconBMP.bmWidth;
    DWord iCursorHeight = iconBMP.bmHeight;
    DWord iTargetHeight = iCursorHeight;
    if ( bMonochrome )
        iTargetHeight = (iCursorHeight >> 1);
    DebugAssert( iCursorWidth <= 32 && iTargetHeight <= 32 );

    // Bitmap infos
    BITMAPINFO iconBMPInfo;
    MemZero( &iconBMPInfo, sizeof(BITMAPINFO) );
    iconBMPInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    iconBMPInfo.bmiHeader.biWidth = iCursorWidth;
    iconBMPInfo.bmiHeader.biHeight = iCursorHeight;
    iconBMPInfo.bmiHeader.biPlanes = 1;
    iconBMPInfo.bmiHeader.biBitCount = 32;
    iconBMPInfo.bmiHeader.biCompression = BI_RGB;

    HDC hScreenDC = GetDC( NULL );
    HGDIOBJ hOldGDIObject = NULL;

    // Get Mask
    HDC hMaskDC = CreateCompatibleDC(hScreenDC);
    DebugAssert( hMaskDC != NULL );
    hOldGDIObject = SelectObject( hMaskDC, iconInfos.hbmMask );
    GetDIBits( hMaskDC, iconInfos.hbmMask, 0, iCursorHeight,
               s_arrMasks, &iconBMPInfo, DIB_RGB_COLORS );
    SelectObject( hMaskDC, hOldGDIObject );
    DeleteObject( iconInfos.hbmMask );
    DeleteDC( hMaskDC );

    // Get Colors
    if ( !bMonochrome ) {
        HDC hColorDC = CreateCompatibleDC(hScreenDC);
        DebugAssert( hColorDC != NULL );
        hOldGDIObject = SelectObject( hColorDC, iconInfos.hbmColor );
        GetDIBits( hColorDC, iconInfos.hbmColor, 0, iCursorHeight,
                   s_arrColors, &iconBMPInfo, DIB_RGB_COLORS );
        SelectObject( hColorDC, hOldGDIObject );
        DeleteObject( iconInfos.hbmColor );
        DeleteDC( hColorDC );
    }

    ReleaseDC( NULL, hScreenDC );

    // Convert to ARGB & re-order scanlines
    COLORREF crMask, crColor;
    for( UInt iY = 0; iY < iTargetHeight; ++iY ) {
        for( UInt iX = 0; iX < iCursorWidth; ++iX ) {
            if ( bMonochrome ) {
                crMask = s_arrMasks[( iCursorHeight - 1 - iY ) * iCursorWidth + iX];
                crColor = s_arrMasks[( iTargetHeight - 1 - iY ) * iCursorWidth + iX];
            } else {
                crMask = s_arrMasks[( iTargetHeight - 1 - iY ) * iCursorWidth + iX];
                crColor = s_arrColors[( iTargetHeight - 1 - iY ) * iCursorWidth + iX];
            }
            if ( crMask == 0 )
                s_pImageARGB[iY * iCursorWidth + iX] = ( crColor | 0xff000000 );
            else
                s_pImageARGB[iY * iCursorWidth + iX] = 0;
        }
    }

    // Setup Cursor
    m_iCursorWidth = iCursorWidth;
    m_iCursorHeight = iTargetHeight;
    m_pCursorImage = s_pImageARGB;
    m_iCursorHotSpotX = iconInfos.xHotspot;
    m_iCursorHotSpotY = iconInfos.yHotspot;
    _D3D9_CreateCursor();
}
Void Direct3D9Renderer::_D3D9_CreateCursor()
{
    IDirect3DSurface9 * pCursorSurface = NULL;
    HRESULT hRes = m_pDevice->CreateOffscreenPlainSurface( m_iCursorWidth, m_iCursorHeight, D3DFMT_A8R8G8B8,
                                                           D3DPOOL_SCRATCH, &pCursorSurface, NULL );
    DebugAssert( hRes == D3D_OK );
    
    D3DLOCKED_RECT lockRect;
    pCursorSurface->LockRect( &lockRect, NULL, 0 );

    MemCopy( lockRect.pBits, m_pCursorImage, m_iCursorWidth * m_iCursorHeight * sizeof(DWord) );

    // 11.. 11.. 11.. CCC
    // 1.1. ..1. 1.1. A2A
    // 1.1. .1.. 1.1. A4A
    // 1.1. ..1. 1.1. A2A
    // 11.. 11.. 11.. CCC
    //const WORD wMask[5] = { 0x0ccc, 0x0a2a, 0x0a4a, 0x0a2a, 0x0ccc };
    //DWord * pBitmap = (DWord*)( lockRect.pBits );
    //for( UInt iY = 0; iY < 5; ++iY ) {
    //    for( UInt iX = 0; iX < 12; ++iX ) {
    //        if ( wMask[iY] & (1 << (11 - iX)) )
    //            pBitmap[iY * lockRect.Pitch + iX] = 0xffffffff;
    //    }
    //}

    pCursorSurface->UnlockRect();

    hRes = m_pDevice->SetCursorProperties( m_iCursorHotSpotX, m_iCursorHotSpotY, pCursorSurface );
    DebugAssert( hRes == D3D_OK );

    pCursorSurface->Release();
}
Void Direct3D9Renderer::_D3D9_UpdateCursorState()
{
    if ( m_devParameters.d3dPresentParams.Windowed == FALSE ) {
        m_pWindow->ClipCursor( true );
        m_pWindow->ShowCursor( false );
        m_pDevice->ShowCursor( TRUE );
    } else {
        m_pWindow->ClipCursor( false );
        m_pWindow->ShowCursor( true );
        m_pDevice->ShowCursor( FALSE );        
    }
}

Void Direct3D9Renderer::_ApplyDefaultRenderStates()
{
    HRESULT hRes;

    // ClippingState
    //////////////////////////////////////////////

    // CullingState
    m_CurrentRS.CullMode = D3DCULL_CW;
    hRes = m_pDevice->SetRenderState( D3DRS_CULLMODE, m_CurrentRS.CullMode );
    DebugAssert( hRes == D3D_OK );

    // DepthState
    m_CurrentRS.DepthEnable = D3DZB_TRUE;
    m_CurrentRS.DepthTest = D3DCMP_LESSEQUAL;
    m_CurrentRS.DepthWritable = TRUE;
    hRes = m_pDevice->SetRenderState( D3DRS_ZENABLE, m_CurrentRS.DepthEnable );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_ZFUNC, m_CurrentRS.DepthTest );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, m_CurrentRS.DepthWritable );
    DebugAssert( hRes == D3D_OK );

    // StencilState
    m_CurrentRS.StencilEnable = FALSE;
    m_CurrentRS.StencilTest = D3DCMP_NEVER;
    m_CurrentRS.StencilOnFail = D3DSTENCILOP_KEEP;
    m_CurrentRS.StencilOnDepthFail = D3DSTENCILOP_KEEP;
    m_CurrentRS.StencilOnDepthPass = D3DSTENCILOP_KEEP;
    m_CurrentRS.StencilRef = 0;
    m_CurrentRS.StencilMask = 0xffffffff;
    m_CurrentRS.StencilWriteMask = 0xffffffff;
    hRes = m_pDevice->SetRenderState( D3DRS_STENCILENABLE, m_CurrentRS.StencilEnable );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_STENCILFUNC, m_CurrentRS.StencilTest );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_STENCILFAIL, m_CurrentRS.StencilOnFail );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_STENCILZFAIL, m_CurrentRS.StencilOnDepthFail );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_STENCILPASS, m_CurrentRS.StencilOnDepthPass );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_STENCILREF, m_CurrentRS.StencilRef );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_STENCILMASK, m_CurrentRS.StencilMask );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_STENCILWRITEMASK, m_CurrentRS.StencilWriteMask );
    DebugAssert( hRes == D3D_OK );

    // BlendingState
    m_CurrentRS.BlendEnable = FALSE;
    m_CurrentRS.BlendSrc = D3DBLEND_SRCALPHA;
    m_CurrentRS.BlendDst = D3DBLEND_INVSRCALPHA;
    m_CurrentRS.BlendTestEnable = FALSE;
    m_CurrentRS.BlendTest = D3DCMP_ALWAYS;
    m_CurrentRS.BlendTestRef = 0;
    m_CurrentRS.BlendFactor = D3DCOLOR_ARGB( 0, 0, 0, 0 );
    hRes = m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, m_CurrentRS.BlendEnable );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_SRCBLEND, m_CurrentRS.BlendSrc );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_DESTBLEND, m_CurrentRS.BlendDst );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, m_CurrentRS.BlendTestEnable );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, m_CurrentRS.BlendTest );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_ALPHAREF, m_CurrentRS.BlendTestRef );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_BLENDFACTOR, m_CurrentRS.BlendFactor );
    DebugAssert( hRes == D3D_OK );

    // PolygonState
    m_CurrentRS.FillMode = D3DFILL_SOLID;
    m_CurrentRS.SlopeScaleDepthBias = 0;
    m_CurrentRS.DepthBias = 0;
    hRes = m_pDevice->SetRenderState( D3DRS_FILLMODE, m_CurrentRS.FillMode );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_SLOPESCALEDEPTHBIAS, m_CurrentRS.SlopeScaleDepthBias );
    DebugAssert( hRes == D3D_OK );
    hRes = m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, m_CurrentRS.DepthBias );
    DebugAssert( hRes == D3D_OK );
}
Void Direct3D9Renderer::_ApplyDefaultSamplerStates()
{
    DWord i;
    HRESULT hRes;
    for( i = 0; i < DIRECT3D9_MAX_VERTEX_SAMPLERS; ++i ) {
        D3D9SamplerState & rState = m_arrCurrentVSState[i];
        rState.Wrap[0] = D3DTADDRESS_WRAP;
        rState.Wrap[1] = D3DTADDRESS_WRAP;
        rState.Wrap[2] = D3DTADDRESS_WRAP;
        rState.MagFilter = D3DTEXF_POINT;
        rState.MinFilter = D3DTEXF_POINT;
        rState.MipFilter = D3DTEXF_NONE;
        rState.Anisotropy = 1.0f;
        rState.LodBias = 0.0f;
        rState.BorderColor = 0;

        hRes = m_pDevice->SetSamplerState( D3DVERTEXTEXTURESAMPLER0 + i, D3DSAMP_ADDRESSU, rState.Wrap[0] );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( D3DVERTEXTEXTURESAMPLER0 + i, D3DSAMP_ADDRESSV, rState.Wrap[1] );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( D3DVERTEXTEXTURESAMPLER0 + i, D3DSAMP_ADDRESSW, rState.Wrap[2] );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( D3DVERTEXTEXTURESAMPLER0 + i, D3DSAMP_MAGFILTER, rState.MagFilter );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( D3DVERTEXTEXTURESAMPLER0 + i, D3DSAMP_MINFILTER, rState.MinFilter );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( D3DVERTEXTEXTURESAMPLER0 + i, D3DSAMP_MIPFILTER, rState.MipFilter );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( D3DVERTEXTEXTURESAMPLER0 + i, D3DSAMP_MAXANISOTROPY, (DWord)( rState.Anisotropy ) );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( D3DVERTEXTEXTURESAMPLER0 + i, D3DSAMP_MIPMAPLODBIAS, *( (DWord*)&(rState.LodBias) ) );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( D3DVERTEXTEXTURESAMPLER0 + i, D3DSAMP_BORDERCOLOR, rState.BorderColor );
        DebugAssert( hRes == D3D_OK );
    }
    for( i = 0; i < DIRECT3D9_MAX_PIXEL_SAMPLERS; ++i ) {
        D3D9SamplerState & rState = m_arrCurrentPSState[i];
        rState.Wrap[0] = D3DTADDRESS_WRAP;
        rState.Wrap[1] = D3DTADDRESS_WRAP;
        rState.Wrap[2] = D3DTADDRESS_WRAP;
        rState.MagFilter = D3DTEXF_POINT;
        rState.MinFilter = D3DTEXF_POINT;
        rState.MipFilter = D3DTEXF_NONE;
        rState.Anisotropy = 1.0f;
        rState.LodBias = 0.0f;
        rState.BorderColor = 0;

        hRes = m_pDevice->SetSamplerState( i, D3DSAMP_ADDRESSU, rState.Wrap[0] );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( i, D3DSAMP_ADDRESSV, rState.Wrap[1] );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( i, D3DSAMP_ADDRESSW, rState.Wrap[2] );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( i, D3DSAMP_MAGFILTER, rState.MagFilter );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( i, D3DSAMP_MINFILTER, rState.MinFilter );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( i, D3DSAMP_MIPFILTER, rState.MipFilter );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( i, D3DSAMP_MAXANISOTROPY, (DWord)( rState.Anisotropy ) );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( i, D3DSAMP_MIPMAPLODBIAS, *( (DWord*)&(rState.LodBias) ) );
        DebugAssert( hRes == D3D_OK );
        hRes = m_pDevice->SetSamplerState( i, D3DSAMP_BORDERCOLOR, rState.BorderColor );
        DebugAssert( hRes == D3D_OK );
    }
}

Void Direct3D9Renderer::_DestroyResources()
{
    // Destroy device-dependant resources
    m_pfOnDestroyResources( m_pResourceUserData );

    // D3D9 cursor
    // nothing to do
}
Void Direct3D9Renderer::_RecreateResources()
{
    // D3D9 cursor
    _D3D9_CreateCursor();

    // Recreate device-dependant resources
    m_pfOnRecreateResources( m_pResourceUserData );
}
Void Direct3D9Renderer::_UpdateShaderVersions()
{
    // VertexShader version
    DWord dwVSVersion = m_devCaps.VertexShaderVersion;
    VertexShaderProfile iVSProfile;
    if ( dwVSVersion == D3DVS_VERSION(3,0) )
        iVSProfile = VERTEXSHADER_3_0;
    else
        iVSProfile = VERTEXSHADER_NONE;

    // PixelShader version
    DWord dwPSVersion = m_devCaps.PixelShaderVersion;
    PixelShaderProfile iPSProfile;
    if ( dwPSVersion == D3DPS_VERSION(3,0) )
        iPSProfile = PIXELSHADER_3_0;
    else
        iPSProfile = PIXELSHADER_NONE;

    // Let the user adjust shader profiles
    m_pfOnShaderVersionUpdate( iVSProfile, iPSProfile, m_pShaderVersionUserData );

    // Shaders limits
    m_iMaxVSSamplers = 4;
    m_iMaxPSSamplers = 16;
    m_iMaxTextureUnits = 8;
}

