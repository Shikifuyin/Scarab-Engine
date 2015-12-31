/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/ParticleSystem.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for particle systems such as mass-spring systems
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ParticleSystem implementation
inline UInt ParticleSystem::GetParticleCount() const {
    return m_iParticleCount;
}
inline Scalar ParticleSystem::GetMass( UInt iParticle ) const {
    Assert( iParticle < m_iParticleCount );
    return m_arrMasses[iParticle];
}
inline const Vertex3 & ParticleSystem::GetPosition( UInt iParticle ) const {
    Assert( iParticle < m_iParticleCount );
    return m_arrPositions[iParticle];
}
inline const Vector3 & ParticleSystem::GetSpeed( UInt iParticle ) const {
    Assert( iParticle < m_iParticleCount );
    return m_arrSpeeds[iParticle];
}

inline Void ParticleSystem::SetMass( UInt iParticle, Scalar fMass ) {
    Assert( iParticle < m_iParticleCount );
    if ( fMass > 0.0f && fMass < SCALAR_INFINITE ) {
        m_arrMasses[iParticle] = fMass;
        m_arrInvMasses[iParticle] = MathFn->Invert(fMass);
    } else {
        m_arrMasses[iParticle] = SCALAR_INFINITE;
        m_arrInvMasses[iParticle] = 0.0f;
    }
}
inline Void ParticleSystem::SetPosition( UInt iParticle, const Vertex3 & vPosition ) {
    Assert( iParticle < m_iParticleCount );
    m_arrPositions[iParticle] = vPosition;
}
inline Void ParticleSystem::SetSpeed( UInt iParticle, const Vector3 & vSpeed ) {
    Assert( iParticle < m_iParticleCount );
    m_arrSpeeds[iParticle] = vSpeed;
}

