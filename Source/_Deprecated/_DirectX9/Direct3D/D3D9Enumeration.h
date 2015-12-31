/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/D3D9Enumeration.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_D3D9ENUMERATION_H
#define SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_D3D9ENUMERATION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Forwards/ForwardDirect3D9.h"

#include "../../System/Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define D3D9ENUM_MAX_ADAPTERS 4
#define D3D9ENUM_MAX_DISPLAYMODES 256
#define D3D9ENUM_MAX_DEVICESETTINGS 48

#define D3D9ENUM_MAX_DEVICETYPES 2
#define D3D9ENUM_MAX_ADAPTERFORMATS 4
#define D3D9ENUM_MAX_BACKBUFFERFORMATS 6
#define D3D9ENUM_MAX_DEPTHSTENCILFORMATS 6
#define D3D9ENUM_MAX_MULTISAMPLETYPES 17
#define D3D9ENUM_MAX_DSMSCONFLICTS 32
#define D3D9ENUM_MAX_PRESENTINTERVALS 6

typedef Bool (*AcceptDevice)( D3DCAPS9 * pCaps, D3DFORMAT adFmt, D3DFORMAT bbFmt, Bool bWindowed );

struct _d3d9_adapter_info;
struct _d3d9_device_info;

typedef struct _d3d9_dsms_conflict
{
    D3DFORMAT d3dDSFormat;
    D3DMULTISAMPLE_TYPE d3dMSType;
} D3D9DSMSConflict;
typedef struct _d3d9_devicesettings_info
{
    struct _d3d9_adapter_info * pAdapterInfo;
    struct _d3d9_device_info * pDeviceInfo;

    UInt iAdapterOrdinal;
    D3DDEVTYPE d3dDeviceType;
    D3DFORMAT d3dAdapterFormat;
    D3DFORMAT d3dBackBufferFormat;
    Bool bWindowed;

    UInt iDepthStencilCount;
    D3DFORMAT arrDepthStencilFormats[D3D9ENUM_MAX_DEPTHSTENCILFORMATS];

    UInt iMultiSampleCount;
    D3DMULTISAMPLE_TYPE arrMultiSampleTypes[D3D9ENUM_MAX_MULTISAMPLETYPES];
    DWord arrMultiSampleQualities[D3D9ENUM_MAX_MULTISAMPLETYPES];

    UInt iDSMSConflictCount;
    D3D9DSMSConflict arrDSMSConflicts[D3D9ENUM_MAX_DSMSCONFLICTS];

    UInt iPresentIntervalCount;
    DWord arrPresentIntervals[D3D9ENUM_MAX_PRESENTINTERVALS];
} D3D9DeviceSettingsInfo;
typedef struct _d3d9_device_info
{
    UInt iAdapterOrdinal;
    D3DDEVTYPE d3dDeviceType;
    D3DCAPS9 d3dCaps;

    UInt iDeviceSettingsCount;
    D3D9DeviceSettingsInfo arrDeviceSettings[D3D9ENUM_MAX_DEVICESETTINGS];
} D3D9DeviceInfo;
typedef struct _d3d9_adapter_info
{
    UInt iAdapterOrdinal;
    D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;

    UInt iDisplayModeCount;
    D3DDISPLAYMODE arrDisplayModes[D3D9ENUM_MAX_DISPLAYMODES];

    UInt iDeviceCount;
    D3D9DeviceInfo arrDeviceInfos[D3D9ENUM_MAX_DEVICETYPES];
} D3D9AdapterInfo;

typedef struct _d3d9_device_parameters
{
    UInt iAdapterOrdinal;
    D3DDEVTYPE d3dDeviceType;
    D3DFORMAT d3dAdapterFormat;
    DWord dwBehaviour;
    D3DPRESENT_PARAMETERS d3dPresentParams;
} D3D9DeviceParameters;

enum D3D9DeviceMatchType
{
    DEVMATCH_IGNORE = 0, // Ignore input, use the closest valid value to default
    DEVMATCH_PRESERVE,   // Preserve input strictly, abort on fail
    DEVMATCH_CLOSEST     // Closest to input
};
typedef struct _d3d9_device_match_options
{
    D3D9DeviceMatchType iAdapterOrdinal;
    D3D9DeviceMatchType iAdapterFormat;
    D3D9DeviceMatchType iDeviceType;
    D3D9DeviceMatchType iVertexProcessing;
    D3D9DeviceMatchType iBackBufferFormat;
    D3D9DeviceMatchType iBackBufferCount;
    D3D9DeviceMatchType iWindowed;
    D3D9DeviceMatchType iResolution;
    D3D9DeviceMatchType iRefreshRate;
    D3D9DeviceMatchType iDepthStencilFormat;
    D3D9DeviceMatchType iMultiSample;
    D3D9DeviceMatchType iPresentFlags;
    D3D9DeviceMatchType iPresentInterval;
    D3D9DeviceMatchType iSwapEffect;
} D3D9DeviceMatchOptions;

/////////////////////////////////////////////////////////////////////////////////
// The D3D9Enumeration class
class D3D9Enumeration
{
public:
    D3D9Enumeration();
    ~D3D9Enumeration();

    // Defered construction
    inline Void SetDriver( IDirect3D9 * pDriver );

