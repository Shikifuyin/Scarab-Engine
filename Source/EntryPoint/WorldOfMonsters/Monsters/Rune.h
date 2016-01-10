/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Rune.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Base Skill interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_RUNE_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_RUNE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Statistics.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define RUNE_SLOT_COUNT 6

#define RUNE_MAX_RANK               6
#define RUNE_MAX_LEVEL              16
#define RUNE_MAX_LEVELBYRANK(_rank) ( ((_rank)==0) ? 3 : (3*(_rank)) ) // 3, 3, 6, 9, 12, 15

enum RuneType {
    RUNE_ENERGY = 0, // (2)HP +15%
    RUNE_FATAL,      // (4)ATT +35%
    RUNE_GUARD,      // (2)DEF +15%
    RUNE_SWIFT,      // (4)SPD +25%

    RUNE_BLADE,      // (2)CRate +12%
    RUNE_RAGE,       // (4)CDmg +40%
    RUNE_FOCUS,      // (2)ACC +20%
    RUNE_ENDURE,     // (2)RES +20%

    RUNE_SHIELD,     // (2)Shield 3 turns 15% HP
    RUNE_WILL,       // (2)Immunity 1 turn

    RUNE_NEMESIS,    // (2)ATB +4% / HP loss 7%
    RUNE_DESTROY,    // (2)HPMax -4% / Dmg dealt 30%

    RUNE_VAMPIRE,    // (4)LifeDrain +35%
    RUNE_DESPAIR,    // (4)Stun +25%
    RUNE_VIOLENT,    // (4)ExtraTurn +22%
    RUNE_REVENGE,    // (2)Counterattack +15%
    
    RUNE_TYPE_COUNT
};

enum RuneStat {
    RUNE_STAT_PRIMARY = 0,
    RUNE_STAT_SECONDARY,
    RUNE_STAT_SUB_1,
    RUNE_STAT_SUB_2,
    RUNE_STAT_SUB_3,
    RUNE_STAT_SUB_4,

    RUNE_STAT_COUNT
};

enum RuneQuality {
    RUNE_QUALITY_COMMON = 0, // White  (nothing)
    RUNE_QUALITY_UNCOMMON,   // White  (secondary only)
    RUNE_QUALITY_MAGIC,      // Green  (secondary + 1 sub)
    RUNE_QUALITY_RARE,       // Blue   (secondary + 2 subs)
    RUNE_QUALITY_EPIC,       // Purple (secondary + 3 subs)
    RUNE_QUALITY_LEGEND,     // Orange (secondary + 4 subs)

    RUNE_QUALITY_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The RuneLevelingStats class
class RuneLevelingStats
{
public:
    RuneLevelingStats( XMLNode * pNode );
    ~RuneLevelingStats();

    // Set counts access
    static UInt GetSetCount( RuneType iType );

    // Allowed slots access
    inline static Bool IsSlotAllowed( UInt iSlot, MonsterStatistic iStat, Bool bRatio );

    // Leveling stats access
    inline UInt GetStatPrimaryI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;
    inline Float GetStatPrimaryF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;

    inline UInt GetStatSecondaryI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;
    inline Float GetStatSecondaryF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;

    inline UInt GetStatSubI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;
    inline Float GetStatSubF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const;

private:
    // Primary stats allocation constraints
    static const Byte sm_arrAllowedSlots[2][MONSTER_STAT_COUNT]; // (flat/ratio,type)

    // Leveling stats
    UInt m_arrHealthI[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL];  // (prim/sec/sub,rank,level)
    Float m_arrHealthF[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL]; // (prim/sec/sub,rank,level)

    UInt m_arrAttackI[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL];  // (prim/sec/sub,rank,level)
    Float m_arrAttackF[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL]; // (prim/sec/sub,rank,level)

    UInt m_arrDefenseI[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL];  // (prim/sec/sub,rank,level)
    Float m_arrDefenseF[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL]; // (prim/sec/sub,rank,level)

    UInt m_arrSpeed[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL]; // (prim/sec/sub,rank,level)

    Float m_arrCritRate[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL]; // (prim/sec/sub,rank,level)
    Float m_arrCritDmg[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL];  // (prim/sec/sub,rank,level)

    Float m_arrAccuracy[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL];   // (prim/sec/sub,rank,level)
    Float m_arrResistance[3][RUNE_MAX_RANK][RUNE_MAX_LEVEL]; // (prim/sec/sub,rank,level)
};

/////////////////////////////////////////////////////////////////////////////////
// The Rune class
class Rune
{
public:
    Rune( const RuneLevelingStats * pLevelingStats, RuneType iType, UInt iSlot );
    ~Rune();

    // Type & slot access
    inline RuneType GetType() const;
    inline UInt GetSlot() const;

    inline Bool IsEquipped() const;
    inline Void SetEquipped( Bool bEquipped );

    // Rank access
    inline UInt GetRank() const;

    UInt RankUp();
    UInt RankDown();
    Void SetRank( UInt iRank );

    // Level access
    inline UInt GetMaxLevel() const;
    inline UInt GetLevel() const;
    inline RuneQuality GetQuality() const;

    UInt LevelUp();
    UInt LevelDown();
    Void SetLevel( UInt iLevel );

    // Stats access
    inline UInt GetBonusCount() const;

    inline MonsterStatistic GetBonusType( RuneStat iStat ) const;
    inline Bool IsBonusRatio( RuneStat iStat ) const;
    inline UInt GetBonusValueI( RuneStat iStat ) const;
    inline Float GetBonusValueF( RuneStat iStat ) const;

    Void AddBonus( RuneStat iStat, MonsterStatistic iStatType, Bool bRatio );
    Void RemoveBonus( RuneStat iStat );

    // Selling price access
    UInt ComputeSellPrice() const;

protected:
    // Helpers
    Void _UpdateStats();

    // Type & slot
    RuneType m_iType;
    UInt m_iSlot;

    Bool m_bEquipped;

    // Leveling stats
    const RuneLevelingStats * m_pLevelingStats;

    UInt m_iRank;
    UInt m_iLevel;
    RuneQuality m_iQuality;

    // Stats
    UInt m_iBonusCount;
    MonsterStatistic m_arrBonusTypes[RUNE_STAT_COUNT];
    Bool m_arrIsBonusRatio[RUNE_STAT_COUNT];
    union {
        UInt iValue;
        Float fValue;
    } m_arrBonusValues[RUNE_STAT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// The RuneSet class
class RuneSet
{
public:
    RuneSet();
    ~RuneSet();

    // Runes access
    inline Bool HasRune( UInt iSlot ) const;

    inline Rune * GetRune( UInt iSlot ) const;
    inline Void SetRune( UInt iSlot, Rune * pRune );

    // SetBonus access
    Bool HasSetBonus( RuneType iType, UInt * outCount = NULL ) const;

    inline UInt GetSetBonusCount() const;
    inline RuneType GetSetBonus( UInt iIndex ) const;

    // Stats compilation
    Void CompileStats( UInt arrFlatBonuses[4], Float arrRatioBonuses[3], Float arrSecondaryBonuses[4] ) const;

protected:
    // Helpers
    Void _UpdateSetBonuses();

    // Rune slots
    Rune * m_arrRunes[RUNE_SLOT_COUNT];

    // Set bonuses
    UInt m_iSetBonusCount;
    RuneType m_arrSetBonuses[4];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Rune.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_RUNE_H

