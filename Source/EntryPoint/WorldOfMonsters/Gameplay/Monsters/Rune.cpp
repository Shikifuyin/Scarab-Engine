/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Gameplay/Monsters/Rune.cpp
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

    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        m_arrBonuses[i].iStat = MONSTER_STAT_COUNT;
        m_arrBonuses[i].bIsRatio = false;
    }
}
Rune::Rune( RuneType iType, UInt iSlot )
{
    m_iType = iType;
    m_iSlot = iSlot;

    m_iRank = 0;
    m_iLevel = 0;

    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        m_arrBonuses[i].iStat = MONSTER_STAT_COUNT;
        m_arrBonuses[i].bIsRatio = false;
    }
}
Rune::Rune( const Rune & rhs )
{
    m_iType = rhs.m_iType;
    m_iSlot = rhs.m_iSlot;

    m_iRank = rhs.m_iRank;
    m_iLevel = rhs.m_iLevel;

    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        m_arrBonuses[i].iStat = rhs.m_arrBonuses[i].iStat;
        m_arrBonuses[i].bIsRatio = rhs.m_arrBonuses[i].bIsRatio;
    }
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

    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        m_arrBonuses[i].iStat = rhs.m_arrBonuses[i].iStat;
        m_arrBonuses[i].bIsRatio = rhs.m_arrBonuses[i].bIsRatio;
    }

    return (*this);
}

