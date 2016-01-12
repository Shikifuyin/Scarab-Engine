/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Skill.cpp
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
#include "Skill.h"

/////////////////////////////////////////////////////////////////////////////////
// Skill implementation
Skill::Skill( XMLNode * pNode ):
    m_hLevelingStats()
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("Skill")) == 0 );

    m_iSkillID = (SkillID)( StringFn->ToUInt(pNode->GetAttribute(TEXT("SkillID"))->GetValue()) );
    StringFn->NCopy( m_strName, pNode->GetAttribute(TEXT("Name"))->GetValue(), SKILL_NAME_LENGTH - 1 );

    XMLNode * pLevelingStatsNode = pNode->GetChildByTag( TEXT("SkillLevelingStats"), 0 );
    Assert( pLevelingStatsNode != NULL );

    m_hLevelingStats.Load( pLevelingStatsNode );
}
Skill::~Skill()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

SkillType Skill::_SkillType_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return SKILL_TYPE_;
    }
    Assert( false );
    return SKILL_TYPE_COUNT;
}
SkillStat Skill::_SkillStat_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return SKILL_STAT_;
    }
    Assert( false );
    return SKILL_STAT_COUNT;
}
SkillEffectType Skill::_SkillEffectType_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return SKILLEFFECT_;
    }
    Assert( false );
    return SKILLEFFECT_COUNT;
}

/////////////////////////////////////////////////////////////////////////////////
// ActiveSkill implementation
ActiveSkill::ActiveSkill( XMLNode * pNode ):
    Skill( pNode )
{
    for ( UInt i = 0; i < SKILL_ACTIVE_COUNT; ++i )
        m_arrEffectCounts[i] = 0;

    XMLNode * pActiveSkillDescriptorNode = pNode->GetChildByTag( TEXT("ActiveSkillDescriptor"), 0 );
    Assert( pActiveSkillDescriptorNode != NULL );

    m_bIsAttack = ( StringFn->ToUInt(pActiveSkillDescriptorNode->GetAttribute(TEXT("IsAttack"))->GetValue()) != 0 );

    UInt iCount = pActiveSkillDescriptorNode->GetChildCount();
    for( UInt i = 0; i < iCount; ++i ) {
        XMLNode * pActiveEffectDescriptorNode = pActiveSkillDescriptorNode->GetChildByTag( TEXT("ActiveEffectDescriptor"), i );
        SkillActiveType iActiveType = _SkillActiveType_FromString( pActiveEffectDescriptorNode->GetAttribute(TEXT("ActiveType"))->GetValue() );

        XMLNode * pEffectDescriptorNode = pActiveEffectDescriptorNode->GetChildByTag( TEXT("SkillEffectDescriptor"), 0 );
        SkillEffectType iEffectType = _SkillEffectType_FromString( pEffectDescriptorNode->GetAttribute(TEXT("EffectType"))->GetValue() );

        SkillEffect * pEffect = NULL;
        switch( iEffectType ) {
            case SKILLEFFECT_DAMAGE: pEffect = New SkillEffectDamage( pEffectDescriptorNode ); break;
            case SKILLEFFECT_HEAL:   pEffect = New SkillEffectHeal( pEffectDescriptorNode ); break;
            case SKILLEFFECT_ATB:    pEffect = New SkillEffectATB( pEffectDescriptorNode ); break;
            case SKILLEFFECT_STATUS: pEffect = New SkillEffectStatus( pEffectDescriptorNode ); break;
            default: Assert(false); break;
        }

        m_arrEffects[iActiveType][m_arrEffectCounts[iActiveType]] = pEffect;
        ++(m_arrEffectCounts[iActiveType]);
    }
}
ActiveSkill::~ActiveSkill()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

SkillActiveType ActiveSkill::_SkillActiveType_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return SKILL_ACTIVE_;
    }
    Assert( false );
    return SKILL_ACTIVE_COUNT;
}

