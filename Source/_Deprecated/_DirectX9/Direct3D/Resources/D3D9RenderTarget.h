/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Resources/D3D9RenderTarget.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant resource for Direct3D9, RenderTarget
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
#ifndef SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_RESOURCES_D3D9RENDERTARGET_H
#define SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_RESOURCES_D3D9RENDERTARGET_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Mappings.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class Direct3D9Renderer;

/////////////////////////////////////////////////////////////////////////////////
// The D3D9RenderTarget class
class D3D9RenderTarget
{
public:
    D3D9RenderTarget( Direct3D9Renderer * pRenderer, UInt iTargetCount, TextureFormat iFormat, TextureFormat iDepthStencilFormat,
                                                     Bool bUseDepthStencil, Bool bGenerateMipMaps,
                                                     UInt iWidth, UInt iHeight );
    ~D3D9RenderTarget();

    Void Enable( Direct3D9Renderer * pRenderer );
    Void Disable( Direct3D9Renderer * pRenderer );

    Void * LockColorSurface( Direct3D9Renderer * pRenderer, UInt iTarget, UInt * outPitch );
    Void UnLockColorSurface( Direct3D9Renderer * pRenderer, UInt iTarget );

private:
    UInt m_iTargetsCount;
    TextureFormat m_iFormat;
    Bool m_bHasDepthStencil;
    TextureFormat m_iDepthStencilFormat;
    Bool m_bGenerateMipMaps;
    UInt m_iWidth, m_iHeight;

    IDirect3DTexture9 * m_arrColorTextures[RENDERTARGET_MAX_TARGETS];
    IDirect3DTexture9 * m_pDepthStencilTexture;

    IDirect3DSurface9 * m_arrColorSurfaces[RENDERTARGET_MAX_TARGETS];    
    IDirect3DSurface9 * m_pDepthStencilSurface;

    // Backbuffer saving
    IDirect3DSurface9 * m_pSavedColorSurface;
    IDirect3DSurface9 * m_pSavedDepthStencilSurface;

    // System memory copies
    IDirect3DTexture9 * m_arrCopyColorTextures[RENDERTARGET_MAX_TARGETS];

    IDirect3DSurface9 * m_arrCopyColorSurfaces[RENDERTARGET_MAX_TARGETS];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D9RenderTarget.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_RESOURCES_D3D9RENDERTARGET_H

