/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Monster.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Base monster interface
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
#include "Monster.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Monster implementation
Monster::Monster():
    m_hSummoningCost(), m_hAwakeningCost(), m_hLevelingStats()
{
    m_iMonsterID = INVALID_OFFSET;
    m_strName[0] = NULLBYTE;
    m_strAwakenedName[0] = NULLBYTE;

    m_iType = MONSTER_TYPE_COUNT;
    m_iElement = MONSTER_ELEMENT_COUNT;

    m_iNativeRank = 0;

    m_bIsSummon = false;
    m_bIsFusion = false;
    for( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i )
        m_hSummoningCost.arrCost[i] = 0;
    for( UInt i = 0; i < 4; ++i )
        m_hFusionCost.arrCost[i] = INVALID_OFFSET;

    m_hAwakeningCost.iElement = MONSTER_ELEMENT_COUNT;
    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i ) {
        m_hAwakeningCost.arrMagicCost[i] = 0;
        m_hAwakeningCost.arrElementalCost[i] = 0;
    }

    m_iSkillCount = 0;
    m_iSkillCountAwaken = 0;
    for( UInt i = 0; i < SKILL_SLOT_COUNT; ++i ) {
        m_arrSkillSet[i] = INVALID_OFFSET;
        m_arrSkillSetAwaken[i] = INVALID_OFFSET;
    }
}
Monster::~Monster()
{
    // nothing to do
}

