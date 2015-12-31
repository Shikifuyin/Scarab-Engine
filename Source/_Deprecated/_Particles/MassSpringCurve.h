/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/MassSpringCurve.h
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
// Header prelude
#ifndef KOALA_ENGINE_PHYSICS_PARTICLES_MASSSPRINGCURVE_H
#define KOALA_ENGINE_PHYSICS_PARTICLES_MASSSPRINGCURVE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ParticleSystem.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MassSpringCurve class
class MassSpringCurve : public ParticleSystem
{
public:
    MassSpringCurve( UInt iParticleCount );
    virtual ~MassSpringCurve();

    // Getters
    inline UInt GetSpringCount() const;
    inline Spring & GetSpring( UInt iSpring );
    inline const Spring & GetSpring( UInt iSpring ) const;

    // Acceleration function
    virtual Vector3 Acceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                const Vector3 * arrSpeeds ) const;

    // External forces support (gravity, friction, wind, ...)
    virtual Vector3 ExternalAcceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                        const Vector3 * arrSpeeds ) const;

protected:
    // N particles linked by (N-1) Springs
    UInt m_iSpringCount;
    Spring * m_arrSprings;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MassSpringCurve.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_PHYSICS_PARTICLES_MASSSPRINGCURVE_H

