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
    Building()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Cost
    const CurrencyCost * pCost = pGameParams->GetBuildingCost( BUILDING_RUNE_STORAGE );
    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        m_hCost.arrCost[i] = pCost->arrCost[i];

    // Dungeon access
    m_pDungeon = pDungeon;

    // Rune storage
    m_iStorageLevel = 0;
    m_iStorageRoom = 16;
    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        m_arrRuneStorage[i].UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
        m_arrRuneStorage[i].Create();
        m_arrRuneStorage[i].EnsureCapacity( BUILDING_RUNE_STORAGE_MAX_ROOM );
    }
}
BuildingRuneStorage::~BuildingRuneStorage()
{
    // Rune storage
    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i )
        m_arrRuneStorage[i].Destroy();
}

Bool BuildingRuneStorage::UpgradeStorageRoom()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetRuneStorageUpgradeCost();

    if ( m_iStorageLevel >= BUILDING_RUNE_STORAGE_MAX_LEVEL - 1 )
        return false;
    if ( !(m_pDungeon->CheckCurrencyCost(pCost)) )
        return false;

    static UInt s_arrRoomByLevel[BUILDING_RUNE_STORAGE_MAX_LEVEL] = {
        16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256, 272, 288, 304, 320, 336, 352, 368, 384, 400,
        416, 432, 448, 464, 480, 496, 512, 528, 544, 560, 576, 592, 608, 624, 640, 656, 672, 688, 704, 720, 736, 752, 768, 784, 800,
        816, 832, 848, 864, 880, 896, 912, 928, 944, 960, 976, 992, 1008, 1024
    };

    m_pDungeon->PayCurrencyCost( pCost );

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
    Rune hRune;
    m_pDungeon->RemoveRune( iType, iIndex, &hRune );
    Assert( hRune.IsPresent() );

    // Add it to storage
    UInt iIndex = m_arrRuneStorage[iType].Search( _Compare_Rune, hRune );
    m_arrRuneStorage[iType].Insert( iIndex, hRune );

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
    Rune hRune;
    m_arrRuneStorage[iType].Remove( iStorageIndex, hRune );
    Assert( hRune.IsPresent() );

    // Add it to collection
    m_pDungeon->AddRune( hRune );

    // Done
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Int BuildingRuneStorage::_Compare_Rune( const Rune & rLeft, const Rune & rRight )
{
    if ( rLeft.GetSlot() < rRight.GetSlot() )
        return +1;
    if ( rLeft.GetSlot() > rRight.GetSlot() )
        return -1;
    if ( rLeft.GetRank() < rRight.GetRank() )
        return +1;
    if ( rLeft.GetRank() > rRight.GetRank() )
        return -1;
    if ( rLeft.GetLevel() < rRight.GetLevel() )
        return +1;
    if ( rLeft.GetLevel() > rRight.GetLevel() )
        return -1;
    if ( rLeft.GetQuality() < rRight.GetQuality() )
        return +1;
    if ( rLeft.GetQuality() > rRight.GetQuality() )
        return -1;
    return 0;
}


