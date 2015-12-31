/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/MassSpringSurface.cpp
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
// Includes
#include "MassSpringSurface.h"

#include "../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MassSpringSurface implementation
MassSpringSurface::MassSpringSurface( UInt iWidth, UInt iHeight ):
    ParticleSystem( iWidth * iHeight )
{
    m_iWidth = iWidth;
    m_iHeight = iHeight;
    m_iWidthM1 = iWidth - 1;
    m_iHeightM1 = iHeight - 1;

    PhysicsFn->SelectMemory( TEXT("Particles") );

    m_arrSpringsR = KNew() Spring[ m_iWidthM1 * m_iHeight ];
    m_arrSpringsD = KNew() Spring[ m_iWidth * m_iHeightM1 ];

    PhysicsFn->UnSelectMemory();
}
MassSpringSurface::~MassSpringSurface()
{
    PhysicsFn->SelectMemory( TEXT("Particles") );

    KDeleteA( m_arrSpringsR );
    KDeleteA( m_arrSpringsD );

    PhysicsFn->UnSelectMemory();
}

Vector3 MassSpringSurface::Acceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                       const Vector3 * arrSpeeds ) const
{
    Vector3 vAccel = ExternalAcceleration( fTime, iParticle, arrPositions, arrSpeeds );
    Vector3 vDeltaP, vDeltaV, vForce;
    Scalar fRatio;

    UInt iX, iY, iPrev, iNext;
    _GetParticleCoords( iParticle, iX, iY );
    if ( iX > 0 ) {
        const Spring & curSpring = GetSpringR( iX - 1, iY );
        if ( curSpring.bActive ) {
            iPrev = ( iParticle - 1 );
            vDeltaP = ( arrPositions[iPrev] - arrPositions[iParticle] );
            vDeltaV = ( arrSpeeds[iPrev] - arrSpeeds[iParticle] );
            fRatio = curSpring.fLength / vDeltaP.Norm();
            vForce = ( vDeltaP * ((1.0f - fRatio) * curSpring.fElasticity) );
            vForce += ( vDeltaV * curSpring.fFriction );
            vAccel += ( vForce * m_arrInvMasses[iParticle] );
        }
    }
    if ( iX < m_iWidthM1 ) {
        const Spring & curSpring = GetSpringR( iX, iY );
        if ( curSpring.bActive ) {
            iNext = ( iParticle + 1 );
            vDeltaP = ( arrPositions[iNext] - arrPositions[iParticle] );
            vDeltaV = ( arrSpeeds[iNext] - arrSpeeds[iParticle] );
            fRatio = curSpring.fLength / vDeltaP.Norm();
            vForce = ( vDeltaP * ((1.0f - fRatio) * curSpring.fElasticity) );
            vForce += ( vDeltaV * curSpring.fFriction );
            vAccel += ( vForce * m_arrInvMasses[iParticle] );
        }
    }
    if ( iY > 0 ) {
        const Spring & curSpring = GetSpringD( iX, iY - 1 );
        if ( curSpring.bActive ) {
            iPrev = ( iParticle - m_iWidth );
            vDeltaP = ( arrPositions[iPrev] - arrPositions[iParticle] );
            vDeltaV = ( arrSpeeds[iPrev] - arrSpeeds[iParticle] );
            fRatio = curSpring.fLength / vDeltaP.Norm();
            vForce = ( vDeltaP * ((1.0f - fRatio) * curSpring.fElasticity) );
            vForce += ( vDeltaV * curSpring.fFriction );
            vAccel += ( vForce * m_arrInvMasses[iParticle] );
        }
    }
    if ( iY < m_iHeightM1 ) {
        const Spring & curSpring = GetSpringD( iX, iY );
        if ( curSpring.bActive ) {
            iNext = ( iParticle + m_iWidth );
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

Vector3 MassSpringSurface::ExternalAcceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                               const Vector3 * arrSpeeds ) const
{
    // Default is no external force
    return Vector3::Null;
}