Void Rune::Load( const XMLNode * pNode )
{
    Assert( m_iType == RUNE_TYPE_COUNT );
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("Rune")) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    m_iType = pGameParams->RuneTypeFromString( pNode->GetAttribute(TEXT("Type"))->GetValue() );
    Assert( m_iType < RUNE_TYPE_COUNT );
    m_iSlot = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("Slot"))->GetValue() ) );
    Assert( m_iSlot < RUNE_SLOT_COUNT );
    m_iRank = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("Rank"))->GetValue() ) );
    Assert( m_iRank < RUNE_MAX_RANK );
    m_iLevel = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("Level"))->GetValue() ) );
    Assert( m_iLevel < RUNE_MAX_LEVEL );

    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        const GChar * strNameI = pGameParams->RuneStatisticToString( (RuneStatistic)i );

        const XMLNode * pStatBonusNode = pNode->GetChildByTag( strNameI, 0 );
        if ( pStatBonusNode != NULL ) {
            m_arrBonuses[i].iStat = pGameParams->MonsterStatisticFromString( pStatBonusNode->GetAttribute(TEXT("Stat"))->GetValue() );
            m_arrBonuses[i].bIsRatio = ( StringFn->ToUInt(pStatBonusNode->GetAttribute(TEXT("IsRatio"))->GetValue()) != 0 );
        }
    }

    Assert( m_arrBonuses[RUNE_STAT_PRIMARY].iStat != MONSTER_STAT_COUNT );
}
Void Rune::Save( XMLNode * outNode ) const
{
    Assert( m_iType != RUNE_TYPE_COUNT );
    Assert( outNode != NULL );
    Assert( StringFn->Cmp(outNode->GetTagName(), TEXT("Rune")) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    GChar strBuffer[1024];
    const GChar * strValue;

    strValue = pGameParams->RuneTypeToString( m_iType );
    outNode->CreateAttribute( TEXT("Type"), strValue );
    StringFn->FromUInt( strBuffer, m_iSlot );
    outNode->CreateAttribute( TEXT("Slot"), strBuffer );
    StringFn->FromUInt( strBuffer, m_iRank );
    outNode->CreateAttribute( TEXT("Rank"), strBuffer );
    StringFn->FromUInt( strBuffer, m_iLevel );
    outNode->CreateAttribute( TEXT("Level"), strBuffer );

    Assert( m_arrBonuses[RUNE_STAT_PRIMARY].iStat != MONSTER_STAT_COUNT );

    for( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        if ( m_arrBonuses[i].iStat == MONSTER_STAT_COUNT )
            continue;
        const GChar * strNameI = pGameParams->RuneStatisticToString( (RuneStatistic)i );

        XMLNode * pStatBonusNode = XMLDocument::CreateNode( strNameI, true );

        strValue = pGameParams->MonsterStatisticToString( m_arrBonuses[i].iStat );
        pStatBonusNode->CreateAttribute( TEXT("Stat"), strValue );
        StringFn->FromUInt( strBuffer, m_arrBonuses[i].bIsRatio ? 1 : 0 );
        pStatBonusNode->CreateAttribute( TEXT("IsRatio"), strBuffer );

        outNode->AppendChild( pStatBonusNode );
    }
}

UInt Rune::GetBonusValueI( RuneStatistic iStat ) const
{
    Assert( iStat < RUNE_STAT_COUNT );
    Assert( m_arrBonuses[iStat].iStat != MONSTER_STAT_COUNT );
    Assert( !(m_arrBonuses[iStat].bIsRatio) );

    const RuneLevelingStats * pLevelingStats = GameplayFn->GetGameParameters()->GetRuneLevelingStats();
    switch( iStat ) {
        case RUNE_STAT_PRIMARY:   return pLevelingStats->GetPrimaryStatI( m_arrBonuses[iStat].iStat, m_iRank, m_iLevel );
        case RUNE_STAT_SECONDARY: return pLevelingStats->GetSecondaryStatI( m_arrBonuses[iStat].iStat, m_iRank, m_iLevel );
        default:                  return pLevelingStats->GetSubStatI( m_arrBonuses[iStat].iStat, m_iRank, m_iLevel );
    }
}
Float Rune::GetBonusValueF( RuneStatistic iStat ) const
{
    Assert( iStat < RUNE_STAT_COUNT );
    Assert( m_arrBonuses[iStat].iStat != MONSTER_STAT_COUNT );
    Assert( m_arrBonuses[iStat].bIsRatio );

    const RuneLevelingStats * pLevelingStats = GameplayFn->GetGameParameters()->GetRuneLevelingStats();
    switch( iStat ) {
        case RUNE_STAT_PRIMARY:   return pLevelingStats->GetPrimaryStatF( m_arrBonuses[iStat].iStat, m_iRank, m_iLevel );
        case RUNE_STAT_SECONDARY: return pLevelingStats->GetSecondaryStatF( m_arrBonuses[iStat].iStat, m_iRank, m_iLevel );
        default:                  return pLevelingStats->GetSubStatF( m_arrBonuses[iStat].iStat, m_iRank, m_iLevel );
    }
}

Void Rune::SetBonus( RuneStatistic iStat, MonsterStatistic iStatType, Bool bRatio )
{
    Assert( iStat < RUNE_STAT_COUNT );
    Assert( iStatType < MONSTER_STAT_COUNT );
    
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    Assert( pGameParams->IsRuneSlotAllowed(m_iSlot, iStatType, bRatio) );

    m_arrBonuses[iStat].iStat = iStatType;
    m_arrBonuses[iStat].bIsRatio = bRatio;
}
Void Rune::RemoveBonus( RuneStatistic iStat )
{
    Assert( iStat < RUNE_STAT_COUNT );

    m_arrBonuses[iStat].iStat = MONSTER_STAT_COUNT;
    m_arrBonuses[iStat].bIsRatio = false;
}

RuneQuality Rune::ComputeQuality() const
{
    if ( m_iLevel >= 12 )
        return RUNE_QUALITY_LEGEND;
    if ( m_iLevel >= 9 )
        return RUNE_QUALITY_EPIC;
    if ( m_iLevel >= 6 )
        return RUNE_QUALITY_RARE;
    if ( m_iLevel >= 3 )
        return RUNE_QUALITY_MAGIC;
    return RUNE_QUALITY_COMMON;
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
// RuneSet implementation
RuneSet::RuneSet()
{
    // nothing to do
}
RuneSet::~RuneSet()
{
    // nothing to do
}

Void RuneSet::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("RuneSet")) == 0 );

    UInt iChildCount = pNode->GetChildCount();
    for ( UInt i = 0; i < iChildCount; ++i ) {
        const XMLNode * pRuneNode = pNode->GetChildByTag( TEXT("Rune"), i );

        Rune hRune;
        hRune.Load( pRuneNode );

        UInt iSlot = hRune.GetSlot();
        Assert( m_arrRunes[iSlot].IsNull() );

        m_arrRunes[iSlot] = hRune;
    }
}
Void RuneSet::Save( XMLNode * outNode ) const
{
    Assert( outNode != NULL );
    Assert( StringFn->Cmp(outNode->GetTagName(), TEXT("RuneSet")) == 0 );

    for( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        if ( m_arrRunes[i].IsNull() )
            continue;

        XMLNode * pRuneNode = XMLDocument::CreateNode( TEXT("Rune"), false );
        m_arrRunes[i].Save( pRuneNode );

        outNode->AppendChild( pRuneNode );
    }
}

Rune * RuneSet::AddRune( const Rune & hRune )
{
    Assert( hRune.IsPresent() );
    UInt iSlot = hRune.GetSlot();
    Assert( m_arrRunes[iSlot].IsNull() );

    m_arrRunes[iSlot] = hRune;
    return ( m_arrRunes + iSlot );
}
Void RuneSet::RemoveRune( UInt iSlot, Rune * outRune )
{
    Assert( iSlot < RUNE_SLOT_COUNT );
    Assert( m_arrRunes[iSlot].IsPresent() );

    if ( outRune != NULL )
        *outRune = m_arrRunes[iSlot];
    m_arrRunes[iSlot] = Rune();
}

