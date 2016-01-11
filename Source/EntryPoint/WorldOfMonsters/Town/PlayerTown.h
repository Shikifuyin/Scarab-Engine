/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/PlayerTown.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : Base PlayerTown interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_PLAYERTOWN_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_PLAYERTOWN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Building.h"
#include "BuildingDungeon.h"
#include "BuildingEssenceStorage.h"
#include "BuildingMonsterStorage.h"
#include "BuildingMonsterSummoning.h"
#include "BuildingMonsterEvolution.h"
#include "BuildingMonsterFusion.h"
#include "BuildingRuneStorage.h"
#include "BuildingRuneEvolution.h"
#include "BuildingMana.h"
#include "BuildingCrystal.h"
#include "BuildingShop.h"
#include "BuildingWishes.h"
#include "BuildingBonus.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Prototypes
class PlayerGuild;

/////////////////////////////////////////////////////////////////////////////////
// The PlayerTown class
class PlayerTown
{
public:
    PlayerTown();
    ~PlayerTown();

    // Buildings access
    inline UInt GetArcaneTowerCount() const;

    inline Bool HasBuilding( BuildingType iType ) const;
    inline Building * GetBuilding( BuildingType iType ) const;

    Void AddBuilding( BuildingType iType );
    Void RemoveBuilding( BuildingType iType );

    // Guild access
    inline Bool HasGuild() const;
    inline PlayerGuild * GetGuild() const;

private:
    // Buildings
    Building * m_arrBuildings[BUILDING_COUNT];

    // Guild data
    PlayerGuild * m_pGuild;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PlayerTown.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_PLAYERTOWN_H

