/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Resources/D3D9VertexFormat.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant resource for Direct3D9, VertexFormat
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

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D9VertexFormat.h"

#include "../Direct3D9.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D9VertexFormat implementation
D3D9VertexFormat::D3D9VertexFormat()
{
    m_iFieldCount = 0;
    MemZero( m_arrAttributes, (VF_MAX_FIELDS + 1) * sizeof(D3DVERTEXELEMENT9) );

    m_pDeclaration = NULL;
}
D3D9VertexFormat::~D3D9VertexFormat()
{
    if ( m_pDeclaration != NULL )
        m_pDeclaration->Release();
}

Void D3D9VertexFormat::PushField( UInt iOffset, VFFieldType iType, VFFieldUsage iUsage, UInt iUsageIndex )
{
    m_arrAttributes[m_iFieldCount].Stream = (WORD)0;
    m_arrAttributes[m_iFieldCount].Offset = (WORD)iOffset;
    m_arrAttributes[m_iFieldCount].Method = D3DDECLMETHOD_DEFAULT;
    m_arrAttributes[m_iFieldCount].Type = (BYTE)( Direct3D9AttributeType[iType] );
    m_arrAttributes[m_iFieldCount].Usage = (BYTE)( Direct3D9AttributeUsage[iUsage] );
    m_arrAttributes[m_iFieldCount].UsageIndex = (BYTE)iUsageIndex;
    ++m_iFieldCount;
}
Void D3D9VertexFormat::Finalize( Direct3D9Renderer * pRenderer )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    // D3DDECL_END()
    m_arrAttributes[m_iFieldCount].Stream = (WORD)0xff;
    m_arrAttributes[m_iFieldCount].Offset = (WORD)0;
    m_arrAttributes[m_iFieldCount].Method = D3DDECLMETHOD_DEFAULT;
    m_arrAttributes[m_iFieldCount].Type = D3DDECLTYPE_UNUSED;
    m_arrAttributes[m_iFieldCount].Usage = 0;
    m_arrAttributes[m_iFieldCount].UsageIndex = 0;
    ++m_iFieldCount;

    HRESULT hRes = pDevice->CreateVertexDeclaration( m_arrAttributes, &m_pDeclaration );
    DebugAssert( hRes == D3D_OK );
}

Void D3D9VertexFormat::Enable( Direct3D9Renderer * pRenderer )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetVertexDeclaration( m_pDeclaration );
    DebugAssert( hRes == D3D_OK );
}
Void D3D9VertexFormat::Disable( Direct3D9Renderer * pRenderer )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;
    HRESULT hRes;

    hRes = pDevice->SetVertexDeclaration( NULL );
    DebugAssert( hRes == D3D_OK );
}

