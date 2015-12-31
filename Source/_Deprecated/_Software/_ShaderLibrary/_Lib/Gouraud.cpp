/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/ShaderLibrary/Gouraud.cpp
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
// Includes
#include "Gouraud.h"

/////////////////////////////////////////////////////////////////////////////////
// Vertex Shaders
const KChar * VertexShader_Gouraud_Color::sm_strCode =
    TEXT("; Shader Library : VS Gouraud - Color\n\
#var Float4 vInPos &in.POSITION\n\
#var Float4 vInNormal &in.NORMAL\n\
#var Float4 vInColor &in.COLOR\n\
#var Float4 vOutPos &out.POSITION\n\
#var Float4 vOutColor &out.COLOR\n\
#var Float4 vOutDiffuse &out.DIFFUSE\n\
#var Float4 vOutSpecular &out.SPECULAR\n\
#var Float4x4 VMatrix c[0]\n\
#var Float4x4 PMatrix c[4]\n\
#var Float4 CameraPosition c[8]\n\
#var Float4 Material0Diffuse c[9]\n\
#var Float4 Material0Specular c[10]\n\
#var Float4 Light0Position c[11]\n\
#var Float4 Light0Diffuse c[13]\n\
#var Float4 Light0Specular c[14]\n\
#var Float4 Light0Attenuation c[15]\n");
//#var Float4 Light0Direction c[12]\n
//#var Float4 Light0SpotCutoff c[16]\n
VertexShader_Gouraud_Color::VertexShader_Gouraud_Color():
    LibVertexShaderProgram(sm_strCode)
{
    // nothing to do
}
VertexShader_Gouraud_Color::~VertexShader_Gouraud_Color()
{
    // nothing to do
}
Void VertexShader_Gouraud_Color::Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters )
{
    const VertexCFL_Gouraud_In_Color * pIn = (const VertexCFL_Gouraud_In_Color*)pVertex;
    VertexCFL_Gouraud_Out_Color * pOut = (VertexCFL_Gouraud_Out_Color*)pOutVertex;

    Matrix4 * pView = (Matrix4*)( arrRegisters );
    Matrix4 * pProj = (Matrix4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );
    Vertex4 * pEye = (Vertex4*)( arrRegisters + 8 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pMatDiffuse = (Color4*)( arrRegisters + 9 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pMatSpecular = (Color4*)( arrRegisters + 10 * VERTEXSHADER_REGISTER_CHANNELS );
    Vertex4 * pLightPos = (Vertex4*)( arrRegisters + 11 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightDir = (Vector4*)( arrRegisters + 12 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightDiffuse = (Color4*)( arrRegisters + 13 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightSpecular = (Color4*)( arrRegisters + 14 * VERTEXSHADER_REGISTER_CHANNELS );
    Vector4 * pLightAttenuation = (Vector4*)( arrRegisters + 15 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightSpotCutOff = (Vector4*)( arrRegisters + 16 * VERTEXSHADER_REGISTER_CHANNELS );

    pOut->vPosition = (*pProj) * ( (*pView) * pIn->vPosition );
    pOut->vColor = pIn->vColor;
    pOut->vDiffuse = Color4::Black;
    pOut->vSpecular = Color4::Black;

    // Vertex Lighting
    Vector4 vView = ( (*pEye) - pIn->vPosition );
    vView.Normalize();
    Vector4 vLight = ( (*pLightPos) - pIn->vPosition );
    Scalar fLightDist = vLight.Normalize();
    Scalar fFadedIntensity = pLightAttenuation->W /
        ( pLightAttenuation->X + ( pLightAttenuation->Y + (pLightAttenuation->Z * fLightDist) ) * fLightDist );

    Vector4 vNormal;
    Scalar fCosPhi = (vView * pIn->vNormal);
    if (fCosPhi < 0.0f)
        vNormal = -(pIn->vNormal);
    else
        vNormal = pIn->vNormal;

    Scalar fCosTheta = (vNormal * vLight);
    if (fCosTheta > 0.0f)
        pOut->vDiffuse = (*pMatDiffuse) * ( (*pLightDiffuse) * (fCosTheta * fFadedIntensity) );

    Vector4 vReflect = ( vNormal * ( (vNormal * vView) * 2.0f ) ) - vView;
    vReflect.Normalize();
    Scalar fSpecFactor = (vLight * vReflect);
    if (fSpecFactor > 0.0f) {
        fSpecFactor = MathFn->ExpN(fSpecFactor, pMatSpecular->A) * fFadedIntensity;
        pOut->vSpecular = (*pMatSpecular) * ( (*pLightSpecular) * fSpecFactor );
        pOut->vSpecular.A = 1.0f;
    }
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * VertexShader_Gouraud_Texture1D::sm_strCode =
    TEXT("; Shader Library : VS Gouraud - Texture1D\n\
#var Float4 vInPos &in.POSITION\n\
#var Float4 vInNormal &in.NORMAL\n\
#var Float vInTexCoord &in.TEXCOORD\n\
#var Float4 vOutPos &out.POSITION\n\
#var Float vOutTexCoord &out.TEXCOORD\n\
#var Float4 vOutDiffuse &out.DIFFUSE\n\
#var Float4 vOutSpecular &out.SPECULAR\n\
#var Float4x4 VMatrix c[0]\n\
#var Float4x4 PMatrix c[4]\n\
#var Float4 CameraPosition c[8]\n\
#var Float4 Material0Diffuse c[9]\n\
#var Float4 Material0Specular c[10]\n\
#var Float4 Light0Position c[11]\n\
#var Float4 Light0Diffuse c[13]\n\
#var Float4 Light0Specular c[14]\n\
#var Float4 Light0Attenuation c[15]\n");
//#var Float4 Light0Direction c[12]\n
//#var Float4 Light0SpotCutoff c[16]\n
VertexShader_Gouraud_Texture1D::VertexShader_Gouraud_Texture1D():
    LibVertexShaderProgram(sm_strCode)
{
    // nothing to do
}
VertexShader_Gouraud_Texture1D::~VertexShader_Gouraud_Texture1D()
{
    // nothing to do
}
Void VertexShader_Gouraud_Texture1D::Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters )
{
    const VertexCFL_Gouraud_In_Texture1D * pIn = (const VertexCFL_Gouraud_In_Texture1D*)pVertex;
    VertexCFL_Gouraud_Out_Texture1D * pOut = (VertexCFL_Gouraud_Out_Texture1D*)pOutVertex;

    Matrix4 * pView = (Matrix4*)( arrRegisters );
    Matrix4 * pProj = (Matrix4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );
    Vertex4 * pEye = (Vertex4*)( arrRegisters + 8 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pMatDiffuse = (Color4*)( arrRegisters + 9 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pMatSpecular = (Color4*)( arrRegisters + 10 * VERTEXSHADER_REGISTER_CHANNELS );
    Vertex4 * pLightPos = (Vertex4*)( arrRegisters + 11 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightDir = (Vector4*)( arrRegisters + 12 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightDiffuse = (Color4*)( arrRegisters + 13 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightSpecular = (Color4*)( arrRegisters + 14 * VERTEXSHADER_REGISTER_CHANNELS );
    Vector4 * pLightAttenuation = (Vector4*)( arrRegisters + 15 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightSpotCutOff = (Vector4*)( arrRegisters + 16 * VERTEXSHADER_REGISTER_CHANNELS );

    pOut->vPosition = (*pProj) * ( (*pView) * pIn->vPosition );
    pOut->vTexCoord = pIn->vTexCoord;
    pOut->vDiffuse = Color4::Black;
    pOut->vSpecular = Color4::Black;

    // Vertex Lighting
    Vector4 vView = ( (*pEye) - pIn->vPosition );
    vView.Normalize();
    Vector4 vLight = ( (*pLightPos) - pIn->vPosition );
    Scalar fLightDist = vLight.Normalize();
    Scalar fFadedIntensity = pLightAttenuation->W /
        ( pLightAttenuation->X + ( pLightAttenuation->Y + (pLightAttenuation->Z * fLightDist) ) * fLightDist );

    Vector4 vNormal;
    Scalar fCosPhi = (vView * pIn->vNormal);
    if (fCosPhi < 0.0f)
        vNormal = -(pIn->vNormal);
    else
        vNormal = pIn->vNormal;

    Scalar fCosTheta = (vNormal * vLight);
    if (fCosTheta > 0.0f)
        pOut->vDiffuse = (*pMatDiffuse) * ( (*pLightDiffuse) * (fCosTheta * fFadedIntensity) );

    Vector4 vReflect = ( vNormal * ( (vNormal * vView) * 2.0f ) ) - vView;
    vReflect.Normalize();
    Scalar fSpecFactor = (vLight * vReflect);
    if (fSpecFactor > 0.0f) {
        fSpecFactor = MathFn->ExpN(fSpecFactor, pMatSpecular->A) * fFadedIntensity;
        pOut->vSpecular = (*pMatSpecular) * ( (*pLightSpecular) * fSpecFactor );
        pOut->vSpecular.A = 1.0f;
    }
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * VertexShader_Gouraud_Texture2D::sm_strCode =
    TEXT("; Shader Library : VS Gouraud - Texture2D\n\
#var Float4 vInPos &in.POSITION\n\
#var Float4 vInNormal &in.NORMAL\n\
#var Float2 vInTexCoord &in.TEXCOORD\n\
#var Float4 vOutPos &out.POSITION\n\
#var Float2 vOutTexCoord &out.TEXCOORD\n\
#var Float4 vOutDiffuse &out.DIFFUSE\n\
#var Float4 vOutSpecular &out.SPECULAR\n\
#var Float4x4 VMatrix c[0]\n\
#var Float4x4 PMatrix c[4]\n\
#var Float4 CameraPosition c[8]\n\
#var Float4 Material0Diffuse c[9]\n\
#var Float4 Material0Specular c[10]\n\
#var Float4 Light0Position c[11]\n\
#var Float4 Light0Diffuse c[13]\n\
#var Float4 Light0Specular c[14]\n\
#var Float4 Light0Attenuation c[15]\n");
//#var Float4 Light0Direction c[12]\n
//#var Float4 Light0SpotCutoff c[16]\n
VertexShader_Gouraud_Texture2D::VertexShader_Gouraud_Texture2D():
    LibVertexShaderProgram(sm_strCode)
{
    // nothing to do
}
VertexShader_Gouraud_Texture2D::~VertexShader_Gouraud_Texture2D()
{
    // nothing to do
}
Void VertexShader_Gouraud_Texture2D::Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters )
{
    const VertexCFL_Gouraud_In_Texture2D * pIn = (const VertexCFL_Gouraud_In_Texture2D*)pVertex;
    VertexCFL_Gouraud_Out_Texture2D * pOut = (VertexCFL_Gouraud_Out_Texture2D*)pOutVertex;

    Matrix4 * pView = (Matrix4*)( arrRegisters );
    Matrix4 * pProj = (Matrix4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );
    Vertex4 * pEye = (Vertex4*)( arrRegisters + 8 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pMatDiffuse = (Color4*)( arrRegisters + 9 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pMatSpecular = (Color4*)( arrRegisters + 10 * VERTEXSHADER_REGISTER_CHANNELS );
    Vertex4 * pLightPos = (Vertex4*)( arrRegisters + 11 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightDir = (Vector4*)( arrRegisters + 12 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightDiffuse = (Color4*)( arrRegisters + 13 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightSpecular = (Color4*)( arrRegisters + 14 * VERTEXSHADER_REGISTER_CHANNELS );
    Vector4 * pLightAttenuation = (Vector4*)( arrRegisters + 15 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightSpotCutOff = (Vector4*)( arrRegisters + 16 * VERTEXSHADER_REGISTER_CHANNELS );

    pOut->vPosition = (*pProj) * ( (*pView) * pIn->vPosition );
    pOut->vTexCoord = pIn->vTexCoord;
    pOut->vDiffuse = Color4::Black;
    pOut->vSpecular = Color4::Black;

    // Vertex Lighting
    Vector4 vView = ( (*pEye) - pIn->vPosition );
    vView.Normalize();
    Vector4 vLight = ( (*pLightPos) - pIn->vPosition );
    Scalar fLightDist = vLight.Normalize();
    Scalar fFadedIntensity = pLightAttenuation->W /
        ( pLightAttenuation->X + ( pLightAttenuation->Y + (pLightAttenuation->Z * fLightDist) ) * fLightDist );

    Vector4 vNormal;
    Scalar fCosPhi = (vView * pIn->vNormal);
    if (fCosPhi < 0.0f)
        vNormal = -(pIn->vNormal);
    else
        vNormal = pIn->vNormal;

    Scalar fCosTheta = (vNormal * vLight);
    if (fCosTheta > 0.0f)
        pOut->vDiffuse = (*pMatDiffuse) * ( (*pLightDiffuse) * (fCosTheta * fFadedIntensity) );

    Vector4 vReflect = ( vNormal * ( (vNormal * vView) * 2.0f ) ) - vView;
    vReflect.Normalize();
    Scalar fSpecFactor = (vLight * vReflect);
    if (fSpecFactor > 0.0f) {
        fSpecFactor = MathFn->ExpN(fSpecFactor, pMatSpecular->A) * fFadedIntensity;
        pOut->vSpecular = (*pMatSpecular) * ( (*pLightSpecular) * fSpecFactor );
        pOut->vSpecular.A = 1.0f;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// Pixel Shaders
const KChar * PixelShader_Gouraud_Color::sm_strCode =
    TEXT("; Shader Library : PS Gouraud - Color\n\
#var Float2 vRasterPos &in.RASTERPOS\n\
#var Float fDepth &in.DEPTH\n\
#var Float fInvW &in.INVERTW\n\
#var Float fRegW &in.REGULARW\n\
#var Float4 vColor &in.COLOR\n\
#var Float4 vDiffuse &in.DIFFUSE\n\
#var Float4 vSpecular &in.SPECULAR\n)");
PixelShader_Gouraud_Color::PixelShader_Gouraud_Color():
    LibPixelShaderProgram(sm_strCode)
{
    // nothing to do
}
PixelShader_Gouraud_Color::~PixelShader_Gouraud_Color()
{
    // nothing to do
}
Void PixelShader_Gouraud_Color::Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers )
{
    const PixelCFL_Gouraud_Color * pIn = (const PixelCFL_Gouraud_Color*)pPixel;
    Scalar fInvInvW = MathFn->Invert( pIn->fInvW );

    outColor = ( pIn->vColor * fInvInvW );
    outColor *= ( pIn->vDiffuse * fInvInvW );
    outColor += ( pIn->vSpecular * fInvInvW );
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * PixelShader_Gouraud_Texture1D::sm_strCode =
    TEXT("; Shader Library : PS Gouraud - Texture1D\n\
#var Float2 vRasterPos &in.RASTERPOS\n\
#var Float fDepth &in.DEPTH\n\
#var Float fInvW &in.INVERTW\n\
#var Float fRegW &in.REGULARW\n\
#var Float vTexCoord &in.TEXCOORD\n\
#var Float4 vDiffuse &in.DIFFUSE\n\
#var Float4 vSpecular &in.SPECULAR\n\
#var Sampler1D 0 texSampler0\n");
PixelShader_Gouraud_Texture1D::PixelShader_Gouraud_Texture1D():
    LibPixelShaderProgram(sm_strCode)
{
    // nothing to do
}
PixelShader_Gouraud_Texture1D::~PixelShader_Gouraud_Texture1D()
{
    // nothing to do
}
Void PixelShader_Gouraud_Texture1D::Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers )
{
    const PixelCFL_Gouraud_Texture1D * pIn = (const PixelCFL_Gouraud_Texture1D*)pPixel;
    Scalar fInvInvW = MathFn->Invert( pIn->fInvW );

    Sampler1D * pTexSampler = (Sampler1D*)( arrSamplers[0] );

    TextureCoord1 vTexCoord = ( pIn->vTexCoord * fInvInvW );
    pTexSampler->TexelSample( outColor, vTexCoord.Values );
    outColor *= ( pIn->vDiffuse * fInvInvW );
    outColor += ( pIn->vSpecular * fInvInvW );
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * PixelShader_Gouraud_Texture2D::sm_strCode =
    TEXT("; Shader Library : PS Gouraud - Texture2D\n\
#var Float2 vRasterPos &in.RASTERPOS\n\
#var Float fDepth &in.DEPTH\n\
#var Float fInvW &in.INVERTW\n\
#var Float fRegW &in.REGULARW\n\
#var Float2 vTexCoord &in.TEXCOORD\n\
#var Float4 vDiffuse &in.DIFFUSE\n\
#var Float4 vSpecular &in.SPECULAR\n\
#var Sampler2D 0 texSampler0\n");
PixelShader_Gouraud_Texture2D::PixelShader_Gouraud_Texture2D():
    LibPixelShaderProgram(sm_strCode)
{
    // nothing to do
}
PixelShader_Gouraud_Texture2D::~PixelShader_Gouraud_Texture2D()
{
    // nothing to do
}
Void PixelShader_Gouraud_Texture2D::Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers )
{
    const PixelCFL_Gouraud_Texture2D * pIn = (const PixelCFL_Gouraud_Texture2D*)pPixel;
    Scalar fInvInvW = MathFn->Invert( pIn->fInvW );

    Sampler2D * pTexSampler = (Sampler2D*)( arrSamplers[0] );

    TextureCoord2 vTexCoord = ( pIn->vTexCoord * fInvInvW );
    pTexSampler->TexelSample( outColor, vTexCoord.Values );
    outColor *= ( pIn->vDiffuse * fInvInvW );
    outColor += ( pIn->vSpecular * fInvInvW );
}

