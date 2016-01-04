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

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// SkillEffect implementation
SkillEffect::SkillEffect()
{
    // nothing to do
}
SkillEffect::~SkillEffect()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

UInt SkillEffect::_ResolveEnnemyTargets( MonsterBattleInstance ** outTargets, SkillTargetPattern iPattern,
                                         BattleTeam * pEnnemyTeam, UInt iEnnemyTarget )
{
    UInt iTargetCount = 0;
    UInt iFirstRoll, iSecondRoll, iThirdRoll;
    
    switch( iPattern ) {
        case SKILL_TARGET_SELF:
            // nothing to do
            break;
        case SKILL_TARGET_ENNEMY_SINGLE:
            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iEnnemyTarget );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_SINGLE_RANDOM:
            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( GameplayFn->GetRandomUInt(pEnnemyTeam->GetTeamSize()) );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_DUAL:
            if ( iEnnemyTarget == pEnnemyTeam->GetTeamSize() - 1 )
                --iEnnemyTarget;

            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iEnnemyTarget );
            ++iTargetCount;
            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iEnnemyTarget + 1 );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_DUAL_RANDOM:
            iFirstRoll = GameplayFn->GetRandomUInt(pEnnemyTeam->GetTeamSize());
            iSecondRoll = GameplayFn->GetRandomUInt(pEnnemyTeam->GetTeamSize());

            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iFirstRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iSecondRoll );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_WIDE:
            if ( iEnnemyTarget == 0 )
                ++iEnnemyTarget;
            else if ( iEnnemyTarget == pEnnemyTeam->GetTeamSize() - 1 )
                --iEnnemyTarget;

            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iEnnemyTarget - 1 );
            ++iTargetCount;
            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iEnnemyTarget );
            ++iTargetCount;
            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iEnnemyTarget + 1 );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_WIDE_RANDOM:
            iFirstRoll = GameplayFn->GetRandomUInt(pEnnemyTeam->GetTeamSize());
            iSecondRoll = GameplayFn->GetRandomUInt(pEnnemyTeam->GetTeamSize());
            iThirdRoll = GameplayFn->GetRandomUInt(pEnnemyTeam->GetTeamSize());

            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iFirstRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iSecondRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( iThirdRoll );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_ALL:
            for ( UInt i = 0; i < pEnnemyTeam->GetTeamSize(); ++i ) {
                outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( i );
                ++iTargetCount;
            }
            break;
        case SKILL_TARGET_ENNEMY_ALL_EXCEPT:
            for ( UInt i = 0; i < pEnnemyTeam->GetTeamSize(); ++i ) {
                if ( i != iEnnemyTarget ) {
                    outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( i );
                    ++iTargetCount;
                }
            }
            break;
        case SKILL_TARGET_ALLY_SINGLE:
        case SKILL_TARGET_ALLY_SINGLE_RANDOM:
        case SKILL_TARGET_ALLY_DUAL:
        case SKILL_TARGET_ALLY_DUAL_RANDOM:
        case SKILL_TARGET_ALLY_WIDE:
        case SKILL_TARGET_ALLY_WIDE_RANDOM:
        case SKILL_TARGET_ALLY_ALL:
        case SKILL_TARGET_ALLY_ALL_EXCEPT:
            // nothing to do
            break;
        case SKILL_TARGET_ALL:
            for ( UInt i = 0; i < pEnnemyTeam->GetTeamSize(); ++i ) {
                outTargets[iTargetCount] = pEnnemyTeam->GetTeamMember( i );
                ++iTargetCount;
            }
            break;
        default: Assert( false ); break;
    }

    return iTargetCount;
}
UInt SkillEffect::_ResolveAllyTargets( MonsterBattleInstance ** outTargets, SkillTargetPattern iPattern,
                                       UInt iCaster, BattleTeam * pAllyTeam, UInt iAllyTarget )
{
    UInt iTargetCount = 0;
    UInt iFirstRoll, iSecondRoll, iThirdRoll;

    switch( iPattern ) {
        case SKILL_TARGET_SELF:
            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iCaster );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_SINGLE:
        case SKILL_TARGET_ENNEMY_SINGLE_RANDOM:
        case SKILL_TARGET_ENNEMY_DUAL:
        case SKILL_TARGET_ENNEMY_DUAL_RANDOM:
        case SKILL_TARGET_ENNEMY_WIDE:
        case SKILL_TARGET_ENNEMY_WIDE_RANDOM:
        case SKILL_TARGET_ENNEMY_ALL:
        case SKILL_TARGET_ENNEMY_ALL_EXCEPT:
            // nothing to do
            break;
        case SKILL_TARGET_ALLY_SINGLE:
            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iAllyTarget );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_SINGLE_RANDOM:
            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( GameplayFn->GetRandomUInt(pAllyTeam->GetTeamSize()) );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_DUAL:
            if ( iAllyTarget == pAllyTeam->GetTeamSize() - 1 )
                --iAllyTarget;

            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iAllyTarget );
            ++iTargetCount;
            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iAllyTarget + 1 );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_DUAL_RANDOM:
            iFirstRoll = GameplayFn->GetRandomUInt(pAllyTeam->GetTeamSize());
            iSecondRoll = GameplayFn->GetRandomUInt(pAllyTeam->GetTeamSize());

            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iFirstRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iSecondRoll );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_WIDE:
            if ( iAllyTarget == 0 )
                ++iAllyTarget;
            else if ( iAllyTarget == pAllyTeam->GetTeamSize() - 1 )
                --iAllyTarget;

            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iAllyTarget - 1 );
            ++iTargetCount;
            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iAllyTarget );
            ++iTargetCount;
            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iAllyTarget + 1 );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_WIDE_RANDOM:
            iFirstRoll = GameplayFn->GetRandomUInt(pAllyTeam->GetTeamSize());
            iSecondRoll = GameplayFn->GetRandomUInt(pAllyTeam->GetTeamSize());
            iThirdRoll = GameplayFn->GetRandomUInt(pAllyTeam->GetTeamSize());

            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iFirstRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iSecondRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pAllyTeam->GetTeamMember( iThirdRoll );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_ALL:
            for ( UInt i = 0; i < pAllyTeam->GetTeamSize(); ++i ) {
                outTargets[iTargetCount] = pAllyTeam->GetTeamMember( i );
                ++iTargetCount;
            }
            break;
        case SKILL_TARGET_ALLY_ALL_EXCEPT:
            for ( UInt i = 0; i < pAllyTeam->GetTeamSize(); ++i ) {
                if ( i != iAllyTarget ) {
                    outTargets[iTargetCount] = pAllyTeam->GetTeamMember( i );
                    ++iTargetCount;
                }
            }
            break;
        case SKILL_TARGET_ALL:
            for ( UInt i = 0; i < pAllyTeam->GetTeamSize(); ++i ) {
                outTargets[iTargetCount] = pAllyTeam->GetTeamMember( i );
                ++iTargetCount;
            }
            break;
        default: Assert( false ); break;
    }

    return iTargetCount;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillEffectDamage implementation