Void Monster::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("Monster") ) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    m_iMonsterID = (MonsterID)( StringFn->ToUInt( pNode->GetAttribute(TEXT("MonsterID"))->GetValue() ) );
    StringFn->NCopy( m_strName, pNode->GetAttribute(TEXT("Name"))->GetValue(), MONSTER_NAME_LENGTH - 1 );
    StringFn->NCopy( m_strAwakenedName, pNode->GetAttribute(TEXT("AwakenedName"))->GetValue(), MONSTER_NAME_LENGTH - 1 );

    m_iType = pGameParams->MonsterTypeFromString( pNode->GetAttribute(TEXT("Type"))->GetValue() );
    m_iElement = pGameParams->MonsterElementFromString( pNode->GetAttribute(TEXT("Element"))->GetValue() );

    m_iNativeRank = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("NativeRank"))->GetValue() ) );

    m_bIsSummon = ( StringFn->ToUInt(pNode->GetAttribute(TEXT("IsSummon"))->GetValue()) != 0 );
    m_bIsFusion = ( StringFn->ToUInt(pNode->GetAttribute(TEXT("IsFusion"))->GetValue()) != 0 );
    Assert( !m_bIsSummon || !m_bIsFusion ); // can't be both !

    for( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i )
        m_hSummoningCost.arrCost[i] = 0;
    for( UInt i = 0; i < 4; ++i )
        m_hFusionCost.arrCost[i] = INVALID_OFFSET;

    if ( m_bIsSummon ) {
        const XMLNode * pSummoningCostNode = pNode->GetChildByTag( TEXT( "SummoningCost" ), 0 );
        Assert( pSummoningCostNode != NULL );

        if ( pSummoningCostNode->HasAttribute(TEXT("Common")) )
            m_hSummoningCost.arrCost[SCROLL_COMMON] = (UInt)( StringFn->ToUInt( pSummoningCostNode->GetAttribute(TEXT("Common"))->GetValue() ) );
        if ( pSummoningCostNode->HasAttribute(TEXT("Mystical")) )
            m_hSummoningCost.arrCost[SCROLL_MYSTICAL] = (UInt)( StringFn->ToUInt( pSummoningCostNode->GetAttribute(TEXT("Mystical"))->GetValue() ) );
        if ( pSummoningCostNode->HasAttribute(TEXT("Legendary")) )
            m_hSummoningCost.arrCost[SCROLL_LEGENDARY] = (UInt)( StringFn->ToUInt( pSummoningCostNode->GetAttribute(TEXT("Legendary"))->GetValue() ) );
        if ( pSummoningCostNode->HasAttribute(TEXT("Fire")) )
            m_hSummoningCost.arrCost[SCROLL_FIRE] = (UInt)( StringFn->ToUInt( pSummoningCostNode->GetAttribute(TEXT("Fire"))->GetValue() ) );
        if ( pSummoningCostNode->HasAttribute(TEXT("Water")) )
            m_hSummoningCost.arrCost[SCROLL_WATER] = (UInt)( StringFn->ToUInt( pSummoningCostNode->GetAttribute(TEXT("Water"))->GetValue() ) );
        if ( pSummoningCostNode->HasAttribute(TEXT("Wind")) )
            m_hSummoningCost.arrCost[SCROLL_WIND] = (UInt)( StringFn->ToUInt( pSummoningCostNode->GetAttribute(TEXT("Wind"))->GetValue() ) );
        if ( pSummoningCostNode->HasAttribute(TEXT("Light")) )
            m_hSummoningCost.arrCost[SCROLL_LIGHT] = (UInt)( StringFn->ToUInt( pSummoningCostNode->GetAttribute(TEXT("Light"))->GetValue() ) );
        if ( pSummoningCostNode->HasAttribute(TEXT("Dark")) )
            m_hSummoningCost.arrCost[SCROLL_DARK] = (UInt)( StringFn->ToUInt( pSummoningCostNode->GetAttribute(TEXT("Dark"))->GetValue() ) );
    } else if ( m_bIsFusion ) {
        const XMLNode * pFusionCostNode = pNode->GetChildByTag( TEXT( "FusionCost" ), 0 );
        Assert( pFusionCostNode != NULL );

        m_hFusionCost.arrCost[0] = (MonsterID)( StringFn->ToUInt( pFusionCostNode->GetAttribute(TEXT("Monster0"))->GetValue() ) );
        m_hFusionCost.arrCost[1] = (MonsterID)( StringFn->ToUInt( pFusionCostNode->GetAttribute(TEXT("Monster1"))->GetValue() ) );
        m_hFusionCost.arrCost[2] = (MonsterID)( StringFn->ToUInt( pFusionCostNode->GetAttribute(TEXT("Monster2"))->GetValue() ) );
        m_hFusionCost.arrCost[3] = (MonsterID)( StringFn->ToUInt( pFusionCostNode->GetAttribute(TEXT("Monster3"))->GetValue() ) );
    }

    const XMLNode * pAwakeningCostNode = pNode->GetChildByTag( TEXT("AwakeningCost"), 0 );
    Assert( pAwakeningCostNode != NULL );

    m_hAwakeningCost.iElement = m_iElement;
    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i ) {
        m_hAwakeningCost.arrMagicCost[i] = 0;
        m_hAwakeningCost.arrElementalCost[i] = 0;
    }

    if ( pAwakeningCostNode->HasAttribute(TEXT("MagicLow")) )
        m_hAwakeningCost.arrMagicCost[ESSENCE_LOW] = (UInt)( StringFn->ToUInt( pAwakeningCostNode->GetAttribute(TEXT("MagicLow"))->GetValue() ) );
    if ( pAwakeningCostNode->HasAttribute(TEXT("MagicMid")) )
        m_hAwakeningCost.arrMagicCost[ESSENCE_MID] = (UInt)( StringFn->ToUInt( pAwakeningCostNode->GetAttribute(TEXT("MagicMid"))->GetValue() ) );
    if ( pAwakeningCostNode->HasAttribute(TEXT("MagicHigh")) )
        m_hAwakeningCost.arrMagicCost[ESSENCE_HIGH] = (UInt)( StringFn->ToUInt( pAwakeningCostNode->GetAttribute(TEXT("MagicHigh"))->GetValue() ) );
    if ( pAwakeningCostNode->HasAttribute(TEXT("ElementLow")) )
        m_hAwakeningCost.arrElementalCost[ESSENCE_LOW] = (UInt)( StringFn->ToUInt( pAwakeningCostNode->GetAttribute(TEXT("ElementLow"))->GetValue() ) );
    if ( pAwakeningCostNode->HasAttribute(TEXT("ElementMid")) )
        m_hAwakeningCost.arrElementalCost[ESSENCE_MID] = (UInt)( StringFn->ToUInt( pAwakeningCostNode->GetAttribute(TEXT("ElementMid"))->GetValue() ) );
    if ( pAwakeningCostNode->HasAttribute(TEXT("ElementHigh")) )
        m_hAwakeningCost.arrElementalCost[ESSENCE_HIGH] = (UInt)( StringFn->ToUInt( pAwakeningCostNode->GetAttribute(TEXT("ElementHigh"))->GetValue() ) );

    const XMLNode * pLevelingStatsNode = pNode->GetChildByTag( TEXT("MonsterLevelingStats"), 0 );
    Assert( pLevelingStatsNode != NULL );

    m_hLevelingStats.Load( pLevelingStatsNode );

    const XMLNode * pSkillSetNode = pNode->GetChildByTag( TEXT("SkillSet"), 0 );
    Assert( pSkillSetNode != NULL );

    m_iSkillCount = (UInt)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("SkillCount"))->GetValue()) );

    for( UInt i = 0; i < SKILL_SLOT_COUNT; ++i )
        m_arrSkillSet[i] = INVALID_OFFSET;

    if ( m_iSkillCount > 0 )
        m_arrSkillSet[0] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot0"))->GetValue()) );
    if ( m_iSkillCount > 1 )
        m_arrSkillSet[1] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot1"))->GetValue()) );
    if ( m_iSkillCount > 2 )
        m_arrSkillSet[2] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot2"))->GetValue()) );
    if ( m_iSkillCount > 3 )
        m_arrSkillSet[3] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot3"))->GetValue()) );

    pSkillSetNode = pNode->GetChildByTag( TEXT("SkillSetAwaken"), 0 );
    Assert( pSkillSetNode != NULL );

    m_iSkillCountAwaken = (UInt)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("SkillCount"))->GetValue()) );

    for( UInt i = 0; i < SKILL_SLOT_COUNT; ++i )
        m_arrSkillSetAwaken[i] = INVALID_OFFSET;

    if ( m_iSkillCountAwaken > 0 )
        m_arrSkillSetAwaken[0] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot0"))->GetValue()) );
    if ( m_iSkillCountAwaken > 1 )
        m_arrSkillSetAwaken[1] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot1"))->GetValue()) );
    if ( m_iSkillCountAwaken > 2 )
        m_arrSkillSetAwaken[2] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot2"))->GetValue()) );
    if ( m_iSkillCountAwaken > 3 )
        m_arrSkillSetAwaken[3] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot3"))->GetValue()) );
}

