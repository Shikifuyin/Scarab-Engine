/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingCrystal.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingCrystal interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BuildingCrystal implementation
inline BuildingType BuildingCrystal::GetType() const {
    return BUILDING_CRYSTAL;
}

inline UInt BuildingCrystal::GetProductionRateLevel() const {
    return m_iProductionRateLevel;
}
inline UInt BuildingCrystal::GetProductionRate() const {
    return m_iProductionRate;
}

inline UInt BuildingCrystal::GetCapacityLevel() const {
    return m_iCapacityLevel;
}
inline UInt BuildingCrystal::GetCapacity() const {
    return m_iCapacity;
}

inline Bool BuildingCrystal::IsEmpty() const {
    return ( m_iCapacity == 0 );
}
inline UInt BuildingCrystal::GetCrystalBuffer() const {
    return m_iCrystalBuffer;
}
