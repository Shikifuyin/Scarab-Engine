/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Resources/D3D9RenderTarget.cpp
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
// Internal dependency : Direct3D9, DirectX
#define WIN32_LEAN_AND_MEAN
#include <d3d9.h>

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D9RenderTarget.h"

#include "../Direct3D9.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D9RenderTarget implementation
D3D9RenderTarget::D3D9RenderTarget( Direct3D9Renderer * pRenderer, UInt iTargetCount, TextureFormat iFormat, TextureFormat iDepthStencilFormat,
                                                                   Bool bUseDepthStencil, Bool bGenerateMipMaps,
                                                                   UInt iWidth, UInt iHeight )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    m_iTargetsCount = iTargetCount;
    m_iFormat = iFormat;
    m_bHasDepthStencil = bUseDepthStencil;
    m_iDepthStencilFormat = iDepthStencilFormat;
    m_bGenerateMipMaps = bGenerateMipMaps;
    m_iWidth = iWidth;
    m_iHeight = iHeight;

    HRESULT hRes;

    UInt iTarget;
    for( iTarget = 0; iTarget < m_iTargetsCount; ++iTarget ) {
        DWord dwUsage = D3DUSAGE_RENDERTARGET;
        D3DFORMAT d3dFormat = Direct3D9TextureFormat[m_iFormat];
        UInt iMipMapLevels = 1;
        if ( m_bGenerateMipMaps ) {
            iMipMapLevels = 0;
            dwUsage |= D3DUSAGE_AUTOGENMIPMAP;
        }

        m_arrColorTextures[iTarget] = NULL;
        hRes = pDevice->CreateTexture( m_iWidth, m_iHeight, iMipMapLevels, dwUsage, d3dFormat, D3DPOOL_DEFAULT,
                                       &(m_arrColorTextures[iTarget]), NULL );
        DebugAssert( hRes == D3D_OK );

        hRes = m_arrColorTextures[iTarget]->GetSurfaceLevel( 0, &(m_arrColorSurfaces[iTarget]) );
        DebugAssert( hRes == D3D_OK );
    }
    for( iTarget = m_iTargetsCount; iTarget < RENDERTARGET_MAX_TARGETS; ++iTarget ) {
        m_arrColorTextures[iTarget] = NULL;
        m_arrColorSurfaces[iTarget] = NULL;
    }

    m_pDepthStencilTexture = NULL;
    m_pDepthStencilSurface = NULL;
    if ( m_bHasDepthStencil ) {
        DWord dwUsage = ( D3DUSAGE_RENDERTARGET | D3DUSAGE_DEPTHSTENCIL );
        D3DFORMAT d3dFormat = Direct3D9TextureFormat[iFormat];

        m_pDepthStencilTexture = NULL;
        hRes = pDevice->CreateTexture( m_iWidth, m_iHeight, 1, dwUsage, d3dFormat, D3DPOOL_DEFAULT,
                                       &m_pDepthStencilTexture, NULL );
        DebugAssert( hRes == D3D_OK );

        hRes = m_pDepthStencilTexture->GetSurfaceLevel( 0, &m_pDepthStencilSurface );
        DebugAssert( hRes == D3D_OK );
    }

    m_pSavedColorSurface = NULL;
    m_pSavedDepthStencilSurface = NULL;

    for( iTarget = 0; iTarget < m_iTargetsCount; ++iTarget ) {
        m_arrCopyColorTextures[iTarget] = NULL;
        m_arrCopyColorSurfaces[iTarget] = NULL;
    }
}
D3D9RenderTarget::~D3D9RenderTarget()
{
    DebugAssert( m_pSavedColorSurface == NULL );
    DebugAssert( m_pSavedDepthStencilSurface == NULL );
    if ( m_bHasDepthStencil ) {
        m_pDepthStencilSurface->Release();
        m_pDepthStencilTexture->Release();
    }
    for( UInt iTarget = 0; iTarget < m_iTargetsCount; ++iTarget ) {
        m_arrColorSurfaces[iTarget]->Release();
        m_arrColorTextures[iTarget]->Release();
    }
}

