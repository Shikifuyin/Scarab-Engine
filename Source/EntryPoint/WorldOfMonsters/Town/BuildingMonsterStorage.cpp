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
    Building( BUILDING_MONSTER_STORAGE, BUILDING_COST_MANA, 100000 )
{
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
    for( UInt i = 0; i < m_arrMonsterStorage.Count(); ++i )
        GameplayFn->DestroyMonsterInstance( m_arrMonsterStorage[i] );
    m_arrMonsterStorage.Destroy();
}

Bool BuildingMonsterStorage::UpgradeStorageRoom()
{
    if ( m_iStorageLevel >= BUILDING_MONSTER_STORAGE_MAX_LEVEL )
        return false;
    if ( m_pDungeon->GetCurrency(CURRENCY_MANA) < BUILDING_MONSTER_STORAGE_UPGRADE_COST )
        return false;

    static UInt s_arrRoomByLevel[BUILDING_MONSTER_COLLECTION_MAX_LEVEL] = {
        5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100
    };

    m_pDungeon->RemoveCurrency( CURRENCY_MANA, BUILDING_MONSTER_STORAGE_UPGRADE_COST );

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
    MonsterInstance * pMonster = m_pDungeon->RemoveMonster( iIndex );
    Assert( pMonster != NULL );

    // Add it to storage
    UInt iIndex = m_arrMonsterStorage.Search( _Compare_MonsterInstance, pMonster );
    m_arrMonsterStorage.Insert( iIndex, pMonster );

    // Done
    return true;
}
Bool BuildingMonsterStorage::RetrieveMonster( UInt iStorageIndex )
{
    Assert( iStorageIndex < m_arrMonsterStorage.Count() );

    if ( m_pDungeon->GetMonsterCount() >= m_pDungeon->GetMonsterCollectionRoom() )
        return false;

    // Remove monster from storage
    MonsterInstance * pMonster = NULL;
    m_arrMonsterStorage.Remove( iStorageIndex, pMonster );
    Assert( pMonster != NULL );

    // Add it to collection
    m_pDungeon->AddMonster( pMonster );

    // Done
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Int BuildingMonsterStorage::_Compare_MonsterInstance( MonsterInstance * const & pLeft, MonsterInstance * const & pRight )
{
    if ( pLeft->GetElement() < pRight->GetElement() )
        return +1;
    if ( pLeft->GetElement() > pRight->GetElement() )
        return -1;
    if ( pLeft->GetRank() < pRight->GetRank() )
        return +1;
    if ( pLeft->GetRank() > pRight->GetRank() )
        return -1;
    if ( pLeft->GetLevel() < pRight->GetLevel() )
        return +1;
    if ( pLeft->GetLevel() > pRight->GetLevel() )
        return -1;
    if ( pLeft->GetID() < pRight->GetID() )
        return +1;
    if ( pLeft->GetID() > pRight->GetID() )
        return -1;
    return 0;
}

