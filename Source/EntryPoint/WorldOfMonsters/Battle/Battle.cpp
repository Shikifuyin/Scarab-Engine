/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Battle/Battle.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Battle : Base Battle interface
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
#include "Battle.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MonsterBattleInstance implementation
MonsterBattleInstance::MonsterBattleInstance( MonsterInstance * pMonsterInstance, LeaderSkill * pActiveLeaderSkill ):
    m_hActiveEffects()
{
    m_pMonsterInstance = pMonsterInstance;

    m_pActiveLeaderSkill = pActiveLeaderSkill;

    m_iCurrentHP = 0;
    m_iShieldHP = 0;
    m_iHPLostCounter_Nemesis = 0;
    m_iHPLostCounter_Destroy = 0;

    m_iATB = 0;

    for( UInt i = 0; i < SKILL_SLOT_COUNT; ++i )
        m_arrSkillCooldowns[i] = 0;

    _UpdateBattleStats();
}
MonsterBattleInstance::~MonsterBattleInstance()
{
    // nothing to do
}

UInt MonsterBattleInstance::Damage( UInt iAmount, Bool bDestroyRunes )
{
    // Handle invincibility
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_INVINCIBILITY) )
        return 0;

    // Handle shield
    if ( m_iShieldHP > iAmount ) {
        m_iShieldHP -= iAmount;
        return 0;
    }

    iAmount -= m_iShieldHP;
    m_iShieldHP = 0;

    // Handle nemesis runes
    if ( m_pMonsterInstance->HasSetBonus(RUNE_NEMESIS) ) {
        m_iHPLostCounter_Nemesis += iAmount;
        UInt iHPStep = (UInt)( MathFn->Floor(0.07f * (Float)m_iHealth) );
        UInt iCount = ( m_iHPLostCounter_Nemesis / iHPStep );
        
        m_iATB += iCount * ( 0.04f * BATTLE_ATB_CAPACITY );

        m_iHPLostCounter_Nemesis = ( m_iHPLostCounter_Nemesis % iHPStep );
    }

    // Handle destroy runes
    if ( bDestroyRunes ) {
        m_iHPLostCounter_Destroy += iAmount;
        UInt iHPStep = (UInt)( MathFn->Floor(0.30f * (Float)m_iHealth) );
        UInt iCount = ( m_iHPLostCounter_Destroy / iHPStep );
        
        if ( m_hActiveEffects.HasStatusEffect( STATUSEFFECT_DEBUFF_DESTROY ) )
            m_hActiveEffects.GetStatusEffect(STATUSEFFECT_DEBUFF_DESTROY)->AddStacks( iCount, 0x0fffffff );
        else
            m_hActiveEffects.Add( STATUSEFFECT_DEBUFF_DESTROY, false, 0.04f, 0x0fffffff, iCount, 0x0fffffff ); // infinite stacks & duration
        _UpdateBattleStats();
        
        m_iHPLostCounter_Destroy = ( m_iHPLostCounter_Destroy % iHPStep );
    }

    // Damage
    if ( m_iCurrentHP > iAmount ) {
        m_iCurrentHP -= iAmount;
        return iAmount;
    }
    m_iCurrentHP = 0;
    return iAmount;
}
Void MonsterBattleInstance::Heal( UInt iAmount )
{
    // Handle no-heal
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_NOHEAL) )
        return;

    // Heal
    m_iCurrentHP += iAmount;
    if ( m_iCurrentHP > m_iHealth )
        m_iCurrentHP = m_iHealth;
}

Void MonsterBattleInstance::AddStatusEffect( StatusEffectType iType, Bool bRemovable, Float fAmplitude, UInt iMaxStacks, UInt iStackCount, UInt iDuration )
{
    // Handle immunity
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_IMMUNITY) ) {
        if ( iType >= STATUSEFFECT_DEBUFF_ATTACK )
            return;
    }

    // Add status effect
    m_hActiveEffects.Add( iType, bRemovable, fAmplitude, iMaxStacks, iStackCount, iDuration );
    _UpdateBattleStats();

    // Handle shield HP
    if ( iType == STATUSEFFECT_BUFF_SHIELD )
        m_iShieldHP = (UInt)( MathFn->Floor(fAmplitude * (Float)m_iHealth) );
}
Void MonsterBattleInstance::RemoveStatusEffect( StatusEffectType iType, UInt iStackCount )
{
    // Remove status effect
    m_hActiveEffects.Remove( iType, iStackCount );
    _UpdateBattleStats();

    // Handle shield HP
    if ( iType == STATUSEFFECT_BUFF_SHIELD )
        m_iShieldHP = 0;
}

