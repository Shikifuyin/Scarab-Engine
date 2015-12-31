/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/ShaderLibrary/Default.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Default very-very-basic shaders tu ensure minimal functions
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
#ifndef KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_DEFAULT_H
#define KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_DEFAULT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ShaderLibrary.h"

/////////////////////////////////////////////////////////////////////////////////
// Vertex CFLs
typedef struct _vertexcfl_default_null
{
    Vertex4 vPosition;
} VertexCFL_Default_Null;

typedef struct _vertexcfl_default_color
{
    Vertex4 vPosition;
    Color4 vColor;
} VertexCFL_Default_Color;

typedef struct _vertexcfl_default_texture1d
{
    Vertex4 vPosition;
    TextureCoord1 vTexCoord;
} VertexCFL_Default_Texture1D;

typedef struct _vertexcfl_default_texture2d
{
    Vertex4 vPosition;
    TextureCoord2 vTexCoord;
} VertexCFL_Default_Texture2D;

/////////////////////////////////////////////////////////////////////////////////
// Pixel CFLs
typedef struct _pixelcfl_default_null
{
    Vertex2 vRasterPos; Scalar fDepth; Scalar fInvW; Scalar fRegW;
} PixelCFL_Default_Null;

typedef struct _pixelcfl_default_color
{
    Vertex2 vRasterPos; Scalar fDepth; Scalar fInvW; Scalar fRegW;
    Color4 vColor;
} PixelCFL_Default_Color;

typedef struct _pixelcfl_default_texture1d
{
    Vertex2 vRasterPos; Scalar fDepth; Scalar fInvW; Scalar fRegW;
    TextureCoord1 vTexCoord;
} PixelCFL_Default_Texture1D;

typedef struct _pixelcfl_default_texture2d
{
    Vertex2 vRasterPos; Scalar fDepth; Scalar fInvW; Scalar fRegW;
    TextureCoord2 vTexCoord;
} PixelCFL_Default_Texture2D;

/////////////////////////////////////////////////////////////////////////////////
// Vertex Shaders
class VertexShader_Default_Null : public LibVertexShaderProgram
{
public:
    VertexShader_Default_Null();
    ~VertexShader_Default_Null();
    inline VShaderLibID GetID() const;
    Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters );
private:
    static const KChar * sm_strCode;
};

class VertexShader_Default_Color : public LibVertexShaderProgram
{
public:
    VertexShader_Default_Color();
    ~VertexShader_Default_Color();
    inline VShaderLibID GetID() const;
    Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters );
private:
    static const KChar * sm_strCode;
};

class VertexShader_Default_Texture1D : public LibVertexShaderProgram
{
public:
    VertexShader_Default_Texture1D();
    ~VertexShader_Default_Texture1D();
    inline VShaderLibID GetID() const;
    Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters );
private:
    static const KChar * sm_strCode;
};

class VertexShader_Default_Texture2D : public LibVertexShaderProgram
{
public:
    VertexShader_Default_Texture2D();
    ~VertexShader_Default_Texture2D();
    inline VShaderLibID GetID() const;
    Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters );
private:
    static const KChar * sm_strCode;
};

/////////////////////////////////////////////////////////////////////////////////
// Pixel Shaders
class PixelShader_Default_Null : public LibPixelShaderProgram
{
public:
    PixelShader_Default_Null();
    ~PixelShader_Default_Null();
    inline PShaderLibID GetID() const;
    Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers );
private:
    static const KChar * sm_strCode;
};

class PixelShader_Default_Color : public LibPixelShaderProgram
{
public:
    PixelShader_Default_Color();
    ~PixelShader_Default_Color();
    inline PShaderLibID GetID() const;
    Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers );
private:
    static const KChar * sm_strCode;
};

class PixelShader_Default_Texture1D : public LibPixelShaderProgram
{
public:
    PixelShader_Default_Texture1D();
    ~PixelShader_Default_Texture1D();
    inline PShaderLibID GetID() const;
    Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers );
private:
    static const KChar * sm_strCode;
};

class PixelShader_Default_Texture2D : public LibPixelShaderProgram
{
public:
    PixelShader_Default_Texture2D();
    ~PixelShader_Default_Texture2D();
    inline PShaderLibID GetID() const;
    Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers );
private:
    static const KChar * sm_strCode;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Default.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_DEFAULT_H


