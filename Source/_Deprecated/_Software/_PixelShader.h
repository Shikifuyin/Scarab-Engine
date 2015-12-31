/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/PixelShader.h
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
// Header prelude
#ifndef KOALA_ENGINE_GRAPHICS_3D_PIXELSHADER_H
#define KOALA_ENGINE_GRAPHICS_3D_PIXELSHADER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Buffering.h"
#include "Shader.h"
#include "RenderStates.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
class Renderer;

#define PIXELSHADER_MAX_PROGRAMS      128
#define PIXELSHADER_MAX_SAMPLERS      8

#define PIXELSHADER_REGISTER_CHANNELS SHADER_REGISTER_CHANNELS
#define PIXELSHADER_REGISTERS         256
#define PIXELSHADER_REGISTERS_SIZE    (PIXELSHADER_REGISTER_CHANNELS * PIXELSHADER_REGISTERS)

/////////////////////////////////////////////////////////////////////////////////
// The PixelShader class
class PixelShader
{
public:
    PixelShader( Renderer * pRenderer );
    ~PixelShader();

    inline CustomFragmentLayout * GetInputCFL() const;

    inline PixelShaderProgram * GetActiveProgram() const;
    inline PixelShaderProgram * GetProgram( UInt iProgID ) const;

    UInt RegisterProgram( PixelShaderProgram * pProgram );
    Void DestroyPrograms( Bool bResetDefault );
    Void SelectProgram( UInt iProgID );

    inline Void Prepare();
    Void ComputeMipmapParameters( CCFLPtr pPixelA, CCFLPtr pPixelB, CCFLPtr pPixelC );
    Void ProcessPixel( UInt iX, UInt iY, CCFLPtr pPixel );

private:
    Bool _DepthCompare( UInt iPixel, UInt iDepth ) const;
    Bool _StencilCompare( UInt iPixel ) const;
    Void _StencilOperation( UInt iPixel, StateStencilOperation iOp ) const;
    Void _Blending( Color4 & outColor, const Color4 & colSrc, const Color4 & colDst ) const;

    Renderer * m_pRenderer;

    CustomFragmentLayout * m_pInputCFL;

    PixelShaderProgram * m_arrPrograms[PIXELSHADER_MAX_PROGRAMS];
    UInt m_iProgramCount;
    PixelShaderProgram * m_pActiveProgram;
    Scalar m_arrRegisters[PIXELSHADER_REGISTERS_SIZE];
    Sampler * m_arrSamplers[PIXELSHADER_MAX_SAMPLERS];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PixelShader.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_PIXELSHADER_H
