/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/SkillEffect.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Base SkillEffect interface
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
#include "SkillEffect.h"

/////////////////////////////////////////////////////////////////////////////////
// SkillEffect implementation
SkillEffect::SkillEffect( XMLNode * pNode )
{
    Assert( pNode != NULL );

    m_fProc = StringFn->ToFloat( pNode->GetAttribute(TEXT("Proc"))->GetValue() );

    m_iTargetPattern = _SkillTargetPattern_FromString( pNode->GetAttribute(TEXT("TargetPattern"))->GetValue() );

    m_iScalingType = _SkillEffectScaling_FromString( pNode->GetAttribute(TEXT("ScalingType"))->GetValue() );
    m_fScalingMultiplier = StringFn->ToFloat( pNode->GetAttribute(TEXT("ScalingMultiplier"))->GetValue() );
}
SkillEffect::~SkillEffect()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

SkillTargetPattern SkillEffect::_SkillTargetPattern_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return SKILL_TARGET_;
    }
    Assert( false );
    return SKILL_TARGET_COUNT;
}
SkillEffectScaling SkillEffect::_SkillEffectScaling_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return SKILLEFFECT_SCALING_;
    }
    Assert( false );
    return SKILLEFFECT_SCALING_COUNT;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectDamage implementation
SkillEffectDamage::SkillEffectDamage( XMLNode * pNode ):
    SkillEffect(pNode)
{
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("SkillEffectDescriptor")) == 0 );

    m_fBonusDmg = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusDamage"))->GetValue() );
    m_fBonusCritRate = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusCritRate"))->GetValue() );
    m_fBonusCritDmg = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusCritDmg"))->GetValue() );
}
SkillEffectDamage::~SkillEffectDamage()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectHeal implementation
SkillEffectHeal::SkillEffectHeal( XMLNode * pNode ):
    SkillEffect(pNode)
{
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("SkillEffectDescriptor")) == 0 );

    m_fBonusHeal = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusHeal"))->GetValue() );
}
SkillEffectHeal::~SkillEffectHeal()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectATB implementation
SkillEffectATB::SkillEffectATB( XMLNode * pNode ):
    SkillEffect(pNode)
{
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("SkillEffectDescriptor")) == 0 );

    m_bIncrease = ( StringFn->ToUInt(pNode->GetAttribute(TEXT("IsIncrease"))->GetValue()) != 0 );
}
SkillEffectATB::~SkillEffectATB()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectStatus implementation
SkillEffectStatus::SkillEffectStatus( XMLNode * pNode ):
    SkillEffect(pNode)
{
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("SkillEffectDescriptor")) == 0 );

    m_iType = _StatusEffectType_FromString( pNode->GetAttribute(TEXT("StatusEffectType"))->GetValue() );
    m_iStackCount = (UInt)( StringFn->ToUInt(pNode->GetAttribute(TEXT("StackCount"))->GetValue()) );
    m_iDuration = (UInt)( StringFn->ToUInt(pNode->GetAttribute(TEXT("Duration"))->GetValue()) );
    m_fAmplitude = StringFn->ToFloat( pNode->GetAttribute(TEXT("Amplitude"))->GetValue() );
}
SkillEffectStatus::~SkillEffectStatus()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

StatusEffectType SkillEffectStatus::_StatusEffectType_FromString( const GChar * strValue )
{
    if ( StringFn->Cmp(strValue, TEXT("")) == 0 ) {
        return STATUSEFFECT_;
    }
    Assert( false );
    return STATUSEFFECT_COUNT;
}

