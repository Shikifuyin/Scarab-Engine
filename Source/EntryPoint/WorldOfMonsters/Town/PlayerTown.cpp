/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/PlayerTown.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : Base PlayerTown interface
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
#include "PlayerTown.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// PlayerTown implementation
PlayerTown::PlayerTown()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Currencies, scrolls & essences
    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        m_arrCurrencies[i] = 0;
    for( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i )
        m_arrScrolls[i] = 0;
    for( UInt i = 0; i < MONSTER_ELEMENT_COUNT; ++i ) {
        for( UInt j = 0; j < ESSENCE_TYPE_COUNT; ++j )
            m_arrEssences[i][j] = 0;
    }

    // Monster collection & storage
    m_iMonsterCollectionLevel = 0;
    m_iMonsterStorageLevel = 0;
    m_iMonsterCollectionRoom = pGameParams->GetMonsterCollectionRoom( m_iMonsterCollectionLevel );
    m_iMonsterStorageRoom = pGameParams->GetMonsterStorageRoom( m_iMonsterStorageLevel );

    m_arrMonsterCollection.UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrMonsterCollection.Create();
    m_arrMonsterCollection.EnsureCapacity( MONSTER_COLLECTION_MAX_ROOM );

    m_arrMonsterStorage.UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrMonsterStorage.Create();
    m_arrMonsterStorage.EnsureCapacity( MONSTER_STORAGE_MAX_ROOM );

    // Rune collection & storage
    m_iRuneCollectionLevel = 0;
    m_iRuneStorageLevel = 0;
    m_iRuneCollectionRoom = pGameParams->GetRuneCollectionRoom( m_iRuneCollectionLevel );
    m_iRuneStorageRoom = pGameParams->GetRuneStorageRoom( m_iRuneStorageLevel );

    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        m_arrRuneCollection[i].UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
        m_arrRuneCollection[i].Create();
        m_arrRuneCollection[i].EnsureCapacity( RUNE_COLLECTION_MAX_ROOM );

        m_arrRuneStorage[i].UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
        m_arrRuneStorage[i].Create();
        m_arrRuneStorage[i].EnsureCapacity( RUNE_STORAGE_MAX_ROOM );
    }

    // Mana & Crystal production
    m_iManaProductionRateLevel = 0;
    m_iCrystalProductionRateLevel = 0;
    m_iManaProductionRate = pGameParams->GetManaProductionRate( m_iManaProductionRateLevel );
    m_iCrystalProductionRate = pGameParams->GetCrystalProductionRate( m_iCrystalProductionRateLevel );

    m_iManaCapacityLevel = 0;
    m_iCrystalCapacityLevel = 0;
    m_iManaCapacity = pGameParams->GetManaCapacity( m_iManaCapacityLevel );
    m_iCrystalCapacity = pGameParams->GetCrystalCapacity( m_iCrystalCapacityLevel );

    m_iManaBuffer = 0;
    m_iCrystalBuffer = 0;

    m_hLastUpdateTime = 0;

    // Shop

    // Wishes

    // Building operations
    m_bEssenceFusionUnlocked = false;
    m_bMonsterSummoningUnlocked = false;
    m_bMonsterFusionUnlocked = false;
    m_bMonsterPowerUpUnlocked = false;
    m_bMonsterEvolutionUnlocked = false;
    m_bMonsterAwakeningUnlocked = false;
    m_bRunePowerUpUnlocked = false;
    m_bRuneEvolutionUnlocked = false;

    // Arena
    m_iArenaScore = 0;
    m_iArenaRank = ARENA_RANK_BEGINNER;

    for( UInt i = 0; i < BATTLE_TEAMSIZE_ARENA; ++i )
        m_arrArenaDefense[i] = NULL;

    // Guild
    m_pGuild = NULL;
}
PlayerTown::~PlayerTown()
{
    // Monster collection & storage
    m_arrMonsterCollection.Destroy();
    m_arrMonsterStorage.Destroy();

    // Rune collection & storage
    for ( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        m_arrRuneCollection[i].Destroy();
        m_arrRuneStorage[i].Destroy();
    }
}

