/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Texture.h
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
// Known Bugs : Missing anisotropic filter and some more advanced ones ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef KOALA_ENGINE_GRAPHICS_3D_TEXTURE_H
#define KOALA_ENGINE_GRAPHICS_3D_TEXTURE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Core/Error/ErrorManager.h"

#include "../../../Lib/Core/Memory/MemoryManager.h"

#include "../../../Lib/Core/Math/Types/TextureCoord.h"
#include "../../../Lib/Core/Math/Types/Color.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define TEXTURE_MAX_DIMENSIONS 3

enum TextureFormat
{
    TEXTURE_FORMAT_UNDEFINED = 0,
        // color formats
    TEXTURE_FORMAT_R5G6B5,         // Bpp = 2
    TEXTURE_FORMAT_R5G5B5A1,       // Bpp = 2
    TEXTURE_FORMAT_R4G4B4A4,       // Bpp = 2
    TEXTURE_FORMAT_R8G8B8,         // Bpp = 3
    TEXTURE_FORMAT_R8G8B8A8,       // Bpp = 4
        // raw formats
    TEXTURE_FORMAT_RGB16I,         // Bpp = 6
    TEXTURE_FORMAT_RGBA16I,        // Bpp = 8
    TEXTURE_FORMAT_RGB16F,         // Bpp = 6
    TEXTURE_FORMAT_RGBA16F,        // Bpp = 8
    TEXTURE_FORMAT_RGB32I,         // Bpp = 12
    TEXTURE_FORMAT_RGBA32I,        // Bpp = 16
    TEXTURE_FORMAT_RGB32F,         // Bpp = 12
    TEXTURE_FORMAT_RGBA32F,        // Bpp = 16
        // depth formats
    TEXTURE_FORMAT_DEPTH16I,       // Bpp = 2
    TEXTURE_FORMAT_DEPTH24I,       // Bpp = 3
    TEXTURE_FORMAT_DEPTH32I,       // Bpp = 4
    TEXTURE_FORMAT_DEPTH32F,       // Bpp = 4
        // intensity formats
    TEXTURE_FORMAT_INTENSITY8I,    // Bpp = 1
    TEXTURE_FORMAT_INTENSITY16I,   // Bpp = 2
    TEXTURE_FORMAT_INTENSITY16F,   // Bpp = 2
    TEXTURE_FORMAT_INTENSITY32I,   // Bpp = 4
    TEXTURE_FORMAT_INTENSITY32F,   // Bpp = 4
        // cubic textures
    TEXTURE_FORMAT_CUBIC_R8G8B8,   // Bpp = 3
    TEXTURE_FORMAT_CUBIC_R8G8B8A8, // Bpp = 4

    TEXTURE_FORMAT_COUNT
};

enum TextureFilter
{
    TEXTURE_FILTER_NEAREST = 0,
    TEXTURE_FILTER_LINEAR,
    TEXTURE_FILTER_NEAREST_NEAREST, //
    TEXTURE_FILTER_NEAREST_LINEAR,  // for these, filter using :
    TEXTURE_FILTER_LINEAR_NEAREST,  // "within texture" _ "across textures"
    TEXTURE_FILTER_LINEAR_LINEAR,   //
    //TEXTURE_FILTER_ANISOTROPIC, // unimplemented yet
    TEXTURE_FILTER_COUNT
};

enum TextureWrapMode
{
    TEXTURE_WRAP_CLAMP = 0,     // clamp
    TEXTURE_WRAP_BORDER,        // use border texel data
    TEXTURE_WRAP_EDGE,          // use edge's last texel
    TEXTURE_WRAP_REPEAT,        // repeat texels by cycling
    TEXTURE_WRAP_REPEAT_MIRROR, // repeat texels in reverse order
    TEXTURE_WRAP_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The TextureData class
class TextureData
{
public:
    // statics
    inline static UInt GetBytesPerPixel( TextureFormat iFormat );
    inline static const KChar * GetFormatName( TextureFormat iFormat );

    // constructors / destructor
    TextureData( TextureFormat iFormat, Byte * pData, UInt iWidth );
    TextureData( TextureFormat iFormat, Byte * pData, UInt iWidth, UInt iHeight );
    TextureData( TextureFormat iFormat, Byte * pData, UInt iWidth, UInt iHeight, UInt iDepth );
    ~TextureData();

    // getters
    inline Byte * GetData() const;
    inline Byte * GetData( UInt iOffset ) const;
    inline UInt GetSize() const;
    inline UInt GetDimension() const;
    inline UInt GetBound( UInt iBound ) const;
    inline UInt GetBytesPerPixel() const;
    inline TextureFormat GetFormat() const;
    inline const KChar * GetFormatName() const;

    inline Bool IsDepth() const;
    inline Bool IsCube() const;

    // methods
    Void Allocate( MemoryAllocatorID idAllocator );
    Void Destroy( MemoryAllocatorID idAllocator );
    Void CopyRGBA( Color4 * pData ) const;

private:
    static const UInt sm_arrBytesPerPixel[TEXTURE_FORMAT_COUNT];
    static const KChar * sm_arrNames[TEXTURE_FORMAT_COUNT];

    Byte * m_pBuffer;
    UInt m_iSize;
    UInt m_iDimension;
    UInt m_iBounds[TEXTURE_MAX_DIMENSIONS];
    TextureFormat m_iFormat;
};

/////////////////////////////////////////////////////////////////////////////////
// The Texture class
class Texture
{
public:
    // statics
    inline static const KChar * GetFilterName( TextureFilter iFilter );
    inline static const KChar * GetWrapModeName( TextureWrapMode iWrapMode );

    Texture( const KChar * strName, TextureData * pImage );
    ~Texture();

    // ID & data
    inline const KChar * GetName() const;
    inline TextureData * GetData() const;
    inline Void SetData( TextureData * pImage );

    // getters
    inline TextureFilter GetFilter() const;
    inline const KChar * GetFilterName() const;
    inline TextureWrapMode GetWrapMode( UInt iDim ) const;
    inline const KChar * GetWrapModeName( UInt iDim ) const;
    inline const Color4 & GetBorderColor() const;
    inline Scalar GetAnisotropy() const;
    inline Bool IsOffscreen() const;

    // setters
    inline Void SetFilter( TextureFilter iFilter );
    inline Void SetWrapMode( TextureWrapMode iWrapMode, UInt iDim );
    inline Void SetBorderColor( const Color4 & colBorder );
    inline Void SetAnisotropy( Scalar fAnisotropy );
    inline Void SetOffscreen( Bool bOffscreen );

private:
    static const KChar * sm_arrFilterNames[TEXTURE_FILTER_COUNT];
    static const KChar * sm_arrWrapNames[TEXTURE_WRAP_COUNT];

    const KChar * m_strName;
    TextureData * m_pImage;
    TextureFilter m_iFilter;                         // default = LINEAR
    TextureWrapMode m_iWrap[TEXTURE_MAX_DIMENSIONS]; // default = EDGE
    Color4 m_colBorder;                              // default = Black
    Scalar m_fAnisotropy;                            // default = 1.0f
    Bool m_bOffscreen;                               // default = false
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Texture.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_TEXTURE_H
