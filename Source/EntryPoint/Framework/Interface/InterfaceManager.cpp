/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Interface/InterfaceManager.cpp
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
// Includes
#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// HUDBackboardModel implementation
HUDBackboardModel::HUDBackboardModel():
    GUIBackboardModel(),
    m_vMoveX(), m_vMoveY(), m_vMoveZ(), m_ptLastMouseDragPos()
{
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
	if ( m_pMenuModel != NULL ) {
		SetContextMenu( NULL );
		Delete( m_pMenuModel );
	}
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
	switch (iAction) {
		case ACTION_CONTROL_EXIT:
			GUIFn->MessageBox( GUIMSGBOX_TYPE_WARNING, TEXT("Are you sure you want to exit ?\nThat's what you want ?!\nDo as you wish ... TT"),
							   GUIMSGBOX_BUTTONS_YES_NO, _MsgBox_ConfirmExit, this );
			break;
        case ACTION_CONTROL_3D_WIREFRAME:
            ///////////////////////////////////////////////////////
            break;
		case ACTION_CONTROL_ANIM_PAUSE:
		    //WorldFn->ToggleAnimation();
		    break;
		case ACTION_CONTROL_ANIM_STEP:
		    //WorldFn->StepAnimation();
		    break;

		case ACTION_CONSOLE_CMDLINE:
			ConsoleFn->Toggle();
			break;
        case ACTION_CONSOLE_VIDEOSETTINGS:
            ConsoleFn->ToggleVideoSettings();
            break;
        case ACTION_CONSOLE_RESOURCES:
            ConsoleFn->ToggleResourceView();
            break;

        case ACTION_MOVE_FORWARD:
            ///////////////////////////////////////////////////////
            break;
        case ACTION_MOVE_BACKWARD:
            ///////////////////////////////////////////////////////
            break;
        case ACTION_MOVE_LEFT:
            ///////////////////////////////////////////////////////
            break;
        case ACTION_MOVE_RIGHT:
            ///////////////////////////////////////////////////////
            break;
        case ACTION_MOVE_UP:
            ///////////////////////////////////////////////////////
            break;
        case ACTION_MOVE_DOWN:
            ///////////////////////////////////////////////////////
            break;

		default:
			// do nothing
			break;
	}
}

