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
#include "LevelingStats.h"
#include "StatusEffect.h"
#include "SkillEffect.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Skill definitions
#define SKILL_NAME_LENGTH 64
typedef UInt SkillID;

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

enum SkillLeaderConstraint {
    SKILL_LEADERCONSTRAINT_NONE = 0,
    SKILL_LEADERCONSTRAINT_FIRE,
    SKILL_LEADERCONSTRAINT_WATER,
    SKILL_LEADERCONSTRAINT_WIND,
    SKILL_LEADERCONSTRAINT_LIGHT,
    SKILL_LEADERCONSTRAINT_DARK,
    SKILL_LEADERCONSTRAINT_DUNGEON,
    SKILL_LEADERCONSTRAINT_ARENA,
    SKILL_LEADERCONSTRAINT_GUILDBATTLE,

    SKILL_LEADERCONSTRAINT_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The Skill class
class Skill
{
public:
    Skill();
    virtual ~Skill();

    // XML serialization
    static Skill * StaticLoad( const XMLNode * pNode );
    virtual Void Load( const XMLNode * pNode );

    // Identifier
    inline SkillID GetID() const;
    inline const GChar * GetName() const;

    // Type
    virtual SkillType GetType() const = 0;

    inline Bool IsActive() const;
    inline Bool IsPassive() const;
    inline Bool IsLeader() const;

    // Leveling stats
    inline const SkillLevelingStats * GetLevelingStats() const;

protected:
    // Identifier
    SkillID m_iSkillID;
    GChar m_strName[SKILL_NAME_LENGTH];

    // Leveling stats
    SkillLevelingStats m_hLevelingStats;
};

/////////////////////////////////////////////////////////////////////////////////
// The ActiveSkill class
class ActiveSkill : public Skill
{
public:
    ActiveSkill();
    virtual ~ActiveSkill();

    // XML serialization
    virtual Void Load( const XMLNode * pNode );

    // Type
    inline virtual SkillType GetType() const;

    inline Bool IsAttack() const;
    inline Bool IsDefense() const;

    // Effects access
    inline UInt GetEffectCount( SkillActiveType iType ) const;
    inline const SkillEffect * GetEffect( SkillActiveType iType, UInt iIndex ) const;

protected:
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
    PassiveSkill();
    virtual ~PassiveSkill();

    // XML serialization
    virtual Void Load( const XMLNode * pNode );

    // Type
    inline virtual SkillType GetType() const;

    // Effects access
    inline UInt GetEffectCount( SkillPassiveType iType ) const;
    inline SkillEffect * GetEffect( SkillPassiveType iType, UInt iIndex ) const;

protected:
    // Effects
    UInt m_arrEffectCounts[SKILL_PASSIVE_COUNT];
    SkillEffect * m_arrEffects[SKILL_PASSIVE_COUNT][SKILL_MAX_EFFECTS];
};

/////////////////////////////////////////////////////////////////////////////////
// The LeaderSkill class
class LeaderSkill : public Skill
{
public:
    LeaderSkill();
    virtual ~LeaderSkill();

    // XML serialization
    virtual Void Load( const XMLNode * pNode );

    // Type
    inline virtual SkillType GetType() const;

    // Effect access
    inline MonsterStatistic GetLeaderBonusStat() const;
    inline Float GetLeaderBonusAmount() const;

    inline SkillLeaderConstraint GetLeaderConstraint() const;

protected:
    // Effect
    MonsterStatistic m_iBonusStat;
    Float m_fBonusAmount;

    SkillLeaderConstraint m_iConstraint;
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

    // XML serialization
    Void Load( const XMLNode * pNode );
    Void Save( XMLNode * outNode ) const;

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

    // Level access
    inline UInt GetMaxLevel() const;
    inline UInt GetLevel() const;
    inline Bool IsMaxLevel() const;
    inline Void LevelUp();
    inline Void LevelDown();
    inline Void SetLevel( UInt iLevel );

    // Effective stats
    inline Float GetBonusDamage() const;
    inline Float GetBonusRecovery() const;
    inline Float GetBonusStatusEffectRate() const;
    inline Float GetBonusSpecific() const;

    inline Bool HasCooldown() const;
    inline UInt GetCooldown() const;

private:
    // Skill
    Skill * m_pSkill;

    UInt m_iLevel;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillSet class
class SkillSet
{
public:
    SkillSet();
    ~SkillSet();

    // XML serialization
    Void Load( const XMLNode * pNode );
    Void Save( XMLNode * outNode ) const;

    // Skills access
    inline UInt GetSkillCount() const;
    inline const SkillInstance * GetSkillInstance( UInt iSlot ) const;
    inline SkillInstance * GetSkillInstance( UInt iSlot );

    SkillInstance * AddSkill( SkillID iSkillID, UInt iLevel );
    Void RemoveAll();

private:
    // Skill set
    UInt m_iSkillCount;
    SkillInstance m_arrSkills[SKILL_SLOT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Skill.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_SKILL_H

