/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Battle/Battle.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_BATTLE_BATTLE_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_BATTLE_BATTLE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Datastruct/Array/Array.h"

#include "../Monsters/Monster.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Battle teams
#define BATTLE_TEAM_MAX_SIZE 16

// Battle types
enum BattleType {
    BATTLE_SCENARIO = 0,
    BATTLE_DUNGEON,
    BATTLE_ARENA,
    BATTLE_GUILD
};

// ATB values
#define BATTLE_ATB_CAPACITY 1000 // Base speed is 100, 10 steps to take a turn
                                 // Max speeds around 200-300, 3-5 steps to take a turn

/////////////////////////////////////////////////////////////////////////////////
// The MonsterBattleInstance class
class MonsterBattleInstance
{
public:
    MonsterBattleInstance( MonsterInstance * pMonsterInstance );
    ~MonsterBattleInstance();

    // Base instance access
    inline const MonsterInstance * GetBaseInstance() const;

    // HP access
    inline Bool IsAlive() const;
    inline Bool IsDead() const;

    inline UInt GetCurrentHP() const;
    inline Void SetCurrentHP( UInt iAmount );
    inline Void ResetCurrentHP();

    inline Void Damage( UInt iAmount );
    inline Void Heal( UInt iAmount );

    // ATB access
    inline Bool CheckATB() const;
    inline UInt GetATB() const;

    inline Void IncreaseATB( Float fRatio );
    inline Void IncreaseATB( UInt iAmount );
    inline Void DecreaseATB( Float fRatio );
    inline Void DecreaseATB( UInt iAmount );
    inline Void FillATB();
    inline Void ResetATB();

    // Skill cooldowns access
    inline UInt GetSkillCooldown( UInt iSlot ) const;
    
    inline Void IncreaseSkillCooldown( UInt iSlot );
    inline Void DecreaseSkillCooldown( UInt iSlot );
    inline Void TriggerSkillCooldown( UInt iSlot );
    inline Void ResetSkillCooldown( UInt iSlot );

    // Status effects access
    inline UInt GetStatusEffectCount() const;
    inline StatusEffectInstance * GetStatusEffect( UInt iIndex ) const;

    inline Void AddStatusEffect( const StatusEffect * pEffect );
    inline Void RemoveStatusEffect( UInt iIndex );
    inline Void RemoveAllStatusEffects();

    // Battle stats access
    inline Bool IsDisabled() const;

    inline UInt GetHP() const;
    inline UInt GetATT() const;
    inline UInt GetDEF() const;
    inline UInt GetSPD() const;

    inline Float GetCritR() const;
    inline Float GetCritD() const;
    inline Float GetACC() const;
    inline Float GetRES() const;

private:
    // Helpers
    friend class StatusBuffStat;
    friend class StatusDebuffStat;

    Void _UpdateBattleStats();

    // Base instance
    MonsterInstance * m_pMonsterInstance;

    // Battle state
    UInt m_iCurrentHP;
    UInt m_iATB;
    UInt m_arrSkillCooldowns[SKILL_SLOT_COUNT];

    Array<StatusEffectInstance> m_arrActiveEffects;

    // Battle stats
    Bool m_bDisabled; // If any effect prevents this from playing

    UInt m_iHealth;
    UInt m_iAttack;
    UInt m_iDefense;
    UInt m_iSpeed;

    Float m_fCriticalRate;
    Float m_fCriticalDamage;

    Float m_fAccuracy;
    Float m_fResistance;
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
    inline MonsterBattleInstance * GetTeamMember( UInt iIndex );

private:
    UInt m_iTeamSize;
    MonsterBattleInstance m_arrMonsters[BATTLE_TEAM_MAX_SIZE];
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

    inline MonsterBattleInstance * GetTurnMonster() const;

    Void Initialize();

    Bool TimeStep();

    Bool StartTurn();
    Void HandleChoice( UInt iSkillChoice, Bool bTargetPlayerTeamElseAI, UInt iTargetMonster );
    Bool EndTurn();

protected:
    // Helpers

    // Battle type
    BattleType m_iType;

    // Teams
    BattleTeam * m_pPlayerTeam;
    BattleTeam * m_pAITeam;

    // ATB State
    Bool m_bTurnPending, m_bTurnInProgress, m_bExtraTurn;
    Bool m_bPlayerTurn; // Else AI
    MonsterBattleInstance * m_pTurnMonster;
};


/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Battle.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_BATTLE_BATTLE_H