Void PlayerTown::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("PlayerTown")) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Currencies, scrolls & essences
    const XMLNode * pCurrenciesNode = pNode->GetChildByTag( TEXT("Currencies"), 0 );
    Assert( pCurrenciesNode != NULL );
    const XMLNode * pScrollsNode = pNode->GetChildByTag( TEXT("Scrolls"), 0 );
    Assert( pScrollsNode != NULL );
    const XMLNode * pEssencesNode = pNode->GetChildByTag( TEXT("Essences"), 0 );
    Assert( pEssencesNode != NULL );

    for( UInt i = 0; i < CURRENCY_COUNT; ++i ) {
        const GChar * strName = pGameParams->CurrencyTypeToString( (CurrencyType)i );
        m_arrCurrencies[i] = (UInt)( StringFn->ToUInt( pCurrenciesNode->GetAttribute(strName)->GetValue() ) );
    }
    for( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i ) {
        const GChar * strName = pGameParams->ScrollTypeToString( (ScrollType)i );
        m_arrScrolls[i] = (UInt)( StringFn->ToUInt( pScrollsNode->GetAttribute(strName)->GetValue() ) );
    }
    for( UInt i = 0; i < MONSTER_ELEMENT_COUNT; ++i ) {
        const GChar * strName = pGameParams->MonsterElementToString( (MonsterElement)i );
        GChar arrEssenceCount[ESSENCE_TYPE_COUNT][16];

        UInt iCount = StringFn->Split( (GChar**)arrEssenceCount, ESSENCE_TYPE_COUNT, 16, pEssencesNode->GetAttribute(strName)->GetValue(), TEXT(','), true );
        Assert( iCount == ESSENCE_TYPE_COUNT );

        for( UInt j = 0; j < ESSENCE_TYPE_COUNT; ++j )
            m_arrEssences[i][j] = (UInt)( StringFn->ToUInt(arrEssenceCount[i]) );
    }
    
    // Monster collection & storage
    const XMLNode * pMonsterCollectionNode = pNode->GetChildByTag( TEXT("MonsterCollection"), 0 );
    Assert( pMonsterCollectionNode != NULL );
    const XMLNode * pMonsterStorageNode = pNode->GetChildByTag( TEXT("MonsterStorage"), 0 );
    Assert( pMonsterStorageNode != NULL );

    m_iMonsterCollectionLevel = (UInt)( StringFn->ToUInt( pMonsterCollectionNode->GetAttribute(TEXT("Level"))->GetValue() ) );
    m_iMonsterStorageLevel = (UInt)( StringFn->ToUInt( pMonsterStorageNode->GetAttribute(TEXT("Level"))->GetValue() ) );
    m_iMonsterCollectionRoom = pGameParams->GetMonsterCollectionRoom( m_iMonsterCollectionLevel );
    m_iMonsterStorageRoom = pGameParams->GetMonsterStorageRoom( m_iMonsterStorageLevel );

    m_arrMonsterCollection.Clear();
    m_arrMonsterStorage.Clear();

    UInt iMonsterCount = pMonsterCollectionNode->GetChildCount();
    for( UInt i = 0; i < iMonsterCount; ++i ) {
        const XMLNode * pMonsterInstanceNode = pMonsterCollectionNode->GetChildByTag( TEXT("MonsterInstance"), i );
        Assert( pMonsterInstanceNode != NULL );

        MonsterInstance hMonsterInstance;
        hMonsterInstance.Load( pMonsterInstanceNode );

        m_arrMonsterCollection.Push( hMonsterInstance );
    }

    iMonsterCount = pMonsterStorageNode->GetChildCount();
    for( UInt i = 0; i < iMonsterCount; ++i ) {
        const XMLNode * pMonsterInstanceNode = pMonsterStorageNode->GetChildByTag( TEXT("MonsterInstance"), i );
        Assert( pMonsterInstanceNode != NULL );

        MonsterInstance hMonsterInstance;
        hMonsterInstance.Load( pMonsterInstanceNode );

        m_arrMonsterStorage.Push( hMonsterInstance );
    }

    // Rune collection & storage
    const XMLNode * pRuneCollectionNode = pNode->GetChildByTag( TEXT("RuneCollection"), 0 );
    Assert( pRuneCollectionNode != NULL );
    const XMLNode * pRuneStorageNode = pNode->GetChildByTag( TEXT("RuneStorage"), 0 );
    Assert( pRuneStorageNode != NULL );

    m_iRuneCollectionLevel = (UInt)( StringFn->ToUInt( pRuneCollectionNode->GetAttribute(TEXT("Level"))->GetValue() ) );
    m_iRuneStorageLevel = (UInt)( StringFn->ToUInt( pRuneStorageNode->GetAttribute(TEXT("Level"))->GetValue() ) );
    m_iRuneCollectionRoom = pGameParams->GetRuneCollectionRoom( m_iRuneCollectionLevel );
    m_iRuneStorageRoom = pGameParams->GetRuneStorageRoom( m_iRuneStorageLevel );

    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        m_arrRuneCollection[i].Clear();
        m_arrRuneStorage[i].Clear();
    }

    UInt iRuneCount = pRuneCollectionNode->GetChildCount();
    for( UInt i = 0; i < iRuneCount; ++i ) {
        const XMLNode * pRuneNode = pRuneCollectionNode->GetChildByTag( TEXT("Rune"), i );
        Assert( pRuneNode != NULL );

        Rune hRune;
        hRune.Load( pRuneNode );

        m_arrRuneCollection[hRune.GetType()].Push( hRune );
    }

    iRuneCount = pRuneStorageNode->GetChildCount();
    for( UInt i = 0; i < iRuneCount; ++i ) {
        const XMLNode * pRuneNode = pRuneStorageNode->GetChildByTag( TEXT("Rune"), i );
        Assert( pRuneNode != NULL );

        Rune hRune;
        hRune.Load( pRuneNode );

        m_arrRuneStorage[hRune.GetType()].Push( hRune );
    }


    // Mana & Crystal production
    const XMLNode * pManaCrystalProductionNode = pNode->GetChildByTag( TEXT("ManaCrystalProduction"), 0 );
    Assert( pManaCrystalProductionNode != NULL );

    m_iManaProductionRateLevel = (UInt)( StringFn->ToUInt( pManaCrystalProductionNode->GetAttribute(TEXT("ManaRateLevel"))->GetValue() ) );
    m_iCrystalProductionRateLevel = (UInt)( StringFn->ToUInt( pManaCrystalProductionNode->GetAttribute(TEXT("CrystalRateLevel"))->GetValue() ) );
    m_iManaProductionRate = pGameParams->GetManaProductionRate( m_iManaProductionRateLevel );
    m_iCrystalProductionRate = pGameParams->GetCrystalProductionRate( m_iCrystalProductionRateLevel );

    m_iManaCapacityLevel = (UInt)( StringFn->ToUInt( pManaCrystalProductionNode->GetAttribute(TEXT("ManaCapacityLevel"))->GetValue() ) );
    m_iCrystalCapacityLevel = (UInt)( StringFn->ToUInt( pManaCrystalProductionNode->GetAttribute(TEXT("CrystalCapacityLevel"))->GetValue() ) );
    m_iManaCapacity = pGameParams->GetManaCapacity( m_iManaCapacityLevel );
    m_iCrystalCapacity = pGameParams->GetCrystalCapacity( m_iCrystalCapacityLevel );

    m_iManaBuffer = (UInt)( StringFn->ToUInt( pManaCrystalProductionNode->GetAttribute(TEXT("ManaBuffer"))->GetValue() ) );
    m_iCrystalBuffer = (UInt)( StringFn->ToUInt( pManaCrystalProductionNode->GetAttribute(TEXT("CrystalBuffer"))->GetValue() ) );

    m_hLastUpdateTime = (TimeValue)( StringFn->ToUInt( pManaCrystalProductionNode->GetAttribute(TEXT("LastUpdateTime"))->GetValue() ) );

    // Shop

    // Wishes

    // Building operations
    const XMLNode * pBuildingOperationsNode = pNode->GetChildByTag( TEXT("BuildingOperations"), 0 );
    Assert( pBuildingOperationsNode != NULL );

    m_bEssenceFusionUnlocked = ( StringFn->ToUInt(pBuildingOperationsNode->GetAttribute(TEXT("EssenceFusion"))->GetValue()) != 0 );
    m_bMonsterSummoningUnlocked = ( StringFn->ToUInt(pBuildingOperationsNode->GetAttribute(TEXT("MonsterSummoning"))->GetValue()) != 0 );
    m_bMonsterFusionUnlocked = ( StringFn->ToUInt(pBuildingOperationsNode->GetAttribute(TEXT("MonsterFusion"))->GetValue()) != 0 );
    m_bMonsterPowerUpUnlocked = ( StringFn->ToUInt(pBuildingOperationsNode->GetAttribute(TEXT("MonsterPowerUp"))->GetValue()) != 0 );
    m_bMonsterEvolutionUnlocked = ( StringFn->ToUInt(pBuildingOperationsNode->GetAttribute(TEXT("MonsterEvolution"))->GetValue()) != 0 );
    m_bMonsterAwakeningUnlocked = ( StringFn->ToUInt(pBuildingOperationsNode->GetAttribute(TEXT("MonsterAwakening"))->GetValue()) != 0 );
    m_bRunePowerUpUnlocked = ( StringFn->ToUInt(pBuildingOperationsNode->GetAttribute(TEXT("RunePowerUp"))->GetValue()) != 0 );
    m_bRuneEvolutionUnlocked = ( StringFn->ToUInt(pBuildingOperationsNode->GetAttribute(TEXT("RuneEvolution"))->GetValue()) != 0 );

    // Arena
    const XMLNode * pArenaNode = pNode->GetChildByTag( TEXT("Arena"), 0 );
    Assert( pArenaNode != NULL );

    m_iArenaScore = (UInt)( StringFn->ToUInt( pArenaNode->GetAttribute(TEXT("Score"))->GetValue() ) );
    m_iArenaRank = (ArenaRank)( StringFn->ToUInt( pArenaNode->GetAttribute(TEXT("Rank"))->GetValue() ) );
    Assert( m_iArenaRank < ARENA_RANK_COUNT );

    for( UInt i = 0; i < BATTLE_TEAMSIZE_ARENA; ++i ) {
        GChar strName[16];
        StringFn->Format( strName, TEXT("DefenseSlot_%d"), i );

        m_arrArenaDefense[i] = (UInt)( StringFn->ToUInt( pArenaNode->GetAttribute(strName)->GetValue() ) );
        Assert( m_arrArenaDefense[i] < m_arrMonsterCollection.Count() );
    }

    // Guild
    m_pGuild = NULL;
    ////////////////////////////////
}
Void PlayerTown::Save( XMLNode * outNode ) const
{
    Assert( outNode != NULL );
    Assert( StringFn->Cmp(outNode->GetTagName(), TEXT("PlayerTown")) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    GChar strBuffer[1024];
    const GChar * strValue;

    // Currencies, scrolls & essences
    XMLNode * pCurrenciesNode = XMLDocument::CreateNode( TEXT("Currencies"), true );
    XMLNode * pScrollsNode = XMLDocument::CreateNode( TEXT("Scrolls"), true );
    XMLNode * pEssencesNode = XMLDocument::CreateNode( TEXT("Essences"), true );

    for( UInt i = 0; i < CURRENCY_COUNT; ++i ) {
        const GChar * strName = pGameParams->CurrencyTypeToString( (CurrencyType)i );
        StringFn->FromUInt( strBuffer, m_arrCurrencies[i] );
        pCurrenciesNode->CreateAttribute( strName, strBuffer );
    }
    for( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i ) {
        const GChar * strName = pGameParams->ScrollTypeToString( (ScrollType)i );
        StringFn->FromUInt( strBuffer, m_arrScrolls[i] );
        pCurrenciesNode->CreateAttribute( strName, strBuffer );
    }
    for( UInt i = 0; i < MONSTER_ELEMENT_COUNT; ++i ) {
        const GChar * strName = pGameParams->MonsterElementToString( (MonsterElement)i );
        StringFn->Format( strBuffer, TEXT("%d,%d,%d"), m_arrEssences[i][0], m_arrEssences[i][1], m_arrEssences[i][2] );
        pCurrenciesNode->CreateAttribute( strName, strBuffer );
    }

    outNode->AppendChild( pCurrenciesNode );
    outNode->AppendChild( pScrollsNode );
    outNode->AppendChild( pEssencesNode );

    // Monster collection & storage
    XMLNode * pMonsterCollectionNode = XMLDocument::CreateNode( TEXT("MonsterCollection"), false );
    XMLNode * pMonsterStorageNode = XMLDocument::CreateNode( TEXT("MonsterStorage"), false );

    StringFn->FromUInt( strBuffer, m_iMonsterCollectionLevel );
    pMonsterCollectionNode->CreateAttribute( TEXT("Level"), strBuffer );
    StringFn->FromUInt( strBuffer, m_iMonsterStorageLevel );
    pMonsterStorageNode->CreateAttribute( TEXT("Level"), strBuffer );

    UInt iMonsterCount = m_arrMonsterCollection.Count();
    for( UInt i = 0; i < iMonsterCount; ++i ) {
        XMLNode * pMonsterInstanceNode = XMLDocument::CreateNode( TEXT("MonsterInstance"), false );

        m_arrMonsterCollection[i].Save( pMonsterInstanceNode );

        pMonsterCollectionNode->AppendChild( pMonsterInstanceNode );
    }

    iMonsterCount = m_arrMonsterStorage.Count();
    for( UInt i = 0; i < iMonsterCount; ++i ) {
        XMLNode * pMonsterInstanceNode = XMLDocument::CreateNode( TEXT("MonsterInstance"), false );

        m_arrMonsterStorage[i].Save( pMonsterInstanceNode );

        pMonsterStorageNode->AppendChild( pMonsterInstanceNode );
    }

    outNode->AppendChild( pMonsterCollectionNode );
    outNode->AppendChild( pMonsterStorageNode );

    // Rune collection & storage
    XMLNode * pRuneCollectionNode = XMLDocument::CreateNode( TEXT("RuneCollection"), false );
    XMLNode * pRuneStorageNode = XMLDocument::CreateNode( TEXT("RuneStorage"), false );

    StringFn->FromUInt( strBuffer, m_iRuneCollectionLevel );
    pRuneCollectionNode->CreateAttribute( TEXT("Level"), strBuffer );
    StringFn->FromUInt( strBuffer, m_iRuneStorageLevel );
    pRuneStorageNode->CreateAttribute( TEXT("Level"), strBuffer );

    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        UInt iRuneCount = m_arrRuneCollection[i].Count();
        for( UInt j = 0; j < iRuneCount; ++j ) {
            XMLNode * pRuneNode = XMLDocument::CreateNode( TEXT("Rune"), false );

            (m_arrRuneCollection[i])[j].Save( pRuneNode );

            pRuneCollectionNode->AppendChild( pRuneNode );
        }

        iRuneCount = m_arrRuneStorage[i].Count();
        for( UInt j = 0; j < iRuneCount; ++j ) {
            XMLNode * pRuneNode = XMLDocument::CreateNode( TEXT("Rune"), false );

            (m_arrRuneStorage[i])[j].Save( pRuneNode );

            pRuneStorageNode->AppendChild( pRuneNode );
        }
    }

    outNode->AppendChild( pRuneCollectionNode );
    outNode->AppendChild( pRuneStorageNode );

    // Mana & Crystal production
    XMLNode * pManaCrystalProductionNode = XMLDocument::CreateNode( TEXT("ManaCrystalProduction"), true );

    StringFn->FromUInt( strBuffer, m_iManaProductionRateLevel );
    pManaCrystalProductionNode->CreateAttribute( TEXT("ManaRateLevel"), strBuffer );
    StringFn->FromUInt( strBuffer, m_iCrystalProductionRateLevel );
    pManaCrystalProductionNode->CreateAttribute( TEXT("CrystalRateLevel"), strBuffer );

    StringFn->FromUInt( strBuffer, m_iManaCapacityLevel );
    pManaCrystalProductionNode->CreateAttribute( TEXT("ManaCapacityLevel"), strBuffer );
    StringFn->FromUInt( strBuffer, m_iCrystalCapacityLevel );
    pManaCrystalProductionNode->CreateAttribute( TEXT("CrystalCapacityLevel"), strBuffer );

    StringFn->FromUInt( strBuffer, m_iManaBuffer );
    pManaCrystalProductionNode->CreateAttribute( TEXT("ManaBuffer"), strBuffer );
    StringFn->FromUInt( strBuffer, m_iCrystalBuffer );
    pManaCrystalProductionNode->CreateAttribute( TEXT("CrystalBuffer"), strBuffer );

    StringFn->FromUInt( strBuffer, m_hLastUpdateTime );
    pManaCrystalProductionNode->CreateAttribute( TEXT("LastUpdateTime"), strBuffer );

    outNode->AppendChild( pManaCrystalProductionNode );

    // Shop

    // Wishes

    // Building operations
    XMLNode * pBuildingOperationsNode = XMLDocument::CreateNode( TEXT("BuildingOperations"), true );

    pBuildingOperationsNode->CreateAttribute( TEXT("EssenceFusion"), m_bEssenceFusionUnlocked ? TEXT("1") : TEXT("0") );
    pBuildingOperationsNode->CreateAttribute( TEXT("MonsterSummoning"), m_bMonsterSummoningUnlocked ? TEXT("1") : TEXT("0") );
    pBuildingOperationsNode->CreateAttribute( TEXT("MonsterFusion"), m_bMonsterFusionUnlocked ? TEXT("1") : TEXT("0") );
    pBuildingOperationsNode->CreateAttribute( TEXT("MonsterPowerUp"), m_bMonsterPowerUpUnlocked ? TEXT("1") : TEXT("0") );
    pBuildingOperationsNode->CreateAttribute( TEXT("MonsterEvolution"), m_bMonsterEvolutionUnlocked ? TEXT("1") : TEXT("0") );
    pBuildingOperationsNode->CreateAttribute( TEXT("MonsterAwakening"), m_bMonsterAwakeningUnlocked ? TEXT("1") : TEXT("0") );
    pBuildingOperationsNode->CreateAttribute( TEXT("RunePowerUp"), m_bRunePowerUpUnlocked ? TEXT("1") : TEXT("0") );
    pBuildingOperationsNode->CreateAttribute( TEXT("RuneEvolution"), m_bRuneEvolutionUnlocked ? TEXT("1") : TEXT("0") );

    outNode->AppendChild( pBuildingOperationsNode );

    // Arena
    XMLNode * pArenaNode = XMLDocument::CreateNode( TEXT("Arena"), true );

    StringFn->FromUInt( strBuffer, m_iArenaScore );
    pArenaNode->CreateAttribute( TEXT("Score"), strBuffer );
    StringFn->FromUInt( strBuffer, m_iArenaRank );
    pArenaNode->CreateAttribute( TEXT("Rank"), strBuffer );

    for( UInt i = 0; i < BATTLE_TEAMSIZE_ARENA; ++i ) {
        GChar strName[16];
        StringFn->Format( strName, TEXT("DefenseSlot_%d"), i );

        StringFn->FromUInt( strBuffer, m_arrArenaDefense[i] );
        pArenaNode->CreateAttribute( strName, strBuffer );
    }

    outNode->AppendChild( pArenaNode );

    // Guild
    /////////////////////////////////////
}

