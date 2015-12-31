/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Resources/D3D9Textures.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant resource for Direct3D9, Textures
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
#include "D3D9Textures.h"

#include "../Direct3D9.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D9Texture1D implementation
D3D9Texture1D::D3D9Texture1D( Direct3D9Renderer * pRenderer, TextureUsage iUsage, UInt iMipMapLevels, TextureFormat iFormat,
                                                             UInt iWidth )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    DWord dwUsage = Direct3D9TextureUsage[iUsage];
    D3DFORMAT d3dFormat = Direct3D9TextureFormat[iFormat];

    m_pTexture = NULL;
    HRESULT hRes = pDevice->CreateTexture( iWidth, 1, iMipMapLevels, dwUsage, d3dFormat, D3DPOOL_DEFAULT, &m_pTexture, NULL );
    DebugAssert( hRes == D3D_OK );
}
D3D9Texture1D::~D3D9Texture1D()
{
    m_pTexture->Release();
}

Void D3D9Texture1D::Initialize( Direct3D9Renderer * pRenderer, const Byte * arrData )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    // Get level 0 surface descriptor
    D3DSURFACE_DESC d3dSurfaceDesc;
    HRESULT hRes = m_pTexture->GetLevelDesc( 0, &d3dSurfaceDesc );
    DebugAssert( hRes == D3D_OK );

    // Create system memory texture
    IDirect3DTexture9 * pSysMemTexture = NULL;
    hRes = pDevice->CreateTexture( d3dSurfaceDesc.Width, d3dSurfaceDesc.Height, m_pTexture->GetLevelCount(),
                                   d3dSurfaceDesc.Usage, d3dSurfaceDesc.Format, D3DPOOL_SYSTEMMEM, &pSysMemTexture, NULL );
    DebugAssert( hRes == D3D_OK );

    // Copy data to system memory texture using LockRect/UnlockRect
    D3DLOCKED_RECT rectLock;
    hRes = pSysMemTexture->LockRect( 0, &rectLock, NULL, D3DLOCK_DISCARD );
    DebugAssert( hRes == D3D_OK );

    UInt iStride = ( Direct3D9FormatBits(d3dSurfaceDesc.Format) >> 3 );
    UInt iSrcPitch = (iStride == 0) ? (d3dSurfaceDesc.Width >> 1) : (d3dSurfaceDesc.Width * iStride);

    Byte * pScanline = (Byte*)( rectLock.pBits );
    for( UInt i = 0; i < d3dSurfaceDesc.Height; ++i ) {
        MemCopy( pScanline, arrData, iSrcPitch );
        pScanline += rectLock.Pitch;
        arrData += iSrcPitch;
    }

    hRes = pSysMemTexture->UnlockRect( 0 );
    DebugAssert( hRes == D3D_OK );

    // Copy data to video memory texture using UpdateTexture
    hRes = pDevice->UpdateTexture( pSysMemTexture, m_pTexture );
    DebugAssert( hRes == D3D_OK );

    // Destroy system memory texture
    pSysMemTexture->Release();
    pSysMemTexture = NULL;

    // Generate mipmap (if any)
    m_pTexture->GenerateMipSubLevels();
}

