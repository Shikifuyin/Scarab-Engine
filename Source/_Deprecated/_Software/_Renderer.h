/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Renderer.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The full-software 3D renderer ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Raw compiling and RenderTargets not yet implemented
//              Need improvements with texture & sampling data management ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef KOALA_ENGINE_GRAPHICS_3D_RENDERER_H
#define KOALA_ENGINE_GRAPHICS_3D_RENDERER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Core/Object/Patterns/Manager.h"

#include "../../../Lib/Core/Datastruct/Array/LeftyArray.h"
#include "../../../Lib/Core/Datastruct/Table/HashMap.h"

#include "../../../Lib/Platform/System/SystemWindow.h"

#include "../../../Lib/Core/Math/Geometry/Primitives/3D/Plane.h"

#include "../2D/PixelMap.h"

#include "Buffering.h"
#include "RenderStates.h"

#include "Texture.h"
#include "Sampler.h"
#include "Shader.h"
#include "Material.h"
#include "Light.h"

#include "Transformer.h"
#include "Clipper.h"
#include "Raster.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "ShaderLibrary/ShaderLibrary.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define RenderFn Renderer::GetInstancePtr()

    // Global renderer limits
#define RENDERER_MAX_CFL 256
#define RENDERER_MEMORY_SELF    16777216 // 16 Mb
#define RENDERER_MEMORY_BUFFER  33554432 // 32 Mb
#define RENDERER_MEMORY_TEXTURE 16777216 // 16 Mb
#define RENDERER_MEMORY_SAMPLER 16777216 // 16 Mb
#define RENDERER_MEMORY_SHADER  8388608  // 8 Mb
#define RENDERER_MAX_MATERIALS  4096
#define RENDERER_MAX_LIGHTS     1024

#define RENDERER_TEXTURE_CHANNELS 8
#define RENDERER_MATERIAL_CHANNELS 8
#define RENDERER_LIGHT_CHANNELS 8

typedef struct _renderer_creation_parameters
{
    FrameBufferDepthType iFrameDepthType;
    FrameBufferStencilType iFrameStencilType;
    Color4 colClearColor;
    Scalar fClearDepth;
    UInt iClearStencil;
    Vertex3 vViewEye;
    Vector3 vViewDir;
    Vector3 vViewUp;
    Scalar fFrustrumFOV, fFrustrumAspect;
    Scalar fFrustrumNear, fFrustrumFar;
    UInt iVPLeft, iVPRight;
    UInt iVPTop, iVPBottom;
    Scalar fDepthMin, fDepthMax;
    VShaderLibID idDefaultVS;
    PShaderLibID idDefaultPS;
} RendererCreationParameters;

typedef HashMap<const KChar*, Scalar*> ShaderUserConstantsMap;

/////////////////////////////////////////////////////////////////////////////////
// The Renderer class
class Renderer : public Manager<Renderer>
{
public:
    inline static const KChar * GetName();

protected:
    friend class Manager<Renderer>;
    Renderer();
    ~Renderer();

public:
    Void Initialize( const RendererCreationParameters & creationParams );
    Void Cleanup();

    // Getters
    inline PixelMap * GetRenderTarget() const;
    inline PixelMap * GetWindow() const;
    inline UInt WinWidth() const;
    inline UInt WinHeight() const;

    // Render states
    inline RenderState * GetState( RenderStateType iType );
    //inline Void SetRenderTarget( ... );

    // CFLs
    inline CustomFragmentLayout * CreateCFL();
    inline Void DestroyCFL( CustomFragmentLayout * pCFL );

    // Buffers
    inline VertexBuffer * CreateVertexBuffer( const CustomFragmentLayout * pCFL, UInt iCapacity );
    inline Void DestroyVertexBuffer( VertexBuffer * pVB );
    inline IndexBuffer * CreateIndexBuffer( IndexBufferMode iMode, UInt iCapacity );
    inline Void DestroyIndexBuffer( IndexBuffer * pIB );

