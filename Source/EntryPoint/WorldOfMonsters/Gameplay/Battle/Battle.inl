/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Gameplay/Battle/Battle.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Battle : Battle interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BattleTeam implementation
inline UInt BattleTeam::GetTeamSize() const {
    return m_iTeamSize;
}
inline const BattleMonsterInstance * BattleTeam::GetTeamMember( UInt iIndex ) const {
    Assert( iIndex < m_iTeamSize );
    return ( m_arrMonsters + iIndex );
}
inline BattleMonsterInstance * BattleTeam::GetTeamMember( UInt iIndex ) {
    Assert( iIndex < m_iTeamSize );
    return ( m_arrMonsters + iIndex );
}

/////////////////////////////////////////////////////////////////////////////////
// Battle implementation
inline BattleType Battle::GetType() const {
    return m_iType;
}

inline const BattleTeam * Battle::GetPlayerTeam() const {
    return m_pPlayerTeam;
}
inline const BattleTeam * Battle::GetAITeam() const {
    return m_pAITeam;
}

inline Bool Battle::IsTurnPending() const {
    return m_bTurnPending;
}
inline Bool Battle::IsTurnInProgress() const {
    return m_bTurnInProgress;
}
inline Bool Battle::IsPlayerTurn() const {
    return ( m_bTurnPending && m_bPlayerTurn );
}
inline Bool Battle::IsAITurn() const {
    return ( m_bTurnPending && !m_bPlayerTurn );
}

inline const BattleMonsterInstance * Battle::GetTurnMonster() const {
    return m_pTurnMonster;
}

/////////////////////////////////////////////////////////////////////////////////

inline Bool Battle::_IsElementWeakAgainst( MonsterElement iElement, MonsterElement iOpposingElement ) {
    return ( sm_arrElementWeakAgainst[iOpposingElement] == iElement );
}
inline Bool Battle::_IsElementStrongAgainst( MonsterElement iElement, MonsterElement iOpposingElement ) {
    return ( sm_arrElementStrongAgainst[iOpposingElement] == iElement );
}

inline MonsterElement Battle::_GetElementWeakAgainst( MonsterElement iElement ) {
    return sm_arrElementWeakAgainst[iElement];
}
inline MonsterElement Battle::_GetElementStrongAgainst( MonsterElement iElement ) {
    return sm_arrElementStrongAgainst[iElement];
}
