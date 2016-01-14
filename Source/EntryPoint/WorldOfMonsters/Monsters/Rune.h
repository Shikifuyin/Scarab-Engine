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
#include "LevelingStats.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Rune definitions
#define RUNE_SLOT_COUNT 6

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
enum RuneStatistic {
    RUNE_STAT_PRIMARY = 0,
    RUNE_STAT_SECONDARY,
    RUNE_STAT_SUB_1,
    RUNE_STAT_SUB_2,
    RUNE_STAT_SUB_3,
    RUNE_STAT_SUB_4,

    RUNE_STAT_COUNT
};
typedef struct _rune_stat_bonus {
    MonsterStatistic iStat;
    Bool bIsRatio;
} RuneStatBonus;

enum RuneQuality {
    RUNE_QUALITY_COMMON = 0, // White  (secondary only)
    RUNE_QUALITY_MAGIC,      // Green  (secondary + 1 sub)
    RUNE_QUALITY_RARE,       // Blue   (secondary + 2 subs)
    RUNE_QUALITY_EPIC,       // Purple (secondary + 3 subs)
    RUNE_QUALITY_LEGEND,     // Orange (secondary + 4 subs)

    RUNE_QUALITY_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The Rune class
class Rune
{
public:
    Rune();
    Rune( RuneType iType, UInt iSlot );
    Rune( const Rune & rhs );
    ~Rune();

    // operators
    Rune & operator=( const Rune & rhs );

    // Test for validity
    inline Bool IsNull() const;
    inline Bool IsPresent() const;

    // XML serialization
    Void Load( const XMLNode * pNode );
    Void Save( XMLNode * outNode ) const;

    // Type & slot
    inline RuneType GetType() const;
    inline UInt GetSlot() const;

    // Rank
    inline UInt GetRank() const;
    inline Bool IsMaxRank() const;
    inline Void RankUp();
    inline Void RankDown();
    inline Void SetRank( UInt iRank );

    // Level
    inline UInt GetLevel() const;
    inline Bool IsMaxLevel() const;
    inline Void LevelUp();
    inline Void LevelDown();
    inline Void SetLevel( UInt iLevel );

    // Stat bonuses
    inline Bool HasBonus( RuneStatistic iStat ) const;
    inline MonsterStatistic GetBonusType( RuneStatistic iStat ) const;
    inline Bool IsBonusRatio( RuneStatistic iStat ) const;

    UInt GetBonusValueI( RuneStatistic iStat ) const;
    Float GetBonusValueF( RuneStatistic iStat ) const;

    Void SetBonus( RuneStatistic iStat, MonsterStatistic iStatType, Bool bRatio );
    Void RemoveBonus( RuneStatistic iStat );

    // Methods
    RuneQuality ComputeQuality() const;
    UInt ComputeSellPrice() const;

protected:
    // Type & slot
    RuneType m_iType;
    UInt m_iSlot;

    // Leveling stats
    UInt m_iRank;
    UInt m_iLevel;

    // Stats
    RuneStatBonus m_arrBonuses[RUNE_STAT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// The RuneSet class
class RuneSet
{
public:
    RuneSet();
    ~RuneSet();

    // XML serialization
    Void Load( const XMLNode * pNode );
    Void Save( XMLNode * outNode ) const;

    // Runes access
    inline Bool HasRune( UInt iSlot ) const;
    inline const Rune * GetRune( UInt iSlot ) const;
    inline Rune * GetRune( UInt iSlot );

    Rune * AddRune( const Rune & hRune );
    Void RemoveRune( UInt iSlot, Rune * outRune );

    // Methods
    Bool HasSetBonus( RuneType iType, UInt * outCount = NULL ) const;
    Void ComputeBonusStat( MonsterStatistic iStat, UInt * outFlatBonus, Float * outRatioBonus ) const;
    Void ComputeBonusStats( UInt outFlatBonus[MONSTER_STAT_COUNT], Float outRatioBonus[MONSTER_STAT_COUNT] ) const;

protected:
    // Rune slots
    Rune m_arrRunes[RUNE_SLOT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Rune.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_RUNE_H

