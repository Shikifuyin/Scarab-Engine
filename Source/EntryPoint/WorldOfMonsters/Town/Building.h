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

    // Types
enum BuildingType {
    BUILDING_DUNGEON = 0, // World & Arena access
    BUILDING_ARCANE_TOWER,

    BUILDING_ESSENCES,

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

    // Costs
enum BuildingCostType {
    BUILDING_COST_MANA = 0,
    BUILDING_COST_CRYSTAL,
    BUILDING_COST_GLORY
};

    // Info texts
#define BUILDING_INFO_TEXT_LENGTH 1024


    // Shop
#define BUILDING_SHOP_MAX_LEVEL 10
#define BUILDING_SHOP_MAX_ITEMS 20 // 2 * 10

enum ShopItemType {
    SHOP_ITEM_SCROLL_COMMON = 0,
    SHOP_ITEM_SCROLL_MYSTICAL,
    SHOP_ITEM_SCROLL_LEGENDARY,
    SHOP_ITEM_SCROLL_FIRE,
    SHOP_ITEM_SCROLL_WATER,
    SHOP_ITEM_SCROLL_WIND,
    SHOP_ITEM_SCROLL_LIGHT,
    SHOP_ITEM_SCROLL_DARK,
};

/////////////////////////////////////////////////////////////////////////////////
// The Building class
class Building
{
public:
    Building( BuildingType iType, BuildingCostType iCostType, UInt iCost );
    virtual ~Building();

    // Type access
    inline BuildingType GetType() const;

    inline const GChar * GetName() const;
    inline const GChar * GetInfoText() const;

    // Cost access
    inline Bool IsManaCost() const;
    inline Bool IsCrystalCost() const;
    inline Bool IsGloryCost() const;

    inline BuildingCostType GetCostType() const;
    inline UInt GetCost() const;

protected:
    // Helpers
    static const GChar * sm_arrNames[BUILDING_COUNT];
    static const GChar * sm_arrInfoTexts[BUILDING_COUNT];

    // Type
    BuildingType m_iType;

    // Cost
    BuildingCostType m_iCostType;
    UInt m_iCost;
};

/////////////////////////////////////////////////////////////////////////////////
// The BuildingEssences class
class BuildingEssences : public Building
{
public:
    BuildingEssences();
    virtual ~BuildingEssences();

    // Essence storage
    inline UInt GetEssenceCount( MonsterElement iElement, EssenceType iType ) const;

    inline Void AddEssences( MonsterElement iElement, EssenceType iType, UInt iCount );
    inline Void RemoveEssences( MonsterElement iElement, EssenceType iType, UInt iCount );

    // Fusion & Awakening
    inline Bool FusionUnlocked() const;
    inline Bool AwakeningUnlocked() const;

    Bool CanFuse( MonsterElement iElement, EssenceType iType ) const;
    Void Fuse( MonsterElement iElement, EssenceType iType );

    Bool CanAwake( UInt iTargetMonster ) const;
    Void Awake( UInt iTargetMonster );

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Essence storage
    UInt m_arrEssences[MONSTER_ELEMENT_COUNT][ESSENCE_TYPE_COUNT];

    // Fusion & Awakening
    Bool m_bFusionUnlocked;
    Bool m_bAwakeningUnlocked;
};


/////////////////////////////////////////////////////////////////////////////////
// The BuildingMonsterStorage class
class BuildingMonsterStorage : public Building
{
public:
    BuildingMonsterStorage();
    virtual ~BuildingMonsterStorage();

    // Monster storage
    inline UInt GetStorageLevel() const;
    inline UInt GetStorageRoom() const;

    Bool UpgradeStorageRoom();

    inline UInt GetMonsterCount() const;
    inline MonsterInstance * GetMonster( UInt iIndex ) const;

    Bool StoreMonster( UInt iIndex );
    Bool RetrieveMonster( UInt iStorageIndex );

private:
    // Helpers
    inline static Int _Compare_MonsterInstance( const MonsterInstance * const & pLeft, const MonsterInstance * const & pRight, Void * pUserData );

    static UInt sm_arrRoomByLevel[BUILDING_MONSTER_STORAGE_MAX_LEVEL];

    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Monster storage
    UInt m_iStorageLevel;
    UInt m_iStorageRoom;

    typedef RedBlackTree<MonsterInstance*> MonsterMap;
    MonsterMap m_mapMonsterStorage;
};

/////////////////////////////////////////////////////////////////////////////////
// The BuildingMonsterSummoning class
class BuildingMonsterSummoning : public Building
{
public:
    BuildingMonsterSummoning();
    virtual ~BuildingMonsterSummoning();

    // Monster summoning
    inline Bool SummoningUnlocked() const;

