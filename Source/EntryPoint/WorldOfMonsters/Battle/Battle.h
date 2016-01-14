/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Battle/Battle.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_BATTLE_BATTLE_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_BATTLE_BATTLE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BattleMonsterInstance.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Battle teams
#define BATTLE_TEAMSIZE_MAX 16
#define BATTLE_TEAMSIZE_SCENARIO 4
#define BATTLE_TEAMSIZE_DUNGEON  5
#define BATTLE_TEAMSIZE_ARENA 4
#define BATTLE_TEAMSIZE_GUILD 3

// Battle types
enum BattleType {
    BATTLE_SCENARIO = 0,
    BATTLE_DUNGEON,
    BATTLE_ARENA,
    BATTLE_GUILD
};

/////////////////////////////////////////////////////////////////////////////////
// The BattleTeam class
class BattleTeam
{
public:
    BattleTeam( BattleType iType, const PlayerTown * pPlayerTown, const MonsterInstance * arrMonsters );
    ~BattleTeam();

    // Getters
    inline UInt GetTeamSize() const;
    inline const BattleMonsterInstance * GetTeamMember( UInt iIndex ) const;
    inline BattleMonsterInstance * GetTeamMember( UInt iIndex );

private:
    BattleType m_iType;
    const PlayerTown * m_pPlayerTown;

    UInt m_iTeamSize;
    BattleMonsterInstance m_arrMonsters[BATTLE_TEAMSIZE_MAX];
};

/////////////////////////////////////////////////////////////////////////////////
// The Battle class
class Battle
{
public:
    Battle( BattleType iType, BattleTeam * pPlayerTeam, BattleTeam * pAITeam );
    ~Battle();

    // Getters
    inline BattleType GetType() const;

    inline const BattleTeam * GetPlayerTeam() const;
    inline const BattleTeam * GetAITeam() const;

    // Battle interface
    inline Bool IsTurnPending() const;
    inline Bool IsTurnInProgress() const;
    inline Bool IsPlayerTurn() const;
    inline Bool IsAITurn() const;
    Bool IsPlayerDead() const;
    Bool IsAIDead() const;

    inline const BattleMonsterInstance * GetTurnMonster() const;

    Void Initialize();

    Bool TimeStep();

    Bool StartTurn();
    Void HandleChoice( UInt iSkillChoice, Bool bTargetPlayerTeamElseAI, UInt iTargetMonster );
    Bool EndTurn();

protected:
    // Helpers
    inline static Bool _IsElementWeakAgainst( MonsterElement iElement, MonsterElement iOpposingElement );
    inline static Bool _IsElementStrongAgainst( MonsterElement iElement, MonsterElement iOpposingElement );

    inline static MonsterElement _GetElementWeakAgainst( MonsterElement iElement );
    inline static MonsterElement _GetElementStrongAgainst( MonsterElement iElement );

    static MonsterElement sm_arrElementWeakAgainst[MONSTER_ELEMENT_COUNT];
    static MonsterElement sm_arrElementStrongAgainst[MONSTER_ELEMENT_COUNT];

    static UInt _ResolveEnnemyTargets( BattleMonsterInstance ** outTargets, SkillTargetPattern iPattern, BattleTeam * pTargetTeam, UInt iTarget );
    static UInt _ResolveAllyTargets( BattleMonsterInstance ** outTargets, SkillTargetPattern iPattern, BattleTeam * pCasterTeam, UInt iCaster, UInt iTarget );

    static UInt _ComputeDamage( const SkillInstance * pSkillInstance, const SkillEffectDamage * pDamageEffect, BattleMonsterInstance * pCaster, BattleMonsterInstance * pTarget,
                                Bool * outIsCrit, Bool * outIsGlancing, Bool * outIsCrushing );
    static UInt _ComputeHeal( const SkillInstance * pSkillInstance, const SkillEffectHeal * pHealEffect, BattleMonsterInstance * pCaster, BattleMonsterInstance * pTarget );

    static Void _HandleSkillEffect( BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam,
                                    const SkillInstance * pSkillInstance, const SkillEffect * pSkillEffect, BattleMonsterInstance ** arrTargets, UInt iTargetCount );

    static Void _HandlePassives( SkillPassiveType iPassiveType, BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam, UInt iTarget );

    // Battle type
    BattleType m_iType;

    // Teams
    BattleTeam * m_pPlayerTeam;
    BattleTeam * m_pAITeam;

    // ATB State
    Bool m_bTurnPending, m_bTurnInProgress, m_bExtraTurn;
    Bool m_bPlayerTurn; // Else AI
    UInt m_iTurnMonster;
    BattleMonsterInstance * m_pTurnMonster;
};


/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Battle.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_BATTLE_BATTLE_H

