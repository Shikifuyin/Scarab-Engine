/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/ShaderLibrary/Default.cpp
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
// Includes
#include "Default.h"

/////////////////////////////////////////////////////////////////////////////////
// Vertex Shaders
const KChar * VertexShader_Default_Null::sm_strCode =
    TEXT("; Shader Library : VS Null\n\
#var Float4 vInPos &in.POSITION\n\
#var Float4 vOutPos &out.POSITION\n\
#var Float4x4 VMatrix c[0]\n\
#var Float4x4 PMatrix c[4]\n");
VertexShader_Default_Null::VertexShader_Default_Null():
    LibVertexShaderProgram(sm_strCode)
{
    // nothing to do
}
VertexShader_Default_Null::~VertexShader_Default_Null()
{
    // nothing to do
}
Void VertexShader_Default_Null::Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters )
{
    const VertexCFL_Default_Null * pIn = (const VertexCFL_Default_Null*)pVertex;
    VertexCFL_Default_Null * pOut = (VertexCFL_Default_Null*)pOutVertex;

    Matrix4 * pView = (Matrix4*)( arrRegisters );
    Matrix4 * pProj = (Matrix4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );

    pOut->vPosition = (*pProj) * ( (*pView) * pIn->vPosition );
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * VertexShader_Default_Color::sm_strCode =
    TEXT("; Shader Library : VS Color\n\
#var Float4 vInPos &in.POSITION\n\
#var Float4 vInColor &in.COLOR\n\
#var Float4 vOutPos &out.POSITION\n\
#var Float4 vOutColor &out.COLOR\n\
#var Float4x4 VMatrix c[0]\n\
#var Float4x4 PMatrix c[4]\n");
VertexShader_Default_Color::VertexShader_Default_Color():
    LibVertexShaderProgram(sm_strCode)
{
    // nothing to do
}
VertexShader_Default_Color::~VertexShader_Default_Color()
{
    // nothing to do
}
Void VertexShader_Default_Color::Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters )
{
    const VertexCFL_Default_Color * pIn = (const VertexCFL_Default_Color*)pVertex;
    VertexCFL_Default_Color * pOut = (VertexCFL_Default_Color*)pOutVertex;

    Matrix4 * pView = (Matrix4*)( arrRegisters );
    Matrix4 * pProj = (Matrix4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );

    pOut->vPosition = (*pProj) * ( (*pView) * pIn->vPosition );
    pOut->vColor = pIn->vColor;
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * VertexShader_Default_Texture1D::sm_strCode =
    TEXT("; Shader Library : VS Texture1D\n\
#var Float4 vInPos &in.POSITION\n\
#var Float vInTexCoord &in.TEXCOORD\n\
#var Float4 vOutPos &out.POSITION\n\
#var Float vOutTexCoord &out.TEXCOORD\n\
#var Float4x4 VMatrix c[0]\n\
#var Float4x4 PMatrix c[4]\n");
VertexShader_Default_Texture1D::VertexShader_Default_Texture1D():
    LibVertexShaderProgram(sm_strCode)
{
    // nothing to do
}
VertexShader_Default_Texture1D::~VertexShader_Default_Texture1D()
{
    // nothing to do
}
Void VertexShader_Default_Texture1D::Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters )
{
    const VertexCFL_Default_Texture1D * pIn = (const VertexCFL_Default_Texture1D*)pVertex;
    VertexCFL_Default_Texture1D * pOut = (VertexCFL_Default_Texture1D*)pOutVertex;

    Matrix4 * pView = (Matrix4*)( arrRegisters );
    Matrix4 * pProj = (Matrix4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );

    pOut->vPosition = (*pProj) * ( (*pView) * pIn->vPosition );
    pOut->vTexCoord = pIn->vTexCoord;
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * VertexShader_Default_Texture2D::sm_strCode =
    TEXT("; Shader Library : VS Texture2D\n\
#var Float4 vInPos &in.POSITION\n\
#var Float2 vInTexCoord &in.TEXCOORD\n\
#var Float4 vOutPos &out.POSITION\n\
#var Float2 vOutTexCoord &out.TEXCOORD\n\
#var Float4x4 VMatrix c[0]\n\
#var Float4x4 PMatrix c[4]\n");
VertexShader_Default_Texture2D::VertexShader_Default_Texture2D():
    LibVertexShaderProgram(sm_strCode)
{
    // nothing to do
}
VertexShader_Default_Texture2D::~VertexShader_Default_Texture2D()
{
    // nothing to do
}
Void VertexShader_Default_Texture2D::Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters )
{
    const VertexCFL_Default_Texture2D * pIn = (const VertexCFL_Default_Texture2D*)pVertex;
    VertexCFL_Default_Texture2D * pOut = (VertexCFL_Default_Texture2D*)pOutVertex;

    Matrix4 * pView = (Matrix4*)( arrRegisters );
    Matrix4 * pProj = (Matrix4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );

    pOut->vPosition = (*pProj) * ( (*pView) * pIn->vPosition );
    pOut->vTexCoord = pIn->vTexCoord;
}

