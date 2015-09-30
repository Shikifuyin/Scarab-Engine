/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DMesh.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Meshes
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
#ifndef SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DMESH_H
#define SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DMESH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPU2DBrush.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// GPU2DMesh definitions
typedef struct _gpu2d_triangle {
    GPU2DPoint ptA;
    GPU2DPoint ptB;
    GPU2DPoint ptC;
} GPU2DTriangle;

// Prototypes
class GPU2DContext;
class GPU2DGeometry;

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DMesh class
class GPU2DMesh
{
private:
    friend class GPU2DContext;
    GPU2DMesh( GPU2DContext * pContext2D );
    ~GPU2DMesh();

public:
    // Binding
    inline Bool IsBound() const;

    Void Bind();
    Void UnBind();

    // Methods
    inline Bool IsOpened() const;

    inline Void Open();
    inline Void Close();

    inline Void AddTriangles( const GPU2DTriangle * arrTriangles, UInt iTriangleCount );

private:
    friend class GPU2DGeometry;

    GPU2DContext * m_pContext2D;

    // Device data
    Device2DMesh m_hDevice2DMesh;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPU2DMesh.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DMESH_H
