/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Mappings.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : API-dependant constants mappings.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Internal dependency : Direct3D9, DirectX
#define WIN32_LEAN_AND_MEAN
#include <d3d9.h>

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Mappings.h"

/////////////////////////////////////////////////////////////////////////////////
// Mappings
D3DDECLTYPE Direct3D9AttributeType[VFTYPE_COUNT] =
{
    (D3DDECLTYPE)0xff,      // VFTYPE_UNDEFINED

    D3DDECLTYPE_FLOAT1,     // VFTYPE_FLOAT
    D3DDECLTYPE_FLOAT2,     // VFTYPE_FLOAT2
    D3DDECLTYPE_FLOAT3,     // VFTYPE_FLOAT3
    D3DDECLTYPE_FLOAT4,     // VFTYPE_FLOAT4
    (D3DDECLTYPE)0xff,      // VFTYPE_FLOAT9
    (D3DDECLTYPE)0xff,      // VFTYPE_FLOAT16
    D3DDECLTYPE_FLOAT16_2,  // VFTYPE_HALF2
    D3DDECLTYPE_FLOAT16_4,  // VFTYPE_HALF4

    D3DDECLTYPE_UBYTE4,     // VFTYPE_UBYTE4
    D3DDECLTYPE_UBYTE4N,    // VFTYPE_UBYTE4N
    D3DDECLTYPE_D3DCOLOR,   // VFTYPE_COLOR (ARGB => RGBA)

    (D3DDECLTYPE)0xff,      // VFTYPE_USHORT2
    (D3DDECLTYPE)0xff,      // VFTYPE_USHORT4

    D3DDECLTYPE_USHORT2N,   // VFTYPE_USHORT2N
    D3DDECLTYPE_USHORT4N,   // VFTYPE_USHORT4N

    D3DDECLTYPE_SHORT2,     // VFTYPE_SHORT2
    D3DDECLTYPE_SHORT4,     // VFTYPE_SHORT4

    D3DDECLTYPE_SHORT2N,    // VFTYPE_SHORT2N
    D3DDECLTYPE_SHORT4N,    // VFTYPE_SHORT4N
};
D3DDECLUSAGE Direct3D9AttributeUsage[VFUSAGE_COUNT] =
{
    (D3DDECLUSAGE)0xff,     // VFUSAGE_UNDEFINED

    D3DDECLUSAGE_POSITION,  // VFUSAGE_POSITION

    D3DDECLUSAGE_NORMAL,    // VFUSAGE_NORMAL
    D3DDECLUSAGE_TANGENT,   // VFUSAGE_TANGENT
    D3DDECLUSAGE_BINORMAL,  // VFUSAGE_BINORMAL

    D3DDECLUSAGE_BLENDWEIGHT, // VFUSAGE_BLENDWEIGHT
    D3DDECLUSAGE_BLENDINDICES, // VFUSAGE_BLENDINDICES
    D3DDECLUSAGE_FOG,       // VFUSAGE_FOG
    D3DDECLUSAGE_TESSFACTOR, // VFUSAGE_FOGCOORD
    D3DDECLUSAGE_DEPTH,     // VFUSAGE_DEPTH
    D3DDECLUSAGE_SAMPLE,    // VFUSAGE_SAMPLE
    D3DDECLUSAGE_PSIZE,     // VFUSAGE_POINTSIZE
    D3DDECLUSAGE_POSITIONT, // VFUSAGE_TPOS

    D3DDECLUSAGE_COLOR,     // VFUSAGE_COLOR
    D3DDECLUSAGE_TEXCOORD,  // VFUSAGE_TEXCOORD

    (D3DDECLUSAGE)0xff      // VFUSAGE_USERDATA
};

DWord Direct3D9VertexBufferUsage[VB_USAGE_COUNT] =
{
    D3DUSAGE_WRITEONLY,                   // VB_USAGE_STATIC
    D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC // VB_USAGE_DYNAMIC
};
DWord Direct3D9VertexBufferLock[VB_LOCK_COUNT] =
{
    0,                  // VB_LOCK_UPDATE
    D3DLOCK_DISCARD,    // VB_LOCK_OVERWRITE
    D3DLOCK_NOOVERWRITE // VB_LOCK_APPEND
};