Bool PlayerTown::CheckCurrencyCost( const CurrencyCost * pCost ) const
{
    for( UInt i = 0; i < CURRENCY_COUNT; ++i ) {
        if ( m_arrCurrencies[i] < pCost->arrCost[i] )
            return false;
    }
    return true;
}
Bool PlayerTown::CheckScrollCost( const ScrollCost * pCost ) const
{
    for( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i ) {
        if ( m_arrScrolls[i] < pCost->arrCost[i] )
            return false;
    }
    return true;
}
Bool PlayerTown::CheckEssenceCost( const EssenceCost * pCost ) const
{
    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i ) {
        if ( m_arrEssences[MONSTER_ELEMENT_MAGIC][i] < pCost->arrMagicCost[i] )
            return false;
        if ( m_arrEssences[pCost->iElement][i] < pCost->arrElementalCost[i] )
            return false;
    }
    return true;
}

Void PlayerTown::PayCurrencyCost( const CurrencyCost * pCost )
{
    for( UInt i = 0; i < CURRENCY_COUNT; ++i ) {
        Assert( m_arrCurrencies[i] >= pCost->arrCost[i] );
        m_arrCurrencies[i] -= pCost->arrCost[i];
    }
}
Void PlayerTown::PayScrollCost( const ScrollCost * pCost )
{
    for( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i ) {
        Assert( m_arrScrolls[i] >= pCost->arrCost[i] );
        m_arrScrolls[i] -= pCost->arrCost[i];
    }
}
Void PlayerTown::PayEssenceCost( const EssenceCost * pCost )
{
    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i ) {
        Assert( m_arrEssences[MONSTER_ELEMENT_MAGIC][i] >= pCost->arrMagicCost[i] );
        Assert( m_arrEssences[pCost->iElement][i] >= pCost->arrElementalCost[i] );
        m_arrEssences[MONSTER_ELEMENT_MAGIC][i] -= pCost->arrMagicCost[i];
        m_arrEssences[pCost->iElement][i] -= pCost->arrElementalCost[i];
    }
}

