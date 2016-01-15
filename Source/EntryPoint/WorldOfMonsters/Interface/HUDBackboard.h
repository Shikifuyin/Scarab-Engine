/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Interface/HUDBackboard.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Framework, Interface : HUDBackboard
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
#ifndef SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_HUDBACKBOARD_H
#define SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_HUDBACKBOARD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Action mapping
#define WOM_ACTION_EXIT                0
#define WOM_ACTION_DEBUG_CONSOLE       1
#define WOM_ACTION_DEBUG_RESOURCES     2
#define WOM_ACTION_DEBUG_VIDEOSETTINGS 3
#define WOM_ACTION_DEBUG_WIREFRAME     4


//#define ACTION_MOVE_FORWARD  20
//#define ACTION_MOVE_BACKWARD 21
//#define ACTION_MOVE_LEFT     22
//#define ACTION_MOVE_RIGHT    23
//#define ACTION_MOVE_UP       24
//#define ACTION_MOVE_DOWN     25

/////////////////////////////////////////////////////////////////////////////////
// The HUDBackboardModel class
class HUDBackboardModel : public GUIBackboardModel
{
public:
    HUDBackboardModel();
    virtual ~HUDBackboardModel();

    Void OnAction( Action iAction );
    Void OnLeftClick( ActionFlag iActionFlags, WorldLeaf * pLeaf );
    Void OnRightClick( ActionFlag iActionFlags, WorldLeaf * pLeaf );

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

    UInt m_iActionMap;

    GUIMenuModel * m_pMenuModel;

    Bool m_bMovingX, m_bMovingY, m_bMovingZ;
    Vector3 m_vMoveX, m_vMoveY, m_vMoveZ;

    Point2 m_ptLastMouseDragPos;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "HUDBackboard.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_HUDBACKBOARD_H

