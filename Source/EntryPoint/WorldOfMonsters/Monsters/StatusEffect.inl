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
    return ( m_iType < STATUSEFFECT_DEBUFF_STAT );
}
inline Bool StatusEffect::IsDebuff() const {
    return ( m_iType >= STATUSEFFECT_DEBUFF_STAT );
}
inline Bool StatusEffect::IsDisabling() const {
    return ( m_iType >= STATUSEFFECT_DEBUFF_STUN );
}

inline Bool StatusEffect::IsStackable() const {
    return sm_arrIsStackable[m_iType];
}

/////////////////////////////////////////////////////////////////////////////////
// StatusBuffStat implementation
inline MonsterStatistic StatusBuffStat::GetStat() const {
    return m_iStat;
}
inline Float StatusBuffStat::GetAmount() const {
    return m_fAmount;
}

/////////////////////////////////////////////////////////////////////////////////
// StatusDebuffStat implementation
inline MonsterStatistic StatusDebuffStat::GetStat() const {
    return m_iStat;
}
inline Float StatusDebuffStat::GetAmount() const {
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

inline Bool StatusEffectInstance::IsExpired() const {
    return ( m_iDuration == 0 );
}
inline UInt StatusEffectInstance::GetDuration() const {
    return m_iDuration;
}

inline Void StatusEffectInstance::DecreaseDuration() {
    if ( m_iDuration > 0 )
        --m_iDuration;
}

inline Void StatusEffectInstance::OnUpdateBattleStats( MonsterBattleInstance * pHost ) const {
    m_pStatusEffect->OnUpdateBattleStats( pHost );
}

inline Void StatusEffectInstance::OnTurnStart( MonsterBattleInstance * pHost ) const {
    m_pStatusEffect->OnTurnStart( pHost );
}
