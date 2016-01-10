/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingMana.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingMana interface
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
#include "BuildingMana.h"

/////////////////////////////////////////////////////////////////////////////////
// BuildingMana implementation
BuildingMana::BuildingMana( BuildingDungeon * pDungeon ):
    Building( BUILDING_MANA, BUILDING_COST_MANA, 1 ) // Built-in
{
    m_pDungeon = pDungeon;

    m_iProductionRateLevel = 0;
    m_iProductionRate = 1;

    m_iCapacityLevel = 0;
    m_iCapacity = 10;

    m_iManaBuffer = 0;
    m_hLastUpdateTime = 0;
}
BuildingMana::~BuildingMana()
{
    // nothing to do
}

Bool BuildingMana::UpgradeProductionRate()
{
    if ( m_iProductionRateLevel >= BUILDING_MANA_PRODUCTION_RATE_MAX_LEVEL )
        return false;
    if ( m_pDungeon->GetCurrency(CURRENCY_GLORYPOINTS) < BUILDING_MANA_PRODUCTION_RATE_UPGRADE_COST )
        return false;

    static UInt s_arrProductionRateByLevel[BUILDING_MANA_PRODUCTION_RATE_MAX_LEVEL] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };

    m_pDungeon->RemoveCurrency( CURRENCY_GLORYPOINTS, BUILDING_MANA_PRODUCTION_RATE_UPGRADE_COST );

    ++m_iProductionRateLevel;
    m_iProductionRate = s_arrProductionRateByLevel[m_iProductionRateLevel];
    return true;
}

Bool BuildingMana::UpgradeCapacity()
{
    if ( m_iCapacityLevel >= BUILDING_MANA_CAPACITY_MAX_LEVEL )
        return false;
    if ( m_pDungeon->GetCurrency(CURRENCY_GLORYPOINTS) < BUILDING_MANA_CAPACITY_UPGRADE_COST )
        return false;

    static UInt s_arrCapacityByLevel[BUILDING_MANA_CAPACITY_MAX_LEVEL] = {
        10, 20, 50, 100, 150, 200, 250, 300, 400, 500
    };

    m_pDungeon->RemoveCurrency( CURRENCY_GLORYPOINTS, BUILDING_MANA_CAPACITY_UPGRADE_COST );

    ++m_iCapacityLevel;
    m_iCapacity = s_arrCapacityByLevel[m_iCapacityLevel];
    return true;
}

Void BuildingMana::RetrieveMana()
{
    if ( m_iManaBuffer == 0 )
        return;

    m_pDungeon->AddCurrency( CURRENCY_MANA, m_iManaBuffer );
    m_iManaBuffer = 0;
}
Void BuildingMana::UpdateMana()
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
        m_iManaBuffer += ( m_iProductionRate * iElapsedHours );

    if ( iRemainingSeconds > 0 ) {
        Float fManaPerSecond = ( s_fInvHour * (Float)m_iProductionRate );
        Float fMana = ( fManaPerSecond * (Float)iRemainingSeconds );
        UInt iMana = (UInt)( MathFn->Floor(fMana) );

        m_iManaBuffer += iMana;

        Float fManaFraction = ( fMana - (Float)iMana );
        Float fDroppedSeconds = ( fManaFraction / fManaPerSecond );
        Assert( fDroppedSeconds < 1.0f );

        if ( fDroppedSeconds > 0.5f )
            iCorrection = 1;
    }

    // Erase overload (!)
    if ( m_iManaBuffer > m_iCapacity )
        m_iManaBuffer = m_iCapacity;

    // Done
    m_hLastUpdateTime = ( hTime - iCorrection );
}

