/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/VertexShader.cpp
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
// Includes
#include "VertexShader.h"
#include "Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// VertexShader implementation
VertexShader::VertexShader( Renderer * pRenderer )
{
    m_pRenderer = pRenderer;
    m_pInputCFL = NULL;
    m_pOutputCFL = NULL;

    UInt i;

    for(i = 0; i < VERTEXSHADER_MAX_PROGRAMS; ++i)
        m_arrPrograms[i] = NULL;
    m_iProgramCount = 0;
    m_pActiveProgram = NULL;

    for(i = 0; i < VERTEXSHADER_REGISTERS_SIZE; ++i)
        m_arrRegisters[i] = 0.0f;
}
VertexShader::~VertexShader()
{
    // User is responsible for destroying his programs
    // nothing to do
}

UInt VertexShader::RegisterProgram( VertexShaderProgram * pProgram )
{
    Assert( pProgram != NULL );
    Assert( m_iProgramCount < VERTEXSHADER_MAX_PROGRAMS );

    UInt iID = m_iProgramCount;
    m_arrPrograms[iID] = pProgram;
    ++m_iProgramCount;
    return iID;
}
Void VertexShader::DestroyPrograms( Bool bResetDefault )
{
    for( UInt i = 0; i < VERTEXSHADER_MAX_PROGRAMS; ++i )
        m_arrPrograms[i] = NULL;
    m_iProgramCount = 0;
    m_pActiveProgram = NULL;

    if (bResetDefault) {
        m_arrPrograms[0] = m_pRenderer->_GetDefaultVS();
        m_iProgramCount = 1;
        m_pActiveProgram = m_arrPrograms[0];
        m_pInputCFL = m_pActiveProgram->GetInputCFL();
        m_pOutputCFL = m_pActiveProgram->GetOutputCFL();
        m_pRenderer->_OnSelectVS();
    }
}

Void VertexShader::SelectProgram( UInt iProgID )
{
    Assert( iProgID < m_iProgramCount );
    m_pActiveProgram = m_arrPrograms[iProgID];
    m_pInputCFL = m_pActiveProgram->GetInputCFL();
    m_pOutputCFL = m_pActiveProgram->GetOutputCFL();
    m_pRenderer->_OnSelectVS();
}

Void VertexShader::ProcessVertex( CCFLPtr pVertex )
{
    CFLPtr outShadedVertex = m_pOutputCFL->Allocate();
    if (m_pActiveProgram != NULL)
        m_pActiveProgram->Execute( outShadedVertex, pVertex, m_arrRegisters );
    m_pRenderer->_VertexShaderOutput_Vertex( outShadedVertex );
}
