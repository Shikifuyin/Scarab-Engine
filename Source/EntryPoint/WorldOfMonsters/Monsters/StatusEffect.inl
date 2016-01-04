/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/StatusEffect.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Base StatusEffect interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// StatusEffect implementation
inline StatusEffectType StatusEffect::GetType() const {
    return m_iType;
}

inline Bool StatusEffect::IsBuff() const {
    return ( m_iType < STATUSEFFECT_DEBUFF_ATTACK );
}
inline Bool StatusEffect::IsDebuff() const {
    return ( m_iType >= STATUSEFFECT_DEBUFF_ATTACK );
}
inline Bool StatusEffect::IsDisabling() const {
    return ( m_iType >= STATUSEFFECT_DEBUFF_STUN );
}

inline Bool StatusEffect::IsStackable() const {
    return sm_arrIsStackable[m_iType];
}

/////////////////////////////////////////////////////////////////////////////////
// StatusBuffAttack implementation
inline Float StatusBuffAttack::GetAmount() const {
    return m_fAmount;
}

/////////////////////////////////////////////////////////////////////////////////
// StatusBuffDefense implementation
inline Float StatusBuffDefense::GetAmount() const {
    return m_fAmount;
}

/////////////////////////////////////////////////////////////////////////////////
// StatusBuffSpeed implementation
inline Float StatusBuffSpeed::GetAmount() const {
    return m_fAmount;
}

/////////////////////////////////////////////////////////////////////////////////
// StatusDebuffAttack implementation
inline Float StatusDebuffAttack::GetAmount() const {
    return m_fAmount;
}

/////////////////////////////////////////////////////////////////////////////////
// StatusDebuffDefense implementation
inline Float StatusDebuffDefense::GetAmount() const {
    return m_fAmount;
}

/////////////////////////////////////////////////////////////////////////////////
// StatusDebuffSpeed implementation
inline Float StatusDebuffSpeed::GetAmount() const {
    return m_fAmount;
}

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectInstance implementation
inline StatusEffectType StatusEffectInstance::GetType() const {
    return m_pStatusEffect->GetType();
}

inline StatusEffect * StatusEffectInstance::GetStatusEffect() const {
    return m_pStatusEffect;
}

inline Bool StatusEffectInstance::IsBuff() const {
    return m_pStatusEffect->IsBuff();
}
inline Bool StatusEffectInstance::IsDebuff() const {
    return m_pStatusEffect->IsDebuff();
}
inline Bool StatusEffectInstance::IsDisabling() const {
    return m_pStatusEffect->IsDisabling();
}

inline Bool StatusEffectInstance::IsStackable() const {
    return m_pStatusEffect->IsStackable();
}
inline Bool StatusEffectInstance::IsRemovable() const {
    return m_bRemovable;
}

inline UInt StatusEffectInstance::GetMaxStacks() const {
}
inline UInt StatusEffectInstance::GetStackCount() const {
}

inline Bool StatusEffectInstance::IsExpired( UInt iStack ) const {
    Assert( iStack < m_iStackCount );
    return ( m_arrDurations[iStack] == 0 );
}
inline UInt StatusEffectInstance::GetDuration( UInt iStack ) const {
    Assert( iStack < m_iStackCount );
    return m_arrDurations[iStack];
}

inline Void StatusEffectInstance::IncreaseDuration( UInt iStack, UInt iAmount ) {
    Assert( iStack < m_iStackCount );
    m_arrDurations[iStack] += iAmount;
}
inline Void StatusEffectInstance::DecreaseDuration( UInt iStack, UInt iAmount ) {
    Assert( iStack < m_iStackCount );
    if ( m_arrDurations[iStack] > iAmount )
        m_arrDurations[iStack] -= iAmount;
    else
        m_arrDurations[iStack] = 0;
}
inline Void StatusEffectInstance::SetDuration( UInt iStack, UInt iAmount ) {
    Assert( iStack < m_iStackCount );
}

inline Void StatusEffectInstance::OnUpdateBattleStats( MonsterBattleInstance * pHost ) const {
    m_pStatusEffect->OnUpdateBattleStats( pHost );
}

inline Void StatusEffectInstance::OnTurnStart( MonsterBattleInstance * pHost ) const {
    m_pStatusEffect->OnTurnStart( pHost );
}
inline Void StatusEffectInstance::OnTurnEnd( MonsterBattleInstance * pHost ) const {
    m_pStatusEffect->OnTurnEnd( pHost );
}

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectSet implementation
inline Bool StatusEffectSet::HasStatusEffect( StatusEffectType iType ) const {
    Assert( iType < STATUSEFFECT_COUNT );
    return ( m_arrEffects[iType].GetStackCount() > 0 );
}

inline StatusEffectInstance * StatusEffectSet::GetStatusEffect( StatusEffectType iType ) {
    Assert( iType < STATUSEFFECT_COUNT );
    return ( m_arrEffects + iType );
}


