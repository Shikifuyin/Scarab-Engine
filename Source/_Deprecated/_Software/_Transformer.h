/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Transformer.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Delegate of the Renderer, handles transformations
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
#ifndef KOALA_ENGINE_GRAPHICS_3D_TRANSFORMER_H
#define KOALA_ENGINE_GRAPHICS_3D_TRANSFORMER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Core/Math/Types/Vertex.h"
#include "../../../Lib/Core/Math/Types/Vector.h"
#include "../../../Lib/Core/Math/Types/Matrix.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
class Renderer;

enum FrustrumLimit
{
    FRUSTRUM_NEAR = 0, // View min
    FRUSTRUM_FAR,      // View max
    FRUSTRUM_BOTTOM,   // Up min
    FRUSTRUM_TOP,      // Up max
    FRUSTRUM_LEFT,     // Right min
    FRUSTRUM_RIGHT,    // Right max
    FRUSTRUM_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The Transformer class
class Transformer
{
public:
	Transformer( Renderer * pRenderer );
	~Transformer();

    // Getters
    inline const Vertex3 & GetViewEye() const;
    inline const Vector3 & GetViewDir() const;
    inline const Vector3 & GetViewUp() const;
    inline const Vector3 & GetViewRight() const;

    inline const Scalar * GetFrustrum() const;
    inline Scalar GetFrustrumNear() const;
    inline Scalar GetFrustrumFar() const;
    inline Scalar GetFrustrumBottom() const;
    inline Scalar GetFrustrumTop() const;
    inline Scalar GetFrustrumLeft() const;
    inline Scalar GetFrustrumRight() const;

    // View transform
    Void SetView( const Vertex3 & vEye, const Vector3 & vDir, const Vector3 & vUp );
    Void SetLocation( const Vertex3 & vEye );
    Void SetOrientation( const Vector3 & vDir, const Vector3 & vUp );

    // Clip-space transform
    Void SetFrustrum( Scalar fMinX, Scalar fMaxX, Scalar fMinY, Scalar fMaxY, Scalar fNear, Scalar fFar );
    Void SetFrustrum( const Scalar arrValues[FRUSTRUM_COUNT] );
    Void SetFrustrum( Scalar fFOV, Scalar fAspect, Scalar fNear, Scalar fFar );
    Void GetFrustum( Scalar & outMinX, Scalar & outMaxX, Scalar & outMinY, Scalar & outMaxY, Scalar & outNear, Scalar & outFar ) const;
    Bool GetFrustum( Scalar & outFOV, Scalar & outAspect, Scalar & outNear, Scalar & outFar ) const;

    // Viewport transform
    Void SetViewport( UInt iLeft, UInt iRight, UInt iBottom, UInt iTop, UInt iWidth, UInt iHeight );
    Void GetViewport( UInt & outLeft, UInt & outRight, UInt & outBottom, UInt & outTop, UInt & outWidth, UInt & outHeight ) const;
    Void GetViewport( Scalar & outLeft, Scalar & outRight, Scalar & outBottom, Scalar & outTop ) const;

    // Depth range
    Void SetDepthRange( Scalar fMinZ, Scalar fMaxZ );
    Void GetDepthRange( Scalar & outMinZ, Scalar & outMaxZ ) const;

    // Matrices
    Void GetViewMatrix( Matrix4 & outView ) const;
    Void GetInvViewMatrix( Matrix4 & outInvView ) const;
    Void GetBillboardMatrix( Matrix4 & outBillboard ) const;
    Void GetPerspectiveMatrix( Matrix4 & outProjection ) const;
    Void GetOrthogonalMatrix( Matrix4 & outProjection ) const;

    // Mouse-Picking
    // We assume a right-hand coordinate window for coherence & to
    // avoid confusion, but be careful that most OS's APIs give
    // left-hand coordinates ... (reversing the vertical Y axis)
    // And most of the time you'll want to use left-hand ones ...
    Bool GetPickingRay( Vertex3 & outRayPos, Vector3 & outRayDir,
                        UInt iX, UInt iY, UInt iWidth, UInt iHeight ) const;

private:
    Renderer * m_pRenderer;

    // View transform
    Vertex3 m_vEye;
    Vector3 m_vView;
    Vector3 m_vUp;
    Vector3 m_vRight;

    // Projection transform
    Scalar m_fFrustrum[FRUSTRUM_COUNT];

    // Viewport + Depth-Range transform
    UInt m_iWidth, m_iHeight;
    UInt m_iLeft, m_iRight;   // in [0;W], L < R
    UInt m_iBottom, m_iTop;   // in [0;H], B < T
    Scalar m_fLeft, m_fRight; // in [0;1], L < R
    Scalar m_fBottom, m_fTop; // in [0;1], B < T
    Scalar m_fMinZ, m_fMaxZ;   // in [0;1], N < F
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Transformer.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_TRANSFORMER_H
