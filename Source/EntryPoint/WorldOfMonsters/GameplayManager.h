/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/GameplayManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters : Global gameplay interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPLAYMANAGER_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPLAYMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Datastruct/Table/TreeMap.h"
#include "../../Lib/Math/Random/Random.h"

// TODO : migrate those XMLs into a database !
#include "../../Engine/Resource/ResourceManager.h"

#include "GameParameters.h"

#include "Monsters\Statistics.h"
#include "Monsters\StatusEffect.h"
#include "Monsters\Skill.h"
#include "Monsters\Rune.h"
#include "Monsters\Monster.h"

#include "Battle\Battle.h"

#include "Town\PlayerTown.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GameplayFn GameplayManager::GetInstancePtr()

// Parameter filr
#define GAMEPLAY_PARAMETERS_FILE TEXT("GameParameters.xml")

// Database paths
#define MONSTER_XML_PATH TEXT("Monsters")

/////////////////////////////////////////////////////////////////////////////////
// The GameplayManager class
class GameplayManager : public Manager<GameplayManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<GameplayManager>;
    GameplayManager();
    virtual ~GameplayManager();

public:
    Void Initialize();
    Void Cleanup();

    // Random number generator
    inline UInt GetRandomUInt( UInt iRange ) const; // in [0;iRange-1]
    inline Int GetRandomInt( Int iRange ) const;    // in [-iRange+1;iRange-1]
    inline Float GetRandomFloat() const;            // in [0;1]

    inline Bool CheckRandomEvent( Float fSuccessRate ) const;

    // Name identifiers
    inline const GChar * GetMonsterStatName( MonsterStatistic iStat ) const;




    // GameParameters access
    inline const GameParameters * GetGameParameters() const;




    // Skill database
    inline Skill * GetSkill( SkillID iSkillID ) const;

    // Monster database
    inline Monster * GetMonster( MonsterID iMonsterID ) const;

    // Battle system

    // Town system

    // World system

private:
    // Helpers
    Void _LoadGameDatabase();

    // Game parameters
    GameParameters m_hParameters;

    // Skill database
    inline static Int _Compare_SkillIDs( const SkillID & rLeft, const SkillID & rRight, Void * pUserData );

    typedef TreeMap<SkillID,Skill*> SkillMap;
    SkillMap m_mapSkills;

    // Monster database
    inline static Int _Compare_MonsterIDs( const MonsterID & rLeft, const SkillID & rRight, Void * pUserData );

    typedef TreeMap<MonsterID,Monster*> MonsterMap;
    MonsterMap m_mapMonsters;

    // World database

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GameplayManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPLAYMANAGER_H



//Bool bValid = false;
//switch( m_pActiveLeaderSkill->GetLeaderConstraint() ) {
//case LeaderSkill::LEADER_CONSTRAINT_NONE:
//    bValid = true;
//    break;
//case LeaderSkill::LEADER_CONSTRAINT_FIRE:
//    bValid = ( m_pMonsterInstance->GetElement() == MONSTER_ELEMENT_FIRE );
//    break;
//case LeaderSkill::LEADER_CONSTRAINT_WATER:
//    bValid = ( m_pMonsterInstance->GetElement() == MONSTER_ELEMENT_WATER );
//    break;
//case LeaderSkill::LEADER_CONSTRAINT_WIND:
//    bValid = ( m_pMonsterInstance->GetElement() == MONSTER_ELEMENT_WIND );
//    break;
//case LeaderSkill::LEADER_CONSTRAINT_LIGHT:
//    bValid = ( m_pMonsterInstance->GetElement() == MONSTER_ELEMENT_LIGHT );
//    break;
//case LeaderSkill::LEADER_CONSTRAINT_DARK:
//    bValid = ( m_pMonsterInstance->GetElement() == MONSTER_ELEMENT_DARK );
//    break;
//case LeaderSkill::LEADER_CONSTRAINT_DUNGEON:
//    bValid = ( m_iBattleType == BATTLE_DUNGEON );
//    break;
//case LeaderSkill::LEADER_CONSTRAINT_ARENA:
//    bValid = ( m_iBattleType == BATTLE_ARENA );
//    break;
//case LeaderSkill::LEADER_CONSTRAINT_GUILDBATTLE:
//    bValid = ( m_iBattleType == BATTLE_GUILD );
//    break;
//default: Assert(false); break;
//}
//
