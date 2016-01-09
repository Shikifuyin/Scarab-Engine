/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/Building.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : Base Building interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Building implementation
inline BuildingType Building::GetType() const {
    return m_iType;
}

inline const GChar * Building::GetName() const {
    return sm_arrNames[m_iType];
}
inline const GChar * Building::GetInfoText() const {
    return sm_arrInfoTexts[m_iType];
}

inline Bool Building::IsManaCost() const {
    return ( m_iCostType == BUILDING_COST_MANA );
}
inline Bool Building::IsCrystalCost() const {
    return ( m_iCostType == BUILDING_COST_CRYSTAL );
}
inline Bool Building::IsGloryCost() const {
    return ( m_iCostType == BUILDING_COST_GLORY );
}

inline BuildingCostType Building::GetCostType() const {
    return m_iCostType;
}
inline UInt Building::GetCost() const {
    return m_iCost;
}



/////////////////////////////////////////////////////////////////////////////////
// BuildingArcaneTower implementation

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
