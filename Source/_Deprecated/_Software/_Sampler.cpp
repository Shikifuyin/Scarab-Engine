/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Sampler.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Basic classes to handle sampling within the rendering process
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Sampler.h"

/////////////////////////////////////////////////////////////////////////////////
// Sampler implementation
Sampler::Sampler( MemoryAllocatorID idAllocator, const Texture * pTexture ):
    m_idAllocator(idAllocator)
{
    m_pTexture = pTexture;

    // Create level 0
    MemZero( m_arrMipMaps, SAMPLER_MIPMAP_MAX_LEVELS * sizeof(Color4*) );
    UInt iSize = m_pTexture->GetData()->GetSize();
    m_arrMipMaps[0] = KNewExA(m_idAllocator) Color4[iSize];
    m_pTexture->GetData()->CopyRGBA(m_arrMipMaps[0]);

    m_iMipMapLevels = 0;
    m_fCurLevel = 0.0f;
    m_fDLvl = 0.0f;
    m_iLevelLow = 0;
    m_iLevelHigh = 0;
    m_pCurMipMap = NULL;
    m_pMipMapLow = NULL;
    m_pMipMapHigh = NULL;

    m_bUseMipMapping = true;
}
Sampler::~Sampler()
{
    for( UInt i = 0; i < SAMPLER_MIPMAP_MAX_LEVELS; ++i ) {
        if ( m_arrMipMaps[i] != NULL ) {
            KDeleteAExA( m_arrMipMaps[i], m_idAllocator );
            m_arrMipMaps[i] = NULL;
        }
    }
}

Void Sampler::WriteToTexture( const Color4 * pColorImage )
{
    // Child classes will need to recreate mipmaps,
    // here we only write the base level
    TextureData * pImage = m_pTexture->GetData();
    switch(pImage->GetDimension()) {
        case 1: {
            // unimplemented
            } break;
        case 2: {
                UInt iWidth = pImage->GetBound(0);
                UInt iHeight = pImage->GetBound(1);
                UInt iScanLineSize = iWidth * sizeof(Color4);
                Color4 * pDest = m_arrMipMaps[0];
                const Color4 * pSrc = pColorImage;
                for( UInt y = 0; y < iHeight; ++y ) {
                    MemCopy( pDest, pSrc, iScanLineSize );
                    pDest += iWidth;
                    pSrc += iWidth;
                }
            } break;
        case 3: {
            // unimplemented
            } break;
    }
}
Void Sampler::WriteToTexture( const Scalar * pDepthImage )
{
    // Child classes will need to recreate mipmaps,
    // here we only write the base level
    TextureData * pImage = m_pTexture->GetData();
    switch(pImage->GetDimension()) {
        case 1: {
            // unimplemented
            } break;
        case 2: {
                UInt iWidth = pImage->GetBound(0);
                UInt iHeight = pImage->GetBound(1);
                UInt iScanLineSize = iWidth * sizeof(Color4);
                Color4 * pDestLine = m_arrMipMaps[0];
                const Scalar * pSrcLine = pDepthImage;
                Scalar fValue;
                for( UInt y = 0; y < iHeight; ++y ) {
                    Color4 * pDest = pDestLine;
                    const Scalar * pSrc = pSrcLine;
                    for( UInt x = 0; x < iWidth; ++x ) {
                        fValue = *pSrc++;
                        pDest->R = fValue;
                        pDest->G = fValue;
                        pDest->B = fValue;
                        pDest->A = 1.0f;
                        ++pDest;
                    }
                    pDestLine += iWidth;
                    pSrcLine += iWidth;
                }
            } break;
        case 3: {
            // unimplemented
            } break;
    }
}

Void Sampler::ComputeMipmapParameters( CCFLPtr pPixelA, CCFLPtr pPixelB, CCFLPtr pPixelC,
                                       const CustomFragmentLayout * pCFL, UInt iTextureChannel )
{
    const Vertex2 * pPosA = (const Vertex2*)( pCFL->GetField( pPixelA, CFLSEMANTIC_RASTERPOS, 0 ) );
    const Vertex2 * pPosB = (const Vertex2*)( pCFL->GetField( pPixelB, CFLSEMANTIC_RASTERPOS, 0 ) );
    const Vertex2 * pPosC = (const Vertex2*)( pCFL->GetField( pPixelC, CFLSEMANTIC_RASTERPOS, 0 ) );

    // Check for degeneracy (should not happen).
    Int iXBmXA = (Int)( pPosB->X - pPosA->X );
    Int iXAmXC = (Int)( pPosA->X - pPosC->X );
    Int iYAmYB = (Int)( pPosA->Y - pPosB->Y );
    Int iYCmYA = (Int)( pPosC->Y - pPosA->Y );
    Int iDet = iXBmXA * iYCmYA - iXAmXC * iYAmYB;
    Assert(iDet != 0);

    // Compute intermediate terms
    Scalar fInvDet = MathFn->Invert( (Scalar)iDet );
    Scalar fXAmXC = (Scalar)iXAmXC;
    Scalar fXBmXA = (Scalar)iXBmXA;
    Scalar fXCmXB = (Scalar)( pPosC->X - pPosB->X );
    Scalar fYAmYB = (Scalar)iYAmYB;
    Scalar fYBmYC = (Scalar)( pPosB->Y - pPosC->Y );
    Scalar fYCmYA = (Scalar)iYCmYA;
    Scalar fXAYBmXBYA = (Scalar)( pPosA->X * pPosB->Y - pPosB->X * pPosA->Y );
    Scalar fXBYCmXCYB = (Scalar)( pPosB->X * pPosC->Y - pPosC->X * pPosB->Y );
    Scalar fXCYAmXAYC = (Scalar)( pPosC->X * pPosA->Y - pPosA->X * pPosC->Y );

    Scalar fWA = *( pCFL->GetField( pPixelA, CFLSEMANTIC_REGULARW, 0 ) );
    Scalar fWB = *( pCFL->GetField( pPixelB, CFLSEMANTIC_REGULARW, 0 ) );
    Scalar fWC = *( pCFL->GetField( pPixelC, CFLSEMANTIC_REGULARW, 0 ) );

    Scalar fWAWB = fWA * fWB;
    Scalar fWAWC = fWA * fWC;
    Scalar fWBWC = fWB * fWC;
    Scalar fWAmWB = fWA - fWB;
    Scalar fWAmWC = fWA - fWC;
    Scalar fWBWAmWC = fWB * fWAmWC;
    Scalar fWCWAmWB = fWC * fWAmWB;

    // Final terms
    m_fMulA0 = fWBWC * fYBmYC * fInvDet;
    m_fMulA1 = fWAWC * fYCmYA * fInvDet;
    m_fMulA2 = fWAWB * fYAmYB * fInvDet;
    m_fMulB0 = fWBWC * fXCmXB * fInvDet;
    m_fMulB1 = fWAWC * fXAmXC * fInvDet;
    m_fMulB2 = fWAWB * fXBmXA * fInvDet;
    m_fMulC0 = fWBWC * fXBYCmXCYB * fInvDet;
    m_fMulC1 = fWAWC * fXCYAmXAYC * fInvDet;
    m_fMulC2 = fWAWB * fXAYBmXBYA * fInvDet;
    m_fD = ( fWCWAmWB * fYCmYA + fWBWAmWC * fYAmYB ) * fInvDet;
    m_fE = ( fWCWAmWB * fXAmXC + fWBWAmWC * fXBmXA ) * fInvDet;
    m_fF = ( fWCWAmWB * fXCYAmXAYC + fWBWAmWC * fXAYBmXBYA ) * fInvDet + fWBWC;

    m_bUseMipMapping = true;
}

