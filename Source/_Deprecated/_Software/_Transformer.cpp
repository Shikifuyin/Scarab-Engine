/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Transformer.cpp
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
// Includes
#include "Transformer.h"
#include "Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// Transformer implementation
Transformer::Transformer( Renderer * pRenderer )
{
    // forbid callbacks on creation as
    // this being-created camera has not
    // been returned to the renderer yet ...
    m_pRenderer = NULL;

    SetView( Vertex3::Null, -Vector3::eK, Vector3::eJ );
    SetFrustrum( -0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 2.0f );
    SetViewport(0,0,0,0,0,0);
    SetDepthRange(0.0f,1.0f);

    m_pRenderer = pRenderer;
}
Transformer::~Transformer()
{
    // nothing to do
}

Void Transformer::SetView( const Vertex3 & vEye, const Vector3 & vDir, const Vector3 & vUp )
{
    m_vEye = vEye;
    SetOrientation(vDir, vUp);
}
Void Transformer::SetLocation( const Vertex3 & vEye )
{
    m_vEye = vEye;
    if (m_pRenderer != NULL)
        m_pRenderer->_OnViewChange();
}
Void Transformer::SetOrientation( const Vector3 & vDir, const Vector3 & vUp )
{
    // double-cross trick to get real up vect
    m_vView = vDir;
    m_vView.Normalize();
    m_vRight = m_vView ^ vUp;
    m_vRight.Normalize();
    m_vUp = m_vRight ^ m_vView;
    m_vUp.Normalize();
    if (m_pRenderer != NULL)
        m_pRenderer->_OnViewChange();
}

Void Transformer::SetFrustrum( Scalar fMinX, Scalar fMaxX, Scalar fMinY, Scalar fMaxY, Scalar fNear, Scalar fFar )
{
    m_fFrustrum[FRUSTRUM_NEAR]   = fNear;
    m_fFrustrum[FRUSTRUM_FAR]    = fFar;
    m_fFrustrum[FRUSTRUM_BOTTOM] = fMinY;
    m_fFrustrum[FRUSTRUM_TOP]    = fMaxY;
    m_fFrustrum[FRUSTRUM_LEFT]   = fMinX;
    m_fFrustrum[FRUSTRUM_RIGHT]  = fMaxX;
    if (m_pRenderer != NULL)
        m_pRenderer->_OnFrustrumChange();
}
Void Transformer::SetFrustrum( const Scalar arrValues[FRUSTRUM_COUNT] )
{
    MemCopy( m_fFrustrum, arrValues, FRUSTRUM_COUNT * sizeof(Scalar) );
    if (m_pRenderer != NULL)
        m_pRenderer->_OnFrustrumChange();
}
Void Transformer::SetFrustrum( Scalar fFOV, Scalar fAspect, Scalar fNear, Scalar fFar )
{
    // FOV is the vertical angle of view, in radians
    m_fFrustrum[FRUSTRUM_TOP]    = fNear * MathFn->Tan(0.5f * fFOV);
    m_fFrustrum[FRUSTRUM_RIGHT]  = fAspect * m_fFrustrum[FRUSTRUM_TOP];
    m_fFrustrum[FRUSTRUM_BOTTOM] = -m_fFrustrum[FRUSTRUM_TOP];
    m_fFrustrum[FRUSTRUM_LEFT]   = -m_fFrustrum[FRUSTRUM_RIGHT];
    m_fFrustrum[FRUSTRUM_NEAR]   = fNear;
    m_fFrustrum[FRUSTRUM_FAR]    = fFar;
    if (m_pRenderer != NULL)
        m_pRenderer->_OnFrustrumChange();
}
Void Transformer::GetFrustum( Scalar & outMinX, Scalar & outMaxX, Scalar & outMinY, Scalar & outMaxY, Scalar & outNear, Scalar & outFar ) const
{
    outNear = m_fFrustrum[FRUSTRUM_NEAR];
    outFar  = m_fFrustrum[FRUSTRUM_FAR];
    outMinY = m_fFrustrum[FRUSTRUM_BOTTOM];
    outMaxY = m_fFrustrum[FRUSTRUM_TOP];
    outMinX = m_fFrustrum[FRUSTRUM_LEFT];
    outMaxX = m_fFrustrum[FRUSTRUM_RIGHT];
}
Bool Transformer::GetFrustum( Scalar & outFOV, Scalar & outAspect, Scalar & outNear, Scalar & outFar ) const
{
    if ( m_fFrustrum[FRUSTRUM_LEFT] == -m_fFrustrum[FRUSTRUM_RIGHT] &&
         m_fFrustrum[FRUSTRUM_BOTTOM] == -m_fFrustrum[FRUSTRUM_TOP] ) {
        Scalar fTmp = m_fFrustrum[FRUSTRUM_TOP] / m_fFrustrum[FRUSTRUM_NEAR];
        outFOV = 2.0f * MathFn->ArcTan(fTmp);
        outAspect = m_fFrustrum[FRUSTRUM_RIGHT] / m_fFrustrum[FRUSTRUM_TOP];
        outNear = m_fFrustrum[FRUSTRUM_NEAR];
        outFar = m_fFrustrum[FRUSTRUM_FAR];
        return true;
    }
    return false;
}

