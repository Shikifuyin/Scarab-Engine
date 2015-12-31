/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/D3D9Enumeration.cpp
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
// Internal dependency : Direct3D9, DirectX
#define WIN32_LEAN_AND_MEAN
#include <d3d9.h>

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D9Enumeration.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D9Enumeration implementation
const D3DDEVTYPE D3D9Enumeration::sm_arrDevTypes[D3D9ENUM_MAX_DEVICETYPES] =
{
    D3DDEVTYPE_HAL,
    D3DDEVTYPE_REF
};
const D3DFORMAT D3D9Enumeration::sm_arrAdapterFormats[D3D9ENUM_MAX_ADAPTERFORMATS] =
{
    D3DFMT_X8R8G8B8,
    D3DFMT_X1R5G5B5,
    D3DFMT_R5G6B5,
    D3DFMT_A2R10G10B10
};
const D3DFORMAT D3D9Enumeration::sm_arrBackBufferFormats[D3D9ENUM_MAX_BACKBUFFERFORMATS] =
{
    D3DFMT_A8R8G8B8,
    D3DFMT_X8R8G8B8,
    D3DFMT_A2R10G10B10,
    D3DFMT_R5G6B5,
    D3DFMT_A1R5G5B5,
    D3DFMT_X1R5G5B5
};
const D3DFORMAT D3D9Enumeration::sm_arrDepthStencilFormats[D3D9ENUM_MAX_DEPTHSTENCILFORMATS] =
{
    D3DFMT_D16,
    D3DFMT_D15S1,
    D3DFMT_D24X8,
    D3DFMT_D24S8,
    D3DFMT_D24X4S4,
    D3DFMT_D32
};
const D3DMULTISAMPLE_TYPE D3D9Enumeration::sm_arrMultiSampleTypes[D3D9ENUM_MAX_MULTISAMPLETYPES] =
{
    D3DMULTISAMPLE_NONE,
    D3DMULTISAMPLE_NONMASKABLE,
    D3DMULTISAMPLE_2_SAMPLES,
    D3DMULTISAMPLE_3_SAMPLES,
    D3DMULTISAMPLE_4_SAMPLES,
    D3DMULTISAMPLE_5_SAMPLES,
    D3DMULTISAMPLE_6_SAMPLES,
    D3DMULTISAMPLE_7_SAMPLES,
    D3DMULTISAMPLE_8_SAMPLES,
    D3DMULTISAMPLE_9_SAMPLES,
    D3DMULTISAMPLE_10_SAMPLES,
    D3DMULTISAMPLE_11_SAMPLES,
    D3DMULTISAMPLE_12_SAMPLES,
    D3DMULTISAMPLE_13_SAMPLES,
    D3DMULTISAMPLE_14_SAMPLES,
    D3DMULTISAMPLE_15_SAMPLES,
    D3DMULTISAMPLE_16_SAMPLES
};
const DWord D3D9Enumeration::sm_arrPresentIntervals[D3D9ENUM_MAX_PRESENTINTERVALS] =
{
    D3DPRESENT_INTERVAL_DEFAULT,
    D3DPRESENT_INTERVAL_IMMEDIATE,
    D3DPRESENT_INTERVAL_ONE,
    D3DPRESENT_INTERVAL_TWO,
    D3DPRESENT_INTERVAL_THREE,
    D3DPRESENT_INTERVAL_FOUR
};

D3D9Enumeration::D3D9Enumeration()
{
    m_pDriver = NULL;

    m_iAdapterCount = 0;

    m_pfAcceptDevice = NULL;
    m_iMinWidth = 640;
    m_iMaxWidth = UINT_MAX;
    m_iMinHeight = 480;
    m_iMaxHeight = UINT_MAX;
    m_iMinRefresh = 0;
    m_iMaxRefresh = UINT_MAX;
    m_iMaxMultiSamples = UINT_MAX;

    m_iAdapterFormatCount = 0;

    m_bEnumDone = false;
}
D3D9Enumeration::~D3D9Enumeration()
{
    // nothing to do
}

const D3D9AdapterInfo * D3D9Enumeration::GetAdapter( UInt iAdapterOrdinal ) const
{
    DebugAssert( m_bEnumDone );
    for( UInt iAdapter = 0; iAdapter < m_iAdapterCount; ++iAdapter ) {
        if ( m_arrAdapters[iAdapter].iAdapterOrdinal == iAdapterOrdinal )
            return (m_arrAdapters + iAdapter);
    }
    return NULL;
}
const D3D9DeviceInfo * D3D9Enumeration::GetDevice( UInt iAdapterOrdinal, D3DDEVTYPE d3dDeviceType ) const
{
    DebugAssert( m_bEnumDone );
    const D3D9AdapterInfo * pAdapter = GetAdapter(iAdapterOrdinal);
    if (pAdapter == NULL)
        return NULL;

    for( UInt iDevice = 0; iDevice < pAdapter->iDeviceCount; ++iDevice ) {
        if ( pAdapter->arrDeviceInfos[iDevice].d3dDeviceType == d3dDeviceType )
            return (pAdapter->arrDeviceInfos + iDevice);
    }
    return NULL;
}
const D3D9DeviceSettingsInfo * D3D9Enumeration::GetDeviceSettings( UInt iAdapterOrdinal, D3DDEVTYPE d3dDeviceType,
                                                                   D3DFORMAT d3dAdapterFormat, D3DFORMAT d3dBackBufferFormat,
                                                                   Bool bWindowed ) const
{
    DebugAssert( m_bEnumDone );
    const D3D9DeviceInfo * pDevice = GetDevice( iAdapterOrdinal, d3dDeviceType );
    if (pDevice == NULL)
        return NULL;

    for( UInt iDeviceSetting = 0; iDeviceSetting < pDevice->iDeviceSettingsCount; ++iDeviceSetting ) {
        const D3D9DeviceSettingsInfo * pDeviceSettings = ( pDevice->arrDeviceSettings + iDeviceSetting );
        if ( pDeviceSettings->d3dAdapterFormat == d3dAdapterFormat &&
             pDeviceSettings->d3dBackBufferFormat == d3dBackBufferFormat &&
             pDeviceSettings->bWindowed == bWindowed )
            return pDeviceSettings;
    }
    return NULL;
}


