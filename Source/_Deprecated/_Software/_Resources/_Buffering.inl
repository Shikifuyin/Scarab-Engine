/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Buffering.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : All stuff needed for buffering tasks in the engine ...
//               Defines the Custom Fragment Layout data system for the renderer.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// CustomFragmentLayout implementation
inline CFLPtr CustomFragmentLayout::Allocate() const {
    Assert(m_bFinalized);
    return KNewExA(m_idFactory) Scalar[m_iSize];
}
inline Void CustomFragmentLayout::Free( CFLPtr pCFLData ) const {
    Assert(m_bFinalized);
    Assert(pCFLData != NULL);
    KDeleteAExA(pCFLData, m_idFactory);
}

inline UInt CustomFragmentLayout::GetTotalSize() const {
    Assert(m_bFinalized);
    return m_iSize;
}
inline UInt CustomFragmentLayout::GetFieldsCount() const {
    Assert(m_bFinalized);
    return m_iFieldsCount;
}

inline Bool CustomFragmentLayout::HasField( CFLSemantic iQuerySemantic, UInt iQueryIndex ) const {
    Assert(m_bFinalized);
    Assert(iQuerySemantic < CFLSEMANTIC_COUNT);
    Assert(iQueryIndex < CFL_MAX_INDEX);
    return ( m_arrFields[(((UInt)iQuerySemantic) << CFL_MAX_INDEX_SHIFT) + iQueryIndex].iSize != 0 );
}
inline UInt CustomFragmentLayout::GetFieldOffset( CFLSemantic iQuerySemantic, UInt iQueryIndex ) const {
    Assert(m_bFinalized);
    Assert(iQuerySemantic < CFLSEMANTIC_COUNT);
    Assert(iQueryIndex < CFL_MAX_INDEX);
    return m_arrFields[(((UInt)iQuerySemantic) << CFL_MAX_INDEX_SHIFT) + iQueryIndex].iOffset;
}
inline UInt CustomFragmentLayout::GetFieldSize( CFLSemantic iQuerySemantic, UInt iQueryIndex ) const {
    Assert(m_bFinalized);
    Assert(iQuerySemantic < CFLSEMANTIC_COUNT);
    Assert(iQueryIndex < CFL_MAX_INDEX);
    return m_arrFields[(((UInt)iQuerySemantic) << CFL_MAX_INDEX_SHIFT) + iQueryIndex].iSize;
}
inline CFLType CustomFragmentLayout::GetFieldType( CFLSemantic iQuerySemantic, UInt iQueryIndex ) const {
    Assert(m_bFinalized);
    Assert(iQuerySemantic < CFLSEMANTIC_COUNT);
    Assert(iQueryIndex < CFL_MAX_INDEX);
    return m_arrFields[(((UInt)iQuerySemantic) << CFL_MAX_INDEX_SHIFT) + iQueryIndex].iType;
}

inline CFLPtr CustomFragmentLayout::GetField( CFLPtr pCFLData, CFLSemantic iQuerySemantic, UInt iQueryIndex ) const {
    Assert(m_bFinalized);
    Assert(pCFLData != NULL);
    Assert(iQuerySemantic < CFLSEMANTIC_COUNT);
    Assert(iQueryIndex < CFL_MAX_INDEX);
    return ( pCFLData + m_arrFields[(((UInt)iQuerySemantic) << CFL_MAX_INDEX_SHIFT) + iQueryIndex].iOffset );
}
inline CCFLPtr CustomFragmentLayout::GetField( CCFLPtr pCFLData, CFLSemantic iQuerySemantic, UInt iQueryIndex ) const {
    Assert(m_bFinalized);
    Assert(pCFLData != NULL);
    Assert(iQuerySemantic < CFLSEMANTIC_COUNT);
    Assert(iQueryIndex < CFL_MAX_INDEX);
    return ( pCFLData + m_arrFields[(((UInt)iQuerySemantic) << CFL_MAX_INDEX_SHIFT) + iQueryIndex].iOffset );
}

/////////////////////////////////////////////////////////////////////////////////
// VertexArray implementation
inline UInt VertexArray::Length() const {
    return m_iLength;
}
inline CFLPtr VertexArray::GetVertex( UInt iVertex ) const {
    Assert(iVertex < m_iLength);
    return m_arrVertices[iVertex];
}

