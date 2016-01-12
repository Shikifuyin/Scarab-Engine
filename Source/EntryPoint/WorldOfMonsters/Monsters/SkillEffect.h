/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/SkillEffect.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Base SkillEffect interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_SKILLEFFECT_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_SKILLEFFECT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "LevelingStats.h"
#include "StatusEffect.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Skill target patterns
enum SkillTargetPattern {
    SKILL_TARGET_SELF = 0,

    SKILL_TARGET_ALLY_SINGLE,
    SKILL_TARGET_ALLY_SINGLE_RANDOM,
    SKILL_TARGET_ALLY_DUAL,
    SKILL_TARGET_ALLY_DUAL_RANDOM,
    SKILL_TARGET_ALLY_WIDE,
    SKILL_TARGET_ALLY_WIDE_RANDOM,
    SKILL_TARGET_ALLY_ALL,
    SKILL_TARGET_ALLY_ALL_EXCEPT,

    SKILL_TARGET_ENNEMY_SINGLE,
    SKILL_TARGET_ENNEMY_SINGLE_RANDOM,
    SKILL_TARGET_ENNEMY_DUAL,
    SKILL_TARGET_ENNEMY_DUAL_RANDOM,
    SKILL_TARGET_ENNEMY_WIDE,
    SKILL_TARGET_ENNEMY_WIDE_RANDOM,
    SKILL_TARGET_ENNEMY_ALL,
    SKILL_TARGET_ENNEMY_ALL_EXCEPT,

    SKILL_TARGET_ALL,

    SKILL_TARGET_COUNT
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
    SKILLEFFECT_SCALING_TARGET_HP_MAX,

    SKILLEFFECT_SCALING_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillEffect class
class SkillEffect
{
public:
    SkillEffect( XMLNode * pNode );
    virtual ~SkillEffect();

    // Type
    virtual SkillEffectType GetType() const = 0;

    // Probability of occurence (Proc)
    inline Float GetProc() const;

    // Target pattern
    inline Bool IsOffensive() const;
    inline Bool IsDefensive() const;
    inline SkillTargetPattern GetTargetPattern() const;

    // Scaling
    inline SkillEffectScaling GetScalingType() const;
    inline Float GetScalingMultiplier() const;

protected:
    // Helpers
    static SkillTargetPattern _SkillTargetPattern_FromString( const GChar * strValue );
    static SkillEffectScaling _SkillEffectScaling_FromString( const GChar * strValue );

    // Probability of occurence (Proc)
    Float m_fProc;

    // Target pattern
    SkillTargetPattern m_iTargetPattern;

    // Scaling
    SkillEffectScaling m_iScalingType;
    Float m_fScalingMultiplier;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillEffectDamage class
class SkillEffectDamage : public SkillEffect
{
public:
    SkillEffectDamage( XMLNode * pNode );
    virtual ~SkillEffectDamage();

    // Type
    inline virtual SkillEffectType GetType() const;

    // Damage
    inline Float GetBonusDmg() const;
    inline Float GetBonusCritRate() const;
    inline Float GetBonusCritDmg() const;

protected:
    // Damage
    Float m_fBonusDmg;
    Float m_fBonusCritRate;
    Float m_fBonusCritDmg;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillEffectHeal class
class SkillEffectHeal : public SkillEffect
{
public:
    SkillEffectHeal( XMLNode * pNode );
    virtual ~SkillEffectHeal();

    // Type
    inline virtual SkillEffectType GetType() const;

    // Heal
    inline Float GetBonusHeal() const;

protected:
    // Heal
    Float m_fBonusHeal;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillEffectATB class
class SkillEffectATB : public SkillEffect
{
public:
    SkillEffectATB( XMLNode * pNode );
    virtual ~SkillEffectATB();

    // Type
    inline virtual SkillEffectType GetType() const;

    // ATB
    inline Bool IsIncrease() const;
    inline Bool IsDecrease() const;

protected:
    // ATB
    Bool m_bIncrease; // else decrease
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillEffectStatus class
class SkillEffectStatus : public SkillEffect
{
public:
    SkillEffectStatus( XMLNode * pNode );
    virtual ~SkillEffectStatus();

    // Type
    inline virtual SkillEffectType GetType() const;

    // StatusEffect
    inline StatusEffectType GetStatusEffectType() const;
    inline UInt GetStackCount() const;
    inline UInt GetDuration() const;
    inline Float GetAmplitude() const;

protected:
    // Helpers
    static StatusEffectType _StatusEffectType_FromString( const GChar * strValue );

    // StatusEffect
    StatusEffectType m_iType;
    UInt m_iStackCount;
    UInt m_iDuration;
    Float m_fAmplitude;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SkillEffect.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_SKILLEFFECT_H