/////////////////////////////////////////////////////////////////////////////////
// MonsterInstance implementation
MonsterInstance::MonsterInstance():
    m_hSkillSet(), m_hRuneSet()
{
    m_pMonster = NULL;

    m_bAwakened = false;
    m_iRank = 0;
    m_iLevel = 0;

    m_iCurrentXP = 0;
}
MonsterInstance::MonsterInstance( const Monster * pMonster ):
    m_hSkillSet(), m_hRuneSet()
{
    m_pMonster = pMonster;

    m_bAwakened = false;
    m_iRank = 0;
    m_iLevel = 0;

    m_iCurrentXP = 0;
}
MonsterInstance::MonsterInstance( const MonsterInstance & rhs ):
    m_hSkillSet(), m_hRuneSet()
{
    m_pMonster = rhs.m_pMonster;

    m_bAwakened = rhs.m_bAwakened;
    m_iRank = rhs.m_iRank;
    m_iLevel = rhs.m_iLevel;

    m_iCurrentXP = rhs.m_iCurrentXP;

    for( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        if ( rhs.HasRune(i) )
            m_hRuneSet.AddRune( *(rhs.GetRune(i)) );
    }
}
MonsterInstance::~MonsterInstance()
{
    // nothing to do
}

MonsterInstance & MonsterInstance::operator=( const MonsterInstance & rhs )
{
    m_pMonster = rhs.m_pMonster;

    m_bAwakened = rhs.m_bAwakened;
    m_iRank = rhs.m_iRank;
    m_iLevel = rhs.m_iLevel;

    m_iCurrentXP = rhs.m_iCurrentXP;

    for( UInt i = 0; i < RUNE_SLOT_COUNT; ++i ) {
        if ( rhs.HasRune(i) )
            m_hRuneSet.AddRune( *(rhs.GetRune(i)) );
    }

    return (*this);
}