/////////////////////////////////////////////////////////////////////////////////
// Pixel Shaders
const KChar * PixelShader_Default_Null::sm_strCode =
    TEXT("; Shader Library : PS Null\n\
#var Float2 vRasterPos &in.RASTERPOS\n\
#var Float fDepth &in.DEPTH\n\
#var Float fInvW &in.INVERTW\n\
#var Float fRegW &in.REGULARW\n");
PixelShader_Default_Null::PixelShader_Default_Null():
    LibPixelShaderProgram(sm_strCode)
{
    // nothing to do
}
PixelShader_Default_Null::~PixelShader_Default_Null()
{
    // nothing to do
}
Void PixelShader_Default_Null::Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers )
{
    const PixelCFL_Default_Null * pIn = (const PixelCFL_Default_Null*)pPixel;

    outColor = Color4::Black;
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * PixelShader_Default_Color::sm_strCode =
    TEXT("; Shader Library : PS Color\n\
#var Float2 vRasterPos &in.RASTERPOS\n\
#var Float fDepth &in.DEPTH\n\
#var Float fInvW &in.INVERTW\n\
#var Float fRegW &in.REGULARW\n\
#var Float4 vColor &in.COLOR\n");
PixelShader_Default_Color::PixelShader_Default_Color():
    LibPixelShaderProgram(sm_strCode)
{
    // nothing to do
}
PixelShader_Default_Color::~PixelShader_Default_Color()
{
    // nothing to do
}
Void PixelShader_Default_Color::Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers )
{
    const PixelCFL_Default_Color * pIn = (const PixelCFL_Default_Color*)pPixel;
    Scalar fInvInvW = MathFn->Invert( pIn->fInvW );

    outColor = ( pIn->vColor * fInvInvW );
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * PixelShader_Default_Texture1D::sm_strCode =
    TEXT("; Shader Library : PS Texture1D\n\
#var Float2 vRasterPos &in.RASTERPOS\n\
#var Float fDepth &in.DEPTH\n\
#var Float fInvW &in.INVERTW\n\
#var Float fRegW &in.REGULARW\n\
#var Float vTexCoord &in.TEXCOORD\n\
#var Sampler1D 0 texSampler0\n");
PixelShader_Default_Texture1D::PixelShader_Default_Texture1D():
    LibPixelShaderProgram(sm_strCode)
{
    // nothing to do
}
PixelShader_Default_Texture1D::~PixelShader_Default_Texture1D()
{
    // nothing to do
}
Void PixelShader_Default_Texture1D::Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers )
{
    const PixelCFL_Default_Texture1D * pIn = (const PixelCFL_Default_Texture1D*)pPixel;
    Scalar fInvInvW = MathFn->Invert( pIn->fInvW );

    Sampler1D * pTexSampler = (Sampler1D*)( arrSamplers[0] );

    TextureCoord1 vTexCoord = ( pIn->vTexCoord * fInvInvW );
    pTexSampler->TexelSample( outColor, vTexCoord.Values );
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * PixelShader_Default_Texture2D::sm_strCode =
    TEXT("; Shader Library : PS Texture2D\n\
#var Float2 vRasterPos &in.RASTERPOS\n\
#var Float fDepth &in.DEPTH\n\
#var Float fInvW &in.INVERTW\n\
#var Float fRegW &in.REGULARW\n\
#var Float2 vTexCoord &in.TEXCOORD\n\
#var Sampler2D 0 texSampler0\n");
PixelShader_Default_Texture2D::PixelShader_Default_Texture2D():
    LibPixelShaderProgram(sm_strCode)
{
    // nothing to do
}
PixelShader_Default_Texture2D::~PixelShader_Default_Texture2D()
{
    // nothing to do
}
Void PixelShader_Default_Texture2D::Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers )
{
    const PixelCFL_Default_Texture2D * pIn = (const PixelCFL_Default_Texture2D*)pPixel;
    Scalar fInvInvW = MathFn->Invert( pIn->fInvW );

    Sampler2D * pTexSampler = (Sampler2D*)( arrSamplers[0] );

    TextureCoord2 vTexCoord = ( pIn->vTexCoord * fInvInvW );
    pTexSampler->TexelSample( outColor, vTexCoord.Values );
}
