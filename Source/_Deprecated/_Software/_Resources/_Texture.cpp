/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Texture.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Universal Texture class.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None / TODO = Move this file to the 3D section
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Texture.h"

/////////////////////////////////////////////////////////////////////////////////
// TextureData implementation
const UInt TextureData::sm_arrBytesPerPixel[TEXTURE_FORMAT_COUNT] = {
    0, 2, 2, 2, 3, 4, 6, 8, 6, 8, 12, 16, 12, 16, 2, 3, 4, 4, 1, 2, 2, 4, 4, 3, 4
};
const KChar * TextureData::sm_arrNames[TEXTURE_FORMAT_COUNT] = {
    TEXT("<UNDEFINED>"),
    TEXT("TexFmt_R5G6B5"), TEXT("TexFmt_R5G5B5A1"), TEXT("TexFmt_R4G4B4A4"), TEXT("TexFmt_R8G8B8"), TEXT("TexFmt_R8G8B8A8"),
    TEXT("TexFmt_RGB16i"), TEXT("TexFmt_RGBA16i"), TEXT("TexFmt_RGB16f"), TEXT("TexFmt_RGBA16f"),
    TEXT("TexFmt_RGB32i"), TEXT("TexFmt_RGBA32i"), TEXT("TexFmt_RGB32f"), TEXT("TexFmt_RGBA32f"),
    TEXT("TexFmt_Depth16i"), TEXT("TexFmt_Depth24i"), TEXT("TexFmt_Depth32i"), TEXT("TexFmt_Depth32f"),
    TEXT("TexFmt_Intensity8i"), TEXT("TexFmt_Intensity16i"), TEXT("TexFmt_Intensity16f"),
    TEXT("TexFmt_Intensity32i"), TEXT("TexFmt_Intensity32f"),
    TEXT("TexFmt_CubicRGB"), TEXT("TexFmt_CubicRGBA")
};

TextureData::TextureData( TextureFormat iFormat, Byte * pData, UInt iWidth )
{
    m_iFormat = iFormat;
    m_iDimension = 1;
    m_iBounds[0] = iWidth;
    m_iBounds[1] = 0;
    m_iBounds[2] = 0;
    m_iSize = m_iBounds[0];
    m_pBuffer = pData;
}
TextureData::TextureData( TextureFormat iFormat, Byte * pData, UInt iWidth, UInt iHeight )
{
    m_iFormat = iFormat;
    m_iDimension = 2;
    m_iBounds[0] = iWidth;
    m_iBounds[1] = iHeight;
    m_iBounds[2] = 0;
    m_iSize = m_iBounds[0] * m_iBounds[1];
    m_pBuffer = pData;
}
TextureData::TextureData( TextureFormat iFormat, Byte * pData, UInt iWidth, UInt iHeight, UInt iDepth )
{
    m_iFormat = iFormat;
    m_iDimension = 3;
    m_iBounds[0] = iWidth;
    m_iBounds[1] = iHeight;
    m_iBounds[2] = iDepth;
    m_iSize = m_iBounds[0] * m_iBounds[1] * m_iBounds[2];
    m_pBuffer = pData;
}
TextureData::~TextureData()
{
    // nothing to do
}

Void TextureData::Allocate( MemoryAllocatorID idAllocator )
{
    Assert(m_pBuffer == NULL);
    m_pBuffer = KNewExA(idAllocator) Byte[m_iSize * sm_arrBytesPerPixel[m_iFormat]];
}
Void TextureData::Destroy( MemoryAllocatorID idAllocator )
{
    Assert(m_pBuffer != NULL);
    KDeleteAExA(m_pBuffer, idAllocator);
}