DWord Direct3D9IndexBufferUsage[IB_USAGE_COUNT] =
{
    D3DUSAGE_WRITEONLY,                   // IB_USAGE_STATIC
    D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC // IB_USAGE_DYNAMIC
};
DWord Direct3D9IndexBufferLock[IB_LOCK_COUNT] =
{
    0,                  // IB_LOCK_UPDATE
    D3DLOCK_DISCARD,    // IB_LOCK_OVERWRITE
    D3DLOCK_NOOVERWRITE // IB_LOCK_APPEND
};

D3DFORMAT Direct3D9TextureFormat[TEXTURE_FMT_COUNT] =
{
    D3DFMT_UNKNOWN,         // TEXTURE_FMT_UNDEFINED

    D3DFMT_R3G3B2,          // TEXTURE_FMT_R3G3B2
    D3DFMT_A8R3G3B2,        // TEXTURE_FMT_A8R3G3B2
    D3DFMT_R5G6B5,          // TEXTURE_FMT_R5G6B5
    D3DFMT_A1R5G5B5,        // TEXTURE_FMT_A1R5G5B5
    D3DFMT_A2R10G10B10,     // TEXTURE_FMT_A2R10G10B10
    D3DFMT_A2B10G10R10,     // TEXTURE_FMT_A2B10G10R10

    D3DFMT_A4L4,            // TEXTURE_FMT_AL4I
    D3DFMT_A4R4G4B4,        // TEXTURE_FMT_ARGB4I

    D3DFMT_A8,              // TEXTURE_FMT_A8I
    D3DFMT_L8,              // TEXTURE_FMT_L8I
    D3DFMT_A8L8,            // TEXTURE_FMT_AL8I
    D3DFMT_V8U8,            // TEXTURE_FMT_VU8I
    D3DFMT_Q8W8V8U8,        // TEXTURE_FMT_QWVU8I
    D3DFMT_R8G8B8,          // TEXTURE_FMT_RGB8I
    D3DFMT_A8R8G8B8,        // TEXTURE_FMT_ARGB8I
    D3DFMT_A8B8G8R8,        // TEXTURE_FMT_ABGR8I
    D3DFMT_R8G8_B8G8,       // TEXTURE_FMT_RGBG8I
    D3DFMT_G8R8_G8B8,       // TEXTURE_FMT_GRGB8I

    D3DFMT_L16,             // TEXTURE_FMT_L16I
    D3DFMT_V16U16,          // TEXTURE_FMT_VU16I
    D3DFMT_Q16W16V16U16,    // TEXTURE_FMT_QWVU16I
    D3DFMT_G16R16,          // TEXTURE_FMT_GR16I
    D3DFMT_A16B16G16R16,    // TEXTURE_FMT_ABGR16I

    D3DFMT_R16F,            // TEXTURE_FMT_R16F
    D3DFMT_G16R16F,         // TEXTURE_FMT_GR16F
    D3DFMT_A16B16G16R16F,   // TEXTURE_FMT_ABGR16F

    D3DFMT_R32F,            // TEXTURE_FMT_R32F
    D3DFMT_G32R32F,         // TEXTURE_FMT_GR32F
    D3DFMT_A32B32G32R32F,   // TEXTURE_FMT_ABGR32F

    D3DFMT_D16,             // TEXTURE_FMT_D16
    D3DFMT_D32,             // TEXTURE_FMT_D32
    D3DFMT_D15S1,           // TEXTURE_FMT_D15S1
    D3DFMT_D24S8,           // TEXTURE_FMT_D24S8
    D3DFMT_D24FS8,          // TEXTURE_FMT_D24FS8

    D3DFMT_DXT1,            // TEXTURE_FMT_DXT1
    D3DFMT_DXT2,            // TEXTURE_FMT_DXT2
    D3DFMT_DXT3,            // TEXTURE_FMT_DXT3
    D3DFMT_DXT4,            // TEXTURE_FMT_DXT4
    D3DFMT_DXT5             // TEXTURE_FMT_DXT5
};
DWord Direct3D9TextureUsage[TEXTURE_USAGE_COUNT] =
{
    0,                     // TEXTURE_USAGE_STATIC
    D3DUSAGE_DYNAMIC,      // TEXTURE_USAGE_DYNAMIC
    D3DUSAGE_RENDERTARGET, // TEXTURE_USAGE_RENDERTARGET
    D3DUSAGE_DEPTHSTENCIL  // TEXTURE_USAGE_DEPTHSTENCIL
};
DWord Direct3D9TextureLock[TEXTURE_LOCK_COUNT] =
{
    0,                  // TEXTURE_LOCK_UPDATE
    D3DLOCK_DISCARD,    // TEXTURE_LOCK_OVERWRITE
    D3DLOCK_NOOVERWRITE // TEXTURE_LOCK_APPEND
};
D3DCUBEMAP_FACES Direct3D9TextureCubeFace[TEXTURECUBE_FACE_COUNT] = 
{
    D3DCUBEMAP_FACE_POSITIVE_X, // TEXTURECUBE_FACE_XPOS
    D3DCUBEMAP_FACE_NEGATIVE_X, // TEXTURECUBE_FACE_XNEG
    D3DCUBEMAP_FACE_POSITIVE_Y, // TEXTURECUBE_FACE_YPOS
    D3DCUBEMAP_FACE_NEGATIVE_Y, // TEXTURECUBE_FACE_YNEG
    D3DCUBEMAP_FACE_POSITIVE_Z, // TEXTURECUBE_FACE_ZPOS
    D3DCUBEMAP_FACE_NEGATIVE_Z  // TEXTURECUBE_FACE_ZNEG
};

