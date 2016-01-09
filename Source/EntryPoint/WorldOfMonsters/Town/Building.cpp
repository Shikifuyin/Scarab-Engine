/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/Building.cpp
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
// Includes
#include "Building.h"

/////////////////////////////////////////////////////////////////////////////////
// Building implementation
Building::Building( BuildingType iType, BuildingCostType iCostType, UInt iCost )
{
    m_iType = iType;

    m_iCostType = iCostType;
    m_iCost = iCost;
}
Building::~Building()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// BuildingArcaneTower implementation
BuildingArcaneTower::BuildingArcaneTower( UInt iInstanceIndex ):
    Building( BUILDING_ARCANE_TOWER )
{
    // Cost
    m_iCostType = BUILDING_COST_MANA;
    m_iCost = 500 * (1 << iInstanceIndex);

    // Index
    m_iInstanceIndex = iInstanceIndex;
}
BuildingArcaneTower::~BuildingArcaneTower()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// BuildingEssenceStorage implementation
BuildingEssenceStorage::BuildingEssenceStorage():
    Building( BUILDING_ESSENCE_STORAGE )
{
    // No cost, built on start
    m_iCostType = BUILDING_COST_MANA;
    m_iCost = 1;

    // Essence storage
    for( UInt i = 0; i < MONSTER_ELEMENT_COUNT; ++i ) {
        for( UInt j = 0; j < ESSENCE_TYPE_COUNT; ++j )
            m_arrEssences[i][j] = 0;
    }
}
BuildingEssenceStorage::~BuildingEssenceStorage()
{
    // nothing to do
}

Bool BuildingEssenceStorage::CheckAwakeningCost( MonsterElement iElement, UInt arrCosts[MONSTER_AWAKENING_COST_COUNT] ) const
{
    Assert( iElement != MONSTER_ELEMENT_MAGIC );
    Assert( iElement < MONSTER_ELEMENT_COUNT );

    UInt iMagicCMN = arrCosts[MONSTER_AWAKENING_MAGIC_CMN];
    if ( iMagicCMN & 0x80000000 ) {
        iMagicCMN &= 0x7fffffff;
        if ( iMagicCMN > m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_MID] )
            return false;
    } else {
        if ( iMagicCMN > m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_LOW] )
            return false;
    }

    UInt iMagicRAR = arrCosts[MONSTER_AWAKENING_MAGIC_RAR];
    if ( iMagicRAR & 0x80000000 ) {
        iMagicRAR &= 0x7fffffff;
        if ( iMagicRAR > m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_HIGH] )
            return false;
    } else {
        if ( iMagicRAR > m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_MID] )
            return false;
    }

    UInt iElementCMN = arrCosts[MONSTER_AWAKENING_ELEMENTAL_CMN];
    if ( iElementCMN & 0x80000000 ) {
        iElementCMN &= 0x7fffffff;
        if ( iElementCMN > m_arrEssences[iElement][ESSENCE_MID] )
            return false;
    } else {
        if ( iElementCMN > m_arrEssences[iElement][ESSENCE_LOW] )
            return false;
    }

    UInt iElementRAR = arrCosts[MONSTER_AWAKENING_ELEMENTAL_RAR];
    if ( iElementRAR & 0x80000000 ) {
        iElementRAR &= 0x7fffffff;
        if ( iElementRAR > m_arrEssences[iElement][ESSENCE_HIGH] )
            return false;
    } else {
        if ( iElementRAR > m_arrEssences[iElement][ESSENCE_MID] )
            return false;
    }

    return true;
}
Void BuildingEssenceStorage::PayAwakeningCost( MonsterElement iElement, UInt arrCosts[MONSTER_AWAKENING_COST_COUNT] )
{
    Assert( iElement != MONSTER_ELEMENT_MAGIC );
    Assert( iElement < MONSTER_ELEMENT_COUNT );

    UInt iMagicCMN = arrCosts[MONSTER_AWAKENING_MAGIC_CMN];
    if ( iMagicCMN & 0x80000000 ) {
        iMagicCMN &= 0x7fffffff;
        m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_MID] -= iMagicCMN;
    } else {
        m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_LOW] -= iMagicCMN;
    }

    UInt iMagicRAR = arrCosts[MONSTER_AWAKENING_MAGIC_RAR];
    if ( iMagicRAR & 0x80000000 ) {
        iMagicRAR &= 0x7fffffff;
        m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_HIGH] -= iMagicRAR;
    } else {
        m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_MID] -= iMagicRAR;
    }

    UInt iElementCMN = arrCosts[MONSTER_AWAKENING_ELEMENTAL_CMN];
    if ( iElementCMN & 0x80000000 ) {
        iElementCMN &= 0x7fffffff;
        m_arrEssences[iElement][ESSENCE_MID] -= iElementCMN;
    } else {
        m_arrEssences[iElement][ESSENCE_LOW] -= iElementCMN;
    }

    UInt iElementRAR = arrCosts[MONSTER_AWAKENING_ELEMENTAL_RAR];
    if ( iElementRAR & 0x80000000 ) {
        iElementRAR &= 0x7fffffff;
        m_arrEssences[iElement][ESSENCE_HIGH] -= iElementRAR;
    } else {
        m_arrEssences[iElement][ESSENCE_MID] -= iElementRAR;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// BuildingEssenceFusion implementation
BuildingEssenceFusion::BuildingEssenceFusion( BuildingEssenceStorage * pEssenceStorage ):
    Building( BUILDING_ESSENCE_FUSION )
{
    // Cost
    m_iCostType = BUILDING_COST_MANA;
    m_iCost = 100000;

    // Essence fusion
    m_pEssenceStorage = pEssenceStorage;
}
BuildingEssenceFusion::~BuildingEssenceFusion()
{
    // nothing to do
}

Bool BuildingEssenceFusion::CanFuse( MonsterElement iElement, EssenceType iType ) const
{
    Assert( iType < ESSENCE_HIGH );
    return ( m_pEssenceStorage->GetEssenceCount(iElement, iType) >= 10 );
}
Void BuildingEssenceFusion::Fuse( MonsterElement iElement, EssenceType iType ) const
{
    Assert( iType < ESSENCE_HIGH );
    m_pEssenceStorage->RemoveEssences( iElement, iType, 10 );
    m_pEssenceStorage->AddEssences( iElement, (EssenceType)(iType+1), 1 );
}

