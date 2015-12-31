/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Main.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Application Entry Point
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
#include "Main.h"

/////////////////////////////////////////////////////////////////////////////////
// Entry point
//int main( int argc, char ** argv )
//{
//    return ApplicationManager::Main( argc, argv );
//}

/////////////////////////////////////////////////////////////////////////////////
#ifdef MAIN_DEBUG

/////////////////////////////////////////////////////////////////////////////////
// ScratchTests implementation
//DefineConsoleApplication( ScratchTests );
//
//ScratchTests::ScratchTests():
//    ConsoleApplication( TEXT("Scratch Tests") )
//{
//    // nothing to do
//}
//ScratchTests::~ScratchTests()
//{
//    // nothing to do
//}
//
//Int ScratchTests::Main( const CommandLine & /*cmdLine*/ )
//{
//
//    //////////////////////////////////////
//
//
//    return 0;
//}

/////////////////////////////////////////////////////////////////////////////////
#else // !MAIN_DEBUG

/////////////////////////////////////////////////////////////////////////////////
// MyBackboardModel implementation
MyBackboardModel::MyBackboardModel( MyGame * pAppInstance ):
    GUIBackboardModel()
{
    m_pAppInstance = pAppInstance;

    const GChar * arrSubTexts1[4] = {
        TEXT("SubItemB 0"), TEXT("SubItemB 1"), TEXT("SubItemB 2"), TEXT("SubItemB 3")
    };
    const GChar * arrSubTexts3[4] = {
        TEXT("SubItemD 0"), TEXT("SubItemD 1"), TEXT("SubItemD 2"), TEXT("SubItemD 3")
    };

    const GChar * arrTexts[4] = {
        TEXT("Item A"), TEXT("Item B"), TEXT("Item C"), TEXT("Item D")
    };

    GUISkinImage arrIcons[4] = {
        GUISKIN_IMAGE_ICON_UNDEFINED, GUISKIN_IMAGE_ICON_UNDEFINED,
        GUISKIN_IMAGE_ICON_UNDEFINED, GUISKIN_IMAGE_ICON_UNDEFINED
    };

    GUIMenuModel * arrSubMenus[4] = {
        NULL, New GUIMenuModel( 16, 4, arrSubTexts1, arrIcons ),
        NULL, New GUIMenuModel( 16, 4, arrSubTexts3, arrIcons )
    };

    m_pMenuModel = New GUIMenuModel( 16, 4, arrTexts, arrIcons, arrSubMenus );
    SetContextMenu( m_pMenuModel );
}
MyBackboardModel::~MyBackboardModel()
{
    SetContextMenu( NULL );
    Delete( m_pMenuModel );
}