DWord Direct3D9MinFilter[SHADERSAMPLING_FILTER_COUNT] =
{
    D3DTEXF_NONE,           // SHADERSAMPLING_FILTER_UNDEFINED

    D3DTEXF_POINT,          // SHADERSAMPLING_FILTER_NEAREST
    D3DTEXF_LINEAR,         // SHADERSAMPLING_FILTER_LINEAR
    D3DTEXF_POINT,          // SHADERSAMPLING_FILTER_NEAREST_NEAREST
    D3DTEXF_POINT,          // SHADERSAMPLING_FILTER_NEAREST_LINEAR
    D3DTEXF_LINEAR,         // SHADERSAMPLING_FILTER_LINEAR_NEAREST
    D3DTEXF_LINEAR          // SHADERSAMPLING_FILTER_LINEAR_LINEAR
};

DWord Direct3D9MipFilter[SHADERSAMPLING_FILTER_COUNT] =
{
    D3DTEXF_NONE,           // SHADERSAMPLING_FILTER_UNDEFINED

    D3DTEXF_NONE,           // SHADERSAMPLING_FILTER_NEAREST
    D3DTEXF_NONE,           // SHADERSAMPLING_FILTER_LINEAR
    D3DTEXF_POINT,          // SHADERSAMPLING_FILTER_NEAREST_NEAREST
    D3DTEXF_LINEAR,         // SHADERSAMPLING_FILTER_NEAREST_LINEAR
    D3DTEXF_POINT,          // SHADERSAMPLING_FILTER_LINEAR_NEAREST
    D3DTEXF_LINEAR          // SHADERSAMPLING_FILTER_LINEAR_LINEAR
};
DWord Direct3D9WrapMode[SHADERSAMPLING_WRAP_COUNT] =
{
    0,                      // SHADERSAMPLING_WRAP_UNDEFINED
    D3DTADDRESS_CLAMP,      // SHADERSAMPLING_WRAP_CLAMP
    D3DTADDRESS_WRAP,       // SHADERSAMPLING_WRAP_REPEAT
    D3DTADDRESS_MIRROR,     // SHADERSAMPLING_WRAP_REPEAT_MIRROR
    D3DTADDRESS_BORDER,     // SHADERSAMPLING_WRAP_CLAMP_BORDER
    D3DTADDRESS_CLAMP       // SHADERSAMPLING_WRAP_CLAMP_EDGE
};

DWord Direct3D9CullOrder[CULLING_CONV_COUNT] = 
{
    D3DCULL_CW,             // CULLING_CONV_CLOCKWISE
    D3DCULL_CCW             // CULLING_CONV_COUNTER_CLOCKWISE
};

