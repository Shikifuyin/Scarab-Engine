/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Texture.inl
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
// TextureData implementation
inline UInt TextureData::GetBytesPerPixel( TextureFormat iFormat ) {
    return sm_arrBytesPerPixel[iFormat];
}
inline const KChar * TextureData::GetFormatName( TextureFormat iFormat ) {
    return sm_arrNames[iFormat];
}

inline Byte * TextureData::GetData() const {
    return m_pBuffer;
}
inline Byte * TextureData::GetData( UInt iOffset ) const {
    Assert(iOffset < m_iSize);
    return m_pBuffer + ( iOffset * sm_arrBytesPerPixel[m_iFormat] );
}
inline UInt TextureData::GetSize() const {
    return m_iSize;
}
inline UInt TextureData::GetDimension() const {
    return m_iDimension;
}
inline UInt TextureData::GetBound( UInt iBound ) const {
    Assert(iBound < m_iDimension);
    return m_iBounds[iBound];
}
inline UInt TextureData::GetBytesPerPixel() const {
    return sm_arrBytesPerPixel[m_iFormat];
}
inline TextureFormat TextureData::GetFormat() const {
    return m_iFormat;
}
inline const KChar * TextureData::GetFormatName() const {
    return sm_arrNames[m_iFormat];
}

inline Bool TextureData::IsDepth() const {
    return ( m_iFormat == TEXTURE_FORMAT_DEPTH16I ||
             m_iFormat == TEXTURE_FORMAT_DEPTH24I ||
             m_iFormat == TEXTURE_FORMAT_DEPTH32I ||
             m_iFormat == TEXTURE_FORMAT_DEPTH32F );
}
inline Bool TextureData::IsCube() const {
    return ( m_iFormat == TEXTURE_FORMAT_CUBIC_R8G8B8 ||
             m_iFormat == TEXTURE_FORMAT_CUBIC_R8G8B8A8 );
}

/////////////////////////////////////////////////////////////////////////////////
// Texture implementation
inline const KChar * Texture::GetFilterName( TextureFilter iFilter ) {
    return sm_arrFilterNames[iFilter];
}
inline const KChar * Texture::GetWrapModeName( TextureWrapMode iWrapMode ) {
    return sm_arrWrapNames[iWrapMode];
}

inline const KChar * Texture::GetName() const {
    return m_strName;
}
inline TextureData * Texture::GetData() const {
    return m_pImage;
}
inline Void Texture::SetData( TextureData * pImage ) {
    Assert(pImage != NULL);
    m_pImage = pImage;
}

inline TextureFilter Texture::GetFilter() const {
    return m_iFilter;
}
inline const KChar * Texture::GetFilterName() const {
    return sm_arrFilterNames[m_iFilter];
}
inline TextureWrapMode Texture::GetWrapMode( UInt iDim ) const  {
    Assert(iDim < TEXTURE_MAX_DIMENSIONS);
    return m_iWrap[iDim];
}
inline const KChar * Texture::GetWrapModeName( UInt iDim ) const {
    Assert(iDim < TEXTURE_MAX_DIMENSIONS);
    return sm_arrWrapNames[ m_iWrap[iDim] ];
}
inline const Color4 & Texture::GetBorderColor() const {
    return m_colBorder;
}
inline Scalar Texture::GetAnisotropy() const {
    return m_fAnisotropy;
}
inline Bool Texture::IsOffscreen() const {
    return m_bOffscreen;
}

inline Void Texture::SetFilter( TextureFilter iFilter ) {
    m_iFilter = iFilter;
}
inline Void Texture::SetWrapMode( TextureWrapMode iWrapMode, UInt iDim ) {
    Assert(iDim < TEXTURE_MAX_DIMENSIONS);
    m_iWrap[iDim] = iWrapMode;
}
inline Void Texture::SetBorderColor( const Color4 & colBorder ) {
    m_colBorder = colBorder;
}
inline Void Texture::SetAnisotropy( Scalar fAnisotropy ) {
    m_fAnisotropy = fAnisotropy;
}
inline Void Texture::SetOffscreen( Bool bOffscreen ) {
    m_bOffscreen = bOffscreen;
}

