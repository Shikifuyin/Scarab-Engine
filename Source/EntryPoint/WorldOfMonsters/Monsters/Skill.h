/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Skill.h
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_SKILL_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_SKILL_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Statistics.h"
#include "StatusEffect.h"
#include "SkillEffect.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Identifiers
#define SKILL_NAME_LENGTH 64
typedef UInt SkillID;

// Skill leveling
#define SKILL_MAX_LEVEL 16

enum SkillStat {
    SKILL_STAT_DAMAGE = 0,
    SKILL_STAT_RECOVERY,
    SKILL_STAT_STATUSEFFECTRATE,
    SKILL_STAT_COOLDOWN,

    SKILL_STAT_SPECIFIC, // Shields, etc ...

    SKILL_STAT_COUNT
};

typedef struct _skilllevel_bonus {
    SkillStat iStat;
    Float fAmount;
} SkillLevelBonus;

// Skill types
enum SkillType {
    SKILL_TYPE_ACTIVE = 0,
    SKILL_TYPE_PASSIVE,
    SKILL_TYPE_LEADER,

    SKILL_TYPE_COUNT
};

enum SkillActiveType {
    SKILL_ACTIVE_ONHIT = 0,
    SKILL_ACTIVE_ONCRIT,
    SKILL_ACTIVE_ONSTATUSEFFECT,

    SKILL_ACTIVE_COUNT
};

enum SkillPassiveType {
    SKILL_PASSIVE_PERSISTENT = 0,
    SKILL_PASSIVE_PERIODIC_SELF,
    SKILL_PASSIVE_PERIODIC_ALLIES,
    SKILL_PASSIVE_PERIODIC_ENNEMIES,
    SKILL_PASSIVE_PERIODIC_ALL,
    SKILL_PASSIVE_ONHIT,
    SKILL_PASSIVE_ONHIT_ALLY,
    SKILL_PASSIVE_ONHIT_ENNEMY,
    SKILL_PASSIVE_ONCRIT,
    SKILL_PASSIVE_ONCRIT_ALLY,
    SKILL_PASSIVE_ONCRIT_ENNEMY,
    SKILL_PASSIVE_ONBEINGHIT,
    SKILL_PASSIVE_ONBEINGHIT_ALLY,
    SKILL_PASSIVE_ONBEINGHIT_ENNEMY,
    SKILL_PASSIVE_ONBEINGCRIT,
    SKILL_PASSIVE_ONBEINGCRIT_ALLY,
    SKILL_PASSIVE_ONBEINGCRIT_ENNEMY,

    SKILL_PASSIVE_COUNT
};

// Skill sets
#define SKILL_SLOT_COUNT 4

/////////////////////////////////////////////////////////////////////////////////
// The Skill class
class Skill
{
public:
    Skill( XMLNode * pSkillNode );
    virtual ~Skill();

    // Identifier
    inline SkillID GetID() const;
    inline const GChar * GetName() const;

    // Type
    inline Bool IsActive() const;
    inline Bool IsPassive() const;
    inline Bool IsLeader() const;

    inline SkillType GetType() const;

    // Cooldown acces
    inline Bool HasCooldown() const;
    inline UInt GetCooldown() const;

    // Leveling stats
    inline UInt GetMaxLevel() const;
    inline const SkillLevelBonus * GetLevelBonus( UInt iLevel ) const;

    // Awakening
    inline Bool RequiresAwakening() const;
    inline Bool HasAwakeningUpgrade() const;
    inline SkillID GetAwakeningUpgrade() const;

protected:
    // Helpers
    static SkillType _SkillType_FromString( const GChar * strValue );
    static SkillStat _SkillStat_FromString( const GChar * strValue );
    static SkillEffectType _SkillEffectType_FromString( const GChar * strValue );

    // Identifier
    SkillID m_iSkillID;
    GChar m_strName[SKILL_NAME_LENGTH];

    // Type
    SkillType m_iType;

    // Cooldown
    UInt m_iCooldown;

    // Leveling stats
    UInt m_iMaxLevel; // 0 = no level (leader, most passives)
    SkillLevelBonus m_arrLevelBonus[SKILL_MAX_LEVEL];

    // Awakening
    Bool m_bRequiresAwakening;
    Bool m_bHasAwakeningUpgrade;
    SkillID m_iAwakeningUpgradeID; // INVALID_OFFSET = none
};

/////////////////////////////////////////////////////////////////////////////////
// The ActiveSkill class
class ActiveSkill : public Skill
{
public:
    ActiveSkill( XMLNode * pSkillNode );
    virtual ~ActiveSkill();

    // Type
    inline Bool IsAttack() const;
    inline Bool IsDefense() const;

    inline SkillActiveType GetActiveType() const;