DWord Direct3D9DepthTest[DEPTH_COUNT] = 
{
    D3DCMP_NEVER,           // DEPTH_NEVER
    D3DCMP_ALWAYS,          // DEPTH_ALLWAYS
    D3DCMP_EQUAL,           // DEPTH_EQUAL
    D3DCMP_NOTEQUAL,        // DEPTH_NOTEQUAL
    D3DCMP_LESS,            // DEPTH_LESSER
    D3DCMP_LESSEQUAL,       // DEPTH_LESSER_EQUAL
    D3DCMP_GREATER,         // DEPTH_GREATER
    D3DCMP_GREATEREQUAL     // DEPTH_GREATER_EQUAL
};

DWord Direct3D9StencilTest[STENCIL_TEST_COUNT] = 
{
    D3DCMP_NEVER,           // STENCIL_TEST_NEVER
    D3DCMP_ALWAYS,          // STENCIL_TEST_ALLWAYS
    D3DCMP_EQUAL,           // STENCIL_TEST_EQUAL
    D3DCMP_NOTEQUAL,        // STENCIL_TEST_NOTEQUAL
    D3DCMP_LESS,            // STENCIL_TEST_LESSER
    D3DCMP_LESSEQUAL,       // STENCIL_TEST_LESSER_EQUAL
    D3DCMP_GREATER,         // STENCIL_TEST_GREATER
    D3DCMP_GREATEREQUAL     // STENCIL_TEST_GREATER_EQUAL
};
DWord Direct3D9StencilOp[STENCIL_OP_COUNT] =
{
    D3DSTENCILOP_KEEP,      // STENCIL_OP_KEEP
    D3DSTENCILOP_ZERO,      // STENCIL_OP_ZERO
    D3DSTENCILOP_REPLACE,   // STENCIL_OP_REPLACE
    D3DSTENCILOP_INCR,      // STENCIL_OP_INCREMENT
    D3DSTENCILOP_DECR,      // STENCIL_OP_DECREMENT
    D3DSTENCILOP_INVERT     // STENCIL_OP_INVERT
};

DWord Direct3D9BlendSource[BLENDING_SRC_COUNT] =
{
    D3DBLEND_ZERO,          // BLENDING_SRC_ZERO
    D3DBLEND_ONE,           // BLENDING_SRC_ONE
    D3DBLEND_DESTCOLOR,     // BLENDING_SRC_DST_COLOR
    D3DBLEND_INVDESTCOLOR,  // BLENDING_SRC_DST_COLOR_INVERT
    D3DBLEND_SRCALPHA,      // BLENDING_SRC_SRC_ALPHA
    D3DBLEND_INVSRCALPHA,   // BLENDING_SRC_SRC_ALPHA_INVERT
    D3DBLEND_SRCALPHASAT,   // BLENDING_SRC_SRC_ALPHA_SATURATE
    D3DBLEND_DESTALPHA,     // BLENDING_SRC_DST_ALPHA
    D3DBLEND_INVDESTALPHA,  // BLENDING_SRC_DST_ALPHA_INVERT
    0,                      // BLENDING_SRC_CONST_COLOR
    0,                      // BLENDING_SRC_CONST_COLOR_INVERT
    0,                      // BLENDING_SRC_CONST_ALPHA
    0                       // BLENDING_SRC_CONST_ALPHA_INVERT
};
DWord Direct3D9BlendDestination[BLENDING_DST_COUNT] =
{
    D3DBLEND_ZERO,          // BLENDING_DST_ZERO
    D3DBLEND_ONE,           // BLENDING_DST_ONE
    D3DBLEND_SRCCOLOR,      // BLENDING_DST_SRC_COLOR
    D3DBLEND_INVSRCCOLOR,   // BLENDING_DST_SRC_COLOR_INVERT
    D3DBLEND_SRCALPHA,      // BLENDING_DST_SRC_ALPHA
    D3DBLEND_INVSRCALPHA,   // BLENDING_DST_SRC_ALPHA_INVERT
    D3DBLEND_DESTALPHA,     // BLENDING_DST_DST_ALPHA
    D3DBLEND_INVDESTALPHA,  // BLENDING_DST_DST_ALPHA_INVERT
    0,                      // BLENDING_DST_CONST_COLOR
    0,                      // BLENDING_DST_CONST_COLOR_INVERT
    0,                      // BLENDING_DST_CONST_ALPHA
    0                       // BLENDING_DST_CONST_ALPHA_INVERT
};
DWord Direct3D9BlendTest[BLENDING_TEST_COUNT] =
{
    D3DCMP_NEVER,           // BLENDING_TEST_NEVER
    D3DCMP_ALWAYS,          // BLENDING_TEST_ALLWAYS
    D3DCMP_EQUAL,           // BLENDING_TEST_EQUAL
    D3DCMP_NOTEQUAL,        // BLENDING_TEST_NOTEQUAL
    D3DCMP_LESS,            // BLENDING_TEST_LESSER
    D3DCMP_LESSEQUAL,       // BLENDING_TEST_LESSER_EQUAL
    D3DCMP_GREATER,         // BLENDING_TEST_GREATER
    D3DCMP_GREATEREQUAL     // BLENDING_TEST_GREATER_EQUAL
};

