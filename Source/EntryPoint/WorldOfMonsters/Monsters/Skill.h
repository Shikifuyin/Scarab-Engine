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
#include "../../Framework/Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Identifiers
#define SKILL_NAME_LENGTH 64
typedef UInt SkillID;

// Target patterns
enum SkillTargetPattern {
    SKILL_TARGET_SELF = 0,

    SKILL_TARGET_ENNEMY_SINGLE,
    SKILL_TARGET_ENNEMY_SINGLE_RANDOM,
    SKILL_TARGET_ENNEMY_DUAL,
    SKILL_TARGET_ENNEMY_DUAL_RANDOM,
    SKILL_TARGET_ENNEMY_WIDE,
    SKILL_TARGET_ENNEMY_WIDE_RANDOM,
    SKILL_TARGET_ENNEMY_ALL,
    SKILL_TARGET_ENNEMY_ALL_EXCEPT,

    SKILL_TARGET_ALLY_SINGLE,
    SKILL_TARGET_ALLY_SINGLE_RANDOM,
    SKILL_TARGET_ALLY_DUAL,
    SKILL_TARGET_ALLY_DUAL_RANDOM,
    SKILL_TARGET_ALLY_WIDE,
    SKILL_TARGET_ALLY_WIDE_RANDOM,
    SKILL_TARGET_ALLY_ALL,
    SKILL_TARGET_ALLY_ALL_EXCEPT,

    SKILL_TARGET_ALL
};

// Skill effects
enum SkillEffectType {
    SKILLEFFECT_HP = 0,
    SKILLEFFECT_ATB,
    SKILLEFFECT_STATUS,

    SKILLEFFECT_COUNT
};

enum SkillEffectScaling {
    SKILLEFFECT_SCALING_CONST = 0,
    SKILLEFFECT_SCALING_SELF_CURRENT,
    SKILLEFFECT_SCALING_SELF_MAX,
    SKILLEFFECT_SCALING_TARGET_CURRENT,
    SKILLEFFECT_SCALING_TARGET_MAX
};

// Skill types
enum SkillType {
    SKILL_TYPE_LEADER = 0,
    SKILL_TYPE_PASSIVE,
    SKILL_TYPE_ATTACK,
    SKILL_TYPE_DEFENSE
};

enum SkillPassiveType {
    SKILL_PASSIVE_BUFF,
    SKILL_PASSIVE_BUFF_ALL,
    SKILL_PASSIVE_DEBUFF,
    SKILL_PASSIVE_DEBUFF_ALL,
    SKILL_PASSIVE_ONTURNSTART,
    SKILL_PASSIVE_ONTURNSTART_ALLY,
    SKILL_PASSIVE_ONTURNSTART_ENNEMY,
    SKILL_PASSIVE_ONTURNEND,
    SKILL_PASSIVE_ONTURNEND_ALLY,
    SKILL_PASSIVE_ONTURNEND_ENNEMY,
    SKILL_PASSIVE_ONHIT,
    SKILL_PASSIVE_ONHIT_ALLY,
    SKILL_PASSIVE_ONCRIT,
    SKILL_PASSIVE_ONCRIT_ALLY,
    SKILL_PASSIVE_ONBEINGHIT,
    SKILL_PASSIVE_ONBEINGHIT_ALLY,
    SKILL_PASSIVE_ONBEINGCRIT,
    SKILL_PASSIVE_ONBEINGCRIT_ALLY,
};

enum SkillAttackType {
    SKILL_ATTACK_SINGLE,
    SKILL_ATTACK_SINGLE_RANDOM,
    SKILL_ATTACK_AOE,
    SKILL_ATTACK_AOE_RANDOM,
};

enum SkillDefenseType {
    SKILL_DEFENSE_SINGLE,
    SKILL_DEFENSE_SINGLE_RANDOM,
    SKILL_DEFENSE_AOE,
    SKILL_DEFENSE_AOE_RANDOM,
};

// Skill sets
#define SKILL_SLOT_COUNT 4

// Ptototypes
class MonsterBattleInstance;
class BattleTeam;

/////////////////////////////////////////////////////////////////////////////////
// The SkillEffect class
class SkillEffect
{
public:
    SkillEffect();
    virtual ~SkillEffect();

    virtual SkillEffectType GetType() const = 0;

