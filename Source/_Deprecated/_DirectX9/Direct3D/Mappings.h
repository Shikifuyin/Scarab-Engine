/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Mappings.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_MAPPINGS_H
#define SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_MAPPINGS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Forwards/ForwardDirect3D9.h"

#include "../../System/Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GEOMETRY_VERTEXFORMAT_H
    enum VFFieldType {
        VFTYPE_UNDEFINED = 0,
        VFTYPE_FLOAT, VFTYPE_FLOAT2, VFTYPE_FLOAT3, VFTYPE_FLOAT4,
        VFTYPE_FLOAT9, VFTYPE_FLOAT16, VFTYPE_HALF2, VFTYPE_HALF4,
        VFTYPE_UBYTE4, VFTYPE_UBYTE4N, VFTYPE_COLOR,
        VFTYPE_USHORT2, VFTYPE_USHORT4, VFTYPE_USHORT2N, VFTYPE_USHORT4N,
        VFTYPE_SHORT2, VFTYPE_SHORT4, VFTYPE_SHORT2N, VFTYPE_SHORT4N,
        VFTYPE_COUNT
    };
    enum VFFieldUsage {
        VFUSAGE_UNDEFINED = 0,
        VFUSAGE_POSITION, VFUSAGE_NORMAL, VFUSAGE_TANGENT, VFUSAGE_BINORMAL,
        VFUSAGE_BLENDWEIGHT, VFUSAGE_BLENDINDICES, VFUSAGE_FOG, VFUSAGE_FOGCOORD,
        VFUSAGE_DEPTH, VFUSAGE_SAMPLE, VFUSAGE_POINTSIZE, VFUSAGE_TPOS,
        VFUSAGE_COLOR, VFUSAGE_TEXCOORD, VFUSAGE_USERDATA,
        VFUSAGE_COUNT
    };
    #define VF_MAX_COLORS 4
    #define VF_MAX_TEXCOORDS 8
    #define VF_MAX_USERDATAS 8
    #define VF_MAX_FIELDS ( VFUSAGE_COUNT + VF_MAX_COLORS + VF_MAX_TEXCOORDS + VF_MAX_USERDATAS - 4 )
#endif

#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GEOMETRY_VERTEXBUFFER_H
    enum VertexBufferUsage {
        VB_USAGE_STATIC = 0, VB_USAGE_DYNAMIC,
        VB_USAGE_COUNT
    };
    enum VertexBufferLock {
        VB_LOCK_UPDATE = 0, VB_LOCK_OVERWRITE, VB_LOCK_APPEND,
        VB_LOCK_COUNT
    };
#endif

#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GEOMETRY_INDEXBUFFER_H
    enum IndexBufferUsage {
        IB_USAGE_STATIC = 0, IB_USAGE_DYNAMIC,
        IB_USAGE_COUNT
    };
    enum IndexBufferLock {
        IB_LOCK_UPDATE = 0, IB_LOCK_OVERWRITE, IB_LOCK_APPEND,
        IB_LOCK_COUNT
    };
#endif

#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_TEXTURE_H
    enum TextureFormat {
        TEXTURE_FMT_UNDEFINED = 0,
        TEXTURE_FMT_R3G3B2, TEXTURE_FMT_A8R3G3B2, TEXTURE_FMT_R5G6B5, TEXTURE_FMT_A1R5G5B5,
        TEXTURE_FMT_A2R10G10B10, TEXTURE_FMT_A2B10G10R10,
        TEXTURE_FMT_AL4I, TEXTURE_FMT_ARGB4I,
        TEXTURE_FMT_A8I, TEXTURE_FMT_L8I, TEXTURE_FMT_AL8I, TEXTURE_FMT_VU8I, TEXTURE_FMT_QWVU8I,
        TEXTURE_FMT_RGB8I, TEXTURE_FMT_ARGB8I, TEXTURE_FMT_ABGR8I, TEXTURE_FMT_RGBG8I, TEXTURE_FMT_GRGB8I,
        TEXTURE_FMT_L16I, TEXTURE_FMT_VU16I, TEXTURE_FMT_QWVU16I, TEXTURE_FMT_GR16I, TEXTURE_FMT_ABGR16I,
        TEXTURE_FMT_R16F, TEXTURE_FMT_GR16F, TEXTURE_FMT_ABGR16F,
        TEXTURE_FMT_R32F, TEXTURE_FMT_GR32F, TEXTURE_FMT_ABGR32F,
        TEXTURE_FMT_D16, TEXTURE_FMT_D32, TEXTURE_FMT_D15S1, TEXTURE_FMT_D24S8, TEXTURE_FMT_D24FS8,
        TEXTURE_FMT_DXT1, TEXTURE_FMT_DXT2, TEXTURE_FMT_DXT3, TEXTURE_FMT_DXT4, TEXTURE_FMT_DXT5,
        TEXTURE_FMT_COUNT
    };
    enum TextureUsage {
        TEXTURE_USAGE_STATIC = 0, TEXTURE_USAGE_DYNAMIC, TEXTURE_USAGE_RENDERTARGET, TEXTURE_USAGE_DEPTHSTENCIL,
        TEXTURE_USAGE_COUNT
    };
    enum TextureLock {
        TEXTURE_LOCK_UPDATE = 0, TEXTURE_LOCK_OVERWRITE, TEXTURE_LOCK_APPEND,
        TEXTURE_LOCK_COUNT,
    };
    enum TextureCubeFace {
        TEXTURECUBE_FACE_XPOS = 0, TEXTURECUBE_FACE_XNEG, TEXTURECUBE_FACE_YPOS,
        TEXTURECUBE_FACE_YNEG, TEXTURECUBE_FACE_ZPOS, TEXTURECUBE_FACE_ZNEG,
        TEXTURECUBE_FACE_COUNT
    };
