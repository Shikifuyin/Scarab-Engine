/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/Edges/JointEdge.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Island edges : Joint
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// JointEdge implementation
inline IslandEdgeType JointEdge::GetType() const {
    return ISLAND_EDGE_JOINT;
}

inline BodyNode * JointEdge::GetBodyA() const {
    return (BodyNode*)( GetNodeA() );
}
inline BodyNode * JointEdge::GetBodyB() const {
    return (BodyNode*)( GetNodeB() );
}

inline JointConstraint * JointEdge::GetJoint() const {
    return m_pJointConstraint;
}

