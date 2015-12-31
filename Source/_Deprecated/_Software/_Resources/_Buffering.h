/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Buffering.h
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
// Header prelude
#ifndef KOALA_ENGINE_GRAPHICS_3D_BUFFERING_H
#define KOALA_ENGINE_GRAPHICS_3D_BUFFERING_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Core/Math/Types/Discrete.h"
#include "../../../Lib/Core/Math/Types/Vector.h"
#include "../../../Lib/Core/Math/Types/Vertex.h"
#include "../../../Lib/Core/Math/Types/TextureCoord.h"
#include "../../../Lib/Core/Math/Types/Color.h"
#include "../../../Lib/Core/Math/Types/Matrix.h"
#include "../../../Lib/Core/Math/Types/Quaternion.h"

#include "../2D/PixelMap.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // CFL types
enum CFLType
{
    CFLTYPE_UNDEFINED = 0,

    CFLTYPE_LONG,
    CFLTYPE_LONG2,
    CFLTYPE_LONG3,
    CFLTYPE_LONG4,
    CFLTYPE_ULONG,
    CFLTYPE_ULONG2,
    CFLTYPE_ULONG3,
    CFLTYPE_ULONG4,

    CFLTYPE_FLOAT,
    CFLTYPE_FLOAT2,
    CFLTYPE_FLOAT3,
    CFLTYPE_FLOAT4,

    CFLTYPE_POINT2,

    CFLTYPE_VERTEX2,
    CFLTYPE_VERTEX3,
    CFLTYPE_VERTEX4,

    CFLTYPE_VECTOR2,
    CFLTYPE_VECTOR3,
    CFLTYPE_VECTOR4,

    CFLTYPE_TEXCOORD1,
    CFLTYPE_TEXCOORD2,
    CFLTYPE_TEXCOORD3,
    CFLTYPE_TEXCOORD4,

    CFLTYPE_COLOR,

    CFLTYPE_MATRIX2,
    CFLTYPE_MATRIX3,
    CFLTYPE_MATRIX4,

    CFLTYPE_QUATERNION,

    CFLTYPE_COUNT
};
    // CFL semantics
enum CFLSemantic
{
    // Vertex-Format basis
    CFLSEMANTIC_POSITION = 0,  // 3D position

    // Pixel-Format basis
    CFLSEMANTIC_RASTERPOS,     // Raster position in current viewport, in pixels
    CFLSEMANTIC_DEPTH,         // Depth value at this pixel
    CFLSEMANTIC_INVERTW,       // Inverted W value at this pixel
    CFLSEMANTIC_REGULARW,      // W value at this pixel

    // Input values
    CFLSEMANTIC_NORMAL,        // Normal at this vertex
    CFLSEMANTIC_TANGENT,       // Tangent at this vertex
    CFLSEMANTIC_BINORMAL,      // Bi-Normal at this vertex
    CFLSEMANTIC_TEXCOORD,      // Texture coordinates
    CFLSEMANTIC_COLOR,         // Color

    // Output / Computed values
    CFLSEMANTIC_DIFFUSE,       // Diffuse Color
    CFLSEMANTIC_SPECULAR,      // Specular Color

    // Specific-use values
    CFLSEMANTIC_BLENDWEIGHT,   // Blending weitgh factor
    CFLSEMANTIC_BLENDINDICES,  // Matrix indices for paletted skinning
    CFLSEMANTIC_FOGCOORD,      // Local fog coordinates
    CFLSEMANTIC_FOGFACTOR,     // Local fog factor
    CFLSEMANTIC_SAMPLE,        // Sampler data
    CFLSEMANTIC_POINTSIZE,     // Dot size at this vertex to use while rasterizing (Point-Sprite)

    // User-defined value
    CFLSEMANTIC_USERDATA,      // Custom user data

    CFLSEMANTIC_COUNT
};
    // CFL limits
#define CFL_FACTORY_SIZE    65536
#define CFL_MAX_INDEX_SHIFT 3
#define CFL_MAX_INDEX       ( 1ul << CFL_MAX_INDEX_SHIFT )
#define CFL_MAX_FIELDS      ( CFLSEMANTIC_COUNT * CFL_MAX_INDEX )
#define CFL_MAX_FIELDSIZE   16 // Matrix4 case
#define CFL_MAX_SIZE        ( CFL_MAX_FIELDS * CFL_MAX_FIELDSIZE )

    // CFL Field
