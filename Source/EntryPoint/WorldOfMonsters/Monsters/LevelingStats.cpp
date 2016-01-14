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

Void MonsterLevelingStats::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("MonsterLevelingStats")) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    m_iSpeed = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("BaseSpeed"))->GetValue() ) );

    m_iAwakeningBonus = pGameParams->MonsterAwakeningBonusFromString( pNode->GetAttribute(TEXT("AwakeningBonus"))->GetValue() );
    Assert( m_iAwakeningBonus < MONSTER_AWAKENING_BONUS_COUNT );

    const XMLNode * pHealthTableNode = pNode->GetChildByTag( TEXT("HealthTable"), 0 );
    Assert( pHealthTableNode != NULL );
    const XMLNode * pAwakenedHealthTableNode = pNode->GetChildByTag( TEXT("AwakenedHealthTable"), 0 );
    Assert( pAwakenedHealthTableNode != NULL );

    const XMLNode * pAttackTableNode = pNode->GetChildByTag( TEXT("AttackTable"), 0 );
    Assert( pAttackTableNode != NULL );
    const XMLNode * pAwakenedAttackTableNode = pNode->GetChildByTag( TEXT("AwakenedAttackTable"), 0 );
    Assert( pAwakenedAttackTableNode != NULL );

    const XMLNode * pDefenseTableNode = pNode->GetChildByTag( TEXT("DefenseTable"), 0 );
    Assert( pDefenseTableNode != NULL );
    const XMLNode * pAwakenedDefenseTableNode = pNode->GetChildByTag( TEXT("AwakenedDefenseTable"), 0 );
    Assert( pAwakenedDefenseTableNode != NULL );

    for( UInt i = 0; i < MONSTER_MAX_RANK; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Rank_%d"), i );

        const XMLNode * pHealthListNode = pHealthTableNode->GetChildByTag( strNameI, 0 );
        Assert( pHealthListNode != NULL );
        const XMLNode * pAwakenedHealthListNode = pAwakenedHealthTableNode->GetChildByTag( strNameI, 0 );
        Assert( pAwakenedHealthListNode != NULL );

        const XMLNode * pAttackListNode = pAttackTableNode->GetChildByTag( strNameI, 0 );
        Assert( pAttackListNode != NULL );
        const XMLNode * pAwakenedAttackListNode = pAwakenedAttackTableNode->GetChildByTag( strNameI, 0 );
        Assert( pAwakenedAttackListNode != NULL );

        const XMLNode * pDefenseListNode = pDefenseTableNode->GetChildByTag( strNameI, 0 );
        Assert( pDefenseListNode != NULL );
        const XMLNode * pAwakenedDefenseListNode = pAwakenedDefenseTableNode->GetChildByTag( strNameI, 0 );
        Assert( pAwakenedDefenseListNode != NULL );

        UInt iMaxLevel = MONSTER_MAX_LEVELBYRANK( i );
        for( UInt j = 0; j < iMaxLevel; ++j ) {
            GChar strNameJ[16];
            StringFn->Format( strNameJ, TEXT("Level_%d"), j );

            m_arrHealth[i][j] = (UInt)( StringFn->ToUInt( pHealthListNode->GetAttribute(strNameJ)->GetValue() ) );
            m_arrHealthAwaken[i][j] = (UInt)( StringFn->ToUInt( pAwakenedHealthListNode->GetAttribute(strNameJ)->GetValue() ) );

            m_arrAttack[i][j] = (UInt)( StringFn->ToUInt( pAttackListNode->GetAttribute(strNameJ)->GetValue() ) );
            m_arrAttackAwaken[i][j] = (UInt)( StringFn->ToUInt( pAwakenedAttackListNode->GetAttribute(strNameJ)->GetValue() ) );

            m_arrDefense[i][j] = (UInt)( StringFn->ToUInt( pDefenseListNode->GetAttribute(strNameJ)->GetValue() ) );
            m_arrDefenseAwaken[i][j] = (UInt)( StringFn->ToUInt( pAwakenedDefenseListNode->GetAttribute(strNameJ)->GetValue() ) );
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

Void SkillLevelingStats::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("SkillLevelingStats")) == 0 );

    m_iMaxLevel = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("MaxLevel"))->GetValue() ) );
    Assert( m_iMaxLevel <= SKILL_MAX_LEVEL );

    const XMLNode * pBonusDamageNode = pNode->GetChildByTag( TEXT("BonusDamage"), 0 );
    Assert( pBonusDamageNode != NULL );
    const XMLNode * pBonusRecoveryNode = pNode->GetChildByTag( TEXT("BonusRecovery"), 0 );
    Assert( pBonusRecoveryNode != NULL );
    const XMLNode * pBonusStatusEffectRateNode = pNode->GetChildByTag( TEXT("BonusStatusEffectRate"), 0 );
    Assert( pBonusStatusEffectRateNode != NULL );
    const XMLNode * pBonusSpecificNode = pNode->GetChildByTag( TEXT("BonusSpecific"), 0 );
    Assert( pBonusSpecificNode != NULL );

    const XMLNode * pCooldownNode = pNode->GetChildByTag( TEXT("Cooldown"), 0 );
    Assert( pCooldownNode != NULL );

    for( UInt i = 0; i < SKILL_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        m_arrBonusDamage[i] = StringFn->ToFloat( pBonusDamageNode->GetAttribute(strNameI)->GetValue() );
        m_arrBonusRecovery[i] = StringFn->ToFloat( pBonusRecoveryNode->GetAttribute(strNameI)->GetValue() );
        m_arrBonusStatusEffectRate[i] = StringFn->ToFloat( pBonusStatusEffectRateNode->GetAttribute(strNameI)->GetValue() );
        m_arrBonusSpecific[i] = StringFn->ToFloat( pBonusSpecificNode->GetAttribute(strNameI)->GetValue() );

        m_arrCooldown[i] = (UInt)( StringFn->ToUInt( pCooldownNode->GetAttribute(strNameI)->GetValue() ) );
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

Void RuneLevelingStats::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("RuneLevelingStats")) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    const XMLNode * pPrimaryTableNodeI = pNode->GetChildByTag( TEXT("PrimaryTableI"), 0 );
    Assert( pPrimaryTableNodeI != NULL );
    const XMLNode * pPrimaryTableNodeF = pNode->GetChildByTag( TEXT("PrimaryTableF"), 0 );
    Assert( pPrimaryTableNodeF != NULL );

    const XMLNode * pSecondaryTableNodeI = pNode->GetChildByTag( TEXT("SecondaryTableNodeI"), 0 );
    Assert( pSecondaryTableNodeI != NULL );
    const XMLNode * pSecondaryTableNodeF = pNode->GetChildByTag( TEXT("SecondaryTableNodeF"), 0 );
    Assert( pSecondaryTableNodeF != NULL );

    const XMLNode * pSubTableNodeI = pNode->GetChildByTag( TEXT("SubTableNodeI"), 0 );
    Assert( pSubTableNodeI != NULL );
    const XMLNode * pSubTableNodeF = pNode->GetChildByTag( TEXT("SubTableNodeF"), 0 );
    Assert( pSubTableNodeF != NULL );

    for( UInt i = 0; i < MONSTER_STAT_COUNT; ++i ) {
        const GChar * strNameI = pGameParams->MonsterStatisticToString( (MonsterStatistic)i );

        const XMLNode * pPrimaryListNodeI = pPrimaryTableNodeI->GetChildByTag( strNameI, 0 );
        Assert( pPrimaryListNodeI != NULL );
        const XMLNode * pPrimaryListNodeF = pPrimaryTableNodeF->GetChildByTag( strNameI, 0 );
        Assert( pPrimaryListNodeF != NULL );

        const XMLNode * pSecondaryListNodeI = pSecondaryTableNodeI->GetChildByTag( strNameI, 0 );
        Assert( pSecondaryListNodeI != NULL );
        const XMLNode * pSecondaryListNodeF = pSecondaryTableNodeF->GetChildByTag( strNameI, 0 );
        Assert( pSecondaryListNodeF != NULL );

        const XMLNode * pSubListNodeI = pSubTableNodeI->GetChildByTag( strNameI, 0 );
        Assert( pSubListNodeI != NULL );
        const XMLNode * pSubListNodeF = pSubTableNodeF->GetChildByTag( strNameI, 0 );
        Assert( pSubListNodeF != NULL );

        for( UInt j = 0; j < RUNE_MAX_RANK; ++j ) {
            GChar strNameJ[16];
            StringFn->Format( strNameJ, TEXT("Rank_%d"), j );

            const XMLNode * pPrimaryNodeI = pPrimaryListNodeI->GetChildByTag( strNameJ, 0 );
            Assert( pPrimaryNodeI != NULL );
            const XMLNode * pPrimaryNodeF = pPrimaryListNodeF->GetChildByTag( strNameJ, 0 );
            Assert( pPrimaryNodeF != NULL );

            const XMLNode * pSecondaryNodeI = pSecondaryListNodeI->GetChildByTag( strNameJ, 0 );
            Assert( pSecondaryNodeI != NULL );
            const XMLNode * pSecondaryNodeF = pSecondaryListNodeF->GetChildByTag( strNameJ, 0 );
            Assert( pSecondaryNodeF != NULL );

            const XMLNode * pSubNodeI = pSubListNodeI->GetChildByTag( strNameJ, 0 );
            Assert( pSubNodeI != NULL );
            const XMLNode * pSubNodeF = pSubListNodeF->GetChildByTag( strNameJ, 0 );
            Assert( pSubNodeF != NULL );

            for( UInt k = 0; k < RUNE_MAX_LEVEL; ++k ) {
                GChar strNameK[16];
                StringFn->Format( strNameK, TEXT("Level_%d"), k );

                m_arrPrimaryStatsI[i][j][k] = (UInt)( StringFn->ToUInt( pPrimaryNodeI->GetAttribute(strNameK)->GetValue() ) );
                m_arrPrimaryStatsF[i][j][k] = StringFn->ToFloat( pPrimaryNodeF->GetAttribute(strNameK)->GetValue() );

                m_arrSecondaryStatsI[i][j][k] = (UInt)( StringFn->ToUInt( pSecondaryNodeI->GetAttribute(strNameK)->GetValue() ) );
                m_arrSecondaryStatsF[i][j][k] = StringFn->ToFloat( pSecondaryNodeF->GetAttribute(strNameK)->GetValue() );

                m_arrSubStatsI[i][j][k] = (UInt)( StringFn->ToUInt( pSubNodeI->GetAttribute(strNameK)->GetValue() ) );
                m_arrSubStatsF[i][j][k] = StringFn->ToFloat( pSubNodeF->GetAttribute(strNameK)->GetValue() );
            }
        }
    }
}

