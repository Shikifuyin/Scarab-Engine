/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/MassSpringSystem.h
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
// Header prelude
#ifndef KOALA_ENGINE_PHYSICS_PARTICLES_MASSSPRINGSYSTEM_H
#define KOALA_ENGINE_PHYSICS_PARTICLES_MASSSPRINGSYSTEM_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Core/Datastruct/Array/Array.h"

#include "ParticleSystem.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define MASSSPRING_ADJACENCY_PAGESIZE 64

/////////////////////////////////////////////////////////////////////////////////
// The MassSpringSystem class
class MassSpringSystem : public ParticleSystem
{
public:
    MassSpringSystem( UInt iParticleCount, UInt iSpringCount );
    virtual ~MassSpringSystem();

    // Spring construction (you MUST init all springs with this)
    Void InitSpring( UInt iSpring, UInt iParticleA, UInt iParticleB,
                     Bool bActive = true, Scalar fLength = 1.0f, Scalar fElasticity = 1.0f, Scalar fFriction = 0.0f );
    inline Void InitSpring( UInt iSpring, const LinkedSpring & newSpring );

    // Getters
    inline UInt GetSpringCount() const;
    inline Spring & GetSpring( UInt iSpring );
    inline const LinkedSpring & GetSpring( UInt iSpring ) const;

    // Acceleration function
    virtual Vector3 Acceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                const Vector3 * arrSpeeds ) const;

    // External forces support (gravity, friction, wind, ...)
    virtual Vector3 ExternalAcceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                        const Vector3 * arrSpeeds ) const;

protected:
    // Springs
    UInt m_iSpringCount;
    LinkedSpring * m_arrSprings;
    Array<UInt> * m_mapAdjacency;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MassSpringSystem.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_PHYSICS_PARTICLES_MASSSPRINGSYSTEM_H

