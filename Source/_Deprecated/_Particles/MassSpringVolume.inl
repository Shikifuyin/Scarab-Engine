/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/MassSpringVolume.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mass-Spring system for volumes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MassSpringVolume implementation
inline UInt MassSpringVolume::GetWidth() const {
    return m_iWidth;
}
inline UInt MassSpringVolume::GetHeight() const {
    return m_iHeight;
}
inline UInt MassSpringVolume::GetDepth() const {
    return m_iDepth;
}
inline Spring & MassSpringVolume::GetSpringR( UInt iX, UInt iY, UInt iZ ) {
    return m_arrSpringsR[ _GetSpringIndexR(iX, iY, iZ) ];
}
inline Spring & MassSpringVolume::GetSpringD( UInt iX, UInt iY, UInt iZ ) {
    return m_arrSpringsD[ _GetSpringIndexD(iX, iY, iZ) ];
}
inline Spring & MassSpringVolume::GetSpringB( UInt iX, UInt iY, UInt iZ ) {
    return m_arrSpringsB[ _GetSpringIndexB(iX, iY, iZ) ];
}
inline const Spring & MassSpringVolume::GetSpringR( UInt iX, UInt iY, UInt iZ ) const {
    return m_arrSpringsR[ _GetSpringIndexR(iX, iY, iZ) ];
}
inline const Spring & MassSpringVolume::GetSpringD( UInt iX, UInt iY, UInt iZ ) const {
    return m_arrSpringsD[ _GetSpringIndexD(iX, iY, iZ) ];
}
inline const Spring & MassSpringVolume::GetSpringB( UInt iX, UInt iY, UInt iZ ) const {
    return m_arrSpringsB[ _GetSpringIndexB(iX, iY, iZ) ];
}

inline Scalar MassSpringVolume::GetMass( UInt iX, UInt iY, UInt iZ ) const {
    return ParticleSystem::GetMass( _GetParticleIndex(iX, iY, iZ) );
}
inline const Vertex3 & MassSpringVolume::GetPosition( UInt iX, UInt iY, UInt iZ ) const {
    return ParticleSystem::GetPosition( _GetParticleIndex(iX, iY, iZ) );
}
inline const Vector3 & MassSpringVolume::GetSpeed( UInt iX, UInt iY, UInt iZ ) const {
    return ParticleSystem::GetSpeed( _GetParticleIndex(iX, iY, iZ) );
}

inline Void MassSpringVolume::SetMass( UInt iX, UInt iY, UInt iZ, Scalar fMass ) {
    ParticleSystem::SetMass( _GetParticleIndex(iX, iY, iZ), fMass );
}
inline Void MassSpringVolume::SetPosition( UInt iX, UInt iY, UInt iZ, const Vertex3 & vPosition ) {
    ParticleSystem::SetPosition( _GetParticleIndex(iX, iY, iZ), vPosition );
}
inline Void MassSpringVolume::SetSpeed( UInt iX, UInt iY, UInt iZ, const Vector3 & vSpeed ) {
    ParticleSystem::SetSpeed( _GetParticleIndex(iX, iY, iZ), vSpeed );
}

/////////////////////////////////////////////////////////////////////////////////

inline UInt MassSpringVolume::_GetSpringIndexR( UInt iX, UInt iY, UInt iZ ) const {
    Assert( iX < m_iWidthM1 && iY < m_iHeight && iZ < m_iDepth );
    return ( (iZ * m_iHeight + iY) * m_iWidthM1 + iX );
}
inline UInt MassSpringVolume::_GetSpringIndexD( UInt iX, UInt iY, UInt iZ ) const {
    Assert( iX < m_iWidth && iY < m_iHeightM1 && iZ < m_iDepth );
    return ( (iZ * m_iHeightM1 + iY) * m_iWidth + iX );
}
inline UInt MassSpringVolume::_GetSpringIndexB( UInt iX, UInt iY, UInt iZ ) const {
    Assert( iX < m_iWidth && iY < m_iHeight && iZ < m_iDepthM1 );
    return ( (iZ * m_iHeight + iY) * m_iWidth + iX );
}
inline UInt MassSpringVolume::_GetParticleIndex( UInt iX, UInt iY, UInt iZ ) const {
    Assert( iX < m_iWidth && iY < m_iHeight && iZ < m_iDepth );
    return ( (iZ * m_iHeight + iY) * m_iWidth + iX );
}
inline Void MassSpringVolume::_GetParticleCoords( UInt iParticle, UInt & outX, UInt & outY, UInt & outZ ) const {
    Assert( iParticle < m_iParticleCount );
    outX = ( iParticle % m_iWidth );
    iParticle = (iParticle - outX) / m_iWidth;
    outY = ( iParticle % m_iHeight );
    outZ = ( iParticle / m_iHeight );
}

