/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/LevelingStats.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : LevelingStats interface
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
#include "LevelingStats.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MonsterLevelingStats implementation
MonsterLevelingStats::MonsterLevelingStats()
{
    for( UInt i = 0; i < MONSTER_MAX_RANK; ++i ) {
        for( UInt j = 0; j < MONSTER_MAX_LEVEL; ++j ) {
            m_arrHealth[i][j] = 0;
            m_arrHealthAwaken[i][j] = 0;
            m_arrAttack[i][j] = 0;
            m_arrAttackAwaken[i][j] = 0;
            m_arrDefense[i][j] = 0;
            m_arrDefenseAwaken[i][j] = 0;
        }
    }
    m_iSpeed = 0;

    m_iAwakeningBonus = MONSTER_AWAKENING_BONUS_COUNT;
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
    m_iSpeed = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("BaseSpeed"))->GetValue() ) );

    m_iAwakeningBonus = _MonsterAwakeningBonus_FromString( pNode->GetAttribute(TEXT("AwakeningBonus"))->GetValue() );
    Assert( m_iAwakeningBonus < MONSTER_AWAKENING_BONUS_COUNT );

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
            m_arrHealth[i][j] = (UInt)( StringFn->ToUInt( arrHealth[j] ) );
            m_arrHealthAwaken[i][j] = (UInt)( StringFn->ToUInt( arrAwakenedHealth[j] ) );

            m_arrAttack[i][j] = (UInt)( StringFn->ToUInt( arrAttack[j] ) );
            m_arrAttackAwaken[i][j] = (UInt)( StringFn->ToUInt( arrAwakenedAttack[j] ) );

            m_arrDefense[i][j] = (UInt)( StringFn->ToUInt( arrDefense[j] ) );
            m_arrDefenseAwaken[i][j] = (UInt)( StringFn->ToUInt( arrAwakenedDefense[j] ) );
        }
    }
}

UInt MonsterLevelingStats::GetSPD( Bool bAwakened ) const
{
    if ( bAwakened ) {
        const GameParameters * pGameParams = GameplayFn->GetGameParameters();

        if ( m_iAwakeningBonus == MONSTER_AWAKENING_SPEED )
            return ( m_iSpeed + pGameParams->GetAwakeningBonusBaseSpeed() + pGameParams->GetAwakeningBonusSpeed() );

        return ( m_iSpeed + pGameParams->GetAwakeningBonusBaseSpeed() );
    }
    return m_iSpeed;
}

Float MonsterLevelingStats::GetCritRate( Bool bAwakened ) const
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    if ( bAwakened && m_iAwakeningBonus == MONSTER_AWAKENING_CRITICALRATE )
        return ( pGameParams->GetMonsterBaseCritRate() + pGameParams->GetAwakeningBonusCritRate() );
    return pGameParams->GetMonsterBaseCritRate();
}
Float MonsterLevelingStats::GetCritDmg( Bool /*bAwakened*/ ) const
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    return pGameParams->GetMonsterBaseCritDamage();
}
Float MonsterLevelingStats::GetAccuracy( Bool bAwakened ) const
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    if ( bAwakened && m_iAwakeningBonus == MONSTER_AWAKENING_ACCURACY )
        return ( pGameParams->GetMonsterBaseAccuracy() + pGameParams->GetAwakeningBonusAccuracy() );
    return pGameParams->GetMonsterBaseAccuracy();
}
Float MonsterLevelingStats::GetResistance( Bool bAwakened ) const
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    if ( bAwakened && m_iAwakeningBonus == MONSTER_AWAKENING_RESISTANCE )
        return ( pGameParams->GetMonsterBaseResistance() + pGameParams->GetAwakeningBonusResistance() );
    return pGameParams->GetMonsterBaseResistance();
}

/////////////////////////////////////////////////////////////////////////////////

MonsterAwakeningBonus MonsterLevelingStats::_MonsterAwakeningBonus_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return MONSTER_AWAKENING_;
    }
    Assert( false );
    return MONSTER_AWAKENING_BONUS_COUNT;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillLevelingStats implementation