    // Getters
    inline Bool IsEnumDone() const;

    inline UInt GetAdapterCount() const;
    const D3D9AdapterInfo * GetAdapter( UInt iAdapterOrdinal ) const;
    const D3D9DeviceInfo * GetDevice( UInt iAdapterOrdinal, D3DDEVTYPE d3dDeviceType ) const;
    const D3D9DeviceSettingsInfo * GetDeviceSettings( UInt iAdapterOrdinal, D3DDEVTYPE d3dDeviceType,
                                                      D3DFORMAT d3dAdapterFormat, D3DFORMAT d3dBackBufferFormat,
                                                      Bool bWindowed ) const;

    // Set enumeration parameters
    inline Void SetEnumCallback( AcceptDevice pfAcceptDevice );
    inline Void SetResolutionMinMax( UInt iMinWidth, UInt iMinHeight, UInt iMaxWidth, UInt iMaxHeight );
    inline Void SetRefreshMinMax( UInt iMin, UInt iMax );
    inline Void SetMultiSampleMax( UInt iMax );

    // Enumeration routine
    Void Enumerate();
    Void Reset();

    // Matching routine
    Bool MatchValidDevice( D3D9DeviceParameters * outParameters, const D3D9DeviceMatchOptions & matchOptions,
                           const D3D9DeviceParameters & inParameters ) const;

private:
    // Enumeration process
    Void _EnumerateDevices( D3D9AdapterInfo * pAdapter );
    Void _EnumerateDeviceSettings( D3D9AdapterInfo * pAdapter, D3D9DeviceInfo * pDevice );
    Void _BuildDepthStencilFormats( D3D9DeviceSettingsInfo * pDeviceSettings );
    Void _BuildMultiSampleTypes( D3D9DeviceSettingsInfo * pDeviceSettings );
    Void _BuildDSMSConflicts( D3D9DeviceSettingsInfo * pDeviceSettings );
    Void _BuildPresentIntervals( D3D9DeviceInfo * pDevice, D3D9DeviceSettingsInfo * pDeviceSettings );

    static Int _SortDisplayModesCB( const D3DDISPLAYMODE & rLeft, const D3DDISPLAYMODE & rRight, Void * pUserData );

    static const D3DDEVTYPE sm_arrDevTypes[D3D9ENUM_MAX_DEVICETYPES];
    static const D3DFORMAT sm_arrAdapterFormats[D3D9ENUM_MAX_ADAPTERFORMATS];
    static const D3DFORMAT sm_arrBackBufferFormats[D3D9ENUM_MAX_BACKBUFFERFORMATS];
    static const D3DFORMAT sm_arrDepthStencilFormats[D3D9ENUM_MAX_DEPTHSTENCILFORMATS];
    static const D3DMULTISAMPLE_TYPE sm_arrMultiSampleTypes[D3D9ENUM_MAX_MULTISAMPLETYPES];
    static const DWord sm_arrPresentIntervals[D3D9ENUM_MAX_PRESENTINTERVALS];

    // Matching process
    Void _BuildOptimalDeviceParameters( D3D9DeviceParameters * outOptParameters, const D3D9DeviceMatchOptions & matchOptions,
                                        const D3D9DeviceParameters & inParameters ) const;
    Bool _CheckPreserveOptions( const D3D9DeviceSettingsInfo * pDeviceSettings, const D3D9DeviceMatchOptions & matchOptions,
                                const D3D9DeviceParameters & inParameters ) const;
    Float _RankDeviceSettings( const D3D9DeviceSettingsInfo * pDeviceSettings, const D3D9DeviceParameters & optParameters,
                                const D3DDISPLAYMODE & desktopDisplayMode ) const;
    Void _BuildValidDeviceParameters( D3D9DeviceParameters * outValidParameters, const D3D9DeviceMatchOptions & matchOptions,
                                      const D3D9DeviceParameters & inParameters, const D3D9DeviceSettingsInfo * pBestDeviceSettings ) const;
    Void _ClosestValidResolution( D3DDISPLAYMODE * outBestDisplayMode, const D3DDISPLAYMODE & d3dDisplayMode,
                                  const D3D9DeviceSettingsInfo * pBestDeviceSettings ) const;

    UInt _GetColorBits( D3DFORMAT d3dFmt ) const;
    UInt _GetAlphaBits( D3DFORMAT d3dFmt ) const;
    UInt _GetDepthBits( D3DFORMAT d3dFmt ) const;
    UInt _GetStencilBits( D3DFORMAT d3dFmt ) const;

    // Data
    IDirect3D9 * m_pDriver;

    UInt m_iAdapterCount;
    D3D9AdapterInfo m_arrAdapters[D3D9ENUM_MAX_ADAPTERS];

    Bool m_bEnumDone;
    AcceptDevice m_pfAcceptDevice;
    UInt m_iMinWidth;
    UInt m_iMaxWidth;
    UInt m_iMinHeight;
    UInt m_iMaxHeight;
    UInt m_iMinRefresh;
    UInt m_iMaxRefresh;
    UInt m_iMaxMultiSamples;

    UInt m_iAdapterFormatCount;
    D3DFORMAT m_arrAdapterFormats[D3D9ENUM_MAX_ADAPTERFORMATS + 1];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D9Enumeration.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_D3D9ENUMERATION_H

