/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Rune.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Base Skill interface
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
#include "Rune.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Rune implementation
Rune::Rune()
{
    m_iType = RUNE_TYPE_COUNT;
    m_iSlot = INVALID_OFFSET;

    m_iRank = INVALID_OFFSET;
    m_iLevel = INVALID_OFFSET;

    m_iQuality = RUNE_QUALITY_COUNT;
    m_iBonusCount = 0;
    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        m_arrBonusTypes[i] = MONSTER_STAT_COUNT;
        m_arrIsBonusRatio[i] = false;
        m_arrBonusValues[i].iValue = INVALID_OFFSET;
    }

    m_pEquippedTo = NULL;
}
Rune::Rune( RuneType iType, UInt iSlot )
{
    m_iType = iType;
    m_iSlot = iSlot;

    m_iRank = 0;
    m_iLevel = 0;

    m_iQuality = RUNE_QUALITY_COMMON;
    m_iBonusCount = 0;
    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        m_arrBonusTypes[i] = MONSTER_STAT_COUNT;
        m_arrIsBonusRatio[i] = false;
        m_arrBonusValues[i].iValue = 0;
    }

    m_pEquippedTo = NULL;
}
Rune::Rune( const Rune & rhs )
{
    m_iType = rhs.m_iType;
    m_iSlot = rhs.m_iSlot;

    m_iRank = rhs.m_iRank;
    m_iLevel = rhs.m_iLevel;

    m_iQuality = rhs.m_iQuality;
    m_iBonusCount = rhs.m_iBonusCount;
    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        m_arrBonusTypes[i] = rhs.m_arrBonusTypes[i];
        m_arrIsBonusRatio[i] = rhs.m_arrIsBonusRatio[i];
        m_arrBonusValues[i].iValue = rhs.m_arrBonusValues[i].iValue;
    }

    m_pEquippedTo = rhs.m_pEquippedTo;
}
Rune::~Rune()
{
    // nothing to do
}

Rune & Rune::operator=( const Rune & rhs )
{
    m_iType = rhs.m_iType;
    m_iSlot = rhs.m_iSlot;

    m_iRank = rhs.m_iRank;
    m_iLevel = rhs.m_iLevel;

    m_iQuality = rhs.m_iQuality;
    m_iBonusCount = rhs.m_iBonusCount;
    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        m_arrBonusTypes[i] = rhs.m_arrBonusTypes[i];
        m_arrIsBonusRatio[i] = rhs.m_arrIsBonusRatio[i];
        m_arrBonusValues[i].iValue = rhs.m_arrBonusValues[i].iValue;
    }

    m_pEquippedTo = rhs.m_pEquippedTo;

    return (*this);
}

UInt Rune::RankUp()
{
    if ( m_iRank < (RUNE_MAX_RANK - 1) ) {
        ++m_iRank;
        _UpdateStats();
    }
    return m_iRank;
}
UInt Rune::RankDown()
{
    if ( m_iRank > 0 ) {
        --m_iRank;
        _UpdateStats();
    }
    return m_iRank;
}
Void Rune::SetRank( UInt iRank )
{
    Assert( iRank < RUNE_MAX_RANK );
    if ( m_iRank != iRank ) {
        m_iRank = iRank;
        _UpdateStats();
    }
}

UInt Rune::LevelUp()
{
    if ( m_iLevel < (RUNE_MAX_LEVEL - 1) ) {
        ++m_iLevel;
        _UpdateStats();
    }
    return m_iLevel;
}
UInt Rune::LevelDown()
{
    if ( m_iLevel > 0 ) {
        --m_iLevel;
        _UpdateStats();
    }
    return m_iLevel;
}
Void Rune::SetLevel( UInt iLevel )
{
    Assert( iLevel < RUNE_MAX_LEVEL );
    if ( m_iLevel != iLevel ) {
        m_iLevel = iLevel;
        _UpdateStats();
    }
}

