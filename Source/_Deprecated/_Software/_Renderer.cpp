/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Renderer.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The master 3D renderer ...
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
#include "Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// Renderer implementation
Renderer::Renderer():
    m_matTrWorld(), m_matInvWorld(),
    m_matView(), m_matTrView(), m_matInvView(),
    m_matProjection(), m_matTrProjection(), m_matInvProjection(),
    m_vViewPortFactor(), m_vViewPortOffset()
{
    UInt i;

    // Memory Model
    m_idAllocStack = INVALID_OFFSET;
    m_idAllocHeap = INVALID_OFFSET;
    m_idAllocCFLPool = INVALID_OFFSET;
    m_idAllocBufferHeap = INVALID_OFFSET;
    m_idAllocTextureHeap = INVALID_OFFSET;
    m_idAllocSamplerHeap = INVALID_OFFSET;
    m_idAllocShaderHeap = INVALID_OFFSET;
    m_idAllocMaterialPool = INVALID_OFFSET;
    m_idAllocLightPool = INVALID_OFFSET;

    // Screen
    m_pWindow = NULL;
    m_pRenderTarget = NULL;
    m_iWinWidth = 0;
    m_iWinHeight = 0;

    // Frame
    m_pFrame = NULL;

    // States
    for( i = 0; i < RENDERSTATE_COUNT; ++i )
        m_arrStates[i] = NULL;

    // Transformations
    m_pWorldStack = NULL;
    m_pCamera = NULL;
    m_pProjector = NULL;

    // Pipeline
    m_pClippedVertices = NULL;
    m_pShadedVertices = NULL;
    m_pClipper = NULL;
    m_pRaster = NULL;
    m_pVertexShader = NULL;
    m_pPixelShader = NULL;
    m_idDefaultVS = VSHADER_DEFAULT_NULL;
    m_idDefaultPS = PSHADER_DEFAULT_NULL;

    // Raw Rendering
    // nothing to do

    // Resources
    m_phmShadersUserConstants = NULL;

    // Done
    m_bInitialized = false;
}
Renderer::~Renderer()
{
    // Cleanup MUST be called explicitly
    // nothing to do
}

Void Renderer::Initialize( const RendererCreationParameters & creationParams )
{
    Assert(!m_bInitialized);

    UInt i;

    MemoryFn->SelectContext( GetMemoryContext() );

    // Memory Model
    m_idAllocStack = MemoryFn->CreateStack( RENDERER_MEMORY_SELF );
    m_idAllocHeap = MemoryFn->CreateHeap( RENDERER_MEMORY_SELF );
    m_idAllocCFLPool = MemoryFn->CreatePool( sizeof(CustomFragmentLayout), RENDERER_MAX_CFL );
    m_idAllocBufferHeap = MemoryFn->CreateHeap( RENDERER_MEMORY_BUFFER );
    m_idAllocTextureHeap = MemoryFn->CreateHeap( RENDERER_MEMORY_TEXTURE );
    m_idAllocSamplerHeap = MemoryFn->CreateHeap( RENDERER_MEMORY_SAMPLER );
    m_idAllocShaderHeap = MemoryFn->CreateHeap( RENDERER_MEMORY_SHADER );
    m_idAllocMaterialPool = MemoryFn->CreatePool( sizeof(Material), RENDERER_MAX_MATERIALS );
    m_idAllocLightPool = MemoryFn->CreatePool( sizeof(Light), RENDERER_MAX_LIGHTS );

    MemoryFn->SelectAllocator( m_idAllocHeap );

    // Screen
    ScreenLayoutFlags iFlags = SystemWindowFn->ScreenLayout();
    PixelMapLayout iLayout = PIXELMAP_LAYOUT_STANDARD;
    if (iFlags & SCREEN_LITTLE_ENDIAN)
        iLayout |= PIXELMAP_LAYOUT_BYTE_REVERT;
    if (iFlags & SCREEN_SCANLINES_RL)
        iLayout |= PIXELMAP_LAYOUT_X_REVERT;
    if (iFlags & SCREEN_VERTICAL_BT)
        iLayout |= PIXELMAP_LAYOUT_Y_REVERT;
    m_pWindow = PixelMap::Wrap( SystemWindowFn->Width(), SystemWindowFn->Height(),
                                (ColorFormat)( SystemWindowFn->PixelFormat() ),
                                SystemWindowFn->BackBuffer(), iLayout );
    m_iWinWidth = m_pWindow->Width();
    m_iWinHeight = m_pWindow->Height();
    m_pRenderTarget = m_pWindow;

    // Frame
    m_pFrame = KNewIm() FrameBuffer( m_idAllocBufferHeap, m_pRenderTarget->PixelFormat(),
                                     creationParams.iFrameDepthType, creationParams.iFrameStencilType );
    m_pFrame->Allocate( m_iWinWidth, m_iWinHeight, m_pRenderTarget );
    m_pFrame->SetClearColor( creationParams.colClearColor );
    m_pFrame->SetClearDepth( creationParams.fClearDepth );
    m_pFrame->SetClearStencil( creationParams.iClearStencil );

    // States
    m_arrStates[RENDERSTATE_TRANSFORM] = KNewIm() StateTransform();
    m_arrStates[RENDERSTATE_CULLING] = KNewIm() StateCulling();
    m_arrStates[RENDERSTATE_CLIPPING] = KNewIm() StateClipping();
    m_arrStates[RENDERSTATE_DEPTH] = KNewIm() StateDepth();
    m_arrStates[RENDERSTATE_STENCIL] = KNewIm() StateStencil();
    m_arrStates[RENDERSTATE_BLENDING] = KNewIm() StateBlending();
    m_pVSInput = NULL;
    m_pVSOutput = NULL;
    m_pPSInput = NULL;
    m_bUpdateClippingPlanes = false;

    // Transformations
    m_pWorldStack = KNewIm() ArrayL<Matrix4>( INVALID_OFFSET, GetMemoryContext(), m_idAllocHeap );
    m_pWorldStack->Push( Matrix4::Identity );
    m_matTrWorld.MakeIdentity();
    m_matInvWorld.MakeIdentity();
    m_pCamera = KNewIm() Transformer( this );
    m_pProjector = KNewIm() Transformer( NULL );

    // Pipeline
    m_pClippedVertices = KNewIm() VertexArray( CLIPPER_MAX_VERTICES, m_idAllocHeap );
    m_pShadedVertices = KNewIm() VertexArray( CLIPPER_MAX_VERTICES, m_idAllocHeap );
    m_pClipper = KNewIm() Clipper( this );
    m_pRaster = KNewIm() Raster( this );
    m_pVertexShader = KNewIm() VertexShader( this );
    m_pPixelShader = KNewIm() PixelShader( this );
    m_idDefaultVS = creationParams.idDefaultVS;
    m_idDefaultPS = creationParams.idDefaultPS;

    // Raw Rendering
    m_bRawRendering = false;
    m_bRawCompiling = false;
    m_iRawMode = INDEXBUFFER_TRIANGLE_LIST;
    m_pRawVertices[0] = NULL;
    m_pRawVertices[1] = NULL;
    m_pRawVertices[2] = NULL;
    m_pRawVertices[3] = NULL;
    m_iRawCurVertex = 0;
    m_iRawBias = 0;

    // Resources
    for( i = 0; i < RENDERER_TEXTURE_CHANNELS; ++i ) {
        m_arrTextureChannels[i] = NULL;
        m_arrTextureSamplersChannels[i] = NULL;
    }
    for( i = 0; i < RENDERER_MATERIAL_CHANNELS; ++i )
        m_arrMaterialChannels[i] = NULL;
    for( i = 0; i < RENDERER_LIGHT_CHANNELS; ++i )
        m_arrLights[i] = NULL;
    m_phmShadersUserConstants = KNewIm() ShaderUserConstantsMap( HASHMAP_PAGESIZE, GetMemoryContext(), m_idAllocHeap );

    // Done
    m_bInitialized = true;

    // Camera
    m_pCamera->SetView( creationParams.vViewEye, creationParams.vViewDir, creationParams.vViewUp );
    m_pCamera->SetFrustrum( creationParams.fFrustrumFOV, creationParams.fFrustrumAspect,
                            creationParams.fFrustrumNear, creationParams.fFrustrumFar );
    m_pCamera->SetViewport( creationParams.iVPLeft, creationParams.iVPRight,
                            creationParams.iVPBottom, creationParams.iVPTop,
                            m_iWinWidth, m_iWinHeight );
    m_pCamera->SetDepthRange( creationParams.fDepthMin, creationParams.fDepthMax );

    // Shaders
    ShaderLib->Initialize( m_idAllocShaderHeap );
    UInt idVS = m_pVertexShader->RegisterProgram( _GetDefaultVS() );
    UInt idPS = m_pPixelShader->RegisterProgram( _GetDefaultPS() );
    m_pVertexShader->SelectProgram(idVS);
    m_pPixelShader->SelectProgram(idPS);
}
Void Renderer::Cleanup()
{
    Assert(m_bInitialized);

    UInt i;

    MemoryFn->SelectContext( GetMemoryContext() );
    MemoryFn->SelectAllocator( m_idAllocHeap );

    // Shaders
    m_pRaster->SetCFL( NULL );
    m_pPixelShader->DestroyPrograms( false );
    m_pVertexShader->DestroyPrograms( false );
    ShaderLib->Cleanup( m_idAllocShaderHeap );

    // Resources
    m_phmShadersUserConstants->Cleanup();
    KDeleteIm(m_phmShadersUserConstants);
    m_phmShadersUserConstants = NULL;
    for( i = 0; i < RENDERER_TEXTURE_CHANNELS; ++i ) {
        KDeleteExA( m_arrTextureSamplersChannels[i], m_idAllocHeap );
        m_arrTextureSamplersChannels[i] = NULL;
    }

    // Raw Rendering
    // nothing to do

    // Pipeline
    m_idDefaultVS = VSHADER_DEFAULT_NULL;
    m_idDefaultPS = PSHADER_DEFAULT_NULL;
    KDeleteIm(m_pPixelShader);
    m_pPixelShader = NULL;
    KDeleteIm(m_pVertexShader);
    m_pVertexShader = NULL;
    KDeleteIm(m_pRaster);
    m_pRaster = NULL;
    KDeleteIm(m_pClipper);
    m_pClipper = NULL;
    m_pShadedVertices->Clear();
    KDeleteIm(m_pShadedVertices);
    m_pShadedVertices = NULL;
    m_pClippedVertices->Clear();
    KDeleteIm(m_pClippedVertices);
    m_pClippedVertices = NULL;

    // Transformations
    KDeleteIm(m_pProjector);
    m_pProjector = NULL;
    KDeleteIm(m_pCamera);
    m_pCamera = NULL;
    m_pWorldStack->Clear();
    KDeleteIm(m_pWorldStack);
    m_pWorldStack = NULL;

    // States
    KDeleteIm( (StateTransform*)m_arrStates[RENDERSTATE_TRANSFORM] );
    KDeleteIm( (StateCulling*)m_arrStates[RENDERSTATE_CULLING] );
    KDeleteIm( (StateClipping*)m_arrStates[RENDERSTATE_CLIPPING] );
    KDeleteIm( (StateDepth*)m_arrStates[RENDERSTATE_DEPTH] );
    KDeleteIm( (StateStencil*)m_arrStates[RENDERSTATE_STENCIL] );
    KDeleteIm( (StateBlending*)m_arrStates[RENDERSTATE_BLENDING] );
    for(i = 0; i < RENDERSTATE_COUNT; ++i)
        m_arrStates[i] = NULL;

    // Frame
    m_pFrame->Destroy();
    KDeleteIm(m_pFrame);
    m_pFrame = NULL;

    // Screen
    KDeleteIm(m_pWindow);
    m_pWindow = NULL;
    m_iWinWidth = 0;
    m_iWinHeight = 0;
    m_pRenderTarget = NULL;

    // Memory Model
    MemoryFn->DestroyAllocator(m_idAllocLightPool);
    m_idAllocLightPool = INVALID_OFFSET;
    MemoryFn->DestroyAllocator(m_idAllocMaterialPool);
    m_idAllocMaterialPool = INVALID_OFFSET;
    MemoryFn->DestroyAllocator(m_idAllocShaderHeap);
    m_idAllocShaderHeap = INVALID_OFFSET;
    MemoryFn->DestroyAllocator(m_idAllocSamplerHeap);
    m_idAllocSamplerHeap = INVALID_OFFSET;
    MemoryFn->DestroyAllocator(m_idAllocTextureHeap);
    m_idAllocTextureHeap = INVALID_OFFSET;
    MemoryFn->DestroyAllocator(m_idAllocBufferHeap);
    m_idAllocBufferHeap = INVALID_OFFSET;
    MemoryFn->DestroyAllocator(m_idAllocCFLPool);
    m_idAllocCFLPool = INVALID_OFFSET;
    MemoryFn->DestroyAllocator(m_idAllocHeap);
    m_idAllocHeap = INVALID_OFFSET;
    MemoryFn->DestroyAllocator(m_idAllocStack);
    m_idAllocStack = INVALID_OFFSET;

    // Done
    m_bInitialized = false;
}

