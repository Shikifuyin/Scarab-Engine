/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Resources/D3D9Shaders.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant resource for Direct3D9, Shaders
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_RESOURCES_D3D9SHADERS_H
#define SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_RESOURCES_D3D9SHADERS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Mappings.h"

#include "../../../System/String.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
struct _d3d9_samplerstate;
typedef struct _d3d9_samplerstate D3D9SamplerState;

class Direct3D9Renderer;

/////////////////////////////////////////////////////////////////////////////////
// The D3D9Shader class
class D3D9Shader
{
public:
    D3D9Shader();
    ~D3D9Shader();

};

/////////////////////////////////////////////////////////////////////////////////
// The D3D9VertexShader class
class D3D9VertexShader : public D3D9Shader
{
public:
    D3D9VertexShader( Direct3D9Renderer * pRenderer, const AChar * strProgramText );
    ~D3D9VertexShader();

    Void Enable( Direct3D9Renderer * pRenderer );
    Void Disable( Direct3D9Renderer * pRenderer );

    Void SetConstant( Direct3D9Renderer * pRenderer, UInt iBaseRegister, UInt iRegisterCount, const Float * arrData );
    Void SetSamplerState( Direct3D9Renderer * pRenderer, UInt iTextureUnit, 
                          ShaderSamplingType iType, const ShaderSamplingWrapMode * arrWrapMode, DWord dwBorderColor,
                          ShaderSamplingFilter iFilter, Float fLODBias, Float fAnisotropy );

private:
    IDirect3DVertexShader9 * m_pVS;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D9PixelShader class
class D3D9PixelShader : public D3D9Shader
{
public:
    D3D9PixelShader( Direct3D9Renderer * pRenderer, const AChar * strProgramText );
    ~D3D9PixelShader();

    Void Enable( Direct3D9Renderer * pRenderer );
    Void Disable( Direct3D9Renderer * pRenderer );

    Void SetConstant( Direct3D9Renderer * pRenderer, UInt iBaseRegister, UInt iRegisterCount, const Float * arrData );
    Void SetSamplerState( Direct3D9Renderer * pRenderer, UInt iTextureUnit, 
                          ShaderSamplingType iType, const ShaderSamplingWrapMode * arrWrapMode, DWord dwBorderColor,
                          ShaderSamplingFilter iFilter, Float fLODBias, Float fAnisotropy );

private:
    IDirect3DPixelShader9 * m_pPS;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D9Shaders.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX9_DIRECT3D_RESOURCES_D3D9SHADERS_H

