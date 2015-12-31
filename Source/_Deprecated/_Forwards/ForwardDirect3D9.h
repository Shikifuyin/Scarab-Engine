/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/Forwards/ForwardDirect3D9.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Forward : <d3d9.h>, <d3dx9.h>
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
#ifndef SCARAB_THIRDPARTY_FORWARDS_FORWARDDIRECT3D9_H
#define SCARAB_THIRDPARTY_FORWARDS_FORWARDDIRECT3D9_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ForwardWin32.h"

/////////////////////////////////////////////////////////////////////////////////
#ifndef _D3D9_H_
/////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 4)

/////////////////////////////////////////////////////////////////////////////////
// Required definitions
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
    ( (unsigned long)(unsigned char)(ch0) | \
     ((unsigned long)(unsigned char)(ch1) << 8) | \
     ((unsigned long)(unsigned char)(ch2) << 16) | \
     ((unsigned long)(unsigned char)(ch3) << 24) )
#endif // MAKEFOURCC

#ifndef _LARGE_INTEGER
    typedef union _LARGE_INTEGER {
        struct {
            unsigned long LowPart;
            long HighPart;
        };
        struct {
            unsigned long LowPart;
            long HighPart;
        } u;
        __int64 QuadPart;
    } LARGE_INTEGER, *PLARGE_INTEGER;
#endif // _LARGE_INTEGER

#ifndef _GUID
    typedef struct _GUID {
        unsigned long  Data1;
        unsigned short Data2;
        unsigned short Data3;
        unsigned char  Data4[8];
    } GUID;
#endif // _GUID

typedef struct _D3DVSHADERCAPS2_0
{
    unsigned long Caps;
    int DynamicFlowControlDepth;
    int NumTemps;
    int StaticFlowControlDepth;
} D3DVSHADERCAPS2_0;

typedef struct _D3DPSHADERCAPS2_0
{
    unsigned long Caps;
    int DynamicFlowControlDepth;
    int NumTemps;
    int StaticFlowControlDepth;
    int NumInstructionSlots;
} D3DPSHADERCAPS2_0;

/////////////////////////////////////////////////////////////////////////////////
// Resource management
typedef enum _D3DPOOL {
    D3DPOOL_DEFAULT     = 0,
    D3DPOOL_MANAGED     = 1,
    D3DPOOL_SYSTEMMEM   = 2,
    D3DPOOL_SCRATCH     = 3,

    D3DPOOL_FORCE_DWORD = 0x7fffffff
} D3DPOOL;

typedef enum _D3DRESOURCETYPE {
    D3DRTYPE_SURFACE       = 1,
    D3DRTYPE_VOLUME        = 2,
    D3DRTYPE_TEXTURE       = 3,
    D3DRTYPE_VOLUMETEXTURE = 4,
    D3DRTYPE_CUBETEXTURE   = 5,
    D3DRTYPE_VERTEXBUFFER  = 6,
    D3DRTYPE_INDEXBUFFER   = 7,

    D3DRTYPE_FORCE_DWORD   = 0x7fffffff
} D3DRESOURCETYPE;

