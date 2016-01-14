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
#define BATTLE_TEAM_MAX_SIZE 16

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
    BattleTeam( UInt iTeamSize );
    ~BattleTeam();

    // Getters
    inline UInt GetTeamSize() const;
    inline BattleMonsterInstance * GetTeamMember( UInt iIndex );

private:
    UInt m_iTeamSize;
    BattleMonsterInstance m_arrMonsters[BATTLE_TEAM_MAX_SIZE];
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

    inline BattleTeam * GetPlayerTeam() const;
    inline BattleTeam * GetAITeam() const;

    // Battle interface
    inline Bool IsTurnPending() const;
    inline Bool IsTurnInProgress() const;
    inline Bool IsPlayerTurn() const;
    inline Bool IsAITurn() const;
    Bool IsPlayerDead() const;
    Bool IsAIDead() const;

    inline BattleMonsterInstance * GetTurnMonster() const;

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

    static UInt _ResolveEnnemyTargets( BattleMonsterInstance ** outTargets, SkillTargetPattern iPattern,
                                       BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam, UInt iTarget );
    static UInt _ResolveAllyTargets( BattleMonsterInstance ** outTargets, SkillTargetPattern iPattern,
                                     BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam, UInt iTarget );

    static UInt _ComputeDamage( SkillInstance * pSkillInstance, SkillEffectDamage * pDamageEffect, BattleMonsterInstance * pCaster, BattleMonsterInstance * pTarget,
                                Bool * outIsCrit, Bool * outIsGlancing, Bool * outIsCrushing );
    static UInt _ComputeHeal( SkillInstance * pSkillInstance, SkillEffectHeal * pHealEffect, BattleMonsterInstance * pCaster, BattleMonsterInstance * pTarget );

    static Void _HandleSkillEffect( BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam,
                                    SkillInstance * pSkillInstance, SkillEffect * pSkillEffect, BattleMonsterInstance ** arrTargets, UInt iTargetCount );

    static Void _HandlePassives( SkillPassiveType iPassiveType, BattleTeam * pCasterTeam, UInt iCaster, BattleTeam * pTargetTeam, UInt iTarget );

    //SKILL_PASSIVE_PERSISTENT = 0,
    //    SKILL_PASSIVE_PERIODIC_SELF,
    //    SKILL_PASSIVE_PERIODIC_ALLIES,
    //    SKILL_PASSIVE_PERIODIC_ENNEMIES,
    //    SKILL_PASSIVE_PERIODIC_ALL,
    //    SKILL_PASSIVE_ONHIT,
    //    SKILL_PASSIVE_ONHIT_ALLY,
    //    SKILL_PASSIVE_ONHIT_ENNEMY,
    //    SKILL_PASSIVE_ONCRIT,
    //    SKILL_PASSIVE_ONCRIT_ALLY,
    //    SKILL_PASSIVE_ONCRIT_ENNEMY,
    //    SKILL_PASSIVE_ONBEINGHIT,
    //    SKILL_PASSIVE_ONBEINGHIT_ALLY,
    //    SKILL_PASSIVE_ONBEINGHIT_ENNEMY,
    //    SKILL_PASSIVE_ONBEINGCRIT,
    //    SKILL_PASSIVE_ONBEINGCRIT_ALLY,
    //    SKILL_PASSIVE_ONBEINGCRIT_ENNEMY,

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