Void Renderer::RegisterTexture( Texture * pTexture, UInt iChannel )
{
    Assert(m_bInitialized);
    Assert(iChannel < RENDERER_TEXTURE_CHANNELS);

    m_arrTextureChannels[iChannel] = pTexture;
    if ( m_arrTextureSamplersChannels[iChannel] != NULL) {
        KDeleteExA( m_arrTextureSamplersChannels[iChannel], m_idAllocHeap );
        m_arrTextureSamplersChannels[iChannel] = NULL;
    }
    if (pTexture != NULL) {
        switch( pTexture->GetData()->GetDimension() ) {
            case 1: m_arrTextureSamplersChannels[iChannel] = KNewExA(m_idAllocHeap) Sampler1D( m_idAllocSamplerHeap, pTexture ); break;
            case 2: m_arrTextureSamplersChannels[iChannel] = KNewExA(m_idAllocHeap) Sampler2D( m_idAllocSamplerHeap, pTexture ); break;
            default: Assert(false); return;
        }
    }
}
Void Renderer::RegisterMaterial( Material * pMaterial, UInt iChannel )
{
    Assert(m_bInitialized);
    Assert(iChannel < RENDERER_MATERIAL_CHANNELS);

    m_arrMaterialChannels[iChannel] = pMaterial;
}
Void Renderer::RegisterLight( Light * pLight, UInt iChannel )
{
    Assert(m_bInitialized);
    Assert(iChannel < RENDERER_LIGHT_CHANNELS);

    m_arrLights[iChannel] = pLight;
}

Void Renderer::Reset()
{
    Assert(m_bInitialized);

	m_pWorldStack->Erase();
    m_pWorldStack->Push( Matrix4::Identity );
    m_matTrWorld.MakeIdentity();
    m_matInvWorld.MakeIdentity();
}
Void Renderer::PushMatrix()
{
    Assert(m_bInitialized);

	m_pWorldStack->Push( (*m_pWorldStack)[m_pWorldStack->Length() - 1] );
}
Void Renderer::PopMatrix()
{
    Assert(m_bInitialized);

	static Matrix4 s_DummyOut;
	m_pWorldStack->Pop(s_DummyOut);
    if ( m_pWorldStack->IsEmpty() ) {
		m_pWorldStack->Push( Matrix4::Identity );
        m_matTrWorld.MakeIdentity();
        m_matInvWorld.MakeIdentity();
    } else {
        (*m_pWorldStack)[m_pWorldStack->Length() - 1].Transpose(m_matTrWorld);
        (*m_pWorldStack)[m_pWorldStack->Length() - 1].Invert(m_matInvWorld);
    }
}
Void Renderer::Identity()
{
    Assert(m_bInitialized);

	(*m_pWorldStack)[m_pWorldStack->Length() - 1].MakeIdentity();
    m_matTrWorld.MakeIdentity();
    m_matInvWorld.MakeIdentity();
}
Void Renderer::Translate( const Vector3 & vTranslate )
{
    Assert(m_bInitialized);

	Matrix4 newWorldMat;
	newWorldMat.MakeTranslate( vTranslate );
    newWorldMat *= (*m_pWorldStack)[m_pWorldStack->Length() - 1];
    (*m_pWorldStack)[m_pWorldStack->Length() - 1] = newWorldMat;

	Matrix4 newInvWorldMat;
    newInvWorldMat.MakeTranslate( -vTranslate );
    m_matInvWorld *= newInvWorldMat;

    (*m_pWorldStack)[m_pWorldStack->Length() - 1].Transpose(m_matTrWorld);
}
Void Renderer::Rotate( const Vector3 & vRotate )
{
    Assert(m_bInitialized);

    Matrix4 matTemp;

    Matrix4 newWorldMat;
	newWorldMat.MakeRotate( AXIS_Z, vRotate.Z );
    matTemp.MakeRotate( AXIS_X, vRotate.X );
    newWorldMat *= matTemp;
	matTemp.MakeRotate( AXIS_Y, vRotate.Y );
	newWorldMat *= matTemp;
	newWorldMat *= (*m_pWorldStack)[m_pWorldStack->Length() - 1];
	(*m_pWorldStack)[m_pWorldStack->Length() - 1] = newWorldMat;

    Matrix4 newInvWorldMat;
	newInvWorldMat.MakeRotate( AXIS_Y, -vRotate.Y );
    m_matInvWorld *= newInvWorldMat;
    newInvWorldMat.MakeRotate( AXIS_X, -vRotate.X );
    m_matInvWorld *= newInvWorldMat;
	newInvWorldMat.MakeRotate( AXIS_Z, -vRotate.Z );
	m_matInvWorld *= newInvWorldMat;

    (*m_pWorldStack)[m_pWorldStack->Length() - 1].Transpose(m_matTrWorld);
}
Void Renderer::Scale( const Vector3 & vScale )
{
    Assert(m_bInitialized);

	Matrix4 newWorldMat;
	newWorldMat.MakeScale( vScale );
	newWorldMat *= (*m_pWorldStack)[m_pWorldStack->Length() - 1];
	(*m_pWorldStack)[m_pWorldStack->Length() - 1] = newWorldMat;

    Matrix4 newInvWorldMat;
    newInvWorldMat.MakeScale( Vector3(MathFn->Invert(vScale.X), MathFn->Invert(vScale.Y), MathFn->Invert(vScale.Z)) );
    m_matInvWorld *= newInvWorldMat;

    (*m_pWorldStack)[m_pWorldStack->Length() - 1].Transpose(m_matTrWorld);
}
Void Renderer::Transform( const Matrix4 & matTransform )
{
    Assert(m_bInitialized);

	Matrix4 newWorldMat(matTransform);
	newWorldMat *= (*m_pWorldStack)[m_pWorldStack->Length() - 1];
	(*m_pWorldStack)[m_pWorldStack->Length() - 1] = newWorldMat;

    Matrix4 newInvWorldMat;
    matTransform.Invert(newInvWorldMat);
    m_matInvWorld *= newInvWorldMat;

    (*m_pWorldStack)[m_pWorldStack->Length() - 1].Transpose(m_matTrWorld);
}

