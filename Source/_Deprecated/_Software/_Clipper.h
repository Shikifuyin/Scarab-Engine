/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Clipper.h
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
// Header prelude
#ifndef KOALA_ENGINE_GRAPHICS_3D_CLIPPER_H
#define KOALA_ENGINE_GRAPHICS_3D_CLIPPER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Core/Math/Algebra/Interpolator.h"
#include "../../../Lib/Core/Math/Geometry/Intersector.h"

#include "Buffering.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
class Renderer;

#define CLIPPER_USER_PLANES   6
#define CLIPPER_MAX_PLANES    12 // maybe more in the future ...
#define CLIPPER_MAX_TRIANGLES (0x00000001u << CLIPPER_MAX_PLANES)
#define CLIPPER_MAX_VERTICES  (CLIPPER_MAX_TRIANGLES * 3)

enum FrustrumPlane {
    FRUSTRUM_PLANE_NEAR = 0,
    FRUSTRUM_PLANE_FAR,
    FRUSTRUM_PLANE_BOTTOM,
    FRUSTRUM_PLANE_TOP,
    FRUSTRUM_PLANE_LEFT,
    FRUSTRUM_PLANE_RIGHT,
    FRUSTRUM_PLANE_COUNT
};

typedef struct _clippingsegment_infos
{
    Vertex3 vA, vB;
    CFLPtr pVertexA;
    CFLPtr pVertexB;
    Bool bClipped;
    Bool bClipsPlane[CLIPPER_MAX_PLANES];
	IntersectorResult intersectA[CLIPPER_MAX_PLANES];
	IntersectorResult intersectB[CLIPPER_MAX_PLANES];

    inline struct _clippingsegment_infos & operator=( const struct _clippingsegment_infos & rhs );
} ClippingSegment_Infos;

typedef struct _clippingtriangle_infos
{
    Vertex3 vA, vB, vC;
    CFLPtr pVertexA;
    CFLPtr pVertexB;
    CFLPtr pVertexC;
    Bool bClipped;
    Bool bClipsPlane[CLIPPER_MAX_PLANES];
	IntersectorResult intersectA[CLIPPER_MAX_PLANES];
	IntersectorResult intersectB[CLIPPER_MAX_PLANES];
	IntersectorResult intersectC[CLIPPER_MAX_PLANES];

    inline struct _clippingtriangle_infos & operator=( const struct _clippingtriangle_infos & rhs );
} ClippingTriangle_Infos;

/////////////////////////////////////////////////////////////////////////////////
// The Clipper class
class Clipper
{
public:
    Clipper( Renderer * pRenderer );
    ~Clipper();

    inline CustomFragmentLayout * GetCFL() const;
    inline Void SetCFL( CustomFragmentLayout * pCFL );

    // Frustrum planes
    Void ExtractFrustrumPlanes();

    // User planes
    inline Void AddUserPlane( const Plane & userPlane );
    inline Void RemoveUserPlane( UInt iIndex );
    inline Void SetUserPlane( UInt iIndex, const Plane & userPlane );

    // Vertex culling
    Bool CullVertex( CFLPtr pVertexA );

    // Segment culling & clipping
    Bool CullSegment( CFLPtr pVertexA, CFLPtr pVertexB, Bool bRejectClipped = false );
    Bool ClipSegment( CFLPtr pVertexA, CFLPtr pVertexB );

    // Triangle culling & clipping
    Bool CullTriangle( CFLPtr pVertexA, CFLPtr pVertexB, CFLPtr pVertexC, Bool bRejectClipped = false );
    Bool ClipTriangle( CFLPtr pVertexA, CFLPtr pVertexB, CFLPtr pVertexC );

private:
    // double queues for fast double buffering (triangles only)
    inline Bool _DblQueue_IsEmpty( UInt iQueue ) const;
    inline Void _DblQueue_Reset();
    inline Void _DblQueue_Swap();
    inline Void _DblQueue_Push( const ClippingTriangle_Infos & clipInfos, UInt iQueue );
    inline Void _DblQueue_Pop( ClippingTriangle_Infos & outClipInfos, UInt iQueue );
    ClippingTriangle_Infos m_arrClippingQueue[2][CLIPPER_MAX_TRIANGLES];
    UInt m_iActiveQueue;
    UInt m_iQueueRead[2], m_iQueueWrite[2];

    // helpers
    inline Void _ClipCopy( CFLPtr pOut, CCFLPtr pIn ) const;
    inline Void _ClipInterpolate( CFLPtr pOut, CCFLPtr pStart, CCFLPtr pEnd, Scalar fT ) const;

    // culling checks
    Bool _CullCheck_Vertex( const Vertex3 & vPos, UInt iPlane ) const;
    Bool _CullCheck_Segment( ClippingSegment_Infos & clippingInfos, UInt iPlane ) const;
    Bool _CullCheck_Triangle( ClippingTriangle_Infos & clippingInfos, UInt iPlane ) const;

    // 1-clipping
    Bool _ClipToPlane_Segment( ClippingSegment_Infos & outputInfos,
                               const ClippingSegment_Infos & clipInfos, UInt iPlane ) const;
    UInt _ClipToPlane_Triangle( ClippingTriangle_Infos outputInfos[2],
                                const ClippingTriangle_Infos & clipInfos, UInt iPlane ) const;

    // clipping processes
    Void _ClippingProcess_Segment( const ClippingSegment_Infos & clipInfos );
    Void _ClippingProcess_Triangle( const ClippingTriangle_Infos & clipInfos );

    // Data
    Renderer * m_pRenderer;

    CustomFragmentLayout * m_pCFL;

    Plane m_arrClippingPlanes[CLIPPER_MAX_PLANES];
    UInt m_iPlaneCount;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Clipper.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_CLIPPER_H

