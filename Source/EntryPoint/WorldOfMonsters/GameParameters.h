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

    // XML serialization
    Void Load( const XMLNode * pNode );

    // String conversions
    const GChar * CurrencyTypeToString( CurrencyType iCurrencyType ) const;
    CurrencyType CurrencyTypeFromString( const GChar * strValue ) const;

    const GChar * ScrollTypeToString( ScrollType iScrollType ) const;
    ScrollType ScrollTypeFromString( const GChar * strValue ) const;

    const GChar * MonsterTypeToString( MonsterType iMonsterType ) const;
    MonsterType MonsterTypeFromString( const GChar * strValue ) const;

    const GChar * MonsterElementToString( MonsterElement iMonsterElement ) const;
    MonsterElement MonsterElementFromString( const GChar * strValue ) const;

    const GChar * MonsterStatisticToString( MonsterStatistic iMonsterStatistic ) const;
    MonsterStatistic MonsterStatisticFromString( const GChar * strValue ) const;

    const GChar * MonsterAwakeningBonusToString( MonsterAwakeningBonus iMonsterAwakeningBonus ) const;
    MonsterAwakeningBonus MonsterAwakeningBonusFromString( const GChar * strValue ) const;

    const GChar * StatusEffectTypeToString( StatusEffectType iStatusEffectType ) const;
    StatusEffectType StatusEffectTypeFromString( const GChar * strValue ) const;

    const GChar * SkillEffectTypeToString( SkillEffectType iSkillEffectType ) const;
    SkillEffectType SkillEffectTypeFromString( const GChar * strValue ) const;

    const GChar * SkillTargetPatternToString( SkillTargetPattern iSkillTargetPattern ) const;
    SkillTargetPattern SkillTargetPatternFromString( const GChar * strValue ) const;

    const GChar * SkillEffectScalingToString( SkillEffectScaling iSkillEffectScaling ) const;
    SkillEffectScaling SkillEffectScalingFromString( const GChar * strValue ) const;

    const GChar * SkillTypeToString( SkillType iSkillType ) const;
    SkillType SkillTypeFromString( const GChar * strValue ) const;

    const GChar * SkillActiveTypeToString( SkillActiveType iSkillActiveType ) const;
    SkillActiveType SkillActiveTypeFromString( const GChar * strValue ) const;

    const GChar * SkillPassiveTypeToString( SkillPassiveType iSkillPassiveType ) const;
    SkillPassiveType SkillPassiveTypeFromString( const GChar * strValue ) const;

    const GChar * SkillLeaderConstraintToString( SkillLeaderConstraint iSkillLeaderConstraint ) const;
    SkillLeaderConstraint SkillLeaderConstraintFromString( const GChar * strValue ) const;

    const GChar * RuneTypeToString( RuneType iRuneType ) const;
    RuneType RuneTypeFromString( const GChar * strValue ) const;

    const GChar * RuneStatisticToString( RuneStatistic iRuneStatistic ) const;
    RuneStatistic RuneStatisticFromString( const GChar * strValue ) const;

    // Monster parameters
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
    inline UInt GetMonsterXPFromPowerUp( UInt iFoodRank, UInt iFoodLevel ) const;

    // Skill parameters
    
    // Rune parameters
    inline Bool IsRuneSlotAllowed( UInt iSlot, MonsterStatistic iStat, Bool bRatio ) const;

    inline UInt GetRuneSetRequiredCount( RuneType iType ) const;
    inline Float GetRuneSetStatBonus( RuneType iType ) const;

    inline const RuneLevelingStats * GetRuneLevelingStats() const;

    // Town parameters
    inline const CurrencyCost * GetBuildingCost( BuildingType iType ) const;

    inline const CurrencyCost * GetEssenceFusionUnlockCost() const;
    inline const CurrencyCost * GetMonsterSummoningUnlockCost() const;
    inline const CurrencyCost * GetMonsterFusionUnlockCost() const;
    inline const CurrencyCost * GetMonsterPowerUpUnlockCost() const;
    inline const CurrencyCost * GetMonsterEvolutionUnlockCost() const;
    inline const CurrencyCost * GetMonsterAwakeningUnlockCost() const;
    inline const CurrencyCost * GetRunePowerUpUnlockCost() const;
    inline const CurrencyCost * GetRuneEvolutionUnlockCost() const;

    inline UInt GetEssenceFusionCost( EssenceType iType ) const;
    inline const CurrencyCost * GetMonsterFusionCost( UInt iRank ) const;
    inline const CurrencyCost * GetMonsterPowerUpCost( UInt iTargetRank, UInt iFoodRank ) const;
    inline const CurrencyCost * GetMonsterEvolutionCost( UInt iRank ) const;
    inline const CurrencyCost * GetRunePowerUpCost( UInt iTargetRank, UInt iFoodRank ) const;
    inline const CurrencyCost * GetRuneEvolutionCost( UInt iRank ) const;

    //inline UInt GetRuneStatCost( UInt iRank, MonsterStatistic iStat ) const;

    inline const CurrencyCost * GetMonsterCollectionUpgradeCost( UInt iLevel ) const;
    inline const CurrencyCost * GetMonsterStorageUpgradeCost( UInt iLevel ) const;
    inline UInt GetMonsterCollectionRoom( UInt iLevel ) const;
    inline UInt GetMonsterStorageRoom( UInt iLevel ) const;

    inline const CurrencyCost * GetRuneCollectionUpgradeCost( UInt iLevel ) const;
    inline const CurrencyCost * GetRuneStorageUpgradeCost( UInt iLevel ) const;
    inline UInt GetRuneCollectionRoom( UInt iLevel ) const;
    inline UInt GetRuneStorageRoom( UInt iLevel ) const;

    inline const CurrencyCost * GetManaProductionRateUpgradeCost( UInt iLevel ) const;
    inline const CurrencyCost * GetManaCapacityUpgradeCost( UInt iLevel ) const;
    inline UInt GetManaProductionRate( UInt iLevel ) const;
    inline UInt GetManaCapacity( UInt iLevel ) const;
    
    inline const CurrencyCost * GetCrystalProductionRateUpgradeCost( UInt iLevel ) const;
    inline const CurrencyCost * GetCrystalCapacityUpgradeCost( UInt iLevel ) const;
    inline UInt GetCrystalProductionRate( UInt iLevel ) const;
    inline UInt GetCrystalCapacity( UInt iLevel ) const;