    // Effects access
    inline UInt GetEffectCount( SkillActiveType iType );
    inline SkillEffect * GetEffect( SkillActiveType iType, UInt iIndex ) const;

protected:
    // Helpers
    static SkillActiveType _SkillActiveType_FromString( const GChar * strValue );

    // Type
    Bool m_bIsAttack; // else defense

    // Effects
    UInt m_arrEffectCounts[SKILL_ACTIVE_COUNT];
    SkillEffect * m_arrEffects[SKILL_ACTIVE_COUNT][SKILL_MAX_EFFECTS];
};

/////////////////////////////////////////////////////////////////////////////////
// The PassiveSkill class
class PassiveSkill : public Skill
{
public:
    PassiveSkill( XMLNode * pSkillNode );
    virtual ~PassiveSkill();

    // Type
    inline SkillPassiveType GetPassiveType() const;

    // Effects access
    inline UInt GetEffectCount( SkillPassiveType iType );
    inline SkillEffect * GetEffect( SkillPassiveType iType, UInt iIndex ) const;

protected:
    // Helpers
    static SkillPassiveType _SkillPassiveType_FromString( const GChar * strValue );

    // Effects
    UInt m_arrEffectCounts[SKILL_PASSIVE_COUNT];
    SkillEffect * m_arrEffects[SKILL_PASSIVE_COUNT][SKILL_MAX_EFFECTS];
};

/////////////////////////////////////////////////////////////////////////////////
// The LeaderSkill class
class LeaderSkill : public Skill
{
public:
    enum LeaderConstraint {
        LEADER_CONSTRAINT_NONE = 0,
        LEADER_CONSTRAINT_FIRE,
        LEADER_CONSTRAINT_WATER,
        LEADER_CONSTRAINT_WIND,
        LEADER_CONSTRAINT_LIGHT,
        LEADER_CONSTRAINT_DARK,
        LEADER_CONSTRAINT_DUNGEON,
        LEADER_CONSTRAINT_ARENA,
        LEADER_CONSTRAINT_GUILDBATTLE
    };
public:
    LeaderSkill( XMLNode * pSkillNode );
    virtual ~LeaderSkill();

    // Effect access
    inline MonsterStatistic GetLeaderBonusStat() const;
    inline Float GetLeaderBonusAmount() const;

    inline LeaderConstraint GetLeaderConstraint() const;

protected:
    // Helpers
    static MonsterStatistic _MonsterStatistic_FromString( const GChar * strValue );
    static LeaderConstraint _LeaderConstraint_FromString( const GChar * strValue );

    // Effect
    MonsterStatistic m_iBonusStat;
    Float m_fBonusAmount;

    LeaderConstraint m_iConstraint;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillInstance class
class SkillInstance
{
public:
    SkillInstance();
    SkillInstance( Skill * pSkill );
    SkillInstance( const SkillInstance & rhs );
    ~SkillInstance();

    // operators
    SkillInstance & operator=( const SkillInstance & rhs );

    // Test for validity
    inline Bool IsNull() const;
    inline Bool IsPresent() const;

    // Base instance access
    inline Skill * GetSkill() const;

    // Identifier access
    inline SkillID GetID() const;
    inline const GChar * GetName() const;

    // Type access
    inline Bool IsActive() const;
    inline Bool IsPassive() const;
    inline Bool IsLeader() const;

    inline SkillType GetType() const;

    // Cooldown acces
    inline Bool HasCooldown() const;
    inline UInt GetCooldown() const;

    // Awakening access
    inline Bool RequiresAwakening() const;
    inline Bool HasAwakeningUpgrade() const;
    inline SkillID GetAwakeningUpgrade() const;

    // Level access
    inline UInt GetMaxLevel() const;
    inline UInt GetLevel() const;

    UInt LevelUp();
    UInt LevelDown();
    Void SetLevel( UInt iLevel );

    // Effective stats access
    inline Bool HasEffectiveBonus( SkillStat iStat ) const;
    inline Float GetEffectiveBonus( SkillStat iStat ) const;

private:
    // Helpers
    Void _UpdateEffectiveStats();

    // Skill model
    Skill * m_pSkill;

    UInt m_iLevel;

    // Effective stats
    Float m_fEffectiveBonus[SKILL_STAT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillSet class
class SkillSet
{
public:
    SkillSet();
    ~SkillSet();

    inline UInt GetSkillCount() const;
    inline SkillInstance * GetSkillInstance( UInt iSlot );

    Void Add( Skill * pSkill, UInt iLevel );
    Void RemoveAll();

private:
    UInt m_iSkillCount;
    SkillInstance m_arrSkills[SKILL_SLOT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Skill.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_SKILL_H

