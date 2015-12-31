/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/Systems/SoftBody.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : System : Soft bodies, Aero-Dynamics, Clustered collisions
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////
/*
/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef KOALA_ENGINE_PHYSICS_BODIES_SYSTEMS_SOFTBODY_H
#define KOALA_ENGINE_PHYSICS_BODIES_SYSTEMS_SOFTBODY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Datastruct/Array/Array.h"

#include "../../../../Lib/Math/Random/Random.h"

#include "../../../../Lib/Math/Geometry/SpacePartitions/DynamicBVTree.h"

#include "../SystemBody.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions


enum SoftBodyJointType
{
    SOFTBODY_JOINT_LINEAR = 0,
    SOFTBODY_JOINT_ANGULAR,
    SOFTBODY_JOINT_CONTACT,

    SOFTBODY_JOINT_COUNT
};

    // Flags
enum SoftBodyCollisionFlags
{
    SOFTBODY_COLLISION_RVSMask = 0x000f,
    SOFTBODY_COLLISION_SDF_RS  = 0x0001,
    SOFTBODY_COLLISION_CL_RS   = 0x0002,

    SOFTBODY_COLLISION_SVSMask = 0x0030,
    SOFTBODY_COLLISION_VF_SS   = 0x0010,
    SOFTBODY_COLLISION_CL_SS   = 0x0020,
    SOFTBODY_COLLISION_CL_SELF = 0x0040
};

/////////////////////////////////////////////////////////////////////////////////
// Collision stuff

class JointParameters { public:
    Scalar fERP;   // erp (def = 1.0f)
    Scalar fSplit; // split (def = 1.0f)
    Scalar fCFM;   // cfm (def = 1.0f)
};
class Joint { public:
    Joint();
    virtual ~Joint();

    virtual SoftBodyJointType GetType() const = 0;

    virtual Void Prepare( Scalar fTimeStep, UInt iIterations );
    virtual Void Solve( Scalar fTimeStep, Scalar fSOR ) = 0;
    virtual Void Finalize( Scalar fTimeStep ) = 0; // Terminate

    Body jointBodyA;        //
    Body jointBodyB;        // m_bodies[2]
    Vector3 vReferenceA;    //
    Vector3 vReferenceB;    // m_refs[2]
    Scalar fERP;            // m_erp
    Scalar fCFM;            // m_cfm
    Scalar fSplit;          // m_split
    Vector3 vDrift;         // m_drift
    Vector3 vSplitDrift;    // m_sdrift
    Matrix3 matMass;        // m_massmatrix
    Bool bDelete;
};

class JointLinearParameters : public JointParameters { public:
    Vertex3 vPosition; // position
};
class JointLinear : public Joint { public:
    inline virtual SoftBodyJointType GetType() const;

    virtual Void Prepare( Scalar fTimeStep, UInt iIterations );
    virtual Void Solve( Scalar fTimeStep, Scalar fSOR );
    virtual Void Finalize( Scalar fTimeStep ); // Terminate

    Vector3 vRelativePosA; //
    Vector3 vRelativePosB; // m_rpos[2]
};

class JointAngularParameters : public JointParameters { public:
    Vector3 vAxis; // axis
    //JointAngular::IControl * icontrol;
};
class JointAngular : public Joint { public:
    //class IControl { public:
    //    virtual Void Prepare( JointAngular* ) {}
    //    virtual Scalar Speed( JointAngular*, Scalar fCurrent ) { return fCurrent; }
    //    static IControl * Default() { static IControl def; return &def; }
    //};

    inline virtual SoftBodyJointType GetType() const;

    virtual Void Prepare( Scalar fTimeStep, UInt iIterations );
    virtual Void Solve( Scalar fTimeStep, Scalar fSOR );
    virtual Void Finalize( Scalar fTimeStep ); // Terminate

    Vector3 vAxisA; //
    Vector3 vAxisB; // m_axis[2]
    //IControl * m_icontrol;
};

class JointContact : public Joint { public:
    inline virtual SoftBodyJointType GetType() const;

    virtual Void Prepare( Scalar fTimeStep, UInt iIterations );
    virtual Void Solve( Scalar fTimeStep, Scalar fSOR );
    virtual Void Finalize( Scalar fTimeStep ); // Terminate

    UInt iLifeTime;          // m_life
    UInt iMaxLifeTime;       // m_maxlife
    Vector3 vRelativePosA;   //
    Vector3 vRelativePosB;   // m_rpos[2]
    Vector3 vNormal;         // m_normal
    Scalar fFriction;        // m_friction
};

/////////////////////////////////////////////////////////////////////////////////
// Misc stuff
typedef Array<SoftBody*> SoftBodyArray; // tSoftBodyArray
typedef Array<DynamicBVNode*> DBVLeafArray; // tLeafArray

typedef Scalar (*ImplicitFn)( const Vertex3 & vPosition, Void * pUserData );

class Impulse { public:
    inline Impulse operator-() const;
    inline Impulse & operator*=( Scalar rhs );

    Vector3 vVelocity; // m_velocity
    Vector3 vDrift;    // m_drift
    Bool bUseVelocity; // m_asVelocity (def = false)
    Bool bUseDrift;    // m_asDrift (def = false)
};

class Body
{
public:
    Body();
    Body( RigidBody * pRigid );
    Body( Cluster * pSoft );
    ~Body();

    inline RigidBody * GetRigidBody() const;
    inline Cluster * GetSoftCluster() const;

    inline Scalar GetInvMass() const;
    inline const Matrix3 & GetInvInertiaTensor() const;

    inline const Transform3 & GetTransform() const; // xform

    inline const Vector3 & GetLinearSpeed() const;
    inline const Vector3 & GetAngularSpeed() const;
    inline Vector3 GetLinearSpeedAt( const Vector3 & vLocalPoint ) const; // velocity
    inline Vector3 GetAngularSpeedAt( const Vector3 & vLocalPoint ) const; // angularVelocity

    inline Void ApplyLinearImpulseV( const Vector3 & vImpulse );
    inline Void ApplyLinearImpulseD( const Vector3 & vImpulse ); // applyDCImpulse
    inline Void ApplyLinearImpulse( const Impulse & vImpulse );

    inline Void ApplyAngularImpulseV( const Vector3 & vImpulse ); // applyVAImpulse
    inline Void ApplyAngularImpulseD( const Vector3 & vImpulse ); // applyDAImpulse
    inline Void ApplyAngularImpulse( const Impulse & vImpulse ); // applyAImpulse

    inline Void ApplyImpulseV( const Vector3 & vImpulse, const Vector3 & vLocalPoint );
    inline Void ApplyImpulseD( const Vector3 & vImpulse, const Vector3 & vLocalPoint );
    inline Void ApplyImpulse( const Impulse & vImpulse, const Vector3 & vLocalPoint );

private:
    RigidBody * m_pRigid; // m_rigid
    Cluster * m_pSoft;    // m_soft
    // btCollisionObject * m_collisionObject;
};

/////////////////////////////////////////////////////////////////////////////////
// The SoftBody class
class SoftBody : public SystemBody
{
public: // Nested types

    class Cluster { public:
        Cluster();
        ~Cluster();

        Array<Node*> arrNodes;              // m_nodes
        Array<Scalar> arrMasses;            // m_masses

        Scalar fInvMass;                    // m_imass
        Vertex3 vCenterOfMass;              // m_com
        Matrix3 matLocalInvInertiaTensor;   // m_locii
        Matrix3 matInvInertiaTensor;        // m_invwi

        Array<Vector3> arrFrameReferences;  // m_framerefs
        Transform3 vFrameTransform;         // m_framexform

        Vector3 vLinearVelocity;            // m_lv
        Vector3 vAngularVelocity;           // m_av

        Vector3 vLinearImpulseV;            //
        Vector3 vAngularImpulseV;           // m_vimpulses[2]
        Vector3 vLinearImpulseD;            //
        Vector3 vAngularImpulseD;           // m_dimpulses[2]
        UInt iImpulseCountV;                // m_nvimpulses
        UInt iImpulseCountD;                // m_ndimpulses

        Scalar fNodeDamping;                // m_ndamping, (def = 0.0f)
        Scalar fLinearDamping;              // m_ldamping, (def = 0.0f)
        Scalar fAngularDamping;             // m_adamping, (def = 0.0f)

        Scalar fMatching;                   // m_matching, (def = 0.0f)

        Scalar fSelfCollisionImpulseFactor; // m_selfCollisionImpulseFactor (def = 0.01f)
        Scalar fSelfCollisionImpulseMax;    // m_maxSelfCollisionImpulse (def = 100.0f)

        Bool bHasAnchor;                    // m_containsAnchor (def = false)
        Bool bCollide;                      // m_collide
        UInt iIndex;                        // m_clusterIndex
        DynamicBVNode * pDBVLeaf;           // m_leaf
    };

public:
    inline Void SetMass( UInt iNode, Scalar fMass );
    Void SetMass( Scalar fMass, Bool bUseAreas = false ); // setTotalMass(mass,fromfaces)
    Void SetVolumeMass( Scalar fMass );

    inline Void SetDensity( Scalar fDensity ); // setTotalDensity
    Void SetVolumeDensity( Scalar fDensity );

    // Nodes access
    Node * PushNode( const Vertex3 & vPosition, Scalar fMass ); // appendNode( x, m )

    // Links access
    inline Bool HasLink( UInt iNodeA, UInt iNodeB ) const;          // checkLink
    Bool HasLink( const Node * pNodeA, const Node * pNodeB ) const; // checkLink

    inline Link * PushLink( UInt iModel );                                           // appendLink(model,mat)
    inline Link * PushLink( UInt iNodeA, UInt iNodeB, Material * pMaterial = NULL ); // appendLink
    Link * PushLink( Node * pNodeA, Node * pNodeB, Material * pMaterial = NULL );    // appendLink

    inline Bool CutLink( UInt iNodeA, UInt iNodeB, Scalar fPosition );
    Bool CutLink( const Node * pNodeA, const Node * pNodeB, Scalar fPosition );

    UInt GenerateBridgeLinks( UInt iBridgeSize, Material * pMaterial = NULL );

    // Areas access
    inline Bool HasArea( UInt iNodeA, UInt iNodeB, UInt iNodeC ) const;                  // checkFace
    Bool HasArea( const Node * pNodeA, const Node * pNodeB, const Node * pNodeC ) const; // checkFace

    inline Area * PushArea( UInt iModel );                                                        // appendFace(model,mat)
    inline Area * PushArea( UInt iNodeA, UInt iNodeB, UInt iNodeC, Material * pMaterial = NULL ); // appendFace
    Area * PushArea( Node * pNodeA, Node * pNodeB, Node * pNodeC, Material * pMaterial = NULL );  // appendFace

    // Volumes access
    inline Bool HasVolume( UInt iNodeA, UInt iNodeB, UInt iNodeC, UInt iNodeD ) const;
    Bool HasVolume( const Node * pNodeA, const Node * pNodeB, const Node * pNodeC, const Node * pNodeD ) const;

    inline Volume * PushVolume( UInt iModel );                                                                      // appendTetra(model,mat)
    inline Volume * PushVolume( UInt iNodeA, UInt iNodeB, UInt iNodeC, UInt iNodeD, Material * pMaterial = NULL );  // appendTetra
    Volume * PushVolume( Node * pNodeA, Node * pNodeB, Node * pNodeC, Node * pNodeD, Material * pMaterial = NULL ); // appendTetra

    // Anchors access
    Anchor * PushAnchor( UInt iNode, RigidBody * pBody ); // appendAnchor(node,body,bNoCollWhenLinked = false)

    // Clusters access
    inline UInt GetClusterCount() const; // clusterCount

    inline Vertex3 GetClusterCOM( UInt iCluster ) const;     // clusterCom
    Vertex3 GetClusterCOM( const Cluster * pCluster ) const; // clusterCom

    inline Vector3 GetClusterVelocityAt( const Cluster * pCluster, const Vector3 & vLocalPoint ) const; // clusterVelocity

    inline static Void ApplyClusterLinearImpulseV( Cluster * pCluster, const Vector3 & vImpulse );
    inline static Void ApplyClusterLinearImpulseD( Cluster * pCluster, const Vector3 & vImpulse ); // clusterDCImpulse
    inline static Void ApplyClusterLinearImpulse( Cluster * pCluster, const Impulse & vImpulse );

    inline static Void ApplyClusterAngularImpulseV( Cluster * pCluster, const Vector3 & vImpulse ); // clusterVAImpulse
    inline static Void ApplyClusterAngularImpulseD( Cluster * pCluster, const Vector3 & vImpulse ); // clusterDAImpulse
    inline static Void ApplyClusterAngularImpulse( Cluster * pCluster, const Impulse & vImpulse ); // clusterAImpulse

    inline static Void ApplyClusterImpulseV( Cluster * pCluster, const Vector3 & vImpulse, const Vector3 & vLocalPoint ); // clusterVImpulse
    inline static Void ApplyClusterImpulseD( Cluster * pCluster, const Vector3 & vImpulse, const Vector3 & vLocalPoint ); // clusterDImpulse
    inline static Void ApplyClusterImpulse( Cluster * pCluster, const Impulse & vImpulse, const Vector3 & vLocalPoint );  // clusterImpulse

    UInt GenerateClusters( UInt iMeanK, UInt iMaxIterations = 8192 );
    Void DestroyCluster( UInt iCluster ); // releaseCluster
    Void DestroyAllClusters();            // releaseClusters

    Void Refine( Scalar fAccuracy, Bool bAllowCut, ImplicitFn pfRefineFunction, Void * pUserData = NULL ); // refine

    // Joints access
    inline JointLinear * PushJoint( const JointLinearParameters & jointParams, const Body & rBody );             // appendLinearJoint
    inline JointLinear * PushJoint( const JointLinearParameters & jointParams, SoftBody * pBody );               // appendLinearJoint
    JointLinear * PushJoint( const JointLinearParameters & jointParams, Cluster * pBodyA, const Body & rBodyB ); // appendLinearJoint

    JointAngular * PushJoint( const JointAngularParameters & jointParams, Cluster * pBodyA, const Body & rBodyB ); // appendAngularJoint
    JointAngular * PushJoint( const JointAngularParameters & jointParams, const Body & rBody );                    // appendAngularJoint
    JointAngular * PushJoint( const JointAngularParameters & jointParams, SoftBody * pBody );                      // appendAngularJoint

    // State access
    Void Translate( const Vector3 & vTranslate );
    Void Rotate( const Quaternion & qRotate );
    Void Scale( const Vector3 & vScale );
    Void Transform( const Transform3 & vTransform );

    // Force & Velocity access
    inline Void ApplyForce( UInt iNode, const Vector3 & vForce ); // addForce(force,node)
    inline Void ApplyForce( const Vector3 & vForce );             // addForce

    inline Void ApplyVelocity( UInt iNode, const Vector3 & vVelocity ); // addVelocity(velocity,node)
    inline Void SetVelocity( UInt iNode, const Vector3 & vVelocity );
    inline Void ApplyVelocity( const Vector3 & vVelocity );             // addVelocity
    inline Void SetVelocity( const Vector3 & vVelocity );               // setVelocity

    // Solver access
    Void RandomizeConstraints();
    static Void SolveClusters( SoftBodyArray & arrBodies );

    Void SolveStatic( UInt iIterations );

private:
    // Helpers
    static UInt MatchEdge( const Node * pNodeA, const Node * pNodeB, const Node * pRefNodeA, const Node * pRefNodeB ) { return INVALID_OFFSET; }
    static Scalar ClusterMetric( const Vertex3 & vCenter, const Vertex3 & vPosition ) { return 0.0f; }
    static Scalar ImplicitSolve( ImplicitFn pfFunction, const Vertex3 & vPosA, const Vertex3 & vPosB,
                                 Scalar fZeroTolerance = SCALAR_ERROR ) { return 0.0f;}
    static Matrix3 ImpulseMatrix( Scalar fNodeInvMass, Scalar fBodyInvMass, const Matrix3 & matBodyInvInertiaTensor,
                                  const Vector3 & vRelativeAnchor, Scalar fDT )
    { return Matrix3::Identity; }
    static Matrix3 ImpulseMatrix( Scalar fInvMassA, const Matrix3 & matInvInertiaTensorA, const Vector3 & vRelativeAnchorA,
                                  Scalar fInvMassB, const Matrix3 & matInvInertiaTensorB, const Vector3 & vRelativeAnchorB )
    { return Matrix3::Identity; }
    static Matrix3 AngularImpulseMatrix( const Matrix3 & matInvInertiaTensorA, const Matrix3 & matInvInertiaTensorB )
    { return Matrix3::Identity; }

    static Vertex3 BaryEval( const Vertex3 & vA, const Vertex3 & vB, const Vertex3 & vC, const Vector3 & vWeights )
    { return Vertex3::Null; }


    static Vector3 ClampVector( const Vector3 & vValue, Scalar fMaxNorm )
    { return Vector3::Null; }

    // Helpers
    Bool _CheckContact( ContactInfos * outInfos, RigidBody * pBody, const Vertex3 & vPosition, Scalar fZeroTolerance ) const;

    // Update support
    Void _Update_Clusters();

    Bool m_bUpdateRuntimeConstants; // m_bUpdateRtCst

    // Clusters support (piece-wise convex collisions)
    Void _Clusters_Initialize();

    Void _Clusters_Apply( Bool bDrift );
    Void _Clusters_Damping();

    Void _Clusters_Prepare( UInt iIterations );
    Void _Clusters_Solve( Scalar fSOR );
    Void _Clusters_Cleanup();

    UInt m_iMaxIterationsCluster; // m_cfg.citerations

    // Shape
    SoftBodyCollisionFlags m_iCollisionFlags; // m_cfg.collisions

    // State
    Transform3 m_vInitialTransform;

    Scalar m_fTimeScale;  // m_cfg.timescale
    Scalar m_fTimeBuffer; // m_timeacc

    // SoftBody structure
    typedef Array<Volume> VolumeArray; // tTetraArray
    VolumeArray m_arrVolumes;          // m_tetras

    typedef Array<Joint*> JointArray; // tJointArray
    JointArray m_arrJoints;           // m_joints

    typedef Array<Cluster*> ClusterArray; // tClusterArray
    ClusterArray m_arrClusters;           // m_clusters

    DynamicBVTree m_dbvClusterTree;
    Array<Bool> m_arrClusterConnectivity;

    // SoftBody properties
    //Scalar m_fRigidHardness;       // m_cfg.kCHR, [0,1]
    //Scalar m_fKineticHardness;     // m_cfg.kKHR, [0,1]
    //Scalar m_fSoftHardness;        // m_cfg.kSHR, [0,1]
    //Scalar m_fAnchorHardness;      // m_cfg.kAHR, [0,1]
    //Scalar m_fSoftRigidHardness;   // m_cfg.kSRHR_CL, [0,1] (cluster only)
    //Scalar m_fSoftKineticHardness; // m_cfg.kSKHR_CL, [0,1] (cluster only)
    //Scalar m_fSoftSoftHardness;    // m_cfg.kSSHR_CL, [0,1] (cluster only)
    //Scalar m_fSoftRigidSplit;      // m_cfg.kSR_SPLT_CL, [0,1] (cluster only)
    //Scalar m_fSoftKineticSplit;    // m_cfg.kSK_SPLT_CL, [0,1] (cluster only)
    //Scalar m_fSoftSoftSplit;       // m_cfg.kSS_SPLT_CL, [0,1] (cluster only)
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SoftBody.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_PHYSICS_BODIES_SYSTEMS_SOFTBODY_H
*/