private:
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
    UInt m_arrMonsterXPFromPowerUp[MONSTER_MAX_RANK][MONSTER_MAX_LEVEL];

    // Skill parameters

    // Rune parameters
    Byte m_arrRuneSlotsAllowed[2][MONSTER_STAT_COUNT]; // (flat/ratio,type)

    UInt m_arrRuneSetRequiredCount[RUNE_TYPE_COUNT];
    Float m_arrRuneSetStatBonus[RUNE_TYPE_COUNT];

    RuneLevelingStats m_hRuneLevelingStats;

    // Town parameters
    CurrencyCost m_arrBuildingCost[BUILDING_COUNT];

    CurrencyCost m_hEssenceFusionUnlockCost;
    CurrencyCost m_hMonsterSummoningUnlockCost;
    CurrencyCost m_hMonsterFusionUnlockCost;
    CurrencyCost m_hMonsterPowerUpUnlockCost;
    CurrencyCost m_hMonsterEvolutionUnlockCost;
    CurrencyCost m_hMonsterAwakeningUnlockCost;
    CurrencyCost m_hRunePowerUpUnlockCost;
    CurrencyCost m_hRuneEvolutionUnlockCost;

    UInt m_arrEssenceFusionCost[ESSENCE_TYPE_COUNT];
    CurrencyCost m_arrMonsterFusionCost[MONSTER_MAX_RANK];
    CurrencyCost m_arrMonsterPowerUpCost[MONSTER_MAX_RANK][MONSTER_MAX_RANK];
    CurrencyCost m_arrMonsterEvolutionCost[MONSTER_MAX_RANK];
    CurrencyCost m_arrRunePowerUpCosts[MONSTER_MAX_RANK][MONSTER_MAX_RANK];
    CurrencyCost m_arrRuneEvolutionCosts[MONSTER_MAX_RANK];

    //UInt m_arrRuneStatCosts[MONSTER_STAT_COUNT][MONSTER_MAX_RANK];

    CurrencyCost m_arrMonsterCollectionUpgradeCost[MONSTER_COLLECTION_MAX_LEVEL];
    CurrencyCost m_arrMonsterStorageUpgradeCost[MONSTER_STORAGE_MAX_LEVEL];
    UInt m_arrMonsterCollectionRoom[MONSTER_COLLECTION_MAX_LEVEL];
    UInt m_arrMonsterStorageRoom[MONSTER_STORAGE_MAX_LEVEL];

    CurrencyCost m_arrRuneCollectionUpgradeCost[RUNE_COLLECTION_MAX_LEVEL];
    CurrencyCost m_arrRuneStorageUpgradeCost[RUNE_STORAGE_MAX_LEVEL];
    UInt m_arrRuneCollectionRoom[RUNE_COLLECTION_MAX_LEVEL];
    UInt m_arrRuneStorageRoom[RUNE_STORAGE_MAX_LEVEL];

    CurrencyCost m_arrManaProductionRateUpgradeCost[BUILDING_MAX_LEVEL];
    CurrencyCost m_arrManaCapacityUpgradeCost[BUILDING_MAX_LEVEL];
    UInt m_arrManaProductionRate[BUILDING_MAX_LEVEL];
    UInt m_arrManaCapacity[BUILDING_MAX_LEVEL];

    CurrencyCost m_arrCrystalProductionRateUpgradeCost[BUILDING_MAX_LEVEL];
    CurrencyCost m_arrCrystalCapacityUpgradeCost[BUILDING_MAX_LEVEL];
    UInt m_arrCrystalProductionRate[BUILDING_MAX_LEVEL];
    UInt m_arrCrystalCapacity[BUILDING_MAX_LEVEL];


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


};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GameParameters.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPARAMETERS_H


