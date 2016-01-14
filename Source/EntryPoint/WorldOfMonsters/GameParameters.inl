/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/GameParameters.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters : Game parameters interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GameParameters implementation
inline Float GameParameters::GetMonsterBaseCritRate() const {
    return m_fMonsterBaseCritRate;
}
inline Float GameParameters::GetMonsterBaseCritDamage() const {
    return m_fMonsterBaseCritDamage;
}
inline Float GameParameters::GetMonsterBaseAccuracy() const {
    return m_fMonsterBaseAccuracy;
}
inline Float GameParameters::GetMonsterBaseResistance() const {
    return m_fMonsterBaseResistance;
}

inline UInt GameParameters::GetAwakeningBonusBaseSpeed() const {
    return m_iAwakeningBonusBaseSpeed;
}
inline UInt GameParameters::GetAwakeningBonusSpeed() const {
    return m_iAwakeningBonusSpeed;
}
inline Float GameParameters::GetAwakeningBonusCritRate() const {
    return m_fAwakeningBonusCritRate;
}
inline Float GameParameters::GetAwakeningBonusAccuracy() const {
    return m_fAwakeningBonusAccuracy;
}
inline Float GameParameters::GetAwakeningBonusResistance() const {
    return m_fAwakeningBonusResistance;
}

inline UInt GameParameters::GetMonsterXPByLevel( UInt iRank, UInt iLevel ) const {
    Assert( iRank < MONSTER_MAX_RANK );
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(iRank) );
    return m_arrMonsterXPByLevel[iRank][iLevel];
}
inline UInt GameParameters::GetMonsterXPFromPowerUp( UInt iFoodRank, UInt iFoodLevel ) const {
    Assert( iFoodRank < MONSTER_MAX_RANK );
    Assert( iFoodLevel < MONSTER_MAX_LEVELBYRANK(iFoodRank) );
    return m_arrMonsterXPFromPowerUp[iFoodRank][iFoodLevel];
}

inline Bool GameParameters::IsRuneSlotAllowed( UInt iSlot, MonsterStatistic iStat, Bool bRatio ) const {
    Assert( iSlot < RUNE_SLOT_COUNT );
    Assert( iStat < MONSTER_STAT_COUNT );
    UInt iMask = ( 1 << iSlot );
    return ( (m_arrRuneSlotsAllowed[bRatio ? 1 : 0][iStat] & iMask) != 0 );
}

inline UInt GameParameters::GetRuneSetRequiredCount( RuneType iType ) const {
    Assert( iType < RUNE_TYPE_COUNT );
    return m_arrRuneSetRequiredCount[iType];
}
inline Float GameParameters::GetRuneSetStatBonus( RuneType iType ) const {
    Assert( iType < RUNE_TYPE_COUNT );
    return m_arrRuneSetStatBonus[iType];
}

inline const RuneLevelingStats * GameParameters::GetRuneLevelingStats() const {
    return &m_hRuneLevelingStats;
}

inline const CurrencyCost * GameParameters::GetBuildingCost( BuildingType iType ) const {
    Assert( iType < BUILDING_COUNT );
    return ( m_arrBuildingCost + iType );
}

inline const CurrencyCost * GameParameters::GetEssenceFusionUnlockCost() const {
    return &m_hEssenceFusionUnlockCost;
}
inline const CurrencyCost * GameParameters::GetMonsterSummoningUnlockCost() const {
    return &m_hMonsterSummoningUnlockCost;
}
inline const CurrencyCost * GameParameters::GetMonsterFusionUnlockCost() const {
    return &m_hMonsterFusionUnlockCost;
}
inline const CurrencyCost * GameParameters::GetMonsterPowerUpUnlockCost() const {
    return &m_hMonsterPowerUpUnlockCost;
}
inline const CurrencyCost * GameParameters::GetMonsterEvolutionUnlockCost() const {
    return &m_hMonsterEvolutionUnlockCost;
}
inline const CurrencyCost * GameParameters::GetMonsterAwakeningUnlockCost() const {
    return &m_hMonsterAwakeningUnlockCost;
}
inline const CurrencyCost * GameParameters::GetRunePowerUpUnlockCost() const {
    return &m_hRunePowerUpUnlockCost;
}
inline const CurrencyCost * GameParameters::GetRuneEvolutionUnlockCost() const {
    return &m_hRuneEvolutionUnlockCost;
}

