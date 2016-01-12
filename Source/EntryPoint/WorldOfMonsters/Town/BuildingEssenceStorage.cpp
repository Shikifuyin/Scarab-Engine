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

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BuildingEssenceStorage implementation
BuildingEssenceStorage::BuildingEssenceStorage( BuildingDungeon * pDungeon ):
    Building()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Cost
    const CurrencyCost * pCost = pGameParams->GetBuildingCost( BUILDING_ESSENCE_STORAGE );
    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        m_hCost.arrCost[i] = pCost->arrCost[i];

    // Dungeon access
    m_pDungeon = pDungeon;

    // Essence storage
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

Bool BuildingEssenceStorage::CheckEssenceCost( const EssenceCost * pCost ) const
{
    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i ) {
        if ( m_arrEssences[MONSTER_ELEMENT_MAGIC][i] < pCost->arrMagicCost[i] )
            return false;
        if ( m_arrEssences[pCost->iElement][i] < pCost->arrElementalCost[i] )
            return false;
    }
    return true;
}
Void BuildingEssenceStorage::PayEssenceCost( const EssenceCost * pCost )
{
    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i ) {
        Assert( m_arrEssences[MONSTER_ELEMENT_MAGIC][i] >= pCost->arrMagicCost[i] );
        Assert( m_arrEssences[pCost->iElement][i] >= pCost->arrElementalCost[i] );
        m_arrEssences[MONSTER_ELEMENT_MAGIC][i] -= pCost->arrMagicCost[i];
        m_arrEssences[pCost->iElement][i] -= pCost->arrElementalCost[i];
    }
}

Bool BuildingEssenceStorage::UnlockAwakening()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterAwakeningUnlockCost();

    if ( m_bAwakeningUnlocked )
        return false;
    if ( !(m_pDungeon->CheckCurrencyCost(pCost)) )
        return false;

    m_pDungeon->PayCurrencyCost( pCost );

    m_bAwakeningUnlocked = true;
    return true;
}
Bool BuildingEssenceStorage::UnlockFusion()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetEssenceFusionUnlockCost();

    if ( m_bFusionUnlocked )
        return false;
    if ( !(m_pDungeon->CheckCurrencyCost(pCost)) )
        return false;

    m_pDungeon->PayCurrencyCost( pCost );

    m_bFusionUnlocked = true;
    return true;
}

Bool BuildingEssenceStorage::CanAwake( UInt iTargetMonster ) const
{
    if ( !m_bAwakeningUnlocked )
        return false;

    MonsterInstance * pMonster = m_pDungeon->GetMonster( iTargetMonster );
    Assert( pMonster != NULL );

    return CheckEssenceCost( pMonster->GetAwakeningCost() );
}
Void BuildingEssenceStorage::Awake( UInt iTargetMonster )
{
    Assert( m_bAwakeningUnlocked );

    MonsterInstance * pMonster = m_pDungeon->GetMonster( iTargetMonster );
    Assert( pMonster != NULL && !(pMonster->IsAwakened()) );

    PayEssenceCost( pMonster->GetAwakeningCost() );

    pMonster->Awake();
}

Bool BuildingEssenceStorage::CanFuse( MonsterElement iElement, EssenceType iType ) const
{
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_HIGH );

    if ( !m_bFusionUnlocked )
        return false;

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    UInt iCost = pGameParams->GetEssenceFusionCost( iType );

    return ( m_arrEssences[iElement][iType] >= iCost );
}
Void BuildingEssenceStorage::Fuse( MonsterElement iElement, EssenceType iType )
{
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_HIGH );
    Assert( m_bFusionUnlocked );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    UInt iCost = pGameParams->GetEssenceFusionCost( iType );

    m_arrEssences[iElement][iType] -= iCost;

    m_arrEssences[iElement][iType+1] += 1;
}


