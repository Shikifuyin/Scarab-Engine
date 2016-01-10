/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Statistics.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Statistics interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MonsterLevelingStats implementation
inline MonsterType MonsterLevelingStats::GetType() const {
    return m_iType;
}
inline MonsterElement MonsterLevelingStats::GetElement() const {
    return m_iElement;
}

inline UInt MonsterLevelingStats::GetNativeRank() const {
    return m_iNativeRank;
}

inline Bool MonsterLevelingStats::IsSummoningCostHighTier( MonsterSummoningCost iCost ) const {
    Assert( iCost < MONSTER_SUMMONING_COST_COUNT );
    return ( (m_arrSummoningCosts[iCost] & 0x80000000) != 0 );
}
inline UInt MonsterLevelingStats::GetSummoningCostAmount( MonsterSummoningCost iCost ) const {
    Assert( iCost < MONSTER_SUMMONING_COST_COUNT );
    return ( m_arrSummoningCosts[iCost] & 0x7fffffff );
}

inline MonsterAwakeningBonus MonsterLevelingStats::GetAwakeningBonus() const {
    return m_iAwakeningBonus;
}
inline Bool MonsterLevelingStats::IsAwakeningCostHighTier( MonsterAwakeningCost iCost ) const {
    Assert( iCost < MONSTER_AWAKENING_COST_COUNT );
    return ( (m_arrAwakeningCosts[iCost] & 0x80000000) != 0 );
}
inline UInt MonsterLevelingStats::GetAwakeningCostAmount( MonsterAwakeningCost iCost ) const {
    Assert( iCost < MONSTER_AWAKENING_COST_COUNT );
    return ( m_arrAwakeningCosts[iCost] & 0x7fffffff );
}

inline UInt MonsterLevelingStats::GetHP( UInt iRank, UInt iLevel, Bool bAwakened ) const {
    Assert( iRank < MONSTER_MAX_RANK );
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return m_arrHealth[bAwakened ? 1 : 0][iRank][iLevel];
}
inline UInt MonsterLevelingStats::GetATT( UInt iRank, UInt iLevel, Bool bAwakened ) const {
    Assert( iRank < MONSTER_MAX_RANK);
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return m_arrAttack[bAwakened ? 1 : 0][iRank][iLevel];
}
inline UInt MonsterLevelingStats::GetDEF( UInt iRank, UInt iLevel, Bool bAwakened ) const {
    Assert( iRank < MONSTER_MAX_RANK);
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return m_arrDefense[bAwakened ? 1 : 0][iRank][iLevel];
}
inline UInt MonsterLevelingStats::GetSPD( Bool bAwakened ) const {
    if ( bAwakened ) {
        if ( m_iAwakeningBonus == MONSTER_AWAKENING_SPEED )
            return ( m_iBaseSpeed + MONSTER_AWAKENING_BONUS_SPEED + 15 );
        return ( m_iBaseSpeed + MONSTER_AWAKENING_BONUS_SPEED );
    }
    return m_iBaseSpeed;
}

inline Float MonsterLevelingStats::GetCritRate( Bool bAwakened ) const {
    if ( bAwakened && m_iAwakeningBonus == MONSTER_AWAKENING_CRITICALRATE )
        return MONSTER_BASE_CRITICAL_RATE + 0.15f;
    return MONSTER_BASE_CRITICAL_RATE;
}
inline Float MonsterLevelingStats::GetCritDmg( Bool /*bAwakened*/ ) const {
    return MONSTER_BASE_CRITICAL_DAMAGE;
}
inline Float MonsterLevelingStats::GetAccuracy( Bool bAwakened ) const {
    if ( bAwakened && m_iAwakeningBonus == MONSTER_AWAKENING_ACCURACY )
        return MONSTER_BASE_ACCURACY + 0.25f;
    return MONSTER_BASE_ACCURACY;
}
inline Float MonsterLevelingStats::GetResistance( Bool bAwakened ) const {
    if ( bAwakened && m_iAwakeningBonus == MONSTER_AWAKENING_RESISTANCE )
        return MONSTER_BASE_RESISTANCE + 0.25f;
    return MONSTER_BASE_RESISTANCE;
}

/////////////////////////////////////////////////////////////////////////////////
// MonsterStats implementation
inline MonsterType MonsterStats::GetType() const {
    return m_pLevelingStats->GetType();
}
inline MonsterElement MonsterStats::GetElement() const {
    return m_pLevelingStats->GetElement();
}

inline Bool MonsterStats::IsSummoningCostHighTier( MonsterSummoningCost iCost ) const {
    return m_pLevelingStats->IsSummoningCostHighTier( iCost );
}
inline UInt MonsterStats::GetSummoningCostAmount( MonsterSummoningCost iCost ) const {
    return m_pLevelingStats->GetSummoningCostAmount( iCost );
}

inline Bool MonsterStats::IsAwakened() const {
    return m_bAwakened;
}

inline MonsterAwakeningBonus MonsterStats::GetAwakeningBonus() const {
    return m_pLevelingStats->GetAwakeningBonus();
}
inline Bool MonsterStats::IsAwakeningCostHighTier( MonsterAwakeningCost iCost ) const {
    return m_pLevelingStats->IsAwakeningCostHighTier( iCost );
}
inline UInt MonsterStats::GetAwakeningCostAmount( MonsterAwakeningCost iCost ) const {
    return m_pLevelingStats->GetAwakeningCostAmount( iCost );
}

inline UInt MonsterStats::GetNativeRank() const {
    return m_pLevelingStats->GetNativeRank();
}
inline UInt MonsterStats::GetRank() const {
    return m_iRank;
}

inline UInt MonsterStats::GetMaxLevel() const {
    return MONSTER_MAX_LEVELBYRANK( m_iRank );
}
inline UInt MonsterStats::GetLevel() const {
    return m_iLevel;
}

inline UInt MonsterStats::GetHP() const {
    return m_iHealth;
}
inline UInt MonsterStats::GetATT() const {
    return m_iAttack;
}
inline UInt MonsterStats::GetDEF() const {
    return m_iDefense;
}
inline UInt MonsterStats::GetSPD() const {
    return m_iSpeed;
}

inline Float MonsterStats::GetCritR() const {
    return m_fCriticalRate;
}
inline Float MonsterStats::GetCritD() const {
    return m_fCriticalDamage;
}
inline Float MonsterStats::GetACC() const {
    return m_fAccuracy;
}
inline Float MonsterStats::GetRES() const {
    return m_fResistance;
}