Void MonsterBattleInstance::Initialize()
{
    // Reset all cooldowns
    for( UInt i = 0; i < m_pMonsterInstance->GetSkillCount(); ++i )
        m_arrSkillCooldowns[i] = 0;

    // Remove all status effects
    m_hActiveEffects.RemoveAll();
    _UpdateBattleStats();

    // Start full health, no shield, 0 ATB
    m_iCurrentHP = m_iHealth;
    m_iShieldHP = 0;
    m_iATB = 0;

    // Handle shield runes
    if ( m_pMonsterInstance->HasSetBonus(RUNE_SHIELD) ) {
        m_hActiveEffects.Add( STATUSEFFECT_BUFF_SHIELD, true, 0.15f, 1, 1, 3 );
        _UpdateBattleStats();
        m_iShieldHP = (UInt)( MathFn->Floor(0.15f * (Float)m_iHealth) );
    }

    // Handle will runes
    if ( m_pMonsterInstance->HasSetBonus(RUNE_WILL) ) {
        m_hActiveEffects.Add( STATUSEFFECT_BUFF_IMMUNITY, true, 0.15f, 1, 1, 1 );
        _UpdateBattleStats();
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void MonsterBattleInstance::_UpdateBattleStats()
{
    // Effective stats
    m_iHealth = m_pMonsterInstance->GetEffectiveHP();
    m_iAttack = m_pMonsterInstance->GetEffectiveATT();
    m_iDefense = m_pMonsterInstance->GetEffectiveDEF();
    m_iSpeed = m_pMonsterInstance->GetEffectiveSPD();

    m_fCriticalRate = m_pMonsterInstance->GetEffectiveCritR();
    m_fCriticalDamage = m_pMonsterInstance->GetEffectiveCritD();
    m_fAccuracy = m_pMonsterInstance->GetEffectiveACC();
    m_fResistance = m_pMonsterInstance->GetEffectiveRES();

    // Add building bonuses
    ////////////////////////////////////////////////////////

    // Add leader skill bonuses if any
    if ( m_pActiveLeaderSkill != NULL ) {
        switch( m_pActiveLeaderSkill->GetLeaderBonusStat() ) {
            case MONSTER_STAT_HEALTH:
                m_iHealth += (UInt)( MathFn->Floor(m_pActiveLeaderSkill->GetLeaderBonusAmount() * (Float)(m_pMonsterInstance->GetHP())) );
                break;
            case MONSTER_STAT_ATTACK:
                m_iAttack += (UInt)( MathFn->Floor(m_pActiveLeaderSkill->GetLeaderBonusAmount() * (Float)(m_pMonsterInstance->GetATT())) );
                break;
            case MONSTER_STAT_DEFENSE:
                m_iDefense += (UInt)( MathFn->Floor(m_pActiveLeaderSkill->GetLeaderBonusAmount() * (Float)(m_pMonsterInstance->GetDEF())) );
                break;
            case MONSTER_STAT_SPEED:
                m_iSpeed += (UInt)( MathFn->Floor(m_pActiveLeaderSkill->GetLeaderBonusAmount() * (Float)(m_pMonsterInstance->GetSPD())) );
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
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
        m_iHealth -= (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(m_pMonsterInstance->GetHP())) );
    }

    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_ATTACK) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_BUFF_ATTACK );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
        m_iAttack += (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(m_pMonsterInstance->GetATT())) );
    }
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_ATTACK) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_DEBUFF_ATTACK );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
        m_iAttack -= (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(m_pMonsterInstance->GetATT())) );
    }

    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_DEFENSE) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_BUFF_DEFENSE );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
        m_iDefense += (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(m_pMonsterInstance->GetDEF())) );
    }
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_DEFENSE) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_DEBUFF_DEFENSE );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
        m_iDefense -= (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(m_pMonsterInstance->GetDEF())) );
    }

    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_SPEED) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_BUFF_SPEED );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
        m_iSpeed += (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(m_pMonsterInstance->GetSPD())) );
    }
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_SPEED) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_DEBUFF_SPEED );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
        m_iSpeed -= (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)(m_pMonsterInstance->GetSPD())) );
    }

    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_BUFF_CRITRATE) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_BUFF_CRITRATE );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
        m_fCriticalRate += fEffectiveAmplitude;
    }
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_CRITRATE) ) {
        const StatusEffect * pStatusEffect = m_hActiveEffects.GetStatusEffect( STATUSEFFECT_DEBUFF_CRITRATE );
        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
        m_fCriticalRate -= fEffectiveAmplitude;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// BattleTeam implementation
BattleTeam::BattleTeam( UInt iTeamSize )
{
    m_iTeamSize = iTeamSize;

    for( UInt i = 0; i < BATTLE_TEAM_MAX_SIZE; ++i )
        m_arrMonsters[i] = NULL;
}
BattleTeam::~BattleTeam()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// Battle implementation
Battle::Battle( BattleType iType, BattleTeam * pPlayerTeam, BattleTeam * pAITeam )
{
    m_iType = iType;

    m_pPlayerTeam = pPlayerTeam;
    m_pAITeam = pAITeam;

    m_bTurnPending = false;
    m_bTurnInProgress = false;
    m_bExtraTurn = false;
    m_bPlayerTurn = false;
    m_iTurnMonster = INVALID_OFFSET;
    m_pTurnMonster = NULL;
}
Battle::~Battle()
{
    // nothing to do
}

Bool Battle::IsPlayerDead() const
{
    for( UInt i = 0; i < m_pPlayerTeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
        if ( pBattleInstance->IsAlive() )
            return false;
    }
    return true;
}
Bool Battle::IsAIDead() const
{
    for( UInt i = 0; i < m_pAITeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
        if ( pBattleInstance->IsAlive() )
            return false;
    }
    return true;
}

Void Battle::Initialize()
{
    // Initialize all monsters
    for( UInt i = 0; i < m_pPlayerTeam->GetTeamSize(); ++i )
        m_pPlayerTeam->GetTeamMember(i)->Initialize();
    for( UInt i = 0; i < m_pAITeam->GetTeamSize(); ++i )
        m_pAITeam->GetTeamMember(i)->Initialize();

    // Initialize ATB state
    m_bTurnPending = false;
    m_bTurnInProgress = false;
    m_bExtraTurn = false;
    m_bPlayerTurn = false;
    m_iTurnMonster = INVALID_OFFSET;
    m_pTurnMonster = NULL;
}

Bool Battle::TimeStep()
{
    Assert( !m_bTurnPending );

    // Note : Give players priority on ATB equality ... yeah I'm nice !
    //        Priority inside a team follows setup order ... smart players will notice and take advantage ;)

    // Check if some monster already has his turn up
    m_iTurnMonster = INVALID_OFFSET;
    m_pTurnMonster = NULL;
    m_bPlayerTurn = false;
    UInt iHighestATB = 0;

    for( UInt i = 0; i < m_pPlayerTeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
        if ( pBattleInstance->CheckATB() ) {
            UInt iATB = pBattleInstance->GetATB();
            if ( iATB > iHighestATB ) {
                iHighestATB = iATB;
                m_bPlayerTurn = true;
                m_iTurnMonster = i;
                m_pTurnMonster = pBattleInstance;
            }
        }
    }
    for( UInt i = 0; i < m_pAITeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pAITeam->GetTeamMember( i );
        if ( pBattleInstance->CheckATB() ) {
            UInt iATB = pBattleInstance->GetATB();
            if ( iATB > iHighestATB ) {
                iHighestATB = iATB;
                m_bPlayerTurn = false;
                m_iTurnMonster = i;
                m_pTurnMonster = pBattleInstance;
            }
        }
    }

    // Someone's turn has come up
    if ( m_pTurnMonster != NULL ) {
        m_bTurnPending = true;
        return true;
    }

    // Step all monsters through time, checking turns again
    iHighestATB = 0;

    for( UInt i = 0; i < m_pPlayerTeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
        pBattleInstance->IncreaseATB( pBattleInstance->GetSPD() );
        if ( pBattleInstance->CheckATB() ) {
            UInt iATB = pBattleInstance->GetATB();
            if ( iATB > iHighestATB ) {
                iHighestATB = iATB;
                m_bPlayerTurn = true;
                m_iTurnMonster = i;
                m_pTurnMonster = pBattleInstance;
            }
        }
    }
    for( UInt i = 0; i < m_pAITeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pAITeam->GetTeamMember( i );
        pBattleInstance->IncreaseATB( pBattleInstance->GetSPD() );
        if ( pBattleInstance->CheckATB() ) {
            UInt iATB = pBattleInstance->GetATB();
            if ( iATB > iHighestATB ) { 
                iHighestATB = iATB;
                m_bPlayerTurn = false;
                m_iTurnMonster = i;
                m_pTurnMonster = pBattleInstance;
            }
        }
    }

    // Someone's turn has come up
    if ( m_pTurnMonster != NULL ) {
        m_bTurnPending = true;
        return true;
    }

    // No one turn has come up
    //m_bTurnPending = false;
    return false;
}

Bool Battle::StartTurn()
{
    Assert( m_bTurnPending );
    Assert( !m_bTurnInProgress );

    // Start a turn
    m_bTurnInProgress = true;
    m_bExtraTurn = false;
    m_pTurnMonster->ResetATB();

    // Handle cooldowns
    for( UInt i = 0; i < m_pPlayerTeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember(i);
        for ( UInt j = 0; j < pBattleInstance->GetSkillCount(); ++j )
            pBattleInstance->DecreaseSkillCooldown(j);
    }
    for( UInt i = 0; i < m_pAITeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pAITeam->GetTeamMember(i);
        for ( UInt j = 0; j < pBattleInstance->GetSkillCount(); ++j )
            pBattleInstance->DecreaseSkillCooldown(j);
    }

    // Handle status effects
    if ( m_pTurnMonster->HasStatusEffect(STATUSEFFECT_BUFF_REGEN) ) {
        StatusEffect * pStatusEffect = m_pTurnMonster->GetStatusEffect(STATUSEFFECT_BUFF_REGEN);

        UInt iHealAmount = (UInt)( MathFn->Floor(pStatusEffect->GetAmplitude() * (Float)(m_pTurnMonster->GetHP())) );

        m_pTurnMonster->Heal( iHealAmount );
    }
    if ( m_pTurnMonster->HasStatusEffect(STATUSEFFECT_DEBUFF_DOT) ) {
        StatusEffect * pStatusEffect = m_pTurnMonster->GetStatusEffect(STATUSEFFECT_DEBUFF_DOT);

        UInt iDamageAmount = (UInt)( MathFn->Floor(pStatusEffect->GetAmplitude() * (Float)(m_pTurnMonster->GetHP())) );

        m_pTurnMonster->Damage( iDamageAmount );
    }
    if ( m_pTurnMonster->HasStatusEffect(STATUSEFFECT_DEBUFF_BOMB) ) {
        StatusEffect * pStatusEffect = m_pTurnMonster->GetStatusEffect(STATUSEFFECT_DEBUFF_BOMB);

        UInt iDamageAmount = (UInt)( MathFn->Floor(pStatusEffect->GetAmplitude()) ); // this is the final dmg value already, passed as float

        m_pTurnMonster->Damage( iDamageAmount );
    }

    // Handle passive effects
    BattleTeam * pAllyTeam = m_bPlayerTurn ? m_pPlayerTeam : m_pAITeam;
    BattleTeam * pEnnemyTeam = m_bPlayerTurn ? m_pAITeam : m_pPlayerTeam;

    for( UInt i = 0; i < pAllyTeam->GetTeamSize(); ++i ) {
        if ( i == m_iTurnMonster )
            _HandlePassives( SKILL_PASSIVE_PERIODIC_SELF, pAllyTeam, m_iTurnMonster, pAllyTeam, m_iTurnMonster );
        else {
            _HandlePassives( SKILL_PASSIVE_PERIODIC_ALLIES, pAllyTeam, i, pAllyTeam, m_iTurnMonster );
            _HandlePassives( SKILL_PASSIVE_PERIODIC_ALL, pAllyTeam, i, pAllyTeam, m_iTurnMonster );
        }
    }
    for( UInt i = 0; i < pEnnemyTeam->GetTeamSize(); ++i ) {
        _HandlePassives( SKILL_PASSIVE_PERIODIC_ENNEMIES, pEnnemyTeam, i, pAllyTeam, m_iTurnMonster );
        _HandlePassives( SKILL_PASSIVE_PERIODIC_ALL, pEnnemyTeam, i, pAllyTeam, m_iTurnMonster );
    }

    // Returns whether the monster can play his turn
    return !( m_pTurnMonster->IsDisabled() || m_pTurnMonster->IsDead() );
}
Void Battle::HandleChoice( UInt iSkillChoice, Bool bTargetPlayerTeamElseAI, UInt iTargetMonster )
{
    Assert( m_bTurnPending );
    Assert( m_bTurnInProgress );
    Assert( m_pTurnMonster->HasSkillUp(iSkillChoice) );

    // Get selection setup
    BattleTeam * pAllyTeam = m_bPlayerTurn ? m_pPlayerTeam : m_pAITeam;
    BattleTeam * pEnnemyTeam = m_bPlayerTurn ? m_pAITeam : m_pPlayerTeam;

    BattleTeam * pTargetTeam = bTargetPlayerTeamElseAI ? m_pPlayerTeam : m_pAITeam;
    MonsterBattleInstance * pTarget = pTargetTeam->GetTeamMember( iTargetMonster );

    // Get skill instance
    SkillInstance * pSkillInstance = m_pTurnMonster->GetSkillInstance( iSkillChoice );
    Assert( pSkillInstance->IsActive() );

    // Enum all types of effects
    for ( UInt i = 0; i < SKILL_ACTIVE_COUNT; ++i ) {
        SkillActiveType iActiveType = (SkillActiveType)i;

        // Check if there are effects to handle
        ActiveSkill * pActiveSkill = (ActiveSkill*)(pSkillInstance->GetSkill());
        UInt iEffectCount = pActiveSkill->GetEffectCount( iActiveType );
        Assert( iEffectCount > 0 );

        // Trigger cooldown
        if ( pActiveSkill->HasCooldown() )
            m_pTurnMonster->TriggerSkillCooldown( iSkillChoice );

        // Handle all effects
        MonsterBattleInstance * arrTargets[BATTLE_TEAM_MAX_SIZE];
        UInt iTargetCount = 0;

        for ( UInt j = 0; j < iEffectCount; ++j ) {
            SkillEffect * pSkillEffect = pActiveSkill->GetEffect( iActiveType, j );

            // Resolve target pattern
            iTargetCount = _ResolveAllyTargets( arrTargets, pSkillEffect->GetTargetPattern(), pAllyTeam, m_iTurnMonster, pTargetTeam, iTargetMonster );

            // Handle effect
            _HandleSkillEffect( pAllyTeam, m_iTurnMonster, pTargetTeam, pSkillInstance, pSkillEffect, arrTargets, iTargetCount );

            // Resolve target pattern
            iTargetCount = _ResolveEnnemyTargets( arrTargets, pSkillEffect->GetTargetPattern(), pAllyTeam, m_iTurnMonster, pTargetTeam, iTargetMonster );

            // Handle effect
            _HandleSkillEffect( pAllyTeam, m_iTurnMonster, pTargetTeam, pSkillInstance, pSkillEffect, arrTargets, iTargetCount );
        }
    }

    // Handle violent runes
    if ( m_pTurnMonster->GetBaseInstance()->HasSetBonus(RUNE_VIOLENT) ) {
        if ( GameplayFn->CheckRandomEvent(0.22f) )
            m_bExtraTurn = true;
    }

    // Handle revenge runes
    UInt iCount = 0;
    if ( m_pTurnMonster->GetBaseInstance()->HasSetBonus(RUNE_REVENGE, &iCount) ) {
        if ( GameplayFn->CheckRandomEvent(0.15f * (Float)iCount) ) {
            m_bPlayerTurn = !m_bPlayerTurn;
            m_iTurnMonster = iTargetMonster;
            m_pTurnMonster = pTarget;
            m_bExtraTurn = true;
        }
    }

    // Handle extra turn skills
    //if ( )
    //    m_bExtraTurn = true;
}
Bool Battle::EndTurn()
{
    Assert( m_bTurnPending );
    Assert( m_bTurnInProgress );

    /////////////////////////////////////////

    // Update status effects durations
    for ( UInt i = 0; i < STATUSEFFECT_COUNT; ++i ) {
        if ( m_pTurnMonster->HasStatusEffect( (StatusEffectType)i ) ) {
            StatusEffect * pStatusEffect = m_pTurnMonster->GetStatusEffect( (StatusEffectType)i );
            for ( UInt j = 0; j < pStatusEffect->GetStackCount(); ++j )
                pStatusEffect->DecreaseDuration( j, 1 );
        }
    }
    m_pTurnMonster->RemoveExpiredStatusEffects();

    // Done with this turn
    m_bTurnInProgress = false;

    // Handle "additional turn" effects
    if ( !m_bExtraTurn )
        m_bTurnPending = false;

    // Returns whether an extra turn is pending
    return m_bTurnPending;
}

/////////////////////////////////////////////////////////////////////////////////

MonsterElement Battle::sm_arrElementWeakAgainst[MONSTER_ELEMENT_COUNT] = {
    MONSTER_ELEMENT_COUNT, // Weak vs MONSTER_ELEMENT_NONE
    MONSTER_ELEMENT_WIND,  // Weak vs MONSTER_ELEMENT_FIRE
    MONSTER_ELEMENT_FIRE,  // Weak vs MONSTER_ELEMENT_WATER
    MONSTER_ELEMENT_WATER, // Weak vs MONSTER_ELEMENT_WIND
    MONSTER_ELEMENT_COUNT, // Weak vs MONSTER_ELEMENT_LIGHT
    MONSTER_ELEMENT_COUNT  // Weak vs MONSTER_ELEMENT_DARK
};
MonsterElement Battle::sm_arrElementStrongAgainst[MONSTER_ELEMENT_COUNT] = {
    MONSTER_ELEMENT_COUNT, // Strong vs MONSTER_ELEMENT_NONE
    MONSTER_ELEMENT_WATER, // Strong vs MONSTER_ELEMENT_FIRE
    MONSTER_ELEMENT_WIND,  // Strong vs MONSTER_ELEMENT_WATER
    MONSTER_ELEMENT_FIRE,  // Strong vs MONSTER_ELEMENT_WIND
    MONSTER_ELEMENT_DARK,  // Strong vs MONSTER_ELEMENT_LIGHT
    MONSTER_ELEMENT_LIGHT  // Strong vs MONSTER_ELEMENT_DARK
};

UInt Battle::_ResolveEnnemyTargets( MonsterBattleInstance ** outTargets, SkillTargetPattern iPattern,
                                    BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam, UInt iTarget )
{
    UInt iTargetCount = 0;
    UInt iFirstRoll, iSecondRoll, iThirdRoll;

    switch( iPattern ) {
        case SKILL_TARGET_SELF:
            // nothing to do
            break;
        case SKILL_TARGET_ENNEMY_SINGLE:
            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iTarget );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_SINGLE_RANDOM:
            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( GameplayFn->GetRandomUInt(pTargetTeam->GetTeamSize()) );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_DUAL:
            if ( iTarget == pTargetTeam->GetTeamSize() - 1 )
                --iTarget;

            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iTarget );
            ++iTargetCount;
            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iTarget + 1 );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_DUAL_RANDOM:
            iFirstRoll = GameplayFn->GetRandomUInt(pTargetTeam->GetTeamSize());
            iSecondRoll = GameplayFn->GetRandomUInt(pTargetTeam->GetTeamSize());

            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iFirstRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iSecondRoll );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_WIDE:
            if ( iTarget == 0 )
                ++iTarget;
            else if ( iTarget == pTargetTeam->GetTeamSize() - 1 )
                --iTarget;

            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iTarget - 1 );
            ++iTargetCount;
            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iTarget );
            ++iTargetCount;
            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iTarget + 1 );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_WIDE_RANDOM:
            iFirstRoll = GameplayFn->GetRandomUInt(pTargetTeam->GetTeamSize());
            iSecondRoll = GameplayFn->GetRandomUInt(pTargetTeam->GetTeamSize());
            iThirdRoll = GameplayFn->GetRandomUInt(pTargetTeam->GetTeamSize());

            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iFirstRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iSecondRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pTargetTeam->GetTeamMember( iThirdRoll );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ENNEMY_ALL:
            for ( UInt i = 0; i < pTargetTeam->GetTeamSize(); ++i ) {
                outTargets[iTargetCount] = pTargetTeam->GetTeamMember( i );
                ++iTargetCount;
            }
            break;
        case SKILL_TARGET_ENNEMY_ALL_EXCEPT:
            for ( UInt i = 0; i < pTargetTeam->GetTeamSize(); ++i ) {
                if ( i != iTarget ) {
                    outTargets[iTargetCount] = pTargetTeam->GetTeamMember( i );
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
            for ( UInt i = 0; i < pTargetTeam->GetTeamSize(); ++i ) {
                outTargets[iTargetCount] = pTargetTeam->GetTeamMember( i );
                ++iTargetCount;
            }
            break;
        default: Assert( false ); break;
    }

    return iTargetCount;
}
UInt Battle::_ResolveAllyTargets( MonsterBattleInstance ** outTargets, SkillTargetPattern iPattern,
                                  BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam, UInt iTarget )
{
    UInt iTargetCount = 0;
    UInt iFirstRoll, iSecondRoll, iThirdRoll;

    switch( iPattern ) {
        case SKILL_TARGET_SELF:
            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iCaster );
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
            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iTarget );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_SINGLE_RANDOM:
            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( GameplayFn->GetRandomUInt(pCasterTeam->GetTeamSize()) );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_DUAL:
            if ( iTarget == pCasterTeam->GetTeamSize() - 1 )
                --iTarget;

            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iTarget );
            ++iTargetCount;
            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iTarget + 1 );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_DUAL_RANDOM:
            iFirstRoll = GameplayFn->GetRandomUInt(pCasterTeam->GetTeamSize());
            iSecondRoll = GameplayFn->GetRandomUInt(pCasterTeam->GetTeamSize());

            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iFirstRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iSecondRoll );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_WIDE:
            if ( iTarget == 0 )
                ++iTarget;
            else if ( iTarget == pCasterTeam->GetTeamSize() - 1 )
                --iTarget;

            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iTarget - 1 );
            ++iTargetCount;
            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iTarget );
            ++iTargetCount;
            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iTarget + 1 );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_WIDE_RANDOM:
            iFirstRoll = GameplayFn->GetRandomUInt(pCasterTeam->GetTeamSize());
            iSecondRoll = GameplayFn->GetRandomUInt(pCasterTeam->GetTeamSize());
            iThirdRoll = GameplayFn->GetRandomUInt(pCasterTeam->GetTeamSize());

            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iFirstRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iSecondRoll );
            ++iTargetCount;
            outTargets[iTargetCount] = pCasterTeam->GetTeamMember( iThirdRoll );
            ++iTargetCount;
            break;
        case SKILL_TARGET_ALLY_ALL:
            for ( UInt i = 0; i < pCasterTeam->GetTeamSize(); ++i ) {
                outTargets[iTargetCount] = pCasterTeam->GetTeamMember( i );
                ++iTargetCount;
            }
            break;
        case SKILL_TARGET_ALLY_ALL_EXCEPT:
            for ( UInt i = 0; i < pCasterTeam->GetTeamSize(); ++i ) {
                if ( i != iTarget ) {
                    outTargets[iTargetCount] = pCasterTeam->GetTeamMember( i );
                    ++iTargetCount;
                }
            }
            break;
        case SKILL_TARGET_ALL:
            for ( UInt i = 0; i < pCasterTeam->GetTeamSize(); ++i ) {
                outTargets[iTargetCount] = pCasterTeam->GetTeamMember( i );
                ++iTargetCount;
            }
            break;
        default: Assert( false ); break;
    }

    return iTargetCount;
}