Bool PlayerTown::UpgradeMonsterCollection()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterCollectionUpgradeCost( m_iMonsterCollectionLevel );

    if ( m_iMonsterCollectionLevel >= MONSTER_COLLECTION_MAX_LEVEL - 1 )
        return false;
    if ( !CheckCurrencyCost(pCost) )
        return false;

    PayCurrencyCost( pCost );

    ++m_iMonsterCollectionLevel;
    m_iMonsterCollectionRoom = pGameParams->GetMonsterCollectionRoom( m_iMonsterCollectionLevel );
    return true;
}
Bool PlayerTown::UpgradeMonsterStorage()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterStorageUpgradeCost( m_iMonsterStorageLevel );

    if ( m_iMonsterStorageLevel >= MONSTER_STORAGE_MAX_LEVEL - 1 )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    ++m_iMonsterStorageLevel;
    m_iMonsterStorageRoom = pGameParams->GetMonsterStorageRoom( m_iMonsterStorageLevel );
    return true;
}

Void PlayerTown::UnSummonMonster( UInt iIndex, MonsterInstance * outMonster )
{
    Assert( iIndex < m_arrMonsterCollection.Count() );

    m_arrMonsterCollection.Remove( iIndex, *outMonster );
    Assert( outMonster->IsPresent() );
}