typedef Scalar * CFLPtr;
typedef const Scalar * CCFLPtr;
typedef struct _cfl_field
{
    UInt iOffset; // in a Scalar array
    UInt iSize;   // in # of Scalar
    CFLType iType;
    CFLSemantic iSemantic;
    UInt iSemanticIndex; // index in the format's LUT to differenciate
                         // multiple fields with same usage flag
} CFLField;

    // IndexBuffer stuff
enum IndexBufferMode
{
	INDEXBUFFER_VERTEX_LIST = 0,
	INDEXBUFFER_LINE_LIST,
	INDEXBUFFER_LINE_STRIPS,
	INDEXBUFFER_LINE_FANS,
	INDEXBUFFER_TRIANGLE_LIST,
	INDEXBUFFER_TRIANGLE_STRIPS,
	INDEXBUFFER_TRIANGLE_FANS,
	INDEXBUFFER_QUAD_LIST,
	INDEXBUFFER_QUAD_STRIPS
};

    // Pipes stuff
//typedef struct _fragment_vertex
//{
//    CustomStructureBasis * pVertex;
//} FragmentVertex;
//typedef struct _fragment_facet
//{
//    CustomStructureBasis * pVertexA;
//    CustomStructureBasis * pVertexB;
//    CustomStructureBasis * pVertexC;
//} FragmentFacet;
//typedef struct _fragment_pixel
//{
//    CustomStructureBasis * pPixel;
//} FragmentPixel;

    // FrameBuffer stuff
enum FrameBufferDepthType
{
    FRAMEBUFFER_DEPTH_NONE,
    FRAMEBUFFER_DEPTH_16,
    FRAMEBUFFER_DEPTH_24,
    FRAMEBUFFER_DEPTH_32
};
enum FrameBufferStencilType
{
    FRAMEBUFFER_STENCIL_NONE,
    FRAMEBUFFER_STENCIL_8,
    FRAMEBUFFER_STENCIL_16,
    FRAMEBUFFER_STENCIL_24,
    FRAMEBUFFER_STENCIL_32
};
//enum FrameBufferSamplingType
//{
//    FRAMEBUFFER_SAMPLING_NONE,
//    FRAMEBUFFER_SAMPLING_2,
//    FRAMEBUFFER_SAMPLING_4
//};

/////////////////////////////////////////////////////////////////////////////////
// The CustomFragmentLayout class
class CustomFragmentLayout
{
public:
    CustomFragmentLayout();
    ~CustomFragmentLayout();

    // Declare fields & finalize
    Void DeclareField( CFLType fieldType, CFLSemantic fieldSemantic );
    Void DeclareVertexFormat();
    Void DeclarePixelFormat();
    Void Finalize();

    // Allocate & Free fragments
    inline CFLPtr Allocate() const;
    inline Void Free( CFLPtr pCFLData ) const;

    // Access fields (Sizes & Offsets in terms of Scalar arrays)
    inline UInt GetTotalSize() const;
    inline UInt GetFieldsCount() const;

    inline Bool HasField( CFLSemantic iQuerySemantic, UInt iQueryIndex = 0 ) const;
    inline UInt GetFieldOffset( CFLSemantic iQuerySemantic, UInt iQueryIndex = 0 ) const;
    inline UInt GetFieldSize( CFLSemantic iQuerySemantic, UInt iQueryIndex = 0 ) const;
    inline CFLType GetFieldType( CFLSemantic iQuerySemantic, UInt iQueryIndex = 0 ) const;

    inline CFLPtr GetField( CFLPtr pCFLData, CFLSemantic iQuerySemantic, UInt iQueryIndex = 0 ) const;
    inline CCFLPtr GetField( CCFLPtr pCFLData, CFLSemantic iQuerySemantic, UInt iQueryIndex = 0 ) const;

    // Helpers for some assertions, see remarks below
    Bool IsValidVertexFormat() const;
    Bool IsValidPixelFormat() const;

