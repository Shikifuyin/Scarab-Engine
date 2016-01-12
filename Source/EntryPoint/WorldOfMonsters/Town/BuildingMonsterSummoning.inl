/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingMonsterSummoning.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingMonsterSummoning interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BuildingMonsterSummoning implementation
inline BuildingType BuildingMonsterSummoning::GetType() const {
    return BUILDING_MONSTER_SUMMONING;
}

inline UInt BuildingMonsterSummoning::GetScrollCount( ScrollType iType ) const {
    Assert( iType < SCROLL_TYPE_COUNT );
    return m_arrScrolls[iType];
}

inline Void BuildingMonsterSummoning::AddScrolls( ScrollType iType, UInt iCount ) {
    Assert( iType < SCROLL_TYPE_COUNT );
    m_arrScrolls[iType] += iCount;
}
inline Void BuildingMonsterSummoning::RemoveScrolls( ScrollType iType, UInt iCount ) {
    Assert( iType < SCROLL_TYPE_COUNT );
    Assert( m_arrScrolls[iType] >= iCount );
    m_arrScrolls[iType] -= iCount;
}