/////////////////////////////////////////////////////////////////////////////////
// Texture formats definitions
typedef enum _D3DFORMAT
{
    D3DFMT_UNKNOWN             =  0,

    D3DFMT_R8G8B8              = 20,
    D3DFMT_A8R8G8B8            = 21,
    D3DFMT_X8R8G8B8            = 22,
    D3DFMT_R5G6B5              = 23,
    D3DFMT_X1R5G5B5            = 24,
    D3DFMT_A1R5G5B5            = 25,
    D3DFMT_A4R4G4B4            = 26,
    D3DFMT_R3G3B2              = 27,
    D3DFMT_A8                  = 28,
    D3DFMT_A8R3G3B2            = 29,
    D3DFMT_X4R4G4B4            = 30,
    D3DFMT_A2B10G10R10         = 31,
    D3DFMT_A8B8G8R8            = 32,
    D3DFMT_X8B8G8R8            = 33,
    D3DFMT_G16R16              = 34,
    D3DFMT_A2R10G10B10         = 35,
    D3DFMT_A16B16G16R16        = 36,

    D3DFMT_A8P8                = 40,
    D3DFMT_P8                  = 41,

    D3DFMT_L8                  = 50,
    D3DFMT_A8L8                = 51,
    D3DFMT_A4L4                = 52,

    D3DFMT_V8U8                = 60,
    D3DFMT_L6V5U5              = 61,
    D3DFMT_X8L8V8U8            = 62,
    D3DFMT_Q8W8V8U8            = 63,
    D3DFMT_V16U16              = 64,
    D3DFMT_A2W10V10U10         = 67,

    D3DFMT_UYVY                = MAKEFOURCC('U', 'Y', 'V', 'Y'),
    D3DFMT_R8G8_B8G8           = MAKEFOURCC('R', 'G', 'B', 'G'),
    D3DFMT_YUY2                = MAKEFOURCC('Y', 'U', 'Y', '2'),
    D3DFMT_G8R8_G8B8           = MAKEFOURCC('G', 'R', 'G', 'B'),
    D3DFMT_DXT1                = MAKEFOURCC('D', 'X', 'T', '1'),
    D3DFMT_DXT2                = MAKEFOURCC('D', 'X', 'T', '2'),
    D3DFMT_DXT3                = MAKEFOURCC('D', 'X', 'T', '3'),
    D3DFMT_DXT4                = MAKEFOURCC('D', 'X', 'T', '4'),
    D3DFMT_DXT5                = MAKEFOURCC('D', 'X', 'T', '5'),

    D3DFMT_D16_LOCKABLE        = 70,
    D3DFMT_D32                 = 71,
    D3DFMT_D15S1               = 73,
    D3DFMT_D24S8               = 75,
    D3DFMT_D24X8               = 77,
    D3DFMT_D24X4S4             = 79,
    D3DFMT_D16                 = 80,
    D3DFMT_L16                 = 81,

    D3DFMT_D32F_LOCKABLE       = 82,
    D3DFMT_D24FS8              = 83,

#ifndef D3D_DISABLE_9EX
    D3DFMT_D32_LOCKABLE        = 84,
    D3DFMT_S8_LOCKABLE         = 85,
#endif // D3D_DISABLE_9EX

    D3DFMT_VERTEXDATA          = 100,
    D3DFMT_INDEX16             = 101,
    D3DFMT_INDEX32             = 102,

    D3DFMT_Q16W16V16U16        = 110,

    D3DFMT_MULTI2_ARGB8        = MAKEFOURCC('M','E','T','1'),

    D3DFMT_R16F                = 111,
    D3DFMT_G16R16F             = 112,
    D3DFMT_A16B16G16R16F       = 113,

    D3DFMT_R32F                = 114,
    D3DFMT_G32R32F             = 115,
    D3DFMT_A32B32G32R32F       = 116,

    D3DFMT_CxV8U8              = 117,

#ifndef D3D_DISABLE_9EX
    D3DFMT_A1                  = 118,
    D3DFMT_A2B10G10R10_XR_BIAS = 119,
    D3DFMT_BINARYBUFFER        = 199,
#endif // D3D_DISABLE_9EX

    D3DFMT_FORCE_DWORD         = 0x7fffffff
} D3DFORMAT;

typedef unsigned long D3DCOLOR;

typedef enum _D3DCUBEMAP_FACES
{
    D3DCUBEMAP_FACE_POSITIVE_X  = 0,
    D3DCUBEMAP_FACE_NEGATIVE_X  = 1,
    D3DCUBEMAP_FACE_POSITIVE_Y  = 2,
    D3DCUBEMAP_FACE_NEGATIVE_Y  = 3,
    D3DCUBEMAP_FACE_POSITIVE_Z  = 4,
    D3DCUBEMAP_FACE_NEGATIVE_Z  = 5,

    D3DCUBEMAP_FACE_FORCE_DWORD = 0x7fffffff
} D3DCUBEMAP_FACES;

/////////////////////////////////////////////////////////////////////////////////
// Device definitions
typedef enum _D3DDEVTYPE
{
    D3DDEVTYPE_HAL         = 1,
    D3DDEVTYPE_REF         = 2,
    D3DDEVTYPE_SW          = 3,
    D3DDEVTYPE_NULLREF     = 4,

    D3DDEVTYPE_FORCE_DWORD = 0x7fffffff
} D3DDEVTYPE;

