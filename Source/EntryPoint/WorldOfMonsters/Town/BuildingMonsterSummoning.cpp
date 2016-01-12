/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingMonsterSummoning.cpp
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
// Includes
#include "BuildingMonsterSummoning.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BuildingMonsterSummoning implementation
BuildingMonsterSummoning::BuildingMonsterSummoning( BuildingDungeon * pDungeon ):
    Building()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Cost
    const CurrencyCost * pCost = pGameParams->GetBuildingCost( BUILDING_MONSTER_SUMMONING );
    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        m_hCost.arrCost[i] = pCost->arrCost[i];

    // Dungeon access
    m_pDungeon = pDungeon;

    // Monster summoning
    for( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i )
        m_arrScrolls[i] = 0;
}
BuildingMonsterSummoning::~BuildingMonsterSummoning()
{
    // nothing to do
}

Bool BuildingMonsterSummoning::CheckScrollCost( const ScrollCost * pCost ) const
{
    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i ) {
        if ( m_arrScrolls[i] < pCost->arrCost[i] )
            return false;
    }
    return true;
}
Void BuildingMonsterSummoning::PayScrollCost( const ScrollCost * pCost )
{
    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i ) {
        Assert( m_arrScrolls[i] >= pCost->arrCost[i] );
        m_arrScrolls[i] -= pCost->arrCost[i];
    }
}

Bool BuildingMonsterSummoning::CanSummon( Monster * pMonster ) const
{
    Assert( pMonster != NULL );

    return CheckScrollCost( pMonster->GetSummoningCost() );
}
Void BuildingMonsterSummoning::Summon( Monster * pMonster )
{
    Assert( pMonster != NULL );
    Assert( m_pDungeon->GetMonsterCount() < m_pDungeon->GetMonsterCollectionRoom() );

    PayScrollCost( pMonster->GetSummoningCost() );

    m_pDungeon->AddMonster( MonsterInstance(pMonster) );
}