#endif

#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_RENDERTARGET_H
    #define RENDERTARGET_MAX_TARGETS 16
#endif

#ifndef SCARAB_ENGINE_RENDERING_SHADERS_SHADERPROGRAM_H
    enum VertexShaderProfile {
        VERTEXSHADER_NONE = 0, VERTEXSHADER_3_0, VERTEXSHADER_ARBVP1
    };
    enum PixelShaderProfile {
        PIXELSHADER_NONE = 0, PIXELSHADER_3_0, PIXELSHADER_ARBFP1
    };
    enum ShaderSamplingType {
        SHADERSAMPLING_TYPE_UNDEFINED = 0,
        SHADERSAMPLING_TYPE_1D, SHADERSAMPLING_TYPE_2D, SHADERSAMPLING_TYPE_3D, SHADERSAMPLING_TYPE_CUBE,
        SHADERSAMPLING_TYPE_COUNT
    };
    enum ShaderSamplingFilter {
        SHADERSAMPLING_FILTER_UNDEFINED = 0,
        SHADERSAMPLING_FILTER_NEAREST, SHADERSAMPLING_FILTER_LINEAR,
        SHADERSAMPLING_FILTER_NEAREST_NEAREST, SHADERSAMPLING_FILTER_NEAREST_LINEAR,
        SHADERSAMPLING_FILTER_LINEAR_NEAREST, SHADERSAMPLING_FILTER_LINEAR_LINEAR,
        SHADERSAMPLING_FILTER_COUNT
    };
    enum ShaderSamplingWrapMode {
        SHADERSAMPLING_WRAP_UNDEFINED = 0,
        SHADERSAMPLING_WRAP_CLAMP, SHADERSAMPLING_WRAP_REPEAT, SHADERSAMPLING_WRAP_REPEAT_MIRROR,
        SHADERSAMPLING_WRAP_CLAMP_BORDER, SHADERSAMPLING_WRAP_CLAMP_EDGE,
        SHADERSAMPLING_WRAP_COUNT
    };
    #define SHADERPROG_MAX_ANISOTROPY 16
#endif

#ifndef SCARAB_ENGINE_RENDERING_STATES_CULLINGSTATE_H
    enum CullingTest {
        CULLING_FRONT = 0, CULLING_BACK,
        CULLING_COUNT
    };
    enum CullingConvention {
        CULLING_CONV_CLOCKWISE = 0, CULLING_CONV_COUNTER_CLOCKWISE,
        CULLING_CONV_COUNT
    };
#endif

#ifndef SCARAB_ENGINE_RENDERING_STATES_DEPTHSTATE_H
    enum DepthTestMode {
        DEPTH_NEVER = 0, DEPTH_ALLWAYS, DEPTH_EQUAL, DEPTH_NOTEQUAL,
        DEPTH_LESSER, DEPTH_LESSER_EQUAL, DEPTH_GREATER, DEPTH_GREATER_EQUAL,
        DEPTH_COUNT
    };
#endif

#ifndef SCARAB_ENGINE_RENDERING_STATES_STENCILSTATE_H
    enum StencilTestMode {
        STENCIL_TEST_NEVER = 0, STENCIL_TEST_ALLWAYS, STENCIL_TEST_EQUAL, STENCIL_TEST_NOTEQUAL,
        STENCIL_TEST_LESSER, STENCIL_TEST_LESSER_EQUAL, STENCIL_TEST_GREATER, STENCIL_TEST_GREATER_EQUAL,
        STENCIL_TEST_COUNT
    };
    enum StencilOperation {
        STENCIL_OP_KEEP = 0, STENCIL_OP_ZERO, STENCIL_OP_REPLACE,
        STENCIL_OP_INCREMENT, STENCIL_OP_DECREMENT, STENCIL_OP_INVERT,
        STENCIL_OP_COUNT,
    };
#endif