//02843 
//02844 //
//02845 void                    btSoftBody::defaultCollisionHandler(btCollisionObject* pco)
//02846 {
//02847         switch(m_cfg.collisions&fCollision::RVSmask)
//02848         {
//02849         case    fCollision::SDF_RS:
//02850                 {
//02851                         btSoftColliders::CollideSDF_RS  docollide;              
//02852                         btRigidBody*            prb1=btRigidBody::upcast(pco);
//02853                         btTransform     wtr=prb1 ? prb1->getInterpolationWorldTransform() : pco->getWorldTransform();
//02854 
//02855                         const btTransform       ctr=pco->getWorldTransform();
//02856                         const btScalar          timemargin=(wtr.getOrigin()-ctr.getOrigin()).length();
//02857                         const btScalar          basemargin=getCollisionShape()->getMargin();
//02858                         btVector3                       mins;
//02859                         btVector3                       maxs;
//02860                         ATTRIBUTE_ALIGNED16(btDbvtVolume)               volume;
//02861                         pco->getCollisionShape()->getAabb(      pco->getInterpolationWorldTransform(),
//02862                                 mins,
//02863                                 maxs);
//02864                         volume=btDbvtVolume::FromMM(mins,maxs);
//02865                         volume.Expand(btVector3(basemargin,basemargin,basemargin));             
//02866                         docollide.psb           =       this;
//02867                         docollide.m_colObj1 = pco;
//02868                         docollide.m_rigidBody = prb1;
//02869 
//02870                         docollide.dynmargin     =       basemargin+timemargin;
//02871                         docollide.stamargin     =       basemargin;
//02872                         m_ndbvt.collideTV(m_ndbvt.m_root,volume,docollide);
//02873                 }
//02874                 break;
//02875         case    fCollision::CL_RS:
//02876                 {
//02877                         btSoftColliders::CollideCL_RS   collider;
//02878                         collider.Process(this,pco);
//02879                 }
//02880                 break;
//02881         }
//02882 }
//02883 
//02884 //
//02885 void                    btSoftBody::defaultCollisionHandler(btSoftBody* psb)
//02886 {
//02887         const int cf=m_cfg.collisions&psb->m_cfg.collisions;
//02888         switch(cf&fCollision::SVSmask)
//02889         {
//02890         case    fCollision::CL_SS:
//02891                 {
//02892                         
//02893                         //support self-collision if CL_SELF flag set
//02894                         if (this!=psb || psb->m_cfg.collisions&fCollision::CL_SELF)
//02895                         {
//02896                                 btSoftColliders::CollideCL_SS   docollide;
//02897                                 docollide.Process(this,psb);
//02898                         }
//02899                         
//02900                 }
//02901                 break;
//02902         case    fCollision::VF_SS:
//02903                 {
//02904                         //only self-collision for Cluster, not Vertex-Face yet
//02905                         if (this!=psb)
//02906                         {
//02907                                 btSoftColliders::CollideVF_SS   docollide;
//02908                                 /* common                                       */ 
//02909                                 docollide.mrg=  getCollisionShape()->getMargin()+
//02910                                         psb->getCollisionShape()->getMargin();
//02911                                 /* psb0 nodes vs psb1 faces     */ 
//02912                                 docollide.psb[0]=this;
//02913                                 docollide.psb[1]=psb;
//02914                                 docollide.psb[0]->m_ndbvt.collideTT(    docollide.psb[0]->m_ndbvt.m_root,
//02915                                         docollide.psb[1]->m_fdbvt.m_root,
//02916                                         docollide);
//02917                                 /* psb1 nodes vs psb0 faces     */ 
//02918                                 docollide.psb[0]=psb;
//02919                                 docollide.psb[1]=this;
//02920                                 docollide.psb[0]->m_ndbvt.collideTT(    docollide.psb[0]->m_ndbvt.m_root,
//02921                                         docollide.psb[1]->m_fdbvt.m_root,
//02922                                         docollide);
//02923                         }
//02924                 }
//02925                 break;
//02926         default:
//02927                 {
//02928                         
//02929                 }
//02930         }
//02931 }


