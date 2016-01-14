/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/PlayerTown.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : Base PlayerTown interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// PlayerTown implementation
inline const GChar * PlayerTown::GetName() const {
    return m_strName;
}

inline UInt PlayerTown::GetCurrency( CurrencyType iType ) const {
    Assert( iType < CURRENCY_COUNT );
    return m_arrCurrencies[iType];
}
inline UInt PlayerTown::GetScrollCount( ScrollType iType ) const {
    Assert( iType < SCROLL_TYPE_COUNT );
    return m_arrScrolls[iType];
}
inline UInt PlayerTown::GetEssenceCount( MonsterElement iElement, EssenceType iType ) const {
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_TYPE_COUNT );
    return m_arrEssences[iElement][iType];
}

inline Void PlayerTown::AddCurrency( CurrencyType iType, UInt iAmount ) {
    Assert( iType < CURRENCY_COUNT );
    m_arrCurrencies[iType] += iAmount;
}
inline Void PlayerTown::AddScrolls( ScrollType iType, UInt iCount ) {
    Assert( iType < SCROLL_TYPE_COUNT );
    m_arrScrolls[iType] += iCount;
}
inline Void PlayerTown::AddEssences( MonsterElement iElement, EssenceType iType, UInt iCount ) {
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_TYPE_COUNT );
    m_arrEssences[iElement][iType] += iCount;
}

inline Void PlayerTown::RemoveCurrency( CurrencyType iType, UInt iAmount ) {
    Assert( iType < CURRENCY_COUNT );
    Assert( m_arrCurrencies[iType] >= iAmount );
    m_arrCurrencies[iType] -= iAmount;
}
inline Void PlayerTown::RemoveScrolls( ScrollType iType, UInt iCount ) {
    Assert( iType < SCROLL_TYPE_COUNT );
    Assert( m_arrScrolls[iType] >= iCount );
    m_arrScrolls[iType] -= iCount;
}
inline Void PlayerTown::RemoveEssences( MonsterElement iElement, EssenceType iType, UInt iCount ) {
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_TYPE_COUNT );
    Assert( m_arrEssences[iElement][iType] >= iCount );
    m_arrEssences[iElement][iType] -= iCount;
}

inline UInt PlayerTown::GetMonsterCollectionLevel() const {
    return m_iMonsterCollectionLevel;
}
inline UInt PlayerTown::GetMonsterStorageLevel() const {
    return m_iMonsterStorageLevel;
}
inline UInt PlayerTown::GetMonsterCollectionRoom() const {
    return m_iMonsterCollectionRoom;
}
inline UInt PlayerTown::GetMonsterStorageRoom() const {
    return m_iMonsterStorageRoom;
}

inline UInt PlayerTown::GetMonsterCount() const {
    return m_arrMonsterCollection.Count();
}
inline UInt PlayerTown::GetStoredMonsterCount() const {
    return m_arrMonsterStorage.Count();
}
inline MonsterInstance * PlayerTown::GetMonster( UInt iIndex ) {
    Assert( iIndex < m_arrMonsterCollection.Count() );
    return &( m_arrMonsterCollection[iIndex] );
}
inline MonsterInstance * PlayerTown::GetStoredMonster( UInt iStorageIndex ) {
    Assert( iStorageIndex < m_arrMonsterStorage.Count() );
    return &( m_arrMonsterStorage[iStorageIndex] );
}

inline UInt PlayerTown::GetRuneCollectionLevel() const {
    return m_iRuneCollectionLevel;
}
inline UInt PlayerTown::GetRuneStorageLevel() const {
    return m_iRuneStorageLevel;
}
inline UInt PlayerTown::GetRuneCollectionRoom() const {
    return m_iRuneCollectionRoom;
}
inline UInt PlayerTown::GetRuneStorageRoom() const {
    return m_iRuneStorageRoom;
}

