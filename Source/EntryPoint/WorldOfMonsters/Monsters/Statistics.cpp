/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Statistics.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Statistics interface
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
#include "Statistics.h"

/////////////////////////////////////////////////////////////////////////////////
// MonsterLevelingStats implementation
MonsterLevelingStats::MonsterLevelingStats()
{
    m_iType = MONSTER_TYPE_COUNT;
    m_iElement = MONSTER_ELEMENT_COUNT;

    m_iNativeRank = 0;

    m_iAwakeningBonus = MONSTER_AWAKENING_BONUS_COUNT;
    for( UInt i = 0; i < MONSTER_AWAKENING_COST_COUNT; ++i )
        m_arrAwakeningCosts[i] = 0;

    for( UInt i = 0; i < MONSTER_MAX_RANK; ++i ) {
        for( UInt j = 0; j < MONSTER_MAX_LEVEL; ++j ) {
            m_arrHealth[0][i][j] = 0;
            m_arrHealth[1][i][j] = 0;
            m_arrAttack[0][i][j] = 0;
            m_arrAttack[1][i][j] = 0;
            m_arrDefense[0][i][j] = 0;
            m_arrDefense[1][i][j] = 0;
        }
    }

    m_iBaseSpeed = 0;
}
MonsterLevelingStats::~MonsterLevelingStats()
{
    // nothing to do
}