    Bool CanSummon( MonsterID iMonsterID ) const;
    Void Summon( MonsterID iMonsterID );

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Monster summoning
    Bool m_bSummoningUnlocked;
};

/////////////////////////////////////////////////////////////////////////////////
// The BuildingMonsterEvolution class
class BuildingMonsterEvolution : public Building
{
public:
    BuildingMonsterEvolution();
    virtual ~BuildingMonsterEvolution();

    // Monster evolution
    inline Bool EvolutionUnlocked() const;

    Bool CanPowerUp( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount ) const;
    Void PowerUp( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount );

    Bool CanEvolve( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount ) const;
    Void Evolve( UInt iTargetMonster, UInt arrFoodMonsters[5], UInt iFoodCount );

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Monster evolution
    Bool m_bEvolutionUnlocked;
};

/////////////////////////////////////////////////////////////////////////////////
// The BuildingMonsterFusion class
class BuildingMonsterFusion : public Building
{
public:
    BuildingMonsterFusion();
    virtual ~BuildingMonsterFusion();

    // Monster fusion
    inline Bool FusionUnlocked() const;

    inline Void GetRecipe( MonsterID iMonsterID, MonsterID outFoodMonsters[4], UInt * outRequiredRank ) const;

    Bool CanFuse( MonsterID iMonsterID ) const;
    Void Fuse( MonsterID iMonsterID );

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Monster fusion
    Bool m_bFusionUnlocked;
};


/////////////////////////////////////////////////////////////////////////////////
// The BuildingRuneStorage class
class BuildingRuneStorage : public Building
{
public:
    BuildingRuneStorage();
    virtual ~BuildingRuneStorage();

    // Rune storage
    inline UInt GetStorageLevel() const;
    inline UInt GetStorageRoom() const;

    Bool UpgradeStorageRoom();

    inline UInt GetRuneCount( RuneType iType ) const;
    inline Rune * GetRune( RuneType iType, UInt iStorageIndex ) const;

    Bool StoreRune( RuneType iType, UInt iIndex );
    Bool RetrieveRune( RuneType iType, UInt iStorageIndex );

private:
    // Helpers
    inline static Int _Compare_Rune( const Rune & rLeft, const Rune & rRight, Void * pUserData );

    static UInt sm_arrRoomByLevel[BUILDING_RUNE_STORAGE_MAX_LEVEL];

    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Rune storage
    UInt m_iStorageLevel;
    UInt m_iStorageRoom;

    typedef RedBlackTree<Rune> RuneMap;
    RuneMap m_mapRuneStorage;
};

/////////////////////////////////////////////////////////////////////////////////
// The BuildingRuneEvolution class
class BuildingRuneEvolution : public Building
{
public:
    BuildingRuneEvolution();
    virtual ~BuildingRuneEvolution();

    // Rune fusion
    inline Bool EvolutionUnlocked() const;

    Bool CanPowerUp( UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount ) const;
    Void PowerUp( UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount );

    Bool CanEvolve( UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount ) const;
    Void Evolve( UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount );

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Rune fusion
    Bool m_bEvolutionUnlocked;
};

/////////////////////////////////////////////////////////////////////////////////
// The BuildingMana class
class BuildingMana : public Building
{
public:
    BuildingMana();
    virtual ~BuildingMana();

    // Mana production
    inline UInt GetProductionRateLevel() const;
    inline Float GetProductionRate() const;

    Bool UpgradeProductionRate();

    inline UInt GetCapacityLevel() const;
    inline UInt GetCapacity() const;

    Bool UpgradeCapacity();

    inline Bool IsEmpty() const;
    inline UInt GetManaBuffer() const;

    Void RetrieveMana();
    Void UpdateMana();

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Mana production
    UInt m_iProductionRateLevel;
    Float m_fProductionRate;

    UInt m_iCapacityLevel;
    UInt m_iCapacity;

    UInt m_iManaBuffer;
    Float m_fLastUpdateTime;
};



/////////////////////////////////////////////////////////////////////////////////
// The BuildingShop class
class BuildingShop : public Building
{
public:
    BuildingShop();
    virtual ~BuildingShop();

    // Shop
    inline UInt GetLevel() const;
    inline UInt GetSlotCount() const;

    Void RefreshItemList();

private:
    // Helpers
    static UInt sm_arrSlotUnlockCosts[BUILDING_SHOP_MAX_LEVEL];

    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Shop
    UInt m_iLevel;
    UInt m_iSlotCount;

    typedef struct _shop_item {
        ShopItemType iType;

    } _ShopItem;

    _ShopItem m_arrItems[BUILDING_SHOP_MAX_ITEMS];
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

