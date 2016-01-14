/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/Building.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Town : Base Building interface
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
#include "Building.h"

/////////////////////////////////////////////////////////////////////////////////
// Building implementation
Building::Building()
{
    //m_hCost.arrCost[CURRENCY_MANA] = 0;
    //m_hCost.arrCost[CURRENCY_CRYSTAL] = 0;
    //m_hCost.arrCost[CURRENCY_GLORYPOINTS] = 0;
    //m_hCost.arrCost[CURRENCY_GUILDPOINTS] = 0;
}
Building::~Building()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

const GChar * Building::sm_arrNames[BUILDING_COUNT] = {
    TEXT(""), // BUILDING_DUNGEON
    TEXT(""), // BUILDING_ARCANE_TOWER

    TEXT(""), // BUILDING_ESSENCE_STORAGE

    TEXT(""), // BUILDING_MONSTER_STORAGE
    TEXT(""), // BUILDING_MONSTER_SUMMONING
    TEXT(""), // BUILDING_MONSTER_EVOLUTION
    TEXT(""), // BUILDING_MONSTER_FUSION
//  TEXT(""), // BUILDING_MONSTER_SKINS

    TEXT(""), // BUILDING_RUNE_STORAGE
    TEXT(""), // BUILDING_RUNE_EVOLUTION

    TEXT(""), // BUILDING_MANA
    TEXT(""), // BUILDING_CRYSTAL

    TEXT(""), // BUILDING_SHOP
    TEXT(""), // BUILDING_WISHES

    TEXT(""), // BUILDING_BONUS_MONSTERS_HP
    TEXT(""), // BUILDING_BONUS_MONSTERS_ATT
    TEXT(""), // BUILDING_BONUS_MONSTERS_ATT_ELEMENT
    TEXT(""), // BUILDING_BONUS_MONSTERS_DEF
    TEXT(""), // BUILDING_BONUS_MONSTERS_SPD
    TEXT(""), // BUILDING_BONUS_MONSTERS_CRITDMG

    TEXT(""), // BUILDING_BONUS_ARCANETOWERS_ATT
    TEXT("")  // BUILDING_BONUS_ARCANETOWERS_SPD
};
const GChar * Building::sm_arrInfoTexts[BUILDING_COUNT] = {
    TEXT(""), // BUILDING_DUNGEON
    TEXT(""), // BUILDING_ARCANE_TOWER

    TEXT(""), // BUILDING_ESSENCE_STORAGE

    TEXT(""), // BUILDING_MONSTER_STORAGE
    TEXT(""), // BUILDING_MONSTER_SUMMONING
    TEXT(""), // BUILDING_MONSTER_EVOLUTION
    TEXT(""), // BUILDING_MONSTER_FUSION
//  TEXT(""), // BUILDING_MONSTER_SKINS

    TEXT(""), // BUILDING_RUNE_STORAGE
    TEXT(""), // BUILDING_RUNE_EVOLUTION

    TEXT(""), // BUILDING_MANA
    TEXT(""), // BUILDING_CRYSTAL

    TEXT(""), // BUILDING_SHOP
    TEXT(""), // BUILDING_WISHES

    TEXT(""), // BUILDING_BONUS_MONSTERS_HP
    TEXT(""), // BUILDING_BONUS_MONSTERS_ATT
    TEXT(""), // BUILDING_BONUS_MONSTERS_ATT_ELEMENT
    TEXT(""), // BUILDING_BONUS_MONSTERS_DEF
    TEXT(""), // BUILDING_BONUS_MONSTERS_SPD
    TEXT(""), // BUILDING_BONUS_MONSTERS_CRITDMG

    TEXT(""), // BUILDING_BONUS_ARCANETOWERS_ATT
    TEXT("")  // BUILDING_BONUS_ARCANETOWERS_SPD
};