typedef enum _D3DMULTISAMPLE_TYPE
{
    D3DMULTISAMPLE_NONE        =  0,
    D3DMULTISAMPLE_NONMASKABLE =  1,
    D3DMULTISAMPLE_2_SAMPLES   =  2,
    D3DMULTISAMPLE_3_SAMPLES   =  3,
    D3DMULTISAMPLE_4_SAMPLES   =  4,
    D3DMULTISAMPLE_5_SAMPLES   =  5,
    D3DMULTISAMPLE_6_SAMPLES   =  6,
    D3DMULTISAMPLE_7_SAMPLES   =  7,
    D3DMULTISAMPLE_8_SAMPLES   =  8,
    D3DMULTISAMPLE_9_SAMPLES   =  9,
    D3DMULTISAMPLE_10_SAMPLES  = 10,
    D3DMULTISAMPLE_11_SAMPLES  = 11,
    D3DMULTISAMPLE_12_SAMPLES  = 12,
    D3DMULTISAMPLE_13_SAMPLES  = 13,
    D3DMULTISAMPLE_14_SAMPLES  = 14,
    D3DMULTISAMPLE_15_SAMPLES  = 15,
    D3DMULTISAMPLE_16_SAMPLES  = 16,

    D3DMULTISAMPLE_FORCE_DWORD = 0x7fffffff
} D3DMULTISAMPLE_TYPE;

typedef enum _D3DSWAPEFFECT
{
    D3DSWAPEFFECT_DISCARD     = 1,
    D3DSWAPEFFECT_FLIP        = 2,
    D3DSWAPEFFECT_COPY        = 3,

#ifndef D3D_DISABLE_9EX
    D3DSWAPEFFECT_OVERLAY     = 4,
    D3DSWAPEFFECT_FLIPEX      = 5,
#endif // D3D_DISABLE_9EX

    D3DSWAPEFFECT_FORCE_DWORD = 0x7fffffff
} D3DSWAPEFFECT;

typedef enum _D3DPRIMITIVETYPE
{
    D3DPT_POINTLIST     = 1,
    D3DPT_LINELIST      = 2,
    D3DPT_LINESTRIP     = 3,
    D3DPT_TRIANGLELIST  = 4,
    D3DPT_TRIANGLESTRIP = 5,
    D3DPT_TRIANGLEFAN   = 6,

    D3DPT_FORCE_DWORD   = 0x7fffffff
} D3DPRIMITIVETYPE;

#define MAX_DEVICE_IDENTIFIER_STRING 512
typedef struct _D3DADAPTER_IDENTIFIER9
{
    char Driver[MAX_DEVICE_IDENTIFIER_STRING];
    char Description[MAX_DEVICE_IDENTIFIER_STRING];
    char DeviceName[32];

#ifdef _WIN32
    LARGE_INTEGER DriverVersion;
#else
    unsigned long DriverVersionLowPart;
    unsigned long DriverVersionHighPart;
#endif

    unsigned long VendorId;
    unsigned long DeviceId;
    unsigned long SubSysId;
    unsigned long Revision;

    GUID DeviceIdentifier;

    unsigned long WHQLLevel;
} D3DADAPTER_IDENTIFIER9;

typedef struct _D3DPRESENT_PARAMETERS_
{
    unsigned int BackBufferWidth;
    unsigned int BackBufferHeight;
    D3DFORMAT BackBufferFormat;
    unsigned int BackBufferCount;

    D3DMULTISAMPLE_TYPE MultiSampleType;
    unsigned long MultiSampleQuality;

    D3DSWAPEFFECT SwapEffect;
    HWND hDeviceWindow;
    int Windowed;
    int EnableAutoDepthStencil;
    D3DFORMAT AutoDepthStencilFormat;
    unsigned long Flags;

    unsigned int FullScreen_RefreshRateInHz;
    unsigned int PresentationInterval;
} D3DPRESENT_PARAMETERS;

typedef struct _D3DDISPLAYMODE
{
    unsigned int Width;
    unsigned int Height;
    unsigned int RefreshRate;
    D3DFORMAT Format;
} D3DDISPLAYMODE;

typedef struct _D3DSURFACE_DESC
{
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    unsigned long Usage;
    D3DPOOL Pool;

    D3DMULTISAMPLE_TYPE MultiSampleType;
    unsigned long MultiSampleQuality;
    unsigned int Width;
    unsigned int Height;
} D3DSURFACE_DESC;

typedef struct _D3DVIEWPORT9 {
    unsigned long X;
    unsigned long Y;
    unsigned long Width;
    unsigned long Height;
    float MinZ;
    float MaxZ;
} D3DVIEWPORT9;

