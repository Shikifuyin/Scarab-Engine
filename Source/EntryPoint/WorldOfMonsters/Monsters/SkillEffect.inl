/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/SkillEffect.inl
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
// SkillEffect implementation
inline Float SkillEffect::GetProc() const {
    return m_fProc;
}

inline Bool SkillEffect::IsOffensive() const {
    return ( m_iTargetPattern >= SKILL_TARGET_ENNEMY_SINGLE );
}
inline Bool SkillEffect::IsDefensive() const {
    return ( m_iTargetPattern < SKILL_TARGET_ENNEMY_SINGLE || m_iTargetPattern == SKILL_TARGET_ALL );
}
inline SkillTargetPattern SkillEffect::GetTargetPattern() const {
    return m_iTargetPattern;
}

inline SkillEffectScaling SkillEffect::GetScalingType() const {
    return m_iScalingType;
}
inline Float SkillEffect::GetScalingMultiplier() const {
    return m_fScalingMultiplier;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectDamage implementation
inline SkillEffectType SkillEffectDamage::GetType() const {
    return SKILLEFFECT_DAMAGE;
}

inline Float SkillEffectDamage::GetBonusDmg() const {
    return m_fBonusDmg;
}
inline Float SkillEffectDamage::GetBonusCritRate() const {
    return m_fBonusCritRate;
}
inline Float SkillEffectDamage::GetBonusCritDmg() const {
    return m_fBonusCritDmg;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectHeal implementation
inline SkillEffectType SkillEffectHeal::GetType() const {
    return SKILLEFFECT_HEAL;
}

inline Float SkillEffectHeal::GetBonusHeal() const {
    return m_fBonusHeal;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectATB implementation
inline SkillEffectType SkillEffectATB::GetType() const {
    return SKILLEFFECT_ATB;
}

inline Bool SkillEffectATB::IsIncrease() const {
    return m_bIncrease;
}
inline Bool SkillEffectATB::IsDecrease() const {
    return !m_bIncrease;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectStatus implementation
inline SkillEffectType SkillEffectStatus::GetType() const {
    return SKILLEFFECT_STATUS;
}

inline StatusEffectType SkillEffectStatus::GetStatusEffectType() const {
    return m_iType;
}

inline Bool SkillEffectStatus::IsRemovable() const {
    return m_bRemovable;
}

inline Float SkillEffectStatus::GetAmplitude() const {
    return m_fAmplitude;
}

inline UInt SkillEffectStatus::GetMaxStacks() const {
    return m_iMaxStacks;
}
inline UInt SkillEffectStatus::GetStackCount() const {
    return m_iStackCount;
}
inline UInt SkillEffectStatus::GetDuration() const {
    return m_iDuration;
}

