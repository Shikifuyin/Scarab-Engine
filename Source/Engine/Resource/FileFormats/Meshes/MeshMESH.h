/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Meshes/MeshMESH.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh formats : MESH (native)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_RESOURCE_FILEFORMATS_MESHES_MESHMESH_H
#define SCARAB_ENGINE_RESOURCE_FILEFORMATS_MESHES_MESHMESH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "MeshFile.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshMESH class
class MeshMESH : public MeshFile
{
public:
	MeshMESH();
	virtual ~MeshMESH();

	inline virtual MeshFileType GetType() const;

    virtual Bool Load( const GChar * strFilename );
	virtual Bool Save( const GChar * strFilename ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshMESH.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_FILEFORMATS_MESHES_MESHMESH_H

