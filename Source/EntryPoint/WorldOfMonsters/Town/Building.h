/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Town/Building.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDING_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDING_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Monsters/Monster.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Building definitions
#define BUILDING_MAX_LEVEL 10

#define BUILDING_INFO_TEXT_LENGTH 1024

enum BuildingType {
    BUILDING_DUNGEON = 0,
    BUILDING_ARCANE_TOWER,

    BUILDING_ESSENCE_STORAGE,

    BUILDING_MONSTER_STORAGE,
    BUILDING_MONSTER_SUMMONING,
    BUILDING_MONSTER_EVOLUTION,
    BUILDING_MONSTER_FUSION,
    //BUILDING_MONSTER_SKINS,

    BUILDING_RUNE_STORAGE,
    BUILDING_RUNE_EVOLUTION,

    BUILDING_MANA,
    BUILDING_CRYSTAL,

    BUILDING_SHOP,   // The only tiny randomized
    BUILDING_WISHES, // part remaining !!!

    BUILDING_BONUS_MONSTERS_HP,
    BUILDING_BONUS_MONSTERS_ATT,
    BUILDING_BONUS_MONSTERS_ATT_ELEMENT,
    BUILDING_BONUS_MONSTERS_DEF,
    BUILDING_BONUS_MONSTERS_SPD,
    BUILDING_BONUS_MONSTERS_CRITDMG,

    BUILDING_BONUS_ARCANETOWERS_ATT,
    BUILDING_BONUS_ARCANETOWERS_SPD,

    BUILDING_COUNT
};

    // Currencies : resources
enum CurrencyType {
    CURRENCY_MANA = 0,
    CURRENCY_CRYSTAL,
    CURRENCY_GLORYPOINTS,
    CURRENCY_GUILDPOINTS,

    CURRENCY_COUNT
};

typedef struct _currency_cost {
    UInt arrCost[CURRENCY_COUNT];
} CurrencyCost;

/////////////////////////////////////////////////////////////////////////////////
// The Building class
class Building
{
public:
    Building();
    virtual ~Building();

    // Type
    virtual BuildingType GetType() const = 0;

    // Name & Info text
    inline const GChar * GetName() const;
    inline const GChar * GetInfoText() const;

    // Cost
    inline const CurrencyCost * GetCost() const;

protected:
    // Helpers
    static const GChar * sm_arrNames[BUILDING_COUNT];
    static const GChar * sm_arrInfoTexts[BUILDING_COUNT];

    // Cost
    CurrencyCost m_hCost;
};

//
///////////////////////////////////////////////////////////////////////////////////
//// The Building class
//class Building : public Building
//{
//public:
//    Building();
//    virtual ~Building();
//
//    // ....
//
//private:
//    // Dungeon access
//    BuildingDungeon * m_pDungeon;
//
//    // ....
//};
//
///////////////////////////////////////////////////////////////////////////////////
//// The Building class
//class Building : public Building
//{
//public:
//    Building();
//    virtual ~Building();
//
//    // ....
//
//private:
//    // Dungeon access
//    BuildingDungeon * m_pDungeon;
//
//    // ....
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Building.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_TOWN_BUILDING_H

