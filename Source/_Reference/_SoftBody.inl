/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/Systems/SoftBody.inl
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
/*
/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Impulse implementation
inline SoftBody::Impulse SoftBody::Impulse::operator-() const {
    Impulse iRes;
    iRes.vVelocity = -vVelocity;
    iRes.vDrift = -vDrift;
    iRes.bUseVelocity = bUseVelocity;
    iRes.bUseDrift = bUseDrift;
    return iRes;
}
inline SoftBody::Impulse & SoftBody::Impulse::operator*=( Scalar rhs ) {
    vVelocity *= rhs;
    vDrift *= rhs;
    return (*this);
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Body implementation
inline RigidBody * SoftBody::Body::GetRigidBody() const {
    return m_pRigid;
}
inline SoftBody::Cluster * SoftBody::Body::GetSoftCluster() const {
    return m_pSoft;
}

inline Scalar SoftBody::Body::GetInvMass() const {
    if ( m_pRigid != NULL )
        return m_pRigid->GetInvMass();
    if ( m_pSoft != NULL )
        return m_pSoft->fInvMass;
    return 0.0f;
}
inline const Matrix3 & SoftBody::Body::GetInvInertiaTensor() const {
    if ( m_pRigid != NULL )
        return m_pRigid->GetInvInertiaTensor();
    if ( m_pSoft != NULL )
        return m_pSoft->matInvInertiaTensor;
    return Matrix3::Null;
}

inline const Transform3 & SoftBody::Body::GetTransform() const {
    if ( m_pRigid != NULL )
        return m_pRigid->GetTransform();
    if ( m_pSoft != NULL )
        return m_pSoft->vTransform;
    return Transform3::Identity;
}

inline const Vector3 & SoftBody::Body::GetLinearSpeed() const {
    if ( m_pRigid != NULL )
        return m_pRigid->GetLinearSpeed();
    if ( m_pSoft != NULL )
        return m_pSoft->vLinearVelocity;
    return Vector3::Null;
}
inline const Vector3 & SoftBody::Body::GetAngularSpeed() const {
    if ( m_pRigid != NULL )
        return m_pRigid->GetAngularSpeed();
    if ( m_pSoft != NULL )
        return m_pSoft->vAngularVelocity;
    return Vector3::Null;
}
inline Vector3 SoftBody::Body::GetLinearSpeedAt( const Vector3 & vLocalPoint ) const {
    if ( m_pRigid != NULL )
        return m_pRigid->GetLinearSpeedAt( vLocalPoint );
    if ( m_pSoft != NULL )
        return m_pSoft->vLinearVelocity + ( m_pSoft->vAngularVelocity ^ vLocalPoint );
    return Vector3::Null;
}
inline Vector3 SoftBody::Body::GetAngularSpeedAt( const Vector3 & vLocalPoint ) const {
    if ( m_pRigid != NULL )
        return m_pRigid->GetAngularSpeedAt( vLocalPoint );
    if ( m_pSoft != NULL )
        return ( m_pSoft->vAngularVelocity ^ vLocalPoint );
    return Vector3::Null;
}

inline Void SoftBody::Body::ApplyLinearImpulseV( const Vector3 & vImpulse ) {
    if ( m_pRigid != NULL )
        return m_pRigid->ApplyLinearImpulse( vImpulse );
    if ( m_pSoft != NULL )
        SoftBody::ApplyClusterLinearImpulseV( m_pSoft, vImpulse );
}
inline Void SoftBody::Body::ApplyLinearImpulseD( const Vector3 & vImpulse ) {
    if ( m_pRigid != NULL )
        return m_pRigid->ApplyLinearImpulse( vImpulse );
    if ( m_pSoft != NULL )
        SoftBody::ApplyClusterLinearImpulseD( m_pSoft, vImpulse );
}
inline Void SoftBody::Body::ApplyLinearImpulse( const Impulse & vImpulse ) {
    if ( vImpulse.bUseVelocity )
        ApplyLinearImpulseV( vImpulse.vVelocity );
    if ( vImpulse.bUseDrift )
        ApplyLinearImpulseD( vImpulse.vDrift );
}

inline Void SoftBody::Body::ApplyAngularImpulseV( const Vector3 & vImpulse ) {
    if ( m_pRigid != NULL )
        return m_pRigid->ApplyAngularImpulse( vImpulse );
    if ( m_pSoft != NULL )
        SoftBody::ApplyClusterAngularImpulseV( m_pSoft, vImpulse );
}
inline Void SoftBody::Body::ApplyAngularImpulseD( const Vector3 & vImpulse ) {
    if ( m_pRigid != NULL )
        return m_pRigid->ApplyAngularImpulse( vImpulse );
    if ( m_pSoft != NULL )
        SoftBody::ApplyClusterAngularImpulseD( m_pSoft, vImpulse );
}
inline Void SoftBody::Body::ApplyAngularImpulse( const Impulse & vImpulse ) {
    if ( vImpulse.bUseVelocity )
        ApplyAngularImpulseV( vImpulse.vVelocity );
    if ( vImpulse.bUseDrift )
        ApplyAngularImpulseD( vImpulse.vDrift );
}

inline Void SoftBody::Body::ApplyImpulseV( const Vector3 & vImpulse, const Vector3 & vLocalPoint ) {
    if ( m_pRigid != NULL )
        m_pRigid->ApplyImpulse( vImpulse, vLocalPoint );
    if ( m_pSoft != NULL )
        SoftBody::ApplyClusterImpulseV( m_pSoft, vImpulse, vLocalPoint );
}
inline Void SoftBody::Body::ApplyImpulseD( const Vector3 & vImpulse, const Vector3 & vLocalPoint ) {
    if ( m_pRigid != NULL )
        m_pRigid->ApplyImpulse( vImpulse, vLocalPoint );
    if ( m_pSoft != NULL )
        SoftBody::ApplyClusterImpulseD( m_pSoft, vImpulse, vLocalPoint );
}
inline Void SoftBody::Body::ApplyImpulse( const Impulse & vImpulse, const Vector3 & vLocalPoint ) {
    if ( vImpulse.bUseVelocity )
        ApplyImpulseV( vImpulse.vVelocity, vLocalPoint );
    if ( vImpulse.bUseDrift )
        ApplyImpulseD( vImpulse.vDrift, vLocalPoint );
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::JointLinear implementation
inline SoftBodyJointType SoftBody::JointLinear::GetType() const {
    return SOFTBODY_JOINT_LINEAR;
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::JointAngular implementation
inline SoftBodyJointType SoftBody::JointAngular::GetType() const {
    return SOFTBODY_JOINT_ANGULAR;
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::JointContact implementation
inline SoftBodyJointType SoftBody::JointContact::GetType() const {
    return SOFTBODY_JOINT_CONTACT;
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody implementation
inline SystemBodyType SoftBody::GetType() const {
    return SYSTEMBODY_SOFTBODY;
}

inline SoftBodyEnvironment * SoftBody::GetEnvironment() const {
    return m_pEnvironment;
}

inline const BVAABox & SoftBody::GetAAB() const {
    return m_bvAABox;
}

inline Scalar SoftBody::GetMass( UInt iNode ) const {
    Assert( iNode <  m_arrNodes.Count() );
    return m_arrNodes[iNode].fMass;
}
inline Scalar SoftBody::GetMass() const {
    Scalar fTotalMass = 0.0f;
    UInt iNodeCount = m_arrNodes.Count();
    for( UInt i = 0; i < iNodeCount; ++i )
        fTotalMass += m_arrNodes[i].fMass;
    return fTotalMass;
}

inline Void SoftBody::SetMass( UInt iNode, Scalar fMass ) {
    Assert( iNode <  m_arrNodes.Count() );
    m_arrNodes[iNode].fMass = fMass;
    m_arrNodes[iNode].fInvMass = MathFn->Invert( fMass );
    m_bUpdateRuntimeConstants = true;
}

inline Void SoftBody::SetDensity( Scalar fDensity ) {
    SetMass( GetVolume() * fDensity, true );
}

inline Bool SoftBody::HasLink( UInt iNodeA, UInt iNodeB ) const {
    Assert( iNodeA < m_arrNodes.Count() );
    Assert( iNodeB < m_arrNodes.Count() );
    return HasLink( &(m_arrNodes[iNodeA]), &(m_arrNodes[iNodeB]) );
}
inline SoftBody::Link * SoftBody::PushLink( UInt iModel ) {
    Assert( iModel < m_arrLinks.Count() );
    m_arrLinks.Push( m_arrLinks[iModel] );
    return &( m_arrLinks[m_arrLinks.Count() - 1] );
}
inline SoftBody::Link * SoftBody::PushLink( UInt iNodeA, UInt iNodeB, Material * pMaterial ) {
    Assert( iNodeA < m_arrNodes.Count() );
    Assert( iNodeB < m_arrNodes.Count() );
    return PushLink( &(m_arrNodes[iNodeA]), &(m_arrNodes[iNodeB]), pMaterial );
}

inline Bool SoftBody::CutLink( UInt iNodeA, UInt iNodeB, Scalar fPosition ) {
    return CutLink( &(m_arrNodes[iNodeA]), &(m_arrNodes[iNodeB]), fPosition );
}

inline Bool SoftBody::HasArea( UInt iNodeA, UInt iNodeB, UInt iNodeC ) const {
    Assert( iNodeA < m_arrNodes.Count() );
    Assert( iNodeB < m_arrNodes.Count() );
    Assert( iNodeC < m_arrNodes.Count() );
    return HasArea( &(m_arrNodes[iNodeA]), &(m_arrNodes[iNodeB]), &(m_arrNodes[iNodeC]) );
}
inline SoftBody::Area * SoftBody::PushArea( UInt iModel ) {
    Assert( iModel < m_arrAreas.Count() );
    m_arrAreas.Push( m_arrAreas[iModel] );
    return &( m_arrAreas[m_arrAreas.Count() - 1] );
}
inline SoftBody::Area * SoftBody::PushArea( UInt iNodeA, UInt iNodeB, UInt iNodeC, Material * pMaterial ) {
    Assert( iNodeA < m_arrNodes.Count() );
    Assert( iNodeB < m_arrNodes.Count() );
    Assert( iNodeC < m_arrNodes.Count() );
    return PushArea( &(m_arrNodes[iNodeA]), &(m_arrNodes[iNodeB]), &(m_arrNodes[iNodeC]), pMaterial );
}

inline Bool SoftBody::HasVolume( UInt iNodeA, UInt iNodeB, UInt iNodeC, UInt iNodeD ) const {
    Assert( iNodeA < m_arrNodes.Count() );
    Assert( iNodeB < m_arrNodes.Count() );
    Assert( iNodeC < m_arrNodes.Count() );
    Assert( iNodeD < m_arrNodes.Count() );
    return HasVolume( &(m_arrNodes[iNodeA]), &(m_arrNodes[iNodeB]), &(m_arrNodes[iNodeC]), &(m_arrNodes[iNodeD]) );
}
inline SoftBody::Volume * SoftBody::PushVolume( UInt iModel ) {
    Assert( iModel < m_arrVolumes.Count() );
    m_arrVolumes.Push( m_arrVolumes[iModel] );
    return &( m_arrVolumes[m_arrVolumes.Count() - 1] );
}
inline SoftBody::Volume * SoftBody::PushVolume( UInt iNodeA, UInt iNodeB, UInt iNodeC, UInt iNodeD, Material * pMaterial ) {
    Assert( iNodeA < m_arrNodes.Count() );
    Assert( iNodeB < m_arrNodes.Count() );
    Assert( iNodeC < m_arrNodes.Count() );
    Assert( iNodeD < m_arrNodes.Count() );
    return PushVolume( &(m_arrNodes[iNodeA]), &(m_arrNodes[iNodeB]), &(m_arrNodes[iNodeC]), &(m_arrNodes[iNodeD]), pMaterial );
}

inline UInt SoftBody::GetClusterCount() const {
    return m_arrClusters.Count();
}

inline Vertex3 SoftBody::GetClusterCOM( UInt iCluster ) const {
    Assert( iCluster < m_arrClusters.Count() );
    return GetClusterCOM( m_arrClusters[iCluster] );
}

inline Vector3 SoftBody::GetClusterVelocityAt( const Cluster * pCluster, const Vector3 & vLocalPoint ) const {
    return pCluster->vLinearVelocity + ( pCluster->vAngularVelocity ^ vLocalPoint );
}

inline Void SoftBody::ApplyClusterLinearImpulseV( Cluster * pCluster, const Vector3 & vImpulse ) {
    Vector3 vLinear = ( vImpulse * pCluster->fInvMass );
    pCluster->vLinearVelocity += vLinear;
    pCluster->vLinearImpulseV += vLinear;
    ++(pCluster->iImpulseCountV);
}
inline Void SoftBody::ApplyClusterLinearImpulseD( Cluster * pCluster, const Vector3 & vImpulse ) {
    Vector3 vLinear = ( vImpulse * pCluster->fInvMass );
    pCluster->vLinearImpulseD += vLinear;
    ++(pCluster->iImpulseCountD);
}
inline Void SoftBody::ApplyClusterLinearImpulse( Cluster * pCluster, const Impulse & vImpulse ) {
    if ( vImpulse.bUseVelocity )
        ApplyClusterLinearImpulseV( pCluster, vImpulse.vVelocity );
    if ( vImpulse.bUseDrift )
        ApplyClusterLinearImpulseD( pCluster, vImpulse.vDrift );
}

inline Void SoftBody::ApplyClusterAngularImpulseV( Cluster * pCluster, const Vector3 & vImpulse ) {
    Vector3 vAngular = ( pCluster->matInvInertiaTensor * vImpulse );
    pCluster->vAngularVelocity += vAngular;
    pCluster->vAngularImpulseV += vAngular;
    ++(pCluster->iImpulseCountV);
}
inline Void SoftBody::ApplyClusterAngularImpulseD( Cluster * pCluster, const Vector3 & vImpulse ) {
    Vector3 vAngular = ( pCluster->matInvInertiaTensor * vImpulse );
    pCluster->vAngularImpulseD += vAngular;
    ++(pCluster->iImpulseCountD);
}
inline Void SoftBody::ApplyClusterAngularImpulse( Cluster * pCluster, const Impulse & vImpulse ) {
    if ( vImpulse.bUseVelocity )
        ApplyClusterAngularImpulseV( pCluster, vImpulse.vVelocity );
    if ( vImpulse.bUseDrift )
        ApplyClusterAngularImpulseD( pCluster, vImpulse.vDrift );
}

inline Void SoftBody::ApplyClusterImpulseV( Cluster * pCluster, const Vector3 & vImpulse, const Vector3 & vLocalPoint ) {
    Vector3 vLinear = ( vImpulse * pCluster->fInvMass );
    Vector3 vAngular = ( pCluster->matInvInertiaTensor * (vLocalPoint ^ vImpulse) );
    pCluster->vLinearVelocity += vLinear;
    pCluster->vAngularVelocity += vAngular;
    pCluster->vLinearImpulseV += vLinear;
    pCluster->vAngularImpulseV += vAngular;
    ++(pCluster->iImpulseCountV);
}
inline Void SoftBody::ApplyClusterImpulseD( Cluster * pCluster, const Vector3 & vImpulse, const Vector3 & vLocalPoint ) {
    Vector3 vLinear = ( vImpulse * pCluster->fInvMass );
    Vector3 vAngular = ( pCluster->matInvInertiaTensor * (vLocalPoint ^ vImpulse) );
    pCluster->vLinearImpulseD += vLinear;
    pCluster->vAngularImpulseD += vAngular;
    ++(pCluster->iImpulseCountD);
}
inline Void SoftBody::ApplyClusterImpulse( Cluster * pCluster, const Impulse & vImpulse, const Vector3 & vLocalPoint ) {
    if ( vImpulse.bUseVelocity )
        ApplyClusterImpulseV( pCluster, vImpulse.vVelocity, vLocalPoint );
    if ( vImpulse.bUseDrift )
        ApplyClusterImpulseD( pCluster, vImpulse.vDrift, vLocalPoint );
}

inline SoftBody::JointLinear * SoftBody::PushJoint( const JointLinearParameters & jointParams, const Body & rBody ) {
    return PushJoint( jointParams, m_arrClusters[0], rBody );
}
inline SoftBody::JointLinear * SoftBody::PushJoint( const JointLinearParameters & jointParams, SoftBody * pBody ) {
    return PushJoint( jointParams, m_arrClusters[0], pBody->m_arrClusters[0] );
}

inline SoftBody::JointAngular * SoftBody::PushJoint( const JointAngularParameters & jointParams, const Body & rBody ) {
    return PushJoint( jointParams, m_arrClusters[0], rBody );
}
inline SoftBody::JointAngular * SoftBody::PushJoint( const JointAngularParameters & jointParams, SoftBody * pBody ) {
    return PushJoint( jointParams, m_arrClusters[0], pBody->m_arrClusters[0] );
}

inline Void SoftBody::ApplyForce( UInt iNode, const Vector3 & vForce ) {
    Assert( iNode <  m_arrNodes.Count() );
    Node * pNode = &( m_arrNodes[iNode] );
    if ( pNode->fInvMass > 0.0f )
        pNode->vForceBuffer += vForce;
}
inline Void SoftBody::ApplyForce( const Vector3 & vForce ) {
    UInt iNodeCount = m_arrNodes.Count();
    for( UInt i = 0; i < iNodeCount; ++i )
        ApplyForce( i, vForce );
}

inline Void SoftBody::ApplyVelocity( UInt iNode, const Vector3 & vVelocity ) {
    Assert( iNode <  m_arrNodes.Count() );
    Node * pNode = &( m_arrNodes[iNode] );
    if ( pNode->fInvMass > 0.0f )
        pNode->vVelocity += vVelocity;
}
inline Void SoftBody::SetVelocity( UInt iNode, const Vector3 & vVelocity ) {
    Assert( iNode <  m_arrNodes.Count() );
    Node * pNode = &( m_arrNodes[iNode] );
    if ( pNode->fInvMass > 0.0f )
        pNode->vVelocity = vVelocity;
}
inline Void SoftBody::ApplyVelocity( const Vector3 & vVelocity ) {
    UInt iNodeCount = m_arrNodes.Count();
    for( UInt i = 0; i < iNodeCount; ++i )
        ApplyVelocity( i, vVelocity );
}
inline Void SoftBody::SetVelocity( const Vector3 & vVelocity ) {
    UInt iNodeCount = m_arrNodes.Count();
    for( UInt i = 0; i < iNodeCount; ++i )
        SetVelocity( i, vVelocity );
}
*/