Bool PlayerTown::StoreMonster( UInt iIndex )
{
    Assert( iIndex < m_arrMonsterCollection.Count() );

    if ( m_arrMonsterStorage.Count() >= m_iMonsterStorageRoom )
        return false;

    MonsterInstance hMonster;
    m_arrMonsterCollection.Remove( iIndex, hMonster );
    Assert( hMonster.IsPresent() );

    UInt iStorageIndex = m_arrMonsterStorage.Search( _Compare_MonsterInstance, hMonster );
    m_arrMonsterStorage.Insert( iStorageIndex, hMonster );

    return true;
}
Bool PlayerTown::RetrieveMonster( UInt iStorageIndex )
{
    Assert( iStorageIndex < m_arrMonsterStorage.Count() );

    if ( m_arrMonsterCollection.Count() >= m_iMonsterCollectionRoom )
        return false;

    MonsterInstance hMonster;
    m_arrMonsterStorage.Remove( iStorageIndex, hMonster );
    Assert( hMonster.IsPresent() );

    UInt iIndex = m_arrMonsterCollection.Search( _Compare_MonsterInstance, hMonster );
    m_arrMonsterCollection.Insert( iIndex, hMonster );

    return true;
}

Bool PlayerTown::UpgradeRuneCollection()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetRuneCollectionUpgradeCost( m_iRuneCollectionLevel );

    if ( m_iRuneCollectionLevel >= RUNE_COLLECTION_MAX_LEVEL - 1 )
        return false;
    if ( !CheckCurrencyCost(pCost) )
        return false;

    PayCurrencyCost( pCost );

    ++m_iRuneCollectionLevel;
    m_iRuneCollectionRoom = pGameParams->GetRuneCollectionRoom( m_iRuneCollectionLevel );
    return true;
}
Bool PlayerTown::UpgradeRuneStorage()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetRuneStorageUpgradeCost( m_iRuneStorageLevel );

    if ( m_iRuneStorageLevel >= RUNE_STORAGE_MAX_LEVEL - 1 )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    ++m_iRuneStorageLevel;
    m_iRuneStorageRoom = pGameParams->GetRuneStorageRoom( m_iRuneStorageLevel );
    return true;
}

Rune * PlayerTown::AddRune( const Rune & hRune )
{
    Assert( hRune.IsPresent() );
    RuneType iType = hRune.GetType();
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( m_arrRuneCollection[iType].Count() < m_iRuneCollectionRoom );

    UInt iIndex = m_arrRuneCollection[iType].Search( _Compare_Rune, hRune );
    m_arrRuneCollection[iType].Insert( iIndex, hRune );

    return &( (m_arrRuneCollection[iType])[iIndex] );
}
Void PlayerTown::RemoveRune( RuneType iType, UInt iIndex, Rune * outRune )
{
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iIndex < m_arrRuneCollection[iType].Count() );

    m_arrRuneCollection[iType].Remove( iIndex, *outRune );
    Assert( outRune->IsPresent() );
}

Bool PlayerTown::StoreRune( RuneType iType, UInt iIndex )
{
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iIndex < m_arrRuneCollection[iType].Count() );

    if ( m_arrRuneStorage[iType].Count() >= m_iRuneStorageRoom )
        return false;

    Rune hRune;
    m_arrRuneCollection[iType].Remove( iIndex, hRune );
    Assert( hRune.IsPresent() );

    UInt iStorageIndex = m_arrRuneStorage[iType].Search( _Compare_Rune, hRune );
    m_arrRuneStorage[iType].Insert( iStorageIndex, hRune );

    return true;
}
Bool PlayerTown::RetrieveRune( RuneType iType, UInt iStorageIndex )
{
    Assert( iType < RUNE_TYPE_COUNT );
    Assert( iStorageIndex < m_arrRuneStorage[iType].Count() );

    if ( m_arrRuneCollection[iType].Count() >= m_iRuneCollectionRoom )
        return false;

    Rune hRune;
    m_arrRuneStorage[iType].Remove( iStorageIndex, hRune );
    Assert( hRune.IsPresent() );

    UInt iIndex = m_arrRuneCollection[iType].Search( _Compare_Rune, hRune );
    m_arrRuneCollection[iType].Insert( iIndex, hRune );

    return true;
}

Bool PlayerTown::UpgradeManaProductionRate()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetManaProductionRateUpgradeCost( m_iManaProductionRateLevel );

    if ( m_iManaProductionRateLevel >= BUILDING_MAX_LEVEL - 1 )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    ++m_iManaProductionRateLevel;
    m_iManaProductionRate = pGameParams->GetManaProductionRate( m_iManaProductionRateLevel );
    return true;
}
Bool PlayerTown::UpgradeCrystalProductionRate()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetCrystalProductionRateUpgradeCost( m_iCrystalProductionRateLevel );

    if ( m_iCrystalProductionRateLevel >= BUILDING_MAX_LEVEL - 1 )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    ++m_iCrystalProductionRateLevel;
    m_iCrystalProductionRate = pGameParams->GetCrystalProductionRate( m_iCrystalProductionRateLevel );
    return true;
}

