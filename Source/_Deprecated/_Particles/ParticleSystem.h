/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/ParticleSystem.h
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
// Header prelude
#ifndef KOALA_ENGINE_PHYSICS_PARTICLES_PARTICLESYSTEM_H
#define KOALA_ENGINE_PHYSICS_PARTICLES_PARTICLESYSTEM_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Core/Error/ErrorManager.h"

#include "../../../Lib/Core/Math/Types/Vector/Vector3.h"
#include "../../../Lib/Core/Math/Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
typedef struct _spring
{
    Bool bActive;
    Scalar fLength;
    Scalar fElasticity;
    Scalar fFriction;
} Spring;

typedef struct _linked_spring
{
    Bool bActive;
    Scalar fLength;
    Scalar fElasticity;
    Scalar fFriction;

    UInt iParticleA;
    UInt iParticleB;
} LinkedSpring;

/////////////////////////////////////////////////////////////////////////////////
// The ParticleSystem interface
class ParticleSystem
{
public:
    ParticleSystem( UInt iParticleCount );
    virtual ~ParticleSystem();

    // Getters
    inline UInt GetParticleCount() const;
    inline Scalar GetMass( UInt iParticle ) const;
    inline const Vertex3 & GetPosition( UInt iParticle ) const;
    inline const Vector3 & GetSpeed( UInt iParticle ) const;

    // Setters
    inline Void SetMass( UInt iParticle, Scalar fMass );
    inline Void SetPosition( UInt iParticle, const Vertex3 & vPosition );
    inline Void SetSpeed( UInt iParticle, const Vector3 & vSpeed );

    // Update procedure
    virtual Void Update( Scalar fTime, Scalar fTimeStep );

    // Acceleration function
    virtual Vector3 Acceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                const Vector3 * arrSpeeds ) const = 0;

protected:
    // Particles properties
    UInt m_iParticleCount;
    Scalar * m_arrMasses;
    Scalar * m_arrInvMasses;
    Vertex3 * m_arrPositions;
    Vector3 * m_arrSpeeds;

    // Temp arrays for integration steps
    Vertex3 * m_arrTmpP;
    Vector3             *m_arrS1dP, *m_arrS2dP, *m_arrS3dP, *m_arrS4dP;
    Vector3 *m_arrTmpV, *m_arrS1dV, *m_arrS2dV, *m_arrS3dV, *m_arrS4dV;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ParticleSystem.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_PHYSICS_PARTICLES_PARTICLESYSTEM_H

