/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/ShaderLibrary/ShaderLibrary.h
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
// Header prelude
#ifndef KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_SHADERLIBRARY_H
#define KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_SHADERLIBRARY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Shader.h"
#include "../VertexShader.h"
#include "../PixelShader.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ShaderLib ShaderLibrary::GetInstancePtr()

enum VShaderLibID
{
    VSHADER_DEFAULT_NULL = 0,
    VSHADER_DEFAULT_COLOR,
    VSHADER_DEFAULT_TEXTURE1D,
    VSHADER_DEFAULT_TEXTURE2D,
    VSHADER_GOURAUD_COLOR,
    VSHADER_GOURAUD_TEXTURE1D,
    VSHADER_GOURAUD_TEXTURE2D,
    VSHADER_PHONG_COLOR,
    VSHADER_PHONG_TEXTURE1D,
    VSHADER_PHONG_TEXTURE2D,
    VSHADER_COUNT
};
enum PShaderLibID
{
    PSHADER_DEFAULT_NULL = 0,
    PSHADER_DEFAULT_COLOR,
    PSHADER_DEFAULT_TEXTURE1D,
    PSHADER_DEFAULT_TEXTURE2D,
    PSHADER_GOURAUD_COLOR,
    PSHADER_GOURAUD_TEXTURE1D,
    PSHADER_GOURAUD_TEXTURE2D,
    PSHADER_PHONG_COLOR,
    PSHADER_PHONG_TEXTURE1D,
    PSHADER_PHONG_TEXTURE2D,
    PSHADER_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The LibVertexShaderProgram class
class LibVertexShaderProgram : public VertexShaderProgram
{
public:
    virtual ~LibVertexShaderProgram();

    virtual VShaderLibID GetID() const = 0;

    virtual Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters ) = 0;

protected:
    friend class ShaderLibrary;
    LibVertexShaderProgram( const KChar * strLoadString );
};

/////////////////////////////////////////////////////////////////////////////////
// The LibPixelShaderProgram class
class LibPixelShaderProgram : public PixelShaderProgram
{
public:
    virtual ~LibPixelShaderProgram();

    virtual PShaderLibID GetID() const = 0;

    virtual Void Execute( Color4 & outColor, CCFLPtr pVertex, Scalar * arrRegisters, Sampler ** arrSamplers ) = 0;

protected:
    friend class ShaderLibrary;
    LibPixelShaderProgram( const KChar * strLoadString );
};

/////////////////////////////////////////////////////////////////////////////////
// The ShaderLibrary class
class ShaderLibrary
{
    // Discrete singleton interface
public:
    inline static ShaderLibrary * GetInstancePtr();

private:
    ShaderLibrary();
    ~ShaderLibrary();

public:
    Void Initialize( MemoryAllocatorID idAllocator );
    Void Cleanup( MemoryAllocatorID idAllocator );

    inline LibVertexShaderProgram * GetVS( VShaderLibID idVS ) const;
    inline LibPixelShaderProgram * GetPS( PShaderLibID idPS ) const;

private:
    LibVertexShaderProgram * m_arrVShaders[VSHADER_COUNT];
    LibPixelShaderProgram * m_arrPShaders[PSHADER_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ShaderLibrary.inl"

// Backward-include all shaders in the lib ...
#include "Default.h"
#include "Gouraud.h"
#include "Phong.h"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_SHADERLIBRARY_H


