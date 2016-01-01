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

// TODO : migrate those XMLs into a database !
#include "../../Engine/Resource/ResourceManager.h"

#include "Monsters\Statistics.h"
#include "Monsters\StatusEffect.h"
#include "Monsters\Skill.h"
#include "Monsters\Rune.h"
#include "Monsters\Monster.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GameplayFn GameplayManager::GetInstancePtr()

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

    // Skill database
    inline Skill * GetSkill( SkillID iSkillID ) const;

    // Monster database
    inline Monster * GetMonster( MonsterID iMonsterID ) const;

    // Battle system

    // Town system

    // World system

private:
    // Helpers
    Void _LoadDatabase();

    // Skill database
    inline static Int _Compare_SkillIDs( const SkillID & rLeft, const SkillID & rRight, Void * pUserData );

    typedef TreeMap<SkillID,Skill*> SkillMap;
    SkillMap m_mapSkills;

    // Monster database
    inline static Int _Compare_MonsterIDs( const MonsterID & rLeft, const SkillID & rRight, Void * pUserData );

    typedef TreeMap<MonsterID,Monster*> MonsterMap;
    MonsterMap m_mapMonsters;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GameplayManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPLAYMANAGER_H

