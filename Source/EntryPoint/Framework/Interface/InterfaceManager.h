/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Interface/InterfaceManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Framework, Interface : InterfaceManager
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
#ifndef SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_INTERFACEMANAGER_H
#define SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_INTERFACEMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define InterfaceFn InterfaceManager::GetInstancePtr()

// Interface Metrics
#define INTERFACE_HUD_ICON_SIZE 32

// Interface Skin
typedef UInt GUIImageID;
enum HUDImage {
    HUD_IMAGE_BACKGROUND_ = 0,
    //HUD_IMAGE_FOREGROUND_,

    HUD_IMAGE_COUNT
};

enum HUDColor {
    HUD_COLOR_ = 0,
    //HUD_COLOR_,

    HUD_COLOR_COUNT
};

enum HUDFont {
    HUD_FONT_ = 0,
    //HUD_FONT_,

    HUD_FONT_COUNT
};

// HUD skin texture locations
#define HUD_TEXTURE_SIZE 128

#define HUD_LOCATION_BACKGROUND_ Rectangle2( Point2(  0, 0 ), 32, 32 )
#define HUD_LOCATION_FOREGROUND_ Rectangle2( Point2( 32, 0 ), 32, 32 )

// HUD status effects icons texture locations
#define HUDICON_LOCATION_STATUSEFFECT_CHILL Rectangle2( Point2( 0, 0 ), 32, 32 )

// HUD skill icons texture locations
#define HUDICON_LOCATION_SKILL_MAGE_FIREBOLT Rectangle2( Point2(  0, 0 ), 32, 32 )
#define HUDICON_LOCATION_SKILL_MAGE_ICENOVA  Rectangle2( Point2( 32, 0 ), 32, 32 )

// HUD item icons texture locations
#define HUDICON_LOCATION_ITEM_WEAPON_FISTS Rectangle2( Point2( 0, 0 ), 32, 32 )

// HUD Drag & Drop
enum HUDDragType{
    HUD_DRAGTYPE_UNDEFINED = 0,

    HUD_DRAGTYPE_SKILL,
    HUD_DRAGTYPE_ITEM,

    HUD_DRAGTYPE_COUNT
};

typedef struct _hud_dragndrop_data {
    HUDDragType iType;
    UInt iIdentifier;
} HUDDragNDropData;

// User default action list
#define ACTION_CONTROL_EXIT          0
#define ACTION_CONTROL_3D_WIREFRAME  1
#define ACTION_CONTROL_ANIM_PAUSE    2
#define ACTION_CONTROL_ANIM_STEP     3

#define ACTION_CONSOLE_CMDLINE       10
#define ACTION_CONSOLE_VIDEOSETTINGS 11
#define ACTION_CONSOLE_RESOURCES     12

#define ACTION_MOVE_FORWARD  20
#define ACTION_MOVE_BACKWARD 21
#define ACTION_MOVE_LEFT     22
#define ACTION_MOVE_RIGHT    23
#define ACTION_MOVE_UP       24
#define ACTION_MOVE_DOWN     25

/////////////////////////////////////////////////////////////////////////////////
// The HUDBackboardModel class
class HUDBackboardModel : public GUIBackboardModel
{
public:
    HUDBackboardModel();
    virtual ~HUDBackboardModel();

    virtual Void OnAction( Action iAction );
	virtual Void OnStartAction( Action iAction );
    virtual Void OnStopAction( Action iAction );

protected:
    virtual Void OnKeyPress( KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnKeyRelease( KeyCode iKey, GUIEventFlag iFlags );

	virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnDragRight( const Point2 & ptLocalPos, GUIEventFlag iFlags );
	virtual Void OnStopDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnScroll( const Point2 & ptLocalPos, Int iWheelDelta, GUIEventFlag iFlags );

private:
    static Void _MsgBox_ConfirmExit( GUIMessageBoxResult iResult, Void * pUserData );

    GUIMenuModel * m_pMenuModel;

    Bool m_bMovingX, m_bMovingY, m_bMovingZ;
    Vector3 m_vMoveX, m_vMoveY, m_vMoveZ;

    Point2 m_ptLastMouseDragPos;
};

/////////////////////////////////////////////////////////////////////////////////
// The InterfaceManager class
class InterfaceManager : public Manager<InterfaceManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<InterfaceManager>;
    InterfaceManager();
    virtual ~InterfaceManager();

public:
    Void Initialize();
    Void Cleanup();

    // HUD access
    inline GPU2DContext * GetHUDContext() const;

    inline GPU2DBitmap * GetHUDSkinBitmap() const;
    inline const Rectangle2 & GetHUDLocation( HUDImage iHUDImage ) const;

    inline GPU2DSolidColorBrush * GetHUDColorBrush() const;
    inline const Color4 & GetHUDColor( HUDColor iHUDColor ) const;

    inline GPU2DSolidColorBrush * GetHUDTextBrush() const;
    inline GPU2DTextFormat * GetHUDTextFormat( HUDFont iHUDFont ) const;

    // HUD views

    // HUD drag & drop data
    inline HUDDragNDropData * GetDragNDropData();

    // Main routines
    Void Update();

    Void Render();

private:
    // HUD Skin
    GPU2DBitmap * m_pSkinBitmap;
    //GPU2DBitmap * m_pSkillIconsBitmap;
    //GPU2DBitmap * m_pMonsterIconsBitmap;
    //GPU2DBitmap * m_pRuneIconsBitmap;
    //GPU2DBitmap * m_pCurrencyIconsBitmap;

    Rectangle2 m_arrHUDLocations[HUD_IMAGE_COUNT];

    mutable GPU2DSolidColorBrush m_hColorBrush;
    Color4 m_arrHUDColors[HUD_COLOR_COUNT];

    mutable GPU2DSolidColorBrush m_hTextBrush;
    GPU2DTextFormat m_hTextFormat;
    GPU2DTextFormat * m_arrTextFormats[HUD_FONT_COUNT];

    // HUD Interface
    Void _CreateHeadUpDisplay();
    Void _DestroyHeadUpDisplay();

    HUDBackboardModel * m_pBackboardModel;
    GUIWindow * m_pWindow;

    /////////////////////////////////////

    HUDDragNDropData m_hDragNDrop;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "InterfaceManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_INTERFACEMANAGER_H

