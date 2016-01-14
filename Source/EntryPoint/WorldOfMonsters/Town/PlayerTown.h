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
#include "../Monsters/Monster.h"

#include "Building.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Currencies : resources
enum CurrencyType {
    CURRENCY_MANA = 0,
    CURRENCY_CRYSTAL,
    CURRENCY_GLORYPOINT,
    CURRENCY_GUILDPOINT,

    CURRENCY_COUNT
};

typedef struct _currency_cost {
    UInt arrCost[CURRENCY_COUNT];
} CurrencyCost;

    // Monsters definitions
#define MONSTER_COLLECTION_MAX_LEVEL 20
#define MONSTER_COLLECTION_MAX_ROOM  100 // 5 * 20

#define MONSTER_STORAGE_MAX_LEVEL 50
#define MONSTER_STORAGE_MAX_ROOM  500 // 10 * 50

    // Runes definitions
#define RUNE_COLLECTION_MAX_LEVEL 16
#define RUNE_COLLECTION_MAX_ROOM  256  // 16 * 16

#define RUNE_STORAGE_MAX_LEVEL 64
#define RUNE_STORAGE_MAX_ROOM  1024 // 16 * 64

    // Arena definitions
#define BATTLE_TEAMSIZE_ARENA 4

enum ArenaRank {
    ARENA_RANK_BEGINNER = 0,
    ARENA_RANK_CHALLENGER,
    ARENA_RANK_FIGHTER,
    ARENA_RANK_CONQUEROR,
    ARENA_RANK_GUARDIAN,
    ARENA_RANK_LEGEND,

    ARENA_RANK_COUNT
};

    // Prototypes
class PlayerGuild;

/////////////////////////////////////////////////////////////////////////////////
// The PlayerTown class
class PlayerTown
{
public:
    PlayerTown();
    ~PlayerTown();

    // XML serialization
    Void Load( const XMLNode * pNode );
    Void Save( XMLNode * outNode ) const;

    // Currencies, scrolls & essences
    inline UInt GetCurrency( CurrencyType iType ) const;
    inline UInt GetScrollCount( ScrollType iType ) const;
    inline UInt GetEssenceCount( MonsterElement iElement, EssenceType iType ) const;

    inline Void AddCurrency( CurrencyType iType, UInt iAmount );
    inline Void AddScrolls( ScrollType iType, UInt iCount );
    inline Void AddEssences( MonsterElement iElement, EssenceType iType, UInt iCount );

    inline Void RemoveCurrency( CurrencyType iType, UInt iAmount );
    inline Void RemoveScrolls( ScrollType iType, UInt iCount );
    inline Void RemoveEssences( MonsterElement iElement, EssenceType iType, UInt iCount );

    Bool CheckCurrencyCost( const CurrencyCost * pCost ) const;
    Bool CheckScrollCost( const ScrollCost * pCost ) const;
    Bool CheckEssenceCost( const EssenceCost * pCost ) const;

    Void PayCurrencyCost( const CurrencyCost * pCost );
    Void PayScrollCost( const ScrollCost * pCost );
    Void PayEssenceCost( const EssenceCost * pCost );

    // Monster collection & storage
    inline UInt GetMonsterCollectionLevel() const;
    inline UInt GetMonsterStorageLevel() const;
    inline UInt GetMonsterCollectionRoom() const;
    inline UInt GetMonsterStorageRoom() const;
    Bool UpgradeMonsterCollection();
    Bool UpgradeMonsterStorage();

    inline UInt GetMonsterCount() const;
    inline UInt GetStoredMonsterCount() const;
    inline MonsterInstance * GetMonster( UInt iIndex );
    inline MonsterInstance * GetStoredMonster( UInt iStorageIndex );

    Void UnSummonMonster( UInt iIndex, MonsterInstance * outMonster ); // DANGER !!!

    Bool StoreMonster( UInt iIndex );
    Bool RetrieveMonster( UInt iStorageIndex );

    // Rune collection & storage
    inline UInt GetRuneCollectionLevel() const;
    inline UInt GetRuneStorageLevel() const;
    inline UInt GetRuneCollectionRoom() const;
    inline UInt GetRuneStorageRoom() const;
    Bool UpgradeRuneCollection();
    Bool UpgradeRuneStorage();

    inline UInt GetRuneCount( RuneType iType ) const;
    inline UInt GetStoredRuneCount( RuneType iType ) const;
    inline Rune * GetRune( RuneType iType, UInt iIndex );
    inline Rune * GetStoredRune( RuneType iType, UInt iStorageIndex );

    Rune * AddRune( const Rune & hRune );
    Void RemoveRune( RuneType iType, UInt iIndex, Rune * outRune );

    Bool StoreRune( RuneType iType, UInt iIndex );
    Bool RetrieveRune( RuneType iType, UInt iStorageIndex );

    // Mana & Crystal production
    inline UInt GetManaProductionRateLevel() const;
    inline UInt GetCrystalProductionRateLevel() const;
    inline UInt GetManaProductionRate() const;
    inline UInt GetCrystalProductionRate() const;
    Bool UpgradeManaProductionRate();
    Bool UpgradeCrystalProductionRate();

    inline UInt GetManaCapacityLevel() const;
    inline UInt GetCrystalCapacityLevel() const;
    inline UInt GetManaCapacity() const;
    inline UInt GetCrystalCapacity() const;
    Bool UpgradeManaCapacity();
    Bool UpgradeCrystalCapacity();

