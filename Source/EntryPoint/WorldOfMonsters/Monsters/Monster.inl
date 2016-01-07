/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Monster.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Base monster interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Monster implementation
inline MonsterID Monster::GetID() const {
    return m_iMonsterID;
}
inline const GChar * Monster::GetName() const {
    return m_strName;
}
inline const GChar * Monster::GetAwakenedName() const {
    return m_strAwakenedName;
}

inline const MonsterLevelingStats * Monster::GetLevelingStats() const {
    return &m_hLevelingStats;
}

inline UInt Monster::GetSkillCount() const {
    return m_iSkillCount;
}
inline SkillID Monster::GetSkill( UInt iSlot ) const {
    Assert( iSlot < m_iSkillCount );
    return m_arrSkills[iSlot];
}

/////////////////////////////////////////////////////////////////////////////////
// MonsterInstance implementation
inline MonsterID MonsterInstance::GetID() const {
    return m_pMonster->GetID();
}
inline const GChar * MonsterInstance::GetName() const {
    return m_pMonster->GetName();
}
inline const GChar * MonsterInstance::GetAwakenedName() const {
    return m_pMonster->GetAwakenedName();
}

inline MonsterType MonsterInstance::GetType() const {
    return m_hStats.GetType();
}
inline MonsterElement MonsterInstance::GetElement() const {
    return m_hStats.GetElement();
}

inline Bool MonsterInstance::IsAwakened() const {
    return m_hStats.IsAwakened();
}

inline MonsterAwakeningBonus MonsterInstance::GetAwakeningBonus() const {
    return m_hStats.GetAwakeningBonus();
}
inline Bool MonsterInstance::IsAwakeningCostHighTier( MonsterAwakeningCost iCost ) const {
    return m_hStats.IsAwakeningCostHighTier( iCost );
}
inline UInt MonsterInstance::GetAwakeningCostAmount( MonsterAwakeningCost iCost ) const {
    return m_hStats.GetAwakeningCostAmount( iCost );
}

inline UInt MonsterInstance::GetNativeRank() const {
    return m_hStats.GetNativeRank();
}
inline UInt MonsterInstance::GetRank() const {
    return m_hStats.GetRank();
}

inline UInt MonsterInstance::GetMaxLevel() const {
    return m_hStats.GetMaxLevel();
}
inline UInt MonsterInstance::GetLevel() const {
    return m_hStats.GetLevel();
}

inline UInt MonsterInstance::GetHP() const {
    return m_hStats.GetHP();
}
inline UInt MonsterInstance::GetATT() const {
    return m_hStats.GetATT();
}
inline UInt MonsterInstance::GetDEF() const {
    return m_hStats.GetDEF();
}
inline UInt MonsterInstance::GetSPD() const {
    return m_hStats.GetSPD();
}

inline Float MonsterInstance::GetCritR() const {
    return m_hStats.GetCritR();
}
inline Float MonsterInstance::GetCritD() const {
    return m_hStats.GetCritD();
}
inline Float MonsterInstance::GetACC() const {
    return m_hStats.GetACC();
}
inline Float MonsterInstance::GetRES() const {
    return m_hStats.GetRES();
}

inline UInt MonsterInstance::GetSkillCount() const {
    return m_hSkillSet.GetSkillCount();
}
inline SkillInstance * MonsterInstance::GetSkillInstance( UInt iSlot ) {
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    return m_hSkillSet.GetSkillInstance( iSlot );
}

inline Bool MonsterInstance::HasRune( UInt iSlot ) const {
    return m_hRuneSet.HasRune( iSlot );
}

inline Rune * MonsterInstance::GetRune( UInt iSlot ) const {
    return m_hRuneSet.GetRune( iSlot );
}
inline Void MonsterInstance::SetRune( UInt iSlot, Rune * pRune ) {
    m_hRuneSet.SetRune( iSlot, pRune );
    _UpdateEffectiveStats();
}

inline Bool MonsterInstance::HasSetBonus( RuneType iType, UInt * outCount ) const {
    return m_hRuneSet.HasSetBonus( iType, outCount );
}

inline UInt MonsterInstance::GetSetBonusCount() const {
    return m_hRuneSet.GetSetBonusCount();
}
inline RuneType MonsterInstance::GetSetBonus( UInt iIndex ) const {
    return m_hRuneSet.GetSetBonus( iIndex );
}

inline UInt MonsterInstance::GetEffectiveHP() const {
    return m_iHealth;
}
inline UInt MonsterInstance::GetEffectiveATT() const {
    return m_iAttack;
}
inline UInt MonsterInstance::GetEffectiveDEF() const {
    return m_iDefense;
}
inline UInt MonsterInstance::GetEffectiveSPD() const {
    return m_iSpeed;
}

inline Float MonsterInstance::GetEffectiveCritR() const {
    return m_fCriticalRate;
}
inline Float MonsterInstance::GetEffectiveCritD() const {
    return m_fCriticalDamage;
}
inline Float MonsterInstance::GetEffectiveACC() const {
    return m_fAccuracy;
}
inline Float MonsterInstance::GetEffectiveRES() const {
    return m_fResistance;
}

