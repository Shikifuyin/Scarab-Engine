/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Interface/HUDBackboard.cpp
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
// Includes
#include "HUDBackboard.h"

/////////////////////////////////////////////////////////////////////////////////
// HUDBackboardModel implementation
HUDBackboardModel::HUDBackboardModel():
    GUIBackboardModel(),
    m_vMoveX(), m_vMoveY(), m_vMoveZ(), m_ptLastMouseDragPos()
{
    // Action map
    m_iActionMap = InputFn->CreateMap();
    InputFn->SelectMap( m_iActionMap );

    InputFn->BindAction( WOM_ACTION_EXIT,                KEYCODE_ESCAPE );
    InputFn->BindAction( WOM_ACTION_DEBUG_CONSOLE,       KEYCODE_OEM_7 ); // '²' for azerty
    InputFn->BindAction( WOM_ACTION_DEBUG_RESOURCES,     KEYCODE_R, ACTIONFLAG_MOD_CTRL );
    InputFn->BindAction( WOM_ACTION_DEBUG_VIDEOSETTINGS, KEYCODE_V, ACTIONFLAG_MOD_CTRL );
    InputFn->BindAction( WOM_ACTION_DEBUG_WIREFRAME,     KEYCODE_W, ACTIONFLAG_MOD_CTRL );

    ////////////////////////////////////////

    // Tracking
    m_pMenuModel = NULL;

    m_bMovingX = false;
    m_bMovingY = false;
    m_bMovingZ = false;
    m_vMoveX = Vector3::Null;
    m_vMoveY = Vector3::Null;
    m_vMoveZ = Vector3::Null;
}
HUDBackboardModel::~HUDBackboardModel()
{
    // Tracking
    if ( m_pMenuModel != NULL ) {
        SetContextMenu( NULL );
        Delete( m_pMenuModel );
    }

    // Action map
    InputFn->DestroyMap( m_iActionMap );
    m_iActionMap = INVALID_OFFSET;
}

Void HUDBackboardModel::OnAction( Action iAction )
{
    // Application data
    //const Camera * pRenderCamera = RenderingFn->GetCamera();

    // Compute camera frame
    //Vector3 vAim = pRenderCamera->GetViewDir();
    //vAim.Z = 0.0f;
    //vAim.Normalize();
    //Vector3 vSide = pRenderCamera->GetViewSide();
    //Vector3 vUp = (vAim ^ vSide);
    //vUp.Normalize();

    // Default behaviour
    switch( iAction ) {
        case WOM_ACTION_EXIT:
            GUIFn->MessageBox( GUIMSGBOX_TYPE_WARNING, TEXT("Are you sure you want to exit ?\nThat's what you want ?!\nDo as you wish ... TT"),
                               GUIMSGBOX_BUTTONS_YES_NO, _MsgBox_ConfirmExit, this );
            break;
        case WOM_ACTION_DEBUG_CONSOLE:
            ConsoleFn->Toggle();
            break;
        case WOM_ACTION_DEBUG_RESOURCES:
            ConsoleFn->ToggleResourceView();
            break;
        case WOM_ACTION_DEBUG_VIDEOSETTINGS:
            ConsoleFn->ToggleVideoSettings();
            break;
        case WOM_ACTION_DEBUG_WIREFRAME:
            ///////////////////////////////////////////////////////
            break;
        default:
            // do nothing
            break;
    }
}
Void HUDBackboardModel::OnLeftClick( ActionFlag iActionFlags, WorldLeaf * pLeaf )
{
    ///////////////////////////////////////////////////////
}
Void HUDBackboardModel::OnRightClick( ActionFlag iActionFlags, WorldLeaf * pLeaf )
{
    ///////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////

Void HUDBackboardModel::OnKeyPress( KeyCode iKey, GUIEventFlag iFlags )
{
    // Action flags
    ActionFlag iActionFlags = ACTIONFLAG_NONE;
    if ( iFlags & GUIEVENT_FLAG_CTRL )
        iActionFlags |= ACTIONFLAG_MOD_CTRL;
    if ( iFlags & GUIEVENT_FLAG_ALT )
        iActionFlags |= ACTIONFLAG_MOD_ALT;
    if ( iFlags & GUIEVENT_FLAG_SHIFT )
        iActionFlags |= ACTIONFLAG_MOD_SHIFT;

    // Action mapping
    Action iAction = InputFn->GetAction( iKey, iActionFlags );

    // Raise user callback
    OnAction( iAction );

}
Void HUDBackboardModel::OnKeyRelease( KeyCode iKey, GUIEventFlag iFlags )
{
    // Action flags
    ActionFlag iActionFlags = ACTIONFLAG_NONE;
    if ( iFlags & GUIEVENT_FLAG_CTRL )
        iActionFlags |= ACTIONFLAG_MOD_CTRL;
    if ( iFlags & GUIEVENT_FLAG_ALT )
        iActionFlags |= ACTIONFLAG_MOD_ALT;
    if ( iFlags & GUIEVENT_FLAG_SHIFT )
        iActionFlags |= ACTIONFLAG_MOD_SHIFT;

    // Action mapping
    Action iAction = InputFn->GetAction( iKey, iActionFlags );

    // Raise user callback
    OnAction( iAction );
}

Void HUDBackboardModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Action flags
    ActionFlag iActionFlags = ACTIONFLAG_NONE;
    if ( iFlags & GUIEVENT_FLAG_CTRL )
        iActionFlags |= ACTIONFLAG_MOD_CTRL;
    if ( iFlags & GUIEVENT_FLAG_ALT )
        iActionFlags |= ACTIONFLAG_MOD_ALT;
    if ( iFlags & GUIEVENT_FLAG_SHIFT )
        iActionFlags |= ACTIONFLAG_MOD_SHIFT;

    // Ray picking
    Line3 vPickRay;
    Bool bPicked = RenderingFn->GetPickRay( &(vPickRay.Position), &(vPickRay.Direction), ptLocalPos.X, ptLocalPos.Y );
    Assert( bPicked );

    // Ray casting
    WorldFn->RayCast( vPickRay, 0.0f, SCALAR_INFINITE );
    WorldLeaf * pLeaf = WorldFn->GetClosestRayCastResult()->pHit;

    // Action mapping
    if ( iKey == KEYCODE_MOUSELEFT )
        OnLeftClick( iActionFlags, pLeaf );
    else if ( iKey == KEYCODE_MOUSERIGHT )
        OnRightClick( iActionFlags, pLeaf );
}

