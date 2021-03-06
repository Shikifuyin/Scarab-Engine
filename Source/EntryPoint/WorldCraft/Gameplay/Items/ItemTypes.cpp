/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Items/ItemTypes.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay, Items : ItemTypes
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
#include "ItemTypes.h"

/////////////////////////////////////////////////////////////////////////////////
// BlockItem implementation
BlockItem::BlockItem():
    Item()
{
    m_iType = ITEMTYPE_BLOCK;
}
BlockItem::~BlockItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ConsumableItem implementation
ConsumableItem::ConsumableItem():
    Item()
{
    m_iType = ITEMTYPE_CONSUMABLE;
}
ConsumableItem::~ConsumableItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// EquipmentItem implementation
EquipmentItem::EquipmentItem():
    Item()
{
    m_iType = ITEMTYPE_EQUIPMENT;

    for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
        m_arrStatBonus[i] = 0.0f;
}
EquipmentItem::~EquipmentItem()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// QuestItem implementation
QuestItem::QuestItem():
    Item()
{
    m_iType = ITEMTYPE_QUEST;
    //m_iClass = ITEMCLASS_QUEST;
    //m_iBindType = ITEMBIND_ON_PICK;

    //m_idParentQuest = QUESTID_UNDEFINED;
}
QuestItem::~QuestItem()
{
    // nothing to do
}

