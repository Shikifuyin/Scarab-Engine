/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingDungeon.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingDungeon interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGDUNGEON_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGDUNGEON_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Building.h"

#include "../Monsters/Monster.h"
#include "../Battle/Battle.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Currencies


    // Monsters
#define BUILDING_MONSTER_COLLECTION_UPGRADE_COST 30000 // in mana
#define BUILDING_MONSTER_COLLECTION_MAX_LEVEL    20
#define BUILDING_MONSTER_COLLECTION_MAX_ROOM     100 // 5 * 20

    // Runes
#define BUILDING_RUNE_COLLECTION_UPGRADE_COST 20000 // in mana
#define BUILDING_RUNE_COLLECTION_MAX_LEVEL    16
#define BUILDING_RUNE_COLLECTION_MAX_ROOM     256  // 16 * 16

    // Arena
enum ArenaRank {
    ARENA_RANK_BEGINNER = 0,
    ARENA_RANK_CHALLENGER,
    ARENA_RANK_FIGHTER,
    ARENA_RANK_CONQUEROR,
    ARENA_RANK_GUARDIAN,
    ARENA_RANK_LEGEND,

    ARENA_RANK_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The BuildingDungeon class
class BuildingDungeon : public Building
{
public:
    BuildingDungeon();
    virtual ~BuildingDungeon();

    // Currencies
    inline UInt GetCurrency( CurrencyType iType ) const;

    inline Void AddCurrency( CurrencyType iType, UInt iAmount );
    inline Void RemoveCurrency( CurrencyType iType, UInt iAmount );
    inline Void SetCurrency( CurrencyType iType, UInt iAmount );

    // Monster collection
    inline UInt GetMonsterCollectionLevel() const;
    inline UInt GetMonsterCollectionRoom() const;

    Bool UpgradeMonsterCollectionRoom();

    inline UInt GetMonsterCount() const;
    inline MonsterInstance * GetMonster( UInt iIndex ) const;

    Void AddMonster( MonsterInstance * pMonster );
    MonsterInstance * RemoveMonster( UInt iIndex );

    // Rune collection
    inline UInt GetRuneCollectionLevel() const;
    inline UInt GetRuneCollectionRoom() const;

    Bool UpgradeRuneCollectionRoom();

    inline UInt GetRuneCount( RuneType iType ) const;
    inline Rune * GetRune( RuneType iType, UInt iIndex ) const;

    Void AddRune( Rune * pRune );
    Rune * RemoveRune( RuneType iType, UInt iIndex );

    // Arena state
    inline UInt GetArenaScore() const;
    inline ArenaRank GetArenaRank() const;

    inline Void SetArenaScore( UInt iArenaScore );
    inline Void SetArenaRank( ArenaRank iRank );

    inline MonsterInstance * GetArenaDefenseMonster( UInt iIndex ) const;
    inline Void SetArenaDefenseMonster( UInt iIndex, MonsterInstance * pMonsterInstance );

private:
    // Helpers
    static Int _Compare_MonsterInstance( MonsterInstance * const & pLeft, MonsterInstance * const & pRight );
    static Int _Compare_Rune( Rune * const & pLeft, Rune * const & pRight );

    // Currencies
    UInt m_arrCurrencies[CURRENCY_COUNT];

    // Monster collection
    UInt m_iMonsterCollectionLevel;
    UInt m_iMonsterCollectionRoom;

    Array<MonsterInstance*> m_arrMonsterCollection;

    // Rune collection
    UInt m_iRuneCollectionLevel;
    UInt m_iRuneCollectionRoom;

    Array<Rune*> m_arrRuneCollection[RUNE_TYPE_COUNT];

    // Arena state
    UInt m_iArenaScore;
    ArenaRank m_iArenaRank;

    MonsterInstance * m_arrArenaDefense[BATTLE_TEAMSIZE_ARENA]; // first is leader
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BuildingDungeon.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGDUNGEON_H

