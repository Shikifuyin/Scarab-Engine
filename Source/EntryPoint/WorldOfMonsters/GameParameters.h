/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/GameParameters.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters : Game parameters interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPARAMETERS_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPARAMETERS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Math/Random/Random.h"

#include "../../Lib/Datastruct/Array/Array.h"
#include "../../Lib/Datastruct/Table/TreeMap.h"

#include "Monsters\StatusEffect.h"
#include "Monsters\Skill.h"
#include "Monsters\Rune.h"
#include "Monsters\Monster.h"

#include "Battle\Battle.h"

#include "Town\PlayerTown.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The GameParameters class
class GameParameters
{
public:
    GameParameters();
    ~GameParameters();

    // Deferred loading
    Void LoadFromXML( const GChar * strParameterFile );

    // Global parameters

    // Leveling parameters
    inline Float GetMonsterBaseCritRate() const;
    inline Float GetMonsterBaseCritDamage() const;
    inline Float GetMonsterBaseAccuracy() const;
    inline Float GetMonsterBaseResistance() const;

    inline UInt GetAwakeningBonusBaseSpeed() const;
    inline UInt GetAwakeningBonusSpeed() const;
    inline Float GetAwakeningBonusCritRate() const;
    inline Float GetAwakeningBonusAccuracy() const;
    inline Float GetAwakeningBonusResistance() const;

    inline UInt GetMonsterXPByLevel( UInt iRank, UInt iLevel ) const;

    // Rune parameters
    inline UInt GetSetCount( RuneType iType ) const;
    inline Bool IsSlotAllowed( UInt iSlot, MonsterStatistic iStat, Bool bRatio ) const;

    inline Float GetRuneSetStatBonus( RuneType iType ) const;

    inline const RuneLevelingStats * GetRuneLevelingStats() const;

    // Economy parameters
    inline const CurrencyCost * GetMonsterCollectionUpgradeCost() const;
    inline const CurrencyCost * GetRuneCollectionUpgradeCost() const;

    inline const CurrencyCost * GetBuildingCost( BuildingType iType ) const;
    inline UInt GetBuildingUpgradeCost( BuildingType iType, CurrencyType * outCurrencyType ) const;

    inline UInt GetEssenceFusionCost( EssenceType iType ) const;

    inline UInt GetMonsterSummoningCost( UInt iNativeRank ) const;
    inline const CurrencyCost * GetMonsterPowerUpCost( UInt iTargetRank, UInt iFoodRank ) const;
    inline const CurrencyCost * GetMonsterEvolutionCost( UInt iRank ) const;
    inline const CurrencyCost * GetMonsterFusionCost( UInt iRank ) const;

    inline const CurrencyCost * GetRunePowerUpCost( UInt iTargetRank, UInt iFoodRank ) const;
    inline const CurrencyCost * GetRuneEvolutionCost( UInt iRank ) const;

    inline UInt GetRuneStatCost( UInt iRank, MonsterStatistic iStat ) const;

private:
    // Helpers

    // Global parameters

    // Monster parameters
    Float m_fMonsterBaseCritRate;
    Float m_fMonsterBaseCritDamage;
    Float m_fMonsterBaseAccuracy;
    Float m_fMonsterBaseResistance;

    UInt m_iAwakeningBonusBaseSpeed;
    UInt m_iAwakeningBonusSpeed;
    Float m_fAwakeningBonusCritRate;
    Float m_fAwakeningBonusAccuracy;
    Float m_fAwakeningBonusResistance;

    UInt m_arrMonsterXPByLevel[MONSTER_MAX_RANK][MONSTER_MAX_LEVEL];

    // Skill parameters

    // Rune parameters
    UInt m_arrRuneSetCounts[RUNE_TYPE_COUNT];
    Byte m_arrRuneAllowedSlots[2][MONSTER_STAT_COUNT]; // (flat/ratio,type)

    RuneLevelingStats m_hRuneLevelingStats;