Void Transformer::SetViewport( UInt iLeft, UInt iRight, UInt iBottom, UInt iTop, UInt iWidth, UInt iHeight )
{
    if (iWidth > 0 && iHeight > 0) {
        if ( iLeft > iWidth || iRight > iWidth )
            return;
        if ( iBottom > iHeight || iTop > iHeight )
            return;
        m_iWidth = iWidth;
        m_iHeight = iHeight;
        m_iLeft = iLeft;
        m_iRight = iRight;
        m_iBottom = iBottom;
        m_iTop = iTop;
        m_fLeft = (Scalar)iLeft / (Scalar)iWidth;
        m_fRight = (Scalar)iRight / (Scalar)iWidth;
        m_fBottom = (Scalar)iBottom / (Scalar)iHeight;
        m_fTop = (Scalar)iTop / (Scalar)iHeight;
    } else {
        m_iWidth = 0;
        m_iHeight = 0;
        m_iLeft = 0;
        m_iRight = 0;
        m_iBottom = 0;
        m_iTop = 0;
        m_fLeft = 0.0f;
        m_fRight = 1.0f;
        m_fBottom = 0.0f;
        m_fTop = 1.0f;
    }
    if (m_pRenderer)
        m_pRenderer->_OnViewportChange();
}
Void Transformer::GetViewport( UInt & outLeft, UInt & outRight, UInt & outBottom, UInt & outTop, UInt & outWidth, UInt & outHeight ) const
{
    outLeft = m_iLeft;
    outRight = m_iRight;
    outBottom = m_iBottom;
    outTop = m_iTop;
    outWidth = m_iWidth;
    outHeight = m_iHeight;
}
Void Transformer::GetViewport( Scalar & outLeft, Scalar & outRight, Scalar & outBottom, Scalar & outTop ) const
{
    outLeft = m_fLeft;
    outRight = m_fRight;
    outBottom = m_fBottom;
    outTop = m_fTop;
}

Void Transformer::SetDepthRange( Scalar fMinZ, Scalar fMaxZ )
{
    m_fMinZ = fMinZ;
    m_fMaxZ = fMaxZ;
    if (m_pRenderer)
        m_pRenderer->_OnDepthRangeChange();
}
Void Transformer::GetDepthRange( Scalar & outMinZ, Scalar & outMaxZ ) const
{
    outMinZ = m_fMinZ;
    outMaxZ = m_fMaxZ;
}

