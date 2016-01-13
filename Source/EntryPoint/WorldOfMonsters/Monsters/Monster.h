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
#include "LevelingStats.h"
#include "StatusEffect.h"
#include "Skill.h"
#include "Rune.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Monster definitions
#define MONSTER_NAME_LENGTH 64
typedef UInt MonsterID;

enum MonsterElement {
    MONSTER_ELEMENT_MAGIC = 0,
    MONSTER_ELEMENT_FIRE,
    MONSTER_ELEMENT_WATER,
    MONSTER_ELEMENT_WIND,
    MONSTER_ELEMENT_LIGHT,
    MONSTER_ELEMENT_DARK,

    MONSTER_ELEMENT_COUNT
};

enum MonsterType {
    MONSTER_ATTACK = 0,
    MONSTER_SUPPORT,
    MONSTER_TANK_HP,
    MONSTER_TANK_DEF,

    MONSTER_TYPE_COUNT
};

typedef struct _monster_cost {
    MonsterID arrCost[4];
} MonsterCost;

    // Currencies : scrolls
enum ScrollType {
    SCROLL_COMMON = 0,
    SCROLL_MYSTICAL,
    SCROLL_LEGENDARY,
    SCROLL_FIRE,
    SCROLL_WATER,
    SCROLL_WIND,
    SCROLL_LIGHT,
    SCROLL_DARK,

    SCROLL_TYPE_COUNT
};

typedef struct _scroll_cost {
    UInt arrCost[SCROLL_TYPE_COUNT];
} ScrollCost;

    // Currencies : Essences
enum EssenceType {
    ESSENCE_LOW = 0,
    ESSENCE_MID,
    ESSENCE_HIGH,

    ESSENCE_TYPE_COUNT
};

typedef struct _essence_cost {
    MonsterElement iElement;
    UInt arrMagicCost[ESSENCE_TYPE_COUNT];
    UInt arrElementalCost[ESSENCE_TYPE_COUNT];
} EssenceCost;

/////////////////////////////////////////////////////////////////////////////////
// The Monster class
class Monster
{
public:
    Monster();
    ~Monster();    

    // Deferred loading
    Void Load( XMLNode * pNode );

    // Identifier
    inline MonsterID GetID() const;
    inline const GChar * GetName() const;
    inline const GChar * GetAwakenedName() const;

    // Type & Element
    inline MonsterType GetType() const;
    inline MonsterElement GetElement() const;

    // Native rank
    inline UInt GetNativeRank() const;

    // Summoning cost
    inline Bool IsSummon() const;
    inline const ScrollCost * GetSummoningCost() const;

    inline Bool IsFusion() const;
    inline const MonsterCost * GetFusionCost() const;

    // Awakening cost
    inline const EssenceCost * GetAwakeningCost() const;

    // Leveling stats
    inline const MonsterLevelingStats * GetLevelingStats() const;

    // Skill set
    inline UInt GetSkillCount( Bool bAwaken ) const;
    inline SkillID GetSkill( Bool bAwaken, UInt iSlot ) const;

protected:
    // Helpers
    static MonsterType _MonsterType_FromString( const GChar * strValue );
    static MonsterElement _MonsterElement_FromString( const GChar * strValue );

    // Identifier
    MonsterID m_iMonsterID;
    GChar m_strName[MONSTER_NAME_LENGTH];
    GChar m_strAwakenedName[MONSTER_NAME_LENGTH];

    // Type & Element
    MonsterType m_iType;
    MonsterElement m_iElement;

    // Native rank
    UInt m_iNativeRank;

    // Summoning cost
    Bool m_bIsFusion;
    ScrollCost m_hSummoningCost;
    MonsterCost m_hFusionCost;

    // Awakening cost
    EssenceCost m_hAwakeningCost;

    // Leveling stats
    MonsterLevelingStats m_hLevelingStats;

