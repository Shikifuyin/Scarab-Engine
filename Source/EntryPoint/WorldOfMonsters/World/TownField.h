/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/World/TownField.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, World : TownField class
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_WORLD_TOWNFIELD_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_WORLD_TOWNFIELD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Entities/EntityManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Dimensions
#define TOWNFIELD_CELL_SIZE    10.0f
#define TOWNFIELD_CELL_COUNT_X 64
#define TOWNFIELD_CELL_COUNT_Y 64

/////////////////////////////////////////////////////////////////////////////////
// The TownFieldFloor class
class TownFieldFloor : public WorldLeaf
{
public:
protected:
};

/////////////////////////////////////////////////////////////////////////////////
// The TownField class
class TownField
{
public:
    TownField();
    virtual ~TownField();

    // Deferred loading
    Void Load( PlayerTown * pPlayerTown );
    Void UnLoad();

    // Building management
    inline Bool HasBuilding( BuildingType iBuilding ) const;
    inline BuildingEntity * GetBuilding( BuildingType iBuilding ) const;

    Bool PlaceBuilding( BuildingEntity * pBuilding, UInt iX, UInt iY );
    Bool MoveBuilding( BuildingType iBuilding, UInt iX, UInt iY );

protected:
    // Helpers
    Bool _CheckOverlaps( const Rectangle2 & hRect ) const;

    // Logical representation
    PlayerTown * m_pPlayerTown;

    // World tree
    WorldNode * m_pRoot;
    TownFieldFloor * m_pFloor;
    BuildingEntity * m_arrBuildingEntities[BUILDING_COUNT];

    // Building positions
    Point2 m_arrBuildingPositions[BUILDING_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TownField.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_WORLD_TOWNFIELD_H


