/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/GameParameters.cpp
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
// Includes
#include "GameParameters.h"

/////////////////////////////////////////////////////////////////////////////////
// GameParameters implementation
GameParameters::GameParameters():
    m_hRuneLevelingStats()
{
    // Monster parameters
    m_fMonsterBaseCritRate = 0.0f;
    m_fMonsterBaseCritDamage = 0.0f;
    m_fMonsterBaseAccuracy = 0.0f;
    m_fMonsterBaseResistance = 0.0f;

    m_iAwakeningBonusBaseSpeed = 0;
    m_iAwakeningBonusSpeed = 0;
    m_fAwakeningBonusCritRate = 0.0f;
    m_fAwakeningBonusAccuracy = 0.0f;
    m_fAwakeningBonusResistance = 0.0f;

    for( UInt i = 0; i < MONSTER_MAX_RANK; ++i ) {
        for( UInt j = 0; j < MONSTER_MAX_LEVEL; ++j ) {
            m_arrMonsterXPByLevel[i][j] = 0;
            m_arrMonsterXPFromPowerUp[i][j] = 0;
        }
    }

    // Skill parameters

    // Rune parameters
    for( UInt i = 0; i < MONSTER_STAT_COUNT; ++i ) {
        m_arrRuneSlotsAllowed[0][i] = 0;
        m_arrRuneSlotsAllowed[1][i] = 0;
    }

    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        m_arrRuneSetRequiredCount[i] = 0;
        m_arrRuneSetStatBonus[i] = 0.0f;
    }

    // Town parameters
    for( UInt i = 0; i < BUILDING_COUNT; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrBuildingCost[i].arrCost[j] = 0;
    }
    
    for( UInt i = 0; i < CURRENCY_COUNT; ++i ) {
        m_hEssenceFusionUnlockCost.arrCost[i] = 0;
        m_hMonsterSummoningUnlockCost.arrCost[i] = 0;
        m_hMonsterFusionUnlockCost.arrCost[i] = 0;
        m_hMonsterPowerUpUnlockCost.arrCost[i] = 0;
        m_hMonsterEvolutionUnlockCost.arrCost[i] = 0;
        m_hMonsterAwakeningUnlockCost.arrCost[i] = 0;
        m_hRunePowerUpUnlockCost.arrCost[i] = 0;
        m_hRuneEvolutionUnlockCost.arrCost[i] = 0;
    }

    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i )
        m_arrEssenceFusionCost[i] = 0;
    for( UInt i = 0; i < MONSTER_MAX_RANK; ++i ) {
        for( UInt k = 0; k < CURRENCY_COUNT; ++k ) {
            m_arrMonsterFusionCost[i].arrCost[k] = 0;
            m_arrMonsterEvolutionCost[i].arrCost[k] = 0;
            m_arrRuneEvolutionCosts[i].arrCost[k] = 0;
        }
        for( UInt j = 0; j < MONSTER_MAX_RANK; ++j ) {
            for( UInt k = 0; k < CURRENCY_COUNT; ++k ) {
                m_arrMonsterPowerUpCost[i][j].arrCost[k] = 0;
                m_arrRunePowerUpCosts[i][j].arrCost[k] = 0;
            }
        }
    }

    for( UInt i = 0; i < MONSTER_COLLECTION_MAX_LEVEL; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrMonsterCollectionUpgradeCost[i].arrCost[j] = 0;
        m_arrMonsterCollectionRoom[i] = 0;
    }
    for( UInt i = 0; i < MONSTER_STORAGE_MAX_LEVEL; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrMonsterStorageUpgradeCost[i].arrCost[j] = 0;
        m_arrMonsterStorageRoom[i] = 0;
    }

    for( UInt i = 0; i < RUNE_COLLECTION_MAX_LEVEL; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrRuneCollectionUpgradeCost[i].arrCost[j] = 0;
        m_arrRuneCollectionRoom[i] = 0;
    }
    for( UInt i = 0; i < RUNE_STORAGE_MAX_LEVEL; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrRuneStorageUpgradeCost[i].arrCost[j] = 0;
        m_arrRuneStorageRoom[i] = 0;
    }

    for( UInt i = 0; i < BUILDING_MAX_LEVEL; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j ) {
            m_arrManaProductionRateUpgradeCost[i].arrCost[j] = 0;
            m_arrManaCapacityUpgradeCost[i].arrCost[j] = 0;
            m_arrCrystalProductionRateUpgradeCost[i].arrCost[j] = 0;
            m_arrCrystalCapacityUpgradeCost[i].arrCost[j] = 0;
        }
        m_arrManaProductionRate[i] = 0;
        m_arrManaCapacity[i] = 0;
        m_arrCrystalProductionRate[i] = 0;
        m_arrCrystalCapacity[i] = 0;
    }
}
GameParameters::~GameParameters()
{
    // nothing to do
}