Void MyBackboardModel::OnKeyPress( KeyCode iKey, GUIEventFlag iFlags )
{
    ActionFlag iActionFlags = ACTIONFLAG_NONE;
    if ( iFlags & GUIEVENT_FLAG_CTRL )
        iActionFlags |= ACTIONFLAG_MOD_CTRL;
    if ( iFlags & GUIEVENT_FLAG_ALT )
        iActionFlags |= ACTIONFLAG_MOD_ALT;
    if ( iFlags & GUIEVENT_FLAG_SHIFT )
        iActionFlags |= ACTIONFLAG_MOD_SHIFT;

    EffectLightPointPhong * pEffectSun = m_pAppInstance->m_pEffectSun;
    MyAvatar * pAvatar = m_pAppInstance->m_pAvatar;
    Camera * pRenderCamera = m_pAppInstance->m_pRenderCamera;
    WorldCamera3rdPerson * pWorldCamera = m_pAppInstance->m_pWorldCamera;

    Vector3 vAim = pRenderCamera->GetViewDir();
    vAim.Z = 0.0f;
    vAim.Normalize();
    Vector3 vSide = pRenderCamera->GetViewSide();
    Vector3 vUp = ( vAim ^ vSide );
    vUp.Normalize();

    Quaternion qStepHM( vUp, -SCALAR_PI_4 * 0.25f ); // Yaw
    Quaternion qStepHP( vUp, +SCALAR_PI_4 * 0.25f ); //
    //Quaternion qStepVM( vSide, -SCALAR_PI_4 * 0.25f ); // Pitch
    //Quaternion qStepVP( vSide, +SCALAR_PI_4 * 0.25f ); //
    //Quaternion qStepRM( vAim, -SCALAR_PI_4 * 0.25f ); // Roll
    //Quaternion qStepRP( vAim, +SCALAR_PI_4 * 0.25f ); //

    Action iAction = InputFn->GetAction( iKey, iActionFlags );
    switch( iAction ) {
        case ACTION_CONSOLE_INVOKE:
            ConsoleFn->Toggle();
            break;
        case ACTION_CONTROL_EXIT:
            GUIFn->MessageBox( GUIMSGBOX_TYPE_WARNING, TEXT("Are you sure you want to exit ?\nThat's what you want ?!\nDo as you wish ... TT"),
                               GUIMSGBOX_BUTTONS_YES_NO, _MsgBox_ConfirmExit, m_pAppInstance );
            break;
        case ACTION_CONTROL_ANIM_PAUSE:
            WorldFn->ToggleAnimation();
            break;
        case ACTION_CONTROL_ANIM_STEP:
            WorldFn->StepAnimation();
            break;
        case ACTION_CONTROL_3D_WIREFRAME:
            pEffectSun->GetPolygonState(0,0)->WireframeEnabled =
                !(pEffectSun->GetPolygonState(0,0)->WireframeEnabled);
            break;
        case ACTION_MOVE_RUN_FORWARD:
            pAvatar->GetController()->Translate( +vAim * 0.4f );
            break;
        case ACTION_MOVE_RUN_BACKWARD:
            pAvatar->GetController()->Translate( -vAim * 0.4f );
            break;
        case ACTION_MOVE_RUN_LEFT:
            pAvatar->GetController()->Translate( +vSide * 0.4f );
            break;
        case ACTION_MOVE_RUN_RIGHT:
            pAvatar->GetController()->Translate( -vSide * 0.4f );
            break;
        case ACTION_MOVE_RUN_UP:
            pAvatar->GetController()->Translate( +vUp * 0.4f );
            break;
        case ACTION_MOVE_RUN_DOWN:
            pAvatar->GetController()->Translate( -vUp * 0.4f );
            break;
        case ACTION_CAMERA_LOOK_LEFT:
            pWorldCamera->RotateH( +SCALAR_PI_4 * 0.25f );
            pAvatar->GetController()->Rotate( qStepHP );
            break;
        case ACTION_CAMERA_LOOK_RIGHT:
            pWorldCamera->RotateH( -SCALAR_PI_4 * 0.25f );
            pAvatar->GetController()->Rotate( qStepHM );
            break;
        case ACTION_CAMERA_LOOK_UP:
            pWorldCamera->RotateV( -SCALAR_PI_4 * 0.25f );
            //pAvatar->GetController()->Rotate( qStepVM );
            break;
        case ACTION_CAMERA_LOOK_DOWN:
            pWorldCamera->RotateV( +SCALAR_PI_4 * 0.25f );
            //pAvatar->GetController()->Rotate( qStepVP );
            break;
        case ACTION_CAMERA_ROLL_LEFT:
            //pAvatar->GetController()->Rotate( qStepRM );
            break;
        case ACTION_CAMERA_ROLL_RIGHT:
            //pAvatar->GetController()->Rotate( qStepRP );
            break;
        case ACTION_CAMERA_ZOOM_OUT:
            pWorldCamera->Zoom( +0.2f );
            break;
        case ACTION_CAMERA_ZOOM_IN:
            pWorldCamera->Zoom( -0.2f );
            break;
        default:
            // do nothing
            break;
    }
}

