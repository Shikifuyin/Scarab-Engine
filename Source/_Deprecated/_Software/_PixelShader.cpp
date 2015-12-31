/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/PixelShader.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Delegate of the Renderer, handles pixel processing
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "PixelShader.h"
#include "Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// PixelShader implementation
PixelShader::PixelShader( Renderer * pRenderer )
{
    m_pRenderer = pRenderer;
    m_pInputCFL = NULL;

    UInt i;

    for(i = 0; i < PIXELSHADER_MAX_PROGRAMS; ++i)
        m_arrPrograms[i] = NULL;
    m_iProgramCount = 0;
    m_pActiveProgram = NULL;

    for(i = 0; i < PIXELSHADER_REGISTERS_SIZE; ++i)
        m_arrRegisters[i] = 0.0f;
    for(i = 0; i < PIXELSHADER_MAX_SAMPLERS; ++i)
        m_arrSamplers[i] = NULL;
}
PixelShader::~PixelShader()
{
    // User is responsible for destroying his programs
    // nothing to do
}

UInt PixelShader::RegisterProgram( PixelShaderProgram * pProgram )
{
    Assert( pProgram != NULL );
    Assert( m_iProgramCount < PIXELSHADER_MAX_PROGRAMS );

    UInt iID = m_iProgramCount;
    m_arrPrograms[iID] = pProgram;
    ++m_iProgramCount;
    return iID;
}
Void PixelShader::DestroyPrograms( Bool bResetDefault )
{
    for( UInt i = 0; i < PIXELSHADER_MAX_PROGRAMS; ++i )
        m_arrPrograms[i] = NULL;
    m_iProgramCount = 0;
    m_pActiveProgram = NULL;

    if (bResetDefault) {
        m_arrPrograms[0] = m_pRenderer->_GetDefaultPS();
        m_iProgramCount = 1;
        m_pActiveProgram = m_arrPrograms[0];
        m_pInputCFL = m_pActiveProgram->GetInputCFL();
        m_pRenderer->_OnSelectPS();
    }
}

Void PixelShader::SelectProgram(UInt iProgID)
{
    Assert( iProgID < m_iProgramCount );
    m_pActiveProgram = m_arrPrograms[iProgID];
    m_pInputCFL = m_pActiveProgram->GetInputCFL();
    m_pRenderer->_OnSelectPS();
}

