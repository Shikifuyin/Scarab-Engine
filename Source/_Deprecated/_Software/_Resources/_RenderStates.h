/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/RenderStates.h
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
// Header prelude
#ifndef KOALA_ENGINE_GRAPHICS_3D_RENDERSTATES_H
#define KOALA_ENGINE_GRAPHICS_3D_RENDERSTATES_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Core/Math/Types/Matrix.h"
#include "../../../Lib/Core/Math/Types/Color.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum RenderStateType
{
    RENDERSTATE_TRANSFORM = 0,
    RENDERSTATE_CULLING,
    RENDERSTATE_CLIPPING,
    RENDERSTATE_DEPTH,
    RENDERSTATE_STENCIL,
    RENDERSTATE_BLENDING,
    RENDERSTATE_COUNT
};

    // Culling state
enum StateCullingTest
{
    STATE_CULLING_FRONT = 0,
    STATE_CULLING_BACK,
    STATE_CULLING_COUNT
};
enum StateCullingConvention
{
    STATE_CULLING_CONV_CLOCKWISE = 0,
    STATE_CULLING_CONV_COUNTER_CLOCKWISE,
    STATE_CULLING_CONV_COUNT
};

    // Depth state
enum StateDepthCompare
{
    STATE_DEPTH_NEVER = 0,
    STATE_DEPTH_ALLWAYS,
    STATE_DEPTH_EQUAL,
    STATE_DEPTH_NOTEQUAL,
    STATE_DEPTH_LESSER,
    STATE_DEPTH_LESSER_EQUAL,
    STATE_DEPTH_GREATER,
    STATE_DEPTH_GREATER_EQUAL,
    STATE_DEPTH_COUNT
};

    // Stencil state
enum StateStencilCompare
{
    STATE_STENCIL_NEVER = 0,
    STATE_STENCIL_ALLWAYS,
    STATE_STENCIL_EQUAL,
    STATE_STENCIL_NOTEQUAL,
    STATE_STENCIL_LESSER,
    STATE_STENCIL_LESSER_EQUAL,
    STATE_STENCIL_GREATER,
    STATE_STENCIL_GREATER_EQUAL,
    STATE_STENCIL_COUNT
};
enum StateStencilOperation
{
    STATE_STENCIL_OP_KEEP = 0,
    STATE_STENCIL_OP_ZERO,
    STATE_STENCIL_OP_REPLACE,
    STATE_STENCIL_OP_INCREMENT,
    STATE_STENCIL_OP_DECREMENT,
    STATE_STENCIL_OP_INVERT,
    STATE_STENCIL_OP_COUNT,
};

    // Blending state
enum StateBlendingSource
{
    STATE_BLENDING_SRC_ZERO = 0,
    STATE_BLENDING_SRC_ONE,
    STATE_BLENDING_SRC_DST_COLOR,
    STATE_BLENDING_SRC_DST_COLOR_INVERT,
    STATE_BLENDING_SRC_SRC_ALPHA,
    STATE_BLENDING_SRC_SRC_ALPHA_INVERT,
    STATE_BLENDING_SRC_SRC_ALPHA_SATURATE,
    STATE_BLENDING_SRC_DST_ALPHA,
    STATE_BLENDING_SRC_DST_ALPHA_INVERT,
    STATE_BLENDING_SRC_CONST_COLOR,
    STATE_BLENDING_SRC_CONST_COLOR_INVERT,
    STATE_BLENDING_SRC_CONST_ALPHA,
    STATE_BLENDING_SRC_CONST_ALPHA_INVERT,
    STATE_BLENDING_SRC_COUNT
};
enum StateBlendingDestination
{
    STATE_BLENDING_DST_ZERO = 0,
    STATE_BLENDING_DST_ONE,
    STATE_BLENDING_DST_SRC_COLOR,
    STATE_BLENDING_DST_SRC_COLOR_INVERT,
    STATE_BLENDING_DST_SRC_ALPHA,
    STATE_BLENDING_DST_SRC_ALPHA_INVERT,
    STATE_BLENDING_DST_DST_ALPHA,
    STATE_BLENDING_DST_DST_ALPHA_INVERT,
    STATE_BLENDING_DST_CONST_COLOR,
    STATE_BLENDING_DST_CONST_COLOR_INVERT,
    STATE_BLENDING_DST_CONST_ALPHA,
    STATE_BLENDING_DST_CONST_ALPHA_INVERT,
    STATE_BLENDING_DST_COUNT
};
enum StateBlendingTest
{
    STATE_BLENDING_TEST_NEVER = 0,
    STATE_BLENDING_TEST_ALLWAYS,
    STATE_BLENDING_TEST_EQUAL,
    STATE_BLENDING_TEST_NOTEQUAL,
    STATE_BLENDING_TEST_LESSER,
    STATE_BLENDING_TEST_LESSER_EQUAL,
    STATE_BLENDING_TEST_GREATER,
    STATE_BLENDING_TEST_GREATER_EQUAL,
    STATE_BLENDING_TEST_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The RenderState interface
class RenderState
{
public:
    virtual ~RenderState();

