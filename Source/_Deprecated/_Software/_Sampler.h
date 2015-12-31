/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Sampler.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Texture advanced sampling, mip-mapping.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef KOALA_ENGINE_GRAPHICS_3D_SAMPLER_H
#define KOALA_ENGINE_GRAPHICS_3D_SAMPLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Buffering.h"
#include "Texture.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define SAMPLER_MIPMAP_MAX_LEVELS 16

enum SamplerType
{
    SAMPLER_UNDEFINED = 0,
    SAMPLER_1D,     // 1D
    SAMPLER_2D,     // 2D
    SAMPLER_3D,     // 3D
    SAMPLER_CUBE,   // 2D (set of 2D images)
    SAMPLER_PROJ    // 2D
};

/////////////////////////////////////////////////////////////////////////////////
// The Sampler class
class Sampler
{
public:
    Sampler( MemoryAllocatorID idAllocator, const Texture * pTexture );
    virtual ~Sampler();

    virtual SamplerType GetType() const = 0;
    virtual UInt GetDimension() const = 0;

    // write to texture support to allow dynamic rendering
    virtual Void WriteToTexture( const Color4 * pColorImage );
    virtual Void WriteToTexture( const Scalar * pDepthImage );

    inline Bool UseMipMapping() const;
    virtual Void DisableMipMapping() = 0;

    virtual Void ComputeMipmapParameters( CCFLPtr pPixelA, CCFLPtr pPixelB, CCFLPtr pPixelC,
                                          const CustomFragmentLayout * pCFL, UInt iTextureChannel );
    virtual Void SetPixel( UInt iX, UInt iY ) = 0;
    virtual Void TexelSample( Color4 & outColor, CCFLPtr pTexCoords ) = 0;

protected:
    // Internal helpers
    Scalar _GetImageCoord( UInt iDim, Scalar fCoord, UInt iBound ) const;
    Void _ClampLevels();

    // Mipmap level builders, builds a new level from previous one.
    Void _CreateMipmap1D( UInt iLevel, UInt iWidth, UInt * outWidths );
    Void _RecreateMipmap1D( UInt iLevel, UInt iWidth );

    Void _CreateMipmap2D( UInt iLevel, UInt iWidth, UInt iHeight, UInt iLog2Width,
                          UInt iParentWidth, UInt iParentLog2Width,
                          UInt * outWidths, UInt * outHeights, UInt * outLog2Widths );
    Void _RecreateMipmap2D( UInt iLevel, UInt iWidth, UInt iHeight, UInt iLog2Width,
                            UInt iParentWidth, UInt iParentLog2Width );

    Void _CreateMipmap3D( UInt iLevel, UInt iWidth, UInt iHeight, UInt iDepth, UInt iLog2Width, UInt iLog2Height,
                          UInt iParentWidth, UInt iParentWidthHeight, UInt iParentLog2Width, UInt iParentLog2Height,
                          UInt * outWidths, UInt * outHeights, UInt * outDepths, UInt * outLog2Widths, UInt * outLog2Heights );
    Void _RecreateMipmap3D( UInt iLevel, UInt iWidth, UInt iHeight, UInt iDepth, UInt iLog2Width, UInt iLog2Height,
                            UInt iParentWidth, UInt iParentWidthHeight, UInt iParentLog2Width, UInt iParentLog2Height );

    // Data
    const MemoryAllocatorID m_idAllocator;
    const Texture * m_pTexture;

    Color4 * m_arrMipMaps[SAMPLER_MIPMAP_MAX_LEVELS];
    UInt m_iMipMapLevels;
    Scalar m_fCurLevel, m_fDLvl;
    UInt m_iLevelLow, m_iLevelHigh;
    Color4 * m_pCurMipMap;
    Color4 *m_pMipMapLow, *m_pMipMapHigh;
    Bool m_bUseMipMapping;

    // Parameters
    Scalar m_fMulA0, m_fMulA1, m_fMulA2;
    Scalar m_fMulB0, m_fMulB1, m_fMulB2;
    Scalar m_fMulC0, m_fMulC1, m_fMulC2;
    Scalar m_fD, m_fE, m_fF;
};

/////////////////////////////////////////////////////////////////////////////////
// The Sampler1D class
class Sampler1D : public Sampler
{
public:
    Sampler1D( MemoryAllocatorID idAllocator, const Texture * pTexture );
    ~Sampler1D();

    // overrides
    inline virtual SamplerType GetType() const;
    inline virtual UInt GetDimension() const;

    virtual Void WriteToTexture( const Color4 * pColorImage );

    virtual Void DisableMipMapping();

    virtual Void ComputeMipmapParameters( CCFLPtr pPixelA, CCFLPtr pPixelB, CCFLPtr pPixelC,
                                          const CustomFragmentLayout * pCFL, UInt iTextureChannel );    
    virtual Void SetPixel( UInt iX, UInt iY );
    virtual Void TexelSample( Color4 & outColor, CCFLPtr pTexCoords );

private:
    Void _GenerateMipMaps();
    Void _RegenerateMipMaps();

    Void _NearestColor( Color4 & outColor, Scalar fImageX ) const;
    Void _LinearColor( Color4 & outColor, Scalar fImageX ) const;

    UInt * m_arrWidths;
    UInt m_iCurWidth;
    UInt m_iWidthLow, m_iWidthHigh;

    // Parameters
    Scalar m_fA, m_fB, m_fC;
    Scalar m_fAEmBD, m_fAFmCD, m_fBFmCE;
};

/////////////////////////////////////////////////////////////////////////////////
// The Sampler2D class
class Sampler2D : public Sampler
{
public:
    Sampler2D( MemoryAllocatorID idAllocator, const Texture * pTexture );
    ~Sampler2D();

    // overrides
    inline virtual SamplerType GetType() const;
    inline virtual UInt GetDimension() const;

    virtual Void WriteToTexture( const Color4 * pColorImage );

    virtual Void DisableMipMapping();

    virtual Void ComputeMipmapParameters( CCFLPtr pPixelA, CCFLPtr pPixelB, CCFLPtr pPixelC,
                                          const CustomFragmentLayout * pCFL, UInt iTextureChannel );    
    virtual Void SetPixel( UInt iX, UInt iY );
    virtual Void TexelSample( Color4 & outColor, CCFLPtr pTexCoords );

private:
    inline UInt _GetIndex( UInt iX, UInt iY ) const;
    Void _GenerateMipMaps();
    Void _RegenerateMipMaps();

    Void _NearestColor( Color4 & outColor, Scalar fImageX, Scalar fImageY ) const;
    Void _LinearColor( Color4 & outColor, Scalar fImageX, Scalar fImageY ) const;

    UInt * m_arrWidths;
    UInt * m_arrHeights;
    UInt * m_arrLog2Widths;
    UInt m_iCurWidth, m_iCurHeight, m_iCurLog2Width;
    UInt m_iWidthLow, m_iWidthHigh;
    UInt m_iHeightLow, m_iHeightHigh;
    UInt m_iLog2WidthLow, m_iLog2WidthHigh;

    // Parameters
    Scalar m_fA[2], m_fB[2], m_fC[2];
    Scalar m_fAEmBD[2], m_fAFmCD[2], m_fBFmCE[2];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Sampler.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_SAMPLER_H