Void Transformer::GetViewMatrix( Matrix4 & outView ) const
{
    Vector3 vEyeVect(m_vEye.X, m_vEye.Y, m_vEye.Z);
    outView.m00 = m_vRight.X; outView.m01 = m_vRight.Y; outView.m02 = m_vRight.Z; outView.m03 = -(vEyeVect * m_vRight);
    outView.m10 = m_vUp.X;    outView.m11 = m_vUp.Y;    outView.m12 = m_vUp.Z;    outView.m13 = -(vEyeVect * m_vUp);
    outView.m20 = m_vView.X;  outView.m21 = m_vView.Y;  outView.m22 = m_vView.Z;  outView.m23 = -(vEyeVect * m_vView);
    outView.m30 = 0.0f;       outView.m31 = 0.0f;       outView.m32 = 0.0f;       outView.m33 = 1.0f;
}
Void Transformer::GetInvViewMatrix( Matrix4 & outInvView ) const
{
    outInvView.m00 = m_vRight.X; outInvView.m01 = m_vUp.X; outInvView.m02 = m_vView.X; outInvView.m03 = m_vEye.X;
    outInvView.m10 = m_vRight.Y; outInvView.m11 = m_vUp.Y; outInvView.m12 = m_vView.Y; outInvView.m13 = m_vEye.Y;
    outInvView.m20 = m_vRight.Z; outInvView.m21 = m_vUp.Z; outInvView.m22 = m_vView.Z; outInvView.m23 = m_vEye.Z;
    outInvView.m30 = 0.0f;       outInvView.m31 = 0.0f;    outInvView.m32 = 0.0f;      outInvView.m33 = 1.0f;
}
Void Transformer::GetBillboardMatrix( Matrix4 & outBillboard ) const
{
    outBillboard.m00 = m_vRight.X; outBillboard.m01 = m_vUp.X; outBillboard.m02 = m_vView.X; outBillboard.m03 = 0.0f;
    outBillboard.m10 = m_vRight.Y; outBillboard.m11 = m_vUp.Y; outBillboard.m12 = m_vView.Y; outBillboard.m13 = 0.0f;
    outBillboard.m20 = m_vRight.Z; outBillboard.m21 = m_vUp.Z; outBillboard.m22 = m_vView.Z; outBillboard.m23 = 0.0f;
    outBillboard.m30 = 0.0f;       outBillboard.m31 = 0.0f;    outBillboard.m32 = 0.0f;      outBillboard.m33 = 1.0f;
}
Void Transformer::GetPerspectiveMatrix( Matrix4 & outProjection ) const
{
    Scalar fInvDiffX = MathFn->Invert( m_fFrustrum[FRUSTRUM_RIGHT] - m_fFrustrum[FRUSTRUM_LEFT] );
    Scalar fInvDiffY = MathFn->Invert( m_fFrustrum[FRUSTRUM_BOTTOM] - m_fFrustrum[FRUSTRUM_TOP] );
    Scalar fInvDiffZ = MathFn->Invert( m_fFrustrum[FRUSTRUM_FAR] - m_fFrustrum[FRUSTRUM_NEAR] );

    // Maps (x,y,z) to [-1;1]x[-1;1]x[0;1]xW
    // Stores original z as W
    outProjection.m00 = 2.0f * m_fFrustrum[FRUSTRUM_NEAR] * fInvDiffX;
    outProjection.m10 = 0.0f;
    outProjection.m20 = 0.0f;
    outProjection.m30 = 0.0f;

    outProjection.m01 = 0.0f;
    outProjection.m11 = 2.0f * m_fFrustrum[FRUSTRUM_NEAR] * fInvDiffY;
    outProjection.m21 = 0.0f;
    outProjection.m31 = 0.0f;

    outProjection.m02 = -(m_fFrustrum[FRUSTRUM_RIGHT] + m_fFrustrum[FRUSTRUM_LEFT]) * fInvDiffX;
    outProjection.m12 = -(m_fFrustrum[FRUSTRUM_TOP] + m_fFrustrum[FRUSTRUM_BOTTOM]) * fInvDiffY;
    outProjection.m22 = m_fFrustrum[FRUSTRUM_FAR] * fInvDiffZ;
    outProjection.m32 = 1.0f;

    outProjection.m03 = 0.0f;
    outProjection.m13 = 0.0f;
    outProjection.m23 = -m_fFrustrum[FRUSTRUM_FAR] * m_fFrustrum[FRUSTRUM_NEAR] * fInvDiffZ;
    outProjection.m33 = 0.0f;
}
Void Transformer::GetOrthogonalMatrix( Matrix4 & outProjection ) const
{
    Scalar fInvDiffX = MathFn->Invert( m_fFrustrum[FRUSTRUM_RIGHT] - m_fFrustrum[FRUSTRUM_LEFT] );
    Scalar fInvDiffY = MathFn->Invert( m_fFrustrum[FRUSTRUM_BOTTOM] - m_fFrustrum[FRUSTRUM_TOP] );
    Scalar fInvDiffZ = MathFn->Invert( m_fFrustrum[FRUSTRUM_FAR] - m_fFrustrum[FRUSTRUM_NEAR] );

    // Maps (x,y,z) to [-1;1]x[-1;1]x[0;1]
    outProjection.m00 = 2.0f * fInvDiffX;
    outProjection.m10 = 0.0f;
    outProjection.m20 = 0.0f;
    outProjection.m30 = 0.0f;

    outProjection.m01 = 0.0f;
    outProjection.m11 = 2.0f * fInvDiffY;
    outProjection.m21 = 0.0f;
    outProjection.m31 = 0.0f;

    outProjection.m02 = 0.0f;
    outProjection.m12 = 0.0f;
    outProjection.m22 = fInvDiffZ;
    outProjection.m32 = 0.0f;

    outProjection.m03 = -(m_fFrustrum[FRUSTRUM_LEFT] + m_fFrustrum[FRUSTRUM_RIGHT]) * fInvDiffX;
    outProjection.m13 = -(m_fFrustrum[FRUSTRUM_BOTTOM] + m_fFrustrum[FRUSTRUM_TOP]) * fInvDiffY;
    outProjection.m23 = -m_fFrustrum[FRUSTRUM_NEAR] * fInvDiffZ;
    outProjection.m33 = 1.0f;
}

Bool Transformer::GetPickingRay( Vertex3 & outRayPos, Vector3 & outRayDir,
                                 UInt iX, UInt iY, UInt iWidth, UInt iHeight ) const
{
    Scalar fVPX = ((Scalar)iX) / (Scalar)(iWidth - 1);
    if ( fVPX < m_fLeft || fVPX > m_fRight )
        return false;
    Scalar fVPY = ((Scalar)iY) / (Scalar)(iHeight - 1);
    if ( fVPY < m_fBottom || fVPY > m_fTop )
        return false;

    Scalar fWeightX = (fVPX - m_fLeft) / (m_fRight - m_fLeft);
    Scalar fWeightY = (fVPY - m_fBottom) / (m_fTop - m_fBottom);
    Scalar fViewX = (1.0f - fWeightX) * m_fFrustrum[FRUSTRUM_LEFT] + fWeightX * m_fFrustrum[FRUSTRUM_RIGHT];
    Scalar fViewY = (1.0f - fWeightY) * m_fFrustrum[FRUSTRUM_BOTTOM] + fWeightY * m_fFrustrum[FRUSTRUM_TOP];

    outRayPos = m_vEye;
    outRayDir = (m_vView * m_fFrustrum[FRUSTRUM_NEAR]) + (m_vRight * fViewX) + (m_vUp * fViewY);
    outRayDir.Normalize();
    return true;
}