/////////////////////////////////////////////////////////////////////////////////

Scalar Sampler::_GetImageCoord( UInt iDim, Scalar fCoord, UInt iBound ) const
{
    // Maps texture coordinate to pixel coordinate keeping
    // a Scalar representation for sub-pixel information.
    // Given a dimension and its bound, fCoord is a texture
    // coordinate in [0;1] but we want to allow sampling outside
    // the texture so we'll consider :
    // - Output interval = I = [ 0; (fBound - 1) ]
    //   Standard bounded result
    // - Output Symetrized interval = SI = [ -fBound; (fBound - 1) ]
    //   Symetrized around 0 of I, with respect to top-left filling
    // - Extended interval = EI = [ 0 - 0.5; (fBound - 1) + 0.5 ]
    //   Bounded result with added sub-pixel data fo a half-pixel
    //   at each side to allow wrapping-around.
    // - Extended Symetrized interval = ESI = ] -fBound - 0.5; (fBound - 1) + 0.5 [
    //   Symetrized result with added sub-pixel data fo a half-pixel
    //   at each side to allow wrapping-around.
    // - Extra pixel interval = EPI = [ 0 - 1; (fBound - 1) + 1 ]
    //   Bounded result with added sub-pixel data fo a full-pixel
    //   at each side to give an extra 1-pixel border.
    // Here are the methods :
    // * Clamping :
    //   constraint to EI = [ -0.5; (fBound - 0.5) ].
    // * Edge : (default)
    //   constraint to I = [ 0; (fBound - 1) ]
    // * Border :
    //   constraint to EPI = [ -1; fBound ]
    // * Repeat :
    //   constraint to ESI = ] (-fBound - 0.5); (fBound - 0.5) [
    // * Repeat-Mirror :
    //   constraint to ESI = ] (-fBound - 0.5); (fBound - 0.5) [
    //   and revert values ordering at each wrap-around.
    // CONVENTIONS = Top-Left filling, Round toward -inf
    Scalar fBound = (Scalar)iBound;
    Scalar fImageCoord;
    TextureWrapMode wrapMode = m_pTexture->GetWrapMode(iDim);
    switch(wrapMode) {
        case TEXTURE_WRAP_CLAMP: {
                fImageCoord = fBound * fCoord - 0.5f;
                if (fImageCoord < -0.5f)
                    fImageCoord = -0.5f;
                else if (fImageCoord > fBound - 0.5f)
                    fImageCoord = fBound - 0.5f;
            } break;
        case TEXTURE_WRAP_EDGE: {
                fImageCoord = fBound * fCoord - 0.5f;
                if (fImageCoord < 0.0f)
                    fImageCoord = 0.0f;
                else if (fImageCoord > fBound - 1.0f)
                    fImageCoord = fBound - 1.0f;
            } break;
        case TEXTURE_WRAP_BORDER: {
                fImageCoord = fBound * fCoord - 0.5f;
                if (fImageCoord < -1.0f)
                    fImageCoord = -1.0f;
                else if (fImageCoord > fBound)
                    fImageCoord = fBound;
            } break;
        case TEXTURE_WRAP_REPEAT: {
                fCoord -= MathFn->Floorf(fCoord); // wrap-around
                fImageCoord = fBound * fCoord - 0.5f;
            } break;
        case TEXTURE_WRAP_REPEAT_MIRROR: {
                Int iFloor = MathFn->Floor(fCoord);
                fCoord -= (Scalar)iFloor; // wrap-around
                if (iFloor & 1) // mirror on ith wrap-around if odd
                    fCoord = 1.0f - fCoord;
                fImageCoord = fBound * fCoord - 0.5f;
            } break;
    }
    return fImageCoord;
}
Void Sampler::_ClampLevels()
{
    Int iTmp;
    TextureFilter filter = m_pTexture->GetFilter();
    switch(filter) {
        case TEXTURE_FILTER_NEAREST_NEAREST:
        case TEXTURE_FILTER_LINEAR_NEAREST: {
                iTmp = (Int)( m_fCurLevel + 0.5f );
                if (iTmp < 0)
                    iTmp = 0;
                else if (iTmp >= (Int)m_iMipMapLevels)
                    iTmp = m_iMipMapLevels - 1;
                m_iLevelLow = (UInt)iTmp;
                m_pMipMapLow = m_arrMipMaps[m_iLevelLow];
            } break;
        case TEXTURE_FILTER_NEAREST_LINEAR:
        case TEXTURE_FILTER_LINEAR_LINEAR: {
                iTmp = MathFn->Floor( m_fCurLevel );
                if (iTmp < 0)
                    iTmp = 0;
                else if (iTmp >= (Int)m_iMipMapLevels)
                    iTmp = m_iMipMapLevels - 1;
                m_iLevelLow = (UInt)iTmp;
                m_pMipMapLow = m_arrMipMaps[m_iLevelLow];
                iTmp = MathFn->Ceil( m_fCurLevel );
                if (iTmp < 0)
                    iTmp = 0;
                else if (iTmp >= (Int)m_iMipMapLevels)
                    iTmp = m_iMipMapLevels - 1;
                m_iLevelHigh = (UInt)iTmp;
                m_pMipMapHigh = m_arrMipMaps[m_iLevelHigh];
                m_fDLvl = ( m_fCurLevel - (Scalar)m_iLevelLow );
            } break;
    }
}

