/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/MassSpringCurve.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mass-Spring system for curves
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "MassSpringCurve.h"

#include "../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MassSpringCurve implementation
MassSpringCurve::MassSpringCurve( UInt iParticleCount ):
    ParticleSystem( iParticleCount )
{
    m_iSpringCount = iParticleCount - 1;

    PhysicsFn->SelectMemory( TEXT("Particles") );
    m_arrSprings = KNew() Spring[m_iSpringCount];
    PhysicsFn->UnSelectMemory();
}
MassSpringCurve::~MassSpringCurve()
{
    PhysicsFn->SelectMemory( TEXT("Particles") );
    KDeleteA( m_arrSprings );
    PhysicsFn->UnSelectMemory();
}

Vector3 MassSpringCurve::Acceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                     const Vector3 * arrSpeeds ) const
{
    Vector3 vAccel = ExternalAcceleration( fTime, iParticle, arrPositions, arrSpeeds );
    Vector3 vDeltaP, vDeltaV, vForce;
    Scalar fRatio;

    if ( iParticle > 0 ) {
        UInt iPrev = iParticle - 1;
        const Spring & curSpring = m_arrSprings[iPrev];
        if ( curSpring.bActive ) {
            vDeltaP = ( arrPositions[iPrev] - arrPositions[iParticle] );
            vDeltaV = ( arrSpeeds[iPrev] - arrSpeeds[iParticle] );
            fRatio = curSpring.fLength / vDeltaP.Norm();
            vForce = ( vDeltaP * ((1.0f - fRatio) * curSpring.fElasticity) );
            vForce += ( vDeltaV * curSpring.fFriction );
            vAccel += ( vForce * m_arrInvMasses[iParticle] );
        }
    }
    UInt iNext = iParticle + 1;
    if ( iNext < m_iParticleCount ) {
        const Spring & curSpring = m_arrSprings[iParticle];
        if ( curSpring.bActive ) {
            vDeltaP = ( arrPositions[iNext] - arrPositions[iParticle] );
            vDeltaV = ( arrSpeeds[iNext] - arrSpeeds[iParticle] );
            fRatio = curSpring.fLength / vDeltaP.Norm();
            vForce = ( vDeltaP * ((1.0f - fRatio) * curSpring.fElasticity) );
            vForce += ( vDeltaV * curSpring.fFriction );
            vAccel += ( vForce * m_arrInvMasses[iParticle] );
        }
    }

    return vAccel;
}

Vector3 MassSpringCurve::ExternalAcceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                             const Vector3 * arrSpeeds ) const
{
    // Default is no external force
    return Vector3::Null;
}
