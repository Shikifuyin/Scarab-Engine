/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/RenderStates.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : States for the 3D renderer, internal
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// StateTransform implementation
inline RenderStateType StateTransform::GetType() const {
    return RENDERSTATE_TRANSFORM;
}

inline Bool StateTransform::UsePerspective() const {
    return m_bUsePerspective;
}
inline Bool StateTransform::UseWireframe() const {
    return m_bUseWireframe;
}
inline Bool StateTransform::UsePostWorld() const {
    return m_bUsePostWorld;
}
inline Bool StateTransform::UsePostProject() const {
    return m_bUsePostProject;
}
inline const Matrix4 & StateTransform::GetPostWorldMatrix() const {
    return m_matPostWorld;
}
inline const Matrix4 & StateTransform::GetPostProjectMatrix() const {
    return m_matPostProject;
}

inline Void StateTransform::SetPerspective( Bool bUsePerspective ) {
    m_bUsePerspective = bUsePerspective;
}
inline Void StateTransform::SetWireframe( Bool bUseWireframe ) {
    m_bUseWireframe = bUseWireframe;
}
inline Void StateTransform::SetPostWorld( Bool bUsePostWorld ) {
    m_bUsePostWorld = bUsePostWorld;
}
inline Void StateTransform::SetPostProject( Bool bUsePostProject ) {
    m_bUsePostProject = bUsePostProject;
}
inline Void StateTransform::SetPostWorldMatrix( const Matrix4 & matPostWorld ) {
    m_matPostWorld = matPostWorld;
}
inline Void StateTransform::SetPostProjectMatrix( const Matrix4 & matPostProject ) {
    m_matPostProject = matPostProject;
}

/////////////////////////////////////////////////////////////////////////////////
// StateCulling implementation
inline const KChar * StateCulling::GetTestName( StateCullingTest iTestMode ) {
    return sm_arrTestNames[iTestMode];
}
inline const KChar * StateCulling::GetConvName( StateCullingConvention iConvention ) {
    return sm_arrConventionNames[iConvention];
}

inline RenderStateType StateCulling::GetType() const {
    return RENDERSTATE_CULLING;
}

inline Bool StateCulling::IsEnabled() const {
    return m_bEnabled;
}
inline StateCullingTest StateCulling::GetCullTest() const {
    return m_iTestMode;
}
inline StateCullingConvention StateCulling::GetCullConv() const {
    return m_iConvention;
}

inline Void StateCulling::SetEnabled( Bool bEnabled ) {
    m_bEnabled = bEnabled;
}
inline Void StateCulling::SetCullTest( StateCullingTest iTestMode ) {
    m_iTestMode = iTestMode;
}
inline Void StateCulling::SetCullConv( StateCullingConvention iConvention ) {
    m_iConvention = iConvention;
}

/////////////////////////////////////////////////////////////////////////////////
// StateClipping implementation
inline RenderStateType StateClipping::GetType() const {
    return RENDERSTATE_CLIPPING;
}

inline Bool StateClipping::UseFrustrumClipping() const {
    return m_bUseFrustrumClipping;
}
inline Bool StateClipping::RejectClipped() const {
    return m_bRejectClipped;
}

inline Bool StateClipping::UseClipSpaceClipping() const {
    return m_bUseClipSpaceClipping;
}

/////////////////////////////////////////////////////////////////////////////////
// StateDepth implementation
inline const KChar * StateDepth::GetCompareName( StateDepthCompare iCompareMode ) {
    return sm_arrCompareNames[iCompareMode];
}

inline RenderStateType StateDepth::GetType() const {
    return RENDERSTATE_DEPTH;
}

inline Bool StateDepth::IsEnabled() const {
    return m_bEnabled;
}
inline Bool StateDepth::IsWritable() const {
    return m_bWritable;
}
inline StateDepthCompare StateDepth::GetCompareMode() const {
    return m_iCompareMode;
}

inline Void StateDepth::SetEnabled( Bool bEnabled ) {
    m_bEnabled = bEnabled;
}
inline Void StateDepth::SetWritable( Bool bWritable ) {
    m_bWritable = bWritable;
}
inline Void StateDepth::SetCompareMode( StateDepthCompare iCompareMode ) {
    m_iCompareMode = iCompareMode;
}

