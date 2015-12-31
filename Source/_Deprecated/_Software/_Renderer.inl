/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Renderer.inl
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
// Renderer implementation
inline const KChar * Renderer::GetName() {
    return TEXT("Renderer");
}

inline PixelMap * Renderer::GetRenderTarget() const {
    Assert(m_bInitialized);
    return m_pRenderTarget;
}
inline PixelMap * Renderer::GetWindow() const {
    Assert(m_bInitialized);
    return m_pWindow;
}
inline UInt Renderer::WinWidth() const {
    Assert(m_bInitialized);
    return m_iWinWidth;
}
inline UInt Renderer::WinHeight() const {
    Assert(m_bInitialized);
    return m_iWinHeight;
}

inline RenderState * Renderer::GetState( RenderStateType iType ) {
    Assert(m_bInitialized);
    Assert( iType < RENDERSTATE_COUNT );
    return m_arrStates[iType];
}

inline CustomFragmentLayout * Renderer::CreateCFL() {
    Assert(m_bInitialized);
    return KNewExA(m_idAllocCFLPool) CustomFragmentLayout();
}
inline Void Renderer::DestroyCFL( CustomFragmentLayout * pCFL ) {
    Assert(m_bInitialized);
    KDeleteExA(pCFL, m_idAllocCFLPool);
}

inline VertexBuffer * Renderer::CreateVertexBuffer( const CustomFragmentLayout * pCFL, UInt iCapacity ) {
    Assert(m_bInitialized);
    return KNewExA(m_idAllocHeap) VertexBuffer( pCFL, iCapacity, m_idAllocBufferHeap );
}
inline Void Renderer::DestroyVertexBuffer( VertexBuffer * pVB ) {
    Assert(m_bInitialized);
    KDeleteExA(pVB, m_idAllocHeap);
}

inline IndexBuffer * Renderer::CreateIndexBuffer( IndexBufferMode iMode, UInt iCapacity ) {
    Assert(m_bInitialized);
    return KNewExA(m_idAllocHeap) IndexBuffer( iMode, iCapacity, m_idAllocBufferHeap );
}
inline Void Renderer::DestroyIndexBuffer( IndexBuffer * pIB ) {
    Assert(m_bInitialized);
    KDeleteExA(pIB, m_idAllocHeap);
}

inline Texture * Renderer::CreateTexture1D( const KChar * strName, TextureFormat iFormat, UInt iWidth ) {
    TextureData * pImage = KNewExA(m_idAllocHeap) TextureData( iFormat, NULL, iWidth );
    pImage->Allocate( m_idAllocTextureHeap );
    return KNewExA(m_idAllocHeap) Texture( strName, pImage );
}
inline Texture * Renderer::CreateTexture2D( const KChar * strName, TextureFormat iFormat, UInt iWidth, UInt iHeight ) {
    TextureData * pImage = KNewExA(m_idAllocHeap) TextureData( iFormat, NULL, iWidth, iHeight );
    pImage->Allocate( m_idAllocTextureHeap );
    return KNewExA(m_idAllocHeap) Texture( strName, pImage );
}
inline Texture * Renderer::CreateTexture3D( const KChar * strName, TextureFormat iFormat, UInt iWidth, UInt iHeight, UInt iDepth ) {
    TextureData * pImage = KNewExA(m_idAllocHeap) TextureData( iFormat, NULL, iWidth, iHeight, iDepth );
    pImage->Allocate( m_idAllocTextureHeap );
    return KNewExA(m_idAllocHeap) Texture( strName, pImage );
}
inline Void Renderer::DestroyTexture( Texture * pTexture ) {
    pTexture->GetData()->Destroy( m_idAllocTextureHeap );
    KDeleteExA( pTexture->GetData(), m_idAllocHeap );
    KDeleteExA( pTexture, m_idAllocHeap );
}

inline Material * Renderer::CreateMaterial() {
    Assert(m_bInitialized);
    return KNewExA(m_idAllocMaterialPool) Material();
}
inline Void Renderer::DestroyMaterial( Material * pMaterial ) {
    Assert(m_bInitialized);
    KDeleteExA(pMaterial, m_idAllocMaterialPool);
}

inline Light * Renderer::CreateLight( LightType iType ) {
    Assert(m_bInitialized);
    return KNewExA(m_idAllocLightPool) Light(iType);
}
inline Void Renderer::DestroyLight( Light * pLight ) {
    Assert(m_bInitialized);
    KDeleteExA(pLight, m_idAllocLightPool);
}

inline UInt Renderer::RegisterVertexShader( VertexShaderProgram * pProgram ) {
    Assert(m_bInitialized);
    return m_pVertexShader->RegisterProgram( pProgram );
}
inline UInt Renderer::RegisterPixelShader( PixelShaderProgram * pProgram ) {
    Assert(m_bInitialized);
    return m_pPixelShader->RegisterProgram( pProgram );
}
inline Void Renderer::DestroyVertexShaders() {
    Assert(m_bInitialized);
    m_pVertexShader->DestroyPrograms( true );
}
inline Void Renderer::DestroyPixelShaders() {
    Assert(m_bInitialized);
    m_pPixelShader->DestroyPrograms( true );
}
inline Void Renderer::SelectVertexShader( UInt iProgID ) {
    Assert(m_bInitialized);
    m_pVertexShader->SelectProgram( iProgID );
}
inline Void Renderer::SelectPixelShader( UInt iProgID ) {
    Assert(m_bInitialized);
    m_pPixelShader->SelectProgram( iProgID );
}
inline VertexShaderProgram * Renderer::GetVertexShader( UInt iProgID ) const {
    Assert(m_bInitialized);
    if (iProgID == INVALID_OFFSET)
        return m_pVertexShader->GetActiveProgram();
    return m_pVertexShader->GetProgram( iProgID );
}
inline PixelShaderProgram * Renderer::GetPixelShader( UInt iProgID ) const {
    Assert(m_bInitialized);
    if (iProgID == INVALID_OFFSET)
        return m_pPixelShader->GetActiveProgram();
    return m_pPixelShader->GetProgram( iProgID );
}

