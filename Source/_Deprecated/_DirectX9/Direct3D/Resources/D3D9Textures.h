/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Resources/D3D9Textures.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_RESOURCES_D3D9TEXTURES_H
#define SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_RESOURCES_D3D9TEXTURES_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Mappings.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class D3D9RenderTarget;
class Direct3D9Renderer;

/////////////////////////////////////////////////////////////////////////////////
// The D3D9Texture1D class
class D3D9Texture1D
{
public:
    D3D9Texture1D( Direct3D9Renderer * pRenderer, TextureUsage iUsage, UInt iMipMapLevels, TextureFormat iFormat,
                                                  UInt iWidth );
    ~D3D9Texture1D();

    Void Initialize( Direct3D9Renderer * pRenderer, const Byte * arrData );

    Void Enable( Direct3D9Renderer * pRenderer, UInt iTextureUnit );
    Void Disable( Direct3D9Renderer * pRenderer, UInt iTextureUnit );
    Void * Lock( UInt iLevel, TextureLock iLockMode );
    Void Unlock( UInt iLevel );

private:
    IDirect3DTexture9 * m_pTexture;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D9Texture2D class
class D3D9Texture2D
{
public:
    D3D9Texture2D( Direct3D9Renderer * pRenderer, TextureUsage iUsage, UInt iMipMapLevels, TextureFormat iFormat,
                                                  UInt iWidth, UInt iHeight );
    ~D3D9Texture2D();

    Void Initialize( Direct3D9Renderer * pRenderer, const Byte * arrData );

    Void Enable( Direct3D9Renderer * pRenderer, UInt iTextureUnit );
    Void Disable( Direct3D9Renderer * pRenderer, UInt iTextureUnit );
    Void * Lock( UInt iLevel, TextureLock iLockMode, UInt * outPitch );
    Void Unlock( UInt iLevel );

private:
    friend class D3D9RenderTarget;
    friend class Direct3D9Renderer;
    D3D9Texture2D( Direct3D9Renderer * pRenderer, TextureFormat iFormat, Bool bIsColorTexture, Bool bGenerateMipMaps,
                                                  UInt iWidth, UInt iHeight );

    IDirect3DTexture9 * m_pTexture;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D9Texture3D class
class D3D9Texture3D
{
public:
    D3D9Texture3D( Direct3D9Renderer * pRenderer, TextureUsage iUsage, UInt iMipMapLevels, TextureFormat iFormat,
                                                  UInt iWidth, UInt iHeight, UInt iDepth );
    ~D3D9Texture3D();

    Void Initialize( Direct3D9Renderer * pRenderer, const Byte * arrData );

    Void Enable( Direct3D9Renderer * pRenderer, UInt iTextureUnit );
    Void Disable( Direct3D9Renderer * pRenderer, UInt iTextureUnit );
    Void * Lock( UInt iLevel, TextureLock iLockMode, UInt * outSlicePitch, UInt * outRowPitch );
    Void Unlock( UInt iLevel );

private:
    IDirect3DVolumeTexture9 * m_pTexture;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D9TextureCube class
class D3D9TextureCube
{
public:
    D3D9TextureCube( Direct3D9Renderer * pRenderer, TextureUsage iUsage, UInt iMipMapLevels, TextureFormat iFormat,
                                                    UInt iEdgeLength );
    ~D3D9TextureCube();

    Void Initialize( Direct3D9Renderer * pRenderer, const Byte * arrData );

    Void Enable( Direct3D9Renderer * pRenderer, UInt iTextureUnit );
    Void Disable( Direct3D9Renderer * pRenderer, UInt iTextureUnit );
    Void * Lock( TextureCubeFace iFace, UInt iLevel, TextureLock iLockMode, UInt * outPitch );
    Void Unlock( TextureCubeFace iFace, UInt iLevel );

private:
    IDirect3DCubeTexture9 * m_pTexture;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D9Textures.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_RESOURCES_D3D9TEXTURES_H