Void MonsterInstance::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("MonsterInstance") ) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    MonsterID iMonsterID = (MonsterID)( StringFn->ToUInt( pNode->GetAttribute(TEXT("MonsterID"))->GetValue() ) );
    m_pMonster = GameplayFn->GetMonster( iMonsterID );
    Assert( m_pMonster != NULL );

    m_bAwakened = ( StringFn->ToUInt(pNode->GetAttribute(TEXT("Awakened"))->GetValue()) != 0 );

    m_iRank = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("Rank"))->GetValue() ) );
    Assert( m_iRank < MONSTER_MAX_RANK );

    m_iLevel = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("Level"))->GetValue() ) );
    Assert( m_iLevel < MONSTER_MAX_LEVELBYRANK(m_iRank) );

    m_iCurrentXP = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("CurrentXP"))->GetValue() ) );
    Assert( m_iCurrentXP < pGameParams->GetMonsterXPByLevel(m_iRank,m_iLevel) );

    const XMLNode * pSkillSetNode = pNode->GetChildByTag( TEXT("SkillSet"), 0 );
    Assert( pSkillSetNode != NULL );
    m_hSkillSet.Load( pSkillSetNode );

    const XMLNode * pRuneSetNode = pNode->GetChildByTag( TEXT("RuneSet"), 0 );
    Assert( pRuneSetNode != NULL );
    m_hRuneSet.Load( pRuneSetNode );
}
Void MonsterInstance::Save( XMLNode * outNode ) const
{
    Assert( outNode != NULL );
    Assert( StringFn->Cmp( outNode->GetTagName(), TEXT("MonsterInstance") ) == 0 );

    GChar strBuffer[1024];

    StringFn->FromUInt( strBuffer, m_pMonster->GetID() );
    outNode->CreateAttribute( TEXT("MonsterID"), strBuffer );

    outNode->CreateAttribute( TEXT("Awakened"), m_bAwakened ? TEXT("1") : TEXT("0") );

    StringFn->FromUInt( strBuffer, m_iRank );
    outNode->CreateAttribute( TEXT("Rank"), strBuffer );

    StringFn->FromUInt( strBuffer, m_iLevel );
    outNode->CreateAttribute( TEXT("Level"), strBuffer );

    StringFn->FromUInt( strBuffer, m_iCurrentXP );
    outNode->CreateAttribute( TEXT("CurrentXP"), strBuffer );

    XMLNode * pSkillSetNode = XMLDocument::CreateNode( TEXT("SkillSet"), false );
    m_hSkillSet.Save( pSkillSetNode );
    outNode->AppendChild( pSkillSetNode );

    XMLNode * pRuneSetNode = XMLDocument::CreateNode( TEXT("RuneSet"), false );
    m_hRuneSet.Save( pRuneSetNode );
    outNode->AppendChild( pRuneSetNode );
}

UInt MonsterInstance::AddXP( UInt iAmount )
{
    UInt iMaxLevel = MONSTER_MAX_LEVELBYRANK( m_iRank );
    Assert( m_iLevel < (iMaxLevel - 1) );

    m_iCurrentXP += iAmount;

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    UInt iLevelUps = 0;

    while( m_iLevel < (iMaxLevel - 1) ) {
        UInt iNextLevelXP = pGameParams->GetMonsterXPByLevel( m_iRank, m_iLevel );
        if ( m_iCurrentXP < iNextLevelXP )
            break;

        m_iCurrentXP -= iNextLevelXP;
        ++m_iLevel;
        ++iLevelUps;
    }
    return iLevelUps;
}