Void D3D9Enumeration::Enumerate()
{
    DebugAssert( m_pDriver != NULL );
    DebugAssert( !m_bEnumDone );

    HRESULT hRes;

    UInt iAdapterCount = m_pDriver->GetAdapterCount();
    DebugAssert( iAdapterCount <= D3D9ENUM_MAX_ADAPTERS );

    m_iAdapterCount = 0;

    for( UInt iAdapter = 0; iAdapter < iAdapterCount; ++iAdapter ) {
        D3D9AdapterInfo * pAdapter = ( m_arrAdapters + m_iAdapterCount );
        pAdapter->iAdapterOrdinal = iAdapter;

        hRes = m_pDriver->GetAdapterIdentifier( iAdapter, 0, &(pAdapter->d3dAdapterIdentifier) );
        DebugAssert( hRes == D3D_OK );

        m_iAdapterFormatCount = 0;
        pAdapter->iDisplayModeCount = 0;

        for( UInt iAdapterFormat = 0; iAdapterFormat < D3D9ENUM_MAX_ADAPTERFORMATS; ++iAdapterFormat ) {
            UInt iModeCount = m_pDriver->GetAdapterModeCount( iAdapter, sm_arrAdapterFormats[iAdapterFormat] );
            for( UInt iMode = 0; iMode < iModeCount; ++iMode ) {
                D3DFORMAT * pAdapterFormat = ( m_arrAdapterFormats + m_iAdapterFormatCount );
                D3DDISPLAYMODE * pDisplayMode = ( pAdapter->arrDisplayModes + pAdapter->iDisplayModeCount );
                *pAdapterFormat = sm_arrAdapterFormats[iAdapterFormat];

                hRes = m_pDriver->EnumAdapterModes( iAdapter, *pAdapterFormat, iMode, pDisplayMode );
                DebugAssert( hRes == D3D_OK );

                if( pDisplayMode->Width < m_iMinWidth || pDisplayMode->Width > m_iMaxWidth ||
                    pDisplayMode->Height < m_iMinHeight || pDisplayMode->Height > m_iMaxHeight ||
                    pDisplayMode->RefreshRate < m_iMinRefresh || pDisplayMode->RefreshRate > m_iMaxRefresh )
                    continue;
                ++(pAdapter->iDisplayModeCount);

                if ( Contains<D3DFORMAT>( m_arrAdapterFormats, m_iAdapterFormatCount, pDisplayMode->Format ) )
                    continue;
                ++m_iAdapterFormatCount;
            }
        }
        DebugAssert( pAdapter->iDisplayModeCount < D3D9ENUM_MAX_DISPLAYMODES );

        D3DDISPLAYMODE d3dDisplayMode;
        hRes = m_pDriver->GetAdapterDisplayMode( iAdapter, &d3dDisplayMode );
        DebugAssert( hRes == D3D_OK );

        if ( m_iAdapterFormatCount < D3D9ENUM_MAX_ADAPTERFORMATS &&
             !(Contains<D3DFORMAT>(m_arrAdapterFormats, m_iAdapterFormatCount, d3dDisplayMode.Format)) ) {
            m_arrAdapterFormats[m_iAdapterFormatCount] = d3dDisplayMode.Format;
            ++m_iAdapterFormatCount;
        }

        QuickSort<D3DDISPLAYMODE>( pAdapter->arrDisplayModes, pAdapter->iDisplayModeCount, _SortDisplayModesCB, NULL );

        _EnumerateDevices( pAdapter );

        if ( pAdapter->iDeviceCount > 0 )
            ++m_iAdapterCount;
    }

    m_bEnumDone = true;
}
Void D3D9Enumeration::Reset()
{
    m_iAdapterCount = 0;

    m_pfAcceptDevice = NULL;
    m_iMinWidth = 640;
    m_iMaxWidth = UINT_MAX;
    m_iMinHeight = 480;
    m_iMaxHeight = UINT_MAX;
    m_iMinRefresh = 0;
    m_iMaxRefresh = UINT_MAX;
    m_iMaxMultiSamples = UINT_MAX;

    m_iAdapterFormatCount = 0;

    m_bEnumDone = false;
}

