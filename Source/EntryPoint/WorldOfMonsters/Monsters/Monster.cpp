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

    for( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i )
        m_hSummoningCost.arrCost[i] = 0;

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

Void Monster::Load( XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("Monster") ) == 0 );

    // Extract data from the node
    m_iMonsterID = (MonsterID)( StringFn->ToUInt( pNode->GetAttribute(TEXT("MonsterID"))->GetValue() ) );
    StringFn->NCopy( m_strName, pNode->GetAttribute(TEXT("Name"))->GetValue(), MONSTER_NAME_LENGTH - 1 );
    StringFn->NCopy( m_strAwakenedName, pNode->GetAttribute(TEXT("AwakenedName"))->GetValue(), MONSTER_NAME_LENGTH - 1 );

    m_iType = _MonsterType_FromString( pNode->GetAttribute(TEXT("Type"))->GetValue() );
    m_iElement = _MonsterElement_FromString( pNode->GetAttribute(TEXT("Element"))->GetValue() );

    m_iNativeRank = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("NativeRank"))->GetValue() ) );

    XMLNode * pSummoningCostNode = pNode->GetChildByTag( TEXT("SummoningCost"), 0 );
    Assert( pSummoningCostNode != NULL );

    for( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i )
        m_hSummoningCost.arrCost[i] = 0;

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
    
    XMLNode * pAwakeningCostNode = pNode->GetChildByTag( TEXT("AwakeningCost"), 0 );
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

    XMLNode * pLevelingStatsNode = pNode->GetChildByTag( TEXT("MonsterLevelingStats"), 0 );
    Assert( pLevelingStatsNode != NULL );

    m_hLevelingStats.Load( pLevelingStatsNode );

    XMLNode * pSkillSetNode = pNode->GetChildByTag( TEXT("SkillSet"), 0 );
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
}

/////////////////////////////////////////////////////////////////////////////////
// MonsterInstance implementation
MonsterInstance::MonsterInstance( const Monster * pMonster ):
    m_hSkillSet(), m_hRuneSet()
{
    m_pMonster = pMonster;

    m_bAwakened = false;
    m_iRank = 0;
    m_iLevel = 0;

    m_iCurrentXP = 0;

    _UpdateBaseStats();
}
MonsterInstance::~MonsterInstance()
{
    // nothing to do
}

Void MonsterInstance::Awake()
{
    if ( !m_bAwakened ) {
        m_bAwakened = true;
        _UpdateBaseStats();
    }
}
Void MonsterInstance::UnAwake()
{
    if ( m_bAwakened ) {
        m_bAwakened = false;
        _UpdateBaseStats();
    }
}

UInt MonsterInstance::RankUp()
{
    if ( m_iRank < (MONSTER_MAX_RANK - 1) ) {
        ++m_iRank;
        m_iCurrentXP = 0;
        _UpdateBaseStats();
    }
    return m_iRank;
}
UInt MonsterInstance::RankDown()
{
    if ( m_iRank > 0 ) {
        --m_iRank;
        m_iCurrentXP = 0;
        _UpdateBaseStats();
    }
    return m_iRank;
}
Void MonsterInstance::SetRank( UInt iRank )
{
    Assert( iRank < MONSTER_MAX_RANK );
    if ( m_iRank != iRank ) {
        m_iRank = iRank;
        m_iCurrentXP = 0;
        _UpdateBaseStats();
    }
}

UInt MonsterInstance::LevelUp()
{
    if ( m_iLevel < (MONSTER_MAX_LEVELBYRANK(m_iRank) - 1) ) {
        ++m_iLevel;
        m_iCurrentXP = 0;
        _UpdateBaseStats();
    }
    return m_iLevel;
}
UInt MonsterInstance::LevelDown()
{
    if ( m_iLevel > 0 ) {
        --m_iLevel;
        m_iCurrentXP = 0;
        _UpdateBaseStats();
    }
    return m_iLevel;
}
Void MonsterInstance::SetLevel( UInt iLevel )
{
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(m_iRank) );
    if ( m_iLevel != iLevel ) {
        m_iLevel = iLevel;
        m_iCurrentXP = 0;
        _UpdateBaseStats();
    }
}

UInt MonsterInstance::AddXP( UInt iAmount )
{
    Assert( m_iLevel < (MONSTER_MAX_LEVELBYRANK(m_iRank) - 1) );

    m_iCurrentXP += iAmount;

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    UInt iLevelUps = 0;

    while( m_iLevel < (MONSTER_MAX_LEVELBYRANK(m_iRank) - 1) ) {
        UInt iNextLevelXP = pGameParams->GetMonsterXPByLevel( m_iRank, m_iLevel );
        if ( m_iCurrentXP < iNextLevelXP )
            break;

        m_iCurrentXP -= iNextLevelXP;
        ++m_iLevel;
        ++iLevelUps;
    }

    if ( iLevelUps > 0 )
        _UpdateBaseStats();
    return iLevelUps;
}

