/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Battle/BattleMonsterInstance.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Battle : BattleMonsterInstance interface
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
#include "BattleMonsterInstance.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BattleMonsterInstance implementation
BattleMonsterInstance::BattleMonsterInstance( PlayerTown * pPlayerTown, MonsterInstance * pMonsterInstance, LeaderSkill * pActiveLeaderSkill ):
    m_hActiveEffects()
{
    m_pPlayerTown = pPlayerTown;
    m_pMonsterInstance = pMonsterInstance;
    m_pActiveLeaderSkill = pActiveLeaderSkill;

    m_iCurrentHP = 0;
    m_iShieldHP = 0;
    m_iHPLostCounter_Nemesis = 0;
    m_iHPLostCounter_Destroy = 0;

    m_iATB = 0;

    for( UInt i = 0; i < SKILL_SLOT_COUNT; ++i )
        m_arrSkillCooldowns[i] = 0;

    m_bDisabled = false;

    _UpdateBattleStats();
}
BattleMonsterInstance::~BattleMonsterInstance()
{
    // nothing to do
}

UInt BattleMonsterInstance::Damage( UInt iAmount, UInt iDestroyRunes )
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Handle invincibility
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_INVINCIBILITY) )
        return 0;

    // Handle shield
    if ( m_iShieldHP >= iAmount ) {
        m_iShieldHP -= iAmount;
        return 0;
    }

    iAmount -= m_iShieldHP;
    m_iShieldHP = 0;

    // Handle nemesis runes
    UInt iSetCount = 0;
    if ( m_pMonsterInstance->HasSetBonus(RUNE_NEMESIS,&iSetCount) ) {
        Float fAmplitude = ( pGameParams->GetRuneSetStatBonus(RUNE_NEMESIS) * (Float)iSetCount );

        m_iHPLostCounter_Nemesis += iAmount;
        UInt iHPStep = (UInt)( MathFn->Floor(0.07f * (Float)m_iHealth) );
        UInt iCount = ( m_iHPLostCounter_Nemesis / iHPStep );

        m_iATB += (UInt)( MathFn->Floor(fAmplitude * ((Float)iCount) * (Float)BATTLE_ATB_CAPACITY) );

        m_iHPLostCounter_Nemesis %= iHPStep;
    }

    // Handle destroy runes
    if ( iDestroyRunes > 0 ) {
        Float fAmplitude = ( pGameParams->GetRuneSetStatBonus(RUNE_DESTROY) * (Float)iDestroyRunes );

        m_iHPLostCounter_Destroy += iAmount;
        UInt iHPStep = (UInt)( MathFn->Floor(0.30f * (Float)m_iHealth) );
        UInt iCount = ( m_iHPLostCounter_Destroy / iHPStep );

        if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_DESTROY) )
            m_hActiveEffects.GetStatusEffect(STATUSEFFECT_DEBUFF_DESTROY)->IncreaseAmplitude( 0, fAmplitude * (Float)iCount );
        else
            m_hActiveEffects.Add( STATUSEFFECT_DEBUFF_DESTROY, 1, 0x0fffffff, fAmplitude * (Float)iCount ); // infinite duration
        _UpdateBattleStats();

        m_iHPLostCounter_Destroy %= iHPStep;
    }

    // Damage
    if ( m_iCurrentHP > iAmount )
        m_iCurrentHP -= iAmount;
    else
        m_iCurrentHP = 0;
    return iAmount;
}
Void BattleMonsterInstance::Heal( UInt iAmount )
{
    // Handle no-heal
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_NOHEAL) )
        return;

    // Heal
    m_iCurrentHP += iAmount;
    if ( m_iCurrentHP > m_iHealth )
        m_iCurrentHP = m_iHealth;
}

Void BattleMonsterInstance::AddStatusEffect( StatusEffectType iType, UInt iStackCount, UInt iDuration, Float fAmplitude )
{
    // Handle immunity
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_IMMUNITY) ) {
        if ( iType >= STATUSEFFECT_DEBUFF_ATTACK )
            return;
    }

    // Add status effect
    m_hActiveEffects.Add( iType, iStackCount, iDuration, fAmplitude );
    _UpdateBattleStats();

    // Handle shield HP
    if ( iType == STATUSEFFECT_BUFF_SHIELD )
        m_iShieldHP = (UInt)( MathFn->Floor(fAmplitude * ((Float)iStackCount) * (Float)m_iHealth) );
}
Void BattleMonsterInstance::RemoveStatusEffect( StatusEffectType iType, UInt iStackCount )
{
    // Remove status effect
    m_hActiveEffects.Remove( iType, iStackCount );
    _UpdateBattleStats();

    // Handle shield HP
    if ( iType == STATUSEFFECT_BUFF_SHIELD )
        m_iShieldHP = 0;
}
Void BattleMonsterInstance::RemoveExpiredStatusEffects()
{
    // Remove expired status effects
    m_hActiveEffects.RemoveExpired();
    _UpdateBattleStats();

    // Handle shield HP
    if ( m_iShieldHP > 0 && !(m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_SHIELD)) )
        m_iShieldHP = 0;
}