Void HUDBackboardModel::OnStartDrag( const Point2 & ptLocalPos, KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    m_ptLastMouseDragPos = ptLocalPos;
}
Void HUDBackboardModel::OnDragRight( const Point2 & ptLocalPos, GUIEventFlag /*iFlags*/ )
{
    // Application data
    WorldCamera3rdPerson * pWorldCamera = (WorldCamera3rdPerson*)( WorldFn->GetWorldCamera() );

    // Arcball rotation
    Int iDX = ( ptLocalPos.X - m_ptLastMouseDragPos.X );
    Int iDY = ( ptLocalPos.Y - m_ptLastMouseDragPos.Y );

    static const Scalar fArcballScale = 1.0e-3f;
    static const Scalar fArcballSpanH = SCALAR_PI;
    static const Scalar fArcballSpanV = SCALAR_PI;

    Scalar fAngleX = ( ((Scalar)iDX) * fArcballScale * fArcballSpanH );
    Scalar fAngleY = ( ((Scalar)iDY) * fArcballScale * fArcballSpanV );

    pWorldCamera->RotateH( -fAngleX );
    pWorldCamera->RotateV( fAngleY );

    m_ptLastMouseDragPos = ptLocalPos;
}
Void HUDBackboardModel::OnStopDrag( const Point2 & /*ptLocalPos*/, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey == KEYCODE_MOUSELEFT ) {
        // nothing to do

        GUIFn->SetDragData( GUIDRAG_LEFT, NULL );

        return;
    }
}

Void HUDBackboardModel::OnScroll( const Point2 & /*ptLocalPos*/, Int iWheelDelta, GUIEventFlag iFlags )
{
    // Action flags
    ActionFlag iActionFlags = ACTIONFLAG_NONE;
    if ( iFlags & GUIEVENT_FLAG_CTRL )
        iActionFlags |= ACTIONFLAG_MOD_CTRL;
    if ( iFlags & GUIEVENT_FLAG_ALT )
        iActionFlags |= ACTIONFLAG_MOD_ALT;
    if ( iFlags & GUIEVENT_FLAG_SHIFT )
        iActionFlags |= ACTIONFLAG_MOD_SHIFT;

    // Application data
    WorldCamera3rdPerson * pWorldCamera = (WorldCamera3rdPerson*)( WorldFn->GetWorldCamera() );

    // Camera Zoom
    pWorldCamera->Zoom( -(Scalar)iWheelDelta );
}

/////////////////////////////////////////////////////////////////////////////////

Void HUDBackboardModel::_MsgBox_ConfirmExit( GUIMessageBoxResult iResult, Void * /*pUserData*/ )
{
    if ( iResult == GUIMSGBOX_RESULT_TRUE )
        ( (WindowApplication*)(ApplicationFn->GetInstance()) )->Exit();
}