    // Retroactive code that should never be used with good
    // coding practice as you obviously allways know what a
    // CFL contains exactly ... (yeah that's compile-time data in fact !)
    Bool IsEqual( const CustomFragmentLayout * pCFL ) const;
    inline Bool IsCompatible( const CustomFragmentLayout * pCFL ) const {
        // Compatibility must be ensured by the programmer ...
        // Finding a pattern for all imaginative stuff shader
        // coders might do is ... impossible xD !
        // Never use this, rely on your rigorous coding practices
        // to ensure you allways make VS & PS code and data both coherent
        // with the pipeline and themselves...
        // Allways test with the software renderer to avoid crashing your
        // video card ... seriously.
        return true;
    }

private:
    UInt _CFL_SizeOf( CFLType fieldType ) const;

    CFLField m_arrFields[CFL_MAX_FIELDS];
    UInt m_iSize, m_iFieldsCount;
    Bool m_bFinalized;

    MemoryAllocatorID m_idFactory; // Pool allocation
};

/////////////////////////////////////////////////////////////////////////////////
// The VertexArray class
class VertexArray
{
public:
    VertexArray( UInt iCapacity, MemoryAllocatorID idAllocator );
    ~VertexArray();

    inline UInt Length() const;
    inline CFLPtr GetVertex( UInt iVertex ) const;

    inline Void Push( CFLPtr pVertex );
    inline Void Clear();

private:
    const MemoryAllocatorID m_idAllocator;

    CFLPtr * m_arrVertices;
    UInt m_iCapacity, m_iLength;
};

/////////////////////////////////////////////////////////////////////////////////
// The VertexBuffer class
class VertexBuffer
{
public:
	VertexBuffer( const CustomFragmentLayout * pCFL, UInt iVertexCount, MemoryAllocatorID idAllocator );
	~VertexBuffer();

    inline const CustomFragmentLayout * GetCFL() const;
    inline UInt GetVertexSize() const;
    inline UInt GetVertexCount() const;
    inline UInt GetSize() const;

    inline CFLPtr * GetData();
    inline const CFLPtr * GetData() const;

    inline CFLPtr GetVertex( UInt iVertex );
    inline CCFLPtr GetVertex( UInt iVertex ) const;

    inline CFLPtr GetVertexField( UInt iVertex, CFLSemantic iQuerySemantic, UInt iQueryIndex );
    inline CCFLPtr GetVertexField( UInt iVertex, CFLSemantic iQuerySemantic, UInt iQueryIndex ) const;

private:
    const MemoryAllocatorID m_idAllocator;
    const CustomFragmentLayout * m_pCFL;

	CFLPtr * m_arrVertices;
	UInt m_iVertexSize, m_iVertexCount;
    UInt m_iTotalSize;
};

/////////////////////////////////////////////////////////////////////////////////
// The IndexBuffer class
class IndexBuffer
{
public:
	IndexBuffer( IndexBufferMode iMode, UInt iFacetCount, MemoryAllocatorID idAllocator );
	~IndexBuffer();

	inline IndexBufferMode GetMode() const;
	inline UInt GetFacetCount() const;
	inline UInt GetSize() const;

    inline UInt * GetData();
    inline const UInt * GetData() const;

    inline UInt & operator[](UInt iIndex);
    inline UInt operator[](UInt iIndex) const;
    inline UInt & operator[](Int iIndex);
    inline UInt operator[](Int iIndex) const;

    Void GetFacet( UInt iFacet, UInt & outVertexA ) const;
    Void GetFacet( UInt iFacet, UInt & outVertexA, UInt & outVertexB ) const;
    Void GetFacet( UInt iFacet, UInt & outVertexA, UInt & outVertexB, UInt & outVertexC ) const;
    Void GetFacet( UInt iFacet, UInt & outVertexA, UInt & outVertexB, UInt & outVertexC, UInt & outVertexD ) const;

    Void SetFacet( UInt iFacet, UInt iVertexA );
    Void SetFacet( UInt iFacet, UInt iVertexA, UInt iVertexB );
    Void SetFacet( UInt iFacet, UInt iVertexA, UInt iVertexB, UInt iVertexC );
    Void SetFacet( UInt iFacet, UInt iVertexA, UInt iVertexB, UInt iVertexC, UInt iVertexD );

private:
    const MemoryAllocatorID m_idAllocator;
    const IndexBufferMode m_iMode;

	UInt * m_arrIndices;
	UInt m_iFacetCount, m_iIndexCount;
};

