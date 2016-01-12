/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingRuneStorage.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingRuneStorage interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BuildingRuneStorage implementation
inline BuildingType BuildingRuneStorage::GetType() const {
    return BUILDING_RUNE_STORAGE;
}

inline UInt BuildingRuneStorage::GetStorageLevel() const {
    return m_iStorageLevel;
}
inline UInt BuildingRuneStorage::GetStorageRoom() const {
    return m_iStorageRoom;
}

inline UInt BuildingRuneStorage::GetRuneCount( RuneType iType ) const {
    Assert( iType < RUNE_TYPE_COUNT );
    return m_arrRuneStorage[iType].Count();
}
inline Rune * BuildingRuneStorage::GetRune( RuneType iType, UInt iStorageIndex ) {
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iStorageIndex < m_arrRuneStorage[iType].Count() );
    return &( (m_arrRuneStorage[iType])[iStorageIndex] );
}

