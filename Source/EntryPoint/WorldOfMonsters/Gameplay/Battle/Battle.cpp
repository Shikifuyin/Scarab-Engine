/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Gameplay/Battle/Battle.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Battle : Battle interface
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
// BattleTeam implementation
BattleTeam::BattleTeam( BattleType iType, const PlayerTown * pPlayerTown, const MonsterInstance * arrMonsters )
{
    m_iType = iType;
    m_pPlayerTown = pPlayerTown;

    m_iTeamSize = 0;
    switch( m_iType ) {
        case BATTLE_SCENARIO: m_iTeamSize = BATTLE_TEAMSIZE_SCENARIO; break;
        case BATTLE_DUNGEON:  m_iTeamSize = BATTLE_TEAMSIZE_DUNGEON;  break;
        case BATTLE_ARENA:    m_iTeamSize = BATTLE_TEAMSIZE_ARENA;    break;
        case BATTLE_GUILD:    m_iTeamSize = BATTLE_TEAMSIZE_GUILD;    break;
        default: Assert( false ); break;
    }

    const LeaderSkill * pActiveLeaderSkill = NULL;
    UInt iSkillCount = arrMonsters[0].GetSkillCount();
    for( UInt i = 0; i < iSkillCount; ++i ) {
        if ( arrMonsters[0].GetSkillInstance(i)->IsLeader() ) {
            pActiveLeaderSkill = (const LeaderSkill *)( arrMonsters[0].GetSkillInstance(i)->GetSkill() );
            break;
        }
    }

    for( UInt i = 0; i < m_iTeamSize; ++i ) {
        const MonsterInstance * pMonsterInstance = ( arrMonsters + i );

        Bool bValid = false;
        switch( pActiveLeaderSkill->GetLeaderConstraint() ) {
            case SKILL_LEADERCONSTRAINT_NONE:
                bValid = true;
                break;
            case SKILL_LEADERCONSTRAINT_FIRE:
                bValid = ( pMonsterInstance->GetElement() == MONSTER_ELEMENT_FIRE );
                break;
            case SKILL_LEADERCONSTRAINT_WATER:
                bValid = ( pMonsterInstance->GetElement() == MONSTER_ELEMENT_WATER );
                break;
            case SKILL_LEADERCONSTRAINT_WIND:
                bValid = ( pMonsterInstance->GetElement() == MONSTER_ELEMENT_WIND );
                break;
            case SKILL_LEADERCONSTRAINT_LIGHT:
                bValid = ( pMonsterInstance->GetElement() == MONSTER_ELEMENT_LIGHT );
                break;
            case SKILL_LEADERCONSTRAINT_DARK:
                bValid = ( pMonsterInstance->GetElement() == MONSTER_ELEMENT_DARK );
                break;
            case SKILL_LEADERCONSTRAINT_DUNGEON:
                bValid = ( m_iType == BATTLE_DUNGEON );
                break;
            case SKILL_LEADERCONSTRAINT_ARENA:
                bValid = ( m_iType == BATTLE_ARENA );
                break;
            case SKILL_LEADERCONSTRAINT_GUILDBATTLE:
                bValid = ( m_iType == BATTLE_GUILD );
                break;
            default: Assert(false); break;
        }

        m_arrMonsters[i] = BattleMonsterInstance( arrMonsters + i, bValid ? pActiveLeaderSkill : NULL );
    }
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
        BattleMonsterInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
        if ( pBattleInstance->IsAlive() )
            return false;
    }
    return true;
}
Bool Battle::IsAIDead() const
{
    for( UInt i = 0; i < m_pAITeam->GetTeamSize(); ++i ) {
        BattleMonsterInstance * pBattleInstance = m_pAITeam->GetTeamMember( i );
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
        BattleMonsterInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
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
        BattleMonsterInstance * pBattleInstance = m_pAITeam->GetTeamMember( i );
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
        BattleMonsterInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember( i );
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
        BattleMonsterInstance * pBattleInstance = m_pAITeam->GetTeamMember( i );
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
    m_pTurnMonster->DecreaseATB( BATTLE_ATB_CAPACITY );

    // Handle cooldowns
    for( UInt i = 0; i < m_pPlayerTeam->GetTeamSize(); ++i ) {
        BattleMonsterInstance * pBattleInstance = m_pPlayerTeam->GetTeamMember(i);
        for ( UInt j = 0; j < pBattleInstance->GetSkillCount(); ++j )
            pBattleInstance->DecreaseSkillCooldown(j);
    }
    for( UInt i = 0; i < m_pAITeam->GetTeamSize(); ++i ) {
        BattleMonsterInstance * pBattleInstance = m_pAITeam->GetTeamMember(i);
        for ( UInt j = 0; j < pBattleInstance->GetSkillCount(); ++j )
            pBattleInstance->DecreaseSkillCooldown(j);
    }

    // Handle status effects
    if ( m_pTurnMonster->HasStatusEffect(STATUSEFFECT_BUFF_REGEN) ) {
        StatusEffect * pStatusEffect = m_pTurnMonster->GetStatusEffect(STATUSEFFECT_BUFF_REGEN);
        Float fHP = (Float)( m_pTurnMonster->GetHP() );

        UInt iHealAmount = (UInt)( MathFn->Floor(pStatusEffect->GetAmplitude(0) * fHP) );

        m_pTurnMonster->Heal( iHealAmount );
    }
    if ( m_pTurnMonster->HasStatusEffect(STATUSEFFECT_DEBUFF_DOT) ) {
        StatusEffect * pStatusEffect = m_pTurnMonster->GetStatusEffect(STATUSEFFECT_DEBUFF_DOT);
        Float fHP = (Float)( m_pTurnMonster->GetHP() );

        UInt iDamageAmount = 0;
        for ( UInt i = 0; i < pStatusEffect->GetStackCount(); ++i )
            iDamageAmount += (UInt)( MathFn->Floor(pStatusEffect->GetAmplitude(i) * fHP) );

        m_pTurnMonster->Damage( iDamageAmount );
    }
    if ( m_pTurnMonster->HasStatusEffect(STATUSEFFECT_DEBUFF_BOMB) ) {
        StatusEffect * pStatusEffect = m_pTurnMonster->GetStatusEffect(STATUSEFFECT_DEBUFF_BOMB);

        // Final dmg value already, passed as amplitude
        UInt iDamageAmount = 0;
        for ( UInt i = 0; i < pStatusEffect->GetStackCount(); ++i )
            iDamageAmount += (UInt)( MathFn->Floor(pStatusEffect->GetAmplitude(i)) ); 

        m_pTurnMonster->Damage( iDamageAmount );
    }

    // Handle passive effects
    BattleTeam * pAllyTeam = m_bPlayerTurn ? m_pPlayerTeam : m_pAITeam;
    BattleTeam * pEnnemyTeam = m_bPlayerTurn ? m_pAITeam : m_pPlayerTeam;

    for( UInt i = 0; i < pAllyTeam->GetTeamSize(); ++i ) {
        if ( i == m_iTurnMonster )
            _HandlePassives( SKILL_PASSIVE_PERIODIC_SELF, pAllyTeam, m_iTurnMonster, pAllyTeam, m_iTurnMonster );
        else
            _HandlePassives( SKILL_PASSIVE_PERIODIC_ALLIES, pAllyTeam, i, pAllyTeam, m_iTurnMonster );
        _HandlePassives( SKILL_PASSIVE_PERIODIC_ALL, pAllyTeam, i, pAllyTeam, m_iTurnMonster );
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
    BattleMonsterInstance * pTarget = pTargetTeam->GetTeamMember( iTargetMonster );

    // Get skill instance
    const SkillInstance * pSkillInstance = m_pTurnMonster->GetSkillInstance( iSkillChoice );
    Assert( pSkillInstance->IsActive() );

    // Trigger cooldown
    if ( pSkillInstance->HasCooldown() )
        m_pTurnMonster->TriggerSkillCooldown( iSkillChoice );

    // Enum all types of effects
    for ( UInt i = 0; i < SKILL_ACTIVE_COUNT; ++i ) {
        SkillActiveType iActiveType = (SkillActiveType)i;

        // Check if there are effects to handle
        const ActiveSkill * pActiveSkill = (const ActiveSkill *)( pSkillInstance->GetSkill() );
        UInt iEffectCount = pActiveSkill->GetEffectCount( iActiveType );
        Assert( iEffectCount > 0 );

        // Handle all effects
        BattleMonsterInstance * arrTargets[BATTLE_TEAMSIZE_MAX];
        UInt iTargetCount = 0;

        for ( UInt j = 0; j < iEffectCount; ++j ) {
            const SkillEffect * pSkillEffect = pActiveSkill->GetEffect( iActiveType, j );

            // Resolve target pattern
            iTargetCount = _ResolveAllyTargets( arrTargets, pSkillEffect->GetTargetPattern(), pAllyTeam, m_iTurnMonster, iTargetMonster );

            // Handle effect
            _HandleSkillEffect( pAllyTeam, m_iTurnMonster, pTargetTeam, pSkillInstance, pSkillEffect, arrTargets, iTargetCount );

            // Resolve target pattern
            iTargetCount = _ResolveEnnemyTargets( arrTargets, pSkillEffect->GetTargetPattern(), pTargetTeam, iTargetMonster );

            // Handle effect
            _HandleSkillEffect( pAllyTeam, m_iTurnMonster, pTargetTeam, pSkillInstance, pSkillEffect, arrTargets, iTargetCount );
        }
    }

    // Handle extra turn skills
    //if ( )
    //    m_bExtraTurn = true;

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Handle violent runes
    if ( m_pTurnMonster->GetMonsterInstance()->HasSetBonus(RUNE_VIOLENT) ) {
        Float fProc = pGameParams->GetRuneSetStatBonus( RUNE_VIOLENT );
        if ( GameplayFn->CheckRandomEvent(fProc) )
            m_bExtraTurn = true;
    }

    // Handle revenge runes
    UInt iSetCount = 0;
    if ( pTargetTeam == pEnnemyTeam && pTarget->GetMonsterInstance()->HasSetBonus(RUNE_REVENGE, &iSetCount) ) {
        Float fProc = ( pGameParams->GetRuneSetStatBonus(RUNE_REVENGE) * (Float)iSetCount );
        if ( GameplayFn->CheckRandomEvent(fProc) ) {
            m_bPlayerTurn = !m_bPlayerTurn;
            m_iTurnMonster = iTargetMonster;
            m_pTurnMonster = pTarget;
            m_bExtraTurn = true;
        }
    }
}
Bool Battle::EndTurn()
{
    Assert( m_bTurnPending );
    Assert( m_bTurnInProgress );

    /////////////////////////////////////////

    // Update status effects durations
    for ( UInt i = 0; i < STATUSEFFECT_COUNT; ++i ) {
        StatusEffectType iType = (StatusEffectType)i;
        if ( m_pTurnMonster->HasStatusEffect(iType) ) {
            StatusEffect * pStatusEffect = m_pTurnMonster->GetStatusEffect( iType );
            for ( UInt j = 0; j < pStatusEffect->GetStackCount(); ++j )
                pStatusEffect->DecreaseDuration( j, 1 );
        }
    }
    m_pTurnMonster->RemoveExpiredStatusEffects();

    // Done with this turn
    m_bTurnInProgress = false;

    // Handle "extra turn" effects
    if ( !m_bExtraTurn )
        m_bTurnPending = false;
    return m_bTurnPending;
}

/////////////////////////////////////////////////////////////////////////////////

MonsterElement Battle::sm_arrElementWeakAgainst[MONSTER_ELEMENT_COUNT] = {
    MONSTER_ELEMENT_COUNT, // Weak vs MONSTER_ELEMENT_MAGIC
    MONSTER_ELEMENT_WIND,  // Weak vs MONSTER_ELEMENT_FIRE
    MONSTER_ELEMENT_FIRE,  // Weak vs MONSTER_ELEMENT_WATER
    MONSTER_ELEMENT_WATER, // Weak vs MONSTER_ELEMENT_WIND
    MONSTER_ELEMENT_COUNT, // Weak vs MONSTER_ELEMENT_LIGHT
    MONSTER_ELEMENT_COUNT  // Weak vs MONSTER_ELEMENT_DARK
};
MonsterElement Battle::sm_arrElementStrongAgainst[MONSTER_ELEMENT_COUNT] = {
    MONSTER_ELEMENT_COUNT, // Strong vs MONSTER_ELEMENT_MAGIC
    MONSTER_ELEMENT_WATER, // Strong vs MONSTER_ELEMENT_FIRE
    MONSTER_ELEMENT_WIND,  // Strong vs MONSTER_ELEMENT_WATER
    MONSTER_ELEMENT_FIRE,  // Strong vs MONSTER_ELEMENT_WIND
    MONSTER_ELEMENT_DARK,  // Strong vs MONSTER_ELEMENT_LIGHT
    MONSTER_ELEMENT_LIGHT  // Strong vs MONSTER_ELEMENT_DARK
};

UInt Battle::_ResolveEnnemyTargets( BattleMonsterInstance ** outTargets, SkillTargetPattern iPattern, BattleTeam * pTargetTeam, UInt iTarget )
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
UInt Battle::_ResolveAllyTargets( BattleMonsterInstance ** outTargets, SkillTargetPattern iPattern, BattleTeam * pCasterTeam, UInt iCaster, UInt iTarget )
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

UInt Battle::_ComputeDamage( const SkillInstance * pSkillInstance, const SkillEffectDamage * pDamageEffect, BattleMonsterInstance * pCaster, BattleMonsterInstance * pTarget,
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
    MonsterElement iCasterElement = pCaster->GetMonsterInstance()->GetElement();
    MonsterElement iTargetElement = pTarget->GetMonsterInstance()->GetElement();

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
    Float fSkillDamage = ( pSkillInstance->GetBonusDamage() + pDamageEffect->GetBonusDmg() );

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
UInt Battle::_ComputeHeal( const SkillInstance * pSkillInstance, const SkillEffectHeal * pHealEffect, BattleMonsterInstance * pCaster, BattleMonsterInstance * pTarget )
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
    Float fSkillHeal = ( pSkillInstance->GetBonusRecovery() + pHealEffect->GetBonusHeal() );

    // Convert to heal value
    UInt iHealAmount = (UInt)( MathFn->Floor((1.0f + fSkillHeal) * fBaseHeal) );

    // Done
    return iHealAmount;
}

Void Battle::_HandleSkillEffect( BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam,
                                 const SkillInstance * pSkillInstance, const SkillEffect * pSkillEffect, BattleMonsterInstance ** arrTargets, UInt iTargetCount )
{
    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    // Get caster
    BattleMonsterInstance * pCaster = pCasterTeam->GetTeamMember( iCaster );

    // Apply to all targets
    for( UInt i = 0; i < iTargetCount; ++i ) {
        BattleMonsterInstance * pTarget = arrTargets[i];

        // Check effect type
        switch( pSkillEffect->GetType() ) {
            case SKILLEFFECT_DAMAGE: {
                    const SkillEffectDamage * pDamageEffect = (const SkillEffectDamage *)pSkillEffect;

                    Bool bIsCrit, bIsGlancing, bIsCrushing;
                    UInt iDamage = _ComputeDamage( pSkillInstance, pDamageEffect, pCaster, pTarget, &bIsCrit, &bIsGlancing, &bIsCrushing );

                    // Handle mark_dmg status
                    if ( pTarget->HasStatusEffect(STATUSEFFECT_DEBUFF_MARK_DMG) ) {
                        StatusEffect * pStatusEffect = pTarget->GetStatusEffect( STATUSEFFECT_DEBUFF_MARK_DMG );
                        iDamage += (UInt)( MathFn->Floor(pStatusEffect->GetAmplitude(0) * (Float)iDamage) );
                    }

                    // Apply damage
                    UInt iInflictedDamage = pTarget->Damage( iDamage );

                    // Handle mark_drain status
                    if ( pTarget->HasStatusEffect(STATUSEFFECT_DEBUFF_MARK_DRAIN) ) {
                        StatusEffect * pStatusEffect = pTarget->GetStatusEffect( STATUSEFFECT_DEBUFF_MARK_DRAIN );
                        UInt iHealAmount = (UInt)( MathFn->Floor(pStatusEffect->GetAmplitude(0) * (Float)iInflictedDamage) );

                        pCaster->Heal( iHealAmount );
                    }

                    // Handle vampire runes
                    if ( pCaster->GetMonsterInstance()->HasSetBonus(RUNE_VAMPIRE) ) {
                        Float fAmplitude = pGameParams->GetRuneSetStatBonus( RUNE_VAMPIRE );
                        UInt iHealAmount = (UInt)( MathFn->Floor(fAmplitude * (Float)iInflictedDamage) );

                        pCaster->Heal( iHealAmount );
                    }

                    // Handle despair runes
                    if ( pCaster->GetMonsterInstance()->HasSetBonus(RUNE_DESPAIR) ) {
                        Float fAmplitude = pGameParams->GetRuneSetStatBonus( RUNE_DESPAIR );

                        if ( GameplayFn->CheckRandomEvent(fAmplitude) )
                            pTarget->AddStatusEffect( STATUSEFFECT_DEBUFF_STUN, 1, 1, 0.0f );
                    }

                    // Handle passive effects
                    for( UInt j = 0; j < pCasterTeam->GetTeamSize(); ++j ) {
                        BattleMonsterInstance * pAlly = pCasterTeam->GetTeamMember( j );
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
                        BattleMonsterInstance * pEnnemy = pTargetTeam->GetTeamMember( j );
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
                    const SkillEffectHeal * pHealEffect = (const SkillEffectHeal *)pSkillEffect;

                    UInt iHeal = _ComputeHeal( pSkillInstance, pHealEffect, pCaster, pTarget );

                    // Apply
                    pTarget->Heal( iHeal );

                    // Handle passive effects
                    //_HandlePassives( SKILL_PASSIVE_ONHEAL, pCaster, pTarget );
                } break;
            case SKILLEFFECT_ATB: {
                    const SkillEffectATB * pATBEffect = (const SkillEffectATB *)pSkillEffect;

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
                    const SkillEffectStatus * pStatusEffect = (const SkillEffectStatus *)pSkillEffect;

                    // Accuracy / Resistance, Check Proc

                    // Apply
                    pTarget->AddStatusEffect( pStatusEffect->GetStatusEffectType(), pStatusEffect->GetStackCount(),
                                              pStatusEffect->GetDuration(), pStatusEffect->GetAmplitude() );

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
    BattleMonsterInstance * pCaster = pCasterTeam->GetTeamMember( iCaster );

    // Check caster's skill set
    for ( UInt i = 0; i < pCaster->GetSkillCount(); ++i ) {
        // Check cooldown availability
        if ( !(pCaster->HasSkillUp(i)) )
            continue;

        // Look for passives
        const SkillInstance * pSkillInstance = pCaster->GetSkillInstance(i);
        if ( !(pSkillInstance->IsPassive()) )
            continue;

        // Check if there are effects to handle
        const PassiveSkill * pPassiveSkill = (const PassiveSkill *)( pSkillInstance->GetSkill() );
        UInt iEffectCount = pPassiveSkill->GetEffectCount( iPassiveType );
        if ( iEffectCount == 0 )
            continue;

        // Trigger cooldown
        if ( pSkillInstance->HasCooldown() )
            pCaster->TriggerSkillCooldown(i);

        // Handle all effects
        BattleMonsterInstance * arrTargets[BATTLE_TEAMSIZE_MAX];
        UInt iTargetCount = 0;

        for ( UInt j = 0; j < iEffectCount; ++j ) {
            SkillEffect * pSkillEffect = pPassiveSkill->GetEffect( iPassiveType, j );

            // Resolve target pattern
            iTargetCount = _ResolveAllyTargets( arrTargets, pSkillEffect->GetTargetPattern(), pCasterTeam, iCaster, iTarget );

            // Handle effect
            _HandleSkillEffect( pCasterTeam, iCaster, pTargetTeam, pSkillInstance, pSkillEffect, arrTargets, iTargetCount );

            // Resolve target pattern
            iTargetCount = _ResolveEnnemyTargets( arrTargets, pSkillEffect->GetTargetPattern(), pTargetTeam, iTarget );

            // Handle effect
            _HandleSkillEffect( pCasterTeam, iCaster, pTargetTeam, pSkillInstance, pSkillEffect, arrTargets, iTargetCount );
        }
    }
}