UInt Battle::_ComputeDamage( SkillInstance * pSkillInstance, SkillEffectDamage * pDamageEffect, MonsterBattleInstance * pCaster, MonsterBattleInstance * pTarget,
                             Bool * outIsCrit, Bool * outIsGlancing, Bool * outIsCrushing )
{
    // Base damage
    Float fBaseDamage = 0.0f;

    UInt iCasterMaxHP = pCaster->GetHP();
    UInt iCasterHP = pCaster->GetCurrentHP();
    UInt iCasterAttack = pCaster->GetATT();

    UInt iTargetMaxHP = pTarget->GetHP();
    UInt iTargetHP = pTarget->GetCurrentHP();
    UInt iTargetDefense = pTarget->GetDEF();

    switch( pDamageEffect->GetScalingType() ) {
        case SKILLEFFECT_SCALING_DEFAULT:
            fBaseDamage = ( pDamageEffect->GetScalingMultiplier() * (100.0f * (Float)(iCasterAttack - iTargetDefense)) );
            if ( fBaseDamage < 0.0f )
                fBaseDamage = 0.0f;
            break;
        case SKILLEFFECT_SCALING_SELF_HP_CURRENT:
            fBaseDamage = ( pDamageEffect->GetScalingMultiplier() * (Float)iCasterHP );
            break;
        case SKILLEFFECT_SCALING_SELF_HP_MAX:
            fBaseDamage = ( pDamageEffect->GetScalingMultiplier() * (Float)iCasterMaxHP );
            break;
        case SKILLEFFECT_SCALING_TARGET_HP_CURRENT:
            fBaseDamage = ( pDamageEffect->GetScalingMultiplier() * (Float)iTargetHP );
            break;
        case SKILLEFFECT_SCALING_TARGET_HP_MAX:
            fBaseDamage = ( pDamageEffect->GetScalingMultiplier() * (Float)iTargetMaxHP );
            break;
        default: Assert( false ); break;
    }

    // Apply element correction
    MonsterElement iCasterElement = pCaster->GetBaseInstance()->GetElement();
    MonsterElement iTargetElement = pTarget->GetBaseInstance()->GetElement();

    *outIsGlancing = false;
    *outIsCrushing = false;
    if ( _IsElementWeakAgainst(iCasterElement, iTargetElement) ) {
        // Chance at a glancing hit
        if ( GameplayFn->CheckRandomEvent(0.5f) ) {
            fBaseDamage *= 0.5f;
            *outIsGlancing = true;
        }
    } else if ( _IsElementStrongAgainst(iCasterElement, iTargetElement) ) {
        // Chance at a crushing hit
        if ( GameplayFn->CheckRandomEvent(0.5f) ) {
            fBaseDamage *= 1.5f;
            *outIsCrushing = true;
        }
    }

    // Skill damage
    Float fSkillDamage = ( pSkillInstance->GetEffectiveBonus(SKILL_STAT_DAMAGE) + pDamageEffect->GetBonusDmg() );

    // Critical damage
    Float fCriticalDamage = 0.0f;

    Float fCasterCritRate = pCaster->GetCritR() + pDamageEffect->GetBonusCritRate();
    Float fCasterCritDmg = pCaster->GetCritD() + pDamageEffect->GetBonusCritDmg();
    if ( fCasterCritRate > 1.0f )
        fCasterCritRate = 1.0f;

    *outIsCrit = false;
    if ( GameplayFn->CheckRandomEvent(fCasterCritRate) ) {
        fCriticalDamage = fCasterCritDmg;
        *outIsCrit = true;
    }

    // Convert to damage value
    UInt iDamageAmount = (UInt)( MathFn->Floor((1.0f + fSkillDamage + fCriticalDamage) * fBaseDamage) );

    // Done
    return iDamageAmount;
}
UInt Battle::_ComputeHeal( SkillInstance * pSkillInstance, SkillEffectHeal * pHealEffect, MonsterBattleInstance * pCaster, MonsterBattleInstance * pTarget )
{
    // Base heal
    Float fBaseHeal = 0.0f;

    UInt iCasterMaxHP = pCaster->GetHP();
    UInt iCasterHP = pCaster->GetCurrentHP();
    UInt iCasterAttack = pCaster->GetATT();

    UInt iTargetMaxHP = pTarget->GetHP();
    UInt iTargetHP = pTarget->GetCurrentHP();

    switch( pHealEffect->GetScalingType() ) {
        case SKILLEFFECT_SCALING_DEFAULT:
            fBaseHeal = ( pHealEffect->GetScalingMultiplier() * (100.0f * (Float)iCasterAttack) );
            if ( fBaseHeal < 0.0f )
                fBaseHeal = 0.0f;
            break;
        case SKILLEFFECT_SCALING_SELF_HP_CURRENT:
            fBaseHeal = ( pHealEffect->GetScalingMultiplier() * (Float)iCasterHP );
            break;
        case SKILLEFFECT_SCALING_SELF_HP_MAX:
            fBaseHeal = ( pHealEffect->GetScalingMultiplier() * (Float)iCasterMaxHP );
            break;
        case SKILLEFFECT_SCALING_TARGET_HP_CURRENT:
            fBaseHeal = ( pHealEffect->GetScalingMultiplier() * (Float)iTargetHP );
            break;
        case SKILLEFFECT_SCALING_TARGET_HP_MAX:
            fBaseHeal = ( pHealEffect->GetScalingMultiplier() * (Float)iTargetMaxHP );
            break;
        default: Assert( false ); break;
    }

    // Skill heal
    Float fSkillHeal = ( pSkillInstance->GetEffectiveBonus(SKILL_STAT_RECOVERY) + pHealEffect->GetBonusHeal() );

    // Convert to heal value
    UInt iHealAmount = (UInt)( MathFn->Floor((1.0f + fSkillHeal) * fBaseHeal) );

    // Done
    return iHealAmount;
}