Void D3D9Texture1D::Enable( Direct3D9Renderer * pRenderer, UInt iTextureUnit )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetTexture( iTextureUnit, m_pTexture );
    DebugAssert( hRes == D3D_OK );
}
Void D3D9Texture1D::Disable( Direct3D9Renderer * pRenderer, UInt iTextureUnit )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;
    HRESULT hRes;

    hRes = pDevice->SetTexture( iTextureUnit, NULL );
    DebugAssert( hRes == D3D_OK );
}
Void * D3D9Texture1D::Lock( UInt iLevel, TextureLock iLockMode )
{
    D3DLOCKED_RECT rectLock;
    HRESULT hRes = m_pTexture->LockRect( iLevel, &rectLock, NULL, Direct3D9TextureLock[iLockMode] );
    DebugAssert( hRes == D3D_OK );
    return rectLock.pBits;
}
Void D3D9Texture1D::Unlock( UInt iLevel )
{
    HRESULT hRes = m_pTexture->UnlockRect( iLevel );
    DebugAssert( hRes == D3D_OK );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D9Texture2D implementation
D3D9Texture2D::D3D9Texture2D( Direct3D9Renderer * pRenderer, TextureUsage iUsage, UInt iMipMapLevels, TextureFormat iFormat,
                                                             UInt iWidth, UInt iHeight )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    DWord dwUsage = Direct3D9TextureUsage[iUsage];
    D3DFORMAT d3dFormat = Direct3D9TextureFormat[iFormat];

    m_pTexture = NULL;
    HRESULT hRes = pDevice->CreateTexture( iWidth, iHeight, iMipMapLevels, dwUsage, d3dFormat, D3DPOOL_DEFAULT, &m_pTexture, NULL );
    DebugAssert( hRes == D3D_OK );
}
D3D9Texture2D::D3D9Texture2D( Direct3D9Renderer * pRenderer, TextureFormat iFormat, Bool bIsColorTexture, Bool bGenerateMipMaps,
                                                             UInt iWidth, UInt iHeight )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    DWord dwUsage = D3DUSAGE_RENDERTARGET;
    D3DFORMAT d3dFormat = Direct3D9TextureFormat[iFormat];

    if ( bIsColorTexture ) {
        UInt iMipMapLevels = 1;
        if ( bGenerateMipMaps ) {
            iMipMapLevels = 0;
            dwUsage |= D3DUSAGE_AUTOGENMIPMAP;
        }

        m_pTexture = NULL;
        HRESULT hRes = pDevice->CreateTexture( iWidth, iHeight, iMipMapLevels, dwUsage, d3dFormat, D3DPOOL_DEFAULT, &m_pTexture, NULL );
        DebugAssert( hRes == D3D_OK );
    } else {
        m_pTexture = NULL;
        HRESULT hRes = pDevice->CreateTexture( iWidth, iHeight, 1, dwUsage, d3dFormat, D3DPOOL_DEFAULT, &m_pTexture, NULL );
        DebugAssert( hRes == D3D_OK );
    }
}
D3D9Texture2D::~D3D9Texture2D()
{
    m_pTexture->Release();
}

Void D3D9Texture2D::Initialize( Direct3D9Renderer * pRenderer, const Byte * arrData )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    // Get level 0 surface descriptor
    D3DSURFACE_DESC d3dSurfaceDesc;
    HRESULT hRes = m_pTexture->GetLevelDesc( 0, &d3dSurfaceDesc );
    DebugAssert( hRes == D3D_OK );

    // Create system memory texture
    IDirect3DTexture9 * pSysMemTexture = NULL;
    hRes = pDevice->CreateTexture( d3dSurfaceDesc.Width, d3dSurfaceDesc.Height, m_pTexture->GetLevelCount(),
                                   d3dSurfaceDesc.Usage, d3dSurfaceDesc.Format, D3DPOOL_SYSTEMMEM, &pSysMemTexture, NULL );
    DebugAssert( hRes == D3D_OK );

    // Copy data to system memory texture using LockRect/UnlockRect
    D3DLOCKED_RECT rectLock;
    hRes = pSysMemTexture->LockRect( 0, &rectLock, NULL, D3DLOCK_DISCARD );
    DebugAssert( hRes == D3D_OK );

    UInt iStride = ( Direct3D9FormatBits(d3dSurfaceDesc.Format) >> 3 );
    UInt iSrcPitch = (iStride == 0) ? (d3dSurfaceDesc.Width >> 1) : (d3dSurfaceDesc.Width * iStride);

    Byte * pScanline = (Byte*)( rectLock.pBits );
    for( UInt i = 0; i < d3dSurfaceDesc.Height; ++i ) {
        MemCopy( pScanline, arrData, iSrcPitch );
        pScanline += rectLock.Pitch;
        arrData += iSrcPitch;
    }

    hRes = pSysMemTexture->UnlockRect( 0 );
    DebugAssert( hRes == D3D_OK );

    // Copy data to video memory texture using UpdateTexture
    hRes = pDevice->UpdateTexture( pSysMemTexture, m_pTexture );
    DebugAssert( hRes == D3D_OK );

    // Destroy system memory texture
    pSysMemTexture->Release();
    pSysMemTexture = NULL;

    // Generate mipmap (if any)
    m_pTexture->GenerateMipSubLevels();
}