Void MonsterLevelingStats::Load( XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("MonsterLevelingStats")) == 0 );

    // Extract data from the node
    m_iType = (MonsterType)( StringFn->ToUInt( pNode->GetAttribute(TEXT("Type"))->GetValue() ) );
    Assert( m_iType < MONSTER_TYPE_COUNT );

    m_iElement = (MonsterElement)( StringFn->ToUInt( pNode->GetAttribute(TEXT("Element"))->GetValue() ) );
    Assert( m_iElement < MONSTER_ELEMENT_COUNT );

    m_iNativeRank = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("NativeRank"))->GetValue() ) );
    Assert( m_iNativeRank < MONSTER_MAX_RANK - 1 ); // Native 6* are disallowed !

    m_iAwakeningBonus = (MonsterAwakeningBonus)( StringFn->ToUInt( pNode->GetAttribute(TEXT("AwakeningBonus"))->GetValue() ) );
    Assert( m_iAwakeningBonus < MONSTER_AWAKENING_BONUS_COUNT );

    m_arrAwakeningCosts[MONSTER_AWAKENING_ELEMENTAL_CMN] = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("AwakeningCost_ELT_CMN"))->GetValue() ) );
    m_arrAwakeningCosts[MONSTER_AWAKENING_ELEMENTAL_RAR] = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("AwakeningCost_ELT_RAR"))->GetValue() ) );
    m_arrAwakeningCosts[MONSTER_AWAKENING_MAGIC_CMN] = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("AwakeningCost_MAG_CMN"))->GetValue() ) );
    m_arrAwakeningCosts[MONSTER_AWAKENING_MAGIC_RAR] = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("AwakeningCost_MAG_RAR"))->GetValue() ) );

    m_iBaseSpeed = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("BaseSpeed"))->GetValue() ) );

    XMLNode * pHealthTableNode = pNode->GetChildByTag( TEXT("HealthTable"), 0 );
    Assert( pHealthTableNode != NULL );
    XMLNode * pAwakenedHealthTableNode = pNode->GetChildByTag( TEXT("AwakenedHealthTable"), 0 );
    Assert( pAwakenedHealthTableNode != NULL );

    XMLNode * pAttackTableNode = pNode->GetChildByTag( TEXT("AttackTable"), 0 );
    Assert( pAttackTableNode != NULL );
    XMLNode * pAwakenedAttackTableNode = pNode->GetChildByTag( TEXT("AwakenedAttackTable"), 0 );
    Assert( pAwakenedAttackTableNode != NULL );

    XMLNode * pDefenseTableNode = pNode->GetChildByTag( TEXT("DefenseTable"), 0 );
    Assert( pDefenseTableNode != NULL );
    XMLNode * pAwakenedDefenseTableNode = pNode->GetChildByTag( TEXT("AwakenedDefenseTable"), 0 );
    Assert( pAwakenedDefenseTableNode != NULL );

    GChar arrHealth[MONSTER_MAX_LEVEL][16];
    GChar arrAwakenedHealth[MONSTER_MAX_LEVEL][16];

    GChar arrAttack[MONSTER_MAX_LEVEL][16];
    GChar arrAwakenedAttack[MONSTER_MAX_LEVEL][16];

    GChar arrDefense[MONSTER_MAX_LEVEL][16];
    GChar arrAwakenedDefense[MONSTER_MAX_LEVEL][16];

    for( UInt i = 0; i < MONSTER_MAX_RANK; ++i ) {
        UInt iMaxLevel = MONSTER_MAX_LEVELBYRANK( i );

        XMLNode * pHealthListNode = pHealthTableNode->GetChild( i );
        Assert( pHealthListNode != NULL && pHealthListNode->GetType() == XML_TEXT );
        XMLNode * pAwakenedHealthListNode = pAwakenedHealthTableNode->GetChild( i );
        Assert( pAwakenedHealthListNode != NULL && pAwakenedHealthListNode->GetType() == XML_TEXT );

        XMLNode * pAttackListNode = pAttackTableNode->GetChild( i );
        Assert( pAttackListNode != NULL && pAttackListNode->GetType() == XML_TEXT );
        XMLNode * pAwakenedAttackListNode = pAwakenedAttackTableNode->GetChild( i );
        Assert( pAwakenedAttackListNode != NULL && pAwakenedAttackListNode->GetType() == XML_TEXT );

        XMLNode * pDefenseListNode = pDefenseTableNode->GetChild( i );
        Assert( pDefenseListNode != NULL && pDefenseListNode->GetType() == XML_TEXT );
        XMLNode * pAwakenedDefenseListNode = pAwakenedDefenseTableNode->GetChild( i );
        Assert( pAwakenedDefenseListNode != NULL && pAwakenedDefenseListNode->GetType() == XML_TEXT );

        UInt iCount = StringFn->Split( (GChar**)arrHealth, iMaxLevel, 16, ((XMLText*)pHealthListNode)->GetText(), TEXT(','), true );
        Assert( iCount == iMaxLevel );
        iCount = StringFn->Split( (GChar**)arrAwakenedHealth, iMaxLevel, 16, ((XMLText*)pAwakenedHealthListNode)->GetText(), TEXT(','), true );
        Assert( iCount == iMaxLevel );

        iCount = StringFn->Split( (GChar**)arrAttack, iMaxLevel, 16, ((XMLText*)pAttackListNode)->GetText(), TEXT(','), true );
        Assert( iCount == iMaxLevel );
        iCount = StringFn->Split( (GChar**)arrAwakenedAttack, iMaxLevel, 16, ((XMLText*)pAwakenedAttackListNode)->GetText(), TEXT(','), true );
        Assert( iCount == iMaxLevel );

        iCount = StringFn->Split( (GChar**)arrDefense, iMaxLevel, 16, ((XMLText*)pDefenseListNode)->GetText(), TEXT(','), true );
        Assert( iCount == iMaxLevel );
        iCount = StringFn->Split( (GChar**)arrAwakenedDefense, iMaxLevel, 16, ((XMLText*)pAwakenedDefenseListNode)->GetText(), TEXT(','), true );
        Assert( iCount == iMaxLevel );

        for( UInt j = 0; j < iMaxLevel; ++j ) {
            m_arrHealth[0][i][j] = (UInt)( StringFn->ToUInt( arrHealth[j] ) );
            m_arrHealth[1][i][j] = (UInt)( StringFn->ToUInt( arrAwakenedHealth[j] ) );

            m_arrAttack[0][i][j] = (UInt)( StringFn->ToUInt( arrAttack[j] ) );
            m_arrAttack[1][i][j] = (UInt)( StringFn->ToUInt( arrAwakenedAttack[j] ) );

            m_arrDefense[0][i][j] = (UInt)( StringFn->ToUInt( arrDefense[j] ) );
            m_arrDefense[1][i][j] = (UInt)( StringFn->ToUInt( arrAwakenedDefense[j] ) );
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////
// MonsterStats implementation
MonsterStats::MonsterStats( const MonsterLevelingStats * pLevelingStats )
{
    m_pLevelingStats = pLevelingStats;

    m_iRank = m_pLevelingStats->GetNativeRank();
    m_iLevel = 0;

    m_bAwakened = false;

    _UpdateStats();
}
MonsterStats::~MonsterStats()
{
    // nothing to do
}

Void MonsterStats::Awake()
{
    if ( !m_bAwakened ) {
        m_bAwakened = true;
        _UpdateStats();
    }
}
Void MonsterStats::UnAwake()
{
    if ( m_bAwakened ) {
        m_bAwakened = false;
        _UpdateStats();
    }
}

UInt MonsterStats::RankUp()
{
    if ( m_iRank < (MONSTER_MAX_RANK - 1) ) {
        ++m_iRank;
        _UpdateStats();
    }
    return m_iRank;
}
UInt MonsterStats::RankDown()
{
    if ( m_iRank > 0 ) {
        --m_iRank;
        _UpdateStats();
    }
    return m_iRank;
}
Void MonsterStats::SetRank( UInt iRank )
{
    Assert( iRank < MONSTER_MAX_RANK );
    if ( m_iRank != iRank ) {
        m_iRank = iRank;
        _UpdateStats();
    }
}

UInt MonsterStats::LevelUp()
{
    if ( m_iLevel < (MONSTER_MAX_LEVELBYRANK(m_iRank) - 1) ) {
        ++m_iLevel;
        _UpdateStats();
    }
    return m_iLevel;
}
UInt MonsterStats::LevelDown()
{
    if ( m_iLevel > 0 ) {
        --m_iLevel;
        _UpdateStats();
    }
    return m_iLevel;
}
Void MonsterStats::SetLevel( UInt iLevel )
{
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(m_iRank) );
    if ( m_iLevel != iLevel ) {
        m_iLevel = iLevel;
        _UpdateStats();
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void MonsterStats::_UpdateStats()
{
    m_iHealth = m_pLevelingStats->GetHP( m_iRank, m_iLevel, m_bAwakened );
    m_iAttack = m_pLevelingStats->GetATT( m_iRank, m_iLevel, m_bAwakened );
    m_iDefense = m_pLevelingStats->GetDEF( m_iRank, m_iLevel, m_bAwakened );
    m_iSpeed = m_pLevelingStats->GetSPD( m_bAwakened );

    m_fCriticalRate = m_pLevelingStats->GetCritRate( m_bAwakened );
    m_fCriticalDamage = m_pLevelingStats->GetCritDmg( m_bAwakened );

    m_fAccuracy = m_pLevelingStats->GetAccuracy( m_bAwakened );
    m_fResistance = m_pLevelingStats->GetResistance( m_bAwakened );
}

