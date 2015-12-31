/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/Systems/SoftBody.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : System : SoftBody, mass-spring dynamics
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "SoftBody.h"

#include "../../PhysicsManager.h"
/*
/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Link implementation
SoftBody::Link::Link( Node * pInputNodeA, Node * pInputNodeB ):
    vGradient()
{
    // Ensure unicity
    if ( pInputNodeA < pInputNodeB ) {
        pNodeA = pInputNodeA;
        pNodeB = pInputNodeB;
    } else {
        pNodeA = pInputNodeB;
        pNodeB = pInputNodeA;
    }

    fRestLength = 0.0f;
    fSqrRestLength = 0.0f;
    fLinearFactor = 0.0f;
    vGradient = Vector3::Null;
    fGradientFactor = 0.0f;
    bIsBridge = false;
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Area implementation
SoftBody::Area::Area( Node * pInputNodeA, Node * pInputNodeB, Node * pInputNodeC ):
    vNormal()
{
    // Ensure unicity
    if ( pInputNodeA < pInputNodeB ) {
        if ( pInputNodeA < pInputNodeC ) {
            if ( pInputNodeB < pInputNodeC ) {
                pNodeA = pInputNodeA;
                pNodeB = pInputNodeB;
                pNodeC = pInputNodeC;
            } else {
                pNodeA = pInputNodeA;
                pNodeB = pInputNodeC;
                pNodeC = pInputNodeB;
            }
        } else {
            pNodeA = pInputNodeC;
            pNodeB = pInputNodeA;
            pNodeC = pInputNodeB;
        }
    } else {
        if ( pInputNodeB < pInputNodeC ) {
            if ( pInputNodeA < pInputNodeC ) {
                pNodeA = pInputNodeB;
                pNodeB = pInputNodeA;
                pNodeC = pInputNodeC;
            } else {
                pNodeA = pInputNodeB;
                pNodeB = pInputNodeC;
                pNodeC = pInputNodeA;
            }
        } else {
            pNodeA = pInputNodeC;
            pNodeB = pInputNodeB;
            pNodeC = pInputNodeA;
        }
    }

    fRestArea = 0.0f;
    vNormal = Vector3::Null;
    //m_leaf = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Volume implementation
Int _Volume_NodeSort( SoftBody::Node * const & rLeft, SoftBody::Node * const & rRight, Void * pUserData ) {
    if ( rLeft < rRight )
        return +1;
    if ( rRight < rLeft )
        return -1;
    return 0;
}
SoftBody::Volume::Volume( Node * pInputNodeA, Node * pInputNodeB, Node * pInputNodeC, Node * pInputNodeD )
{
    // Ensure unicity
    Node * arrNodes[4] = { pInputNodeA, pInputNodeB, pInputNodeC, pInputNodeD };
    QuickSort<Node*>( arrNodes, 4, _Volume_NodeSort );
    pNodeA = arrNodes[0];
    pNodeB = arrNodes[1];
    pNodeC = arrNodes[2];
    pNodeD = arrNodes[3];

    fRestVolume = 0.0f;
    fVolumeFactor = 0.0f;
    arrGradients[0] = Vector3::Null;
    arrGradients[1] = Vector3::Null;
    arrGradients[2] = Vector3::Null;
    arrGradients[3] = Vector3::Null;
    fGradientFactor = 0.0f;
    //m_leaf = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Body implementation
SoftBody::Body::Body()
{
    m_pRigid = NULL;
    m_pSoft = NULL;
}
SoftBody::Body::Body( RigidBody * pRigid )
{
    m_pRigid = pRigid;
    m_pSoft = NULL;
}
SoftBody::Body::Body( Cluster * pSoft )
{
    m_pRigid = NULL;
    m_pSoft = pSoft;
}
SoftBody::Body::~Body()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Joint implementation
SoftBody::Joint::Joint()
{
    bDelete = false;
}
SoftBody::Joint::~Joint()
{
    // nothing to do
}

Void SoftBody::Joint::Prepare( Scalar fTimeStep, UInt iIterations )
{
    //jointBodyA.SetActive( true );
    //jointBodyB.SetActive( true );
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::JointLinear implementation
Void SoftBody::JointLinear::Prepare( Scalar fTimeStep, UInt iIterations )
{
    static const Scalar fMaxDrift = 4.0f;

    Joint::Prepare( fTimeStep, iIterations );

    vRelativePosA = ( jointBodyA.GetTransform() * vReferenceA );
    vRelativePosB = ( jointBodyB.GetTransform() * vReferenceB );

    vDrift = ClampVector( (vRelativePosA - vRelativePosB), fMaxDrift );
    vDrift *= ( fERP / fTimeStep );

    vRelativePosA -= jointBodyA.GetTransform().GetTranslate();
    vRelativePosB -= jointBodyB.GetTransform().GetTranslate();

    matMass = ImpulseMatrix( jointBodyA.GetInvMass(), jointBodyA.GetInvInertiaTensor(), vRelativePosA,
                             jointBodyB.GetInvMass(), jointBodyB.GetInvInertiaTensor(), vRelativePosB );

    if ( fSplit > 0.0f ) {
        vSplitDrift = ( matMass * (vDrift * fSplit) );
        vDrift *= ( 1.0f - fSplit );
    }
    vDrift *= MathFn->Invert( (Scalar)iIterations );
}
Void SoftBody::JointLinear::Solve( Scalar fTimeStep, Scalar fSOR )
{
    Vector3 vVelocityA = jointBodyA.GetLinearSpeedAt( vRelativePosA );
    Vector3 vVelocityB = jointBodyB.GetLinearSpeedAt( vRelativePosB );
    Vector3 vRelVelocity = ( vVelocityA - vVelocityB );

    Impulse vImpulse;
    vImpulse.bUseVelocity = true;
    vImpulse.vVelocity = matMass * ( (vDrift + (vRelVelocity * fCFM)) * fSOR );
    vImpulse.bUseDrift = false;
    vImpulse.vDrift = Vector3::Null;

    jointBodyA.ApplyImpulse( -vImpulse, vRelativePosA );
    jointBodyB.ApplyImpulse( vImpulse, vRelativePosB );
}
Void SoftBody::JointLinear::Finalize( Scalar fTimeStep )
{
    if ( fSplit > 0.0f ) {
        jointBodyA.ApplyImpulseD( -vSplitDrift, vRelativePosA );
        jointBodyB.ApplyImpulseD( vSplitDrift, vRelativePosB );
    }
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::JointAngular implementation
Void SoftBody::JointAngular::Prepare( Scalar fTimeStep, UInt iIterations )
{
    static const Scalar fMaxDrift = ( SCALAR_PI / 16.0f );

    //m_icontrol->Prepare( this );
    Joint::Prepare( fTimeStep, iIterations );

    vAxisA = jointBodyA.GetTransform() * vReferenceA;
    vAxisB = jointBodyB.GetTransform() * vReferenceB;

    vDrift = ( vAxisB ^ vAxisA );
    vDrift.Normalize();

    Scalar fDot = Clamp<Scalar>( (vAxisA * vAxisB) ,-1.0f , +1.0f );
    Scalar fArcCos = MathFn->ArcCos( fDot );
    vDrift *= Min<Scalar>( fArcCos, fMaxDrift );
    vDrift *= ( fERP / fTimeStep );

    matMass = AngularImpulseMatrix( jointBodyA.GetInvInertiaTensor(), jointBodyB.GetInvInertiaTensor() );

    if ( fSplit > 0.0f ) {
        vSplitDrift = ( matMass * (vDrift * fSplit) );
        vDrift *= ( 1.0f - fSplit );
    }
    vDrift *= MathFn->Invert( (Scalar)iIterations );
}
Void SoftBody::JointAngular::Solve( Scalar fTimeStep, Scalar fSOR )
{
    Vector3 vVelocityA = jointBodyA.GetAngularSpeed();
    Vector3 vVelocityB = jointBodyB.GetAngularSpeed();
    Vector3 vRelVelocity = ( vVelocityA - vVelocityB );

    //Scalar fDot = ( vRelVelocity * vAxisA );
    //vRelVelocity -= ( vAxisA * m_icontrol->Speed(this,fDot) );

    Impulse vImpulse;
    vImpulse.bUseVelocity = true;
    vImpulse.vVelocity = matMass * ( (vDrift + (vRelVelocity * fCFM)) * fSOR );
    vImpulse.bUseDrift = false;
    vImpulse.vDrift = Vector3::Null;

    jointBodyA.ApplyAngularImpulse( -vImpulse );
    jointBodyB.ApplyAngularImpulse( vImpulse );
}
Void SoftBody::JointAngular::Finalize( Scalar fTimeStep )
{
    if ( fSplit > 0.0f ) {
        jointBodyA.ApplyAngularImpulseD( -vSplitDrift );
        jointBodyB.ApplyAngularImpulseD( vSplitDrift );
    }
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::JointContact implementation
Void SoftBody::JointContact::Prepare( Scalar fTimeStep, UInt iIterations )
{
    Joint::Prepare( fTimeStep, iIterations );

    Bool bUseDrift = ( iLifeTime == 0 );
    bDelete = ( (++iLifeTime) > iMaxLifeTime );

    if ( bUseDrift ) {
        vDrift *= ( fERP / fTimeStep );
        if ( fSplit > 0.0f ) {
            vSplitDrift = ( matMass * (vDrift * fSplit) );
            vDrift *= ( 1.0f - fSplit );
        }
        vDrift *= MathFn->Invert( (Scalar)iIterations );
    } else {
        vDrift = Vector3::Null;
        vSplitDrift = Vector3::Null;
    }
}
Void SoftBody::JointContact::Solve( Scalar fTimeStep, Scalar fSOR )
{
    Vector3 vVelocityA = jointBodyA.GetLinearSpeedAt( vRelativePosA );
    Vector3 vVelocityB = jointBodyB.GetLinearSpeedAt( vRelativePosB );
    Vector3 vRelVelocity = ( vVelocityA - vVelocityB );

    Impulse vImpulse;
    vImpulse.bUseVelocity = true;
    vImpulse.vVelocity = vDrift;
    vImpulse.bUseDrift = false;
    vImpulse.vDrift = Vector3::Null;

    Scalar fDot = ( vRelVelocity * vNormal );
    if ( fDot < 0.0f ) {
        Vector3 vI = ( vNormal * fDot );
        Vector3 vF = ( vRelVelocity - vI );
        vImpulse.vVelocity += ( vI + (vF * fFriction) );
    }
    vImpulse.vVelocity = matMass * ( vImpulse.vVelocity * fSOR );

    if ( jointBodyA.GetSoftCluster() == jointBodyB.GetSoftCluster() ) {
        if ( vImpulse.vVelocity.Norm() >= jointBodyA.GetSoftCluster()->fSelfCollisionImpulseMax ) {
            vImpulse *= jointBodyA.GetSoftCluster()->fSelfCollisionImpulseFactor;
            jointBodyA.ApplyImpulse( -vImpulse, vRelativePosA );
            jointBodyB.ApplyImpulse( vImpulse, vRelativePosB );
        }
    } else {
        jointBodyA.ApplyImpulse( -vImpulse, vRelativePosA );
        jointBodyB.ApplyImpulse( vImpulse, vRelativePosB );
    }
}
Void SoftBody::JointContact::Finalize( Scalar fTimeStep )
{
    if ( fSplit > 0.0f ) {
        jointBodyA.ApplyImpulseD( -vSplitDrift, vRelativePosA );
        jointBodyB.ApplyImpulseD( vSplitDrift, vRelativePosB );
    }
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody implementation
SoftBody::SoftBody( SoftBodyEnvironment * pEnvironment, Shape * pLocalShape,
                    const Vertex3 * arrPositions, Scalar * arrMasses, UInt iNodeCount ):
    SystemBody(),
    m_bvAABox(),
    m_vPosition(), m_qOrientation(), m_vTransform(),
    m_vLinearSpeed(), m_vAngularSpeed(), m_vInitialTransform(),
    m_arrMaterials(), m_arrNodes(), m_arrLinks(), m_arrAreas(), m_arrVolumes(),
    m_arrRigidContacts(), m_arrSoftContacts(), m_arrAnchors(), m_arrNotes(),
    m_arrClusters(), m_arrJoints(),
    m_dbvNodeTree(), m_dbvAreaTree(), m_dbvClusterTree()
{
    // Shape
    m_pLocalShape = pLocalShape;

    // State
    m_vPosition = Vertex3::Null;
    m_qOrientation = Quaternion::Identity;
    m_vTransform = Transform3::Identity;

    m_vLinearSpeed = Vector3::Null;
    m_vAngularSpeed = Vector3::Null;

    m_vInitialTransform = Transform3::Identity;

    // World shape
    PhysicsFn->SelectMemory( TEXT("Shapes") );
    if ( m_pLocalShape->IsConvex() )
        m_pWorldShape = New TransformedShapeConvex( (const ShapeConvex *)m_pLocalShape, m_vTransform );
    else
        m_pWorldShape = New TransformedShapeConcave( (const ShapeConcave *)m_pLocalShape, m_vTransform );
    PhysicsFn->UnSelectMemory();

    // SoftBody state
    m_bUpdateRuntimeConstants = true;
    m_fTimeBuffer = 0.0f;

    // Environment
    m_pEnvironment = pEnvironment;
    m_pUserData = NULL;

    // Configuration
    m_hConfig.iModel = SOFTBODY_AERODYNAMICS_V_Point;
    m_hConfig.fBaumgarte               = 1.0f;
    m_hConfig.fDamping                 = 0.0f;
    m_hConfig.fDrag                    = 0.0f;
    m_hConfig.fLift                    = 0.0f;
    m_hConfig.fPressure                = 0.0f;
    m_hConfig.fVolumeConversation      = 0.0f;
    m_hConfig.fDynamicFriction         = 0.2f;
    m_hConfig.fMatching                = 0.0f;
    m_hConfig.fRigidHardness           = 1.0f;
    m_hConfig.fKineticHardness         = 0.1f;
    m_hConfig.fSoftHardness            = 1.0f;
    m_hConfig.fAnchorHardness          = 0.7f;
    m_hConfig.fSoftRigidHardness       = 0.1f;
    m_hConfig.fSoftKineticHardness     = 1.0f;
    m_hConfig.fSoftSoftHardness        = 0.5f;
    m_hConfig.fSoftRigidSplit          = 0.5f;
    m_hConfig.fSoftKineticSplit        = 0.5f;
    m_hConfig.fSoftSoftSplit           = 0.5f;
    m_hConfig.fMaxVolumeRatio          = 1.0f;
    m_hConfig.fTimeScale               = 1.0f;
    m_hConfig.iMaxIterationsVelocities = 0;
    m_hConfig.iMaxIterationsPositions  = 1;
    m_hConfig.iMaxIterationsDrift      = 0;
    m_hConfig.iMaxIterationsCluster    = 4;
    m_hConfig.iCollisionFlags = SOFTBODY_COLLISION_SDF_RS;

    m_hConfig.arrSolverSequenceVelocities.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_hConfig.arrSolverSequenceVelocities.Create();
    m_hConfig.arrSolverSequencePositions.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_hConfig.arrSolverSequencePositions.Create();
    m_hConfig.arrSolverSequenceDrift.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_hConfig.arrSolverSequenceDrift.Create();

    // Pose
    m_hPose.bIsVolume      = false;
    m_hPose.bIsFrame       = false;
    m_hPose.fRestVolume    = 0.0f;
    m_hPose.vCenterOfMass  = Vertex3::Null;
    m_hPose.matRotation    = Matrix3::Identity;
    m_hPose.matBaseScaling = Matrix3::Identity;
    m_hPose.matScaling     = Matrix3::Identity;

    m_hPose.arrPositions.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_hPose.arrPositions.Create();
    m_hPose.arrWeights.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_hPose.arrWeights.Create();

    // SoftBody sructure
    m_arrMaterials.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrMaterials.Create();
    m_arrNodes.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrNodes.Create();
    m_arrLinks.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrLinks.Create();
    m_arrAreas.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrAreas.Create();
    m_arrVolumes.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrVolumes.Create();
    m_arrRigidContacts.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrRigidContacts.Create();
    m_arrSoftContacts.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrSoftContacts.Create();
    m_arrAnchors.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrAnchors.Create();
    m_arrNotes.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrNotes.Create();
    m_arrClusters.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrClusters.Create();
    m_arrJoints.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_arrJoints.Create();

    m_dbvNodeTree.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_dbvNodeTree.Create();
    m_dbvAreaTree.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_dbvAreaTree.Create();
    m_dbvClusterTree.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
    m_dbvClusterTree.Create();

    // Default material
    Material * pMaterial = PushMaterial();
    pMaterial->fLST = 1.0f;
    pMaterial->fAST = 1.0f;
    pMaterial->fVST = 1.0f;
    pMaterial->dwFlags = 0;

    // Input nodes
    Node newNode;
    for( UInt i = 0; i < iNodeCount; ++i ) {
        newNode.pUserData = NULL;
        newNode.pMaterial = pMaterial;

        newNode.vPosition = arrPositions[i];
        newNode.vOldPosition = newNode.vPosition;
        newNode.vVelocity = Vector3::Null;
        newNode.vForceBuffer = Vector3::Null;
        newNode.vNormal = Vector3::Null;
        newNode.fMass = arrMasses[i];
        newNode.fInvMass = MathFn->Invert( arrMasses[i] );
        newNode.fArea = 0.0f;
        newNode.bAttached = false;

        //newNode.m_leaf = m_ndbvt.insert( btDbvtVolume::FromCR(newNode.vPosition,margin), &newNode );

        m_arrNodes.Push( newNode );
    }

    // Update
    _Update_Bounds();

    // Initialize
    SetSolver( SOFTBODY_SOLVER_POSITIONS );
}
SoftBody::~SoftBody()
{
    // SoftBody sructure
    UInt iMaterialCount = m_arrMaterials.Count();
    UInt iJointCount = m_arrJoints.Count();
    UInt i;

    DestroyAllClusters();

    PhysicsFn->SelectMemory( TEXT("SoftBodies") );
    for( i = 0; i < iJointCount; ++i ) {
        Delete( m_arrJoints[i] );
        m_arrJoints[i] = NULL;
    }
    for( i = 0; i < iMaterialCount; ++i ) {
        Delete( m_arrMaterials[i] );
        m_arrMaterials[i] = NULL;
    }
    PhysicsFn->UnSelectMemory();

    m_dbvClusterTree.Destroy();
    m_dbvAreaTree.Destroy();
    m_dbvNodeTree.Destroy();

    m_arrJoints.Destroy();
    m_arrClusters.Destroy();
    m_arrNotes.Destroy();
    m_arrAnchors.Destroy();
    m_arrSoftContacts.Destroy();
    m_arrRigidContacts.Destroy();
    m_arrVolumes.Destroy();
    m_arrAreas.Destroy();
    m_arrLinks.Destroy();
    m_arrNodes.Destroy();
    m_arrMaterials.Destroy();

    // Pose
    m_hPose.arrWeights.Destroy();
    m_hPose.arrPositions.Destroy();

    // Config
    m_hConfig.arrSolverSequenceDrift.Destroy();
    m_hConfig.arrSolverSequencePositions.Destroy();
    m_hConfig.arrSolverSequenceVelocities.Destroy();
}


Void SoftBody::SetMass( Scalar fMass, Bool bUseAreas )
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt i;

    // Compute node masses from areas
    if ( bUseAreas ) {
        for( i = 0; i < iNodeCount; ++i )
            m_arrNodes[i].fMass = 0.0f;

        UInt iAreaCount = m_arrAreas.Count();
        for( i = 0; i < iAreaCount; ++i ) {
            const Area & curArea = m_arrAreas[i];

            curArea.pNodeA->fMass += curArea.fRestArea;
            curArea.pNodeB->fMass += curArea.fRestArea;
            curArea.pNodeC->fMass += curArea.fRestArea;
        }
    }

    // Compute total mass
    Scalar fTotalMass = 0.0f;
    for( i = 0; i < iNodeCount; ++i )
        fTotalMass += m_arrNodes[i].fMass;

    // Update node masses
    Scalar fMassFactor = ( fMass / fTotalMass );
    for( i = 0; i < iNodeCount; ++i ) {
        m_arrNodes[i].fMass *= fMassFactor;
        m_arrNodes[i].fInvMass = MathFn->Invert( m_arrNodes[i].fMass );
    }

    // Update
    m_bUpdateRuntimeConstants = true;
}
Void SoftBody::SetVolumeMass( Scalar fMass )
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt i;

    // Create temp array
    PhysicsFn->SelectMemory( TEXT("Scratch") );
    Scalar * arrWeights = New Scalar[iNodeCount];
    PhysicsFn->UnSelectMemory();

    // Compute node masses from volumes
    for( i = 0; i < iNodeCount; ++i ) {
        arrWeights[i] = 0.0f;
        m_arrNodes[i].fMass = 0.0f;
    }

    UInt iVolumeCount = m_arrVolumes.Count();
    for( i = 0; i < iVolumeCount; ++i ) {
        const Volume & curVolume = m_arrVolumes[i];

        curVolume.pNodeA->fMass += curVolume.fRestVolume;
        curVolume.pNodeB->fMass += curVolume.fRestVolume;
        curVolume.pNodeC->fMass += curVolume.fRestVolume;
        curVolume.pNodeD->fMass += curVolume.fRestVolume;

        arrWeights[ curVolume.pNodeA - &(m_arrNodes[0]) ] += 1.0f;
        arrWeights[ curVolume.pNodeB - &(m_arrNodes[0]) ] += 1.0f;
        arrWeights[ curVolume.pNodeC - &(m_arrNodes[0]) ] += 1.0f;
        arrWeights[ curVolume.pNodeD - &(m_arrNodes[0]) ] += 1.0f;
    }

    for( i = 0; i < iNodeCount; ++i ) {
        m_arrNodes[i].fMass /= arrWeights[i];
        m_arrNodes[i].fInvMass = MathFn->Invert( m_arrNodes[i].fMass );
    }

    // Destroy temp array
    PhysicsFn->SelectMemory( TEXT("Scratch") );
    DeleteA( arrWeights );
    PhysicsFn->UnSelectMemory();

    // Set total mass
    SetMass( fMass, false );
}

Void SoftBody::SetVolumeDensity( Scalar fDensity )
{
    UInt iVolumeCount = m_arrVolumes.Count();
    Scalar fVolume = 0.0f;
    for( UInt i = 0; i < iVolumeCount; ++i ) {
        const Volume & curVolume = m_arrVolumes[i];
        fVolume += curVolume.fRestVolume;
    }
    fVolume *= ( 2.0f / 3.0f );

    // Set volume mass
    SetVolumeMass( fVolume * fDensity );
}

Scalar SoftBody::GetVolume() const
{
    if ( m_arrNodes.Count() == 0 )
        return 0.0f;

    UInt iAreaCount = m_arrAreas.Count();
    Vector3 vCross;
    Scalar fVolume = 0.0f;
    
    const Vertex3 & vOrigin = m_arrNodes[0].vPosition;
    for( UInt i = 0; i < iAreaCount; ++i ) {
        const Area & curArea = m_arrAreas[i];
        vCross = ( (curArea.pNodeB->vPosition - vOrigin) ^ (curArea.pNodeC->vPosition - vOrigin) );
        fVolume += ( (curArea.pNodeA->vPosition - vOrigin) * vCross );
    }
    fVolume /= 6.0f;

    return fVolume;
}

SoftBody::Material * SoftBody::PushMaterial()
{
    PhysicsFn->SelectMemory( TEXT("SoftBodies") );
    Material * pMaterial = New Material();
    PhysicsFn->UnSelectMemory();

    pMaterial->pUserData = NULL;

    pMaterial->fLST = 0.0f;
    pMaterial->fAST = 0.0f;
    pMaterial->fVST = 0.0f;
    pMaterial->dwFlags = 0;

    m_arrMaterials.Push( pMaterial );

    return pMaterial;
}

SoftBody::Node * SoftBody::PushNode( const Vertex3 & vPosition, Scalar fMass )
{
    Node newNode;

    newNode.pUserData = NULL;
    newNode.pMaterial = m_arrMaterials[0];

    newNode.vPosition = vPosition;
    newNode.vOldPosition = vPosition;
    newNode.vVelocity = Vector3::Null;
    newNode.vForceBuffer = Vector3::Null;
    newNode.vNormal = Vector3::Null;
    newNode.fMass = fMass;
    newNode.fInvMass = MathFn->Invert( fMass );
    newNode.fArea = 0.0f;
    newNode.bAttached = false;

    //newNode.m_leaf = m_ndbvt.insert( btDbvtVolume::FromCR(newNode.vPosition, margin), &newNode );

    m_arrNodes.Push( newNode );

    return &( m_arrNodes[m_arrNodes.Count() - 1] );
}

Bool SoftBody::HasLink( const Node * pNodeA, const Node * pNodeB ) const
{
    UInt iLinkCount = m_arrLinks.Count();
    for( UInt i = 0; i < iLinkCount; ++i ) {
        const Link & curLink = m_arrLinks[i];
        if ( curLink.pNodeA == pNodeA && curLink.pNodeB == pNodeB )
            return true;
    }
    return false;
}
SoftBody::Link * SoftBody::PushLink( Node * pNodeA, Node * pNodeB, Material * pMaterial )
{
    Link newLink( pNodeA, pNodeB );

    newLink.pUserData = NULL;
    newLink.pMaterial = (pMaterial == NULL) ? m_arrMaterials[0] : pMaterial;

    newLink.fRestLength = ( pNodeB->vPosition - pNodeA->vPosition ).Norm();
    newLink.bIsBridge = false;

    m_bUpdateRuntimeConstants = true;
    //newLink.fSqrRestLength = ; // m_c1
    //newLink.fLinearFactor = ; // m_c0, ( fLST * (1/m0 + 1/m1) )
    //newLink.vGradient = ; // m_c3
    //newLink.fGradientFactor = ; // m_c2, ( |vGradient|^2 / fLinearFactor )

    m_arrLinks.Push( newLink );

    return &( m_arrLinks[m_arrLinks.Count() - 1] );
}

Bool SoftBody::CutLink( const Node * pNodeA, const Node * pNodeB, Scalar fPosition )
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt iLinkCount = m_arrLinks.Count();
    UInt iAreaCount = m_arrAreas.Count();
    UInt i, iFound;

    UInt iA = ( pNodeA - &( m_arrNodes[0] ) );
    UInt iB = ( pNodeB - &( m_arrNodes[0] ) );

    Bool bDone = false;

    Vector3 vAB = ( pNodeB->vPosition - pNodeA->vPosition );
    Scalar fMass = 1.0f;

    Vertex3 vLerpPos = pNodeA->vPosition * (1.0f - fPosition);
    vLerpPos += ( pNodeB->vPosition.ToVector() * fPosition );
    Vector3 vLerpVelocity = pNodeA->vVelocity * (1.0f - fPosition);
    vLerpVelocity += pNodeB->vVelocity * fPosition;

    Node * arrLastNodes[2] = {
        PushNode( vLerpPos, fMass ),
        PushNode( vLerpPos, fMass )
    };

    arrLastNodes[0]->vVelocity = vLerpVelocity;
    arrLastNodes[1]->vVelocity = vLerpVelocity;

    Link * pModelLink;
    Link * pNewLink;
    Area * pModelArea;
    Area * pNewArea;

    for( i = 0; i < iLinkCount; ++i ) {
        iFound = MatchEdge( m_arrLinks[i].pNodeA, m_arrLinks[i].pNodeB, pNodeA, pNodeB );
        if ( iFound != INVALID_OFFSET ) {
            pModelLink = &( m_arrLinks[i] );
            pNewLink = PushLink( i );
            pModelLink->pNodeB = arrLastNodes[iFound];
            pNewLink->pNodeA = arrLastNodes[1 - iFound];
            bDone = true;
        }
    }
    for( i = 0; i < iAreaCount; ++i ) {
        iFound = MatchEdge( m_arrAreas[i].pNodeA, m_arrAreas[i].pNodeB, pNodeA, pNodeB );
        if ( iFound != INVALID_OFFSET ) {
            pModelArea = &( m_arrAreas[i] );
            pNewArea = PushArea( i );
            pModelArea->pNodeB = arrLastNodes[iFound];
            pNewArea->pNodeA = arrLastNodes[1 - iFound];
            PushLink( arrLastNodes[0], pModelArea->pNodeC, pModelArea->pMaterial );
            PushLink( arrLastNodes[1], pModelArea->pNodeC, pModelArea->pMaterial );
        }

        iFound = MatchEdge( m_arrAreas[i].pNodeB, m_arrAreas[i].pNodeC, pNodeA, pNodeB );
        if ( iFound != INVALID_OFFSET ) {
            pModelArea = &( m_arrAreas[i] );
            pNewArea = PushArea( i );
            pModelArea->pNodeC = arrLastNodes[iFound];
            pNewArea->pNodeB = arrLastNodes[1 - iFound];
            PushLink( arrLastNodes[0], pModelArea->pNodeA, pModelArea->pMaterial );
            PushLink( arrLastNodes[1], pModelArea->pNodeA, pModelArea->pMaterial );
        }

        iFound = MatchEdge( m_arrAreas[i].pNodeC, m_arrAreas[i].pNodeA, pNodeA, pNodeB );
        if ( iFound != INVALID_OFFSET ) {
            pModelArea = &( m_arrAreas[i] );
            pNewArea = PushArea( i );
            pModelArea->pNodeA = arrLastNodes[iFound];
            pNewArea->pNodeC = arrLastNodes[1 - iFound];
            PushLink( arrLastNodes[0], pModelArea->pNodeB, pModelArea->pMaterial );
            PushLink( arrLastNodes[1], pModelArea->pNodeB, pModelArea->pMaterial );
        }
    }

    if ( !bDone ) {
        //m_ndbvt.remove( pNodeNm2->m_leaf );
        //m_ndbvt.remove( pNodeNm1->m_leaf );
        m_arrNodes.Pop( NULL, 2 );
    }
    return bDone;
}

UInt SoftBody::GenerateBridgeLinks( UInt iBridgeSize, Material * pMaterial )
{
    if ( iBridgeSize < 2 )
        return 0;

    UInt iNodeCount = m_arrNodes.Count();
    UInt iLinkCount = m_arrLinks.Count();
    UInt i, j, k, iA, iB, iSum;

    // Create distance map
    PhysicsFn->SelectMemory( TEXT("Scratch") );
    UInt * arrDistanceMap = New UInt[iNodeCount * iNodeCount];
    PhysicsFn->UnSelectMemory();

    // Generate distance map
    for( j = 0; j < iNodeCount; ++j ) {
        for( i = 0; i < iNodeCount; ++i ) {
            if ( i != j )
                arrDistanceMap[j * iNodeCount + i] = UINT_MAX;
            else
                arrDistanceMap[j * iNodeCount + i] = 0;
        }
    }
    for( i = 0; i < iLinkCount; ++i ) {
        const Link & curLink = m_arrLinks[i];
        iA = ( curLink.pNodeA - &(m_arrNodes[0]) );
        iB = ( curLink.pNodeB - &(m_arrNodes[0]) );
        arrDistanceMap[iA * iNodeCount + iB] = 1;
        arrDistanceMap[iB * iNodeCount + iA] = 1;
    }
    for( k = 0; k < iNodeCount; ++k ) {
        for( j = 0; j < iNodeCount; ++j ) {
            for( i = j+1; i < iNodeCount; ++i ) {
                iSum = ( arrDistanceMap[k * iNodeCount + i] + arrDistanceMap[j * iNodeCount + k] );
                if ( iSum < arrDistanceMap[j * iNodeCount + i] ) {
                    arrDistanceMap[j * iNodeCount + i] = iSum;
                    arrDistanceMap[i * iNodeCount + j] = iSum;
                }
            }
        }
    }

    // Generate bridge links
    UInt iBridgeLinkCount = 0;
    Link * pLink;

    for( j = 0; j < iNodeCount; ++j ) {
        for( i = j+1; i < iNodeCount; ++i ) {
            if ( arrDistanceMap[j * iNodeCount + i] == iBridgeSize ) {
                pLink = PushLink( i, j, pMaterial );
                pLink->bIsBridge = true;
                ++iBridgeLinkCount;
            }
        }
    }

    // Destroy distance map
    PhysicsFn->SelectMemory( TEXT("Scratch") );
    DeleteA( arrDistanceMap );
    PhysicsFn->UnSelectMemory();

    return iBridgeLinkCount;
}

Bool SoftBody::HasArea( const Node * pNodeA, const Node * pNodeB, const Node * pNodeC ) const
{
    UInt iAreaCount = m_arrAreas.Count();
    for( UInt i = 0; i < iAreaCount; ++i ) {
        const Area & curArea = m_arrAreas[i];
        if ( curArea.pNodeA == pNodeA && curArea.pNodeB == pNodeB && curArea.pNodeC == pNodeC )
            return true;
    }
    return false;
}
SoftBody::Area * SoftBody::PushArea( Node * pNodeA, Node * pNodeB, Node * pNodeC, Material * pMaterial )
{
    Area newArea( pNodeA, pNodeB, pNodeC );

    newArea.pUserData = NULL;
    newArea.pMaterial = (pMaterial == NULL) ? m_arrMaterials[0] : pMaterial;

    Triangle3 triTmp( pNodeA->vPosition, pNodeB->vPosition, pNodeC->vPosition );
    newArea.fRestArea = 0.0f; //triTmp.Area();

    m_bUpdateRuntimeConstants = true;
    //newArea.vNormal = ; // m_normal
    //newArea.m_leaf = ; // Leaf data

    m_arrAreas.Push( newArea );

    return &( m_arrAreas[m_arrAreas.Count() - 1] );
}

Bool SoftBody::HasVolume( const Node * pNodeA, const Node * pNodeB, const Node * pNodeC, const Node * pNodeD ) const
{
    UInt iVolumeCount = m_arrVolumes.Count();
    for( UInt i = 0; i < iVolumeCount; ++i ) {
        const Volume & curVolume = m_arrVolumes[i];
        if ( curVolume.pNodeA == pNodeA && curVolume.pNodeB == pNodeB &&
             curVolume.pNodeC == pNodeC && curVolume.pNodeD == pNodeD )
            return true;
    }
    return false;
}
SoftBody::Volume * SoftBody::PushVolume( Node * pNodeA, Node * pNodeB, Node * pNodeC, Node * pNodeD, Material * pMaterial )
{
    Volume newVolume( pNodeA, pNodeB, pNodeC, pNodeD );

    newVolume.pUserData = NULL;
    newVolume.pMaterial = (pMaterial == NULL) ? m_arrMaterials[0] : pMaterial;

    //Tetrahedron tetraTmp( pNodeA->vPosition, pNodeB->vPosition, pNodeC->vPosition, pNodeD->vPosition );
    newVolume.fRestVolume = 0.0f; //tetraTmp.Volume();

    m_bUpdateRuntimeConstants = true;
    //newVolume.fVolumeFactor = ; // m_c1, ( (4 * fVST) / (1/m0 + 1/m1 + 1/m2 + 1/m3) )
    //newVolume.arrGradients[4] = ; // m_c0[4]
    //newVolume.fGradientFactor = ; // m_c2, ( fVolumeFactor / sum(|grad0..3|^2) )
    //newVolume.m_leaf = ; // Leaf data

    m_arrVolumes.Push( newVolume );

    return &( m_arrVolumes[m_arrVolumes.Count() - 1] );
}

SoftBody::Anchor * SoftBody::PushAnchor( UInt iNode, RigidBody * pBody )
{
    Assert( iNode < m_arrNodes.Count() );

    Anchor newAnchor;

    newAnchor.pNode = &( m_arrNodes[iNode] );
    newAnchor.pBody = pBody;

    Transform3 trInvWorld;
    pBody->GetTransform().Invert( trInvWorld );
    newAnchor.vLocalAnchor = ( trInvWorld * newAnchor.pNode->vPosition );

    newAnchor.pNode->bAttached = true;

    m_arrAnchors.Push( newAnchor );

    return &( m_arrAnchors[m_arrAnchors.Count() - 1] );
}

Vertex3 SoftBody::GetClusterCOM( const Cluster * pCluster ) const
{
    Vertex3 vCOM = Vertex3::Null;
    UInt iNodeCount = pCluster->arrNodes.Count();
    for( UInt i = 0; i < iNodeCount; ++i )
        vCOM += ( pCluster->arrNodes[i]->vPosition.ToVector() * pCluster->arrMasses[i] );
    vCOM *= pCluster->fInvMass;
    return vCOM;
}

UInt SoftBody::GenerateClusters( UInt iMeanK, UInt iMaxIterations )
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt iAreaCount = m_arrAreas.Count();
    UInt iVolumeCount = m_arrVolumes.Count();
    UInt i, j, k;

    if ( iMeanK > iNodeCount )
        iMeanK = iNodeCount;

    // Destroy previous clusters
    DestroyAllClusters();

    // Generate clusters
    UInt iClusterCount = iMeanK;
    Cluster * pCluster;

    if ( iMeanK > 0 ) {
        // Create new clusters
        PhysicsFn->SelectMemory( TEXT("SoftBodies") );
        for( i = 0; i < iClusterCount; ++i ) {
            pCluster = New Cluster();

            pCluster->arrNodes.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
            pCluster->arrNodes.Create();
            pCluster->arrMasses.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
            pCluster->arrMasses.Create();
            pCluster->arrFrames.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
            pCluster->arrFrames.Create();

            pCluster->bCollide = true;
            m_arrClusters.Push( pCluster );
        }
        PhysicsFn->UnSelectMemory();

        // Compute geometric center
        Vertex3 vCenter( 0.0f, 0.0f, 0.0f );
        for( i = 0; i < iNodeCount; ++i ) {
            vCenter += m_arrNodes[i].vPosition.ToVector();
            m_arrClusters[ (i * 29873) % iClusterCount ]->arrNodes.Push( &(m_arrNodes[i]) );
        }
        vCenter *= MathFn->Invert( (Scalar)iNodeCount );

        // Phase 1 : Iterations
        PhysicsFn->SelectMemory( TEXT("Scratch") );
        Vertex3 * arrCenters = New Vertex3[iMeanK];
        PhysicsFn->UnSelectMemory();

        for( i = 0; i < iMeanK; ++i )
            arrCenters[i] = vCenter;

        Scalar fW, fSlope = 16.0f;
        Bool bChanged;

        Vertex3 vC, vPos;
        Scalar fDistanceK, fDist;
        UInt iClusterNodeCount, iBestK;

        for( UInt iIteration = 0; iIteration < iMaxIterations; ++iIteration ) {
            fW = ( 2.0f - Min<Scalar>(1.0f, iIteration / fSlope) );
            bChanged = false;

            for( i = 0; i < iMeanK; ++i ) {
                vC = Vertex3::Null;
                iClusterNodeCount = m_arrClusters[i]->arrNodes.Count();
                for( j = 0; j < iClusterNodeCount; ++i )
                    vC += m_arrClusters[i]->arrNodes[j]->vPosition.ToVector();
                if ( iClusterNodeCount > 0 ) {
                    vC *= MathFn->Invert( (Scalar)iClusterNodeCount );
                    vC = ( arrCenters[i] + (vC - arrCenters[i]) * fW );
                    bChanged = ( bChanged || ((vC - arrCenters[i]).NormSqr() > SCALAR_EPSILON) );
                    arrCenters[i] = vC;
                    m_arrClusters[i]->arrNodes.Clear();
                }
            }
            for( i = 0; i < iNodeCount; ++i ) {
                vPos = m_arrNodes[i].vPosition;
                fDistanceK = ClusterMetric( arrCenters[0], vPos );
                iBestK = 0;
                for( j = 1; j < iMeanK; ++j ) {
                    fDist = ClusterMetric( arrCenters[j], vPos );
                    if ( fDist < fDistanceK ) {
                        fDistanceK = fDist;
                        iBestK = j;
                    }
                }
                m_arrClusters[iBestK]->arrNodes.Push( &(m_arrNodes[i]) );
            }

            if ( !bChanged )
                break;
        }

        PhysicsFn->SelectMemory( TEXT("Scratch") );
        DeleteA( arrCenters );
        PhysicsFn->UnSelectMemory();

        // Phase 2 : Merge
        PhysicsFn->SelectMemory( TEXT("Scratch") );
        UInt * arrIndices = New UInt[iNodeCount];
        PhysicsFn->UnSelectMemory();

        for( i = 0; i < iNodeCount; ++i )
            arrIndices[i] = INVALID_OFFSET;

        UInt arrCurIndices[3];
        UInt iIndexC, iIndexK;
        UInt iFound;

        for( i = 0; i < iClusterCount; ++i ) {
            iClusterNodeCount = m_arrClusters[i]->arrNodes.Count();
            for( j = 0; j < iClusterNodeCount; ++j )
                arrIndices[m_arrClusters[i]->arrNodes[j] - &(m_arrNodes[0])] = i;
        }
        for( i = 0; i < iAreaCount; ++i ) {
            arrCurIndices[0] = ( m_arrAreas[i].pNodeA - &(m_arrNodes[0]) );
            arrCurIndices[1] = ( m_arrAreas[i].pNodeB - &(m_arrNodes[0]) );
            arrCurIndices[2] = ( m_arrAreas[i].pNodeC - &(m_arrNodes[0]) );
            for( j = 0; j < 3; ++j ) {
                iIndexC = arrIndices[ arrCurIndices[j] ];
                for( k = 1; k < 3; ++k ) {
                    iIndexK = arrCurIndices[(j+k) % 3];
                    if ( arrIndices[iIndexK] != iIndexC ) {
                        iFound = m_arrClusters[iIndexC]->arrNodes.Search( &(m_arrNodes[iIndexK]) );
                        if ( iFound == INVALID_OFFSET )
                            m_arrClusters[iIndexC]->arrNodes.Push( &(m_arrNodes[iIndexK]) );
                    }
                }
            }
        }

        PhysicsFn->SelectMemory( TEXT("Scratch") );
        DeleteA( arrIndices );
        PhysicsFn->UnSelectMemory();

        // Phase 3 : Master
        if ( iClusterCount > 1 ) {
            PhysicsFn->SelectMemory( TEXT("SoftBodies") );
            Cluster * pMaster = New Cluster();
            PhysicsFn->UnSelectMemory();

            pMaster->arrNodes.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
            pMaster->arrNodes.Create();
            pMaster->arrMasses.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
            pMaster->arrMasses.Create();
            pMaster->arrFrames.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
            pMaster->arrFrames.Create();

            for( i = 0; i < iNodeCount; ++i )
                pMaster->arrNodes.Push( &(m_arrNodes[i]) );

            pMaster->bCollide = false;

            m_arrClusters.Push( pMaster );

            iClusterCount = m_arrClusters.Count();
            Swap<Cluster*>( &(m_arrClusters[0]), &(m_arrClusters[iClusterCount - 1]) );
        }

        // Phase 4 : Cleanup
        for( i = 0; i < iClusterCount; ++i ) {
            if ( m_arrClusters[i]->arrNodes.IsEmpty() )
                DestroyCluster( i-- );
        }
    } else {
        // Use volumes if present, else use areas
        if ( iVolumeCount > 0 ) {
            iClusterCount = iVolumeCount;

            PhysicsFn->SelectMemory( TEXT("SoftBodies") );
            for( i = 0; i < iClusterCount; ++i ) {
                pCluster = New Cluster();

                pCluster->arrNodes.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
                pCluster->arrNodes.Create();
                pCluster->arrMasses.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
                pCluster->arrMasses.Create();
                pCluster->arrFrames.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
                pCluster->arrFrames.Create();

                pCluster->bCollide = true;

                pCluster->arrNodes.Push( m_arrVolumes[i].pNodeA );
                pCluster->arrNodes.Push( m_arrVolumes[i].pNodeB );
                pCluster->arrNodes.Push( m_arrVolumes[i].pNodeC );
                pCluster->arrNodes.Push( m_arrVolumes[i].pNodeD );

                m_arrClusters.Push( pCluster );
            }
            PhysicsFn->UnSelectMemory();
        } else {
            iClusterCount = iAreaCount;

            PhysicsFn->SelectMemory( TEXT("SoftBodies") );
            for( i = 0; i < iClusterCount; ++i ) {
                pCluster = New Cluster();

                pCluster->arrNodes.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
                pCluster->arrNodes.Create();
                pCluster->arrMasses.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
                pCluster->arrMasses.Create();
                pCluster->arrFrames.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("SoftBodies") );
                pCluster->arrFrames.Create();

                pCluster->bCollide = true;

                pCluster->arrNodes.Push( m_arrAreas[i].pNodeA );
                pCluster->arrNodes.Push( m_arrAreas[i].pNodeB );
                pCluster->arrNodes.Push( m_arrAreas[i].pNodeC );

                m_arrClusters.Push( pCluster );
            }
            PhysicsFn->UnSelectMemory();
        }
    }

    // Initialize & update clusters
    if ( iClusterCount > 0 ) {
        _Clusters_Initialize();
        _Update_Clusters();

        // Self-Collision support
        UInt iA, iB;
        Bool bConnected;

        m_arrClusterConnectivity.Clear();
        for( iA = 0; iA < iClusterCount; ++iA ) {
            m_arrClusters[iA]->iIndex = iA;
            for( iB = 0; iB < iClusterCount; ++iB ) {
                bConnected = false;
                Cluster * pClusterA = m_arrClusters[iA];
                Cluster * pClusterB = m_arrClusters[iB];
                for( i = 0; i < pClusterA->arrNodes.Count(); ++i ) {
                    for( j = 0; j < pClusterB->arrNodes.Count(); ++j ) {
                        if ( pClusterA->arrNodes[i] == pClusterB->arrNodes[j] ) {
                            bConnected = true;
                            break;
                        }
                    }
                    if ( bConnected )
                        break;
                }
                m_arrClusterConnectivity.Push( bConnected );
            }
        }
    }

    return iClusterCount;
}
Void SoftBody::DestroyCluster( UInt iCluster )
{
    Assert( iCluster < m_arrClusters.Count() );
    Cluster * pCluster = m_arrClusters[iCluster];

    //if ( pCluster->m_leaf != NULL )
    //    m_cdbvt.remove( pCluster->m_leaf );

    pCluster->arrFrames.Destroy();
    pCluster->arrMasses.Destroy();
    pCluster->arrNodes.Destroy();

    PhysicsFn->SelectMemory( TEXT("SoftBodies") );
    Delete( pCluster );
    PhysicsFn->UnSelectMemory();

    m_arrClusters.Remove( iCluster, NULL, 1 );
}
Void SoftBody::DestroyAllClusters()
{
    while( m_arrClusters.Count() > 0 )
        DestroyCluster( 0 );
}

Void SoftBody::Refine( Scalar fAccuracy, Bool bAllowCut, ImplicitFn pfRefineFunction, Void * pUserData )
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt iLinkCount = m_arrLinks.Count();
    UInt iAreaCount = m_arrAreas.Count();
    UInt i, j, k, l, iA, iB, iC;

    Node *pNode, *pNodeA, *pNodeB;
    Link * pLink;
    Area * pArea;

    UInt iNewNodes = 0;

    // Create edge map
    UInt iEdgePitch = iNodeCount;
    UInt iEdgeCount = iEdgePitch * iEdgePitch;

    PhysicsFn->SelectMemory( TEXT("Scratch") );
    Int * arrEdges = New Int[iEdgeCount];
    PhysicsFn->UnSelectMemory();

    for( i = 0; i < iEdgeCount; ++i )
        arrEdges[i] = -2;

    // Filter links
    Scalar fEvalA, fEvalB;

    for( i = 0; i < iLinkCount; ++i ) {
        pLink = &( m_arrLinks[i] );
        if ( pLink->bIsBridge ) {
            fEvalA = pfRefineFunction( pLink->pNodeA->vPosition, pUserData );
            fEvalB = pfRefineFunction( pLink->pNodeB->vPosition, pUserData );
            if ( MathFn->Sign(fEvalA) != MathFn->Sign(fEvalB) ) {
                Swap<Link>( pLink, &(m_arrLinks[iLinkCount-1]) );
                m_arrLinks.Pop( NULL, 1 );
                --i;
            }
        }
    }
    iLinkCount = m_arrLinks.Count();

    // Generate edge map
    for( i = 0; i < iLinkCount; ++i ) {
        pLink = &( m_arrLinks[i] );
        iA = ( pLink->pNodeA - &(m_arrNodes[0]) );
        iB = ( pLink->pNodeB - &(m_arrNodes[0]) );
        arrEdges[iB * iEdgePitch + iA] = -1;
    }
    for( i = 0; i < iAreaCount; ++i ) {
        pArea = &( m_arrAreas[i] );
        iA = ( pArea->pNodeA - &(m_arrNodes[0]) );
        iB = ( pArea->pNodeB - &(m_arrNodes[0]) );
        iC = ( pArea->pNodeC - &(m_arrNodes[0]) );
        arrEdges[iB * iEdgePitch + iA] = -1;
        arrEdges[iC * iEdgePitch + iB] = -1;
        arrEdges[iA * iEdgePitch + iC] = -1;
    }

    // Intersections
    Scalar fT, fMass;
    Scalar fMassA, fMassB, fMassC, fMassRatio;
    Vertex3 vLerpPos;
    Vector3 vLerpVelocity;

    for( i = 0; i < iNodeCount; ++i ) {
        for( j = i+1; j < iNodeCount; ++j ) {
            if ( arrEdges[j * iEdgePitch + i] == -1 ) {
                pNodeA = &( m_arrNodes[i] );
                pNodeB = &( m_arrNodes[j] );

                fT = ImplicitSolve( pfRefineFunction, pNodeA->vPosition, pNodeB->vPosition, fAccuracy );
                if ( fT > 0.0f ) {
                    vLerpPos = ( pNodeA->vPosition * (1.0f - fT) );
                    vLerpPos += ( pNodeB->vPosition.ToVector() * fT );
                    vLerpVelocity = ( pNodeA->vVelocity * (1.0f - fT) );
                    vLerpVelocity += ( pNodeB->vVelocity * fT );

                    if ( pNodeA->fInvMass > 0.0f ) {
                        if ( pNodeB->fInvMass > 0.0f ) {
                            fMassA = pNodeA->fMass;
                            fMassB = pNodeB->fMass;
                            fMassC = ( fMassA * (1.0f - fT) + fMassB * fT );
                            fMassRatio = (fMassA + fMassB) / (fMassA + fMassB + fMassC);
                            pNodeA->fMass = fMassA * fMassRatio;
                            pNodeA->fInvMass = MathFn->Invert( pNodeA->fMass );
                            pNodeB->fMass = fMassB * fMassRatio;
                            pNodeB->fInvMass = MathFn->Invert( pNodeB->fMass );
                            fMass = fMassC * fMassRatio;
                        } else {
                            pNodeA->fMass *= 0.5;
                            pNodeA->fInvMass *= 2.0f;
                            fMass = pNodeA->fMass;
                        }
                    } else {
                        if ( pNodeB->fInvMass > 0.0f ) {
                            pNodeB->fMass *= 0.5;
                            pNodeB->fInvMass *= 2.0f;
                            fMass = pNodeB->fMass;
                        } else
                            fMass = 0.0f;
                    }

                    pNode = PushNode( vLerpPos, fMass );
                    pNode->vVelocity = vLerpVelocity;
                    arrEdges[j * iEdgePitch + i] = m_arrNodes.Count() - 1;
                    ++iNewNodes;
                }
            }
        }
    }
    iNodeCount = m_arrNodes.Count();

    // Refine links
    Int iEdge;
    Link * pModelLink;
    Link * pNewLink;

    for( i = 0; i < iLinkCount; ++i ) {
        pLink = &( m_arrLinks[i] );
        iA = ( pLink->pNodeA - &(m_arrNodes[0]) );
        iB = ( pLink->pNodeB - &(m_arrNodes[0]) );
        if ( iA < iEdgePitch && iB < iEdgePitch ) {
            iEdge = arrEdges[iB * iEdgePitch + iA];
            if ( iEdge > 0 ) {
                pModelLink = &( m_arrLinks[i] );
                pNewLink = PushLink( i );
                pModelLink->pNodeA = &( m_arrNodes[iA] );
                pModelLink->pNodeB = &( m_arrNodes[iEdge] );
                pNewLink->pNodeA = &( m_arrNodes[iEdge] );
                pNewLink->pNodeB = &( m_arrNodes[iB] );
            }
        }
    }
    iLinkCount = m_arrLinks.Count();

    // Refine areas
    UInt arrIndices[3];
    Area * pModelArea;
    Area * pNewArea;

    for( i = 0; i < iAreaCount; ++i ) {
        pArea = &( m_arrAreas[i] );
        arrIndices[0] = ( pArea->pNodeA - &(m_arrNodes[0]) );
        arrIndices[1] = ( pArea->pNodeB - &(m_arrNodes[0]) );
        arrIndices[2] = ( pArea->pNodeC - &(m_arrNodes[0]) );
        j = 2;
        for( k = 0; k < 3; ++k ) {
            if ( arrIndices[j] < iEdgePitch && arrIndices[k] < iEdgePitch ) {
                iEdge = arrEdges[arrIndices[k] * iEdgePitch + arrIndices[j]];
                if ( iEdge > 0 ) {
                    l = (k+1) % 3;
                    pModelArea = &( m_arrAreas[i] );
                    pNewArea = PushArea( i );
                    pModelArea->pNodeA = &( m_arrNodes[ arrIndices[l] ] );
                    pModelArea->pNodeB = &( m_arrNodes[ arrIndices[j] ] );
                    pModelArea->pNodeC = &( m_arrNodes[ iEdge ] );
                    pNewArea->pNodeA = &( m_arrNodes[ iEdge ] );
                    pNewArea->pNodeB = &( m_arrNodes[ arrIndices[k] ] );
                    pNewArea->pNodeC = &( m_arrNodes[ arrIndices[l] ] );
                    PushLink( iEdge, arrIndices[l], pModelArea->pMaterial );
                    --i;
                    break;
                }
            }
            j = k;
        }
    }
    iLinkCount = m_arrLinks.Count();
    iAreaCount = m_arrAreas.Count();

    // Destroy edge map
    PhysicsFn->SelectMemory( TEXT("Scratch") );
    DeleteA( arrEdges );
    PhysicsFn->UnSelectMemory();

    // Cut phase
    if ( bAllowCut ) {
        // Create cut nodes array
        UInt iCutNodeCount = iNodeCount;

        PhysicsFn->SelectMemory( TEXT("Scratch") );
        UInt * arrCutNodes = New UInt[iCutNodeCount];
        PhysicsFn->UnSelectMemory();

        for( i = 0; i < iNodeCount; ++i )
            arrCutNodes[i] = 0;

        // Nodes
        Node * pNewNode;

        for( i = 0; i < iNodeCount; ++i ) {
            pNode = &( m_arrNodes[i] );
            fT = pfRefineFunction( pNode->vPosition, pUserData );
            if ( i >= iEdgePitch || MathFn->Abs(fT) < fAccuracy ) {
                fMass = pNode->fMass;
                if ( fMass > 0.0f ) {
                    fMass *= 0.5f;
                    pNode->fInvMass *= 2.0f;
                }
                pNewNode = PushNode( pNode->vPosition, fMass );
                pNewNode->vVelocity = pNode->vVelocity;
                arrCutNodes[i] = m_arrNodes.Count() - 1;
            }
        }
        iNodeCount = m_arrNodes.Count();

        // Links
        UInt iDetach;

        for( i = 0; i < iLinkCount; ++i ) {
            pLink = &( m_arrLinks[i] );
            iA = ( pLink->pNodeA - &(m_arrNodes[0]) );
            iB = ( pLink->pNodeB - &(m_arrNodes[0]) );
            iDetach = 0;
            if ( arrCutNodes[iA] != 0 && arrCutNodes[iB] != 0 ) {
                pNewLink = PushLink( i );
                iDetach = m_arrLinks.Count() - 1;
            } else {
                if ( ( pfRefineFunction(pLink->pNodeA->vPosition, pUserData) < fAccuracy ) &&
                     ( pfRefineFunction(pLink->pNodeB->vPosition, pUserData) < fAccuracy ) )
                    iDetach = i;
            }
            if ( iDetach != 0 ) {
                pLink = &( m_arrLinks[iDetach] );
                iA = ( pLink->pNodeA - &(m_arrNodes[0]) );
                iB = ( pLink->pNodeB - &(m_arrNodes[0]) );
                if ( arrCutNodes[iA] != 0 )
                    pLink->pNodeA = &( m_arrNodes[arrCutNodes[iA]] );
                if ( arrCutNodes[iB] != 0 )
                    pLink->pNodeB = &( m_arrNodes[arrCutNodes[iB]] );
            }
        }
        iLinkCount = m_arrLinks.Count();

        // Areas
        for( i = 0; i < iAreaCount; ++i ) {
            pArea = &( m_arrAreas[i] );
            if ( ( pfRefineFunction(pArea->pNodeA->vPosition, pUserData) < fAccuracy ) &&
                 ( pfRefineFunction(pArea->pNodeB->vPosition, pUserData) < fAccuracy ) &&
                 ( pfRefineFunction(pArea->pNodeC->vPosition, pUserData) < fAccuracy ) ) {
                iA = ( pArea->pNodeA - &(m_arrNodes[0]) );
                iB = ( pArea->pNodeB - &(m_arrNodes[0]) );
                iC = ( pArea->pNodeC - &(m_arrNodes[0]) );
                if ( arrCutNodes[iA] != 0 )
                    pArea->pNodeA = &( m_arrNodes[arrCutNodes[iA]] );
                if ( arrCutNodes[iB] != 0 )
                    pArea->pNodeB = &( m_arrNodes[arrCutNodes[iB]] );
                if ( arrCutNodes[iC] != 0 )
                    pArea->pNodeC = &( m_arrNodes[arrCutNodes[iC]] );
            }
        }

        // Destroy cut nodes array
        PhysicsFn->SelectMemory( TEXT("Scratch") );
        DeleteA( arrCutNodes );
        PhysicsFn->UnSelectMemory();

        // Create reference array
        PhysicsFn->SelectMemory( TEXT("Scratch") );
        UInt * arrReferences = New UInt[iNodeCount];
        PhysicsFn->UnSelectMemory();

        for( i = 0; i < iNodeCount; ++i )
            arrReferences[i] = 0;

        // Cleanup orphans
        for( i = 0; i < iLinkCount; ++i ) {
            pLink = &( m_arrLinks[i] );
            iA = ( pLink->pNodeA - &(m_arrNodes[0]) );
            iB = ( pLink->pNodeB - &(m_arrNodes[0]) );
            ++(arrReferences[iA]);
            ++(arrReferences[iB]);
        }
        for( i = 0; i < iAreaCount; ++i ) {
            pArea = &( m_arrAreas[i] );
            iA = ( pArea->pNodeA - &(m_arrNodes[0]) );
            iB = ( pArea->pNodeB - &(m_arrNodes[0]) );
            iC = ( pArea->pNodeC - &(m_arrNodes[0]) );
            ++(arrReferences[iA]);
            ++(arrReferences[iB]);
            ++(arrReferences[iC]);
        }
        for( i = 0; i < iLinkCount; ++i ) {
            pLink = &( m_arrLinks[i] );
            iA = ( pLink->pNodeA - &(m_arrNodes[0]) );
            iB = ( pLink->pNodeB - &(m_arrNodes[0]) );
            if ( arrReferences[iA] == 1 || arrReferences[iB] == 1 ) {
                --arrReferences[iA];
                --arrReferences[iB];
                Swap<Link>( pLink, &(m_arrLinks[iLinkCount-1]) );
                m_arrLinks.Pop( NULL, 1 );
                --iLinkCount;
                --i;
            }
        }

        // Destroy reference array
        PhysicsFn->SelectMemory( TEXT("Scratch") );
        DeleteA( arrReferences );
        PhysicsFn->UnSelectMemory();
//01358 #if 0   
//01334                 btAlignedObjectArray<int>       todelete;
//01359                 for(i=nnodes-1;i>=0;--i)
//01360                 {
//01361                         if(!ranks[i]) todelete.push_back(i);
//01362                 }       
//01363                 if(todelete.size())
//01364                 {               
//01365                         btAlignedObjectArray<int>&      map=ranks;
//01366                         for(int i=0;i<nnodes;++i) map[i]=i;
//01367                         PointersToIndices(this);
//01368                         for(int i=0,ni=todelete.size();i<ni;++i)
//01369                         {
//01370                                 int             j=todelete[i];
//01371                                 int&    a=map[j];
//01372                                 int&    b=map[--nnodes];
//01373                                 m_ndbvt.remove(m_nodes[a].m_leaf);m_nodes[a].m_leaf=0;
//01374                                 btSwap(m_nodes[a],m_nodes[b]);
//01375                                 j=a;a=b;b=j;                    
//01376                         }
//01377                         IndicesToPointers(this,&map[0]);
//01378                         m_nodes.resize(nnodes);
//01379                 }
//01380 #endif
    }

    // Update
    m_bUpdateRuntimeConstants = true;
}

SoftBody::JointLinear * SoftBody::PushJoint( const JointLinearParameters & jointParams, Cluster * pBodyA, const Body & rBodyB )
{
    PhysicsFn->SelectMemory( TEXT("SoftBodies") );
    JointLinear * pJoint = New JointLinear();
    PhysicsFn->UnSelectMemory();

    Transform3 trInvWorldA, trInvWorldB;
    pBodyA->vTransform.Invert( trInvWorldA );
    rBodyB.GetTransform().Invert( trInvWorldB );

    pJoint->jointBodyA = pBodyA;
    pJoint->jointBodyB = rBodyB;
    pJoint->vReferenceA = ( trInvWorldA * jointParams.vPosition ).ToVector();
    pJoint->vReferenceB = ( trInvWorldB * jointParams.vPosition ).ToVector();

    pJoint->fERP = jointParams.fERP;
    pJoint->fSplit = jointParams.fSplit;
    pJoint->fCFM = jointParams.fCFM;

    pJoint->bDelete = false;

    m_arrJoints.Push( pJoint );

    return pJoint;
}
SoftBody::JointAngular * SoftBody::PushJoint( const JointAngularParameters & jointParams, Cluster * pBodyA, const Body & rBodyB )
{
    PhysicsFn->SelectMemory( TEXT("SoftBodies") );
    JointAngular * pJoint = New JointAngular();
    PhysicsFn->UnSelectMemory();

    Transform3 trInvWorldA, trInvWorldB;
    pBodyA->vTransform.Invert( trInvWorldA );
    rBodyB.GetTransform().Invert( trInvWorldB );

    pJoint->jointBodyA = pBodyA;
    pJoint->jointBodyB = rBodyB;
    pJoint->vReferenceA = ( trInvWorldA * jointParams.vAxis );
    pJoint->vReferenceB = ( trInvWorldB * jointParams.vAxis );

    pJoint->fERP = jointParams.fERP;
    pJoint->fSplit = jointParams.fSplit;
    pJoint->fCFM = jointParams.fCFM;
    //pJoint->m_iControl = jointParams.iControl;

    pJoint->bDelete = false;

    m_arrJoints.Push( pJoint );

    return pJoint;
}

Void SoftBody::Translate( const Vector3 & vTranslate )
{
    Transform3 vTransform;
    vTransform.SetTranslate( vTranslate );
    Transform( vTransform );
}
Void SoftBody::Rotate( const Quaternion & qRotate )
{
    Matrix3 matRotate;
    qRotate.GetMatrix( matRotate );

    Transform3 vTransform;
    vTransform.SetRotate( matRotate );
    Transform( vTransform );
}
Void SoftBody::Scale( const Vector3 & vScale )
{
    UInt iNodeCount = m_arrNodes.Count();
    Node * pNode;

    for( UInt i = 0; i < iNodeCount; ++i ) {
        pNode = &( m_arrNodes[i] );

        pNode->vPosition.X *= ( vScale.X );
        pNode->vPosition.Y *= ( vScale.Y );
        pNode->vPosition.Z *= ( vScale.Z );
        pNode->vOldPosition.X *= ( vScale.X );
        pNode->vOldPosition.Y *= ( vScale.Y );
        pNode->vOldPosition.Z *= ( vScale.Z );

        //ATTRIBUTE_ALIGNED16(btDbvtVolume) vol = btDbvtVolume::FromCR( pNode->vPosition, margin );
        //m_ndbvt.update( pNode->m_leaf, vol );
    }

    // Update
    _Update_Normals();
    _Update_Bounds();
    _Update_Constants();
}
Void SoftBody::Transform( const Transform3 & vTransform )
{
    UInt iNodeCount = m_arrNodes.Count();
    Node * pNode;

    for( UInt i = 0; i < iNodeCount; ++i ) {
        pNode = &( m_arrNodes[i] );

        pNode->vPosition = ( vTransform * pNode->vPosition );
        pNode->vOldPosition = ( vTransform * pNode->vOldPosition );
        pNode->vNormal = ( vTransform * pNode->vNormal );

        //ATTRIBUTE_ALIGNED16(btDbvtVolume) vol = btDbvtVolume::FromCR( pNode->vPosition, margin );
        //m_ndbvt.update( pNode->m_leaf, vol );
    }

    // Update
    _Update_Normals();
    _Update_Bounds();
    _Update_Constants();

    m_vInitialTransform = vTransform;
}

Void SoftBody::SetPose( Bool bIsVolume, Bool bIsFrame )
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt i;

    // Flags
    m_hPose.bIsVolume = bIsVolume;
    m_hPose.bIsFrame = bIsFrame;

    // Weights
    Scalar fMassO = GetMass();
    Scalar fMassK = fMassO * (iNodeCount * 1000);

    Scalar fTotalMass = fMassO;

    m_hPose.arrWeights.Clear();
    for( i = 0; i < iNodeCount; ++i ) {
        if ( m_arrNodes[i].fInvMass <= 0.0f )
            fTotalMass += fMassK;
    }
    Scalar fInvTotalMass = MathFn->Invert( fTotalMass );

    for( i = 0; i < iNodeCount; ++i ) {
        Node * pNode = &( m_arrNodes[i] );
        if ( pNode->fInvMass > 0.0f )
            m_hPose.arrWeights.Push( pNode->fMass * fInvTotalMass );
        else
            m_hPose.arrWeights.Push( fMassK * fInvTotalMass );
    }

    // Positions
    m_hPose.vCenterOfMass = _ComputeCenterOfMass();

    m_hPose.arrPositions.Clear();
    for( i = 0; i < iNodeCount; ++i )
        m_hPose.arrPositions.Push( m_arrNodes[i].vPosition - m_hPose.vCenterOfMass );

    m_hPose.fRestVolume = (bIsVolume) ? GetVolume() : 0.0f;
    m_hPose.matRotation.MakeIdentity();
    m_hPose.matScaling.MakeIdentity();

    // BaseScaling
    Matrix3 matBaseScaling;
    Vector3 vTmpRow;

    matBaseScaling.MakeNull();
    for( i = 0; i < iNodeCount; ++i ) {
        Vector3 vPos = m_hPose.arrPositions[i].ToVector();
        Vector3 vWeightedPos = ( vPos * m_hPose.arrWeights[i] );
        matBaseScaling.GetRow( vTmpRow, 0 );
        matBaseScaling.SetRow( 0, vTmpRow + (vPos * vWeightedPos.X) );
        matBaseScaling.GetRow( vTmpRow, 1 );
        matBaseScaling.SetRow( 1, vTmpRow + (vPos * vWeightedPos.Y) );
        matBaseScaling.GetRow( vTmpRow, 2 );
        matBaseScaling.SetRow( 2, vTmpRow + (vPos * vWeightedPos.Z) );
    }
    matBaseScaling.Invert( m_hPose.matBaseScaling );

    // Update
    _Update_Constants();
}

Void SoftBody::SetSolver( SoftBodySolver iSolver )
{
    m_hConfig.arrSolverSequenceVelocities.Clear();
    m_hConfig.arrSolverSequencePositions.Clear();
    m_hConfig.arrSolverSequenceDrift.Clear();

    switch( iSolver ) {
        case SOFTBODY_SOLVER_POSITIONS:
            m_hConfig.arrSolverSequencePositions.Push( SOFTBODY_PSOLVER_ANCHORS );
            m_hConfig.arrSolverSequencePositions.Push( SOFTBODY_PSOLVER_CONTACTS_RIGID );
            m_hConfig.arrSolverSequencePositions.Push( SOFTBODY_PSOLVER_CONTACTS_SOFT );
            m_hConfig.arrSolverSequencePositions.Push( SOFTBODY_PSOLVER_LINEAR );
            break;
        case SOFTBODY_SOLVER_VELOCITIES:
            m_hConfig.arrSolverSequenceVelocities.Push( SOFTBODY_VSOLVER_LINEAR );

            m_hConfig.arrSolverSequencePositions.Push( SOFTBODY_PSOLVER_ANCHORS );
            m_hConfig.arrSolverSequencePositions.Push( SOFTBODY_PSOLVER_CONTACTS_RIGID );
            m_hConfig.arrSolverSequencePositions.Push( SOFTBODY_PSOLVER_CONTACTS_SOFT );

            m_hConfig.arrSolverSequenceDrift.Push( SOFTBODY_PSOLVER_LINEAR );
            break;
        default: Assert(false); break;
    }
}

Void SoftBody::RandomizeConstraints()
{
    UInt iLinkCount = m_arrLinks.Count();
    UInt iAreaCount = m_arrAreas.Count();
    UInt i;

    for( i = 0; i < iLinkCount; ++i )
        Swap<Link>( &(m_arrLinks[i]), &(m_arrLinks[(UInt)(RandFn->XorShift128() % iLinkCount)]) );
    for( i = 0; i < iAreaCount; ++i )
        Swap<Area>( &(m_arrAreas[i]), &(m_arrAreas[(UInt)(RandFn->XorShift128() % iAreaCount)]) );
}
Void SoftBody::SolveConstraints()
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt iLinkCount = m_arrLinks.Count();
    UInt iAnchorCount = m_arrAnchors.Count();
    UInt i, iSeq, iSeqLength;

    Node * pNode;
    Link * pLink;
    Anchor * pAnchor;

    VelocitySolver pfVelocitySolver;
    PositionSolver pfPositionSolver;

    Scalar fT;

    // Apply clusters
    _Clusters_Apply( false );

    // Prepare links
    for( i = 0; i < iLinkCount; ++i ) {
        pLink = &( m_arrLinks[i] );
        pLink->vGradient = ( pLink->pNodeB->vOldPosition - pLink->pNodeA->vOldPosition );
        pLink->fGradientFactor = MathFn->Invert( pLink->vGradient.NormSqr() * pLink->fLinearFactor );
    }

    // Prepare anchors
    for( i = 0; i < iAnchorCount; ++i ) {
        pAnchor = &( m_arrAnchors[i] );
        pAnchor->vRelativeAnchor = ( pAnchor->pBody->GetTransform() * pAnchor->vLocalAnchor );
        pAnchor->matImpulse = ImpulseMatrix( pAnchor->pNode->fInvMass, pAnchor->pBody->GetInvMass(),
                                             pAnchor->pBody->GetInvInertiaTensor(),
                                             pAnchor->vRelativeAnchor, m_hSolverState.fScaledDT );
        pAnchor->fInvMassDT = ( pAnchor->pNode->fInvMass * m_hSolverState.fScaledDT );
        //pAnchor->pBody->SetActive( true );
    }

    // Solve velocities
    if ( m_hConfig.iMaxIterationsVelocities > 0 ) {
        iSeqLength = m_hConfig.arrSolverSequenceVelocities.Count();

        for( i = 0; i < m_hConfig.iMaxIterationsVelocities; ++i ) {
            for( iSeq = 0; iSeq < iSeqLength; ++iSeq ) {
                pfVelocitySolver = _GetVelocitySolver( m_hConfig.arrSolverSequenceVelocities[iSeq] );
                pfVelocitySolver( this, 1.0f );
            }
        }

        for( i = 0; i < iNodeCount; ++i ) {
            pNode = &( m_arrNodes[i] );
            pNode->vPosition = pNode->vOldPosition + ( pNode->vVelocity * m_hSolverState.fScaledDT );
        }
    }

    // Solve positions
    if ( m_hConfig.iMaxIterationsPositions > 0 ) {
        iSeqLength = m_hConfig.arrSolverSequencePositions.Count();

        for( i = 0; i < m_hConfig.iMaxIterationsPositions; ++i ) {
            fT = ( ((Scalar)i) / (Scalar)(m_hConfig.iMaxIterationsPositions) );
            for( iSeq = 0; iSeq < iSeqLength; ++iSeq ) {
                pfPositionSolver = _GetPositionSolver( m_hConfig.arrSolverSequencePositions[iSeq] );
                pfPositionSolver( this, 1.0f, fT );
            }
        }

        fT = (1.0f - m_hConfig.fDamping) * m_hSolverState.fInvScaledDT;
        for( i = 0; i < iNodeCount; ++i ) {
            pNode = &( m_arrNodes[i] );
            pNode->vVelocity = ( pNode->vPosition - pNode->vOldPosition ) * fT;
            pNode->vForceBuffer = Vector3::Null;
        }
    }

    // Solve drift
    if ( m_hConfig.iMaxIterationsDrift ) {
        iSeqLength = m_hConfig.arrSolverSequenceDrift.Count();

        for( i = 0; i < iNodeCount; ++i ) {
            pNode = &( m_arrNodes[i] );
            pNode->vOldPosition = pNode->vPosition;
        }

        for( i = 0; i < m_hConfig.iMaxIterationsDrift; ++i ) {
            for( iSeq = 0; iSeq < iSeqLength; ++iSeq ) {
                pfPositionSolver = _GetPositionSolver( m_hConfig.arrSolverSequenceDrift[iSeq] );
                pfPositionSolver( this, 1.0f, 0.0f );
            }
        }

        fT = ( m_hConfig.fBaumgarte * m_hSolverState.fInvScaledDT );
        for( i = 0; i < iNodeCount; ++i ) {
            pNode = &( m_arrNodes[i] );
            pNode->vVelocity += ( pNode->vPosition - pNode->vOldPosition ) * fT;
        }
    }

    // Apply clusters
    _Clusters_Damping();
    _Clusters_Apply( true );
}
Void SoftBody::SolveClusters( SoftBodyArray & arrBodies )
{
    UInt iBodyCount = arrBodies.Count();
    UInt i, j;

    UInt iMaxIterations = 0;
    for( i = 0; i < iBodyCount; ++i ) {
        if ( arrBodies[i]->m_hConfig.iMaxIterationsCluster > iMaxIterations )
            iMaxIterations = arrBodies[i]->m_hConfig.iMaxIterationsCluster;
    }

    for( i = 0; i < iBodyCount; ++i )
        arrBodies[i]->_Clusters_Prepare( iMaxIterations );

    Scalar fSOR = 1.0f; // Successive over-relaxation
    for( i = 0; i < iMaxIterations; ++i ) {
        for( j = 0; j < iBodyCount; ++j )
            arrBodies[i]->_Clusters_Solve( fSOR );
    }

    for( i = 0; i < iBodyCount; ++i )
        arrBodies[i]->_Clusters_Cleanup();
}

Void SoftBody::SolveStatic( UInt iIterations )
{
    UInt iSeqLength = m_hConfig.arrSolverSequencePositions.Count();
    UInt i, iSeq;

    PositionSolver pfPositionSolver;

    for( i = 0; i < iIterations; ++i )  {
        for( iSeq = 0; iSeq < iSeqLength; ++iSeq ) {
            pfPositionSolver =  _GetPositionSolver( m_hConfig.arrSolverSequencePositions[iSeq] );
            pfPositionSolver( this, 1.0f, 0.0f );
        }
    }
}

Void SoftBody::PredictMotion( Scalar fTimeStep )
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt i;

    // Update
    if ( m_bUpdateRuntimeConstants ) {
        _Update_Constants();
        //m_fdbvt.clear();
        if ( m_hConfig.iCollisionFlags & 0x01 ) //fCollision::VF_SS )
            _Initialize_AreaTree();
        m_bUpdateRuntimeConstants = false;
    }

    // Prepare
    m_hSolverState.fScaledDT = (  m_hConfig.fTimeScale * fTimeStep );
    m_hSolverState.fInvScaledDT = MathFn->Invert( m_hSolverState.fScaledDT );
    m_hSolverState.fVelocityMargin = m_hSolverState.fScaledDT * 3.0f;
    m_hSolverState.fRadialMargin = 0.1f;
    m_hSolverState.fUpdateMargin = m_hSolverState.fRadialMargin * 0.25f;

    // Apply forces
    ApplyVelocity( m_pEnvironment->vGravity * m_hSolverState.fScaledDT );
    _ApplyForces();

    // Integrate
    Node * pNode;
    for( i = 0; i < iNodeCount; ++i ) {
        pNode = &( m_arrNodes[i] );
        pNode->vOldPosition = pNode->vPosition;
        pNode->vVelocity += pNode->vForceBuffer * ( pNode->fInvMass * m_hSolverState.fScaledDT );
        pNode->vPosition += pNode->vVelocity * m_hSolverState.fScaledDT;
        pNode->vForceBuffer = Vector3::Null;
    }

    // Update
    _Update_Clusters();
    _Update_Bounds();

    // Nodes
    iNodeCount = m_arrNodes.Count();
    //ATTRIBUTE_ALIGNED16(btDbvtVolume) vol;
    //for( i = 0; i < iNodeCount; ++i ) {
    //    pNode = &( m_arrNodes[i] );
    //    vol = btDbvtVolume::FromCR( pNode->vPosition, m_hSolverState.fRadialMargin );
    //    m_ndbvt.update( pNode->m_leaf, vol, pNode->vVelocity * m_hSolverState.fVelocityMargin, m_hSolverState.fUpdateMargin );
    //}

    // Areas
    //UInt iAreaCount = m_arrAreas.Count();
    //Area * pArea;
    //Vector3 vVelocity;

    //if ( !(m_fdbvt.empty()) ) {
    //    for( i = 0; i < iAreaCount; ++i ) {
    //        pArea = &( m_arrAreas[i] );
    //        vVelocity = ( pArea->pNodeA->vVelocity + pArea->pNodeB->vVelocity + pArea->pNodeC->vVelocity );
    //        vVelocity *= MathFn->Invert( 3.0f );
    //        vol = VolumeOf( pArea, m_hSolverState.fRadialMargin );
    //        m_ndbvt.update( pArea->m_leaf, vol, vVelocity * m_hSolverState.fVelocityMargin, m_hSolverState.fUpdateMargin );
    //    }
    //}

    // Update
    _Update_Pose();

    // Match
    Vertex3 vPosition;

    if ( m_hPose.bIsFrame && (m_hConfig.fMatching > 0.0f) ) {
        Matrix3 matPoseOrientation( m_hPose.matRotation );
        for( i = 0; i < iNodeCount; ++i ) {
            pNode = &( m_arrNodes[i] );
            if ( pNode->fInvMass > 0.0f ) {
                vPosition = m_hPose.vCenterOfMass.ToVector();
                vPosition += ( matPoseOrientation * m_hPose.arrPositions[i].ToVector() );
                pNode->vPosition *= ( 1.0f - m_hConfig.fMatching );
                pNode->vPosition += vPosition * m_hConfig.fMatching;
            }
        }
    }

    // Cleanup contacts
    m_arrRigidContacts.Clear();
    m_arrSoftContacts.Clear();

    // Optimize DynamicBVTree
    //m_ndbvt.optimizeIncremental(1);
    //m_fdbvt.optimizeIncremental(1);
    //m_cdbvt.optimizeIncremental(1);
}
Void SoftBody::IntegrateMotion()
{
    _Update_Normals();
}

Void SoftBody::Update( Scalar fTimeStep )
{
    /////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////

Void SoftBody::_SolveVelocities_Links( SoftBody * pBody, Scalar fKST )
{
    UInt iLinkCount = pBody->m_arrLinks.Count();
    Link * pLink;
    Node *pNodeA, *pNodeB;

    Vector3 vRelVelocity;
    Scalar fJ;

    for( UInt i = 0; i < iLinkCount; ++i ) {
        pLink = &( pBody->m_arrLinks[i] );
        pNodeA = pLink->pNodeA;
        pNodeB = pLink->pNodeB;

        vRelVelocity = ( pNodeA->vVelocity - pNodeB->vVelocity );
        fJ = -(pLink->vGradient * vRelVelocity) * pLink->fGradientFactor * fKST;

        pNodeA->vVelocity += pLink->vGradient * (fJ * pNodeA->fInvMass);
        pNodeB->vVelocity -= pLink->vGradient * (fJ * pNodeB->fInvMass);
    }
}

Void SoftBody::_SolvePositions_Links( SoftBody * pBody, Scalar fKST, Scalar fTI )
{
    UInt iLinkCount = pBody->m_arrLinks.Count();
    Link * pLink;
    Node *pNodeA, *pNodeB;

    Vector3 vDelta;
    Scalar fSqrLength, fNum, fDen, fK;

    for( UInt i = 0; i < iLinkCount; ++i ) {
        pLink = &( pBody->m_arrLinks[i] );
        if ( pLink->fLinearFactor > 0.0f ) {
            pNodeA = pLink->pNodeA;
            pNodeB = pLink->pNodeB;
            vDelta = ( pNodeB->vPosition - pNodeA->vPosition );
            fSqrLength = vDelta.NormSqr();
            fNum = ( pLink->fSqrRestLength - fSqrLength );
            fDen = ( pLink->fLinearFactor * (pLink->fSqrRestLength + fSqrLength) );
            fK = ( fNum / fDen ) * fKST;

            pNodeA->vPosition -= vDelta * (fK * pNodeA->fInvMass);
            pNodeB->vPosition += vDelta * (fK * pNodeB->fInvMass);
        }
    }
}
Void SoftBody::_SolvePositions_RigidContacts( SoftBody * pBody, Scalar fKST, Scalar fTI )
{
    Scalar fDT = ( pBody->m_hSolverState.fScaledDT );

    UInt iRigidContactCount = pBody->m_arrRigidContacts.Count();
    RigidContact * pRigidContact;
    RigidBody * pRigidBody;
    Node * pNode;

    Vector3 vBodyVelocity, vNodeVelocity, vRelVelocity;
    Vector3 vFrictionImpulse, vImpulse;
    Scalar fDotN, fDotP;

    for( UInt i = 0; i < iRigidContactCount; ++i ) {
        pRigidContact = &( pBody->m_arrRigidContacts[i] );
        const ContactInfos & cInfos = pRigidContact->cInfos;
        pRigidBody = cInfos.pBody;
        pNode = pRigidContact->pNode;

        vBodyVelocity = pRigidBody->GetLinearSpeedAt( pRigidContact->vRelativeAnchor ) * fDT;
        vNodeVelocity = ( pNode->vPosition - pNode->vOldPosition );
        vRelVelocity = ( vBodyVelocity - vNodeVelocity );

        fDotN = ( vRelVelocity * cInfos.vNormal );
        if ( fDotN <= SCALAR_ERROR ) {
            fDotP = ( pNode->vPosition.ToVector() * cInfos.vNormal ) + cInfos.fDistance;
            //fDotP = Min<Scalar>( fDotP, 0.1f ); // = shape margin

            vFrictionImpulse = ( vRelVelocity - (cInfos.vNormal * fDotN) );
            vImpulse = pRigidContact->matImpulse * ( (
                    vRelVelocity
                    - ( vFrictionImpulse * pRigidContact->fFriction )
                    + ( cInfos.vNormal * (fDotP * pRigidContact->fHardness) )
                ) * fKST
            );

            pRigidBody->ApplyImpulse( -vImpulse, pRigidContact->vRelativeAnchor );
            pNode->vPosition += ( vImpulse * pRigidContact->fInvMassDT );
        }
    }
}
Void SoftBody::_SolvePositions_SoftContacts( SoftBody * pBody, Scalar fKST, Scalar fTI )
{
    UInt iSoftContactCount = pBody->m_arrSoftContacts.Count();
    SoftContact * pSoftContact;
    Node * pNode;
    Area * pArea;

    Vertex3 vBary, vOldBary;
    Vector3 vNormal, vRelVelocity, vCorrection;
    Scalar fDot, fCorrection;

    for( UInt i = 0; i < iSoftContactCount; ++i ) {
        pSoftContact = &( pBody->m_arrSoftContacts[i] );
        pNode = pSoftContact->pNode;
        pArea = pSoftContact->pArea;

        vNormal = pSoftContact->vNormal;
        vBary = BaryEval( pArea->pNodeA->vPosition, pArea->pNodeB->vPosition, pArea->pNodeC->vPosition,
                          pSoftContact->vWeights );
        vOldBary = BaryEval( pArea->pNodeA->vOldPosition, pArea->pNodeB->vOldPosition, pArea->pNodeC->vOldPosition,
                             pSoftContact->vWeights );
        vRelVelocity = ( (pNode->vPosition - pNode->vOldPosition) - (vBary - vOldBary) );
        vCorrection = Vector3::Null;

        fDot = ( vRelVelocity * vNormal );
        if ( fDot < 0.0f ) {
            fCorrection = (
                ( vNormal * vBary.ToVector() )
                - ( vNormal * pNode->vPosition.ToVector() )
            ) + pSoftContact->fMargin;
            vCorrection += ( vNormal * fCorrection );
        }
        vCorrection -= ( ProjectOnPlane(vRelVelocity,vNormal) * pSoftContact->fFriction );

        pNode->vPosition += ( vCorrection * pSoftContact->fCFM[0] );
        pArea->pNodeA->vPosition -= vCorrection * ( pSoftContact->vWeights.X * pSoftContact->fCFM[1] );
        pArea->pNodeB->vPosition -= vCorrection * ( pSoftContact->vWeights.Y * pSoftContact->fCFM[1] );
        pArea->pNodeC->vPosition -= vCorrection * ( pSoftContact->vWeights.Z * pSoftContact->fCFM[1] );
    }
}
Void SoftBody::_SolvePositions_Anchors( SoftBody * pBody, Scalar fKST, Scalar fTI )
{
    Scalar fAnchorHardness = ( pBody->m_hConfig.fAnchorHardness * fKST );
    Scalar fDT = ( pBody->m_hSolverState.fScaledDT );

    UInt iAnchorCount = pBody->m_arrAnchors.Count();
    Anchor * pAnchor;
    Node * pNode;

    Vertex3 vWorldAnchor;
    Vector3 vBodyVelocity, vNodeVelocity, vRelVelocity;
    Vector3 vImpulse;

    for( UInt i = 0; i < iAnchorCount; ++i ) {
        pAnchor = &( pBody->m_arrAnchors[i] );

        const Transform3 & vWorld = pAnchor->pBody->GetTransform();
        pNode = pAnchor->pNode;

        vWorldAnchor = ( vWorld * pAnchor->vLocalAnchor );
        vBodyVelocity = pAnchor->pBody->GetLinearSpeedAt( pAnchor->vRelativeAnchor ) * fDT;
        vNodeVelocity = ( pNode->vPosition - pNode->vOldPosition );
        vRelVelocity = ( vBodyVelocity - vNodeVelocity ) + ( (vWorldAnchor - pNode->vPosition) * fAnchorHardness );
        vImpulse = ( pAnchor->matImpulse * vRelVelocity );

        pAnchor->pBody->ApplyImpulse( -vImpulse, pAnchor->vRelativeAnchor );
        pNode->vPosition += ( vImpulse * pAnchor->fInvMassDT );
    }
}

SoftBody::VelocitySolver SoftBody::_GetVelocitySolver( SoftBodyVelocitySolver iSolver )
{
    switch( iSolver ) {
        case SOFTBODY_VSOLVER_LINEAR: return SoftBody::_SolveVelocities_Links;
        default: Assert(false); break;
    }
    return NULL;
}
SoftBody::PositionSolver SoftBody::_GetPositionSolver( SoftBodyPositionSolver iSolver )
{
    switch( iSolver ) {
        case SOFTBODY_PSOLVER_LINEAR:         return SoftBody::_SolvePositions_Links;
        case SOFTBODY_PSOLVER_CONTACTS_RIGID: return SoftBody::_SolvePositions_RigidContacts;
        case SOFTBODY_PSOLVER_CONTACTS_SOFT:  return SoftBody::_SolvePositions_SoftContacts;
        case SOFTBODY_PSOLVER_ANCHORS:        return SoftBody::_SolvePositions_Anchors;
        default: Assert(false); break;
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////

Vertex3 SoftBody::_ComputeCenterOfMass() const
{
    Vertex3 vCOM( Vertex3::Null );
    if ( m_hPose.bIsFrame ) {
        UInt iNodeCount = m_arrNodes.Count();
        for( UInt i = 0; i < iNodeCount; ++i )
            vCOM += ( m_arrNodes[i].vPosition.ToVector() * m_hPose.arrWeights[i] );
    }
    return vCOM;
}

Bool SoftBody::_CheckContact( ContactInfos * outInfos, RigidBody * pBody, const Vertex3 & vPosition, Scalar fZeroTolerance ) const
{
    TransformedShape * pShape = pBody->GetWorldShape();

    const Transform3 & trWorld = pBody->GetTransform();
    Transform3 trInvWorld;
    trWorld.Invert( trInvWorld );

    Vector3 vNormal;
    Scalar fDistance = 0.0f; //m_pEnvironment->m_sparsesdf.Evaluate( trInvWorld * vPosition, pShape, vNormal );

    if ( fDistance < fZeroTolerance ) {
        outInfos->pBody = pBody;
        outInfos->vNormal = ( trWorld * vNormal );

        Vector3 vContact = ( vPosition.ToVector() - (outInfos->vNormal * fDistance) );
        outInfos->fDistance = -( outInfos->vNormal * vContact );

        return true;
    }

    return false;
}

Void SoftBody::_Initialize_AreaTree()
{
    UInt iAreaCount = m_arrAreas.Count();
    UInt i;

    Area * pArea;

    //m_fdbvt.clear();
    for( i = 0; i < iAreaCount; ++i ) {
        pArea = &( m_arrAreas[i] );
        //pArea->m_leaf = m_fdbvt.insert( VolumeOf(pArea,0), pArea );
    }
}

Void SoftBody::_Update_Normals()
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt iAreaCount = m_arrAreas.Count();
    UInt i;

    Area * pArea;

    for( i = 0; i < iNodeCount; ++i )
        m_arrNodes[i].vNormal = Vector3::Null;

    for( i = 0; i < iAreaCount; ++i ) {
        pArea = &( m_arrAreas[i] );
        pArea->vNormal = ( (pArea->pNodeB->vPosition - pArea->pNodeA->vPosition) ^
                           (pArea->pNodeC->vPosition - pArea->pNodeA->vPosition) );
        pArea->vNormal.Normalize();

        pArea->pNodeA->vNormal += pArea->vNormal;
        pArea->pNodeB->vNormal += pArea->vNormal;
        pArea->pNodeC->vNormal += pArea->vNormal;
    }

    for( i = 0; i < iNodeCount; ++i )
        m_arrNodes[i].vNormal.Normalize();
}
Void SoftBody::_Update_Bounds()
{
    m_bvAABox.Reset( Vertex3::Null );

    //if ( m_ndbvt.m_root == NULL ) {
    //    m_bvAABox.Reset( Vertex3::Null );
    //    return;
    //}

    //const btVector3 & mins = m_ndbvt.m_root->volume.Mins();
    //const btVector3 & maxs = m_ndbvt.m_root->volume.Maxs();
    //const btScalar csm = getCollisionShape()->getMargin();
    //const btVector3 mrg = btVector3( csm, csm, csm)*1; // ??? to investigate...
    //m_bounds[0]=mins-mrg;
    //m_bounds[1]=maxs+mrg;
    //if(0!=getBroadphaseHandle())
    //{                                       
    //    m_worldinfo->m_broadphase->setaabb(     getbroadphasehandle(),
    //    m_bounds[0],
    //    m_bounds[1],
    //    m_worldinfo->m_dispatcher);
    //}

}
Void SoftBody::_Update_Pose()
{
    if ( !(m_hPose.bIsFrame) )
        return;

    UInt iNodeCount = m_arrNodes.Count();
    UInt i;

    Node * pNode;
    Vector3 vA, vB;
    Matrix3 matRotScale;
    matRotScale.MakeNull();
    matRotScale.m00 = SCALAR_EPSILON;
    matRotScale.m11 = SCALAR_EPSILON * 2.0f;
    matRotScale.m22 = SCALAR_EPSILON * 3.0f;

    m_hPose.vCenterOfMass = _ComputeCenterOfMass();

    for( i = 0; i < iNodeCount; ++i ) {
        pNode = &( m_arrNodes[i] );
        vA = ( (pNode->vPosition - m_hPose.vCenterOfMass) * m_hPose.arrWeights[i] );
        vB = m_hPose.arrPositions[i].ToVector();

        matRotScale.m00 += ( vA.X * vB.X ); matRotScale.m01 += ( vA.X * vB.Y ); matRotScale.m02 += ( vA.X * vB.Z );
        matRotScale.m10 += ( vA.Y * vB.X ); matRotScale.m11 += ( vA.Y * vB.Y ); matRotScale.m12 += ( vA.Y * vB.Z );
        matRotScale.m20 += ( vA.Z * vB.X ); matRotScale.m21 += ( vA.Z * vB.Y ); matRotScale.m22 += ( vA.Z * vB.Z );
    }

    Matrix3 matRot, matScale;
    PolarDecompose( matRot, matScale, matRotScale );

    Matrix3 matInvRot;
    matRot.Transpose( matInvRot );

    m_hPose.matRotation = matRot;
    m_hPose.matScaling = ( m_hPose.matBaseScaling * matInvRot * matRotScale );
    if ( m_hConfig.fMaxVolumeRatio > 1.0f ) {
        Scalar fInvDet = Clamp<Scalar>( MathFn->Invert( m_hPose.matScaling.Determinant() ),
                                        1.0f, m_hConfig.fMaxVolumeRatio );
        m_hPose.matScaling *= fInvDet;
    }
}
Void SoftBody::_Update_Constants()
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt iLinkCount = m_arrLinks.Count();
    UInt iAreaCount = m_arrAreas.Count();
    UInt i, iA, iB, iC;

    Material * pMaterial;
    Link * pLink;
    Area * pArea;

    // Links
    for( i = 0; i < iLinkCount; ++i ) {
        pLink = &( m_arrLinks[i] );
        pMaterial = pLink->pMaterial;

        pLink->fRestLength = ( pLink->pNodeB->vPosition - pLink->pNodeA->vPosition ).Norm();
        pLink->fSqrRestLength = ( pLink->fRestLength * pLink->fRestLength );
        pLink->fLinearFactor = ( pLink->pNodeA->fInvMass + pLink->pNodeB->fInvMass ) / pMaterial->fLST;
    }

    // Areas
    for( i = 0; i < iAreaCount; ++i ) {
        pArea = &( m_arrAreas[i] );

        Triangle3 triTmp( pArea->pNodeA->vPosition, pArea->pNodeB->vPosition, pArea->pNodeC->vPosition );
        pArea->fRestArea = 0.0f; //triTmp.Area();
    }

    PhysicsFn->SelectMemory( TEXT("Scratch") );
    UInt * arrCounts = New UInt[iNodeCount];
    PhysicsFn->UnSelectMemory();

    for( i = 0; i < iNodeCount; ++i ) {
        arrCounts[i] = 0;
        m_arrNodes[i].fArea = 0.0f;
    }
    for( i = 0; i < iAreaCount; ++i ) {
        pArea = &( m_arrAreas[i] );
        iA = ( pArea->pNodeA - &(m_arrNodes[0]) );
        iB = ( pArea->pNodeB - &(m_arrNodes[0]) );
        iC = ( pArea->pNodeC - &(m_arrNodes[0]) );
        pArea->pNodeA->fArea += pArea->fRestArea;
        pArea->pNodeB->fArea += pArea->fRestArea;
        pArea->pNodeC->fArea += pArea->fRestArea;
        ++(arrCounts[iA]);
        ++(arrCounts[iB]);
        ++(arrCounts[iC]);
    }
    for( i = 0; i < iNodeCount; ++i ) {
        if ( arrCounts[i] > 0 )
            m_arrNodes[i].fArea /= (Scalar)( arrCounts[i] );
        else
            m_arrNodes[i].fArea = 0.0f;
    }

    PhysicsFn->SelectMemory( TEXT("Scratch") );
    DeleteA( arrCounts );
    PhysicsFn->UnSelectMemory();
}
Void SoftBody::_Update_Clusters()
{
    UInt iClusterCount = m_arrClusters.Count();
    UInt i, iNodeCount;

    Cluster * pCluster;
    Matrix3 matRotScale, matRotate, matScale;
    Matrix3 matInvRotate;
    Vector3 vA, vB, vVelocity, vPosition;

    for( UInt iCluster = 0; iCluster < iClusterCount; ++iCluster ) {
        pCluster = m_arrClusters[iCluster];
        iNodeCount = pCluster->arrNodes.Count();
        if ( iNodeCount == 0 )
            continue;

        // Frame
        matRotScale.MakeNull();
        matRotScale.m00 = SCALAR_EPSILON;
        matRotScale.m11 = SCALAR_EPSILON * 2.0f;
        matRotScale.m22 = SCALAR_EPSILON * 3.0f;

        pCluster->vCenterOfMass = GetClusterCOM( pCluster );
        for( i = 0; i < iNodeCount; ++i ) {
            vA = ( pCluster->arrNodes[i]->vPosition - pCluster->vCenterOfMass );
            vB = ( pCluster->arrFrames[i] );

            matRotScale.m00 += ( vA.X * vB.X ); matRotScale.m01 += ( vA.X * vB.Y ); matRotScale.m02 += ( vA.X * vB.Z );
            matRotScale.m10 += ( vA.Y * vB.X ); matRotScale.m11 += ( vA.Y * vB.Y ); matRotScale.m12 += ( vA.Y * vB.Z );
            matRotScale.m20 += ( vA.Z * vB.X ); matRotScale.m21 += ( vA.Z * vB.Y ); matRotScale.m22 += ( vA.Z * vB.Z );
        }

        PolarDecompose( matRotate, matScale, matRotScale );
        matRotate.Transpose( matInvRotate );

        pCluster->vTransform.SetRotate( matRotate );
        pCluster->vTransform.SetTranslate( pCluster->vCenterOfMass.ToVector() );

        // Inertia tensor
#if 1 // Use constant
        pCluster->matInvInertiaTensor = ( matRotate * pCluster->matLocalInvInertiaTensor * matInvRotate );
#else // Re-compute
        Scalar fMass;
        Matrix3 matInertiaTensor;
        matInertiaTensor.MakeNull();
        for( i = 0; i < iNodeCount; ++i ) {
            vA = ( pCluster->arrNodes[i]->vPosition - pCluster->vCenterOfMass );
            vB.X = vA.X * vA.X;
            vB.Y = vA.Y * vA.Y;
            vB.Z = vA.Z * vA.Z;
            fMass = pCluster->arrNodes[i]->fMass;
            matInertiaTensor.m00 += ( vB.Y + vB.Z ) * fMass;
            matInertiaTensor.m11 += ( vB.X + vB.Z ) * fMass;
            matInertiaTensor.m22 += ( vB.X + vB.Y ) * fMass;
            matInertiaTensor.m01 -= ( vA.X * vA.Y ) * fMass;
            matInertiaTensor.m02 -= ( vA.X * vA.Z ) * fMass;
            matInertiaTensor.m12 -= ( vA.Y * vA.Z ) * fMass;
        }
        matInertiaTensor.m10 = matInertiaTensor.m01;
        matInertiaTensor.m20 = matInertiaTensor.m02;
        matInertiaTensor.m21 = matInertiaTensor.m12;
        matInertiaTensor.Invert( pCluster->matInvInertiaTensor );
#endif

        // Velocities
        pCluster->vLinearVelocity = Vector3::Null;
        pCluster->vAngularVelocity = Vector3::Null;
        for( i = 0; i < iNodeCount; ++i ) {
            vVelocity = ( pCluster->arrNodes[i]->vVelocity * pCluster->arrMasses[i] );
            pCluster->vLinearVelocity += vVelocity;
            pCluster->vAngularVelocity += ( (pCluster->arrNodes[i]->vPosition - pCluster->vCenterOfMass) ^ vVelocity );
        }
        pCluster->vLinearVelocity *= ( pCluster->fInvMass * (1.0f - pCluster->fLinearDamping) );
        pCluster->vAngularVelocity = pCluster->matInvInertiaTensor * ( pCluster->vAngularVelocity * (1.0f - pCluster->fAngularDamping) );
        pCluster->vLinearImpulseV = Vector3::Null;
        pCluster->vAngularImpulseV = Vector3::Null;
        pCluster->vLinearImpulseD = Vector3::Null;
        pCluster->vAngularImpulseD = Vector3::Null;
        pCluster->iImpulseCountV = 0;
        pCluster->iImpulseCountD = 0;

        // Matching
        if ( pCluster->fMatching > 0.0f ) {
            for( i = 0; i < iNodeCount; ++i ) {
                vPosition = ( pCluster->vTransform * pCluster->arrFrames[i] );
                pCluster->arrNodes[i]->vPosition *= (1.0f - pCluster->fMatching);
                pCluster->arrNodes[i]->vPosition += ( vPosition * pCluster->fMatching );
            }
        }

        // Dynamic BVTree
        //if ( pCluster->bCollide ) {
        //    vA = pCluster->arrNodes[0]->vPosition;
        //    vB = vA;
        //    for( i = 1; i < iNodeCount; ++i ) {
        //        if ( pCluster->arrNodes[i]->vPosition.X < vA.X )
        //            vA.X = pCluster->arrNodes[i]->vPosition.X;
        //        if ( pCluster->arrNodes[i]->vPosition.Y < vA.Y )
        //            vA.Y = pCluster->arrNodes[i]->vPosition.Y;
        //        if ( pCluster->arrNodes[i]->vPosition.Z < vA.Z )
        //            vA.Z = pCluster->arrNodes[i]->vPosition.Z;
        //        if ( pCluster->arrNodes[i]->vPosition.X > vB.X )
        //            vB.X = pCluster->arrNodes[i]->vPosition.X;
        //        if ( pCluster->arrNodes[i]->vPosition.Y > vB.Y )
        //            vB.Y = pCluster->arrNodes[i]->vPosition.Y;
        //        if ( pCluster->arrNodes[i]->vPosition.Z > vB.Z )
        //            vB.Z = pCluster->arrNodes[i]->vPosition.Z;
        //    }
        //    ATTRIBUTE_ALIGNED16(btDbvtVolume) bounds = btDbvtVolume::FromMM( vA, vB );
        //    if ( pCluster->m_leaf != NULL ) {
        //        m_cdbvt.update( pCluster->m_leaf, bounds, pCluster->vLinearVelocity * m_hSolverState.fScaledDT * 3.0f,
        //                                                  m_hSolverState.fRadialMargin );
        //    } else
        //        pCluster->m_leaf = m_cdbvt.insert( bounds, pCluster );
        //}
    }
}

Void SoftBody::_Clusters_Initialize()
{
    UInt iClusterCount = m_arrClusters.Count();
    UInt i, iNodeCount;

    Cluster * pCluster;

    Matrix3 matInertiaTensor;
    Vector3 vA, vB;
    Scalar fMass;

    for( UInt iCluster = 0; iCluster < iClusterCount; ++iCluster ) {
        pCluster = m_arrClusters[iCluster];
        iNodeCount = pCluster->arrNodes.Count();

        // Masses
        pCluster->fInvMass = 0.0f;
        pCluster->arrMasses.Clear();
        for( i = 0; i < iNodeCount; ++i ) {
            if ( pCluster->arrNodes[i]->fInvMass > 0.0f )
                pCluster->arrMasses.Push( pCluster->arrNodes[i]->fMass );
            else {
                pCluster->arrMasses.Push( SCALAR_INFINITE );
                pCluster->bHasAnchor = true;
            }
            pCluster->fInvMass += pCluster->arrMasses[i];
        }
        pCluster->fInvMass = MathFn->Invert( pCluster->fInvMass );

        // State
        pCluster->vCenterOfMass = GetClusterCOM( pCluster );
        pCluster->vLinearVelocity = Vector3::Null;
        pCluster->vAngularVelocity = Vector3::Null;
        //pCluster->m_leaf = NULL;

        // Inertia tensor
        matInertiaTensor.MakeNull();
        for( i = 0; i < iNodeCount; ++i ) {
            vA = ( pCluster->arrNodes[i]->vPosition - pCluster->vCenterOfMass );
            vB.X = vA.X * vA.X;
            vB.Y = vA.Y * vA.Y;
            vB.Z = vA.Z * vA.Z;
            fMass = pCluster->arrMasses[i];
            matInertiaTensor.m00 += ( vB.Y + vB.Z ) * fMass;
            matInertiaTensor.m11 += ( vB.X + vB.Z ) * fMass;
            matInertiaTensor.m22 += ( vB.X + vB.Y ) * fMass;
            matInertiaTensor.m01 -= ( vA.X * vA.Y ) * fMass;
            matInertiaTensor.m02 -= ( vA.X * vA.Z ) * fMass;
            matInertiaTensor.m12 -= ( vA.Y * vA.Z ) * fMass;
        }
        matInertiaTensor.m10 = matInertiaTensor.m01;
        matInertiaTensor.m20 = matInertiaTensor.m02;
        matInertiaTensor.m21 = matInertiaTensor.m12;
        matInertiaTensor.Invert( pCluster->matLocalInvInertiaTensor );

        // Frame
        pCluster->vTransform.MakeIdentity();
        pCluster->vTransform.SetTranslate( pCluster->vCenterOfMass.ToVector() );

        pCluster->arrFrames.Clear();
        for( i = 0; i < iNodeCount; ++i )
            pCluster->arrFrames.Push( pCluster->arrNodes[i]->vPosition - pCluster->vCenterOfMass );
    }
}

Void SoftBody::_Clusters_Apply( Bool bDrift )
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt iClusterCount = m_arrClusters.Count();
    UInt i, j, iClusterNodeCount;

    Node * pNode;
    Cluster * pCluster;

    Vector3 vLinearImpulse;
    Vector3 vAngularImpulse;
    Vector3 vDelta;
    Scalar fInvImpulseCount;
    UInt iImpulseCount;

    UInt iNodeIndex;

    PhysicsFn->SelectMemory( TEXT("Scratch") );
    Vector3 * arrDeltas = New Vector3[iNodeCount];
    Scalar * arrWeights = New Scalar[iNodeCount];
    PhysicsFn->UnSelectMemory();

    for( i = 0; i < iNodeCount; ++i ) {
        arrDeltas[i] = Vector3::Null;
        arrWeights[i] = 0.0f;
    }

    if ( bDrift ) {
        for( i = 0; i < iClusterCount; ++i ) {
            pCluster = m_arrClusters[i];
            if ( pCluster->iImpulseCountD > 0 ) {
                fInvImpulseCount = MathFn->Invert( (Scalar)(pCluster->iImpulseCountD) );
                pCluster->vLinearImpulseD *= fInvImpulseCount;
                pCluster->vAngularImpulseD *= fInvImpulseCount;
            }
        }
    }

    for( i = 0; i < iClusterCount; ++i ) {
        pCluster = m_arrClusters[i];
        iImpulseCount = ( bDrift ) ? pCluster->iImpulseCountD : pCluster->iImpulseCountV;
        if ( iImpulseCount > 0 ) {
            vLinearImpulse = ( (bDrift) ? pCluster->vLinearImpulseD : pCluster->vLinearImpulseV ) * m_hSolverState.fScaledDT;
            vAngularImpulse = ( (bDrift) ? pCluster->vAngularImpulseD : pCluster->vAngularImpulseV ) * m_hSolverState.fScaledDT;

            iClusterNodeCount = pCluster->arrNodes.Count();
            for( j = 0; j < iClusterNodeCount; ++j ) {
                pNode = pCluster->arrNodes[j];
                iNodeIndex = ( pNode - &(m_arrNodes[0]) );
                vDelta = ( pNode->vPosition - pCluster->vCenterOfMass );
                arrDeltas[iNodeIndex] += ( vLinearImpulse + (vAngularImpulse ^ vDelta) ) * pCluster->arrMasses[j];
                arrWeights[iNodeIndex] += pCluster->arrMasses[j];
            }
        }
    }

    for( i = 0; i < iNodeCount; ++i ) {
        if ( arrWeights[i] > 0.0f )
            m_arrNodes[i].vPosition += ( arrDeltas[i] / arrWeights[i] );
    }

    PhysicsFn->SelectMemory( TEXT("Scratch") );
    DeleteA( arrWeights );
    DeleteA( arrDeltas );
    PhysicsFn->UnSelectMemory();
}
Void SoftBody::_Clusters_Damping()
{
    UInt iClusterCount = m_arrClusters.Count();
    UInt i, j, iNodeCount;

    Node * pNode;
    Cluster * pCluster;

    Vector3 vDelta;
    Vector3 vVelocity;

    for( i = 0; i < iClusterCount; ++i ) {
        pCluster = m_arrClusters[i];
        iNodeCount = pCluster->arrNodes.Count();

        if ( pCluster->fNodeDamping > 0.0f ) {
            for( j = 0; j < iNodeCount; ++j ) {
                pNode = pCluster->arrNodes[j];
                if ( pNode->fInvMass > 0.0f ) {
                    vDelta = ( pNode->vOldPosition - pCluster->vCenterOfMass );
                    vVelocity = ( pCluster->vLinearVelocity + (pCluster->vAngularVelocity ^ vDelta) );
                    if ( vVelocity.NormSqr() <= pNode->vVelocity.NormSqr() )
                        pNode->vVelocity += ( (vVelocity - pNode->vVelocity) * pCluster->fNodeDamping );
                }
            }
        }
    }
}

Void SoftBody::_Clusters_Prepare( UInt iIterations )
{
    UInt iJointCount = m_arrJoints.Count();
    UInt i;

    for( i = 0; i < iJointCount; ++i )
        m_arrJoints[i]->Prepare( m_hSolverState.fScaledDT, iIterations );
}
Void SoftBody::_Clusters_Solve( Scalar fSOR )
{
    UInt iJointCount = m_arrJoints.Count();
    UInt i;

    for( i = 0; i < iJointCount; ++i )
        m_arrJoints[i]->Solve( m_hSolverState.fScaledDT, fSOR );
}
Void SoftBody::_Clusters_Cleanup()
{
    UInt iJointCount = m_arrJoints.Count();
    UInt i;

    for( i = 0; i < iJointCount; ++i ) {
        m_arrJoints[i]->Finalize( m_hSolverState.fScaledDT );
        if ( m_arrJoints[i]->bDelete ) {
            PhysicsFn->SelectMemory( TEXT("SoftBodies") );
            Delete( m_arrJoints[i] );
            PhysicsFn->UnSelectMemory();

            m_arrJoints.Remove( i, NULL, 1 );
            iJointCount = m_arrJoints.Count();
            --i;
        }
    }
}

Void SoftBody::_ApplyForces()
{
    UInt iNodeCount = m_arrNodes.Count();
    UInt iAreaCount = m_arrAreas.Count();
    UInt i;

    Node * pNode;
    Area * pArea;

    Scalar fDT = m_hSolverState.fScaledDT;
    Scalar fLift = m_hConfig.fLift;
    Scalar fDrag = m_hConfig.fDrag;
    Scalar fPressure = m_hConfig.fPressure;
    Scalar fVolumeConversation = m_hConfig.fVolumeConversation;

    Bool bLift = ( fLift > 0.0f );
    Bool bDrag = ( fDrag > 0.0f );
    Bool bPressure = ( fPressure != 0.0f );
    Bool bVolumeConversation = ( fVolumeConversation > 0.0f );

    Bool bAeroDynamics = ( bLift || bDrag );
    Bool bAeroDynamicsV = bAeroDynamics && ( m_hConfig.iModel <= SOFTBODY_AERODYNAMICS_V_TwoSided );
    Bool bAeroDynamicsF = bAeroDynamics && ( m_hConfig.iModel > SOFTBODY_AERODYNAMICS_V_TwoSided );

    Bool bUseMedium = bAeroDynamics;
    Bool bUseVolume = ( bPressure || bVolumeConversation );

    Scalar fVolume = 0.0f;
    Scalar fIVolumeTP = 0.0f;
    Scalar fDVolumeTV = 0.0f;

    Medium curMedium;

    if ( bUseVolume ) {
        fVolume = GetVolume();
        fIVolumeTP = MathFn->Invert( fVolume ) * fPressure;
        fDVolumeTV = ( m_hPose.fRestVolume - fVolume ) * fVolumeConversation;
    }

    // Node forces
    Vector3 vRelVelocity, vNormal, vForce, vRelVelNormalized;
    Scalar fSqrRelVelocity, fDot, fAmplitude;

    for( i = 0; i < iNodeCount; ++i ) {
        pNode = &( m_arrNodes[i] );
        if ( pNode->fInvMass == 0.0f )
            continue;

        if ( bUseMedium ) {
            EvaluateMedium( &curMedium, pNode->vPosition );

            // Aero-Dynamics
            if ( bAeroDynamicsV ) {
                vRelVelocity = ( pNode->vVelocity - curMedium.vVelocity );
                fSqrRelVelocity = vRelVelocity.NormSqr();
                if ( fSqrRelVelocity > SCALAR_ERROR ) {
                    switch( m_hConfig.iModel ) {
                        case SOFTBODY_AERODYNAMICS_V_Point:
                            vNormal = vRelVelocity;
                            vNormal.Normalize();
                            break;
                        case SOFTBODY_AERODYNAMICS_V_OneSided:
                            vNormal = pNode->vNormal;
                            break;
                        case SOFTBODY_AERODYNAMICS_V_TwoSided:
                            vNormal = pNode->vNormal;
                            vNormal *= ( (vRelVelocity*vNormal) < 0.0f ) ? -1.0f : +1.0f;
                            break;
                        default: Assert(false); break;
                    }
                    fDot = ( vRelVelocity * vNormal );
                    if ( fDot > 0.0f ) {
                        vRelVelNormalized = vRelVelocity;
                        vRelVelNormalized.Normalize();

                        vForce = Vector3::Null;
                        fAmplitude = ( pNode->fArea * fDot * fSqrRelVelocity * 0.5f );
                        fAmplitude *= curMedium.fDensity;
                        vForce += ( vNormal * (-fAmplitude * fLift) );
                        vForce += ( vRelVelNormalized * (-fAmplitude * fDrag) );
                        ApplyClampedForce( pNode, vForce, fDT );
                    }
                }
            }
        }
        if ( bPressure )
            pNode->vForceBuffer += ( pNode->vNormal * (pNode->fArea * fIVolumeTP) );
        if ( bVolumeConversation )
            pNode->vForceBuffer += ( pNode->vNormal * (pNode->fArea * fDVolumeTV) );
    }

    // Area forces
    Vector3 vAreaVelocity;
    Vertex3 vAreaPosition;

    const Scalar fInv3 = MathFn->Invert( 3.0f );

    for( i = 0; i < iAreaCount; ++i ) {
        pArea = &( m_arrAreas[i] );

        // Aero-Dynamics
        if ( bAeroDynamicsF ) {
            vAreaVelocity = ( pArea->pNodeA->vVelocity + pArea->pNodeB->vVelocity + pArea->pNodeC->vVelocity ) / 3.0f;
            vAreaPosition = pArea->pNodeA->vPosition;
            vAreaPosition += pArea->pNodeB->vPosition.ToVector();
            vAreaPosition += pArea->pNodeC->vPosition.ToVector();
            vAreaPosition *= fInv3;

            EvaluateMedium( &curMedium, vAreaPosition );

            vRelVelocity = ( vAreaVelocity - curMedium.vVelocity );
            fSqrRelVelocity = vRelVelocity.NormSqr();
            if ( fSqrRelVelocity > SCALAR_ERROR ) {
                switch( m_hConfig.iModel ) {
                    case SOFTBODY_AERODYNAMICS_F_OneSided:
                        vNormal = pArea->vNormal;
                        break;
                    case SOFTBODY_AERODYNAMICS_F_TwoSided:
                        vNormal = pArea->vNormal;
                        vNormal *= ( (vRelVelocity*vNormal) < 0.0f ) ? -1.0f : +1.0f;
                        break;
                    default: Assert(false); break;
                }
                fDot = ( vRelVelocity * vNormal );
                if ( fDot > 0.0f ) {
                    vRelVelNormalized = vRelVelocity;
                    vRelVelNormalized.Normalize();

                    vForce = Vector3::Null;
                    fAmplitude = ( pArea->fRestArea * fDot * fSqrRelVelocity );
                    fAmplitude *= curMedium.fDensity;
                    vForce += ( vNormal * (-fAmplitude * fLift) );
                    vForce += ( vRelVelNormalized * (-fAmplitude * fDrag) );
                    vForce *= fInv3;
                    ApplyClampedForce( pArea->pNodeA, vForce, fDT );
                    ApplyClampedForce( pArea->pNodeB, vForce, fDT );
                    ApplyClampedForce( pArea->pNodeC, vForce, fDT );
                }
            }
        }
    }
}

*/