/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Battle/BattleMonsterInstance.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Battle : BattleMonsterInstance interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BattleMonsterInstance implementation
inline Bool BattleMonsterInstance::IsNull() const {
    return ( m_pMonsterInstance == NULL );
}
inline Bool BattleMonsterInstance::IsPresent() const {
    return ( m_pMonsterInstance != NULL );
}

inline const MonsterInstance * BattleMonsterInstance::GetMonsterInstance() const {
    return m_pMonsterInstance;
}
inline const LeaderSkill * BattleMonsterInstance::GetActiveLeaderSkill() const {
    return m_pActiveLeaderSkill;
}

inline UInt BattleMonsterInstance::GetHP() const {
    return m_iHealth;
}
inline UInt BattleMonsterInstance::GetATT() const {
    return m_iAttack;
}
inline UInt BattleMonsterInstance::GetDEF() const {
    return m_iDefense;
}
inline UInt BattleMonsterInstance::GetSPD() const {
    return m_iSpeed;
}

inline Float BattleMonsterInstance::GetCritR() const {
    return m_fCriticalRate;
}
inline Float BattleMonsterInstance::GetCritD() const {
    return m_fCriticalDamage;
}
inline Float BattleMonsterInstance::GetACC() const {
    return m_fAccuracy;
}
inline Float BattleMonsterInstance::GetRES() const {
    return m_fResistance;
}

inline Bool BattleMonsterInstance::IsAlive() const {
    return ( m_iCurrentHP > 0 );
}
inline Bool BattleMonsterInstance::IsDead() const {
    return ( m_iCurrentHP == 0 );
}

inline UInt BattleMonsterInstance::GetCurrentHP() const {
    return m_iCurrentHP;
}
inline Void BattleMonsterInstance::SetCurrentHP( UInt iAmount ) {
    if ( iAmount > m_iHealth )
        iAmount = m_iHealth;
    m_iCurrentHP = iAmount;
}
inline Void BattleMonsterInstance::ResetCurrentHP() {
    m_iCurrentHP = m_iHealth;
}

inline UInt BattleMonsterInstance::GetShieldHP() const {
    Assert( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_SHIELD) );
    return m_iShieldHP;
}
inline Void BattleMonsterInstance::SetShieldHP( UInt iAmount ) {
    Assert( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_SHIELD) );
    m_iShieldHP = iAmount;
}

inline Bool BattleMonsterInstance::CheckATB() const {
    return ( m_iATB >= BATTLE_ATB_CAPACITY );
}
inline UInt BattleMonsterInstance::GetATB() const {
    return m_iATB;
}

inline Void BattleMonsterInstance::IncreaseATB( Float fRatio ) {
    UInt iAmount = (UInt)(MathFn->Floor( fRatio * (Float)BATTLE_ATB_CAPACITY ));
    m_iATB += iAmount;
}
inline Void BattleMonsterInstance::IncreaseATB( UInt iAmount ) {
    m_iATB += iAmount;
}
inline Void BattleMonsterInstance::DecreaseATB( Float fRatio ) {
    UInt iAmount = (UInt)(MathFn->Floor( fRatio * (Float)BATTLE_ATB_CAPACITY ));
    if ( m_iATB > iAmount )
        m_iATB -= iAmount;
    else
        m_iATB = 0;
}
inline Void BattleMonsterInstance::DecreaseATB( UInt iAmount ) {
    if ( m_iATB > iAmount )
        m_iATB -= iAmount;
    else
        m_iATB = 0;
}
inline Void BattleMonsterInstance::FillATB() {
    m_iATB = BATTLE_ATB_CAPACITY;
}
inline Void BattleMonsterInstance::ResetATB() {
    m_iATB = 0;
}

inline UInt BattleMonsterInstance::GetSkillCount() const {
    return m_pMonsterInstance->GetSkillCount();
}
inline const SkillInstance * BattleMonsterInstance::GetSkillInstance( UInt iSlot ) const {
    return m_pMonsterInstance->GetSkillInstance( iSlot );
}

inline Bool BattleMonsterInstance::HasSkillUp( UInt iSlot ) const {
    Assert( iSlot < m_pMonsterInstance->GetSkillCount() );
    return ( m_arrSkillCooldowns[iSlot] == 0 );
}
inline UInt BattleMonsterInstance::GetSkillCooldown( UInt iSlot ) const {
    Assert( iSlot < m_pMonsterInstance->GetSkillCount() );
    return m_arrSkillCooldowns[iSlot];
}

inline Void BattleMonsterInstance::IncreaseSkillCooldown( UInt iSlot ) {
    Assert( iSlot < m_pMonsterInstance->GetSkillCount() );
    ++(m_arrSkillCooldowns[iSlot]);
}
inline Void BattleMonsterInstance::DecreaseSkillCooldown( UInt iSlot ) {
    Assert( iSlot < m_pMonsterInstance->GetSkillCount() );
    if ( m_arrSkillCooldowns[iSlot] > 0 )
        --(m_arrSkillCooldowns[iSlot]);
}
inline Void BattleMonsterInstance::TriggerSkillCooldown( UInt iSlot ) {
    Assert( iSlot < m_pMonsterInstance->GetSkillCount() );
    m_arrSkillCooldowns[iSlot] = m_pMonsterInstance->GetSkillInstance(iSlot)->GetCooldown();
}
inline Void BattleMonsterInstance::ResetSkillCooldown( UInt iSlot ) {
    Assert( iSlot < m_pMonsterInstance->GetSkillCount() );
    m_arrSkillCooldowns[iSlot] = 0;
}

inline Bool BattleMonsterInstance::HasStatusEffect( StatusEffectType iType ) const {
    return m_hActiveEffects.HasStatusEffect( iType );
}
inline const StatusEffect * BattleMonsterInstance::GetStatusEffect( StatusEffectType iType ) const {
    return m_hActiveEffects.GetStatusEffect( iType );
}
inline StatusEffect * BattleMonsterInstance::GetStatusEffect( StatusEffectType iType ) {
    return m_hActiveEffects.GetStatusEffect( iType );
}

inline Bool BattleMonsterInstance::IsDisabled() const {
    return m_bDisabled;
}



