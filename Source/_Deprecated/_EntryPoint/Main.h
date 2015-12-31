/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Main.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_MAIN_H
#define SCARAB_ENTRYPOINT_MAIN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Engine/Application/ApplicationManager.h"

#include "Entities.h"
#include "MyLandscape.h"
#include "MyDungeon.h"

#include "../Engine/Rendering/Effects/EffectLibrary/Lighting/EffectMaterial.h"
#include "../Engine/Rendering/Effects/EffectLibrary/Lighting/EffectLightPointPhong.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define MAIN_DEBUG

// Prototypes
class MyBackboardModel;

class MyGame;

/////////////////////////////////////////////////////////////////////////////////
#ifdef MAIN_DEBUG

/////////////////////////////////////////////////////////////////////////////////
// The ScratchTests class
//class ScratchTests : public ConsoleApplication
//{
//    // Application class
//    DeclareApplication;
//
//public:
//    ScratchTests();
//    ~ScratchTests();
//
//    virtual Int Main( const CommandLine & cmdLine );
//};
//RegisterApplication( ScratchTests );

/////////////////////////////////////////////////////////////////////////////////
#else // !MAIN_DEBUG

/////////////////////////////////////////////////////////////////////////////////
// The MyBackboard class
class MyBackboardModel : public GUIBackboardModel
{
public:
    MyBackboardModel( MyGame * pAppInstance );
    virtual ~MyBackboardModel();

    virtual Void OnKeyPress( KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnMousePress( const Point2 & ptMousePos, KeyCode iKey, GUIEventFlag iFlags );

private:
    static Void _MsgBox_ConfirmExit( GUIMessageBoxResult iResult, Void * pUserData );

    MyGame * m_pAppInstance;
    GUIMenuModel * m_pMenuModel;
};

/////////////////////////////////////////////////////////////////////////////////
// The MyGame class
class MyGame : public WindowApplication
{
    // Application class
    DeclareApplication;

public:
    MyGame();
    virtual ~MyGame();

    virtual Bool OnInitialize();
    virtual Void OnCleanup();

    virtual Bool OnFrameMove();
    virtual Void OnFrameRender();

    virtual Void OnExit();

    virtual Bool OnKeyPress( KeyCode iKey );
    virtual Bool OnKeyRelease( KeyCode iKey );

    virtual Void OnMouseMove( UInt iX, UInt iY );
    virtual Bool OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta );
    virtual Bool OnMousePress( UInt iX, UInt iY, KeyCode iButton );
    virtual Bool OnMouseRelease( UInt iX, UInt iY, KeyCode iButton );
    virtual Bool OnMouseDblClick( UInt iX, UInt iY, KeyCode iButton );

private:
    friend class MyBackboardModel;

    // Input
    Void _CreateActions();
    Void _DestroyActions();

    UInt m_iActionMap;

    // Effects
    Void _CreateEffects();
    Void _DestroyEffects();

    VertexFormat * m_pVFColor;
    VertexFormat * m_pVFSun;

    Light m_lightSun;

    Material m_matAvatarColor;
    Material m_matCurveColor;
    Material m_matSurfaceColor;
    Material m_matFloorColor;
    Material m_matBallColor;
    Material m_matBoxColor;
    Material m_matParticleColor;

    EffectMaterial * m_pEffectColor;
    EffectLightPointPhong * m_pEffectSun;

    EffectInstance * m_pAvatarEffect;
    EffectInstance * m_pCurveEffect;
    EffectInstance * m_pSurfaceEffect;
    EffectInstance * m_pFloorEffect;
    EffectInstance * m_pBallEffect;
    EffectInstance * m_pBoxEffect;
    EffectInstance * m_pParticleEffect;

    //Material m_matShadowPlaneColor;
    //EffectInstance * m_pShadowPlaneEffect;
    //ShadowPlane * m_pShadowPlane;
    //EffectPlanarShadows * m_pShadowsEffect;

    // Instanced geometry
    Void _CreateInstancedGeometry();
    Void _DestroyInstancedGeometry();

    MeshBox * m_pFloorMesh;
    ShapeBox * m_pFloorShape;

    MeshSphere * m_pBallMesh;
    ShapeSphere * m_pBallShape;

    MeshBox * m_pBoxMesh;
    ShapeBox * m_pBoxShape;

    MeshBox * m_pParticleMesh;

    // World
#define MAIN_BALL_COUNT 5
#define MAIN_BOX_COUNT 5
#define MAIN_JOINT_COUNT 1

    Void _CreateWorld();
    Void _DestroyWorld();

    MyAvatar * m_pAvatar;

    MyCurve * m_pCurve;
    MySurface * m_pSurface;

    MyFloor * m_pFloor;
    //MyLandscape * m_pLandscape;
    MyDungeon * m_pDungeon;

    MyDynamicBall * m_arrBalls[MAIN_BALL_COUNT];
    MyDynamicBox * m_arrBoxes[MAIN_BOX_COUNT];

    MyKinematicBall * m_pKinematicBall;
    MySkeleton * m_pSkeleton;
    //MyParticles * m_pParticles;

    JointEdge * m_arrJoints[MAIN_JOINT_COUNT];

    Camera * m_pRenderCamera;
    //WorldCameraSubjective * m_pWorldCamera;
    WorldCamera3rdPerson * m_pWorldCamera;

    // GUI
    Void _CreateGUI();
    Void _DestroyGUI();

    MyBackboardModel * m_pBackboardModel;

    GUISwitchTabsModel * m_pSwitchTabsModel;
    GUILabelModel * m_arrLabelModels[3];

    GUIWindow * m_pTestWindow;
    GChar m_strFPSText[64];
};

RegisterApplication( MyGame );

/////////////////////////////////////////////////////////////////////////////////
#endif // MAIN_DEBUG

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Main.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_MAIN_H