Void PixelShader::ComputeMipmapParameters( CCFLPtr pPixelA, CCFLPtr pPixelB, CCFLPtr pPixelC )
{
    for(UInt i = 0; i < PIXELSHADER_MAX_SAMPLERS; ++i) {
        if (m_arrSamplers[i] != NULL)
            m_arrSamplers[i]->ComputeMipmapParameters( pPixelA, pPixelB, pPixelC, m_pInputCFL, i );
    }
}
Void PixelShader::ProcessPixel( UInt iX, UInt iY, CCFLPtr pPixel )
{
    FrameBuffer * pFrame = m_pRenderer->m_pFrame;
    StateTransform * pStateTransform = m_pRenderer->_GetStateTransform();
    StateDepth * pStateDepth = m_pRenderer->_GetStateDepth();
    StateStencil * pStateStencil = m_pRenderer->_GetStateStencil();
    StateBlending * pStateBlending = m_pRenderer->_GetStateBlending();

    UInt iPixel = pFrame->Width() * iY + iX;

    // Stencil test
    if ( pFrame->HasStencilBuffer() ) {
        if ( pStateStencil->IsEnabled() ) {
            if ( !_StencilCompare(iPixel) ) {
                _StencilOperation(iPixel, pStateStencil->GetOpFail());
                return;
            }
        }
    }

    // Depth test
    if ( pFrame->HasDepthBuffer() ) {
        if ( pStateDepth->IsEnabled() ) {
            Scalar fZ;
            if ( pStateTransform->UsePerspective() )
                fZ = MathFn->Invert( *( m_pInputCFL->GetField( pPixel, CFLSEMANTIC_INVERTW, 0 ) ) );
            else
                fZ = *( m_pInputCFL->GetField( pPixel, CFLSEMANTIC_DEPTH, 0 ) );
            UInt iDepth = ( (UInt)(MathFn->Round(fZ)) ) & pFrame->MaxDepthValue();
            if ( !_DepthCompare(iPixel, iDepth) ) {
                if ( pStateStencil->IsEnabled() )
                    _StencilOperation(iPixel, pStateStencil->GetOpDepthFail());
                return;
            }
            if ( pStateDepth->IsWritable() )
                pFrame->UpdateDepth( iPixel, fZ, iDepth );
            if ( pStateStencil->IsEnabled() )
                _StencilOperation( iPixel, pStateStencil->GetOpDepthSuccess() );
        }
    }

    // Draw the pixel
    if ( pFrame->HasColorBuffer() ) {
        // Prepare samplers & execute shader
        for(UInt i = 0; i < PIXELSHADER_MAX_SAMPLERS; ++i) {
            if ( m_arrSamplers[i] != NULL && m_arrSamplers[i]->UseMipMapping() )
                m_arrSamplers[i]->SetPixel( iX, iY );
        }

        Color4 colSrc(Color4::Black);
        if (m_pActiveProgram != NULL)
            m_pActiveProgram->Execute( colSrc, pPixel, m_arrRegisters, m_arrSamplers );
        colSrc.Clamping();

        // No blending case
        if ( !pStateBlending->IsEnabled() ) {
            pFrame->UpdateColor( iPixel, iX, iY, colSrc );
            return;
        }

        // Blending
        Color4 colFinal;
        _Blending( colFinal, colSrc, pFrame->GetColorF(iPixel) );
        colFinal.Clamping();
        pFrame->UpdateColor( iPixel, iX, iY, colFinal );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Bool PixelShader::_DepthCompare( UInt iPixel, UInt iDepth ) const
{
    UInt iCurDepth = (UInt)( m_pRenderer->m_pFrame->GetDepthI(iPixel) );
    switch( m_pRenderer->_GetStateDepth()->GetCompareMode() ) {
        case STATE_DEPTH_NEVER:         return false;
        case STATE_DEPTH_ALLWAYS:       return true;
        case STATE_DEPTH_EQUAL:         return (iDepth == iCurDepth);
        case STATE_DEPTH_NOTEQUAL:      return (iDepth != iCurDepth);
        case STATE_DEPTH_LESSER:        return (iDepth <  iCurDepth);
        case STATE_DEPTH_LESSER_EQUAL:  return (iDepth <= iCurDepth);
        case STATE_DEPTH_GREATER:       return (iDepth >  iCurDepth);
        case STATE_DEPTH_GREATER_EQUAL: return (iDepth >= iCurDepth);
        default: return false;
    }
    return false;
}
Bool PixelShader::_StencilCompare( UInt iPixel ) const
{
    StateStencil * pStateStencil = m_pRenderer->_GetStateStencil();
    UInt iCurStencil = (UInt)( m_pRenderer->m_pFrame->GetStencil(iPixel) );
    UInt iReference = pStateStencil->GetReference();
    UInt iMask = pStateStencil->GetMask();
    switch( pStateStencil->GetCompareMode() ) {
        case STATE_STENCIL_NEVER:         return false;
        case STATE_STENCIL_ALLWAYS:       return true;
        case STATE_STENCIL_EQUAL:         return ( (iReference & iMask) == (iCurStencil & iMask) );
        case STATE_STENCIL_NOTEQUAL:      return ( (iReference & iMask) != (iCurStencil & iMask) );
        case STATE_STENCIL_LESSER:        return ( (iReference & iMask) <  (iCurStencil & iMask) );
        case STATE_STENCIL_LESSER_EQUAL:  return ( (iReference & iMask) <= (iCurStencil & iMask) );
        case STATE_STENCIL_GREATER:       return ( (iReference & iMask) >  (iCurStencil & iMask) );
        case STATE_STENCIL_GREATER_EQUAL: return ( (iReference & iMask) >= (iCurStencil & iMask) );
        default: return false;
    }
    return false;
}
Void PixelShader::_StencilOperation( UInt iPixel, StateStencilOperation iOp ) const
{
    FrameBuffer * pFrame = m_pRenderer->m_pFrame;
    StateStencil * pStateStencil = m_pRenderer->_GetStateStencil();
    UInt iMaxStencil = (UInt)( pFrame->MaxStencilValue() );
    UInt iCurStencil = (UInt)( pFrame->GetStencil(iPixel) );
    UInt iNewStencil = 0;
    UInt iWriteMask = pStateStencil->GetWriteMask();
    switch( iOp ) {
        case STATE_STENCIL_OP_KEEP:
            iNewStencil = iCurStencil;
            break;
        case STATE_STENCIL_OP_ZERO:
            iNewStencil = 0;
            break;
        case STATE_STENCIL_OP_REPLACE:
            iNewStencil = pStateStencil->GetReference();
            break;
        case STATE_STENCIL_OP_INCREMENT:
            iNewStencil = iCurStencil;
            ++iNewStencil;
            if ( iNewStencil > iMaxStencil )
                iNewStencil = iMaxStencil;
            break;
        case STATE_STENCIL_OP_DECREMENT:
            iNewStencil = iCurStencil;
            if (iNewStencil > 0)
                --iNewStencil;
            break;
        case STATE_STENCIL_OP_INVERT:
            iNewStencil = ~iCurStencil;
            break;
        default: break;
    }
    pFrame->GetStencil(iPixel) = ( (iCurStencil & ~iWriteMask) | (iNewStencil & iWriteMask) );
}
Void PixelShader::_Blending( Color4 & outColor, const Color4 & colSrc, const Color4 & colDst ) const
{
    StateBlending * pStateBlending = m_pRenderer->_GetStateBlending();
    switch( pStateBlending->GetSrcBlend() ) {
        case STATE_BLENDING_SRC_ZERO:
            outColor.R = 0.0f;
            outColor.G = 0.0f;
            outColor.B = 0.0f;
            outColor.A = 0.0f;
            break;
        case STATE_BLENDING_SRC_ONE:              outColor = colSrc; break;
        case STATE_BLENDING_SRC_DST_COLOR:        outColor = colSrc * colDst; break;
        case STATE_BLENDING_SRC_DST_COLOR_INVERT: outColor = colSrc * (Color4::White - colDst); break;
        case STATE_BLENDING_SRC_SRC_ALPHA:        outColor = colSrc * colSrc.A; break;
        case STATE_BLENDING_SRC_SRC_ALPHA_INVERT: outColor = colSrc * (1.0f - colSrc.A); break;
        case STATE_BLENDING_SRC_SRC_ALPHA_SATURATE: {
                Scalar fMin = (1.0f - colDst.A);
                if (colSrc.A < fMin)
                    fMin = colSrc.A;
                outColor.R = fMin * colSrc.R;
                outColor.G = fMin * colSrc.G;
                outColor.B = fMin * colSrc.B;
                outColor.A = colSrc.A;
            } break;
        case STATE_BLENDING_SRC_DST_ALPHA:          outColor = colSrc * colDst.A; break;
        case STATE_BLENDING_SRC_DST_ALPHA_INVERT:   outColor = colSrc * (1.0f - colDst.A); break;
        case STATE_BLENDING_SRC_CONST_COLOR:        outColor = colSrc * pStateBlending->GetConstColor(); break;
        case STATE_BLENDING_SRC_CONST_COLOR_INVERT: outColor = colSrc * (Color4::White - pStateBlending->GetConstColor()); break;
        case STATE_BLENDING_SRC_CONST_ALPHA:        outColor = colSrc * pStateBlending->GetConstColor().A; break;
        case STATE_BLENDING_SRC_CONST_ALPHA_INVERT: outColor = colSrc * (1.0f - pStateBlending->GetConstColor().A); break;
        default: break;
    }
    switch( pStateBlending->GetDstBlend() ) {
        case STATE_BLENDING_DST_ZERO:               /*nothing to do*/ break;
        case STATE_BLENDING_DST_ONE:                outColor += colDst; break;
        case STATE_BLENDING_DST_SRC_COLOR:          outColor += colDst * colSrc; break;
        case STATE_BLENDING_DST_SRC_COLOR_INVERT:   outColor += colDst * (Color4::White - colSrc); break;
        case STATE_BLENDING_DST_SRC_ALPHA:          outColor += colDst * colSrc.A; break;
        case STATE_BLENDING_DST_SRC_ALPHA_INVERT:   outColor += colDst * (1.0f - colSrc.A); break;
        case STATE_BLENDING_DST_DST_ALPHA:          outColor += colDst * colDst.A; break;
        case STATE_BLENDING_DST_DST_ALPHA_INVERT:   outColor += colDst * (1.0f - colDst.A); break;
        case STATE_BLENDING_DST_CONST_COLOR:        outColor += colDst * pStateBlending->GetConstColor(); break;
        case STATE_BLENDING_DST_CONST_COLOR_INVERT: outColor += colDst * (Color4::White - pStateBlending->GetConstColor()); break;
        case STATE_BLENDING_DST_CONST_ALPHA:        outColor += colDst * pStateBlending->GetConstColor().A; break;
        case STATE_BLENDING_DST_CONST_ALPHA_INVERT: outColor += colDst * (1.0f - pStateBlending->GetConstColor().A); break;
        default: break;
    }
}

