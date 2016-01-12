/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingEssenceStorage.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGESSENCESTORAGE_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGESSENCESTORAGE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BuildingDungeon.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The BuildingEssenceStorage class
class BuildingEssenceStorage : public Building
{
public:
    BuildingEssenceStorage( BuildingDungeon * pDungeon );
    virtual ~BuildingEssenceStorage();

    // Type
    inline virtual BuildingType GetType() const;

    // Essence storage
    inline UInt GetEssenceCount( MonsterElement iElement, EssenceType iType ) const;

    inline Void AddEssences( MonsterElement iElement, EssenceType iType, UInt iCount );
    inline Void RemoveEssences( MonsterElement iElement, EssenceType iType, UInt iCount );

    Bool CheckEssenceCost( const EssenceCost * pCost ) const;
    Void PayEssenceCost( const EssenceCost * pCost );

    // Fusion & Awakening
    inline Bool AwakeningUnlocked() const;
    inline Bool FusionUnlocked() const;

    Bool UnlockAwakening();
    Bool UnlockFusion();

    Bool CanAwake( UInt iTargetMonster ) const;
    Void Awake( UInt iTargetMonster );

    Bool CanFuse( MonsterElement iElement, EssenceType iType ) const;
    Void Fuse( MonsterElement iElement, EssenceType iType );

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Essence storage
    UInt m_arrEssences[MONSTER_ELEMENT_COUNT][ESSENCE_TYPE_COUNT];

    // Fusion & Awakening
    Bool m_bAwakeningUnlocked;
    Bool m_bFusionUnlocked;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BuildingEssenceStorage.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDINGESSENCESTORAGE_H

