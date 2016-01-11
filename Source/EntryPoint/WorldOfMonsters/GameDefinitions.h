/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/GameDefinitions.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters : Game definitions interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEDEFINITIONS_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEDEFINITIONS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Datastruct/Array/Array.h"
#include "../../Lib/Datastruct/Table/TreeMap.h"

#include "../../Engine/Resource/XML/XML.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Monster definitions
#define MONSTER_MAX_RANK               6
#define MONSTER_MAX_LEVEL              40
#define MONSTER_MAX_LEVELBYRANK(_rank) ( 15 + (5*(_rank)) ) // 15, 20, 25, 30, 35, 40

enum MonsterElement {
    MONSTER_ELEMENT_MAGIC = 0,
    MONSTER_ELEMENT_FIRE,
    MONSTER_ELEMENT_WATER,
    MONSTER_ELEMENT_WIND,
    MONSTER_ELEMENT_LIGHT,
    MONSTER_ELEMENT_DARK,

    MONSTER_ELEMENT_COUNT
};

enum MonsterType {
    MONSTER_ATTACK = 0,
    MONSTER_SUPPORT,
    MONSTER_TANK_HP,
    MONSTER_TANK_DEF,

    MONSTER_TYPE_COUNT
};

enum MonsterStatistic {
    MONSTER_STAT_HEALTH = 0,
    MONSTER_STAT_ATTACK,
    MONSTER_STAT_DEFENSE,
    MONSTER_STAT_SPEED,

    MONSTER_STAT_CRIT_RATE,
    MONSTER_STAT_CRIT_DMG,
    MONSTER_STAT_ACCURACY,
    MONSTER_STAT_RESISTANCE,

    MONSTER_STAT_COUNT
};

enum MonsterAwakeningBonus {
    MONSTER_AWAKENING_SPEED = 0,
    MONSTER_AWAKENING_CRITICALRATE,
    MONSTER_AWAKENING_ACCURACY,
    MONSTER_AWAKENING_RESISTANCE,
    MONSTER_AWAKENING_SKILLNEW,
    MONSTER_AWAKENING_SKILLUPGRADE,

    MONSTER_AWAKENING_BONUS_COUNT
};

    // Skill definitions
#define SKILL_MAX_LEVEL 16

enum SkillStat {
    SKILL_STAT_DAMAGE = 0,
    SKILL_STAT_RECOVERY,
    SKILL_STAT_STATUSEFFECTRATE,
    SKILL_STAT_COOLDOWN,

    SKILL_STAT_SPECIFIC, // Shields, etc ...

    SKILL_STAT_COUNT
};

    // Rune definitions
#define RUNE_MAX_RANK  6
#define RUNE_MAX_LEVEL 16

#define RUNE_SLOT_COUNT 6

enum RuneType {
    RUNE_ENERGY = 0, // (2)HP +15%
    RUNE_FATAL,      // (4)ATT +35%
    RUNE_GUARD,      // (2)DEF +15%
    RUNE_SWIFT,      // (4)SPD +25%

    RUNE_BLADE,      // (2)CRate +12%
    RUNE_RAGE,       // (4)CDmg +40%
    RUNE_FOCUS,      // (2)ACC +20%
    RUNE_ENDURE,     // (2)RES +20%

    RUNE_SHIELD,     // (2)Shield 3 turns 15% HP
    RUNE_WILL,       // (2)Immunity 1 turn

    RUNE_NEMESIS,    // (2)ATB +4% / HP loss 7%
    RUNE_DESTROY,    // (2)HPMax -4% / Dmg dealt 30%

    RUNE_VAMPIRE,    // (4)LifeDrain +35%
    RUNE_DESPAIR,    // (4)Stun +25%
    RUNE_VIOLENT,    // (4)ExtraTurn +22%
    RUNE_REVENGE,    // (2)Counterattack +15%

    RUNE_TYPE_COUNT
};

enum RuneStatistic {
    RUNE_STAT_PRIMARY = 0,
    RUNE_STAT_SECONDARY,
    RUNE_STAT_SUB_1,
    RUNE_STAT_SUB_2,
    RUNE_STAT_SUB_3,
    RUNE_STAT_SUB_4,

    RUNE_STAT_COUNT
};

enum RuneQuality {
    RUNE_QUALITY_COMMON = 0, // White  (nothing)
    RUNE_QUALITY_UNCOMMON,   // White  (secondary only)
    RUNE_QUALITY_MAGIC,      // Green  (secondary + 1 sub)
    RUNE_QUALITY_RARE,       // Blue   (secondary + 2 subs)
    RUNE_QUALITY_EPIC,       // Purple (secondary + 3 subs)
    RUNE_QUALITY_LEGEND,     // Orange (secondary + 4 subs)

    RUNE_QUALITY_COUNT
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

    // Currencies : scrolls
enum ScrollType {
    SCROLL_COMMON = 0,
    SCROLL_MYSTICAL,
    SCROLL_LEGENDARY,
    SCROLL_FIRE,
    SCROLL_WATER,
    SCROLL_WIND,
    SCROLL_LIGHT,
    SCROLL_DARK,

    SCROLL_TYPE_COUNT
};

typedef struct _scroll_cost {
    UInt arrCost[SCROLL_TYPE_COUNT];
} ScrollCost;

    // Currencies : Essences
enum EssenceType {
    ESSENCE_LOW = 0,
    ESSENCE_MID,
    ESSENCE_HIGH,

    ESSENCE_TYPE_COUNT
};

typedef struct _essence_cost {
    MonsterElement iElement;
    UInt arrMagicCost[ESSENCE_TYPE_COUNT];
    UInt arrElementalCost[ESSENCE_TYPE_COUNT];
} EssenceCost;

    // Building definitions
#define BUILDING_MAX_LEVEL 10

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

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GameDefinitions.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEDEFINITIONS_H


