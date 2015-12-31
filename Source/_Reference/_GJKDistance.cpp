//
//
//8 #include "BulletCollision/CollisionShapes/btConvexShape.h"
//19 #include "btGjkEpaPenetrationDepthSolver.h"
//20 
//21 
//22 #include "BulletCollision/NarrowPhaseCollision/btGjkEpa2.h"
//23 
//24 bool btGjkEpaPenetrationDepthSolver::calcPenDepth( btSimplexSolverInterface& simplexSolver,
//25  const btConvexShape* pConvexA, const btConvexShape* pConvexB,
//26  const btTransform& transformA, const btTransform& transformB,
//27  btVector3& v, btVector3& wWitnessOnA, btVector3& wWitnessOnB,
//28  class btIDebugDraw* debugDraw, btStackAlloc* stackAlloc )
//29 {
//30 
//31  (void)debugDraw;
//32  (void)v;
//33  (void)simplexSolver;
//34 
//35 // const btScalar radialmargin(btScalar(0.));
//36 
//37  btVector3 guessVector(transformA.getOrigin()-transformB.getOrigin());
//38  btGjkEpaSolver2::sResults results;
//39 
//40 
//41  if(btGjkEpaSolver2::Penetration(pConvexA,transformA,
//42  pConvexB,transformB,
//43  guessVector,results))
//44 
//45  {
//46  // debugDraw->drawLine(results.witnesses[1],results.witnesses[1]+results.normal,btVector3(255,0,0));
//47  //resultOut->addContactPoint(results.normal,results.witnesses[1],-results.depth);
//48  wWitnessOnA = results.witnesses[0];
//49  wWitnessOnB = results.witnesses[1];
//50  v = results.normal;
//51  return true;
//52  } else
//53  {
//54  if(btGjkEpaSolver2::Distance(pConvexA,transformA,pConvexB,transformB,guessVector,results))
//55  {
//56  wWitnessOnA = results.witnesses[0];
//57  wWitnessOnB = results.witnesses[1];
//58  v = results.normal;
//59  return false;
//60  }
//61  }
//62 
//63  return false;
//64 }
//
//
//25 #include "BulletCollision/CollisionShapes/btConvexInternalShape.h"
//26 #include "BulletCollision/CollisionShapes/btSphereShape.h"
//27 #include "btGjkEpa2.h"
//28 
//29 #if defined(DEBUG) || defined (_DEBUG)
//30 #include <stdio.h> //for debug printf
//31 #ifdef __SPU__
//32 #include <spu_printf.h>
//33 #define printf spu_printf
//34 #endif //__SPU__
//35 #endif
//36 
//37 namespace gjkepa2_impl
//38 {
//39 
//40  // Config
//41 
//42  /* GJK */
//43 #define GJK_MAX_ITERATIONS 128
//44 #define GJK_ACCURARY ((btScalar)0.0001)
//45 #define GJK_MIN_DISTANCE ((btScalar)0.0001)
//46 #define GJK_DUPLICATED_EPS ((btScalar)0.0001)
//47 #define GJK_SIMPLEX2_EPS ((btScalar)0.0)
//48 #define GJK_SIMPLEX3_EPS ((btScalar)0.0)
//49 #define GJK_SIMPLEX4_EPS ((btScalar)0.0)
//50 
//51  /* EPA */
//52 #define EPA_MAX_VERTICES 64
//53 #define EPA_MAX_FACES (EPA_MAX_VERTICES*2)
//54 #define EPA_MAX_ITERATIONS 255
//55 #define EPA_ACCURACY ((btScalar)0.0001)
//56 #define EPA_FALLBACK (10*EPA_ACCURACY)
//57 #define EPA_PLANE_EPS ((btScalar)0.00001)
//58 #define EPA_INSIDE_EPS ((btScalar)0.01)
//59 
//60 
//61  // Shorthands
//62  typedef unsigned int U;
//63  typedef unsigned char U1;
//64 
//65  // MinkowskiDiff
//66  struct MinkowskiDiff
//67  {
//68  const btConvexShape* m_shapes[2];
//69  btMatrix3x3 m_toshape1;
//70  btTransform m_toshape0;
//71 #ifdef __SPU__
//72  bool m_enableMargin;
//73 #else
//74  btVector3 (btConvexShape::*Ls)(const btVector3&) const;
//75 #endif//__SPU__
//76 
//77 
//78  MinkowskiDiff()
//79  {
//80 
//81  }
//82 #ifdef __SPU__
//83  void EnableMargin(bool enable)
//84  {
//85  m_enableMargin = enable;
//86  }
//87  inline btVector3 Support0(const btVector3& d) const
//88  {
//89  if (m_enableMargin)
//90  {
//91  return m_shapes[0]->localGetSupportVertexNonVirtual(d);
//92  } else
//93  {
//94  return m_shapes[0]->localGetSupportVertexWithoutMarginNonVirtual(d);
//95  }
//96  }
//97  inline btVector3 Support1(const btVector3& d) const
//98  {
//99  if (m_enableMargin)
//100  {
//101  return m_toshape0*(m_shapes[1]->localGetSupportVertexNonVirtual(m_toshape1*d));
//102  } else
//103  {
//104  return m_toshape0*(m_shapes[1]->localGetSupportVertexWithoutMarginNonVirtual(m_toshape1*d));
//105  }
//106  }
//107 #else
//108  void EnableMargin(bool enable)
//109  {
//110  if(enable)
//111  Ls=&btConvexShape::localGetSupportVertexNonVirtual;
//112  else
//113  Ls=&btConvexShape::localGetSupportVertexWithoutMarginNonVirtual;
//114  }
//115  inline btVector3 Support0(const btVector3& d) const
//116  {
//117  return(((m_shapes[0])->*(Ls))(d));
//118  }
//119  inline btVector3 Support1(const btVector3& d) const
//120  {
//121  return(m_toshape0*((m_shapes[1])->*(Ls))(m_toshape1*d));
//122  }
//123 #endif //__SPU__
//124 
//125  inline btVector3 Support(const btVector3& d) const
//126  {
//127  return(Support0(d)-Support1(-d));
//128  }
//129  btVector3 Support(const btVector3& d,U index) const
//130  {
//131  if(index)
//132  return(Support1(d));
//133  else
//134  return(Support0(d));
//135  }
//136  };
//137 
//138  typedef MinkowskiDiff tShape;
//139 
//140 
//141  // GJK
//142  struct GJK
//143  {
//144  /* Types */
//145  struct sSV
//146  {
//147  btVector3 d,w;
//148  };
//149  struct sSimplex
//150  {
//151  sSV* c[4];
//152  btScalar p[4];
//153  U rank;
//154  };
//155  struct eStatus { enum _ {
//156  Valid,
//157  Inside,
//158  Failed };};
//159  /* Fields */
//160  tShape m_shape;
//161  btVector3 m_ray;
//162  btScalar m_distance;
//163  sSimplex m_simplices[2];
//164  sSV m_store[4];
//165  sSV* m_free[4];
//166  U m_nfree;
//167  U m_current;
//168  sSimplex* m_simplex;
//169  eStatus::_ m_status;
//170  /* Methods */
//171  GJK()
//172  {
//173  Initialize();
//174  }
//175  void Initialize()
//176  {
//177  m_ray = btVector3(0,0,0);
//178  m_nfree = 0;
//179  m_status = eStatus::Failed;
//180  m_current = 0;
//181  m_distance = 0;
//182  }
//183  eStatus::_ Evaluate(const tShape& shapearg,const btVector3& guess)
//184  {
//185  U iterations=0;
//186  btScalar sqdist=0;
//187  btScalar alpha=0;
//188  btVector3 lastw[4];
//189  U clastw=0;
//190  /* Initialize solver */
//191  m_free[0] = &m_store[0];
//192  m_free[1] = &m_store[1];
//193  m_free[2] = &m_store[2];
//194  m_free[3] = &m_store[3];
//195  m_nfree = 4;
//196  m_current = 0;
//197  m_status = eStatus::Valid;
//198  m_shape = shapearg;
//199  m_distance = 0;
//200  /* Initialize simplex */
//201  m_simplices[0].rank = 0;
//202  m_ray = guess;
//203  const btScalar sqrl= m_ray.length2();
//204  appendvertice(m_simplices[0],sqrl>0?-m_ray:btVector3(1,0,0));
//205  m_simplices[0].p[0] = 1;
//206  m_ray = m_simplices[0].c[0]->w;
//207  sqdist = sqrl;
//208  lastw[0] =
//209  lastw[1] =
//210  lastw[2] =
//211  lastw[3] = m_ray;
//212  /* Loop */
//213  do {
//214  const U next=1-m_current;
//215  sSimplex& cs=m_simplices[m_current];
//216  sSimplex& ns=m_simplices[next];
//217  /* Check zero */
//218  const btScalar rl=m_ray.length();
//219  if(rl<GJK_MIN_DISTANCE)
//220  {/* Touching or inside */
//221  m_status=eStatus::Inside;
//222  break;
//223  }
//224  /* Append new vertice in -'v' direction */
//225  appendvertice(cs,-m_ray);
//226  const btVector3& w=cs.c[cs.rank-1]->w;
//227  bool found=false;
//228  for(U i=0;i<4;++i)
//229  {
//230  if((w-lastw[i]).length2()<GJK_DUPLICATED_EPS)
//231  { found=true;break; }
//232  }
//233  if(found)
//234  {/* Return old simplex */
//235  removevertice(m_simplices[m_current]);
//236  break;
//237  }
//238  else
//239  {/* Update lastw */
//240  lastw[clastw=(clastw+1)&3]=w;
//241  }
//242  /* Check for termination */
//243  const btScalar omega=btDot(m_ray,w)/rl;
//244  alpha=btMax(omega,alpha);
//245  if(((rl-alpha)-(GJK_ACCURARY*rl))<=0)
//246  {/* Return old simplex */
//247  removevertice(m_simplices[m_current]);
//248  break;
//249  }
//250  /* Reduce simplex */
//251  btScalar weights[4];
//252  U mask=0;
//253  switch(cs.rank)
//254  {
//255  case 2: sqdist=projectorigin( cs.c[0]->w,
//256  cs.c[1]->w,
//257  weights,mask);break;
//258  case 3: sqdist=projectorigin( cs.c[0]->w,
//259  cs.c[1]->w,
//260  cs.c[2]->w,
//261  weights,mask);break;
//262  case 4: sqdist=projectorigin( cs.c[0]->w,
//263  cs.c[1]->w,
//264  cs.c[2]->w,
//265  cs.c[3]->w,
//266  weights,mask);break;
//267  }
//268  if(sqdist>=0)
//269  {/* Valid */
//270  ns.rank = 0;
//271  m_ray = btVector3(0,0,0);
//272  m_current = next;
//273  for(U i=0,ni=cs.rank;i<ni;++i)
//274  {
//275  if(mask&(1<<i))
//276  {
//277  ns.c[ns.rank] = cs.c[i];
//278  ns.p[ns.rank++] = weights[i];
//279  m_ray += cs.c[i]->w*weights[i];
//280  }
//281  else
//282  {
//283  m_free[m_nfree++] = cs.c[i];
//284  }
//285  }
//286  if(mask==15) m_status=eStatus::Inside;
//287  }
//288  else
//289  {/* Return old simplex */
//290  removevertice(m_simplices[m_current]);
//291  break;
//292  }
//293  m_status=((++iterations)<GJK_MAX_ITERATIONS)?m_status:eStatus::Failed;
//294  } while(m_status==eStatus::Valid);
//295  m_simplex=&m_simplices[m_current];
//296  switch(m_status)
//297  {
//298  case eStatus::Valid: m_distance=m_ray.length();break;
//299  case eStatus::Inside: m_distance=0;break;
//300  default:
//301  {
//302  }
//303  }
//304  return(m_status);
//305  }
//306  bool EncloseOrigin()
//307  {
//308  switch(m_simplex->rank)
//309  {
//310  case 1:
//311  {
//312  for(U i=0;i<3;++i)
//313  {
//314  btVector3 axis=btVector3(0,0,0);
//315  axis[i]=1;
//316  appendvertice(*m_simplex, axis);
//317  if(EncloseOrigin()) return(true);
//318  removevertice(*m_simplex);
//319  appendvertice(*m_simplex,-axis);
//320  if(EncloseOrigin()) return(true);
//321  removevertice(*m_simplex);
//322  }
//323  }
//324  break;
//325  case 2:
//326  {
//327  const btVector3 d=m_simplex->c[1]->w-m_simplex->c[0]->w;
//328  for(U i=0;i<3;++i)
//329  {
//330  btVector3 axis=btVector3(0,0,0);
//331  axis[i]=1;
//332  const btVector3 p=btCross(d,axis);
//333  if(p.length2()>0)
//334  {
//335  appendvertice(*m_simplex, p);
//336  if(EncloseOrigin()) return(true);
//337  removevertice(*m_simplex);
//338  appendvertice(*m_simplex,-p);
//339  if(EncloseOrigin()) return(true);
//340  removevertice(*m_simplex);
//341  }
//342  }
//343  }
//344  break;
//345  case 3:
//346  {
//347  const btVector3 n=btCross(m_simplex->c[1]->w-m_simplex->c[0]->w,
//348  m_simplex->c[2]->w-m_simplex->c[0]->w);
//349  if(n.length2()>0)
//350  {
//351  appendvertice(*m_simplex,n);
//352  if(EncloseOrigin()) return(true);
//353  removevertice(*m_simplex);
//354  appendvertice(*m_simplex,-n);
//355  if(EncloseOrigin()) return(true);
//356  removevertice(*m_simplex);
//357  }
//358  }
//359  break;
//360  case 4:
//361  {
//362  if(btFabs(det( m_simplex->c[0]->w-m_simplex->c[3]->w,
//363  m_simplex->c[1]->w-m_simplex->c[3]->w,
//364  m_simplex->c[2]->w-m_simplex->c[3]->w))>0)
//365  return(true);
//366  }
//367  break;
//368  }
//369  return(false);
//370  }
//371  /* Internals */
//372  void getsupport(const btVector3& d,sSV& sv) const
//373  {
//374  sv.d = d/d.length();
//375  sv.w = m_shape.Support(sv.d);
//376  }
//377  void removevertice(sSimplex& simplex)
//378  {
//379  m_free[m_nfree++]=simplex.c[--simplex.rank];
//380  }
//381  void appendvertice(sSimplex& simplex,const btVector3& v)
//382  {
//383  simplex.p[simplex.rank]=0;
//384  simplex.c[simplex.rank]=m_free[--m_nfree];
//385  getsupport(v,*simplex.c[simplex.rank++]);
//386  }
//387  static btScalar det(const btVector3& a,const btVector3& b,const btVector3& c)
//388  {
//389  return( a.y()*b.z()*c.x()+a.z()*b.x()*c.y()-
//390  a.x()*b.z()*c.y()-a.y()*b.x()*c.z()+
//391  a.x()*b.y()*c.z()-a.z()*b.y()*c.x());
//392  }
//393  static btScalar projectorigin( const btVector3& a,
//394  const btVector3& b,
//395  btScalar* w,U& m)
//396  {
//397  const btVector3 d=b-a;
//398  const btScalar l=d.length2();
//399  if(l>GJK_SIMPLEX2_EPS)
//400  {
//401  const btScalar t(l>0?-btDot(a,d)/l:0);
//402  if(t>=1) { w[0]=0;w[1]=1;m=2;return(b.length2()); }
//403  else if(t<=0) { w[0]=1;w[1]=0;m=1;return(a.length2()); }
//404  else { w[0]=1-(w[1]=t);m=3;return((a+d*t).length2()); }
//405  }
//406  return(-1);
//407  }
//408  static btScalar projectorigin( const btVector3& a,
//409  const btVector3& b,
//410  const btVector3& c,
//411  btScalar* w,U& m)
//412  {
//413  static const U imd3[]={1,2,0};
//414  const btVector3* vt[]={&a,&b,&c};
//415  const btVector3 dl[]={a-b,b-c,c-a};
//416  const btVector3 n=btCross(dl[0],dl[1]);
//417  const btScalar l=n.length2();
//418  if(l>GJK_SIMPLEX3_EPS)
//419  {
//420  btScalar mindist=-1;
//421  btScalar subw[2]={0.f,0.f};
//422  U subm(0);
//423  for(U i=0;i<3;++i)
//424  {
//425  if(btDot(*vt[i],btCross(dl[i],n))>0)
//426  {
//427  const U j=imd3[i];
//428  const btScalar subd(projectorigin(*vt[i],*vt[j],subw,subm));
//429  if((mindist<0)||(subd<mindist))
//430  {
//431  mindist = subd;
//432  m = static_cast<U>(((subm&1)?1<<i:0)+((subm&2)?1<<j:0));
//433  w[i] = subw[0];
//434  w[j] = subw[1];
//435  w[imd3[j]] = 0;
//436  }
//437  }
//438  }
//439  if(mindist<0)
//440  {
//441  const btScalar d=btDot(a,n);
//442  const btScalar s=btSqrt(l);
//443  const btVector3 p=n*(d/l);
//444  mindist = p.length2();
//445  m = 7;
//446  w[0] = (btCross(dl[1],b-p)).length()/s;
//447  w[1] = (btCross(dl[2],c-p)).length()/s;
//448  w[2] = 1-(w[0]+w[1]);
//449  }
//450  return(mindist);
//451  }
//452  return(-1);
//453  }
//454  static btScalar projectorigin( const btVector3& a,
//455  const btVector3& b,
//456  const btVector3& c,
//457  const btVector3& d,
//458  btScalar* w,U& m)
//459  {
//460  static const U imd3[]={1,2,0};
//461  const btVector3* vt[]={&a,&b,&c,&d};
//462  const btVector3 dl[]={a-d,b-d,c-d};
//463  const btScalar vl=det(dl[0],dl[1],dl[2]);
//464  const bool ng=(vl*btDot(a,btCross(b-c,a-b)))<=0;
//465  if(ng&&(btFabs(vl)>GJK_SIMPLEX4_EPS))
//466  {
//467  btScalar mindist=-1;
//468  btScalar subw[3]={0.f,0.f,0.f};
//469  U subm(0);
//470  for(U i=0;i<3;++i)
//471  {
//472  const U j=imd3[i];
//473  const btScalar s=vl*btDot(d,btCross(dl[i],dl[j]));
//474  if(s>0)
//475  {
//476  const btScalar subd=projectorigin(*vt[i],*vt[j],d,subw,subm);
//477  if((mindist<0)||(subd<mindist))
//478  {
//479  mindist = subd;
//480  m = static_cast<U>((subm&1?1<<i:0)+
//481  (subm&2?1<<j:0)+
//482  (subm&4?8:0));
//483  w[i] = subw[0];
//484  w[j] = subw[1];
//485  w[imd3[j]] = 0;
//486  w[3] = subw[2];
//487  }
//488  }
//489  }
//490  if(mindist<0)
//491  {
//492  mindist = 0;
//493  m = 15;
//494  w[0] = det(c,b,d)/vl;
//495  w[1] = det(a,c,d)/vl;
//496  w[2] = det(b,a,d)/vl;
//497  w[3] = 1-(w[0]+w[1]+w[2]);
//498  }
//499  return(mindist);
//500  }
//501  return(-1);
//502  }
//503  };
//504 
//505  // EPA
//506  struct EPA
//507  {
//508  /* Types */
//509  typedef GJK::sSV sSV;
//510  struct sFace
//511  {
//512  btVector3 n;
//513  btScalar d;
//514  sSV* c[3];
//515  sFace* f[3];
//516  sFace* l[2];
//517  U1 e[3];
//518  U1 pass;
//519  };
//520  struct sList
//521  {
//522  sFace* root;
//523  U count;
//524  sList() : root(0),count(0) {}
//525  };
//526  struct sHorizon
//527  {
//528  sFace* cf;
//529  sFace* ff;
//530  U nf;
//531  sHorizon() : cf(0),ff(0),nf(0) {}
//532  };
//533  struct eStatus { enum _ {
//534  Valid,
//535  Touching,
//536  Degenerated,
//537  NonConvex,
//538  InvalidHull,
//539  OutOfFaces,
//540  OutOfVertices,
//541  AccuraryReached,
//542  FallBack,
//543  Failed };};
//544  /* Fields */
//545  eStatus::_ m_status;
//546  GJK::sSimplex m_result;
//547  btVector3 m_normal;
//548  btScalar m_depth;
//549  sSV m_sv_store[EPA_MAX_VERTICES];
//550  sFace m_fc_store[EPA_MAX_FACES];
//551  U m_nextsv;
//552  sList m_hull;
//553  sList m_stock;
//554  /* Methods */
//555  EPA()
//556  {
//557  Initialize();
//558  }
//559 
//560 
//561  static inline void bind(sFace* fa,U ea,sFace* fb,U eb)
//562  {
//563  fa->e[ea]=(U1)eb;fa->f[ea]=fb;
//564  fb->e[eb]=(U1)ea;fb->f[eb]=fa;
//565  }
//566  static inline void append(sList& list,sFace* face)
//567  {
//568  face->l[0] = 0;
//569  face->l[1] = list.root;
//570  if(list.root) list.root->l[0]=face;
//571  list.root = face;
//572  ++list.count;
//573  }
//574  static inline void remove(sList& list,sFace* face)
//575  {
//576  if(face->l[1]) face->l[1]->l[0]=face->l[0];
//577  if(face->l[0]) face->l[0]->l[1]=face->l[1];
//578  if(face==list.root) list.root=face->l[1];
//579  --list.count;
//580  }
//581 
//582 
//583  void Initialize()
//584  {
//585  m_status = eStatus::Failed;
//586  m_normal = btVector3(0,0,0);
//587  m_depth = 0;
//588  m_nextsv = 0;
//589  for(U i=0;i<EPA_MAX_FACES;++i)
//590  {
//591  append(m_stock,&m_fc_store[EPA_MAX_FACES-i-1]);
//592  }
//593  }
//594  eStatus::_ Evaluate(GJK& gjk,const btVector3& guess)
//595  {
//596  GJK::sSimplex& simplex=*gjk.m_simplex;
//597  if((simplex.rank>1)&&gjk.EncloseOrigin())
//598  {
//599 
//600  /* Clean up */
//601  while(m_hull.root)
//602  {
//603  sFace* f = m_hull.root;
//604  remove(m_hull,f);
//605  append(m_stock,f);
//606  }
//607  m_status = eStatus::Valid;
//608  m_nextsv = 0;
//609  /* Orient simplex */
//610  if(gjk.det( simplex.c[0]->w-simplex.c[3]->w,
//611  simplex.c[1]->w-simplex.c[3]->w,
//612  simplex.c[2]->w-simplex.c[3]->w)<0)
//613  {
//614  btSwap(simplex.c[0],simplex.c[1]);
//615  btSwap(simplex.p[0],simplex.p[1]);
//616  }
//617  /* Build initial hull */
//618  sFace* tetra[]={newface(simplex.c[0],simplex.c[1],simplex.c[2],true),
//619  newface(simplex.c[1],simplex.c[0],simplex.c[3],true),
//620  newface(simplex.c[2],simplex.c[1],simplex.c[3],true),
//621  newface(simplex.c[0],simplex.c[2],simplex.c[3],true)};
//622  if(m_hull.count==4)
//623  {
//624  sFace* best=findbest();
//625  sFace outer=*best;
//626  U pass=0;
//627  U iterations=0;
//628  bind(tetra[0],0,tetra[1],0);
//629  bind(tetra[0],1,tetra[2],0);
//630  bind(tetra[0],2,tetra[3],0);
//631  bind(tetra[1],1,tetra[3],2);
//632  bind(tetra[1],2,tetra[2],1);
//633  bind(tetra[2],2,tetra[3],1);
//634  m_status=eStatus::Valid;
//635  for(;iterations<EPA_MAX_ITERATIONS;++iterations)
//636  {
//637  if(m_nextsv<EPA_MAX_VERTICES)
//638  {
//639  sHorizon horizon;
//640  sSV* w=&m_sv_store[m_nextsv++];
//641  bool valid=true;
//642  best->pass = (U1)(++pass);
//643  gjk.getsupport(best->n,*w);
//644  const btScalar wdist=btDot(best->n,w->w)-best->d;
//645  if(wdist>EPA_ACCURACY)
//646  {
//647  for(U j=0;(j<3)&&valid;++j)
//648  {
//649  valid&=expand( pass,w,
//650  best->f[j],best->e[j],
//651  horizon);
//652  }
//653  if(valid&&(horizon.nf>=3))
//654  {
//655  bind(horizon.cf,1,horizon.ff,2);
//656  remove(m_hull,best);
//657  append(m_stock,best);
//658  best=findbest();
//659  outer=*best;
//660  } else { m_status=eStatus::InvalidHull;break; }
//661  } else { m_status=eStatus::AccuraryReached;break; }
//662  } else { m_status=eStatus::OutOfVertices;break; }
//663  }
//664  const btVector3 projection=outer.n*outer.d;
//665  m_normal = outer.n;
//666  m_depth = outer.d;
//667  m_result.rank = 3;
//668  m_result.c[0] = outer.c[0];
//669  m_result.c[1] = outer.c[1];
//670  m_result.c[2] = outer.c[2];
//671  m_result.p[0] = btCross( outer.c[1]->w-projection,
//672  outer.c[2]->w-projection).length();
//673  m_result.p[1] = btCross( outer.c[2]->w-projection,
//674  outer.c[0]->w-projection).length();
//675  m_result.p[2] = btCross( outer.c[0]->w-projection,
//676  outer.c[1]->w-projection).length();
//677  const btScalar sum=m_result.p[0]+m_result.p[1]+m_result.p[2];
//678  m_result.p[0] /= sum;
//679  m_result.p[1] /= sum;
//680  m_result.p[2] /= sum;
//681  return(m_status);
//682  }
//683  }
//684  /* Fallback */
//685  m_status = eStatus::FallBack;
//686  m_normal = -guess;
//687  const btScalar nl=m_normal.length();
//688  if(nl>0)
//689  m_normal = m_normal/nl;
//690  else
//691  m_normal = btVector3(1,0,0);
//692  m_depth = 0;
//693  m_result.rank=1;
//694  m_result.c[0]=simplex.c[0];
//695  m_result.p[0]=1;
//696  return(m_status);
//697  }
//698  bool getedgedist(sFace* face, sSV* a, sSV* b, btScalar& dist)
//699  {
//700  const btVector3 ba = b->w - a->w;
//701  const btVector3 n_ab = btCross(ba, face->n); // Outward facing edge normal direction, on triangle plane
//702  const btScalar a_dot_nab = btDot(a->w, n_ab); // Only care about the sign to determine inside/outside, so not normalization required
//703 
//704  if(a_dot_nab < 0)
//705  {
//706  // Outside of edge a->b
//707 
//708  const btScalar ba_l2 = ba.length2();
//709  const btScalar a_dot_ba = btDot(a->w, ba);
//710  const btScalar b_dot_ba = btDot(b->w, ba);
//711 
//712  if(a_dot_ba > 0)
//713  {
//714  // Pick distance vertex a
//715  dist = a->w.length();
//716  }
//717  else if(b_dot_ba < 0)
//718  {
//719  // Pick distance vertex b
//720  dist = b->w.length();
//721  }
//722  else
//723  {
//724  // Pick distance to edge a->b
//725  const btScalar a_dot_b = btDot(a->w, b->w);
//726  dist = btSqrt(btMax((a->w.length2() * b->w.length2() - a_dot_b * a_dot_b) / ba_l2, (btScalar)0));
//727  }
//728 
//729  return true;
//730  }
//731 
//732  return false;
//733  }
//734  sFace* newface(sSV* a,sSV* b,sSV* c,bool forced)
//735  {
//736  if(m_stock.root)
//737  {
//738  sFace* face=m_stock.root;
//739  remove(m_stock,face);
//740  append(m_hull,face);
//741  face->pass = 0;
//742  face->c[0] = a;
//743  face->c[1] = b;
//744  face->c[2] = c;
//745  face->n = btCross(b->w-a->w,c->w-a->w);
//746  const btScalar l=face->n.length();
//747  const bool v=l>EPA_ACCURACY;
//748 
//749  if(v)
//750  {
//751  if(!(getedgedist(face, a, b, face->d) ||
//752  getedgedist(face, b, c, face->d) ||
//753  getedgedist(face, c, a, face->d)))
//754  {
//755  // Origin projects to the interior of the triangle
//756  // Use distance to triangle plane
//757  face->d = btDot(a->w, face->n) / l;
//758  }
//759 
//760  face->n /= l;
//761  if(forced || (face->d >= -EPA_PLANE_EPS))
//762  {
//763  return face;
//764  }
//765  else
//766  m_status=eStatus::NonConvex;
//767  }
//768  else
//769  m_status=eStatus::Degenerated;
//770 
//771  remove(m_hull, face);
//772  append(m_stock, face);
//773  return 0;
//774 
//775  }
//776  m_status = m_stock.root ? eStatus::OutOfVertices : eStatus::OutOfFaces;
//777  return 0;
//778  }
//779  sFace* findbest()
//780  {
//781  sFace* minf=m_hull.root;
//782  btScalar mind=minf->d*minf->d;
//783  for(sFace* f=minf->l[1];f;f=f->l[1])
//784  {
//785  const btScalar sqd=f->d*f->d;
//786  if(sqd<mind)
//787  {
//788  minf=f;
//789  mind=sqd;
//790  }
//791  }
//792  return(minf);
//793  }
//794  bool expand(U pass,sSV* w,sFace* f,U e,sHorizon& horizon)
//795  {
//796  static const U i1m3[]={1,2,0};
//797  static const U i2m3[]={2,0,1};
//798  if(f->pass!=pass)
//799  {
//800  const U e1=i1m3[e];
//801  if((btDot(f->n,w->w)-f->d)<-EPA_PLANE_EPS)
//802  {
//803  sFace* nf=newface(f->c[e1],f->c[e],w,false);
//804  if(nf)
//805  {
//806  bind(nf,0,f,e);
//807  if(horizon.cf) bind(horizon.cf,1,nf,2); else horizon.ff=nf;
//808  horizon.cf=nf;
//809  ++horizon.nf;
//810  return(true);
//811  }
//812  }
//813  else
//814  {
//815  const U e2=i2m3[e];
//816  f->pass = (U1)pass;
//817  if( expand(pass,w,f->f[e1],f->e[e1],horizon)&&
//818  expand(pass,w,f->f[e2],f->e[e2],horizon))
//819  {
//820  remove(m_hull,f);
//821  append(m_stock,f);
//822  return(true);
//823  }
//824  }
//825  }
//826  return(false);
//827  }
//828 
//829  };
//830 
//831  //
//832  static void Initialize( const btConvexShape* shape0,const btTransform& wtrs0,
//833  const btConvexShape* shape1,const btTransform& wtrs1,
//834  btGjkEpaSolver2::sResults& results,
//835  tShape& shape,
//836  bool withmargins)
//837  {
//838  /* Results */
//839  results.witnesses[0] =
//840  results.witnesses[1] = btVector3(0,0,0);
//841  results.status = btGjkEpaSolver2::sResults::Separated;
//842  /* Shape */
//843  shape.m_shapes[0] = shape0;
//844  shape.m_shapes[1] = shape1;
//845  shape.m_toshape1 = wtrs1.getBasis().transposeTimes(wtrs0.getBasis());
//846  shape.m_toshape0 = wtrs0.inverseTimes(wtrs1);
//847  shape.EnableMargin(withmargins);
//848  }
//849 
//850 }
//851 
//852 //
//853 // Api
//854 //
//855 
//856 using namespace gjkepa2_impl;
//857 
//858 //
//859 int btGjkEpaSolver2::StackSizeRequirement()
//860 {
//861  return(sizeof(GJK)+sizeof(EPA));
//862 }
//863 
//864 //
//865 bool btGjkEpaSolver2::Distance( const btConvexShape* shape0,
//866  const btTransform& wtrs0,
//867  const btConvexShape* shape1,
//868  const btTransform& wtrs1,
//869  const btVector3& guess,
//870  sResults& results)
//871 {
//872  tShape shape;
//873  Initialize(shape0,wtrs0,shape1,wtrs1,results,shape,false);
//874  GJK gjk;
//875  GJK::eStatus::_ gjk_status=gjk.Evaluate(shape,guess);
//876  if(gjk_status==GJK::eStatus::Valid)
//877  {
//878  btVector3 w0=btVector3(0,0,0);
//879  btVector3 w1=btVector3(0,0,0);
//880  for(U i=0;i<gjk.m_simplex->rank;++i)
//881  {
//882  const btScalar p=gjk.m_simplex->p[i];
//883  w0+=shape.Support( gjk.m_simplex->c[i]->d,0)*p;
//884  w1+=shape.Support(-gjk.m_simplex->c[i]->d,1)*p;
//885  }
//886  results.witnesses[0] = wtrs0*w0;
//887  results.witnesses[1] = wtrs0*w1;
//888  results.normal = w0-w1;
//889  results.distance = results.normal.length();
//890  results.normal /= results.distance>GJK_MIN_DISTANCE?results.distance:1;
//891  return(true);
//892  }
//893  else
//894  {
//895  results.status = gjk_status==GJK::eStatus::Inside?
//896  sResults::Penetrating :
//897  sResults::GJK_Failed ;
//898  return(false);
//899  }
//900 }
//901 
//902 //
//903 bool btGjkEpaSolver2::Penetration( const btConvexShape* shape0,
//904  const btTransform& wtrs0,
//905  const btConvexShape* shape1,
//906  const btTransform& wtrs1,
//907  const btVector3& guess,
//908  sResults& results,
//909  bool usemargins)
//910 {
//911  tShape shape;
//912  Initialize(shape0,wtrs0,shape1,wtrs1,results,shape,usemargins);
//913  GJK gjk;
//914  GJK::eStatus::_ gjk_status=gjk.Evaluate(shape,-guess);
//915  switch(gjk_status)
//916  {
//917  case GJK::eStatus::Inside:
//918  {
//919  EPA epa;
//920  EPA::eStatus::_ epa_status=epa.Evaluate(gjk,-guess);
//921  if(epa_status!=EPA::eStatus::Failed)
//922  {
//923  btVector3 w0=btVector3(0,0,0);
//924  for(U i=0;i<epa.m_result.rank;++i)
//925  {
//926  w0+=shape.Support(epa.m_result.c[i]->d,0)*epa.m_result.p[i];
//927  }
//928  results.status = sResults::Penetrating;
//929  results.witnesses[0] = wtrs0*w0;
//930  results.witnesses[1] = wtrs0*(w0-epa.m_normal*epa.m_depth);
//931  results.normal = -epa.m_normal;
//932  results.distance = -epa.m_depth;
//933  return(true);
//934  } else results.status=sResults::EPA_Failed;
//935  }
//936  break;
//937  case GJK::eStatus::Failed:
//938  results.status=sResults::GJK_Failed;
//939  break;
//940  default:
//941  {
//942  }
//943  }
//944  return(false);
//945 }
//946 
//947 #ifndef __SPU__
//948 //
//949 btScalar btGjkEpaSolver2::SignedDistance(const btVector3& position,
//950  btScalar margin,
//951  const btConvexShape* shape0,
//952  const btTransform& wtrs0,
//953  sResults& results)
//954 {
//955  tShape shape;
//956  btSphereShape shape1(margin);
//957  btTransform wtrs1(btQuaternion(0,0,0,1),position);
//958  Initialize(shape0,wtrs0,&shape1,wtrs1,results,shape,false);
//959  GJK gjk;
//960  GJK::eStatus::_ gjk_status=gjk.Evaluate(shape,btVector3(1,1,1));
//961  if(gjk_status==GJK::eStatus::Valid)
//962  {
//963  btVector3 w0=btVector3(0,0,0);
//964  btVector3 w1=btVector3(0,0,0);
//965  for(U i=0;i<gjk.m_simplex->rank;++i)
//966  {
//967  const btScalar p=gjk.m_simplex->p[i];
//968  w0+=shape.Support( gjk.m_simplex->c[i]->d,0)*p;
//969  w1+=shape.Support(-gjk.m_simplex->c[i]->d,1)*p;
//970  }
//971  results.witnesses[0] = wtrs0*w0;
//972  results.witnesses[1] = wtrs0*w1;
//973  const btVector3 delta= results.witnesses[1]-
//974  results.witnesses[0];
//975  const btScalar margin= shape0->getMarginNonVirtual()+
//976  shape1.getMarginNonVirtual();
//977  const btScalar length= delta.length();
//978  results.normal = delta/length;
//979  results.witnesses[0] += results.normal*margin;
//980  return(length-margin);
//981  }
//982  else
//983  {
//984  if(gjk_status==GJK::eStatus::Inside)
//985  {
//986  if(Penetration(shape0,wtrs0,&shape1,wtrs1,gjk.m_ray,results))
//987  {
//988  const btVector3 delta= results.witnesses[0]-
//989  results.witnesses[1];
//990  const btScalar length= delta.length();
//991  if (length >= SIMD_EPSILON)
//992  results.normal = delta/length;
//993  return(-length);
//994  }
//995  }
//996  }
//997  return(SIMD_INFINITY);
//998 }
//999 
//1000 //
//1001 bool btGjkEpaSolver2::SignedDistance(const btConvexShape* shape0,
//1002  const btTransform& wtrs0,
//1003  const btConvexShape* shape1,
//1004  const btTransform& wtrs1,
//1005  const btVector3& guess,
//1006  sResults& results)
//1007 {
//1008  if(!Distance(shape0,wtrs0,shape1,wtrs1,guess,results))
//1009  return(Penetration(shape0,wtrs0,shape1,wtrs1,guess,results,false));
//1010  else
//1011  return(true);
//1012 }
//1013 #endif //__SPU__
//1014 
//1015 /* Symbols cleanup */
//1016 
//1017 #undef GJK_MAX_ITERATIONS
//1018 #undef GJK_ACCURARY
//1019 #undef GJK_MIN_DISTANCE
//1020 #undef GJK_DUPLICATED_EPS
//1021 #undef GJK_SIMPLEX2_EPS
//1022 #undef GJK_SIMPLEX3_EPS
//1023 #undef GJK_SIMPLEX4_EPS
//1024 
//1025 #undef EPA_MAX_VERTICES
//1026 #undef EPA_MAX_FACES
//1027 #undef EPA_MAX_ITERATIONS
//1028 #undef EPA_ACCURACY
//1029 #undef EPA_FALLBACK
//1030 #undef EPA_PLANE_EPS
//1031 #undef EPA_INSIDE_EPS
//
//
//
//
//
//
//16 #include "btGjkPairDetector.h"
//17 #include "BulletCollision/CollisionShapes/btConvexShape.h"
//18 #include "BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h"
//19 #include "BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h"
//20 
//21 
//22 
//23 #if defined(DEBUG) || defined (_DEBUG)
//24 //#define TEST_NON_VIRTUAL 1
//25 #include <stdio.h> //for debug printf
//26 #ifdef __SPU__
//27 #include <spu_printf.h>
//28 #define printf spu_printf
//29 //#define DEBUG_SPU_COLLISION_DETECTION 1
//30 #endif //__SPU__
//31 #endif
//32 
//33 //must be above the machine epsilon
//34 #define REL_ERROR2 btScalar(1.0e-6)
//35 
//36 //temp globals, to improve GJK/EPA/penetration calculations
//37 int gNumDeepPenetrationChecks = 0;
//38 int gNumGjkChecks = 0;
//39 
//40 
//41 btGjkPairDetector::btGjkPairDetector(const btConvexShape* objectA,const btConvexShape* objectB,btSimplexSolverInterface* simplexSolver,btConvexPenetrationDepthSolver* penetrationDepthSolver)
//42 :m_cachedSeparatingAxis(btScalar(0.),btScalar(1.),btScalar(0.)),
//43 m_penetrationDepthSolver(penetrationDepthSolver),
//44 m_simplexSolver(simplexSolver),
//45 m_minkowskiA(objectA),
//46 m_minkowskiB(objectB),
//47 m_shapeTypeA(objectA->getShapeType()),
//48 m_shapeTypeB(objectB->getShapeType()),
//49 m_marginA(objectA->getMargin()),
//50 m_marginB(objectB->getMargin()),
//51 m_ignoreMargin(false),
//52 m_lastUsedMethod(-1),
//53 m_catchDegeneracies(1)
//54 {
//55 }
//56 btGjkPairDetector::btGjkPairDetector(const btConvexShape* objectA,const btConvexShape* objectB,int shapeTypeA,int shapeTypeB,btScalar marginA, btScalar marginB, btSimplexSolverInterface* simplexSolver,btConvexPenetrationDepthSolver* penetrationDepthSolver)
//57 :m_cachedSeparatingAxis(btScalar(0.),btScalar(1.),btScalar(0.)),
//58 m_penetrationDepthSolver(penetrationDepthSolver),
//59 m_simplexSolver(simplexSolver),
//60 m_minkowskiA(objectA),
//61 m_minkowskiB(objectB),
//62 m_shapeTypeA(shapeTypeA),
//63 m_shapeTypeB(shapeTypeB),
//64 m_marginA(marginA),
//65 m_marginB(marginB),
//66 m_ignoreMargin(false),
//67 m_lastUsedMethod(-1),
//68 m_catchDegeneracies(1)
//69 {
//70 }
//71 
//72 void btGjkPairDetector::getClosestPoints(const ClosestPointInput& input,Result& output,class btIDebugDraw* debugDraw,bool swapResults)
//73 {
//74  (void)swapResults;
//75 
//76  getClosestPointsNonVirtual(input,output,debugDraw);
//77 }
//78 
//79 #ifdef __SPU__
//80 void btGjkPairDetector::getClosestPointsNonVirtual(const ClosestPointInput& input,Result& output,class btIDebugDraw* debugDraw)
//81 #else
//82 void btGjkPairDetector::getClosestPointsNonVirtual(const ClosestPointInput& input,Result& output,class btIDebugDraw* debugDraw)
//83 #endif
//84 {
//85  m_cachedSeparatingDistance = 0.f;
//86 
//87  btScalar distance=btScalar(0.);
//88  btVector3 normalInB(btScalar(0.),btScalar(0.),btScalar(0.));
//89  btVector3 pointOnA,pointOnB;
//90  btTransform localTransA = input.m_transformA;
//91  btTransform localTransB = input.m_transformB;
//92  btVector3 positionOffset = (localTransA.getOrigin() + localTransB.getOrigin()) * btScalar(0.5);
//93  localTransA.getOrigin() -= positionOffset;
//94  localTransB.getOrigin() -= positionOffset;
//95 
//96  bool check2d = m_minkowskiA->isConvex2d() && m_minkowskiB->isConvex2d();
//97 
//98  btScalar marginA = m_marginA;
//99  btScalar marginB = m_marginB;
//100 
//101  gNumGjkChecks++;
//102 
//103 #ifdef DEBUG_SPU_COLLISION_DETECTION
//104  spu_printf("inside gjk\n");
//105 #endif
//106  //for CCD we don't use margins
//107  if (m_ignoreMargin)
//108  {
//109  marginA = btScalar(0.);
//110  marginB = btScalar(0.);
//111 #ifdef DEBUG_SPU_COLLISION_DETECTION
//112  spu_printf("ignoring margin\n");
//113 #endif
//114  }
//115 
//116  m_curIter = 0;
//117  int gGjkMaxIter = 1000;//this is to catch invalid input, perhaps check for #NaN?
//118  m_cachedSeparatingAxis.setValue(0,1,0);
//119 
//120  bool isValid = false;
//121  bool checkSimplex = false;
//122  bool checkPenetration = true;
//123  m_degenerateSimplex = 0;
//124 
//125  m_lastUsedMethod = -1;
//126 
//127  {
//128  btScalar squaredDistance = BT_LARGE_FLOAT;
//129  btScalar delta = btScalar(0.);
//130 
//131  btScalar margin = marginA + marginB;
//132 
//133 
//134 
//135  m_simplexSolver->reset();
//136 
//137  for ( ; ; )
//138  //while (true)
//139  {
//140 
//141  btVector3 seperatingAxisInA = (-m_cachedSeparatingAxis)* input.m_transformA.getBasis();
//142  btVector3 seperatingAxisInB = m_cachedSeparatingAxis* input.m_transformB.getBasis();
//143 
//144 #if 1
//145 
//146  btVector3 pInA = m_minkowskiA->localGetSupportVertexWithoutMarginNonVirtual(seperatingAxisInA);
//147  btVector3 qInB = m_minkowskiB->localGetSupportVertexWithoutMarginNonVirtual(seperatingAxisInB);
//148 
//149 // btVector3 pInA = localGetSupportingVertexWithoutMargin(m_shapeTypeA, m_minkowskiA, seperatingAxisInA,input.m_convexVertexData[0]);//, &featureIndexA);
//150 // btVector3 qInB = localGetSupportingVertexWithoutMargin(m_shapeTypeB, m_minkowskiB, seperatingAxisInB,input.m_convexVertexData[1]);//, &featureIndexB);
//151 
//152 #else
//153 #ifdef __SPU__
//154  btVector3 pInA = m_minkowskiA->localGetSupportVertexWithoutMarginNonVirtual(seperatingAxisInA);
//155  btVector3 qInB = m_minkowskiB->localGetSupportVertexWithoutMarginNonVirtual(seperatingAxisInB);
//156 #else
//157  btVector3 pInA = m_minkowskiA->localGetSupportingVertexWithoutMargin(seperatingAxisInA);
//158  btVector3 qInB = m_minkowskiB->localGetSupportingVertexWithoutMargin(seperatingAxisInB);
//159 #ifdef TEST_NON_VIRTUAL
//160  btVector3 pInAv = m_minkowskiA->localGetSupportingVertexWithoutMargin(seperatingAxisInA);
//161  btVector3 qInBv = m_minkowskiB->localGetSupportingVertexWithoutMargin(seperatingAxisInB);
//162  btAssert((pInAv-pInA).length() < 0.0001);
//163  btAssert((qInBv-qInB).length() < 0.0001);
//164 #endif //
//165 #endif //__SPU__
//166 #endif
//167 
//168 
//169  btVector3 pWorld = localTransA(pInA);
//170  btVector3 qWorld = localTransB(qInB);
//171 
//172 #ifdef DEBUG_SPU_COLLISION_DETECTION
//173  spu_printf("got local supporting vertices\n");
//174 #endif
//175 
//176  if (check2d)
//177  {
//178  pWorld[2] = 0.f;
//179  qWorld[2] = 0.f;
//180  }
//181 
//182  btVector3 w = pWorld - qWorld;
//183  delta = m_cachedSeparatingAxis.dot(w);
//184 
//185  // potential exit, they don't overlap
//186  if ((delta > btScalar(0.0)) && (delta * delta > squaredDistance * input.m_maximumDistanceSquared))
//187  {
//188  m_degenerateSimplex = 10;
//189  checkSimplex=true;
//190  //checkPenetration = false;
//191  break;
//192  }
//193 
//194  //exit 0: the new point is already in the simplex, or we didn't come any closer
//195  if (m_simplexSolver->inSimplex(w))
//196  {
//197  m_degenerateSimplex = 1;
//198  checkSimplex = true;
//199  break;
//200  }
//201  // are we getting any closer ?
//202  btScalar f0 = squaredDistance - delta;
//203  btScalar f1 = squaredDistance * REL_ERROR2;
//204 
//205  if (f0 <= f1)
//206  {
//207  if (f0 <= btScalar(0.))
//208  {
//209  m_degenerateSimplex = 2;
//210  } else
//211  {
//212  m_degenerateSimplex = 11;
//213  }
//214  checkSimplex = true;
//215  break;
//216  }
//217 
//218 #ifdef DEBUG_SPU_COLLISION_DETECTION
//219  spu_printf("addVertex 1\n");
//220 #endif
//221  //add current vertex to simplex
//222  m_simplexSolver->addVertex(w, pWorld, qWorld);
//223 #ifdef DEBUG_SPU_COLLISION_DETECTION
//224  spu_printf("addVertex 2\n");
//225 #endif
//226  btVector3 newCachedSeparatingAxis;
//227 
//228  //calculate the closest point to the origin (update vector v)
//229  if (!m_simplexSolver->closest(newCachedSeparatingAxis))
//230  {
//231  m_degenerateSimplex = 3;
//232  checkSimplex = true;
//233  break;
//234  }
//235 
//236  if(newCachedSeparatingAxis.length2()<REL_ERROR2)
//237  {
//238  m_cachedSeparatingAxis = newCachedSeparatingAxis;
//239  m_degenerateSimplex = 6;
//240  checkSimplex = true;
//241  break;
//242  }
//243 
//244  btScalar previousSquaredDistance = squaredDistance;
//245  squaredDistance = newCachedSeparatingAxis.length2();
//246 #if 0
//247 
//248  if (squaredDistance>previousSquaredDistance)
//249  {
//250  m_degenerateSimplex = 7;
//251  squaredDistance = previousSquaredDistance;
//252  checkSimplex = false;
//253  break;
//254  }
//255 #endif //
//256 
//257 
//258  //redundant m_simplexSolver->compute_points(pointOnA, pointOnB);
//259 
//260  //are we getting any closer ?
//261  if (previousSquaredDistance - squaredDistance <= SIMD_EPSILON * previousSquaredDistance)
//262  {
//263 // m_simplexSolver->backup_closest(m_cachedSeparatingAxis);
//264  checkSimplex = true;
//265  m_degenerateSimplex = 12;
//266 
//267  break;
//268  }
//269 
//270  m_cachedSeparatingAxis = newCachedSeparatingAxis;
//271 
//272  //degeneracy, this is typically due to invalid/uninitialized worldtransforms for a btCollisionObject
//273  if (m_curIter++ > gGjkMaxIter)
//274  {
//275  #if defined(DEBUG) || defined (_DEBUG) || defined (DEBUG_SPU_COLLISION_DETECTION)
//276 
//277  printf("btGjkPairDetector maxIter exceeded:%i\n",m_curIter);
//278  printf("sepAxis=(%f,%f,%f), squaredDistance = %f, shapeTypeA=%i,shapeTypeB=%i\n",
//279  m_cachedSeparatingAxis.getX(),
//280  m_cachedSeparatingAxis.getY(),
//281  m_cachedSeparatingAxis.getZ(),
//282  squaredDistance,
//283  m_minkowskiA->getShapeType(),
//284  m_minkowskiB->getShapeType());
//285 
//286  #endif
//287  break;
//288 
//289  }
//290 
//291 
//292  bool check = (!m_simplexSolver->fullSimplex());
//293  //bool check = (!m_simplexSolver->fullSimplex() && squaredDistance > SIMD_EPSILON * m_simplexSolver->maxVertex());
//294 
//295  if (!check)
//296  {
//297  //do we need this backup_closest here ?
//298 // m_simplexSolver->backup_closest(m_cachedSeparatingAxis);
//299  m_degenerateSimplex = 13;
//300  break;
//301  }
//302  }
//303 
//304  if (checkSimplex)
//305  {
//306  m_simplexSolver->compute_points(pointOnA, pointOnB);
//307  normalInB = m_cachedSeparatingAxis;
//308  btScalar lenSqr =m_cachedSeparatingAxis.length2();
//309 
//310  //valid normal
//311  if (lenSqr < 0.0001)
//312  {
//313  m_degenerateSimplex = 5;
//314  }
//315  if (lenSqr > SIMD_EPSILON*SIMD_EPSILON)
//316  {
//317  btScalar rlen = btScalar(1.) / btSqrt(lenSqr );
//318  normalInB *= rlen; //normalize
//319  btScalar s = btSqrt(squaredDistance);
//320 
//321  btAssert(s > btScalar(0.0));
//322  pointOnA -= m_cachedSeparatingAxis * (marginA / s);
//323  pointOnB += m_cachedSeparatingAxis * (marginB / s);
//324  distance = ((btScalar(1.)/rlen) - margin);
//325  isValid = true;
//326 
//327  m_lastUsedMethod = 1;
//328  } else
//329  {
//330  m_lastUsedMethod = 2;
//331  }
//332  }
//333 
//334  bool catchDegeneratePenetrationCase =
//335  (m_catchDegeneracies && m_penetrationDepthSolver && m_degenerateSimplex && ((distance+margin) < 0.01));
//336 
//337  //if (checkPenetration && !isValid)
//338  if (checkPenetration && (!isValid || catchDegeneratePenetrationCase ))
//339  {
//340  //penetration case
//341 
//342  //if there is no way to handle penetrations, bail out
//343  if (m_penetrationDepthSolver)
//344  {
//345  // Penetration depth case.
//346  btVector3 tmpPointOnA,tmpPointOnB;
//347 
//348  gNumDeepPenetrationChecks++;
//349  m_cachedSeparatingAxis.setZero();
//350 
//351  bool isValid2 = m_penetrationDepthSolver->calcPenDepth(
//352  *m_simplexSolver,
//353  m_minkowskiA,m_minkowskiB,
//354  localTransA,localTransB,
//355  m_cachedSeparatingAxis, tmpPointOnA, tmpPointOnB,
//356  debugDraw,input.m_stackAlloc
//357  );
//358 
//359 
//360  if (isValid2)
//361  {
//362  btVector3 tmpNormalInB = tmpPointOnB-tmpPointOnA;
//363  btScalar lenSqr = tmpNormalInB.length2();
//364  if (lenSqr <= (SIMD_EPSILON*SIMD_EPSILON))
//365  {
//366  tmpNormalInB = m_cachedSeparatingAxis;
//367  lenSqr = m_cachedSeparatingAxis.length2();
//368  }
//369 
//370  if (lenSqr > (SIMD_EPSILON*SIMD_EPSILON))
//371  {
//372  tmpNormalInB /= btSqrt(lenSqr);
//373  btScalar distance2 = -(tmpPointOnA-tmpPointOnB).length();
//374  //only replace valid penetrations when the result is deeper (check)
//375  if (!isValid || (distance2 < distance))
//376  {
//377  distance = distance2;
//378  pointOnA = tmpPointOnA;
//379  pointOnB = tmpPointOnB;
//380  normalInB = tmpNormalInB;
//381  isValid = true;
//382  m_lastUsedMethod = 3;
//383  } else
//384  {
//385  m_lastUsedMethod = 8;
//386  }
//387  } else
//388  {
//389  m_lastUsedMethod = 9;
//390  }
//391  } else
//392 
//393  {
//399 
//400 
//401  if (m_cachedSeparatingAxis.length2() > btScalar(0.))
//402  {
//403  btScalar distance2 = (tmpPointOnA-tmpPointOnB).length()-margin;
//404  //only replace valid distances when the distance is less
//405  if (!isValid || (distance2 < distance))
//406  {
//407  distance = distance2;
//408  pointOnA = tmpPointOnA;
//409  pointOnB = tmpPointOnB;
//410  pointOnA -= m_cachedSeparatingAxis * marginA ;
//411  pointOnB += m_cachedSeparatingAxis * marginB ;
//412  normalInB = m_cachedSeparatingAxis;
//413  normalInB.normalize();
//414  isValid = true;
//415  m_lastUsedMethod = 6;
//416  } else
//417  {
//418  m_lastUsedMethod = 5;
//419  }
//420  }
//421  }
//422 
//423  }
//424 
//425  }
//426  }
//427 
//428 
//429 
//430  if (isValid && ((distance < 0) || (distance*distance < input.m_maximumDistanceSquared)))
//431  {
//432 #if 0
//433 
//434 // if (check2d)
//435  {
//436  printf("n = %2.3f,%2.3f,%2.3f. ",normalInB[0],normalInB[1],normalInB[2]);
//437  printf("distance = %2.3f exit=%d deg=%d\n",distance,m_lastUsedMethod,m_degenerateSimplex);
//438  }
//439 #endif
//440 
//441  m_cachedSeparatingAxis = normalInB;
//442  m_cachedSeparatingDistance = distance;
//443 
//444  output.addContactPoint(
//445  normalInB,
//446  pointOnB+positionOffset,
//447  distance);
//448 
//449  }
//450 
//451 
//452 }
//
//
//7 #include "btVoronoiSimplexSolver.h"
//28 
//29 #define VERTA 0
//30 #define VERTB 1
//31 #define VERTC 2
//32 #define VERTD 3
//33 
//34 #define CATCH_DEGENERATE_TETRAHEDRON 1
//35 void btVoronoiSimplexSolver::removeVertex(int index)
//36 {
//37 
//38  btAssert(m_numVertices>0);
//39  m_numVertices--;
//40  m_simplexVectorW[index] = m_simplexVectorW[m_numVertices];
//41  m_simplexPointsP[index] = m_simplexPointsP[m_numVertices];
//42  m_simplexPointsQ[index] = m_simplexPointsQ[m_numVertices];
//43 }
//44 
//45 void btVoronoiSimplexSolver::reduceVertices (const btUsageBitfield& usedVerts)
//46 {
//47  if ((numVertices() >= 4) && (!usedVerts.usedVertexD))
//48  removeVertex(3);
//49 
//50  if ((numVertices() >= 3) && (!usedVerts.usedVertexC))
//51  removeVertex(2);
//52 
//53  if ((numVertices() >= 2) && (!usedVerts.usedVertexB))
//54  removeVertex(1);
//55 
//56  if ((numVertices() >= 1) && (!usedVerts.usedVertexA))
//57  removeVertex(0);
//58 
//59 }
//60 
//61 
//62 
//63 
//64 
//65 //clear the simplex, remove all the vertices
//66 void btVoronoiSimplexSolver::reset()
//67 {
//68  m_cachedValidClosest = false;
//69  m_numVertices = 0;
//70  m_needsUpdate = true;
//71  m_lastW = btVector3(btScalar(BT_LARGE_FLOAT),btScalar(BT_LARGE_FLOAT),btScalar(BT_LARGE_FLOAT));
//72  m_cachedBC.reset();
//73 }
//74 
//75 
//76 
//77  //add a vertex
//78 void btVoronoiSimplexSolver::addVertex(const btVector3& w, const btVector3& p, const btVector3& q)
//79 {
//80  m_lastW = w;
//81  m_needsUpdate = true;
//82 
//83  m_simplexVectorW[m_numVertices] = w;
//84  m_simplexPointsP[m_numVertices] = p;
//85  m_simplexPointsQ[m_numVertices] = q;
//86 
//87  m_numVertices++;
//88 }
//89 
//90 bool btVoronoiSimplexSolver::updateClosestVectorAndPoints()
//91 {
//92 
//93  if (m_needsUpdate)
//94  {
//95  m_cachedBC.reset();
//96 
//97  m_needsUpdate = false;
//98 
//99  switch (numVertices())
//100  {
//101  case 0:
//102  m_cachedValidClosest = false;
//103  break;
//104  case 1:
//105  {
//106  m_cachedP1 = m_simplexPointsP[0];
//107  m_cachedP2 = m_simplexPointsQ[0];
//108  m_cachedV = m_cachedP1-m_cachedP2; //== m_simplexVectorW[0]
//109  m_cachedBC.reset();
//110  m_cachedBC.setBarycentricCoordinates(btScalar(1.),btScalar(0.),btScalar(0.),btScalar(0.));
//111  m_cachedValidClosest = m_cachedBC.isValid();
//112  break;
//113  };
//114  case 2:
//115  {
//116  //closest point origin from line segment
//117  const btVector3& from = m_simplexVectorW[0];
//118  const btVector3& to = m_simplexVectorW[1];
//119  btVector3 nearest;
//120 
//121  btVector3 p (btScalar(0.),btScalar(0.),btScalar(0.));
//122  btVector3 diff = p - from;
//123  btVector3 v = to - from;
//124  btScalar t = v.dot(diff);
//125 
//126  if (t > 0) {
//127  btScalar dotVV = v.dot(v);
//128  if (t < dotVV) {
//129  t /= dotVV;
//130  diff -= t*v;
//131  m_cachedBC.m_usedVertices.usedVertexA = true;
//132  m_cachedBC.m_usedVertices.usedVertexB = true;
//133  } else {
//134  t = 1;
//135  diff -= v;
//136  //reduce to 1 point
//137  m_cachedBC.m_usedVertices.usedVertexB = true;
//138  }
//139  } else
//140  {
//141  t = 0;
//142  //reduce to 1 point
//143  m_cachedBC.m_usedVertices.usedVertexA = true;
//144  }
//145  m_cachedBC.setBarycentricCoordinates(1-t,t);
//146  nearest = from + t*v;
//147 
//148  m_cachedP1 = m_simplexPointsP[0] + t * (m_simplexPointsP[1] - m_simplexPointsP[0]);
//149  m_cachedP2 = m_simplexPointsQ[0] + t * (m_simplexPointsQ[1] - m_simplexPointsQ[0]);
//150  m_cachedV = m_cachedP1 - m_cachedP2;
//151 
//152  reduceVertices(m_cachedBC.m_usedVertices);
//153 
//154  m_cachedValidClosest = m_cachedBC.isValid();
//155  break;
//156  }
//157  case 3:
//158  {
//159  //closest point origin from triangle
//160  btVector3 p (btScalar(0.),btScalar(0.),btScalar(0.));
//161 
//162  const btVector3& a = m_simplexVectorW[0];
//163  const btVector3& b = m_simplexVectorW[1];
//164  const btVector3& c = m_simplexVectorW[2];
//165 
//166  closestPtPointTriangle(p,a,b,c,m_cachedBC);
//167  m_cachedP1 = m_simplexPointsP[0] * m_cachedBC.m_barycentricCoords[0] +
//168  m_simplexPointsP[1] * m_cachedBC.m_barycentricCoords[1] +
//169  m_simplexPointsP[2] * m_cachedBC.m_barycentricCoords[2];
//170 
//171  m_cachedP2 = m_simplexPointsQ[0] * m_cachedBC.m_barycentricCoords[0] +
//172  m_simplexPointsQ[1] * m_cachedBC.m_barycentricCoords[1] +
//173  m_simplexPointsQ[2] * m_cachedBC.m_barycentricCoords[2];
//174 
//175  m_cachedV = m_cachedP1-m_cachedP2;
//176 
//177  reduceVertices (m_cachedBC.m_usedVertices);
//178  m_cachedValidClosest = m_cachedBC.isValid();
//179 
//180  break;
//181  }
//182  case 4:
//183  {
//184 
//185 
//186  btVector3 p (btScalar(0.),btScalar(0.),btScalar(0.));
//187 
//188  const btVector3& a = m_simplexVectorW[0];
//189  const btVector3& b = m_simplexVectorW[1];
//190  const btVector3& c = m_simplexVectorW[2];
//191  const btVector3& d = m_simplexVectorW[3];
//192 
//193  bool hasSeperation = closestPtPointTetrahedron(p,a,b,c,d,m_cachedBC);
//194 
//195  if (hasSeperation)
//196  {
//197 
//198  m_cachedP1 = m_simplexPointsP[0] * m_cachedBC.m_barycentricCoords[0] +
//199  m_simplexPointsP[1] * m_cachedBC.m_barycentricCoords[1] +
//200  m_simplexPointsP[2] * m_cachedBC.m_barycentricCoords[2] +
//201  m_simplexPointsP[3] * m_cachedBC.m_barycentricCoords[3];
//202 
//203  m_cachedP2 = m_simplexPointsQ[0] * m_cachedBC.m_barycentricCoords[0] +
//204  m_simplexPointsQ[1] * m_cachedBC.m_barycentricCoords[1] +
//205  m_simplexPointsQ[2] * m_cachedBC.m_barycentricCoords[2] +
//206  m_simplexPointsQ[3] * m_cachedBC.m_barycentricCoords[3];
//207 
//208  m_cachedV = m_cachedP1-m_cachedP2;
//209  reduceVertices (m_cachedBC.m_usedVertices);
//210  } else
//211  {
//212 // printf("sub distance got penetration\n");
//213 
//214  if (m_cachedBC.m_degenerate)
//215  {
//216  m_cachedValidClosest = false;
//217  } else
//218  {
//219  m_cachedValidClosest = true;
//220  //degenerate case == false, penetration = true + zero
//221  m_cachedV.setValue(btScalar(0.),btScalar(0.),btScalar(0.));
//222  }
//223  break;
//224  }
//225 
//226  m_cachedValidClosest = m_cachedBC.isValid();
//227 
//228  //closest point origin from tetrahedron
//229  break;
//230  }
//231  default:
//232  {
//233  m_cachedValidClosest = false;
//234  }
//235  };
//236  }
//237 
//238  return m_cachedValidClosest;
//239 
//240 }
//241 
//242 //return/calculate the closest vertex
//243 bool btVoronoiSimplexSolver::closest(btVector3& v)
//244 {
//245  bool succes = updateClosestVectorAndPoints();
//246  v = m_cachedV;
//247  return succes;
//248 }
//249 
//250 
//251 
//252 btScalar btVoronoiSimplexSolver::maxVertex()
//253 {
//254  int i, numverts = numVertices();
//255  btScalar maxV = btScalar(0.);
//256  for (i=0;i<numverts;i++)
//257  {
//258  btScalar curLen2 = m_simplexVectorW[i].length2();
//259  if (maxV < curLen2)
//260  maxV = curLen2;
//261  }
//262  return maxV;
//263 }
//264 
//265 
//266 
//267  //return the current simplex
//268 int btVoronoiSimplexSolver::getSimplex(btVector3 *pBuf, btVector3 *qBuf, btVector3 *yBuf) const
//269 {
//270  int i;
//271  for (i=0;i<numVertices();i++)
//272  {
//273  yBuf[i] = m_simplexVectorW[i];
//274  pBuf[i] = m_simplexPointsP[i];
//275  qBuf[i] = m_simplexPointsQ[i];
//276  }
//277  return numVertices();
//278 }
//279 
//280 
//281 
//282 
//283 bool btVoronoiSimplexSolver::inSimplex(const btVector3& w)
//284 {
//285  bool found = false;
//286  int i, numverts = numVertices();
//287  //btScalar maxV = btScalar(0.);
//288 
//289  //w is in the current (reduced) simplex
//290  for (i=0;i<numverts;i++)
//291  {
//292 #ifdef BT_USE_EQUAL_VERTEX_THRESHOLD
//293  if ( m_simplexVectorW[i].distance2(w) <= m_equalVertexThreshold)
//294 #else
//295  if (m_simplexVectorW[i] == w)
//296 #endif
//297  found = true;
//298  }
//299 
//300  //check in case lastW is already removed
//301  if (w == m_lastW)
//302  return true;
//303 
//304  return found;
//305 }
//306 
//307 void btVoronoiSimplexSolver::backup_closest(btVector3& v)
//308 {
//309  v = m_cachedV;
//310 }
//311 
//312 
//313 bool btVoronoiSimplexSolver::emptySimplex() const
//314 {
//315  return (numVertices() == 0);
//316 
//317 }
//318 
//319 void btVoronoiSimplexSolver::compute_points(btVector3& p1, btVector3& p2)
//320 {
//321  updateClosestVectorAndPoints();
//322  p1 = m_cachedP1;
//323  p2 = m_cachedP2;
//324 
//325 }
//326 
//327 
//328 
//329 
//330 bool btVoronoiSimplexSolver::closestPtPointTriangle(const btVector3& p, const btVector3& a, const btVector3& b, const btVector3& c,btSubSimplexClosestResult& result)
//331 {
//332  result.m_usedVertices.reset();
//333 
//334  // Check if P in vertex region outside A
//335  btVector3 ab = b - a;
//336  btVector3 ac = c - a;
//337  btVector3 ap = p - a;
//338  btScalar d1 = ab.dot(ap);
//339  btScalar d2 = ac.dot(ap);
//340  if (d1 <= btScalar(0.0) && d2 <= btScalar(0.0))
//341  {
//342  result.m_closestPointOnSimplex = a;
//343  result.m_usedVertices.usedVertexA = true;
//344  result.setBarycentricCoordinates(1,0,0);
//345  return true;// a; // barycentric coordinates (1,0,0)
//346  }
//347 
//348  // Check if P in vertex region outside B
//349  btVector3 bp = p - b;
//350  btScalar d3 = ab.dot(bp);
//351  btScalar d4 = ac.dot(bp);
//352  if (d3 >= btScalar(0.0) && d4 <= d3)
//353  {
//354  result.m_closestPointOnSimplex = b;
//355  result.m_usedVertices.usedVertexB = true;
//356  result.setBarycentricCoordinates(0,1,0);
//357 
//358  return true; // b; // barycentric coordinates (0,1,0)
//359  }
//360  // Check if P in edge region of AB, if so return projection of P onto AB
//361  btScalar vc = d1*d4 - d3*d2;
//362  if (vc <= btScalar(0.0) && d1 >= btScalar(0.0) && d3 <= btScalar(0.0)) {
//363  btScalar v = d1 / (d1 - d3);
//364  result.m_closestPointOnSimplex = a + v * ab;
//365  result.m_usedVertices.usedVertexA = true;
//366  result.m_usedVertices.usedVertexB = true;
//367  result.setBarycentricCoordinates(1-v,v,0);
//368  return true;
//369  //return a + v * ab; // barycentric coordinates (1-v,v,0)
//370  }
//371 
//372  // Check if P in vertex region outside C
//373  btVector3 cp = p - c;
//374  btScalar d5 = ab.dot(cp);
//375  btScalar d6 = ac.dot(cp);
//376  if (d6 >= btScalar(0.0) && d5 <= d6)
//377  {
//378  result.m_closestPointOnSimplex = c;
//379  result.m_usedVertices.usedVertexC = true;
//380  result.setBarycentricCoordinates(0,0,1);
//381  return true;//c; // barycentric coordinates (0,0,1)
//382  }
//383 
//384  // Check if P in edge region of AC, if so return projection of P onto AC
//385  btScalar vb = d5*d2 - d1*d6;
//386  if (vb <= btScalar(0.0) && d2 >= btScalar(0.0) && d6 <= btScalar(0.0)) {
//387  btScalar w = d2 / (d2 - d6);
//388  result.m_closestPointOnSimplex = a + w * ac;
//389  result.m_usedVertices.usedVertexA = true;
//390  result.m_usedVertices.usedVertexC = true;
//391  result.setBarycentricCoordinates(1-w,0,w);
//392  return true;
//393  //return a + w * ac; // barycentric coordinates (1-w,0,w)
//394  }
//395 
//396  // Check if P in edge region of BC, if so return projection of P onto BC
//397  btScalar va = d3*d6 - d5*d4;
//398  if (va <= btScalar(0.0) && (d4 - d3) >= btScalar(0.0) && (d5 - d6) >= btScalar(0.0)) {
//399  btScalar w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
//400 
//401  result.m_closestPointOnSimplex = b + w * (c - b);
//402  result.m_usedVertices.usedVertexB = true;
//403  result.m_usedVertices.usedVertexC = true;
//404  result.setBarycentricCoordinates(0,1-w,w);
//405  return true;
//406  // return b + w * (c - b); // barycentric coordinates (0,1-w,w)
//407  }
//408 
//409  // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
//410  btScalar denom = btScalar(1.0) / (va + vb + vc);
//411  btScalar v = vb * denom;
//412  btScalar w = vc * denom;
//413 
//414  result.m_closestPointOnSimplex = a + ab * v + ac * w;
//415  result.m_usedVertices.usedVertexA = true;
//416  result.m_usedVertices.usedVertexB = true;
//417  result.m_usedVertices.usedVertexC = true;
//418  result.setBarycentricCoordinates(1-v-w,v,w);
//419 
//420  return true;
//421 // return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = btScalar(1.0) - v - w
//422 
//423 }
//424 
//425 
//426 
//427 
//428 
//430 int btVoronoiSimplexSolver::pointOutsideOfPlane(const btVector3& p, const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& d)
//431 {
//432  btVector3 normal = (b-a).cross(c-a);
//433 
//434  btScalar signp = (p - a).dot(normal); // [AP AB AC]
//435  btScalar signd = (d - a).dot( normal); // [AD AB AC]
//436 
//437 #ifdef CATCH_DEGENERATE_TETRAHEDRON
//438 #ifdef BT_USE_DOUBLE_PRECISION
//439 if (signd * signd < (btScalar(1e-8) * btScalar(1e-8)))
//440  {
//441  return -1;
//442  }
//443 #else
//444  if (signd * signd < (btScalar(1e-4) * btScalar(1e-4)))
//445  {
//446 // printf("affine dependent/degenerate\n");//
//447  return -1;
//448  }
//449 #endif
//450 
//451 #endif
//452  // Points on opposite sides if expression signs are opposite
//453  return signp * signd < btScalar(0.);
//454 }
//455 
//456 
//457 bool btVoronoiSimplexSolver::closestPtPointTetrahedron(const btVector3& p, const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& d, btSubSimplexClosestResult& finalResult)
//458 {
//459  btSubSimplexClosestResult tempResult;
//460 
//461  // Start out assuming point inside all halfspaces, so closest to itself
//462  finalResult.m_closestPointOnSimplex = p;
//463  finalResult.m_usedVertices.reset();
//464  finalResult.m_usedVertices.usedVertexA = true;
//465  finalResult.m_usedVertices.usedVertexB = true;
//466  finalResult.m_usedVertices.usedVertexC = true;
//467  finalResult.m_usedVertices.usedVertexD = true;
//468 
//469  int pointOutsideABC = pointOutsideOfPlane(p, a, b, c, d);
//470  int pointOutsideACD = pointOutsideOfPlane(p, a, c, d, b);
//471  int pointOutsideADB = pointOutsideOfPlane(p, a, d, b, c);
//472  int pointOutsideBDC = pointOutsideOfPlane(p, b, d, c, a);
//473 
//474  if (pointOutsideABC < 0 || pointOutsideACD < 0 || pointOutsideADB < 0 || pointOutsideBDC < 0)
//475  {
//476  finalResult.m_degenerate = true;
//477  return false;
//478  }
//479 
//480  if (!pointOutsideABC && !pointOutsideACD && !pointOutsideADB && !pointOutsideBDC)
//481  {
//482  return false;
//483  }
//484 
//485 
//486  btScalar bestSqDist = FLT_MAX;
//487  // If point outside face abc then compute closest point on abc
//488  if (pointOutsideABC)
//489  {
//490  closestPtPointTriangle(p, a, b, c,tempResult);
//491  btVector3 q = tempResult.m_closestPointOnSimplex;
//492 
//493  btScalar sqDist = (q - p).dot( q - p);
//494  // Update best closest point if (squared) distance is less than current best
//495  if (sqDist < bestSqDist) {
//496  bestSqDist = sqDist;
//497  finalResult.m_closestPointOnSimplex = q;
//498  //convert result bitmask!
//499  finalResult.m_usedVertices.reset();
//500  finalResult.m_usedVertices.usedVertexA = tempResult.m_usedVertices.usedVertexA;
//501  finalResult.m_usedVertices.usedVertexB = tempResult.m_usedVertices.usedVertexB;
//502  finalResult.m_usedVertices.usedVertexC = tempResult.m_usedVertices.usedVertexC;
//503  finalResult.setBarycentricCoordinates(
//504  tempResult.m_barycentricCoords[VERTA],
//505  tempResult.m_barycentricCoords[VERTB],
//506  tempResult.m_barycentricCoords[VERTC],
//507  0
//508  );
//509 
//510  }
//511  }
//512 
//513 
//514  // Repeat test for face acd
//515  if (pointOutsideACD)
//516  {
//517  closestPtPointTriangle(p, a, c, d,tempResult);
//518  btVector3 q = tempResult.m_closestPointOnSimplex;
//519  //convert result bitmask!
//520 
//521  btScalar sqDist = (q - p).dot( q - p);
//522  if (sqDist < bestSqDist)
//523  {
//524  bestSqDist = sqDist;
//525  finalResult.m_closestPointOnSimplex = q;
//526  finalResult.m_usedVertices.reset();
//527  finalResult.m_usedVertices.usedVertexA = tempResult.m_usedVertices.usedVertexA;
//528 
//529  finalResult.m_usedVertices.usedVertexC = tempResult.m_usedVertices.usedVertexB;
//530  finalResult.m_usedVertices.usedVertexD = tempResult.m_usedVertices.usedVertexC;
//531  finalResult.setBarycentricCoordinates(
//532  tempResult.m_barycentricCoords[VERTA],
//533  0,
//534  tempResult.m_barycentricCoords[VERTB],
//535  tempResult.m_barycentricCoords[VERTC]
//536  );
//537 
//538  }
//539  }
//540  // Repeat test for face adb
//541 
//542 
//543  if (pointOutsideADB)
//544  {
//545  closestPtPointTriangle(p, a, d, b,tempResult);
//546  btVector3 q = tempResult.m_closestPointOnSimplex;
//547  //convert result bitmask!
//548 
//549  btScalar sqDist = (q - p).dot( q - p);
//550  if (sqDist < bestSqDist)
//551  {
//552  bestSqDist = sqDist;
//553  finalResult.m_closestPointOnSimplex = q;
//554  finalResult.m_usedVertices.reset();
//555  finalResult.m_usedVertices.usedVertexA = tempResult.m_usedVertices.usedVertexA;
//556  finalResult.m_usedVertices.usedVertexB = tempResult.m_usedVertices.usedVertexC;
//557 
//558  finalResult.m_usedVertices.usedVertexD = tempResult.m_usedVertices.usedVertexB;
//559  finalResult.setBarycentricCoordinates(
//560  tempResult.m_barycentricCoords[VERTA],
//561  tempResult.m_barycentricCoords[VERTC],
//562  0,
//563  tempResult.m_barycentricCoords[VERTB]
//564  );
//565 
//566  }
//567  }
//568  // Repeat test for face bdc
//569 
//570 
//571  if (pointOutsideBDC)
//572  {
//573  closestPtPointTriangle(p, b, d, c,tempResult);
//574  btVector3 q = tempResult.m_closestPointOnSimplex;
//575  //convert result bitmask!
//576  btScalar sqDist = (q - p).dot( q - p);
//577  if (sqDist < bestSqDist)
//578  {
//579  bestSqDist = sqDist;
//580  finalResult.m_closestPointOnSimplex = q;
//581  finalResult.m_usedVertices.reset();
//582  //
//583  finalResult.m_usedVertices.usedVertexB = tempResult.m_usedVertices.usedVertexA;
//584  finalResult.m_usedVertices.usedVertexC = tempResult.m_usedVertices.usedVertexC;
//585  finalResult.m_usedVertices.usedVertexD = tempResult.m_usedVertices.usedVertexB;
//586 
//587  finalResult.setBarycentricCoordinates(
//588  0,
//589  tempResult.m_barycentricCoords[VERTA],
//590  tempResult.m_barycentricCoords[VERTC],
//591  tempResult.m_barycentricCoords[VERTB]
//592  );
//593 
//594  }
//595  }
//596 
//597  //help! we ended up full !
//598 
//599  if (finalResult.m_usedVertices.usedVertexA &&
//600  finalResult.m_usedVertices.usedVertexB &&
//601  finalResult.m_usedVertices.usedVertexC &&
//602  finalResult.m_usedVertices.usedVertexD)
//603  {
//604  return true;
//605  }
//606 
//607  return true;
//608 }
//
//
//
//
//16 #include "btMinkowskiPenetrationDepthSolver.h"
//17 #include "BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h"
//18 #include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
//19 #include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
//20 #include "BulletCollision/CollisionShapes/btConvexShape.h"
//21 
//22 #define NUM_UNITSPHERE_POINTS 42
//23 
//24 
//25 bool btMinkowskiPenetrationDepthSolver::calcPenDepth(btSimplexSolverInterface& simplexSolver,
//26  const btConvexShape* convexA,const btConvexShape* convexB,
//27  const btTransform& transA,const btTransform& transB,
//28  btVector3& v, btVector3& pa, btVector3& pb,
//29  class btIDebugDraw* debugDraw,btStackAlloc* stackAlloc
//30  )
//31 {
//32 
//33  (void)stackAlloc;
//34  (void)v;
//35 
//36  bool check2d= convexA->isConvex2d() && convexB->isConvex2d();
//37 
//38  struct btIntermediateResult : public btDiscreteCollisionDetectorInterface::Result
//39  {
//40 
//41  btIntermediateResult():m_hasResult(false)
//42  {
//43  }
//44 
//45  btVector3 m_normalOnBInWorld;
//46  btVector3 m_pointInWorld;
//47  btScalar m_depth;
//48  bool m_hasResult;
//49 
//50  virtual void setShapeIdentifiersA(int partId0,int index0)
//51  {
//52  (void)partId0;
//53  (void)index0;
//54  }
//55  virtual void setShapeIdentifiersB(int partId1,int index1)
//56  {
//57  (void)partId1;
//58  (void)index1;
//59  }
//60  void addContactPoint(const btVector3& normalOnBInWorld,const btVector3& pointInWorld,btScalar depth)
//61  {
//62  m_normalOnBInWorld = normalOnBInWorld;
//63  m_pointInWorld = pointInWorld;
//64  m_depth = depth;
//65  m_hasResult = true;
//66  }
//67  };
//68 
//69  //just take fixed number of orientation, and sample the penetration depth in that direction
//70  btScalar minProj = btScalar(BT_LARGE_FLOAT);
//71  btVector3 minNorm(btScalar(0.), btScalar(0.), btScalar(0.));
//72  btVector3 minA,minB;
//73  btVector3 seperatingAxisInA,seperatingAxisInB;
//74  btVector3 pInA,qInB,pWorld,qWorld,w;
//75 
//76 #ifndef __SPU__
//77 #define USE_BATCHED_SUPPORT 1
//78 #endif
//79 #ifdef USE_BATCHED_SUPPORT
//80 
//81  btVector3 supportVerticesABatch[NUM_UNITSPHERE_POINTS+MAX_PREFERRED_PENETRATION_DIRECTIONS*2];
//82  btVector3 supportVerticesBBatch[NUM_UNITSPHERE_POINTS+MAX_PREFERRED_PENETRATION_DIRECTIONS*2];
//83  btVector3 seperatingAxisInABatch[NUM_UNITSPHERE_POINTS+MAX_PREFERRED_PENETRATION_DIRECTIONS*2];
//84  btVector3 seperatingAxisInBBatch[NUM_UNITSPHERE_POINTS+MAX_PREFERRED_PENETRATION_DIRECTIONS*2];
//85  int i;
//86 
//87  int numSampleDirections = NUM_UNITSPHERE_POINTS;
//88 
//89  for (i=0;i<numSampleDirections;i++)
//90  {
//91  btVector3 norm = getPenetrationDirections()[i];
//92  seperatingAxisInABatch[i] = (-norm) * transA.getBasis() ;
//93  seperatingAxisInBBatch[i] = norm * transB.getBasis() ;
//94  }
//95 
//96  {
//97  int numPDA = convexA->getNumPreferredPenetrationDirections();
//98  if (numPDA)
//99  {
//100  for (int i=0;i<numPDA;i++)
//101  {
//102  btVector3 norm;
//103  convexA->getPreferredPenetrationDirection(i,norm);
//104  norm = transA.getBasis() * norm;
//105  getPenetrationDirections()[numSampleDirections] = norm;
//106  seperatingAxisInABatch[numSampleDirections] = (-norm) * transA.getBasis();
//107  seperatingAxisInBBatch[numSampleDirections] = norm * transB.getBasis();
//108  numSampleDirections++;
//109  }
//110  }
//111  }
//112 
//113  {
//114  int numPDB = convexB->getNumPreferredPenetrationDirections();
//115  if (numPDB)
//116  {
//117  for (int i=0;i<numPDB;i++)
//118  {
//119  btVector3 norm;
//120  convexB->getPreferredPenetrationDirection(i,norm);
//121  norm = transB.getBasis() * norm;
//122  getPenetrationDirections()[numSampleDirections] = norm;
//123  seperatingAxisInABatch[numSampleDirections] = (-norm) * transA.getBasis();
//124  seperatingAxisInBBatch[numSampleDirections] = norm * transB.getBasis();
//125  numSampleDirections++;
//126  }
//127  }
//128  }
//129 
//130 
//131 
//132 
//133  convexA->batchedUnitVectorGetSupportingVertexWithoutMargin(seperatingAxisInABatch,supportVerticesABatch,numSampleDirections);
//134  convexB->batchedUnitVectorGetSupportingVertexWithoutMargin(seperatingAxisInBBatch,supportVerticesBBatch,numSampleDirections);
//135 
//136  for (i=0;i<numSampleDirections;i++)
//137  {
//138  btVector3 norm = getPenetrationDirections()[i];
//139  if (check2d)
//140  {
//141  norm[2] = 0.f;
//142  }
//143  if (norm.length2()>0.01)
//144  {
//145 
//146  seperatingAxisInA = seperatingAxisInABatch[i];
//147  seperatingAxisInB = seperatingAxisInBBatch[i];
//148 
//149  pInA = supportVerticesABatch[i];
//150  qInB = supportVerticesBBatch[i];
//151 
//152  pWorld = transA(pInA);
//153  qWorld = transB(qInB);
//154  if (check2d)
//155  {
//156  pWorld[2] = 0.f;
//157  qWorld[2] = 0.f;
//158  }
//159 
//160  w = qWorld - pWorld;
//161  btScalar delta = norm.dot(w);
//162  //find smallest delta
//163  if (delta < minProj)
//164  {
//165  minProj = delta;
//166  minNorm = norm;
//167  minA = pWorld;
//168  minB = qWorld;
//169  }
//170  }
//171  }
//172 #else
//173 
//174  int numSampleDirections = NUM_UNITSPHERE_POINTS;
//175 
//176 #ifndef __SPU__
//177  {
//178  int numPDA = convexA->getNumPreferredPenetrationDirections();
//179  if (numPDA)
//180  {
//181  for (int i=0;i<numPDA;i++)
//182  {
//183  btVector3 norm;
//184  convexA->getPreferredPenetrationDirection(i,norm);
//185  norm = transA.getBasis() * norm;
//186  getPenetrationDirections()[numSampleDirections] = norm;
//187  numSampleDirections++;
//188  }
//189  }
//190  }
//191 
//192  {
//193  int numPDB = convexB->getNumPreferredPenetrationDirections();
//194  if (numPDB)
//195  {
//196  for (int i=0;i<numPDB;i++)
//197  {
//198  btVector3 norm;
//199  convexB->getPreferredPenetrationDirection(i,norm);
//200  norm = transB.getBasis() * norm;
//201  getPenetrationDirections()[numSampleDirections] = norm;
//202  numSampleDirections++;
//203  }
//204  }
//205  }
//206 #endif // __SPU__
//207 
//208  for (int i=0;i<numSampleDirections;i++)
//209  {
//210  const btVector3& norm = getPenetrationDirections()[i];
//211  seperatingAxisInA = (-norm)* transA.getBasis();
//212  seperatingAxisInB = norm* transB.getBasis();
//213  pInA = convexA->localGetSupportVertexWithoutMarginNonVirtual(seperatingAxisInA);
//214  qInB = convexB->localGetSupportVertexWithoutMarginNonVirtual(seperatingAxisInB);
//215  pWorld = transA(pInA);
//216  qWorld = transB(qInB);
//217  w = qWorld - pWorld;
//218  btScalar delta = norm.dot(w);
//219  //find smallest delta
//220  if (delta < minProj)
//221  {
//222  minProj = delta;
//223  minNorm = norm;
//224  minA = pWorld;
//225  minB = qWorld;
//226  }
//227  }
//228 #endif //USE_BATCHED_SUPPORT
//229 
//230  //add the margins
//231 
//232  minA += minNorm*convexA->getMarginNonVirtual();
//233  minB -= minNorm*convexB->getMarginNonVirtual();
//234  //no penetration
//235  if (minProj < btScalar(0.))
//236  return false;
//237 
//238  btScalar extraSeparation = 0.5f;
//239  minProj += extraSeparation+(convexA->getMarginNonVirtual() + convexB->getMarginNonVirtual());
//240 
//241 
//242 
//243 
//244 
//245 //#define DEBUG_DRAW 1
//246 #ifdef DEBUG_DRAW
//247  if (debugDraw)
//248  {
//249  btVector3 color(0,1,0);
//250  debugDraw->drawLine(minA,minB,color);
//251  color = btVector3 (1,1,1);
//252  btVector3 vec = minB-minA;
//253  btScalar prj2 = minNorm.dot(vec);
//254  debugDraw->drawLine(minA,minA+(minNorm*minProj),color);
//255 
//256  }
//257 #endif //DEBUG_DRAW
//258 
//259 
//260 
//261  btGjkPairDetector gjkdet(convexA,convexB,&simplexSolver,0);
//262 
//263  btScalar offsetDist = minProj;
//264  btVector3 offset = minNorm * offsetDist;
//265 
//266 
//267 
//268  btGjkPairDetector::ClosestPointInput input;
//269 
//270  btVector3 newOrg = transA.getOrigin() + offset;
//271 
//272  btTransform displacedTrans = transA;
//273  displacedTrans.setOrigin(newOrg);
//274 
//275  input.m_transformA = displacedTrans;
//276  input.m_transformB = transB;
//277  input.m_maximumDistanceSquared = btScalar(BT_LARGE_FLOAT);//minProj;
//278 
//279  btIntermediateResult res;
//280  gjkdet.setCachedSeperatingAxis(-minNorm);
//281  gjkdet.getClosestPoints(input,res,debugDraw);
//282 
//283  btScalar correctedMinNorm = minProj - res.m_depth;
//284 
//285 
//286  //the penetration depth is over-estimated, relax it
//287  btScalar penetration_relaxation= btScalar(1.);
//288  minNorm*=penetration_relaxation;
//289 
//290 
//291  if (res.m_hasResult)
//292  {
//293 
//294  pa = res.m_pointInWorld - minNorm * correctedMinNorm;
//295  pb = res.m_pointInWorld;
//296  v = minNorm;
//297 
//298 #ifdef DEBUG_DRAW
//299  if (debugDraw)
//300  {
//301  btVector3 color(1,0,0);
//302  debugDraw->drawLine(pa,pb,color);
//303  }
//304 #endif//DEBUG_DRAW
//305 
//306 
//307  }
//308  return res.m_hasResult;
//309 }
//310 
//311 btVector3* btMinkowskiPenetrationDepthSolver::getPenetrationDirections()
//312 {
//313  static btVector3 sPenetrationDirections[NUM_UNITSPHERE_POINTS+MAX_PREFERRED_PENETRATION_DIRECTIONS*2] =
//314  {
//315  btVector3(btScalar(0.000000) , btScalar(-0.000000),btScalar(-1.000000)),
//316  btVector3(btScalar(0.723608) , btScalar(-0.525725),btScalar(-0.447219)),
//317  btVector3(btScalar(-0.276388) , btScalar(-0.850649),btScalar(-0.447219)),
//318  btVector3(btScalar(-0.894426) , btScalar(-0.000000),btScalar(-0.447216)),
//319  btVector3(btScalar(-0.276388) , btScalar(0.850649),btScalar(-0.447220)),
//320  btVector3(btScalar(0.723608) , btScalar(0.525725),btScalar(-0.447219)),
//321  btVector3(btScalar(0.276388) , btScalar(-0.850649),btScalar(0.447220)),
//322  btVector3(btScalar(-0.723608) , btScalar(-0.525725),btScalar(0.447219)),
//323  btVector3(btScalar(-0.723608) , btScalar(0.525725),btScalar(0.447219)),
//324  btVector3(btScalar(0.276388) , btScalar(0.850649),btScalar(0.447219)),
//325  btVector3(btScalar(0.894426) , btScalar(0.000000),btScalar(0.447216)),
//326  btVector3(btScalar(-0.000000) , btScalar(0.000000),btScalar(1.000000)),
//327  btVector3(btScalar(0.425323) , btScalar(-0.309011),btScalar(-0.850654)),
//328  btVector3(btScalar(-0.162456) , btScalar(-0.499995),btScalar(-0.850654)),
//329  btVector3(btScalar(0.262869) , btScalar(-0.809012),btScalar(-0.525738)),
//330  btVector3(btScalar(0.425323) , btScalar(0.309011),btScalar(-0.850654)),
//331  btVector3(btScalar(0.850648) , btScalar(-0.000000),btScalar(-0.525736)),
//332  btVector3(btScalar(-0.525730) , btScalar(-0.000000),btScalar(-0.850652)),
//333  btVector3(btScalar(-0.688190) , btScalar(-0.499997),btScalar(-0.525736)),
//334  btVector3(btScalar(-0.162456) , btScalar(0.499995),btScalar(-0.850654)),
//335  btVector3(btScalar(-0.688190) , btScalar(0.499997),btScalar(-0.525736)),
//336  btVector3(btScalar(0.262869) , btScalar(0.809012),btScalar(-0.525738)),
//337  btVector3(btScalar(0.951058) , btScalar(0.309013),btScalar(0.000000)),
//338  btVector3(btScalar(0.951058) , btScalar(-0.309013),btScalar(0.000000)),
//339  btVector3(btScalar(0.587786) , btScalar(-0.809017),btScalar(0.000000)),
//340  btVector3(btScalar(0.000000) , btScalar(-1.000000),btScalar(0.000000)),
//341  btVector3(btScalar(-0.587786) , btScalar(-0.809017),btScalar(0.000000)),
//342  btVector3(btScalar(-0.951058) , btScalar(-0.309013),btScalar(-0.000000)),
//343  btVector3(btScalar(-0.951058) , btScalar(0.309013),btScalar(-0.000000)),
//344  btVector3(btScalar(-0.587786) , btScalar(0.809017),btScalar(-0.000000)),
//345  btVector3(btScalar(-0.000000) , btScalar(1.000000),btScalar(-0.000000)),
//346  btVector3(btScalar(0.587786) , btScalar(0.809017),btScalar(-0.000000)),
//347  btVector3(btScalar(0.688190) , btScalar(-0.499997),btScalar(0.525736)),
//348  btVector3(btScalar(-0.262869) , btScalar(-0.809012),btScalar(0.525738)),
//349  btVector3(btScalar(-0.850648) , btScalar(0.000000),btScalar(0.525736)),
//350  btVector3(btScalar(-0.262869) , btScalar(0.809012),btScalar(0.525738)),
//351  btVector3(btScalar(0.688190) , btScalar(0.499997),btScalar(0.525736)),
//352  btVector3(btScalar(0.525730) , btScalar(0.000000),btScalar(0.850652)),
//353  btVector3(btScalar(0.162456) , btScalar(-0.499995),btScalar(0.850654)),
//354  btVector3(btScalar(-0.425323) , btScalar(-0.309011),btScalar(0.850654)),
//355  btVector3(btScalar(-0.425323) , btScalar(0.309011),btScalar(0.850654)),
//356  btVector3(btScalar(0.162456) , btScalar(0.499995),btScalar(0.850654))
//357  };
//358 
//359  return sPenetrationDirections;
//360 }
//
//
//
//19 //define BT_DISABLE_CAPSULE_CAPSULE_COLLIDER 1
//20 //#define ZERO_MARGIN
//21 
//22 #include "btConvexConvexAlgorithm.h"
//23 
//24 //#include <stdio.h>
//25 #include "BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h"
//26 #include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
//27 #include "BulletCollision/CollisionDispatch/btCollisionObject.h"
//28 #include "BulletCollision/CollisionShapes/btConvexShape.h"
//29 #include "BulletCollision/CollisionShapes/btCapsuleShape.h"
//30 #include "BulletCollision/CollisionShapes/btTriangleShape.h"
//31 
//32 
//33 
//34 #include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
//35 #include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
//36 #include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
//37 #include "BulletCollision/CollisionShapes/btBoxShape.h"
//38 #include "BulletCollision/CollisionDispatch/btManifoldResult.h"
//39 
//40 #include "BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h"
//41 #include "BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h"
//42 #include "BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h"
//43 #include "BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h"
//44 
//45 
//46 
//47 #include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
//48 #include "BulletCollision/CollisionShapes/btSphereShape.h"
//49 
//50 #include "BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h"
//51 
//52 #include "BulletCollision/NarrowPhaseCollision/btGjkEpa2.h"
//53 #include "BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h"
//54 #include "BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.h"
//55 #include "BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h"
//56 
//58 
//59 
//60 
//61 static SIMD_FORCE_INLINE void segmentsClosestPoints(
//62  btVector3& ptsVector,
//63  btVector3& offsetA,
//64  btVector3& offsetB,
//65  btScalar& tA, btScalar& tB,
//66  const btVector3& translation,
//67  const btVector3& dirA, btScalar hlenA,
//68  const btVector3& dirB, btScalar hlenB )
//69 {
//70  // compute the parameters of the closest points on each line segment
//71 
//72  btScalar dirA_dot_dirB = btDot(dirA,dirB);
//73  btScalar dirA_dot_trans = btDot(dirA,translation);
//74  btScalar dirB_dot_trans = btDot(dirB,translation);
//75 
//76  btScalar denom = 1.0f - dirA_dot_dirB * dirA_dot_dirB;
//77 
//78  if ( denom == 0.0f ) {
//79  tA = 0.0f;
//80  } else {
//81  tA = ( dirA_dot_trans - dirB_dot_trans * dirA_dot_dirB ) / denom;
//82  if ( tA < -hlenA )
//83  tA = -hlenA;
//84  else if ( tA > hlenA )
//85  tA = hlenA;
//86  }
//87 
//88  tB = tA * dirA_dot_dirB - dirB_dot_trans;
//89 
//90  if ( tB < -hlenB ) {
//91  tB = -hlenB;
//92  tA = tB * dirA_dot_dirB + dirA_dot_trans;
//93 
//94  if ( tA < -hlenA )
//95  tA = -hlenA;
//96  else if ( tA > hlenA )
//97  tA = hlenA;
//98  } else if ( tB > hlenB ) {
//99  tB = hlenB;
//100  tA = tB * dirA_dot_dirB + dirA_dot_trans;
//101 
//102  if ( tA < -hlenA )
//103  tA = -hlenA;
//104  else if ( tA > hlenA )
//105  tA = hlenA;
//106  }
//107 
//108  // compute the closest points relative to segment centers.
//109 
//110  offsetA = dirA * tA;
//111  offsetB = dirB * tB;
//112 
//113  ptsVector = translation - offsetA + offsetB;
//114 }
//115 
//116 
//117 static SIMD_FORCE_INLINE btScalar capsuleCapsuleDistance(
//118  btVector3& normalOnB,
//119  btVector3& pointOnB,
//120  btScalar capsuleLengthA,
//121  btScalar capsuleRadiusA,
//122  btScalar capsuleLengthB,
//123  btScalar capsuleRadiusB,
//124  int capsuleAxisA,
//125  int capsuleAxisB,
//126  const btTransform& transformA,
//127  const btTransform& transformB,
//128  btScalar distanceThreshold )
//129 {
//130  btVector3 directionA = transformA.getBasis().getColumn(capsuleAxisA);
//131  btVector3 translationA = transformA.getOrigin();
//132  btVector3 directionB = transformB.getBasis().getColumn(capsuleAxisB);
//133  btVector3 translationB = transformB.getOrigin();
//134 
//135  // translation between centers
//136 
//137  btVector3 translation = translationB - translationA;
//138 
//139  // compute the closest points of the capsule line segments
//140 
//141  btVector3 ptsVector; // the vector between the closest points
//142 
//143  btVector3 offsetA, offsetB; // offsets from segment centers to their closest points
//144  btScalar tA, tB; // parameters on line segment
//145 
//146  segmentsClosestPoints( ptsVector, offsetA, offsetB, tA, tB, translation,
//147  directionA, capsuleLengthA, directionB, capsuleLengthB );
//148 
//149  btScalar distance = ptsVector.length() - capsuleRadiusA - capsuleRadiusB;
//150 
//151  if ( distance > distanceThreshold )
//152  return distance;
//153 
//154  btScalar lenSqr = ptsVector.length2();
//155  if (lenSqr<= (SIMD_EPSILON*SIMD_EPSILON))
//156  {
//157  //degenerate case where 2 capsules are likely at the same location: take a vector tangential to 'directionA'
//158  btVector3 q;
//159  btPlaneSpace1(directionA,normalOnB,q);
//160  } else
//161  {
//162  // compute the contact normal
//163  normalOnB = ptsVector*-btRecipSqrt(lenSqr);
//164  }
//165  pointOnB = transformB.getOrigin()+offsetB + normalOnB * capsuleRadiusB;
//166 
//167  return distance;
//168 }
//169 
//170 
//171 
//172 
//173 
//174 
//175 
//177 
//178 
//179 
//180 
//181 
//182 btConvexConvexAlgorithm::CreateFunc::CreateFunc(btSimplexSolverInterface* simplexSolver, btConvexPenetrationDepthSolver* pdSolver)
//183 {
//184  m_numPerturbationIterations = 0;
//185  m_minimumPointsPerturbationThreshold = 3;
//186  m_simplexSolver = simplexSolver;
//187  m_pdSolver = pdSolver;
//188 }
//189 
//190 btConvexConvexAlgorithm::CreateFunc::~CreateFunc()
//191 {
//192 }
//193 
//194 btConvexConvexAlgorithm::btConvexConvexAlgorithm(btPersistentManifold* mf,const btCollisionAlgorithmConstructionInfo& ci,const btCollisionObjectWrapper* body0Wrap,const btCollisionObjectWrapper* body1Wrap,btSimplexSolverInterface* simplexSolver, btConvexPenetrationDepthSolver* pdSolver,int numPerturbationIterations, int minimumPointsPerturbationThreshold)
//195 : btActivatingCollisionAlgorithm(ci,body0Wrap,body1Wrap),
//196 m_simplexSolver(simplexSolver),
//197 m_pdSolver(pdSolver),
//198 m_ownManifold (false),
//199 m_manifoldPtr(mf),
//200 m_lowLevelOfDetail(false),
//201 #ifdef USE_SEPDISTANCE_UTIL2
//202 m_sepDistance((static_cast<btConvexShape*>(body0->getCollisionShape()))->getAngularMotionDisc(),
//203  (static_cast<btConvexShape*>(body1->getCollisionShape()))->getAngularMotionDisc()),
//204 #endif
//205 m_numPerturbationIterations(numPerturbationIterations),
//206 m_minimumPointsPerturbationThreshold(minimumPointsPerturbationThreshold)
//207 {
//208  (void)body0Wrap;
//209  (void)body1Wrap;
//210 }
//211 
//212 
//213 
//214 
//215 btConvexConvexAlgorithm::~btConvexConvexAlgorithm()
//216 {
//217  if (m_ownManifold)
//218  {
//219  if (m_manifoldPtr)
//220  m_dispatcher->releaseManifold(m_manifoldPtr);
//221  }
//222 }
//223 
//224 void btConvexConvexAlgorithm ::setLowLevelOfDetail(bool useLowLevel)
//225 {
//226  m_lowLevelOfDetail = useLowLevel;
//227 }
//228 
//229 
//230 struct btPerturbedContactResult : public btManifoldResult
//231 {
//232  btManifoldResult* m_originalManifoldResult;
//233  btTransform m_transformA;
//234  btTransform m_transformB;
//235  btTransform m_unPerturbedTransform;
//236  bool m_perturbA;
//237  btIDebugDraw* m_debugDrawer;
//238 
//239 
//240  btPerturbedContactResult(btManifoldResult* originalResult,const btTransform& transformA,const btTransform& transformB,const btTransform& unPerturbedTransform,bool perturbA,btIDebugDraw* debugDrawer)
//241  :m_originalManifoldResult(originalResult),
//242  m_transformA(transformA),
//243  m_transformB(transformB),
//244  m_unPerturbedTransform(unPerturbedTransform),
//245  m_perturbA(perturbA),
//246  m_debugDrawer(debugDrawer)
//247  {
//248  }
//249  virtual ~ btPerturbedContactResult()
//250  {
//251  }
//252 
//253  virtual void addContactPoint(const btVector3& normalOnBInWorld,const btVector3& pointInWorld,btScalar orgDepth)
//254  {
//255  btVector3 endPt,startPt;
//256  btScalar newDepth;
//257  btVector3 newNormal;
//258 
//259  if (m_perturbA)
//260  {
//261  btVector3 endPtOrg = pointInWorld + normalOnBInWorld*orgDepth;
//262  endPt = (m_unPerturbedTransform*m_transformA.inverse())(endPtOrg);
//263  newDepth = (endPt - pointInWorld).dot(normalOnBInWorld);
//264  startPt = endPt+normalOnBInWorld*newDepth;
//265  } else
//266  {
//267  endPt = pointInWorld + normalOnBInWorld*orgDepth;
//268  startPt = (m_unPerturbedTransform*m_transformB.inverse())(pointInWorld);
//269  newDepth = (endPt - startPt).dot(normalOnBInWorld);
//270 
//271  }
//272 
//273 //#define DEBUG_CONTACTS 1
//274 #ifdef DEBUG_CONTACTS
//275  m_debugDrawer->drawLine(startPt,endPt,btVector3(1,0,0));
//276  m_debugDrawer->drawSphere(startPt,0.05,btVector3(0,1,0));
//277  m_debugDrawer->drawSphere(endPt,0.05,btVector3(0,0,1));
//278 #endif //DEBUG_CONTACTS
//279 
//280 
//281  m_originalManifoldResult->addContactPoint(normalOnBInWorld,startPt,newDepth);
//282  }
//283 
//284 };
//285 
//286 extern btScalar gContactBreakingThreshold;
//287 
//288 
//289 //
//290 // Convex-Convex collision algorithm
//291 //
//292 void btConvexConvexAlgorithm ::processCollision (const btCollisionObjectWrapper* body0Wrap,const btCollisionObjectWrapper* body1Wrap,const btDispatcherInfo& dispatchInfo,btManifoldResult* resultOut)
//293 {
//294 
//295  if (!m_manifoldPtr)
//296  {
//297  //swapped?
//298  m_manifoldPtr = m_dispatcher->getNewManifold(body0Wrap->getCollisionObject(),body1Wrap->getCollisionObject());
//299  m_ownManifold = true;
//300  }
//301  resultOut->setPersistentManifold(m_manifoldPtr);
//302 
//303  //comment-out next line to test multi-contact generation
//304  //resultOut->getPersistentManifold()->clearManifold();
//305 
//306 
//307  const btConvexShape* min0 = static_cast<const btConvexShape*>(body0Wrap->getCollisionShape());
//308  const btConvexShape* min1 = static_cast<const btConvexShape*>(body1Wrap->getCollisionShape());
//309 
//310  btVector3 normalOnB;
//311  btVector3 pointOnBWorld;
//312 #ifndef BT_DISABLE_CAPSULE_CAPSULE_COLLIDER
//313  if ((min0->getShapeType() == CAPSULE_SHAPE_PROXYTYPE) && (min1->getShapeType() == CAPSULE_SHAPE_PROXYTYPE))
//314  {
//315  btCapsuleShape* capsuleA = (btCapsuleShape*) min0;
//316  btCapsuleShape* capsuleB = (btCapsuleShape*) min1;
//317  // btVector3 localScalingA = capsuleA->getLocalScaling();
//318  // btVector3 localScalingB = capsuleB->getLocalScaling();
//319 
//320  btScalar threshold = m_manifoldPtr->getContactBreakingThreshold();
//321 
//322  btScalar dist = capsuleCapsuleDistance(normalOnB, pointOnBWorld,capsuleA->getHalfHeight(),capsuleA->getRadius(),
//323  capsuleB->getHalfHeight(),capsuleB->getRadius(),capsuleA->getUpAxis(),capsuleB->getUpAxis(),
//324  body0Wrap->getWorldTransform(),body1Wrap->getWorldTransform(),threshold);
//325 
//326  if (dist<threshold)
//327  {
//328  btAssert(normalOnB.length2()>=(SIMD_EPSILON*SIMD_EPSILON));
//329  resultOut->addContactPoint(normalOnB,pointOnBWorld,dist);
//330  }
//331  resultOut->refreshContactPoints();
//332  return;
//333  }
//334 #endif //BT_DISABLE_CAPSULE_CAPSULE_COLLIDER
//335 
//336 
//337 
//338 
//339 #ifdef USE_SEPDISTANCE_UTIL2
//340  if (dispatchInfo.m_useConvexConservativeDistanceUtil)
//341  {
//342  m_sepDistance.updateSeparatingDistance(body0->getWorldTransform(),body1->getWorldTransform());
//343  }
//344 
//345  if (!dispatchInfo.m_useConvexConservativeDistanceUtil || m_sepDistance.getConservativeSeparatingDistance()<=0.f)
//346 #endif //USE_SEPDISTANCE_UTIL2
//347 
//348  {
//349 
//350 
//351  btGjkPairDetector::ClosestPointInput input;
//352 
//353  btGjkPairDetector gjkPairDetector(min0,min1,m_simplexSolver,m_pdSolver);
//354  //TODO: if (dispatchInfo.m_useContinuous)
//355  gjkPairDetector.setMinkowskiA(min0);
//356  gjkPairDetector.setMinkowskiB(min1);
//357 
//358 #ifdef USE_SEPDISTANCE_UTIL2
//359  if (dispatchInfo.m_useConvexConservativeDistanceUtil)
//360  {
//361  input.m_maximumDistanceSquared = BT_LARGE_FLOAT;
//362  } else
//363 #endif //USE_SEPDISTANCE_UTIL2
//364  {
//365  //if (dispatchInfo.m_convexMaxDistanceUseCPT)
//366  //{
//367  // input.m_maximumDistanceSquared = min0->getMargin() + min1->getMargin() + m_manifoldPtr->getContactProcessingThreshold();
//368  //} else
//369  //{
//370  input.m_maximumDistanceSquared = min0->getMargin() + min1->getMargin() + m_manifoldPtr->getContactBreakingThreshold();
//371 // }
//372 
//373  input.m_maximumDistanceSquared*= input.m_maximumDistanceSquared;
//374  }
//375 
//376  input.m_stackAlloc = dispatchInfo.m_stackAllocator;
//377  input.m_transformA = body0Wrap->getWorldTransform();
//378  input.m_transformB = body1Wrap->getWorldTransform();
//379 
//380 
//381 
//382 
//383 
//384 #ifdef USE_SEPDISTANCE_UTIL2
//385  btScalar sepDist = 0.f;
//386  if (dispatchInfo.m_useConvexConservativeDistanceUtil)
//387  {
//388  sepDist = gjkPairDetector.getCachedSeparatingDistance();
//389  if (sepDist>SIMD_EPSILON)
//390  {
//391  sepDist += dispatchInfo.m_convexConservativeDistanceThreshold;
//392  //now perturbe directions to get multiple contact points
//393 
//394  }
//395  }
//396 #endif //USE_SEPDISTANCE_UTIL2
//397 
//398  if (min0->isPolyhedral() && min1->isPolyhedral())
//399  {
//400 
//401 
//402  struct btDummyResult : public btDiscreteCollisionDetectorInterface::Result
//403  {
//404  virtual void setShapeIdentifiersA(int partId0,int index0){}
//405  virtual void setShapeIdentifiersB(int partId1,int index1){}
//406  virtual void addContactPoint(const btVector3& normalOnBInWorld,const btVector3& pointInWorld,btScalar depth)
//407  {
//408  }
//409  };
//410 
//411 
//412  struct btWithoutMarginResult : public btDiscreteCollisionDetectorInterface::Result
//413  {
//414  btDiscreteCollisionDetectorInterface::Result* m_originalResult;
//415  btVector3 m_reportedNormalOnWorld;
//416  btScalar m_marginOnA;
//417  btScalar m_marginOnB;
//418  btScalar m_reportedDistance;
//419 
//420  bool m_foundResult;
//421  btWithoutMarginResult(btDiscreteCollisionDetectorInterface::Result* result, btScalar marginOnA, btScalar marginOnB)
//422  :m_originalResult(result),
//423  m_marginOnA(marginOnA),
//424  m_marginOnB(marginOnB),
//425  m_foundResult(false)
//426  {
//427  }
//428 
//429  virtual void setShapeIdentifiersA(int partId0,int index0){}
//430  virtual void setShapeIdentifiersB(int partId1,int index1){}
//431  virtual void addContactPoint(const btVector3& normalOnBInWorld,const btVector3& pointInWorldOrg,btScalar depthOrg)
//432  {
//433  m_reportedDistance = depthOrg;
//434  m_reportedNormalOnWorld = normalOnBInWorld;
//435 
//436  btVector3 adjustedPointB = pointInWorldOrg - normalOnBInWorld*m_marginOnB;
//437  m_reportedDistance = depthOrg+(m_marginOnA+m_marginOnB);
//438  if (m_reportedDistance<0.f)
//439  {
//440  m_foundResult = true;
//441  }
//442  m_originalResult->addContactPoint(normalOnBInWorld,adjustedPointB,m_reportedDistance);
//443  }
//444  };
//445 
//446 
//447  btDummyResult dummy;
//448 
//450 
//451  btScalar min0Margin = min0->getShapeType()==BOX_SHAPE_PROXYTYPE? 0.f : min0->getMargin();
//452  btScalar min1Margin = min1->getShapeType()==BOX_SHAPE_PROXYTYPE? 0.f : min1->getMargin();
//453 
//454  btWithoutMarginResult withoutMargin(resultOut, min0Margin,min1Margin);
//455 
//456  btPolyhedralConvexShape* polyhedronA = (btPolyhedralConvexShape*) min0;
//457  btPolyhedralConvexShape* polyhedronB = (btPolyhedralConvexShape*) min1;
//458  if (polyhedronA->getConvexPolyhedron() && polyhedronB->getConvexPolyhedron())
//459  {
//460 
//461 
//462 
//463 
//464  btScalar threshold = m_manifoldPtr->getContactBreakingThreshold();
//465 
//466  btScalar minDist = -1e30f;
//467  btVector3 sepNormalWorldSpace;
//468  bool foundSepAxis = true;
//469 
//470  if (dispatchInfo.m_enableSatConvex)
//471  {
//472  foundSepAxis = btPolyhedralContactClipping::findSeparatingAxis(
//473  *polyhedronA->getConvexPolyhedron(), *polyhedronB->getConvexPolyhedron(),
//474  body0Wrap->getWorldTransform(),
//475  body1Wrap->getWorldTransform(),
//476  sepNormalWorldSpace,*resultOut);
//477  } else
//478  {
//479 #ifdef ZERO_MARGIN
//480  gjkPairDetector.setIgnoreMargin(true);
//481  gjkPairDetector.getClosestPoints(input,*resultOut,dispatchInfo.m_debugDraw);
//482 #else
//483 
//484 
//485  gjkPairDetector.getClosestPoints(input,withoutMargin,dispatchInfo.m_debugDraw);
//486  //gjkPairDetector.getClosestPoints(input,dummy,dispatchInfo.m_debugDraw);
//487 #endif //ZERO_MARGIN
//488  //btScalar l2 = gjkPairDetector.getCachedSeparatingAxis().length2();
//489  //if (l2>SIMD_EPSILON)
//490  {
//491  sepNormalWorldSpace = withoutMargin.m_reportedNormalOnWorld;//gjkPairDetector.getCachedSeparatingAxis()*(1.f/l2);
//492  //minDist = -1e30f;//gjkPairDetector.getCachedSeparatingDistance();
//493  minDist = withoutMargin.m_reportedDistance;//gjkPairDetector.getCachedSeparatingDistance()+min0->getMargin()+min1->getMargin();
//494 
//495 #ifdef ZERO_MARGIN
//496  foundSepAxis = true;//gjkPairDetector.getCachedSeparatingDistance()<0.f;
//497 #else
//498  foundSepAxis = withoutMargin.m_foundResult && minDist<0;//-(min0->getMargin()+min1->getMargin());
//499 #endif
//500  }
//501  }
//502  if (foundSepAxis)
//503  {
//504 
//505 // printf("sepNormalWorldSpace=%f,%f,%f\n",sepNormalWorldSpace.getX(),sepNormalWorldSpace.getY(),sepNormalWorldSpace.getZ());
//506 
//507  btPolyhedralContactClipping::clipHullAgainstHull(sepNormalWorldSpace, *polyhedronA->getConvexPolyhedron(), *polyhedronB->getConvexPolyhedron(),
//508  body0Wrap->getWorldTransform(),
//509  body1Wrap->getWorldTransform(), minDist-threshold, threshold, *resultOut);
//510 
//511  }
//512  if (m_ownManifold)
//513  {
//514  resultOut->refreshContactPoints();
//515  }
//516  return;
//517 
//518  } else
//519  {
//520  //we can also deal with convex versus triangle (without connectivity data)
//521  if (polyhedronA->getConvexPolyhedron() && polyhedronB->getShapeType()==TRIANGLE_SHAPE_PROXYTYPE)
//522  {
//523 
//524  btVertexArray vertices;
//525  btTriangleShape* tri = (btTriangleShape*)polyhedronB;
//526  vertices.push_back( body1Wrap->getWorldTransform()*tri->m_vertices1[0]);
//527  vertices.push_back( body1Wrap->getWorldTransform()*tri->m_vertices1[1]);
//528  vertices.push_back( body1Wrap->getWorldTransform()*tri->m_vertices1[2]);
//529 
//530  //tri->initializePolyhedralFeatures();
//531 
//532  btScalar threshold = m_manifoldPtr->getContactBreakingThreshold();
//533 
//534  btVector3 sepNormalWorldSpace;
//535  btScalar minDist =-1e30f;
//536  btScalar maxDist = threshold;
//537 
//538  bool foundSepAxis = false;
//539  if (0)
//540  {
//541  polyhedronB->initializePolyhedralFeatures();
//542  foundSepAxis = btPolyhedralContactClipping::findSeparatingAxis(
//543  *polyhedronA->getConvexPolyhedron(), *polyhedronB->getConvexPolyhedron(),
//544  body0Wrap->getWorldTransform(),
//545  body1Wrap->getWorldTransform(),
//546  sepNormalWorldSpace,*resultOut);
//547  // printf("sepNormalWorldSpace=%f,%f,%f\n",sepNormalWorldSpace.getX(),sepNormalWorldSpace.getY(),sepNormalWorldSpace.getZ());
//548 
//549  } else
//550  {
//551 #ifdef ZERO_MARGIN
//552  gjkPairDetector.setIgnoreMargin(true);
//553  gjkPairDetector.getClosestPoints(input,*resultOut,dispatchInfo.m_debugDraw);
//554 #else
//555  gjkPairDetector.getClosestPoints(input,dummy,dispatchInfo.m_debugDraw);
//556 #endif//ZERO_MARGIN
//557 
//558  btScalar l2 = gjkPairDetector.getCachedSeparatingAxis().length2();
//559  if (l2>SIMD_EPSILON)
//560  {
//561  sepNormalWorldSpace = gjkPairDetector.getCachedSeparatingAxis()*(1.f/l2);
//562  //minDist = gjkPairDetector.getCachedSeparatingDistance();
//563  //maxDist = threshold;
//564  minDist = gjkPairDetector.getCachedSeparatingDistance()-min0->getMargin()-min1->getMargin();
//565  foundSepAxis = true;
//566  }
//567  }
//568 
//569 
//570  if (foundSepAxis)
//571  {
//572  btPolyhedralContactClipping::clipFaceAgainstHull(sepNormalWorldSpace, *polyhedronA->getConvexPolyhedron(),
//573  body0Wrap->getWorldTransform(), vertices, minDist-threshold, maxDist, *resultOut);
//574  }
//575 
//576 
//577  if (m_ownManifold)
//578  {
//579  resultOut->refreshContactPoints();
//580  }
//581 
//582  return;
//583  }
//584 
//585  }
//586 
//587 
//588  }
//589 
//590  gjkPairDetector.getClosestPoints(input,*resultOut,dispatchInfo.m_debugDraw);
//591 
//592  //now perform 'm_numPerturbationIterations' collision queries with the perturbated collision objects
//593 
//594  //perform perturbation when more then 'm_minimumPointsPerturbationThreshold' points
//595  if (m_numPerturbationIterations && resultOut->getPersistentManifold()->getNumContacts() < m_minimumPointsPerturbationThreshold)
//596  {
//597 
//598  int i;
//599  btVector3 v0,v1;
//600  btVector3 sepNormalWorldSpace;
//601  btScalar l2 = gjkPairDetector.getCachedSeparatingAxis().length2();
//602 
//603  if (l2>SIMD_EPSILON)
//604  {
//605  sepNormalWorldSpace = gjkPairDetector.getCachedSeparatingAxis()*(1.f/l2);
//606 
//607  btPlaneSpace1(sepNormalWorldSpace,v0,v1);
//608 
//609 
//610  bool perturbeA = true;
//611  const btScalar angleLimit = 0.125f * SIMD_PI;
//612  btScalar perturbeAngle;
//613  btScalar radiusA = min0->getAngularMotionDisc();
//614  btScalar radiusB = min1->getAngularMotionDisc();
//615  if (radiusA < radiusB)
//616  {
//617  perturbeAngle = gContactBreakingThreshold /radiusA;
//618  perturbeA = true;
//619  } else
//620  {
//621  perturbeAngle = gContactBreakingThreshold / radiusB;
//622  perturbeA = false;
//623  }
//624  if ( perturbeAngle > angleLimit )
//625  perturbeAngle = angleLimit;
//626 
//627  btTransform unPerturbedTransform;
//628  if (perturbeA)
//629  {
//630  unPerturbedTransform = input.m_transformA;
//631  } else
//632  {
//633  unPerturbedTransform = input.m_transformB;
//634  }
//635 
//636  for ( i=0;i<m_numPerturbationIterations;i++)
//637  {
//638  if (v0.length2()>SIMD_EPSILON)
//639  {
//640  btQuaternion perturbeRot(v0,perturbeAngle);
//641  btScalar iterationAngle = i*(SIMD_2_PI/btScalar(m_numPerturbationIterations));
//642  btQuaternion rotq(sepNormalWorldSpace,iterationAngle);
//643 
//644 
//645  if (perturbeA)
//646  {
//647  input.m_transformA.setBasis( btMatrix3x3(rotq.inverse()*perturbeRot*rotq)*body0Wrap->getWorldTransform().getBasis());
//648  input.m_transformB = body1Wrap->getWorldTransform();
//649  #ifdef DEBUG_CONTACTS
//650  dispatchInfo.m_debugDraw->drawTransform(input.m_transformA,10.0);
//651  #endif //DEBUG_CONTACTS
//652  } else
//653  {
//654  input.m_transformA = body0Wrap->getWorldTransform();
//655  input.m_transformB.setBasis( btMatrix3x3(rotq.inverse()*perturbeRot*rotq)*body1Wrap->getWorldTransform().getBasis());
//656  #ifdef DEBUG_CONTACTS
//657  dispatchInfo.m_debugDraw->drawTransform(input.m_transformB,10.0);
//658  #endif
//659  }
//660 
//661  btPerturbedContactResult perturbedResultOut(resultOut,input.m_transformA,input.m_transformB,unPerturbedTransform,perturbeA,dispatchInfo.m_debugDraw);
//662  gjkPairDetector.getClosestPoints(input,perturbedResultOut,dispatchInfo.m_debugDraw);
//663  }
//664  }
//665  }
//666  }
//667 
//668 
//669 
//670 #ifdef USE_SEPDISTANCE_UTIL2
//671  if (dispatchInfo.m_useConvexConservativeDistanceUtil && (sepDist>SIMD_EPSILON))
//672  {
//673  m_sepDistance.initSeparatingDistance(gjkPairDetector.getCachedSeparatingAxis(),sepDist,body0->getWorldTransform(),body1->getWorldTransform());
//674  }
//675 #endif //USE_SEPDISTANCE_UTIL2
//676 
//677 
//678  }
//679 
//680  if (m_ownManifold)
//681  {
//682  resultOut->refreshContactPoints();
//683  }
//684 
//685 }
//686 
//687 
//688 
//689 bool disableCcd = false;
//690 btScalar btConvexConvexAlgorithm::calculateTimeOfImpact(btCollisionObject* col0,btCollisionObject* col1,const btDispatcherInfo& dispatchInfo,btManifoldResult* resultOut)
//691 {
//692  (void)resultOut;
//693  (void)dispatchInfo;
//695 
//698  btScalar resultFraction = btScalar(1.);
//699 
//700 
//701  btScalar squareMot0 = (col0->getInterpolationWorldTransform().getOrigin() - col0->getWorldTransform().getOrigin()).length2();
//702  btScalar squareMot1 = (col1->getInterpolationWorldTransform().getOrigin() - col1->getWorldTransform().getOrigin()).length2();
//703 
//704  if (squareMot0 < col0->getCcdSquareMotionThreshold() &&
//705  squareMot1 < col1->getCcdSquareMotionThreshold())
//706  return resultFraction;
//707 
//708  if (disableCcd)
//709  return btScalar(1.);
//710 
//711 
//712  //An adhoc way of testing the Continuous Collision Detection algorithms
//713  //One object is approximated as a sphere, to simplify things
//714  //Starting in penetration should report no time of impact
//715  //For proper CCD, better accuracy and handling of 'allowed' penetration should be added
//716  //also the mainloop of the physics should have a kind of toi queue (something like Brian Mirtich's application of Timewarp for Rigidbodies)
//717 
//718 
//720  {
//721  btConvexShape* convex0 = static_cast<btConvexShape*>(col0->getCollisionShape());
//722 
//723  btSphereShape sphere1(col1->getCcdSweptSphereRadius()); //todo: allow non-zero sphere sizes, for better approximation
//724  btConvexCast::CastResult result;
//725  btVoronoiSimplexSolver voronoiSimplex;
//726  //SubsimplexConvexCast ccd0(&sphere,min0,&voronoiSimplex);
//728  btGjkConvexCast ccd1( convex0 ,&sphere1,&voronoiSimplex);
//729  //ContinuousConvexCollision ccd(min0,min1,&voronoiSimplex,0);
//730  if (ccd1.calcTimeOfImpact(col0->getWorldTransform(),col0->getInterpolationWorldTransform(),
//731  col1->getWorldTransform(),col1->getInterpolationWorldTransform(),result))
//732  {
//733 
//734  //store result.m_fraction in both bodies
//735 
//736  if (col0->getHitFraction()> result.m_fraction)
//737  col0->setHitFraction( result.m_fraction );
//738 
//739  if (col1->getHitFraction() > result.m_fraction)
//740  col1->setHitFraction( result.m_fraction);
//741 
//742  if (resultFraction > result.m_fraction)
//743  resultFraction = result.m_fraction;
//744 
//745  }
//746 
//747 
//748 
//749 
//750  }
//751 
//753  {
//754  btConvexShape* convex1 = static_cast<btConvexShape*>(col1->getCollisionShape());
//755 
//756  btSphereShape sphere0(col0->getCcdSweptSphereRadius()); //todo: allow non-zero sphere sizes, for better approximation
//757  btConvexCast::CastResult result;
//758  btVoronoiSimplexSolver voronoiSimplex;
//759  //SubsimplexConvexCast ccd0(&sphere,min0,&voronoiSimplex);
//761  btGjkConvexCast ccd1(&sphere0,convex1,&voronoiSimplex);
//762  //ContinuousConvexCollision ccd(min0,min1,&voronoiSimplex,0);
//763  if (ccd1.calcTimeOfImpact(col0->getWorldTransform(),col0->getInterpolationWorldTransform(),
//764  col1->getWorldTransform(),col1->getInterpolationWorldTransform(),result))
//765  {
//766 
//767  //store result.m_fraction in both bodies
//768 
//769  if (col0->getHitFraction() > result.m_fraction)
//770  col0->setHitFraction( result.m_fraction);
//771 
//772  if (col1->getHitFraction() > result.m_fraction)
//773  col1->setHitFraction( result.m_fraction);
//774 
//775  if (resultFraction > result.m_fraction)
//776  resultFraction = result.m_fraction;
//777 
//778  }
//779  }
//780 
//781  return resultFraction;
//782 
//783 }
//
