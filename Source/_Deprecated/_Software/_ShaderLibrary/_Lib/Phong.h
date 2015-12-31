/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/ShaderLibrary/Phong.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Phong shading ...
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
#ifndef KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_PHONG_H
#define KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_PHONG_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ShaderLibrary.h"

/////////////////////////////////////////////////////////////////////////////////
// Vertex CFLs
typedef struct _vertexcfl_phong_in_color
{
    Vertex4 vPosition;
    Vector4 vNormal;
    Color4 vColor;
} VertexCFL_Phong_In_Color;
typedef struct _vertexcfl_phong_out_color
{
    Vertex4 vPosition;
    Vector4 vNormal;
    Color4 vColor;
    Vertex4 vWorldPos; // user data
} VertexCFL_Phong_Out_Color;

typedef struct _vertexcfl_phong_in_texture1d
{
    Vertex4 vPosition;
    Vector4 vNormal;
    TextureCoord1 vTexCoord;
} VertexCFL_Phong_In_Texture1D;
typedef struct _vertexcfl_phong_out_texture1d
{
    Vertex4 vPosition;
    Vector4 vNormal;
    TextureCoord1 vTexCoord;
    Vertex4 vWorldPos; // user data
} VertexCFL_Phong_Out_Texture1D;

typedef struct _vertexcfl_phong_in_texture2d
{
    Vertex4 vPosition;
    Vector4 vNormal;
    TextureCoord2 vTexCoord;
} VertexCFL_Phong_In_Texture2D;
typedef struct _vertexcfl_phong_out_texture2d
{
    Vertex4 vPosition;
    Vector4 vNormal;
    TextureCoord2 vTexCoord;
    Vertex4 vWorldPos; // user data
} VertexCFL_Phong_Out_Texture2D;

/////////////////////////////////////////////////////////////////////////////////
// Pixel CFLs
typedef struct _pixelcfl_phong_color
{
    Vertex2 vRasterPos; Scalar fDepth; Scalar fInvW; Scalar fRegW;
    Vector4 vNormal;
    Color4 vColor;
    Vertex4 vWorldPos; // user data
} PixelCFL_Phong_Color;

typedef struct _pixelcfl_phong_texture1d
{
    Vertex2 vRasterPos; Scalar fDepth; Scalar fInvW; Scalar fRegW;
    Vector4 vNormal;
    TextureCoord1 vTexCoord;
    Vertex4 vWorldPos; // user data
} PixelCFL_Phong_Texture1D;

typedef struct _pixelcfl_phong_texture2d
{
    Vertex2 vRasterPos; Scalar fDepth; Scalar fInvW; Scalar fRegW;
    Vector4 vNormal;
    TextureCoord2 vTexCoord;
    Vertex4 vWorldPos; // user data
} PixelCFL_Phong_Texture2D;

/////////////////////////////////////////////////////////////////////////////////
// Vertex Shaders
class VertexShader_Phong_Color : public LibVertexShaderProgram
{
public:
    VertexShader_Phong_Color();
    ~VertexShader_Phong_Color();
    inline VShaderLibID GetID() const;
    Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters );
private:
    static const KChar * sm_strCode;
};

class VertexShader_Phong_Texture1D : public LibVertexShaderProgram
{
public:
    VertexShader_Phong_Texture1D();
    ~VertexShader_Phong_Texture1D();
    inline VShaderLibID GetID() const;
    Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters );
private:
    static const KChar * sm_strCode;
};

class VertexShader_Phong_Texture2D : public LibVertexShaderProgram
{
public:
    VertexShader_Phong_Texture2D();
    ~VertexShader_Phong_Texture2D();
    inline VShaderLibID GetID() const;
    Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters );
private:
    static const KChar * sm_strCode;
};

/////////////////////////////////////////////////////////////////////////////////
// Pixel Shaders
class PixelShader_Phong_Color : public LibPixelShaderProgram
{
public:
    PixelShader_Phong_Color();
    ~PixelShader_Phong_Color();
    inline PShaderLibID GetID() const;
    Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers );
private:
    static const KChar * sm_strCode;
};

class PixelShader_Phong_Texture1D : public LibPixelShaderProgram
{
public:
    PixelShader_Phong_Texture1D();
    ~PixelShader_Phong_Texture1D();
    inline PShaderLibID GetID() const;
    Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers );
private:
    static const KChar * sm_strCode;
};

class PixelShader_Phong_Texture2D : public LibPixelShaderProgram
{
public:
    PixelShader_Phong_Texture2D();
    ~PixelShader_Phong_Texture2D();
    inline PShaderLibID GetID() const;
    Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers );
private:
    static const KChar * sm_strCode;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Phong.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_SHADERLIBRARY_PHONG_H