/////////////////////////////////////////////////////////////////////////////////
// PassiveSkill implementation
PassiveSkill::PassiveSkill( XMLNode * pSkillNode ):
    Skill( pSkillNode )
{
    for ( UInt i = 0; i < SKILL_PASSIVE_COUNT; ++i )
        m_arrEffectCounts[i] = 0;

    XMLNode * pPassiveSkillDescriptorNode = pSkillNode->GetChildByTag( TEXT("PassiveSkillDescriptor"), 0 );
    Assert( pPassiveSkillDescriptorNode != NULL );

    UInt iCount = pPassiveSkillDescriptorNode->GetChildCount();
    for( UInt i = 0; i < iCount; ++i ) {
        XMLNode * pPassiveEffectDescriptorNode = pPassiveSkillDescriptorNode->GetChildByTag( TEXT("PassiveEffectDescriptor"), i );
        SkillPassiveType iPassiveType = _SkillPassiveType_FromString( pPassiveEffectDescriptorNode->GetAttribute(TEXT("PassiveType"))->GetValue() );

        XMLNode * pEffectDescriptorNode = pPassiveEffectDescriptorNode->GetChildByTag( TEXT("SkillEffectDescriptor"), 0 );
        SkillEffectType iEffectType = _SkillEffectType_FromString( pEffectDescriptorNode->GetAttribute(TEXT("EffectType"))->GetValue() );

        SkillEffect * pEffect = NULL;
        switch( iEffectType ) {
            case SKILLEFFECT_DAMAGE: pEffect = New SkillEffectDamage( pEffectDescriptorNode ); break;
            case SKILLEFFECT_HEAL:   pEffect = New SkillEffectHeal( pEffectDescriptorNode ); break;
            case SKILLEFFECT_ATB:    pEffect = New SkillEffectATB( pEffectDescriptorNode ); break;
            case SKILLEFFECT_STATUS: pEffect = New SkillEffectStatus( pEffectDescriptorNode ); break;
            default: Assert(false); break;
        }

        m_arrEffects[iPassiveType][m_arrEffectCounts[iPassiveType]] = pEffect;
        ++(m_arrEffectCounts[iPassiveType]);
    }
}
PassiveSkill::~PassiveSkill()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

SkillPassiveType PassiveSkill::_SkillPassiveType_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return SKILL_PASSIVE_;
    }
    Assert( false );
    return SKILL_PASSIVE_COUNT;
}

/////////////////////////////////////////////////////////////////////////////////
// LeaderSkill implementation
LeaderSkill::LeaderSkill( XMLNode * pSkillNode ):
    Skill( pSkillNode )
{
    XMLNode * pLeaderSkillDescriptorNode = pSkillNode->GetChildByTag( TEXT("LeaderSkillDescriptor"), 0 );
    Assert( pLeaderSkillDescriptorNode != NULL );

    m_iBonusStat = _MonsterStatistic_FromString( pLeaderSkillDescriptorNode->GetAttribute(TEXT("Stat"))->GetValue() );
    m_fBonusAmount = StringFn->ToFloat( pLeaderSkillDescriptorNode->GetAttribute(TEXT("Amount"))->GetValue() );

    m_iConstraint = _SkillLeaderConstraint_FromString( pLeaderSkillDescriptorNode->GetAttribute(TEXT("Constraint"))->GetValue() );
}
LeaderSkill::~LeaderSkill()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

MonsterStatistic LeaderSkill::_MonsterStatistic_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return MONSTER_STAT_;
    }
    Assert( false );
    return MONSTER_STAT_COUNT;
}
SkillLeaderConstraint LeaderSkill::_SkillLeaderConstraint_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return SKILL_LEADERCONSTRAINT_;
    }
    Assert( false );
    return SKILL_LEADERCONSTRAINT_NONE;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillInstance implementation