Void D3D9Texture2D::Enable( Direct3D9Renderer * pRenderer, UInt iTextureUnit )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetTexture( iTextureUnit, m_pTexture );
    DebugAssert( hRes == D3D_OK );
}
Void D3D9Texture2D::Disable( Direct3D9Renderer * pRenderer, UInt iTextureUnit )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;
    HRESULT hRes;

    hRes = pDevice->SetTexture( iTextureUnit, NULL );
    DebugAssert( hRes == D3D_OK );
}
Void * D3D9Texture2D::Lock( UInt iLevel, TextureLock iLockMode, UInt * outPitch )
{
    D3DLOCKED_RECT rectLock;
    HRESULT hRes = m_pTexture->LockRect( iLevel, &rectLock, NULL, Direct3D9TextureLock[iLockMode] );
    DebugAssert( hRes == D3D_OK );
    *outPitch = rectLock.Pitch;
    return rectLock.pBits;
}
Void D3D9Texture2D::Unlock( UInt iLevel )
{
    HRESULT hRes = m_pTexture->UnlockRect( iLevel );
    DebugAssert( hRes == D3D_OK );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D9Texture3D implementation
D3D9Texture3D::D3D9Texture3D( Direct3D9Renderer * pRenderer, TextureUsage iUsage, UInt iMipMapLevels, TextureFormat iFormat,
                                                             UInt iWidth, UInt iHeight, UInt iDepth )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    DWord dwUsage = Direct3D9TextureUsage[iUsage];
    D3DFORMAT d3dFormat = Direct3D9TextureFormat[iFormat];

    m_pTexture = NULL;
    HRESULT hRes = pDevice->CreateVolumeTexture( iWidth, iHeight, iDepth, iMipMapLevels, dwUsage, d3dFormat,
                                                 D3DPOOL_DEFAULT, &m_pTexture, NULL );
    DebugAssert( hRes == D3D_OK );
}
D3D9Texture3D::~D3D9Texture3D()
{
    m_pTexture->Release();
}

