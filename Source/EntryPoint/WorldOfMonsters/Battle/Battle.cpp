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
    m_iATB = 0;

    for( UInt i = 0; i < SKILL_SLOT_COUNT; ++i )
        m_arrSkillCooldowns[i] = 0;

    _UpdateBattleStats();
    m_iCurrentHP = m_iHealth;
}
MonsterBattleInstance::~MonsterBattleInstance()
{
    // nothing to do
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

    // Apply StatusEffects modifiers
    m_bDisabled = false;
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_STUN) )
        m_bDisabled = true;
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_FREEZE) )
        m_bDisabled = true;
    if ( m_hActiveEffects.HasStatusEffect(STATUSEFFECT_DEBUFF_SLEEP) )
        m_bDisabled = true;

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
    m_pTurnMonster = NULL;
}
Battle::~Battle()
{
    // nothing to do
}

Bool Battle::IsPlayerDead() const
{
    for ( UInt i = 0; i < m_pPlayerTeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
        if ( pBattleInstance->IsAlive() )
            return false;
    }
    return true;
}
Bool Battle::IsAIDead() const
{
    for ( UInt i = 0; i < m_pAITeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
        if ( pBattleInstance->IsAlive() )
            return false;
    }
    return true;
}

Void Battle::Initialize()
{
    // Set all monsters to full HP and 0 ATB

    // Cleanup all skill cooldowns

    // Cleanup all effects

    // Battle state
    m_bTurnPending = false;
    m_bTurnInProgress = false;
    m_bExtraTurn = false;
    m_bPlayerTurn = false;
    m_pTurnMonster = NULL;
}