Bool PlayerTown::UpgradeManaCapacity()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetManaCapacityUpgradeCost( m_iManaCapacityLevel );

    if ( m_iManaCapacityLevel >= BUILDING_MAX_LEVEL - 1 )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    ++m_iManaCapacityLevel;
    m_iManaCapacity = pGameParams->GetManaCapacity( m_iManaCapacityLevel );
    return true;
}
Bool PlayerTown::UpgradeCrystalCapacity()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetCrystalCapacityUpgradeCost( m_iCrystalCapacityLevel );

    if ( m_iCrystalCapacityLevel >= BUILDING_MAX_LEVEL - 1 )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    ++m_iCrystalCapacityLevel;
    m_iCrystalCapacity = pGameParams->GetCrystalCapacity( m_iCrystalCapacityLevel );
    return true;
}

Void PlayerTown::RetrieveMana()
{
    if ( m_iManaBuffer == 0 )
        return;
    m_arrCurrencies[CURRENCY_MANA] += m_iManaBuffer;
    m_iManaBuffer = 0;
}
Void PlayerTown::RetrieveCrystals()
{
    if ( m_iCrystalBuffer == 0 )
        return;
    m_arrCurrencies[CURRENCY_CRYSTAL] += m_iCrystalBuffer;
    m_iCrystalBuffer = 0;
}

Void PlayerTown::UpdateManaAndCrystals()
{
    // Get elapsed time since last update
    TimeValue hTime = SystemFn->GetUTCTime( TIMEUNIT_SECONDS );
    TimeValue hElapsedTime = ( hTime - m_hLastUpdateTime );

    // Update
    static Float s_fInvHour = ( 1.0f / 3600.0f );

    UInt iElapsedHours = (UInt)( hElapsedTime / 3600 );
    UInt iRemainingSeconds = (UInt)( hElapsedTime % 3600 );
    UInt iManaCorrection = 0;
    UInt iCrystalCorrection = 0;

    if ( iElapsedHours > 0 ) {
        m_iManaBuffer += ( m_iManaProductionRate * iElapsedHours );
        m_iCrystalBuffer += ( m_iCrystalProductionRate * iElapsedHours );
    }

    if ( iRemainingSeconds > 0 ) {
        Float fManaPerSecond = ( s_fInvHour * (Float)m_iManaProductionRate );
        Float fCrystalsPerSecond = ( s_fInvHour * (Float)m_iCrystalProductionRate );
        Float fMana = ( fManaPerSecond * (Float)iRemainingSeconds );
        Float fCrystals = ( fCrystalsPerSecond * (Float)iRemainingSeconds );
        UInt iMana = (UInt)( MathFn->Floor(fMana) );
        UInt iCrystals = (UInt)( MathFn->Floor(fCrystals) );

        m_iManaBuffer += iMana;
        m_iCrystalBuffer += iCrystals;

        Float fManaFraction = ( fMana - (Float)iMana );
        Float fCrystalFraction = ( fCrystals - (Float)iCrystals );
        Float fManaDroppedSeconds = ( fManaFraction / fManaPerSecond );
        Float fCrystalDroppedSeconds = ( fCrystalFraction / fCrystalsPerSecond );
        Assert( fManaDroppedSeconds < 1.0f );
        Assert( fCrystalDroppedSeconds < 1.0f );

        if ( fManaDroppedSeconds > 0.5f )
            iManaCorrection = 1;
        if ( fCrystalDroppedSeconds > 0.5f )
            iCrystalCorrection = 1;
    }

    // Erase overload (!)
    if ( m_iManaBuffer > m_iManaCapacity )
        m_iManaBuffer = m_iManaCapacity;
    if ( m_iCrystalBuffer > m_iCrystalCapacity )
        m_iCrystalBuffer = m_iCrystalCapacity;

    // Update and apply correction
    m_hLastUpdateTime = ( hTime - (iManaCorrection | iCrystalCorrection) );
}

Bool PlayerTown::UnlockEssenceFusion()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetEssenceFusionUnlockCost();

    if ( m_bEssenceFusionUnlocked )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    m_bEssenceFusionUnlocked = true;
    return true;
}
Bool PlayerTown::UnlockMonsterSummoning()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterSummoningUnlockCost();

    if ( m_bMonsterSummoningUnlocked )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    m_bMonsterSummoningUnlocked = true;
    return true;
}
Bool PlayerTown::UnlockMonsterFusion()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterFusionUnlockCost();

    if ( m_bMonsterFusionUnlocked )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    m_bMonsterFusionUnlocked = true;
    return true;
}
Bool PlayerTown::UnlockMonsterPowerUp()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterPowerUpUnlockCost();

    if ( m_bMonsterPowerUpUnlocked )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    m_bMonsterPowerUpUnlocked = true;
    return true;
}
Bool PlayerTown::UnlockMonsterEvolution()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterEvolutionUnlockCost();

    if ( m_bMonsterEvolutionUnlocked )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    m_bMonsterEvolutionUnlocked = true;
    return true;
}
Bool PlayerTown::UnlockMonsterAwakening()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterAwakeningUnlockCost();

    if ( m_bMonsterAwakeningUnlocked )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    m_bMonsterAwakeningUnlocked = true;
    return true;
}
Bool PlayerTown::UnlockRunePowerUp()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetRunePowerUpUnlockCost();

    if ( m_bRunePowerUpUnlocked )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    m_bRunePowerUpUnlocked = true;
    return true;
}
Bool PlayerTown::UnlockRuneEvolution()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetRuneEvolutionUnlockCost();

    if ( m_bRuneEvolutionUnlocked )
        return false;
    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    PayCurrencyCost( pCost );

    m_bRuneEvolutionUnlocked = true;
    return true;
}

