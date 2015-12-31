/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Resources/D3D9Shaders.cpp
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
// Internal dependency : Direct3D9, DirectX
#define WIN32_LEAN_AND_MEAN
#include <d3d9.h>
#include <d3dx9shader.h> // Shader compilation

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D9Shaders.h"

#include "../Direct3D9.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D9Shader implementation
D3D9Shader::D3D9Shader()
{
    // nothing to do
}
D3D9Shader::~D3D9Shader()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// D3D9VertexShader implementation
D3D9VertexShader::D3D9VertexShader( Direct3D9Renderer * pRenderer, const AChar * strProgramText )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    UInt iProgramLength = StringFn->LengthA( strProgramText );

    ID3DXBuffer * pCompiledShader = NULL;
    ID3DXBuffer * pErrors = NULL;
    HRESULT hRes = D3DXAssembleShader( strProgramText, iProgramLength, NULL, NULL, 0,
                                       &pCompiledShader, &pErrors );
    if ( pErrors != NULL ) {
        //DWord dwSize = pErrors->GetBufferSize();
        //Byte * pData = (Byte*)( pErrors->GetBufferPointer() );
    }
    DebugAssert( (hRes == D3D_OK && pCompiledShader != NULL && pErrors == NULL) );

    m_pVS = NULL;
    hRes = pDevice->CreateVertexShader( (DWord*)( pCompiledShader->GetBufferPointer() ), &m_pVS );
    DebugAssert( hRes == D3D_OK );

    pCompiledShader->Release();
    if ( pErrors != NULL )
        pErrors->Release();
}
D3D9VertexShader::~D3D9VertexShader()
{
    m_pVS->Release();
}

Void D3D9VertexShader::Enable( Direct3D9Renderer * pRenderer )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetVertexShader( m_pVS );
    DebugAssert( hRes == D3D_OK );
}
Void D3D9VertexShader::Disable( Direct3D9Renderer * pRenderer )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes;

    hRes = pDevice->SetVertexShader( NULL );
    DebugAssert( hRes == D3D_OK );
}

Void D3D9VertexShader::SetConstant( Direct3D9Renderer * pRenderer, UInt iBaseRegister, UInt iRegisterCount, const Float * arrData )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetVertexShaderConstantF( iBaseRegister, arrData, iRegisterCount );
    DebugAssert( hRes == D3D_OK );
}
Void D3D9VertexShader::SetSamplerState( Direct3D9Renderer * pRenderer, UInt iTextureUnit, 
                                        ShaderSamplingType iType, const ShaderSamplingWrapMode * arrWrapMode, DWord dwBorderColor,
                                        ShaderSamplingFilter iFilter, Float fLODBias, Float fAnisotropy )
{
    UInt iBase = pRenderer->GetVSBaseSampler();
    pRenderer->SetSamplerWrapState( iBase + iTextureUnit, iType, arrWrapMode, dwBorderColor );
    pRenderer->SetSamplerFilterState( iBase + iTextureUnit, iFilter, fLODBias, fAnisotropy );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D9PixelShader implementation
D3D9PixelShader::D3D9PixelShader( Direct3D9Renderer * pRenderer, const AChar * strProgramText )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    UInt iProgramLength = StringFn->LengthA( strProgramText );

    ID3DXBuffer * pCompiledShader = NULL;
    ID3DXBuffer * pErrors = NULL;
    HRESULT hRes = D3DXAssembleShader( strProgramText, iProgramLength, NULL, NULL, 0,
                                       &pCompiledShader, &pErrors );
    if ( pErrors != NULL ) {
        //DWord dwSize = pErrors->GetBufferSize();
        //Byte * pData = (Byte*)( pErrors->GetBufferPointer() );
    }
    DebugAssert( (hRes == D3D_OK && pCompiledShader != NULL && pErrors == NULL) );

    m_pPS = NULL;
    hRes = pDevice->CreatePixelShader( (DWord*)( pCompiledShader->GetBufferPointer() ), &m_pPS );
    DebugAssert( hRes == D3D_OK );

    pCompiledShader->Release();
    if ( pErrors != NULL )
        pErrors->Release();
}
D3D9PixelShader::~D3D9PixelShader()
{
    m_pPS->Release();
}

Void D3D9PixelShader::Enable( Direct3D9Renderer * pRenderer )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetPixelShader( m_pPS );
    DebugAssert( hRes == D3D_OK );
}
Void D3D9PixelShader::Disable( Direct3D9Renderer * pRenderer )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes;

    hRes = pDevice->SetPixelShader( NULL );
    DebugAssert( hRes == D3D_OK );
}

Void D3D9PixelShader::SetConstant( Direct3D9Renderer * pRenderer, UInt iBaseRegister, UInt iRegisterCount, const Float * arrData )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetPixelShaderConstantF( iBaseRegister, arrData, iRegisterCount );
    DebugAssert( hRes == D3D_OK );
}
Void D3D9PixelShader::SetSamplerState( Direct3D9Renderer * pRenderer, UInt iTextureUnit, 
                                       ShaderSamplingType iType, const ShaderSamplingWrapMode * arrWrapMode, DWord dwBorderColor,
                                       ShaderSamplingFilter iFilter, Float fLODBias, Float fAnisotropy )
{
    UInt iBase = pRenderer->GetPSBaseSampler();
    pRenderer->SetSamplerWrapState( iBase + iTextureUnit, iType, arrWrapMode, dwBorderColor );
    pRenderer->SetSamplerFilterState( iBase + iTextureUnit, iFilter, fLODBias, fAnisotropy );
}

