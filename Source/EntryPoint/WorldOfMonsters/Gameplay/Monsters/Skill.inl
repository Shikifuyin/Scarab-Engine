/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Gameplay/Monsters/Skill.inl
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
    return ( GetType() == SKILL_TYPE_ACTIVE );
}
inline Bool Skill::IsPassive() const {
    return ( GetType() == SKILL_TYPE_PASSIVE );
}
inline Bool Skill::IsLeader() const {
    return ( GetType() == SKILL_TYPE_LEADER );
}

inline const SkillLevelingStats * Skill::GetLevelingStats() const {
    return &m_hLevelingStats;
}

/////////////////////////////////////////////////////////////////////////////////
// ActiveSkill implementation
inline SkillType ActiveSkill::GetType() const {
    return SKILL_TYPE_ACTIVE;
}

inline Bool ActiveSkill::IsAttack() const {
    return m_bIsAttack;
}
inline Bool ActiveSkill::IsDefense() const {
    return !m_bIsAttack;
}

inline UInt ActiveSkill::GetEffectCount( SkillActiveType iType ) const {
    Assert( iType < SKILL_ACTIVE_COUNT );
    return m_arrEffectCounts[iType];
}
inline const SkillEffect * ActiveSkill::GetEffect( SkillActiveType iType, UInt iIndex ) const {
    Assert( iType < SKILL_ACTIVE_COUNT );
    Assert( iIndex < m_arrEffectCounts[iType] );
    return m_arrEffects[iType][iIndex];
}

/////////////////////////////////////////////////////////////////////////////////
// PassiveSkill implementation
inline SkillType PassiveSkill::GetType() const {
    return SKILL_TYPE_PASSIVE;
}

inline UInt PassiveSkill::GetEffectCount( SkillPassiveType iType ) const {
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
inline SkillType LeaderSkill::GetType() const {
    return SKILL_TYPE_LEADER;
}

inline MonsterStatistic LeaderSkill::GetLeaderBonusStat() const {
    return m_iBonusStat;
}
inline Float LeaderSkill::GetLeaderBonusAmount() const {
    return m_fBonusAmount;
}

inline SkillLeaderConstraint LeaderSkill::GetLeaderConstraint() const {
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

inline UInt SkillInstance::GetMaxLevel() const {
    return m_pSkill->GetLevelingStats()->GetMaxLevel();
}
inline UInt SkillInstance::GetLevel() const {
    return m_iLevel;
}
inline Bool SkillInstance::IsMaxLevel() const {
    return ( m_iLevel == (m_pSkill->GetLevelingStats()->GetMaxLevel() - 1) );
}
inline Void SkillInstance::LevelUp() {
    if ( m_iLevel < (m_pSkill->GetLevelingStats()->GetMaxLevel() - 1) )
        ++m_iLevel;
}
inline Void SkillInstance::LevelDown() {
    if ( m_iLevel > 0 )
        --m_iLevel;
}
inline Void SkillInstance::SetLevel( UInt iLevel ) {
    Assert( iLevel < m_pSkill->GetLevelingStats()->GetMaxLevel() );
    m_iLevel = iLevel;
}

inline Float SkillInstance::GetBonusDamage() const {
    return m_pSkill->GetLevelingStats()->GetBonusDamage( m_iLevel );
}
inline Float SkillInstance::GetBonusRecovery() const {
    return m_pSkill->GetLevelingStats()->GetBonusRecovery( m_iLevel );
}
inline Float SkillInstance::GetBonusStatusEffectRate() const {
    return m_pSkill->GetLevelingStats()->GetBonusStatusEffectRate( m_iLevel );
}
inline Float SkillInstance::GetBonusSpecific() const {
    return m_pSkill->GetLevelingStats()->GetBonusSpecific( m_iLevel );
}

inline Bool SkillInstance::HasCooldown() const {
    return ( m_pSkill->GetLevelingStats()->GetCooldown(m_iLevel) > 0 );
}
inline UInt SkillInstance::GetCooldown() const {
    return m_pSkill->GetLevelingStats()->GetCooldown( m_iLevel );
}

/////////////////////////////////////////////////////////////////////////////////
// SkillSet implementation
inline UInt SkillSet::GetSkillCount() const {
    return m_iSkillCount;
}
inline const SkillInstance * SkillSet::GetSkillInstance( UInt iSlot ) const {
    Assert( iSlot < m_iSkillCount );
    return ( m_arrSkills + iSlot );
}
inline SkillInstance * SkillSet::GetSkillInstance( UInt iSlot ) {
    Assert( iSlot < m_iSkillCount );
    return ( m_arrSkills + iSlot );
}

