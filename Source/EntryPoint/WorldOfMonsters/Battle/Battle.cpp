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

/////////////////////////////////////////////////////////////////////////////////
// MonsterBattleInstance implementation
MonsterBattleInstance::MonsterBattleInstance( MonsterInstance * pMonsterInstance ):
    m_arrActiveEffects()
{
    m_pMonsterInstance = pMonsterInstance;

    m_iCurrentHP = 0;
    m_iCurrentATB = 0;

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
    for( UInt i = 0; i < m_arrActiveEffects.Count(); ++i ) {

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

    m_bTurnHandled = false;
    m_bNextTurnIsPlayer = false;
    m_pNextTurnMonster = NULL;
}
Battle::~Battle()
{
    // nothing to do
}

Void Battle::Initialize()
{
    // Set all monsters to full HP and 0 ATB

    // Cleanup all skill cooldowns

    // Cleanup all effects
}

Bool Battle::TimeStep()
{
    if ( !m_bTurnHandled )
        return true; // a turn is already pending, don't step more yet

    // Step all monsters through time
        // => add their speed stat to their ATB
        // Detect who's turn it is, update m_bNextTurnIsPlayer and  m_pNextTurnMonster

    // Return true if someone's turn has come up
    //m_bTurnHandled = false;
    //return true;
}
Void Battle::HandleNextTurn( UInt iChoice, UInt iTarget )
{
    if ( m_bTurnHandled )
        return; // already done, nothing to do

    if ( m_bNextTurnIsPlayer ) {
        // It's player's turn, he made his choice, let's roll !

        // m_pNextTurnMonster
        // get skill interface ...
    } else {
        // It's AI's turn, he computed his choice, let's roll !

        // m_pNextTurnMonster
        // get skill interface ...
    }

    // Turn handled, time step can continue
    m_bTurnHandled = true;
}

