/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Skill.inl
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
// Skill implementation
inline SkillID Skill::GetID() const {
    return m_iSkillID;
}
inline const GChar * Skill::GetName() const {
    return m_strName;
}

inline Bool Skill::IsActive() const {
    return ( m_iType == SKILL_TYPE_ACTIVE );
}
inline Bool Skill::IsPassive() const {
    return ( m_iType == SKILL_TYPE_PASSIVE );
}
inline Bool Skill::IsLeader() const {
    return ( m_iType == SKILL_TYPE_LEADER );
}

inline SkillType Skill::GetType() const {
    return m_iType;
}

inline UInt Skill::GetCooldown() const {
    return m_iCooldown;
}

inline UInt Skill::GetMaxLevel() const {
    return m_iMaxLevel;
}
inline const SkillLevelBonus * Skill::GetLevelBonus( UInt iLevel ) const {
    Assert( iLevel < m_iMaxLevel );
    return ( m_arrLevelBonus + iLevel );
}

inline Bool Skill::RequiresAwakening() const {
    return m_bRequiresAwakening;
}
inline Bool Skill::HasAwakeningUpgrade() const {
    return m_bHasAwakeningUpgrade;
}
inline SkillID Skill::GetAwakeningUpgrade() const {
    return m_iAwakeningUpgradeID;
}

/////////////////////////////////////////////////////////////////////////////////
// ActiveSkill implementation
inline Bool ActiveSkill::IsAttack() const {
    return m_bIsAttack;
}
inline Bool ActiveSkill::IsDefense() const {
    return !m_bIsAttack;
}

inline UInt ActiveSkill::GetEffectCount( SkillActiveType iType ) {
    Assert( iType < SKILL_ACTIVE_COUNT );
    return m_arrEffectCounts[iType];
}
inline SkillEffect * ActiveSkill::GetEffect( SkillActiveType iType, UInt iIndex ) const {
    Assert( iType < SKILL_ACTIVE_COUNT );
    Assert( iIndex < m_arrEffectCounts[iType] );
    return m_arrEffects[iType][iIndex];
}

/////////////////////////////////////////////////////////////////////////////////
// PassiveSkill implementation
inline UInt PassiveSkill::GetEffectCount( SkillPassiveType iType ) {
    Assert( iType < SKILL_PASSIVE_COUNT );
    return m_arrEffectCounts[iType];
}
inline SkillEffect * PassiveSkill::GetEffect( SkillPassiveType iType, UInt iIndex ) const {
    Assert( iType < SKILL_PASSIVE_COUNT );
    Assert( iIndex < m_arrEffectCounts[iType] );
    return m_arrEffects[iType][iIndex];
}

/////////////////////////////////////////////////////////////////////////////////
// LeaderSkill implementation
inline MonsterStatistic LeaderSkill::GetLeaderBonusStat() const {
    return m_iBonusStat;
}
inline Float LeaderSkill::GetLeaderBonusAmount() const {
    return m_fBonusAmount;
}

inline LeaderSkill::LeaderConstraint LeaderSkill::GetLeaderConstraint() const {
    return m_iConstraint;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillInstance implementation
inline Bool SkillInstance::IsNull() const {
    return ( m_pSkill == NULL );
}
inline Bool SkillInstance::IsPresent() const {
    return ( m_pSkill != NULL );
}

inline Skill * SkillInstance::GetSkill() const {
    return m_pSkill;
}

inline SkillID SkillInstance::GetID() const {
    return m_pSkill->GetID();
}
inline const GChar * SkillInstance::GetName() const {
    return m_pSkill->GetName();
}

inline Bool SkillInstance::IsActive() const {
    return m_pSkill->IsActive();
}
inline Bool SkillInstance::IsPassive() const {
    return m_pSkill->IsPassive();
}
inline Bool SkillInstance::IsLeader() const {
    return m_pSkill->IsLeader();
}

inline SkillType SkillInstance::GetType() const {
    return m_pSkill->GetType();
}

inline UInt SkillInstance::GetCooldown() const {
    return m_pSkill->GetCooldown();
}

inline Bool SkillInstance::RequiresAwakening() const {
    return m_pSkill->RequiresAwakening();
}
inline Bool SkillInstance::HasAwakeningUpgrade() const {
    return m_pSkill->HasAwakeningUpgrade();
}
inline SkillID SkillInstance::GetAwakeningUpgrade() const {
    return m_pSkill->GetAwakeningUpgrade();
}

inline UInt SkillInstance::GetMaxLevel() const {
    return m_pSkill->GetMaxLevel();
}
inline UInt SkillInstance::GetLevel() const {
    return m_iLevel;
}

inline Bool SkillInstance::HasEffectiveBonus( SkillStat iStat ) const {
    Assert( iStat < SKILL_STAT_COUNT );
    return ( m_fEffectiveBonus[iStat] != 0.0f );
}
inline Float SkillInstance::GetEffectiveBonus( SkillStat iStat ) const {
    Assert( iStat < SKILL_STAT_COUNT );
    return m_fEffectiveBonus[iStat];
}

/////////////////////////////////////////////////////////////////////////////////
// SkillSet implementation
inline UInt SkillSet::GetSkillCount() const {
    return m_iSkillCount;
}
inline SkillInstance * SkillSet::GetSkillInstance( UInt iSlot ) {
    Assert( iSlot < m_iSkillCount );
    return ( m_arrSkills + iSlot );
}

