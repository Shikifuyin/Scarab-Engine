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

    SKILL_STAT_SPECIFIC_1,
    SKILL_STAT_SPECIFIC_2,

    SKILL_STAT_COUNT
};

typedef struct _skilllevel_bonus {
    SkillStat iStat;
    Float fAmount;
} SkillLevelBonus;

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
#define SKILL_MAX_EFFECTS 16 // should be enough to do insane stuff already

enum SkillEffectType {
    SKILLEFFECT_DAMAGE = 0,
    SKILLEFFECT_HEAL,
    SKILLEFFECT_ATB,
    SKILLEFFECT_STATUS,

    SKILLEFFECT_COUNT
};

enum SkillEffectScaling {
    SKILLEFFECT_SCALING_DEFAULT = 0,
    SKILLEFFECT_SCALING_SELF_HP_CURRENT,
    SKILLEFFECT_SCALING_SELF_HP_MAX,
    SKILLEFFECT_SCALING_TARGET_HP_CURRENT,
    SKILLEFFECT_SCALING_TARGET_HP_MAX
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
    virtual Void Apply( BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pEnnemyTeam,
                        BattleTeam * pCasterTargetTeam, UInt iCasterTarget ) const = 0;

protected:
    // Helpers
    static UInt _ResolveEnnemyTargets( MonsterBattleInstance ** outTargets, SkillTargetPattern iPattern,
                                       BattleTeam * pEnnemyTeam, UInt iEnnemyTarget );
    static UInt _ResolveAllyTargets( MonsterBattleInstance ** outTargets, SkillTargetPattern iPattern,
                                     UInt iCaster, BattleTeam * pAllyTeam, UInt iAllyTarget );
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillEffectHP class
class SkillEffectDamage : public SkillEffect
{
public:
    SkillEffectDamage( XMLNode * pNode );
    virtual ~SkillEffectDamage();

    inline virtual SkillEffectType GetType() const;

    // Interface
    virtual Void Apply( BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pEnnemyTeam,
                        BattleTeam * pCasterTargetTeam, UInt iCasterTarget ) const;

protected:
    // Helpers
    UInt _ComputeDamage( MonsterBattleInstance * pCaster, MonsterBattleInstance * pTarget ) const;

    // Target pattern
    SkillTargetPattern m_iTargetPattern;

    // Damage scaling
    SkillEffectScaling m_iScalingType;
    Float m_fScalingMultiplier;

    // Skill bonuses
    Float m_fSkillBonusDmg;
    Float m_fSkillBonusCritRate;
    Float m_fSkillBonusCritDmg;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillEffectHP class
class SkillEffectHeal : public SkillEffect
{
public:
    SkillEffectHeal();
    virtual ~SkillEffectHeal();

    inline virtual SkillEffectType GetType() const;

    // Interface
    virtual Void Apply( MonsterBattleInstance * pCaster, MonsterBattleInstance * pTarget ) const;

protected:
    // Helpers
    UInt _ComputeHeal( MonsterBattleInstance * pCaster, MonsterBattleInstance * pTarget ) const;

    // Damage part
    Bool m_bDamage;
    SkillTargetPattern m_iDamageTargetPattern;

    SkillEffectScaling m_iDamageScaling;
    Float m_fDamageMultiplier;

    Float m_fSkillBonusCritRate;
    Float m_fSkillBonusCritDmg;

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

    // Identifier
    inline SkillID GetID() const;
    inline const GChar * GetName() const;

    // Type
    inline Bool IsLeader() const;
    inline Bool IsPassive() const;
    inline Bool IsActive() const;

    inline Bool IsAttack() const;
    inline Bool IsDefense() const;

    inline Bool IsSingleTarget() const;
    inline Bool IsAreaOfEffect() const;
    inline Bool IsRandomTarget() const;

    inline SkillType GetType() const;

    // Leveling stats
    inline UInt GetMaxLevel() const;
    inline const SkillLevelBonus * GetLevelBonus( UInt iLevel ) const;

    // Awakening
    inline Bool RequiresAwakening() const;
    inline Bool HasAwakeningUpgrade() const;

    // Skill effects
    inline UInt GetEffectCount() const;
    inline SkillEffect * GetEffect( UInt iIndex ) const;

protected:
    // Identifier
    SkillID m_iSkillID;
    GChar m_strName[SKILL_NAME_LENGTH];

    // Type
    SkillType m_iType;

    // Leveling stats
    UInt m_iMaxLevel; // 0 = no level (leader, most passives)
    SkillLevelBonus m_arrLevelBonus[SKILL_MAX_LEVEL];

    // Awakening
    Bool m_bRequiresAwakening;   // Awakened only
    Bool m_bHasAwakeningUpgrade; // Awakened only

    // Skill effects
    UInt m_iEffectCount;
    SkillEffect * m_arrEffects[SKILL_MAX_EFFECTS];
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

    // Leader skill
    inline MonsterStatistic GetLeaderBonusStat() const;
    inline Float GetLeaderBonusAmount() const;

    inline LeaderConstraint GetLeaderConstraint() const;

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

