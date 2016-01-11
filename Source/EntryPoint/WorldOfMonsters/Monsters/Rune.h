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

    // Prototypes
class MonsterInstance;

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

    // Type & slot access
    inline RuneType GetType() const;
    inline UInt GetSlot() const;

    // Rank access
    inline UInt GetRank() const;

    UInt RankUp();
    UInt RankDown();
    Void SetRank( UInt iRank );

    // Level access
    inline UInt GetLevel() const;

    UInt LevelUp();
    UInt LevelDown();
    Void SetLevel( UInt iLevel );

    // Stats access
    inline RuneQuality GetQuality() const;
    inline UInt GetBonusCount() const;

    inline MonsterStatistic GetBonusType( RuneStatistic iStat ) const;
    inline Bool IsBonusRatio( RuneStatistic iStat ) const;
    inline UInt GetBonusValueI( RuneStatistic iStat ) const;
    inline Float GetBonusValueF( RuneStatistic iStat ) const;

    Void AddBonus( RuneStatistic iStat, MonsterStatistic iStatType, Bool bRatio );
    Void RemoveBonus( RuneStatistic iStat );

    // Equipment support
    inline Bool IsEquipped() const;
    inline Void SetEquipped( MonsterInstance * pEquippedTo );

    // Selling price access
    UInt ComputeSellPrice() const;

protected:
    // Helpers
    Void _UpdateStats();

    // Type & slot
    RuneType m_iType;
    UInt m_iSlot;

    // Leveling stats
    UInt m_iRank;
    UInt m_iLevel;

    // Stats
    RuneQuality m_iQuality;

    UInt m_iBonusCount;
    MonsterStatistic m_arrBonusTypes[RUNE_STAT_COUNT];
    Bool m_arrIsBonusRatio[RUNE_STAT_COUNT];
    union {
        UInt iValue;
        Float fValue;
    } m_arrBonusValues[RUNE_STAT_COUNT];

    // Equipment support
    MonsterInstance * m_pEquippedTo;
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
    inline Rune * GetRune( UInt iSlot );

    Void AddRune( const Rune & hRune );
    Void RemoveRune( UInt iSlot );

    // SetBonus access
    Bool HasSetBonus( RuneType iType, UInt * outCount = NULL ) const;

    inline UInt GetSetBonusCount() const;
    inline RuneType GetSetBonus( UInt iIndex ) const;

    // Stats compilation
    Void CompileStats( UInt outFlatBonuses[4], Float outRatioBonuses[4], Float outSecondaryBonuses[4] ) const;

protected:
    // Helpers
    Void _UpdateSetBonuses();

    // Rune slots
    Rune m_arrRunes[RUNE_SLOT_COUNT];

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