UInt MonsterInstance::SkillLevelUp( UInt iSlot )
{
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    SkillInstance * pSkillInstance = m_hSkillSet.GetSkillInstance( iSlot );

    if ( pSkillInstance->GetLevel() < (pSkillInstance->GetMaxLevel() - 1) ) {
        pSkillInstance->LevelUp();
        _UpdateEffectiveStats();
    }
    return pSkillInstance->GetLevel();
}
UInt MonsterInstance::SkillLevelDown( UInt iSlot )
{
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    SkillInstance * pSkillInstance = m_hSkillSet.GetSkillInstance( iSlot );

    if ( pSkillInstance->GetLevel() > 0 ) {
        pSkillInstance->LevelDown();
        _UpdateEffectiveStats();
    }
    return pSkillInstance->GetLevel();
}
Void MonsterInstance::SetSkillLevel( UInt iSlot, UInt iLevel )
{
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    SkillInstance * pSkillInstance = m_hSkillSet.GetSkillInstance( iSlot );

    Assert( iLevel < pSkillInstance->GetMaxLevel() );
    if ( pSkillInstance->GetLevel() != iLevel ) {
        pSkillInstance->SetLevel( iLevel );
        _UpdateEffectiveStats();
    }
}

Void MonsterInstance::EquipRune( Rune * pRune )
{
    Assert( !(pRune->IsEquipped()) );

    m_hRuneSet.AddRune( pRune );
    pRune->SetEquipped( this );

    _UpdateEffectiveStats();
}
Void MonsterInstance::UnEquipRune( UInt iSlot )
{
    Assert( iSlot < RUNE_SLOT_COUNT );

    Rune * pRune = m_hRuneSet.RemoveRune( iSlot );
    pRune->SetEquipped( NULL );

    _UpdateEffectiveStats();
}

/////////////////////////////////////////////////////////////////////////////////

Void MonsterInstance::_UpdateBaseStats()
{
    m_iBaseHealth = m_pMonster->GetLevelingStats()->GetHP( m_iRank, m_iLevel, m_bAwakened );
    m_iBaseAttack = m_pMonster->GetLevelingStats()->GetATT( m_iRank, m_iLevel, m_bAwakened );
    m_iBaseDefense = m_pMonster->GetLevelingStats()->GetDEF( m_iRank, m_iLevel, m_bAwakened );
    m_iBaseSpeed = m_pMonster->GetLevelingStats()->GetSPD( m_bAwakened );

    m_fBaseCriticalRate = m_pMonster->GetLevelingStats()->GetCritRate( m_bAwakened );
    m_fBaseCriticalDamage = m_pMonster->GetLevelingStats()->GetCritDmg( m_bAwakened );

    m_fBaseAccuracy = m_pMonster->GetLevelingStats()->GetAccuracy( m_bAwakened );
    m_fBaseResistance = m_pMonster->GetLevelingStats()->GetResistance( m_bAwakened );

    _UpdateEffectiveStats();
}
Void MonsterInstance::_UpdateEffectiveStats()
{
    // Resolve skill set
    UInt m_arrLevels[SKILL_SLOT_COUNT];
    for( UInt i = 0; i < m_hSkillSet.GetSkillCount(); ++i )
        m_arrLevels[i] = m_hSkillSet.GetSkillInstance(i)->GetLevel();

    m_hSkillSet.RemoveAll();

    UInt iSkillCount = m_pMonster->GetSkillCount( m_bAwakened );
    for( UInt i = 0; i < iSkillCount; ++i ) {
        Skill * pSkill = GameplayFn->GetSkill( m_pMonster->GetSkill(m_bAwakened,i) );
        m_hSkillSet.Add( pSkill, m_arrLevels[i] );
    }

    // Compile rune stats
    UInt arrFlatBonuses[4];       // HP, ATT, DEF, SPD
    Float arrRatioBonuses[4];     // %HP, %ATT, %DEF, %SPD
    Float arrSecondaryBonuses[4]; // CritR, CritD, Acc, Res
    m_hRuneSet.CompileStats( arrFlatBonuses, arrRatioBonuses, arrSecondaryBonuses );

    // Compute effective stats
    m_iEffectiveHealth = ( m_iBaseHealth + arrFlatBonuses[0] );
    m_iEffectiveHealth += (UInt)( MathFn->Floor(arrRatioBonuses[0] * (Float)m_iBaseHealth) );

    m_iEffectiveAttack = ( m_iBaseAttack + arrFlatBonuses[1] );
    m_iEffectiveAttack += (UInt)( MathFn->Floor(arrRatioBonuses[1] * (Float)m_iBaseAttack) );

    m_iEffectiveDefense = ( m_iBaseDefense + arrFlatBonuses[2] );
    m_iEffectiveDefense += (UInt)( MathFn->Floor(arrRatioBonuses[2] * (Float)m_iBaseDefense) );

    m_iEffectiveSpeed = ( m_iBaseSpeed + arrFlatBonuses[3] );
    m_iEffectiveSpeed += (UInt)( MathFn->Floor(arrRatioBonuses[3] * (Float)m_iBaseSpeed) );

    m_fEffectiveCriticalRate = ( m_fBaseCriticalRate + arrSecondaryBonuses[0] );
    if ( m_fEffectiveCriticalRate > 1.0f )
        m_fEffectiveCriticalRate = 1.0f;

    m_fEffectiveCriticalDamage = ( m_fBaseCriticalDamage + arrSecondaryBonuses[1] );
    // no cap !

    m_fEffectiveAccuracy = ( m_fBaseAccuracy + arrSecondaryBonuses[2] );
    if ( m_fEffectiveAccuracy > 1.0f )
        m_fEffectiveAccuracy = 1.0f;

    m_fEffectiveResistance = ( m_fBaseResistance + arrSecondaryBonuses[3] );
    if ( m_fEffectiveResistance > 1.0f )
        m_fEffectiveResistance = 1.0f;
}

