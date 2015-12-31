/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Particles/MassSpringSurface.h
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
// Header prelude
#ifndef KOALA_ENGINE_PHYSICS_PARTICLES_MASSSPRINGSURFACE_H
#define KOALA_ENGINE_PHYSICS_PARTICLES_MASSSPRINGSURFACE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ParticleSystem.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MassSpringSurface class
class MassSpringSurface : public ParticleSystem
{
public:
    MassSpringSurface( UInt iWidth, UInt iHeight );
    virtual ~MassSpringSurface();

    // Getters ( *R gives right link, *D gives down link )
    inline UInt GetWidth() const;
    inline UInt GetHeight() const;
    inline Spring & GetSpringR( UInt iX, UInt iY );
    inline Spring & GetSpringD( UInt iX, UInt iY );
    inline const Spring & GetSpringR( UInt iX, UInt iY ) const;
    inline const Spring & GetSpringD( UInt iX, UInt iY ) const;

    inline Scalar GetMass( UInt iX, UInt iY ) const;
    inline const Vertex3 & GetPosition( UInt iX, UInt iY ) const;
    inline const Vector3 & GetSpeed( UInt iX, UInt iY ) const;

    // Setters
    inline Void SetMass( UInt iX, UInt iY, Scalar fMass );
    inline Void SetPosition( UInt iX, UInt iY, const Vertex3 & vPosition );
    inline Void SetSpeed( UInt iX, UInt iY, const Vector3 & vSpeed );

    // Acceleration function
    virtual Vector3 Acceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                const Vector3 * arrSpeeds ) const;

    // External forces support (gravity, friction, wind, ...)
    virtual Vector3 ExternalAcceleration( Scalar fTime, UInt iParticle, const Vertex3 * arrPositions,
                                                                        const Vector3 * arrSpeeds ) const;

protected:
    // Helpers
    inline UInt _GetSpringIndexR( UInt iX, UInt iY ) const;
    inline UInt _GetSpringIndexD( UInt iX, UInt iY ) const;
    inline UInt _GetParticleIndex( UInt iX, UInt iY ) const;
    inline Void _GetParticleCoords( UInt iParticle, UInt & outX, UInt & outY ) const;

    // (W*H) particles linked by a 4-connectivity spring graph
    // Each particle keeps links to the springs directly right & down of it.
    UInt m_iWidth, m_iHeight;
    UInt m_iWidthM1, m_iHeightM1;
    Spring * m_arrSpringsR; // ( (W-1) * H )
    Spring * m_arrSpringsD; // ( W * (H-1) )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MassSpringSurface.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_PHYSICS_PARTICLES_MASSSPRINGSURFACE_H

