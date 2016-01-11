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

inline MonsterType Monster::GetType() const {
    return m_iType;
}
inline MonsterElement Monster::GetElement() const {
    return m_iElement;
}

inline UInt Monster::GetNativeRank() const {
    return m_iNativeRank;
}

inline const ScrollCost * Monster::GetSummoningCost() const {
    return &m_hSummoningCost;
}

inline const EssenceCost * Monster::GetAwakeningCost() const {
    return &m_hAwakeningCost;
}

inline const MonsterLevelingStats * Monster::GetLevelingStats() const {
    return &m_hLevelingStats;
}

inline UInt Monster::GetSkillCount( Bool bAwaken ) const {
    return ( bAwaken ? m_iSkillCountAwaken : m_iSkillCount );
}
inline SkillID Monster::GetSkill( Bool bAwaken, UInt iSlot ) const {
    Assert( iSlot < (bAwaken ? m_iSkillCountAwaken : m_iSkillCount) );
    return ( bAwaken ? m_arrSkillSetAwaken[iSlot] : m_arrSkillSet[iSlot] );
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
    return m_pMonster->GetType();
}
inline MonsterElement MonsterInstance::GetElement() const {
    return m_pMonster->GetElement();
}

inline const ScrollCost * MonsterInstance::GetSummoningCost() const {
    return m_pMonster->GetSummoningCost();
}

inline const EssenceCost * MonsterInstance::GetAwakeningCost() const {
    return m_pMonster->GetAwakeningCost();
}
inline MonsterAwakeningBonus MonsterInstance::GetAwakeningBonus() const {
    return m_pMonster->GetLevelingStats()->GetAwakeningBonus();
}
inline Bool MonsterInstance::IsAwakened() const {
    return m_bAwakened;
}

inline UInt MonsterInstance::GetNativeRank() const {
    return m_pMonster->GetNativeRank();
}
inline UInt MonsterInstance::GetRank() const {
    return m_iRank;
}

inline UInt MonsterInstance::GetMaxLevel() const {
    return MONSTER_MAX_LEVELBYRANK(m_iRank);
}
inline UInt MonsterInstance::GetLevel() const {
    return m_iLevel;
}

inline Bool MonsterInstance::CanReceiveXP() const {
    return ( m_iLevel < (MONSTER_MAX_LEVELBYRANK(m_iRank) - 1) );
}

inline UInt MonsterInstance::GetBaseHP() const {
    return m_iBaseHealth;
}
inline UInt MonsterInstance::GetBaseATT() const {
    return m_iBaseAttack;
}
inline UInt MonsterInstance::GetBaseDEF() const {
    return m_iBaseDefense;
}
inline UInt MonsterInstance::GetBaseSPD() const {
    return m_iBaseSpeed;
}

inline Float MonsterInstance::GetBaseCritR() const {
    return m_fBaseCriticalRate;
}
inline Float MonsterInstance::GetBaseCritD() const {
    return m_fBaseCriticalDamage;
}
inline Float MonsterInstance::GetBaseACC() const {
    return m_fBaseAccuracy;
}
inline Float MonsterInstance::GetBaseRES() const {
    return m_fBaseResistance;
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
inline Rune * MonsterInstance::GetRune( UInt iSlot ) {
    return m_hRuneSet.GetRune( iSlot );
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
    return m_iEffectiveHealth;
}
inline UInt MonsterInstance::GetEffectiveATT() const {
    return m_iEffectiveAttack;
}
inline UInt MonsterInstance::GetEffectiveDEF() const {
    return m_iEffectiveDefense;
}
inline UInt MonsterInstance::GetEffectiveSPD() const {
    return m_iEffectiveSpeed;
}

inline Float MonsterInstance::GetEffectiveCritR() const {
    return m_fEffectiveCriticalRate;
}
inline Float MonsterInstance::GetEffectiveCritD() const {
    return m_fEffectiveCriticalDamage;
}
inline Float MonsterInstance::GetEffectiveACC() const {
    return m_fEffectiveAccuracy;
}
inline Float MonsterInstance::GetEffectiveRES() const {
    return m_fEffectiveResistance;
}

