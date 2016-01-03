/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/GameplayManager.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters : Global gameplay interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GameplayManager implementation
inline const GChar * GameplayManager::GetName() {
    return TEXT("GameplayManager");
}
inline const GChar * GameplayManager::GetMemoryBudget() {
    return TEXT (
        "Heap:Scratch(4194304);"
        );
}

inline UInt GameplayManager::GetRandomUInt( UInt iRange ) const {
    return (UInt)( RandFn->XorShift128() );
}
inline Int GameplayManager::GetRandomInt( Int iRange ) const {
    return (Int)( RandFn->XorShift128() );
}
inline Float GameplayManager::GetRandomFloat() const {
    static const Float s_fInvMax = ( 1.0f / (Float)(RandFn->XorShiftMax()) );
    return ( s_fInvMax * (Float)(RandFn->XorShift128()) );
}
inline Bool GameplayManager::CheckRandomEvent( Float fSuccessRate ) const {
    return ( GetRandomFloat() <= fSuccessRate );
}

inline Skill * GameplayManager::GetSkill( SkillID iSkillID ) const {
    SkillMap::Iterator it = m_mapSkills.Get( iSkillID );
    if ( it.IsNull() )
        return NULL;
    return it.GetItem();
}

inline Monster * GameplayManager::GetMonster( MonsterID iMonsterID ) const {
    MonsterMap::Iterator it = m_mapMonsters.Get( iMonsterID );
    if ( it.IsNull() )
        return NULL;
    return it.GetItem();
}

/////////////////////////////////////////////////////////////////////////////////

inline Int GameplayManager::_Compare_SkillIDs( const SkillID & rLeft, const SkillID & rRight, Void * pUserData ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

inline Int GameplayManager::_Compare_MonsterIDs( const MonsterID & rLeft, const SkillID & rRight, Void * pUserData ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

