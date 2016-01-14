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

    // XML serialization
    Void Load( const XMLNode * pNode );

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
    Bool m_bIsSummon;
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

    // XML serialization
    Void Load( const XMLNode * pNode );
    Void Save( XMLNode * outNode ) const;

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
    inline Void Awake();
    inline Void UnAwake();

    // Rank
    inline UInt GetNativeRank() const;
    inline UInt GetRank() const;
    inline Bool IsMaxRank() const;
    inline Void RankUp();
    inline Void RankDown();
    inline Void SetRank( UInt iRank );

    // Level
    inline UInt GetMaxLevel() const;
    inline UInt GetLevel() const;
    inline Bool IsMaxLevel() const;
    inline Void LevelUp();
    inline Void LevelDown();
    inline Void SetLevel( UInt iLevel );

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

    Void GetBaseStats( UInt outFlatStats[4], Float outRatioStats[4] ) const;

    // Skill set
    inline UInt GetSkillCount() const;
    inline const SkillInstance * GetSkillInstance( UInt iSlot ) const;
    inline SkillInstance * GetSkillInstance( UInt iSlot );
    inline Void SkillLevelUp( UInt iSlot );
    inline Void SkillLevelDown( UInt iSlot );
    inline Void SetSkillLevel( UInt iSlot, UInt iLevel );

    // Runes
    inline Bool HasRune( UInt iSlot ) const;
    inline const Rune * GetRune( UInt iSlot ) const;
    inline Rune * GetRune( UInt iSlot );

    inline Rune * EquipRune( const Rune & hRune );
    inline Void UnEquipRune( UInt iSlot, Rune * outRune );

    inline Bool HasSetBonus( RuneType iType, UInt * outCount = NULL ) const;

    // Effective stats
    inline UInt GetEffectiveHP() const;
    inline UInt GetEffectiveATT() const;
    inline UInt GetEffectiveDEF() const;
    inline UInt GetEffectiveSPD() const;

    inline Float GetEffectiveCritR() const;
    inline Float GetEffectiveCritD() const;
    inline Float GetEffectiveACC() const;
    inline Float GetEffectiveRES() const;

    Void GetEffectiveStats( UInt outFlatStats[4], Float outRatioStats[4] ) const;

private:
    // Monster
    const Monster * m_pMonster;

    Bool m_bAwakened;
    UInt m_iRank;
    UInt m_iLevel;

    UInt m_iCurrentXP;

    // Skill set
    SkillSet m_hSkillSet;

    // Rune set
    RuneSet m_hRuneSet;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Monster.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_MONSTER_H