Void GameParameters::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("GameParameters") ) == 0 );

    // Monster parameters
    const XMLNode * pMonsterParametersNode = pNode->GetChildByTag( TEXT("MonsterParameters"), 0 );
    Assert( pMonsterParametersNode != NULL );

    m_fMonsterBaseCritRate = StringFn->ToFloat( pMonsterParametersNode->GetAttribute(TEXT("BaseCritRate"))->GetValue() );
    m_fMonsterBaseCritDamage = StringFn->ToFloat( pMonsterParametersNode->GetAttribute(TEXT("BaseCritDamage"))->GetValue() );
    m_fMonsterBaseAccuracy = StringFn->ToFloat( pMonsterParametersNode->GetAttribute(TEXT("BaseAccuracy"))->GetValue() );
    m_fMonsterBaseResistance = StringFn->ToFloat( pMonsterParametersNode->GetAttribute(TEXT("BaseResistance"))->GetValue() );

    const XMLNode * pAwakeningBonusNode = pMonsterParametersNode->GetChildByTag( TEXT("AwakeningBonus"), 0 );
    Assert( pAwakeningBonusNode != NULL );

    m_iAwakeningBonusBaseSpeed = (UInt)( StringFn->ToUInt( pAwakeningBonusNode->GetAttribute(TEXT("BaseSpeed"))->GetValue() ) );
    m_iAwakeningBonusSpeed = (UInt)( StringFn->ToUInt( pAwakeningBonusNode->GetAttribute(TEXT("Speed"))->GetValue() ) );
    m_fAwakeningBonusCritRate = StringFn->ToFloat( pMonsterParametersNode->GetAttribute(TEXT("CritRate"))->GetValue() );
    m_fAwakeningBonusAccuracy = StringFn->ToFloat( pMonsterParametersNode->GetAttribute(TEXT("Accuracy"))->GetValue() );
    m_fAwakeningBonusResistance = StringFn->ToFloat( pMonsterParametersNode->GetAttribute(TEXT("Resistance"))->GetValue() );

    const XMLNode * pXPByLevelNode = pMonsterParametersNode->GetChildByTag( TEXT("XPByLevel"), 0 );
    Assert( pXPByLevelNode != NULL );
    const XMLNode * pXPFromPowerUpNode = pMonsterParametersNode->GetChildByTag( TEXT("XPFromPowerUp"), 0 );
    Assert( pXPFromPowerUpNode != NULL );

    for( UInt i = 0; i < MONSTER_MAX_RANK; ++i ) {
        GChar strRankName[16];
        StringFn->Format( strRankName, TEXT("Rank_%d"), i );

        const XMLNode * pXPByLevelRankNode = pXPByLevelNode->GetChildByTag( strRankName, 0 );
        Assert( pXPByLevelRankNode != NULL );
        const XMLNode * pXPFromPowerUpRankNode = pXPFromPowerUpNode->GetChildByTag( strRankName, 0 );
        Assert( pXPFromPowerUpRankNode != NULL );

        for( UInt j = 0; j < MONSTER_MAX_LEVEL; ++j ) {
            GChar strLevelName[16];
            StringFn->Format( strLevelName, TEXT("Level_%d"), i );

            m_arrMonsterXPByLevel[i][j] = (UInt)( StringFn->ToUInt( pXPByLevelRankNode->GetAttribute(strLevelName)->GetValue() ) );
            m_arrMonsterXPFromPowerUp[i][j] = (UInt)( StringFn->ToUInt( pXPFromPowerUpRankNode->GetAttribute(strLevelName)->GetValue() ) );
        }
    }

    // Skill parameters

    // Rune parameters
    const XMLNode * pRuneParametersNode = pNode->GetChildByTag( TEXT("RuneParameters"), 0 );
    Assert( pRuneParametersNode != NULL );

    const XMLNode * pRuneSlotsAllowedNode = pNode->GetChildByTag( TEXT("SlotsAllowed"), 0 );
    Assert( pRuneSlotsAllowedNode != NULL );

    for( UInt i = 0; i < MONSTER_STAT_COUNT; ++i ) {
        const GChar * strName = MonsterStatisticToString( (MonsterStatistic)i );
        const XMLNode * pMonsterStatNode = pRuneSlotsAllowedNode->GetChildByTag( strName, 0 );
        Assert( pMonsterStatNode != NULL );

        m_arrRuneSlotsAllowed[0][i] = (Byte)( StringFn->ToUInt( pMonsterStatNode->GetAttribute(TEXT("Flat"))->GetValue() ) );
        m_arrRuneSlotsAllowed[1][i] = (Byte)( StringFn->ToUInt( pMonsterStatNode->GetAttribute(TEXT("Ratio"))->GetValue() ) );
    }

    const XMLNode * pRuneSetsNode = pNode->GetChildByTag( TEXT("RuneSets"), 0 );
    Assert( pRuneSetsNode != NULL );

    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        const GChar * strName = RuneTypeToString( (RuneType)i );
        const XMLNode * pRuneTypeNode = pRuneSetsNode->GetChildByTag( strName, 0 );
        Assert( pRuneTypeNode != NULL );

        m_arrRuneSetRequiredCount[i] = (UInt)( StringFn->ToUInt( pRuneTypeNode->GetAttribute(TEXT("RequiredCount"))->GetValue() ) );
        m_arrRuneSetStatBonus[i] = StringFn->ToFloat( pRuneTypeNode->GetAttribute(TEXT("StatBonus"))->GetValue() );
    }

    const XMLNode * pRuneLevelingStatsNode = pNode->GetChildByTag( TEXT("RuneLevelingStats"), 0 );
    Assert( pRuneLevelingStatsNode != NULL );

    m_hRuneLevelingStats.Load( pRuneLevelingStatsNode );

    // Town parameters
    for( UInt i = 0; i < BUILDING_COUNT; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrBuildingCost[i].arrCost[j] = 0;
    }

    for( UInt i = 0; i < CURRENCY_COUNT; ++i ) {
        m_hEssenceFusionUnlockCost.arrCost[i] = 0;
        m_hMonsterSummoningUnlockCost.arrCost[i] = 0;
        m_hMonsterFusionUnlockCost.arrCost[i] = 0;
        m_hMonsterPowerUpUnlockCost.arrCost[i] = 0;
        m_hMonsterEvolutionUnlockCost.arrCost[i] = 0;
        m_hMonsterAwakeningUnlockCost.arrCost[i] = 0;
        m_hRunePowerUpUnlockCost.arrCost[i] = 0;
        m_hRuneEvolutionUnlockCost.arrCost[i] = 0;
    }

    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i )
        m_arrEssenceFusionCost[i] = 0;
    for( UInt i = 0; i < MONSTER_MAX_RANK; ++i ) {
        for( UInt k = 0; k < CURRENCY_COUNT; ++k ) {
            m_arrMonsterFusionCost[i].arrCost[k] = 0;
            m_arrMonsterEvolutionCost[i].arrCost[k] = 0;
            m_arrRuneEvolutionCosts[i].arrCost[k] = 0;
        }
        for( UInt j = 0; j < MONSTER_MAX_RANK; ++j ) {
            for( UInt k = 0; k < CURRENCY_COUNT; ++k ) {
                m_arrMonsterPowerUpCost[i][j].arrCost[k] = 0;
                m_arrRunePowerUpCosts[i][j].arrCost[k] = 0;
            }
        }
    }

    for( UInt i = 0; i < MONSTER_COLLECTION_MAX_LEVEL; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrMonsterCollectionUpgradeCost[i].arrCost[j] = 0;
        m_arrMonsterCollectionRoom[i] = 0;
    }
    for( UInt i = 0; i < MONSTER_STORAGE_MAX_LEVEL; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrMonsterStorageUpgradeCost[i].arrCost[j] = 0;
        m_arrMonsterStorageRoom[i] = 0;
    }

    for( UInt i = 0; i < RUNE_COLLECTION_MAX_LEVEL; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrRuneCollectionUpgradeCost[i].arrCost[j] = 0;
        m_arrRuneCollectionRoom[i] = 0;
    }
    for( UInt i = 0; i < RUNE_STORAGE_MAX_LEVEL; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrRuneStorageUpgradeCost[i].arrCost[j] = 0;
        m_arrRuneStorageRoom[i] = 0;
    }

    for( UInt i = 0; i < BUILDING_MAX_LEVEL; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j ) {
            m_arrManaProductionRateUpgradeCost[i].arrCost[j] = 0;
            m_arrManaCapacityUpgradeCost[i].arrCost[j] = 0;
            m_arrCrystalProductionRateUpgradeCost[i].arrCost[j] = 0;
            m_arrCrystalCapacityUpgradeCost[i].arrCost[j] = 0;
        }
        m_arrManaProductionRate[i] = 0;
        m_arrManaCapacity[i] = 0;
        m_arrCrystalProductionRate[i] = 0;
        m_arrCrystalCapacity[i] = 0;
    }

    m_bIsSummon = ( StringFn->ToUInt(pNode->GetAttribute(TEXT("IsSummon"))->GetValue()) != 0 );
    m_iMonsterID = (MonsterID)( StringFn->ToUInt( pNode->GetAttribute(TEXT("MonsterID"))->GetValue() ) );
    StringFn->NCopy( m_strName, pNode->GetAttribute(TEXT("Name"))->GetValue(), MONSTER_NAME_LENGTH - 1 );
    m_iNativeRank = (UInt)( StringFn->ToUInt( pNode->GetAttribute(TEXT("NativeRank"))->GetValue() ) );
    m_arrBonusDamage[i] = StringFn->ToFloat( arrBonusDamage[i] );

    const XMLNode * pEssencesNode = pNode->GetChildByTag( TEXT("Essences"), 0 );
    Assert( pEssencesNode != NULL );
}

