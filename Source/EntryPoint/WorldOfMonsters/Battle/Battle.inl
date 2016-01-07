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
inline MonsterInstance * MonsterBattleInstance::GetBaseInstance() {
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

inline UInt MonsterBattleInstance::GetShieldHP() const {
    Assert( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_SHIELD) );
    return m_iShieldHP;
}
inline Void MonsterBattleInstance::SetShieldHP( UInt iAmount ) {
    Assert( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_SHIELD) );
    m_iShieldHP = iAmount;
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

inline UInt MonsterBattleInstance::GetSkillCount() const {
    return m_pMonsterInstance->GetSkillCount();
}
inline SkillInstance * MonsterBattleInstance::GetSkillInstance( UInt iSlot ) {
    return m_pMonsterInstance->GetSkillInstance( iSlot );
}

inline Bool MonsterBattleInstance::HasSkillUp( UInt iSlot ) const {
    Assert( iSlot < SKILL_SLOT_COUNT );
    return ( m_arrSkillCooldowns[iSlot] == 0 );
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
    m_arrSkillCooldowns[iSlot] = m_pMonsterInstance->GetSkillInstance(iSlot)->GetCooldown();
}
inline Void MonsterBattleInstance::ResetSkillCooldown( UInt iSlot ) {
    Assert( iSlot < SKILL_SLOT_COUNT );
    m_arrSkillCooldowns[iSlot] = 0;
}

inline Bool MonsterBattleInstance::HasStatusEffect( StatusEffectType iType ) const {
    return m_hActiveEffects.HasStatusEffect( iType );
}

inline StatusEffect * MonsterBattleInstance::GetStatusEffect( StatusEffectType iType ) {
    return m_hActiveEffects.GetStatusEffect( iType );
}

inline Void MonsterBattleInstance::RemoveExpiredStatusEffects() {
    m_hActiveEffects.RemoveExpiredStatusEffects();
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

inline BattleTeam * Battle::GetPlayerTeam() const {
    return m_pPlayerTeam;
}
inline BattleTeam * Battle::GetAITeam() const {
    return m_pAITeam;
}

inline Bool Battle::IsTurnPending() const {
    return m_bTurnPending;
}
inline Bool Battle::IsTurnInProgress() const {
    return m_bTurnInProgress;
}
inline Bool Battle::IsPlayerTurn() const {
    return ( m_bTurnPending && m_bPlayerTurn );
}
inline Bool Battle::IsAITurn() const {
    return ( m_bTurnPending && !m_bPlayerTurn );
}

inline MonsterBattleInstance * Battle::GetTurnMonster() const {
    return m_pTurnMonster;
}

/////////////////////////////////////////////////////////////////////////////////

inline Bool Battle::_IsElementWeakAgainst( MonsterElement iElement, MonsterElement iOpposingElement ) {
    return ( sm_arrElementWeakAgainst[iOpposingElement] == iElement );
}
inline Bool Battle::_IsElementStrongAgainst( MonsterElement iElement, MonsterElement iOpposingElement ) {
    return ( sm_arrElementStrongAgainst[iOpposingElement] == iElement );
}

inline MonsterElement Battle::_GetElementWeakAgainst( MonsterElement iElement ) {
    return sm_arrElementWeakAgainst[iElement];
}
inline MonsterElement Battle::_GetElementStrongAgainst( MonsterElement iElement ) {
    return sm_arrElementStrongAgainst[iElement];
}
