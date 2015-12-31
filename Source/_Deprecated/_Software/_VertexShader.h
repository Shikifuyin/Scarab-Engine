/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/VertexShader.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Delegate of the Renderer, handles vertex processing
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
#ifndef KOALA_ENGINE_GRAPHICS_3D_VERTEXSHADER_H
#define KOALA_ENGINE_GRAPHICS_3D_VERTEXSHADER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Buffering.h"
#include "Shader.h"
#include "RenderStates.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
class Renderer;

#define VERTEXSHADER_MAX_PROGRAMS      128

#define VERTEXSHADER_REGISTER_CHANNELS SHADER_REGISTER_CHANNELS
#define VERTEXSHADER_REGISTERS         256
#define VERTEXSHADER_REGISTERS_SIZE    (VERTEXSHADER_REGISTER_CHANNELS * VERTEXSHADER_REGISTERS)

/////////////////////////////////////////////////////////////////////////////////
// The VertexShader class
class VertexShader
{
public:
    VertexShader( Renderer * pRenderer );
    ~VertexShader();

    inline CustomFragmentLayout * GetInputCFL() const;
    inline CustomFragmentLayout * GetOutputCFL() const;

    inline VertexShaderProgram * GetActiveProgram() const;
    inline VertexShaderProgram * GetProgram( UInt iProgID ) const;

    UInt RegisterProgram( VertexShaderProgram * pProgram );
    Void DestroyPrograms( Bool bResetDefault );
    Void SelectProgram( UInt iProgID );

    inline Void Prepare();
    Void ProcessVertex( CCFLPtr pVertex );

private:
    Renderer * m_pRenderer;

    CustomFragmentLayout * m_pInputCFL;
    CustomFragmentLayout * m_pOutputCFL;

    VertexShaderProgram * m_arrPrograms[VERTEXSHADER_MAX_PROGRAMS];
    UInt m_iProgramCount;
    VertexShaderProgram * m_pActiveProgram;
    Scalar m_arrRegisters[VERTEXSHADER_REGISTERS_SIZE];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "VertexShader.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_VERTEXSHADER_H
