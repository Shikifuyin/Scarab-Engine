/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/InventoryView.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : InventoryView
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_INVENTORYVIEW_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_INVENTORYVIEW_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Gameplay/Character.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The InventoryViewModel class
class InventoryViewModel : public GUIWidgetModel
{
public:
    InventoryViewModel( BaseCharacter * pCharacter );
    virtual ~InventoryViewModel();

    // Model
    inline InventoryBagID GetCurrentBag() const;
    inline Void SetCurrentBag( InventoryBagID idBag );

    inline ItemInstance * GetItem( EquipmentSoltID idEquipSlot ) const;
    inline ItemInstance * GetItem( UInt iSlot ) const;

    // Controller
    Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    BaseCharacter * m_pCharacter;

    InventoryBagID m_idCurrentBag;
};

/////////////////////////////////////////////////////////////////////////////////
// The InventoryView class
class InventoryView : public GUIWidget
{
public:
    InventoryView( InventoryViewModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~InventoryView();

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "InventoryView.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_INVENTORYVIEW_H