    inline Bool IsManaBufferEmpty() const;
    inline Bool IsCrystalBufferEmpty() const;
    inline UInt GetManaBuffer() const;
    inline UInt GetCrystalBuffer() const;

    Void RetrieveMana();
    Void RetrieveCrystals();
    Void UpdateManaAndCrystals();

    // Shop

    // Wishes

    // Building operations
    inline Bool EssenceFusionUnlocked() const;
    inline Bool MonsterSummoningUnlocked() const;
    inline Bool MonsterFusionUnlocked() const;
    inline Bool MonsterPowerUpUnlocked() const;
    inline Bool MonsterEvolutionUnlocked() const;
    inline Bool MonsterAwakeningUnlocked() const;
    inline Bool RunePowerUpUnlocked() const;
    inline Bool RuneEvolutionUnlocked() const;

    Bool UnlockEssenceFusion();
    Bool UnlockMonsterSummoning();
    Bool UnlockMonsterFusion();
    Bool UnlockMonsterPowerUp();
    Bool UnlockMonsterEvolution();
    Bool UnlockMonsterAwakening();
    Bool UnlockRunePowerUp();
    Bool UnlockRuneEvolution();

    Bool CanFuseEssences( MonsterElement iElement, EssenceType iType ) const;
    Bool CanSummonMonster( const Monster * pMonster ) const;
    Bool CanFuseMonster( const Monster * pMonster, UInt arrFoodMonsters[4] ) const;
    Bool CanPowerUpMonster( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount ) const;
    Bool CanEvolveMonster( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount ) const;
    Bool CanAwakeMonster( UInt iTargetMonster ) const;
    Bool CanPowerUpRune( RuneType iRuneType, UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount ) const;
    Bool CanEvolveRune( RuneType iRuneType, UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount ) const;

    Void FuseEssences( MonsterElement iElement, EssenceType iType );
    Void SummonMonster( const Monster * pMonster );
    Void FuseMonster( const Monster * pMonster, UInt arrFoodMonsters[4] );
    Void PowerUpMonster( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount );
    Void EvolveMonster( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount );
    Void AwakeMonster( UInt iTargetMonster );
    Void PowerUpRune( RuneType iRuneType, UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount );
    Void EvolveRune( RuneType iRuneType, UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount );

    // Arena
    inline UInt GetArenaScore() const;
    inline ArenaRank GetArenaRank() const;

    inline Void SetArenaScore( UInt iArenaScore );
    inline Void SetArenaRank( ArenaRank iRank );

    inline UInt GetArenaDefenseMonster( UInt iIndex ) const;
    inline Void SetArenaDefenseMonster( UInt iIndex, UInt iMonsterCollectionIndex );

    // Guild access
    inline Bool HasGuild() const;
    inline PlayerGuild * GetGuild() const;

private:
    // Helpers
    static Int _Compare_UInt( const UInt & rLeft, const UInt & rRight, Void * pUserData );
    static Int _Compare_MonsterInstance( const MonsterInstance & rLeft, const MonsterInstance & rRight );
    static Int _Compare_Rune( const Rune & rLeft, const Rune & rRight );

    // Currencies, scrolls & essences
    UInt m_arrCurrencies[CURRENCY_COUNT];
    UInt m_arrScrolls[SCROLL_TYPE_COUNT];
    UInt m_arrEssences[MONSTER_ELEMENT_COUNT][ESSENCE_TYPE_COUNT];

    // Monster collection & storage
    UInt m_iMonsterCollectionLevel;
    UInt m_iMonsterStorageLevel;
    UInt m_iMonsterCollectionRoom;
    UInt m_iMonsterStorageRoom;
    Array<MonsterInstance> m_arrMonsterCollection;
    Array<MonsterInstance> m_arrMonsterStorage;

    // Rune collection & storage
    UInt m_iRuneCollectionLevel;
    UInt m_iRuneStorageLevel;
    UInt m_iRuneCollectionRoom;
    UInt m_iRuneStorageRoom;
    Array<Rune> m_arrRuneCollection[RUNE_TYPE_COUNT];
    Array<Rune> m_arrRuneStorage[RUNE_TYPE_COUNT];

    // Mana & Crystal production
    UInt m_iManaProductionRateLevel;
    UInt m_iCrystalProductionRateLevel;
    UInt m_iManaProductionRate; // in mana / hour
    UInt m_iCrystalProductionRate; // in crystals / hour

    UInt m_iManaCapacityLevel;
    UInt m_iCrystalCapacityLevel;
    UInt m_iManaCapacity;
    UInt m_iCrystalCapacity;

    UInt m_iManaBuffer;
    UInt m_iCrystalBuffer;

    TimeValue m_hLastUpdateTime;

    // Shop

    // Wishes

    // Building operations
    Bool m_bEssenceFusionUnlocked;
    Bool m_bMonsterSummoningUnlocked;
    Bool m_bMonsterFusionUnlocked;
    Bool m_bMonsterPowerUpUnlocked;
    Bool m_bMonsterEvolutionUnlocked;
    Bool m_bMonsterAwakeningUnlocked;
    Bool m_bRunePowerUpUnlocked;
    Bool m_bRuneEvolutionUnlocked;

    // Arena
    UInt m_iArenaScore;
    ArenaRank m_iArenaRank;

    UInt m_arrArenaDefense[BATTLE_TEAMSIZE_ARENA]; // first is leader

    // Guild
    PlayerGuild * m_pGuild;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PlayerTown.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_PLAYERTOWN_H