Bool D3D9Enumeration::MatchValidDevice( D3D9DeviceParameters * outParameters, const D3D9DeviceMatchOptions & matchOptions,
                                        const D3D9DeviceParameters & inParameters ) const
{
    DebugAssert( m_pDriver != NULL );
    DebugAssert( m_bEnumDone );

    HRESULT hRes;

    Bool bCheck = false;
    Float fBestRanking = -1.0f;
    const D3D9DeviceSettingsInfo * pBestDeviceSettings = NULL;

    // Build Optimal parameters first
    D3D9DeviceParameters devOptimalParameters;
    _BuildOptimalDeviceParameters( &devOptimalParameters, matchOptions, inParameters );

    for ( UInt iAdapter = 0; iAdapter < m_iAdapterCount; ++iAdapter ) {
        const D3D9AdapterInfo * pAdapter = ( m_arrAdapters + iAdapter );

        D3DDISPLAYMODE d3dAdapterDesktopDisplayMode;
        hRes = m_pDriver->GetAdapterDisplayMode( pAdapter->iAdapterOrdinal, &d3dAdapterDesktopDisplayMode );
        DebugAssert( hRes == D3D_OK );

        for ( UInt iDevice = 0; iDevice < pAdapter->iDeviceCount; ++iDevice ) {
            const D3D9DeviceInfo * pDevice = ( pAdapter->arrDeviceInfos + iDevice );

            for ( UInt iDeviceSettings = 0; iDeviceSettings < pDevice->iDeviceSettingsCount; ++iDeviceSettings ) {
                const D3D9DeviceSettingsInfo * pDeviceSettings = ( pDevice->arrDeviceSettings + iDeviceSettings );

                // Windowed mode implies same adapter format as the desktop
                if ( pDeviceSettings->bWindowed &&
                     (pDeviceSettings->d3dAdapterFormat != d3dAdapterDesktopDisplayMode.Format) )
                    continue;

                // Check DEVMATCH_PRESERVE options
                bCheck = _CheckPreserveOptions( pDeviceSettings, matchOptions, inParameters );
                if ( !bCheck )
                    continue;

                // Rank those settings
                Float fRanking = _RankDeviceSettings( pDeviceSettings, devOptimalParameters, d3dAdapterDesktopDisplayMode );
                if ( fRanking > fBestRanking ) {
                    fBestRanking = fRanking;
                    pBestDeviceSettings = pDeviceSettings;
                }
            }
        }
    }
    if( pBestDeviceSettings == NULL )
        return false;

    _BuildValidDeviceParameters( outParameters, matchOptions, inParameters, pBestDeviceSettings );
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D9Enumeration::_EnumerateDevices( D3D9AdapterInfo * pAdapter )
{
    HRESULT hRes;

    pAdapter->iDeviceCount = 0;

    for( UInt iDevType = 0; iDevType < D3D9ENUM_MAX_DEVICETYPES; ++iDevType ) {
        D3D9DeviceInfo * pDevice = ( pAdapter->arrDeviceInfos + pAdapter->iDeviceCount );
        pDevice->iAdapterOrdinal = pAdapter->iAdapterOrdinal;
        pDevice->d3dDeviceType = sm_arrDevTypes[iDevType];
        hRes = m_pDriver->GetDeviceCaps( pDevice->iAdapterOrdinal, pDevice->d3dDeviceType, &(pDevice->d3dCaps) );
        DebugAssert( hRes == D3D_OK );

        if ( pDevice->d3dDeviceType != D3DDEVTYPE_HAL ) {
            // make a creation test ?
        }

        _EnumerateDeviceSettings( pAdapter, pDevice );
        if ( pDevice->iDeviceSettingsCount > 0 )
            ++(pAdapter->iDeviceCount);
    }
}
Void D3D9Enumeration::_EnumerateDeviceSettings( D3D9AdapterInfo * pAdapter, D3D9DeviceInfo * pDevice )
{
    HRESULT hRes;

    pDevice->iDeviceSettingsCount = 0;

    for( UInt iAdapterFormat = 0; iAdapterFormat < m_iAdapterFormatCount; ++iAdapterFormat ) {
        D3DFORMAT d3dAdapterFormat = m_arrAdapterFormats[iAdapterFormat];
        for( UInt iBackBufferFormat = 0; iBackBufferFormat < D3D9ENUM_MAX_BACKBUFFERFORMATS; ++iBackBufferFormat ) {
            D3DFORMAT d3dBackBufferFormat = sm_arrBackBufferFormats[iBackBufferFormat];
            for( UInt iWindowed = 0; iWindowed < 2; ++iWindowed ) {
                if ( !iWindowed && pAdapter->iDisplayModeCount == 0 )
                    continue;

                // Check if settings are valid
                hRes = m_pDriver->CheckDeviceType( pAdapter->iAdapterOrdinal, pDevice->d3dDeviceType,
                                                   d3dAdapterFormat, d3dBackBufferFormat, iWindowed );
                if ( hRes != D3D_OK )
                    continue;

                // Allways require blending
                hRes = m_pDriver->CheckDeviceFormat( pAdapter->iAdapterOrdinal, pDevice->d3dDeviceType,
                                                     d3dAdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                                     D3DRTYPE_TEXTURE, d3dBackBufferFormat );
                if ( hRes != D3D_OK )
                    continue;

                // Call user selection callback
                if ( m_pfAcceptDevice != NULL ) {
                    Bool bAccept = m_pfAcceptDevice( &(pDevice->d3dCaps), d3dAdapterFormat, d3dBackBufferFormat, (iWindowed != 0) );
                    if (!bAccept)
                        continue;
                }

                // Record those settings
                D3D9DeviceSettingsInfo * pDeviceSettings = ( pDevice->arrDeviceSettings + pDevice->iDeviceSettingsCount );
                pDeviceSettings->pAdapterInfo = pAdapter;
                pDeviceSettings->pDeviceInfo = pDevice;
                pDeviceSettings->iAdapterOrdinal = pAdapter->iAdapterOrdinal;
                pDeviceSettings->d3dDeviceType = pDevice->d3dDeviceType;
                pDeviceSettings->d3dAdapterFormat = d3dAdapterFormat;
                pDeviceSettings->d3dBackBufferFormat = d3dBackBufferFormat;
                pDeviceSettings->bWindowed = (iWindowed != 0);

                // Build DS Formats & MS Types
                _BuildDepthStencilFormats( pDeviceSettings );
                _BuildMultiSampleTypes( pDeviceSettings );
                if ( pDeviceSettings->iMultiSampleCount == 0 )
                    continue;

                // Build Conflicts & Intervals
                _BuildDSMSConflicts( pDeviceSettings );
                _BuildPresentIntervals( pDevice, pDeviceSettings );

                ++(pDevice->iDeviceSettingsCount);
            }
        }
    }
}
Void D3D9Enumeration::_BuildDepthStencilFormats( D3D9DeviceSettingsInfo * pDeviceSettings )
{
    HRESULT hRes;

    pDeviceSettings->iDepthStencilCount = 0;

    for( UInt iDSFormat = 0; iDSFormat < D3D9ENUM_MAX_DEPTHSTENCILFORMATS; ++iDSFormat ) {
        D3DFORMAT * pFormat = ( pDeviceSettings->arrDepthStencilFormats + pDeviceSettings->iDepthStencilCount );
        *pFormat = sm_arrDepthStencilFormats[iDSFormat];

        hRes = m_pDriver->CheckDeviceFormat( pDeviceSettings->iAdapterOrdinal, pDeviceSettings->d3dDeviceType,
                                             pDeviceSettings->d3dAdapterFormat,
                                             D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, *pFormat );
        if ( hRes != D3D_OK )
            continue;

        hRes = m_pDriver->CheckDepthStencilMatch( pDeviceSettings->iAdapterOrdinal, pDeviceSettings->d3dDeviceType,
                                                  pDeviceSettings->d3dAdapterFormat,
                                                  pDeviceSettings->d3dBackBufferFormat, *pFormat );
        if ( hRes != D3D_OK )
            continue;

        ++(pDeviceSettings->iDepthStencilCount);
    }
}
Void D3D9Enumeration::_BuildMultiSampleTypes( D3D9DeviceSettingsInfo * pDeviceSettings )
{
    HRESULT hRes;

    pDeviceSettings->iMultiSampleCount = 0;

    for( UInt iMSType = 0; iMSType < D3D9ENUM_MAX_MULTISAMPLETYPES; ++iMSType ) {
        D3DMULTISAMPLE_TYPE * pType = ( pDeviceSettings->arrMultiSampleTypes + pDeviceSettings->iMultiSampleCount );
        DWord * pQuality = ( pDeviceSettings->arrMultiSampleQualities + pDeviceSettings->iMultiSampleCount );
        *pType = sm_arrMultiSampleTypes[iMSType];

        hRes = m_pDriver->CheckDeviceMultiSampleType( pDeviceSettings->iAdapterOrdinal, pDeviceSettings->d3dDeviceType,
                                                      pDeviceSettings->d3dBackBufferFormat, pDeviceSettings->bWindowed,
                                                      *pType, pQuality );
        if ( hRes != D3D_OK )
            continue;

        if ( *pQuality > m_iMaxMultiSamples + 1 )
            *pQuality = m_iMaxMultiSamples + 1;
        ++(pDeviceSettings->iMultiSampleCount);
    }
}
Void D3D9Enumeration::_BuildDSMSConflicts( D3D9DeviceSettingsInfo * pDeviceSettings )
{
    HRESULT hRes;

    pDeviceSettings->iDSMSConflictCount = 0;

    for( UInt iDSFormat = 0; iDSFormat < pDeviceSettings->iDepthStencilCount; ++iDSFormat ) {
        for( UInt iMSType = 0; iMSType < pDeviceSettings->iMultiSampleCount; ++iMSType ) {
            D3D9DSMSConflict * pConflict = ( pDeviceSettings->arrDSMSConflicts + pDeviceSettings->iDSMSConflictCount );
            pConflict->d3dDSFormat = pDeviceSettings->arrDepthStencilFormats[iDSFormat];
            pConflict->d3dMSType = pDeviceSettings->arrMultiSampleTypes[iMSType];

            hRes = m_pDriver->CheckDeviceMultiSampleType( pDeviceSettings->iAdapterOrdinal, pDeviceSettings->d3dDeviceType,
                                                          pConflict->d3dDSFormat, pDeviceSettings->bWindowed,
                                                          pConflict->d3dMSType, NULL );
            if ( hRes != D3D_OK )
                ++(pDeviceSettings->iDSMSConflictCount);
        }
    }
    DebugAssert( pDeviceSettings->iDSMSConflictCount < D3D9ENUM_MAX_DSMSCONFLICTS );
}
Void D3D9Enumeration::_BuildPresentIntervals( D3D9DeviceInfo * pDevice, D3D9DeviceSettingsInfo * pDeviceSettings )
{
    pDeviceSettings->iPresentIntervalCount = 0;

    for( UInt iInterval = 0; iInterval < D3D9ENUM_MAX_PRESENTINTERVALS; ++iInterval ) {
        DWord * pInterval = ( pDeviceSettings->arrPresentIntervals + pDeviceSettings->iPresentIntervalCount );
        *pInterval = sm_arrPresentIntervals[iInterval];

        if ( pDeviceSettings->bWindowed ) {
            if ( *pInterval == D3DPRESENT_INTERVAL_TWO ||
                 *pInterval == D3DPRESENT_INTERVAL_THREE ||
                 *pInterval == D3DPRESENT_INTERVAL_FOUR )
                continue;
        }

        if ( *pInterval == D3DPRESENT_INTERVAL_DEFAULT ||
             ( pDevice->d3dCaps.PresentationIntervals & (*pInterval) ) )
            ++(pDeviceSettings->iPresentIntervalCount);
    }
}

Int D3D9Enumeration::_SortDisplayModesCB( const D3DDISPLAYMODE & rLeft, const D3DDISPLAYMODE & rRight, Void * /*pUserData*/ )
{
    if( rLeft.Width > rRight.Width )
        return +1;
    if( rLeft.Width < rRight.Width )
        return -1;
    if( rLeft.Height > rRight.Height )
        return +1;
    if( rLeft.Height < rRight.Height )
        return -1;
    if( rLeft.Format > rRight.Format )
        return +1;
    if( rLeft.Format < rRight.Format )
        return -1;
    if( rLeft.RefreshRate > rRight.RefreshRate )
        return +1;
    if( rLeft.RefreshRate < rRight.RefreshRate )
        return -1;
    return 0;
}

Void D3D9Enumeration::_BuildOptimalDeviceParameters( D3D9DeviceParameters * outOptParameters, const D3D9DeviceMatchOptions & matchOptions,
                                                     const D3D9DeviceParameters & inParameters ) const
{
    MemZero( outOptParameters, sizeof(D3D9DeviceParameters) );

    // Adapter ordinal
    if ( matchOptions.iAdapterOrdinal == DEVMATCH_IGNORE )
        outOptParameters->iAdapterOrdinal = D3DADAPTER_DEFAULT;
    else
        outOptParameters->iAdapterOrdinal = inParameters.iAdapterOrdinal;

    // Pick desktop display mode
    HRESULT hRes;
    D3DDISPLAYMODE d3dAdapterDesktopDisplayMode;
    hRes = m_pDriver->GetAdapterDisplayMode( outOptParameters->iAdapterOrdinal, &d3dAdapterDesktopDisplayMode );
    DebugAssert( hRes == D3D_OK );

    // Windowed (early, needed by adapter format)
    if ( matchOptions.iWindowed == DEVMATCH_IGNORE )
        outOptParameters->d3dPresentParams.Windowed = TRUE;
    else
        outOptParameters->d3dPresentParams.Windowed = inParameters.d3dPresentParams.Windowed;

    // Adapter format
    if ( matchOptions.iAdapterFormat == DEVMATCH_IGNORE ) {
        if ( outOptParameters->d3dPresentParams.Windowed ||
             _GetColorBits( d3dAdapterDesktopDisplayMode.Format ) >= 8 )
            outOptParameters->d3dAdapterFormat = d3dAdapterDesktopDisplayMode.Format;
        else
            outOptParameters->d3dAdapterFormat = D3DFMT_X8R8G8B8;
    } else
        outOptParameters->d3dAdapterFormat = inParameters.d3dAdapterFormat;

    // Device type
    if ( matchOptions.iDeviceType == DEVMATCH_IGNORE )
        outOptParameters->d3dDeviceType = D3DDEVTYPE_HAL;
    else
        outOptParameters->d3dDeviceType = inParameters.d3dDeviceType;

    // Vertex processing
    if ( matchOptions.iVertexProcessing == DEVMATCH_IGNORE )
        outOptParameters->dwBehaviour = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    else
        outOptParameters->dwBehaviour = inParameters.dwBehaviour;

    // BackBuffer format
    if ( matchOptions.iBackBufferFormat == DEVMATCH_IGNORE )
        outOptParameters->d3dPresentParams.BackBufferFormat = outOptParameters->d3dAdapterFormat;
    else
        outOptParameters->d3dPresentParams.BackBufferFormat = inParameters.d3dPresentParams.BackBufferFormat;

    // Back buffer count
    if ( matchOptions.iBackBufferCount == DEVMATCH_IGNORE )
        outOptParameters->d3dPresentParams.BackBufferCount = 1;
    else
        outOptParameters->d3dPresentParams.BackBufferCount = inParameters.d3dPresentParams.BackBufferCount;

    // Resolution
    if ( matchOptions.iResolution == DEVMATCH_IGNORE ) {
        if ( outOptParameters->d3dPresentParams.Windowed ) {
            outOptParameters->d3dPresentParams.BackBufferWidth = 640;
            outOptParameters->d3dPresentParams.BackBufferHeight = 480;
        } else {
            outOptParameters->d3dPresentParams.BackBufferWidth = d3dAdapterDesktopDisplayMode.Width;
            outOptParameters->d3dPresentParams.BackBufferHeight = d3dAdapterDesktopDisplayMode.Height;
        }
    } else {
        outOptParameters->d3dPresentParams.BackBufferWidth = inParameters.d3dPresentParams.BackBufferWidth;
        outOptParameters->d3dPresentParams.BackBufferHeight = inParameters.d3dPresentParams.BackBufferHeight;
    }

    // Refresh rate
    if ( matchOptions.iRefreshRate == DEVMATCH_IGNORE )
        outOptParameters->d3dPresentParams.FullScreen_RefreshRateInHz = 0;
    else
        outOptParameters->d3dPresentParams.FullScreen_RefreshRateInHz = inParameters.d3dPresentParams.FullScreen_RefreshRateInHz;

    // Depth-Stencil
    if ( matchOptions.iDepthStencilFormat == DEVMATCH_IGNORE ) {
        UInt iBackBufferBits = _GetColorBits( outOptParameters->d3dPresentParams.BackBufferFormat );
        if ( iBackBufferBits >= 8 )
            outOptParameters->d3dPresentParams.AutoDepthStencilFormat = D3DFMT_D32;
        else
            outOptParameters->d3dPresentParams.AutoDepthStencilFormat = D3DFMT_D16;
    } else
        outOptParameters->d3dPresentParams.AutoDepthStencilFormat = inParameters.d3dPresentParams.AutoDepthStencilFormat;

    // Multisample
    if ( matchOptions.iMultiSample == DEVMATCH_IGNORE ) {
        outOptParameters->d3dPresentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
        outOptParameters->d3dPresentParams.MultiSampleQuality = 0;
    } else {
        outOptParameters->d3dPresentParams.MultiSampleType = inParameters.d3dPresentParams.MultiSampleType;
        outOptParameters->d3dPresentParams.MultiSampleQuality = inParameters.d3dPresentParams.MultiSampleQuality;
    }

    // Present flags
    if ( matchOptions.iPresentFlags == DEVMATCH_IGNORE )
        outOptParameters->d3dPresentParams.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    else
        outOptParameters->d3dPresentParams.Flags = inParameters.d3dPresentParams.Flags;

    // Present interval
    if ( matchOptions.iPresentInterval == DEVMATCH_IGNORE )
        outOptParameters->d3dPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    else
        outOptParameters->d3dPresentParams.PresentationInterval = inParameters.d3dPresentParams.PresentationInterval;

    // Swap effect
    if ( matchOptions.iSwapEffect == DEVMATCH_IGNORE )
        outOptParameters->d3dPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    else
        outOptParameters->d3dPresentParams.SwapEffect = inParameters.d3dPresentParams.SwapEffect;    
}
Bool D3D9Enumeration::_CheckPreserveOptions( const D3D9DeviceSettingsInfo * pDeviceSettings, const D3D9DeviceMatchOptions & matchOptions,
                                             const D3D9DeviceParameters & inParameters ) const
{
    UInt i;
    Bool bMatch;
    D3DDISPLAYMODE d3dDisplayMode;
    D3DMULTISAMPLE_TYPE d3dMultiSampleType;
    DWord d3dMultiSampleQuality;

    // Adapter ordinal
    if ( matchOptions.iAdapterOrdinal == DEVMATCH_PRESERVE ) {
        if ( pDeviceSettings->iAdapterOrdinal != inParameters.iAdapterOrdinal )
            return false;
    }

    // Adapter format
    if ( matchOptions.iAdapterFormat == DEVMATCH_PRESERVE ) {
        if ( pDeviceSettings->d3dAdapterFormat != inParameters.d3dAdapterFormat )
            return false;
    }

    // Device type
    if ( matchOptions.iDeviceType == DEVMATCH_PRESERVE ) {
        if ( pDeviceSettings->d3dDeviceType != inParameters.d3dDeviceType )
            return false;
    }

    // Vertex processing
    if ( matchOptions.iVertexProcessing == DEVMATCH_PRESERVE ) {
        if ( ( inParameters.dwBehaviour & D3DCREATE_HARDWARE_VERTEXPROCESSING ) != 0 &&
             ( pDeviceSettings->pDeviceInfo->d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 )
            return false;
    }

    // BackBuffer format
    if ( matchOptions.iBackBufferFormat == DEVMATCH_PRESERVE ) {
        if ( pDeviceSettings->d3dBackBufferFormat != inParameters.d3dPresentParams.BackBufferFormat )
            return false;
    }

    // Windowed
    if ( matchOptions.iWindowed == DEVMATCH_PRESERVE ) {
        if ( pDeviceSettings->bWindowed != (inParameters.d3dPresentParams.Windowed != FALSE) )
            return false;
    }

    // Resolution
    if ( matchOptions.iResolution == DEVMATCH_PRESERVE ) {
        bMatch = false;
        for( i = 0; i < pDeviceSettings->pAdapterInfo->iDisplayModeCount; ++i ) {
            d3dDisplayMode = pDeviceSettings->pAdapterInfo->arrDisplayModes[i];
            if ( d3dDisplayMode.Format != pDeviceSettings->d3dAdapterFormat )
                continue;
            if ( d3dDisplayMode.Width == inParameters.d3dPresentParams.BackBufferWidth &&
                 d3dDisplayMode.Height == inParameters.d3dPresentParams.BackBufferHeight ) {
                bMatch = true;
                break;
            }
        }
        if ( !bMatch )
            return false;
    }

    // Refresh rate
    if ( matchOptions.iRefreshRate == DEVMATCH_PRESERVE ) {
        bMatch = false;
        for( i = 0; i < pDeviceSettings->pAdapterInfo->iDisplayModeCount; ++i ) {
            d3dDisplayMode = pDeviceSettings->pAdapterInfo->arrDisplayModes[i];
            if ( d3dDisplayMode.Format != pDeviceSettings->d3dAdapterFormat )
                continue;
            if ( d3dDisplayMode.RefreshRate == inParameters.d3dPresentParams.FullScreen_RefreshRateInHz ) {
                bMatch = true;
                break;
            }
        }
        if ( !bMatch )
            return false;
    }

    // Depth stencil
    if ( matchOptions.iDepthStencilFormat == DEVMATCH_PRESERVE ) {
        if ( inParameters.d3dPresentParams.AutoDepthStencilFormat != D3DFMT_UNKNOWN ) {
            if ( !( Contains<D3DFORMAT>( pDeviceSettings->arrDepthStencilFormats, pDeviceSettings->iDepthStencilCount,
                                         inParameters.d3dPresentParams.AutoDepthStencilFormat ) ) )
                return false;
        }
    }

    // Multisample
    if ( matchOptions.iMultiSample == DEVMATCH_PRESERVE ) {
        bMatch = false;
        for( i = 0; i < pDeviceSettings->iMultiSampleCount; ++i ) {
            d3dMultiSampleType = pDeviceSettings->arrMultiSampleTypes[i];
            d3dMultiSampleQuality = pDeviceSettings->arrMultiSampleQualities[i];
            if ( d3dMultiSampleType == inParameters.d3dPresentParams.MultiSampleType &&
                 d3dMultiSampleQuality > inParameters.d3dPresentParams.MultiSampleQuality ) {
                bMatch = true;
                break;
            }
        }
        if ( !bMatch )
            return false;
    }

    // Present interval
    if( matchOptions.iPresentInterval == DEVMATCH_PRESERVE ) {
        if ( !( Contains<DWord>( pDeviceSettings->arrPresentIntervals, pDeviceSettings->iPresentIntervalCount,
                                 inParameters.d3dPresentParams.PresentationInterval ) ) )
            return false;
    }

    // Accept
    return true;
}
Float D3D9Enumeration::_RankDeviceSettings( const D3D9DeviceSettingsInfo * pDeviceSettings, const D3D9DeviceParameters & optParameters,
                                             const D3DDISPLAYMODE & desktopDisplayMode ) const
{
    Float fRanking = 0.0f;

    // Arbitrary weights
    const Float fAdapterWeight = 1000.0f;
    const Float fAdaptFmtWeight = 1000.0f;
    const Float fDevTypeWeight = 500.0f;
    const Float fVertexProcWeight = 500.0f;
    const Float fPureDeviceWeight = 500.0f;
    const Float fBackBufFmtWeight = 200.0f;
    const Float fWinFSWeight = 100.0f;
    const Float fResolutionWeight = 100.0f;
    const Float fRefreshRateWeight = 50.0f;
    const Float fDepthStencilWeight = 10.0f;
    const Float fMultiSampleWeight = 5.0f;
    const Float fPresentIntervalWeight = 1.0f;

    Int iBitDepthDelta;
    Float fScale;
    UInt i;
    Bool bMatch;
    D3DDISPLAYMODE d3dDisplayMode;
    D3DMULTISAMPLE_TYPE d3dMultiSampleType;
    DWord dwMultiSampleQuality;

    // Adapter ordinal
    if ( pDeviceSettings->iAdapterOrdinal == optParameters.iAdapterOrdinal )
        fRanking += fAdapterWeight;

    // Adapter format ( Bonus for XRGB8I or desktop match when fullscreen )
    if ( pDeviceSettings->d3dAdapterFormat == optParameters.d3dAdapterFormat )
        fRanking += fAdaptFmtWeight;
    else {
        iBitDepthDelta = ( _GetColorBits( pDeviceSettings->d3dAdapterFormat ) -
                           _GetColorBits( optParameters.d3dAdapterFormat ) );
        if (iBitDepthDelta < 0) iBitDepthDelta = -iBitDepthDelta;
        fScale = Max<Float>( 0.9f - ( (Float)iBitDepthDelta * 0.2f ), 0.0f );
        fRanking += fScale * fAdaptFmtWeight;
    }
    if ( !pDeviceSettings->bWindowed ) {
        if ( _GetColorBits( desktopDisplayMode.Format ) >= 8 )
            bMatch = ( pDeviceSettings->d3dAdapterFormat == desktopDisplayMode.Format );
        else
            bMatch = ( pDeviceSettings->d3dAdapterFormat == D3DFMT_X8R8G8B8 );
        if ( bMatch )
            fRanking += 0.1f;
    }

    // Device type ( Bonus for HAL )
    if ( pDeviceSettings->d3dDeviceType == optParameters.d3dDeviceType )
        fRanking += fDevTypeWeight;
    if ( pDeviceSettings->d3dDeviceType == D3DDEVTYPE_HAL )
        fRanking += 0.1f;

    // Vertex processing ( Bonus for Hardware T&L and Pure devices )
    if ( ( optParameters.dwBehaviour & D3DCREATE_HARDWARE_VERTEXPROCESSING ) != 0 ) {
        if ( ( pDeviceSettings->pDeviceInfo->d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) != 0 ) {
            fRanking += fVertexProcWeight;
            if ( ( optParameters.dwBehaviour & D3DCREATE_PUREDEVICE ) != 0 )
                fRanking += fPureDeviceWeight;
        }
    }
    if ( ( pDeviceSettings->pDeviceInfo->d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) != 0 )
        fRanking += 0.1f;

    // BackBuffer format ( Bonus for adapter match )
    if ( pDeviceSettings->d3dBackBufferFormat == optParameters.d3dPresentParams.BackBufferFormat )
        fRanking += fBackBufFmtWeight;
    else {
        iBitDepthDelta = ( _GetColorBits( pDeviceSettings->d3dBackBufferFormat ) -
                           _GetColorBits( optParameters.d3dPresentParams.BackBufferFormat ) );
        if (iBitDepthDelta < 0) iBitDepthDelta = -iBitDepthDelta;
        fScale = Max<Float>( 0.9f - ( (Float)iBitDepthDelta * 0.2f ), 0.0f );
        fRanking += fScale * fAdaptFmtWeight;
    }
    if ( pDeviceSettings->d3dBackBufferFormat == pDeviceSettings->d3dAdapterFormat )
        fRanking += 0.1f;

    // Windowed
    if ( pDeviceSettings->bWindowed == (optParameters.d3dPresentParams.Windowed != FALSE) )
        fRanking += fWinFSWeight;

    // Resolution
    bMatch = false;
    for( i = 0; i < pDeviceSettings->pAdapterInfo->iDisplayModeCount; ++i ) {
        d3dDisplayMode = pDeviceSettings->pAdapterInfo->arrDisplayModes[i];
        if ( d3dDisplayMode.Format != pDeviceSettings->d3dAdapterFormat )
            continue;
        if ( d3dDisplayMode.Width == optParameters.d3dPresentParams.BackBufferWidth &&
            d3dDisplayMode.Height == optParameters.d3dPresentParams.BackBufferHeight ) {
            bMatch = true;
            break;
        }
    }
    if ( bMatch )
        fRanking += fResolutionWeight;

    // Refresh rate
    bMatch = false;
    for( i = 0; i < pDeviceSettings->pAdapterInfo->iDisplayModeCount; ++i ) {
        d3dDisplayMode = pDeviceSettings->pAdapterInfo->arrDisplayModes[i];
        if ( d3dDisplayMode.Format != pDeviceSettings->d3dAdapterFormat )
            continue;
        if ( d3dDisplayMode.RefreshRate == optParameters.d3dPresentParams.FullScreen_RefreshRateInHz ) {
            bMatch = true;
            break;
        }
    }
    if ( bMatch )
        fRanking += fRefreshRateWeight;

    // Depth-Stencil 
    if ( Contains<D3DFORMAT>( pDeviceSettings->arrDepthStencilFormats, pDeviceSettings->iDepthStencilCount,
                              optParameters.d3dPresentParams.AutoDepthStencilFormat ) )
        fRanking += fDepthStencilWeight;

    // MultiSample
    bMatch = false;
    for( i = 0; i < pDeviceSettings->iMultiSampleCount; ++i ) {
        d3dMultiSampleType = pDeviceSettings->arrMultiSampleTypes[i];
        dwMultiSampleQuality = pDeviceSettings->arrMultiSampleQualities[i];
        if ( d3dMultiSampleType == optParameters.d3dPresentParams.MultiSampleType &&
             dwMultiSampleQuality > optParameters.d3dPresentParams.MultiSampleQuality ) {
            bMatch = true;
            break;
        }
    }
    if ( bMatch )
        fRanking += fMultiSampleWeight;

    // Present interval
    if ( Contains<DWord>( pDeviceSettings->arrPresentIntervals, pDeviceSettings->iPresentIntervalCount,
                          optParameters.d3dPresentParams.PresentationInterval ) )
        fRanking += fPresentIntervalWeight;

    return fRanking;
}
Void D3D9Enumeration::_BuildValidDeviceParameters( D3D9DeviceParameters * outValidParameters, const D3D9DeviceMatchOptions & matchOptions,
                                                   const D3D9DeviceParameters & inParameters,
                                                   const D3D9DeviceSettingsInfo * pBestDeviceSettings ) const
{
    D3DDISPLAYMODE d3dAdapterDesktopDisplayMode;
    HRESULT hRes = m_pDriver->GetAdapterDisplayMode( pBestDeviceSettings->iAdapterOrdinal, &d3dAdapterDesktopDisplayMode );
    DebugAssert( hRes == D3D_OK );

    // Vertex processing
    DWord dwBestBehaviour = 0;
    if ( matchOptions.iVertexProcessing == DEVMATCH_PRESERVE )
        dwBestBehaviour = inParameters.dwBehaviour;
    else if ( matchOptions.iVertexProcessing == DEVMATCH_IGNORE ) {
        if ( (pBestDeviceSettings->pDeviceInfo->d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0 )
            dwBestBehaviour = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
        else
            dwBestBehaviour = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    } else { // matchOptions.iVertexProcessing == DEVMATCH_CLOSEST
        dwBestBehaviour = inParameters.dwBehaviour;
        if ( (pBestDeviceSettings->pDeviceInfo->d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 )
            dwBestBehaviour = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // Back buffer count
    UInt iBestBackBufferCount;
    if ( matchOptions.iBackBufferCount == DEVMATCH_PRESERVE )
        iBestBackBufferCount = inParameters.d3dPresentParams.BackBufferCount;
    else if ( matchOptions.iBackBufferCount == DEVMATCH_IGNORE )
        iBestBackBufferCount = 1;
    else { // matchOptions.iBackBufferCount == DEVMATCH_CLOSEST
        iBestBackBufferCount = inParameters.d3dPresentParams.BackBufferCount;
        if ( iBestBackBufferCount > 3 )
            iBestBackBufferCount = 3;
        if ( iBestBackBufferCount < 1 )
            iBestBackBufferCount = 1;
    }

    // Resolution
    D3DDISPLAYMODE d3dBestDisplayMode;
    if ( matchOptions.iResolution == DEVMATCH_PRESERVE ) {
        d3dBestDisplayMode.Width = inParameters.d3dPresentParams.BackBufferWidth;
        d3dBestDisplayMode.Height = inParameters.d3dPresentParams.BackBufferHeight;
    } else {
        D3DDISPLAYMODE d3dDisplayMode;
        if ( matchOptions.iResolution == DEVMATCH_CLOSEST ) {
            d3dDisplayMode.Width = inParameters.d3dPresentParams.BackBufferWidth;
            d3dDisplayMode.Height = inParameters.d3dPresentParams.BackBufferHeight;
        } else { // matchOptions.iResolution == DEVMATCH_IGNORE
            if( pBestDeviceSettings->bWindowed ) {
                d3dDisplayMode.Width = 640;
                d3dDisplayMode.Height = 480;
            } else {
                d3dDisplayMode.Width = d3dAdapterDesktopDisplayMode.Width;
                d3dDisplayMode.Height = d3dAdapterDesktopDisplayMode.Height;
            }
        }
        _ClosestValidResolution( &d3dBestDisplayMode, d3dDisplayMode, pBestDeviceSettings );
    }

    // Refresh rate
    if ( pBestDeviceSettings->bWindowed )
        d3dBestDisplayMode.RefreshRate = 0;
    else {
        if ( matchOptions.iRefreshRate == DEVMATCH_PRESERVE )
            d3dBestDisplayMode.RefreshRate = inParameters.d3dPresentParams.FullScreen_RefreshRateInHz;
        else {
            UInt iRefreshRate;
            if ( matchOptions.iRefreshRate == DEVMATCH_CLOSEST )
                iRefreshRate = inParameters.d3dPresentParams.FullScreen_RefreshRateInHz;
            else // matchOptions.iRefreshRate == DEVMATCH_IGNORE
                iRefreshRate = d3dAdapterDesktopDisplayMode.RefreshRate;
            d3dBestDisplayMode.RefreshRate = 0;
            if ( iRefreshRate != 0 ) {
                Int iBestRanking = 100000;
                for( UInt i = 0; i < pBestDeviceSettings->pAdapterInfo->iDisplayModeCount; ++i ) {
                    D3DDISPLAYMODE d3dDisplayMode = pBestDeviceSettings->pAdapterInfo->arrDisplayModes[i];
                    if ( d3dDisplayMode.Format != pBestDeviceSettings->d3dAdapterFormat ||
                         d3dDisplayMode.Width != d3dBestDisplayMode.Width ||
                         d3dDisplayMode.Height != d3dBestDisplayMode.Height )
                        continue;
                    Int iRanking = ( d3dDisplayMode.RefreshRate - iRefreshRate );
                    if ( iRanking < 0 ) iRanking = -iRanking;
                    if ( iRanking < iBestRanking ) {
                        iBestRanking = iRanking;
                        d3dBestDisplayMode.RefreshRate = d3dDisplayMode.RefreshRate;
                        if ( iBestRanking == 0 )
                            break;
                    }
                }
            }
        }
    }

    // Depth-Stencil
    D3DFORMAT d3dBestDepthStencil;
    Bool bBestEnableDepthStencil;
    Int iBestRanking = 100000;
    UInt iBestIndex = INVALID_OFFSET;
    UInt iBackBufferBitDepth = _GetColorBits( pBestDeviceSettings->d3dBackBufferFormat );
    UInt iInputDepthBitDepth = _GetDepthBits( inParameters.d3dPresentParams.AutoDepthStencilFormat );
    UInt iInputStencilBitDepth = _GetStencilBits( inParameters.d3dPresentParams.AutoDepthStencilFormat );
    for( UInt i = 0; i < pBestDeviceSettings->iDepthStencilCount; ++i ) {
        D3DFORMAT d3dDSFormat = pBestDeviceSettings->arrDepthStencilFormats[i];
        UInt iDepthBitDepth = _GetDepthBits( d3dDSFormat );
        UInt iStencilBitDepth = _GetStencilBits( d3dDSFormat );
        Int iRanking;
        if ( matchOptions.iDepthStencilFormat == DEVMATCH_PRESERVE ) {
            iRanking = (iDepthBitDepth == iInputDepthBitDepth) ? 0 : 10000;
            iRanking += (iStencilBitDepth == iInputStencilBitDepth) ? 0 : 10000;
        } else if ( matchOptions.iDepthStencilFormat == DEVMATCH_IGNORE ) {
            iRanking = ( iDepthBitDepth - (iBackBufferBitDepth << 2) );
            if ( iRanking < 0 ) iRanking = -iRanking;
            iRanking += iStencilBitDepth;
        } else { // matchOptions.iDepthStencilFormat == DEVMATCH_CLOSEST
            iRanking = ( iDepthBitDepth - iInputDepthBitDepth );
            if ( iRanking < 0 ) iRanking = -iRanking;
            Int iTmp = ( iStencilBitDepth - iInputStencilBitDepth );
            if ( iTmp < 0 ) iTmp = -iTmp;
            iRanking += iTmp;
        }
        if ( iRanking < iBestRanking ) {
            iBestRanking = iRanking;
            iBestIndex = i;
        }
    }
    if ( iBestIndex != INVALID_OFFSET ) {
        d3dBestDepthStencil = pBestDeviceSettings->arrDepthStencilFormats[iBestIndex];
        bBestEnableDepthStencil = true;
    } else {
        d3dBestDepthStencil = D3DFMT_UNKNOWN;
        bBestEnableDepthStencil = false;
    }

    // Swap Effect (early, needed by multisample)
    D3DSWAPEFFECT d3dBestSwapEffect;
    if ( matchOptions.iSwapEffect == DEVMATCH_PRESERVE )
        d3dBestSwapEffect = inParameters.d3dPresentParams.SwapEffect;
    else if ( matchOptions.iSwapEffect == DEVMATCH_IGNORE )
        d3dBestSwapEffect = D3DSWAPEFFECT_DISCARD;
    else { // matchOptions.iSwapEffect == DEVMATCH_CLOSEST
        d3dBestSwapEffect = inParameters.d3dPresentParams.SwapEffect;
        if ( d3dBestSwapEffect != D3DSWAPEFFECT_DISCARD &&
             d3dBestSwapEffect != D3DSWAPEFFECT_FLIP && d3dBestSwapEffect != D3DSWAPEFFECT_COPY )
            d3dBestSwapEffect = D3DSWAPEFFECT_DISCARD;
    }

    // Multisample
    D3DMULTISAMPLE_TYPE d3dBestMultiSampleType;
    DWord iBestMultiSampleQuality;
    if ( d3dBestSwapEffect != D3DSWAPEFFECT_DISCARD ) {
        d3dBestMultiSampleType = D3DMULTISAMPLE_NONE;
        iBestMultiSampleQuality = 0;
    } else {
        if ( matchOptions.iMultiSample == DEVMATCH_PRESERVE ) {
            d3dBestMultiSampleType = inParameters.d3dPresentParams.MultiSampleType;
            iBestMultiSampleQuality = inParameters.d3dPresentParams.MultiSampleQuality;
        } else if ( matchOptions.iMultiSample == DEVMATCH_IGNORE ) {
            d3dBestMultiSampleType = D3DMULTISAMPLE_NONE;
            iBestMultiSampleQuality = 0;
        } else { // matchOptions.iMultiSample == DEVMATCH_CLOSEST
            d3dBestMultiSampleType = D3DMULTISAMPLE_NONE;
            iBestMultiSampleQuality = 0;
            Int iBestTypeDelta = ( d3dBestMultiSampleType - inParameters.d3dPresentParams.MultiSampleType );
            if ( iBestTypeDelta < 0 ) iBestTypeDelta = -iBestTypeDelta;
            for( UInt i = 0; i < pBestDeviceSettings->iMultiSampleCount; ++i ) {
                D3DMULTISAMPLE_TYPE d3dMSType = pBestDeviceSettings->arrMultiSampleTypes[i];
                DWord iMSQuality = pBestDeviceSettings->arrMultiSampleQualities[i];
                Int iTypeDelta = ( d3dMSType - inParameters.d3dPresentParams.MultiSampleType );
                if ( iTypeDelta < 0 ) iTypeDelta = -iTypeDelta;
                if ( iTypeDelta < iBestTypeDelta ) {
                    iBestTypeDelta = iTypeDelta;
                    d3dBestMultiSampleType = d3dMSType;
                    iBestMultiSampleQuality = Min<DWord>( iMSQuality - 1, inParameters.d3dPresentParams.MultiSampleQuality );
                }
            }
        }
    }

    // Present flags
    DWord iBestPresentFlag;
    if ( matchOptions.iPresentFlags == DEVMATCH_PRESERVE )
        iBestPresentFlag = inParameters.d3dPresentParams.Flags;
    else if ( matchOptions.iPresentFlags == DEVMATCH_IGNORE ) {
        iBestPresentFlag = 0;
        if ( bBestEnableDepthStencil )
            iBestPresentFlag = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    } else { // matchOptions.iPresentFlags == DEVMATCH_CLOSEST
        iBestPresentFlag = inParameters.d3dPresentParams.Flags;
        if ( bBestEnableDepthStencil )
            iBestPresentFlag |= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    }

    // Present interval
    DWord iBestPresentInterval;
    if ( matchOptions.iPresentInterval == DEVMATCH_PRESERVE )
        iBestPresentInterval = inParameters.d3dPresentParams.PresentationInterval;
    else if ( matchOptions.iPresentInterval == DEVMATCH_IGNORE )
        iBestPresentInterval = D3DPRESENT_INTERVAL_DEFAULT;
    else { // matchOptions.iPresentInterval == DEVMATCH_CLOSEST
        if ( Contains<DWord>( pBestDeviceSettings->arrPresentIntervals, pBestDeviceSettings->iPresentIntervalCount,
                              inParameters.d3dPresentParams.PresentationInterval ) )
            iBestPresentInterval = inParameters.d3dPresentParams.PresentationInterval;
        else
            iBestPresentInterval = D3DPRESENT_INTERVAL_DEFAULT;
    }

    // Fill with results
    MemZero( outValidParameters, sizeof(D3D9DeviceParameters) );
    outValidParameters->iAdapterOrdinal = pBestDeviceSettings->iAdapterOrdinal;
    outValidParameters->d3dDeviceType = pBestDeviceSettings->d3dDeviceType;
    outValidParameters->d3dAdapterFormat = pBestDeviceSettings->d3dAdapterFormat;
    outValidParameters->dwBehaviour = dwBestBehaviour;
    outValidParameters->d3dPresentParams.BackBufferFormat = pBestDeviceSettings->d3dBackBufferFormat;
    outValidParameters->d3dPresentParams.BackBufferCount = iBestBackBufferCount;
    outValidParameters->d3dPresentParams.BackBufferWidth = d3dBestDisplayMode.Width;
    outValidParameters->d3dPresentParams.BackBufferHeight = d3dBestDisplayMode.Height;
    outValidParameters->d3dPresentParams.Windowed = (pBestDeviceSettings->bWindowed) ? TRUE : FALSE;
    outValidParameters->d3dPresentParams.FullScreen_RefreshRateInHz = d3dBestDisplayMode.RefreshRate;
    outValidParameters->d3dPresentParams.EnableAutoDepthStencil = bBestEnableDepthStencil;
    outValidParameters->d3dPresentParams.AutoDepthStencilFormat = d3dBestDepthStencil;
    outValidParameters->d3dPresentParams.MultiSampleType = d3dBestMultiSampleType;
    outValidParameters->d3dPresentParams.MultiSampleQuality = iBestMultiSampleQuality;
    outValidParameters->d3dPresentParams.Flags = iBestPresentFlag;
    outValidParameters->d3dPresentParams.PresentationInterval = iBestPresentInterval;
    outValidParameters->d3dPresentParams.SwapEffect = d3dBestSwapEffect;
    outValidParameters->d3dPresentParams.hDeviceWindow = NULL;
}
Void D3D9Enumeration::_ClosestValidResolution( D3DDISPLAYMODE * outBestDisplayMode, const D3DDISPLAYMODE & d3dDisplayMode,
                                               const D3D9DeviceSettingsInfo * pBestDeviceSettings ) const
{
    D3DDISPLAYMODE d3dBestDisplayMode;
    MemZero( &d3dBestDisplayMode, sizeof(D3DDISPLAYMODE) );

    if( pBestDeviceSettings->bWindowed )
        *outBestDisplayMode = d3dDisplayMode;
    else {
        Int iBestRanking = 100000;
        Int iRanking, iTmp;
        for( UInt i = 0; i < pBestDeviceSettings->pAdapterInfo->iDisplayModeCount; ++i ) {
            D3DDISPLAYMODE d3dCurDisplayMode = pBestDeviceSettings->pAdapterInfo->arrDisplayModes[i];
            if ( d3dCurDisplayMode.Format != pBestDeviceSettings->d3dAdapterFormat )
                continue;
            iRanking = ( d3dCurDisplayMode.Width - d3dDisplayMode.Width );
            if ( iRanking < 0 ) iRanking = -iRanking;
            iTmp = ( d3dCurDisplayMode.Height - d3dDisplayMode.Height );
            if ( iTmp < 0 ) iTmp = -iTmp;
            iRanking += iTmp;
            if ( iRanking < iBestRanking ) {
                iBestRanking = iRanking;
                d3dBestDisplayMode = d3dCurDisplayMode;
                if ( iBestRanking == 0 )
                    break;
            }
        }
        DebugAssert( d3dBestDisplayMode.Width > 0 );
        DebugAssert( d3dBestDisplayMode.Height > 0 );
        *outBestDisplayMode = d3dBestDisplayMode;
    }
}

UInt D3D9Enumeration::_GetColorBits( D3DFORMAT d3dFmt ) const
{
    switch( d3dFmt ) {
        case D3DFMT_R3G3B2:
        case D3DFMT_A8R3G3B2: return 2;
        case D3DFMT_A4R4G4B4:
        case D3DFMT_X4R4G4B4: return 4;
        case D3DFMT_R5G6B5:
        case D3DFMT_A1R5G5B5:
        case D3DFMT_X1R5G5B5: return 5;
        case D3DFMT_R8G8B8:
        case D3DFMT_A8R8G8B8:
        case D3DFMT_X8R8G8B8:
        case D3DFMT_A8B8G8R8: return 8;
        case D3DFMT_A2R10G10B10:
        case D3DFMT_A2B10G10R10: return 10;
        case D3DFMT_A16B16G16R16: return 16;
        default: return 0;
    }
}
UInt D3D9Enumeration::_GetAlphaBits( D3DFORMAT d3dFmt ) const
{
    switch( d3dFmt ) {
        case D3DFMT_R3G3B2:
        case D3DFMT_R5G6B5:
        case D3DFMT_R8G8B8:
        case D3DFMT_X4R4G4B4:
        case D3DFMT_X1R5G5B5:
        case D3DFMT_X8R8G8B8: return 0;
        case D3DFMT_A1R5G5B5: return 1;
        case D3DFMT_A2R10G10B10:
        case D3DFMT_A2B10G10R10: return 2;
        case D3DFMT_A4R4G4B4: return 4;
        case D3DFMT_A8R3G3B2:
        case D3DFMT_A8R8G8B8:
        case D3DFMT_A8B8G8R8: return 8;
        case D3DFMT_A16B16G16R16: return 16;
        default: return 0;
    }
}
UInt D3D9Enumeration::_GetDepthBits( D3DFORMAT d3dFmt ) const
{
    switch( d3dFmt ) {
        case D3DFMT_D15S1: return 15;
        case D3DFMT_D16_LOCKABLE:
        case D3DFMT_D16: return 16;
        case D3DFMT_D24X8:
        case D3DFMT_D24S8:
        case D3DFMT_D24X4S4:
        case D3DFMT_D24FS8: return 24;
        case D3DFMT_D32F_LOCKABLE:
        case D3DFMT_D32: return 32;
        default: return 0;
    }
}
UInt D3D9Enumeration::_GetStencilBits( D3DFORMAT d3dFmt ) const
{
    switch( d3dFmt ) {
        case D3DFMT_D16_LOCKABLE:
        case D3DFMT_D16:
        case D3DFMT_D32F_LOCKABLE:
        case D3DFMT_D32:
        case D3DFMT_D24X8: return 0;
        case D3DFMT_D15S1: return 1;
        case D3DFMT_D24X4S4: return 4;
        case D3DFMT_D24S8:
        case D3DFMT_D24FS8: return 8;
        default: return 0;
    }
}

