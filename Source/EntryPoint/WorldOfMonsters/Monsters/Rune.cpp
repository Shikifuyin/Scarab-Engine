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

/////////////////////////////////////////////////////////////////////////////////
// RuneLevelingStats implementation
RuneLevelingStats::RuneLevelingStats( XMLNode * pNode )
{
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("RuneLevelingStats") ) == 0 );

    // Extract data from the node
    XMLNode * pHealthTableNode = pNode->GetChildByTag( TEXT("HealthTable"), 0 );
    Assert( pHealthTableNode != NULL );
    XMLNode * pHealthRatioTableNode = pNode->GetChildByTag( TEXT("HealthRatioTable"), 0 );
    Assert( pHealthRatioTableNode != NULL );

    XMLNode * pAttackTableNode = pNode->GetChildByTag( TEXT("AttackTable"), 0 );
    Assert( pAttackTableNode != NULL );
    XMLNode * pAttackRatioTableNode = pNode->GetChildByTag( TEXT("AttackRatioTable"), 0 );
    Assert( pAttackRatioTableNode != NULL );

    XMLNode * pDefenseTableNode = pNode->GetChildByTag( TEXT("DefenseTable"), 0 );
    Assert( pDefenseTableNode != NULL );
    XMLNode * pDefenseRatioTableNode = pNode->GetChildByTag( TEXT("DefenseRatioTable"), 0 );
    Assert( pDefenseRatioTableNode != NULL );

    XMLNode * pSpeedTableNode = pNode->GetChildByTag( TEXT("SpeedTable"), 0 );
    Assert( pSpeedTableNode != NULL );

    XMLNode * pCritRateTableNode = pNode->GetChildByTag( TEXT("CritRateTable"), 0 );
    Assert( pCritRateTableNode != NULL );
    XMLNode * pCritDmgTableNode = pNode->GetChildByTag( TEXT("CritDmgTable"), 0 );
    Assert( pCritDmgTableNode != NULL );

    XMLNode * pAccuracyTableNode = pNode->GetChildByTag( TEXT("AccuracyTable"), 0 );
    Assert( pAccuracyTableNode != NULL );
    XMLNode * pResistanceTableNode = pNode->GetChildByTag( TEXT("ResistanceTable"), 0 );
    Assert( pResistanceTableNode != NULL );

    GChar arrHealth[RUNE_MAX_LEVEL][16];
    GChar arrHealthRatio[RUNE_MAX_LEVEL][16];

    GChar arrAttack[RUNE_MAX_LEVEL][16];
    GChar arrAttackRatio[RUNE_MAX_LEVEL][16];

    GChar arrDefense[RUNE_MAX_LEVEL][16];
    GChar arrDefenseRatio[RUNE_MAX_LEVEL][16];

    GChar arrSpeed[RUNE_MAX_LEVEL][16];

    GChar arrCritRate[RUNE_MAX_LEVEL][16];
    GChar arrCritDmg[RUNE_MAX_LEVEL][16];

    GChar arrAccuracy[RUNE_MAX_LEVEL][16];
    GChar arrResistance[RUNE_MAX_LEVEL][16];

    for( UInt i = 0; i < 3; ++i ) {
        XMLNode * pHealthRankNode = pHealthTableNode->GetChild( i );
        Assert( pHealthRankNode != NULL );
        XMLNode * pHealthRatioRankNode = pHealthRatioTableNode->GetChild( i );
        Assert( pHealthRatioRankNode != NULL );

        XMLNode * pAttackRankNode = pAttackTableNode->GetChild( i );
        Assert( pAttackRankNode != NULL );
        XMLNode * pAttackRatioRankNode = pAttackRatioTableNode->GetChild( i );
        Assert( pAttackRatioRankNode != NULL );

        XMLNode * pDefenseRankNode = pDefenseTableNode->GetChild( i );
        Assert( pDefenseRankNode != NULL );
        XMLNode * pDefenseRatioRankNode = pDefenseRatioTableNode->GetChild( i );
        Assert( pDefenseRatioRankNode != NULL );

        XMLNode * pSpeedRankNode = pSpeedTableNode->GetChild( i );
        Assert( pSpeedRankNode != NULL );

        XMLNode * pCritRateRankNode = pCritRateTableNode->GetChild( i );
        Assert( pCritRateRankNode != NULL );
        XMLNode * pCritDmgRankNode = pCritDmgTableNode->GetChild( i );
        Assert( pCritDmgRankNode != NULL );

        XMLNode * pAccuracyRankNode = pAccuracyTableNode->GetChild( i );
        Assert( pAccuracyRankNode != NULL );
        XMLNode * pResistanceRankNode = pResistanceTableNode->GetChild( i );
        Assert( pResistanceRankNode != NULL );

        for( UInt j = 0; j < RUNE_MAX_RANK; ++j ) {
            UInt iMaxLevel = RUNE_MAX_LEVELBYRANK( j );

            XMLNode * pHealthListNode = pHealthRankNode->GetChild( j );
            Assert( pHealthListNode != NULL && pHealthListNode->GetType() == XML_TEXT );
            XMLNode * pHealthRatioListNode = pHealthRatioRankNode->GetChild( j );
            Assert( pHealthRatioListNode != NULL && pHealthRatioListNode->GetType() == XML_TEXT );

            XMLNode * pAttackListNode = pAttackRankNode->GetChild( j );
            Assert( pAttackListNode != NULL && pAttackListNode->GetType() == XML_TEXT );
            XMLNode * pAttackRatioListNode = pAttackRatioRankNode->GetChild( j );
            Assert( pAttackRatioListNode != NULL && pAttackRatioListNode->GetType() == XML_TEXT );

            XMLNode * pDefenseListNode = pDefenseRankNode->GetChild( j );
            Assert( pDefenseListNode != NULL && pDefenseListNode->GetType() == XML_TEXT );
            XMLNode * pDefenseRatioListNode = pDefenseRatioRankNode->GetChild( j );
            Assert( pDefenseRatioListNode != NULL && pDefenseRatioListNode->GetType() == XML_TEXT );

            XMLNode * pSpeedListNode = pSpeedRankNode->GetChild( j );
            Assert( pSpeedListNode != NULL && pSpeedListNode->GetType() == XML_TEXT );

            XMLNode * pCritRateListNode = pCritRateRankNode->GetChild( j );
            Assert( pCritRateListNode != NULL && pCritRateListNode->GetType() == XML_TEXT );
            XMLNode * pCritDmgListNode = pCritDmgRankNode->GetChild( j );
            Assert( pCritDmgListNode != NULL && pCritDmgListNode->GetType() == XML_TEXT );

            XMLNode * pAccuracyListNode = pAccuracyRankNode->GetChild( j );
            Assert( pAccuracyListNode != NULL && pAccuracyListNode->GetType() == XML_TEXT );
            XMLNode * pResistanceListNode = pResistanceRankNode->GetChild( j );
            Assert( pResistanceListNode != NULL && pResistanceListNode->GetType() == XML_TEXT );

            UInt iCount = StringFn->Split( (GChar**)arrHealth, iMaxLevel, 16, ((XMLText*)pHealthListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );
            iCount = StringFn->Split( (GChar**)arrHealthRatio, iMaxLevel, 16, ((XMLText*)pHealthRatioListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );

            iCount = StringFn->Split( (GChar**)arrAttack, iMaxLevel, 16, ((XMLText*)pAttackListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );
            iCount = StringFn->Split( (GChar**)arrAttackRatio, iMaxLevel, 16, ((XMLText*)pAttackRatioListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );

            iCount = StringFn->Split( (GChar**)arrDefense, iMaxLevel, 16, ((XMLText*)pDefenseListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );
            iCount = StringFn->Split( (GChar**)arrDefenseRatio, iMaxLevel, 16, ((XMLText*)pDefenseRatioListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );

            iCount = StringFn->Split( (GChar**)arrSpeed, iMaxLevel, 16, ((XMLText*)pSpeedListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );

            iCount = StringFn->Split( (GChar**)arrCritRate, iMaxLevel, 16, ((XMLText*)pCritRateListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );
            iCount = StringFn->Split( (GChar**)arrCritDmg, iMaxLevel, 16, ((XMLText*)pCritDmgListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );

            iCount = StringFn->Split( (GChar**)arrAccuracy, iMaxLevel, 16, ((XMLText*)pAccuracyListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );
            iCount = StringFn->Split( (GChar**)arrResistance, iMaxLevel, 16, ((XMLText*)pResistanceListNode)->GetText(), TEXT(','), true );
            Assert( iCount == iMaxLevel );

            for ( UInt k = 0; k < iMaxLevel; ++k ) {
                m_arrHealthI[i][j][k] = (UInt)( StringFn->ToUInt( arrHealth[k] ) );
                m_arrHealthF[i][j][k] = StringFn->ToFloat( arrHealthRatio[k] );

                m_arrAttackI[i][j][k] = (UInt)( StringFn->ToUInt( arrAttack[k] ) );
                m_arrAttackF[i][j][k] = StringFn->ToFloat( arrAttackRatio[k] );

                m_arrDefenseI[i][j][k] = (UInt)( StringFn->ToUInt( arrDefense[k] ) );
                m_arrDefenseF[i][j][k] = StringFn->ToFloat( arrDefenseRatio[k] );

                m_arrSpeed[i][j][k] = (UInt)( StringFn->ToUInt( arrSpeed[k] ) );

                m_arrCritRate[i][j][k] = StringFn->ToFloat( arrCritRate[k] );
                m_arrCritDmg[i][j][k] = StringFn->ToFloat( arrCritDmg[k] );

                m_arrAccuracy[i][j][k] = StringFn->ToFloat( arrAccuracy[k] );
                m_arrResistance[i][j][k] = StringFn->ToFloat( arrResistance[k] );
            }
        }
    }
}
RuneLevelingStats::~RuneLevelingStats()
{
    // nothing to do
}

UInt RuneLevelingStats::GetSetCount( RuneType iType )
{
    Assert( iType < RUNE_TYPE_COUNT );
    static UInt s_arrSetCounts[RUNE_TYPE_COUNT] = {
        2, 4, 2, 4,
        2, 4, 2, 2,
        2, 2, 2, 2,
        4, 4, 4, 2
    };
    return s_arrSetCounts[iType];
}

/////////////////////////////////////////////////////////////////////////////////

const Byte RuneLevelingStats::sm_arrAllowedSlots[2][MONSTER_STAT_COUNT] = {
    { // flat
        0x3f, // MONSTER_STAT_HEALTH
        0x3f, // MONSTER_STAT_ATTACK
        0x3f, // MONSTER_STAT_DEFENSE
        0x02, // MONSTER_STAT_SPEED
        0x00, // MONSTER_STAT_CRIT_RATE  (doesn't exist)
        0x00, // MONSTER_STAT_CRIT_DMG   (doesn't exist)
        0x00, // MONSTER_STAT_ACCURACY   (doesn't exist)
        0x00  // MONSTER_STAT_RESISTANCE (doesn't exist)
    },
    { // ratio
        0x2a, // MONSTER_STAT_HEALTH
        0x2a, // MONSTER_STAT_ATTACK
        0x2a, // MONSTER_STAT_DEFENSE
        0x00, // MONSTER_STAT_SPEED (doesn't exist)
        0x08, // MONSTER_STAT_CRIT_RATE
        0x08, // MONSTER_STAT_CRIT_DMG
        0x20, // MONSTER_STAT_ACCURACY
        0x20  // MONSTER_STAT_RESISTANCE
    }
};

/////////////////////////////////////////////////////////////////////////////////
// Rune implementation
Rune::Rune( const RuneLevelingStats * pLevelingStats, RuneType iType, UInt iSlot )
{
    m_pLevelingStats = pLevelingStats;

    m_iType = iType;
    m_iSlot = iSlot;

    m_bEquipped = false;

    m_iRank = 0;
    m_iLevel = 0;

    m_iBonusCount = 1;
    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        m_arrBonusTypes[i] = MONSTER_STAT_COUNT;
        m_arrIsBonusRatio[i] = false;
        m_arrBonusValues[i].iValue = 0;
    }

    _UpdateStats();
}
Rune::~Rune()
{
    // nothing to do
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
    if ( m_iLevel < (RUNE_MAX_LEVELBYRANK(m_iRank) - 1) ) {
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
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(m_iRank) );
    if ( m_iLevel != iLevel ) {
        m_iLevel = iLevel;
        _UpdateStats();
    }
}

Void Rune::AddBonus( RuneStat iStat, MonsterStatistic iStatType, Bool bRatio )
{
    Assert( iStat < RUNE_STAT_COUNT );
    Assert( RuneLevelingStats::IsSlotAllowed(m_iSlot, iStatType, bRatio) );

    if ( m_arrBonusTypes[iStat] == MONSTER_STAT_COUNT )
        ++m_iBonusCount;

    m_arrBonusTypes[iStat] = iStatType;
    m_arrIsBonusRatio[iStat] = bRatio;
    
    _UpdateStats();
}
Void Rune::RemoveBonus( RuneStat iStat )
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
    // Update primary bonus
    if ( m_arrBonusTypes[RUNE_STAT_PRIMARY] != MONSTER_STAT_COUNT ) {
        if ( m_arrIsBonusRatio[RUNE_STAT_PRIMARY] )
            m_arrBonusValues[RUNE_STAT_PRIMARY].fValue = m_pLevelingStats->GetStatPrimaryF( m_arrBonusTypes[RUNE_STAT_PRIMARY], m_iRank, m_iLevel );
        else
            m_arrBonusValues[RUNE_STAT_PRIMARY].iValue = m_pLevelingStats->GetStatPrimaryI( m_arrBonusTypes[RUNE_STAT_PRIMARY], m_iRank, m_iLevel );
    }

    // Update secondary bonus
    if ( m_arrBonusTypes[RUNE_STAT_SECONDARY] != MONSTER_STAT_COUNT ) {
        if ( m_arrIsBonusRatio[RUNE_STAT_SECONDARY] )
            m_arrBonusValues[RUNE_STAT_SECONDARY].fValue = m_pLevelingStats->GetStatSecondaryF( m_arrBonusTypes[RUNE_STAT_SECONDARY], m_iRank, m_iLevel );
        else
            m_arrBonusValues[RUNE_STAT_SECONDARY].iValue = m_pLevelingStats->GetStatSecondaryI( m_arrBonusTypes[RUNE_STAT_SECONDARY], m_iRank, m_iLevel );
    }

    // Update substat bonuses
    for ( UInt i = 0; i < 4; ++i ) {
        RuneStat iStat = (RuneStat)( RUNE_STAT_SUB_1 + i );
        if ( m_arrBonusTypes[iStat] != MONSTER_STAT_COUNT ) {
            if ( m_arrIsBonusRatio[iStat] )
                m_arrBonusValues[iStat].fValue = m_pLevelingStats->GetStatSubF( m_arrBonusTypes[iStat], m_iRank, m_iLevel );
            else
                m_arrBonusValues[iStat].iValue = m_pLevelingStats->GetStatSubI( m_arrBonusTypes[iStat], m_iRank, m_iLevel );
        }
    }

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
}

/////////////////////////////////////////////////////////////////////////////////
// RuneSet implementation
RuneSet::RuneSet()
{
    m_iSetBonusCount = 0;
    m_arrSetBonuses[0] = RUNE_TYPE_COUNT;
    m_arrSetBonuses[1] = RUNE_TYPE_COUNT;
    m_arrSetBonuses[2] = RUNE_TYPE_COUNT;
    m_arrSetBonuses[3] = RUNE_TYPE_COUNT;

    m_arrRunes[0] = NULL;
    m_arrRunes[1] = NULL;
    m_arrRunes[2] = NULL;
    m_arrRunes[3] = NULL;
    m_arrRunes[4] = NULL;
    m_arrRunes[5] = NULL;
}
RuneSet::~RuneSet()
{
    // nothing to do
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

Void RuneSet::CompileStats( UInt arrFlatBonuses[4], Float arrRatioBonuses[3], Float arrSecondaryBonuses[4] ) const
{
    arrFlatBonuses[0] = 0; // HP
    arrFlatBonuses[1] = 0; // ATT
    arrFlatBonuses[2] = 0; // DEF
    arrFlatBonuses[3] = 0; // SPD

    arrRatioBonuses[0] = 0.0f; // HP%
    arrRatioBonuses[1] = 0.0f; // ATT%
    arrRatioBonuses[2] = 0.0f; // DEF%

    arrSecondaryBonuses[0] = 0.0f; // CRITR
    arrSecondaryBonuses[1] = 0.0f; // CRITD
    arrSecondaryBonuses[2] = 0.0f; // ACC
    arrSecondaryBonuses[3] = 0.0f; // RES

    // Enumerate all runes
    for ( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        Rune * pRune = m_arrRunes[i];
        if ( pRune == NULL )
            continue;

        // Enumerate all stats
        for( UInt j = 0; j < RUNE_STAT_COUNT; ++j ) {
            RuneStat iRuneStat = (RuneStat)j;
            MonsterStatistic iBonusStat = pRune->GetBonusType( iRuneStat );
            if ( iBonusStat == MONSTER_STAT_COUNT )
                continue;

            switch( iBonusStat ) {
                case MONSTER_STAT_HEALTH:
                    if ( pRune->IsBonusRatio(iRuneStat) )
                        arrRatioBonuses[0] += pRune->GetBonusValueF( iRuneStat );
                    else
                        arrFlatBonuses[0] += pRune->GetBonusValueI( iRuneStat );
                    break;
                case MONSTER_STAT_ATTACK:
                    if ( pRune->IsBonusRatio(iRuneStat) )
                        arrRatioBonuses[1] += pRune->GetBonusValueF( iRuneStat );
                    else
                        arrFlatBonuses[1] += pRune->GetBonusValueI( iRuneStat );
                    break;
                case MONSTER_STAT_DEFENSE:
                    if ( pRune->IsBonusRatio(iRuneStat) )
                        arrRatioBonuses[2] += pRune->GetBonusValueF( iRuneStat );
                    else
                        arrFlatBonuses[2] += pRune->GetBonusValueI( iRuneStat );
                    break;
                case MONSTER_STAT_SPEED:
                    arrFlatBonuses[3] += pRune->GetBonusValueI( iRuneStat );
                    break;
                case MONSTER_STAT_CRIT_RATE:
                    arrSecondaryBonuses[0] += pRune->GetBonusValueF( iRuneStat );
                    break;
                case MONSTER_STAT_CRIT_DMG:
                    arrSecondaryBonuses[1] += pRune->GetBonusValueF( iRuneStat );
                    break;
                case MONSTER_STAT_ACCURACY:
                    arrSecondaryBonuses[2] += pRune->GetBonusValueF( iRuneStat );
                    break;
                case MONSTER_STAT_RESISTANCE:
                    arrSecondaryBonuses[3] += pRune->GetBonusValueF( iRuneStat );
                    break;
                default: Assert( false ); break;
            }
        }
    }

    // Add relevant set bonuses
    for( UInt i = 0; i < m_iSetBonusCount; ++i ) {
        switch( m_arrSetBonuses[i] ) {
            case RUNE_ENERGY:
                arrRatioBonuses[0] += 0.15f;
                break;
            case RUNE_FATAL:
                arrRatioBonuses[1] += 0.35f;
                break;
            case RUNE_GUARD:
                arrRatioBonuses[2] += 0.15f;
                break;
            case RUNE_SWIFT:
                arrFlatBonuses[3] += 25;
                break;
            case RUNE_BLADE:
                arrSecondaryBonuses[0] += 0.12f;
                break;
            case RUNE_RAGE:
                arrSecondaryBonuses[1] += 0.40f;
                break;
            case RUNE_FOCUS:
                arrSecondaryBonuses[2] += 0.20f;
                break;
            case RUNE_ENDURE:
                arrSecondaryBonuses[3] += 0.20f;
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
    m_arrSetBonuses[0] = RUNE_TYPE_COUNT;
    m_arrSetBonuses[1] = RUNE_TYPE_COUNT;
    m_arrSetBonuses[2] = RUNE_TYPE_COUNT;
    m_arrSetBonuses[3] = RUNE_TYPE_COUNT;

    UInt arrCounts[RUNE_TYPE_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    // Count type occurences in the rune set
    for( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        if ( m_arrRunes[i] != NULL )
            ++( arrCounts[m_arrRunes[i]->GetType()] );
    }

    // Check occurence counts against set thresholds for each type, account for multiple instances
    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        UInt iSetCount = RuneLevelingStats::GetSetCount( (RuneType)i );
        while( arrCounts[i] >= iSetCount ) {
            m_arrSetBonuses[m_iSetBonusCount] = (RuneType)i;
            ++m_iSetBonusCount;
            arrCounts[i] -= iSetCount;
        }
    }
}

