/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/BuildingMonsterEvolution.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : BuildingMonsterEvolution interface
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
#include "BuildingMonsterEvolution.h"

/////////////////////////////////////////////////////////////////////////////////
// BuildingMonsterEvolution implementation
BuildingMonsterEvolution::BuildingMonsterEvolution( BuildingDungeon * pDungeon ):
    Building( BUILDING_MONSTER_EVOLUTION, BUILDING_COST_MANA, 10000 )
{
    m_pDungeon = pDungeon;
}
BuildingMonsterEvolution::~BuildingMonsterEvolution()
{
    // nothing to do
}

Bool BuildingMonsterEvolution::CanPowerUp( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount ) const
{
    // Get monster 
}
Void BuildingMonsterEvolution::PowerUp( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount )
{
}

Bool BuildingMonsterEvolution::CanEvolve( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount ) const
{
}
Void BuildingMonsterEvolution::Evolve( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount )
{
}
