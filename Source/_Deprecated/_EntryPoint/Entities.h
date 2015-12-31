/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Entities.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_ENTITIES_H
#define SCARAB_ENTRYPOINT_ENTITIES_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Engine/Application/ApplicationManager.h"

#include "../Lib/Math/Geometry/Curves/2D/CurveLibrary/BSplinePatch2.h"
#include "../Lib/Math/Geometry/Curves/3D/CurveLibrary/BSplinePatch3.h"
#include "../Lib/Math/Geometry/Surfaces/SurfaceLibrary/BSplineSurface.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// Entities
class MyAvatar : public WorldLeaf
{
public:
    MyAvatar( ShapeSphere * pShape, EffectInstance * pEffect, const Vertex3 & vPosition );
    virtual ~MyAvatar();

    inline ManualController * GetController() const;

private:
    KinematicBody * m_pAvatarBody;
    ManualController * m_pAvatarController;
};

/////////////////////////////////////////////////////////////////////////////////

class MyCurve : public WorldLeaf
{
public:
    MyCurve( VertexFormat * pVF, EffectInstance * pEffect );
    virtual ~MyCurve();

private:
    Vertex3 m_arrControlPoints[6];

    Curve3 * m_pCurve;
    MeshCurve * m_pCurveMesh;
};

/////////////////////////////////////////////////////////////////////////////////

class MySurface : public WorldLeaf
{
public:
    MySurface( VertexFormat * pVF, EffectInstance * pEffect );
    virtual ~MySurface();

private:
    Vertex3 m_arrControlPoints[16];

    ParametricSurface * m_pSurface;
    MeshSurface * m_pSurfaceMesh;
};

/////////////////////////////////////////////////////////////////////////////////

class MyFloor : public WorldLeaf
{
public:
    MyFloor( ShapeBox * pShape, EffectInstance * pEffect, const Vertex3 & vPosition );
    virtual ~MyFloor();

private:
    RigidBody * m_pFloorBody;
};

/////////////////////////////////////////////////////////////////////////////////

class MyDynamicBall : public WorldLeaf
{
public:
    MyDynamicBall( const GChar * strName, ShapeSphere * pShape, EffectInstance * pEffect, const Vertex3 & vPosition );
    virtual ~MyDynamicBall();

private:
    RigidBody * m_pBallBody;
};

/////////////////////////////////////////////////////////////////////////////////

class MyDynamicBox : public WorldLeaf
{
public:
    MyDynamicBox( const GChar * strName, ShapeBox * pShape, EffectInstance * pEffect, const Vertex3 & vPosition );
    virtual ~MyDynamicBox();

private:
    RigidBody * m_pBoxBody;
};

/////////////////////////////////////////////////////////////////////////////////

class MyKinematicBall : public WorldLeaf
{
public:
    MyKinematicBall( ShapeSphere * pShape, EffectInstance * pEffect );
    virtual ~MyKinematicBall();

private:
    KinematicBody * m_pBallBody;

    //KeyFrameController * m_pController;

    Vertex3 m_arrControlPointsLP[6];
    Vertex2 m_arrControlPointsLT[6];
    Vertex3 m_arrControlPointsAP[6];
    Vertex2 m_arrControlPointsAT[6];

    Curve3 * m_pLinearPath;
    Curve2 * m_pLinearTravel;
    Curve3 * m_pAngularPath;
    Curve2 * m_pAngularTravel;

    PathController * m_pController;

    //KeyFrameController * m_pControllerA;
    //KeyFrameController * m_pControllerB;
    //BlendController * m_pController;
};

/////////////////////////////////////////////////////////////////////////////////

class MySkeleton : public WorldNode
{
public:
    MySkeleton( ShapeSphere * pShape, BodyNode * pGoal, EffectInstance * pEffect );
    virtual ~MySkeleton();

private:
    KinematicBody * m_pChestBody;
    KinematicBody * m_pArmBody;
    KinematicBody * m_pHandBody;

    Skeleton * m_pSkeleton;

    WorldLeaf * m_pChest;
    WorldLeaf * m_pArm;
    WorldLeaf * m_pHand;
};

/////////////////////////////////////////////////////////////////////////////////

//class MyParticles : public WorldNode
//{
//public:
//    MyParticles( MeshBox * pMesh, EffectInstance * pEffect );
//    virtual ~MyParticles();
//
//private:
//    ParticleSystem * m_pParticles;
//
//    WorldLeaf * m_arrParticleLeaves[20];
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Entities.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_ENTITIES_H