//00206 static inline btVector3         ClampVector(const btVector3& v,btScalar maxlength)
//00207 {
//00208         const btScalar sql=v.length2();
//00209         if(sql>(maxlength*maxlength))
//00210                 return((v*maxlength)/btSqrt(sql));
//00211         else
//00212                 return(v);
//00213 }
//
//00235 static inline btScalar          ClusterMetric(const btVector3& x,const btVector3& y)
//00236 {
//00237         const btVector3 d=x-y;
//00238         return(btFabs(d[0])+btFabs(d[1])+btFabs(d[2]));
//00239 }
//
//00265 static inline btMatrix3x3       Diagonal(btScalar x)
//00266 {
//00267         btMatrix3x3     m;
//00268         m[0]=btVector3(x,0,0);
//00269         m[1]=btVector3(0,x,0);
//00270         m[2]=btVector3(0,0,x);
//00271         return(m);
//00272 }
//
//00298 static inline void                      Orthogonalize(btMatrix3x3& m)
//00299 {
//00300         m[2]=btCross(m[0],m[1]).normalized();
//00301         m[1]=btCross(m[2],m[0]).normalized();
//00302         m[0]=btCross(m[1],m[2]).normalized();
//00303 }
//
//00305 static inline btMatrix3x3       MassMatrix(btScalar im,const btMatrix3x3& iwi,const btVector3& r)
//00306 {
//00307         const btMatrix3x3       cr=Cross(r);
//00308         return(Sub(Diagonal(im),cr*iwi*cr));
//00309 }
//
//00312 static inline btMatrix3x3       ImpulseMatrix(  btScalar dt, btScalar ima, btScalar imb, const btMatrix3x3& iwi, const btVector3& r)
//00317 {
//00318         return(Diagonal(1/dt)*Add(Diagonal(ima),MassMatrix(imb,iwi,r)).inverse());
//00319 }
//
//00322 static inline btMatrix3x3       ImpulseMatrix(  btScalar ima,const btMatrix3x3& iia,const btVector3& ra,
//00323                                                 btScalar imb,const btMatrix3x3& iib,const btVector3& rb)      
//00324 {
//00325         return(Add(MassMatrix(ima,iia,ra),MassMatrix(imb,iib,rb)).inverse());
//00326 }
//
//00329 static inline btMatrix3x3       AngularImpulseMatrix(   const btMatrix3x3& iia, const btMatrix3x3& iib)
//00331 {
//00332         return(Add(iia,iib).inverse());
//00333 }
//
//00336 static inline btVector3         ProjectOnAxis(  const btVector3& v, const btVector3& a)
//00338 {
//00339         return(a*btDot(v,a));
//00340 }
//
//00342 static inline btVector3         ProjectOnPlane( const btVector3& v, const btVector3& a)
//00344 {
//00345         return(v-ProjectOnAxis(v,a));
//00346 }
//
//00403 template <typename T>
//00404 static inline T BaryEval( const T& a, const T& b, const T& c, const btVector3& coord)
//00408 {
//00409         return(a*coord.x()+b*coord.y()+c*coord.z());
//00410 }
//
//00412 static inline btVector3 BaryCoord( const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& p)
//00416 {
//00417         const btScalar  w[]={   btCross(a-p,b-p).length(),
//00418                 btCross(b-p,c-p).length(),
//00419                 btCross(c-p,a-p).length()};
//00420         const btScalar  isum=1/(w[0]+w[1]+w[2]);
//00421         return(btVector3(w[1]*isum,w[2]*isum,w[0]*isum));
//00422 }
//
//00425 static btScalar ImplicitSolve(  btSoftBody::ImplicitFn* fn, const btVector3& a, const btVector3& b,
//00428                                 const btScalar accuracy, const int maxiterations=256)
//00430 {
//00431         btScalar        span[2]={0,1};
//00432         btScalar        values[2]={fn->Eval(a),fn->Eval(b)};
//00433         if(values[0]>values[1])
//00434         {
//00435                 btSwap(span[0],span[1]);
//00436                 btSwap(values[0],values[1]);
//00437         }
//00438         if(values[0]>-accuracy) return(-1);
//00439         if(values[1]<+accuracy) return(-1);
//00440         for(int i=0;i<maxiterations;++i)
//00441         {
//00442                 const btScalar  t=Lerp(span[0],span[1],values[0]/(values[0]-values[1]));
//00443                 const btScalar  v=fn->Eval(Lerp(a,b,t));
//00444                 if((t<=0)||(t>=1))              break;
//00445                 if(btFabs(v)<accuracy)  return(t);
//00446                 if(v<0)
//00447                 { span[0]=t;values[0]=v; }
//00448                 else
//00449                 { span[1]=t;values[1]=v; }
//00450         }
//00451         return(-1);
//00452 }
//
//00465 static inline btDbvtVolume VolumeOf( const btSoftBody::Face& f,  btScalar margin)
//00467 {
//00468         const btVector3*        pts[]={ &f.m_n[0]->m_x,
//00469                 &f.m_n[1]->m_x,
//00470                 &f.m_n[2]->m_x};
//00471         btDbvtVolume            vol=btDbvtVolume::FromPoints(pts,3);
//00472         vol.Expand(btVector3(margin,margin,margin));
//00473         return(vol);
//00474 }
//
//00477 static inline btVector3 CenterOf( const btSoftBody::Face& f)
//00478 {
//00479         return((f.m_n[0]->m_x+f.m_n[1]->m_x+f.m_n[2]->m_x)/3);
//00480 }
//
//00483 static inline btScalar AreaOf( const btVector3& x0, const btVector3& x1, const btVector3& x2)
//00486 {
//00487         const btVector3 a=x1-x0;
//00488         const btVector3 b=x2-x0;
//00489         const btVector3 cr=btCross(a,b);
//00490         const btScalar  area=cr.length();
//00491         return(area);
//00492 }
//
//00495 static inline btScalar VolumeOf( const btVector3& x0, const btVector3& x1, const btVector3& x2, const btVector3& x3)
//00499 {
//00500         const btVector3 a=x1-x0;
//00501         const btVector3 b=x2-x0;
//00502         const btVector3 c=x3-x0;
//00503         return(btDot(a,btCross(b,c)));
//00504 }
//
//00507 static void EvaluateMedium( const btSoftBodyWorldInfo* wfi, const btVector3& x, btSoftBody::sMedium& medium)
//00510 {
//00511         medium.m_velocity       =       btVector3(0,0,0);
//00512         medium.m_pressure       =       0;
//00513         medium.m_density        =       wfi->air_density;
//00514         if(wfi->water_density>0)
//00515         {
//00516                 const btScalar  depth=-(btDot(x,wfi->water_normal)+wfi->water_offset);
//00517                 if(depth>0)
//00518                 {
//00519                         medium.m_density        =       wfi->water_density;
//00520                         medium.m_pressure       =       depth*wfi->water_density*wfi->m_gravity.length();
//00521                 }
//00522         }
//00523 }
//
//00526 static inline void ApplyClampedForce( btSoftBody::Node& n, const btVector3& f, btScalar dt)
//00529 {
//00530         const btScalar  dtim=dt*n.m_im;
//00531         if((f*dtim).length2()>n.m_v.length2())
//00532         {/* Clamp       */ 
//00533                 n.m_f-=ProjectOnAxis(n.m_v,f.normalized())/dtim;                                                
//00534         }
//00535         else
//00536         {/* Apply       */ 
//00537                 n.m_f+=f;
//00538         }
//00539 }
//
//00542 static inline int MatchEdge( const btSoftBody::Node* a, const btSoftBody::Node* b,
//00544                              const btSoftBody::Node* ma, const btSoftBody::Node* mb)
//00546 {
//00547         if((a==ma)&&(b==mb)) return(0);
//00548         if((a==mb)&&(b==ma)) return(1);
//00549         return(-1);
//00550 }
//
//00616 static inline int PolarDecompose( const btMatrix3x3& m,btMatrix3x3& q,btMatrix3x3& s)
//00617 {
//00618         static const btScalar   half=(btScalar)0.5;
//00619         static const btScalar   accuracy=(btScalar)0.0001;
//00620         static const int                maxiterations=16;
//00621         int                                             i=0;
//00622         btScalar                                det=0;
//00623         q       =       Mul(m,1/btVector3(m[0][0],m[1][1],m[2][2]).length());
//00624         det     =       q.determinant();
//00625         if(!btFuzzyZero(det))
//00626         {
//00627                 for(;i<maxiterations;++i)
//00628                 {
//00629                         q=Mul(Add(q,Mul(q.adjoint(),1/det).transpose()),half);
//00630                         const btScalar  ndet=q.determinant();
//00631                         if(Sq(ndet-det)>accuracy) det=ndet; else break;
//00632                 }
//00633                 /* Final orthogonalization      */ 
//00634                 Orthogonalize(q);
//00635                 /* Compute 'S'                          */ 
//00636                 s=q.transpose()*m;
//00637         }
//00638         else
//00639         {
//00640                 q.setIdentity();
//00641                 s.setIdentity();
//00642         }
//00643         return(i);
//00644 }
//
//00557 struct  btEigen
//00558 {
//00559         static int                      system(btMatrix3x3& a,btMatrix3x3* vectors,btVector3* values=0)
//00560         {
//00561                 static const int                maxiterations=16;
//00562                 static const btScalar   accuracy=(btScalar)0.0001;
//00563                 btMatrix3x3&                    v=*vectors;
//00564                 int                                             iterations=0;
//00565                 vectors->setIdentity();
//00566                 do      {
//00567                         int                             p=0,q=1;
//00568                         if(btFabs(a[p][q])<btFabs(a[0][2])) { p=0;q=2; }
//00569                         if(btFabs(a[p][q])<btFabs(a[1][2])) { p=1;q=2; }
//00570                         if(btFabs(a[p][q])>accuracy)
//00571                         {
//00572                                 const btScalar  w=(a[q][q]-a[p][p])/(2*a[p][q]);
//00573                                 const btScalar  z=btFabs(w);
//00574                                 const btScalar  t=w/(z*(btSqrt(1+w*w)+z));
//00575                                 if(t==t)/* [WARNING] let hope that one does not get thrown aways by some compilers... */ 
//00576                                 {
//00577                                         const btScalar  c=1/btSqrt(t*t+1);
//00578                                         const btScalar  s=c*t;
//00579                                         mulPQ(a,c,s,p,q);
//00580                                         mulTPQ(a,c,s,p,q);
//00581                                         mulPQ(v,c,s,p,q);
//00582                                 } else break;
//00583                         } else break;
//00584                 } while((++iterations)<maxiterations);
//00585                 if(values)
//00586                 {
//00587                         *values=btVector3(a[0][0],a[1][1],a[2][2]);
//00588                 }
//00589                 return(iterations);
//00590         }
//00591 private:
//00592         static inline void      mulTPQ(btMatrix3x3& a,btScalar c,btScalar s,int p,int q)
//00593         {
//00594                 const btScalar  m[2][3]={       {a[p][0],a[p][1],a[p][2]},
//00595                 {a[q][0],a[q][1],a[q][2]}};
//00596                 int i;
//00597 
//00598                 for(i=0;i<3;++i) a[p][i]=c*m[0][i]-s*m[1][i];
//00599                 for(i=0;i<3;++i) a[q][i]=c*m[1][i]+s*m[0][i];
//00600         }
//00601         static inline void      mulPQ(btMatrix3x3& a,btScalar c,btScalar s,int p,int q)
//00602         {
//00603                 const btScalar  m[2][3]={       {a[0][p],a[1][p],a[2][p]},
//00604                 {a[0][q],a[1][q],a[2][q]}};
//00605                 int i;
//00606 
//00607                 for(i=0;i<3;++i) a[i][p]=c*m[0][i]-s*m[1][i];
//00608                 for(i=0;i<3;++i) a[i][q]=c*m[1][i]+s*m[0][i];
//00609         }
//00610 };
//
//00646 //
//00647 // btSoftColliders
//00648 //
//00649 struct btSoftColliders
//00650 {
//00651         //
//00652         // ClusterBase
//00653         //
//00654         struct  ClusterBase : btDbvt::ICollide
//00655         {
//00656                 btScalar                        erp;
//00657                 btScalar                        idt;
//00658                 btScalar                        m_margin;
//00659                 btScalar                        friction;
//00660                 btScalar                        threshold;
//00661                 ClusterBase()
//00662                 {
//00663                         erp                     =(btScalar)1;
//00664                         idt                     =0;
//00665                         m_margin                =0;
//00666                         friction        =0;
//00667                         threshold       =(btScalar)0;
//00668                 }
//00669                 bool                            SolveContact(   const btGjkEpaSolver2::sResults& res,
//00670                         btSoftBody::Body ba,btSoftBody::Body bb,
//00671                         btSoftBody::CJoint& joint)
//00672                 {
//00673                         if(res.distance<m_margin)
//00674                         {
//00675                                 btVector3 norm = res.normal;
//00676                                 norm.normalize();//is it necessary?
//00677 
//00678                                 const btVector3         ra=res.witnesses[0]-ba.xform().getOrigin();
//00679                                 const btVector3         rb=res.witnesses[1]-bb.xform().getOrigin();
//00680                                 const btVector3         va=ba.velocity(ra);
//00681                                 const btVector3         vb=bb.velocity(rb);
//00682                                 const btVector3         vrel=va-vb;
//00683                                 const btScalar          rvac=btDot(vrel,norm);
//00684                                  btScalar               depth=res.distance-m_margin;
//00685                                 
//00686 //                              printf("depth=%f\n",depth);
//00687                                 const btVector3         iv=norm*rvac;
//00688                                 const btVector3         fv=vrel-iv;
//00689                                 joint.m_bodies[0]       =       ba;
//00690                                 joint.m_bodies[1]       =       bb;
//00691                                 joint.m_refs[0]         =       ra*ba.xform().getBasis();
//00692                                 joint.m_refs[1]         =       rb*bb.xform().getBasis();
//00693                                 joint.m_rpos[0]         =       ra;
//00694                                 joint.m_rpos[1]         =       rb;
//00695                                 joint.m_cfm                     =       1;
//00696                                 joint.m_erp                     =       1;
//00697                                 joint.m_life            =       0;
//00698                                 joint.m_maxlife         =       0;
//00699                                 joint.m_split           =       1;
//00700                                 
//00701                                 joint.m_drift           =       depth*norm;
//00702 
//00703                                 joint.m_normal          =       norm;
//00704 //                              printf("normal=%f,%f,%f\n",res.normal.getX(),res.normal.getY(),res.normal.getZ());
//00705                                 joint.m_delete          =       false;
//00706                                 joint.m_friction        =       fv.length2()<(-rvac*friction)?1:friction;
//00707                                 joint.m_massmatrix      =       ImpulseMatrix(  ba.invMass(),ba.invWorldInertia(),joint.m_rpos[0],
//00708                                         bb.invMass(),bb.invWorldInertia(),joint.m_rpos[1]);
//00709 
//00710                                 return(true);
//00711                         }
//00712                         return(false);
//00713                 }
//00714         };
//00715         //
//00716         // CollideCL_RS
//00717         //
//00718         struct  CollideCL_RS : ClusterBase
//00719         {
//00720                 btSoftBody*             psb;
//00721                 
//00722                 btCollisionObject*      m_colObj;
//00723                 void            Process(const btDbvtNode* leaf)
//00724                 {
//00725                         btSoftBody::Cluster*            cluster=(btSoftBody::Cluster*)leaf->data;
//00726                         btSoftClusterCollisionShape     cshape(cluster);
//00727                         
//00728                         const btConvexShape*            rshape=(const btConvexShape*)m_colObj->getCollisionShape();
//00729 
//00731                         if(m_colObj->isStaticOrKinematicObject() && cluster->m_containsAnchor)
//00732                                 return;
//00733 
//00734                         btGjkEpaSolver2::sResults       res;            
//00735                         if(btGjkEpaSolver2::SignedDistance(     &cshape,btTransform::getIdentity(),
//00736                                 rshape,m_colObj->getInterpolationWorldTransform(),
//00737                                 btVector3(1,0,0),res))
//00738                         {
//00739                                 btSoftBody::CJoint      joint;
//00740                                 if(SolveContact(res,cluster,m_colObj,joint))//prb,joint))
//00741                                 {
//00742                                         btSoftBody::CJoint*     pj=new(btAlignedAlloc(sizeof(btSoftBody::CJoint),16)) btSoftBody::CJoint();
//00743                                         *pj=joint;psb->m_joints.push_back(pj);
//00744                                         if(m_colObj->isStaticOrKinematicObject())
//00745                                         {
//00746                                                 pj->m_erp       *=      psb->m_cfg.kSKHR_CL;
//00747                                                 pj->m_split     *=      psb->m_cfg.kSK_SPLT_CL;
//00748                                         }
//00749                                         else
//00750                                         {
//00751                                                 pj->m_erp       *=      psb->m_cfg.kSRHR_CL;
//00752                                                 pj->m_split     *=      psb->m_cfg.kSR_SPLT_CL;
//00753                                         }
//00754                                 }
//00755                         }
//00756                 }
//00757                 void            Process(btSoftBody* ps,btCollisionObject* colOb)
//00758                 {
//00759                         psb                     =       ps;
//00760                         m_colObj                        =       colOb;
//00761                         idt                     =       ps->m_sst.isdt;
//00762                         m_margin                =       m_colObj->getCollisionShape()->getMargin()+psb->getCollisionShape()->getMargin();
//00764                         friction        =       btMin(psb->m_cfg.kDF,m_colObj->getFriction());
//00765                         btVector3                       mins;
//00766                         btVector3                       maxs;
//00767 
//00768                         ATTRIBUTE_ALIGNED16(btDbvtVolume)               volume;
//00769                         colOb->getCollisionShape()->getAabb(colOb->getInterpolationWorldTransform(),mins,maxs);
//00770                         volume=btDbvtVolume::FromMM(mins,maxs);
//00771                         volume.Expand(btVector3(1,1,1)*m_margin);
//00772                         ps->m_cdbvt.collideTV(ps->m_cdbvt.m_root,volume,*this);
//00773                 }       
//00774         };
//00775         //
//00776         // CollideCL_SS
//00777         //
//00778         struct  CollideCL_SS : ClusterBase
//00779         {
//00780                 btSoftBody*     bodies[2];
//00781                 void            Process(const btDbvtNode* la,const btDbvtNode* lb)
//00782                 {
//00783                         btSoftBody::Cluster*            cla=(btSoftBody::Cluster*)la->data;
//00784                         btSoftBody::Cluster*            clb=(btSoftBody::Cluster*)lb->data;
//00785 
//00786 
//00787                         bool connected=false;
//00788                         if ((bodies[0]==bodies[1])&&(bodies[0]->m_clusterConnectivity.size()))
//00789                         {
//00790                                 connected = bodies[0]->m_clusterConnectivity[cla->m_clusterIndex+bodies[0]->m_clusters.size()*clb->m_clusterIndex];
//00791                         }
//00792 
//00793                         if (!connected)
//00794                         {
//00795                                 btSoftClusterCollisionShape     csa(cla);
//00796                                 btSoftClusterCollisionShape     csb(clb);
//00797                                 btGjkEpaSolver2::sResults       res;            
//00798                                 if(btGjkEpaSolver2::SignedDistance(     &csa,btTransform::getIdentity(),
//00799                                         &csb,btTransform::getIdentity(),
//00800                                         cla->m_com-clb->m_com,res))
//00801                                 {
//00802                                         btSoftBody::CJoint      joint;
//00803                                         if(SolveContact(res,cla,clb,joint))
//00804                                         {
//00805                                                 btSoftBody::CJoint*     pj=new(btAlignedAlloc(sizeof(btSoftBody::CJoint),16)) btSoftBody::CJoint();
//00806                                                 *pj=joint;bodies[0]->m_joints.push_back(pj);
//00807                                                 pj->m_erp       *=      btMax(bodies[0]->m_cfg.kSSHR_CL,bodies[1]->m_cfg.kSSHR_CL);
//00808                                                 pj->m_split     *=      (bodies[0]->m_cfg.kSS_SPLT_CL+bodies[1]->m_cfg.kSS_SPLT_CL)/2;
//00809                                         }
//00810                                 }
//00811                         } else
//00812                         {
//00813                                 static int count=0;
//00814                                 count++;
//00815                                 //printf("count=%d\n",count);
//00816                                 
//00817                         }
//00818                 }
//00819                 void            Process(btSoftBody* psa,btSoftBody* psb)
//00820                 {
//00821                         idt                     =       psa->m_sst.isdt;
//00822                         //m_margin              =       (psa->getCollisionShape()->getMargin()+psb->getCollisionShape()->getMargin())/2;
//00823                         m_margin                =       (psa->getCollisionShape()->getMargin()+psb->getCollisionShape()->getMargin());
//00824                         friction        =       btMin(psa->m_cfg.kDF,psb->m_cfg.kDF);
//00825                         bodies[0]       =       psa;
//00826                         bodies[1]       =       psb;
//00827                         psa->m_cdbvt.collideTT(psa->m_cdbvt.m_root,psb->m_cdbvt.m_root,*this);
//00828                 }       
//00829         };
//00830         //
//00831         // CollideSDF_RS
//00832         //
//00833         struct  CollideSDF_RS : btDbvt::ICollide
//00834         {
//00835                 void            Process(const btDbvtNode* leaf)
//00836                 {
//00837                         btSoftBody::Node*       node=(btSoftBody::Node*)leaf->data;
//00838                         DoNode(*node);
//00839                 }
//00840                 void            DoNode(btSoftBody::Node& n) const
//00841                 {
//00842                         const btScalar                  m=n.m_im>0?dynmargin:stamargin;
//00843                         btSoftBody::RContact    c;
//00844                         if(     (!n.m_battach)&&
//00845                                 psb->checkContact(m_colObj1,n.m_x,m,c.m_cti))
//00846                         {
//00847                                 const btScalar  ima=n.m_im;
//00848                                 const btScalar  imb= m_rigidBody? m_rigidBody->getInvMass() : 0.f;
//00849                                 const btScalar  ms=ima+imb;
//00850                                 if(ms>0)
//00851                                 {
//00852                                         const btTransform&      wtr=m_rigidBody?m_rigidBody->getInterpolationWorldTransform() : m_colObj1->getWorldTransform();
//00853                                         static const btMatrix3x3        iwiStatic(0,0,0,0,0,0,0,0,0);
//00854                                         const btMatrix3x3&      iwi=m_rigidBody?m_rigidBody->getInvInertiaTensorWorld() : iwiStatic;
//00855                                         const btVector3         ra=n.m_x-wtr.getOrigin();
//00856                                         const btVector3         va=m_rigidBody ? m_rigidBody->getVelocityInLocalPoint(ra)*psb->m_sst.sdt : btVector3(0,0,0);
//00857                                         const btVector3         vb=n.m_x-n.m_q; 
//00858                                         const btVector3         vr=vb-va;
//00859                                         const btScalar          dn=btDot(vr,c.m_cti.m_normal);
//00860                                         const btVector3         fv=vr-c.m_cti.m_normal*dn;
//00861                                         const btScalar          fc=psb->m_cfg.kDF*m_colObj1->getFriction();
//00862                                         c.m_node        =       &n;
//00863                                         c.m_c0          =       ImpulseMatrix(psb->m_sst.sdt,ima,imb,iwi,ra);
//00864                                         c.m_c1          =       ra;
//00865                                         c.m_c2          =       ima*psb->m_sst.sdt;
//00866                                         c.m_c3          =       fv.length2()<(btFabs(dn)*fc)?0:1-fc;
//00867                                         c.m_c4          =       m_colObj1->isStaticOrKinematicObject()?psb->m_cfg.kKHR:psb->m_cfg.kCHR;
//00868                                         psb->m_rcontacts.push_back(c);
//00869                                         if (m_rigidBody)
//00870                                                 m_rigidBody->activate();
//00871                                 }
//00872                         }
//00873                 }
//00874                 btSoftBody*             psb;
//00875                 btCollisionObject*      m_colObj1;
//00876                 btRigidBody*    m_rigidBody;
//00877                 btScalar                dynmargin;
//00878                 btScalar                stamargin;
//00879         };
//00880         //
//00881         // CollideVF_SS
//00882         //
//00883         struct  CollideVF_SS : btDbvt::ICollide
//00884         {
//00885                 void            Process(const btDbvtNode* lnode,
//00886                         const btDbvtNode* lface)
//00887                 {
//00888                         btSoftBody::Node*       node=(btSoftBody::Node*)lnode->data;
//00889                         btSoftBody::Face*       face=(btSoftBody::Face*)lface->data;
//00890                         btVector3                       o=node->m_x;
//00891                         btVector3                       p;
//00892                         btScalar                        d=SIMD_INFINITY;
//00893                         ProjectOrigin(  face->m_n[0]->m_x-o,
//00894                                 face->m_n[1]->m_x-o,
//00895                                 face->m_n[2]->m_x-o,
//00896                                 p,d);
//00897                         const btScalar  m=mrg+(o-node->m_q).length()*2;
//00898                         if(d<(m*m))
//00899                         {
//00900                                 const btSoftBody::Node* n[]={face->m_n[0],face->m_n[1],face->m_n[2]};
//00901                                 const btVector3                 w=BaryCoord(n[0]->m_x,n[1]->m_x,n[2]->m_x,p+o);
//00902                                 const btScalar                  ma=node->m_im;
//00903                                 btScalar                                mb=BaryEval(n[0]->m_im,n[1]->m_im,n[2]->m_im,w);
//00904                                 if(     (n[0]->m_im<=0)||
//00905                                         (n[1]->m_im<=0)||
//00906                                         (n[2]->m_im<=0))
//00907                                 {
//00908                                         mb=0;
//00909                                 }
//00910                                 const btScalar  ms=ma+mb;
//00911                                 if(ms>0)
//00912                                 {
//00913                                         btSoftBody::SContact    c;
//00914                                         c.m_normal              =       p/-btSqrt(d);
//00915                                         c.m_margin              =       m;
//00916                                         c.m_node                =       node;
//00917                                         c.m_face                =       face;
//00918                                         c.m_weights             =       w;
//00919                                         c.m_friction    =       btMax(psb[0]->m_cfg.kDF,psb[1]->m_cfg.kDF);
//00920                                         c.m_cfm[0]              =       ma/ms*psb[0]->m_cfg.kSHR;
//00921                                         c.m_cfm[1]              =       mb/ms*psb[1]->m_cfg.kSHR;
//00922                                         psb[0]->m_scontacts.push_back(c);
//00923                                 }
//00924                         }       
//00925                 }
//00926                 btSoftBody*             psb[2];
//00927                 btScalar                mrg;
//00928         };
//00929 };
