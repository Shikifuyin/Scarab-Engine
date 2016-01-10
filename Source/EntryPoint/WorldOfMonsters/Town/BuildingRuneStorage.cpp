/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingRuneStorage.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingRuneStorage interface
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
#include "BuildingRuneStorage.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BuildingRuneStorage implementation
BuildingRuneStorage::BuildingRuneStorage( BuildingDungeon * pDungeon ):
    Building( BUILDING_MONSTER_STORAGE, BUILDING_COST_MANA, 100000 )
{
    m_pDungeon = pDungeon;

    // Rune storage
    m_iStorageLevel = 0;
    m_iStorageRoom = 16;

    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        m_arrRuneStorage[i].UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
        m_arrRuneStorage[i].Create();
        m_arrRuneStorage[i].EnsureCapacity( BUILDING_RUNE_COLLECTION_MAX_ROOM );
    }
}
BuildingRuneStorage::~BuildingRuneStorage()
{
    // Rune storage
    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        for( UInt j = 0; j < m_arrRuneStorage[i].Count(); ++j )
            GameplayFn->DestroyRune( (m_arrRuneStorage[i])[j] );
        m_arrRuneStorage[i].Destroy();
    }
}

Bool BuildingRuneStorage::UpgradeStorageRoom()
{
    if ( m_iStorageLevel >= BUILDING_RUNE_COLLECTION_MAX_LEVEL )
        return false;
    if ( m_pDungeon->GetCurrency(CURRENCY_MANA) < BUILDING_RUNE_STORAGE_UPGRADE_COST )
        return false;

    static UInt s_arrRoomByLevel[BUILDING_RUNE_COLLECTION_MAX_LEVEL] = {
        16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256
    };

    m_pDungeon->RemoveCurrency( CURRENCY_MANA, BUILDING_RUNE_STORAGE_UPGRADE_COST );

    ++m_iStorageLevel;
    m_iStorageRoom = s_arrRoomByLevel[m_iStorageLevel];
    return true;
}

Bool BuildingRuneStorage::StoreRune( RuneType iType, UInt iIndex )
{
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iIndex < m_pDungeon->GetRuneCount(iType) );

    if ( m_arrRuneStorage[iType].Count() >= m_iStorageRoom )
        return false;

    // Remove rune from collection
    Rune * pRune = m_pDungeon->RemoveRune( iType, iIndex );
    Assert( pRune != NULL );

    // Add it to storage
    UInt iIndex = m_arrRuneStorage[iType].Search( _Compare_Rune, pRune );
    m_arrRuneStorage[iType].Insert( iIndex, pRune );

    // Done
    return true;
}
Bool BuildingRuneStorage::RetrieveRune( RuneType iType, UInt iStorageIndex )
{
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iStorageIndex < m_arrRuneStorage[iType].Count() );

    if ( m_pDungeon->GetRuneCount(iType) >= m_pDungeon->GetRuneCollectionRoom() )
        return false;

    // Remove rune from storage
    Rune * pRune = NULL;
    m_arrRuneStorage[iType].Remove( iStorageIndex, pRune );
    Assert( pRune != NULL );

    // Add it to collection
    m_pDungeon->AddRune( pRune );

    // Done
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Int BuildingRuneStorage::_Compare_Rune( Rune * const & pLeft, Rune * const & pRight )
{
    if ( pLeft->GetSlot() < pRight->GetSlot() )
        return +1;
    if ( pLeft->GetSlot() > pRight->GetSlot() )
        return -1;
    if ( pLeft->GetRank() < pRight->GetRank() )
        return +1;
    if ( pLeft->GetRank() > pRight->GetRank() )
        return -1;
    if ( pLeft->GetLevel() < pRight->GetLevel() )
        return +1;
    if ( pLeft->GetLevel() > pRight->GetLevel() )
        return -1;
    if ( pLeft->GetQuality() < pRight->GetQuality() )
        return +1;
    if ( pLeft->GetQuality() > pRight->GetQuality() )
        return -1;
    return 0;
}


