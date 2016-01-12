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
// Rune implementation
inline Bool Rune::IsNull() const {
    return ( m_iType == RUNE_TYPE_COUNT );
}
inline Bool Rune::IsPresent() const {
    return ( m_iType != RUNE_TYPE_COUNT );
}

inline RuneType Rune::GetType() const {
    return m_iType;
}
inline UInt Rune::GetSlot() const {
    return m_iSlot;
}

inline UInt Rune::GetRank() const {
    return m_iRank;
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

inline MonsterStatistic Rune::GetBonusType( RuneStatistic iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    return m_arrBonusTypes[iStat];
}
inline Bool Rune::IsBonusRatio( RuneStatistic iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    return m_arrIsBonusRatio[iStat];
}
inline UInt Rune::GetBonusValueI( RuneStatistic iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    Assert( !(m_arrIsBonusRatio[iStat]) );
    return m_arrBonusValues[iStat].iValue;
}
inline Float Rune::GetBonusValueF( RuneStatistic iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    Assert( m_arrIsBonusRatio[iStat] );
    return m_arrBonusValues[iStat].fValue;
}

inline Bool Rune::IsEquipped() const {
    return ( m_pEquippedTo != NULL );
}
inline Void Rune::SetEquipped( MonsterInstance * pEquippedTo ) {
    m_pEquippedTo = pEquippedTo;
}

/////////////////////////////////////////////////////////////////////////////////
// RuneSet implementation
inline Bool RuneSet::HasRune( UInt iSlot ) const {
    Assert( iSlot < RUNE_SLOT_COUNT );
    return ( m_arrRunes[iSlot] != NULL );
}
inline Rune * RuneSet::GetRune( UInt iSlot ) {
    Assert( iSlot < RUNE_SLOT_COUNT );
    return m_arrRunes[iSlot];
}

inline UInt RuneSet::GetSetBonusCount() const {
    return m_iSetBonusCount;
}
inline RuneType RuneSet::GetSetBonus( UInt iIndex ) const {
    Assert( iIndex < m_iSetBonusCount );
    return m_arrSetBonuses[iIndex];
}