/////////////////////////////////////////////////////////////////////////////////
// The FrameBuffer class
class FrameBuffer
{
public:
    FrameBuffer( MemoryAllocatorID idAllocator,
                 ColorFormat iColorFormat = COLOR_FORMAT_A8R8G8B8,
                 FrameBufferDepthType iDepthType = FRAMEBUFFER_DEPTH_32,
                 FrameBufferStencilType iStencilType = FRAMEBUFFER_STENCIL_NONE );
    ~FrameBuffer();

    // Memory management
    Void Allocate( UInt iWidth, UInt iHeight, PixelMap * pColorBufferI = NULL );
    Void Destroy();

    // Dimensions
    inline UInt Width() const;
    inline UInt Height() const;
    inline UInt Size() const;

    // Clearing
    inline Void SetClearColor( const Color4 & colClear );
    inline Void SetClearDepth( Scalar fClear );
    inline Void SetClearStencil( UInt iClear );

    Void ClearColorBuffer();
    Void ClearDepthBuffer();
    Void ClearStencilBuffer();
    Void ClearBuffers();
    Void ClearSubColorBuffer( UInt iMinX, UInt iMaxX, UInt iMinY, UInt iMaxY )
    {
        Assert(m_bAllocated);
        // UNIMPLEMENTED
    }
    Void ClearSubDepthBuffer( UInt iMinX, UInt iMaxX, UInt iMinY, UInt iMaxY )
    {
        Assert(m_bAllocated);
        // UNIMPLEMENTED
    }
    Void ClearSubStencilBuffer( UInt iMinX, UInt iMaxX, UInt iMinY, UInt iMaxY )
    {
        Assert(m_bAllocated);
        // UNIMPLEMENTED
    }
    Void ClearSubBuffers( UInt iMinX, UInt iMaxX, UInt iMinY, UInt iMaxY );

    // Color buffer
    inline Bool HasColorBuffer() const;
    inline ColorFormat GetColorFormat() const;
    inline Color4 * GetColorBufferF() const;
    inline Color4 & GetColorF( UInt iPixel );
    inline const Color4 & GetColorF( UInt iPixel ) const;
    inline PixelMap * GetColorBufferI();
    inline const PixelMap * GetColorBufferI() const;

    // Depth buffer
    inline Bool HasDepthBuffer() const;
    inline FrameBufferDepthType GetDepthType() const;
    inline ULong MaxDepthValue() const;
    inline Scalar * GetDepthBufferF() const;
    inline Scalar & GetDepthF( UInt iPixel );
    inline const Scalar & GetDepthF( UInt iPixel ) const;
    inline ULong * GetDepthBufferI() const;
    inline ULong & GetDepthI( UInt iPixel );
    inline const ULong & GetDepthI( UInt iPixel ) const;

    // Stencil buffer
    inline Bool HasStencilBuffer() const;
    inline FrameBufferStencilType GetStencilType() const;
    inline ULong MaxStencilValue() const;
    inline ULong * GetStencilBuffer() const;
    inline ULong & GetStencil( UInt iPixel );
    inline const ULong & GetStencil( UInt iPixel ) const;

    // Update operations
    Void SetColorFormat( ColorFormat iColorFormat );
    Void SetDepthType( FrameBufferDepthType iDepthType );
    Void SetStencilType( FrameBufferStencilType iStencilType );
    Void UpdateColor( UInt iPixel, UInt iX, UInt iY, const Color4 & colValue );
    Void UpdateDepth( UInt iPixel, Scalar fValue, ULong iValue );
    Void UpdateStencil( UInt iPixel, ULong iValue );

private:
    const MemoryAllocatorID m_idAllocator;

    // State
    Bool m_bOwnsColorBuffer;
    Bool m_bAllocated;

    // Parameters
    ColorFormat m_iColorFormat;
    FrameBufferDepthType m_iDepthType;
    FrameBufferStencilType m_iStencilType;
    Color4 m_colClearColor;
    Scalar m_fClearDepth;
    UInt m_iClearStencil;

    // Dimensions
    UInt m_iWidth, m_iHeight;
    UInt m_iSize;

    // Color buffer    
    Color4 * m_pColorBufferF;
    PixelMap * m_pColorBufferI;
    // Depth Buffer
    Scalar * m_pDepthBufferF;
    ULong * m_pDepthBufferI;
    ULong m_iMaxDepthValue;

    // Stencil Buffer
    ULong * m_pStencilBuffer;
    ULong m_iMaxStencilValue;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Buffering.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_BUFFERING_H