inline UInt GameParameters::GetEssenceFusionCost( EssenceType iType ) const {
    Assert( iType < ESSENCE_HIGH );
    return m_arrEssenceFusionCost[iType];
}
inline const CurrencyCost * GameParameters::GetMonsterFusionCost( UInt iRank ) const {
    Assert( iRank < MONSTER_MAX_RANK );
    return ( m_arrMonsterFusionCost + iRank );
}
inline const CurrencyCost * GameParameters::GetMonsterPowerUpCost( UInt iTargetRank, UInt iFoodRank ) const {
    Assert( iTargetRank < MONSTER_MAX_RANK );
    Assert( iFoodRank < MONSTER_MAX_RANK );
    return &( m_arrMonsterPowerUpCost[iTargetRank][iFoodRank] );
}
inline const CurrencyCost * GameParameters::GetMonsterEvolutionCost( UInt iRank ) const {
    Assert( iRank < MONSTER_MAX_RANK );
    return ( m_arrMonsterEvolutionCost + iRank );
}
inline const CurrencyCost * GameParameters::GetRunePowerUpCost( UInt iTargetRank, UInt iFoodRank ) const {
    Assert( iTargetRank < RUNE_MAX_RANK );
    Assert( iFoodRank < RUNE_MAX_RANK );
    return &( m_arrRunePowerUpCosts[iTargetRank][iFoodRank] );
}
inline const CurrencyCost * GameParameters::GetRuneEvolutionCost( UInt iRank ) const {
    Assert( iRank < RUNE_MAX_RANK );
    return ( m_arrRuneEvolutionCosts + iRank );
}

inline const CurrencyCost * GameParameters::GetMonsterCollectionUpgradeCost( UInt iLevel ) const {
    Assert( iLevel < MONSTER_COLLECTION_MAX_LEVEL );
    return ( m_arrMonsterCollectionUpgradeCost + iLevel );
}
inline const CurrencyCost * GameParameters::GetMonsterStorageUpgradeCost( UInt iLevel ) const {
    Assert( iLevel < MONSTER_STORAGE_MAX_LEVEL );
    return ( m_arrMonsterStorageUpgradeCost + iLevel );
}
inline UInt GameParameters::GetMonsterCollectionRoom( UInt iLevel ) const {
    Assert( iLevel < MONSTER_COLLECTION_MAX_LEVEL );
    return m_arrMonsterCollectionRoom[iLevel];
}
inline UInt GameParameters::GetMonsterStorageRoom( UInt iLevel ) const {
    Assert( iLevel < MONSTER_STORAGE_MAX_LEVEL );
    return m_arrMonsterStorageRoom[iLevel];
}

inline const CurrencyCost * GameParameters::GetRuneCollectionUpgradeCost( UInt iLevel ) const {
    Assert( iLevel < RUNE_COLLECTION_MAX_LEVEL );
    return ( m_arrRuneCollectionUpgradeCost + iLevel );
}
inline const CurrencyCost * GameParameters::GetRuneStorageUpgradeCost( UInt iLevel ) const {
    Assert( iLevel < RUNE_STORAGE_MAX_LEVEL );
    return ( m_arrRuneStorageUpgradeCost + iLevel );
}
inline UInt GameParameters::GetRuneCollectionRoom( UInt iLevel ) const {
    Assert( iLevel < RUNE_COLLECTION_MAX_LEVEL );
    return m_arrRuneCollectionRoom[iLevel];
}
inline UInt GameParameters::GetRuneStorageRoom( UInt iLevel ) const {
    Assert( iLevel < RUNE_STORAGE_MAX_LEVEL );
    return m_arrRuneStorageRoom[iLevel];
}

inline const CurrencyCost * GameParameters::GetManaProductionRateUpgradeCost( UInt iLevel ) const {
    Assert( iLevel < BUILDING_MAX_LEVEL );
    return ( m_arrManaProductionRateUpgradeCost + iLevel );
}
inline const CurrencyCost * GameParameters::GetManaCapacityUpgradeCost( UInt iLevel ) const {
    Assert( iLevel < BUILDING_MAX_LEVEL );
    return ( m_arrManaCapacityUpgradeCost + iLevel );
}
inline UInt GameParameters::GetManaProductionRate( UInt iLevel ) const {
    Assert( iLevel < BUILDING_MAX_LEVEL );
    return m_arrManaProductionRate[iLevel];
}
inline UInt GameParameters::GetManaCapacity( UInt iLevel ) const {
    Assert( iLevel < BUILDING_MAX_LEVEL );
    return m_arrManaCapacity[iLevel];
}

inline const CurrencyCost * GameParameters::GetCrystalProductionRateUpgradeCost( UInt iLevel ) const {
    Assert( iLevel < BUILDING_MAX_LEVEL );
    return ( m_arrCrystalProductionRateUpgradeCost + iLevel );
}
inline const CurrencyCost * GameParameters::GetCrystalCapacityUpgradeCost( UInt iLevel ) const {
    Assert( iLevel < BUILDING_MAX_LEVEL );
    return ( m_arrCrystalCapacityUpgradeCost + iLevel );
}
inline UInt GameParameters::GetCrystalProductionRate( UInt iLevel ) const {
    Assert( iLevel < BUILDING_MAX_LEVEL );
    return m_arrCrystalProductionRate[iLevel];
}
inline UInt GameParameters::GetCrystalCapacity( UInt iLevel ) const {
    Assert( iLevel < BUILDING_MAX_LEVEL );
    return m_arrCrystalCapacity[iLevel];
}

