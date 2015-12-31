/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/ShaderLibrary/Phong.cpp
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
// Includes
#include "Phong.h"

/////////////////////////////////////////////////////////////////////////////////
// Vertex Shaders
const KChar * VertexShader_Phong_Color::sm_strCode =
    TEXT("; Shader Library : VS Phong - Color\n\
#var Float4 vInPos &in.POSITION\n\
#var Float4 vInNormal &in.NORMAL\n\
#var Float4 vInColor &in.COLOR\n\
#var Float4 vOutPos &out.POSITION\n\
#var Float4 vOutNormal &out.NORMAL\n\
#var Float4 vOutColor &out.COLOR\n\
#var Float4 vOutWorldPos &out.USERDATA\n\
#var Float4x4 VMatrix c[0]\n\
#var Float4x4 PMatrix c[4]\n");
VertexShader_Phong_Color::VertexShader_Phong_Color():
    LibVertexShaderProgram(sm_strCode)
{
    // nothing to do
}
VertexShader_Phong_Color::~VertexShader_Phong_Color()
{
    // nothing to do
}
Void VertexShader_Phong_Color::Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters )
{
    const VertexCFL_Phong_In_Color * pIn = (const VertexCFL_Phong_In_Color*)pVertex;
    VertexCFL_Phong_Out_Color * pOut = (VertexCFL_Phong_Out_Color*)pOutVertex;

    Matrix4 * pView = (Matrix4*)( arrRegisters );
    Matrix4 * pProj = (Matrix4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );

    pOut->vPosition = (*pProj) * ( (*pView) * pIn->vPosition );
    pOut->vNormal = pIn->vNormal;
    pOut->vColor = pIn->vColor;
    pOut->vWorldPos = pIn->vPosition;
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * VertexShader_Phong_Texture1D::sm_strCode =
    TEXT("; Shader Library : VS Phong - Texture1D\n\
#var Float4 vInPos &in.POSITION\n\
#var Float4 vInNormal &in.NORMAL\n\
#var Float vInTexCoord &in.TEXCOORD\n\
#var Float4 vOutPos &out.POSITION\n\
#var Float4 vOutNormal &out.NORMAL\n\
#var Float vOutTexCoord &out.TEXCOORD\n\
#var Float4 vOutWorldPos &out.USERDATA\n\
#var Float4x4 VMatrix c[0]\n\
#var Float4x4 PMatrix c[4]\n");
VertexShader_Phong_Texture1D::VertexShader_Phong_Texture1D():
    LibVertexShaderProgram(sm_strCode)
{
    // nothing to do
}
VertexShader_Phong_Texture1D::~VertexShader_Phong_Texture1D()
{
    // nothing to do
}
Void VertexShader_Phong_Texture1D::Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters )
{
    const VertexCFL_Phong_In_Texture1D * pIn = (const VertexCFL_Phong_In_Texture1D*)pVertex;
    VertexCFL_Phong_Out_Texture1D * pOut = (VertexCFL_Phong_Out_Texture1D*)pOutVertex;

    Matrix4 * pView = (Matrix4*)( arrRegisters );
    Matrix4 * pProj = (Matrix4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );

    pOut->vPosition = (*pProj) * ( (*pView) * pIn->vPosition );
    pOut->vNormal = pIn->vNormal;
    pOut->vTexCoord = pIn->vTexCoord;
    pOut->vWorldPos = pIn->vPosition;
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * VertexShader_Phong_Texture2D::sm_strCode =
    TEXT("; Shader Library : VS Phong - Texture2D\n\
#var Float4 vInPos &in.POSITION\n\
#var Float4 vInNormal &in.NORMAL\n\
#var Float2 vInTexCoord &in.TEXCOORD\n\
#var Float4 vOutPos &out.POSITION\n\
#var Float4 vOutNormal &out.NORMAL\n\
#var Float2 vOutTexCoord &out.TEXCOORD\n\
#var Float4 vOutWorldPos &out.USERDATA\n\
#var Float4x4 VMatrix c[0]\n\
#var Float4x4 PMatrix c[4]\n");
VertexShader_Phong_Texture2D::VertexShader_Phong_Texture2D():
    LibVertexShaderProgram(sm_strCode)
{
    // nothing to do
}
VertexShader_Phong_Texture2D::~VertexShader_Phong_Texture2D()
{
    // nothing to do
}
Void VertexShader_Phong_Texture2D::Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters )
{
    const VertexCFL_Phong_In_Texture2D * pIn = (const VertexCFL_Phong_In_Texture2D*)pVertex;
    VertexCFL_Phong_Out_Texture2D * pOut = (VertexCFL_Phong_Out_Texture2D*)pOutVertex;

    Matrix4 * pView = (Matrix4*)( arrRegisters );
    Matrix4 * pProj = (Matrix4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );

    pOut->vPosition = (*pProj) * ( (*pView) * pIn->vPosition );
    pOut->vNormal = pIn->vNormal;
    pOut->vTexCoord = pIn->vTexCoord;
    pOut->vWorldPos = pIn->vPosition;
}

