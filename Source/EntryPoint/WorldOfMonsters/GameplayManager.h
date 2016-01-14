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
#include "GameParameters.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GameplayFn GameplayManager::GetInstancePtr()

// XML files
#define GAMEPLAY_PARAMETERS_FILE TEXT("GameParameters.xml")

#define GAMEPLAY_MONSTERFILES_PATH    TEXT("Monsters")
#define GAMEPLAY_PLAYERTOWNFILES_PATH TEXT("PlayerTowns")

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

    // GameParameters access
    inline const GameParameters * GetGameParameters() const;

    // Skill database
    inline Skill * GetSkill( SkillID iSkillID ) const;

    // Monster database
    inline Monster * GetMonster( MonsterID iMonsterID ) const;

    // World database

    // Town system
    PlayerTown * LoadPlayerTown( const GChar * strTownName ) const;
    PlayerTown * CreatePlayerTown( const GChar * strTownName ) const;
    Void SavePlayerTown( const PlayerTown * pPlayerTown ) const;
    Void ReleasePlayerTown( PlayerTown * pPlayerTown, Bool bSave = true ) const;

    // Battle system

private:
    // Helpers
    Void _LoadGameDatabase( const GChar * strPath );

    // Game parameters
    GameParameters m_hGameParameters;

    // Skill database
    inline static Int _Compare_SkillIDs( const SkillID & rLeft, const SkillID & rRight, Void * pUserData );

    typedef TreeMap<SkillID,Skill*> SkillMap;
    SkillMap m_mapSkills;

    // Monster database
    inline static Int _Compare_MonsterIDs( const MonsterID & rLeft, const MonsterID & rRight, Void * pUserData );

    typedef TreeMap<MonsterID,Monster> MonsterMap;
    MonsterMap m_mapMonsters;

    // World database

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GameplayManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPLAYMANAGER_H


