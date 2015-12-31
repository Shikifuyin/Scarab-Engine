/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/ParticleSystem.cpp
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
// Includes
#include "ParticleSystem.h"

#include "../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ParticleSystem implementation
ParticleSystem::ParticleSystem( UInt iParticleCount )
{
    m_iParticleCount = iParticleCount;

    PhysicsFn->SelectMemory( TEXT("Particles") );

    m_arrMasses = KNew() Scalar[m_iParticleCount];
    m_arrInvMasses = KNew() Scalar[m_iParticleCount];
    m_arrPositions = KNew() Vertex3[m_iParticleCount];
    m_arrSpeeds = KNew() Vector3[m_iParticleCount];

    m_arrTmpP = KNew() Vertex3[m_iParticleCount];
    m_arrS1dP = KNew() Vector3[m_iParticleCount];
    m_arrS2dP = KNew() Vector3[m_iParticleCount];
    m_arrS3dP = KNew() Vector3[m_iParticleCount];
    m_arrS4dP = KNew() Vector3[m_iParticleCount];
    m_arrTmpV = KNew() Vector3[m_iParticleCount];
    m_arrS1dV = KNew() Vector3[m_iParticleCount];
    m_arrS2dV = KNew() Vector3[m_iParticleCount];
    m_arrS3dV = KNew() Vector3[m_iParticleCount];
    m_arrS4dV = KNew() Vector3[m_iParticleCount];

    PhysicsFn->UnSelectMemory();
}
ParticleSystem::~ParticleSystem()
{
    PhysicsFn->SelectMemory( TEXT("Particles") );

    KDeleteA( m_arrMasses );
    KDeleteA( m_arrInvMasses );
    KDeleteA( m_arrPositions );
    KDeleteA( m_arrSpeeds );
    KDeleteA( m_arrTmpP );
    KDeleteA( m_arrS1dP );
    KDeleteA( m_arrS2dP );
    KDeleteA( m_arrS3dP );
    KDeleteA( m_arrS4dP );
    KDeleteA( m_arrTmpV );
    KDeleteA( m_arrS1dV );
    KDeleteA( m_arrS2dV );
    KDeleteA( m_arrS3dV );
    KDeleteA( m_arrS4dV );

    PhysicsFn->UnSelectMemory();
}

Void ParticleSystem::Update( Scalar fTime, Scalar fTimeStep )
{
    // Use a 4th order Runge-Kutta ODE solver
    Scalar fHalfStep = (fTimeStep * 0.5f);
    Scalar fSixthStep = (fTimeStep / 6.0f);
    Scalar fHalfT = fTime + fHalfStep;
    Scalar fLastT = fTime + fTimeStep;
    UInt i;

    // Step 1
    for( i = 0; i < m_iParticleCount; ++i ) {
        if ( m_arrInvMasses[i] > 0.0f ) {
            m_arrS1dP[i] = m_arrSpeeds[i];
            m_arrS1dV[i] = Acceleration( fTime, i, m_arrPositions, m_arrSpeeds );
        }
    }
    for( i = 0; i < m_iParticleCount; ++i ) {
        if ( m_arrInvMasses[i] > 0.0f ) {
            m_arrTmpP[i] = m_arrPositions[i] + ( m_arrS1dP[i] * fHalfStep );
            m_arrTmpV[i] = m_arrSpeeds[i]    + ( m_arrS1dV[i] * fHalfStep );
        } else {
            m_arrTmpP[i] = m_arrPositions[i];
            m_arrTmpV[i] = Vector3::Null;
        }
    }

    // Step 2
    for( i = 0; i < m_iParticleCount; ++i ) {
        if ( m_arrInvMasses[i] > 0.0f ) {
            m_arrS2dP[i] = m_arrTmpV[i];
            m_arrS2dV[i] = Acceleration( fHalfT, i, m_arrTmpP, m_arrTmpV );
        }
    }
    for( i = 0; i < m_iParticleCount; ++i ) {
        if ( m_arrInvMasses[i] > 0.0f ) {
            m_arrTmpP[i] = m_arrPositions[i] + ( m_arrS2dP[i] * fHalfStep );
            m_arrTmpV[i] = m_arrSpeeds[i]    + ( m_arrS2dV[i] * fHalfStep );
        } else {
            m_arrTmpP[i] = m_arrPositions[i];
            m_arrTmpV[i] = Vector3::Null;
        }
    }

    // Step 3
    for( i = 0; i < m_iParticleCount; ++i ) {
        if ( m_arrInvMasses[i] > 0.0f ) {
            m_arrS3dP[i] = m_arrTmpV[i];
            m_arrS3dV[i] = Acceleration( fHalfT, i, m_arrTmpP, m_arrTmpV );
        }
    }
    for( i = 0; i < m_iParticleCount; ++i ) {
        if ( m_arrInvMasses[i] > 0.0f ) {
            m_arrTmpP[i] = m_arrPositions[i] + ( m_arrS3dP[i] * fTimeStep );
            m_arrTmpV[i] = m_arrSpeeds[i]    + ( m_arrS3dV[i] * fTimeStep );
        } else {
            m_arrTmpP[i] = m_arrPositions[i];
            m_arrTmpV[i] = Vector3::Null;
        }
    }

    // Step 4
    for( i = 0; i < m_iParticleCount; ++i ) {
        if ( m_arrInvMasses[i] > 0.0f ) {
            m_arrS4dP[i] = m_arrTmpV[i];
            m_arrS4dV[i] = Acceleration( fLastT, i, m_arrTmpP, m_arrTmpV );
        }
    }
    for( i = 0; i < m_iParticleCount; ++i ) {
        if ( m_arrInvMasses[i] > 0.0f ) {
            m_arrPositions[i] += ( m_arrS1dP[i] + ((m_arrS2dP[i]+m_arrS3dP[i]) * 2.0f) + m_arrS4dP[i] ) * fSixthStep;
            m_arrSpeeds[i]    += ( m_arrS1dV[i] + ((m_arrS2dV[i]+m_arrS3dV[i]) * 2.0f) + m_arrS4dV[i] ) * fSixthStep;
        }
    }
}