Void D3D9Texture3D::Initialize( Direct3D9Renderer * pRenderer, const Byte * arrData )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    // Get level 0 surface descriptor
    D3DVOLUME_DESC d3dVolumeDesc;
    HRESULT hRes = m_pTexture->GetLevelDesc( 0, &d3dVolumeDesc );
    DebugAssert( hRes == D3D_OK );

    // Create system memory texture
    IDirect3DVolumeTexture9 * pSysMemTexture = NULL;
    hRes = pDevice->CreateVolumeTexture( d3dVolumeDesc.Width, d3dVolumeDesc.Height, d3dVolumeDesc.Depth, m_pTexture->GetLevelCount(),
                                         d3dVolumeDesc.Usage, d3dVolumeDesc.Format, D3DPOOL_SYSTEMMEM, &pSysMemTexture, NULL );
    DebugAssert( hRes == D3D_OK );

    // Copy data to system memory texture using LockBox/UnlockBox
    D3DLOCKED_BOX boxLock;
    hRes = pSysMemTexture->LockBox( 0, &boxLock, NULL, D3DLOCK_DISCARD );
    DebugAssert( hRes == D3D_OK );

    const Byte * pSrcScanline;
    UInt iStride = ( Direct3D9FormatBits(d3dVolumeDesc.Format) >> 3 );
    UInt iSrcPitch = (iStride == 0) ? (d3dVolumeDesc.Width >> 1) : (d3dVolumeDesc.Width * iStride);
    UInt iSrcSlice = d3dVolumeDesc.Height * iSrcPitch;

    Byte * pSlice = (Byte*)( boxLock.pBits );
    Byte * pDstScanline;
    for( UInt i = 0; i < d3dVolumeDesc.Depth; ++i ) {
        pDstScanline = pSlice;
        pSrcScanline = arrData;
        for( UInt j = 0; j < d3dVolumeDesc.Height; ++j ) {
            MemCopy( pDstScanline, pSrcScanline, iSrcPitch );
            pDstScanline += boxLock.RowPitch;
            pSrcScanline += iSrcPitch;
        }
        pSlice += boxLock.SlicePitch;
        arrData += iSrcSlice;
    }

    hRes = pSysMemTexture->UnlockBox( 0 );
    DebugAssert( hRes == D3D_OK );

    // Copy data to video memory texture using UpdateTexture
    hRes = pDevice->UpdateTexture( pSysMemTexture, m_pTexture );
    DebugAssert( hRes == D3D_OK );

    // Destroy system memory texture
    pSysMemTexture->Release();
    pSysMemTexture = NULL;

    // Generate mipmap (if any)
    m_pTexture->GenerateMipSubLevels();
}

Void D3D9Texture3D::Enable( Direct3D9Renderer * pRenderer, UInt iTextureUnit )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetTexture( iTextureUnit, m_pTexture );
    DebugAssert( hRes == D3D_OK );
}
Void D3D9Texture3D::Disable( Direct3D9Renderer * pRenderer, UInt iTextureUnit )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;
    HRESULT hRes;

    hRes = pDevice->SetTexture( iTextureUnit, NULL );
    DebugAssert( hRes == D3D_OK );
}
Void * D3D9Texture3D::Lock( UInt iLevel, TextureLock iLockMode, UInt * outSlicePitch, UInt * outRowPitch )
{
    D3DLOCKED_BOX boxLock;
    HRESULT hRes = m_pTexture->LockBox( iLevel, &boxLock, NULL, Direct3D9TextureLock[iLockMode] );
    DebugAssert( hRes == D3D_OK );
    *outSlicePitch = boxLock.SlicePitch;
    *outRowPitch = boxLock.RowPitch;
    return boxLock.pBits;
}
Void D3D9Texture3D::Unlock( UInt iLevel )
{
    HRESULT hRes = m_pTexture->UnlockBox( iLevel );
    DebugAssert( hRes == D3D_OK );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D9TextureCube implementation
D3D9TextureCube::D3D9TextureCube( Direct3D9Renderer * pRenderer, TextureUsage iUsage, UInt iMipMapLevels, TextureFormat iFormat,
                                                                 UInt iEdgeLength )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    DWord dwUsage = Direct3D9TextureUsage[iUsage];
    D3DFORMAT d3dFormat = Direct3D9TextureFormat[iFormat];

    m_pTexture = NULL;
    HRESULT hRes = pDevice->CreateCubeTexture( iEdgeLength, iMipMapLevels, dwUsage, d3dFormat, D3DPOOL_DEFAULT, &m_pTexture, NULL );
    DebugAssert( hRes == D3D_OK );
}
D3D9TextureCube::~D3D9TextureCube()
{
    m_pTexture->Release();
}

