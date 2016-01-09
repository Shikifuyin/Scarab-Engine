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
    Building( BUILDING_DUNGEON, BUILDING_COST_MANA, 1 )
{
    // Currencies
    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        m_arrCurrencies[i] = 0;

    // Monster collection
    m_iMonsterCollectionLevel = 0;
    m_iMonsterCollectionRoom = 0;

    m_arrMonsterCollection.UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrMonsterCollection.Create();
    m_arrMonsterCollection.EnsureCapacity( BUILDING_MONSTER_COLLECTION_MAX_ROOM );

    // Rune collection
    m_iRuneCollectionLevel = 0;
    m_iRuneCollectionRoom = 0;

    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        m_arrRuneCollection[i].UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
        m_arrRuneCollection[i].Create();
        m_arrRuneCollection[i].EnsureCapacity( BUILDING_RUNE_COLLECTION_MAX_ROOM );
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

Bool BuildingDungeon::UpgradeMonsterCollectionRoom()
{
    if ( m_iMonsterCollectionLevel >= BUILDING_MONSTER_COLLECTION_MAX_LEVEL )
        return false;
    if ( m_arrCurrencies[CURRENCY_MANA] < 20000 )
        return false;

    static UInt s_arrRoomByLevel[BUILDING_MONSTER_COLLECTION_MAX_LEVEL] = {
        5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100
    };

    m_arrCurrencies[CURRENCY_MANA] -= 20000;

    ++m_iMonsterCollectionLevel;
    m_iMonsterCollectionRoom = s_arrRoomByLevel[m_iMonsterCollectionLevel];
    return true;
}

Void BuildingDungeon::AddMonster( MonsterInstance * pMonster )
{
    Assert( pMonster != NULL );
    Assert( m_arrMonsterCollection.Count() < m_iMonsterCollectionRoom );

    UInt iIndex = m_arrMonsterCollection.Search( _Compare_MonsterInstance, pMonster );
    m_arrMonsterCollection.Insert( iIndex, pMonster );
}
MonsterInstance * BuildingDungeon::UnSummonMonster( UInt iIndex )
{
    Assert( iIndex < m_arrMonsterCollection.Count() );

    MonsterInstance * pMonster = NULL;
    m_arrMonsterCollection.Remove( iIndex, pMonster );

    return pMonster;
}

Bool BuildingDungeon::UpgradeRuneCollectionRoom()
{
    if ( m_iRuneCollectionLevel >= BUILDING_RUNE_COLLECTION_MAX_LEVEL )
        return false;
    if ( m_arrCurrencies[CURRENCY_MANA] < 20000 )
        return false;

    static UInt s_arrRoomByLevel[BUILDING_RUNE_COLLECTION_MAX_LEVEL] = {
        16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256
    };

    m_arrCurrencies[CURRENCY_MANA] -= 20000;

    ++m_iRuneCollectionLevel;
    m_iRuneCollectionRoom = s_arrRoomByLevel[m_iRuneCollectionLevel];
    return true;
}

Void BuildingDungeon::AddRune( Rune * pRune )
{
    RuneType iType = pRune->GetType();
    Assert( iType < RUNE_TYPE_COUNT );

    UInt iIndex = m_arrRuneCollection[iType].Search( _Compare_Rune, pRune );
    m_arrRuneCollection[iType].Insert( iIndex, pRune );
}
Void BuildingDungeon::SellRune( RuneType iType, UInt iIndex )
{
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iIndex < m_arrRuneCollection[iType].Count() );

    Rune * pRune = NULL;
    m_arrRuneCollection[iType].Remove( iIndex, pRune );

    // Ranges in [1000;375000], no random fluctuation for now ...
    static Float s_arrTypeFactor[RUNE_TYPE_COUNT] = { 1.0f, 1.0f, 1.0f, 2.0f, 3.0f, 5.0f, 3.0f, 3.0f, 3.0f, 3.0f, 4.0f, 4.0f, 4.0f, 5.0f, 5.0f, 4.0f };
    static Float s_arrRankFactor[RUNE_MAX_RANK]   = { 1.0f, 2.0f, 5.0f, 7.0f, 10.0f, 15.0f };
    static Float s_arrLevelFactor[RUNE_MAX_LEVEL] = { 1.0f, 1.2f, 1.5f, 1.7f, 2.0f, 2.2f, 2.5f, 2.7f, 3.0f, 3.2f, 3.5f, 3.7f, 4.0f, 4.2f, 4.5f, 5.0f };

    UInt iManaPrice = (UInt)( MathFn->Floor(1000.0f * s_arrTypeFactor[iType] * s_arrRankFactor[pRune->GetRank()] * s_arrLevelFactor[pRune->GetLevel()]) );
    m_arrCurrencies[CURRENCY_MANA] += iManaPrice;
}

/////////////////////////////////////////////////////////////////////////////////

Int BuildingDungeon::_Compare_MonsterInstance( MonsterInstance * const & pLeft, MonsterInstance * const & pRight )
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
Int BuildingDungeon::_Compare_Rune( Rune * const & pLeft, Rune * const & pRight )
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