#ifndef SCARAB_ENGINE_RENDERING_STATES_BLENDINGSTATE_H
    enum BlendingSrcMode {
        BLENDING_SRC_ZERO = 0, BLENDING_SRC_ONE, BLENDING_SRC_DST_COLOR, BLENDING_SRC_DST_COLOR_INVERT,
        BLENDING_SRC_SRC_ALPHA, BLENDING_SRC_SRC_ALPHA_INVERT, BLENDING_SRC_SRC_ALPHA_SATURATE,
        BLENDING_SRC_DST_ALPHA, BLENDING_SRC_DST_ALPHA_INVERT,
        BLENDING_SRC_CONST_COLOR, BLENDING_SRC_CONST_COLOR_INVERT,
        BLENDING_SRC_CONST_ALPHA, BLENDING_SRC_CONST_ALPHA_INVERT,
        BLENDING_SRC_COUNT
    };
    enum BlendingDstMode {
        BLENDING_DST_ZERO = 0, BLENDING_DST_ONE, BLENDING_DST_SRC_COLOR, BLENDING_DST_SRC_COLOR_INVERT,
        BLENDING_DST_SRC_ALPHA, BLENDING_DST_SRC_ALPHA_INVERT,
        BLENDING_DST_DST_ALPHA, BLENDING_DST_DST_ALPHA_INVERT,
        BLENDING_DST_CONST_COLOR, BLENDING_DST_CONST_COLOR_INVERT,
        BLENDING_DST_CONST_ALPHA, BLENDING_DST_CONST_ALPHA_INVERT,
        BLENDING_DST_COUNT
    };
    enum BlendingTestMode {
        BLENDING_TEST_NEVER = 0, BLENDING_TEST_ALLWAYS, BLENDING_TEST_EQUAL, BLENDING_TEST_NOTEQUAL,
        BLENDING_TEST_LESSER, BLENDING_TEST_LESSER_EQUAL, BLENDING_TEST_GREATER, BLENDING_TEST_GREATER_EQUAL,
        BLENDING_TEST_COUNT
    };
#endif

#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_MESH_H
    enum MeshType {
        MESH_POINT_LIST = 0,
        MESH_LINE_LIST, MESH_LINE_STRIP, MESH_LINE_FAN,
        MESH_TRIANGLE_LIST, MESH_TRIANGLE_STRIP, MESH_TRIANGLE_FAN,
        MESH_COUNT
    };
#endif

//#ifndef _FONTDESCRIPTOR_DEFINED
//#define _FONTDESCRIPTOR_DEFINED
//    typedef struct _font_descriptor {
//        UInt iFirstChar, iLastChar;
//        Byte * pGlyphMap;
//        UInt iGlyphStride, iGlyphPitch;
//        UInt iCellSize, iCellAscent, iCellDescent, iCellLeading;
//        Int * arrBlackBoxes;
//        UInt iCharMaxWidth, iCharMaxHeight;
//    } FontDescriptor;
//#endif // _FONTDESCRIPTOR_DEFINED

extern D3DDECLTYPE Direct3D9AttributeType[VFTYPE_COUNT];
extern D3DDECLUSAGE Direct3D9AttributeUsage[VFUSAGE_COUNT];
extern DWord Direct3D9VertexBufferUsage[VB_USAGE_COUNT];
extern DWord Direct3D9VertexBufferLock[VB_LOCK_COUNT];
extern DWord Direct3D9IndexBufferUsage[IB_USAGE_COUNT];
extern DWord Direct3D9IndexBufferLock[IB_LOCK_COUNT];
extern D3DFORMAT Direct3D9TextureFormat[TEXTURE_FMT_COUNT];
extern DWord Direct3D9TextureUsage[TEXTURE_USAGE_COUNT];
extern DWord Direct3D9TextureLock[TEXTURE_LOCK_COUNT];
extern D3DCUBEMAP_FACES Direct3D9TextureCubeFace[TEXTURECUBE_FACE_COUNT];
extern DWord Direct3D9MinFilter[SHADERSAMPLING_FILTER_COUNT];
extern DWord Direct3D9MipFilter[SHADERSAMPLING_FILTER_COUNT];
extern DWord Direct3D9WrapMode[SHADERSAMPLING_WRAP_COUNT];
extern DWord Direct3D9CullOrder[CULLING_CONV_COUNT];
extern DWord Direct3D9DepthTest[DEPTH_COUNT];
extern DWord Direct3D9StencilTest[STENCIL_TEST_COUNT];
extern DWord Direct3D9StencilOp[STENCIL_OP_COUNT];
extern DWord Direct3D9BlendSource[BLENDING_SRC_COUNT];
extern DWord Direct3D9BlendDestination[BLENDING_DST_COUNT];
extern DWord Direct3D9BlendTest[BLENDING_TEST_COUNT];
extern D3DPRIMITIVETYPE Direct3D9PrimitiveType[MESH_COUNT];

// Helpers
TextureFormat Direct3D9ConvertFormat( D3DFORMAT d3dFormat );
UInt Direct3D9FormatBits( D3DFORMAT d3dFormat );

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Mappings.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_MAPPINGS_H