/////////////////////////////////////////////////////////////////////////////////
// StateStencil implementation
inline const KChar * StateStencil::GetCompareName( StateStencilCompare iCompareMode ) {
    return sm_arrCompareNames[iCompareMode];
}
inline const KChar * StateStencil::GetOpName( StateStencilOperation iOp ) {
    return sm_arrOperationNames[iOp];
}

inline RenderStateType StateStencil::GetType() const {
    return RENDERSTATE_STENCIL;
}

inline Bool StateStencil::IsEnabled() const {
    return m_bEnabled;
}
inline UInt StateStencil::GetReference() const {
    return m_iReference;
}
inline UInt StateStencil::GetMask() const {
    return m_iMask;
}
inline UInt StateStencil::GetWriteMask() const {
    return m_iWriteMask;
}
inline StateStencilCompare StateStencil::GetCompareMode() const {
    return m_iCompareMode;
}
inline StateStencilOperation StateStencil::GetOpFail() const {
    return m_iOnStencilFail;
}
inline StateStencilOperation StateStencil::GetOpDepthFail() const {
    return m_iOnDepthFail;
}
inline StateStencilOperation StateStencil::GetOpDepthSuccess() const {
    return m_iOnDepthSuccess;
}

inline Void StateStencil::SetEnabled( Bool bEnabled ) {
    m_bEnabled = bEnabled;
}
inline Void StateStencil::SetReference( UInt iReference ) {
    m_iReference = iReference;
}
inline Void StateStencil::SetMask( UInt iMask ) {
    m_iMask = iMask;
}
inline Void StateStencil::SetWriteMask( UInt iWriteMask ) {
    m_iWriteMask = iWriteMask;
}
inline Void StateStencil::SetCompareMode( StateStencilCompare iCompareMode ) {
    m_iCompareMode = iCompareMode;
}
inline Void StateStencil::SetOpFail( StateStencilOperation iOp ) {
    m_iOnStencilFail = iOp;
}
inline Void StateStencil::SetOpDepthFail( StateStencilOperation iOp ) {
    m_iOnDepthFail = iOp;
}
inline Void StateStencil::SetOpDepthSuccess( StateStencilOperation iOp ) {
    m_iOnDepthSuccess = iOp;
}

/////////////////////////////////////////////////////////////////////////////////
// StateBlending implementation
inline const KChar * StateBlending::GetSrcBlendName( StateBlendingSource iSrcBlend ) {
    return sm_arrSrcBlendNames[iSrcBlend];
}
inline const KChar * StateBlending::GetDstBlendName( StateBlendingDestination iDstBlend ) {
    return sm_arrDstBlendNames[iDstBlend];
}
inline const KChar * StateBlending::GetTestName( StateBlendingTest iTestMode ) {
    return sm_arrTestNames[iTestMode];
}

inline RenderStateType StateBlending::GetType() const {
    return RENDERSTATE_BLENDING;
}

inline Bool StateBlending::IsEnabled() const {
    return m_bEnabled;
}
inline Bool StateBlending::IsTestEnabled() const {
    return m_bTestEnabled;
}
inline Scalar StateBlending::GetReference() const {
    return m_fReference;
}
inline const Color4 & StateBlending::GetConstColor() const {
    return m_colConst;
}
inline StateBlendingSource StateBlending::GetSrcBlend() const {
    return m_iSrcBlend;
}
inline StateBlendingDestination StateBlending::GetDstBlend() const {
    return m_iDstBlend;
}
inline StateBlendingTest StateBlending::GetTestMode() const {
    return m_iTestMode;
}

inline Void StateBlending::SetEnabled( Bool bEnabled ) {
    m_bEnabled = bEnabled;
}
inline Void StateBlending::SetTestEnabled( Bool bTestEnabled ) {
    m_bTestEnabled = bTestEnabled;
}
inline Void StateBlending::SetReference( Scalar fReference ) {
    m_fReference = fReference;
}
inline Void StateBlending::SetConstColor( const Color4 & colConst ) {
    m_colConst = colConst;
}
inline Void StateBlending::SetSrcBlend( StateBlendingSource iSrcBlend ) {
    m_iSrcBlend = iSrcBlend;
}
inline Void StateBlending::SetDstBlend( StateBlendingDestination iDstBlend ) {
    m_iDstBlend = iDstBlend;
}
inline Void StateBlending::SetTestMode( StateBlendingTest iTestMode ) {
    m_iTestMode = iTestMode;
}