Void MonsterInstance::GetBaseStats( UInt outFlatStats[4], Float outRatioStats[4] ) const
{
    outFlatStats[0] = m_pMonster->GetLevelingStats()->GetHP( m_iRank, m_iLevel, m_bAwakened );
    outFlatStats[1] = m_pMonster->GetLevelingStats()->GetATT( m_iRank, m_iLevel, m_bAwakened );
    outFlatStats[2] = m_pMonster->GetLevelingStats()->GetDEF( m_iRank, m_iLevel, m_bAwakened );
    outFlatStats[3] = m_pMonster->GetLevelingStats()->GetSPD( m_bAwakened );
    outRatioStats[0] = m_pMonster->GetLevelingStats()->GetCritRate( m_bAwakened );
    outRatioStats[1] = m_pMonster->GetLevelingStats()->GetCritDmg( m_bAwakened );
    outRatioStats[2] = m_pMonster->GetLevelingStats()->GetAccuracy( m_bAwakened );
    outRatioStats[3] = m_pMonster->GetLevelingStats()->GetResistance( m_bAwakened );
}

Void MonsterInstance::GetEffectiveStats( UInt outFlatStats[4], Float outRatioStats[4] ) const
{
    UInt arrFlatBonuses[MONSTER_STAT_COUNT];
    Float arrRatioBonuses[MONSTER_STAT_COUNT];
    m_hRuneSet.ComputeBonusStats( arrFlatBonuses, arrRatioBonuses );

    outFlatStats[0] = m_pMonster->GetLevelingStats()->GetHP( m_iRank, m_iLevel, m_bAwakened );
    outFlatStats[0] += (UInt)( arrRatioBonuses[MONSTER_STAT_HEALTH] * (Float)(outFlatStats[0]) ) + arrFlatBonuses[MONSTER_STAT_HEALTH];

    outFlatStats[1] = m_pMonster->GetLevelingStats()->GetATT( m_iRank, m_iLevel, m_bAwakened );
    outFlatStats[1] += (UInt)( arrRatioBonuses[MONSTER_STAT_ATTACK] * (Float)(outFlatStats[1]) ) + arrFlatBonuses[MONSTER_STAT_ATTACK];

    outFlatStats[2] = m_pMonster->GetLevelingStats()->GetDEF( m_iRank, m_iLevel, m_bAwakened );
    outFlatStats[2] += (UInt)( arrRatioBonuses[MONSTER_STAT_DEFENSE] * (Float)(outFlatStats[2]) ) + arrFlatBonuses[MONSTER_STAT_DEFENSE];

    outFlatStats[3] = m_pMonster->GetLevelingStats()->GetSPD( m_bAwakened );
    outFlatStats[3] += (UInt)( arrRatioBonuses[MONSTER_STAT_SPEED] * (Float)(outFlatStats[3]) ) + arrFlatBonuses[MONSTER_STAT_SPEED];

    outRatioStats[0] = m_pMonster->GetLevelingStats()->GetCritRate( m_bAwakened );
    outRatioStats[0] += arrRatioBonuses[MONSTER_STAT_CRIT_RATE];
    if ( outRatioStats[0] > 1.0f )
        outRatioStats[0] = 1.0f;

    outRatioStats[1] = m_pMonster->GetLevelingStats()->GetCritDmg( m_bAwakened );
    outRatioStats[1] += arrRatioBonuses[MONSTER_STAT_CRIT_DMG];

    outRatioStats[2] = m_pMonster->GetLevelingStats()->GetAccuracy( m_bAwakened );
    outRatioStats[2] += arrRatioBonuses[MONSTER_STAT_ACCURACY];
    if ( outRatioStats[2] > 1.0f )
        outRatioStats[2] = 1.0f;

    outRatioStats[3] = m_pMonster->GetLevelingStats()->GetResistance( m_bAwakened );
    outRatioStats[3] += arrRatioBonuses[MONSTER_STAT_RESISTANCE];
    if ( outRatioStats[3] > 1.0f )
        outRatioStats[3] = 1.0f;
}