typedef struct _D3DCAPS9
{
    D3DDEVTYPE DeviceType;
    unsigned int AdapterOrdinal;

    unsigned long Caps;
    unsigned long Caps2;
    unsigned long Caps3;
    unsigned long PresentationIntervals;

    unsigned long CursorCaps;

    unsigned long DevCaps;

    unsigned long PrimitiveMiscCaps;
    unsigned long RasterCaps;
    unsigned long ZCmpCaps;
    unsigned long SrcBlendCaps;
    unsigned long DestBlendCaps;
    unsigned long AlphaCmpCaps;
    unsigned long ShadeCaps;
    unsigned long TextureCaps;
    unsigned long TextureFilterCaps;
    unsigned long CubeTextureFilterCaps;
    unsigned long VolumeTextureFilterCaps;
    unsigned long TextureAddressCaps;
    unsigned long VolumeTextureAddressCaps;

    unsigned long LineCaps;

    unsigned long MaxTextureWidth, MaxTextureHeight;
    unsigned long MaxVolumeExtent;

    unsigned long MaxTextureRepeat;
    unsigned long MaxTextureAspectRatio;
    unsigned long MaxAnisotropy;
    float MaxVertexW;

    float GuardBandLeft;
    float GuardBandTop;
    float GuardBandRight;
    float GuardBandBottom;

    float ExtentsAdjust;
    unsigned long StencilCaps;

    unsigned long FVFCaps;
    unsigned long TextureOpCaps;
    unsigned long MaxTextureBlendStages;
    unsigned long MaxSimultaneousTextures;

    unsigned long VertexProcessingCaps;
    unsigned long MaxActiveLights;
    unsigned long MaxUserClipPlanes;
    unsigned long MaxVertexBlendMatrices;
    unsigned long MaxVertexBlendMatrixIndex;

    float MaxPointSize;

    unsigned long MaxPrimitiveCount;
    unsigned long MaxVertexIndex;
    unsigned long MaxStreams;
    unsigned long MaxStreamStride;

    unsigned long VertexShaderVersion;
    unsigned long MaxVertexShaderConst;

    unsigned long PixelShaderVersion;
    float PixelShader1xMaxValue;

    unsigned long DevCaps2;

    float MaxNpatchTessellationLevel;
    unsigned long Reserved5;

    unsigned int MasterAdapterOrdinal;
    unsigned int AdapterOrdinalInGroup;
    unsigned int NumberOfAdaptersInGroup;
    unsigned long DeclTypes;
    unsigned long NumSimultaneousRTs;
    unsigned long StretchRectFilterCaps;
    D3DVSHADERCAPS2_0 VS20Caps;
    D3DPSHADERCAPS2_0 PS20Caps;
    unsigned long VertexTextureFilterCaps;
    unsigned long MaxVShaderInstructionsExecuted;
    unsigned long MaxPShaderInstructionsExecuted;
    unsigned long MaxVertexShader30InstructionSlots;
    unsigned long MaxPixelShader30InstructionSlots;
} D3DCAPS9;

#define D3DDMAPSAMPLER 256
#define D3DVERTEXTEXTURESAMPLER0 (D3DDMAPSAMPLER+1)
#define D3DVERTEXTEXTURESAMPLER1 (D3DDMAPSAMPLER+2)
#define D3DVERTEXTEXTURESAMPLER2 (D3DDMAPSAMPLER+3)
#define D3DVERTEXTEXTURESAMPLER3 (D3DDMAPSAMPLER+4)

/////////////////////////////////////////////////////////////////////////////////
// Vertex declaration definitions
typedef enum _D3DDECLTYPE
{
    D3DDECLTYPE_FLOAT1    =  0,
    D3DDECLTYPE_FLOAT2    =  1,
    D3DDECLTYPE_FLOAT3    =  2,
    D3DDECLTYPE_FLOAT4    =  3,
    D3DDECLTYPE_D3DCOLOR  =  4,
    D3DDECLTYPE_UBYTE4    =  5,
    D3DDECLTYPE_SHORT2    =  6,
    D3DDECLTYPE_SHORT4    =  7,

    // Shader >= 2.0
    D3DDECLTYPE_UBYTE4N   =  8,
    D3DDECLTYPE_SHORT2N   =  9,
    D3DDECLTYPE_SHORT4N   = 10,
    D3DDECLTYPE_USHORT2N  = 11,
    D3DDECLTYPE_USHORT4N  = 12,
    D3DDECLTYPE_UDEC3     = 13,
    D3DDECLTYPE_DEC3N     = 14,
    D3DDECLTYPE_FLOAT16_2 = 15,
    D3DDECLTYPE_FLOAT16_4 = 16,

    D3DDECLTYPE_UNUSED    = 17
} D3DDECLTYPE;
#define MAXD3DDECLTYPE D3DDECLTYPE_UNUSED