    // Interface
    virtual Void Apply( BattleTeam * pAllyTeam, UInt iCasterMonster, BattleTeam * pEnnemyTeam,
                        BattleTeam * pCasterTargetTeam, UInt iCasterTarget ) const = 0;

protected:
    // Helpers
    static UInt _ResolveEnnemyTargets( MonsterBattleInstance ** outTargets, UInt * outMainTarget, SkillTargetPattern iPattern,
                                       BattleTeam * pAllyTeam, UInt iAllyTarget, BattleTeam * pEnnemyTeam, UInt iEnnemyTarget );
    static UInt _ResolveAllyTargets( MonsterBattleInstance ** outTargets, UInt * outMainTarget, SkillTargetPattern iPattern,
                                     BattleTeam * pAllyTeam, UInt iAllyTarget, BattleTeam * pEnnemyTeam, UInt iEnnemyTarget );
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillEffectHP class
class SkillEffectHP : public SkillEffect
{
public:
    SkillEffectHP();
    virtual ~SkillEffectHP();

    inline virtual SkillEffectType GetType() const;

    // Interface
    inline Bool IsDamage() const;
    inline Bool IsHeal() const;
    inline Bool IsMixed() const;

    inline Bool IsSingle() const;
    inline Bool IsMulti() const;
    inline Bool IsAOE() const;
    inline Bool IsRandommized() const;

    virtual Void Apply( BattleTeam * pCasterTeam, UInt iCasterMonster, BattleTeam * pEnnemyTeam,
                        BattleTeam * pCasterTargetTeam, UInt iCasterTarget ) const;

protected:
    // Damage part
    Bool m_bDamage;
    SkillTargetPattern m_iDamageTargetPattern;

    SkillEffectScaling m_iDamageScaling;
    Float m_fDamageMultiplier;

    // Heal part
    Bool m_bHeal;
    SkillTargetPattern m_iHealTargetPattern;

    SkillEffectScaling m_iHealScaling;
    Float m_fHealMultiplier;
};


/////////////////////////////////////////////////////////////////////////////////
// The Skill class
class Skill
{
public:
    Skill( XMLNode * pSkillNode );
    virtual ~Skill();

    // Getters
    inline SkillID GetID() const;

    inline Bool IsLeader() const;
    inline Bool IsPassive() const;
    inline Bool IsActive() const;

    inline Bool IsAttack() const;
    inline Bool IsDefense() const;

    inline Bool IsSingleTarget() const;
    inline Bool IsAreaOfEffect() const;
    inline Bool IsRandomTarget() const;

    // Interface

    // Callbacks

protected:
    SkillID m_iSkillID;

    SkillType m_iType;
    GChar m_strName[SKILL_NAME_LENGTH];

    UInt m_iMaxLevel; // 0 = no level (leader, most passives)

    Bool m_bRequiresAwakening;   // Awakened only
    Bool m_bHasAwakeningUpgrade; // Awakened only

    // Skill effects

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
    LeaderSkill();
    virtual ~LeaderSkill();

    // Getters
    inline MonsterStatistic GetBonusStat() const;
    inline Float GetBonusAmount() const;

    inline LeaderConstraint GetConstraint() const;

    // Interface

    // Callbacks

protected:
    MonsterStatistic m_iBonusStat;
    Float m_fBonusAmount;

    LeaderConstraint m_iConstraint;
};

/////////////////////////////////////////////////////////////////////////////////
// The PassiveSkill class
class PassiveSkill : public Skill
{
public:
    PassiveSkill();
    virtual ~PassiveSkill();

    // Interface
    virtual Bool AppliesEffects() const = 0;



    // Callbacks

protected:
};

/////////////////////////////////////////////////////////////////////////////////
// The ActiveSkill class
class ActiveSkill : public Skill
{
public:
    ActiveSkill();
    virtual ~ActiveSkill();

    // Interface
    virtual Bool AppliesEffects() const = 0;

    // Callbacks

protected:
};

/////////////////////////////////////////////////////////////////////////////////
// The AttackSkill class
class AttackSkill : public ActiveSkill
{
public:
    AttackSkill();
    virtual ~AttackSkill();

    // Interface
    virtual Bool InflictsDamage() const = 0;

    // Callbacks
protected:
};

/////////////////////////////////////////////////////////////////////////////////
// The DefenseSkill class
class DefenseSkill : public ActiveSkill
{
public:
    DefenseSkill();
    virtual ~DefenseSkill();

    // Interface
    virtual Bool AppliesHeal() const = 0;

    // Callbacks

protected:
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillInstance class
class SkillInstance
{
public:
    SkillInstance( Skill * pSkill );
    ~SkillInstance();

private:
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillSet class
class SkillSet
{
public:
    SkillSet();
    ~SkillSet();

    // Deferred loading
    Void Load( XMLNode * pNode );

private:
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Skill.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_SKILL_H

