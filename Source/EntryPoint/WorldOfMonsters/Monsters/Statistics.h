/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Statistics.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Statistics interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO : Find a function awakenstat(normalstat, monstertype) ?
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_STATISTICS_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_STATISTICS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Engine/Resource/XML/XML.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Ranks, levels, ...
#define MONSTER_MAX_RANK               6
#define MONSTER_MAX_LEVEL              40
#define MONSTER_MAX_LEVELBYRANK(_rank) ( 15 + (5*(_rank)) ) // 15, 20, 25, 30, 35, 40

// Statistics
#define MONSTER_BASE_CRITICAL_RATE   0.15f // 15%
#define MONSTER_BASE_CRITICAL_DAMAGE 0.50f  // +50%

#define MONSTER_BASE_ACCURACY   0.0f  // 0%
#define MONSTER_BASE_RESISTANCE 0.15f // 15%

enum MonsterStatistic {
    MONSTER_STAT_HEALTH = 0,
    MONSTER_STAT_ATTACK,
    MONSTER_STAT_DEFENSE,
    MONSTER_STAT_SPEED,

    MONSTER_STAT_CRIT_RATE,
    MONSTER_STAT_CRIT_DMG,
    MONSTER_STAT_ACCURACY,
    MONSTER_STAT_RESISTANCE,

    MONSTER_STAT_COUNT
};

// Types
enum MonsterType {
    MONSTER_ATTACK = 0,
    MONSTER_SUPPORT,
    MONSTER_TANK_HP,
    MONSTER_TANK_DEF,

    MONSTER_TYPE_COUNT
};

// Elements
enum MonsterElement {
    MONSTER_ELEMENT_MAGIC = 0,
    MONSTER_ELEMENT_FIRE,
    MONSTER_ELEMENT_WATER,
    MONSTER_ELEMENT_WIND,
    MONSTER_ELEMENT_LIGHT,
    MONSTER_ELEMENT_DARK,

    MONSTER_ELEMENT_COUNT
};

// Awakening
#define MONSTER_AWAKENING_BONUS_SPEED 1

enum MonsterAwakeningBonus {
    MONSTER_AWAKENING_SPEED = 0,    // +15
    MONSTER_AWAKENING_CRITICALRATE, // +15%
    MONSTER_AWAKENING_ACCURACY,     // +25%
    MONSTER_AWAKENING_RESISTANCE,   // +25%
    MONSTER_AWAKENING_SKILLNEW,     // monster-specific
    MONSTER_AWAKENING_SKILLUPGRADE, // monster-specific

    MONSTER_AWAKENING_BONUS_COUNT
};

enum MonsterAwakeningCost {
    MONSTER_AWAKENING_ELEMENTAL_CMN = 0,
    MONSTER_AWAKENING_ELEMENTAL_RAR,
    MONSTER_AWAKENING_MAGIC_CMN,
    MONSTER_AWAKENING_MAGIC_RAR,

    MONSTER_AWAKENING_COST_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The MonsterLevelingStats class
class MonsterLevelingStats
{
public:
    MonsterLevelingStats();
    ~MonsterLevelingStats();

    // Deferred loading
    Void Load( XMLNode * pNode );

    // Native stats access
    inline MonsterType GetType() const;
    inline MonsterElement GetElement() const;

    inline UInt GetNativeRank() const;

    inline MonsterAwakeningBonus GetAwakeningBonus() const;
    inline Bool IsAwakeningCostHighTier( MonsterAwakeningCost iCost ) const;
    inline UInt GetAwakeningCostAmount( MonsterAwakeningCost iCost ) const;

    // Leveling stats access
    inline UInt GetHP( UInt iRank, UInt iLevel, Bool bAwakened ) const;
    inline UInt GetATT( UInt iRank, UInt iLevel, Bool bAwakened ) const;
    inline UInt GetDEF( UInt iRank, UInt iLevel, Bool bAwakened ) const;
    inline UInt GetSPD( Bool bAwakened ) const;

    inline Float GetCritRate( Bool bAwakened ) const;
    inline Float GetCritDmg( Bool bAwakened ) const;
    inline Float GetAccuracy( Bool bAwakened ) const;
    inline Float GetResistance( Bool bAwakened ) const;

private:
    // Native stats
    MonsterType m_iType;
    MonsterElement m_iElement;

    UInt m_iNativeRank;

    MonsterAwakeningBonus m_iAwakeningBonus;
    UInt m_arrAwakeningCosts[MONSTER_AWAKENING_COST_COUNT]; // MSB => 0 = lows/mids, 1 = mids/highs

    // Leveling stats
    UInt m_arrHealth[2][MONSTER_MAX_RANK][MONSTER_MAX_LEVEL]; // (normal/awaken,rank,level)
    UInt m_arrAttack[2][MONSTER_MAX_RANK][MONSTER_MAX_LEVEL]; // (normal/awaken,rank,level)
    UInt m_arrDefense[2][MONSTER_MAX_RANK][MONSTER_MAX_LEVEL]; // (normal/awaken,rank,level)

    UInt m_iBaseSpeed; // +1 when awakened

    // no tables for crit/effect stats, fixed
};

/////////////////////////////////////////////////////////////////////////////////
// The MonsterStats class
class MonsterStats
{
public:
    MonsterStats( const MonsterLevelingStats * pLevelingStats );
    ~MonsterStats();

    // Type & element access
    inline MonsterType GetType() const;
    inline MonsterElement GetElement() const;

    // Awakening access
    inline Bool IsAwakened() const;

    inline MonsterAwakeningBonus GetAwakeningBonus() const;
    inline Bool IsAwakeningCostHighTier( MonsterAwakeningCost iCost ) const;
    inline UInt GetAwakeningCostAmount( MonsterAwakeningCost iCost ) const;

    Void Awake();
    Void UnAwake();

    // Rank access
    inline UInt GetNativeRank() const;
    inline UInt GetRank() const;

    UInt RankUp();
    UInt RankDown();
    Void SetRank( UInt iRank );

    // Level access
    inline UInt GetMaxLevel() const;
    inline UInt GetLevel() const;

    UInt LevelUp();
    UInt LevelDown();
    Void SetLevel( UInt iLevel );

    // Stats access
    inline UInt GetHP() const;
    inline UInt GetATT() const;
    inline UInt GetDEF() const;
    inline UInt GetSPD() const;

    inline Float GetCritR() const;
    inline Float GetCritD() const;
    inline Float GetACC() const;
    inline Float GetRES() const;

private:
    // Helpers
    Void _UpdateStats();

    // Leveling stats
    const MonsterLevelingStats * m_pLevelingStats;

    Bool m_bAwakened;
    UInt m_iRank;
    UInt m_iLevel;

    // Stats
    UInt m_iHealth;
    UInt m_iAttack;
    UInt m_iDefense;
    UInt m_iSpeed;

    Float m_fCriticalRate;   // in [0;1]
    Float m_fCriticalDamage; // in [1;+inf] (no cap)

    Float m_fAccuracy;       // in [0;1]
    Float m_fResistance;     // in [0;1]
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Statistics.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_STATISTICS_H