typedef enum _D3DDECLUSAGE
{
    D3DDECLUSAGE_POSITION     = 0,
    D3DDECLUSAGE_BLENDWEIGHT  = 1,
    D3DDECLUSAGE_BLENDINDICES = 2,
    D3DDECLUSAGE_NORMAL       = 3,
    D3DDECLUSAGE_PSIZE        = 4,
    D3DDECLUSAGE_TEXCOORD     = 5,
    D3DDECLUSAGE_TANGENT      = 6,
    D3DDECLUSAGE_BINORMAL     = 7,
    D3DDECLUSAGE_TESSFACTOR   = 8,
    D3DDECLUSAGE_POSITIONT    = 9,
    D3DDECLUSAGE_COLOR        = 10,
    D3DDECLUSAGE_FOG          = 11,
    D3DDECLUSAGE_DEPTH        = 12,
    D3DDECLUSAGE_SAMPLE       = 13
} D3DDECLUSAGE;
#define MAXD3DDECLUSAGE D3DDECLUSAGE_SAMPLE

typedef struct _D3DVERTEXELEMENT9
{
    unsigned short Stream;
    unsigned short Offset;
    unsigned char  Type;
    unsigned char  Method;
    unsigned char  Usage;
    unsigned char  UsageIndex;
} D3DVERTEXELEMENT9, *LPD3DVERTEXELEMENT9;
#define D3DDECL_END() { 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 }

/////////////////////////////////////////////////////////////////////////////////
// Interfaces
struct IDirect3D9;
typedef struct IDirect3D9 *PDIRECT3D9, *LPDIRECT3D9;
struct IDirect3DDevice9;
typedef struct IDirect3DDevice9 *PDIRECT3DDEVICE9, *LPDIRECT3DDEVICE9;

struct IDirect3DVertexDeclaration9;
typedef struct IDirect3DVertexDeclaration9 *PDIRECT3DVERTEXDECLARATION9, *LPDIRECT3DVERTEXDECLARATION9;

struct IDirect3DVertexBuffer9;
typedef struct IDirect3DVertexBuffer9 *PDIRECT3DVERTEXBUFFER9, *LPDIRECT3DVERTEXBUFFER9;
struct IDirect3DIndexBuffer9;
typedef struct IDirect3DIndexBuffer9 *PDIRECT3DINDEXBUFFER9, *LPDIRECT3DINDEXBUFFER9;

struct IDirect3DBaseTexture9;
typedef struct IDirect3DBaseTexture9 *PDIRECT3DBASETEXTURE9, *LPDIRECT3DBASETEXTURE9;
struct IDirect3DTexture9;
typedef struct IDirect3DTexture9 *PDIRECT3DTEXTURE9, *LPDIRECT3DTEXTURE9;
struct IDirect3DVolumeTexture9;
typedef struct IDirect3DVolumeTexture9 *PDIRECT3DVOLUMETEXTURE9, *LPDIRECT3DVOLUMETEXTURE9;
struct IDirect3DCubeTexture9;
typedef struct IDirect3DCubeTexture9 *PDIRECT3DCUBETEXTURE9, *LPDIRECT3DCUBETEXTURE9;

struct IDirect3DSurface9;
typedef struct IDirect3DSurface9 *PDIRECT3DSURFACE9, *LPDIRECT3DSURFACE9;

struct IDirect3DVertexShader9;
typedef struct IDirect3DVertexShader9 *PDIRECT3DVERTEXSHADER9, *LPDIRECT3DVERTEXSHADER9;
struct IDirect3DPixelShader9;
typedef struct IDirect3DPixelShader9 *PDIRECT3DPIXELSHADER9, *LPDIRECT3DPIXELSHADER9;

#pragma pack(pop)
/////////////////////////////////////////////////////////////////////////////////
#endif // _D3D9_H_
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
#ifndef __D3DX9_H__
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Direct3DX9 helpers
struct ID3DXSprite;
struct ID3DXFont;

/////////////////////////////////////////////////////////////////////////////////
#endif // __D3DX9_H__
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ForwardDirect3D9.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_FORWARDS_FORWARDDIRECT3D9_H

