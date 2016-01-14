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
inline Bool StatusEffect::IsNull() const {
    return ( m_iType == STATUSEFFECT_COUNT );
}
inline Bool StatusEffect::IsPresent() const {
    return ( m_iType != STATUSEFFECT_COUNT );
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

inline StatusEffectType StatusEffect::GetType() const {
    return m_iType;
}

inline Bool StatusEffect::IsStackable() const {
    return ( sm_arrMaxStacks[m_iType] > 1 );
}
inline UInt StatusEffect::GetMaxStacks() const {
    return sm_arrMaxStacks[m_iType];
}
inline UInt StatusEffect::GetStackCount() const {
    return m_iStackCount;
}

inline Bool StatusEffect::IsExpired( UInt iStack ) const {
    Assert( iStack < m_iStackCount );
    return ( m_arrDurations[iStack] == 0 );
}
inline UInt StatusEffect::GetDuration( UInt iStack ) const {
    Assert( iStack < m_iStackCount );
    return m_arrDurations[iStack];
}

inline Void StatusEffect::IncreaseDuration( UInt iStack, UInt iAmount ) {
    Assert( iStack < m_iStackCount );
    m_arrDurations[iStack] += iAmount;
}
inline Void StatusEffect::DecreaseDuration( UInt iStack, UInt iAmount ) {
    Assert( iStack < m_iStackCount );
    if ( m_arrDurations[iStack] > iAmount )
        m_arrDurations[iStack] -= iAmount;
    else
        m_arrDurations[iStack] = 0;
}
inline Void StatusEffect::SetDuration( UInt iStack, UInt iAmount ) {
    Assert( iStack < m_iStackCount );
    m_arrDurations[iStack] = iAmount;
}
inline Void StatusEffect::ResetDuration( UInt iStack ) {
    Assert( iStack < m_iStackCount );
    m_arrDurations[iStack] = 0;
}

inline Bool StatusEffect::IsExhausted( UInt iStack ) const {
    Assert( iStack < m_iStackCount );
    return ( m_arrAmplitudes[iStack] == 0.0f );
}
inline Float StatusEffect::GetAmplitude( UInt iStack ) const {
    Assert( iStack < m_iStackCount );
    return m_arrAmplitudes[iStack];
}

inline Void StatusEffect::IncreaseAmplitude( UInt iStack, Float fAmount ) {
    Assert( iStack < m_iStackCount );
    m_arrAmplitudes[iStack] += fAmount;
}
inline Void StatusEffect::DecreaseAmplitude( UInt iStack, Float fAmount ) {
    Assert( iStack < m_iStackCount );
    if ( m_arrAmplitudes[iStack] > fAmount )
        m_arrAmplitudes[iStack] -= fAmount;
    else
        m_arrAmplitudes[iStack] = 0.0f;
}
inline Void StatusEffect::SetAmplitude( UInt iStack, Float fAmount ) {
    Assert( iStack < m_iStackCount );
    m_arrAmplitudes[iStack] = fAmount;
}
inline Void StatusEffect::ResetAmplitude( UInt iStack ) {
    Assert( iStack < m_iStackCount );
    m_arrAmplitudes[iStack] = 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectSet implementation
inline Bool StatusEffectSet::HasStatusEffect( StatusEffectType iType ) const {
    Assert( iType < STATUSEFFECT_COUNT );
    return m_arrEffects[iType].IsPresent();
}
inline const StatusEffect * StatusEffectSet::GetStatusEffect( StatusEffectType iType ) const {
    Assert( iType < STATUSEFFECT_COUNT );
    if ( m_arrEffects[iType].IsNull() )
        return NULL;
    return ( m_arrEffects + iType );
}
inline StatusEffect * StatusEffectSet::GetStatusEffect( StatusEffectType iType ) {
    Assert( iType < STATUSEFFECT_COUNT );
    if ( m_arrEffects[iType].IsNull() )
        return NULL;
    return ( m_arrEffects + iType );
}


