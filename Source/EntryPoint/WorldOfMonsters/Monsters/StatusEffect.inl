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

inline Bool StatusEffect::IsRemovable() const {
    return m_bRemovable;
}
inline Float StatusEffect::GetAmplitude() const {
    return m_fAmplitude;
}

inline Void StatusEffect::SetRemovable( Bool bRemovable ) {
    m_bRemovable = bRemovable;
}
inline Void StatusEffect::SetAmplitude( Float fAmplitude ) {
    m_fAmplitude = fAmplitude;
}

inline Bool StatusEffect::IsStackable() const {
    return sm_arrIsStackable[m_iType];
}
inline UInt StatusEffect::GetMaxStacks() const {
    return m_iMaxStacks;
}
inline UInt StatusEffect::GetStackCount() const {
    return m_iStackCount;
}

inline Void StatusEffect::SetMaxStacks( UInt iMaxStacks ) {
    Assert( iMaxStacks <= STATUSEFFECT_MAX_STACKS );
    m_iMaxStacks = iMaxStacks;
    if ( m_iStackCount > m_iMaxStacks )
        m_iStackCount = m_iMaxStacks;
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

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectSet implementation
inline Bool StatusEffectSet::HasStatusEffect( StatusEffectType iType ) const {
    Assert( iType < STATUSEFFECT_COUNT );
    return m_arrEffects[iType].IsPresent();
}

inline StatusEffect * StatusEffectSet::GetStatusEffect( StatusEffectType iType ) {
    Assert( iType < STATUSEFFECT_COUNT );
    if ( m_arrEffects[iType].IsNull() )
        return NULL;
    return ( m_arrEffects + iType );
}


