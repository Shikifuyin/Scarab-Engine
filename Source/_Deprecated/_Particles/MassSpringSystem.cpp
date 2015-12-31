/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/MassSpringSystem.cpp
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
// Includes
#include "MassSpringSystem.h"

#include "../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MassSpringSystem implementation
MassSpringSystem::MassSpringSystem( UInt iParticleCount, UInt iSpringCount ):
    ParticleSystem( iParticleCount )
{
    m_iSpringCount = iSpringCount;

    PhysicsFn->SelectMemory( TEXT("Particles") );
    m_arrSprings = KNew() LinkedSpring[m_iSpringCount];
    m_mapAdjacency = KNew() Array<UInt>[m_iParticleCount];
    PhysicsFn->UnSelectMemory();

    for( UInt i = 0; i < m_iParticleCount; ++i ) {
        m_mapAdjacency[i] = Array<UInt>( MASSSPRING_ADJACENCY_PAGESIZE );
        m_mapAdjacency[i].UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Particles") );
        m_mapAdjacency[i].Create();
    }
}
MassSpringSystem::~MassSpringSystem()
{
    for( UInt i = 0; i < m_iParticleCount; ++i )
        m_mapAdjacency[i].Destroy();

    PhysicsFn->SelectMemory( TEXT("Particles") );
    KDeleteA( m_mapAdjacency );
    KDeleteA( m_arrSprings );
    PhysicsFn->UnSelectMemory();
}

Void MassSpringSystem::InitSpring( UInt iSpring, UInt iParticleA, UInt iParticleB,
                                   Bool bActive, Scalar fLength, Scalar fElasticity, Scalar fFriction )
{
    Assert( iSpring < m_iSpringCount );

    m_arrSprings[iSpring].bActive = bActive;
    m_arrSprings[iSpring].fLength = fLength;
    m_arrSprings[iSpring].fElasticity = fElasticity;
    m_arrSprings[iSpring].fFriction = fFriction;
    m_arrSprings[iSpring].iParticleA = iParticleA;
    m_arrSprings[iSpring].iParticleB = iParticleB;

    m_mapAdjacency[iParticleA].Push( iSpring );
    m_mapAdjacency[iParticleB].Push( iSpring );
}

Vector3 MassSpringSystem::Acceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                      const Vector3 * arrSpeeds ) const
{
    Vector3 vAccel = ExternalAcceleration( fTime, iParticle, arrPositions, arrSpeeds );
    Vector3 vDeltaP, vDeltaV, vForce;
    Scalar fRatio;

    const UInt iCount = m_mapAdjacency[iParticle].Count();
    for( UInt i = 0; i < iCount; ++i ) {
        const LinkedSpring & curSpring = m_arrSprings[ (m_mapAdjacency[iParticle])[i] ];
        if ( !(curSpring.bActive) )
            continue;
        if ( curSpring.iParticleA == iParticle ) {
            vDeltaP = ( arrPositions[curSpring.iParticleB] - arrPositions[iParticle] );
            vDeltaV = ( arrSpeeds[curSpring.iParticleB] - arrSpeeds[iParticle] );
        } else {
            vDeltaP = ( arrPositions[curSpring.iParticleA] - arrPositions[iParticle] );
            vDeltaV = ( arrSpeeds[curSpring.iParticleA] - arrSpeeds[iParticle] );
        }
        fRatio = curSpring.fLength / vDeltaP.Norm();
        vForce = ( vDeltaP * ((1.0f - fRatio) * curSpring.fElasticity) );
        vForce += ( vDeltaV * curSpring.fFriction );
        vAccel += ( vForce * m_arrInvMasses[iParticle] );
    }

    return vAccel;
}

Vector3 MassSpringSystem::ExternalAcceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                              const Vector3 * arrSpeeds ) const
{
    // Default is no external force
    return Vector3::Null;
}

