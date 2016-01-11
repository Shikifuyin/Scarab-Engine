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



/////////////////////////////////////////////////////////////////////////////////
// The MonsterLevelingStats class
{
public:
    //MonsterLevelingStats();
    //~MonsterLevelingStats();

    // Deferred loading
    Void Load( XMLNode * pNode );

    // Native stats access
    inline MonsterType GetType() const;
    inline MonsterElement GetElement() const;

    inline UInt GetNativeRank() const;

    inline Bool IsSummoningCostHighTier( MonsterSummoningCost iCost ) const;
    inline UInt GetSummoningCostAmount( MonsterSummoningCost iCost ) const;

    inline MonsterAwakeningBonus GetAwakeningBonus() const;
    inline Bool IsAwakeningCostHighTier( MonsterAwakeningCost iCost ) const;
    inline UInt GetAwakeningCostAmount( MonsterAwakeningCost iCost ) const;


private:
    // Native stats
    MonsterType m_iType;
    MonsterElement m_iElement;

    UInt m_iNativeRank;

    UInt m_arrSummoningCosts[MONSTER_SUMMONING_COST_COUNT];

    MonsterAwakeningBonus m_iAwakeningBonus;
    UInt m_arrAwakeningCosts[MONSTER_AWAKENING_COST_COUNT]; // MSB => 0 = lows/mids, 1 = mids/highs

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

    // Summoning access
    inline Bool IsSummoningCostHighTier( MonsterSummoningCost iCost ) const;
    inline UInt GetSummoningCostAmount( MonsterSummoningCost iCost ) const;

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