Void Renderer::RenderStream( const VertexBuffer * pVertices, const IndexBuffer * pIndices )
{
    Assert(m_bInitialized);

    UInt iFacet;
    UInt iA, iB, iC, iQuad;
    switch( pIndices->GetMode() ) {
        case INDEXBUFFER_VERTEX_LIST:
            for(iFacet = 0; iFacet < pIndices->GetFacetCount(); ++iFacet) {
                pIndices->GetFacet( iFacet, iA );
                _Draw_Vertex( pVertices->GetVertex(iA) );
            }
            break;
        case INDEXBUFFER_LINE_LIST:
        case INDEXBUFFER_LINE_STRIPS:
        case INDEXBUFFER_LINE_FANS:
            for(iFacet = 0; iFacet < pIndices->GetFacetCount(); ++iFacet) {
                pIndices->GetFacet( iFacet, iA, iB );
                _Draw_Segment( pVertices->GetVertex(iA),
                               pVertices->GetVertex(iB) );
            }
            break;
        case INDEXBUFFER_TRIANGLE_LIST:
        case INDEXBUFFER_TRIANGLE_STRIPS:
        case INDEXBUFFER_TRIANGLE_FANS:
            for(iFacet = 0; iFacet < pIndices->GetFacetCount(); ++iFacet) {
                pIndices->GetFacet( iFacet, iA, iB, iC );
                _Draw_Triangle( pVertices->GetVertex(iA),
                                pVertices->GetVertex(iB),
                                pVertices->GetVertex(iC) );
            }
            break;
        case INDEXBUFFER_QUAD_LIST:
        case INDEXBUFFER_QUAD_STRIPS:
            for(iFacet = 0; iFacet < pIndices->GetFacetCount(); ++iFacet) {
                pIndices->GetFacet( iFacet, iA, iB, iC, iQuad );
                _Draw_Triangle( pVertices->GetVertex(iA),
                                pVertices->GetVertex(iB),
                                pVertices->GetVertex(iC) );
                _Draw_Triangle( pVertices->GetVertex(iA),
                                pVertices->GetVertex(iC),
                                pVertices->GetVertex(iQuad) );
            }
            break;
        default: Assert(false); return; // unsupported
    }
}

Void Renderer::RawStart( IndexBufferMode iMode, Bool bCompile )
{
    Assert(m_bInitialized);
    Assert(!m_bRawRendering);

    m_bRawRendering = true;
    m_bRawCompiling = bCompile;
    m_iRawMode = iMode;
    m_pRawVertices[0] = m_pVSInput->Allocate();
    m_pRawVertices[1] = m_pVSInput->Allocate();
    m_pRawVertices[2] = m_pVSInput->Allocate();
    m_pRawVertices[3] = m_pVSInput->Allocate();
    m_iRawCurVertex = 0;
    m_iRawBias = 0;
}
Void Renderer::RawVertex( CFLPtr pVertex )
{
    Assert(m_bInitialized);
    Assert(m_bRawRendering);

    for(UInt j = 0; j < m_pVSInput->GetTotalSize(); ++j)
        (m_pRawVertices[m_iRawCurVertex])[j] = pVertex[j];

    CFLPtr pSwap = NULL;
	switch( m_iRawMode ) {
		case INDEXBUFFER_VERTEX_LIST:
			_Draw_Vertex( m_pRawVertices[0] );
			m_iRawCurVertex = 0;
			break;
		case INDEXBUFFER_LINE_LIST:
			if (m_iRawCurVertex == 1) {
				_Draw_Segment( m_pRawVertices[0], m_pRawVertices[1] );
				m_iRawCurVertex = 0;
			} else
				++m_iRawCurVertex;
			break;
		case INDEXBUFFER_LINE_STRIPS:
			if (m_iRawCurVertex == 1) {
                _Draw_Segment( m_pRawVertices[0], m_pRawVertices[1] );
                pSwap = m_pRawVertices[0];
				m_pRawVertices[0] = m_pRawVertices[1];
                m_pRawVertices[1] = pSwap;
				m_iRawCurVertex = 1;
			} else
				++m_iRawCurVertex;
			break;
		case INDEXBUFFER_LINE_FANS:
			if (m_iRawCurVertex == 1) {
                _Draw_Segment( m_pRawVertices[0], m_pRawVertices[1] );
				m_iRawCurVertex = 1;
			} else
				++m_iRawCurVertex;
			break;
		case INDEXBUFFER_TRIANGLE_LIST:
			if (m_iRawCurVertex == 2) {
				_Draw_Triangle( m_pRawVertices[0], m_pRawVertices[1], m_pRawVertices[2] );
				m_iRawCurVertex = 0;
			} else
				++m_iRawCurVertex;
			break;
		case INDEXBUFFER_TRIANGLE_STRIPS:
			if (m_iRawCurVertex == 2) {
                if (m_iRawBias)
				    _Draw_Triangle( m_pRawVertices[1], m_pRawVertices[0], m_pRawVertices[2] );
                else
                    _Draw_Triangle( m_pRawVertices[0], m_pRawVertices[1], m_pRawVertices[2] );
                m_iRawBias = (1 - m_iRawBias);
                pSwap = m_pRawVertices[0];
				m_pRawVertices[0] = m_pRawVertices[1];
                m_pRawVertices[1] = m_pRawVertices[2];
                m_pRawVertices[2] = pSwap;
				m_iRawCurVertex = 2;
			} else
				++m_iRawCurVertex;
			break;
		case INDEXBUFFER_TRIANGLE_FANS:
			if (m_iRawCurVertex == 2) {
				_Draw_Triangle( m_pRawVertices[0], m_pRawVertices[1], m_pRawVertices[2] );
                pSwap = m_pRawVertices[1];
				m_pRawVertices[1] = m_pRawVertices[2];
                m_pRawVertices[2] = pSwap;
				m_iRawCurVertex = 2;
			} else
				++m_iRawCurVertex;
			break;
		case INDEXBUFFER_QUAD_LIST:
			if (m_iRawCurVertex == 3) {
				_Draw_Triangle( m_pRawVertices[0], m_pRawVertices[1], m_pRawVertices[2] );
				_Draw_Triangle( m_pRawVertices[0], m_pRawVertices[2], m_pRawVertices[3] );
				m_iRawCurVertex = 0;
			} else
				++m_iRawCurVertex;
			break;
		case INDEXBUFFER_QUAD_STRIPS:
			if (m_iRawCurVertex == 3) {
				_Draw_Triangle( m_pRawVertices[0], m_pRawVertices[1], m_pRawVertices[2] );
				_Draw_Triangle( m_pRawVertices[0], m_pRawVertices[2], m_pRawVertices[3] );
                pSwap = m_pRawVertices[0];
				m_pRawVertices[0] = m_pRawVertices[3];
                m_pRawVertices[3] = pSwap;
                pSwap = m_pRawVertices[1];
				m_pRawVertices[1] = m_pRawVertices[2];
                m_pRawVertices[2] = pSwap;
				m_iRawCurVertex = 2;
			} else
				++m_iRawCurVertex;
			break;
		default:
			break;
	}
}
Void Renderer::RawFinish()
{
    Assert(m_bInitialized);
    Assert(m_bRawRendering);

    // Check unfinished primitive
	// Emit a warning ?

    m_bRawRendering = false;
    m_pVSInput->Free(m_pRawVertices[0]);
    m_pVSInput->Free(m_pRawVertices[1]);
    m_pVSInput->Free(m_pRawVertices[2]);
    m_pVSInput->Free(m_pRawVertices[3]);
}