Void Rune::AddBonus( RuneStatistic iStat, MonsterStatistic iStatType, Bool bRatio )
{
    Assert( iStat < RUNE_STAT_COUNT );
    Assert( GameplayFn->GetGameParameters()->IsSlotAllowed(m_iSlot, iStatType, bRatio) );

    if ( m_arrBonusTypes[iStat] == MONSTER_STAT_COUNT )
        ++m_iBonusCount;

    m_arrBonusTypes[iStat] = iStatType;
    m_arrIsBonusRatio[iStat] = bRatio;
    
    _UpdateStats();
}
Void Rune::RemoveBonus( RuneStatistic iStat )
{
    Assert( iStat < RUNE_STAT_COUNT );

    if ( m_arrBonusTypes[iStat] != MONSTER_STAT_COUNT )
        --m_iBonusCount;

    m_arrBonusTypes[iStat] = MONSTER_STAT_COUNT;
    m_arrIsBonusRatio[iStat] = false;
    
    _UpdateStats();
}

UInt Rune::ComputeSellPrice() const
{
    // Ranges in [1000;375000], no random fluctuation for now ...
    static Float s_arrTypeFactor[RUNE_TYPE_COUNT] = { 1.0f, 1.0f, 1.0f, 2.0f, 3.0f, 5.0f, 3.0f, 3.0f, 3.0f, 3.0f, 4.0f, 4.0f, 4.0f, 5.0f, 5.0f, 4.0f };
    static Float s_arrRankFactor[RUNE_MAX_RANK]   = { 1.0f, 2.0f, 5.0f, 7.0f, 10.0f, 15.0f };
    static Float s_arrLevelFactor[RUNE_MAX_LEVEL] = { 1.0f, 1.2f, 1.5f, 1.7f, 2.0f, 2.2f, 2.5f, 2.7f, 3.0f, 3.2f, 3.5f, 3.7f, 4.0f, 4.2f, 4.5f, 5.0f };

    return (UInt)( MathFn->Floor(1000.0f * s_arrTypeFactor[m_iType] * s_arrRankFactor[m_iRank] * s_arrLevelFactor[m_iLevel]) );
}

/////////////////////////////////////////////////////////////////////////////////