Void HUDBackboardModel::OnStartAction( Action iAction )
{
    ///////////////////////////////////////////////////////
}
Void HUDBackboardModel::OnStopAction( Action iAction )
{
    ///////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////

Void HUDBackboardModel::OnKeyPress( KeyCode iKey, GUIEventFlag iFlags )
{
	// Action flags
	ActionFlag iActionFlags = ACTIONFLAG_NONE;
	if (iFlags & GUIEVENT_FLAG_CTRL)
		iActionFlags |= ACTIONFLAG_MOD_CTRL;
	if (iFlags & GUIEVENT_FLAG_ALT)
		iActionFlags |= ACTIONFLAG_MOD_ALT;
	if (iFlags & GUIEVENT_FLAG_SHIFT)
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

    // Application data
    //PCharacter * pPlayer = (PCharacter*)( GameplayFn->GetCharacter(TEXT("Shiki")) );

	// Ray picking
    Line3 vPickRay;
    Bool bPicked = RenderingFn->GetPickRay( &(vPickRay.Position), &(vPickRay.Direction), ptLocalPos.X, ptLocalPos.Y );
    Assert( bPicked );

    // Ray casting
    WorldFn->RayCast( vPickRay, 0.0f, SCALAR_INFINITE );
    WorldLeaf * pLeaf = WorldFn->GetClosestRayCastResult()->pHit;

	// Action mapping
    if ( iKey == KEYCODE_MOUSELEFT ) {
        if ( iActionFlags == ACTIONFLAG_NONE ) {
            // Left click
            ////////////////////////////////////////
        }
    } else if ( iKey == KEYCODE_MOUSERIGHT ) {
        if ( iActionFlags == ACTIONFLAG_NONE ) {
            // Right click
            ////////////////////////////////////////
        }
    }
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
        ((WindowApplication*)ApplicationFn->GetInstance())->Exit();
}

/////////////////////////////////////////////////////////////////////////////////
// InterfaceManager implementation
InterfaceManager::InterfaceManager():
    Manager<InterfaceManager>(),
    m_hColorBrush( GUIFn->GetScreenContext() ), m_hTextBrush( GUIFn->GetScreenContext() ), m_hTextFormat(), m_hDragNDrop()
{
    // nothing to do
}
InterfaceManager::~InterfaceManager()
{
    // nothing to do
}

Void InterfaceManager::Initialize()
{
    // HUD bitmaps
    //m_pSkinBitmap = GraphicsFn->GetBitmap( TEXT("Bitmap_HUD_Skin") );

    // Initialize HUD locations
    //m_arrHUDLocations[HUD_IMAGE_BACKGROUND_] = HUD_LOCATION_BACKGROUND_;

    // Initialize status effect icons locations

    // Initialize skill icons locations

    // Initialize item icons locations

    // Initialize skin colors
    m_hColorBrush.Bind();
    //m_arrHUDColors[HUD_COLOR_] = Color4::White;
    
    // Initialize skin text formats
    m_hTextBrush.Bind();
    m_hTextFormat.Bind( LOCALENAME_EN_US, GPU2DFONTFAMILY_ARIAL, GPU2DFONT_WEIGHT_NORMAL, GPU2DFONT_STYLE_NORMAL, GPU2DFONT_STRETCH_NORMAL, 10.0f, NULL );
    //m_arrTextFormats[HUD_FONT_] = &m_hTextFormat;

    // Create HUD
    m_pBackboardModel = NULL;

    /////////////////////////////////

    m_pWindow = NULL;

    _CreateHeadUpDisplay();

    // Initialize DragNDrop
    m_hDragNDrop.iType = HUD_DRAGTYPE_UNDEFINED;
    m_hDragNDrop.iIdentifier = 0;
}
Void InterfaceManager::Cleanup()
{
    // Destroy HUD
    _DestroyHeadUpDisplay();

    // Destroy HUD skin
    m_hTextFormat.UnBind();
    m_hTextBrush.UnBind();

    m_hColorBrush.UnBind();
}

Void InterfaceManager::Update()
{
    GUIFn->Update();
}

Void InterfaceManager::Render()
{
    GUIFn->Render();
}

Void InterfaceManager::_CreateHeadUpDisplay()
{
    //PCharacter * pPlayer = (PCharacter*)( GameplayFn->GetCharacter(TEXT("Shiki")) );

    // HUD models
    SelectMemory( TEXT("Scratch") );

    m_pBackboardModel = New HUDBackboardModel();

    ////////////////////////////////////////////////

    UnSelectMemory();

    // HUD window
    UInt iWidth, iHeight;
    GUIFn->GetScreenSize( &iWidth, &iHeight );
    m_pWindow = GUIFn->CreateWindow( TEXT("HUD"), Rectangle2( Point2(4,4), iWidth - 4, iHeight - 4 ), GUIWINDOW_STYLE_NAKED );
    GUIWidget * pHUDRoot = m_pWindow->GetClientRoot();

    // HUD layout
    GUIFn->SelectMemory( TEXT("LayoutData") );

    GUIGridLayout hudLayout;
    hudLayout.FillX = GUILAYOUT_FILL_FIXED;
    hudLayout.FillY = GUILAYOUT_FILL_FIXED;
    hudLayout.Anchor = GUILAYOUT_ANCHOR_FIXED;

    ////////////////////////////////////////////////

    GUIFn->UnSelectMemory();

    // HUD setup
    GUIFn->SetBackboardModel( m_pBackboardModel );

    GUIFn->RegisterWindow( m_pWindow );
    m_pWindow->SetEventBlocking( false );
    m_pWindow->SetVisible( true );
}
Void InterfaceManager::_DestroyHeadUpDisplay()
{
    // HUD setup
    GUIFn->UnregisterWindow( m_pWindow );
   
    GUIFn->SetBackboardModel( NULL );

    // HUD window
    GUIFn->DestroyWindow( m_pWindow );
    m_pWindow = NULL;

    // HUD models
    SelectMemory( TEXT("Scratch") );

    /////////////////////////////////////////////////

    Delete( m_pBackboardModel );
    m_pBackboardModel = NULL;

    UnSelectMemory();
}