/////////////////////////////////////////////////////////////////////////////////

Void Renderer::_OnViewChange()
{
    Assert(m_bInitialized);

    m_pCamera->GetViewMatrix(m_matView);
    m_matView.Transpose(m_matTrView);
    m_pCamera->GetInvViewMatrix(m_matInvView);
    m_bUpdateClippingPlanes = true;
}
Void Renderer::_OnFrustrumChange()
{
    Assert(m_bInitialized);

    if ( _GetStateTransform()->UsePerspective() )
        m_pCamera->GetPerspectiveMatrix(m_matProjection);
    else
        m_pCamera->GetOrthogonalMatrix(m_matProjection);
    m_matProjection.Transpose(m_matTrProjection);
    m_matProjection.Invert(m_matInvProjection);
    m_bUpdateClippingPlanes = true;
}
Void Renderer::_OnViewportChange()
{
    Assert(m_bInitialized);

    // Clip-Space is [-1;1]x[-1;1]x[0;1]xW
    // f(x) = [-1;1] => [L;R] = ( (x+1)/2 * (R-L) ) + L = (R+L)/2 + x * (R-L)/2
    // f(y) = [-1;1] => [T;B] = ( (y+1)/2 * (B-T) ) + T = (B+T)/2 + y * (B-T)/2
    // with 0 <= L < R <= m_iWinWidth
    //      0 <= T < B <= m_iWinHeight
    UInt iLeft, iRight, iBottom, iTop, iWidth, iHeight;
    m_pCamera->GetViewport( iLeft, iRight, iBottom, iTop, iWidth, iHeight );
    m_vViewPortOffset.X = 0.5f * (Scalar)( iRight + iLeft );
    m_vViewPortFactor.X = 0.5f * (Scalar)( iRight - iLeft );
    m_vViewPortOffset.Y = 0.5f * (Scalar)( iBottom + iTop );
    m_vViewPortFactor.Y = 0.5f * (Scalar)( iBottom - iTop );
}
Void Renderer::_OnDepthRangeChange()
{
    Assert(m_bInitialized);

    // Clip-Space is [-1;1]x[-1;1]x[0;1]xW
    // f(z) = [0;1]  => [N;F] = ( z * (F-N) ) + N = N + z * (F-N)
    // with 0 <= N < F <= 1
    Scalar fNear, fFar;
    m_pCamera->GetDepthRange( fNear, fFar );
    m_vViewPortOffset.Z = fNear;
    m_vViewPortFactor.Z = ( fFar - fNear );
}

Void Renderer::_OnSelectVS()
{
    Assert(m_bInitialized);

    m_pVSInput = m_pVertexShader->GetInputCFL();
    m_pVSOutput = m_pVertexShader->GetOutputCFL();
    m_pClipper->SetCFL( m_pVSInput );
}
Void Renderer::_OnSelectPS()
{
    Assert(m_bInitialized);

    m_pPSInput = m_pPixelShader->GetInputCFL();
    m_pRaster->SetCFL( m_pPSInput );
}

Void Renderer::_ClippingOutput_Vertex( CFLPtr pVertexA )
{
    Assert(m_bInitialized);

    m_pClippedVertices->Push(pVertexA);
}
Void Renderer::_ClippingOutput_Segment( CFLPtr pVertexA, CFLPtr pVertexB )
{
    Assert(m_bInitialized);

    m_pClippedVertices->Push(pVertexA);
    m_pClippedVertices->Push(pVertexB);
}
Void Renderer::_ClippingOutput_Triangle( CFLPtr pVertexA, CFLPtr pVertexB, CFLPtr pVertexC )
{
    Assert(m_bInitialized);

    m_pClippedVertices->Push(pVertexA);
    m_pClippedVertices->Push(pVertexB);
    m_pClippedVertices->Push(pVertexC);
}

Void Renderer::_VertexShaderOutput_Vertex( CFLPtr pVertexA )
{
    Assert(m_bInitialized);

    m_pShadedVertices->Push(pVertexA);
}

Void Renderer::_VertexToPixel( CFLPtr pPixel, CCFLPtr pVertex ) const
{
    Assert(m_bInitialized);

    const Vertex4 * pClipPos = (const Vertex4*)( m_pVSOutput->GetField( pVertex, CFLSEMANTIC_POSITION, 0 ) );
    Vertex2 * pRasterPos =           (Vertex2*)( m_pPSInput->GetField( pPixel, CFLSEMANTIC_RASTERPOS, 0 ) );
    Scalar * pDepth =                 (Scalar*)( m_pPSInput->GetField( pPixel, CFLSEMANTIC_DEPTH, 0 ) );
    Scalar * pInvW =                  (Scalar*)( m_pPSInput->GetField( pPixel, CFLSEMANTIC_INVERTW, 0 ) );
    Scalar * pRegW =                  (Scalar*)( m_pPSInput->GetField( pPixel, CFLSEMANTIC_REGULARW, 0 ) );
    *pInvW = MathFn->Invert(pClipPos->W);
    *pRegW = pClipPos->W;
    Scalar fClipX = ( pClipPos->X * (*pInvW) );
    Scalar fClipY = ( pClipPos->Y * (*pInvW) );
    Scalar fClipZ = ( pClipPos->Z * (*pInvW) );

    // Viewport clipping
    if ( _GetStateClipping()->UseClipSpaceClipping() ) {
        fClipX = Clamp<Scalar>( fClipX, -1.0f, +1.0f );
        fClipY = Clamp<Scalar>( fClipY, -1.0f, +1.0f );
        fClipZ = Clamp<Scalar>( fClipZ, 0.0f, +1.0f );
    }

    // Transforms from clip-space position given as standard output
    // by the vertex shader to viewport coordinates using m_vViewPortOffset
    // and m_vViewPortFactor as defined in _OnViewPortChange() and
    // _OnDepthRangeChange() methods ...
    pRasterPos->X = m_vViewPortOffset.X + m_vViewPortFactor.X * fClipX;
    pRasterPos->Y = m_vViewPortOffset.Y + m_vViewPortFactor.Y * fClipY;
    *pDepth = m_vViewPortOffset.Z + m_vViewPortFactor.Z * fClipZ;

    // transform remaining data with W
    for(UInt i = 4; i < m_pVSOutput->GetTotalSize(); ++i)
        pPixel[i+1] = ( pVertex[i] * (*pInvW) );
}

