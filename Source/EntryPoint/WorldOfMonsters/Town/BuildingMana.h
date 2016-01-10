/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingMana.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGMANA_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGMANA_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BuildingDungeon.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Mana
#define BUILDING_MANA_PRODUCTION_RATE_UPGRADE_COST 300 // in glory points
#define BUILDING_MANA_PRODUCTION_RATE_MAX_LEVEL    10

#define BUILDING_MANA_CAPACITY_UPGRADE_COST 100 // in glory points
#define BUILDING_MANA_CAPACITY_MAX_LEVEL    10

/////////////////////////////////////////////////////////////////////////////////
// The BuildingMana class
class BuildingMana : public Building
{
public:
    BuildingMana( BuildingDungeon * pDungeon );
    virtual ~BuildingMana();

    // Mana production
    inline UInt GetProductionRateLevel() const;
    inline UInt GetProductionRate() const;

    Bool UpgradeProductionRate();

    inline UInt GetCapacityLevel() const;
    inline UInt GetCapacity() const;

    Bool UpgradeCapacity();

    inline Bool IsEmpty() const;
    inline UInt GetManaBuffer() const;

    Void RetrieveMana();
    Void UpdateMana();

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Mana production
    UInt m_iProductionRateLevel;
    UInt m_iProductionRate; // in mana / hour

    UInt m_iCapacityLevel;
    UInt m_iCapacity;

    UInt m_iManaBuffer;
    TimeValue m_hLastUpdateTime;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BuildingMana.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGMANA_H

