/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingDungeon.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingDungeon interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BuildingDungeon implementation
inline UInt BuildingDungeon::GetCurrency( CurrencyType iType ) const {
    Assert( iType < CURRENCY_COUNT );
    return m_arrCurrencies[iType];
}

inline Void BuildingDungeon::AddCurrency( CurrencyType iType, UInt iAmount ) {
    Assert( iType < CURRENCY_COUNT );
    m_arrCurrencies[iType] += iAmount;
}
inline Void BuildingDungeon::RemoveCurrency( CurrencyType iType, UInt iAmount ) {
    Assert( iType < CURRENCY_COUNT );
    Assert( m_arrCurrencies[iType] >= iAmount );
    m_arrCurrencies[iType] -= iAmount;
}
inline Void BuildingDungeon::SetCurrency( CurrencyType iType, UInt iAmount ) {
    Assert( iType < CURRENCY_COUNT );
    m_arrCurrencies[iType] = iAmount;
}

inline UInt BuildingDungeon::GetMonsterCollectionLevel() const {
    return m_iMonsterCollectionLevel;
}
inline UInt BuildingDungeon::GetMonsterCollectionRoom() const {
    return m_iMonsterCollectionRoom;
}

inline UInt BuildingDungeon::GetMonsterCount() const {
    return m_arrMonsterCollection.Count();
}
inline MonsterInstance * BuildingDungeon::GetMonster( UInt iIndex ) const {
    Assert( iIndex < m_arrMonsterCollection.Count() );
    return m_arrMonsterCollection[iIndex];
}

inline UInt BuildingDungeon::GetRuneCollectionLevel() const {
    return m_iRuneCollectionLevel;
}
inline UInt BuildingDungeon::GetRuneCollectionRoom() const {
    return m_iRuneCollectionRoom;
}

inline UInt BuildingDungeon::GetRuneCount( RuneType iType ) const {
    Assert( iType < RUNE_TYPE_COUNT );
    return m_arrRuneCollection[iType].Count();
}
inline Rune * BuildingDungeon::GetRune( RuneType iType, UInt iIndex ) const {
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iIndex < m_arrRuneCollection[iType].Count() );
    return (m_arrRuneCollection[iType])[iIndex];
}

inline UInt BuildingDungeon::GetArenaScore() const {
    return m_iArenaScore;
}
inline ArenaRank BuildingDungeon::GetArenaRank() const {
    return m_iArenaRank;
}

inline Void BuildingDungeon::SetArenaScore( UInt iArenaScore ) {
    m_iArenaScore = iArenaScore;
}
inline Void BuildingDungeon::SetArenaRank( ArenaRank iRank ) {
    Assert( iRank < ARENA_RANK_COUNT );
    m_iArenaRank = iRank;
}

inline MonsterInstance * BuildingDungeon::GetArenaDefenseMonster( UInt iIndex ) const {
    Assert( iIndex < BATTLE_TEAMSIZE_ARENA );
    return m_arrArenaDefense[iIndex];
}
inline Void BuildingDungeon::SetArenaDefenseMonster( UInt iIndex, MonsterInstance * pMonsterInstance ) {
    Assert( iIndex < BATTLE_TEAMSIZE_ARENA );
    m_arrArenaDefense[iIndex] = pMonsterInstance;
}

