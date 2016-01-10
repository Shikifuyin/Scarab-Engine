/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingMonsterStorage.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingMonsterStorage interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGMONSTERSTORAGE_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGMONSTERSTORAGE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BuildingDungeon.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Monster storage
#define BUILDING_MONSTER_STORAGE_UPGRADE_COST 20000 // in mana
#define BUILDING_MONSTER_STORAGE_MAX_LEVEL    50
#define BUILDING_MONSTER_STORAGE_MAX_ROOM     500 // 10 * 50

/////////////////////////////////////////////////////////////////////////////////
// The BuildingMonsterStorage class
class BuildingMonsterStorage : public Building
{
public:
    BuildingMonsterStorage( BuildingDungeon * pDungeon );
    virtual ~BuildingMonsterStorage();

    // Monster storage
    inline UInt GetStorageLevel() const;
    inline UInt GetStorageRoom() const;

    Bool UpgradeStorageRoom();

    inline UInt GetMonsterCount() const;
    inline MonsterInstance * GetMonster( UInt iIndex ) const;

    Bool StoreMonster( UInt iIndex );
    Bool RetrieveMonster( UInt iStorageIndex );

private:
    // Helpers
    static Int _Compare_MonsterInstance( MonsterInstance * const & pLeft, MonsterInstance * const & pRight );

    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Monster storage
    UInt m_iStorageLevel;
    UInt m_iStorageRoom;

    Array<MonsterInstance*> m_arrMonsterStorage;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BuildingMonsterStorage.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGMONSTERSTORAGE_H

