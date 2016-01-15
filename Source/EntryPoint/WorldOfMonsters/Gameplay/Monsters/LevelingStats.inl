/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Gameplay/Monsters/LevelingStats.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : LevelingStats interface
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
inline UInt MonsterLevelingStats::GetHP( UInt iRank, UInt iLevel, Bool bAwakened ) const {
    Assert( iRank < MONSTER_MAX_RANK );
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return bAwakened ? m_arrHealthAwaken[iRank][iLevel] : m_arrHealth[iRank][iLevel];
}
inline UInt MonsterLevelingStats::GetATT( UInt iRank, UInt iLevel, Bool bAwakened ) const {
    Assert( iRank < MONSTER_MAX_RANK);
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return bAwakened ? m_arrAttackAwaken[iRank][iLevel] : m_arrAttack[iRank][iLevel];
}
inline UInt MonsterLevelingStats::GetDEF( UInt iRank, UInt iLevel, Bool bAwakened ) const {
    Assert( iRank < MONSTER_MAX_RANK);
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return bAwakened ? m_arrDefenseAwaken[iRank][iLevel] : m_arrDefense[iRank][iLevel];
}

inline MonsterAwakeningBonus MonsterLevelingStats::GetAwakeningBonus() const {
    return m_iAwakeningBonus;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillLevelingStats implementation
inline UInt SkillLevelingStats::GetMaxLevel() const {
    return m_iMaxLevel;
}

inline Float SkillLevelingStats::GetBonusDamage( UInt iLevel ) const {
    Assert( iLevel < m_iMaxLevel );
    return m_arrBonusDamage[iLevel];
}
inline Float SkillLevelingStats::GetBonusRecovery( UInt iLevel ) const {
    Assert( iLevel < m_iMaxLevel );
    return m_arrBonusRecovery[iLevel];
}
inline Float SkillLevelingStats::GetBonusStatusEffectRate( UInt iLevel ) const {
    Assert( iLevel < m_iMaxLevel );
    return m_arrBonusStatusEffectRate[iLevel];
}
inline Float SkillLevelingStats::GetBonusSpecific( UInt iLevel ) const {
    Assert( iLevel < m_iMaxLevel );
    return m_arrBonusSpecific[iLevel];
}

inline UInt SkillLevelingStats::GetCooldown( UInt iLevel ) const {
    Assert( iLevel < m_iMaxLevel );
    return m_arrCooldown[iLevel];
}

/////////////////////////////////////////////////////////////////////////////////
// RuneLevelingStats implementation
inline UInt RuneLevelingStats::GetPrimaryStatI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    Assert( iStat < MONSTER_STAT_COUNT );
    Assert( iRank < MONSTER_MAX_RANK );
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return m_arrPrimaryStatsI[iStat][iRank][iLevel];
}
inline Float RuneLevelingStats::GetPrimaryStatF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    Assert( iStat < MONSTER_STAT_COUNT );
    Assert( iRank < MONSTER_MAX_RANK );
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return m_arrPrimaryStatsF[iStat][iRank][iLevel];
}

inline UInt RuneLevelingStats::GetSecondaryStatI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    Assert( iStat < MONSTER_STAT_COUNT );
    Assert( iRank < MONSTER_MAX_RANK );
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return m_arrSecondaryStatsI[iStat][iRank][iLevel];
}
inline Float RuneLevelingStats::GetSecondaryStatF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    Assert( iStat < MONSTER_STAT_COUNT );
    Assert( iRank < MONSTER_MAX_RANK );
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return m_arrSecondaryStatsF[iStat][iRank][iLevel];
}

inline UInt RuneLevelingStats::GetSubStatI( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    Assert( iStat < MONSTER_STAT_COUNT );
    Assert( iRank < MONSTER_MAX_RANK );
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return m_arrSubStatsI[iStat][iRank][iLevel];
}
inline Float RuneLevelingStats::GetSubStatF( MonsterStatistic iStat, UInt iRank, UInt iLevel ) const {
    Assert( iStat < MONSTER_STAT_COUNT );
    Assert( iRank < MONSTER_MAX_RANK );
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return m_arrSubStatsF[iStat][iRank][iLevel];
}

