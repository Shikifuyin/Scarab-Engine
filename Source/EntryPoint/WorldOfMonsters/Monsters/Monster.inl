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

inline Bool Monster::IsSummon() const {
    return m_bIsSummon;
}
inline const ScrollCost * Monster::GetSummoningCost() const {
    Assert( m_bIsSummon );
    return &m_hSummoningCost;
}

inline Bool Monster::IsFusion() const {
    return m_bIsFusion;
}
inline const MonsterCost * Monster::GetFusionCost() const {
    Assert( m_bIsFusion );
    return &m_hFusionCost;
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
inline Bool MonsterInstance::IsNull() const {
    return ( m_pMonster == NULL );
}
inline Bool MonsterInstance::IsPresent() const {
    return ( m_pMonster != NULL );
}

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

inline Bool MonsterInstance::IsSummon() const {
    return m_pMonster->IsSummon();
}
inline const ScrollCost * MonsterInstance::GetSummoningCost() const {
    return m_pMonster->GetSummoningCost();
}

inline Bool MonsterInstance::IsFusion() const {
    return m_pMonster->IsFusion();
}
inline const MonsterCost * MonsterInstance::GetFusionCost() const {
    return m_pMonster->GetFusionCost();
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
inline Void MonsterInstance::Awake() {
    m_bAwakened = true;
}
inline Void MonsterInstance::UnAwake() {
    m_bAwakened = false;
}

inline UInt MonsterInstance::GetNativeRank() const {
    return m_pMonster->GetNativeRank();
}
inline UInt MonsterInstance::GetRank() const {
    return m_iRank;
}
inline Bool MonsterInstance::IsMaxRank() const {
    return ( m_iRank == MONSTER_MAX_RANK - 1 );
}
inline Void MonsterInstance::RankUp() {
    if ( m_iRank < (MONSTER_MAX_RANK - 1) ) {
        ++m_iRank;
        m_iLevel = 0;
        m_iCurrentXP = 0;
    }
}
inline Void MonsterInstance::RankDown() {
    if ( m_iRank > 0 ) {
        --m_iRank;
        m_iLevel = 0;
        m_iCurrentXP = 0;
    }
}
inline Void MonsterInstance::SetRank( UInt iRank ) {
    Assert( iRank < MONSTER_MAX_RANK );
    m_iRank = iRank;
    m_iLevel = 0;
    m_iCurrentXP = 0;
}

inline UInt MonsterInstance::GetMaxLevel() const {
    return MONSTER_MAX_LEVELBYRANK(m_iRank);
}
inline UInt MonsterInstance::GetLevel() const {
    return m_iLevel;
}
inline Bool MonsterInstance::IsMaxLevel() const {
    return ( m_iLevel == MONSTER_MAX_LEVELBYRANK(m_iRank) - 1 );
}
inline Void MonsterInstance::LevelUp() {
    if ( m_iLevel < (MONSTER_MAX_LEVELBYRANK(m_iRank) - 1) ) {
        ++m_iLevel;
        m_iCurrentXP = 0;
    }
}
inline Void MonsterInstance::LevelDown() {
    if ( m_iLevel > 0 ) {
        --m_iLevel;
        m_iCurrentXP = 0;
    }
}
inline Void MonsterInstance::SetLevel( UInt iLevel ) {
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(m_iRank) );
    m_iLevel = iLevel;
    m_iCurrentXP = 0;
}

inline UInt MonsterInstance::GetCurrentXP() const {
    return m_iCurrentXP;
}

inline UInt MonsterInstance::GetBaseHP() const {
    return m_pMonster->GetLevelingStats()->GetHP( m_iRank, m_iLevel, m_bAwakened );
}
inline UInt MonsterInstance::GetBaseATT() const {
    return m_pMonster->GetLevelingStats()->GetATT( m_iRank, m_iLevel, m_bAwakened );
}
inline UInt MonsterInstance::GetBaseDEF() const {
    return m_pMonster->GetLevelingStats()->GetDEF( m_iRank, m_iLevel, m_bAwakened );
}
inline UInt MonsterInstance::GetBaseSPD() const {
    return m_pMonster->GetLevelingStats()->GetSPD( m_bAwakened );
}

inline Float MonsterInstance::GetBaseCritR() const {
    return m_pMonster->GetLevelingStats()->GetCritRate( m_bAwakened );
}
inline Float MonsterInstance::GetBaseCritD() const {
    return m_pMonster->GetLevelingStats()->GetCritDmg( m_bAwakened );
}
inline Float MonsterInstance::GetBaseACC() const {
    return m_pMonster->GetLevelingStats()->GetAccuracy( m_bAwakened );
}
inline Float MonsterInstance::GetBaseRES() const {
    return m_pMonster->GetLevelingStats()->GetResistance( m_bAwakened );
}

inline UInt MonsterInstance::GetSkillCount() const {
    return m_hSkillSet.GetSkillCount();
}
inline SkillInstance * MonsterInstance::GetSkillInstance( UInt iSlot ) {
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    return m_hSkillSet.GetSkillInstance( iSlot );
}
inline Void MonsterInstance::SkillLevelUp( UInt iSlot ) {
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    m_hSkillSet.GetSkillInstance( iSlot )->LevelUp();
}
inline Void MonsterInstance::SkillLevelDown( UInt iSlot ) {
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    m_hSkillSet.GetSkillInstance( iSlot )->LevelDown();
}
inline Void MonsterInstance::SetSkillLevel( UInt iSlot, UInt iLevel ) {
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    SkillInstance * pSkillInstance = m_hSkillSet.GetSkillInstance( iSlot );
    Assert( iLevel < pSkillInstance->GetMaxLevel() );
    pSkillInstance->SetLevel( iLevel );
}

inline Bool MonsterInstance::HasRune( UInt iSlot ) const {
    Assert( iSlot < RUNE_SLOT_COUNT );
    return m_hRuneSet.HasRune( iSlot );
}
inline const Rune * MonsterInstance::GetRune( UInt iSlot ) const {
    Assert( iSlot < RUNE_SLOT_COUNT );
    return m_hRuneSet.GetRune( iSlot );
}
inline Rune * MonsterInstance::GetRune( UInt iSlot ) {
    Assert( iSlot < RUNE_SLOT_COUNT );
    return m_hRuneSet.GetRune( iSlot );
}

inline Rune * MonsterInstance::EquipRune( const Rune & hRune ) {
    Assert( hRune.IsPresent() );
    return m_hRuneSet.AddRune( hRune );
}
inline Void MonsterInstance::UnEquipRune( UInt iSlot, Rune * outRune ) {
    Assert( iSlot < RUNE_SLOT_COUNT );
    m_hRuneSet.RemoveRune( iSlot, outRune );
    Assert( outRune->IsPresent() );
}

inline Bool MonsterInstance::HasSetBonus( RuneType iType, UInt * outCount ) const {
    Assert( iType < RUNE_TYPE_COUNT );
    return m_hRuneSet.HasSetBonus( iType, outCount );
}

inline UInt MonsterInstance::GetEffectiveHP() const {
    UInt iFlatBonus; Float fRatioBonus;
    m_hRuneSet.ComputeBonusStat( MONSTER_STAT_HEALTH, &iFlatBonus, &fRatioBonus );
    UInt iBaseHP = m_pMonster->GetLevelingStats()->GetHP( m_iRank, m_iLevel, m_bAwakened );
    return ( iBaseHP + (UInt)(fRatioBonus * (Float)iBaseHP) + iFlatBonus );
}
inline UInt MonsterInstance::GetEffectiveATT() const {
    UInt iFlatBonus; Float fRatioBonus;
    m_hRuneSet.ComputeBonusStat( MONSTER_STAT_ATTACK, &iFlatBonus, &fRatioBonus );
    UInt iBaseATT = m_pMonster->GetLevelingStats()->GetATT( m_iRank, m_iLevel, m_bAwakened );
    return ( iBaseATT + (UInt)(fRatioBonus * (Float)iBaseATT) + iFlatBonus );
}
inline UInt MonsterInstance::GetEffectiveDEF() const {
    UInt iFlatBonus; Float fRatioBonus;
    m_hRuneSet.ComputeBonusStat( MONSTER_STAT_DEFENSE, &iFlatBonus, &fRatioBonus );
    UInt iBaseDEF = m_pMonster->GetLevelingStats()->GetDEF( m_iRank, m_iLevel, m_bAwakened );
    return ( iBaseDEF + (UInt)(fRatioBonus * (Float)iBaseDEF) + iFlatBonus );
}
inline UInt MonsterInstance::GetEffectiveSPD() const {
    UInt iFlatBonus; Float fRatioBonus;
    m_hRuneSet.ComputeBonusStat( MONSTER_STAT_SPEED, &iFlatBonus, &fRatioBonus );
    UInt iBaseSPD = m_pMonster->GetLevelingStats()->GetSPD( m_bAwakened );
    return ( iBaseSPD + (UInt)(fRatioBonus * (Float)iBaseSPD) + iFlatBonus );
}

inline Float MonsterInstance::GetEffectiveCritR() const {
    UInt iFlatBonus; Float fRatioBonus;
    m_hRuneSet.ComputeBonusStat( MONSTER_STAT_CRIT_RATE, &iFlatBonus, &fRatioBonus );
    Float fBaseCritR = m_pMonster->GetLevelingStats()->GetCritRate( m_bAwakened );
    return Min<Float>( fBaseCritR + fRatioBonus, 1.0f );
}
inline Float MonsterInstance::GetEffectiveCritD() const {
    UInt iFlatBonus; Float fRatioBonus;
    m_hRuneSet.ComputeBonusStat( MONSTER_STAT_CRIT_DMG, &iFlatBonus, &fRatioBonus );
    Float fBaseCritD = m_pMonster->GetLevelingStats()->GetCritDmg( m_bAwakened );
    return ( fBaseCritD + fRatioBonus );
}
inline Float MonsterInstance::GetEffectiveACC() const {
    UInt iFlatBonus; Float fRatioBonus;
    m_hRuneSet.ComputeBonusStat( MONSTER_STAT_ACCURACY, &iFlatBonus, &fRatioBonus );
    Float fBaseACC = m_pMonster->GetLevelingStats()->GetAccuracy( m_bAwakened );
    return Min<Float>( fBaseACC + fRatioBonus, 1.0f );
}
inline Float MonsterInstance::GetEffectiveRES() const {
    UInt iFlatBonus; Float fRatioBonus;
    m_hRuneSet.ComputeBonusStat( MONSTER_STAT_RESISTANCE, &iFlatBonus, &fRatioBonus );
    Float fBaseRES = m_pMonster->GetLevelingStats()->GetResistance( m_bAwakened );
    return Min<Float>( fBaseRES + fRatioBonus, 1.0f );
}