Void D3D9RenderTarget::Enable( Direct3D9Renderer * pRenderer )
{
    DebugAssert( m_pSavedColorSurface == NULL );
    DebugAssert( m_pSavedDepthStencilSurface == NULL );

    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->GetRenderTarget( 0, &m_pSavedColorSurface );
    DebugAssert( hRes == D3D_OK );
    if ( m_bHasDepthStencil ) {
        hRes = pDevice->GetDepthStencilSurface( &m_pSavedDepthStencilSurface );
        DebugAssert( hRes == D3D_OK );
    }

    for( UInt iTarget = 0; iTarget < m_iTargetsCount; ++iTarget ) {
        hRes = pDevice->SetRenderTarget( iTarget, m_arrColorSurfaces[iTarget] );
        DebugAssert( hRes == D3D_OK );
    }
    if ( m_bHasDepthStencil ) {
        hRes = pDevice->SetDepthStencilSurface( m_pDepthStencilSurface );
        DebugAssert( hRes == D3D_OK );
    }
}
Void D3D9RenderTarget::Disable( Direct3D9Renderer * pRenderer )
{
    DebugAssert( m_pSavedColorSurface != NULL );
    DebugAssert( m_pSavedDepthStencilSurface != NULL );

    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetRenderTarget( 0, m_pSavedColorSurface );
    DebugAssert( hRes == D3D_OK );
    for( UInt iTarget = 1; iTarget < m_iTargetsCount; ++iTarget ) {
        hRes = pDevice->SetRenderTarget( iTarget, NULL );
        DebugAssert( hRes == D3D_OK );
    }
    if ( m_bHasDepthStencil ) {
        hRes = pDevice->SetDepthStencilSurface( m_pSavedDepthStencilSurface );
        DebugAssert( hRes == D3D_OK );
    }

    m_pSavedColorSurface->Release();
    m_pSavedColorSurface = NULL;
    if ( m_bHasDepthStencil ) {
        m_pSavedDepthStencilSurface->Release();
        m_pSavedDepthStencilSurface = NULL;
    }
}

Void * D3D9RenderTarget::LockColorSurface( Direct3D9Renderer * pRenderer, UInt iTarget, UInt * outPitch )
{
    DebugAssert( m_arrCopyColorTextures[iTarget] == NULL );
    DebugAssert( m_arrCopyColorSurfaces[iTarget] == NULL );

    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes;

    // Save primary surface
    m_pSavedColorSurface = NULL;
    if ( iTarget == 0 ) {
        hRes = pDevice->GetRenderTarget( 0, &m_pSavedColorSurface );
        DebugAssert( hRes == D3D_OK );
    }

    // Enable input surface
    hRes = pDevice->SetRenderTarget( iTarget, m_arrColorSurfaces[iTarget] );
    DebugAssert( hRes == D3D_OK );

    // Create system memory
    hRes = pDevice->CreateTexture( m_iWidth, m_iHeight, 0, 0, Direct3D9TextureFormat[m_iFormat],
                                   D3DPOOL_SYSTEMMEM, &(m_arrCopyColorTextures[iTarget]), NULL );
    DebugAssert( hRes == D3D_OK );

    hRes = m_arrCopyColorTextures[iTarget]->GetSurfaceLevel( 0, &(m_arrCopyColorSurfaces[iTarget]) );
    DebugAssert( hRes == D3D_OK );

    // Perform the copy
    hRes = pDevice->GetRenderTargetData( m_arrColorSurfaces[iTarget], m_arrCopyColorSurfaces[iTarget] );
    DebugAssert( hRes == D3D_OK );

    // Lock system memory
    D3DLOCKED_RECT rectLock;
    hRes = m_arrCopyColorSurfaces[iTarget]->LockRect( &rectLock, NULL, 0 );
    DebugAssert( hRes == D3D_OK );

    *outPitch = rectLock.Pitch;
    return rectLock.pBits;
}
Void D3D9RenderTarget::UnLockColorSurface( Direct3D9Renderer * pRenderer, UInt iTarget )
{
    DebugAssert( m_arrCopyColorTextures[iTarget] != NULL );
    DebugAssert( m_arrCopyColorSurfaces[iTarget] != NULL );

    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes;

    // Unlock system memory
    hRes = m_arrCopyColorSurfaces[iTarget]->UnlockRect();
    DebugAssert( hRes == D3D_OK );

    // Destroy system memory
    m_arrCopyColorSurfaces[iTarget]->Release();
    m_arrCopyColorTextures[iTarget]->Release();

    // Restore primary surface
    if ( iTarget == 0 ) {
        DebugAssert( m_pSavedColorSurface != NULL );
        hRes = pDevice->SetRenderTarget( 0, m_pSavedColorSurface );
        DebugAssert( hRes == D3D_OK );

        m_pSavedColorSurface->Release();
        m_pSavedColorSurface = NULL;
    }
}