    virtual RenderStateType GetType() const = 0;

protected:
    RenderState();
};

/////////////////////////////////////////////////////////////////////////////////
// The StateTransform class
class StateTransform : public RenderState
{
public:
    StateTransform();
    virtual ~StateTransform();

    inline virtual RenderStateType GetType() const;

    inline Bool UsePerspective() const;
    inline Bool UseWireframe() const;
    inline Bool UsePostWorld() const;
    inline Bool UsePostProject() const;
    inline const Matrix4 & GetPostWorldMatrix() const;
    inline const Matrix4 & GetPostProjectMatrix() const;

    inline Void SetPerspective( Bool bUsePerspective );
    inline Void SetWireframe( Bool bUseWireframe );
    inline Void SetPostWorld( Bool bUsePostWorld );
    inline Void SetPostProject( Bool bUsePostProject );
    inline Void SetPostWorldMatrix( const Matrix4 & matPostWorld );
    inline Void SetPostProjectMatrix( const Matrix4 & matPostProject );

private:
    Bool m_bUsePerspective;   // default = true
    Bool m_bUseWireframe;     // default = false
    Bool m_bUsePostWorld;     // default = false
    Bool m_bUsePostProject;   // default = false
    Matrix4 m_matPostWorld;   // default = Matrix4::Identity
    Matrix4 m_matPostProject; // default = Matrix4::Identity
};

/////////////////////////////////////////////////////////////////////////////////
// The StateCulling class
class StateCulling : public RenderState
{
public:
    inline static const KChar * GetTestName( StateCullingTest iTestMode );
    inline static const KChar * GetConvName( StateCullingConvention iConvention );

    StateCulling();
    virtual ~StateCulling();

    inline virtual RenderStateType GetType() const;

    inline Bool IsEnabled() const;
    inline StateCullingTest GetCullTest() const;
    inline StateCullingConvention GetCullConv() const;

    inline Void SetEnabled( Bool bEnabled);
    inline Void SetCullTest( StateCullingTest iTestMode );
    inline Void SetCullConv( StateCullingConvention iConvention );

private:
    static const KChar * sm_arrTestNames[STATE_CULLING_COUNT];
    static const KChar * sm_arrConventionNames[STATE_CULLING_CONV_COUNT];

    Bool m_bEnabled;                      // default = true
    StateCullingTest m_iTestMode;         // default = STATE_CULLING_BACK
    StateCullingConvention m_iConvention; // default = STATE_CULLING_CONV_COUNTER_CLOCKWISE    
};

/////////////////////////////////////////////////////////////////////////////////
// The StateClipping class
class StateClipping : public RenderState
{
public:
    StateClipping();
    virtual ~StateClipping();

    inline virtual RenderStateType GetType() const;

    inline Bool UseFrustrumClipping() const;
    inline Bool RejectClipped() const;

    inline Bool UseClipSpaceClipping() const;

private:
    Bool m_bUseFrustrumClipping;  // default = true
    Bool m_bRejectClipped;        // default = true
    Bool m_bUseClipSpaceClipping; // default = true
};

/////////////////////////////////////////////////////////////////////////////////
// The StateDepth class
class StateDepth : public RenderState
{
public:
    inline static const KChar * GetCompareName( StateDepthCompare iCompareMode );

    StateDepth();
    virtual ~StateDepth();

    inline virtual RenderStateType GetType() const;

    inline Bool IsEnabled() const;
    inline Bool IsWritable() const;
    inline StateDepthCompare GetCompareMode() const;

    inline Void SetEnabled( Bool bEnabled );
    inline Void SetWritable( Bool bWritable );
    inline Void SetCompareMode( StateDepthCompare iCompareMode );

private:
    static const KChar * sm_arrCompareNames[STATE_DEPTH_COUNT];

