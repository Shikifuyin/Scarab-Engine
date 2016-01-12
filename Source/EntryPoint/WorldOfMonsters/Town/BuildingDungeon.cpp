/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingDungeon.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingDungeon interface
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
#include "BuildingDungeon.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BuildingDungeon implementation
BuildingDungeon::BuildingDungeon():
    Building()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Cost
    const CurrencyCost * pCost = pGameParams->GetBuildingCost( BUILDING_DUNGEON );
    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        m_hCost.arrCost[i] = pCost->arrCost[i];

    // Currencies
    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        m_arrCurrencies[i] = 0;

    // Monster collection
    m_iMonsterCollectionLevel = 0;
    m_iMonsterCollectionRoom = 5;
    m_arrMonsterCollection.UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrMonsterCollection.Create();
    m_arrMonsterCollection.EnsureCapacity( BUILDING_DUNGEON_MONSTER_COLLECTION_MAX_ROOM );

    // Rune collection
    m_iRuneCollectionLevel = 0;
    m_iRuneCollectionRoom = 16;
    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        m_arrRuneCollection[i].UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
        m_arrRuneCollection[i].Create();
        m_arrRuneCollection[i].EnsureCapacity( BUILDING_DUNGEON_RUNE_COLLECTION_MAX_ROOM );
    }

    // Arena state
    m_iArenaScore = 0;
    m_iArenaRank = ARENA_RANK_BEGINNER;

    for( UInt i = 0; i < BATTLE_TEAMSIZE_ARENA; ++i )
        m_arrArenaDefense[i] = NULL;
}
BuildingDungeon::~BuildingDungeon()
{
    // Monster collection
    m_arrMonsterCollection.Destroy();

    // Rune collection
    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i )
        m_arrRuneCollection[i].Destroy();
}

Bool BuildingDungeon::CheckCurrencyCost( const CurrencyCost * pCost ) const
{
    for( UInt i = 0; i < CURRENCY_COUNT; ++i ) {
        if ( m_arrCurrencies[i] < pCost->arrCost[i] )
            return false;
    }
    return true;
}
Void BuildingDungeon::PayCurrencyCost( const CurrencyCost * pCost )
{
    for( UInt i = 0; i < CURRENCY_COUNT; ++i ) {
        Assert( m_arrCurrencies[i] >= pCost->arrCost[i] );
        m_arrCurrencies[i] -= pCost->arrCost[i];
    }
}

Bool BuildingDungeon::UpgradeMonsterCollection()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterCollectionUpgradeCost();

    if ( m_iMonsterCollectionLevel >= BUILDING_DUNGEON_MONSTER_COLLECTION_MAX_LEVEL - 1 )
        return false;
    if ( !CheckCurrencyCost(pCost) )
        return false;

    static UInt s_arrRoomByLevel[BUILDING_DUNGEON_MONSTER_COLLECTION_MAX_LEVEL] = {
        5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100
    };

    PayCurrencyCost( pCost );

    ++m_iMonsterCollectionLevel;
    m_iMonsterCollectionRoom = s_arrRoomByLevel[m_iMonsterCollectionLevel];
    return true;
}

MonsterInstance * BuildingDungeon::AddMonster( const MonsterInstance & hMonster )
{
    Assert( hMonster.IsPresent() );
    Assert( m_arrMonsterCollection.Count() < m_iMonsterCollectionRoom );

    UInt iIndex = m_arrMonsterCollection.Search( _Compare_MonsterInstance, hMonster );
    m_arrMonsterCollection.Insert( iIndex, hMonster );

    return &( m_arrMonsterCollection[iIndex] );
}
Void BuildingDungeon::RemoveMonster( UInt iIndex, MonsterInstance * outMonster )
{
    Assert( iIndex < m_arrMonsterCollection.Count() );

    m_arrMonsterCollection.Remove( iIndex, *outMonster );
    Assert( outMonster->IsPresent() );
}

Bool BuildingDungeon::UpgradeRuneCollection()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetRuneCollectionUpgradeCost();

    if ( m_iRuneCollectionLevel >= BUILDING_DUNGEON_RUNE_COLLECTION_MAX_LEVEL - 1 )
        return false;
    if ( !CheckCurrencyCost(pCost) )
        return false;

    static UInt s_arrRoomByLevel[BUILDING_DUNGEON_RUNE_COLLECTION_MAX_LEVEL] = {
        16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256
    };

    PayCurrencyCost( pCost );

    ++m_iRuneCollectionLevel;
    m_iRuneCollectionRoom = s_arrRoomByLevel[m_iRuneCollectionLevel];
    return true;
}

Rune * BuildingDungeon::AddRune( const Rune & hRune )
{
    Assert( hRune.IsPresent() );

    RuneType iType = hRune.GetType();
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( m_arrRuneCollection[iType].Count() < m_iRuneCollectionRoom );

    UInt iIndex = m_arrRuneCollection[iType].Search( _Compare_Rune, hRune );
    m_arrRuneCollection[iType].Insert( iIndex, hRune );

    return &( (m_arrRuneCollection[iType])[iIndex] );
}
Void BuildingDungeon::RemoveRune( RuneType iType, UInt iIndex, Rune * outRune )
{
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iIndex < m_arrRuneCollection[iType].Count() );

    m_arrRuneCollection[iType].Remove( iIndex, *outRune );
    Assert( outRune->IsPresent() );
}

/////////////////////////////////////////////////////////////////////////////////

Int BuildingDungeon::_Compare_MonsterInstance( const MonsterInstance & rLeft, const MonsterInstance & rRight )
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
Int BuildingDungeon::_Compare_Rune( const Rune & rLeft, const Rune & rRight )
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