SkillLevelingStats::SkillLevelingStats()
{
    m_iMaxLevel = 0;

    for( UInt i = 0; i < SKILL_MAX_LEVEL; ++i ) {
        m_arrBonusDamage[i] = 0.0f;
        m_arrBonusRecovery[i] = 0.0f;
        m_arrBonusStatusEffectRate[i] = 0.0f;
        m_arrBonusSpecific[i] = 0.0f;

        m_arrCooldown[i] = 0;
    }
}
SkillLevelingStats::~SkillLevelingStats()
{
    // nothing to do
}

Void SkillLevelingStats::Load( XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("SkillLevelingStats")) == 0 );

    // Extract data from the node
    m_iMaxLevel = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("MaxLevel"))->GetValue() ) );
    Assert( m_iMaxLevel <= SKILL_MAX_LEVEL );

    XMLNode * pBonusDamageNode = pNode->GetChildByTag( TEXT("BonusDamage"), 0 );
    Assert( pBonusDamageNode != NULL && pBonusDamageNode->GetType() == XML_TEXT );
    XMLNode * pBonusRecoveryNode = pNode->GetChildByTag( TEXT("BonusRecovery"), 0 );
    Assert( pBonusRecoveryNode != NULL && pBonusRecoveryNode->GetType() == XML_TEXT );
    XMLNode * pBonusStatusEffectRateNode = pNode->GetChildByTag( TEXT("BonusStatusEffectRate"), 0 );
    Assert( pBonusStatusEffectRateNode != NULL && pBonusStatusEffectRateNode->GetType() == XML_TEXT );
    XMLNode * pBonusSpecificNode = pNode->GetChildByTag( TEXT("BonusSpecific"), 0 );
    Assert( pBonusSpecificNode != NULL && pBonusSpecificNode->GetType() == XML_TEXT );

    XMLNode * pCooldownNode = pNode->GetChildByTag( TEXT("Cooldown"), 0 );
    Assert( pCooldownNode != NULL && pCooldownNode->GetType() == XML_TEXT );

    GChar arrBonusDamage[SKILL_MAX_LEVEL][16];
    GChar arrBonusRecovery[SKILL_MAX_LEVEL][16];
    GChar arrBonusStatusEffectRate[SKILL_MAX_LEVEL][16];
    GChar arrBonusSpecific[SKILL_MAX_LEVEL][16];

    GChar arrCooldown[SKILL_MAX_LEVEL][16];

    UInt iCount = StringFn->Split( (GChar**)arrBonusDamage, m_iMaxLevel, 16, ((XMLText*)pBonusDamageNode)->GetText(), TEXT(','), true );
    Assert( iCount == m_iMaxLevel );
    iCount = StringFn->Split( (GChar**)arrBonusRecovery, m_iMaxLevel, 16, ((XMLText*)pBonusRecoveryNode)->GetText(), TEXT(','), true );
    Assert( iCount == m_iMaxLevel );
    iCount = StringFn->Split( (GChar**)arrBonusStatusEffectRate, m_iMaxLevel, 16, ((XMLText*)pBonusStatusEffectRateNode)->GetText(), TEXT(','), true );
    Assert( iCount == m_iMaxLevel );
    iCount = StringFn->Split( (GChar**)arrBonusSpecific, m_iMaxLevel, 16, ((XMLText*)pBonusSpecificNode)->GetText(), TEXT(','), true );
    Assert( iCount == m_iMaxLevel );

    iCount = StringFn->Split( (GChar**)arrCooldown, m_iMaxLevel, 16, ((XMLText*)pCooldownNode)->GetText(), TEXT(','), true );
    Assert( iCount == m_iMaxLevel );

    for( UInt i = 0; i < SKILL_MAX_LEVEL; ++i ) {
        m_arrBonusDamage[i] = StringFn->ToFloat( arrBonusDamage[i] );
        m_arrBonusRecovery[i] = StringFn->ToFloat( arrBonusRecovery[i] );
        m_arrBonusStatusEffectRate[i] = StringFn->ToFloat( arrBonusStatusEffectRate[i] );
        m_arrBonusSpecific[i] = StringFn->ToFloat( arrBonusSpecific[i] );

        m_arrCooldown[i] = (UInt)( StringFn->ToUInt(arrCooldown[i]) );
    }
}