Void TextureData::CopyRGBA( Color4 * pData ) const
{
    Color4 * pDest = pData;
    Scalar fValue;
    UInt i;

    switch( m_iFormat ) {
        case TEXTURE_FORMAT_R5G6B5:
            // unimplemented
            break;
        case TEXTURE_FORMAT_R5G5B5A1:
            // unimplemented
            break;
        case TEXTURE_FORMAT_R4G4B4A4:
            // unimplemented
            break;
        case TEXTURE_FORMAT_R8G8B8: {
                Scalar fFactor = MathFn->Invert( (Scalar)0xfful );
                const Byte * pSrc = (const Byte *)m_pBuffer;
                for(i = 0; i < m_iSize; ++i) {
                    pDest->R = fFactor * (Scalar)( *pSrc++ );
                    pDest->G = fFactor * (Scalar)( *pSrc++ );
                    pDest->B = fFactor * (Scalar)( *pSrc++ );
                    pDest->A = 1.0f;
                    ++pDest;
                }
            } break;
        case TEXTURE_FORMAT_R8G8B8A8: {
                Scalar fFactor = MathFn->Invert( (Scalar)0xfful );
                const Byte * pSrc = (const Byte *)m_pBuffer;
                for(i = 0; i < m_iSize; ++i) {
                    pDest->R = fFactor * (Scalar)( *pSrc++ );
                    pDest->G = fFactor * (Scalar)( *pSrc++ );
                    pDest->B = fFactor * (Scalar)( *pSrc++ );
                    pDest->A = fFactor * (Scalar)( *pSrc++ );
                    ++pDest;
                }
            } break;
        case TEXTURE_FORMAT_RGB16I:
            // unimplemented
            break;
        case TEXTURE_FORMAT_RGBA16I:
            // unimplemented
            break;
        case TEXTURE_FORMAT_RGB16F:
            // unimplemented
            break;
        case TEXTURE_FORMAT_RGBA16F:
            // unimplemented
            break;
        case TEXTURE_FORMAT_RGB32I:
            // unimplemented
            break;
        case TEXTURE_FORMAT_RGBA32I:
            // unimplemented
            break;
        case TEXTURE_FORMAT_RGB32F:
            // unimplemented
            break;
        case TEXTURE_FORMAT_RGBA32F:
            // unimplemented
            break;
        case TEXTURE_FORMAT_DEPTH16I: {
                Scalar fFactor = MathFn->Invert( (Scalar)0xfffful );
                const Word * pSrc = (const Word *)m_pBuffer;
                for(i = 0; i < m_iSize; ++i) {
                    fValue = fFactor * (Scalar)( *pSrc++ );
                    pDest->R = fValue;
                    pDest->G = fValue;
                    pDest->B = fValue;
                    pDest->A = 1.0f;
                    ++pDest;
                }
            } break;
        case TEXTURE_FORMAT_DEPTH24I: {
                Scalar fFactor = MathFn->Invert( (Scalar)0xfffffful );
                const Byte * pSrc = (const Byte *)m_pBuffer;
                for(i = 0; i < m_iSize; ++i) {
                    fValue = fFactor * (Scalar)(  *( (const DWord *)pSrc ) >> 8 );
                    pDest->R = fValue;
                    pDest->G = fValue;
                    pDest->B = fValue;
                    pDest->A = 1.0f;
                    pSrc += 3;
                    ++pDest;
                }
            } break;
        case TEXTURE_FORMAT_DEPTH32I: {
                Scalar fFactor = MathFn->Invert( (Scalar)0xfffffffful );
                const DWord * pSrc = (const DWord *)m_pBuffer;
                for(i = 0; i < m_iSize; ++i) {
                    fValue = fFactor * (Scalar)( *pSrc++ );
                    pDest->R = fValue;
                    pDest->G = fValue;
                    pDest->B = fValue;
                    pDest->A = 1.0f;
                    ++pDest;
                }
            } break;
        case TEXTURE_FORMAT_DEPTH32F: {
                const Scalar * pSrc = (const Scalar *)m_pBuffer;
                for(i = 0; i < m_iSize; ++i) {
                    fValue = *pSrc++;
                    pDest->R = fValue;
                    pDest->G = fValue;
                    pDest->B = fValue;
                    pDest->A = 1.0f;
                    ++pDest;
                }
            } break;
        case TEXTURE_FORMAT_INTENSITY8I: {
                Scalar fFactor = MathFn->Invert( (Scalar)0xfful );
                const Byte * pSrc = (const Byte *)m_pBuffer;
                for(i = 0; i < m_iSize; ++i) {
                    fValue = fFactor * (Scalar)( *pSrc++ );
                    pDest->R = fValue;
                    pDest->G = fValue;
                    pDest->B = fValue;
                    pDest->A = 1.0f;
                    ++pDest;
                }
            } break;
        case TEXTURE_FORMAT_INTENSITY16I: {
                Scalar fFactor = MathFn->Invert( (Scalar)0xfffful );
                const Word * pSrc = (const Word *)m_pBuffer;
                for(i = 0; i < m_iSize; ++i) {
                    fValue = fFactor * (Scalar)( *pSrc++ );
                    pDest->R = fValue;
                    pDest->G = fValue;
                    pDest->B = fValue;
                    pDest->A = 1.0f;
                    ++pDest;
                }
            } break;
        case TEXTURE_FORMAT_INTENSITY16F:
            // unimplemented
            break;
        case TEXTURE_FORMAT_INTENSITY32I: {
                Scalar fFactor = MathFn->Invert( (Scalar)0xfffffffful );
                const DWord * pSrc = (const DWord *)m_pBuffer;
                for(i = 0; i < m_iSize; ++i) {
                    fValue = fFactor * (Scalar)( *pSrc++ );
                    pDest->R = fValue;
                    pDest->G = fValue;
                    pDest->B = fValue;
                    pDest->A = 1.0f;
                    ++pDest;
                }
            } break;
        case TEXTURE_FORMAT_INTENSITY32F: {
                const Scalar * pSrc = (const Scalar *)m_pBuffer;
                for(i = 0; i < m_iSize; ++i) {
                    fValue = *pSrc++;
                    pDest->R = fValue;
                    pDest->G = fValue;
                    pDest->B = fValue;
                    pDest->A = 1.0f;
                    ++pDest;
                }
            } break;
        case TEXTURE_FORMAT_CUBIC_R8G8B8:
            // unimplemented
            break;
        case TEXTURE_FORMAT_CUBIC_R8G8B8A8:
            // unimplemented
            break;
        default: return;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// Texture implementation
const KChar * Texture::sm_arrFilterNames[TEXTURE_FILTER_COUNT] = {
    TEXT("TexFilter_Nearest"),
    TEXT("TexFilter_Linear"),
    TEXT("TexFilter_Nearest_Nearest"),
    TEXT("TexFilter_Nearest_Linear"),
    TEXT("TexFilter_Linear_Nearest"),
    TEXT("TexFilter_Linear_Linear")
};
const KChar * Texture::sm_arrWrapNames[TEXTURE_WRAP_COUNT] = {
    TEXT("TexWrap_Clamp"),
    TEXT("TexWrap_Border"),
    TEXT("TexWrap_Edge"),
    TEXT("TexWrap_Repeat"),
    TEXT("TexWrap_RepeatMirror")
};

Texture::Texture( const KChar * strName, TextureData * pImage ):
    m_strName(strName), m_colBorder(Color4::Black)
{
    m_pImage = pImage;
    m_iFilter = TEXTURE_FILTER_LINEAR;
    m_iWrap[0] = TEXTURE_WRAP_EDGE;
    m_iWrap[1] = TEXTURE_WRAP_EDGE;
    m_iWrap[2] = TEXTURE_WRAP_EDGE;
    m_fAnisotropy = 1.0f;
    m_bOffscreen = false;
}
Texture::~Texture()
{
    // nothing to do
}
