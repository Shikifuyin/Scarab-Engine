//
//
//
//17 #ifndef BT_GJP_EPA_PENETRATION_DEPTH_H
//18 #define BT_GJP_EPA_PENETRATION_DEPTH_H
//19 
//20 #include "btConvexPenetrationDepthSolver.h"
//21 
//24 class btGjkEpaPenetrationDepthSolver : public btConvexPenetrationDepthSolver
//25 {
//26  public :
//27 
//28  btGjkEpaPenetrationDepthSolver()
//29  {
//30  }
//31 
//32  bool calcPenDepth( btSimplexSolverInterface& simplexSolver,
//33  const btConvexShape* pConvexA, const btConvexShape* pConvexB,
//34  const btTransform& transformA, const btTransform& transformB,
//35  btVector3& v, btVector3& wWitnessOnA, btVector3& wWitnessOnB,
//36  class btIDebugDraw* debugDraw,btStackAlloc* stackAlloc );
//37 
//38  private :
//39 
//40 };
//41 
//42 #endif // BT_GJP_EPA_PENETRATION_DEPTH_H
//
//25 #ifndef BT_GJK_EPA2_H
//26 #define BT_GJK_EPA2_H
//27 
//28 #include "BulletCollision/CollisionShapes/btConvexShape.h"
//29 
//31 struct btGjkEpaSolver2
//32 {
//33 struct sResults
//34  {
//35  enum eStatus
//36  {
//37  Separated, /* Shapes doesnt penetrate */
//38  Penetrating, /* Shapes are penetrating */
//39  GJK_Failed, /* GJK phase fail, no big issue, shapes are probably just 'touching' */
//40  EPA_Failed /* EPA phase fail, bigger problem, need to save parameters, and debug */
//41  } status;
//42  btVector3 witnesses[2];
//43  btVector3 normal;
//44  btScalar distance;
//45  };
//46 
//47 static int StackSizeRequirement();
//48 
//49 static bool Distance( const btConvexShape* shape0,const btTransform& wtrs0,
//50  const btConvexShape* shape1,const btTransform& wtrs1,
//51  const btVector3& guess,
//52  sResults& results);
//53 
//54 static bool Penetration(const btConvexShape* shape0,const btTransform& wtrs0,
//55  const btConvexShape* shape1,const btTransform& wtrs1,
//56  const btVector3& guess,
//57  sResults& results,
//58  bool usemargins=true);
//59 #ifndef __SPU__
//60 static btScalar SignedDistance( const btVector3& position,
//61  btScalar margin,
//62  const btConvexShape* shape,
//63  const btTransform& wtrs,
//64  sResults& results);
//65 
//66 static bool SignedDistance( const btConvexShape* shape0,const btTransform& wtrs0,
//67  const btConvexShape* shape1,const btTransform& wtrs1,
//68  const btVector3& guess,
//69  sResults& results);
//70 #endif //__SPU__
//71 
//72 };
//73 
//74 #endif //BT_GJK_EPA2_H
//
//
//
//
//
//19 #ifndef BT_GJK_PAIR_DETECTOR_H
//20 #define BT_GJK_PAIR_DETECTOR_H
//21 
//22 #include "btDiscreteCollisionDetectorInterface.h"
//23 #include "BulletCollision/CollisionShapes/btCollisionMargin.h"
//24 
//25 class btConvexShape;
//26 #include "btSimplexSolverInterface.h"
//27 class btConvexPenetrationDepthSolver;
//28 
//30 class btGjkPairDetector : public btDiscreteCollisionDetectorInterface
//31 {
//32 
//33 
//34  btVector3 m_cachedSeparatingAxis;
//35  btConvexPenetrationDepthSolver* m_penetrationDepthSolver;
//36  btSimplexSolverInterface* m_simplexSolver;
//37  const btConvexShape* m_minkowskiA;
//38  const btConvexShape* m_minkowskiB;
//39  int m_shapeTypeA;
//40  int m_shapeTypeB;
//41  btScalar m_marginA;
//42  btScalar m_marginB;
//43 
//44  bool m_ignoreMargin;
//45  btScalar m_cachedSeparatingDistance;
//46 
//47 
//48 public:
//49 
//50  //some debugging to fix degeneracy problems
//51  int m_lastUsedMethod;
//52  int m_curIter;
//53  int m_degenerateSimplex;
//54  int m_catchDegeneracies;
//55 
//56 
//57  btGjkPairDetector(const btConvexShape* objectA,const btConvexShape* objectB,btSimplexSolverInterface* simplexSolver,btConvexPenetrationDepthSolver* penetrationDepthSolver);
//58  btGjkPairDetector(const btConvexShape* objectA,const btConvexShape* objectB,int shapeTypeA,int shapeTypeB,btScalar marginA, btScalar marginB, btSimplexSolverInterface* simplexSolver,btConvexPenetrationDepthSolver* penetrationDepthSolver);
//59  virtual ~btGjkPairDetector() {};
//60 
//61  virtual void getClosestPoints(const ClosestPointInput& input,Result& output,class btIDebugDraw* debugDraw,bool swapResults=false);
//62 
//63  void getClosestPointsNonVirtual(const ClosestPointInput& input,Result& output,class btIDebugDraw* debugDraw);
//64 
//65 
//66  void setMinkowskiA(const btConvexShape* minkA)
//67  {
//68  m_minkowskiA = minkA;
//69  }
//70 
//71  void setMinkowskiB(const btConvexShape* minkB)
//72  {
//73  m_minkowskiB = minkB;
//74  }
//75  void setCachedSeperatingAxis(const btVector3& seperatingAxis)
//76  {
//77  m_cachedSeparatingAxis = seperatingAxis;
//78  }
//79 
//80  const btVector3& getCachedSeparatingAxis() const
//81  {
//82  return m_cachedSeparatingAxis;
//83  }
//84  btScalar getCachedSeparatingDistance() const
//85  {
//86  return m_cachedSeparatingDistance;
//87  }
//88 
//89  void setPenetrationDepthSolver(btConvexPenetrationDepthSolver* penetrationDepthSolver)
//90  {
//91  m_penetrationDepthSolver = penetrationDepthSolver;
//92  }
//93 
//95  void setIgnoreMargin(bool ignoreMargin)
//96  {
//97  m_ignoreMargin = ignoreMargin;
//98  }
//99 
//100 
//101 };
//102 
//103 #endif //BT_GJK_PAIR_DETECTOR_H
//
//
//18 #ifndef BT_VORONOI_SIMPLEX_SOLVER_H
//19 #define BT_VORONOI_SIMPLEX_SOLVER_H
//20 
//21 #include "btSimplexSolverInterface.h"
//22 
//23 
//24 
//25 #define VORONOI_SIMPLEX_MAX_VERTS 5
//26 
//28 #define BT_USE_EQUAL_VERTEX_THRESHOLD
//29 #define VORONOI_DEFAULT_EQUAL_VERTEX_THRESHOLD 0.0001f
//30 
//31 
//32 struct btUsageBitfield{
//33  btUsageBitfield()
//34  {
//35  reset();
//36  }
//37 
//38  void reset()
//39  {
//40  usedVertexA = false;
//41  usedVertexB = false;
//42  usedVertexC = false;
//43  usedVertexD = false;
//44  }
//45  unsigned short usedVertexA : 1;
//46  unsigned short usedVertexB : 1;
//47  unsigned short usedVertexC : 1;
//48  unsigned short usedVertexD : 1;
//49  unsigned short unused1 : 1;
//50  unsigned short unused2 : 1;
//51  unsigned short unused3 : 1;
//52  unsigned short unused4 : 1;
//53 };
//54 
//55 
//56 struct btSubSimplexClosestResult
//57 {
//58  btVector3 m_closestPointOnSimplex;
//59  //MASK for m_usedVertices
//60  //stores the simplex vertex-usage, using the MASK,
//61  // if m_usedVertices & MASK then the related vertex is used
//62  btUsageBitfield m_usedVertices;
//63  btScalar m_barycentricCoords[4];
//64  bool m_degenerate;
//65 
//66  void reset()
//67  {
//68  m_degenerate = false;
//69  setBarycentricCoordinates();
//70  m_usedVertices.reset();
//71  }
//72  bool isValid()
//73  {
//74  bool valid = (m_barycentricCoords[0] >= btScalar(0.)) &&
//75  (m_barycentricCoords[1] >= btScalar(0.)) &&
//76  (m_barycentricCoords[2] >= btScalar(0.)) &&
//77  (m_barycentricCoords[3] >= btScalar(0.));
//78 
//79 
//80  return valid;
//81  }
//82  void setBarycentricCoordinates(btScalar a=btScalar(0.),btScalar b=btScalar(0.),btScalar c=btScalar(0.),btScalar d=btScalar(0.))
//83  {
//84  m_barycentricCoords[0] = a;
//85  m_barycentricCoords[1] = b;
//86  m_barycentricCoords[2] = c;
//87  m_barycentricCoords[3] = d;
//88  }
//89 
//90 };
//91 
//94 #ifdef NO_VIRTUAL_INTERFACE
//95 ATTRIBUTE_ALIGNED16(class) btVoronoiSimplexSolver
//96 #else
//97 ATTRIBUTE_ALIGNED16(class) btVoronoiSimplexSolver : public btSimplexSolverInterface
//98 #endif
//99 {
//100 public:
//101 
//102  BT_DECLARE_ALIGNED_ALLOCATOR();
//103 
//104  int m_numVertices;
//105 
//106  btVector3 m_simplexVectorW[VORONOI_SIMPLEX_MAX_VERTS];
//107  btVector3 m_simplexPointsP[VORONOI_SIMPLEX_MAX_VERTS];
//108  btVector3 m_simplexPointsQ[VORONOI_SIMPLEX_MAX_VERTS];
//109 
//110 
//111 
//112  btVector3 m_cachedP1;
//113  btVector3 m_cachedP2;
//114  btVector3 m_cachedV;
//115  btVector3 m_lastW;
//116 
//117  btScalar m_equalVertexThreshold;
//118  bool m_cachedValidClosest;
//119 
//120 
//121  btSubSimplexClosestResult m_cachedBC;
//122 
//123  bool m_needsUpdate;
//124 
//125  void removeVertex(int index);
//126  void reduceVertices (const btUsageBitfield& usedVerts);
//127  bool updateClosestVectorAndPoints();
//128 
//129  bool closestPtPointTetrahedron(const btVector3& p, const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& d, btSubSimplexClosestResult& finalResult);
//130  int pointOutsideOfPlane(const btVector3& p, const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& d);
//131  bool closestPtPointTriangle(const btVector3& p, const btVector3& a, const btVector3& b, const btVector3& c,btSubSimplexClosestResult& result);
//132 
//133 public:
//134 
//135  btVoronoiSimplexSolver()
//136  : m_equalVertexThreshold(VORONOI_DEFAULT_EQUAL_VERTEX_THRESHOLD)
//137  {
//138  }
//139  void reset();
//140 
//141  void addVertex(const btVector3& w, const btVector3& p, const btVector3& q);
//142 
//143  void setEqualVertexThreshold(btScalar threshold)
//144  {
//145  m_equalVertexThreshold = threshold;
//146  }
//147 
//148  btScalar getEqualVertexThreshold() const
//149  {
//150  return m_equalVertexThreshold;
//151  }
//152 
//153  bool closest(btVector3& v);
//154 
//155  btScalar maxVertex();
//156 
//157  bool fullSimplex() const
//158  {
//159  return (m_numVertices == 4);
//160  }
//161 
//162  int getSimplex(btVector3 *pBuf, btVector3 *qBuf, btVector3 *yBuf) const;
//163 
//164  bool inSimplex(const btVector3& w);
//165 
//166  void backup_closest(btVector3& v) ;
//167 
//168  bool emptySimplex() const ;
//169 
//170  void compute_points(btVector3& p1, btVector3& p2) ;
//171 
//172  int numVertices() const
//173  {
//174  return m_numVertices;
//175  }
//176 
//177 
//178 };
//179 
//180 #endif //BT_VORONOI_SIMPLEX_SOLVER_H
//
//
//
//16 #ifndef BT_MINKOWSKI_PENETRATION_DEPTH_SOLVER_H
//17 #define BT_MINKOWSKI_PENETRATION_DEPTH_SOLVER_H
//18 
//19 #include "btConvexPenetrationDepthSolver.h"
//20 
//23 class btMinkowskiPenetrationDepthSolver : public btConvexPenetrationDepthSolver
//24 {
//25 protected:
//26 
//27  static btVector3* getPenetrationDirections();
//28 
//29 public:
//30 
//31  virtual bool calcPenDepth( btSimplexSolverInterface& simplexSolver,
//32  const btConvexShape* convexA,const btConvexShape* convexB,
//33  const btTransform& transA,const btTransform& transB,
//34  btVector3& v, btVector3& pa, btVector3& pb,
//35  class btIDebugDraw* debugDraw,btStackAlloc* stackAlloc
//36  );
//37 };
//38 
//39 #endif //BT_MINKOWSKI_PENETRATION_DEPTH_SOLVER_H
//40 
//
//
//
//
//16 #ifndef BT_CONVEX_CONVEX_ALGORITHM_H
//17 #define BT_CONVEX_CONVEX_ALGORITHM_H
//18 
//19 #include "btActivatingCollisionAlgorithm.h"
//20 #include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
//21 #include "BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"
//22 #include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
//23 #include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
//24 #include "btCollisionCreateFunc.h"
//25 #include "btCollisionDispatcher.h"
//26 #include "LinearMath/btTransformUtil.h" //for btConvexSeparatingDistanceUtil
//27 
//28 class btConvexPenetrationDepthSolver;
//29 
//34 
//35 //#define USE_SEPDISTANCE_UTIL2 1
//36 
//40 class btConvexConvexAlgorithm : public btActivatingCollisionAlgorithm
//41 {
//42 #ifdef USE_SEPDISTANCE_UTIL2
//43  btConvexSeparatingDistanceUtil m_sepDistance;
//44 #endif
//45  btSimplexSolverInterface* m_simplexSolver;
//46  btConvexPenetrationDepthSolver* m_pdSolver;
//47 
//48 
//49  bool m_ownManifold;
//50  btPersistentManifold* m_manifoldPtr;
//51  bool m_lowLevelOfDetail;
//52 
//53  int m_numPerturbationIterations;
//54  int m_minimumPointsPerturbationThreshold;
//55 
//56 
//58 
//59 
//60 public:
//61 
//62  btConvexConvexAlgorithm(btPersistentManifold* mf,const btCollisionAlgorithmConstructionInfo& ci,const btCollisionObjectWrapper* body0Wrap,const btCollisionObjectWrapper* body1Wrap, btSimplexSolverInterface* simplexSolver, btConvexPenetrationDepthSolver* pdSolver, int numPerturbationIterations, int minimumPointsPerturbationThreshold);
//63 
//64  virtual ~btConvexConvexAlgorithm();
//65 
//66  virtual void processCollision (const btCollisionObjectWrapper* body0Wrap,const btCollisionObjectWrapper* body1Wrap,const btDispatcherInfo& dispatchInfo,btManifoldResult* resultOut);
//67 
//68  virtual btScalar calculateTimeOfImpact(btCollisionObject* body0,btCollisionObject* body1,const btDispatcherInfo& dispatchInfo,btManifoldResult* resultOut);
//69 
//70  virtual void getAllContactManifolds(btManifoldArray& manifoldArray)
//71  {
//73  if (m_manifoldPtr && m_ownManifold)
//74  manifoldArray.push_back(m_manifoldPtr);
//75  }
//76 
//77 
//78  void setLowLevelOfDetail(bool useLowLevel);
//79 
//80 
//81  const btPersistentManifold* getManifold()
//82  {
//83  return m_manifoldPtr;
//84  }
//85 
//86  struct CreateFunc :public btCollisionAlgorithmCreateFunc
//87  {
//88 
//89  btConvexPenetrationDepthSolver* m_pdSolver;
//90  btSimplexSolverInterface* m_simplexSolver;
//91  int m_numPerturbationIterations;
//92  int m_minimumPointsPerturbationThreshold;
//93 
//94  CreateFunc(btSimplexSolverInterface* simplexSolver, btConvexPenetrationDepthSolver* pdSolver);
//95 
//96  virtual ~CreateFunc();
//97 
//98  virtual btCollisionAlgorithm* CreateCollisionAlgorithm(btCollisionAlgorithmConstructionInfo& ci, const btCollisionObjectWrapper* body0Wrap,const btCollisionObjectWrapper* body1Wrap)
//99  {
//100  void* mem = ci.m_dispatcher1->allocateCollisionAlgorithm(sizeof(btConvexConvexAlgorithm));
//101  return new(mem) btConvexConvexAlgorithm(ci.m_manifold,ci,body0Wrap,body1Wrap,m_simplexSolver,m_pdSolver,m_numPerturbationIterations,m_minimumPointsPerturbationThreshold);
//102  }
//103  };
//104 
//105 
//106 };
//107 
//108 #endif //BT_CONVEX_CONVEX_ALGORITHM_H
//