Bool PlayerTown::CanFuseEssences( MonsterElement iElement, EssenceType iType ) const
{
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_HIGH );

    if ( !m_bEssenceFusionUnlocked )
        return false;

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    UInt iCost = pGameParams->GetEssenceFusionCost( iType );

    return ( m_arrEssences[iElement][iType] >= iCost );
}
Bool PlayerTown::CanSummonMonster( const Monster * pMonster ) const
{
    Assert( pMonster != NULL );

    if ( !(pMonster->IsSummon()) )
        return false;

    return CheckScrollCost( pMonster->GetSummoningCost() );
}
Bool PlayerTown::CanFuseMonster( const Monster * pMonster, UInt arrFoodMonsters[4] ) const
{
    Assert( pMonster != NULL );

    if ( !(pMonster->IsFusion()) )
        return false;

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterFusionCost( pMonster->GetNativeRank() );
    const MonsterCost * pFusionCost = pMonster->GetFusionCost();

    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    for( UInt i = 0; i < 4; ++i ) {
        Assert( arrFoodMonsters[i] < m_arrMonsterCollection.Count() );
        const MonsterInstance & hFoodMonster = m_arrMonsterCollection[arrFoodMonsters[i]];
        if ( hFoodMonster.GetRank() != pMonster->GetNativeRank() )
            return false;
        if ( !(hFoodMonster.IsMaxLevel()) )
            return false;
        if ( !(hFoodMonster.IsAwakened()) )
            return false;
        if ( hFoodMonster.GetID() != pFusionCost->arrCost[i] )
            return false;
    }
    return true;
}
Bool PlayerTown::CanPowerUpMonster( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount ) const
{
    Assert( iTargetMonster < m_arrMonsterCollection.Count() );
    const MonsterInstance & hTargetMonster = m_arrMonsterCollection[iTargetMonster];

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    CurrencyCost hTotalCost;

    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        hTotalCost.arrCost[i] = 0;
    for( UInt i = 0; i < iFoodCount; ++i ) {
        Assert( arrFoodMonsters[i] < m_arrMonsterCollection.Count() );
        const MonsterInstance & hFoodMonster = m_arrMonsterCollection[arrFoodMonsters[i]];

        const CurrencyCost * pCost = pGameParams->GetMonsterPowerUpCost( hTargetMonster.GetRank(), hFoodMonster.GetRank() );
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            hTotalCost.arrCost[j] += pCost->arrCost[j];
    }
    return CheckCurrencyCost( &hTotalCost );
}
Bool PlayerTown::CanEvolveMonster( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount ) const
{
    Assert( iTargetMonster < m_arrMonsterCollection.Count() );
    const MonsterInstance & hTargetMonster = m_arrMonsterCollection[iTargetMonster];

    if ( hTargetMonster.IsMaxRank() || !(hTargetMonster.IsMaxLevel()) )
        return false;

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterEvolutionCost( hTargetMonster.GetRank() );

    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    UInt iTargetRank = hTargetMonster.GetRank();
    if ( iFoodCount < iTargetRank )
        return false;
    for( UInt i = 0; i < iFoodCount; ++i ) {
        Assert( arrFoodMonsters[i] < m_arrMonsterCollection.Count() );
        const MonsterInstance & hFoodMonster = m_arrMonsterCollection[arrFoodMonsters[i]];

        if ( hFoodMonster.GetRank() != iTargetRank )
            return false;
    }
    return true;
}
Bool PlayerTown::CanAwakeMonster( UInt iTargetMonster ) const
{
    Assert( iTargetMonster < m_arrMonsterCollection.Count() );
    const MonsterInstance & hMonster = m_arrMonsterCollection[iTargetMonster];

    if ( !m_bMonsterAwakeningUnlocked )
        return false;
    if ( hMonster.IsAwakened() )
        return false;

    return CheckEssenceCost( hMonster.GetAwakeningCost() );
}
Bool PlayerTown::CanPowerUpRune( RuneType iRuneType, UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount ) const
{
    Assert( iRuneType < RUNE_TYPE_COUNT );
    Assert( iTargetRune < m_arrRuneCollection[iRuneType].Count() );
    const Rune & hTargetRune = (m_arrRuneCollection[iRuneType])[iTargetRune];

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    CurrencyCost hTotalCost;
    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        hTotalCost.arrCost[i] = 0;

    for( UInt i = 0; i < iFoodCount; ++i ) {
        Assert( arrFoodRunes[i] < m_arrRuneCollection[iRuneType].Count() );
        const Rune & hFoodRune = (m_arrRuneCollection[iRuneType])[arrFoodRunes[i]];

        const CurrencyCost * pCost = pGameParams->GetRunePowerUpCost( hTargetRune.GetRank(), hFoodRune.GetRank() );
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            hTotalCost.arrCost[j] += pCost->arrCost[j];
    }
    return CheckCurrencyCost( &hTotalCost );
}
Bool PlayerTown::CanEvolveRune( RuneType iRuneType, UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount ) const
{
    Assert( iRuneType < RUNE_TYPE_COUNT );
    Assert( iTargetRune < m_arrRuneCollection[iRuneType].Count() );
    const Rune & hTargetRune = (m_arrRuneCollection[iRuneType])[iTargetRune];

    if ( hTargetRune.IsMaxRank() || !(hTargetRune.IsMaxLevel()) )
        return false;

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetRuneEvolutionCost( hTargetRune.GetRank() );

    if ( !(CheckCurrencyCost(pCost)) )
        return false;

    UInt iTargetRank = hTargetRune.GetRank();
    if ( iFoodCount < iTargetRank )
        return false;
    for( UInt i = 0; i < iFoodCount; ++i ) {
        Assert( arrFoodRunes[i] < m_arrRuneCollection[iRuneType].Count() );
        const Rune & hFoodRune = (m_arrRuneCollection[iRuneType])[arrFoodRunes[i]];

        if ( hFoodRune.GetRank() != iTargetRank )
            return false;
    }

    return true;
}