/////////////////////////////////////////////////////////////////////////////////
// RuneLevelingStats implementation
RuneLevelingStats::RuneLevelingStats()
{
    for( UInt i = 0; i < MONSTER_STAT_COUNT; ++i ) {
        for( UInt j = 0; j < RUNE_MAX_RANK; ++j ) {
            for( UInt k = 0; k < RUNE_MAX_LEVEL; ++k ) {
                m_arrPrimaryStatsI[i][j][k] = 0;
                m_arrPrimaryStatsF[i][j][k] = 0.0f;

                m_arrSecondaryStatsI[i][j][k] = 0;
                m_arrSecondaryStatsF[i][j][k] = 0.0f;

                m_arrSubStatsI[i][j][k] = 0;
                m_arrSubStatsF[i][j][k] = 0.0f;
            }
        }
    }
}
RuneLevelingStats::~RuneLevelingStats()
{
     // nothing to do
}

Void RuneLevelingStats::Load( XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("RuneLevelingStats")) == 0 );

    // Extract data from the node
    XMLNode * pPrimaryTableNodeI = pNode->GetChildByTag( TEXT("PrimaryTableI"), 0 );
    Assert( pPrimaryTableNodeI != NULL );
    XMLNode * pPrimaryTableNodeF = pNode->GetChildByTag( TEXT("PrimaryTableF"), 0 );
    Assert( pPrimaryTableNodeF != NULL );

    XMLNode * pSecondaryTableNodeI = pNode->GetChildByTag( TEXT("SecondaryTableNodeI"), 0 );
    Assert( pSecondaryTableNodeI != NULL );
    XMLNode * pSecondaryTableNodeF = pNode->GetChildByTag( TEXT("SecondaryTableNodeF"), 0 );
    Assert( pSecondaryTableNodeF != NULL );

    XMLNode * pSubTableNodeI = pNode->GetChildByTag( TEXT("SubTableNodeI"), 0 );
    Assert( pSubTableNodeI != NULL );
    XMLNode * pSubTableNodeF = pNode->GetChildByTag( TEXT("SubTableNodeF"), 0 );
    Assert( pSubTableNodeF != NULL );

    GChar arrPrimaryStatI[RUNE_MAX_LEVEL][16];
    GChar arrPrimaryStatF[RUNE_MAX_LEVEL][16];

    GChar arrSecondaryStatI[RUNE_MAX_LEVEL][16];
    GChar arrSecondaryStatF[RUNE_MAX_LEVEL][16];

    GChar arrSubStatI[RUNE_MAX_LEVEL][16];
    GChar arrSubStatF[RUNE_MAX_LEVEL][16];

    GChar strRankNodeName[16];

    for( UInt i = 0; i < MONSTER_STAT_COUNT; ++i ) {
        const GChar * strStatName = GameplayFn->GetMonsterStatName( (MonsterStatistic)i );

        XMLNode * pPrimaryListNodeI = pPrimaryTableNodeI->GetChildByTag( strStatName, 0 );
        Assert( pPrimaryListNodeI != NULL );
        XMLNode * pPrimaryListNodeF = pPrimaryTableNodeF->GetChildByTag( strStatName, 0 );
        Assert( pPrimaryListNodeF != NULL );

        XMLNode * pSecondaryListNodeI = pSecondaryTableNodeI->GetChildByTag( strStatName, 0 );
        Assert( pSecondaryListNodeI != NULL );
        XMLNode * pSecondaryListNodeF = pSecondaryTableNodeF->GetChildByTag( strStatName, 0 );
        Assert( pSecondaryListNodeF != NULL );

        XMLNode * pSubListNodeI = pSubTableNodeI->GetChildByTag( strStatName, 0 );
        Assert( pSubListNodeI != NULL );
        XMLNode * pSubListNodeF = pSubTableNodeF->GetChildByTag( strStatName, 0 );
        Assert( pSubListNodeF != NULL );

        for( UInt j = 0; j < RUNE_MAX_RANK; ++j ) {
            StringFn->Format( strRankNodeName, TEXT("Rank_%d"), j );

            XMLNode * pPrimaryNodeI = pPrimaryListNodeI->GetChildByTag( strRankNodeName, 0 );
            Assert( pPrimaryNodeI != NULL && pPrimaryNodeI->GetType() == XML_TEXT );
            XMLNode * pPrimaryNodeF = pPrimaryListNodeF->GetChildByTag( strRankNodeName, 0 );
            Assert( pPrimaryNodeF != NULL && pPrimaryNodeF->GetType() == XML_TEXT );

            XMLNode * pSecondaryNodeI = pSecondaryListNodeI->GetChildByTag( strRankNodeName, 0 );
            Assert( pSecondaryNodeI != NULL && pSecondaryNodeI->GetType() == XML_TEXT );
            XMLNode * pSecondaryNodeF = pSecondaryListNodeF->GetChildByTag( strRankNodeName, 0 );
            Assert( pSecondaryNodeF != NULL && pSecondaryNodeF->GetType() == XML_TEXT );

            XMLNode * pSubNodeI = pSubListNodeI->GetChildByTag( strRankNodeName, 0 );
            Assert( pSubNodeI != NULL && pSubNodeI->GetType() == XML_TEXT );
            XMLNode * pSubNodeF = pSubListNodeF->GetChildByTag( strRankNodeName, 0 );
            Assert( pSubNodeF != NULL && pSubNodeF->GetType() == XML_TEXT );

            UInt iCount = StringFn->Split( (GChar**)arrPrimaryStatI, RUNE_MAX_LEVEL, 16, ((XMLText*)pPrimaryNodeI)->GetText(), TEXT(','), true );
            Assert( iCount == RUNE_MAX_LEVEL );
            iCount = StringFn->Split( (GChar**)arrPrimaryStatF, RUNE_MAX_LEVEL, 16, ((XMLText*)pPrimaryNodeF)->GetText(), TEXT(','), true );
            Assert( iCount == RUNE_MAX_LEVEL );

            iCount = StringFn->Split( (GChar**)arrSecondaryStatI, RUNE_MAX_LEVEL, 16, ((XMLText*)pSecondaryNodeI)->GetText(), TEXT(','), true );
            Assert( iCount == RUNE_MAX_LEVEL );
            iCount = StringFn->Split( (GChar**)arrSecondaryStatF, RUNE_MAX_LEVEL, 16, ((XMLText*)pSecondaryNodeF)->GetText(), TEXT(','), true );
            Assert( iCount == RUNE_MAX_LEVEL );

            iCount = StringFn->Split( (GChar**)arrSubStatI, RUNE_MAX_LEVEL, 16, ((XMLText*)pSubNodeI)->GetText(), TEXT(','), true );
            Assert( iCount == RUNE_MAX_LEVEL );
            iCount = StringFn->Split( (GChar**)arrSubStatF, RUNE_MAX_LEVEL, 16, ((XMLText*)pSubNodeF)->GetText(), TEXT(','), true );
            Assert( iCount == RUNE_MAX_LEVEL );

            for( UInt k = 0; k < RUNE_MAX_LEVEL; ++k ) {
                m_arrPrimaryStatsI[i][j][k] = (UInt)( StringFn->ToUInt(arrPrimaryStatI[k]) );
                m_arrPrimaryStatsF[i][j][k] = StringFn->ToFloat( arrPrimaryStatF[k] );

                m_arrSecondaryStatsI[i][j][k] = (UInt)( StringFn->ToUInt(arrSecondaryStatI[k]) );
                m_arrSecondaryStatsF[i][j][k] = StringFn->ToFloat( arrSecondaryStatF[k] );

                m_arrSubStatsI[i][j][k] = (UInt)( StringFn->ToUInt(arrSubStatI[k]) );
                m_arrSubStatsF[i][j][k] = StringFn->ToFloat( arrSubStatF[k] );
            }
        }
    }
}