Void Battle::_HandleSkillEffect( BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam,
                                 SkillInstance * pSkillInstance, SkillEffect * pSkillEffect, MonsterBattleInstance ** arrTargets, UInt iTargetCount )
{
    // Get caster
    MonsterBattleInstance * pCaster = pCasterTeam->GetTeamMember( iCaster );

    // Apply to all targets
    for( UInt i = 0; i < iTargetCount; ++i ) {
        MonsterBattleInstance * pTarget = arrTargets[i];

        // Check effect type
        switch( pSkillEffect->GetType() ) {
            case SKILLEFFECT_DAMAGE: {
                    SkillEffectDamage * pDamageEffect = (SkillEffectDamage*)pSkillEffect;

                    Bool bIsCrit, bIsGlancing, bIsCrushing;
                    UInt iDamage = _ComputeDamage( pSkillInstance, pDamageEffect, pCaster, pTarget, &bIsCrit, &bIsGlancing, &bIsCrushing );

                    // Handle mark_dmg status
                    if ( pTarget->HasStatusEffect(STATUSEFFECT_DEBUFF_MARK_DMG) ) {
                        StatusEffect * pStatusEffect = pTarget->GetStatusEffect( STATUSEFFECT_DEBUFF_MARK_DMG );
                        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
                        iDamage += (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)iDamage) );
                    }

                    // Apply (shield status is handled inside)
                    UInt iInflictedDamage = pTarget->Damage( iDamage );

                    // Handle mark_drain status
                    if ( pTarget->HasStatusEffect(STATUSEFFECT_DEBUFF_MARK_DRAIN) ) {
                        StatusEffect * pStatusEffect = pTarget->GetStatusEffect( STATUSEFFECT_DEBUFF_MARK_DRAIN );
                        Float fEffectiveAmplitude = ( pStatusEffect->GetAmplitude() * (Float)(pStatusEffect->GetStackCount()) );
                        UInt iHealAmount = (UInt)( MathFn->Floor(fEffectiveAmplitude * (Float)iInflictedDamage) );
                        pCaster->Heal( iHealAmount );
                    }

                    // Handle vampire runes
                    if ( pCaster->GetBaseInstance()->HasSetBonus(RUNE_VAMPIRE) ) {
                        UInt iHealAmount = (UInt)( MathFn->Floor(0.35f * (Float)iInflictedDamage) );
                        pCaster->Heal( iHealAmount );
                    }

                    // Handle despair runes
                    if ( pCaster->GetBaseInstance()->HasSetBonus(RUNE_DESPAIR) ) {
                        if ( GameplayFn->CheckRandomEvent(0.25f) )
                            pTarget->AddStatusEffect( STATUSEFFECT_DEBUFF_STUN, true, 0.0f, 1, 1, 1 );
                    }

                    // Handle destroy runes
                    ////////////////////////////////////////////

                    // Handle passive effects
                    for( UInt j = 0; j < pCasterTeam->GetTeamSize(); ++j ) {
                        MonsterBattleInstance * pAlly = pCasterTeam->GetTeamMember( j );
                        if ( pAlly == pCaster ) {
                            // Caster hits Target
                            if ( bIsCrit )
                                _HandlePassives( SKILL_PASSIVE_ONCRIT, pCasterTeam, j, pTargetTeam, i );
                            else
                                _HandlePassives( SKILL_PASSIVE_ONHIT, pCasterTeam, j, pTargetTeam, i );
                        } else {
                            // Allies see Caster hit Target
                            if ( bIsCrit )
                                _HandlePassives( SKILL_PASSIVE_ONCRIT_ALLY, pCasterTeam, j, pTargetTeam, i );
                            else
                                _HandlePassives( SKILL_PASSIVE_ONHIT_ALLY, pCasterTeam, j, pTargetTeam, i );
                        }
                        // Allies see Target being hit
                        if ( bIsCrit )
                            _HandlePassives( SKILL_PASSIVE_ONBEINGCRIT_ENNEMY, pCasterTeam, j, pTargetTeam, i );
                        else
                            _HandlePassives( SKILL_PASSIVE_ONBEINGHIT_ENNEMY, pCasterTeam, j, pTargetTeam, i );
                    }
                    for( UInt j = 0; j < pTargetTeam->GetTeamSize(); ++j ) {
                        MonsterBattleInstance * pEnnemy = pTargetTeam->GetTeamMember( j );
                        if ( pEnnemy == pTarget ) {
                            // Target being hit by Caster
                            if ( bIsCrit )
                                _HandlePassives( SKILL_PASSIVE_ONBEINGCRIT, pTargetTeam, j, pCasterTeam, iCaster );
                            else
                                _HandlePassives( SKILL_PASSIVE_ONBEINGHIT, pTargetTeam, j, pCasterTeam, iCaster );
                        } else {
                            // Ennemies see Target being hit by Caster
                            if ( bIsCrit )
                                _HandlePassives( SKILL_PASSIVE_ONBEINGCRIT_ALLY, pTargetTeam, j, pCasterTeam, iCaster );
                            else
                                _HandlePassives( SKILL_PASSIVE_ONBEINGHIT_ALLY, pTargetTeam, j, pCasterTeam, iCaster );
                        }
                        // Ennemies see Caster hit
                        if ( bIsCrit )
                            _HandlePassives( SKILL_PASSIVE_ONCRIT_ENNEMY, pTargetTeam, j, pCasterTeam, iCaster );
                        else
                            _HandlePassives( SKILL_PASSIVE_ONHIT_ENNEMY, pTargetTeam, j, pCasterTeam, iCaster );
                    }
                } break;
            case SKILLEFFECT_HEAL: {
                    SkillEffectHeal * pHealEffect = (SkillEffectHeal*)pSkillEffect;

                    UInt iHeal = _ComputeHeal( pSkillInstance, pHealEffect, pCaster, pTarget );

                    // Apply
                    pTarget->Heal( iHeal );

                    // Handle passive effects
                    //_HandlePassives( SKILL_PASSIVE_ONHEAL, pCaster, pTarget );
                } break;
            case SKILLEFFECT_ATB: {
                    SkillEffectATB * pATBEffect = (SkillEffectATB*)pSkillEffect;

                    Float fATB = ( pATBEffect->GetScalingMultiplier() * BATTLE_ATB_CAPACITY );

                    // Apply
                    if ( pATBEffect->IsIncrease() )
                        pTarget->IncreaseATB( fATB );
                    else
                        pTarget->DecreaseATB( fATB );

                    // Handle passive effects
                    //_HandlePassives( SKILL_PASSIVE_ONATB, pCaster, pTarget );
                } break;
            case SKILLEFFECT_STATUS: {
                    SkillEffectStatus * pStatusEffect = (SkillEffectStatus*)pSkillEffect;

                    // Apply
                    pTarget->AddStatusEffect( pStatusEffect->GetStatusEffectType(), pStatusEffect->IsRemovable(), pStatusEffect->GetAmplitude(),
                                              pStatusEffect->GetMaxStacks(), pStatusEffect->GetStackCount(), pStatusEffect->GetDuration() );

                    // Handle passive effects
                    //_HandlePassives( SKILL_PASSIVE_ONSTATUS, pCaster, pTarget );
                } break;
            default: Assert(false); break;
        }
    }
}

