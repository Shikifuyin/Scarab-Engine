/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingMonsterStorage.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingMonsterStorage interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BuildingMonsterStorage implementation
inline BuildingType BuildingMonsterStorage::GetType() const {
    return BUILDING_MONSTER_STORAGE;
}

inline UInt BuildingMonsterStorage::GetStorageLevel() const {
    return m_iStorageLevel;
}
inline UInt BuildingMonsterStorage::GetStorageRoom() const {
    return m_iStorageRoom;
}

inline UInt BuildingMonsterStorage::GetMonsterCount() const {
    return m_arrMonsterStorage.Count();
}
inline MonsterInstance * BuildingMonsterStorage::GetMonster( UInt iStorageIndex ) {
    Assert( iStorageIndex < m_arrMonsterStorage.Count() );
    return &( m_arrMonsterStorage[iStorageIndex] );
}
