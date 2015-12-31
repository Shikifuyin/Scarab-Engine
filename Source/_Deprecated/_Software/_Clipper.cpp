/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Clipper.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Delegate of the Renderer, handles clipping process ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Clipper.h"
#include "Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// Clipper implementation
Clipper::Clipper( Renderer * pRenderer )
{
    m_pRenderer = pRenderer;
    m_pCFL = NULL;

    m_iPlaneCount = FRUSTRUM_PLANE_COUNT;

    _DblQueue_Reset();
}
Clipper::~Clipper()
{
    // nothing to do
}

Void Clipper::ExtractFrustrumPlanes()
{
    Transformer * pCamera = m_pRenderer->m_pCamera;
    Scalar fLeft, fRight, fBottom, fTop, fNear, fFar;
    pCamera->GetFrustum( fLeft, fRight, fBottom, fTop, fNear, fFar );
    Scalar fTanFOV = fTop / fNear;
	Scalar fAspectTanFOV = fTanFOV * (fRight / fTop);

	Vertex3 vEye = pCamera->GetViewEye();
	Vector3 vView = pCamera->GetViewDir();
	Vector3 vUp = pCamera->GetViewUp();
	Vector3 vRight = pCamera->GetViewRight();

	m_arrClippingPlanes[FRUSTRUM_PLANE_NEAR]   = Plane( vView,    (vEye + (vView * fNear)) );
	m_arrClippingPlanes[FRUSTRUM_PLANE_FAR]    = Plane( (-vView), (vEye + (vView * fFar)) );
    m_arrClippingPlanes[FRUSTRUM_PLANE_BOTTOM] = Plane( ((vView * fTanFOV) + vUp), vEye );
	m_arrClippingPlanes[FRUSTRUM_PLANE_TOP]    = Plane( ((vView * fTanFOV) - vUp), vEye );
	m_arrClippingPlanes[FRUSTRUM_PLANE_LEFT]   = Plane( ((vView * fAspectTanFOV) + vRight), vEye );
	m_arrClippingPlanes[FRUSTRUM_PLANE_RIGHT]  = Plane( ((vView * fAspectTanFOV) - vRight), vEye );
}

Bool Clipper::CullVertex( CFLPtr pVertexA )
{
    const Vertex4 * pPosA = (const Vertex4*)( m_pCFL->GetField( pVertexA, CFLSEMANTIC_POSITION, 0) );

    for(UInt i = 0; i < m_iPlaneCount; ++i) {
        if ( !_CullCheck_Vertex( *pPosA, i ) )
            return false;
    }
    m_pRenderer->_ClippingOutput_Vertex(pVertexA);
    return true;
}

Bool Clipper::CullSegment( CFLPtr pVertexA, CFLPtr pVertexB, Bool bRejectClipped )
{
    ClippingSegment_Infos clipInfos;
    clipInfos.vA = *( (const Vertex4*)( m_pCFL->GetField(pVertexA, CFLSEMANTIC_POSITION, 0) ) );
    clipInfos.vB = *( (const Vertex4*)( m_pCFL->GetField(pVertexB, CFLSEMANTIC_POSITION, 0) ) );

	for(UInt i = 0; i < m_iPlaneCount; ++i) {
        IntersectFn->Intersect3D( m_arrClippingPlanes[i], clipInfos.vA, &(clipInfos.intersectA[i]) );
        IntersectFn->Intersect3D( m_arrClippingPlanes[i], clipInfos.vB, &(clipInfos.intersectB[i]) );
		if ( !_CullCheck_Segment( clipInfos, i ) )
			return false;
        if ( bRejectClipped && clipInfos.bClipsPlane[i] )
            return false;
    }
    m_pRenderer->_ClippingOutput_Segment(pVertexA, pVertexB);
	return true;
}
Bool Clipper::ClipSegment( CFLPtr pVertexA, CFLPtr pVertexB )
{
    ClippingSegment_Infos clipInfos;
    clipInfos.vA = *( (const Vertex4*)( m_pCFL->GetField(pVertexA, CFLSEMANTIC_POSITION, 0) ) );
    clipInfos.vB = *( (const Vertex4*)( m_pCFL->GetField(pVertexB, CFLSEMANTIC_POSITION, 0) ) );
    clipInfos.pVertexA = pVertexA;
    clipInfos.pVertexB = pVertexB;
    clipInfos.bClipped = false;

	for(UInt i = 0; i < m_iPlaneCount; ++i) {
        IntersectFn->Intersect3D( m_arrClippingPlanes[i], clipInfos.vA, &(clipInfos.intersectA[i]) );
        IntersectFn->Intersect3D( m_arrClippingPlanes[i], clipInfos.vB, &(clipInfos.intersectB[i]) );
		if ( !_CullCheck_Segment( clipInfos, i ) )
			return false;
        if ( clipInfos.bClipsPlane[i] )
            clipInfos.bClipped = true;
    }
    if (clipInfos.bClipped)
        _ClippingProcess_Segment(clipInfos);
    else
        m_pRenderer->_ClippingOutput_Segment(pVertexA, pVertexB);
    return true;
}