    //UInt RuneLevelingStats::GetSetCount( RuneType iType )
    //{
    //    Assert( iType < RUNE_TYPE_COUNT );
    //    static UInt s_arrSetCounts[RUNE_TYPE_COUNT] = {
    //        2, 4, 2, 4,
    //        2, 4, 2, 2,
    //        2, 2, 2, 2,
    //        4, 4, 4, 2
    //    };
    //    return s_arrSetCounts[iType];
    //}
    //const Byte RuneLevelingStats::sm_arrAllowedSlots[2][MONSTER_STAT_COUNT] = {
    //    { // flat
    //        0x3f, // MONSTER_STAT_HEALTH
    //        0x3f, // MONSTER_STAT_ATTACK
    //    0x3f, // MONSTER_STAT_DEFENSE
    //    0x02, // MONSTER_STAT_SPEED
    //    0x00, // MONSTER_STAT_CRIT_RATE  (doesn't exist)
    //    0x00, // MONSTER_STAT_CRIT_DMG   (doesn't exist)
    //    0x00, // MONSTER_STAT_ACCURACY   (doesn't exist)
    //    0x00  // MONSTER_STAT_RESISTANCE (doesn't exist)
    //    },
    //    { // ratio
    //        0x2a, // MONSTER_STAT_HEALTH
    //        0x2a, // MONSTER_STAT_ATTACK
    //    0x2a, // MONSTER_STAT_DEFENSE
    //    0x00, // MONSTER_STAT_SPEED (doesn't exist)
    //    0x08, // MONSTER_STAT_CRIT_RATE
    //    0x08, // MONSTER_STAT_CRIT_DMG
    //    0x20, // MONSTER_STAT_ACCURACY
    //    0x20  // MONSTER_STAT_RESISTANCE
    //    }
    //};


    // Building parameters
    CurrencyCost m_arrBuildingCost[BUILDING_COUNT];

    CurrencyCost m_arrMonsterCollectionUpgradeCost[BUILDING_DUNGEON_MONSTER_COLLECTION_MAX_LEVEL];
    CurrencyCost m_arrRuneCollectionUpgradeCost[BUILDING_DUNGEON_RUNE_COLLECTION_MAX_LEVEL];

    CurrencyCost m_hMonsterAwakeningUnlockCost;
    CurrencyCost m_hEssenceFusionUnlockCost;
    UInt m_arrEssenceFusionCost[ESSENCE_TYPE_COUNT];

    CurrencyCost m_arrMonsterPowerUpCost[MONSTER_MAX_RANK][MONSTER_MAX_RANK];
    UInt m_arrMonsterPowerUpXP[MONSTER_MAX_RANK][MONSTER_MAX_LEVEL];

    CurrencyCost m_arrMonsterEvolutionCost[MONSTER_MAX_RANK];
    CurrencyCost m_arrMonsterFusionCost[MONSTER_MAX_RANK];

    CurrencyCost m_arrRunePowerUpCosts[MONSTER_MAX_RANK][MONSTER_MAX_RANK];
    CurrencyCost m_arrRuneEvolutionCosts[MONSTER_MAX_RANK];


    CurrencyCost m_arrMonsterStorageUpgradeCost[BUILDING_MONSTER_STORAGE_MAX_LEVEL];
    CurrencyCost m_arrRuneStorageUpgradeCost[BUILDING_RUNE_STORAGE_MAX_LEVEL];

    CurrencyCost m_arrManaProductionRateUpgradeCost[BUILDING_MAX_LEVEL];
    CurrencyCost m_arrManaCapacityUpgradeCost[BUILDING_MAX_LEVEL];
    CurrencyCost m_arrCrystalProductionRateUpgradeCost[BUILDING_MAX_LEVEL];
    CurrencyCost m_arrCrystalCapacityUpgradeCost[BUILDING_MAX_LEVEL];





    UInt m_arrRuneStatCosts[MONSTER_STAT_COUNT][MONSTER_MAX_RANK];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GameParameters.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPARAMETERS_H