//01446 //
//01447 bool                    btSoftBody::rayTest(const btVector3& rayFrom,
//01448                                                                         const btVector3& rayTo,
//01449                                                                         sRayCast& results)
//01450 {
//01451         if(m_faces.size()&&m_fdbvt.empty()) 
//01452                 initializeFaceTree();
//01453 
//01454         results.body    =       this;
//01455         results.fraction = 1.f;
//01456         results.feature =       eFeature::None;
//01457         results.index   =       -1;
//01458 
//01459         return(rayTest(rayFrom,rayTo,results.fraction,results.feature,results.index,false)!=0);
//01460 }
//01461 


//01718 
//01726 //
//01727 btSoftBody::RayFromToCaster::RayFromToCaster(const btVector3& rayFrom,const btVector3& rayTo,btScalar mxt)
//01728 {
//01729         m_rayFrom = rayFrom;
//01730         m_rayNormalizedDirection = (rayTo-rayFrom);
//01731         m_rayTo = rayTo;
//01732         m_mint  =       mxt;
//01733         m_face  =       0;
//01734         m_tests =       0;
//01735 }
//01736 
//01737 //
//01738 void                            btSoftBody::RayFromToCaster::Process(const btDbvtNode* leaf)
//01739 {
//01740         btSoftBody::Face&       f=*(btSoftBody::Face*)leaf->data;
//01741         const btScalar          t=rayFromToTriangle(    m_rayFrom,m_rayTo,m_rayNormalizedDirection,
//01742                 f.m_n[0]->m_x,
//01743                 f.m_n[1]->m_x,
//01744                 f.m_n[2]->m_x,
//01745                 m_mint);
//01746         if((t>0)&&(t<m_mint)) 
//01747         { 
//01748                 m_mint=t;m_face=&f; 
//01749         }
//01750         ++m_tests;
//01751 }
//01752 
//01753 //
//01754 btScalar                        btSoftBody::RayFromToCaster::rayFromToTriangle( const btVector3& rayFrom,
//01755                                                                                                                                    const btVector3& rayTo,
//01756                                                                                                                                    const btVector3& rayNormalizedDirection,
//01757                                                                                                                                    const btVector3& a,
//01758                                                                                                                                    const btVector3& b,
//01759                                                                                                                                    const btVector3& c,
//01760                                                                                                                                    btScalar maxt)
//01761 {
//01762         static const btScalar   ceps=-SIMD_EPSILON*10;
//01763         static const btScalar   teps=SIMD_EPSILON*10;
//01764 
//01765         const btVector3                 n=btCross(b-a,c-a);
//01766         const btScalar                  d=btDot(a,n);
//01767         const btScalar                  den=btDot(rayNormalizedDirection,n);
//01768         if(!btFuzzyZero(den))
//01769         {
//01770                 const btScalar          num=btDot(rayFrom,n)-d;
//01771                 const btScalar          t=-num/den;
//01772                 if((t>teps)&&(t<maxt))
//01773                 {
//01774                         const btVector3 hit=rayFrom+rayNormalizedDirection*t;
//01775                         if(     (btDot(n,btCross(a-hit,b-hit))>ceps)    &&                      
//01776                                 (btDot(n,btCross(b-hit,c-hit))>ceps)    &&
//01777                                 (btDot(n,btCross(c-hit,a-hit))>ceps))
//01778                         {
//01779                                 return(t);
//01780                         }
//01781                 }
//01782         }
//01783         return(-1);
//01784 }
//01785 


