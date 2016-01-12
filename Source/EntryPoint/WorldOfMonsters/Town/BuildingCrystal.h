/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingCrystal.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGCRYSTAL_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGCRYSTAL_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BuildingDungeon.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The BuildingCrystal class
class BuildingCrystal : public Building
{
public:
    BuildingCrystal( BuildingDungeon * pDungeon );
    virtual ~BuildingCrystal();

    // Type
    inline virtual BuildingType GetType() const;

    // Crystal production
    inline UInt GetProductionRateLevel() const;
    inline UInt GetProductionRate() const;
    Bool UpgradeProductionRate();

    inline UInt GetCapacityLevel() const;
    inline UInt GetCapacity() const;
    Bool UpgradeCapacity();

    inline Bool IsEmpty() const;
    inline UInt GetCrystalBuffer() const;

    Void RetrieveCrystals();
    Void UpdateCrystals();

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Crystal production
    UInt m_iProductionRateLevel;
    UInt m_iProductionRate; // in crystals / hour

    UInt m_iCapacityLevel;
    UInt m_iCapacity;

    UInt m_iCrystalBuffer;
    TimeValue m_hLastUpdateTime;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BuildingCrystal.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGCRYSTAL_H