SkillInstance::SkillInstance()
{
    m_pSkill = NULL;

    m_iLevel = 0;

    m_fBonusDamage = 0.0f;
    m_fBonusRecovery = 0.0f;
    m_fBonusStatusEffectRate = 0.0f;
    m_fBonusSpecific = 0.0f;

    m_iCooldown = 0;
}
SkillInstance::SkillInstance( Skill * pSkill )
{
    Assert( pSkill != NULL );

    m_pSkill = pSkill;

    m_iLevel = 0;

    m_fBonusDamage = 0.0f;
    m_fBonusRecovery = 0.0f;
    m_fBonusStatusEffectRate = 0.0f;
    m_fBonusSpecific = 0.0f;

    m_iCooldown = 0;

    _UpdateEffectiveStats();
}
SkillInstance::SkillInstance( const SkillInstance & rhs )
{
    m_pSkill = rhs.m_pSkill;

    m_iLevel = rhs.m_iLevel;

    m_fBonusDamage = rhs.m_fBonusDamage;
    m_fBonusRecovery = rhs.m_fBonusRecovery;
    m_fBonusStatusEffectRate = rhs.m_fBonusStatusEffectRate;
    m_fBonusSpecific = rhs.m_fBonusSpecific;

    m_iCooldown = rhs.m_iCooldown;
}
SkillInstance::~SkillInstance()
{
    // nothing to do
}

SkillInstance & SkillInstance::operator=( const SkillInstance & rhs )
{
    m_pSkill = rhs.m_pSkill;

    m_iLevel = rhs.m_iLevel;

    m_fBonusDamage = rhs.m_fBonusDamage;
    m_fBonusRecovery = rhs.m_fBonusRecovery;
    m_fBonusStatusEffectRate = rhs.m_fBonusStatusEffectRate;
    m_fBonusSpecific = rhs.m_fBonusSpecific;

    m_iCooldown = rhs.m_iCooldown;

    return (*this);
}

UInt SkillInstance::LevelUp()
{
    if ( m_iLevel < (m_pSkill->GetLevelingStats()->GetMaxLevel() - 1) ) {
        ++m_iLevel;
        _UpdateEffectiveStats();
    }
    return m_iLevel;
}
UInt SkillInstance::LevelDown()
{
    if ( m_iLevel > 0 ) {
        --m_iLevel;
        _UpdateEffectiveStats();
    }
    return m_iLevel;
}
Void SkillInstance::SetLevel( UInt iLevel )
{
    Assert( iLevel < m_pSkill->GetLevelingStats()->GetMaxLevel() );
    if ( m_iLevel != iLevel ) {
        m_iLevel = iLevel;
        _UpdateEffectiveStats();
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void SkillInstance::_UpdateEffectiveStats()
{
    const SkillLevelingStats * pLevelingStats = m_pSkill->GetLevelingStats();

    m_fBonusDamage = 0.0f;
    m_fBonusRecovery = 0.0f;
    m_fBonusStatusEffectRate = 0.0f;
    m_fBonusSpecific = 0.0f;

    for( UInt i = 0; i < m_iLevel; ++i ) {
        m_fBonusDamage += pLevelingStats->GetBonusDamage( i );
        m_fBonusRecovery += pLevelingStats->GetBonusRecovery( i );
        m_fBonusStatusEffectRate += pLevelingStats->GetBonusStatusEffectRate( i );
        m_fBonusSpecific += pLevelingStats->GetBonusSpecific( i );
    }

    m_iCooldown = pLevelingStats->GetCooldown( m_iLevel );
}

/////////////////////////////////////////////////////////////////////////////////
// SkillSet implementation
SkillSet::SkillSet()
{
    m_iSkillCount = 0;
}
SkillSet::~SkillSet()
{
    // nothing to do
}

Void SkillSet::Add( Skill * pSkill, UInt iLevel )
{
    Assert( pSkill != NULL );
    Assert( iLevel < pSkill->GetLevelingStats()->GetMaxLevel() );
    Assert( m_iSkillCount < SKILL_SLOT_COUNT );

    m_arrSkills[m_iSkillCount] = SkillInstance( pSkill );
    m_arrSkills[m_iSkillCount].SetLevel( iLevel );
    ++m_iSkillCount;
}
Void SkillSet::RemoveAll()
{
    for( UInt i = 0; i < m_iSkillCount; ++i )
        m_arrSkills[i] = SkillInstance();

    m_iSkillCount = 0;
}