inline Void VertexArray::Push( CFLPtr pVertex ) {
    Assert(m_iLength < m_iCapacity);
    m_arrVertices[m_iLength] = pVertex;
    ++m_iLength;
}
inline Void VertexArray::Clear() {
    m_iLength = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// VertexBuffer implementation
inline const CustomFragmentLayout * VertexBuffer::GetCFL() const {
    return m_pCFL;
}
inline UInt VertexBuffer::GetVertexSize() const {
    return m_iVertexSize;
}
inline UInt VertexBuffer::GetVertexCount() const {
    return m_iVertexCount;
}
inline UInt VertexBuffer::GetSize() const {
    return m_iTotalSize;
}

inline CFLPtr * VertexBuffer::GetData() {
    return m_arrVertices;
}
inline const CFLPtr * VertexBuffer::GetData() const {
    return m_arrVertices;
}

inline CFLPtr VertexBuffer::GetVertex( UInt iVertex ) {
    Assert(iVertex < m_iVertexCount);
    return m_arrVertices[iVertex];
}
inline CCFLPtr VertexBuffer::GetVertex( UInt iVertex ) const {
    Assert(iVertex < m_iVertexCount);
    return m_arrVertices[iVertex];
}

inline CFLPtr VertexBuffer::GetVertexField( UInt iVertex, CFLSemantic iQuerySemantic, UInt iQueryIndex ) {
    Assert( iVertex < m_iVertexCount );
    Assert( m_pCFL->HasField(iQuerySemantic, iQueryIndex) );
    return m_pCFL->GetField( m_arrVertices[iVertex], iQuerySemantic, iQueryIndex );
}
inline CCFLPtr VertexBuffer::GetVertexField( UInt iVertex, CFLSemantic iQuerySemantic, UInt iQueryIndex ) const {
    Assert( iVertex < m_iVertexCount );
    Assert( m_pCFL->HasField(iQuerySemantic, iQueryIndex) );
    return m_pCFL->GetField( m_arrVertices[iVertex], iQuerySemantic, iQueryIndex );
}

/////////////////////////////////////////////////////////////////////////////////
// IndexBuffer implementation
inline IndexBufferMode IndexBuffer::GetMode() const {
    return m_iMode;
}
inline UInt IndexBuffer::GetFacetCount() const {
    return m_iFacetCount;
}
inline UInt IndexBuffer::GetSize() const {
    return m_iIndexCount;
}

inline UInt * IndexBuffer::GetData() {
    return m_arrIndices;
}
inline const UInt * IndexBuffer::GetData() const {
    return m_arrIndices;
}

inline UInt & IndexBuffer::operator[](UInt iIndex) {
    Assert(iIndex < m_iIndexCount);
    return m_arrIndices[iIndex];
}
inline UInt IndexBuffer::operator[](UInt iIndex) const {
    Assert(iIndex < m_iIndexCount);
    return m_arrIndices[iIndex];
}
inline UInt & IndexBuffer::operator[](Int iIndex) {
    Assert(iIndex < (Int)m_iIndexCount);
    return m_arrIndices[iIndex];
}
inline UInt IndexBuffer::operator[](Int iIndex) const {
    Assert(iIndex < (Int)m_iIndexCount);
    return m_arrIndices[iIndex];
}

/////////////////////////////////////////////////////////////////////////////////
// FrameBuffer implementation
inline UInt FrameBuffer::Width() const {
    return m_iWidth;
}
inline UInt FrameBuffer::Height() const {
    return m_iHeight;
}
inline UInt FrameBuffer::Size() const {
    return m_iSize;
}

inline Void FrameBuffer::SetClearColor( const Color4 & colClear ) {
    m_colClearColor = colClear;
}
inline Void FrameBuffer::SetClearDepth( Scalar fClear ) {
    m_fClearDepth = Clamp<Scalar>( fClear, 0.0f, 1.0f );
}
inline Void FrameBuffer::SetClearStencil( UInt iClear ) {
    m_iClearStencil = Clamp<UInt>( iClear, 0u, m_iMaxStencilValue );
}

inline Bool FrameBuffer::HasColorBuffer() const {
    return (m_pColorBufferI != NULL);
}
inline ColorFormat FrameBuffer::GetColorFormat() const {
    return m_iColorFormat;
}
inline Color4 * FrameBuffer::GetColorBufferF() const {
    return m_pColorBufferF;
}
inline Color4 & FrameBuffer::GetColorF( UInt iPixel ) {
    Assert(iPixel < m_iSize);
    return m_pColorBufferF[iPixel];
}
inline const Color4 & FrameBuffer::GetColorF( UInt iPixel ) const {
    Assert(iPixel < m_iSize);
    return m_pColorBufferF[iPixel];
}
inline PixelMap * FrameBuffer::GetColorBufferI() {
    return m_pColorBufferI;
}
inline const PixelMap * FrameBuffer::GetColorBufferI() const {
    return m_pColorBufferI;
}

inline Bool FrameBuffer::HasDepthBuffer() const {
    return (m_iDepthType != FRAMEBUFFER_DEPTH_NONE);
}
inline FrameBufferDepthType FrameBuffer::GetDepthType() const {
    return m_iDepthType;
}
inline ULong FrameBuffer::MaxDepthValue() const {
    return m_iMaxDepthValue;
}
inline Scalar * FrameBuffer::GetDepthBufferF() const {
    return m_pDepthBufferF;
}
inline Scalar & FrameBuffer::GetDepthF( UInt iPixel ) {
    Assert(iPixel < m_iSize);
    return m_pDepthBufferF[iPixel];
}
inline const Scalar & FrameBuffer::GetDepthF( UInt iPixel ) const {
    Assert(iPixel < m_iSize);
    return m_pDepthBufferF[iPixel];
}
inline ULong * FrameBuffer::GetDepthBufferI() const {
    return m_pDepthBufferI;
}
inline ULong & FrameBuffer::GetDepthI( UInt iPixel ) {
    Assert(iPixel < m_iSize);
    return m_pDepthBufferI[iPixel];
}
inline const ULong & FrameBuffer::GetDepthI( UInt iPixel ) const {
    Assert(iPixel < m_iSize);
    return m_pDepthBufferI[iPixel];
}

inline Bool FrameBuffer::HasStencilBuffer() const {
    return (m_iStencilType != FRAMEBUFFER_STENCIL_NONE);
}
inline FrameBufferStencilType FrameBuffer::GetStencilType() const {
    return m_iStencilType;
}
inline ULong FrameBuffer::MaxStencilValue() const {
    return m_iMaxStencilValue;
}
inline ULong * FrameBuffer::GetStencilBuffer() const {
    return m_pStencilBuffer;
}
inline ULong & FrameBuffer::GetStencil( UInt iPixel ) {
    Assert(iPixel < m_iSize);
    return m_pStencilBuffer[iPixel];
}
inline const ULong & FrameBuffer::GetStencil( UInt iPixel ) const {
    Assert(iPixel < m_iSize);
    return m_pStencilBuffer[iPixel];
}