Void Battle::_HandlePassives( SkillPassiveType iPassiveType, BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam, UInt iTarget )
{
    // Get caster & target
    MonsterBattleInstance * pCaster = pCasterTeam->GetTeamMember( iCaster );
    MonsterBattleInstance * pTarget = pTargetTeam->GetTeamMember( iTarget );

    // Check caster's skill set
    for ( UInt i = 0; i < pCaster->GetSkillCount(); ++i ) {
        // Check cooldown availability
        if ( !(pCaster->HasSkillUp(i)) )
            continue;

        // Look for passives
        SkillInstance * pSkillInstance = pCaster->GetSkillInstance(i);
        if ( !(pSkillInstance->IsPassive()) )
            continue;

        // Check if there are effects to handle
        PassiveSkill * pPassiveSkill = (PassiveSkill*)( pSkillInstance->GetSkill() );
        UInt iEffectCount = pPassiveSkill->GetEffectCount( iPassiveType );
        if ( iEffectCount == 0 )
            continue;

        // Trigger cooldown
        if ( pPassiveSkill->HasCooldown() )
            pCaster->TriggerSkillCooldown(i);

        // Handle all effects
        MonsterBattleInstance * arrTargets[BATTLE_TEAM_MAX_SIZE];
        UInt iTargetCount = 0;

        for ( UInt j = 0; j < iEffectCount; ++j ) {
            SkillEffect * pSkillEffect = pPassiveSkill->GetEffect( iPassiveType, j );

            // Resolve target pattern
            iTargetCount = _ResolveAllyTargets( arrTargets, pSkillEffect->GetTargetPattern(), pCasterTeam, iCaster, pTargetTeam, iTarget );

            // Handle effect
            _HandleSkillEffect( pCasterTeam, iCaster, pTargetTeam, pSkillInstance, pSkillEffect, arrTargets, iTargetCount );

            // Resolve target pattern
            iTargetCount = _ResolveEnnemyTargets( arrTargets, pSkillEffect->GetTargetPattern(), pCasterTeam, iCaster, pTargetTeam, iTarget );

            // Handle effect
            _HandleSkillEffect( pCasterTeam, iCaster, pTargetTeam, pSkillInstance, pSkillEffect, arrTargets, iTargetCount );
        }
    }
}



