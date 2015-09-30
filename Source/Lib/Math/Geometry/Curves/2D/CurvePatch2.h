/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/2D/CurvePatch2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 2D : Single patch definition
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVEPATCH2_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVEPATCH2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Analysis/Integration/Integrator1.h"

#include "Curve2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TCurvePatch2 class
template<typename Real>
class TCurvePatch2 : public TCurve2<Real>
{
public:
    TCurvePatch2( Real fMinT, Real fMaxT );
    virtual ~TCurvePatch2();

    // Length access
    virtual Real Length( Real fT0, Real fT1 ) const;
    virtual Real Parameter( Real fLength, UInt iMaxIterations = 32, Real fTolerance = (Real)SCALAR_ERROR ) const;

private:
    inline static Real _SpeedFunction( Real fT, Void * pInstance );
};

// Explicit instanciation
typedef TCurvePatch2<Float> CurvePatch2f;
typedef TCurvePatch2<Double> CurvePatch2d;
typedef TCurvePatch2<Scalar> CurvePatch2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CurvePatch2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_2D_CURVEPATCH2_H

