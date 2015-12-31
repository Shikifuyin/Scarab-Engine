/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/ShaderLibrary/Gouraud.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Gouraud shading ...
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
#ifndef KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_GOURAUD_H
#define KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_GOURAUD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ShaderLibrary.h"

/////////////////////////////////////////////////////////////////////////////////
// Vertex CFLs
typedef struct _vertexcfl_gouraud_in_color
{
    Vertex4 vPosition;
    Vector4 vNormal;
    Color4 vColor;
} VertexCFL_Gouraud_In_Color;
typedef struct _vertexcfl_gouraud_out_color
{
    Vertex4 vPosition;
    Color4 vColor;
    Color4 vDiffuse;
    Color4 vSpecular;
} VertexCFL_Gouraud_Out_Color;

typedef struct _vertexcfl_gouraud_in_texture1d
{
    Vertex4 vPosition;
    Vector4 vNormal;
    TextureCoord1 vTexCoord;
} VertexCFL_Gouraud_In_Texture1D;
typedef struct _vertexcfl_gouraud_out_texture1d
{
    Vertex4 vPosition;
    TextureCoord1 vTexCoord;
    Color4 vDiffuse;
    Color4 vSpecular;
} VertexCFL_Gouraud_Out_Texture1D;

typedef struct _vertexcfl_gouraud_in_texture2d
{
    Vertex4 vPosition;
    Vector4 vNormal;
    TextureCoord2 vTexCoord;
} VertexCFL_Gouraud_In_Texture2D;
typedef struct _vertexcfl_gouraud_out_texture2d
{
    Vertex4 vPosition;
    TextureCoord2 vTexCoord;
    Color4 vDiffuse;
    Color4 vSpecular;
} VertexCFL_Gouraud_Out_Texture2D;

/////////////////////////////////////////////////////////////////////////////////
// Pixel CFLs
typedef struct _pixelcfl_gouraud_color
{
    Vertex2 vRasterPos; Scalar fDepth; Scalar fInvW; Scalar fRegW;
    Color4 vColor;
    Color4 vDiffuse;
    Color4 vSpecular;
} PixelCFL_Gouraud_Color;

typedef struct _pixelcfl_gouraud_texture1d
{
    Vertex2 vRasterPos; Scalar fDepth; Scalar fInvW; Scalar fRegW;
    TextureCoord1 vTexCoord;
    Color4 vDiffuse;
    Color4 vSpecular;
} PixelCFL_Gouraud_Texture1D;

typedef struct _pixelcfl_gouraud_texture2d
{
    Vertex2 vRasterPos; Scalar fDepth; Scalar fInvW; Scalar fRegW;
    TextureCoord2 vTexCoord;
    Color4 vDiffuse;
    Color4 vSpecular;
} PixelCFL_Gouraud_Texture2D;

/////////////////////////////////////////////////////////////////////////////////
// Vertex Shaders
class VertexShader_Gouraud_Color : public LibVertexShaderProgram
{
public:
    VertexShader_Gouraud_Color();
    ~VertexShader_Gouraud_Color();
    inline VShaderLibID GetID() const;
    Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters );
private:
    static const KChar * sm_strCode;
};

class VertexShader_Gouraud_Texture1D : public LibVertexShaderProgram
{
public:
    VertexShader_Gouraud_Texture1D();
    ~VertexShader_Gouraud_Texture1D();
    inline VShaderLibID GetID() const;
    Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters );
private:
    static const KChar * sm_strCode;
};

class VertexShader_Gouraud_Texture2D : public LibVertexShaderProgram
{
public:
    VertexShader_Gouraud_Texture2D();
    ~VertexShader_Gouraud_Texture2D();
    inline VShaderLibID GetID() const;
    Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters );
private:
    static const KChar * sm_strCode;
};

/////////////////////////////////////////////////////////////////////////////////
// Pixel Shaders
class PixelShader_Gouraud_Color : public LibPixelShaderProgram
{
public:
    PixelShader_Gouraud_Color();
    ~PixelShader_Gouraud_Color();
    inline PShaderLibID GetID() const;
    Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers );
private:
    static const KChar * sm_strCode;
};

class PixelShader_Gouraud_Texture1D : public LibPixelShaderProgram
{
public:
    PixelShader_Gouraud_Texture1D();
    ~PixelShader_Gouraud_Texture1D();
    inline PShaderLibID GetID() const;
    Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers );
private:
    static const KChar * sm_strCode;
};

class PixelShader_Gouraud_Texture2D : public LibPixelShaderProgram
{
public:
    PixelShader_Gouraud_Texture2D();
    ~PixelShader_Gouraud_Texture2D();
    inline PShaderLibID GetID() const;
    Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers );
private:
    static const KChar * sm_strCode;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Gouraud.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_GOURAUD_H