    // Skill set
    UInt m_iSkillCount;
    SkillID m_arrSkillSet[SKILL_SLOT_COUNT];
    UInt m_iSkillCountAwaken;
    SkillID m_arrSkillSetAwaken[SKILL_SLOT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// The MonsterInstance class
class MonsterInstance
{
public:
    MonsterInstance();
    MonsterInstance( const Monster * pMonster );
    MonsterInstance( const MonsterInstance & rhs );
    ~MonsterInstance();

    // operators
    MonsterInstance & operator=( const MonsterInstance & rhs );

    // Test for validity
    inline Bool IsNull() const;
    inline Bool IsPresent() const;

    // Identifier
    inline MonsterID GetID() const;
    inline const GChar * GetName() const;
    inline const GChar * GetAwakenedName() const;

    // Type & Element
    inline MonsterType GetType() const;
    inline MonsterElement GetElement() const;

    // Summoning
    inline Bool IsSummon() const;
    inline const ScrollCost * GetSummoningCost() const;

    inline Bool IsFusion() const;
    inline const MonsterCost * GetFusionCost() const;

    // Awakening
    inline const EssenceCost * GetAwakeningCost() const;
    inline MonsterAwakeningBonus GetAwakeningBonus() const;
    inline Bool IsAwakened() const;

    Void Awake();
    Void UnAwake();

    // Rank
    inline UInt GetNativeRank() const;
    inline UInt GetRank() const;
    inline Bool IsMaxRank() const;

    UInt RankUp();
    UInt RankDown();
    Void SetRank( UInt iRank );

    // Level
    inline UInt GetMaxLevel() const;
    inline UInt GetLevel() const;
    inline Bool IsMaxLevel() const;

    UInt LevelUp();
    UInt LevelDown();
    Void SetLevel( UInt iLevel );

    inline UInt GetCurrentXP() const;
    UInt AddXP( UInt iAmount ); // returns number of levelups

    // Base stats
    inline UInt GetBaseHP() const;
    inline UInt GetBaseATT() const;
    inline UInt GetBaseDEF() const;
    inline UInt GetBaseSPD() const;

    inline Float GetBaseCritR() const;
    inline Float GetBaseCritD() const;
    inline Float GetBaseACC() const;
    inline Float GetBaseRES() const;

    // Skill set
    inline UInt GetSkillCount() const;
    inline SkillInstance * GetSkillInstance( UInt iSlot );

    UInt SkillLevelUp( UInt iSlot );
    UInt SkillLevelDown( UInt iSlot );
    Void SetSkillLevel( UInt iSlot, UInt iLevel );

    // Runes
    inline Bool HasRune( UInt iSlot ) const;
    inline Rune * GetRune( UInt iSlot ) const;

    Void EquipRune( Rune * pRune );
    Void UnEquipRune( UInt iSlot );

    inline Bool HasSetBonus( RuneType iType, UInt * outCount = NULL ) const;

    inline UInt GetSetBonusCount() const;
    inline RuneType GetSetBonus( UInt iIndex ) const;

    // Effective stats
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
    Void _UpdateBaseStats();
    Void _UpdateEffectiveStats();

    // Monster
    const Monster * m_pMonster;

    Bool m_bAwakened;
    UInt m_iRank;
    UInt m_iLevel;

    UInt m_iCurrentXP;

    // Base statistics
    UInt m_iBaseHealth;
    UInt m_iBaseAttack;
    UInt m_iBaseDefense;
    UInt m_iBaseSpeed;

    Float m_fBaseCriticalRate;   // in [0;1]
    Float m_fBaseCriticalDamage; // in [1;+inf] (no cap)

    Float m_fBaseAccuracy;       // in [0;1]
    Float m_fBaseResistance;     // in [0;1]

    // Skill set
    SkillSet m_hSkillSet;

    // Rune set
    RuneSet m_hRuneSet;

    // Effective statistics
    UInt m_iEffectiveHealth;
    UInt m_iEffectiveAttack;
    UInt m_iEffectiveDefense;
    UInt m_iEffectiveSpeed;

    Float m_fEffectiveCriticalRate;   // in [0;1]
    Float m_fEffectiveCriticalDamage; // in [1;+inf]

    Float m_fEffectiveAccuracy;
    Float m_fEffectiveResistance;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Monster.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_MONSTER_H

