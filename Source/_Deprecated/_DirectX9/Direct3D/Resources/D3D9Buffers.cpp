/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX9/Direct3D/Resources/D3D9Buffers.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant resource for Direct3D9, Buffers
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
#include "D3D9Buffers.h"

#include "../Direct3D9.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D9VertexBuffer implementation
D3D9VertexBuffer::D3D9VertexBuffer( Direct3D9Renderer * pRenderer, VertexBufferUsage iUsage, UInt iDataSize, UInt iDataStride )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    DWord dwUsage = Direct3D9VertexBufferUsage[iUsage];

    m_pVB = NULL;
    HRESULT hRes = pDevice->CreateVertexBuffer( iDataSize, dwUsage, 0, D3DPOOL_DEFAULT, &m_pVB, NULL );
    DebugAssert( hRes == D3D_OK );


    m_iStride = iDataStride;
}
D3D9VertexBuffer::~D3D9VertexBuffer()
{
    m_pVB->Release();
}

Void D3D9VertexBuffer::Enable( Direct3D9Renderer * pRenderer, UInt iStream, UInt iOffset )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetStreamSource( iStream, m_pVB, (iOffset * m_iStride), m_iStride );
    DebugAssert( hRes == D3D_OK );
}
Void D3D9VertexBuffer::Disable( Direct3D9Renderer * pRenderer, UInt iStream )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes;

    hRes = pDevice->SetStreamSource( iStream, NULL, 0, 0 );
    DebugAssert( hRes == D3D_OK );
}
Void * D3D9VertexBuffer::Lock( VertexBufferLock iLockMode )
{
    Void * pVideoMemory = NULL;

    HRESULT hRes = m_pVB->Lock( 0, 0, &pVideoMemory, Direct3D9VertexBufferLock[iLockMode] );
    DebugAssert( hRes == D3D_OK );

    return pVideoMemory;
}
Void D3D9VertexBuffer::Unlock()
{
    HRESULT hRes = m_pVB->Unlock();
    DebugAssert( hRes == D3D_OK );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D9IndexBuffer implementation
D3D9IndexBuffer::D3D9IndexBuffer( Direct3D9Renderer * pRenderer, IndexBufferUsage iUsage, UInt iDataSize, UInt iDataStride )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    DWord dwUsage = Direct3D9IndexBufferUsage[iUsage];
    D3DFORMAT d3dFormat;
    if ( iDataStride == 2 )
        d3dFormat = D3DFMT_INDEX16;
    else if ( iDataStride == 4 )
        d3dFormat = D3DFMT_INDEX32;
    else
        DebugAssert( false );

    m_pIB = NULL;
    HRESULT hRes = pDevice->CreateIndexBuffer( iDataSize, dwUsage, d3dFormat, D3DPOOL_DEFAULT, &m_pIB, NULL );
    DebugAssert( hRes == D3D_OK );
}
D3D9IndexBuffer::~D3D9IndexBuffer()
{
    m_pIB->Release();
}

Void D3D9IndexBuffer::Enable( Direct3D9Renderer * pRenderer )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes = pDevice->SetIndices(m_pIB);
    DebugAssert( hRes == D3D_OK );
}
Void D3D9IndexBuffer::Disable( Direct3D9Renderer * pRenderer )
{
    IDirect3DDevice9 * pDevice = pRenderer->m_pDevice;

    HRESULT hRes;

    hRes = pDevice->SetIndices( NULL );
    DebugAssert( hRes == D3D_OK );
}
Void * D3D9IndexBuffer::Lock( IndexBufferLock iLockMode )
{
    Void * pVideoMemory = NULL;

    HRESULT hRes = m_pIB->Lock( 0, 0, &pVideoMemory, Direct3D9IndexBufferLock[iLockMode] );
    DebugAssert( hRes == D3D_OK );

    return pVideoMemory;
}
Void D3D9IndexBuffer::Unlock()
{
    HRESULT hRes = m_pIB->Unlock();
    DebugAssert( hRes == D3D_OK );
}