//01873 //
//01874 int                                     btSoftBody::rayTest(const btVector3& rayFrom,const btVector3& rayTo,
//01875                                                                                 btScalar& mint,eFeature::_& feature,int& index,bool bcountonly) const
//01876 {
//01877         int     cnt=0;
//01878         if(bcountonly||m_fdbvt.empty())
//01879         {/* Full search */ 
//01880                 btVector3 dir = rayTo-rayFrom;
//01881                 dir.normalize();
//01882 
//01883                 for(int i=0,ni=m_faces.size();i<ni;++i)
//01884                 {
//01885                         const btSoftBody::Face& f=m_faces[i];
//01886 
//01887                         const btScalar                  t=RayFromToCaster::rayFromToTriangle(   rayFrom,rayTo,dir,
//01888                                 f.m_n[0]->m_x,
//01889                                 f.m_n[1]->m_x,
//01890                                 f.m_n[2]->m_x,
//01891                                 mint);
//01892                         if(t>0)
//01893                         {
//01894                                 ++cnt;
//01895                                 if(!bcountonly)
//01896                                 {
//01897                                         feature=btSoftBody::eFeature::Face;
//01898                                         index=i;
//01899                                         mint=t;
//01900                                 }
//01901                         }
//01902                 }
//01903         }
//01904         else
//01905         {/* Use dbvt    */ 
//01906                 RayFromToCaster collider(rayFrom,rayTo,mint);
//01907 
//01908                 btDbvt::rayTest(m_fdbvt.m_root,rayFrom,rayTo,collider);
//01909                 if(collider.m_face)
//01910                 {
//01911                         mint=collider.m_mint;
//01912                         feature=btSoftBody::eFeature::Face;
//01913                         index=(int)(collider.m_face-&m_faces[0]);
//01914                         cnt=1;
//01915                 }
//01916         }
//01917         return(cnt);
//01918 }

