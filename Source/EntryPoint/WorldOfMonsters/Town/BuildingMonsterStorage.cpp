/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingMonsterStorage.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingMonsterStorage interface
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
#include "BuildingMonsterStorage.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BuildingMonsterStorage implementation
BuildingMonsterStorage::BuildingMonsterStorage( BuildingDungeon * pDungeon ):
    Building()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Cost
    const CurrencyCost * pCost = pGameParams->GetBuildingCost( BUILDING_MONSTER_STORAGE );
    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        m_hCost.arrCost[i] = pCost->arrCost[i];

    // Dungeon access
    m_pDungeon = pDungeon;

    // Monster storage
    m_iStorageLevel = 0;
    m_iStorageRoom = 5;
    m_arrMonsterStorage.UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrMonsterStorage.Create();
    m_arrMonsterStorage.EnsureCapacity( BUILDING_MONSTER_STORAGE_MAX_ROOM );
}
BuildingMonsterStorage::~BuildingMonsterStorage()
{
    // Monster storage
    m_arrMonsterStorage.Destroy();
}

Bool BuildingMonsterStorage::UpgradeStorageRoom()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterStorageUpgradeCost();

    if ( m_iStorageLevel >= BUILDING_MONSTER_STORAGE_MAX_LEVEL - 1 )
        return false;
    if ( !(m_pDungeon->CheckCurrencyCost(pCost)) )
        return false;

    static UInt s_arrRoomByLevel[BUILDING_MONSTER_STORAGE_MAX_LEVEL] = {
        20, 30, 40, 50, 60, 70, 80, 90, 95, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200,
        210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 380, 390, 400,
        410, 420, 430, 440, 450, 460, 470, 480, 490, 500
    };

    m_pDungeon->PayCurrencyCost( pCost );

    ++m_iStorageLevel;
    m_iStorageRoom = s_arrRoomByLevel[m_iStorageLevel];
    return true;
}

Bool BuildingMonsterStorage::StoreMonster( UInt iIndex )
{
    Assert( iIndex < m_pDungeon->GetMonsterCount() );

    if ( m_arrMonsterStorage.Count() >= m_iStorageRoom )
        return false;

    // Remove monster from collection
    MonsterInstance hMonster;
    m_pDungeon->RemoveMonster( iIndex, &hMonster );
    Assert( hMonster.IsPresent() );

    // Add it to storage
    UInt iIndex = m_arrMonsterStorage.Search( _Compare_MonsterInstance, hMonster );
    m_arrMonsterStorage.Insert( iIndex, hMonster );

    // Done
    return true;
}
Bool BuildingMonsterStorage::RetrieveMonster( UInt iStorageIndex )
{
    Assert( iStorageIndex < m_arrMonsterStorage.Count() );

    if ( m_pDungeon->GetMonsterCount() >= m_pDungeon->GetMonsterCollectionRoom() )
        return false;

    // Remove monster from storage
    MonsterInstance hMonster;
    m_arrMonsterStorage.Remove( iStorageIndex, hMonster );
    Assert( hMonster.IsPresent() );

    // Add it to collection
    m_pDungeon->AddMonster( hMonster );

    // Done
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Int BuildingMonsterStorage::_Compare_MonsterInstance( const MonsterInstance & rLeft, const MonsterInstance & rRight )
{
    if ( rLeft.GetElement() < rRight.GetElement() )
        return +1;
    if ( rLeft.GetElement() > rRight.GetElement() )
        return -1;
    if ( rLeft.GetRank() < rRight.GetRank() )
        return +1;
    if ( rLeft.GetRank() > rRight.GetRank() )
        return -1;
    if ( rLeft.GetLevel() < rRight.GetLevel() )
        return +1;
    if ( rLeft.GetLevel() > rRight.GetLevel() )
        return -1;
    if ( rLeft.GetID() < rRight.GetID() )
        return +1;
    if ( rLeft.GetID() > rRight.GetID() )
        return -1;
    return 0;
}

