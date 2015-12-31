/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Clipper.inl
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
// Clipper implementation
inline ClippingSegment_Infos & ClippingSegment_Infos::operator=( const ClippingSegment_Infos & rhs )
{
    vA = rhs.vA;
    vB = rhs.vB;
    pVertexA = rhs.pVertexA;
    pVertexB = rhs.pVertexB;
    bClipped = rhs.bClipped;
    for(UInt i=0; i<CLIPPER_MAX_PLANES; ++i) {
        bClipsPlane[i] = rhs.bClipsPlane[i];
        intersectA[i] = rhs.intersectA[i];
        intersectB[i] = rhs.intersectB[i];
    }
    return (*this);
}
inline ClippingTriangle_Infos & ClippingTriangle_Infos::operator=( const ClippingTriangle_Infos & rhs )
{
    vA = rhs.vA;
    vB = rhs.vB;
    vC = rhs.vC;
    pVertexA = rhs.pVertexA;
    pVertexB = rhs.pVertexB;
    pVertexC = rhs.pVertexC;
    bClipped = rhs.bClipped;
    for(UInt i=0; i<CLIPPER_MAX_PLANES; ++i) {
        bClipsPlane[i] = rhs.bClipsPlane[i];
        intersectA[i] = rhs.intersectA[i];
        intersectB[i] = rhs.intersectB[i];
        intersectC[i] = rhs.intersectC[i];
    }
    return (*this);
}

inline CustomFragmentLayout * Clipper::GetCFL() const {
    return m_pCFL;
}
inline Void Clipper::SetCFL( CustomFragmentLayout * pCFL ) {
    m_pCFL = pCFL;
}

inline Void Clipper::AddUserPlane( const Plane & userPlane ) {
    Assert(m_iPlaneCount < CLIPPER_MAX_PLANES);
    m_arrClippingPlanes[m_iPlaneCount] = userPlane;
    ++m_iPlaneCount;
}
inline Void Clipper::RemoveUserPlane( UInt iIndex ) {
    Assert(iIndex < m_iPlaneCount);
    Assert(iIndex >= CLIPPER_USER_PLANES);
    for( UInt i = iIndex; i < (m_iPlaneCount-1); ++i )
        m_arrClippingPlanes[i] = m_arrClippingPlanes[i+1];
    --m_iPlaneCount;
}
inline Void Clipper::SetUserPlane( UInt iIndex, const Plane & userPlane ) {
    Assert(iIndex < m_iPlaneCount);
    Assert(iIndex >= CLIPPER_USER_PLANES);
    m_arrClippingPlanes[iIndex] = userPlane;
}

/////////////////////////////////////////////////////////////////////////////////

inline Bool Clipper::_DblQueue_IsEmpty( UInt iQueue ) const {
    return ( m_iQueueRead[iQueue] == m_iQueueWrite[iQueue] );
}
inline Void Clipper::_DblQueue_Reset() {
    m_iQueueRead[0] = 0;
    m_iQueueRead[1] = 0;
    m_iQueueWrite[0] = 0;
    m_iQueueWrite[1] = 0;
    m_iActiveQueue = 0;
}
inline Void Clipper::_DblQueue_Swap() {
    m_iActiveQueue = (1 - m_iActiveQueue);
}
inline Void Clipper::_DblQueue_Push( const ClippingTriangle_Infos & clipInfos, UInt iQueue ) {
    m_arrClippingQueue[iQueue][m_iQueueWrite[iQueue]] = clipInfos;
    ++m_iQueueWrite[iQueue];
    if ( m_iQueueWrite[iQueue] >= CLIPPER_MAX_TRIANGLES )
        m_iQueueWrite[iQueue] = 0;
}
inline Void Clipper::_DblQueue_Pop( ClippingTriangle_Infos & outClipInfos, UInt iQueue ) {
    outClipInfos = m_arrClippingQueue[iQueue][m_iQueueRead[iQueue]];
    ++m_iQueueRead[iQueue];
    if ( m_iQueueRead[iQueue] >= CLIPPER_MAX_TRIANGLES )
        m_iQueueRead[iQueue] = 0;
}

inline Void Clipper::_ClipCopy( CFLPtr pOut, CCFLPtr pIn ) const {
    for(UInt i=0; i < m_pCFL->GetTotalSize(); ++i)
        pOut[i] = pIn[i];
}
inline Void Clipper::_ClipInterpolate( CFLPtr pOut, CCFLPtr pStart, CCFLPtr pEnd, Scalar fT ) const {
    for(UInt i=0; i < m_pCFL->GetTotalSize(); ++i)
        InterpolateFn->Lerp( pOut[i], pStart[i],  pEnd[i], fT );
}

