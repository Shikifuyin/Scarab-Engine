/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Direct3D9.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_DIRECT3D9_H
#define SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_DIRECT3D9_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Forwards/ForwardDirect3D9.h"
#include "../../GDI+/GDIWindow.h"

#include "Mappings.h"
#include "D3D9Enumeration.h"

#include "Resources/D3D9VertexFormat.h"
#include "Resources/D3D9Buffers.h"
#include "Resources/D3D9Textures.h"
#include "Resources/D3D9RenderTarget.h"
#include "Resources/D3D9Shaders.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Sampler limits
#define DIRECT3D9_MAX_VERTEX_SAMPLERS 4
#define DIRECT3D9_MAX_PIXEL_SAMPLERS 16
#define DIRECT3D9_MAX_TEXTURE_UNITS   8

    // Input data
typedef Void (*D3D9DeviceResourceCallback)( Void* );
typedef Void (*D3D9DeviceShaderVersionCallback)( VertexShaderProfile, PixelShaderProfile, Void* );

typedef struct _direct3d9_input
{
    GDIWindow * pWindow;
    D3D9DeviceResourceCallback pfOnDestroyResources;
    D3D9DeviceResourceCallback pfOnRecreateResources;
    Void * pResourceUserData;
    D3D9DeviceShaderVersionCallback pfOnShaderVersionUpdate;
    Void * pShaderVersionUserData;
} Direct3D9Input;

    // Device State
enum Direct3D9State
{
    D3D9DEVICE_NULL = 0,
    D3D9DEVICE_OK,
    D3D9DEVICE_LOST,
    D3D9DEVICE_CREATEFAILED,
    D3D9DEVICE_RESETFAILED
};

    // RenderState
typedef struct _d3d9_renderstate
{
    // CullingState
    DWord CullMode;

    // DepthState
    DWord DepthEnable;
    DWord DepthWritable;
    DWord DepthTest;

    // StencilState
    DWord StencilEnable;
    DWord StencilTest;
    DWord StencilOnFail;
    DWord StencilOnDepthFail;
    DWord StencilOnDepthPass;
    DWord StencilRef;
    DWord StencilMask;
    DWord StencilWriteMask;

    // BlendingState
    DWord BlendEnable;
    DWord BlendSrc;
    DWord BlendDst;
    D3DCOLOR BlendFactor;
    DWord BlendTestEnable;
    DWord BlendTest;
    DWord BlendTestRef;

    // PolygonState
    DWord FillMode;
    DWord SlopeScaleDepthBias;
    DWord DepthBias;
} D3D9RenderState;

    // SamplerState
typedef struct _d3d9_samplerstate
{
    DWord Wrap[3];
    DWord MagFilter;
    DWord MinFilter;
    DWord MipFilter;
    Float Anisotropy;
    Float LodBias;
    D3DCOLOR BorderColor;
} D3D9SamplerState;

/////////////////////////////////////////////////////////////////////////////////
// The Direct3D9Renderer class
class Direct3D9Renderer
{
public:
    Direct3D9Renderer( const Direct3D9Input & hInput, UInt iWidth, UInt iHeight,
                       TextureFormat iColorFormat, TextureFormat iDepthStencilFormat, UInt iMultiSampleCount );
    ~Direct3D9Renderer();

    // Device Caps
        // Shaders
    inline UInt GetVSBaseSampler() const;
    inline UInt GetVSMaxSamplers() const;
    inline UInt GetPSBaseSampler() const;
    inline UInt GetPSMaxSamplers() const;
    inline UInt GetMaxTextureUnits() const;

    // DisplayModes support
    inline Bool IsFullScreen() const;
    inline UInt GetBackBufferWidth() const;
    inline UInt GetBackBufferHeight() const;
    inline TextureFormat GetBackBufferFormat() const;
    inline TextureFormat GetDepthStencilFormat() const;
    inline UInt GetMultiSampleCount() const;

    Void ToggleFullScreen();
    Void SwitchMonitor( Void * pMonitor );

    Void SwitchResolution( UInt iBackBufferWidth, UInt iBackBufferHeight );
    Void SwitchBackBufferFormat( TextureFormat iBackBufferFormat );
    Void SwitchDepthStencilFormat( TextureFormat iDepthStencilFormat );
    Void SwitchMultiSampleCount( UInt iMultiSampleCount );
    Void SwitchDisplay( UInt iBackBufferWidth, UInt iBackBufferHeight, TextureFormat iBackBufferFormat );
    Void SwitchDevice( UInt iBackBufferWidth, UInt iBackBufferHeight, TextureFormat iBackBufferFormat,
                       TextureFormat iDepthStencilFormat, UInt iMultiSampleCount );

    // RenderStates access
    //Void SetClippingState() {}
    Void SetCullingState( Bool bCullEnabled, CullingTest iCullTest, CullingConvention iCullConvention );
    Void SetDepthState( Bool bDepthEnabled, Bool bDepthWritable, DepthTestMode iDepthTest );
    Void SetStencilState( Bool bStencilEnabled, StencilTestMode iStencilTest, StencilOperation iOnStencilFail,
                          StencilOperation iOnDepthFail, StencilOperation iOnDepthSuccess,
                          DWord dwStencilRef, DWord dwStencilMask, DWord dwStencilWriteMask );
    Void SetBlendingState( Bool bBlendEnabled, BlendingSrcMode iBlendSrc, BlendingDstMode iBlendDst, DWord dwConstColor,
                           Bool bBlendTestEnabled, BlendingTestMode iBlendTest, DWord dwBlendTestRef );
    Void SetPolygonState( Bool bWireframeEnabled, Bool bOffsetFills, Float fSlopeScale, Float fDepthBias );

