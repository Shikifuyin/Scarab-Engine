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
MonsterBattleInstance::MonsterBattleInstance( MonsterInstance * pMonsterInstance ):
    m_arrActiveEffects()
{
    m_pMonsterInstance = pMonsterInstance;

    m_iCurrentHP = 0;
    m_iATB = 0;

    m_arrSkillCooldowns[0] = 0;
    m_arrSkillCooldowns[1] = 0;
    m_arrSkillCooldowns[2] = 0;
    m_arrSkillCooldowns[3] = 0;

    m_arrActiveEffects.UseMemoryContext( ApplicationFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrActiveEffects.Create();

    _UpdateBattleStats();
    m_iCurrentHP = m_iHealth;
}
MonsterBattleInstance::~MonsterBattleInstance()
{
    m_arrActiveEffects.Destroy();
}

/////////////////////////////////////////////////////////////////////////////////

Void MonsterBattleInstance::_UpdateBattleStats()
{
    // Start with effective stats
    m_iHealth = m_pMonsterInstance->GetEffectiveHP();
    m_iAttack = m_pMonsterInstance->GetEffectiveATT();
    m_iDefense = m_pMonsterInstance->GetEffectiveDEF();
    m_iSpeed = m_pMonsterInstance->GetEffectiveSPD();

    m_fCriticalRate = m_pMonsterInstance->GetEffectiveCritR();
    m_fCriticalDamage = m_pMonsterInstance->GetEffectiveCritD();
    m_fAccuracy = m_pMonsterInstance->GetEffectiveACC();
    m_fResistance = m_pMonsterInstance->GetEffectiveRES();

    // Add leader skill bonuses if any
    LeaderSkill * pLeaderSkill = m_pMonsterInstance->GetLeaderSkill();
    if ( pLeaderSkill != NULL ) {
        //////////////////////////////////////
    }

    // Apply StatusEffects modifiers
    m_bDisabled = false;
    for( UInt i = 0; i < m_arrActiveEffects.Count(); ++i ) {
        StatusEffectInstance * pStatusEffectInstance = &(m_arrActiveEffects[i]);

        pStatusEffectInstance->OnUpdateBattleStats( this );

        if ( pStatusEffectInstance->IsDisabling() )
            m_bDisabled = true;
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

