/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingRuneStorage.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingRuneStorage interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGRUNESTORAGE_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGRUNESTORAGE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BuildingDungeon.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Monster storage
#define BUILDING_RUNE_STORAGE_UPGRADE_COST 20000 // in mana
#define BUILDING_RUNE_STORAGE_MAX_LEVEL    64
#define BUILDING_RUNE_STORAGE_MAX_ROOM     1024 // 16 * 64

/////////////////////////////////////////////////////////////////////////////////
// The BuildingRuneStorage class
class BuildingRuneStorage : public Building
{
public:
    BuildingRuneStorage( BuildingDungeon * pDungeon );
    virtual ~BuildingRuneStorage();

    // Rune storage
    inline UInt GetStorageLevel() const;
    inline UInt GetStorageRoom() const;

    Bool UpgradeStorageRoom();

    inline UInt GetRuneCount( RuneType iType ) const;
    inline Rune * GetRune( RuneType iType, UInt iStorageIndex ) const;

    Bool StoreRune( RuneType iType, UInt iIndex );
    Bool RetrieveRune( RuneType iType, UInt iStorageIndex );

private:
    // Helpers
    static Int _Compare_Rune( Rune * const & pLeft, Rune * const & pRight );

    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Rune storage
    UInt m_iStorageLevel;
    UInt m_iStorageRoom;

    Array<Rune*> m_arrRuneStorage[RUNE_TYPE_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BuildingRuneStorage.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGRUNESTORAGE_H