Bool Clipper::CullTriangle( CFLPtr pVertexA, CFLPtr pVertexB, CFLPtr pVertexC, Bool bRejectClipped )
{
    ClippingTriangle_Infos clipInfos;
    clipInfos.vA = *( (const Vertex4*)( m_pCFL->GetField(pVertexA, CFLSEMANTIC_POSITION, 0) ) );
    clipInfos.vB = *( (const Vertex4*)( m_pCFL->GetField(pVertexB, CFLSEMANTIC_POSITION, 0) ) );
    clipInfos.vC = *( (const Vertex4*)( m_pCFL->GetField(pVertexC, CFLSEMANTIC_POSITION, 0) ) );

	for(UInt i = 0; i < m_iPlaneCount; ++i) {
        IntersectFn->Intersect3D( m_arrClippingPlanes[i], clipInfos.vA, &(clipInfos.intersectA[i]) );
        IntersectFn->Intersect3D( m_arrClippingPlanes[i], clipInfos.vB, &(clipInfos.intersectB[i]) );
        IntersectFn->Intersect3D( m_arrClippingPlanes[i], clipInfos.vC, &(clipInfos.intersectC[i]) );
		if ( !_CullCheck_Triangle( clipInfos, i ) )
			return false;
        if ( bRejectClipped && clipInfos.bClipsPlane[i] )
            return false;
    }
    m_pRenderer->_ClippingOutput_Triangle(pVertexA, pVertexB, pVertexC);
	return true;
}
Bool Clipper::ClipTriangle( CFLPtr pVertexA, CFLPtr pVertexB, CFLPtr pVertexC )
{
    ClippingTriangle_Infos clipInfos;
    clipInfos.vA = *( (const Vertex4*)( m_pCFL->GetField(pVertexA, CFLSEMANTIC_POSITION, 0) ) );
    clipInfos.vB = *( (const Vertex4*)( m_pCFL->GetField(pVertexB, CFLSEMANTIC_POSITION, 0) ) );
    clipInfos.vC = *( (const Vertex4*)( m_pCFL->GetField(pVertexC, CFLSEMANTIC_POSITION, 0) ) );
    clipInfos.pVertexA = pVertexA;
    clipInfos.pVertexB = pVertexB;
    clipInfos.pVertexC = pVertexC;
    clipInfos.bClipped = false;

	for(UInt i = 0; i < m_iPlaneCount; ++i) {
        IntersectFn->Intersect3D( m_arrClippingPlanes[i], clipInfos.vA, &(clipInfos.intersectA[i]) );
        IntersectFn->Intersect3D( m_arrClippingPlanes[i], clipInfos.vB, &(clipInfos.intersectB[i]) );
        IntersectFn->Intersect3D( m_arrClippingPlanes[i], clipInfos.vC, &(clipInfos.intersectC[i]) );
		if ( !_CullCheck_Triangle( clipInfos, i ) )
			return false;
        if ( clipInfos.bClipsPlane[i] )
            clipInfos.bClipped = true;
    }
    if (clipInfos.bClipped)
        _ClippingProcess_Triangle(clipInfos);
    else
        m_pRenderer->_ClippingOutput_Triangle(pVertexA, pVertexB, pVertexC);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Bool Clipper::_CullCheck_Vertex( const Vertex3 & vPos, UInt iPlane ) const
{
    IntersectorResult intRes;
    IntersectFn->Intersect3D( m_arrClippingPlanes[iPlane], vPos, &intRes );
    if (intRes.iRelative == INTERSECT_RELATIVE_BACK)
        return false;
    return true;
}
Bool Clipper::_CullCheck_Segment( ClippingSegment_Infos & clippingInfos, UInt iPlane ) const
{
    // Fills clippingInfos "bClipsPlane" flags for the plane iPlane,
    // using intersector's results to check if we need clipping,
    // returns whether the triangle is globally culled in or not.
    if ( (clippingInfos.intersectA[iPlane].iRelative != INTERSECT_RELATIVE_BACK) &&
	     (clippingInfos.intersectB[iPlane].iRelative != INTERSECT_RELATIVE_BACK) ) {
	    // all vertices are either planar or on good side
        clippingInfos.bClipsPlane[iPlane] = false;
        return true;
    } else {
	    // at least 1 vertice on bad side
	    if ( (clippingInfos.intersectA[iPlane].iRelative == INTERSECT_RELATIVE_FRONT) ||
		     (clippingInfos.intersectB[iPlane].iRelative == INTERSECT_RELATIVE_FRONT) ) {
		    // at least 1 vertex on good side
            clippingInfos.bClipsPlane[iPlane] = true;
            return true;
	    } else {
		    // no vertex is on the good side
            clippingInfos.bClipsPlane[iPlane] = false;
            return false;
	    }
    }
}
Bool Clipper::_CullCheck_Triangle( ClippingTriangle_Infos & clippingInfos, UInt iPlane ) const
{
    // Fills clippingInfos "bClipsPlane" flags for the plane iPlane,
    // using intersector's results to check if we need clipping,
    // returns whether the triangle is globally culled in or not.
    if ( (clippingInfos.intersectA[iPlane].iRelative != INTERSECT_RELATIVE_BACK) &&
	     (clippingInfos.intersectB[iPlane].iRelative != INTERSECT_RELATIVE_BACK) &&
	     (clippingInfos.intersectC[iPlane].iRelative != INTERSECT_RELATIVE_BACK) ) {
	    // all vertices are either planar or on good side
        clippingInfos.bClipsPlane[iPlane] = false;
        return true;
    } else {
	    // at least 1 vertice on bad side
	    if ( (clippingInfos.intersectA[iPlane].iRelative == INTERSECT_RELATIVE_FRONT) ||
		     (clippingInfos.intersectB[iPlane].iRelative == INTERSECT_RELATIVE_FRONT) ||
		     (clippingInfos.intersectC[iPlane].iRelative == INTERSECT_RELATIVE_FRONT) ) {
		    // at least 1 vertex on good side
            clippingInfos.bClipsPlane[iPlane] = true;
            return true;
	    } else {
		    // no vertex is on the good side
            clippingInfos.bClipsPlane[iPlane] = false;
            return false;
	    }
    }
}

Bool Clipper::_ClipToPlane_Segment( ClippingSegment_Infos & outputInfos, const ClippingSegment_Infos & clipInfos, UInt iPlane ) const
{
    // Clips a segment against one plane and updates vertices,
    // gives new segment, and updates intersection data for
    // subsequent planes.
    // Also gathers & transmits interpolation data ...
    UInt i;
    Bool bCulled = true;
    Scalar paramT1;
    Vertex3 vC;
    Line3D seg1 = Line3D( clipInfos.vA, clipInfos.vB );
    IntersectFn->Intersect3D( paramT1, m_arrClippingPlanes[iPlane], seg1, NULL );
    InterpolateFn->Lerp( vC, clipInfos.vA, clipInfos.vB, paramT1 );
    if ( clipInfos.intersectA[iPlane].iRelative == INTERSECT_RELATIVE_FRONT ) {
        // A front, B back
        outputInfos.vA = clipInfos.vA;
        outputInfos.vB = vC;
        // update result's clipping infos
		outputInfos.bClipped = false;
		for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
			outputInfos.bClipsPlane[i] = false;
			if ( clipInfos.bClipsPlane[i] ) {
				outputInfos.intersectA[i] = clipInfos.intersectA[i];
                IntersectFn->Intersect3D( m_arrClippingPlanes[i], vC, &(outputInfos.intersectB[i]) );
                bCulled = _CullCheck_Segment( outputInfos, i );
                if (!bCulled)
                    break;
                if (outputInfos.bClipsPlane[i])
                    outputInfos.bClipped = true;
			}
		}
        // output the segment
        if (bCulled) {
            outputInfos.pVertexA = clipInfos.pVertexA;
		    outputInfos.pVertexB = clipInfos.pVertexB;
            _ClipInterpolate( outputInfos.pVertexB, clipInfos.pVertexA, clipInfos.pVertexB, paramT1 );
            return true;
        }
    } else {
        // A back, B front
        outputInfos.vA = vC;
        outputInfos.vB = clipInfos.vB;
        // update result's clipping infos
		outputInfos.bClipped = false;
		for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
			outputInfos.bClipsPlane[i] = false;
			if ( clipInfos.bClipsPlane[i] ) {
                IntersectFn->Intersect3D( m_arrClippingPlanes[i], vC, &(outputInfos.intersectA[i]) );
                outputInfos.intersectB[i] = clipInfos.intersectB[i];
                bCulled = _CullCheck_Segment( outputInfos, i );
                if (!bCulled)
                    break;
                if (outputInfos.bClipsPlane[i])
                    outputInfos.bClipped = true;
			}
		}
        // output the segment
        if (bCulled) {
            outputInfos.pVertexA = clipInfos.pVertexA;
		    outputInfos.pVertexB = clipInfos.pVertexB;
            _ClipInterpolate( outputInfos.pVertexA, clipInfos.pVertexA, clipInfos.pVertexB, paramT1 );
            return true;
        }
    }
    return false;
}
UInt Clipper::_ClipToPlane_Triangle( ClippingTriangle_Infos outputInfos[2], const ClippingTriangle_Infos & clipInfos, UInt iPlane ) const
{
    // Clips a triangle against one plane and computes new vertices,
    // gives new triangle(s) if there is(are), and updates intersection
    // data for subsequent planes.
    // Also gathers & transmits interpolation data ...
	UInt i;
	Bool bCulled = true;
	Line3D seg1, seg2;
	Scalar paramT1, paramT2; // interpolation data
	Vertex3 vD, vE; // new vertices

	UInt iOutputCount = 0;

	// Special case : A is planar
	if ( clipInfos.intersectA[iPlane].iRelative == INTERSECT_RELATIVE_PLANAR ) {
		seg1 = Line3D( clipInfos.vB, clipInfos.vC );
        IntersectFn->Intersect3D( paramT1, m_arrClippingPlanes[iPlane], seg1, NULL );
        InterpolateFn->Lerp( vD, clipInfos.vB, clipInfos.vC, paramT1 );
		if ( clipInfos.intersectB[iPlane].iRelative == INTERSECT_RELATIVE_FRONT ) {
			// A planar, B front, C back, result triangle = ABD
			outputInfos[iOutputCount].vA = clipInfos.vA;
			outputInfos[iOutputCount].vB = clipInfos.vB;
			outputInfos[iOutputCount].vC = vD;
			// update result's clipping infos
			outputInfos[iOutputCount].bClipped = false;
			for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
				outputInfos[iOutputCount].bClipsPlane[i] = false;
				if ( clipInfos.bClipsPlane[i] ) {
					outputInfos[iOutputCount].intersectA[i] = clipInfos.intersectA[i];
					outputInfos[iOutputCount].intersectB[i] = clipInfos.intersectB[i];
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectC[i]) );
                    bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
                    if (!bCulled)
                        break;
                    if (outputInfos[iOutputCount].bClipsPlane[i])
                        outputInfos[iOutputCount].bClipped = true;
				}
			}
			// output this triangle
            if (bCulled) {
                outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                _ClipCopy( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA );
                _ClipCopy( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexB );
                _ClipInterpolate( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexB, clipInfos.pVertexC, paramT1 );
                ++iOutputCount;
            }
		} else {
			// A planar, B back, C front, result triangle = ADC
			outputInfos[iOutputCount].vA = clipInfos.vA;
			outputInfos[iOutputCount].vB = vD;
			outputInfos[iOutputCount].vC = clipInfos.vC;
			// update result's clipping infos
			outputInfos[iOutputCount].bClipped = false;
			for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
				outputInfos[iOutputCount].bClipsPlane[i] = false;
				if ( clipInfos.bClipsPlane[i] ) {
					outputInfos[iOutputCount].intersectA[i] = clipInfos.intersectA[i];
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectB[i]) );
					outputInfos[iOutputCount].intersectC[i] = clipInfos.intersectC[i];
                    bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
                    if (!bCulled)
                        break;
                    if (outputInfos[iOutputCount].bClipsPlane[i])
                        outputInfos[iOutputCount].bClipped = true;
				}
			}
			// output this triangle
            if (bCulled) {
                outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                _ClipCopy( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA );
                _ClipInterpolate( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexB, clipInfos.pVertexC, paramT1 );
                _ClipCopy( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexC );
                ++iOutputCount;
            }
		}
        m_pCFL->Free(clipInfos.pVertexA);
        m_pCFL->Free(clipInfos.pVertexB);
        m_pCFL->Free(clipInfos.pVertexC);
		return iOutputCount;
	}

    // Special case : B is planar
	if ( clipInfos.intersectB[iPlane].iRelative == INTERSECT_RELATIVE_PLANAR ) {
		seg1 = Line3D( clipInfos.vA, clipInfos.vC );
        IntersectFn->Intersect3D( paramT1, m_arrClippingPlanes[iPlane], seg1, NULL );
        InterpolateFn->Lerp( vD, clipInfos.vA, clipInfos.vC, paramT1 );
		if ( clipInfos.intersectA[iPlane].iRelative == INTERSECT_RELATIVE_FRONT ) {
			// A front, B planar, C back, result triangle = ABD
			outputInfos[iOutputCount].vA = clipInfos.vA;
			outputInfos[iOutputCount].vB = clipInfos.vB;
			outputInfos[iOutputCount].vC = vD;
			// update result's clipping infos
			outputInfos[iOutputCount].bClipped = false;
			for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
				outputInfos[iOutputCount].bClipsPlane[i] = false;
				if ( clipInfos.bClipsPlane[i] ) {
					outputInfos[iOutputCount].intersectA[i] = clipInfos.intersectA[i];
					outputInfos[iOutputCount].intersectB[i] = clipInfos.intersectB[i];
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectC[i]) );
                    bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
					if (!bCulled)
                        break;
					if (outputInfos[iOutputCount].bClipsPlane[i])
						outputInfos[iOutputCount].bClipped = true;
				}
			}
			// output this triangle
            if (bCulled) {
                outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                _ClipCopy( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA );
                _ClipCopy( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexB );
                _ClipInterpolate( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexA, clipInfos.pVertexC, paramT1 );
                ++iOutputCount;
            }
		} else {
			// A back, B planar, C front, result triangle = DBC
			outputInfos[iOutputCount].vA = vD;
			outputInfos[iOutputCount].vB = clipInfos.vB;
			outputInfos[iOutputCount].vC = clipInfos.vC;
			// update result's clipping infos
			outputInfos[iOutputCount].bClipped = false;
			for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
				outputInfos[iOutputCount].bClipsPlane[i] = false;
				if ( clipInfos.bClipsPlane[i] ) {
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectA[i]) );
					outputInfos[iOutputCount].intersectB[i] = clipInfos.intersectB[i];
					outputInfos[iOutputCount].intersectC[i] = clipInfos.intersectC[i];
                    bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
					if (!bCulled)
                        break;
					if (outputInfos[iOutputCount].bClipsPlane[i])
						outputInfos[iOutputCount].bClipped = true;
				}
			}
			// output this triangle
            if (bCulled) {
                outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                _ClipInterpolate( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA, clipInfos.pVertexC, paramT1 );
                _ClipCopy( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexB );
                _ClipCopy( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexC );
                ++iOutputCount;
            }
		}
        m_pCFL->Free(clipInfos.pVertexA);
        m_pCFL->Free(clipInfos.pVertexB);
        m_pCFL->Free(clipInfos.pVertexC);
		return iOutputCount;
	}

    // Special case : C is planar
	if ( clipInfos.intersectC[iPlane].iRelative == INTERSECT_RELATIVE_PLANAR ) {
        seg1 = Line3D( clipInfos.vA, clipInfos.vB );
        IntersectFn->Intersect3D( paramT1, m_arrClippingPlanes[iPlane], seg1, NULL );
        InterpolateFn->Lerp( vD, clipInfos.vA, clipInfos.vB, paramT1 );
		if ( clipInfos.intersectA[iPlane].iRelative == INTERSECT_RELATIVE_FRONT ) {
			// A front, B back, C planar, result triangle = ADC
			outputInfos[iOutputCount].vA = clipInfos.vA;
			outputInfos[iOutputCount].vB = vD;
			outputInfos[iOutputCount].vC = clipInfos.vC;
			// update result's clipping infos
			outputInfos[iOutputCount].bClipped = false;
			for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
				outputInfos[iOutputCount].bClipsPlane[i] = false;
				if ( clipInfos.bClipsPlane[i] ) {
					outputInfos[iOutputCount].intersectA[i] = clipInfos.intersectA[i];
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectB[i]) );
					outputInfos[iOutputCount].intersectC[i] = clipInfos.intersectC[i];
					bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
					if (!bCulled)
                        break;
					if (outputInfos[iOutputCount].bClipsPlane[i])
						outputInfos[iOutputCount].bClipped = true;
				}
			}
			// output this triangle
			if (bCulled) {
                outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                _ClipCopy( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA );
                _ClipInterpolate( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexA, clipInfos.pVertexB, paramT1 );
                _ClipCopy( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexC );
                ++iOutputCount;
            }
		} else {
			// A back, B front, C planar, result triangle = DBC
			outputInfos[iOutputCount].vA = vD;
			outputInfos[iOutputCount].vB = clipInfos.vB;
			outputInfos[iOutputCount].vC = clipInfos.vC;
			// update result's clipping infos
			outputInfos[iOutputCount].bClipped = false;
			for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
				outputInfos[iOutputCount].bClipsPlane[i] = false;
				if ( clipInfos.bClipsPlane[i] ) {
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectA[i]) );
					outputInfos[iOutputCount].intersectB[i] = clipInfos.intersectB[i];
					outputInfos[iOutputCount].intersectC[i] = clipInfos.intersectC[i];
					bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
					if (!bCulled)
                        break;
					if (outputInfos[iOutputCount].bClipsPlane[i])
						outputInfos[iOutputCount].bClipped = true;
				}
			}
			// output this triangle
			if (bCulled) {
                outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                _ClipInterpolate( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA, clipInfos.pVertexB, paramT1 );
                _ClipCopy( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexB );
                _ClipCopy( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexC );
                ++iOutputCount;
            }
		}
        m_pCFL->Free(clipInfos.pVertexA);
        m_pCFL->Free(clipInfos.pVertexB);
        m_pCFL->Free(clipInfos.pVertexC);
		return iOutputCount;
	}

    // Ok only general cases remaining ...
    if ( clipInfos.intersectA[iPlane].iRelative == INTERSECT_RELATIVE_FRONT ) {
		if ( clipInfos.intersectB[iPlane].iRelative  == INTERSECT_RELATIVE_FRONT ) {
			// A front, B front, C back
            seg1 = Line3D( clipInfos.vA, clipInfos.vC );
            IntersectFn->Intersect3D( paramT1, m_arrClippingPlanes[iPlane], seg1, NULL );
            InterpolateFn->Lerp( vD, clipInfos.vA, clipInfos.vC, paramT1 );
            seg2 = Line3D( clipInfos.vB, clipInfos.vC );
            IntersectFn->Intersect3D( paramT2, m_arrClippingPlanes[iPlane], seg2, NULL );
            InterpolateFn->Lerp( vE, clipInfos.vB, clipInfos.vC, paramT2 );

			// result triangle = ABE
			outputInfos[iOutputCount].vA = clipInfos.vA;
			outputInfos[iOutputCount].vB = clipInfos.vB;
			outputInfos[iOutputCount].vC = vE;
			// update result's clipping infos
			outputInfos[iOutputCount].bClipped = false;
			for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
				outputInfos[iOutputCount].bClipsPlane[i] = false;
				if ( clipInfos.bClipsPlane[i] ) {
					outputInfos[iOutputCount].intersectA[i] = clipInfos.intersectA[i];
					outputInfos[iOutputCount].intersectB[i] = clipInfos.intersectB[i];
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vE, &(outputInfos[iOutputCount].intersectC[i]) );
                    bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
					if (!bCulled)
                        break;
					if (outputInfos[iOutputCount].bClipsPlane[i])
						outputInfos[iOutputCount].bClipped = true;
				}
			}
			// output this triangle
			if (bCulled) {
                outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                _ClipCopy( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA );
                _ClipCopy( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexB );
                _ClipInterpolate( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexB, clipInfos.pVertexC, paramT2 );
                ++iOutputCount;
            }

			bCulled = true;
			// result triangle = AED
			outputInfos[iOutputCount].vA = clipInfos.vA;
			outputInfos[iOutputCount].vB = vE;
			outputInfos[iOutputCount].vC = vD;
			// update result's clipping infos
			outputInfos[iOutputCount].bClipped = false;
			for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
				outputInfos[iOutputCount].bClipsPlane[i] = false;
				if ( clipInfos.bClipsPlane[i] ) {
					outputInfos[iOutputCount].intersectA[i] = clipInfos.intersectA[i];
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vE, &(outputInfos[iOutputCount].intersectB[i]) );
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectC[i]) );
					bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
					if (!bCulled)
                        break;
					if (outputInfos[iOutputCount].bClipsPlane[i])
						outputInfos[iOutputCount].bClipped = true;
				}
			}
			// output this triangle
			if (bCulled) {
                outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			    outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                _ClipCopy( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA );
                _ClipInterpolate( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexB, clipInfos.pVertexC, paramT2 );
                _ClipInterpolate( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexA, clipInfos.pVertexC, paramT1 );
                ++iOutputCount;
            }
        } else {
			if ( clipInfos.intersectC[iPlane].iRelative == INTERSECT_RELATIVE_FRONT ) {
				// A front, B back, C front
				seg1 = Line3D( clipInfos.vA, clipInfos.vB );
                IntersectFn->Intersect3D( paramT1, m_arrClippingPlanes[iPlane], seg1, NULL );
                InterpolateFn->Lerp( vD, clipInfos.vA, clipInfos.vB, paramT1 );
				seg2 = Line3D( clipInfos.vC, clipInfos.vB );
                IntersectFn->Intersect3D( paramT2, m_arrClippingPlanes[iPlane], seg2, NULL );
                InterpolateFn->Lerp( vE, clipInfos.vC, clipInfos.vB, paramT2 );

				// result triangle = ADC
				outputInfos[iOutputCount].vA = clipInfos.vA;
				outputInfos[iOutputCount].vB = vD;
				outputInfos[iOutputCount].vC = clipInfos.vC;
				// update result's clipping infos
				outputInfos[iOutputCount].bClipped = false;
				for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
					outputInfos[iOutputCount].bClipsPlane[i] = false;
					if ( clipInfos.bClipsPlane[i] ) {
						outputInfos[iOutputCount].intersectA[i] = clipInfos.intersectA[i];
                        IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectB[i]) );
						outputInfos[iOutputCount].intersectC[i] = clipInfos.intersectC[i];
						bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
						if (!bCulled)
                            break;
						if (outputInfos[iOutputCount].bClipsPlane[i])
							outputInfos[iOutputCount].bClipped = true;
					}
				}
				// output this triangle
				if (bCulled) {
                    outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                    _ClipCopy( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA );
                    _ClipInterpolate( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexA, clipInfos.pVertexB, paramT1 );
                    _ClipCopy( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexC );
                    ++iOutputCount;
                }

				bCulled = true;
				// result triangle = DEC
				outputInfos[iOutputCount].vA = vD;
				outputInfos[iOutputCount].vB = vE;
				outputInfos[iOutputCount].vC = clipInfos.vC;
				// update result's clipping infos
				outputInfos[iOutputCount].bClipped = false;
				for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
					outputInfos[iOutputCount].bClipsPlane[i] = false;
					if ( clipInfos.bClipsPlane[i] ) {
                        IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectA[i]) );
                        IntersectFn->Intersect3D( m_arrClippingPlanes[i], vE, &(outputInfos[iOutputCount].intersectB[i]) );
						outputInfos[iOutputCount].intersectC[i] = clipInfos.intersectC[i];
						bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
						if (!bCulled)
                            break;
						if (outputInfos[iOutputCount].bClipsPlane[i])
							outputInfos[iOutputCount].bClipped = true;
					}
				}
				// output this triangle
				if (bCulled) {
                    outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                    _ClipInterpolate( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA, clipInfos.pVertexB, paramT1 );
                    _ClipInterpolate( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexC, clipInfos.pVertexB, paramT2 );
                    _ClipCopy( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexC );
                    ++iOutputCount;
                }
			} else {
				// A front, B back, C back
                seg1 = Line3D( clipInfos.vA, clipInfos.vB );
                IntersectFn->Intersect3D( paramT1, m_arrClippingPlanes[iPlane], seg1, NULL );
                InterpolateFn->Lerp( vD, clipInfos.vA, clipInfos.vB, paramT1 );
				seg2 = Line3D( clipInfos.vA, clipInfos.vC );
                IntersectFn->Intersect3D( paramT2, m_arrClippingPlanes[iPlane], seg2, NULL );
                InterpolateFn->Lerp( vE, clipInfos.vA, clipInfos.vC, paramT2 );

				// result triangle = ADE
				outputInfos[iOutputCount].vA = clipInfos.vA;
				outputInfos[iOutputCount].vB = vD;
				outputInfos[iOutputCount].vC = vE;
				// update result's clipping infos
				outputInfos[iOutputCount].bClipped = false;
				for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
					outputInfos[iOutputCount].bClipsPlane[i] = false;
					if ( clipInfos.bClipsPlane[i] ) {
						outputInfos[iOutputCount].intersectA[i] = clipInfos.intersectA[i];
                        IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectB[i]) );
                        IntersectFn->Intersect3D( m_arrClippingPlanes[i], vE, &(outputInfos[iOutputCount].intersectC[i]) );
						bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
						if (!bCulled)
                            break;
						if (outputInfos[iOutputCount].bClipsPlane[i])
							outputInfos[iOutputCount].bClipped = true;
					}
				}
				// output this triangle
				if (bCulled) {
                    outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                    _ClipCopy( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexA );
                    _ClipInterpolate( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexA, clipInfos.pVertexB, paramT1 );
                    _ClipInterpolate( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexA, clipInfos.pVertexC, paramT2 );
                    ++iOutputCount;
                }
			}
		}
	} else {
		if ( clipInfos.intersectB[iPlane].iRelative == INTERSECT_RELATIVE_FRONT ) {
			if ( clipInfos.intersectC[iPlane].iRelative == INTERSECT_RELATIVE_FRONT ) {
				// A back, B front, C front
                seg1 = Line3D( clipInfos.vB, clipInfos.vA );
                IntersectFn->Intersect3D( paramT1, m_arrClippingPlanes[iPlane], seg1, NULL );
                InterpolateFn->Lerp( vD, clipInfos.vB, clipInfos.vA, paramT1 );
				seg2 = Line3D( clipInfos.vC, clipInfos.vA );
                IntersectFn->Intersect3D( paramT2, m_arrClippingPlanes[iPlane], seg2, NULL );
                InterpolateFn->Lerp( vE, clipInfos.vC, clipInfos.vA, paramT2 );

				// result triangle = DBC
				outputInfos[iOutputCount].vA = vD;
				outputInfos[iOutputCount].vB = clipInfos.vB;
				outputInfos[iOutputCount].vC = clipInfos.vC;
				// update result's clipping infos
				outputInfos[iOutputCount].bClipped = false;
				for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
					outputInfos[iOutputCount].bClipsPlane[i] = false;
					if ( clipInfos.bClipsPlane[i] ) {
                        IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectA[i]) );
						outputInfos[iOutputCount].intersectB[i] = clipInfos.intersectB[i];
						outputInfos[iOutputCount].intersectC[i] = clipInfos.intersectC[i];
						bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
						if (!bCulled)
                            break;
						if (outputInfos[iOutputCount].bClipsPlane[i])
							outputInfos[iOutputCount].bClipped = true;
					}
				}
				// output this triangle
				if (bCulled) {
                    outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                    _ClipInterpolate( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexB, clipInfos.pVertexA, paramT1 );
                    _ClipCopy( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexB );
                    _ClipCopy( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexC );
                    ++iOutputCount;
                }

				bCulled = true;
				// result triangle = EDC
				outputInfos[iOutputCount].vA = vE;
				outputInfos[iOutputCount].vB = vD;
				outputInfos[iOutputCount].vC = clipInfos.vC;
				// update result's clipping infos
				outputInfos[iOutputCount].bClipped = false;
				for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
					outputInfos[iOutputCount].bClipsPlane[i] = false;
					if ( clipInfos.bClipsPlane[i] ) {
                        IntersectFn->Intersect3D( m_arrClippingPlanes[i], vE, &(outputInfos[iOutputCount].intersectA[i]) );
                        IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectB[i]) );
						outputInfos[iOutputCount].intersectC[i] = clipInfos.intersectC[i];
						bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
						if (!bCulled)
                            break;
						if (outputInfos[iOutputCount].bClipsPlane[i])
							outputInfos[iOutputCount].bClipped = true;
					}
				}
				// output this triangle
				if (bCulled) {
                    outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                    _ClipInterpolate( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexC, clipInfos.pVertexA, paramT2 );
                    _ClipInterpolate( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexB, clipInfos.pVertexA, paramT1 );
                    _ClipCopy( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexC );
                    ++iOutputCount;
                }
			} else {
				// A back, B front, C back
                seg1 = Line3D( clipInfos.vB, clipInfos.vA );
                IntersectFn->Intersect3D( paramT1, m_arrClippingPlanes[iPlane], seg1, NULL );
                InterpolateFn->Lerp( vD, clipInfos.vB, clipInfos.vA, paramT1 );
				seg2 = Line3D( clipInfos.vB, clipInfos.vC );
                IntersectFn->Intersect3D( paramT2, m_arrClippingPlanes[iPlane], seg2, NULL );
                InterpolateFn->Lerp( vE, clipInfos.vB, clipInfos.vC, paramT2 );

				// result triangle = DBE
				outputInfos[iOutputCount].vA = vD;
				outputInfos[iOutputCount].vB = clipInfos.vB;
				outputInfos[iOutputCount].vC = vE;
				// update result's clipping infos
				outputInfos[iOutputCount].bClipped = false;
				for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
					outputInfos[iOutputCount].bClipsPlane[i] = false;
					if ( clipInfos.bClipsPlane[i] ) {
                        IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectA[i]) );
						outputInfos[iOutputCount].intersectB[i] = clipInfos.intersectB[i];
                        IntersectFn->Intersect3D( m_arrClippingPlanes[i], vE, &(outputInfos[iOutputCount].intersectC[i]) );
						bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
						if (!bCulled)
                            break;
						if (outputInfos[iOutputCount].bClipsPlane[i])
							outputInfos[iOutputCount].bClipped = true;
					}
				}
				// output this triangle
				if (bCulled) {
                    outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
			        outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                    _ClipInterpolate( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexB, clipInfos.pVertexA, paramT1 );
                    _ClipCopy( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexB );
                    _ClipInterpolate( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexB, clipInfos.pVertexC, paramT2 );
                    ++iOutputCount;
                }
			}
		} else {
			// A back, B back, C front
            seg1 = Line3D( clipInfos.vC, clipInfos.vA );
            IntersectFn->Intersect3D( paramT1, m_arrClippingPlanes[iPlane], seg1, NULL );
            InterpolateFn->Lerp( vD, clipInfos.vC, clipInfos.vA, paramT1 );
			seg2 = Line3D( clipInfos.vC, clipInfos.vB );
            IntersectFn->Intersect3D( paramT2, m_arrClippingPlanes[iPlane], seg2, NULL );
            InterpolateFn->Lerp( vE, clipInfos.vC, clipInfos.vB, paramT2 );

			// result triangle = DEC
			outputInfos[iOutputCount].vA = vD;
			outputInfos[iOutputCount].vB = vE;
			outputInfos[iOutputCount].vC = clipInfos.vC;
			// update result's clipping infos
			outputInfos[iOutputCount].bClipped = false;
			for(i = (iPlane + 1); i < m_iPlaneCount; ++i) {
				outputInfos[iOutputCount].bClipsPlane[i] = false;
				if ( clipInfos.bClipsPlane[i] ) {
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vD, &(outputInfos[iOutputCount].intersectA[i]) );
                    IntersectFn->Intersect3D( m_arrClippingPlanes[i], vE, &(outputInfos[iOutputCount].intersectB[i]) );
					outputInfos[iOutputCount].intersectC[i] = clipInfos.intersectC[i];
					bCulled = _CullCheck_Triangle( outputInfos[iOutputCount], i );
					if (!bCulled)
                        break;
					if (outputInfos[iOutputCount].bClipsPlane[i])
						outputInfos[iOutputCount].bClipped = true;
				}
			}
			// output this triangle
			if (bCulled) {
                outputInfos[iOutputCount].pVertexA = m_pCFL->Allocate();
		        outputInfos[iOutputCount].pVertexB = m_pCFL->Allocate();
		        outputInfos[iOutputCount].pVertexC = m_pCFL->Allocate();
                _ClipInterpolate( outputInfos[iOutputCount].pVertexA, clipInfos.pVertexC, clipInfos.pVertexA, paramT1 );
                _ClipInterpolate( outputInfos[iOutputCount].pVertexB, clipInfos.pVertexC, clipInfos.pVertexB, paramT2 );
                _ClipCopy( outputInfos[iOutputCount].pVertexC, clipInfos.pVertexC );
                ++iOutputCount;
            }
		}
	}
    m_pCFL->Free(clipInfos.pVertexA);
    m_pCFL->Free(clipInfos.pVertexB);
    m_pCFL->Free(clipInfos.pVertexC);
    return iOutputCount;
}