D3DPRIMITIVETYPE Direct3D9PrimitiveType[MESH_COUNT] =
{
    D3DPT_POINTLIST,        // MESH_POINT_LIST
    D3DPT_LINELIST,         // MESH_LINE_LIST
    D3DPT_LINESTRIP,        // MESH_LINE_STRIP
    D3DPT_FORCE_DWORD,      // MESH_LINE_FAN (not available)
    D3DPT_TRIANGLELIST,     // MESH_TRIANGLE_LIST
    D3DPT_TRIANGLESTRIP,    // MESH_TRIANGLE_STRIP
    D3DPT_TRIANGLEFAN       // MESH_TRIANGLE_FAN
};

TextureFormat Direct3D9ConvertFormat( D3DFORMAT d3dFormat )
{
    // the most probable in display enumeration
    if ( d3dFormat == D3DFMT_X8R8G8B8 || d3dFormat == D3DFMT_A8R8G8B8 )
        return TEXTURE_FMT_ARGB8I;
    if ( d3dFormat == D3DFMT_R8G8B8 )
        return TEXTURE_FMT_RGB8I;

    D3DFORMAT curFormat;
    for( UInt i = 0; i < TEXTURE_FMT_COUNT; ++i ) {
        curFormat = Direct3D9TextureFormat[i];
        if ( curFormat == d3dFormat )
            return (TextureFormat)i;
    }

    return TEXTURE_FMT_UNDEFINED;
}
UInt Direct3D9FormatBits( D3DFORMAT d3dFormat )
{
    switch( d3dFormat ) {
        case D3DFMT_DXT1:
            return 4;
        case D3DFMT_R3G3B2:
        case D3DFMT_A8:
        case D3DFMT_L8:
        case D3DFMT_A4L4:
        case D3DFMT_DXT2:
        case D3DFMT_DXT3:
        case D3DFMT_DXT4:
        case D3DFMT_DXT5:
            return 8;
        case D3DFMT_A4R4G4B4:
        case D3DFMT_A1R5G5B5:
        case D3DFMT_R5G6B5:
        case D3DFMT_A8R3G3B2:
        case D3DFMT_A8L8:
        case D3DFMT_V8U8:
        case D3DFMT_D15S1:
        case D3DFMT_D16:
        case D3DFMT_L16:
        case D3DFMT_G8R8_G8B8:
        case D3DFMT_R8G8_B8G8:
        case D3DFMT_R16F:
            return 16;
        case D3DFMT_R8G8B8:
            return 24;
        case D3DFMT_A2B10G10R10:
        case D3DFMT_A2R10G10B10:
        case D3DFMT_A8R8G8B8:
        case D3DFMT_A8B8G8R8:
        case D3DFMT_Q8W8V8U8:
        case D3DFMT_V16U16:
        case D3DFMT_G16R16:
        case D3DFMT_D32:
        case D3DFMT_D24S8:
        case D3DFMT_D24FS8:
        case D3DFMT_G16R16F:
        case D3DFMT_R32F:
            return 32;
        case D3DFMT_A16B16G16R16:
        case D3DFMT_Q16W16V16U16:
        case D3DFMT_A16B16G16R16F:
        case D3DFMT_G32R32F:
            return 64;
        case D3DFMT_A32B32G32R32F:
            return 128; // !
        default: DebugAssert( false ); return 0;
    }
}
