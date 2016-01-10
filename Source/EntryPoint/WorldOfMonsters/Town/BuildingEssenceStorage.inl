/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingEssenceStorage.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingEssenceStorage interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BuildingEssenceStorage implementation
inline UInt BuildingEssenceStorage::GetEssenceCount( MonsterElement iElement, EssenceType iType ) const {
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_TYPE_COUNT );
    return m_arrEssences[iElement][iType];
}

inline Void BuildingEssenceStorage::AddEssences( MonsterElement iElement, EssenceType iType, UInt iCount ) {
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_TYPE_COUNT );
    m_arrEssences[iElement][iType] += iCount;
}
inline Void BuildingEssenceStorage::RemoveEssences( MonsterElement iElement, EssenceType iType, UInt iCount ) {
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_TYPE_COUNT );
    Assert( m_arrEssences[iElement][iType] >= iCount )
        m_arrEssences[iElement][iType] -= iCount;
}

inline Bool BuildingEssenceStorage::AwakeningUnlocked() const {
    return m_bAwakeningUnlocked;
}
inline Bool BuildingEssenceStorage::FusionUnlocked() const {
    return m_bFusionUnlocked;
}


