/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Buffering.cpp
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
// Includes
#include "Buffering.h"

/////////////////////////////////////////////////////////////////////////////////
// CustomFragmentLayout implementation
CustomFragmentLayout::CustomFragmentLayout()
{
    for( UInt i = 0; i < CFLSEMANTIC_COUNT; ++i ) {
        for( UInt j = 0; j < CFL_MAX_INDEX; ++j ) {
            UInt iIndex = (i << CFL_MAX_INDEX_SHIFT) + j;
            m_arrFields[iIndex].iOffset = INVALID_OFFSET;
            m_arrFields[iIndex].iSize = 0;
            m_arrFields[iIndex].iType = CFLTYPE_UNDEFINED;
            m_arrFields[iIndex].iSemantic = (CFLSemantic)i;
            m_arrFields[iIndex].iSemanticIndex = j;
        }
    }
    m_iSize = 0;
    m_iFieldsCount = 0;
    m_bFinalized = false;

    m_idFactory = INVALID_OFFSET;
}
CustomFragmentLayout::~CustomFragmentLayout()
{
    // Structures which are still allocated and their fields
    // can be referenced no more and data is lost, so the
    // factory is legitimately destroyed ...
    if ( m_idFactory != INVALID_OFFSET )
        MemoryFn->DestroyAllocator( m_idFactory );
}

Void CustomFragmentLayout::DeclareField( CFLType fieldType, CFLSemantic fieldSemantic )
{
    Assert(!m_bFinalized);

    UInt iIndex = ( ((UInt)fieldSemantic) << CFL_MAX_INDEX_SHIFT );
    for( UInt i = 0; i < CFL_MAX_INDEX; ++i ) {
        if ( m_arrFields[iIndex + i].iSize == 0 ) {
            m_arrFields[iIndex + i].iSize = _CFL_SizeOf(fieldType);
            m_arrFields[iIndex + i].iType = fieldType;
            return;
        }
    }

    Assert(false); // Too much fields for this semantic
}
Void CustomFragmentLayout::DeclareVertexFormat()
{
    Assert(!m_bFinalized);
    DeclareField( CFLTYPE_VERTEX4, CFLSEMANTIC_POSITION );
}
Void CustomFragmentLayout::DeclarePixelFormat()
{
    Assert(!m_bFinalized);
    DeclareField( CFLTYPE_VERTEX2, CFLSEMANTIC_RASTERPOS );
    DeclareField( CFLTYPE_FLOAT, CFLSEMANTIC_DEPTH );
    DeclareField( CFLTYPE_FLOAT, CFLSEMANTIC_INVERTW );
    DeclareField( CFLTYPE_FLOAT, CFLSEMANTIC_REGULARW );
}
Void CustomFragmentLayout::Finalize()
{
    Assert(!m_bFinalized);

    // Compute offsets
    m_iSize = 0;
    m_iFieldsCount = 0;
    for( UInt i = 0; i < CFL_MAX_FIELDS; ++i ) {
        if ( m_arrFields[i].iSize != 0 ) {
            m_arrFields[i].iOffset = m_iSize;
            m_iSize += m_arrFields[i].iSize;
            ++m_iFieldsCount;
        } else
            m_arrFields[i].iOffset = INVALID_OFFSET;
    }

    // Create factory
    if (m_iSize > 0)
        m_idFactory = MemoryFn->CreatePool( m_iSize * sizeof(Scalar), CFL_FACTORY_SIZE );

    // Finished
    m_bFinalized = true;
}