Void D3D9TextureCube::Initialize( Direct3D9Renderer * pRenderer, const Byte * arrData )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    UInt iLevelCount = m_pTexture->GetLevelCount();

    // Get level 0 surface descriptor
    D3DSURFACE_DESC d3dSurfaceDesc;
    HRESULT hRes = m_pTexture->GetLevelDesc( 0, &d3dSurfaceDesc );
    DebugAssert( hRes == D3D_OK );

    // Create system memory texture
    IDirect3DCubeTexture9 * pSysMemTexture = NULL;
    hRes = pDevice->CreateCubeTexture( d3dSurfaceDesc.Width, iLevelCount, d3dSurfaceDesc.Usage,
                                       d3dSurfaceDesc.Format, D3DPOOL_SYSTEMMEM, &pSysMemTexture, NULL );
    DebugAssert( hRes == D3D_OK );

    // Copy data to system memory texture using LockRect/UnlockRect
    UInt iStride = ( Direct3D9FormatBits(d3dSurfaceDesc.Format) >> 3 );
    UInt iSrcPitch = (iStride == 0) ? (d3dSurfaceDesc.Width >> 1) : (d3dSurfaceDesc.Width * iStride);
    UInt iShiftSize = 0, iTmp = ( (d3dSurfaceDesc.Height * iSrcPitch) >> 2 );
    for( UInt iLevel = 1; iLevel < iLevelCount; ++iLevel ) {
        iShiftSize += iTmp;
        iTmp >>= 2;
    }

    for( UInt iFace = 0; iFace < 6; ++iFace ) {
        D3DLOCKED_RECT rectLock;
        hRes = pSysMemTexture->LockRect( (D3DCUBEMAP_FACES)iFace, 0, &rectLock, NULL, D3DLOCK_DISCARD );
        DebugAssert( hRes == D3D_OK );

        Byte * pScanline = (Byte*)( rectLock.pBits );
        for( UInt i = 0; i < d3dSurfaceDesc.Height; ++i ) {
            MemCopy( pScanline, arrData, iSrcPitch );
            pScanline += rectLock.Pitch;
            arrData += iSrcPitch;
        }
        arrData += iShiftSize;

        hRes = pSysMemTexture->UnlockRect( (D3DCUBEMAP_FACES)iFace, 0 );
        DebugAssert( hRes == D3D_OK );
    }

    // Copy data to video memory texture using UpdateTexture
    hRes = pDevice->UpdateTexture( pSysMemTexture, m_pTexture );
    DebugAssert( hRes == D3D_OK );

    // Destroy system memory texture
    pSysMemTexture->Release();
    pSysMemTexture = NULL;

    // Generate mipmap (if any)
    m_pTexture->GenerateMipSubLevels();
}

Void D3D9TextureCube::Enable( Direct3D9Renderer * pRenderer, UInt iTextureUnit )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetTexture( iTextureUnit, m_pTexture );
    DebugAssert( hRes == D3D_OK );
}
Void D3D9TextureCube::Disable( Direct3D9Renderer * pRenderer, UInt iTextureUnit )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;
    HRESULT hRes;

    hRes = pDevice->SetTexture( iTextureUnit, NULL );
    DebugAssert( hRes == D3D_OK );
}
Void * D3D9TextureCube::Lock( TextureCubeFace iFace, UInt iLevel, TextureLock iLockMode, UInt * outPitch )
{
    D3DCUBEMAP_FACES d3dFace = Direct3D9TextureCubeFace[iFace];

    D3DLOCKED_RECT rectLock;
    HRESULT hRes = m_pTexture->LockRect( d3dFace, iLevel, &rectLock, NULL, Direct3D9TextureLock[iLockMode] );
    DebugAssert( hRes == D3D_OK );
    *outPitch = rectLock.Pitch;
    return rectLock.pBits;
}
Void D3D9TextureCube::Unlock( TextureCubeFace iFace, UInt iLevel )
{
    D3DCUBEMAP_FACES d3dFace = Direct3D9TextureCubeFace[iFace];

    HRESULT hRes = m_pTexture->UnlockRect( d3dFace, iLevel );
    DebugAssert( hRes == D3D_OK );
}
