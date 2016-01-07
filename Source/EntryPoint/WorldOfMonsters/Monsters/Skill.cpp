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
Skill::Skill( XMLNode * pSkillNode )
{
    Assert( pSkillNode != NULL );
    Assert( StringFn->Cmp(pSkillNode->GetTagName(), TEXT("Skill")) == 0 );

    m_iSkillID = (SkillID)( StringFn->ToUInt(pSkillNode->GetAttribute(TEXT("ID"))->GetValue()) );
    StringFn->NCopy( m_strName, pSkillNode->GetAttribute(TEXT("Name"))->GetValue(), SKILL_NAME_LENGTH - 1 );

    m_iType = _SkillType_FromString( pSkillNode->GetAttribute(TEXT("Type"))->GetValue() );

    m_iCooldown = (UInt)( StringFn->ToUInt(pSkillNode->GetAttribute(TEXT("Cooldown"))->GetValue()) );

    m_iMaxLevel = (UInt)( StringFn->ToUInt(pSkillNode->GetAttribute(TEXT("MaxLevel"))->GetValue()) );
    Assert( m_iMaxLevel <= SKILL_MAX_LEVEL );

    XMLNode * pLevelBonusListNode = pSkillNode->GetChildByTag( TEXT("LevelBonusList"), 0 );
    Assert( pLevelBonusListNode != NULL );

    for( UInt i = 0; i < m_iMaxLevel; ++i ) {
        XMLNode * pLevelBonusNode = pLevelBonusListNode->GetChildByTag( TEXT("LevelBonus"), i );
        Assert( pLevelBonusNode != NULL );

        m_arrLevelBonus[i].iStat = _SkillStat_FromString( pLevelBonusNode->GetAttribute(TEXT("Stat"))->GetValue() );
        m_arrLevelBonus[i].fAmount = StringFn->ToFloat( pLevelBonusNode->GetAttribute(TEXT("Amount"))->GetValue() );
    }

    m_bRequiresAwakening = ( StringFn->ToUInt(pSkillNode->GetAttribute(TEXT("RequiresAwakening"))->GetValue()) != 0 );
    m_bHasAwakeningUpgrade = ( StringFn->ToUInt(pSkillNode->GetAttribute(TEXT("HasAwakeningUpgrade"))->GetValue()) != 0 );
    m_iAwakeningUpgradeID = (SkillID)( StringFn->ToUInt(pSkillNode->GetAttribute(TEXT("AwakeningUpgradeID"))->GetValue()) );
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
ActiveSkill::ActiveSkill( XMLNode * pSkillNode ):
    Skill( pSkillNode )
{
    for ( UInt i = 0; i < SKILL_ACTIVE_COUNT; ++i )
        m_arrEffectCounts[i] = 0;

    XMLNode * pActiveSkillDescriptorNode = pSkillNode->GetChildByTag( TEXT("ActiveSkillDescriptor"), 0 );
    Assert( pActiveSkillDescriptorNode != NULL );

    m_bIsAttack = ( StringFn->ToUInt(pSkillNode->GetAttribute(TEXT("IsAttack"))->GetValue()) != 0 );

    UInt iCount = pActiveSkillDescriptorNode->GetChildCount();
    for( UInt i = 0; i < iCount; ++i ) {
        XMLNode * pActiveEffectDescriptorNode = pActiveSkillDescriptorNode->GetChildByTag( TEXT("ActiveEffectDescriptor"), i );
        SkillActiveType iActiveType = _SkillActiveType_FromString( pActiveEffectDescriptorNode->GetAttribute(TEXT("ActiveType"))->GetValue() );

        XMLNode * pEffectDescriptorNode = pActiveEffectDescriptorNode->GetChildByTag( TEXT("SkillEffectDescriptor"), 0 );
        SkillEffectType iEffectType = _SkillEffectType_FromString( pEffectDescriptorNode->GetAttribute(TEXT("EffectType"))->GetValue() );

        SkillEffect * pEffect = NULL;
        switch( iEffectType ) {
            case SKILLEFFECT_DAMAGE:
                pEffect = New SkillEffectDamage( pEffectDescriptorNode );
                break;
            case SKILLEFFECT_HEAL:
                pEffect = New SkillEffectHeal( pEffectDescriptorNode );
                break;
            case SKILLEFFECT_ATB:
                pEffect = New SkillEffectATB( pEffectDescriptorNode );
                break;
            case SKILLEFFECT_STATUS:
                pEffect = New SkillEffectStatus( pEffectDescriptorNode );
                break;
            default: Assert(false);  break;
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
            case SKILLEFFECT_DAMAGE:
                pEffect = New SkillEffectDamage( pEffectDescriptorNode );
                break;
            case SKILLEFFECT_HEAL:
                pEffect = New SkillEffectHeal( pEffectDescriptorNode );
                break;
            case SKILLEFFECT_ATB:
                pEffect = New SkillEffectATB( pEffectDescriptorNode );
                break;
            case SKILLEFFECT_STATUS:
                pEffect = New SkillEffectStatus( pEffectDescriptorNode );
                break;
            default: Assert(false);  break;
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

    m_iConstraint = _LeaderConstraint_FromString( pLeaderSkillDescriptorNode->GetAttribute(TEXT("Constraint"))->GetValue() );
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
LeaderSkill::LeaderConstraint LeaderSkill::_LeaderConstraint_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return LEADER_CONSTRAINT_;
    }
    Assert( false );
    return LEADER_CONSTRAINT_NONE;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillInstance implementation
SkillInstance::SkillInstance()
{
    m_pSkill = NULL;

    m_iLevel = 0;

    for( UInt i = 0; i < SKILL_STAT_COUNT; ++i )
        m_fEffectiveBonus[i] = 0.0f;
}
SkillInstance::SkillInstance( Skill * pSkill )
{
    Assert( pSkill != NULL );

    m_pSkill = pSkill;

    m_iLevel = 0;

    for( UInt i = 0; i < SKILL_STAT_COUNT; ++i )
        m_fEffectiveBonus[i] = 0.0f;
}
SkillInstance::SkillInstance( const SkillInstance & rhs )
{
    m_pSkill = rhs.m_pSkill;

    m_iLevel = rhs.m_iLevel;

    for( UInt i = 0; i < SKILL_STAT_COUNT; ++i )
        m_fEffectiveBonus[i] = rhs.m_fEffectiveBonus[i];
}
SkillInstance::~SkillInstance()
{
    // nothing to do
}

SkillInstance & SkillInstance::operator=( const SkillInstance & rhs )
{
    m_pSkill = rhs.m_pSkill;

    m_iLevel = rhs.m_iLevel;

    for( UInt i = 0; i < SKILL_STAT_COUNT; ++i )
        m_fEffectiveBonus[i] = rhs.m_fEffectiveBonus[i];

    return (*this);
}

UInt SkillInstance::LevelUp()
{
    if ( m_iLevel < m_pSkill->GetMaxLevel() ) {
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
    Assert( iLevel < m_pSkill->GetMaxLevel() );
    if ( m_iLevel != iLevel ) {
        m_iLevel = iLevel;
        _UpdateEffectiveStats();
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void SkillInstance::_UpdateEffectiveStats()
{
    for( UInt i = 0; i < SKILL_STAT_COUNT; ++i )
        m_fEffectiveBonus[i] = 0.0f;

    for( UInt i = 0; i < m_iLevel; ++i ) {
        const SkillLevelBonus * pBonus = m_pSkill->GetLevelBonus( i );
        m_fEffectiveBonus[pBonus->iStat] += pBonus->fAmount;
    }
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
    Assert( iLevel <= pSkill->GetMaxLevel() );
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

