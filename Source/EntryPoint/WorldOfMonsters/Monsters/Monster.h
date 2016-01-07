/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Monster.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Base monster interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_MONSTER_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_MONSTER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Statistics.h"
#include "StatusEffect.h"
#include "Skill.h"
#include "Rune.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Identifiers
#define MONSTER_NAME_LENGTH 64
typedef UInt MonsterID;

/////////////////////////////////////////////////////////////////////////////////
// The Monster class
class Monster
{
public:
    Monster( XMLNode * pMonsterNode );
    virtual ~Monster();    

    // Identifier
    inline MonsterID GetID() const;
    inline const GChar * GetName() const;
    inline const GChar * GetAwakenedName() const;

    // Leveling stats
    inline const MonsterLevelingStats * GetLevelingStats() const;

    // Skills
    inline UInt GetSkillCount() const;
    inline SkillID GetSkill( UInt iSlot ) const;

protected:
    // Identifier
    MonsterID m_iMonsterID;
    GChar m_strName[MONSTER_NAME_LENGTH];
    GChar m_strAwakenedName[MONSTER_NAME_LENGTH];

    // Leveling stats
    MonsterLevelingStats m_hLevelingStats;

    // Skills
    UInt m_iSkillCount;
    SkillID m_arrSkills[SKILL_SLOT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// The MonsterInstance class
class MonsterInstance
{
public:
    MonsterInstance( const Monster * pMonster );
    ~MonsterInstance();

    // Identifier access
    inline MonsterID GetID() const;
    inline const GChar * GetName() const;
    inline const GChar * GetAwakenedName() const;

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

    // Skills access
    inline UInt GetSkillCount() const;
    inline SkillInstance * GetSkillInstance( UInt iSlot );

    UInt SkillLevelUp( UInt iSlot );
    UInt SkillLevelDown( UInt iSlot );
    Void SetSkillLevel( UInt iSlot, UInt iLevel );

    // Runes access
    inline Bool HasRune( UInt iSlot ) const;

    inline Rune * GetRune( UInt iSlot ) const;
    inline Void SetRune( UInt iSlot, Rune * pRune );

    inline Bool HasSetBonus( RuneType iType, UInt * outCount = NULL ) const;

    inline UInt GetSetBonusCount() const;
    inline RuneType GetSetBonus( UInt iIndex ) const;

    // Effective stats access
    inline UInt GetEffectiveHP() const;
    inline UInt GetEffectiveATT() const;
    inline UInt GetEffectiveDEF() const;
    inline UInt GetEffectiveSPD() const;

    inline Float GetEffectiveCritR() const;
    inline Float GetEffectiveCritD() const;
    inline Float GetEffectiveACC() const;
    inline Float GetEffectiveRES() const;

private:
    // Helpers
    Void _UpdateEffectiveStats();

    // Monster model
    const Monster * m_pMonster;

    // Statistics
    MonsterStats m_hStats;

    // Skills
    SkillSet m_hSkillSet;

    // Runes
    RuneSet m_hRuneSet;

    // Effective statistics
    UInt m_iHealth;
    UInt m_iAttack;
    UInt m_iDefense;
    UInt m_iSpeed;

    Float m_fCriticalRate;   // in [0;1]
    Float m_fCriticalDamage; // in [1;+inf]

    Float m_fAccuracy;
    Float m_fResistance;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Monster.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_MONSTER_H