SkillEffectDamage::SkillEffectDamage( XMLNode * pNode ):
    SkillEffect()
{
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("SkillEffectDamage")) == 0 );
    Assert( pNode->IsLeaf() );

    m_iTargetPattern = _SkillTargetPattern_FromString( pNode->GetAttribute(TEXT("TargetPattern"))->GetValue() );

    m_iScalingType = _SkillEffectScaling_FromString( pNode->GetAttribute(TEXT("ScalingType"))->GetValue() );
    m_fScalingMultiplier = StringFn->ToFloat( pNode->GetAttribute(TEXT("ScalingMultiplier"))->GetValue() );

    m_fSkillBonusDmg = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusDamage"))->GetValue() );
    m_fSkillBonusCritRate = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusCritRate"))->GetValue() );
    m_fSkillBonusCritDmg = StringFn->ToFloat( pNode->GetAttribute(TEXT("BonusCritDmg"))->GetValue() );
}
SkillEffectDamage::~SkillEffectDamage()
{
    // nothing to do
}

Void SkillEffectDamage::Apply( BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pEnnemyTeam,
                               BattleTeam * pCasterTargetTeam, UInt iCasterTarget ) const
{
    Assert( pCasterTargetTeam == pEnnemyTeam );

    // Grab caster instance
    MonsterBattleInstance * pCaster = pCasterTeam->GetTeamMember( iCaster );

    // Resolve targets
    MonsterBattleInstance * arrTargets[BATTLE_TEAM_MAX_SIZE];
    UInt iTargetCount = _ResolveEnnemyTargets( arrTargets, m_iTargetPattern, pCasterTargetTeam, iCasterTarget );

    // Apply effect to all targets
    for ( UInt i = 0; i < iTargetCount; ++i ) {
        MonsterBattleInstance * pTarget = arrTargets[i];

        // Check for invulnerability buff
        if ( pTarget->HasStatusEffect( STATUSEFFECT_BUFF_INVINCIBILITY ) )
            continue;

        // Compute effect values
        UInt iDamageAmount = _ComputeDamage( pCaster, pTarget );

        // Check for mark_dmg debuff
        if ( pTarget->HasStatusEffect( STATUSEFFECT_DEBUFF_MARK_DMG ) )
            iDamageAmount = (UInt)(MathFn->Floor( 1.25f * (Float)iDamageAmount ));

        // Check for shield buff
        UInt iRemainingDamageAmount = iDamageAmount;
        if ( pTarget->HasStatusEffect( STATUSEFFECT_BUFF_SHIELD ) ) {
            StatusEffectInstance * pShieldEffect = pTarget->GetStatusEffect( STATUSEFFECT_BUFF_SHIELD );
            iRemainingDamageAmount = pShieldEffect->Damage( iDamageAmount );
        }
        arrTargets[i]->Damage( iRemainingDamageAmount );

        // Check for mark_drain debuff
        if ( pTarget->HasStatusEffect( STATUSEFFECT_DEBUFF_MARK_DRAIN ) ) {
            UInt iHealAmount = (UInt)(MathFn->Floor( 0.25f * (Float)iDamageAmount ));
            pCaster->Heal( iHealAmount );
        }

        // Check for vampire runes
        if ( pCaster->GetBaseInstance()->HasSetBonus( RUNE_VAMPIRE ) ) {
            UInt iHealAmount = (UInt)(MathFn->Floor( 0.35f * (Float)iDamageAmount ));
            pCaster->Heal( iHealAmount );
        }

        // Check for destroy runes
        if ( pCaster->GetBaseInstance()->HasSetBonus( RUNE_DESTROY ) ) {
            StatusEffectInstance * pDestroyEffect = pTarget->GetStatusEffect( STATUSEFFECT_DEBUFF_DESTROY );
            pDestroyEffect->Increase( iRemainingDamageAmount );
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

UInt SkillEffectDamage::_ComputeDamage( MonsterBattleInstance * pCaster, MonsterBattleInstance * pTarget ) const
{
    // Compute base damage
    UInt iCasterMaxHP = pCaster->GetHP();
    UInt iCasterHP = pCaster->GetCurrentHP();
    UInt iCasterAttack = pCaster->GetATT();

    UInt iTargetMaxHP = pTarget->GetHP();
    UInt iTargetHP = pTarget->GetCurrentHP();
    UInt iTargetDefense = pTarget->GetDEF();

    Float fBaseDamage = 0.0f;
    switch( m_iScalingType ) {
        case SKILLEFFECT_SCALING_DEFAULT:
            fBaseDamage = ( m_fScalingMultiplier * (Float)(iCasterAttack - iTargetDefense) );
            if ( fBaseDamage < 0.0f )
                fBaseDamage = 0.0f;
            break;
        case SKILLEFFECT_SCALING_SELF_HP_CURRENT:
            fBaseDamage = ( m_fScalingMultiplier * (Float)iCasterHP );
            break;
        case SKILLEFFECT_SCALING_SELF_HP_MAX:
            fBaseDamage = ( m_fScalingMultiplier * (Float)iCasterMaxHP );
            break;
        case SKILLEFFECT_SCALING_TARGET_HP_CURRENT:
            fBaseDamage = ( m_fScalingMultiplier * (Float)iTargetHP );
            break;
        case SKILLEFFECT_SCALING_TARGET_HP_MAX:
            fBaseDamage = ( m_fScalingMultiplier * (Float)iTargetMaxHP );
            break;
        default: Assert( false ); break;
    }

    // Check for a critical hit
    Float fCasterCritRate = pCaster->GetCritR() + m_fSkillBonusCritRate;
    if ( fCasterCritRate > 1.0f )
        fCasterCritRate = 1.0f;
    Float fCasterCritDmg = pCaster->GetCritD() + m_fSkillBonusCritDmg;

    if ( GameplayFn->CheckRandomEvent(fCasterCritRate) )
        fBaseDamage += ( fBaseDamage * fCasterCritDmg );

    // Apply element correction
    MonsterElement iCasterElement = pCaster->GetBaseInstance()->GetElement();
    MonsterElement iTargetElement = pTarget->GetBaseInstance()->GetElement();

    if ( Monster::IsElementStrongAgainst(iCasterElement, iTargetElement) ) {
        // Chance at a crushing hit
        if ( GameplayFn->CheckRandomEvent(0.50f) )
            fBaseDamage *= 1.5f;
    } else if ( Monster::IsElementWeakAgainst(iCasterElement, iTargetElement) ) {
        // Chance at a glancing hit
        if ( GameplayFn->CheckRandomEvent(0.50f) )
            fBaseDamage *= 0.5f;
    }

    // Convert to damage value
    UInt iDamageAmount = MathFn->Floor( fBaseDamage * 100.0f );

    // Done
    return iDamageAmount;
}