inline Void Renderer::RegisterShaderConstant( const KChar * strName, Scalar * pDataSource ) {
    Assert(m_bInitialized);
    (*m_phmShadersUserConstants)[strName] = pDataSource;
}

inline Void Renderer::SetView( const Vertex3 & vEye, const Vector3 & vDir, const Vector3 & vUp ) {
    Assert(m_bInitialized);
    m_pCamera->SetView( vEye, vDir, vUp );
}
inline Void Renderer::GetBillboardMatrix( Matrix4 & outBillboardMatrix ) const {
    Assert(m_bInitialized);
    m_pCamera->GetBillboardMatrix( outBillboardMatrix );
}

inline Void Renderer::SetFrustrum( Scalar fFOV, Scalar fAspect, Scalar fNear, Scalar fFar ) {
    Assert(m_bInitialized);
    m_pCamera->SetFrustrum( fFOV, fAspect, fNear, fFar );
}

inline Void Renderer::SetViewport( UInt iLeft, UInt iRight, UInt iBottom, UInt iTop ) {
    Assert(m_bInitialized);
    m_pCamera->SetViewport( iLeft, iRight, iBottom, iTop, m_iWinWidth, m_iWinHeight );
}
inline Void Renderer::SetDepthRange( Scalar fMinZ, Scalar fMaxZ ) {
    Assert(m_bInitialized);
    m_pCamera->SetDepthRange( fMinZ, fMaxZ );
}

inline Void Renderer::SetClearColor( const Color4 & clearColor ) {
    Assert(m_bInitialized);
    m_pFrame->SetClearColor(clearColor);
}
inline Void Renderer::SetClearDepth( Scalar clearDepth ) {
    Assert(m_bInitialized);
    m_pFrame->SetClearDepth(clearDepth);
}
inline Void Renderer::SetClearStencil( UInt clearStencil ) {
    Assert(m_bInitialized);
    m_pFrame->SetClearStencil(clearStencil);
}
inline Void Renderer::ClearFrame() {
    Assert(m_bInitialized);
    m_pFrame->ClearBuffers();
}
inline Void Renderer::ClearFrame( Bool bClearColors, Bool bClearDepth, Bool bClearStencil ) {
    Assert(m_bInitialized);
    if (bClearColors)
        m_pFrame->ClearColorBuffer();
    if (bClearDepth)
        m_pFrame->ClearDepthBuffer();
    if (bClearStencil)
        m_pFrame->ClearStencilBuffer();
}
inline Void Renderer::Flip() {
    Assert(m_bInitialized);
    SystemWindowFn->DisplayBackBuffer();
}

/////////////////////////////////////////////////////////////////////////////////

inline StateTransform * Renderer::_GetStateTransform() const { return (StateTransform*)(m_arrStates[RENDERSTATE_TRANSFORM]); }
inline StateCulling * Renderer::_GetStateCulling() const     { return (StateCulling*)(m_arrStates[RENDERSTATE_CULLING]); }
inline StateClipping * Renderer::_GetStateClipping() const   { return (StateClipping*)(m_arrStates[RENDERSTATE_CLIPPING]); }
inline StateDepth * Renderer::_GetStateDepth() const         { return (StateDepth*)(m_arrStates[RENDERSTATE_DEPTH]); }
inline StateStencil * Renderer::_GetStateStencil() const     { return (StateStencil*)(m_arrStates[RENDERSTATE_STENCIL]); }
inline StateBlending * Renderer::_GetStateBlending() const   { return (StateBlending*)(m_arrStates[RENDERSTATE_BLENDING]); }

inline LibVertexShaderProgram * Renderer::_GetDefaultVS() const {
    return ShaderLib->GetVS( m_idDefaultVS );
}
inline LibPixelShaderProgram * Renderer::_GetDefaultPS() const {
    return ShaderLib->GetPS( m_idDefaultPS );
}

inline Void Renderer::_GetShaderUserConstant( Scalar * outValues, UInt iSize, const KChar * strName ) const {
    Assert( m_phmShadersUserConstants->Contains(strName) );
    Scalar * pConstant = (*m_phmShadersUserConstants)[strName];
    for(UInt i = 0; i < iSize; ++i)
        outValues[i] = pConstant[i];
}
inline Texture * Renderer::_GetTexture( UInt iChannel ) const {
    Assert( iChannel < RENDERER_TEXTURE_CHANNELS );
    return m_arrTextureChannels[iChannel];
}
inline Sampler * Renderer::_GetSampler( UInt iChannel ) const {
    Assert( iChannel < RENDERER_TEXTURE_CHANNELS );
    return m_arrTextureSamplersChannels[iChannel];
}
