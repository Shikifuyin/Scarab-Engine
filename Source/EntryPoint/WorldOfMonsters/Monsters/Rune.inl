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
inline Bool Rune::IsMaxRank() const {
    return ( m_iRank == RUNE_MAX_RANK - 1 );
}
inline Void Rune::RankUp() {
    if ( m_iRank < (RUNE_MAX_RANK - 1) ) {
        ++m_iRank;
        m_iLevel = 0;
    }
}
inline Void Rune::RankDown() {
    if ( m_iRank > 0 ) {
        --m_iRank;
        m_iLevel = 0;
    }
}
inline Void Rune::SetRank( UInt iRank ) {
    Assert( iRank < RUNE_MAX_RANK );
    m_iRank = iRank;
    m_iLevel = 0;
}

inline UInt Rune::GetLevel() const {
    return m_iLevel;
}
inline Bool Rune::IsMaxLevel() const {
    return ( m_iLevel == RUNE_MAX_LEVEL - 1 );
}
inline Void Rune::LevelUp() {
    if ( m_iLevel < (RUNE_MAX_LEVEL - 1) )
        ++m_iLevel;
}
inline Void Rune::LevelDown() {
    if ( m_iLevel > 0 )
        --m_iLevel;
}
inline Void Rune::SetLevel( UInt iLevel ) {
    Assert( iLevel < RUNE_MAX_LEVEL );
    m_iLevel = iLevel;
}

inline Bool Rune::HasBonus( RuneStatistic iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    return ( m_arrBonuses[iStat].iStat != MONSTER_STAT_COUNT );
}
inline MonsterStatistic Rune::GetBonusType( RuneStatistic iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    return m_arrBonuses[iStat].iStat;
}
inline Bool Rune::IsBonusRatio( RuneStatistic iStat ) const {
    Assert( iStat < RUNE_STAT_COUNT );
    return m_arrBonuses[iStat].bIsRatio;
}

/////////////////////////////////////////////////////////////////////////////////
// RuneSet implementation
inline Bool RuneSet::HasRune( UInt iSlot ) const {
    Assert( iSlot < RUNE_SLOT_COUNT );
    return m_arrRunes[iSlot].IsPresent();
}
inline const Rune * RuneSet::GetRune( UInt iSlot ) const {
    Assert( iSlot < RUNE_SLOT_COUNT );
    Assert( m_arrRunes[iSlot].IsPresent() );
    return ( m_arrRunes + iSlot );
}
inline Rune * RuneSet::GetRune( UInt iSlot ) {
    Assert( iSlot < RUNE_SLOT_COUNT );
    Assert( m_arrRunes[iSlot].IsPresent() );
    return ( m_arrRunes + iSlot );
}
