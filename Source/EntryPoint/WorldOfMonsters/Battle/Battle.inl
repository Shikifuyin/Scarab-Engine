/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Battle/Battle.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Battle : Base Battle interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MonsterBattleInstance implementation
inline const MonsterInstance * MonsterBattleInstance::GetBaseInstance() const {
    return m_pMonsterInstance;
}

inline Bool MonsterBattleInstance::IsAlive() const {
    return ( m_iCurrentHP > 0 );
}
inline Bool MonsterBattleInstance::IsDead() const {
    return ( m_iCurrentHP == 0 );
}

inline UInt MonsterBattleInstance::GetCurrentHP() const {
    return m_iCurrentHP;
}
inline Void MonsterBattleInstance::SetCurrentHP( UInt iAmount ) {
    if ( iAmount > m_iHealth )
        iAmount = m_iHealth;
    m_iCurrentHP = iAmount;
}
inline Void MonsterBattleInstance::ResetCurrentHP() {
    m_iCurrentHP = m_iHealth;
}

inline Void MonsterBattleInstance::Damage( UInt iAmount ) {
    if ( m_iCurrentHP > iAmount )
        m_iCurrentHP -= iAmount;
    else
        m_iCurrentHP = 0;
}
inline Void MonsterBattleInstance::Heal( UInt iAmount ) {
    m_iCurrentHP += iAmount;
    if ( m_iCurrentHP > m_iHealth )
        m_iCurrentHP = m_iHealth;
}

inline Bool MonsterBattleInstance::CheckATB() const {
    return ( m_iATB >= BATTLE_ATB_CAPACITY );
}
inline UInt MonsterBattleInstance::GetATB() const {
    return m_iATB;
}

inline Void MonsterBattleInstance::IncreaseATB( Float fRatio ) {
    UInt iAmount = (UInt)(MathFn->Floor( fRatio * (Float)BATTLE_ATB_CAPACITY ));
    m_iATB += iAmount;
}
inline Void MonsterBattleInstance::IncreaseATB( UInt iAmount ) {
    m_iATB += iAmount;
}
inline Void MonsterBattleInstance::DecreaseATB( Float fRatio ) {
    UInt iAmount = (UInt)(MathFn->Floor( fRatio * (Float)BATTLE_ATB_CAPACITY ));
    if ( m_iATB > iAmount )
        m_iATB -= iAmount;
    else
        m_iATB = 0;
}
inline Void MonsterBattleInstance::DecreaseATB( UInt iAmount ) {
    if ( m_iATB > iAmount )
        m_iATB -= iAmount;
    else
        m_iATB = 0;
}
inline Void MonsterBattleInstance::FillATB() {
    m_iATB = BATTLE_ATB_CAPACITY;
}
inline Void MonsterBattleInstance::ResetATB() {
    m_iATB = 0;
}

inline UInt MonsterBattleInstance::GetSkillCooldown( UInt iSlot ) const {
    Assert( iSlot < SKILL_SLOT_COUNT );
    return m_arrSkillCooldowns[iSlot];
}

inline Void MonsterBattleInstance::IncreaseSkillCooldown( UInt iSlot ) {
    Assert( iSlot < SKILL_SLOT_COUNT );
    ++(m_arrSkillCooldowns[iSlot]);
}
inline Void MonsterBattleInstance::DecreaseSkillCooldown( UInt iSlot ) {
    Assert( iSlot < SKILL_SLOT_COUNT );
    if ( m_arrSkillCooldowns[iSlot] > 0 )
        --(m_arrSkillCooldowns[iSlot]);
}
inline Void MonsterBattleInstance::TriggerSkillCooldown( UInt iSlot ) {
    Assert( iSlot < SKILL_SLOT_COUNT );
    m_arrSkillCooldowns[iSlot] = m_pMonsterInstance->GetSkillCooldown( iSlot );
}
inline Void MonsterBattleInstance::ResetSkillCooldown( UInt iSlot ) {
    Assert( iSlot < SKILL_SLOT_COUNT );
    m_arrSkillCooldowns[iSlot] = 0;
}

inline UInt MonsterBattleInstance::GetStatusEffectCount() const {
    return m_arrActiveEffects.Count();
}
inline StatusEffectInstance * MonsterBattleInstance::GetStatusEffect( UInt iIndex ) const {
    Assert( iIndex < m_arrActiveEffects.Count() );
    return &( (StatusEffectInstance&)(m_arrActiveEffects[iIndex]) );
}

inline Void MonsterBattleInstance::AddStatusEffect( const StatusEffectInstance & hEffect ) {
    if ( hEffect.IsStackable() )
        m_arrActiveEffects.Push( hEffect );
    else {
        
    }
    _UpdateBattleStats();
}
inline Void MonsterBattleInstance::RemoveStatusEffect( UInt iIndex ) {
    Assert( iIndex < m_arrActiveEffects.Count() );
    m_arrActiveEffects.Remove( iIndex, NULL, 1 );
    _UpdateBattleStats();
}
inline Void MonsterBattleInstance::RemoveAllStatusEffects() {
    m_arrActiveEffects.Clear();
    _UpdateBattleStats();
}

inline Bool MonsterBattleInstance::IsDisabled() const {
    return m_bDisabled;
}

inline UInt MonsterBattleInstance::GetHP() const {
    return m_iHealth;
}
inline UInt MonsterBattleInstance::GetATT() const {
    return m_iAttack;
}
inline UInt MonsterBattleInstance::GetDEF() const {
    return m_iDefense;
}
inline UInt MonsterBattleInstance::GetSPD() const {
    return m_iSpeed;
}

inline Float MonsterBattleInstance::GetCritR() const {
    return m_fCriticalRate;
}
inline Float MonsterBattleInstance::GetCritD() const {
    return m_fCriticalDamage;
}
inline Float MonsterBattleInstance::GetACC() const {
    return m_fAccuracy;
}
inline Float MonsterBattleInstance::GetRES() const {
    return m_fResistance;
}

/////////////////////////////////////////////////////////////////////////////////
// BattleTeam implementation
inline UInt BattleTeam::GetTeamSize() const {
    return m_iTeamSize;
}
inline MonsterBattleInstance * BattleTeam::GetTeamMember( UInt iIndex ) {
    Assert( iIndex < m_iTeamSize );
    return ( m_arrMonsters + iIndex );
}

/////////////////////////////////////////////////////////////////////////////////
// Battle implementation
inline BattleType Battle::GetType() const {
    return m_iType;
}

inline Bool Battle::IsPlayerTurn() const {
    return ( m_bTurnPending && m_bPlayerTurn );
}
inline Bool Battle::IsAITurn() const {
    return ( m_bTurnPending && !m_bPlayerTurn );
}