Void Clipper::_ClippingProcess_Segment( const ClippingSegment_Infos & clipInfos )
{
    UInt i;
    Bool bOutSegment = false;
    ClippingSegment_Infos clippedSegment, curClipSegment = clipInfos;

    for(i = 0; i < m_iPlaneCount; ++i) {
        if ( curClipSegment.bClipsPlane[i] ) {
            bOutSegment = _ClipToPlane_Segment( clippedSegment, curClipSegment, i );
            if (!bOutSegment)
                return;
            if ( clippedSegment.bClipped )
                curClipSegment = clippedSegment;
            else {
                m_pRenderer->_ClippingOutput_Segment( clippedSegment.pVertexA,
                                                      clippedSegment.pVertexB );
                return;
            }
        }
    }
    m_pRenderer->_ClippingOutput_Segment( curClipSegment.pVertexA,
                                          curClipSegment.pVertexB );
}
Void Clipper::_ClippingProcess_Triangle( const ClippingTriangle_Infos & clipInfos )
{
    UInt i, triCount;
    ClippingTriangle_Infos clippedTriangles[2], curClipTriangle;

    _DblQueue_Reset();
    _DblQueue_Push( clipInfos, m_iActiveQueue );

    for(i = 0; i < m_iPlaneCount; ++i) {
        while( !_DblQueue_IsEmpty(m_iActiveQueue) ) {
            _DblQueue_Pop( curClipTriangle, m_iActiveQueue );
            if ( curClipTriangle.bClipsPlane[i] ) {
                triCount = _ClipToPlane_Triangle( clippedTriangles, curClipTriangle, i );
                if (triCount > 0) {
                    if ( clippedTriangles[0].bClipped )
                        _DblQueue_Push( clippedTriangles[0], (1 - m_iActiveQueue) );
                    else
                        m_pRenderer->_ClippingOutput_Triangle( clippedTriangles[0].pVertexA,
                                                               clippedTriangles[0].pVertexB,
                                                               clippedTriangles[0].pVertexC );
                }
                if (triCount > 1) {
                    if ( clippedTriangles[1].bClipped )
                        _DblQueue_Push( clippedTriangles[1], (1 - m_iActiveQueue) );
                    else
                        m_pRenderer->_ClippingOutput_Triangle( clippedTriangles[1].pVertexA,
                                                               clippedTriangles[1].pVertexB,
                                                               clippedTriangles[1].pVertexC );
                }
            } else
                _DblQueue_Push( curClipTriangle, (1 - m_iActiveQueue) );
        }
        _DblQueue_Swap();
    }

	while( !_DblQueue_IsEmpty(m_iActiveQueue) ) {
		_DblQueue_Pop( curClipTriangle, m_iActiveQueue );
        m_pRenderer->_ClippingOutput_Triangle( curClipTriangle.pVertexA,
                                               curClipTriangle.pVertexB,
                                               curClipTriangle.pVertexC );
	}
}
