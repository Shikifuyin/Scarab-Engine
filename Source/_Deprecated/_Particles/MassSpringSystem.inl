/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/MassSpringSystem.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mass-Spring systems, a generic implementation
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MassSpringSystem implementation
inline Void MassSpringSystem::InitSpring( UInt iSpring, const LinkedSpring & newSpring ) {
    InitSpring( iSpring, newSpring.iParticleA, newSpring.iParticleB,
                newSpring.bActive, newSpring.fLength, newSpring.fElasticity, newSpring.fFriction );
}

inline UInt MassSpringSystem::GetSpringCount() const {
    return m_iSpringCount;
}
inline Spring & MassSpringSystem::GetSpring( UInt iSpring ) {
    Assert( iSpring < m_iSpringCount );
    return (Spring&)( m_arrSprings[iSpring] );
}
inline const LinkedSpring & MassSpringSystem::GetSpring( UInt iSpring ) const {
    Assert( iSpring < m_iSpringCount );
    return m_arrSprings[iSpring];
}

