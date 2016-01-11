/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/LevelingStats.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : LevelingStats interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_LEVELINGSTATS_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_LEVELINGSTATS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../GameParameters.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MonsterLevelingStats class
class MonsterLevelingStats
{
public:
    MonsterLevelingStats();
    ~MonsterLevelingStats();

    // Deferred loading
    Void Load( XMLNode * pNode );

    // Leveling stats access
    inline UInt GetHP( UInt iRank, UInt iLevel, Bool bAwakened ) const;
    inline UInt GetATT( UInt iRank, UInt iLevel, Bool bAwakened ) const;
    inline UInt GetDEF( UInt iRank, UInt iLevel, Bool bAwakened ) const;
    UInt GetSPD( Bool bAwakened ) const;

    Float GetCritRate( Bool bAwakened ) const;
    Float GetCritDmg( Bool bAwakened ) const;
    Float GetAccuracy( Bool bAwakened ) const;
    Float GetResistance( Bool bAwakened ) const;

    inline MonsterAwakeningBonus GetAwakeningBonus() const;

private:
    // Leveling stats
    UInt m_arrHealth[MONSTER_MAX_RANK][MONSTER_MAX_LEVEL];
    UInt m_arrHealthAwaken[MONSTER_MAX_RANK][MONSTER_MAX_LEVEL];

    UInt m_arrAttack[MONSTER_MAX_RANK][MONSTER_MAX_LEVEL];
    UInt m_arrAttackAwaken[MONSTER_MAX_RANK][MONSTER_MAX_LEVEL];

    UInt m_arrDefense[MONSTER_MAX_RANK][MONSTER_MAX_LEVEL];
    UInt m_arrDefenseAwaken[MONSTER_MAX_RANK][MONSTER_MAX_LEVEL];

    UInt m_iSpeed;

    MonsterAwakeningBonus m_iAwakeningBonus;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillLevelingStats class
class SkillLevelingStats
{
public:
    SkillLevelingStats();
    ~SkillLevelingStats();

    // Deferred loading
    Void Load( XMLNode * pNode );

    // Leveling stats access
    inline UInt GetMaxLevel() const;

    inline Float GetBonusDamage( UInt iLevel ) const;
    inline Float GetBonusRecovery( UInt iLevel ) const;
    inline Float GetBonusStatusEffectRate( UInt iLevel ) const;
    inline Float GetBonusSpecific( UInt iLevel ) const;

    inline UInt GetCooldown( UInt iLevel ) const;

private:
    // Leveling stats
    UInt m_iMaxLevel; // 0 = no level

    Float m_arrBonusDamage[SKILL_MAX_LEVEL];
    Float m_arrBonusRecovery[SKILL_MAX_LEVEL];
    Float m_arrBonusStatusEffectRate[SKILL_MAX_LEVEL];
    Float m_arrBonusSpecific[SKILL_MAX_LEVEL];

    UInt m_arrCooldown[SKILL_MAX_LEVEL];
};

/////////////////////////////////////////////////////////////////////////////////
// The RuneLevelingStats class
class RuneLevelingStats
{
public:
    RuneLevelingStats();
    ~RuneLevelingStats();

    // Deferred loading
    Void Load( XMLNode * pNode );

    // Leveling stats access
    inline UInt GetPrimaryStatI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;
    inline Float GetPrimaryStatF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;

    inline UInt GetSecondaryStatI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;
    inline Float GetSecondaryStatF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;

    inline UInt GetSubStatI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;
    inline Float GetSubStatF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;

private:
    // Leveling stats
    UInt m_arrPrimaryStatsI[MONSTER_STAT_COUNT][RUNE_MAX_RANK][RUNE_MAX_LEVEL];
    Float m_arrPrimaryStatsF[MONSTER_STAT_COUNT][RUNE_MAX_RANK][RUNE_MAX_LEVEL];

    UInt m_arrSecondaryStatsI[MONSTER_STAT_COUNT][RUNE_MAX_RANK][RUNE_MAX_LEVEL];
    Float m_arrSecondaryStatsF[MONSTER_STAT_COUNT][RUNE_MAX_RANK][RUNE_MAX_LEVEL];

    UInt m_arrSubStatsI[MONSTER_STAT_COUNT][RUNE_MAX_RANK][RUNE_MAX_LEVEL];
    Float m_arrSubStatsF[MONSTER_STAT_COUNT][RUNE_MAX_RANK][RUNE_MAX_LEVEL];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "LevelingStats.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_LEVELINGSTATS_H