Void Renderer::_Draw_Vertex( CCFLPtr pVertexA )
{
    Assert(m_bInitialized);

    // not yet implemented
}
Void Renderer::_Draw_Segment( CCFLPtr pVertexA, CCFLPtr pVertexB )
{
    Assert(m_bInitialized);

    StateClipping * pStateClipping = _GetStateClipping();
    CFLPtr pVertex;
    UInt i;

    // Copy Data
    CFLPtr pVA = m_pVSInput->Allocate();
    CFLPtr pVB = m_pVSInput->Allocate();
    for(i = 0; i < m_pVSInput->GetTotalSize(); ++i) {
        pVA[i] = pVertexA[i];
        pVB[i] = pVertexB[i];
    }

    // Transform to World
    Vertex4 * pA = (Vertex4*)( m_pVSInput->GetField( pVA, CFLSEMANTIC_POSITION, 0 ) );
    Vertex4 * pB = (Vertex4*)( m_pVSInput->GetField( pVB, CFLSEMANTIC_POSITION, 0 ) );
    *pA = ( (*m_pWorldStack)[m_pWorldStack->Length() - 1] * (*pA) );
    *pB = ( (*m_pWorldStack)[m_pWorldStack->Length() - 1] * (*pB) );

    // Clipping
    if ( m_bUpdateClippingPlanes )
        m_pClipper->ExtractFrustrumPlanes();
    if ( pStateClipping->UseFrustrumClipping() ) {
        // Full clipping
        if ( !m_pClipper->ClipSegment( pVA, pVB ) ) {
            m_pVSInput->Free(pVA);
            m_pVSInput->Free(pVB);
            return; // rejected !
        }
    } else {
        // Simple cull only
        if ( !m_pClipper->CullSegment( pVA, pVB, pStateClipping->RejectClipped() ) ) {
            m_pVSInput->Free(pVA);
            m_pVSInput->Free(pVB);
            return; // rejected !
        }
    }

    // Vertex Shader
    m_pVertexShader->Prepare();
    for(i = 0; i < m_pClippedVertices->Length(); ++i) {
        pVertex = m_pClippedVertices->GetVertex(i);
        m_pVertexShader->ProcessVertex(pVertex);
        m_pVSInput->Free(pVertex);
    }
    m_pClippedVertices->Clear();

    // Rasterize + Pixel Shader
    m_pPixelShader->Prepare();
    CFLPtr pPixelA = m_pPSInput->Allocate();
    CFLPtr pPixelB = m_pPSInput->Allocate();
    i = 0;
    while (i < m_pShadedVertices->Length()) {
        pVertex = m_pShadedVertices->GetVertex(i++);
        _VertexToPixel( pPixelA, pVertex );
        m_pVSOutput->Free(pVertex);
        pVertex = m_pShadedVertices->GetVertex(i++);
        _VertexToPixel( pPixelB, pVertex );
        m_pVSOutput->Free(pVertex);

        m_pRaster->ScanSegment( pPixelA, pPixelB );
    }
    m_pShadedVertices->Clear();
    m_pPSInput->Free(pPixelA);
    m_pPSInput->Free(pPixelB);
}
Void Renderer::_Draw_Triangle( CCFLPtr pVertexA, CCFLPtr pVertexB, CCFLPtr pVertexC )
{
    Assert(m_bInitialized);

    StateCulling * pStateCulling = _GetStateCulling();
    StateClipping * pStateClipping = _GetStateClipping();
    CFLPtr pVertex;
    UInt i;

    // Copy Data
    CFLPtr pVA = m_pVSInput->Allocate();
    CFLPtr pVB = m_pVSInput->Allocate();
    CFLPtr pVC = m_pVSInput->Allocate();
    for(i = 0; i < m_pVSInput->GetTotalSize(); ++i) {
        pVA[i] = pVertexA[i];
        pVB[i] = pVertexB[i];
        pVC[i] = pVertexC[i];
    }

    // Transform to World
    Vertex4 * pA = (Vertex4*)( m_pVSInput->GetField( pVA, CFLSEMANTIC_POSITION, 0 ) );
    Vertex4 * pB = (Vertex4*)( m_pVSInput->GetField( pVB, CFLSEMANTIC_POSITION, 0 ) );
    Vertex4 * pC = (Vertex4*)( m_pVSInput->GetField( pVC, CFLSEMANTIC_POSITION, 0 ) );
    *pA = ( (*m_pWorldStack)[m_pWorldStack->Length() - 1] * (*pA) );
    *pB = ( (*m_pWorldStack)[m_pWorldStack->Length() - 1] * (*pB) );
    *pC = ( (*m_pWorldStack)[m_pWorldStack->Length() - 1] * (*pC) );

    // Culling
    if ( pStateCulling->IsEnabled() ) {
        // We assume right-hand coordinates ...
        Vector4 vAB = ( (*pB) - (*pA) );
		Vector4 vAC = ( (*pC) - (*pA) );
		Vector4 vFaceNormal;
        if ( pStateCulling->GetCullConv() == STATE_CULLING_CONV_COUNTER_CLOCKWISE )
            vFaceNormal = (vAB ^ vAC);
        else if ( pStateCulling->GetCullConv() == STATE_CULLING_CONV_CLOCKWISE )
            vFaceNormal = (vAC ^ vAB);
        else {
            m_pVSInput->Free(pVA);
            m_pVSInput->Free(pVB);
            m_pVSInput->Free(pVC);
            Assert(false);
            return; // corrupted state
        }

        // Cull test
        Vector4 vView = ( (*pA) - m_pCamera->GetViewEye() );
        vView += ( (*pB) - m_pCamera->GetViewEye() );
        vView += ( (*pC) - m_pCamera->GetViewEye() );
        Scalar fVisible = ( vFaceNormal * vView );
        if ( pStateCulling->GetCullTest() == STATE_CULLING_BACK ) {
            if ( fVisible >= 0.0f ) {
                m_pVSInput->Free(pVA);
                m_pVSInput->Free(pVB);
                m_pVSInput->Free(pVC);
			    return; // rejected !
            }
        } else if ( pStateCulling->GetCullTest() == STATE_CULLING_FRONT ) {
            if ( fVisible <= 0.0f ) {
                m_pVSInput->Free(pVA);
                m_pVSInput->Free(pVB);
                m_pVSInput->Free(pVC);
			    return; // rejected !
            }
        } else {
            m_pVSInput->Free(pVA);
            m_pVSInput->Free(pVB);
            m_pVSInput->Free(pVC);
            Assert(false);
            return; // corrupted state
        }
    }

    // Clipping
    if ( m_bUpdateClippingPlanes )
        m_pClipper->ExtractFrustrumPlanes();
    if ( pStateClipping->UseFrustrumClipping() ) {
        // Full clipping
        if ( !m_pClipper->ClipTriangle( pVA, pVB, pVC ) ) {
            m_pVSInput->Free(pVA);
            m_pVSInput->Free(pVB);
            m_pVSInput->Free(pVC);
            return; // rejected !
        }
    } else {
        // Simple cull only
        if ( !m_pClipper->CullTriangle( pVA, pVB, pVC, pStateClipping->RejectClipped() ) ) {
            m_pVSInput->Free(pVA);
            m_pVSInput->Free(pVB);
            m_pVSInput->Free(pVC);
            return; // rejected !
        }
    }

    // Vertex Shader
    m_pVertexShader->Prepare();
    for(i = 0; i < m_pClippedVertices->Length(); ++i) {
        pVertex = m_pClippedVertices->GetVertex(i);
        m_pVertexShader->ProcessVertex(pVertex);
        m_pVSInput->Free(pVertex);
    }
    m_pClippedVertices->Clear();

    // Rasterize + Pixel Shader
    m_pPixelShader->Prepare();
    CFLPtr pPixelA = m_pPSInput->Allocate();
    CFLPtr pPixelB = m_pPSInput->Allocate();
    CFLPtr pPixelC = m_pPSInput->Allocate();
    i = 0;
    while (i < m_pShadedVertices->Length()) {
        pVertex = m_pShadedVertices->GetVertex(i++);
        _VertexToPixel( pPixelA, pVertex );
        m_pVSOutput->Free(pVertex);
        pVertex = m_pShadedVertices->GetVertex(i++);
        _VertexToPixel( pPixelB, pVertex );
        m_pVSOutput->Free(pVertex);
        pVertex = m_pShadedVertices->GetVertex(i++);
        _VertexToPixel( pPixelC, pVertex );
        m_pVSOutput->Free(pVertex);
        
        m_pRaster->ScanTriangle( pPixelA, pPixelB, pPixelC );
    }
    m_pShadedVertices->Clear();
    m_pPSInput->Free(pPixelA);
    m_pPSInput->Free(pPixelB);
    m_pPSInput->Free(pPixelC);
}

