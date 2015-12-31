/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/ShaderLibrary/ShaderLibrary.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Software implementation for lots of common-use shaders
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
#include "ShaderLibrary.h"

/////////////////////////////////////////////////////////////////////////////////
// LibVertexShaderProgram implementation
LibVertexShaderProgram::LibVertexShaderProgram( const KChar * strLoadString ):
    VertexShaderProgram( strLoadString, false )
{
    // nothing to do
}
LibVertexShaderProgram::~LibVertexShaderProgram()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// LibPixelShaderProgram implementation
LibPixelShaderProgram::LibPixelShaderProgram( const KChar * strLoadString ):
    PixelShaderProgram( strLoadString, false )
{
    // nothing to do
}
LibPixelShaderProgram::~LibPixelShaderProgram()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ShaderLibrary implementation
ShaderLibrary::ShaderLibrary()
{
    // nothing to do
}
ShaderLibrary::~ShaderLibrary()
{
    // nothing to do
}

Void ShaderLibrary::Initialize( MemoryAllocatorID idAllocator )
{
    MemoryAllocatorID idSaved = MemoryFn->GetAllocator();
    MemoryFn->SelectAllocator( idAllocator );

    m_arrVShaders[VSHADER_DEFAULT_NULL] = KNewIm() VertexShader_Default_Null();
    m_arrVShaders[VSHADER_DEFAULT_COLOR] = KNewIm() VertexShader_Default_Color();
    m_arrVShaders[VSHADER_DEFAULT_TEXTURE1D] = KNewIm() VertexShader_Default_Texture1D();
    m_arrVShaders[VSHADER_DEFAULT_TEXTURE2D] = KNewIm() VertexShader_Default_Texture2D();
    m_arrVShaders[VSHADER_GOURAUD_COLOR] = KNewIm() VertexShader_Gouraud_Color();
    m_arrVShaders[VSHADER_GOURAUD_TEXTURE1D] = KNewIm() VertexShader_Gouraud_Texture1D();
    m_arrVShaders[VSHADER_GOURAUD_TEXTURE2D] = KNewIm() VertexShader_Gouraud_Texture2D();
    m_arrVShaders[VSHADER_PHONG_COLOR] = KNewIm() VertexShader_Phong_Color();
    m_arrVShaders[VSHADER_PHONG_TEXTURE1D] = KNewIm() VertexShader_Phong_Texture1D();
    m_arrVShaders[VSHADER_PHONG_TEXTURE2D] = KNewIm() VertexShader_Phong_Texture2D();

    m_arrPShaders[PSHADER_DEFAULT_NULL] = KNewIm() PixelShader_Default_Null();
    m_arrPShaders[PSHADER_DEFAULT_COLOR] = KNewIm() PixelShader_Default_Color();
    m_arrPShaders[PSHADER_DEFAULT_TEXTURE1D] = KNewIm() PixelShader_Default_Texture1D();
    m_arrPShaders[PSHADER_DEFAULT_TEXTURE2D] = KNewIm() PixelShader_Default_Texture2D();
    m_arrPShaders[PSHADER_GOURAUD_COLOR] = KNewIm() PixelShader_Gouraud_Color();
    m_arrPShaders[PSHADER_GOURAUD_TEXTURE1D] = KNewIm() PixelShader_Gouraud_Texture1D();
    m_arrPShaders[PSHADER_GOURAUD_TEXTURE2D] = KNewIm() PixelShader_Gouraud_Texture2D();
    m_arrPShaders[PSHADER_PHONG_COLOR] = KNewIm() PixelShader_Phong_Color();
    m_arrPShaders[PSHADER_PHONG_TEXTURE1D] = KNewIm() PixelShader_Phong_Texture1D();
    m_arrPShaders[PSHADER_PHONG_TEXTURE2D] = KNewIm() PixelShader_Phong_Texture2D();

    MemoryFn->SelectAllocator( idSaved );
}
Void ShaderLibrary::Cleanup( MemoryAllocatorID idAllocator )
{
    MemoryAllocatorID idSaved = MemoryFn->GetAllocator();
    MemoryFn->SelectAllocator( idAllocator );

    UInt i;
    for(i = 0; i < PSHADER_COUNT; ++i)
        KDeleteIm(m_arrPShaders[i]);
    for(i = 0; i < VSHADER_COUNT; ++i)
        KDeleteIm(m_arrVShaders[i]);

    MemoryFn->SelectAllocator( idSaved );
}