Void Sampler::_CreateMipmap1D( UInt iLevel, UInt iWidth, UInt * outWidths )
{
    m_arrMipMaps[iLevel] = KNewExA(m_idAllocator) Color4[iWidth];
    outWidths[iLevel] = iWidth;
    _RecreateMipmap1D( iLevel, iWidth );
}
Void Sampler::_RecreateMipmap1D( UInt iLevel, UInt iWidth )
{
    Color4 * pPrev = m_arrMipMaps[iLevel-1];
    Color4 * pCur = m_arrMipMaps[iLevel];
    UInt iX = 0, iPrevX = 0;
    while(iX < iWidth) {
        pCur[iX] = ( pPrev[iPrevX] + pPrev[iPrevX+1] ) * 0.5f;
        ++iX;
        iPrevX += 2;
    }
}

Void Sampler::_CreateMipmap2D( UInt iLevel, UInt iWidth, UInt iHeight, UInt iLog2Width,
                               UInt iParentWidth, UInt iParentLog2Width,
                               UInt * outWidths, UInt * outHeights, UInt * outLog2Widths )
{
    m_arrMipMaps[iLevel] = KNewExA(m_idAllocator) Color4[iWidth*iHeight];
    outWidths[iLevel] = iWidth;
    outHeights[iLevel] = iHeight;
    outLog2Widths[iLevel] = iLog2Width;
    _RecreateMipmap2D( iLevel, iWidth, iHeight, iLog2Width, iParentWidth, iParentLog2Width );
}
Void Sampler::_RecreateMipmap2D( UInt iLevel, UInt iWidth, UInt iHeight, UInt iLog2Width,
                                 UInt iParentWidth, UInt iParentLog2Width )
{
    Color4 * pPrev = m_arrMipMaps[iLevel-1];
    Color4 * pCur = m_arrMipMaps[iLevel];
    UInt iOffset, iPrev0, iPrev1, iPrev2, iPrev3;
    UInt iX, iPrevX;
    UInt iY = 0, iPrevY = 0;
    while(iY < iHeight) {
        iX = 0;
        iPrevX = 0;
        while(iX < iWidth) {
            iOffset = (iY << iLog2Width) + iX;
            iPrev0 = (iPrevY << iParentLog2Width) + iPrevX;
            iPrev1 = iPrev0 + 1;
            iPrev2 = iPrev0 + iParentWidth;
            iPrev3 = iPrev2 + 1;
            pCur[iOffset] = ( pPrev[iPrev0] + pPrev[iPrev1] +
                              pPrev[iPrev2] + pPrev[iPrev3] ) * 0.25f;
            ++iX;
            iPrevX += 2;
        }
        ++iY;
        iPrevY += 2;
    }
}