Void MyBackboardModel::OnMousePress( const Point2 & ptMousePos, KeyCode iKey, GUIEventFlag /*iFlags*/ )
{
    if ( iKey == KEYCODE_MOUSELEFT ) {
        // Ray picking
        Line3 vPickRay;
        Bool bPicked = RenderingFn->GetPickRay( vPickRay.Position, vPickRay.Direction, ptMousePos.X, ptMousePos.Y );
        Assert( bPicked );

        // Ray casting
        WorldFn->RayCast( vPickRay, 0.0f, SCALAR_INFINITE );
        WorldLeaf * pLeaf = WorldFn->GetClosestRayCastResult()->pHit;

        // Print result
        if ( pLeaf == NULL )
            DebugPrint( TEXT("RayCast result : MISS \n") );
        else
            DebugPrint( TEXT("RayCast result : HIT (%s) \n"), pLeaf->GetName() );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void MyBackboardModel::_MsgBox_ConfirmExit( GUIMessageBoxResult iResult, Void * pUserData )
{
    if ( iResult == GUIMSGBOX_RESULT_TRUE )
        ((MyGame*)pUserData)->Exit();
}

/////////////////////////////////////////////////////////////////////////////////
// MyGame implementation
DefineWindowApplication( MyGame );

MyGame::MyGame():
    WindowApplication( TEXT("Scarab Engine Application"), 1024, 768, TEXTURE_FMT_ARGB8I, 0x80808080 )
{
    // Input
    m_iActionMap = INVALID_OFFSET;

    // Effects
    m_pVFSun = NULL;
    m_pVFColor = NULL;

    m_lightSun.Diffuse = Color4::White;
    m_lightSun.Position = Vertex3( 30.0f, 5.0f, 9.0f );

    m_matAvatarColor.Diffuse = Color4( 0.45f, 0.1f, 0.6f, 1.0f );
    m_matCurveColor.Diffuse = Color4::Yellow;
    m_matSurfaceColor.Diffuse = Color4::Yellow;
    m_matFloorColor.Diffuse = Color4::Cyan;
    m_matBallColor.Diffuse = Color4::Magenta;
    m_matBoxColor.Diffuse = Color4::Magenta;
    m_matParticleColor.Diffuse = Color4::Yellow;

    m_pEffectColor = NULL;
    m_pEffectSun = NULL;    

    m_pCurveEffect = NULL;
    m_pSurfaceEffect = NULL;
    m_pFloorEffect = NULL;
    m_pBallEffect = NULL;
    m_pBoxEffect = NULL;
    m_pParticleEffect = NULL;

    //m_matShadowPlaneColor.Diffuse = Color4( 0.0f, 0.0f, 0.0f, 0.0f );
    //m_pShadowPlaneEffect = NULL;
    //m_pShadowPlane = NULL;
    //m_pShadowsEffect = NULL;

    // Instanced geometry
    m_pFloorMesh = NULL;
    m_pFloorShape = NULL;

    m_pBallMesh = NULL;
    m_pBallShape = NULL;

    m_pBoxMesh = NULL;
    m_pBoxShape = NULL;

    // World
    m_pAvatar = NULL;

    m_pCurve = NULL;
    m_pSurface = NULL;

    m_pFloor = NULL;
    //m_pLandscape = NULL;
    m_pDungeon = NULL;

    UInt i;
    for( i = 0; i < MAIN_BALL_COUNT; ++i )
        m_arrBalls[i] = NULL;
    for( i = 0; i < MAIN_BOX_COUNT; ++i )
        m_arrBoxes[i] = NULL;

    m_pKinematicBall = NULL;
    m_pSkeleton = NULL;
    //m_pParticles = NULL;

    for( i = 0; i < MAIN_JOINT_COUNT; ++i )
        m_arrJoints[i] = NULL;

    m_pRenderCamera = NULL;
    m_pWorldCamera = NULL;

    // GUI
    m_pBackboardModel = NULL;

    m_pSwitchTabsModel = NULL;
    m_arrLabelModels[0] = NULL;
    m_arrLabelModels[1] = NULL;
    m_arrLabelModels[2] = NULL;

    m_pTestWindow = NULL;
    m_strFPSText[0] = NULLBYTE;
}
MyGame::~MyGame()
{
    // nothing to do
}

Bool MyGame::OnInitialize()
{
    _CreateActions();
    _CreateEffects();
    _CreateInstancedGeometry();
    _CreateWorld();
    _CreateGUI();

    //m_pDungeon->UpdateSelfLight( m_lightSun.Position );

    WorldFn->ToggleDebugDraw( true );

    return true;
}
Void MyGame::OnCleanup()
{
    WorldFn->ToggleDebugDraw( false );

    _DestroyGUI();
    _DestroyWorld();
    _DestroyInstancedGeometry();
    _DestroyEffects();
    _DestroyActions();
}

Bool MyGame::OnFrameMove()
{
    // Timing
    static const Scalar fSimulationTimeStep = MathFn->Invert( 500.0f );
    //static Bool bFirstFrame = true;

    // Update
    //if ( fElapsedTime >= fSimulationTimeStep ) {
        // Update World
        WorldFn->UpdateWorld( fSimulationTimeStep );
        //m_pDungeon->UpdateSelfLight( Vertex3::Null + m_pAvatar->WorldTransform.GetTranslate() + Vector3::eK * 2.0f );

        //if ( bFirstFrame ) {
        //    WorldFn->PauseAnimation();
        //    bFirstFrame = false;
        //}

        // Update GUI
        GUIFn->Update();

        return true;
    //}

    //return false;
}
Void MyGame::OnFrameRender()
{
    // Begin Rendering
    RenderingFn->ClearBuffers();
    RenderingFn->DrawBegin();

    // Render World
    WorldFn->RenderWorld();

    // Render GUI
    GUIFn->Render();

    // Render FPS text
    StringFn->Format( m_strFPSText, TEXT("FPS = %3f"), GetAverageFPS() );
    RenderingFn->BatchTextStart( 0 );
    RenderingFn->BatchText( 0, m_strFPSText, 5, 5, 0.0f, Color4::White );
    RenderingFn->BatchTextFinish( 0 );

    // End Rendering
    RenderingFn->DrawEnd();
}

Void MyGame::OnExit()
{
    GUIFn->OnExit();
}

Bool MyGame::OnKeyPress( KeyCode iKey )
{
    GUIFn->OnKeyPress( iKey );

    return true;
}
Bool MyGame::OnKeyRelease( KeyCode iKey )
{
    GUIFn->OnKeyRelease( iKey );

    return true;
}

Void MyGame::OnMouseMove( UInt iX, UInt iY )
{
    GUIFn->OnMouseMove( Point2(iX, iY) );
}
Bool MyGame::OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta )
{
    GUIFn->OnMouseWheel( Point2(iX, iY), iWheelDelta );

    return true;
}
Bool MyGame::OnMousePress( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMousePress( Point2(iX, iY), iButton );

    return true;
}
Bool MyGame::OnMouseRelease( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseRelease( Point2(iX, iY), iButton );

    return true;
}
Bool MyGame::OnMouseDblClick( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseDblClick( Point2(iX, iY), iButton );

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Void MyGame::_CreateActions()
{
    // Action map
    m_iActionMap = InputFn->CreateMap();
    InputFn->SelectMap( m_iActionMap );
    InputFn->BindAction( ACTION_CONSOLE_INVOKE,       KEYCODE_OEM_7 );
    InputFn->BindAction( ACTION_CONTROL_EXIT,         KEYCODE_ESCAPE );
    InputFn->BindAction( ACTION_CONTROL_3D_WIREFRAME, KEYCODE_W );
    InputFn->BindAction( ACTION_CONTROL_ANIM_PAUSE,   KEYCODE_P );
    InputFn->BindAction( ACTION_CONTROL_ANIM_STEP,    KEYCODE_SPACE );
    InputFn->BindAction( ACTION_MOVE_RUN_FORWARD,     KEYCODE_Z );
    InputFn->BindAction( ACTION_MOVE_RUN_BACKWARD,    KEYCODE_S );
    InputFn->BindAction( ACTION_MOVE_RUN_LEFT,        KEYCODE_Q );
    InputFn->BindAction( ACTION_MOVE_RUN_RIGHT,       KEYCODE_D );
    InputFn->BindAction( ACTION_MOVE_RUN_UP,          KEYCODE_R, ACTIONFLAG_MOD_CTRL );
    InputFn->BindAction( ACTION_MOVE_RUN_DOWN,        KEYCODE_F, ACTIONFLAG_MOD_CTRL );
    InputFn->BindAction( ACTION_CAMERA_LOOK_LEFT,     KEYCODE_A );
    InputFn->BindAction( ACTION_CAMERA_LOOK_RIGHT,    KEYCODE_E );
    InputFn->BindAction( ACTION_CAMERA_LOOK_UP,       KEYCODE_R );
    InputFn->BindAction( ACTION_CAMERA_LOOK_DOWN,     KEYCODE_F );
    InputFn->BindAction( ACTION_CAMERA_ROLL_LEFT,     KEYCODE_X, ACTIONFLAG_MOD_CTRL );
    InputFn->BindAction( ACTION_CAMERA_ROLL_RIGHT,    KEYCODE_C, ACTIONFLAG_MOD_CTRL );
    InputFn->BindAction( ACTION_CAMERA_ZOOM_OUT,      KEYCODE_X );
    InputFn->BindAction( ACTION_CAMERA_ZOOM_IN,       KEYCODE_C );
}
Void MyGame::_DestroyActions()
{
    // Action map
    InputFn->DestroyMap( m_iActionMap );
    m_iActionMap = INVALID_OFFSET;
}

Void MyGame::_CreateEffects()
{
    // VFs
    m_pVFColor = RenderingFn->CreateVertexFormat();
    m_pVFColor->DeclareField( VFTYPE_FLOAT4, VFUSAGE_POSITION );
    m_pVFColor->Finalize();

    m_pVFSun = RenderingFn->CreateVertexFormat();
    m_pVFSun->DeclareField( VFTYPE_FLOAT4, VFUSAGE_POSITION );
    m_pVFSun->DeclareField( VFTYPE_FLOAT4, VFUSAGE_NORMAL );
    m_pVFSun->Finalize();

    // Effects
    m_pEffectColor = New EffectMaterial();
    m_pEffectSun = New EffectLightPointPhong();

    // Effect instances
    m_pAvatarEffect = m_pEffectSun->CreateInstance( &m_lightSun, &m_matAvatarColor );
    m_pCurveEffect = m_pEffectColor->CreateInstance( &m_matCurveColor );
    m_pSurfaceEffect = m_pEffectSun->CreateInstance( &m_lightSun, &m_matSurfaceColor );
    m_pFloorEffect = m_pEffectSun->CreateInstance( &m_lightSun, &m_matFloorColor );
    m_pBallEffect = m_pEffectSun->CreateInstance( &m_lightSun, &m_matBallColor );
    m_pBoxEffect = m_pEffectSun->CreateInstance( &m_lightSun, &m_matBoxColor );
    m_pParticleEffect = m_pEffectColor->CreateInstance( &m_matParticleColor );

    // Global effects
    //m_pShadowPlaneEffect = m_pEffectColor->CreateInstance( &m_matShadowPlaneColor );

    //Quad3 vQuad( Vertex3(25.0f,0.0f,0.1f), Vertex3(30.0f,0.0f,0.1f),
    //             Vertex3(30.0f,10.0f,0.1f), Vertex3(25.0f,10.0f,0.1f) );
    //MeshQuad * pPlaneMesh = New MeshQuad( vQuad, m_pVFColor, false );
    //m_pShadowPlane = New ShadowPlane( pPlaneMesh, m_pShadowPlaneEffect, TEXT("ShadowPlane_0") );
    //m_pShadowsEffect = New EffectPlanarShadows( 1, 1 );

    //m_pShadowsEffect->SetPlane( 0, m_pShadowPlane );
    //m_pShadowsEffect->SetProjector( 0, &m_lightSun );
    //WorldFn->SetGlobalEffect( m_pShadowsEffect );
}
Void MyGame::_DestroyEffects()
{
    // Global effects
    //WorldFn->SetGlobalEffect( NULL );
    //m_pShadowsEffect->SetProjector( 0, NULL );
    //m_pShadowsEffect->SetPlane( 0, NULL );

    //Delete( m_pShadowsEffect );
    //m_pShadowsEffect = NULL;
    //Delete( m_pShadowPlane->GetMesh() );
    //Delete( m_pShadowPlane );
    //m_pShadowPlane = NULL;

    //Delete( m_pShadowPlaneEffect );
    //m_pShadowPlaneEffect = NULL;

    // Effect instances
    Delete( m_pParticleEffect );
    m_pParticleEffect = NULL;
    Delete( m_pBoxEffect );
    m_pBoxEffect = NULL;
    Delete( m_pBallEffect );
    m_pBallEffect = NULL;
    Delete( m_pFloorEffect );
    m_pFloorEffect = NULL;
    Delete( m_pSurfaceEffect );
    m_pSurfaceEffect = NULL;
    Delete( m_pCurveEffect );
    m_pCurveEffect = NULL;
    Delete( m_pAvatarEffect );
    m_pAvatarEffect = NULL;

    // Effects
    Delete( m_pEffectSun );
    m_pEffectSun = NULL;
    Delete( m_pEffectColor );
    m_pEffectColor = NULL;

    // VFs
    RenderingFn->DestroyVertexFormat( m_pVFSun );
    m_pVFSun = NULL;
    RenderingFn->DestroyVertexFormat( m_pVFColor );
    m_pVFColor = NULL;
}

Void MyGame::_CreateInstancedGeometry()
{
    // Floor
    Box vFloor( Vertex3::Null, Vector3(10.0f, 10.0f, 0.1f) );
    m_pFloorMesh = New MeshBox( vFloor, m_pVFSun, false, false );
    m_pFloorShape = PhysicsFn->CreateShapeBox( m_pFloorMesh );

    // Balls
    Sphere vBall( Vertex3::Null, 1.0f );
    m_pBallMesh = New MeshSphere( vBall, 16, 16, m_pVFSun, false, false );
    m_pBallShape = PhysicsFn->CreateShapeSphere( m_pBallMesh );

    // Boxes
    Box vBox( Vertex3::Null, Vector3(1.0f, 1.0f, 1.0f) );
    m_pBoxMesh = New MeshBox( vBox, m_pVFSun, false, false );
    m_pBoxShape = PhysicsFn->CreateShapeBox( m_pBoxMesh );

    // Particles
    Box vParticle( Vertex3::Null, Vector3(0.1f, 0.1f, 0.1f) );
    m_pParticleMesh = New MeshBox( vParticle, m_pVFColor, false, false );
}
Void MyGame::_DestroyInstancedGeometry()
{
    // Particles
    Delete( m_pParticleMesh );
    m_pParticleMesh = NULL;

    // Boxes
    PhysicsFn->DestroyShape( m_pBoxShape );
    m_pBoxShape = NULL;
    Delete( m_pBoxMesh );
    m_pBoxMesh = NULL;

    // Balls
    PhysicsFn->DestroyShape( m_pBallShape );
    m_pBallShape = NULL;
    Delete( m_pBallMesh );
    m_pBallMesh = NULL;

    // Floor
    PhysicsFn->DestroyShape( m_pFloorShape );
    m_pFloorShape = NULL;
    Delete( m_pFloorMesh );
    m_pFloorMesh = NULL;
}

Void MyGame::_CreateWorld()
{
    UInt i;
    GChar strName[WORLDGRAPH_NAME_LENGTH];

    // World Objects
    m_pAvatar = New MyAvatar( m_pBallShape, m_pAvatarEffect, Vertex3(10.0f, 10.0f, 0.0f) );

    //m_pCurve = New MyCurve( m_pVFColor, m_pCurveEffect );
    //m_pSurface = New MySurface( m_pVFSun, m_pSurfaceEffect );

    m_pFloor = New MyFloor( m_pFloorShape, m_pFloorEffect, Vertex3(0.0f, 0.0f, -5.0f) );
    //m_pLandscape = New MyLandscape( m_pVFSun, m_pFloorEffect );
    //m_pDungeon = New MyDungeon( m_pVFSun, m_pEffectSun );

    //m_arrBalls[0] = New MyDynamicBall( TEXT("MyDynamicBall_0"), m_pBallShape, m_pBallEffect, Vertex3(65.0f, 65.0f, 10.0f) );
    //for( i = 0; i < MAIN_BALL_COUNT; ++i ) {
    //    //Scalar fX = 3.0f * (Scalar)( i % 5 );
    //    //Scalar fY = 3.0f * (Scalar)( i / 5 );
    //    StrFormat( strName, TEXT("MyDynamicBall_%d"), i );
    //    m_arrBalls[i] = New MyDynamicBall( strName, m_pBallShape, m_pBallEffect,
    //                                          Vertex3( 0.0f, 0.0f, i * 3.0f ) );
    //}
    //m_arrBoxes[0] = New MyDynamicBox( TEXT("MyDynamicBox_0"), m_pBoxShape, m_pBoxEffect, Vertex3(65.0f, 65.0f, 10.0f) );
    //for( i = 0; i < MAIN_BOX_COUNT; ++i ) {
    //    StringFn->Format( strName, TEXT("MyDynamicBox_%d"), i );
    //    m_arrBoxes[i] = New MyDynamicBox( strName, m_pBoxShape, m_pBoxEffect,
    //                                      Vertex3( 0.0f, 0.0f, i * 3.0f ) );
    //}

    //m_pKinematicBall = New MyKinematicBall( m_pBallShape, m_pBallEffect );
    //m_pSkeleton = New MySkeleton( m_pBallShape, m_pAvatar->GetBody(), m_pBallEffect );
    //m_pParticles = New MyParticles( m_pParticleMesh, m_pParticleEffect );

    //m_arrJoints[0] = PhysicsFn->CreateJointFixed( m_arrBalls[0]->GetRigidBody(), m_pFloorBox->GetRigidBody() );
    //m_arrJoints[0] = PhysicsFn->CreateJointAnchor( m_arrBalls[0]->GetRigidBody(), m_pFloorBox->GetRigidBody(),
    //                                               Vector3(-1.0f, 1.0f, 4.9f), Vector3(0.0f, 0.0f, -0.1f) );
    //m_arrJoints[0] = PhysicsFn->CreateJointHinge( m_arrBalls[0]->GetRigidBody(), m_pFloorBox->GetRigidBody(),
    //                                              Vector3(0.0f, 1.0f, 4.9f), Vector3(0.0f, 0.0f, -0.1f), Vector3::eI );
    //m_arrJoints[0] = PhysicsFn->CreateJointSlider( m_arrBalls[0]->GetRigidBody(), m_pFloorBox->GetRigidBody(), Vector3(1.0f,0.0f,0.1f) );
    //m_arrJoints[0] = PhysicsFn->CreateJointUniversal( m_arrBalls[0]->GetRigidBody(), m_pFloorBox->GetRigidBody(),
    //                                                  Vector3(0.0f, 0.0f, 4.9f), Vector3(0.0f, 0.0f, -0.1f), Vector3::eI, Vector3::eJ );

    m_pRenderCamera = RenderingFn->CreateCamera( true );
    //m_pWorldCamera = New WorldCameraSubjective( m_pRenderCamera, m_pAvatar );
    m_pWorldCamera = New WorldCamera3rdPerson( m_pRenderCamera, m_pAvatar, NULL, 3.0f );

    // World Structure
    WorldFn->GetWorldRoot()->SetDebugFlags( WORLD_DEBUGDRAW_GIZMOS );
    WorldFn->AddChild( m_pAvatar );

    //WorldFn->AddChild( m_pCurve );
    //WorldFn->AddChild( m_pSurface );

    WorldFn->AddChild( m_pFloor );
    //WorldFn->AddChild( m_pLandscape );
    //WorldFn->AddChild( m_pDungeon );

    //for( i = 0; i < MAIN_BALL_COUNT; ++i )
    //    WorldFn->AddChild( m_arrBalls[i] );
    //for( i = 0; i < MAIN_BOX_COUNT; ++i )
    //    WorldFn->AddChild( m_arrBoxes[i] );

    //WorldFn->AddChild( m_pKinematicBall );
    //WorldFn->AddChild( m_pSkeleton );
    //WorldFn->AddChild( m_pParticles );

    WorldFn->SetWorldCamera( m_pWorldCamera );
}
Void MyGame::_DestroyWorld()
{
    UInt i;

    // World Structure
    WorldFn->SetWorldCamera( NULL );
    WorldFn->ResetWorld();

    // World Objects
    Delete( m_pWorldCamera );
    m_pWorldCamera = NULL;
    RenderingFn->DestroyCamera( m_pRenderCamera );
    m_pRenderCamera = NULL;

    //for( i = 0; i < MAIN_JOINT_COUNT; ++i ) {
    //    PhysicsFn->DestroyJoint( m_arrJoints[i] );
    //    m_arrJoints[i] = NULL;
    //}

    //Delete( m_pParticles );
    //m_pParticles = NULL;
    //Delete( m_pSkeleton );
    //m_pSkeleton = NULL;
    //Delete( m_pKinematicBall );
    //m_pKinematicBall = NULL;

    //for( i = 0; i < MAIN_BOX_COUNT; ++i ) {
    //    Delete( m_arrBoxes[i] );
    //    m_arrBoxes[i] = NULL;
    //}
    //for( i = 0; i < MAIN_BALL_COUNT; ++i ) {
    //    Delete( m_arrBalls[i] );
    //    m_arrBalls[i] = NULL;
    //}

    //Delete( m_pDungeon );
    //m_pDungeon = NULL;
    //Delete( m_pLandscape );
    //m_pLandscape = NULL;
    Delete( m_pFloor );
    m_pFloor = NULL;

    //Delete( m_pSurface );
    //m_pSurface = NULL;
    //Delete( m_pCurve );
    //m_pCurve = NULL;

    Delete( m_pAvatar );
    m_pAvatar = NULL;
}

Void MyGame::_CreateGUI()
{
    // Backboard
    m_pBackboardModel = New MyBackboardModel( this );
    GUIFn->SetBackboardModel( m_pBackboardModel );

    // Build a window with all widgets ...
    ////////////////////////////////////////////////////

    // Models
    //GUISectionModel * arrTabs[3] = {
    //    New GUISectionModelDefault(),
    //    New GUISectionModelDefault(),
    //    New GUISectionModelDefault()
    //};
    //const GChar * arrTabNames[3] = {
    //    TEXT("Tab_0"), TEXT("Tab_1"), TEXT("Tab_2")
    //};
    //m_pSwitchTabsModel = New GUISwitchTabsModelDefault( arrTabs, arrTabNames, 3 );
    //m_arrLabelModels[0] = New GUILabelModelDefault( TEXT("There is nothing here ...") );
    //m_arrLabelModels[1] = New GUILabelModelDefault( TEXT("You should look elsewhere ...") );
    //m_arrLabelModels[2] = New GUILabelModelDefault( TEXT("You missed something while blinking ...") );

    // Layout
    //m_pTestWindow = GUIFn->CreateWindow( TEXT("Window_0"), Rectangle2( Point2(50,50), 600, 300 ),
    //                                     GUIWINDOW_STYLE_DEFAULT );

    //GUIWidget * pClientRoot = m_pTestWindow->GetClientRoot();
    //const Rectangle2 & rectClient = pClientRoot->GetClientRect();

    //GUIGridLayout winLayout;
    //winLayout.FillX = GUILAYOUT_FILL_FIXED;
    //winLayout.FillY = GUILAYOUT_FILL_FIXED;
    //winLayout.Anchor = GUILAYOUT_ANCHOR_FIXED;

    //winLayout.FixedPos.X = 0;
    //winLayout.FixedPos.Y = 0;
    //winLayout.FixedSize.X = rectClient.Width();
    //winLayout.FixedSize.Y = rectClient.Height();
    //GUISwitchTabs * pSwitchTabs = (GUISwitchTabs*)( GUIFn->CreateWidget( GUIWIDGET_SWITCHTABS, m_pSwitchTabsModel, pClientRoot,
    //                                                                     winLayout, TEXT("SwitchTabsTest") ) );

    //winLayout.FixedPos.X = 50;
    //winLayout.FixedPos.Y = 50;
    //winLayout.FixedSize.X = 400;
    //winLayout.FixedSize.Y = 20;
    //GUIFn->CreateWidget( GUIWIDGET_LABEL, m_arrLabelModels[0], pSwitchTabs->GetTab(0), winLayout, TEXT("Tab0_Label") );
    //GUIFn->CreateWidget( GUIWIDGET_LABEL, m_arrLabelModels[1], pSwitchTabs->GetTab(1), winLayout, TEXT("Tab1_Label") );
    //GUIFn->CreateWidget( GUIWIDGET_LABEL, m_arrLabelModels[2], pSwitchTabs->GetTab(2), winLayout, TEXT("Tab2_Label") );

    //GUIFn->RegisterWindow( m_pTestWindow );
    //m_pTestWindow->SetVisible( true );
    //m_pTestWindow->QueryUpdate();
}
Void MyGame::_DestroyGUI()
{
    // Layout
    //GUIFn->UnregisterWindow( m_pTestWindow );
    //GUIFn->DestroyWindow( m_pTestWindow );
    //m_pTestWindow = NULL;

    // Models
    //Delete( m_arrLabelModels[2] );
    //m_arrLabelModels[2] = NULL;
    //Delete( m_arrLabelModels[1] );
    //m_arrLabelModels[1] = NULL;
    //Delete( m_arrLabelModels[0] );
    //m_arrLabelModels[0] = NULL;
    //Delete( m_pSwitchTabsModel );
    //m_pSwitchTabsModel = NULL;

    // Backboard
    GUIFn->SetBackboardModel( NULL );
    Delete( m_pBackboardModel );
    m_pBackboardModel = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
#endif // MAIN_DEBUG