Bool CustomFragmentLayout::IsValidVertexFormat() const
{
    Assert(m_bFinalized);

    UInt iIndex;
    Bool bOk;

    // Check Position values
    iIndex = ( ((UInt)CFLSEMANTIC_POSITION) << CFL_MAX_INDEX_SHIFT ) + 0;
    bOk = ( m_arrFields[iIndex].iSize != 0 &&
            m_arrFields[iIndex].iOffset == 0 &&
            m_arrFields[iIndex].iType == CFLTYPE_VERTEX4 );
    if (!bOk)
        return false;

    return true;
}
Bool CustomFragmentLayout::IsValidPixelFormat() const
{
    Assert(m_bFinalized);

    UInt iIndex;
    Bool bOk;

    // Check Raster Position values
    iIndex = ( ((UInt)CFLSEMANTIC_RASTERPOS) << CFL_MAX_INDEX_SHIFT ) + 0;
    bOk = ( m_arrFields[iIndex].iSize != 0 &&
            m_arrFields[iIndex].iOffset == 0 &&
            m_arrFields[iIndex].iType == CFLTYPE_VERTEX2 );
    if (!bOk)
        return false;

    // Check Depth values
    iIndex = ( ((UInt)CFLSEMANTIC_DEPTH) << CFL_MAX_INDEX_SHIFT ) + 0;
    bOk = ( m_arrFields[iIndex].iSize != 0 &&
            m_arrFields[iIndex].iOffset == 2 &&
            m_arrFields[iIndex].iType == CFLTYPE_FLOAT );
    if (!bOk)
        return false;

    // Check Invert-W values
    iIndex = ( ((UInt)CFLSEMANTIC_INVERTW) << CFL_MAX_INDEX_SHIFT ) + 0;
    bOk = ( m_arrFields[iIndex].iSize != 0 &&
            m_arrFields[iIndex].iOffset == 3 &&
            m_arrFields[iIndex].iType == CFLTYPE_FLOAT );
    if (!bOk)
        return false;

    // Check Regular-W values
    iIndex = ( ((UInt)CFLSEMANTIC_REGULARW) << CFL_MAX_INDEX_SHIFT ) + 0;
    bOk = ( m_arrFields[iIndex].iSize != 0 &&
            m_arrFields[iIndex].iOffset == 4 &&
            m_arrFields[iIndex].iType == CFLTYPE_FLOAT );
    if (!bOk)
        return false;

    return true;
}