Bool RuneSet::HasSetBonus( RuneType iType, UInt * outCount ) const
{
    Assert( iType < RUNE_TYPE_COUNT );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    UInt iRequiredCount = pGameParams->GetRuneSetRequiredCount( iType );

    UInt iCount = 0;
    for( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        if ( m_arrRunes[i].IsPresent() && m_arrRunes[i].GetType() == iType )
            ++iCount;
    }

    if ( outCount != NULL )
        *outCount = ( iCount / iRequiredCount );
    return ( iCount >= iRequiredCount );
}
Void RuneSet::ComputeBonusStat( MonsterStatistic iStat, UInt * outFlatBonus, Float * outRatioBonus ) const
{
    Assert( iStat < MONSTER_STAT_COUNT );

    *outFlatBonus = 0;
    *outRatioBonus = 0.0f;
    
    // Enumerate all runes
    for( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        const Rune & hRune = m_arrRunes[i];
        if ( hRune.IsNull() )
            continue;

        // Enumerate all stats
        for( UInt j = 0; j < RUNE_STAT_COUNT; ++j ) {
            RuneStatistic iRuneStat = (RuneStatistic)j;
            if ( hRune.HasBonus(iRuneStat) && hRune.GetBonusType(iRuneStat) == iStat ) {
                if ( hRune.IsBonusRatio(iRuneStat) )
                    *outRatioBonus += hRune.GetBonusValueF( iRuneStat );
                else
                    *outFlatBonus += hRune.GetBonusValueI( iRuneStat );
            }
        }
    }

    // Set bonus
    static const RuneType s_arrMonsterStatToRuneType[MONSTER_STAT_COUNT] = {
        RUNE_ENERGY, // MONSTER_STAT_HEALTH
        RUNE_FATAL,  // MONSTER_STAT_ATTACK
        RUNE_GUARD,  // MONSTER_STAT_DEFENSE
        RUNE_SWIFT,  // MONSTER_STAT_SPEED
        RUNE_BLADE,  // MONSTER_STAT_CRIT_RATE
        RUNE_RAGE,   // MONSTER_STAT_CRIT_DMG
        RUNE_FOCUS,  // MONSTER_STAT_ACCURACY
        RUNE_ENDURE  // MONSTER_STAT_RESISTANCE
    };
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    RuneType iRuneType = s_arrMonsterStatToRuneType[iStat];

    UInt iCount = 0;
    if ( HasSetBonus(iRuneType, &iCount) )
        *outRatioBonus += ( pGameParams->GetRuneSetStatBonus(iRuneType) * (Float)iCount );
}
Void RuneSet::ComputeBonusStats( UInt outFlatBonus[MONSTER_STAT_COUNT], Float outRatioBonus[MONSTER_STAT_COUNT] ) const
{
    for( UInt i = 0; i < MONSTER_STAT_COUNT; ++i ) {
        outFlatBonus[i] = 0;
        outRatioBonus[i] = 0.0f;
    }

    // Enumerate all runes
    for( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        const Rune & hRune = m_arrRunes[i];
        if ( hRune.IsNull() )
            continue;

        // Enumerate all stats
        for( UInt j = 0; j < RUNE_STAT_COUNT; ++j ) {
            RuneStatistic iRuneStat = (RuneStatistic)j;
            if ( !(hRune.HasBonus(iRuneStat)) )
                continue;
            MonsterStatistic iStat = hRune.GetBonusType( iRuneStat );
            if ( hRune.IsBonusRatio(iRuneStat) )
                outRatioBonus[iStat] += hRune.GetBonusValueF( iRuneStat );
            else
                outFlatBonus[iStat] += hRune.GetBonusValueI( iRuneStat );
        }
    }

    // Set bonus
    static const RuneType s_arrMonsterStatToRuneType[MONSTER_STAT_COUNT] = {
        RUNE_ENERGY, // MONSTER_STAT_HEALTH
        RUNE_FATAL,  // MONSTER_STAT_ATTACK
        RUNE_GUARD,  // MONSTER_STAT_DEFENSE
        RUNE_SWIFT,  // MONSTER_STAT_SPEED
        RUNE_BLADE,  // MONSTER_STAT_CRIT_RATE
        RUNE_RAGE,   // MONSTER_STAT_CRIT_DMG
        RUNE_FOCUS,  // MONSTER_STAT_ACCURACY
        RUNE_ENDURE  // MONSTER_STAT_RESISTANCE
    };
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    UInt arrCounts[MONSTER_STAT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    for( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        if ( m_arrRunes[i].IsPresent() )
            ++(arrCounts[m_arrRunes[i].GetType()]);
    }

    for( UInt i = 0; i < MONSTER_STAT_COUNT; ++i ) {
        RuneType iRuneType = s_arrMonsterStatToRuneType[i];
        UInt iRequiredCount = pGameParams->GetRuneSetRequiredCount( iRuneType );
        UInt iCount = ( arrCounts[i] / iRequiredCount );
        if ( arrCounts[i] > iRequiredCount )
            *outRatioBonus += ( pGameParams->GetRuneSetStatBonus(iRuneType) * (Float)iCount );
    }
}