inline UInt PlayerTown::GetRuneCount( RuneType iType ) const {
    Assert( iType < RUNE_TYPE_COUNT );
    return m_arrRuneCollection[iType].Count();
}
inline UInt PlayerTown::GetStoredRuneCount( RuneType iType ) const {
    Assert( iType < RUNE_TYPE_COUNT );
    return m_arrRuneStorage[iType].Count();
}
inline Rune * PlayerTown::GetRune( RuneType iType, UInt iIndex ) {
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iIndex < m_arrRuneCollection[iType].Count() );
    return &( (m_arrRuneCollection[iType])[iIndex] );
}
inline Rune * PlayerTown::GetStoredRune( RuneType iType, UInt iStorageIndex ) {
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iStorageIndex < m_arrRuneStorage[iType].Count() );
    return &( (m_arrRuneStorage[iType])[iStorageIndex] );
}

inline UInt PlayerTown::GetManaProductionRateLevel() const {
    return m_iManaProductionRateLevel;
}
inline UInt PlayerTown::GetCrystalProductionRateLevel() const {
    return m_iCrystalProductionRateLevel;
}
inline UInt PlayerTown::GetManaProductionRate() const {
    return m_iManaProductionRate;
}
inline UInt PlayerTown::GetCrystalProductionRate() const {
    return m_iCrystalProductionRate;
}

inline UInt PlayerTown::GetManaCapacityLevel() const {
    return m_iManaCapacityLevel;
}
inline UInt PlayerTown::GetCrystalCapacityLevel() const {
    return m_iCrystalCapacityLevel;
}
inline UInt PlayerTown::GetManaCapacity() const {
    return m_iManaCapacity;
}
inline UInt PlayerTown::GetCrystalCapacity() const {
    return m_iCrystalCapacity;
}

inline Bool PlayerTown::IsManaBufferEmpty() const {
    return ( m_iManaCapacity == 0 );
}
inline Bool PlayerTown::IsCrystalBufferEmpty() const {
    return ( m_iCrystalCapacity == 0 );
}
inline UInt PlayerTown::GetManaBuffer() const {
    return m_iManaBuffer;
}
inline UInt PlayerTown::GetCrystalBuffer() const {
    return m_iCrystalBuffer;
}

inline Bool PlayerTown::EssenceFusionUnlocked() const {
    return m_bEssenceFusionUnlocked;
}
inline Bool PlayerTown::MonsterSummoningUnlocked() const {
    return m_bMonsterSummoningUnlocked;
}
inline Bool PlayerTown::MonsterFusionUnlocked() const {
    return m_bMonsterFusionUnlocked;
}
inline Bool PlayerTown::MonsterPowerUpUnlocked() const {
    return m_bMonsterPowerUpUnlocked;
}
inline Bool PlayerTown::MonsterEvolutionUnlocked() const {
    return m_bMonsterEvolutionUnlocked;
}
inline Bool PlayerTown::MonsterAwakeningUnlocked() const {
    return m_bMonsterAwakeningUnlocked;
}
inline Bool PlayerTown::RunePowerUpUnlocked() const {
    return m_bRunePowerUpUnlocked;
}
inline Bool PlayerTown::RuneEvolutionUnlocked() const {
    return m_bRuneEvolutionUnlocked;
}

inline UInt PlayerTown::GetArenaScore() const {
    return m_iArenaScore;
}
inline ArenaRank PlayerTown::GetArenaRank() const {
    return m_iArenaRank;
}

inline Void PlayerTown::SetArenaScore( UInt iArenaScore ) {
    m_iArenaScore = iArenaScore;
}
inline Void PlayerTown::SetArenaRank( ArenaRank iRank ) {
    Assert( iRank < ARENA_RANK_COUNT );
    m_iArenaRank = iRank;
}

inline UInt PlayerTown::GetArenaDefenseMonster( UInt iIndex ) const {
    Assert( iIndex < BATTLE_TEAMSIZE_ARENA );
    return m_arrArenaDefense[iIndex];
}
inline Void PlayerTown::SetArenaDefenseMonster( UInt iIndex, UInt iMonsterCollectionIndex ) {
    Assert( iIndex < BATTLE_TEAMSIZE_ARENA );
    Assert( iMonsterCollectionIndex < m_arrMonsterCollection.Count() );
    m_arrArenaDefense[iIndex] = iMonsterCollectionIndex;
}

inline Bool PlayerTown::HasGuild() const {
    return ( m_pGuild != NULL );
}
inline PlayerGuild * PlayerTown::GetGuild() const {
    return m_pGuild;
}

