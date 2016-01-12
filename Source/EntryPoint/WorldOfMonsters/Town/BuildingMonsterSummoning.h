/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingMonsterSummoning.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGMONSTERSUMMONING_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGMONSTERSUMMONING_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BuildingDungeon.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The BuildingMonsterSummoning class
class BuildingMonsterSummoning : public Building
{
public:
    BuildingMonsterSummoning( BuildingDungeon * pDungeon );
    virtual ~BuildingMonsterSummoning();

    // Type
    inline virtual BuildingType GetType() const;

    // Monster summoning
    inline UInt GetScrollCount( ScrollType iType ) const;

    inline Void AddScrolls( ScrollType iType, UInt iCount );
    inline Void RemoveScrolls( ScrollType iType, UInt iCount );

    Bool CheckScrollCost( const ScrollCost * pCost ) const;
    Void PayScrollCost( const ScrollCost * pCost );

    Bool CanSummon( Monster * pMonster ) const;
    Void Summon( Monster * pMonster );

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Monster summoning
    UInt m_arrScrolls[SCROLL_TYPE_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BuildingMonsterSummoning.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGMONSTERSUMMONING_H



