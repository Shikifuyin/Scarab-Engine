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
#include "GameDefinitions.h"

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

    // Rune parameters
    inline UInt GetSetCount( RuneType iType ) const;
    inline Bool IsSlotAllowed( UInt iSlot, MonsterStatistic iStat, Bool bRatio ) const;

    inline Float GetRuneSetStatBonus( RuneType iType ) const;

    // Economy parameters
    inline UInt GetBuildingCost( BuildingType iType, CurrencyType * outCurrencyType ) const;
    inline UInt GetBuildingUpgradeCost( BuildingType iType, CurrencyType * outCurrencyType ) const;

    inline UInt GetEssenceFusionCost( EssenceType iType ) const;

    inline UInt GetMonsterSummoningCost( UInt iNativeRank ) const;
    inline UInt GetMonsterPowerUpCost( UInt iRank ) const;
    inline UInt GetMonsterEvolutionCost( UInt iRank ) const;
    inline UInt GetMonsterFusionCost( UInt iRank ) const;

    inline UInt GetRunePowerUpCost( UInt iRank ) const;
    inline UInt GetRuneEvolutionCost( UInt iRank ) const;
    inline UInt GetRuneStatCost( UInt iRank, MonsterStatistic iStat ) const;

private:
    // Helpers

    // Global parameters

    // Leveling parameters
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

    // Rune parameters
    UInt m_arrRuneSetCounts[RUNE_TYPE_COUNT];
    Byte m_arrRuneAllowedSlots[2][MONSTER_STAT_COUNT]; // (flat/ratio,type)

    Float m_arrRuneSetStatBonus[RUNE_TYPE_COUNT];

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


    // Economy parameters
    CurrencyType m_arrBuildingCostCurrencies[BUILDING_COUNT];
    UInt m_arrBuildingCosts[BUILDING_COUNT];
    CurrencyType m_arrBuildingUpgradeCostCurrencies[BUILDING_COUNT];
    UInt m_arrBuildingUpgradeCosts[BUILDING_COUNT][BUILDING_MAX_LEVEL];

    UInt m_arrEssenceFusionCosts[ESSENCE_TYPE_COUNT];

    UInt m_arrMonsterSummoningCosts[MONSTER_MAX_RANK];
    UInt m_arrMonsterPowerUpCosts[MONSTER_MAX_RANK][MONSTER_MAX_RANK];
    UInt m_arrMonsterEvolutionCosts[MONSTER_MAX_RANK];
    UInt m_arrMonsterAwakeningCosts[MONSTER_MAX_RANK];
    UInt m_arrMonsterFusionCosts[MONSTER_MAX_RANK];

    UInt m_arrRunePowerUpCosts[MONSTER_MAX_RANK];
    UInt m_arrRuneEvolutionCosts[MONSTER_MAX_RANK];
    UInt m_arrRuneStatCosts[MONSTER_STAT_COUNT][MONSTER_MAX_RANK];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GameParameters.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPARAMETERS_H