Void Rune::_UpdateStats()
{
    const RuneLevelingStats  * pLevelingStats = GameplayFn->GetGameParameters()->GetRuneLevelingStats();

    // Update quality
    if ( m_iLevel >= 12 || m_iBonusCount >= 6 )
        m_iQuality = RUNE_QUALITY_LEGEND;
    else if ( m_iLevel >= 9 || m_iBonusCount >= 5 )
        m_iQuality = RUNE_QUALITY_EPIC;
    else if ( m_iLevel >= 6 || m_iBonusCount >= 4 )
        m_iQuality = RUNE_QUALITY_RARE;
    else if ( m_iLevel >= 3 || m_iBonusCount >= 3 )
        m_iQuality = RUNE_QUALITY_MAGIC;
    else if ( m_iBonusCount >= 2 )
        m_iQuality = RUNE_QUALITY_UNCOMMON;
    else
        m_iQuality = RUNE_QUALITY_COMMON;

    // Update primary bonus
    if ( m_arrBonusTypes[RUNE_STAT_PRIMARY] != MONSTER_STAT_COUNT ) {
        if ( m_arrIsBonusRatio[RUNE_STAT_PRIMARY] )
            m_arrBonusValues[RUNE_STAT_PRIMARY].fValue = pLevelingStats->GetPrimaryStatF( m_arrBonusTypes[RUNE_STAT_PRIMARY], m_iRank, m_iLevel );
        else
            m_arrBonusValues[RUNE_STAT_PRIMARY].iValue = pLevelingStats->GetPrimaryStatI( m_arrBonusTypes[RUNE_STAT_PRIMARY], m_iRank, m_iLevel );
    }

    // Update secondary bonus
    if ( m_arrBonusTypes[RUNE_STAT_SECONDARY] != MONSTER_STAT_COUNT ) {
        if ( m_arrIsBonusRatio[RUNE_STAT_SECONDARY] )
            m_arrBonusValues[RUNE_STAT_SECONDARY].fValue = pLevelingStats->GetSecondaryStatF( m_arrBonusTypes[RUNE_STAT_SECONDARY], m_iRank, m_iLevel );
        else
            m_arrBonusValues[RUNE_STAT_SECONDARY].iValue = pLevelingStats->GetSecondaryStatI( m_arrBonusTypes[RUNE_STAT_SECONDARY], m_iRank, m_iLevel );
    }

    // Update substat bonuses
    for ( UInt i = 0; i < 4; ++i ) {
        RuneStatistic iStat = (RuneStatistic)( RUNE_STAT_SUB_1 + i );
        if ( m_arrBonusTypes[iStat] != MONSTER_STAT_COUNT ) {
            if ( m_arrIsBonusRatio[iStat] )
                m_arrBonusValues[iStat].fValue = pLevelingStats->GetSubStatF( m_arrBonusTypes[iStat], m_iRank, m_iLevel );
            else
                m_arrBonusValues[iStat].iValue = pLevelingStats->GetSubStatI( m_arrBonusTypes[iStat], m_iRank, m_iLevel );
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////
// RuneSet implementation
RuneSet::RuneSet()
{
    m_iSetBonusCount = 0;
    for( UInt i = 0; i < 4; ++i )
        m_arrSetBonuses[i] = RUNE_TYPE_COUNT;
}
RuneSet::~RuneSet()
{
    // nothing to do
}

Void RuneSet::AddRune( Rune * pRune )
{
    Assert( pRune!= NULL && pRune->IsPresent() );
    UInt iSlot = pRune->GetSlot();
    Assert( m_arrRunes[iSlot] == NULL );

    m_arrRunes[iSlot] = pRune;

    _UpdateSetBonuses();
}
Rune * RuneSet::RemoveRune( UInt iSlot )
{
    Assert( iSlot < RUNE_SLOT_COUNT );
    Assert( m_arrRunes[iSlot] != NULL );

    Rune * pRune = m_arrRunes[iSlot];
    m_arrRunes[iSlot] = NULL;

    _UpdateSetBonuses();
    return pRune;
}

Bool RuneSet::HasSetBonus( RuneType iType, UInt * outCount ) const
{
    Assert( iType < RUNE_TYPE_COUNT );

    // Count setbonus instances
    UInt iCount = 0;
    for( UInt i = 0; i < 4; ++i ) {
        if ( m_arrSetBonuses[i] == iType )
            ++iCount;
    }

    if ( outCount != NULL )
        *outCount = iCount;
    return ( iCount > 0 );
}

Void RuneSet::CompileStats( UInt outFlatBonuses[4], Float outRatioBonuses[4], Float outSecondaryBonuses[4] ) const
{
    outFlatBonuses[0] = 0; // HP
    outFlatBonuses[1] = 0; // ATT
    outFlatBonuses[2] = 0; // DEF
    outFlatBonuses[3] = 0; // SPD

    outRatioBonuses[0] = 0.0f; // HP%
    outRatioBonuses[1] = 0.0f; // ATT%
    outRatioBonuses[2] = 0.0f; // DEF%
    outRatioBonuses[3] = 0.0f; // SPD%

    outSecondaryBonuses[0] = 0.0f; // CRITR
    outSecondaryBonuses[1] = 0.0f; // CRITD
    outSecondaryBonuses[2] = 0.0f; // ACC
    outSecondaryBonuses[3] = 0.0f; // RES

    // Enumerate all runes
    for ( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        const Rune * pRune = m_arrRunes[i];
        if ( pRune->IsNull() )
            continue;

        // Enumerate all stats
        for( UInt j = 0; j < RUNE_STAT_COUNT; ++j ) {
            RuneStatistic iRuneStat = (RuneStatistic)j;
            MonsterStatistic iBonusStat = pRune->GetBonusType( iRuneStat );
            if ( iBonusStat == MONSTER_STAT_COUNT )
                continue;

            switch( iBonusStat ) {
                case MONSTER_STAT_HEALTH:
                    if ( pRune->IsBonusRatio(iRuneStat) )
                        outRatioBonuses[0] += pRune->GetBonusValueF( iRuneStat );
                    else
                        outFlatBonuses[0] += pRune->GetBonusValueI( iRuneStat );
                    break;
                case MONSTER_STAT_ATTACK:
                    if ( pRune->IsBonusRatio(iRuneStat) )
                        outRatioBonuses[1] += pRune->GetBonusValueF( iRuneStat );
                    else
                        outFlatBonuses[1] += pRune->GetBonusValueI( iRuneStat );
                    break;
                case MONSTER_STAT_DEFENSE:
                    if ( pRune->IsBonusRatio(iRuneStat) )
                        outRatioBonuses[2] += pRune->GetBonusValueF( iRuneStat );
                    else
                        outFlatBonuses[2] += pRune->GetBonusValueI( iRuneStat );
                    break;
                case MONSTER_STAT_SPEED:
                    outFlatBonuses[3] += pRune->GetBonusValueI( iRuneStat );
                    break;
                case MONSTER_STAT_CRIT_RATE:
                    outSecondaryBonuses[0] += pRune->GetBonusValueF( iRuneStat );
                    break;
                case MONSTER_STAT_CRIT_DMG:
                    outSecondaryBonuses[1] += pRune->GetBonusValueF( iRuneStat );
                    break;
                case MONSTER_STAT_ACCURACY:
                    outSecondaryBonuses[2] += pRune->GetBonusValueF( iRuneStat );
                    break;
                case MONSTER_STAT_RESISTANCE:
                    outSecondaryBonuses[3] += pRune->GetBonusValueF( iRuneStat );
                    break;
                default: Assert( false ); break;
            }
        }
    }

    // Add relevant set bonuses
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    for( UInt i = 0; i < m_iSetBonusCount; ++i ) {
        Float fSetBonus = pGameParams->GetRuneSetStatBonus( m_arrSetBonuses[i] );
        switch( m_arrSetBonuses[i] ) {
            case RUNE_ENERGY:
                outRatioBonuses[0] += fSetBonus;
                break;
            case RUNE_FATAL:
                outRatioBonuses[1] += fSetBonus;
                break;
            case RUNE_GUARD:
                outRatioBonuses[2] += fSetBonus;
                break;
            case RUNE_SWIFT:
                outRatioBonuses[3] += fSetBonus;
                break;
            case RUNE_BLADE:
                outSecondaryBonuses[0] += fSetBonus;
                break;
            case RUNE_RAGE:
                outSecondaryBonuses[1] += fSetBonus;
                break;
            case RUNE_FOCUS:
                outSecondaryBonuses[2] += fSetBonus;
                break;
            case RUNE_ENDURE:
                outSecondaryBonuses[3] += fSetBonus;
                break;
            default: // not a stat bonus, do nothing for now
                break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void RuneSet::_UpdateSetBonuses()
{
    m_iSetBonusCount = 0;
    for( UInt i = 0; i < 4; ++i )
        m_arrSetBonuses[i] = RUNE_TYPE_COUNT;

    UInt arrCounts[RUNE_TYPE_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    // Count type occurences in the rune set
    for( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        if ( m_arrRunes[i] != NULL )
            ++( arrCounts[m_arrRunes[i]->GetType()] );
    }

    // Check occurence counts against set thresholds for each type, account for multiple instances
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        UInt iSetCount = pGameParams->GetSetCount( (RuneType)i );
        while( arrCounts[i] >= iSetCount ) {
            m_arrSetBonuses[m_iSetBonusCount] = (RuneType)i;
            ++m_iSetBonusCount;
            arrCounts[i] -= iSetCount;
        }
    }
}

