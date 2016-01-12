/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingCrystal.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingCrystal interface
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
#include "BuildingCrystal.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BuildingCrystal implementation
BuildingCrystal::BuildingCrystal( BuildingDungeon * pDungeon ):
    Building()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Cost
    const CurrencyCost * pCost = pGameParams->GetBuildingCost( BUILDING_CRYSTAL );
    for( UInt i = 0; i < CURRENCY_COUNT; ++i )
        m_hCost.arrCost[i] = pCost->arrCost[i];

    // Dungeon access
    m_pDungeon = pDungeon;

    // Crystals
    m_iProductionRateLevel = 0;
    m_iProductionRate = 1;

    m_iCapacityLevel = 0;
    m_iCapacity = 10;

    m_iCrystalBuffer = 0;
    m_hLastUpdateTime = 0;
}
BuildingCrystal::~BuildingCrystal()
{
    // nothing to do
}

Bool BuildingCrystal::UpgradeProductionRate()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetCrystalProductionRateUpgradeCost();

    if ( m_iProductionRateLevel >= BUILDING_MAX_LEVEL - 1 )
        return false;
    if ( !(m_pDungeon->CheckCurrencyCost(pCost)) )
        return false;

    static UInt s_arrProductionRateByLevel[BUILDING_MAX_LEVEL] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };

    m_pDungeon->PayCurrencyCost( pCost );

    ++m_iProductionRateLevel;
    m_iProductionRate = s_arrProductionRateByLevel[m_iProductionRateLevel];
    return true;
}

Bool BuildingCrystal::UpgradeCapacity()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    const CurrencyCost * pCost = pGameParams->GetCrystalCapacityUpgradeCost();

    if ( m_iCapacityLevel >= BUILDING_MAX_LEVEL - 1 )
        return false;
    if ( !(m_pDungeon->CheckCurrencyCost(pCost)) )
        return false;

    static UInt s_arrCapacityByLevel[BUILDING_MAX_LEVEL] = {
        10, 20, 50, 100, 150, 200, 250, 300, 400, 500
    };

    m_pDungeon->PayCurrencyCost( pCost );

    ++m_iCapacityLevel;
    m_iCapacity = s_arrCapacityByLevel[m_iCapacityLevel];
    return true;
}

Void BuildingCrystal::RetrieveCrystals()
{
    if ( m_iCrystalBuffer == 0 )
        return;

    m_pDungeon->AddCurrency( CURRENCY_CRYSTAL, m_iCrystalBuffer );
    m_iCrystalBuffer = 0;
}
Void BuildingCrystal::UpdateCrystals()
{
    // Get elapsed time since last update
    TimeValue hTime = SystemFn->GetUTCTime( TIMEUNIT_SECONDS );
    TimeValue hElapsedTime = ( hTime - m_hLastUpdateTime );

    // Update
    static Float s_fInvHour = ( 1.0f / 3600.0f );

    UInt iElapsedHours = (UInt)( hElapsedTime / 3600 );
    UInt iRemainingSeconds = (UInt)( hElapsedTime % 3600 );
    UInt iCorrection = 0;

    if ( iElapsedHours > 0 )
        m_iCrystalBuffer += ( m_iProductionRate * iElapsedHours );

    if ( iRemainingSeconds > 0 ) {
        Float fCrystalsPerSecond = ( s_fInvHour * (Float)m_iProductionRate );
        Float fCrystals = ( fCrystalsPerSecond * (Float)iRemainingSeconds );
        UInt iCrystals = (UInt)( MathFn->Floor(fCrystals) );

        m_iCrystalBuffer += iCrystals;

        Float fCrystalFraction = ( fCrystals - (Float)iCrystals );
        Float fDroppedSeconds = ( fCrystalFraction / fCrystalsPerSecond );
        Assert( fDroppedSeconds < 1.0f );

        if ( fDroppedSeconds > 0.5f )
            iCorrection = 1;
    }

    // Erase overload (!)
    if ( m_iCrystalBuffer > m_iCapacity )
        m_iCrystalBuffer = m_iCapacity;

    // Done
    m_hLastUpdateTime = ( hTime - iCorrection );
}