    Bool m_bEnabled;                  // default = true
    Bool m_bWritable;                 // default = true
    StateDepthCompare m_iCompareMode; // default = STATE_DEPTH_LESSER_EQUAL    
};

/////////////////////////////////////////////////////////////////////////////////
// The StateStencil class
class StateStencil : public RenderState
{
public:
    inline static const KChar * GetCompareName( StateStencilCompare iCompareMode );
    inline static const KChar * GetOpName( StateStencilOperation iOp );

    StateStencil();
    virtual ~StateStencil();

    inline virtual RenderStateType GetType() const;

    inline Bool IsEnabled() const;
    inline UInt GetReference() const;
    inline UInt GetMask() const;
    inline UInt GetWriteMask() const;
    inline StateStencilCompare GetCompareMode() const;
    inline StateStencilOperation GetOpFail() const;
    inline StateStencilOperation GetOpDepthFail() const;
    inline StateStencilOperation GetOpDepthSuccess() const;

    inline Void SetEnabled( Bool bEnabled );
    inline Void SetReference( UInt iReference );
    inline Void SetMask( UInt iMask );
    inline Void SetWriteMask( UInt iWriteMask );
    inline Void SetCompareMode( StateStencilCompare iCompareMode );
    inline Void SetOpFail( StateStencilOperation iOp );
    inline Void SetOpDepthFail( StateStencilOperation iOp );
    inline Void SetOpDepthSuccess( StateStencilOperation iOp );

private:
    static const KChar * sm_arrCompareNames[STATE_STENCIL_COUNT];
    static const KChar * sm_arrOperationNames[STATE_STENCIL_OP_COUNT];

    Bool m_bEnabled;                         // default = false
    UInt m_iReference;                       // default = 0
    UInt m_iMask;                            // default = 0xffffffff
    UInt m_iWriteMask;                       // default = 0xffffffff
    StateStencilCompare m_iCompareMode;      // default = STATE_STENCIL_NEVER
    StateStencilOperation m_iOnStencilFail;  // default = STATE_STENCIL_OP_KEEP
    StateStencilOperation m_iOnDepthFail;    // default = STATE_STENCIL_OP_KEEP
    StateStencilOperation m_iOnDepthSuccess; // default = STATE_STENCIL_OP_KEEP
};

/////////////////////////////////////////////////////////////////////////////////
// The StateBlending class
class StateBlending : public RenderState
{
public:
    inline static const KChar * GetSrcBlendName( StateBlendingSource iSrcBlend );
    inline static const KChar * GetDstBlendName( StateBlendingDestination iDstBlend );
    inline static const KChar * GetTestName( StateBlendingTest iTestMode );

    StateBlending();
    virtual ~StateBlending();

    inline virtual RenderStateType GetType() const;

    inline Bool IsEnabled() const;
    inline Bool IsTestEnabled() const;
    inline Scalar GetReference() const;
    inline const Color4 & GetConstColor() const;
    inline StateBlendingSource GetSrcBlend() const;
    inline StateBlendingDestination GetDstBlend() const;
    inline StateBlendingTest GetTestMode() const;

    inline Void SetEnabled( Bool bEnabled );
    inline Void SetTestEnabled( Bool bTestEnabled );
    inline Void SetReference( Scalar fReference );
    inline Void SetConstColor( const Color4 & colConst );
    inline Void SetSrcBlend( StateBlendingSource iSrcBlend );
    inline Void SetDstBlend( StateBlendingDestination iDstBlend );
    inline Void SetTestMode( StateBlendingTest iTestMode );

private:
    static const KChar * sm_arrSrcBlendNames[STATE_BLENDING_SRC_COUNT];
    static const KChar * sm_arrDstBlendNames[STATE_BLENDING_DST_COUNT];
    static const KChar * sm_arrTestNames[STATE_BLENDING_TEST_COUNT];

    Bool m_bEnabled;                      // default = false
    Bool m_bTestEnabled;                  // default = false
    Scalar m_fReference;                  // default = 0, ranges in [0;1]
    Color4 m_colConst;                    // default = Black
    StateBlendingSource m_iSrcBlend;      // default = STATE_BLENDING_SRC_SRC_ALPHA
    StateBlendingDestination m_iDstBlend; // default = STATE_BLENDING_DST_SRC_ALPHA_INVERT
    StateBlendingTest m_iTestMode;        // default = STATE_BLENDING_TEST_ALLWAYS
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "RenderStates.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_RENDERSTATES_H