Void Renderer::_GetShaderConstant( Scalar * outValues, RenderingConstantType iConstantType ) const
{
    Vertex4 * outVertex = (Vertex4*)outValues;
    Vector4 * outVector = (Vector4*)outValues;
    Color4 * outColor = (Color4*)outValues;
    Matrix4 * outMatrix = (Matrix4*)outValues;
    Matrix4 matTemp;

    switch(iConstantType) {
        case RENDERING_CONST_MATRIX_WORLD:
            *outMatrix = (*m_pWorldStack)[m_pWorldStack->Length() - 1];
            break;
        case RENDERING_CONST_MATRIX_VIEW:
            *outMatrix = m_matView;
            break;
        case RENDERING_CONST_MATRIX_PROJ:
            *outMatrix = m_matProjection;
            break;
        case RENDERING_CONST_MATRIX_WORLDVIEW:
            *outMatrix = m_matView;
            *outMatrix *= (*m_pWorldStack)[m_pWorldStack->Length() - 1];
            break;
        case RENDERING_CONST_MATRIX_VIEWPROJ:
            *outMatrix = m_matProjection;
            *outMatrix *= m_matView;
            break;
        case RENDERING_CONST_MATRIX_WORLDVIEWPROJ:
            *outMatrix = m_matProjection;
            *outMatrix *= m_matView;
            *outMatrix *= (*m_pWorldStack)[m_pWorldStack->Length() - 1];
            break;
        case RENDERING_CONST_MATRIX_PROJECTOR:
            m_pProjector->GetViewMatrix(*outMatrix);
            break;
        case RENDERING_CONST_MATRIX_WORLD_TRANSPOSE:
            *outMatrix = m_matTrWorld;
            break;
        case RENDERING_CONST_MATRIX_VIEW_TRANSPOSE:
            *outMatrix = m_matTrView;
            break;
        case RENDERING_CONST_MATRIX_PROJ_TRANSPOSE:
            *outMatrix = m_matTrProjection;
            break;
        case RENDERING_CONST_MATRIX_WORLDVIEW_TRANSPOSE:
            *outMatrix = m_matTrView;
            *outMatrix *= m_matTrWorld;
            break;
        case RENDERING_CONST_MATRIX_VIEWPROJ_TRANSPOSE:
            *outMatrix = m_matTrProjection;
            *outMatrix *= m_matTrView;
            break;
        case RENDERING_CONST_MATRIX_WORLDVIEWPROJ_TRANSPOSE:
            *outMatrix = m_matTrProjection;
            *outMatrix *= m_matTrView;
            *outMatrix *= m_matTrWorld;
            break;
        case RENDERING_CONST_MATRIX_WORLD_INVERSE:
            *outMatrix = m_matInvWorld;
            break;
        case RENDERING_CONST_MATRIX_VIEW_INVERSE:
            *outMatrix = m_matInvView;
            break;
        case RENDERING_CONST_MATRIX_PROJ_INVERSE:
            *outMatrix = m_matInvProjection;
            break;
        case RENDERING_CONST_MATRIX_WORLDVIEW_INVERSE:
            *outMatrix = m_matInvWorld;
            *outMatrix *= m_matInvView;
            break;
        case RENDERING_CONST_MATRIX_VIEWPROJ_INVERSE:
            *outMatrix = m_matInvView;
            *outMatrix *= m_matInvProjection;
            break;
        case RENDERING_CONST_MATRIX_WORLDVIEWPROJ_INVERSE:
            *outMatrix = m_matInvWorld;
            *outMatrix *= m_matInvView;
            *outMatrix *= m_matInvProjection;
            break;
        case RENDERING_CONST_MATRIX_WORLD_INVERSE_TRANSPOSE:
            m_matInvWorld.Transpose(*outMatrix);
            break;
        case RENDERING_CONST_MATRIX_VIEW_INVERSE_TRANSPOSE:
            m_matInvView.Transpose(*outMatrix);
            break;
        case RENDERING_CONST_MATRIX_PROJ_INVERSE_TRANSPOSE:
            m_matInvProjection.Transpose(*outMatrix);
            break;
        case RENDERING_CONST_MATRIX_WORLDVIEW_INVERSE_TRANSPOSE:
            matTemp = m_matInvWorld;
            matTemp *= m_matInvView;
            matTemp.Transpose(*outMatrix);
            break;
        case RENDERING_CONST_MATRIX_VIEWPROJ_INVERSE_TRANSPOSE:
            matTemp = m_matInvView;
            matTemp *= m_matInvProjection;
            matTemp.Transpose(*outMatrix);
            break;
        case RENDERING_CONST_MATRIX_WORLDVIEWPROJ_INVERSE_TRANSPOSE:
            matTemp = m_matInvWorld;
            matTemp *= m_matInvView;
            matTemp *= m_matInvProjection;
            matTemp.Transpose(*outMatrix);
            break;
        case RENDERING_CONST_CAMERA_POSITION:
            *outVertex = Vertex4( m_pCamera->GetViewEye() );
            break;
        case RENDERING_CONST_CAMERA_DIRECTION:
            *outVector = Vector4( m_pCamera->GetViewDir() );
            break;
        case RENDERING_CONST_CAMERA_UP:
            *outVector = Vector4( m_pCamera->GetViewUp() );
            break;
        case RENDERING_CONST_CAMERA_RIGHT:
            *outVector = Vector4( m_pCamera->GetViewRight() );
            break;
        case RENDERING_CONST_PROJECTOR_POSITION:
            *outVertex = Vertex4( m_pProjector->GetViewEye() );
            break;
        case RENDERING_CONST_PROJECTOR_DIRECTION:
            *outVector = Vector4( m_pProjector->GetViewDir() );
            break;
        case RENDERING_CONST_PROJECTOR_UP:
            *outVector = Vector4( m_pProjector->GetViewUp() );
            break;
        case RENDERING_CONST_PROJECTOR_RIGHT:
            *outVector = Vector4( m_pProjector->GetViewRight() );
            break;
        case RENDERING_CONST_MATERIAL0_EMISSIVE:
            if ( m_arrMaterialChannels[0] != NULL ) {
                if ( m_arrMaterialChannels[0]->IsEmissive() )
                    *outColor = m_arrMaterialChannels[0]->Emissive();
                else
                    *outColor = Color4::Black;
            }
            break;
        case RENDERING_CONST_MATERIAL0_AMBIENT:
            if ( m_arrMaterialChannels[0] != NULL )
                *outColor = m_arrMaterialChannels[0]->Ambient();
            break;
        case RENDERING_CONST_MATERIAL0_DIFFUSE:
            if ( m_arrMaterialChannels[0] != NULL )
                *outColor = m_arrMaterialChannels[0]->Diffuse();
            break;
        case RENDERING_CONST_MATERIAL0_SPECULAR:
            if ( m_arrMaterialChannels[0] != NULL )
                *outColor = m_arrMaterialChannels[0]->Specular();
            break;
        case RENDERING_CONST_MATERIAL1_EMISSIVE:
            if ( m_arrMaterialChannels[1] != NULL ) {
                if ( m_arrMaterialChannels[1]->IsEmissive() )
                    *outColor = m_arrMaterialChannels[1]->Emissive();
                else
                    *outColor = Color4::Black;
            }
            break;
        case RENDERING_CONST_MATERIAL1_AMBIENT:
            if ( m_arrMaterialChannels[1] != NULL )
                *outColor = m_arrMaterialChannels[1]->Ambient();
            break;
        case RENDERING_CONST_MATERIAL1_DIFFUSE:
            if ( m_arrMaterialChannels[1] != NULL )
                *outColor = m_arrMaterialChannels[1]->Diffuse();
            break;
        case RENDERING_CONST_MATERIAL1_SPECULAR:
            if ( m_arrMaterialChannels[1] != NULL )
                *outColor = m_arrMaterialChannels[1]->Specular();
            break;
        case RENDERING_CONST_MATERIAL2_EMISSIVE:
            if ( m_arrMaterialChannels[2] != NULL ) {
                if ( m_arrMaterialChannels[2]->IsEmissive() )
                    *outColor = m_arrMaterialChannels[2]->Emissive();
                else
                    *outColor = Color4::Black;
            }
            break;
        case RENDERING_CONST_MATERIAL2_AMBIENT:
            if ( m_arrMaterialChannels[2] != NULL )
                *outColor = m_arrMaterialChannels[2]->Ambient();
            break;
        case RENDERING_CONST_MATERIAL2_DIFFUSE:
            if ( m_arrMaterialChannels[2] != NULL )
                *outColor = m_arrMaterialChannels[2]->Diffuse();
            break;
        case RENDERING_CONST_MATERIAL2_SPECULAR:
            if ( m_arrMaterialChannels[2] != NULL )
                *outColor = m_arrMaterialChannels[2]->Specular();
            break;
        case RENDERING_CONST_MATERIAL3_EMISSIVE:
            if ( m_arrMaterialChannels[3] != NULL ) {
                if ( m_arrMaterialChannels[3]->IsEmissive() )
                    *outColor = m_arrMaterialChannels[3]->Emissive();
                else
                    *outColor = Color4::Black;
            }
            break;
        case RENDERING_CONST_MATERIAL3_AMBIENT:
            if ( m_arrMaterialChannels[3] != NULL )
                *outColor = m_arrMaterialChannels[3]->Ambient();
            break;
        case RENDERING_CONST_MATERIAL3_DIFFUSE:
            if ( m_arrMaterialChannels[3] != NULL )
                *outColor = m_arrMaterialChannels[3]->Diffuse();
            break;
        case RENDERING_CONST_MATERIAL3_SPECULAR:
            if ( m_arrMaterialChannels[3] != NULL )
                *outColor = m_arrMaterialChannels[3]->Specular();
            break;
        case RENDERING_CONST_MATERIAL4_EMISSIVE:
            if ( m_arrMaterialChannels[4] != NULL ) {
                if ( m_arrMaterialChannels[4]->IsEmissive() )
                    *outColor = m_arrMaterialChannels[4]->Emissive();
                else
                    *outColor = Color4::Black;
            }
            break;
        case RENDERING_CONST_MATERIAL4_AMBIENT:
            if ( m_arrMaterialChannels[4] != NULL )
                *outColor = m_arrMaterialChannels[4]->Ambient();
            break;
        case RENDERING_CONST_MATERIAL4_DIFFUSE:
            if ( m_arrMaterialChannels[4] != NULL )
                *outColor = m_arrMaterialChannels[4]->Diffuse();
            break;
        case RENDERING_CONST_MATERIAL4_SPECULAR:
            if ( m_arrMaterialChannels[4] != NULL )
                *outColor = m_arrMaterialChannels[4]->Specular();
            break;
        case RENDERING_CONST_MATERIAL5_EMISSIVE:
            if ( m_arrMaterialChannels[5] != NULL ) {
                if ( m_arrMaterialChannels[5]->IsEmissive() )
                    *outColor = m_arrMaterialChannels[5]->Emissive();
                else
                    *outColor = Color4::Black;
            }
            break;
        case RENDERING_CONST_MATERIAL5_AMBIENT:
            if ( m_arrMaterialChannels[5] != NULL )
                *outColor = m_arrMaterialChannels[5]->Ambient();
            break;
        case RENDERING_CONST_MATERIAL5_DIFFUSE:
            if ( m_arrMaterialChannels[5] != NULL )
                *outColor = m_arrMaterialChannels[5]->Diffuse();
            break;
        case RENDERING_CONST_MATERIAL5_SPECULAR:
            if ( m_arrMaterialChannels[5] != NULL )
                *outColor = m_arrMaterialChannels[5]->Specular();
            break;
        case RENDERING_CONST_MATERIAL6_EMISSIVE:
            if ( m_arrMaterialChannels[6] != NULL ) {
                if ( m_arrMaterialChannels[6]->IsEmissive() )
                    *outColor = m_arrMaterialChannels[6]->Emissive();
                else
                    *outColor = Color4::Black;
            }
            break;
        case RENDERING_CONST_MATERIAL6_AMBIENT:
            if ( m_arrMaterialChannels[6] != NULL )
                *outColor = m_arrMaterialChannels[6]->Ambient();
            break;
        case RENDERING_CONST_MATERIAL6_DIFFUSE:
            if ( m_arrMaterialChannels[6] != NULL )
                *outColor = m_arrMaterialChannels[6]->Diffuse();
            break;
        case RENDERING_CONST_MATERIAL6_SPECULAR:
            if ( m_arrMaterialChannels[6] != NULL )
                *outColor = m_arrMaterialChannels[6]->Specular();
            break;
        case RENDERING_CONST_MATERIAL7_EMISSIVE:
            if ( m_arrMaterialChannels[7] != NULL ) {
                if ( m_arrMaterialChannels[7]->IsEmissive() )
                    *outColor = m_arrMaterialChannels[7]->Emissive();
                else
                    *outColor = Color4::Black;
            }
            break;
        case RENDERING_CONST_MATERIAL7_AMBIENT:
            if ( m_arrMaterialChannels[7] != NULL )
                *outColor = m_arrMaterialChannels[7]->Ambient();
            break;
        case RENDERING_CONST_MATERIAL7_DIFFUSE:
            if ( m_arrMaterialChannels[7] != NULL )
                *outColor = m_arrMaterialChannels[7]->Diffuse();
            break;
        case RENDERING_CONST_MATERIAL7_SPECULAR:
            if ( m_arrMaterialChannels[7] != NULL )
                *outColor = m_arrMaterialChannels[7]->Specular();
            break;
        case RENDERING_CONST_LIGHT0_POSITION:
            if ( m_arrLights[0] != NULL )
                *outVertex = m_arrLights[0]->Position();
            break;
        case RENDERING_CONST_LIGHT0_DIRECTION:
            if ( m_arrLights[0] != NULL )
                *outVector = m_arrLights[0]->Direction();
            break;
        case RENDERING_CONST_LIGHT0_AMBIENT:
            if ( m_arrLights[0] != NULL )
                *outColor = m_arrLights[0]->Ambient();
            break;
        case RENDERING_CONST_LIGHT0_DIFFUSE:
            if ( m_arrLights[0] != NULL )
                *outColor = m_arrLights[0]->Diffuse();
            break;
        case RENDERING_CONST_LIGHT0_SPECULAR:
            if ( m_arrLights[0] != NULL )
                *outColor = m_arrLights[0]->Specular();
            break;
        case RENDERING_CONST_LIGHT0_ATTENUATION:
            if ( m_arrLights[0] != NULL ) {
                outVector->X = m_arrLights[0]->FadeConstant();
                outVector->Y = m_arrLights[0]->FadeLinear();
                outVector->Z = m_arrLights[0]->FadeQuadratic();
                outVector->W = m_arrLights[0]->Intensity();
            }
            break;
        case RENDERING_CONST_LIGHT0_SPOTCUTOFF:
            if ( m_arrLights[0] != NULL ) {
                outVector->X = m_arrLights[0]->SpotAngle();
                outVector->Y = m_arrLights[0]->SpotSin();
                outVector->Z = m_arrLights[0]->SpotCos();
                outVector->W = m_arrLights[0]->SpotExp();
            }
            break;
        case RENDERING_CONST_LIGHT1_POSITION:
            if ( m_arrLights[1] != NULL )
                *outVertex = m_arrLights[1]->Position();
            break;
        case RENDERING_CONST_LIGHT1_DIRECTION:
            if ( m_arrLights[1] != NULL )
                *outVector = m_arrLights[1]->Direction();
            break;
        case RENDERING_CONST_LIGHT1_AMBIENT:
            if ( m_arrLights[1] != NULL )
                *outColor = m_arrLights[1]->Ambient();
            break;
        case RENDERING_CONST_LIGHT1_DIFFUSE:
            if ( m_arrLights[1] != NULL )
                *outColor = m_arrLights[1]->Diffuse();
            break;
        case RENDERING_CONST_LIGHT1_SPECULAR:
            if ( m_arrLights[1] != NULL )
                *outColor = m_arrLights[1]->Specular();
            break;
        case RENDERING_CONST_LIGHT1_ATTENUATION:
            if ( m_arrLights[1] != NULL ) {
                outVector->X = m_arrLights[1]->FadeConstant();
                outVector->Y = m_arrLights[1]->FadeLinear();
                outVector->Z = m_arrLights[1]->FadeQuadratic();
                outVector->W = m_arrLights[1]->Intensity();
            }
            break;
        case RENDERING_CONST_LIGHT1_SPOTCUTOFF:
            if ( m_arrLights[1] != NULL ) {
                outVector->X = m_arrLights[1]->SpotAngle();
                outVector->Y = m_arrLights[1]->SpotSin();
                outVector->Z = m_arrLights[1]->SpotCos();
                outVector->W = m_arrLights[1]->SpotExp();
            }
            break;
        case RENDERING_CONST_LIGHT2_POSITION:
            if ( m_arrLights[2] != NULL )
                *outVertex = m_arrLights[2]->Position();
            break;
        case RENDERING_CONST_LIGHT2_DIRECTION:
            if ( m_arrLights[2] != NULL )
                *outVector = m_arrLights[2]->Direction();
            break;
        case RENDERING_CONST_LIGHT2_AMBIENT:
            if ( m_arrLights[2] != NULL )
                *outColor = m_arrLights[2]->Ambient();
            break;
        case RENDERING_CONST_LIGHT2_DIFFUSE:
            if ( m_arrLights[2] != NULL )
                *outColor = m_arrLights[2]->Diffuse();
            break;
        case RENDERING_CONST_LIGHT2_SPECULAR:
            if ( m_arrLights[2] != NULL )
                *outColor = m_arrLights[2]->Specular();
            break;
        case RENDERING_CONST_LIGHT2_ATTENUATION:
            if ( m_arrLights[2] != NULL ) {
                outVector->X = m_arrLights[2]->FadeConstant();
                outVector->Y = m_arrLights[2]->FadeLinear();
                outVector->Z = m_arrLights[2]->FadeQuadratic();
                outVector->W = m_arrLights[2]->Intensity();
            }
            break;
        case RENDERING_CONST_LIGHT2_SPOTCUTOFF:
            if ( m_arrLights[2] != NULL ) {
                outVector->X = m_arrLights[2]->SpotAngle();
                outVector->Y = m_arrLights[2]->SpotSin();
                outVector->Z = m_arrLights[2]->SpotCos();
                outVector->W = m_arrLights[2]->SpotExp();
            }
            break;
        case RENDERING_CONST_LIGHT3_POSITION:
            if ( m_arrLights[3] != NULL )
                *outVertex = m_arrLights[3]->Position();
            break;
        case RENDERING_CONST_LIGHT3_DIRECTION:
            if ( m_arrLights[3] != NULL )
                *outVector = m_arrLights[3]->Direction();
            break;
        case RENDERING_CONST_LIGHT3_AMBIENT:
            if ( m_arrLights[3] != NULL )
                *outColor = m_arrLights[3]->Ambient();
            break;
        case RENDERING_CONST_LIGHT3_DIFFUSE:
            if ( m_arrLights[3] != NULL )
                *outColor = m_arrLights[3]->Diffuse();
            break;
        case RENDERING_CONST_LIGHT3_SPECULAR:
            if ( m_arrLights[3] != NULL )
                *outColor = m_arrLights[3]->Specular();
            break;
        case RENDERING_CONST_LIGHT3_ATTENUATION:
            if ( m_arrLights[3] != NULL ) {
                outVector->X = m_arrLights[3]->FadeConstant();
                outVector->Y = m_arrLights[3]->FadeLinear();
                outVector->Z = m_arrLights[3]->FadeQuadratic();
                outVector->W = m_arrLights[3]->Intensity();
            }
            break;
        case RENDERING_CONST_LIGHT3_SPOTCUTOFF:
            if ( m_arrLights[3] != NULL ) {
                outVector->X = m_arrLights[3]->SpotAngle();
                outVector->Y = m_arrLights[3]->SpotSin();
                outVector->Z = m_arrLights[3]->SpotCos();
                outVector->W = m_arrLights[3]->SpotExp();
            }
            break;
        case RENDERING_CONST_LIGHT4_POSITION:
            if ( m_arrLights[4] != NULL )
                *outVertex = m_arrLights[4]->Position();
            break;
        case RENDERING_CONST_LIGHT4_DIRECTION:
            if ( m_arrLights[4] != NULL )
                *outVector = m_arrLights[4]->Direction();
            break;
        case RENDERING_CONST_LIGHT4_AMBIENT:
            if ( m_arrLights[4] != NULL )
                *outColor = m_arrLights[4]->Ambient();
            break;
        case RENDERING_CONST_LIGHT4_DIFFUSE:
            if ( m_arrLights[4] != NULL )
                *outColor = m_arrLights[4]->Diffuse();
            break;
        case RENDERING_CONST_LIGHT4_SPECULAR:
            if ( m_arrLights[4] != NULL )
                *outColor = m_arrLights[4]->Specular();
            break;
        case RENDERING_CONST_LIGHT4_ATTENUATION:
            if ( m_arrLights[4] != NULL ) {
                outVector->X = m_arrLights[4]->FadeConstant();
                outVector->Y = m_arrLights[4]->FadeLinear();
                outVector->Z = m_arrLights[4]->FadeQuadratic();
                outVector->W = m_arrLights[4]->Intensity();
            }
            break;
        case RENDERING_CONST_LIGHT4_SPOTCUTOFF:
            if ( m_arrLights[4] != NULL ) {
                outVector->X = m_arrLights[4]->SpotAngle();
                outVector->Y = m_arrLights[4]->SpotSin();
                outVector->Z = m_arrLights[4]->SpotCos();
                outVector->W = m_arrLights[4]->SpotExp();
            }
            break;
        case RENDERING_CONST_LIGHT5_POSITION:
            if ( m_arrLights[5] != NULL )
                *outVertex = m_arrLights[5]->Position();
            break;
        case RENDERING_CONST_LIGHT5_DIRECTION:
            if ( m_arrLights[5] != NULL )
                *outVector = m_arrLights[5]->Direction();
            break;
        case RENDERING_CONST_LIGHT5_AMBIENT:
            if ( m_arrLights[5] != NULL )
                *outColor = m_arrLights[5]->Ambient();
            break;
        case RENDERING_CONST_LIGHT5_DIFFUSE:
            if ( m_arrLights[5] != NULL )
                *outColor = m_arrLights[5]->Diffuse();
            break;
        case RENDERING_CONST_LIGHT5_SPECULAR:
            if ( m_arrLights[5] != NULL )
                *outColor = m_arrLights[5]->Specular();
            break;
        case RENDERING_CONST_LIGHT5_ATTENUATION:
            if ( m_arrLights[5] != NULL ) {
                outVector->X = m_arrLights[5]->FadeConstant();
                outVector->Y = m_arrLights[5]->FadeLinear();
                outVector->Z = m_arrLights[5]->FadeQuadratic();
                outVector->W = m_arrLights[5]->Intensity();
            }
            break;
        case RENDERING_CONST_LIGHT5_SPOTCUTOFF:
            if ( m_arrLights[5] != NULL ) {
                outVector->X = m_arrLights[5]->SpotAngle();
                outVector->Y = m_arrLights[5]->SpotSin();
                outVector->Z = m_arrLights[5]->SpotCos();
                outVector->W = m_arrLights[5]->SpotExp();
            }
            break;
        case RENDERING_CONST_LIGHT6_POSITION:
            if ( m_arrLights[6] != NULL )
                *outVertex = m_arrLights[6]->Position();
            break;
        case RENDERING_CONST_LIGHT6_DIRECTION:
            if ( m_arrLights[6] != NULL )
                *outVector = m_arrLights[6]->Direction();
            break;
        case RENDERING_CONST_LIGHT6_AMBIENT:
            if ( m_arrLights[6] != NULL )
                *outColor = m_arrLights[6]->Ambient();
            break;
        case RENDERING_CONST_LIGHT6_DIFFUSE:
            if ( m_arrLights[6] != NULL )
                *outColor = m_arrLights[6]->Diffuse();
            break;
        case RENDERING_CONST_LIGHT6_SPECULAR:
            if ( m_arrLights[6] != NULL )
                *outColor = m_arrLights[6]->Specular();
            break;
        case RENDERING_CONST_LIGHT6_ATTENUATION:
            if ( m_arrLights[6] != NULL ) {
                outVector->X = m_arrLights[6]->FadeConstant();
                outVector->Y = m_arrLights[6]->FadeLinear();
                outVector->Z = m_arrLights[6]->FadeQuadratic();
                outVector->W = m_arrLights[6]->Intensity();
            }
            break;
        case RENDERING_CONST_LIGHT6_SPOTCUTOFF:
            if ( m_arrLights[6] != NULL ) {
                outVector->X = m_arrLights[6]->SpotAngle();
                outVector->Y = m_arrLights[6]->SpotSin();
                outVector->Z = m_arrLights[6]->SpotCos();
                outVector->W = m_arrLights[6]->SpotExp();
            }
            break;
        case RENDERING_CONST_LIGHT7_POSITION:
            if ( m_arrLights[7] != NULL )
                *outVertex = m_arrLights[7]->Position();
            break;
        case RENDERING_CONST_LIGHT7_DIRECTION:
            if ( m_arrLights[7] != NULL )
                *outVector = m_arrLights[7]->Direction();
            break;
        case RENDERING_CONST_LIGHT7_AMBIENT:
            if ( m_arrLights[7] != NULL )
                *outColor = m_arrLights[7]->Ambient();
            break;
        case RENDERING_CONST_LIGHT7_DIFFUSE:
            if ( m_arrLights[7] != NULL )
                *outColor = m_arrLights[7]->Diffuse();
            break;
        case RENDERING_CONST_LIGHT7_SPECULAR:
            if ( m_arrLights[7] != NULL )
                *outColor = m_arrLights[7]->Specular();
            break;
        case RENDERING_CONST_LIGHT7_ATTENUATION:
            if ( m_arrLights[7] != NULL ) {
                outVector->X = m_arrLights[7]->FadeConstant();
                outVector->Y = m_arrLights[7]->FadeLinear();
                outVector->Z = m_arrLights[7]->FadeQuadratic();
                outVector->W = m_arrLights[7]->Intensity();
            }
            break;
        case RENDERING_CONST_LIGHT7_SPOTCUTOFF:
            if ( m_arrLights[7] != NULL ) {
                outVector->X = m_arrLights[7]->SpotAngle();
                outVector->Y = m_arrLights[7]->SpotSin();
                outVector->Z = m_arrLights[7]->SpotCos();
                outVector->W = m_arrLights[7]->SpotExp();
            }
            break;
        case RENDERING_CONST_UNDEFINED:
        case RENDERING_CONST_COUNT:
        default: // undefined constant type
            break;
    }
}


