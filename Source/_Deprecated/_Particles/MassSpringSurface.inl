/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/MassSpringSurface.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mass-Spring system for surfaces
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MassSpringSurface implementation
inline UInt MassSpringSurface::GetWidth() const {
    return m_iWidth;
}
inline UInt MassSpringSurface::GetHeight() const {
    return m_iHeight;
}
inline Spring & MassSpringSurface::GetSpringR( UInt iX, UInt iY ) {
    return m_arrSpringsR[ _GetSpringIndexR(iX, iY) ];
}
inline Spring & MassSpringSurface::GetSpringD( UInt iX, UInt iY ) {
    return m_arrSpringsD[ _GetSpringIndexD(iX, iY) ];
}
inline const Spring & MassSpringSurface::GetSpringR( UInt iX, UInt iY ) const {
    return m_arrSpringsR[ _GetSpringIndexR(iX, iY) ];
}
inline const Spring & MassSpringSurface::GetSpringD( UInt iX, UInt iY ) const {
    return m_arrSpringsD[ _GetSpringIndexD(iX, iY) ];
}

inline Scalar MassSpringSurface::GetMass( UInt iX, UInt iY ) const {
    return ParticleSystem::GetMass( _GetParticleIndex(iX, iY) );
}
inline const Vertex3 & MassSpringSurface::GetPosition( UInt iX, UInt iY ) const {
    return ParticleSystem::GetPosition( _GetParticleIndex(iX, iY) );
}
inline const Vector3 & MassSpringSurface::GetSpeed( UInt iX, UInt iY ) const {
    return ParticleSystem::GetSpeed( _GetParticleIndex(iX, iY) );
}

inline Void MassSpringSurface::SetMass( UInt iX, UInt iY, Scalar fMass ) {
    ParticleSystem::SetMass( _GetParticleIndex(iX, iY), fMass );
}
inline Void MassSpringSurface::SetPosition( UInt iX, UInt iY, const Vertex3 & vPosition ) {
    ParticleSystem::SetPosition( _GetParticleIndex(iX, iY), vPosition );
}
inline Void MassSpringSurface::SetSpeed( UInt iX, UInt iY, const Vector3 & vSpeed ) {
    ParticleSystem::SetSpeed( _GetParticleIndex(iX, iY), vSpeed );
}

/////////////////////////////////////////////////////////////////////////////////

inline UInt MassSpringSurface::_GetSpringIndexR( UInt iX, UInt iY ) const {
    Assert( iX < m_iWidthM1 && iY < m_iHeight );
    return ( iY * m_iWidthM1 + iX );
}
inline UInt MassSpringSurface::_GetSpringIndexD( UInt iX, UInt iY ) const {
    Assert( iX < m_iWidth && iY < m_iHeightM1 );
    return ( iY * m_iWidth + iX );
}
inline UInt MassSpringSurface::_GetParticleIndex( UInt iX, UInt iY ) const {
    Assert( iX < m_iWidth && iY < m_iHeight );
    return ( iY * m_iWidth + iX );
}
inline Void MassSpringSurface::_GetParticleCoords( UInt iParticle, UInt & outX, UInt & outY ) const {
    Assert( iParticle < m_iParticleCount );
    outX = ( iParticle % m_iWidth );
    outY = ( iParticle / m_iWidth );
}