/////////////////////////////////////////////////////////////////////////////////
// Pixel Shaders
const KChar * PixelShader_Phong_Color::sm_strCode =
    TEXT("; Shader Library : PS Phong - Color\n\
#var Float2 vRasterPos &in.RASTERPOS\n\
#var Float fDepth &in.DEPTH\n\
#var Float fInvW &in.INVERTW\n\
#var Float fRegW &in.REGULARW\n\
#var Float4 vNormal &in.NORMAL\n\
#var Float4 vColor &in.COLOR\n\
#var Float4 vWorldPos &in.USERDATA\n\
#var Float4 CameraPosition c[0]\n\
#var Float4 Material0Diffuse c[1]\n\
#var Float4 Material0Specular c[2]\n\
#var Float4 Light0Position c[3]\n\
#var Float4 Light0Diffuse c[5]\n\
#var Float4 Light0Specular c[6]\n\
#var Float4 Light0Attenuation c[7]\n");
//#var Float4 Light0Direction c[4]\n
//#var Float4 Light0SpotCutoff c[8]\n
PixelShader_Phong_Color::PixelShader_Phong_Color():
    LibPixelShaderProgram(sm_strCode)
{
    // nothing to do
}
PixelShader_Phong_Color::~PixelShader_Phong_Color()
{
    // nothing to do
}
Void PixelShader_Phong_Color::Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers )
{
    const PixelCFL_Phong_Color * pIn = (const PixelCFL_Phong_Color*)pPixel;
    Scalar fInvInvW = MathFn->Invert( pIn->fInvW );

    Vertex4 * pEye = (Vertex4*)( arrRegisters );
    Color4 * pMatDiffuse = (Color4*)( arrRegisters + 1 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pMatSpecular = (Color4*)( arrRegisters + 2 * VERTEXSHADER_REGISTER_CHANNELS );
    Vertex4 * pLightPos = (Vertex4*)( arrRegisters + 3 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightDir = (Vector4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightDiffuse = (Color4*)( arrRegisters + 5 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightSpecular = (Color4*)( arrRegisters + 6 * VERTEXSHADER_REGISTER_CHANNELS );
    Vector4 * pLightAttenuation = (Vector4*)( arrRegisters + 7 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightSpotCutOff = (Vector4*)( arrRegisters + 8 * VERTEXSHADER_REGISTER_CHANNELS );

    outColor = ( pIn->vColor * fInvInvW );

    Vector4 vNormal = ( pIn->vNormal * fInvInvW );
    Vertex4 vPosition = ( pIn->vWorldPos * fInvInvW );

    // Pixel Lighting
    Vector4 vView = ( (*pEye) - vPosition );
    vView.Normalize();
    Vector4 vLight = ( (*pLightPos) - vPosition );
    Scalar fLightDist = vLight.Normalize();
    Scalar fFadedIntensity = pLightAttenuation->W /
        ( pLightAttenuation->X + ( pLightAttenuation->Y + (pLightAttenuation->Z * fLightDist) ) * fLightDist );

    Scalar fCosPhi = (vView * vNormal);
    if (fCosPhi < 0.0f)
        vNormal = -vNormal;

    Scalar fCosTheta = (vNormal * vLight);
    if (fCosTheta > 0.0f)
        outColor *= (*pMatDiffuse) * ( (*pLightDiffuse) * (fCosTheta * fFadedIntensity) );

    Vector4 vReflect = ( vNormal * ( (vNormal * vView) * 2.0f ) ) - vView;
    vReflect.Normalize();
    Scalar fSpecFactor = (vLight * vReflect);
    if (fSpecFactor > 0.0f) {
        fSpecFactor = MathFn->ExpN(fSpecFactor, pMatSpecular->A) * fFadedIntensity;
        outColor.R += pMatSpecular->R * ( pLightSpecular->R * fSpecFactor );
        outColor.G += pMatSpecular->G * ( pLightSpecular->G * fSpecFactor );
        outColor.B += pMatSpecular->B * ( pLightSpecular->B * fSpecFactor );
    }
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * PixelShader_Phong_Texture1D::sm_strCode =
    TEXT("; Shader Library : PS Phong - Texture1D\n\
#var Float2 vRasterPos &in.RASTERPOS\n\
#var Float fDepth &in.DEPTH\n\
#var Float fInvW &in.INVERTW\n\
#var Float fRegW &in.REGULARW\n\
#var Float4 vNormal &in.NORMAL\n\
#var Float vTexCoord &in.TEXCOORD\n\
#var Float4 vWorldPos &in.USERDATA\n\
#var Sampler1D 0 texSampler0\n\
#var Float4 CameraPosition c[0]\n\
#var Float4 Material0Diffuse c[1]\n\
#var Float4 Material0Specular c[2]\n\
#var Float4 Light0Position c[3]\n\
#var Float4 Light0Diffuse c[5]\n\
#var Float4 Light0Specular c[6]\n\
#var Float4 Light0Attenuation c[7]\n");
//#var Float4 Light0Direction c[4]\n
//#var Float4 Light0SpotCutoff c[8]\n
PixelShader_Phong_Texture1D::PixelShader_Phong_Texture1D():
    LibPixelShaderProgram(sm_strCode)
{
    // nothing to do
}
PixelShader_Phong_Texture1D::~PixelShader_Phong_Texture1D()
{
    // nothing to do
}
Void PixelShader_Phong_Texture1D::Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers )
{
    const PixelCFL_Phong_Texture1D * pIn = (const PixelCFL_Phong_Texture1D*)pPixel;
    Scalar fInvInvW = MathFn->Invert( pIn->fInvW );

    Vertex4 * pEye = (Vertex4*)( arrRegisters );
    Color4 * pMatDiffuse = (Color4*)( arrRegisters + 1 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pMatSpecular = (Color4*)( arrRegisters + 2 * VERTEXSHADER_REGISTER_CHANNELS );
    Vertex4 * pLightPos = (Vertex4*)( arrRegisters + 3 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightDir = (Vector4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightDiffuse = (Color4*)( arrRegisters + 5 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightSpecular = (Color4*)( arrRegisters + 6 * VERTEXSHADER_REGISTER_CHANNELS );
    Vector4 * pLightAttenuation = (Vector4*)( arrRegisters + 7 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightSpotCutOff = (Vector4*)( arrRegisters + 8 * VERTEXSHADER_REGISTER_CHANNELS );

    Sampler1D * pTexSampler = (Sampler1D*)( arrSamplers[0] );

    TextureCoord1 vTexCoord = ( pIn->vTexCoord * fInvInvW );
    pTexSampler->TexelSample( outColor, vTexCoord.Values );

    Vector4 vNormal = ( pIn->vNormal * fInvInvW );
    Vertex4 vPosition = ( pIn->vWorldPos * fInvInvW );

    // Pixel Lighting
    Vector4 vView = ( (*pEye) - vPosition );
    vView.Normalize();
    Vector4 vLight = ( (*pLightPos) - vPosition );
    Scalar fLightDist = vLight.Normalize();
    Scalar fFadedIntensity = pLightAttenuation->W /
        ( pLightAttenuation->X + ( pLightAttenuation->Y + (pLightAttenuation->Z * fLightDist) ) * fLightDist );

    Scalar fCosPhi = (vView * vNormal);
    if (fCosPhi < 0.0f)
        vNormal = -vNormal;

    Scalar fCosTheta = (vNormal * vLight);
    if (fCosTheta > 0.0f)
        outColor *= (*pMatDiffuse) * ( (*pLightDiffuse) * (fCosTheta * fFadedIntensity) );

    Vector4 vReflect = ( vNormal * ( (vNormal * vView) * 2.0f ) ) - vView;
    vReflect.Normalize();
    Scalar fSpecFactor = (vLight * vReflect);
    if (fSpecFactor > 0.0f) {
        fSpecFactor = MathFn->ExpN(fSpecFactor, pMatSpecular->A) * fFadedIntensity;
        outColor.R += pMatSpecular->R * ( pLightSpecular->R * fSpecFactor );
        outColor.G += pMatSpecular->G * ( pLightSpecular->G * fSpecFactor );
        outColor.B += pMatSpecular->B * ( pLightSpecular->B * fSpecFactor );
    }
}

/////////////////////////////////////////////////////////////////////////////////

const KChar * PixelShader_Phong_Texture2D::sm_strCode =
    TEXT("; Shader Library : PS Phong - Texture2D\n\
#var Float2 vRasterPos &in.RASTERPOS\n\
#var Float fDepth &in.DEPTH\n\
#var Float fInvW &in.INVERTW\n\
#var Float fRegW &in.REGULARW\n\
#var Float4 vNormal &in.NORMAL\n\
#var Float2 vTexCoord &in.TEXCOORD\n\
#var Float4 vWorldPos &in.USERDATA\n\
#var Sampler2D 0 texSampler0\n\
#var Float4 CameraPosition c[0]\n\
#var Float4 Material0Diffuse c[1]\n\
#var Float4 Material0Specular c[2]\n\
#var Float4 Light0Position c[3]\n\
#var Float4 Light0Diffuse c[5]\n\
#var Float4 Light0Specular c[6]\n\
#var Float4 Light0Attenuation c[7]\n");
//#var Float4 Light0Direction c[4]\n
//#var Float4 Light0SpotCutoff c[8]\n
PixelShader_Phong_Texture2D::PixelShader_Phong_Texture2D():
    LibPixelShaderProgram(sm_strCode)
{
    // nothing to do
}
PixelShader_Phong_Texture2D::~PixelShader_Phong_Texture2D()
{
    // nothing to do
}
Void PixelShader_Phong_Texture2D::Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers )
{
    const PixelCFL_Phong_Texture2D * pIn = (const PixelCFL_Phong_Texture2D*)pPixel;
    Scalar fInvInvW = MathFn->Invert( pIn->fInvW );

    Vertex4 * pEye = (Vertex4*)( arrRegisters );
    Color4 * pMatDiffuse = (Color4*)( arrRegisters + 1 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pMatSpecular = (Color4*)( arrRegisters + 2 * VERTEXSHADER_REGISTER_CHANNELS );
    Vertex4 * pLightPos = (Vertex4*)( arrRegisters + 3 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightDir = (Vector4*)( arrRegisters + 4 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightDiffuse = (Color4*)( arrRegisters + 5 * VERTEXSHADER_REGISTER_CHANNELS );
    Color4 * pLightSpecular = (Color4*)( arrRegisters + 6 * VERTEXSHADER_REGISTER_CHANNELS );
    Vector4 * pLightAttenuation = (Vector4*)( arrRegisters + 7 * VERTEXSHADER_REGISTER_CHANNELS );
    //Vector4 * pLightSpotCutOff = (Vector4*)( arrRegisters + 8 * VERTEXSHADER_REGISTER_CHANNELS );

    Sampler2D * pTexSampler = (Sampler2D*)( arrSamplers[0] );

    TextureCoord2 vTexCoord = ( pIn->vTexCoord * fInvInvW );
    pTexSampler->TexelSample( outColor, vTexCoord.Values );

    Vector4 vNormal = ( pIn->vNormal * fInvInvW );
    Vertex4 vPosition = ( pIn->vWorldPos * fInvInvW );

    // Pixel Lighting
    Vector4 vView = ( (*pEye) - vPosition );
    vView.Normalize();
    Vector4 vLight = ( (*pLightPos) - vPosition );
    Scalar fLightDist = vLight.Normalize();
    Scalar fFadedIntensity = pLightAttenuation->W /
        ( pLightAttenuation->X + ( pLightAttenuation->Y + (pLightAttenuation->Z * fLightDist) ) * fLightDist );

    Scalar fCosPhi = (vView * vNormal);
    if (fCosPhi < 0.0f)
        vNormal = -vNormal;

    Scalar fCosTheta = (vNormal * vLight);
    if (fCosTheta > 0.0f)
        outColor *= (*pMatDiffuse) * ( (*pLightDiffuse) * (fCosTheta * fFadedIntensity) );

    Vector4 vReflect = ( vNormal * ( (vNormal * vView) * 2.0f ) ) - vView;
    vReflect.Normalize();
    Scalar fSpecFactor = (vLight * vReflect);
    if (fSpecFactor > 0.0f) {
        fSpecFactor = MathFn->ExpN(fSpecFactor, pMatSpecular->A) * fFadedIntensity;
        outColor.R += pMatSpecular->R * ( pLightSpecular->R * fSpecFactor );
        outColor.G += pMatSpecular->G * ( pLightSpecular->G * fSpecFactor );
        outColor.B += pMatSpecular->B * ( pLightSpecular->B * fSpecFactor );
    }
}

