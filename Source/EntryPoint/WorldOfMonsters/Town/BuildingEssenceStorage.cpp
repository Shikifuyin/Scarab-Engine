/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingEssenceStorage.cpp
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
// Includes
#include "BuildingEssenceStorage.h"

/////////////////////////////////////////////////////////////////////////////////
// BuildingEssenceStorage implementation
BuildingEssenceStorage::BuildingEssenceStorage( BuildingDungeon * pDungeon ):
    Building( BUILDING_ESSENCE_STORAGE, BUILDING_COST_MANA, 1 ) // Built-in
{
    m_pDungeon = pDungeon;

    for( UInt i = 0; i < MONSTER_ELEMENT_COUNT; ++i ) {
        for( UInt j = 0; j < ESSENCE_TYPE_COUNT; ++j )
            m_arrEssences[i][j] = 0;
    }

    m_bAwakeningUnlocked = false;
    m_bFusionUnlocked = false;
}
BuildingEssenceStorage::~BuildingEssenceStorage()
{
    // nothing to do
}

Bool BuildingEssenceStorage::UnlockAwakening()
{
    if ( m_bAwakeningUnlocked )
        return true;

    if ( m_pDungeon->GetCurrency(CURRENCY_MANA) < BUILDING_ESSENCE_STORAGE_UNLOCK_COST_AWAKENING )
        return false;

    m_pDungeon->RemoveCurrency( CURRENCY_MANA, BUILDING_ESSENCE_STORAGE_UNLOCK_COST_AWAKENING );
    m_bAwakeningUnlocked = true;
    return true;
}
Bool BuildingEssenceStorage::UnlockFusion()
{
    if ( m_bFusionUnlocked )
        return true;

    if ( m_pDungeon->GetCurrency(CURRENCY_MANA) < BUILDING_ESSENCE_STORAGE_UNLOCK_COST_FUSION )
        return false;

    m_pDungeon->RemoveCurrency( CURRENCY_MANA, BUILDING_ESSENCE_STORAGE_UNLOCK_COST_FUSION );
    m_bFusionUnlocked = true;
    return true;
}

Bool BuildingEssenceStorage::CanAwake( UInt iTargetMonster ) const
{
    if ( !m_bAwakeningUnlocked )
        return false;

    // Get monster & element
    MonsterInstance * pMonster = m_pDungeon->GetMonster( iTargetMonster );
    Assert( pMonster != NULL );

    MonsterElement iElement = pMonster->GetElement();

    // Check awakening cost
    UInt iCost = pMonster->GetAwakeningCostAmount( MONSTER_AWAKENING_MAGIC_CMN );
    if ( pMonster->IsAwakeningCostHighTier(MONSTER_AWAKENING_MAGIC_CMN) ) {
        if ( iCost > m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_MID] )
            return false;
    } else {
        if ( iCost > m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_LOW] )
            return false;
    }

    iCost = pMonster->GetAwakeningCostAmount( MONSTER_AWAKENING_MAGIC_RAR );
    if ( pMonster->IsAwakeningCostHighTier(MONSTER_AWAKENING_MAGIC_RAR) ) {
        if ( iCost > m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_HIGH] )
            return false;
    } else {
        if ( iCost > m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_MID] )
            return false;
    }

    iCost = pMonster->GetAwakeningCostAmount( MONSTER_AWAKENING_ELEMENTAL_CMN );
    if ( pMonster->IsAwakeningCostHighTier(MONSTER_AWAKENING_ELEMENTAL_CMN) ) {
        if ( iCost > m_arrEssences[iElement][ESSENCE_MID] )
            return false;
    } else {
        if ( iCost > m_arrEssences[iElement][ESSENCE_LOW] )
            return false;
    }

    iCost = pMonster->GetAwakeningCostAmount( MONSTER_AWAKENING_ELEMENTAL_RAR );
    if ( pMonster->IsAwakeningCostHighTier(MONSTER_AWAKENING_ELEMENTAL_RAR) ) {
        if ( iCost > m_arrEssences[iElement][ESSENCE_HIGH] )
            return false;
    } else {
        if ( iCost > m_arrEssences[iElement][ESSENCE_MID] )
            return false;
    }

    // Pass !
    return true;
}
Void BuildingEssenceStorage::Awake( UInt iTargetMonster )
{
    Assert( m_bAwakeningUnlocked );

    // Get monster & element
    MonsterInstance * pMonster = m_pDungeon->GetMonster( iTargetMonster );
    Assert( pMonster != NULL && !(pMonster->IsAwakened()) );

    MonsterElement iElement = pMonster->GetElement();

    // Pay awakening cost
    UInt iCost = pMonster->GetAwakeningCostAmount( MONSTER_AWAKENING_MAGIC_CMN );
    if ( pMonster->IsAwakeningCostHighTier(MONSTER_AWAKENING_MAGIC_CMN) )
        m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_MID] -= iCost;
    else
        m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_LOW] -= iCost;

    iCost = pMonster->GetAwakeningCostAmount( MONSTER_AWAKENING_MAGIC_RAR );
    if ( pMonster->IsAwakeningCostHighTier(MONSTER_AWAKENING_MAGIC_RAR) )
        m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_HIGH] -= iCost;
    else
        m_arrEssences[MONSTER_ELEMENT_MAGIC][ESSENCE_MID] -= iCost;

    iCost = pMonster->GetAwakeningCostAmount( MONSTER_AWAKENING_ELEMENTAL_CMN );
    if ( pMonster->IsAwakeningCostHighTier(MONSTER_AWAKENING_ELEMENTAL_CMN) )
        m_arrEssences[iElement][ESSENCE_MID] -= iCost;
    else
        m_arrEssences[iElement][ESSENCE_LOW] -= iCost;

    iCost = pMonster->GetAwakeningCostAmount( MONSTER_AWAKENING_ELEMENTAL_RAR );
    if ( pMonster->IsAwakeningCostHighTier(MONSTER_AWAKENING_ELEMENTAL_RAR) )
        m_arrEssences[iElement][ESSENCE_HIGH] -= iCost;
    else
        m_arrEssences[iElement][ESSENCE_MID] -= iCost;

    // Awake the monster
    pMonster->Awake();
}

Bool BuildingEssenceStorage::CanFuse( MonsterElement iElement, EssenceType iType ) const
{
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_HIGH );

    if ( !m_bFusionUnlocked )
        return false;

    return ( m_arrEssences[iElement][iType] >= BUILDING_ESSENCE_STORAGE_FUSION_COST );
}
Void BuildingEssenceStorage::Fuse( MonsterElement iElement, EssenceType iType )
{
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_HIGH );
    Assert( m_bFusionUnlocked );

    m_arrEssences[iElement][iType] -= BUILDING_ESSENCE_STORAGE_FUSION_COST;
    m_arrEssences[iElement][iType+1] += 1;
}