Void BattleMonsterInstance::Initialize()
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Reset all cooldowns
    for( UInt i = 0; i < m_pMonsterInstance->GetSkillCount(); ++i )
        m_arrSkillCooldowns[i] = 0;

    // Remove all status effects
    m_hActiveEffects.RemoveAll();
    _UpdateBattleStats();

    // Start full health, no shield, 0 ATB
    m_iCurrentHP = m_iHealth;
    m_iShieldHP = 0;
    m_iHPLostCounter_Nemesis = 0;
    m_iHPLostCounter_Destroy = 0;

    m_iATB = 0;

    // Handle shield runes
    UInt iSetCount = 0;
    if ( m_pMonsterInstance->HasSetBonus(RUNE_SHIELD,&iSetCount) ) {
        Float fAmplitude = ( pGameParams->GetRuneSetStatBonus(RUNE_SHIELD) * (Float)iSetCount );
        m_hActiveEffects.Add( STATUSEFFECT_BUFF_SHIELD, 1, 3, fAmplitude );
        m_iShieldHP = (UInt)( MathFn->Floor(fAmplitude * (Float)m_iHealth) );
    }

    // Handle will runes
    if ( m_pMonsterInstance->HasSetBonus(RUNE_WILL) ) {
        Float fAmplitude = pGameParams->GetRuneSetStatBonus( RUNE_WILL );
        m_hActiveEffects.Add( STATUSEFFECT_BUFF_IMMUNITY, 1, 1, fAmplitude );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void BattleMonsterInstance::_UpdateBattleStats()
{
    // Effective stats
    UInt arrFlats[4];
    Float arrRatios[4];
    m_pMonsterInstance->GetEffectiveStats( arrFlats, arrRatios );

    m_iHealth = arrFlats[0];
    m_iAttack = arrFlats[1];
    m_iDefense = arrFlats[2];
    m_iSpeed = arrFlats[3];
    m_fCriticalRate = arrRatios[0];
    m_fCriticalDamage = arrRatios[1];
    m_fAccuracy = arrRatios[2];
    m_fResistance = arrRatios[3];

    // Add building bonuses
    //////////////////////////////////////////////////////

    // Add leader skill bonuses if any
    if ( m_pActiveLeaderSkill != NULL ) {
        switch( m_pActiveLeaderSkill->GetLeaderBonusStat() ) {
            case MONSTER_STAT_HEALTH:
                m_iHealth += (UInt)( MathFn->Floor(m_pActiveLeaderSkill->GetLeaderBonusAmount() * (Float)(arrFlats[0])) );
                break;
            case MONSTER_STAT_ATTACK:
                m_iAttack += (UInt)( MathFn->Floor(m_pActiveLeaderSkill->GetLeaderBonusAmount() * (Float)(arrFlats[1])) );
                break;
            case MONSTER_STAT_DEFENSE:
                m_iDefense += (UInt)( MathFn->Floor(m_pActiveLeaderSkill->GetLeaderBonusAmount() * (Float)(arrFlats[2])) );
                break;
            case MONSTER_STAT_SPEED:
                m_iSpeed += (UInt)( MathFn->Floor(m_pActiveLeaderSkill->GetLeaderBonusAmount() * (Float)(arrFlats[3])) );
                break;
            case MONSTER_STAT_CRIT_RATE:
                m_fCriticalRate += m_pActiveLeaderSkill->GetLeaderBonusAmount();
                break;
            case MONSTER_STAT_CRIT_DMG:
                m_fCriticalDamage += m_pActiveLeaderSkill->GetLeaderBonusAmount();
                break;
            case MONSTER_STAT_ACCURACY:
                m_fAccuracy += m_pActiveLeaderSkill->GetLeaderBonusAmount();
                break;
            case MONSTER_STAT_RESISTANCE:
                m_fResistance += m_pActiveLeaderSkill->GetLeaderBonusAmount();
                break;
            default: Assert(false); break;
        }
    }

    // Handle disabling status effects
    m_bDisabled = false;
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_STUN) )
        m_bDisabled = true;
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_FREEZE) )
        m_bDisabled = true;
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_SLEEP) )
        m_bDisabled = true;

    // Handle stat-based status effects
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_DESTROY) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_DEBUFF_DESTROY );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude(0) * (Float)(pStatusEffect->GetStackCount()) );
        m_iHealth -= (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(arrFlats[0])) );
    }

    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_ATTACK) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_BUFF_ATTACK );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude(0) * (Float)(pStatusEffect->GetStackCount()) );
        m_iAttack += (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(arrFlats[1])) );
    }
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_ATTACK) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_DEBUFF_ATTACK );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude(0) * (Float)(pStatusEffect->GetStackCount()) );
        m_iAttack -= (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(arrFlats[1])) );
    }

    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_DEFENSE) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_BUFF_DEFENSE );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude(0) * (Float)(pStatusEffect->GetStackCount()) );
        m_iDefense += (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(arrFlats[2])) );
    }
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_DEFENSE) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_DEBUFF_DEFENSE );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude(0) * (Float)(pStatusEffect->GetStackCount()) );
        m_iDefense -= (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(arrFlats[2])) );
    }

    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_SPEED) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_BUFF_SPEED );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude(0) * (Float)(pStatusEffect->GetStackCount()) );
        m_iSpeed += (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(arrFlats[3])) );
    }
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_SPEED) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_DEBUFF_SPEED );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude(0) * (Float)(pStatusEffect->GetStackCount()) );
        m_iSpeed -= (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(arrFlats[3])) );
    }

    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_CRITRATE) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_BUFF_CRITRATE );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude(0) * (Float)(pStatusEffect->GetStackCount()) );
        m_fCriticalRate += fEffectiveAmplitude;
    }
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_CRITRATE) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_DEBUFF_CRITRATE );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude(0) * (Float)(pStatusEffect->GetStackCount()) );
        m_fCriticalRate -= fEffectiveAmplitude;
    }
}