const GChar * GameParameters::CurrencyTypeToString( CurrencyType iCurrencyType ) const
{
}
CurrencyType GameParameters::CurrencyTypeFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::ScrollTypeToString( ScrollType iScrollType ) const
{
}
ScrollType GameParameters::ScrollTypeFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::MonsterTypeToString( MonsterType iMonsterType ) const
{
}
MonsterType GameParameters::MonsterTypeFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::MonsterElementToString( MonsterElement iMonsterElement ) const
{
}
MonsterElement GameParameters::MonsterElementFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::MonsterStatisticToString( MonsterStatistic iMonsterStatistic ) const
{
}
MonsterStatistic GameParameters::MonsterStatisticFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::MonsterAwakeningBonusToString( MonsterAwakeningBonus iMonsterAwakeningBonus ) const
{
}
MonsterAwakeningBonus GameParameters::MonsterAwakeningBonusFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::StatusEffectTypeToString( StatusEffectType iStatusEffectType ) const
{
}
StatusEffectType GameParameters::StatusEffectTypeFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::SkillEffectTypeToString( SkillEffectType iSkillEffectType ) const
{
}
SkillEffectType GameParameters::SkillEffectTypeFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::SkillTargetPatternToString( SkillTargetPattern iSkillTargetPattern ) const
{
}
SkillTargetPattern GameParameters::SkillTargetPatternFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::SkillEffectScalingToString( SkillEffectScaling iSkillEffectScaling ) const
{
}
SkillEffectScaling GameParameters::SkillEffectScalingFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::SkillTypeToString( SkillType iSkillType ) const
{
}
SkillType GameParameters::SkillTypeFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::SkillActiveTypeToString( SkillActiveType iSkillActiveType ) const
{
}
SkillActiveType GameParameters::SkillActiveTypeFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::SkillPassiveTypeToString( SkillPassiveType iSkillPassiveType ) const
{
}
SkillPassiveType GameParameters::SkillPassiveTypeFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::SkillLeaderConstraintToString( SkillLeaderConstraint iSkillLeaderConstraint ) const
{
}
SkillLeaderConstraint GameParameters::SkillLeaderConstraintFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::RuneTypeToString( RuneType iRuneType ) const
{
}
RuneType GameParameters::RuneTypeFromString( const GChar * strValue ) const
{
}

const GChar * GameParameters::RuneStatisticToString( RuneStatistic iRuneStatistic ) const
{
}
RuneStatistic GameParameters::RuneStatisticFromString( const GChar * strValue ) const
{
}

