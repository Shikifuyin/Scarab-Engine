/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Entities.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Testing stuff ...
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
#include "Entities.h"

/////////////////////////////////////////////////////////////////////////////////
// Entities implementation
MyAvatar::MyAvatar( ShapeSphere * pShape, EffectInstance * pEffect, const Vertex3 & vPosition ):
    WorldLeaf( TEXT("MyAvatar") )
{
    m_pAvatarBody = PhysicsFn->CreateKinematicBody( true, pShape, 1.0f, vPosition, Quaternion::Identity, Vector3::Null, Vector3::Null );
    m_pAvatarBody->SetRestitution( 0.0f );
    m_pAvatarBody->SetFriction( 0.0f );

    m_pAvatarController = PhysicsFn->CreateManualController( TEXT("MyAvatarController"), vPosition, Quaternion::Identity,
                                                                                         Vector3::Null, Vector3::Null );
    m_pAvatarController->Enabled = true;

    m_pAvatarBody->AttachController( m_pAvatarController );

    SetMesh( pShape->GetMesh() );
    SetEffectInstance( pEffect );
    SetBody( m_pAvatarBody );

    //CastShadows = true;

    SetDebugFlags( WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS |
                   WORLD_DEBUGDRAW_LINEARVELOCITY | WORLD_DEBUGDRAW_ANGULARVELOCITY );
}
MyAvatar::~MyAvatar()
{
    SetBody( NULL );
    SetEffectInstance( NULL );
    SetMesh( NULL );

    m_pAvatarBody->DetachMotionController();

    PhysicsFn->DestroyController( m_pAvatarController );
    m_pAvatarController = NULL;

    PhysicsFn->DestroyBody( m_pAvatarBody );
    m_pAvatarBody = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

MyCurve::MyCurve( VertexFormat * pVF, EffectInstance * pEffect ):
    WorldLeaf( TEXT("MyCurve") )
{
    m_arrControlPoints[0] = Vertex3(  5.0f,  5.0f, -3.75f );
    m_arrControlPoints[1] = Vertex3(  3.0f,  3.0f, -3.75f );
    m_arrControlPoints[2] = Vertex3( -3.0f,  3.0f, -3.75f );
    m_arrControlPoints[3] = Vertex3( -3.0f, -3.0f, -3.75f );
    m_arrControlPoints[4] = Vertex3(  3.0f, -3.0f, -3.75f );
    m_arrControlPoints[5] = Vertex3(  5.0f, -5.0f, -3.75f );

    m_pCurve = New BSplinePatch3( m_arrControlPoints, 6, 3, false, false );

    m_pCurveMesh = New MeshCurve( m_pCurve, false, 32, pVF, false );

    SetMesh( m_pCurveMesh );
    SetEffectInstance( pEffect );
}
MyCurve::~MyCurve()
{
    SetEffectInstance( NULL );
    SetMesh( NULL );

    Delete( m_pCurveMesh );
    m_pCurveMesh = NULL;

    Delete( m_pCurve );
    m_pCurve = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

MySurface::MySurface( VertexFormat * pVF, EffectInstance * pEffect ):
    WorldLeaf( TEXT("MySurface") )
{
    m_arrControlPoints[0]  = Vertex3(0.0f,0.0f,0.0f); m_arrControlPoints[1]  = Vertex3(2.0f,0.0f, 0.0f); m_arrControlPoints[2]  = Vertex3(4.0f,0.0f, 0.0f); m_arrControlPoints[3]  = Vertex3(6.0f,0.0f,0.0f);
    m_arrControlPoints[4]  = Vertex3(0.0f,2.0f,0.0f); m_arrControlPoints[5]  = Vertex3(2.0f,2.0f,-2.0f); m_arrControlPoints[6]  = Vertex3(4.0f,2.0f,+2.0f); m_arrControlPoints[7]  = Vertex3(6.0f,2.0f,0.0f);
    m_arrControlPoints[8]  = Vertex3(0.0f,4.0f,0.0f); m_arrControlPoints[9]  = Vertex3(2.0f,4.0f,+2.0f); m_arrControlPoints[10] = Vertex3(4.0f,4.0f,-2.0f); m_arrControlPoints[11] = Vertex3(6.0f,4.0f,0.0f);
    m_arrControlPoints[12] = Vertex3(0.0f,6.0f,0.0f); m_arrControlPoints[13] = Vertex3(2.0f,6.0f, 0.0f); m_arrControlPoints[14] = Vertex3(4.0f,6.0f, 0.0f); m_arrControlPoints[15] = Vertex3(6.0f,6.0f,0.0f);

    m_pSurface = New BSplineSurface( m_arrControlPoints, 4, 4, 3, 3, false, false, false, false );

    m_pSurfaceMesh = New MeshSurface( m_pSurface, 32, 32, pVF, false );

    SetMesh( m_pSurfaceMesh );
    SetEffectInstance( pEffect );
}
MySurface::~MySurface()
{
    SetEffectInstance( NULL );
    SetMesh( NULL );

    Delete( m_pSurfaceMesh );
    m_pSurfaceMesh = NULL;

    Delete( m_pSurface );
    m_pSurface = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

MyFloor::MyFloor( ShapeBox * pShape, EffectInstance * pEffect, const Vertex3 & vPosition ):
    WorldLeaf( TEXT("MyFloor") )
{
    m_pFloorBody = PhysicsFn->CreateRigidBody( true, pShape, 1.0f, vPosition );
    m_pFloorBody->SetRestitution( 1.0f );
    //m_pFloorBody->SetFriction( 1.0f );

    SetMesh( pShape->GetMesh() );
    SetEffectInstance( pEffect );
    SetBody( m_pFloorBody );

    SetDebugFlags( WORLD_DEBUGDRAW_GIZMOS | WORLD_DEBUGDRAW_PROXIES );
}
MyFloor::~MyFloor()
{
    SetBody( NULL );
    SetEffectInstance( NULL );
    SetMesh( NULL );

    PhysicsFn->DestroyBody( m_pFloorBody );
    m_pFloorBody = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

MyDynamicBall::MyDynamicBall( const GChar * strName, ShapeSphere * pShape, EffectInstance * pEffect, const Vertex3 & vPosition ):
    WorldLeaf( strName )
{
    m_pBallBody = PhysicsFn->CreateRigidBody( false, pShape, 1.0f, vPosition );
    //m_pBallBody->SetRestitution( 0.5f );
    //m_pBallBody->SetFriction( 0.8f );

    SetMesh( pShape->GetMesh() );
    SetEffectInstance( pEffect );
    SetBody( m_pBallBody );

    SetDebugFlags( WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS |
                   WORLD_DEBUGDRAW_LINEARVELOCITY | WORLD_DEBUGDRAW_ANGULARVELOCITY );
}
MyDynamicBall::~MyDynamicBall()
{
    SetBody( NULL );
    SetEffectInstance( NULL );
    SetMesh( NULL );

    PhysicsFn->DestroyBody( m_pBallBody );
    m_pBallBody = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

MyDynamicBox::MyDynamicBox( const GChar * strName, ShapeBox * pShape, EffectInstance * pEffect, const Vertex3 & vPosition ):
    WorldLeaf( strName )
{
    m_pBoxBody = PhysicsFn->CreateRigidBody( false, pShape, 1.0f, vPosition );
    //m_pBoxBody->SetRestitution( 0.8f );
    //m_pBoxBody->SetFriction( 0.8f );

    SetMesh( pShape->GetMesh() );
    SetEffectInstance( pEffect );
    SetBody( m_pBoxBody );

    SetDebugFlags( WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS |
                   WORLD_DEBUGDRAW_LINEARVELOCITY | WORLD_DEBUGDRAW_ANGULARVELOCITY );
}
MyDynamicBox::~MyDynamicBox()
{
    SetBody( NULL );
    SetEffectInstance( NULL );
    SetMesh( NULL );

    PhysicsFn->DestroyBody( m_pBoxBody );
    m_pBoxBody = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

MyKinematicBall::MyKinematicBall( ShapeSphere * pShape, EffectInstance * pEffect ):
    WorldLeaf( TEXT("MyKinematicBall") )
{
    m_pBallBody = PhysicsFn->CreateKinematicBody( true, pShape, 1.0f, Vertex3(1.0f, 0.0f, 0.0f) );
    //m_pBallBody->SetRestitution( 0.5f );
    //m_pBallBody->SetFriction( 0.5f );

    /////////////////////////////////////////////////

    //m_pController = PhysicsFn->CreateKeyFrameController( TEXT("BallController"), 3, 0, 0, 0, 0 );
    //m_pController->Enabled = true;
    //m_pController->MinTime = 0.0f;
    //m_pController->MaxTime = 1.0f;
    //m_pController->Frequency = 1.0f;
    //m_pController->Phase = 0.0f;
    //m_pController->Repeat = CONTROLLER_REPEAT_MIRROR;

    //m_pController->SetPositionTime( 0, 0.0f );
    //m_pController->SetPosition( 0, Vertex3(1.0f, 1.0f, 1.0f) );
    //m_pController->SetPositionTime( 1, 0.5f );
    //m_pController->SetPosition( 1, Vertex3(2.0f, 2.0f, 1.0f) );
    //m_pController->SetPositionTime( 2, 1.0f );
    //m_pController->SetPosition( 2, Vertex3(3.0f, 1.0f, 1.0f) );

    /////////////////////////////////////////////////

    m_arrControlPointsLP[0] = Vertex3(  5.0f,  5.0f, -3.75f );
    m_arrControlPointsLP[1] = Vertex3(  3.0f,  3.0f, -3.75f );
    m_arrControlPointsLP[2] = Vertex3( -3.0f,  3.0f, -3.75f );
    m_arrControlPointsLP[3] = Vertex3( -3.0f, -3.0f, -3.75f );
    m_arrControlPointsLP[4] = Vertex3(  3.0f, -3.0f, -3.75f );
    m_arrControlPointsLP[5] = Vertex3(  5.0f, -5.0f, -3.75f );
    m_pLinearPath = New BSplinePatch3( m_arrControlPointsLP, 6, 3, false, false );

    Scalar fLength = m_pLinearPath->Length();
    m_arrControlPointsLT[0] = Vertex2( 0.0f,           0.0f );
    m_arrControlPointsLT[1] = Vertex2( fLength * 0.2f, 0.2f );
    m_arrControlPointsLT[2] = Vertex2( fLength * 0.4f, 0.4f );
    m_arrControlPointsLT[3] = Vertex2( fLength * 0.6f, 0.6f );
    m_arrControlPointsLT[4] = Vertex2( fLength * 0.8f, 0.8f );
    m_arrControlPointsLT[5] = Vertex2( fLength,        1.0f );
    m_pLinearTravel = New BSplinePatch2( m_arrControlPointsLT, 6, 3, false, false );

    m_arrControlPointsAP[0] = Vertex3( 0.0f, 0.0f, 0.0f );
    m_arrControlPointsAP[1] = Vertex3( 0.0f, 0.0f, 0.0f );
    m_arrControlPointsAP[2] = Vertex3( 0.0f, 0.0f, 0.0f );
    m_arrControlPointsAP[3] = Vertex3( 0.0f, 0.0f, 0.0f );
    m_arrControlPointsAP[4] = Vertex3( 0.0f, 0.0f, 0.0f );
    m_arrControlPointsAP[5] = Vertex3( 0.0f, 0.0f, 0.0f );
    m_pAngularPath = New BSplinePatch3( m_arrControlPointsAP, 6, 3, false, false );

    fLength = m_pAngularPath->Length();
    m_arrControlPointsAT[0] = Vertex2( 0.0f,           0.0f );
    m_arrControlPointsAT[1] = Vertex2( fLength * 0.2f, 0.2f );
    m_arrControlPointsAT[2] = Vertex2( fLength * 0.4f, 0.4f );
    m_arrControlPointsAT[3] = Vertex2( fLength * 0.6f, 0.6f );
    m_arrControlPointsAT[4] = Vertex2( fLength * 0.8f, 0.8f );
    m_arrControlPointsAT[5] = Vertex2( fLength,        1.0f );
    m_pAngularTravel = New BSplinePatch2( m_arrControlPointsAT, 6, 3, false, false );

    m_pController = PhysicsFn->CreatePathController( TEXT("BallController"), m_pLinearPath, m_pLinearTravel,
                                                                             m_pAngularPath, m_pAngularTravel,
                                                                             EULER_ANGLES_ZYX );
    m_pController->Enabled = true;
    m_pController->MinTime = 0.0f;
    m_pController->MaxTime = 1.0f;
    m_pController->Frequency = 1.0f;
    m_pController->Phase = 0.0f;
    m_pController->Repeat = CONTROLLER_REPEAT_MIRROR;

    /////////////////////////////////////////////////

    //m_pControllerA = PhysicsFn->CreateKeyFrameController( TEXT("BallController_A"), 3, 0, 0 );
    //m_pControllerA->Enabled = true;
    //m_pControllerA->MinTime = 0.0f;
    //m_pControllerA->MaxTime = 10.0f;
    //m_pControllerA->Frequency = 1.0f;
    //m_pControllerA->Phase = 0.0f;
    //m_pControllerA->Repeat = CONTROLLER_REPEAT_CLAMP;

    //m_pControllerA->SetPositionTime( 0, 0.0f );
    //m_pControllerA->SetPosition( 0, Vertex3(1.0f, 0.0f, 0.0f) );
    //m_pControllerA->SetPositionTime( 1, 5.0f );
    //m_pControllerA->SetPosition( 1, Vertex3(2.0f, 0.0f, 0.0f) );
    //m_pControllerA->SetPositionTime( 2, 10.0f );
    //m_pControllerA->SetPosition( 2, Vertex3(3.0f, 0.0f, 0.0f) );

    //m_pControllerB = PhysicsFn->CreateKeyFrameController( TEXT("BallController_B"), 3, 0, 0 );
    //m_pControllerB->Enabled = true;
    //m_pControllerB->MinTime = 0.0f;
    //m_pControllerB->MaxTime = 10.0f;
    //m_pControllerB->Frequency = 1.0f;
    //m_pControllerB->Phase = 0.0f;
    //m_pControllerB->Repeat = CONTROLLER_REPEAT_CLAMP;

    //m_pControllerB->SetPositionTime( 0, 0.0f );
    //m_pControllerB->SetPosition( 0, Vertex3(3.0f, 0.0f, 0.0f) );
    //m_pControllerB->SetPositionTime( 1, 5.0f );
    //m_pControllerB->SetPosition( 1, Vertex3(3.0f, 1.0f, 0.0f) );
    //m_pControllerB->SetPositionTime( 2, 10.0f );
    //m_pControllerB->SetPosition( 2, Vertex3(3.0f, 2.0f, 0.0f) );

    //m_pController = PhysicsFn->CreateBlendController( TEXT("BallController"), m_pControllerA, m_pControllerB, true );
    //m_pController->Enabled = true;
    //m_pController->MinTime = 0.0f;
    //m_pController->MaxTime = 5.0f;
    //m_pController->Frequency = 1.0f;
    //m_pController->Phase = 0.0f;
    //m_pController->Repeat = CONTROLLER_REPEAT_CLAMP;

    //m_pController->StartTime = 5.0f;
    //m_pController->StopTime = 5.0f;

    /////////////////////////////////////////////////

    m_pBallBody->AttachController( m_pController );

    SetMesh( pShape->GetMesh() );
    SetEffectInstance( pEffect );
    SetBody( m_pBallBody );
}
MyKinematicBall::~MyKinematicBall()
{
    SetBody( NULL );
    SetEffectInstance( NULL );
    SetMesh( NULL );

    m_pBallBody->DetachMotionController();

    PhysicsFn->DestroyController( m_pController );
    m_pController = NULL;

    //PhysicsFn->DestroyController( m_pControllerB );
    //PhysicsFn->DestroyController( m_pControllerA );
    //m_pControllerB = NULL;
    //m_pControllerA = NULL;

    Delete( m_pAngularTravel );
    m_pAngularTravel = NULL;
    Delete( m_pAngularPath );
    m_pAngularPath = NULL;
    Delete( m_pLinearTravel );
    m_pLinearTravel = NULL;
    Delete( m_pLinearPath );
    m_pLinearPath = NULL;

    PhysicsFn->DestroyBody( m_pBallBody );
    m_pBallBody = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

MySkeleton::MySkeleton( ShapeSphere * pShape, BodyNode * pGoal, EffectInstance * pEffect ):
    WorldNode( TEXT("MySkeleton") )
{
    m_pChestBody = PhysicsFn->CreateKinematicBody( true, pShape, 1.0f, Vertex3(0.0f, 0.0f, 0.0f) );
    m_pArmBody = PhysicsFn->CreateKinematicBody( true, pShape, 1.0f, Vertex3(3.0f, 0.0f, 0.0f) );
    m_pHandBody = PhysicsFn->CreateKinematicBody( true, pShape, 1.0f, Vertex3(6.0f, 0.0f, 0.0f) );

    m_pSkeleton = PhysicsFn->CreateSkeleton( m_pChestBody, 1, 1 );

    IKBone * pChestBone = m_pSkeleton->GetRootBone();
    IKBone * pArmBone = m_pSkeleton->CreateBone( m_pArmBody, 1 );
    IKBone * pHandBone = m_pSkeleton->CreateBone( m_pHandBody, 0 );

    IKJointController * pShoulder = m_pSkeleton->CreateJoint( TEXT("Shoulder"), pChestBone, pArmBone, 0, 1 );
    IKJointController * pAnckle = m_pSkeleton->CreateJoint( TEXT("Anckle"), pArmBone, pHandBone, 0, 1 );

    m_pSkeleton->CreateGoal( 0, pHandBone, pGoal, 1.0f );

    m_pSkeleton->Finalize();

    pShoulder->Enabled = true;
    pAnckle->Enabled = true;

    //pShoulder->AllowTranslate[0] = true;
    //pShoulder->MinTranslate[0] = -3.0f;
    //pShoulder->MaxTranslate[0] = +3.0f;
    //pShoulder->AllowTranslate[1] = true;
    //pShoulder->MinTranslate[1] = -3.0f;
    //pShoulder->MaxTranslate[1] = +3.0f;
    pShoulder->AllowRotate[2] = true;

    //pAnckle->AllowTranslate[0] = true;
    //pAnckle->MinTranslate[0] = -3.0f;
    //pAnckle->MaxTranslate[0] = +3.0f;
    //pAnckle->AllowTranslate[1] = true;
    //pAnckle->MinTranslate[1] = -3.0f;
    //pAnckle->MaxTranslate[1] = +3.0f;
    pAnckle->AllowRotate[2] = true;

    m_pChest = WorldFn->CreateLeaf( TEXT("Chest") );
    m_pChest->SetMesh( pShape->GetMesh() );
    m_pChest->SetEffectInstance( pEffect );
    m_pChest->SetBody( m_pChestBody );

    m_pArm = WorldFn->CreateLeaf( TEXT("Arm") );
    m_pArm->SetMesh( pShape->GetMesh() );
    m_pArm->SetEffectInstance( pEffect );
    m_pArm->SetBody( m_pArmBody );

    m_pHand = WorldFn->CreateLeaf( TEXT("Hand") );
    m_pHand->SetMesh( pShape->GetMesh() );
    m_pHand->SetEffectInstance( pEffect );
    m_pHand->SetBody( m_pHandBody );

    AddChild( m_pChest );
    AddChild( m_pArm );
    AddChild( m_pHand );
}
MySkeleton::~MySkeleton()
{
    RemoveAllChildren();

    m_pHand->SetBody( NULL );
    m_pHand->SetEffectInstance( NULL );
    m_pHand->SetMesh( NULL );
    WorldFn->DestroyNode( m_pHand );

    m_pArm->SetBody( NULL );
    m_pArm->SetEffectInstance( NULL );
    m_pArm->SetMesh( NULL );
    WorldFn->DestroyNode( m_pArm );

    m_pChest->SetBody( NULL );
    m_pChest->SetEffectInstance( NULL );
    m_pChest->SetMesh( NULL );
    WorldFn->DestroyNode( m_pChest );

    PhysicsFn->DestroySystem( m_pSkeleton );
    m_pSkeleton = NULL;

    PhysicsFn->DestroyBody( m_pHandBody );
    m_pHandBody = NULL;

    PhysicsFn->DestroyBody( m_pArmBody );
    m_pArmBody = NULL;

    PhysicsFn->DestroyBody( m_pChestBody );
    m_pChestBody = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

//MyParticles::MyParticles( MeshBox * pMesh, EffectInstance * pEffect )
//{
//    m_pParticles = PhysicsFn->CreateParticleSystem( 0.1f );
//
//    m_pParticles->GenerateSphere( Sphere( Vertex3(0.0f,0.0f,0.0f), 3.0f ), 20 );
//    m_pParticles->SetActiveParticleCount( 20 );
//
//    GChar strName[16];
//
//    for( UInt i = 0; i < 20; ++i ) {
//        StrFormat( strName, TEXT("Particle_%d"), i );
//        m_arrParticleLeaves[i] = WorldFn->CreateLeaf( strName );
//        m_arrParticleLeaves[i]->SetMesh( pMesh );
//        m_arrParticleLeaves[i]->SetEffectInstance( pEffect );
//        m_arrParticleLeaves[i]->SetBody( m_pParticles->GetParticleBody(i) );
//
//        AddChild( m_arrParticleLeaves[i] );
//    }
//}
//MyParticles::~MyParticles()
//{
//    RemoveAllChildren();
//
//    for( UInt i = 0; i < 20; ++i ) {
//        m_arrParticleLeaves[i]->SetBody( NULL );
//        m_arrParticleLeaves[i]->SetEffectInstance( NULL );
//        m_arrParticleLeaves[i]->SetMesh( NULL );
//        WorldFn->DestroyNode( m_arrParticleLeaves[i] );
//    }
//
//    PhysicsFn->DestroySystem( m_pParticles );
//    m_pParticles = NULL;
//}