    // Textures
    inline Texture * CreateTexture1D( const KChar * strName, TextureFormat iFormat, UInt iWidth );
    inline Texture * CreateTexture2D( const KChar * strName, TextureFormat iFormat, UInt iWidth, UInt iHeight );
    inline Texture * CreateTexture3D( const KChar * strName, TextureFormat iFormat, UInt iWidth, UInt iHeight, UInt iDepth );
    inline Void DestroyTexture( Texture * pTexture );
    Void RegisterTexture( Texture * pTexture, UInt iChannel );

    // Materials
    inline Material * CreateMaterial();
    inline Void DestroyMaterial( Material * pMaterial );
    Void RegisterMaterial( Material * pMaterial, UInt iChannel );

    // Lights
    inline Light * CreateLight( LightType iType );
    inline Void DestroyLight( Light * pLight );
    Void RegisterLight( Light * pLight, UInt iChannel );

    // Shaders
    inline UInt RegisterVertexShader( VertexShaderProgram * pProgram );
    inline UInt RegisterPixelShader( PixelShaderProgram * pProgram );
    inline Void DestroyVertexShaders();
    inline Void DestroyPixelShaders();
    inline Void SelectVertexShader(UInt iProgID);
    inline Void SelectPixelShader(UInt iProgID);
    inline VertexShaderProgram * GetVertexShader( UInt iProgID = INVALID_OFFSET ) const;
    inline PixelShaderProgram * GetPixelShader( UInt iProgID = INVALID_OFFSET ) const;

    inline Void RegisterShaderConstant( const KChar * strName, Scalar * pDataSource );

    // Transformations
    	// World stack
	Void Reset();
	Void PushMatrix();
	Void PopMatrix();
	Void Identity();
	Void Translate( const Vector3 & vTranslate );
	Void Rotate( const Vector3 & vRotate );
	Void Scale( const Vector3 & vScale );
	Void Transform( const Matrix4 & matTransform );
        // View
    inline Void SetView( const Vertex3 & vEye, const Vector3 & vDir, const Vector3 & vUp );
    inline Void GetBillboardMatrix( Matrix4 & outBillboardMatrix ) const;
        // Projection
    inline Void SetFrustrum( Scalar fFOV, Scalar fAspect, Scalar fNear, Scalar fFar );
        // Viewport
    inline Void SetViewport( UInt iLeft, UInt iRight, UInt iBottom, UInt iTop );
        // Depth Range
    inline Void SetDepthRange( Scalar fMinZ, Scalar fMaxZ );

    // 3D Rendering
    inline Void SetClearColor( const Color4 & clearColor );
    inline Void SetClearDepth( Scalar clearDepth );
    inline Void SetClearStencil( UInt clearStencil );
    inline Void ClearFrame();
    inline Void ClearFrame( Bool bClearColors, Bool bClearDepth, Bool bClearStencil );
    Void RenderStream( const VertexBuffer * pVertices, const IndexBuffer * pIndices );
    Void RawStart( IndexBufferMode iMode, Bool bCompile = false );
	Void RawVertex( CFLPtr pVertex );
    Void RawFinish();
    inline Void Flip();

private:
    // Delegates /////////////////////////////////////////////////////////////////////////
    friend class Transformer;
    friend class Clipper;
    friend class Raster;
    friend class ShaderProgram;
    friend class VertexShader;
    friend class PixelShader;

    // Helpers ///////////////////////////////////////////////////////////////////////////
    inline StateTransform * _GetStateTransform() const;
    inline StateCulling * _GetStateCulling() const;
    inline StateClipping * _GetStateClipping() const;
    inline StateDepth * _GetStateDepth() const;
    inline StateStencil * _GetStateStencil() const;
    inline StateBlending * _GetStateBlending() const;

    Void _OnViewChange();
    Void _OnFrustrumChange();
    Void _OnViewportChange();
    Void _OnDepthRangeChange();

    Void _OnSelectVS();
    Void _OnSelectPS();

    inline LibVertexShaderProgram * _GetDefaultVS() const;
    inline LibPixelShaderProgram * _GetDefaultPS() const;

