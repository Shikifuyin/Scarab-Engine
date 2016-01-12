


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

// Shop
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

    SHOP_ITEM_COUNT
};