Void PlayerTown::FuseEssences( MonsterElement iElement, EssenceType iType )
{
    Assert( iElement < MONSTER_ELEMENT_COUNT );
    Assert( iType < ESSENCE_HIGH );

    Assert( m_bEssenceFusionUnlocked );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    UInt iCost = pGameParams->GetEssenceFusionCost( iType );

    m_arrEssences[iElement][iType] -= iCost;

    m_arrEssences[iElement][iType+1] += 1;
}
Void PlayerTown::SummonMonster( const Monster * pMonster )
{
    Assert( pMonster != NULL );
    Assert( pMonster->IsSummon() );
    Assert( m_arrMonsterCollection.Count() < m_iMonsterCollectionRoom );

    PayScrollCost( pMonster->GetSummoningCost() );

    MonsterInstance hMonster( pMonster );
    UInt iIndex = m_arrMonsterCollection.Search( _Compare_MonsterInstance, hMonster );
    m_arrMonsterCollection.Insert( iIndex, hMonster );
}
Void PlayerTown::FuseMonster( const Monster * pMonster, UInt arrFoodMonsters[4] )
{
    Assert( pMonster != NULL );
    Assert( pMonster->IsFusion() );
    Assert( m_arrMonsterCollection.Count() < m_iMonsterCollectionRoom );

    // Avoid indices corruption during successive removals
    QuickSort<UInt>( arrFoodMonsters, 4, _Compare_UInt, NULL );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterFusionCost( pMonster->GetNativeRank() );

    PayCurrencyCost( pCost );

    for( UInt i = 0; i < 4; ++i ) {
        Assert( arrFoodMonsters[i] < m_arrMonsterCollection.Count() );
        MonsterInstance hFoodMonster;
        m_arrMonsterCollection.Remove( arrFoodMonsters[i], hFoodMonster );
        Assert( hFoodMonster.IsPresent() );
    }

    MonsterInstance hMonster( pMonster );
    UInt iIndex = m_arrMonsterCollection.Search( _Compare_MonsterInstance, hMonster );
    m_arrMonsterCollection.Insert( iIndex, hMonster );
}
Void PlayerTown::PowerUpMonster( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount )
{
    Assert( iTargetMonster < m_arrMonsterCollection.Count() );
    MonsterInstance & hTargetMonster = m_arrMonsterCollection[iTargetMonster];

    // Avoid indices corruption during successive removals
    QuickSort<UInt>( arrFoodMonsters, iFoodCount, _Compare_UInt, NULL );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    for( UInt i = 0; i < iFoodCount; ++i ) {
        Assert( arrFoodMonsters[i] < m_arrMonsterCollection.Count() );
        MonsterInstance hFoodMonster;
        m_arrMonsterCollection.Remove( arrFoodMonsters[i], hFoodMonster );
        Assert( hFoodMonster.IsPresent() );

        const CurrencyCost * pCost = pGameParams->GetMonsterPowerUpCost( hTargetMonster.GetRank(), hFoodMonster.GetRank() );
        PayCurrencyCost( pCost );

        UInt iXP = pGameParams->GetMonsterXPFromPowerUp( hFoodMonster.GetRank(), hFoodMonster.GetLevel() );
        if ( !(hTargetMonster.IsMaxLevel()) )
            hTargetMonster.AddXP( iXP );

        if ( StringFn->Cmp(hFoodMonster.GetName(), hTargetMonster.GetName()) == 0 ) {
            UInt iSkillCount = hTargetMonster.GetSkillCount();
            UInt iValidSlotCount = 0;
            UInt arrValidSlots[4];
            for( UInt j = 0; j < iSkillCount; ++j ) {
                SkillInstance * pSkillInstance = hTargetMonster.GetSkillInstance( j );
                if ( !(pSkillInstance->IsMaxLevel()) ) {
                    arrValidSlots[iValidSlotCount] = j;
                    ++iValidSlotCount;
                }
            }
            if ( iValidSlotCount > 0 )
                hTargetMonster.SkillLevelUp( arrValidSlots[GameplayFn->GetRandomUInt(iValidSlotCount)] );
        }
    }
}
Void PlayerTown::EvolveMonster( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount )
{
    Assert( iTargetMonster < m_arrMonsterCollection.Count() );
    MonsterInstance & hTargetMonster = m_arrMonsterCollection[iTargetMonster];

    // Avoid indices corruption during successive removals
    QuickSort<UInt>( arrFoodMonsters, iFoodCount, _Compare_UInt, NULL );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetMonsterEvolutionCost( hTargetMonster.GetRank() );

    PayCurrencyCost( pCost );

    for( UInt i = 0; i < iFoodCount; ++i ) {
        Assert( arrFoodMonsters[i] < m_arrMonsterCollection.Count() );
        MonsterInstance hFoodMonster;
        m_arrMonsterCollection.Remove( arrFoodMonsters[i], hFoodMonster );
        Assert( hFoodMonster.IsPresent() );
    }

    hTargetMonster.RankUp();
}
Void PlayerTown::AwakeMonster( UInt iTargetMonster )
{
    Assert( iTargetMonster < m_arrMonsterCollection.Count() );
    MonsterInstance & hMonster = m_arrMonsterCollection[iTargetMonster];

    Assert( m_bMonsterAwakeningUnlocked );
    Assert( !(hMonster.IsAwakened()) );

    PayEssenceCost( hMonster.GetAwakeningCost() );

    hMonster.Awake();
}
Void PlayerTown::PowerUpRune( RuneType iRuneType, UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount )
{
    Assert( iRuneType < RUNE_TYPE_COUNT );
    Assert( iTargetRune < m_arrRuneCollection[iRuneType].Count() );
    Rune & hTargetRune = (m_arrRuneCollection[iRuneType])[iTargetRune];

    // Avoid indices corruption during successive removals
    QuickSort<UInt>( arrFoodRunes, iFoodCount, _Compare_UInt, NULL );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    for( UInt i = 0; i < iFoodCount; ++i ) {
        Assert( arrFoodRunes[i] < m_arrRuneCollection[iRuneType].Count() );
        Rune hFoodRune;
        m_arrRuneCollection[iRuneType].Remove( arrFoodRunes[i], hFoodRune );
        Assert( hFoodRune.IsPresent() );

        const CurrencyCost * pCost = pGameParams->GetRunePowerUpCost( hTargetRune.GetRank(), hFoodRune.GetRank() );
        PayCurrencyCost( pCost );

        ////////////////////////////////////////////////////////////
    }
}
Void PlayerTown::EvolveRune( RuneType iRuneType, UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount )
{
    Assert( iRuneType < RUNE_TYPE_COUNT );
    Assert( iTargetRune < m_arrRuneCollection[iRuneType].Count() );
    Rune & hTargetRune = (m_arrRuneCollection[iRuneType])[iTargetRune];

    // Avoid indices corruption during successive removals
    QuickSort<UInt>( arrFoodRunes, iFoodCount, _Compare_UInt, NULL );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetRuneEvolutionCost( hTargetRune.GetRank() );

    PayCurrencyCost( pCost );

    for( UInt i = 0; i < iFoodCount; ++i ) {
        Assert( arrFoodRunes[i] < m_arrRuneCollection[iRuneType].Count() );
        Rune hFoodRune;
        m_arrRuneCollection[iRuneType].Remove( arrFoodRunes[i], hFoodRune );
        Assert( hFoodRune.IsPresent() );
    }
    hTargetRune.RankUp();
}

/////////////////////////////////////////////////////////////////////////////////

Int PlayerTown::_Compare_UInt( const UInt & rLeft, const UInt & rRight, Void * /*pUserData*/ )
{
    if ( rLeft > rRight )
        return +1;
    if ( rLeft < rRight )
        return -1;
    return 0;
}
Int PlayerTown::_Compare_MonsterInstance( const MonsterInstance & rLeft, const MonsterInstance & rRight )
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
Int PlayerTown::_Compare_Rune( const Rune & rLeft, const Rune & rRight )
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
    return 0;
}

