/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Rune.inl
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
// RuneLevelingStats implementation
inline Bool RuneLevelingStats::IsSlotAllowed( UInt iSlot, MonsterStatistic iStat, Bool bRatio ) {
    Assert( iStat < MONSTER_STAT_COUNT );
    Byte iMask = (Byte)( 1 << iSlot );
    return ( (sm_arrAllowedSlots[bRatio ? 1 : 0][iStat] & iMask) != 0 );
}

inline UInt RuneLevelingStats::GetStatPrimaryI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    switch( iStat ) {
        case MONSTER_STAT_HEALTH:  return m_arrHealthI[0][iRank][iLevel]; break;
        case MONSTER_STAT_ATTACK:  return m_arrAttackI[0][iRank][iLevel]; break;
        case MONSTER_STAT_DEFENSE: return m_arrDefenseI[0][iRank][iLevel]; break;
        case MONSTER_STAT_SPEED:   return m_arrSpeed[0][iRank][iLevel]; break;
        default: Assert( false ); return 0; break;
    }
}
inline Float RuneLevelingStats::GetStatPrimaryF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    switch( iStat ) {
        case MONSTER_STAT_HEALTH:     return m_arrHealthF[0][iRank][iLevel]; break;
        case MONSTER_STAT_ATTACK:     return m_arrAttackF[0][iRank][iLevel]; break;
        case MONSTER_STAT_DEFENSE:    return m_arrDefenseF[0][iRank][iLevel]; break;
        case MONSTER_STAT_CRIT_RATE:  return m_arrCritRate[0][iRank][iLevel]; break;
        case MONSTER_STAT_CRIT_DMG:   return m_arrCritDmg[0][iRank][iLevel]; break;
        case MONSTER_STAT_ACCURACY:   return m_arrAccuracy[0][iRank][iLevel]; break;
        case MONSTER_STAT_RESISTANCE: return m_arrResistance[0][iRank][iLevel]; break;
        default: Assert( false ); return 0.0f; break;
    }
}

inline UInt RuneLevelingStats::GetStatSecondaryI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    switch( iStat ) {
        case MONSTER_STAT_HEALTH:  return m_arrHealthI[1][iRank][iLevel]; break;
        case MONSTER_STAT_ATTACK:  return m_arrAttackI[1][iRank][iLevel]; break;
        case MONSTER_STAT_DEFENSE: return m_arrDefenseI[1][iRank][iLevel]; break;
        case MONSTER_STAT_SPEED:   return m_arrSpeed[1][iRank][iLevel]; break;
        default: Assert( false ); return 0; break;
    }
}
inline Float RuneLevelingStats::GetStatSecondaryF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    switch( iStat ) {
        case MONSTER_STAT_HEALTH:     return m_arrHealthF[1][iRank][iLevel]; break;
        case MONSTER_STAT_ATTACK:     return m_arrAttackF[1][iRank][iLevel]; break;
        case MONSTER_STAT_DEFENSE:    return m_arrDefenseF[1][iRank][iLevel]; break;
        case MONSTER_STAT_CRIT_RATE:  return m_arrCritRate[1][iRank][iLevel]; break;
        case MONSTER_STAT_CRIT_DMG:   return m_arrCritDmg[1][iRank][iLevel]; break;
        case MONSTER_STAT_ACCURACY:   return m_arrAccuracy[1][iRank][iLevel]; break;
        case MONSTER_STAT_RESISTANCE: return m_arrResistance[1][iRank][iLevel]; break;
        default: Assert( false ); return 0.0f; break;
    }
}

inline UInt RuneLevelingStats::GetStatSubI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    switch( iStat ) {
        case MONSTER_STAT_HEALTH:  return m_arrHealthI[2][iRank][iLevel]; break;
        case MONSTER_STAT_ATTACK:  return m_arrAttackI[2][iRank][iLevel]; break;
        case MONSTER_STAT_DEFENSE: return m_arrDefenseI[2][iRank][iLevel]; break;
        case MONSTER_STAT_SPEED:   return m_arrSpeed[2][iRank][iLevel]; break;
        default: Assert( false ); return 0; break;
    }
}
inline Float RuneLevelingStats::GetStatSubF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    switch( iStat ) {
        case MONSTER_STAT_HEALTH:     return m_arrHealthF[2][iRank][iLevel]; break;
        case MONSTER_STAT_ATTACK:     return m_arrAttackF[2][iRank][iLevel]; break;
        case MONSTER_STAT_DEFENSE:    return m_arrDefenseF[2][iRank][iLevel]; break;
        case MONSTER_STAT_CRIT_RATE:  return m_arrCritRate[2][iRank][iLevel]; break;
        case MONSTER_STAT_CRIT_DMG:   return m_arrCritDmg[2][iRank][iLevel]; break;
        case MONSTER_STAT_ACCURACY:   return m_arrAccuracy[2][iRank][iLevel]; break;
        case MONSTER_STAT_RESISTANCE: return m_arrResistance[2][iRank][iLevel]; break;
        default: Assert( false ); return 0.0f; break;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// Rune implementation
inline RuneType Rune::GetType() const {
    return m_iType;
}
inline UInt Rune::GetSlot() const {
    return m_iSlot;
}

inline Bool Rune::IsEquipped() const {
    return m_bEquipped;
}
inline Void Rune::SetEquipped( Bool bEquipped ) {
    m_bEquipped = bEquipped;
}

inline UInt Rune::GetRank() const {
    return m_iRank;
}

inline UInt Rune::GetMaxLevel() const {
    return RUNE_MAX_LEVELBYRANK(m_iRank);
}
inline UInt Rune::GetLevel() const {
    return m_iLevel;
}
inline RuneQuality Rune::GetQuality() const {
    return m_iQuality;
}

inline UInt Rune::GetBonusCount() const {
    return m_iBonusCount;
}

inline MonsterStatistic Rune::GetBonusType( RuneStat iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    return m_arrBonusTypes[iStat];
}
inline Bool Rune::IsBonusRatio( RuneStat iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    return m_arrIsBonusRatio[iStat];
}
inline UInt Rune::GetBonusValueI( RuneStat iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    Assert( !(m_arrIsBonusRatio[iStat]) );
    return m_arrBonusValues[iStat].iValue;
}
inline Float Rune::GetBonusValueF( RuneStat iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    Assert( m_arrIsBonusRatio[iStat] );
    return m_arrBonusValues[iStat].fValue;
}

/////////////////////////////////////////////////////////////////////////////////
// RuneSet implementation
inline Bool RuneSet::HasRune( UInt iSlot ) const {
    Assert( iSlot < RUNE_SLOT_COUNT );
    return ( m_arrRunes[iSlot] != NULL );
}

inline Rune * RuneSet::GetRune( UInt iSlot ) const {
    Assert( iSlot < RUNE_SLOT_COUNT );
    return m_arrRunes[iSlot];
}
inline Void RuneSet::SetRune( UInt iSlot, Rune * pRune ) {
    Assert( iSlot < RUNE_SLOT_COUNT );
    m_arrRunes[iSlot] = pRune;
    _UpdateSetBonuses();
}

inline UInt RuneSet::GetSetBonusCount() const {
    return m_iSetBonusCount;
}
inline RuneType RuneSet::GetSetBonus( UInt iIndex ) const {
    Assert( iIndex < m_iSetBonusCount );
    return m_arrSetBonuses[iIndex];
}