Bool CustomFragmentLayout::IsEqual( const CustomFragmentLayout * pCFL ) const
{
    Assert(m_bFinalized);
    Assert(pCFL->m_bFinalized);

    if (m_iSize != pCFL->m_iSize)
        return false;
    if (m_iFieldsCount != pCFL->m_iFieldsCount)
        return false;

    for( UInt i = 0; i < CFLSEMANTIC_COUNT; ++i ) {
        for( UInt j = 0; j < CFL_MAX_INDEX; ++j ) {
            UInt iIndex = (i << CFL_MAX_INDEX_SHIFT) + j;
            if ( m_arrFields[iIndex].iSize != pCFL->m_arrFields[iIndex].iSize )
                return false;
            if ( m_arrFields[iIndex].iOffset != pCFL->m_arrFields[iIndex].iOffset )
                return false;
            if ( m_arrFields[iIndex].iType != pCFL->m_arrFields[iIndex].iType )
                return false;
        }
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

UInt CustomFragmentLayout::_CFL_SizeOf( CFLType fieldType ) const
{
    switch(fieldType) {
        case CFLTYPE_LONG:
        case CFLTYPE_ULONG:
        case CFLTYPE_FLOAT:
        case CFLTYPE_TEXCOORD1: return 1;
        case CFLTYPE_LONG2:
        case CFLTYPE_ULONG2:
        case CFLTYPE_FLOAT2:
        case CFLTYPE_POINT2:
        case CFLTYPE_VERTEX2:
        case CFLTYPE_VECTOR2:
        case CFLTYPE_TEXCOORD2: return 2;
        case CFLTYPE_LONG3:
        case CFLTYPE_ULONG3:
        case CFLTYPE_FLOAT3:
        case CFLTYPE_VERTEX3:
        case CFLTYPE_VECTOR3:
        case CFLTYPE_TEXCOORD3: return 3;
        case CFLTYPE_LONG4:
        case CFLTYPE_ULONG4:
        case CFLTYPE_FLOAT4:
        case CFLTYPE_VERTEX4:
        case CFLTYPE_VECTOR4:
        case CFLTYPE_TEXCOORD4:
        case CFLTYPE_COLOR:
        case CFLTYPE_QUATERNION:
        case CFLTYPE_MATRIX2: return 4;
        case CFLTYPE_MATRIX3: return 9;
        case CFLTYPE_MATRIX4: return 16;
        default: Assert(false); return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// VertexArray implementation
VertexArray::VertexArray( UInt iCapacity, MemoryAllocatorID idAllocator ):
    m_idAllocator(idAllocator)
{
    m_iCapacity = iCapacity;
    m_iLength = 0;
    m_arrVertices = KNewExA(m_idAllocator) CFLPtr[m_iCapacity];
}
VertexArray::~VertexArray()
{
    KDeleteAExA(m_arrVertices, m_idAllocator);
}

/////////////////////////////////////////////////////////////////////////////////
// VertexBuffer implementation
VertexBuffer::VertexBuffer( const CustomFragmentLayout * pCFL, UInt iVertexCount, MemoryAllocatorID idAllocator ):
    m_idAllocator(idAllocator), m_pCFL(pCFL)
{
    m_iVertexSize = m_pCFL->GetTotalSize();
    m_iVertexCount = iVertexCount;
    m_iTotalSize = m_iVertexSize * m_iVertexCount;
    m_arrVertices = KNewExA(m_idAllocator) CFLPtr[m_iVertexCount];
    for( UInt i = 0; i < m_iVertexCount; ++i ) {
        m_arrVertices[i] = m_pCFL->Allocate();
        MemZero( m_arrVertices[i], m_iVertexSize * sizeof(Scalar) );
    }
}
VertexBuffer::~VertexBuffer()
{
    for( UInt i = 0; i < m_iVertexCount; ++i )
        m_pCFL->Free( m_arrVertices[i] );
    KDeleteAExA(m_arrVertices, m_idAllocator);
}

/////////////////////////////////////////////////////////////////////////////////
// IndexBuffer implementation
IndexBuffer::IndexBuffer( IndexBufferMode iMode, UInt iFacetCount, MemoryAllocatorID idAllocator ):
    m_idAllocator(idAllocator), m_iMode(iMode)
{
	m_iFacetCount = iFacetCount;
	switch(iMode) {
		case INDEXBUFFER_VERTEX_LIST:
			m_iIndexCount = m_iFacetCount;
			break;
		case INDEXBUFFER_LINE_LIST:
			m_iIndexCount = (m_iFacetCount << 1);
			break;
		case INDEXBUFFER_LINE_STRIPS:
		case INDEXBUFFER_LINE_FANS:
			m_iIndexCount = (m_iFacetCount + 1);
			break;
		case INDEXBUFFER_TRIANGLE_LIST:
			m_iIndexCount = (m_iFacetCount << 1) + m_iFacetCount;
			break;
		case INDEXBUFFER_TRIANGLE_STRIPS:
		case INDEXBUFFER_TRIANGLE_FANS:
			m_iIndexCount = (m_iFacetCount + 2);
			break;
		case INDEXBUFFER_QUAD_LIST:
			m_iIndexCount = (m_iFacetCount << 2);
			break;
		case INDEXBUFFER_QUAD_STRIPS:
			m_iIndexCount = (m_iFacetCount << 1) + 2;
			break;
		default: // never happen
			break;
	}
	m_arrIndices = KNewExA(m_idAllocator) UInt[m_iIndexCount];
}
IndexBuffer::~IndexBuffer()
{
	KDeleteAExA(m_arrIndices, m_idAllocator);
}

Void IndexBuffer::GetFacet( UInt iFacet, UInt & outVertexA ) const
{
    Assert(iFacet < m_iFacetCount);

    switch(m_iMode) {
        case INDEXBUFFER_VERTEX_LIST:
            outVertexA = m_arrIndices[iFacet];
	        break;
        default: Assert(false); return;
    }
}
Void IndexBuffer::GetFacet( UInt iFacet, UInt & outVertexA, UInt & outVertexB ) const
{
    Assert(iFacet < m_iFacetCount);

    switch(m_iMode) {
        case INDEXBUFFER_LINE_LIST:
            iFacet = (iFacet << 1);
            outVertexA = m_arrIndices[iFacet];
            outVertexB = m_arrIndices[iFacet + 1];
            break;
        case INDEXBUFFER_LINE_STRIPS:
            outVertexA = m_arrIndices[iFacet];
            outVertexB = m_arrIndices[iFacet + 1];
            break;
        case INDEXBUFFER_LINE_FANS:
            outVertexA = m_arrIndices[0];
            outVertexB = m_arrIndices[iFacet + 1];
            break;
        default: Assert(false); return;
    }
}
Void IndexBuffer::GetFacet( UInt iFacet, UInt & outVertexA, UInt & outVertexB, UInt & outVertexC ) const
{
    Assert(iFacet < m_iFacetCount);

    switch(m_iMode) {
        case INDEXBUFFER_TRIANGLE_LIST:
            iFacet = (iFacet << 1) + iFacet;
            outVertexA = m_arrIndices[iFacet];
            outVertexB = m_arrIndices[iFacet + 1];
            outVertexC = m_arrIndices[iFacet + 2];
            break;
        case INDEXBUFFER_TRIANGLE_STRIPS:
            // use a bias here to re-orient triangles
            if (iFacet & 0x01) {
                outVertexA = m_arrIndices[iFacet + 1];
                outVertexB = m_arrIndices[iFacet];
            } else {
                outVertexA = m_arrIndices[iFacet];
                outVertexB = m_arrIndices[iFacet + 1];
            }
            outVertexC = m_arrIndices[iFacet + 2];
            break;
        case INDEXBUFFER_TRIANGLE_FANS:
            outVertexA = m_arrIndices[0];
            outVertexB = m_arrIndices[iFacet + 1];
            outVertexC = m_arrIndices[iFacet + 2];
            break;
        default: Assert(false); return;
    }
}
Void IndexBuffer::GetFacet( UInt iFacet, UInt & outVertexA, UInt & outVertexB, UInt & outVertexC, UInt & outVertexD ) const
{
    Assert(iFacet < m_iFacetCount);

    switch(m_iMode) {
        case INDEXBUFFER_QUAD_LIST:
            iFacet = (iFacet << 2);
            outVertexA = m_arrIndices[iFacet];
            outVertexB = m_arrIndices[iFacet + 1];
            outVertexC = m_arrIndices[iFacet + 2];
            outVertexD = m_arrIndices[iFacet + 3];
            break;
        case INDEXBUFFER_QUAD_STRIPS:
            // use a bias here to re-orient quads
            iFacet = (iFacet << 1);
            if (iFacet > 0) {
                outVertexA = m_arrIndices[iFacet + 1];
                outVertexB = m_arrIndices[iFacet];
            } else {
                outVertexA = m_arrIndices[iFacet];
                outVertexB = m_arrIndices[iFacet + 1];
            }
            outVertexC = m_arrIndices[iFacet + 2];
            outVertexD = m_arrIndices[iFacet + 3];
            break;
        default: Assert(false); return;
    }
}

Void IndexBuffer::SetFacet( UInt iFacet, UInt iVertexA )
{
    Assert(iFacet < m_iFacetCount);

    UInt iIndex;
    switch(m_iMode) {
        case INDEXBUFFER_VERTEX_LIST:
            iIndex = iFacet;
            m_arrIndices[iIndex] = iVertexA;
            break;
        case INDEXBUFFER_LINE_STRIPS:
        case INDEXBUFFER_LINE_FANS:
            Assert(iFacet > 0);
            iIndex = iFacet + 1;
            m_arrIndices[iIndex] = iVertexA;
            break;
        case INDEXBUFFER_TRIANGLE_STRIPS:
        case INDEXBUFFER_TRIANGLE_FANS:
            Assert(iFacet > 0);
            iIndex = iFacet + 2;
            m_arrIndices[iIndex] = iVertexA;
            break;
        default: Assert(false); return;
    }
}
Void IndexBuffer::SetFacet( UInt iFacet, UInt iVertexA, UInt iVertexB )
{
    Assert(iFacet < m_iFacetCount);

    UInt iIndex;
    switch(m_iMode) {
        case INDEXBUFFER_LINE_LIST:
            iIndex = (iFacet << 1); // * 2
            m_arrIndices[iIndex] = iVertexA;
            m_arrIndices[iIndex + 1] = iVertexB;
            break;
        case INDEXBUFFER_LINE_STRIPS:
        case INDEXBUFFER_LINE_FANS:
            // special case for first facet
            Assert(iFacet == 0);
            m_arrIndices[0] = iVertexA;
            m_arrIndices[1] = iVertexB;
            break;
        case INDEXBUFFER_QUAD_STRIPS:
            Assert(iFacet > 0);
            iIndex = (iFacet << 1) + 2; // * 2
            m_arrIndices[iIndex] = iVertexA;
            m_arrIndices[iIndex + 1] = iVertexB;
            break;
        default: Assert(false); return;
    }
}
Void IndexBuffer::SetFacet( UInt iFacet, UInt iVertexA, UInt iVertexB, UInt iVertexC )
{
    Assert(iFacet < m_iFacetCount);

    UInt iIndex;
    switch(m_iMode) {
        case INDEXBUFFER_TRIANGLE_LIST:
            iIndex = (iFacet << 1) + iFacet; // * 3
            m_arrIndices[iIndex] = iVertexA;
            m_arrIndices[iIndex + 1] = iVertexB;
            m_arrIndices[iIndex + 2] = iVertexC;
            break;
        case INDEXBUFFER_TRIANGLE_STRIPS:
        case INDEXBUFFER_TRIANGLE_FANS:
            // special case for first facet
            Assert(iFacet == 0);
            m_arrIndices[0] = iVertexA;
            m_arrIndices[1] = iVertexB;
            m_arrIndices[2] = iVertexC;
            break;
        default: Assert(false); return;
    }
}
Void IndexBuffer::SetFacet( UInt iFacet, UInt iVertexA, UInt iVertexB, UInt iVertexC, UInt iVertexD )
{
    Assert(iFacet < m_iFacetCount);

    UInt iIndex;
    switch(m_iMode) {
        case INDEXBUFFER_QUAD_LIST:
            iIndex = (iFacet << 2); // * 4
            m_arrIndices[iIndex] = iVertexA;
            m_arrIndices[iIndex + 1] = iVertexB;
            m_arrIndices[iIndex + 2] = iVertexC;
            m_arrIndices[iIndex + 3] = iVertexD;
            break;
        case INDEXBUFFER_QUAD_STRIPS:
            // special case for first facet
            Assert(iFacet == 0);
            m_arrIndices[0] = iVertexA;
            m_arrIndices[1] = iVertexB;
            m_arrIndices[2] = iVertexC;
            m_arrIndices[3] = iVertexD;
            break;
        default: Assert(false); return;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// FrameBuffer implementation
FrameBuffer::FrameBuffer( MemoryAllocatorID idAllocator,
                          ColorFormat iColorFormat,
                          FrameBufferDepthType iDepthType,
                          FrameBufferStencilType iStencilType ):
    m_idAllocator(idAllocator)
{
    m_iColorFormat = iColorFormat;
    m_iDepthType = iDepthType;
    m_iStencilType = iStencilType;
    m_iWidth = 0;
    m_iHeight = 0;
    m_iSize = 0;
    m_pColorBufferF = NULL;
    m_pColorBufferI = NULL;
    m_pDepthBufferF = NULL;
    m_pDepthBufferI = NULL;
    m_iMaxDepthValue = 0;
    m_pStencilBuffer = NULL;
    m_iMaxStencilValue = 0;
    m_bOwnsColorBuffer = false;
    m_bAllocated = false;
}
FrameBuffer::~FrameBuffer()
{
    if (m_bAllocated)
        Destroy();
}

Void FrameBuffer::Allocate( UInt iWidth, UInt iHeight, PixelMap * pColorBufferI )
{
    Assert(!m_bAllocated);

    m_iWidth = iWidth;
    m_iHeight = iHeight;
    m_iSize = m_iWidth * m_iHeight;

    MemoryAllocatorID idSaved = MemoryFn->GetAllocator();
    MemoryFn->SelectAllocator( m_idAllocator );

    m_pColorBufferF = KNewIm() Color4[m_iSize];
    if (pColorBufferI == NULL) {
        m_pColorBufferI = KNewIm() PixelMap();
        m_pColorBufferI->Allocate( m_iWidth, m_iHeight, m_iColorFormat );
        m_bOwnsColorBuffer = true;
    } else {
        m_pColorBufferI = pColorBufferI;
        m_bOwnsColorBuffer = false;
    }

    if (m_iDepthType == FRAMEBUFFER_DEPTH_NONE) {
        m_pDepthBufferF = NULL;
        m_pDepthBufferI = NULL;
        m_iMaxDepthValue = 0;
    } else {
        m_pDepthBufferF = KNewIm() Scalar[m_iSize];
        m_pDepthBufferI = KNewIm() ULong[m_iSize];
        switch(m_iDepthType) {
            case FRAMEBUFFER_DEPTH_32: m_iMaxDepthValue = 0xffffffff; break;
            case FRAMEBUFFER_DEPTH_24: m_iMaxDepthValue = 0x00ffffff; break;
            case FRAMEBUFFER_DEPTH_16: m_iMaxDepthValue = 0x0000ffff; break;
        }
    }

    if (m_iStencilType == FRAMEBUFFER_STENCIL_NONE) {
        m_pStencilBuffer = NULL;
        m_iMaxStencilValue = 0;
    } else {
        m_pStencilBuffer = KNewIm() ULong[m_iSize];
        switch(m_iStencilType) {
            case FRAMEBUFFER_STENCIL_32: m_iMaxStencilValue = 0xffffffff; break;
            case FRAMEBUFFER_STENCIL_24: m_iMaxStencilValue = 0x00ffffff; break;
            case FRAMEBUFFER_STENCIL_16: m_iMaxStencilValue = 0x0000ffff; break;
            case FRAMEBUFFER_STENCIL_8:  m_iMaxStencilValue = 0x000000ff; break;
        }
    }

    MemoryFn->SelectAllocator( idSaved );

    m_bAllocated = true;
}
Void FrameBuffer::Destroy()
{
    Assert(m_bAllocated);

    MemoryAllocatorID idSaved = MemoryFn->GetAllocator();
    MemoryFn->SelectAllocator( m_idAllocator );

    if (m_iStencilType != FRAMEBUFFER_STENCIL_NONE)
        KDeleteAIm(m_pStencilBuffer);
    m_pStencilBuffer = NULL;
    m_iMaxStencilValue = 0;

    if (m_iDepthType != FRAMEBUFFER_DEPTH_NONE) {
        KDeleteAIm(m_pDepthBufferI);
        KDeleteAIm(m_pDepthBufferF);
    }
    m_pDepthBufferI = NULL;
    m_pDepthBufferF = NULL;
    m_iMaxDepthValue = 0;

    if (m_bOwnsColorBuffer) {
        m_pColorBufferI->Destroy();
        KDeleteIm(m_pColorBufferI);
    }
    KDeleteAIm(m_pColorBufferF);
    m_pColorBufferI = NULL;
    m_pColorBufferF = NULL;
    m_bOwnsColorBuffer = false;

    m_iWidth = 0;
    m_iHeight = 0;
    m_iSize = 0;

    MemoryFn->SelectAllocator( idSaved );

    m_bAllocated = false;
}

Void FrameBuffer::ClearColorBuffer()
{
    Assert(m_bAllocated);

    for(UInt i = 0; i < m_iSize; ++i)
        m_pColorBufferF[i] = m_colClearColor;
    m_pColorBufferI->FillRect( m_pColorBufferI->Rect(), m_colClearColor );
}
Void FrameBuffer::ClearDepthBuffer()
{
    Assert(m_bAllocated);

    if (m_iDepthType != FRAMEBUFFER_DEPTH_NONE) {
        ULong iClearDepth = (ULong)( MathFn->Round(m_fClearDepth * (Scalar)m_iMaxDepthValue) );
        for(UInt i = 0; i < m_iSize; ++i) {
            m_pDepthBufferF[i] = m_fClearDepth;
            m_pDepthBufferI[i] = iClearDepth;
        }
    }
}
Void FrameBuffer::ClearStencilBuffer()
{
    Assert(m_bAllocated);

    if (m_iStencilType != FRAMEBUFFER_STENCIL_NONE) {
        for(UInt i = 0; i < m_iSize; ++i)
            m_pStencilBuffer[i] = m_iClearStencil;
    }
}
Void FrameBuffer::ClearBuffers()
{
    ClearColorBuffer();
    ClearDepthBuffer();
    ClearStencilBuffer();
}
Void FrameBuffer::ClearSubBuffers( UInt iMinX, UInt iMaxX, UInt iMinY, UInt iMaxY )
{
    ClearSubColorBuffer(iMinX, iMaxX, iMinY, iMaxY);
    ClearSubDepthBuffer(iMinX, iMaxX, iMinY, iMaxY);
    ClearSubStencilBuffer(iMinX, iMaxX, iMinY, iMaxY);
}

Void FrameBuffer::SetColorFormat(ColorFormat iColorFormat)
{
    Assert(!m_bAllocated);

    m_iColorFormat = iColorFormat;
}
Void FrameBuffer::SetDepthType(FrameBufferDepthType iDepthType)
{
    Assert(!m_bAllocated);

    m_iDepthType = iDepthType;
}
Void FrameBuffer::SetStencilType(FrameBufferStencilType iStencilType)
{
    Assert(!m_bAllocated);

    m_iStencilType = iStencilType;
}
Void FrameBuffer::UpdateColor( UInt iPixel, UInt iX, UInt iY, const Color4 & colValue )
{
    Assert(m_bAllocated);
    Assert(iPixel < m_iSize);

    m_pColorBufferF[iPixel] = colValue;
    m_pColorBufferI->SetPixel( iX, iY, colValue );
}
Void FrameBuffer::UpdateDepth( UInt iPixel, Scalar fValue, ULong iValue )
{
    Assert(m_bAllocated);
    Assert(iPixel < m_iSize);

    m_pDepthBufferF[iPixel] = fValue;
    if (iValue > m_iMaxDepthValue)
        m_pDepthBufferI[iPixel] = m_iMaxDepthValue;
    else
        m_pDepthBufferI[iPixel] = iValue;
}
Void FrameBuffer::UpdateStencil( UInt iPixel, ULong iValue )
{
    Assert(m_bAllocated);
    Assert(iPixel < m_iSize);

    if (iValue > m_iMaxStencilValue)
        m_pStencilBuffer[iPixel] = m_iMaxStencilValue;
    else
        m_pStencilBuffer[iPixel] = iValue;
}

