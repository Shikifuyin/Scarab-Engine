/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/RenderStates.cpp
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
// Includes
#include "RenderStates.h"

/////////////////////////////////////////////////////////////////////////////////
// RenderState implementation
RenderState::RenderState()
{
    // nothing to do
}
RenderState::~RenderState()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// StateTransform implementation
StateTransform::StateTransform():
    RenderState()
{
    m_bUsePerspective = true;
    m_bUseWireframe = false;
    m_bUsePostWorld = false;
    m_bUsePostProject = false;
    m_matPostWorld = Matrix4::Identity;
    m_matPostProject = Matrix4::Identity;
}
StateTransform::~StateTransform()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// StateCulling implementation
const KChar * StateCulling::sm_arrTestNames[STATE_CULLING_COUNT] = {
    TEXT("CullTest_Front"),
    TEXT("CullTest_Back")
};
const KChar * StateCulling::sm_arrConventionNames[STATE_CULLING_CONV_COUNT] = {
    TEXT("CullConv_CW"),
    TEXT("CullConv_CCW")
};

StateCulling::StateCulling():
    RenderState()
{
    m_bEnabled = true;
    m_iTestMode = STATE_CULLING_BACK;
    m_iConvention = STATE_CULLING_CONV_COUNTER_CLOCKWISE;
}
StateCulling::~StateCulling()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// StateClipping implementation
StateClipping::StateClipping():
    RenderState()
{
    m_bUseFrustrumClipping = true;
    m_bRejectClipped = true;

    m_bUseClipSpaceClipping = true;
}
StateClipping::~StateClipping()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// StateDepth implementation
const KChar * StateDepth::sm_arrCompareNames[STATE_DEPTH_COUNT] = {
    TEXT("DepthTest_Never"),
    TEXT("DepthTest_Allways"),
    TEXT("DepthTest_Equal"),
    TEXT("DepthTest_NotEqual"),
    TEXT("DepthTest_Lesser"),
    TEXT("DepthTest_LesserEqual"),
    TEXT("DepthTest_Greater"),
    TEXT("DepthTest_GreaterEqual")
};

StateDepth::StateDepth():
    RenderState()
{
    m_bEnabled = true;
    m_bWritable = true;
    m_iCompareMode = STATE_DEPTH_LESSER_EQUAL;
}
StateDepth::~StateDepth()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// StateStencil implementation
const KChar * StateStencil::sm_arrCompareNames[STATE_STENCIL_COUNT] = {
    TEXT("StencilTest_Never"),
    TEXT("StencilTest_Allways"),
    TEXT("StencilTest_Equal"),
    TEXT("StencilTest_NotEqual"),
    TEXT("StencilTest_Lesser"),
    TEXT("StencilTest_LesserEqual"),
    TEXT("StencilTest_Greater"),
    TEXT("StencilTest_GreaterEqual")
};
const KChar * StateStencil::sm_arrOperationNames[STATE_STENCIL_OP_COUNT] = {
    TEXT("StencilOp_Keep"),
    TEXT("StencilOp_Zero"),
    TEXT("StencilOp_Replace"),
    TEXT("StencilOp_Increment"),
    TEXT("StencilOp_Decrement"),
    TEXT("StencilOp_Invert")
};

StateStencil::StateStencil():
    RenderState()
{
    m_bEnabled = false;
    m_iReference = 0;
    m_iMask = 0xffffffff;
    m_iWriteMask = 0xffffffff;
    m_iCompareMode = STATE_STENCIL_NEVER;
    m_iOnStencilFail = STATE_STENCIL_OP_KEEP;
    m_iOnDepthFail = STATE_STENCIL_OP_KEEP;
    m_iOnDepthSuccess = STATE_STENCIL_OP_KEEP;
}
StateStencil::~StateStencil()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// StateBlending implementation
const KChar * StateBlending::sm_arrSrcBlendNames[STATE_BLENDING_SRC_COUNT] = {
    TEXT("SrcBlend_Zero"),
    TEXT("SrcBlend_One"),
    TEXT("SrcBlend_DstColor"),
    TEXT("SrcBlend_DstColorInv"),
    TEXT("SrcBlend_SrcAlpha"),
    TEXT("SrcBlend_SrcAlphaInv"),
    TEXT("SrcBlend_SrcAlphaSat"),
    TEXT("SrcBlend_DstAlpha"),
    TEXT("SrcBlend_DstAlphaInv"),
    TEXT("SrcBlend_ConstColor"),
    TEXT("SrcBlend_ConstColorInv"),
    TEXT("SrcBlend_ConstAlpha"),
    TEXT("SrcBlend_ConstAlphaInv")
};
const KChar * StateBlending::sm_arrDstBlendNames[STATE_BLENDING_DST_COUNT] = {
    TEXT("DstBlend_Zero"),
    TEXT("DstBlend_One"),
    TEXT("DstBlend_SrcColor"),
    TEXT("DstBlend_SrcColorInv"),
    TEXT("DstBlend_SrcAlpha"),
    TEXT("DstBlend_SrcAlphaInv"),
    TEXT("DstBlend_DstAlpha"),
    TEXT("DstBlend_DstAlphaInv"),
    TEXT("DstBlend_ConstColor"),
    TEXT("DstBlend_ConstColorInv"),
    TEXT("DstBlend_ConstAlpha"),
    TEXT("DstBlend_ConstAlphaInv"),
};
const KChar * StateBlending::sm_arrTestNames[STATE_BLENDING_TEST_COUNT] = {
    TEXT("BlendingTest_Never"),
    TEXT("BlendingTest_Allways"),
    TEXT("BlendingTest_Equal"),
    TEXT("BlendingTest_NotEqual"),
    TEXT("BlendingTest_Lesser"),
    TEXT("BlendingTest_LesserEqual"),
    TEXT("BlendingTest_Greater"),
    TEXT("BlendingTest_GreaterEqual")
};

StateBlending::StateBlending():
    RenderState(), m_colConst(Color4::Black)
{
    m_bEnabled = false;
    m_bTestEnabled = false;
    m_fReference = 0.0f;
    m_iSrcBlend = STATE_BLENDING_SRC_SRC_ALPHA;
    m_iDstBlend = STATE_BLENDING_DST_SRC_ALPHA_INVERT;
    m_iTestMode = STATE_BLENDING_TEST_ALLWAYS;
}
StateBlending::~StateBlending()
{
    // nothing to do
}

