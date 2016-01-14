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

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// SkillEffect implementation
SkillEffect::SkillEffect()
{
    m_fProc = 0.0f;

    m_iTargetPattern = SKILL_TARGET_COUNT;

    m_iScalingType = SKILLEFFECT_SCALING_COUNT;
    m_fScalingMultiplier = 0.0f;
}
SkillEffect::~SkillEffect()
{
    // nothing to do
}

SkillEffect * SkillEffect::StaticLoad( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    SkillEffectType iType = pGameParams->SkillEffectTypeFromString( pNode->GetAttribute(TEXT("Type"))->GetValue() );
    SkillEffect * pSkillEffect = NULL;

    GameplayFn->SelectMemory( TEXT("Scratch") );
    switch( iType ) {
        case SKILLEFFECT_DAMAGE: pSkillEffect = New SkillEffectDamage(); break;
        case SKILLEFFECT_HEAL:   pSkillEffect = New SkillEffectHeal();   break;
        case SKILLEFFECT_ATB:    pSkillEffect = New SkillEffectATB();    break;
        case SKILLEFFECT_STATUS: pSkillEffect = New SkillEffectStatus(); break;
        default: Assert( false ); break;
    }
    GameplayFn->UnSelectMemory();

    pSkillEffect->Load( pNode );

    return pSkillEffect;
}
Void SkillEffect::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    m_fProc = StringFn->ToFloat( pNode->GetAttribute(TEXT("Proc"))->GetValue() );
    m_iTargetPattern = pGameParams->SkillTargetPatternFromString( pNode->GetAttribute(TEXT("TargetPattern"))->GetValue() );
    m_iScalingType = pGameParams->SkillEffectScalingFromString( pNode->GetAttribute(TEXT("ScalingType"))->GetValue() );
    m_fScalingMultiplier = StringFn->ToFloat( pNode->GetAttribute(TEXT("ScalingMultiplier"))->GetValue() );
}
Void SkillEffect::Save( XMLNode * outNode ) const
{
    Assert( outNode != NULL );
    Assert( StringFn->Cmp( outNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    GChar strBuffer[1024];
    const GChar * strValue;

    StringFn->FromUInt( strBuffer, GetType() );
    outNode->CreateAttribute( TEXT("Type"), strBuffer );
    StringFn->FromFloat( strBuffer, m_fProc );
    outNode->CreateAttribute( TEXT("Proc"), strBuffer );
    strValue = pGameParams->SkillTargetPatternToString( m_iTargetPattern );
    outNode->CreateAttribute( TEXT("TargetPattern"), strValue );
    strValue = pGameParams->SkillEffectScalingToString( m_iScalingType );
    outNode->CreateAttribute( TEXT("ScalingType"), strValue );
    StringFn->FromFloat( strBuffer, m_fScalingMultiplier );
    outNode->CreateAttribute( TEXT("ScalingMultiplier"), strBuffer );
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectDamage implementation
SkillEffectDamage::SkillEffectDamage():
    SkillEffect()
{
    m_fBonusDmg = 0.0f;
    m_fBonusCritRate = 0.0f;
    m_fBonusCritDmg = 0.0f;
}
SkillEffectDamage::~SkillEffectDamage()
{
    // nothing to do
}

Void SkillEffectDamage::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    SkillEffect::Load( pNode );

    m_fBonusDmg = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusDamage"))->GetValue() );
    m_fBonusCritRate = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusCritRate"))->GetValue() );
    m_fBonusCritDmg = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusCritDmg"))->GetValue() );
}
Void SkillEffectDamage::Save( XMLNode * outNode ) const
{
    Assert( outNode != NULL );
    Assert( StringFn->Cmp( outNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    SkillEffect::Save( outNode );

    GChar strBuffer[1024];

    StringFn->FromFloat( strBuffer, m_fBonusDmg );
    outNode->CreateAttribute( TEXT("BonusDmg"), strBuffer );
    StringFn->FromFloat( strBuffer, m_fBonusCritRate );
    outNode->CreateAttribute( TEXT("BonusCritRate"), strBuffer );
    StringFn->FromFloat( strBuffer, m_fBonusCritDmg );
    outNode->CreateAttribute( TEXT("BonusCritDmg"), strBuffer );
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectHeal implementation
SkillEffectHeal::SkillEffectHeal():
    SkillEffect()
{
    m_fBonusHeal = 0.0f;
}
SkillEffectHeal::~SkillEffectHeal()
{
    // nothing to do
}

Void SkillEffectHeal::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    SkillEffect::Load( pNode );

    m_fBonusHeal = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusHeal"))->GetValue() );
}
Void SkillEffectHeal::Save( XMLNode * outNode ) const
{
    Assert( outNode != NULL );
    Assert( StringFn->Cmp( outNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    SkillEffect::Save( outNode );

    GChar strBuffer[1024];

    StringFn->FromFloat( strBuffer, m_fBonusHeal );
    outNode->CreateAttribute( TEXT("BonusHeal"), strBuffer );
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectATB implementation
SkillEffectATB::SkillEffectATB():
    SkillEffect()
{
    m_bIncrease = false;
}
SkillEffectATB::~SkillEffectATB()
{
    // nothing to do
}

Void SkillEffectATB::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    SkillEffect::Load( pNode );

    m_bIncrease = ( StringFn->ToUInt(pNode->GetAttribute(TEXT("IsIncrease"))->GetValue()) != 0 );
}
Void SkillEffectATB::Save( XMLNode * outNode ) const
{
    Assert( outNode != NULL );
    Assert( StringFn->Cmp( outNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    SkillEffect::Save( outNode );

    outNode->CreateAttribute( TEXT("IsIncrease"), m_bIncrease ? TEXT("1") : TEXT("0") );
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectStatus implementation
SkillEffectStatus::SkillEffectStatus():
    SkillEffect()
{
    m_iType = STATUSEFFECT_COUNT;
    m_iStackCount = 0;
    m_iDuration = 0;
    m_fAmplitude = 0.0f;
}
SkillEffectStatus::~SkillEffectStatus()
{
    // nothing to do
}

Void SkillEffectStatus::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    SkillEffect::Load( pNode );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    m_iType = pGameParams->StatusEffectTypeFromString( pNode->GetAttribute(TEXT("StatusEffectType"))->GetValue() );
    m_iStackCount = (UInt)( StringFn->ToUInt(pNode->GetAttribute(TEXT("StackCount"))->GetValue()) );
    m_iDuration = (UInt)( StringFn->ToUInt(pNode->GetAttribute(TEXT("Duration"))->GetValue()) );
    m_fAmplitude = StringFn->ToFloat( pNode->GetAttribute(TEXT("Amplitude"))->GetValue() );
}
Void SkillEffectStatus::Save( XMLNode * outNode ) const
{
    Assert( outNode != NULL );
    Assert( StringFn->Cmp( outNode->GetTagName(), TEXT("SkillEffect") ) == 0 );

    SkillEffect::Save( outNode );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();
    GChar strBuffer[1024];
    const GChar * strValue;

    strValue = pGameParams->StatusEffectTypeToString( m_iType );
    outNode->CreateAttribute( TEXT("StatusEffectType"), strValue );
    StringFn->FromUInt( strBuffer, m_iStackCount );
    outNode->CreateAttribute( TEXT("StackCount"), strBuffer );
    StringFn->FromUInt( strBuffer, m_iDuration );
    outNode->CreateAttribute( TEXT("Duration"), strBuffer );
    StringFn->FromFloat( strBuffer, m_fAmplitude );
    outNode->CreateAttribute( TEXT("Amplitude"), strBuffer );
}

