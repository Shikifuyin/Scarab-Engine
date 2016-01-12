/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingMana.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingMana interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BuildingMana implementation
inline BuildingType BuildingMana::GetType() const {
    return BUILDING_MANA;
}

inline UInt BuildingMana::GetProductionRateLevel() const {
    return m_iProductionRateLevel;
}
inline UInt BuildingMana::GetProductionRate() const {
    return m_iProductionRate;
}

inline UInt BuildingMana::GetCapacityLevel() const {
    return m_iCapacityLevel;
}
inline UInt BuildingMana::GetCapacity() const {
    return m_iCapacity;
}

inline Bool BuildingMana::IsEmpty() const {
    return ( m_iCapacity == 0 );
}
inline UInt BuildingMana::GetManaBuffer() const {
    return m_iManaBuffer;
}