    Void _ClippingOutput_Vertex( CFLPtr pVertexA );
    Void _ClippingOutput_Segment( CFLPtr pVertexA, CFLPtr pVertexB );
    Void _ClippingOutput_Triangle( CFLPtr pVertexA, CFLPtr pVertexB, CFLPtr pVertexC );

    Void _VertexShaderOutput_Vertex( CFLPtr pVertexA );

    Void _VertexToPixel( CFLPtr pPixel, CCFLPtr pVertex ) const;

    Void _Draw_Vertex( CCFLPtr pVertexA );
    Void _Draw_Segment( CCFLPtr pVertexA, CCFLPtr pVertexB );
    Void _Draw_Triangle( CCFLPtr pVertexA, CCFLPtr pVertexB, CCFLPtr pVertexC );

    Void _GetShaderConstant( Scalar * outValues, RenderingConstantType iConstantType ) const;
    inline Void _GetShaderUserConstant( Scalar * outValues, UInt iSize, const KChar * strName ) const;
    inline Texture * _GetTexture( UInt iChannel ) const;
    inline Sampler * _GetSampler( UInt iChannel ) const;

    // Renderer Data /////////////////////////////////////////////////////////////////////
    Bool m_bInitialized;

        // Memory model
    MemoryAllocatorID m_idAllocStack;
    MemoryAllocatorID m_idAllocHeap;
    MemoryAllocatorID m_idAllocCFLPool;
    MemoryAllocatorID m_idAllocBufferHeap;
    MemoryAllocatorID m_idAllocTextureHeap;
    MemoryAllocatorID m_idAllocSamplerHeap;
    MemoryAllocatorID m_idAllocShaderHeap;
    MemoryAllocatorID m_idAllocMaterialPool;
    MemoryAllocatorID m_idAllocLightPool;

        // Screen, RenderTarget & FrameBuffer
    PixelMap  * m_pWindow, * m_pRenderTarget;
    UInt m_iWinWidth, m_iWinHeight;
    FrameBuffer * m_pFrame;

        // States
    RenderState * m_arrStates[RENDERSTATE_COUNT];
    CustomFragmentLayout * m_pVSInput;
    CustomFragmentLayout * m_pVSOutput;
    CustomFragmentLayout * m_pPSInput;
    Bool m_bUpdateClippingPlanes;

        // Transformations
    ArrayL<Matrix4> * m_pWorldStack;
    Matrix4 m_matTrWorld;
    Matrix4 m_matInvWorld;
	Matrix4 m_matView;
	Matrix4 m_matTrView;
	Matrix4 m_matInvView;
	Matrix4 m_matProjection;
	Matrix4 m_matTrProjection;
	Matrix4 m_matInvProjection;
    Vector3 m_vViewPortFactor;
    Vector3 m_vViewPortOffset;
    Transformer * m_pCamera;
    Transformer * m_pProjector;

        // Pipeline
    VertexArray * m_pClippedVertices;
    VertexArray * m_pShadedVertices;
    Clipper * m_pClipper;
    Raster * m_pRaster;
    VertexShader * m_pVertexShader;
    PixelShader * m_pPixelShader;
    VShaderLibID m_idDefaultVS;
    PShaderLibID m_idDefaultPS;

        // Raw Rendering State
    Bool m_bRawRendering;
    Bool m_bRawCompiling;
    IndexBufferMode m_iRawMode;
    CFLPtr m_pRawVertices[4];
    UInt m_iRawCurVertex;
    UInt m_iRawBias; // Triangle & Quad strips need biasing

        // Resources
    Texture * m_arrTextureChannels[RENDERER_TEXTURE_CHANNELS];
    Sampler * m_arrTextureSamplersChannels[RENDERER_TEXTURE_CHANNELS];
    Material * m_arrMaterialChannels[RENDERER_MATERIAL_CHANNELS];
    Light * m_arrLights[RENDERER_LIGHT_CHANNELS];
    ShaderUserConstantsMap * m_phmShadersUserConstants;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Renderer.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_RENDERER_H