Bool Battle::TimeStep()
{
    Assert( !m_bTurnPending );

    // Note : Give players priority on ATB equality ... yeah I'm nice !

    // Check if some monster already has his turn up
    m_pTurnMonster = NULL;
    m_bPlayerTurn = false;
    UInt iHighestATB = 0;

    for ( UInt i = 0; i < m_pPlayerTeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
        if ( pBattleInstance->CheckATB() ) {
            UInt iATB = pBattleInstance->GetATB();
            if ( iATB >= iHighestATB ) {
                iHighestATB = iATB;
                m_pTurnMonster = pBattleInstance;
                m_bPlayerTurn = true;
            }
        }
    }
    for ( UInt i = 0; i < m_pAITeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pAITeam->GetTeamMember( i );
        if ( pBattleInstance->CheckATB() ) {
            UInt iATB = pBattleInstance->GetATB();
            if ( iATB > iHighestATB ) {
                iHighestATB = iATB;
                m_pTurnMonster = pBattleInstance;
                m_bPlayerTurn = false;
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

    for ( UInt i = 0; i < m_pPlayerTeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
        pBattleInstance->IncreaseATB( pBattleInstance->GetSPD() );
        if ( pBattleInstance->CheckATB() ) {
            UInt iATB = pBattleInstance->GetATB();
            if ( iATB >= iHighestATB ) {
                iHighestATB = iATB;
                m_pTurnMonster = pBattleInstance;
                m_bPlayerTurn = true;
            }
        }
    }
    for ( UInt i = 0; i < m_pAITeam->GetTeamSize(); ++i ) {
        MonsterBattleInstance * pBattleInstance = m_pAITeam->GetTeamMember( i );
        pBattleInstance->IncreaseATB( pBattleInstance->GetSPD() );
        if ( pBattleInstance->CheckATB() ) {
            UInt iATB = pBattleInstance->GetATB();
            if ( iATB > iHighestATB ) { 
                iHighestATB = iATB;
                m_pTurnMonster = pBattleInstance;
                m_bPlayerTurn = false;
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

    // Handle status effects
    UInt iStatusEffectCount = m_pTurnMonster->GetStatusEffectCount();
    for ( UInt i = 0; i < iStatusEffectCount; ++i ) {
        StatusEffectInstance * pStatusEffectInstance = m_pTurnMonster->GetStatusEffect( i );

        pStatusEffectInstance->OnTurnStart( m_pTurnMonster );
    }

    // Handle passive effects (OnTurnStart)

    // Returns whether the monster can play his turn
    return !( m_pTurnMonster->IsDisabled() || m_pTurnMonster->IsDead() );
}
Void Battle::HandleChoice( UInt iSkillChoice, Bool bTargetPlayerTeamElseAI, UInt iTargetMonster )
{
    Assert( m_bTurnPending );
    Assert( m_bTurnInProgress );

    // Get selection setup
    BattleTeam * pAllyTeam = m_bPlayerTurn ? m_pPlayerTeam : m_pAITeam;
    BattleTeam * pEnnemyTeam = m_bPlayerTurn ? m_pAITeam : m_pPlayerTeam;

    BattleTeam * pTargetTeam = bTargetPlayerTeamElseAI ? m_pPlayerTeam : m_pAITeam;
    MonsterBattleInstance * pTarget = pTargetTeam->GetTeamMember( iTargetMonster );

    // Check selection validity (should never fail at this point)
    if ( pTargetTeam == pAllyTeam ) {
        Assert( pSkillInstance->TargetsAllies() );
    } else {
        Assert( pSkillInstance->TargetsEnnemies() );
    }

    // Cast the skill (apply skill effects)
    SkillInstance * pSkillInstance = m_pTurnMonster->GetBaseInstance()->GetSkill( iSkillChoice );

    pSkillInstance->Cast( pAllyTeam, m_pTurnMonster, pEnnemyTeam, pTargetTeam, pTarget );
    m_pTurnMonster->TriggerSkillCooldown( iSkillChoice );

    // Handle passive effects (OnHit, OnCrit, OnBeingHit, OnBeingCrit)

    /////////////////////////////////////////

    // Handle "additional turn" effects
    if ( ) // check for a skill proc
        m_bExtraTurn = true;
    if ( m_pTurnMonster->GetBaseInstance()->HasSetBonus(RUNE_VIOLENT) ) {
        if ( GameplayFn->CheckRandomEvent(0.22f) ) // check for violent proc
            m_bExtraTurn = true;
    }
}
Bool Battle::EndTurn()
{
    Assert( m_bTurnPending );
    Assert( m_bTurnInProgress );

    /////////////////////////////////////////

    // Handle passive effects (OnTurnEnd)

    // Remove expired status effects
    UInt iStatusEffectCount = m_pTurnMonster->GetStatusEffectCount();
    for ( UInt i = 0; i < iStatusEffectCount; ++i ) {
        StatusEffectInstance * pStatusEffectInstance = m_pTurnMonster->GetStatusEffect( i );

        pStatusEffectInstance->DecreaseDuration();
        if ( pStatusEffectInstance->IsExpired() )
            m_pTurnMonster->RemoveStatusEffect( i );
    }

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

















Bool bValid = false;
switch( m_pActiveLeaderSkill->GetLeaderConstraint() ) {
case LeaderSkill::LEADER_CONSTRAINT_NONE:
    bValid = true;
    break;
case LeaderSkill::LEADER_CONSTRAINT_FIRE:
    bValid = ( m_pMonsterInstance->GetElement() == MONSTER_ELEMENT_FIRE );
    break;
case LeaderSkill::LEADER_CONSTRAINT_WATER:
    bValid = ( m_pMonsterInstance->GetElement() == MONSTER_ELEMENT_WATER );
    break;
case LeaderSkill::LEADER_CONSTRAINT_WIND:
    bValid = ( m_pMonsterInstance->GetElement() == MONSTER_ELEMENT_WIND );
    break;
case LeaderSkill::LEADER_CONSTRAINT_LIGHT:
    bValid = ( m_pMonsterInstance->GetElement() == MONSTER_ELEMENT_LIGHT );
    break;
case LeaderSkill::LEADER_CONSTRAINT_DARK:
    bValid = ( m_pMonsterInstance->GetElement() == MONSTER_ELEMENT_DARK );
    break;
case LeaderSkill::LEADER_CONSTRAINT_DUNGEON:
    bValid = ( m_iBattleType == BATTLE_DUNGEON );
    break;
case LeaderSkill::LEADER_CONSTRAINT_ARENA:
    bValid = ( m_iBattleType == BATTLE_ARENA );
    break;
case LeaderSkill::LEADER_CONSTRAINT_GUILDBATTLE:
    bValid = ( m_iBattleType == BATTLE_GUILD );
    break;
default: Assert(false); break;
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