Void Sampler::_CreateMipmap3D( UInt iLevel, UInt iWidth, UInt iHeight, UInt iDepth, UInt iLog2Width, UInt iLog2Height,
                               UInt iParentWidth, UInt iParentWidthHeight, UInt iParentLog2Width, UInt iParentLog2Height,
                               UInt * outWidths, UInt * outHeights, UInt * outDepths, UInt * outLog2Widths, UInt * outLog2Heights )
{
    m_arrMipMaps[iLevel] = KNewExA(m_idAllocator) Color4[iWidth*iHeight*iDepth];
    outWidths[iLevel] = iWidth;
    outHeights[iLevel] = iHeight;
    outDepths[iLevel] = iDepth;
    outLog2Widths[iLevel] = iLog2Width;
    outLog2Heights[iLevel] = iLog2Height;
    _RecreateMipmap3D( iLevel, iWidth, iHeight, iDepth, iLog2Width, iLog2Height,
                       iParentWidth, iParentWidthHeight, iParentLog2Width, iParentLog2Height );
}
Void Sampler::_RecreateMipmap3D( UInt iLevel, UInt iWidth, UInt iHeight, UInt iDepth, UInt iLog2Width, UInt iLog2Height,
                                 UInt iParentWidth, UInt iParentWidthHeight, UInt iParentLog2Width, UInt iParentLog2Height )
{
    Color4 * pPrev = m_arrMipMaps[iLevel-1];
    Color4 * pCur = m_arrMipMaps[iLevel];
    UInt iOffset, iPrev0, iPrev1, iPrev2, iPrev3;
    UInt iPrev4, iPrev5, iPrev6, iPrev7;
    UInt iX, iPrevX;
    UInt iY, iPrevY;
    UInt iZ = 0, iPrevZ = 0;
    while(iZ < iDepth) {
        iY = 0;
        iPrevY = 0;
        while(iY < iHeight) {
            iX = 0;
            iPrevX = 0;
            while(iX < iWidth) {
                iOffset = ( ((iZ << iLog2Height) + iY) << iLog2Width ) + iX;
                iPrev0 = ( ((iPrevZ << iParentLog2Height) + iPrevY) << iParentLog2Width ) + iPrevX;
                iPrev1 = iPrev0 + 1;
                iPrev2 = iPrev0 + iParentWidth;
                iPrev3 = iPrev2 + 1;
                iPrev4 = iPrev0 + iParentWidthHeight;
                iPrev5 = iPrev4 + 1;
                iPrev6 = iPrev4 + iParentWidth;
                iPrev7 = iPrev6 + 1;
                pCur[iOffset] = ( pPrev[iPrev0] + pPrev[iPrev1] + pPrev[iPrev2] + pPrev[iPrev3] +
                                  pPrev[iPrev4] + pPrev[iPrev5] + pPrev[iPrev6] + pPrev[iPrev7] ) * 0.125f;
                ++iX;
                iPrevX += 2;
            }
            ++iY;
            iPrevY += 2;
        }
        ++iZ;
        iPrevZ += 2;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// Sampler1D implementation
Sampler1D::Sampler1D( MemoryAllocatorID idAllocator, const Texture * pTexture ):
    Sampler(idAllocator, pTexture)
{
    const TextureData * pImage = m_pTexture->GetData();
    m_pCurMipMap = m_arrMipMaps[0];
    m_iCurWidth = pImage->GetBound(0);
    TextureFilter iFilter = m_pTexture->GetFilter();
    if ( iFilter == TEXTURE_FILTER_NEAREST ||
         iFilter == TEXTURE_FILTER_LINEAR ) {
        m_iMipMapLevels = 1;
        m_arrWidths = KNewExA(m_idAllocator) UInt[1];
        m_arrWidths[0] = m_iCurWidth;
    } else
        _GenerateMipMaps();
}
Sampler1D::~Sampler1D()
{
    if (m_arrWidths != NULL)
        KDeleteAExA(m_arrWidths, m_idAllocator);
}

Void Sampler1D::WriteToTexture( const Color4 * pColorImage )
{
    Sampler::WriteToTexture( pColorImage );

    const TextureData * pImage = m_pTexture->GetData();
    m_pCurMipMap = m_arrMipMaps[0];
    m_iCurWidth = pImage->GetBound(0);
    TextureFilter iFilter = m_pTexture->GetFilter();
    if ( iFilter != TEXTURE_FILTER_NEAREST &&
         iFilter != TEXTURE_FILTER_LINEAR )
        _RegenerateMipMaps();
}

Void Sampler1D::DisableMipMapping()
{
    m_pCurMipMap = m_arrMipMaps[0];
    m_iCurWidth = m_arrWidths[0];
    m_bUseMipMapping = false;
}

Void Sampler1D::ComputeMipmapParameters( CCFLPtr pPixelA, CCFLPtr pPixelB, CCFLPtr pPixelC,
                                         const CustomFragmentLayout * pCFL, UInt iTextureChannel )
{
    TextureFilter iFilter = m_pTexture->GetFilter();
    if ( iFilter == TEXTURE_FILTER_NEAREST ||
         iFilter == TEXTURE_FILTER_LINEAR ) {
        m_pCurMipMap = m_arrMipMaps[0];
        m_iCurWidth = m_arrWidths[0];
        return;
    }

    Sampler::ComputeMipmapParameters( pPixelA, pPixelB, pPixelC, pCFL, iTextureChannel );

    Scalar fT0 = ( (const TextureCoord1*)( pCFL->GetField( pPixelA, CFLSEMANTIC_TEXCOORD, iTextureChannel) ) )->T;
    Scalar fT1 = ( (const TextureCoord1*)( pCFL->GetField( pPixelB, CFLSEMANTIC_TEXCOORD, iTextureChannel) ) )->T;
    Scalar fT2 = ( (const TextureCoord1*)( pCFL->GetField( pPixelC, CFLSEMANTIC_TEXCOORD, iTextureChannel) ) )->T;

    m_fA = m_arrWidths[0] * ( m_fMulA0 * fT0 + m_fMulA1 * fT1 + m_fMulA2 * fT2 );
    m_fB = m_arrWidths[0] * ( m_fMulB0 * fT0 + m_fMulB1 * fT1 + m_fMulB2 * fT2 );
    m_fC = m_arrWidths[0] * ( m_fMulC0 * fT0 + m_fMulC1 * fT1 + m_fMulC2 * fT2 );
    m_fAEmBD = m_fA * m_fE - m_fB * m_fD;
    m_fAFmCD = m_fA * m_fF - m_fC * m_fD;
    m_fBFmCE = m_fB * m_fF - m_fC * m_fE;
}
Void Sampler1D::SetPixel( UInt iX, UInt iY )
{
    Scalar fDenom = m_fD * iX + m_fE * iY + m_fF;
    Scalar fInvDenomSqr = MathFn->Invert(fDenom * fDenom);
    Scalar fDuDx = (+m_fAEmBD * iY + m_fAFmCD) * fInvDenomSqr;
    Scalar fDuDy = (-m_fAEmBD * iX + m_fBFmCE) * fInvDenomSqr;
    Scalar fArg = fDuDx * fDuDx + fDuDy * fDuDy;
    if (fArg > 1.0f)
        m_fCurLevel = 0.5f * MathFn->Log2(fArg);
    else
        m_fCurLevel = 0.0f;

    _ClampLevels();

    TextureFilter iFilter = m_pTexture->GetFilter();
    if ( iFilter == TEXTURE_FILTER_NEAREST_NEAREST ||
         iFilter == TEXTURE_FILTER_LINEAR_NEAREST ) {
        m_pCurMipMap = m_arrMipMaps[m_iLevelLow];
        m_iCurWidth = m_arrWidths[m_iLevelLow];
    } else {
        m_iWidthLow = m_arrWidths[m_iLevelLow];
        m_iWidthHigh = m_arrWidths[m_iLevelHigh];
    }
}
Void Sampler1D::TexelSample( Color4 & outColor, CCFLPtr pTexCoords )
{
    outColor = Color4::Black;
    Scalar fImageX;

    TextureFilter iFilter = m_pTexture->GetFilter();
    if ( iFilter == TEXTURE_FILTER_NEAREST ||
         iFilter == TEXTURE_FILTER_NEAREST_NEAREST ) {
        fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
        _NearestColor( outColor, fImageX );
        return;
    }
    if ( iFilter == TEXTURE_FILTER_LINEAR ||
         iFilter == TEXTURE_FILTER_LINEAR_NEAREST ) {
        fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
        _LinearColor( outColor, fImageX );
        return;
    }
    if (m_bUseMipMapping) {
        Color4 colHigh;
        if ( iFilter == TEXTURE_FILTER_NEAREST_LINEAR ) {
            m_pCurMipMap = m_pMipMapLow;
            m_iCurWidth = m_iWidthLow;
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            _NearestColor( outColor, fImageX );
            m_pCurMipMap = m_pMipMapHigh;
            m_iCurWidth = m_iWidthHigh;
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            _NearestColor( colHigh, fImageX );
            outColor += ( (colHigh - outColor) * m_fDLvl );
            return;
        }
        if ( iFilter == TEXTURE_FILTER_LINEAR_LINEAR ) {
            m_pCurMipMap = m_pMipMapLow;
            m_iCurWidth = m_iWidthLow;
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            _LinearColor( outColor, fImageX );
            m_pCurMipMap = m_pMipMapHigh;
            m_iCurWidth = m_iWidthHigh;
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            _LinearColor( colHigh, fImageX );
            outColor += ( (colHigh - outColor) * m_fDLvl );
            return;
        }
    } else {
        if ( iFilter == TEXTURE_FILTER_NEAREST_LINEAR ) {
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            _NearestColor( outColor, fImageX );
            return;
        }
        if ( iFilter == TEXTURE_FILTER_LINEAR_LINEAR ) {
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            _LinearColor( outColor, fImageX );
            return;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void Sampler1D::_GenerateMipMaps()
{
    const TextureData * pImage = m_pTexture->GetData();
    UInt iLog2Width = MathFn->Log2OfPower2( pImage->GetBound(0) );
    m_iMipMapLevels = iLog2Width + 1;
    m_arrWidths = KNewExA(m_idAllocator) UInt[m_iMipMapLevels];
    m_arrWidths[0] = pImage->GetBound(0);
    UInt iWidth = (m_arrWidths[0] >> 1);
    for( UInt i = 1; i < m_iMipMapLevels; ++i ) {
        _CreateMipmap1D( i, iWidth, m_arrWidths );
        iWidth >>= 1;
    }
}
Void Sampler1D::_RegenerateMipMaps()
{
    UInt iWidth = (m_arrWidths[0] >> 1);
    for( UInt i = 1; i < m_iMipMapLevels; ++i ) {
        _RecreateMipmap1D( i, iWidth );
        iWidth >>= 1;
    }
}

Void Sampler1D::_NearestColor(Color4 & outColor, Scalar fImageX) const
{
    UInt iX = (UInt)( fImageX + 0.5f );
    outColor = m_pCurMipMap[iX];
}
Void Sampler1D::_LinearColor(Color4 & outColor, Scalar fImageX) const
{
    Scalar fX0 = MathFn->Floorf(fImageX);
    Scalar fX1 = MathFn->Ceilf(fImageX);
    Scalar fDX = fImageX - fX0;
    Int iX0 = (Int)fX0;
    Int iX1 = (Int)fX1;

    // adjust offset if wrapping around with a different
    // pixel involved ... only happen in the REPEAT case.
    // (Repeat-Mirror uses reverses each side so border
    // pixels end up being allways equal)
    if ( m_pTexture->GetWrapMode(0) == TEXTURE_WRAP_REPEAT ) {
        if (iX0 < 0)
            iX0 = m_iCurWidth - 1;
        if (iX1 >= (Int)m_iCurWidth)
            iX1 = 0;
    }

    const Color4 & colBorder = m_pTexture->GetBorderColor();
    outColor = (iX0 >= 0) ? m_pCurMipMap[iX0] : colBorder;
    Color4 col1 = (iX1 < (Int)m_iCurWidth) ? m_pCurMipMap[iX1] : colBorder;
    outColor += ( (col1 - outColor) * fDX );
}

/////////////////////////////////////////////////////////////////////////////////
// Sampler2D implementation
Sampler2D::Sampler2D( MemoryAllocatorID idAllocator, const Texture * pTexture ):
    Sampler(idAllocator, pTexture)
{
    const TextureData * pImage = m_pTexture->GetData();
    m_pCurMipMap = m_arrMipMaps[0];
    m_iCurWidth = pImage->GetBound(0);
    m_iCurHeight = pImage->GetBound(1);
    m_iCurLog2Width = MathFn->Log2OfPower2( m_iCurWidth );
    TextureFilter iFilter = m_pTexture->GetFilter();
    if ( iFilter == TEXTURE_FILTER_NEAREST ||
         iFilter == TEXTURE_FILTER_LINEAR ) {
        m_iMipMapLevels = 1;
        m_arrWidths = KNewExA(m_idAllocator) UInt[1];
        m_arrHeights = KNewExA(m_idAllocator) UInt[1];
        m_arrLog2Widths = KNewExA(m_idAllocator) UInt[1];
        m_arrWidths[0] = m_iCurWidth;
        m_arrHeights[0] = m_iCurHeight;
        m_arrLog2Widths[0] = m_iCurLog2Width;
    }
    else
        _GenerateMipMaps();
}
Sampler2D::~Sampler2D()
{
    if (m_arrWidths != NULL)
        KDeleteAExA(m_arrWidths, m_idAllocator);
    if (m_arrHeights != NULL)
        KDeleteAExA(m_arrHeights, m_idAllocator);
    if (m_arrLog2Widths != NULL)
        KDeleteAExA(m_arrLog2Widths, m_idAllocator);
}

Void Sampler2D::WriteToTexture( const Color4 * pColorImage )
{
    Sampler::WriteToTexture( pColorImage );

    const TextureData * pImage = m_pTexture->GetData();
    m_pCurMipMap = m_arrMipMaps[0];
    m_iCurWidth = pImage->GetBound(0);
    m_iCurHeight = pImage->GetBound(1);
    m_iCurLog2Width = MathFn->Log2OfPower2( m_iCurWidth );
    TextureFilter iFilter = m_pTexture->GetFilter();
    if ( iFilter != TEXTURE_FILTER_NEAREST &&
         iFilter != TEXTURE_FILTER_LINEAR )
        _RegenerateMipMaps();
}

Void Sampler2D::DisableMipMapping()
{
    m_pCurMipMap = m_arrMipMaps[0];
    m_iCurWidth = m_arrWidths[0];
    m_iCurHeight = m_arrHeights[0];
    m_iCurLog2Width = m_arrLog2Widths[0];
    m_bUseMipMapping = false;
}

Void Sampler2D::ComputeMipmapParameters( CCFLPtr pPixelA, CCFLPtr pPixelB, CCFLPtr pPixelC,
                                         const CustomFragmentLayout * pCFL, UInt iTextureChannel )
{
    TextureFilter iFilter = m_pTexture->GetFilter();
    if ( iFilter == TEXTURE_FILTER_NEAREST ||
         iFilter == TEXTURE_FILTER_LINEAR ) {
        m_pCurMipMap = m_arrMipMaps[0];
        m_iCurWidth = m_arrWidths[0];
        m_iCurHeight = m_arrHeights[0];
        m_iCurLog2Width = m_arrLog2Widths[0];
        return;
    }

    Sampler::ComputeMipmapParameters( pPixelA, pPixelB, pPixelC, pCFL, iTextureChannel );

    const TextureCoord2 * pTC0 = (const TextureCoord2*)( pCFL->GetField(pPixelA, CFLSEMANTIC_TEXCOORD, iTextureChannel) );
    const TextureCoord2 * pTC1 = (const TextureCoord2*)( pCFL->GetField(pPixelB, CFLSEMANTIC_TEXCOORD, iTextureChannel) );
    const TextureCoord2 * pTC2 = (const TextureCoord2*)( pCFL->GetField(pPixelC, CFLSEMANTIC_TEXCOORD, iTextureChannel) );

    Scalar fT0 = pTC0->T;
    Scalar fT1 = pTC1->T;
    Scalar fT2 = pTC2->T;
    m_fA[0] = m_arrWidths[0] * ( m_fMulA0 * fT0 + m_fMulA1 * fT1 + m_fMulA2 * fT2);
    m_fB[0] = m_arrWidths[0] * ( m_fMulB0 * fT0 + m_fMulB1 * fT1 + m_fMulB2 * fT2);
    m_fC[0] = m_arrWidths[0] * ( m_fMulC0 * fT0 + m_fMulC1 * fT1 + m_fMulC2 * fT2);
    m_fAEmBD[0] = m_fA[0] * m_fE - m_fB[0] * m_fD;
    m_fAFmCD[0] = m_fA[0] * m_fF - m_fC[0] * m_fD;
    m_fBFmCE[0] = m_fB[0] * m_fF - m_fC[0] * m_fE;

    Scalar fU0 = pTC0->U;
    Scalar fU1 = pTC1->U;
    Scalar fU2 = pTC2->U;
    m_fA[1] = m_arrHeights[0] * ( m_fMulA0 * fU0 + m_fMulA1 * fU1 + m_fMulA2 * fU2);
    m_fB[1] = m_arrHeights[0] * ( m_fMulB0 * fU0 + m_fMulB1 * fU1 + m_fMulB2 * fU2);
    m_fC[1] = m_arrHeights[0] * ( m_fMulC0 * fU0 + m_fMulC1 * fU1 + m_fMulC2 * fU2);
    m_fAEmBD[1] = m_fA[1] * m_fE - m_fB[1] * m_fD;
    m_fAFmCD[1] = m_fA[1] * m_fF - m_fC[1] * m_fD;
    m_fBFmCE[1] = m_fB[1] * m_fF - m_fC[1] * m_fE;
}    
Void Sampler2D::SetPixel( UInt iX, UInt iY )
{
    Scalar fDenom = m_fD * iX + m_fE * iY + m_fF;
    Scalar fInvDenomSqr = MathFn->Invert(fDenom * fDenom);
    Scalar fLevel;
    m_fCurLevel  = 0.0f;

    Scalar fDuDx = ( +m_fAEmBD[0] * iY + m_fAFmCD[0] ) * fInvDenomSqr;
    Scalar fDuDy = ( -m_fAEmBD[0] * iX + m_fBFmCE[0] ) * fInvDenomSqr;
    Scalar fArg = fDuDx * fDuDx + fDuDy * fDuDy;
    if (fArg > 1.0f) {
        fLevel = 0.5f * MathFn->Log2(fArg); 
        if (fLevel > m_fCurLevel)
            m_fCurLevel = fLevel;
    }
    fDuDx = ( +m_fAEmBD[1] * iY + m_fAFmCD[1] ) * fInvDenomSqr;
    fDuDy = ( -m_fAEmBD[1] * iX + m_fBFmCE[1] ) * fInvDenomSqr;
    fArg = fDuDx * fDuDx + fDuDy * fDuDy;
    if (fArg > 1.0f) {
        fLevel = 0.5f * MathFn->Log2(fArg); 
        if (fLevel > m_fCurLevel)
            m_fCurLevel = fLevel;
    }

    _ClampLevels();

    TextureFilter iFilter = m_pTexture->GetFilter();
    if ( iFilter == TEXTURE_FILTER_NEAREST_NEAREST ||
         iFilter == TEXTURE_FILTER_LINEAR_NEAREST ) {
        m_pCurMipMap = m_arrMipMaps[m_iLevelLow];
        m_iCurWidth = m_arrWidths[m_iLevelLow];
        m_iCurHeight = m_arrHeights[m_iLevelLow];
        m_iCurLog2Width = m_arrLog2Widths[m_iLevelLow];
    } else {
        m_iWidthLow = m_arrWidths[m_iLevelLow];
        m_iHeightLow = m_arrHeights[m_iLevelLow];
        m_iLog2WidthLow = m_arrLog2Widths[m_iLevelLow];
        m_iWidthHigh = m_arrWidths[m_iLevelHigh];
        m_iHeightHigh = m_arrHeights[m_iLevelHigh];
        m_iLog2WidthHigh = m_arrLog2Widths[m_iLevelHigh];
    }
}
Void Sampler2D::TexelSample( Color4 & outColor, CCFLPtr pTexCoords )
{
    outColor = Color4::Black;
    Scalar fImageX, fImageY;

    TextureFilter iFilter = m_pTexture->GetFilter();
    if ( iFilter == TEXTURE_FILTER_NEAREST ||
         iFilter == TEXTURE_FILTER_NEAREST_NEAREST ) {
        fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
        fImageY = _GetImageCoord( 1, pTexCoords[1], m_iCurHeight );
        _NearestColor( outColor, fImageX, fImageY );
        return;
    }
    if ( iFilter == TEXTURE_FILTER_LINEAR ||
         iFilter == TEXTURE_FILTER_LINEAR_NEAREST ) {
        fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
        fImageY = _GetImageCoord( 1, pTexCoords[1], m_iCurHeight );
        _LinearColor( outColor, fImageX, fImageY );
        return;
    }
    if (m_bUseMipMapping) {
        Color4 colHigh;
        if ( iFilter == TEXTURE_FILTER_NEAREST_LINEAR ) {
            m_pCurMipMap = m_pMipMapLow;
            m_iCurWidth = m_iWidthLow;
            m_iCurHeight = m_iHeightLow;
            m_iCurLog2Width = m_iLog2WidthLow;
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            fImageY = _GetImageCoord( 1, pTexCoords[1], m_iCurHeight );
            _NearestColor( outColor, fImageX, fImageY );

            m_pCurMipMap = m_pMipMapHigh;
            m_iCurWidth = m_iWidthHigh;
            m_iCurHeight = m_iHeightHigh;
            m_iCurLog2Width = m_iLog2WidthHigh;
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            fImageY = _GetImageCoord( 1, pTexCoords[1], m_iCurHeight );
            _NearestColor( colHigh, fImageX, fImageY );

            outColor += ( (colHigh - outColor) * m_fDLvl );
            return;
        }
        if ( iFilter == TEXTURE_FILTER_LINEAR_LINEAR ) {
            m_pCurMipMap = m_pMipMapLow;
            m_iCurWidth = m_iWidthLow;
            m_iCurHeight = m_iHeightLow;
            m_iCurLog2Width = m_iLog2WidthLow;
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            fImageY = _GetImageCoord( 1, pTexCoords[1], m_iCurHeight );
            _LinearColor( outColor, fImageX, fImageY );

            m_pCurMipMap = m_pMipMapHigh;
            m_iCurWidth = m_iWidthHigh;
            m_iCurHeight = m_iHeightHigh;
            m_iCurLog2Width = m_iLog2WidthHigh;
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            fImageY = _GetImageCoord( 1, pTexCoords[1], m_iCurHeight );
            _LinearColor( colHigh, fImageX, fImageY );
            
            outColor += ( (colHigh - outColor) * m_fDLvl );
            return;
        }
    } else {
        if ( iFilter == TEXTURE_FILTER_NEAREST_LINEAR ) {
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            fImageY = _GetImageCoord( 1, pTexCoords[1], m_iCurHeight );
            _NearestColor( outColor, fImageX, fImageY );
            return;
        }
        if ( iFilter == TEXTURE_FILTER_LINEAR_LINEAR ) {
            fImageX = _GetImageCoord( 0, pTexCoords[0], m_iCurWidth );
            fImageY = _GetImageCoord( 1, pTexCoords[1], m_iCurHeight );
            _LinearColor( outColor, fImageX, fImageY );
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////

Void Sampler2D::_GenerateMipMaps()
{
    const TextureData * pImage = m_pTexture->GetData();
    UInt iParentLog2Width = MathFn->Log2OfPower2( pImage->GetBound(0) );
    UInt iParentLog2Height = MathFn->Log2OfPower2( pImage->GetBound(1) );

    UInt iMaxLevel, iMinLevel;
    if ( pImage->GetBound(0) >= pImage->GetBound(1) ) {
        iMaxLevel = iParentLog2Width;
        iMinLevel = iParentLog2Height;
    } else {
        iMaxLevel = iParentLog2Height;
        iMinLevel = iParentLog2Width;
    }
    m_iMipMapLevels = iMaxLevel + 1;

    m_arrWidths = KNewExA(m_idAllocator) UInt[m_iMipMapLevels];
    m_arrHeights = KNewExA(m_idAllocator) UInt[m_iMipMapLevels];
    m_arrLog2Widths = KNewExA(m_idAllocator) UInt[m_iMipMapLevels];
    m_arrWidths[0] = pImage->GetBound(0);
    m_arrHeights[0] = pImage->GetBound(1);
    m_arrLog2Widths[0] = iParentLog2Width;

    // 2D reduction
    UInt iParentWidth = m_arrWidths[0];
    UInt iWidth = (iParentWidth >> 1);
    UInt iHeight = (m_arrHeights[0] >> 1);
    UInt iLog2Width = iParentLog2Width - 1;
    UInt i;
    for(i = 1; i <= iMinLevel; ++i) {
        _CreateMipmap2D( i, iWidth, iHeight, iLog2Width, iParentWidth, iParentLog2Width,
                         m_arrWidths, m_arrHeights, m_arrLog2Widths );
        iParentWidth = iWidth;
        iWidth >>= 1;
        iHeight >>= 1;
        iParentLog2Width = iLog2Width;
        --iLog2Width;
    }

    // 1D reduction
    if ( pImage->GetBound(0) > pImage->GetBound(1) ) {
        for( ; i <= iMaxLevel; ++i) {
            _CreateMipmap1D( i, iWidth, m_arrWidths );
            m_arrHeights[i] = 1;
            m_arrLog2Widths[i] = iLog2Width;
            iWidth >>= 1;
            --iLog2Width;
        }
    } else if ( pImage->GetBound(0) < pImage->GetBound(1) ) {
        for( ; i <= iMaxLevel; ++i) {
            _CreateMipmap1D( i, iHeight, m_arrHeights );
            m_arrWidths[i] = 1;
            m_arrLog2Widths[i] = 0;
            iHeight >>= 1;
        }
    }
}
Void Sampler2D::_RegenerateMipMaps()
{
    const TextureData * pImage = m_pTexture->GetData();
    UInt iParentLog2Width = MathFn->Log2OfPower2( pImage->GetBound(0) );
    UInt iParentLog2Height = MathFn->Log2OfPower2( pImage->GetBound(1) );

    UInt iMaxLevel, iMinLevel;
    if ( pImage->GetBound(0) >= pImage->GetBound(1) ) {
        iMaxLevel = iParentLog2Width;
        iMinLevel = iParentLog2Height;
    } else {
        iMaxLevel = iParentLog2Height;
        iMinLevel = iParentLog2Width;
    }

    // 2D reduction
    UInt iParentWidth = m_arrWidths[0];
    UInt iWidth = (iParentWidth >> 1);
    UInt iHeight = (m_arrHeights[0] >> 1);
    UInt iLog2Width = iParentLog2Width - 1;
    UInt i;
    for(i = 1; i <= iMinLevel; ++i) {
        _RecreateMipmap2D( i, iWidth, iHeight, iLog2Width, iParentWidth, iParentLog2Width );
        iParentWidth = iWidth;
        iWidth >>= 1;
        iHeight >>= 1;
        iParentLog2Width = iLog2Width;
        --iLog2Width;
    }

    // 1D reduction
    if ( pImage->GetBound(0) > pImage->GetBound(1) ) {
        for( ; i <= iMaxLevel; ++i) {
            _RecreateMipmap1D( i, iWidth );
            iWidth >>= 1;
            //--iLog2Width;
        }
    } else if ( pImage->GetBound(0) < pImage->GetBound(1) ) {
        for( ; i <= iMaxLevel; ++i) {
            _RecreateMipmap1D( i, iHeight );
            iHeight >>= 1;
        }
    }
}

Void Sampler2D::_NearestColor( Color4 & outColor, Scalar fImageX, Scalar fImageY ) const
{
    UInt iX = (UInt)( fImageX + 0.5f );
    UInt iY = (UInt)( fImageY + 0.5f );
    outColor = m_pCurMipMap[ _GetIndex(iX,iY) ];
}
Void Sampler2D::_LinearColor( Color4 & outColor, Scalar fImageX, Scalar fImageY ) const
{
    Scalar fX0 = MathFn->Floorf(fImageX);
    Scalar fX1 = MathFn->Ceilf(fImageX);
    Scalar fY0 = MathFn->Floorf(fImageY);
    Scalar fY1 = MathFn->Ceilf(fImageY);
    Scalar fDX = fImageX - fX0;
    Scalar fDY = fImageY - fY0;
    Int iX0 = (Int)fX0;
    Int iX1 = (Int)fX1;
    Int iY0 = (Int)fY0;
    Int iY1 = (Int)fY1;
    UInt iIndex;

    // adjust offset if wrapping around with a different
    // pixel involved ... only happen in the REPEAT case.
    // (Repeat-Mirror uses reverses each side so border
    // pixels end up being allways equal)
    if ( m_pTexture->GetWrapMode(0) == TEXTURE_WRAP_REPEAT ) {
        if (iX0 < 0)
            iX0 = m_iCurWidth - 1;
        if (iX1 >= (Int)m_iCurWidth)
            iX1 = 0;
    }
    if ( m_pTexture->GetWrapMode(1) == TEXTURE_WRAP_REPEAT ) {
        if (iY0 < 0)
            iY0 = m_iCurHeight - 1;
        if (iY1 >= (Int)m_iCurHeight)
            iY1 = 0;
    }

    const Color4 & colBorder = m_pTexture->GetBorderColor();
    Color4 col0, col1, col2, col3;
    if (iX0 >= 0) {
        iIndex = _GetIndex(iX0,iY0);
        col0 = (iY0 >= 0) ? m_pCurMipMap[iIndex] : colBorder;
        iIndex = _GetIndex(iX0,iY1);
        col1 = (iY1 < (Int)m_iCurHeight) ? m_pCurMipMap[iIndex] : colBorder;
    } else {
        col0 = colBorder;
        col1 = colBorder;
    }
    if (iX1 < (Int)m_iCurWidth) {
        iIndex = _GetIndex(iX1,iY0);
        col2 = (iY0 >= 0) ? m_pCurMipMap[iIndex] : colBorder;
        iIndex = _GetIndex(iX1,iY1);
        col3 = (iY1 < (Int)m_iCurHeight) ? m_pCurMipMap[iIndex] : colBorder;
    } else {
        col2 = colBorder;
        col3 = colBorder;
    }
    outColor = col0 + ( (col1 - col0) * fDY );
    Color4 colTmp1 = col2 + ( (col3 - col2) * fDY );
    outColor += ( (colTmp1 - outColor) * fDX );
}

