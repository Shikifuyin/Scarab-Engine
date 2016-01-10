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
    Building( BUILDING_MONSTER_SUMMONING, BUILDING_COST_MANA, 5000 )
{
    m_pDungeon = pDungeon;

    m_bSummoningUnlocked = false;
}
BuildingMonsterSummoning::~BuildingMonsterSummoning()
{
    // nothing to do
}

Bool BuildingMonsterSummoning::UnlockSummoning()
{
    if ( m_bSummoningUnlocked )
        return true;

    if ( m_pDungeon->GetCurrency(CURRENCY_MANA) < BUILDING_MONSTER_SUMMONING_UNLOCK_COST_SUMMONING )
        return false;

    m_pDungeon->RemoveCurrency( CURRENCY_MANA, BUILDING_MONSTER_SUMMONING_UNLOCK_COST_SUMMONING );
    m_bSummoningUnlocked = true;
    return true;
}

Bool BuildingMonsterSummoning::CanSummon( Monster * pMonster ) const
{
    if ( !m_bSummoningUnlocked )
        return false;

    // Get levelingstats & element
    const MonsterLevelingStats * pLevelingStats = pMonster->GetLevelingStats();
    MonsterElement iElement = pLevelingStats->GetElement();

    // Check summoning cost
    UInt iCost = pLevelingStats->GetSummoningCostAmount( MONSTER_SUMMONING_CMN );
    if ( pLevelingStats->IsSummoningCostHighTier(MONSTER_SUMMONING_CMN) ) {
        if ( iCost > m_pDungeon->GetCurrency(CURRENCY_SCROLL_MYSTICAL) )
            return false;
    } else {
        if ( iCost > m_pDungeon->GetCurrency(CURRENCY_SCROLL_COMMON) )
            return false;
    }

    iCost = pLevelingStats->GetSummoningCostAmount( MONSTER_SUMMONING_RAR );
    if ( pLevelingStats->IsSummoningCostHighTier(MONSTER_SUMMONING_RAR) ) {
        if ( iCost > m_pDungeon->GetCurrency(CURRENCY_SCROLL_LEGENDARY) )
            return false;
    } else {
        if ( iCost > m_pDungeon->GetCurrency(CURRENCY_SCROLL_MYSTICAL) )
            return false;
    }

    iCost = pLevelingStats->GetSummoningCostAmount( MONSTER_SUMMONING_ELEMENTAL );
    CurrencyType iCurrency = CURRENCY_SCROLL_FIRE;
    switch( iElement ) {
        case MONSTER_ELEMENT_FIRE:  iCurrency = CURRENCY_SCROLL_FIRE; break;
        case MONSTER_ELEMENT_WATER: iCurrency = CURRENCY_SCROLL_WATER; break;
        case MONSTER_ELEMENT_WIND:  iCurrency = CURRENCY_SCROLL_WIND; break;
        case MONSTER_ELEMENT_LIGHT: iCurrency = CURRENCY_SCROLL_LIGHT; break;
        case MONSTER_ELEMENT_DARK:  iCurrency = CURRENCY_SCROLL_DARK; break;
        default: Assert(false); break;
    }
    if ( iCost > m_pDungeon->GetCurrency(iCurrency) )
        return false;

    // Pass !
    return true;
}
Void BuildingMonsterSummoning::Summon( Monster * pMonster )
{
    Assert( m_bSummoningUnlocked );
    Assert( m_pDungeon->GetMonsterCount() < m_pDungeon->GetMonsterCollectionRoom() );

    // Get levelingstats & element
    const MonsterLevelingStats * pLevelingStats = pMonster->GetLevelingStats();
    MonsterElement iElement = pLevelingStats->GetElement();

    // Pay summoning cost
    UInt iCost = pLevelingStats->GetSummoningCostAmount( MONSTER_SUMMONING_CMN );
    if ( pLevelingStats->IsSummoningCostHighTier(MONSTER_SUMMONING_CMN) )
        m_pDungeon->RemoveCurrency( CURRENCY_SCROLL_MYSTICAL, iCost );
    else
        m_pDungeon->RemoveCurrency( CURRENCY_SCROLL_COMMON, iCost );

    iCost = pLevelingStats->GetSummoningCostAmount( MONSTER_SUMMONING_RAR );
    if ( pLevelingStats->IsSummoningCostHighTier(MONSTER_SUMMONING_RAR) )
        m_pDungeon->RemoveCurrency( CURRENCY_SCROLL_LEGENDARY, iCost );
    else
        m_pDungeon->RemoveCurrency( CURRENCY_SCROLL_MYSTICAL, iCost );

    iCost = pLevelingStats->GetSummoningCostAmount( MONSTER_SUMMONING_ELEMENTAL );
    CurrencyType iCurrency = CURRENCY_SCROLL_FIRE;
    switch( iElement ) {
        case MONSTER_ELEMENT_FIRE:  iCurrency = CURRENCY_SCROLL_FIRE; break;
        case MONSTER_ELEMENT_WATER: iCurrency = CURRENCY_SCROLL_WATER; break;
        case MONSTER_ELEMENT_WIND:  iCurrency = CURRENCY_SCROLL_WIND; break;
        case MONSTER_ELEMENT_LIGHT: iCurrency = CURRENCY_SCROLL_LIGHT; break;
        case MONSTER_ELEMENT_DARK:  iCurrency = CURRENCY_SCROLL_DARK; break;
        default: Assert(false); break;
    }
    m_pDungeon->RemoveCurrency( iCurrency, iCost );

    // Summon the monster
    MonsterInstance * pMonsterInstance = GameplayFn->CreateMonsterInstance( pMonster );
    Assert( pMonsterInstance != NULL );

    m_pDungeon->AddMonster( pMonsterInstance );
}