    // SamplerStates access
    Void SetSamplerWrapState( UInt iSamplerUnit, ShaderSamplingType iType, const ShaderSamplingWrapMode * arrWrapMode,
                                                 DWord dwBorderColor );
    Void SetSamplerFilterState( UInt iSamplerUnit, ShaderSamplingFilter iFilter, Float fLODBias, Float fAnisotropy );

    // Viewport access
    Void GetViewport( UInt * outX, UInt * outY, UInt * outWidth, UInt * outHeight ) const;
    Void GetDepthRange( Float * outDepthMin, Float * outDepthMax ) const;

    Void MoveViewport( UInt iX, UInt iY );
    Void ResizeViewport( UInt iWidth, UInt iHeight );
    Void SetViewport( UInt iX, UInt iY, UInt iWidth, UInt iHeight );
    Void SetDepthRange( Float fDepthMin, Float fDepthMax );

    // Clearing & Masking
    Void ClearColorBuffer( DWord dwClearColor );
    Void ClearDepthBuffer( Float fClearDepth );
    Void ClearStencilBuffer( DWord dwClearStencil );
    Void ClearBuffers( DWord dwClearColor, Float fClearDepth, DWord dwClearStencil );

    Void ClearColorBuffer( DWord dwClearColor, UInt iX, UInt iY, UInt iWidth, UInt iHeight );
    Void ClearDepthBuffer( Float fClearDepth, UInt iX, UInt iY, UInt iWidth, UInt iHeight );
    Void ClearStencilBuffer( DWord dwClearStencil, UInt iX, UInt iY, UInt iWidth, UInt iHeight );
    Void ClearBuffers( DWord dwClearColor, Float fClearDepth, DWord dwClearStencil,
                       UInt iX, UInt iY, UInt iWidth, UInt iHeight );

    Void SetColorMask( Bool bAllowRed, Bool bAllowGreen, Bool bAllowBlue, Bool bAllowAlpha );

    // Basic rendering primitives
    Bool QueryRendering();
    Void PresentBackBuffer();

    Void DrawBegin();
    Void DrawEnd();

    Void DrawPrimitive( MeshType iPrimitiveType, UInt iPrimitiveCount, UInt iVertexCount,
                        Bool bIndexed, UInt iVertexOffset = 0, UInt iIndexOffset = 0 );

    // Cursor support
    Void SetCursorImage( UInt iWidth, UInt iHeight, const DWord * pImageARGB,
                         UInt iHotSpotX, UInt iHotSpotY );
    Void ShowCursor( Bool bShow );
    Void WarpCursor( UInt iX, UInt iY );

private:
    // Delegates access
    friend class D3D9VertexFormat;
    friend class D3D9VertexBuffer;
    friend class D3D9IndexBuffer;
    friend class D3D9Texture1D;
    friend class D3D9Texture2D;
    friend class D3D9Texture3D;
    friend class D3D9TextureCube;
    friend class D3D9RenderTarget;
    friend class D3D9Shader;
    friend class D3D9VertexShader;
    friend class D3D9PixelShader;

    // D3D9 Interface
        // Device
    static Bool _AcceptMinimalDevice( D3DCAPS9 * pCaps, D3DFORMAT d3dAdapterFormat,
                                      D3DFORMAT d3dBackBufferFormat, Bool bWindowed );

    inline Bool _D3D9_CanResetDevice( const D3D9DeviceParameters & newParameters,
                                      const D3D9DeviceParameters & oldParameters ) const;
    UInt _D3D9_AdapterFromMonitor( HMONITOR hMonitor ) const;

    Void _D3D9_CreateDevice( D3D9DeviceParameters * pParameters );
    Void _D3D9_ResetDevice( D3D9DeviceParameters * pParameters );
    Void _D3D9_DestroyDevice();
    Bool _D3D9_SwitchDevice( D3D9DeviceParameters * pNewParameters, Bool bForceRecreate );

        // Cursor
    Void _D3D9_SetWindowCursor();
    Void _D3D9_CreateCursor();
    Void _D3D9_UpdateCursorState();

    // Device state management helpers
    Void _ApplyDefaultRenderStates();
    Void _ApplyDefaultSamplerStates();

    Void _DestroyResources();
    Void _RecreateResources();
    Void _UpdateShaderVersions();

    D3D9DeviceResourceCallback m_pfOnDestroyResources;
    D3D9DeviceResourceCallback m_pfOnRecreateResources;
    Void * m_pResourceUserData;
    D3D9DeviceShaderVersionCallback m_pfOnShaderVersionUpdate;
    Void * m_pShaderVersionUserData;

    // GDIWindow instance
    GDIWindow * m_pWindow;

    // Direct3D9 data
    IDirect3D9 * m_pDriver;
    D3D9Enumeration m_eDevices;
    D3D9DeviceParameters m_devParameters;

    IDirect3DDevice9 * m_pDevice;
    Direct3D9State m_devState;
    D3DCAPS9 m_devCaps;
    D3DSURFACE_DESC m_devBackBufferDesc;
    D3DVIEWPORT9 m_devViewport;
    
    // Renderer & Samplers states
    D3D9RenderState m_CurrentRS;
    D3D9SamplerState m_arrCurrentVSState[DIRECT3D9_MAX_VERTEX_SAMPLERS];
    D3D9SamplerState m_arrCurrentPSState[DIRECT3D9_MAX_PIXEL_SAMPLERS];
    UInt m_iMaxVSSamplers;
    UInt m_iMaxPSSamplers;
    UInt m_iMaxTextureUnits;

    // Cursor data
    UInt m_iCursorWidth;
    UInt m_iCursorHeight;
    const DWord * m_pCursorImage;
    UInt m_iCursorHotSpotX;
    UInt m_iCursorHotSpotY;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Direct3D9.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_DIRECT3D9_H

