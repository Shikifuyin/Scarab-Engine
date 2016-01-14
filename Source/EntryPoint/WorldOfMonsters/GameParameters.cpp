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
        // Building costs
    for( UInt i = 0; i < BUILDING_COUNT; ++i ) {
        for( UInt j = 0; j < CURRENCY_COUNT; ++j )
            m_arrBuildingCost[i].arrCost[j] = 0;
    }
    
        // Unlock costs
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

        // Operation costs
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

        // Upgrade costs & parameters
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

    _LoadMonsterParameters( pMonsterParametersNode );

    // Skill parameters

    // Rune parameters
    const XMLNode * pRuneParametersNode = pNode->GetChildByTag( TEXT("RuneParameters"), 0 );
    Assert( pRuneParametersNode != NULL );

    _LoadRuneParameters( pRuneParametersNode );

    // Town parameters
    const XMLNode * pTownParametersNode = pNode->GetChildByTag( TEXT("TownParameters"), 0 );
    Assert( pTownParametersNode != NULL );

        // Building costs
    const XMLNode * pBuildingCostsNode = pTownParametersNode->GetChildByTag( TEXT("BuildingCosts"), 0 );
    Assert( pBuildingCostsNode != NULL );

    _LoadTownParameters_BuildingCosts( pBuildingCostsNode );

        // Unlock costs
    const XMLNode * pUnlockCostsNode = pTownParametersNode->GetChildByTag( TEXT("UnlockCosts"), 0 );
    Assert( pUnlockCostsNode != NULL );

    _LoadTownParameters_UnlockCosts( pUnlockCostsNode );

        // Operations costs
    const XMLNode * pOperationCostsNode = pTownParametersNode->GetChildByTag( TEXT("OperationCosts"), 0 );
    Assert( pOperationCostsNode != NULL );

    _LoadTownParameters_OperationCosts( pOperationCostsNode );

        // Upgrade costs
    const XMLNode * pUpgradeCostsNode = pTownParametersNode->GetChildByTag( TEXT("UpgradeCosts"), 0 );
    Assert( pUpgradeCostsNode != NULL );

    _LoadTownParameters_UpgradeCosts( pUpgradeCostsNode );

        // Upgrade parameters
    const XMLNode * pUpgradeParametersNode = pTownParametersNode->GetChildByTag( TEXT("UpgradeParameters"), 0 );
    Assert( pUpgradeParametersNode != NULL );

    _LoadTownParameters_UpgradeParameters( pUpgradeParametersNode );
}

const GChar * GameParameters::BuildingTypeToString( BuildingType iBuildingType ) const
{
    static const GChar * s_arrConvert[BUILDING_COUNT] = {
        TEXT("Dungeon"),          // BUILDING_DUNGEON
        TEXT("ArcaneTower"),      // BUILDING_ARCANE_TOWER
        TEXT("EssenceStorage"),   // BUILDING_ESSENCE_STORAGE
        TEXT("MonsterStorage"),   // BUILDING_MONSTER_STORAGE
        TEXT("MonsterSummoning"), // BUILDING_MONSTER_SUMMONING
        TEXT("MonsterEvolution"), // BUILDING_MONSTER_EVOLUTION
        TEXT("MonsterFusion"),    // BUILDING_MONSTER_FUSION
      //TEXT("MonsterSkin"),      // BUILDING_MONSTER_SKINS
        TEXT("RuneStorage"),      // BUILDING_RUNE_STORAGE
        TEXT("RuneEvolution"),    // BUILDING_RUNE_EVOLUTION
        TEXT("Mana"),             // BUILDING_MANA
        TEXT("Crystal"),          // BUILDING_CRYSTAL
        TEXT("Shop"),             // BUILDING_SHOP
        TEXT("Wishes"),           // BUILDING_WISHES
        TEXT("BonusHP"),          // BUILDING_BONUS_MONSTERS_HP
        TEXT("BonusATT"),         // BUILDING_BONUS_MONSTERS_ATT
        TEXT("BonusATT_Elt"),     // BUILDING_BONUS_MONSTERS_ATT_ELEMENT
        TEXT("BonusDEF"),         // BUILDING_BONUS_MONSTERS_DEF
        TEXT("BonusSPD"),         // BUILDING_BONUS_MONSTERS_SPD
        TEXT("BonusCritD"),       // BUILDING_BONUS_MONSTERS_CRITDMG
        TEXT("BonusTowerATT"),    // BUILDING_BONUS_ARCANETOWERS_ATT
        TEXT("BonusTowerSPD")     // BUILDING_BONUS_ARCANETOWERS_SPD
    };
    Assert( iBuildingType < BUILDING_COUNT );
    return s_arrConvert[iBuildingType];
}
BuildingType GameParameters::BuildingTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < BUILDING_COUNT; ++i ) {
        BuildingType iEnum = (BuildingType)i;
        if ( StringFn->Cmp(strValue, BuildingTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return BUILDING_COUNT;
}

const GChar * GameParameters::CurrencyTypeToString( CurrencyType iCurrencyType ) const
{
    static const GChar * s_arrConvert[CURRENCY_COUNT] = {
        TEXT("Mana"),       // CURRENCY_MANA
        TEXT("Crystal"),    // CURRENCY_CRYSTAL
        TEXT("GloryPoint"), // CURRENCY_GLORYPOINT
        TEXT("GuildPoint")  // CURRENCY_GUILDPOINT
    };
    Assert( iCurrencyType < CURRENCY_COUNT );
    return s_arrConvert[iCurrencyType];
}
CurrencyType GameParameters::CurrencyTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < CURRENCY_COUNT; ++i ) {
        CurrencyType iEnum = (CurrencyType)i;
        if ( StringFn->Cmp(strValue, CurrencyTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return CURRENCY_COUNT;
}

const GChar * GameParameters::ScrollTypeToString( ScrollType iScrollType ) const
{
    static const GChar * s_arrConvert[SCROLL_TYPE_COUNT] = {
        TEXT("Common"),    // SCROLL_COMMON
        TEXT("Mystical"),  // SCROLL_MYSTICAL
        TEXT("Legendary"), // SCROLL_LEGENDARY
        TEXT("Fire"),      // SCROLL_FIRE
        TEXT("Water"),     // SCROLL_WATER
        TEXT("Wind"),      // SCROLL_WIND
        TEXT("Light"),     // SCROLL_LIGHT
        TEXT("Dark")       // SCROLL_DARK
    };
    Assert( iScrollType < SCROLL_TYPE_COUNT );
    return s_arrConvert[iScrollType];
}
ScrollType GameParameters::ScrollTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < SCROLL_TYPE_COUNT; ++i ) {
        ScrollType iEnum = (ScrollType)i;
        if ( StringFn->Cmp(strValue, ScrollTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return SCROLL_TYPE_COUNT;
}

const GChar * GameParameters::EssenceTypeToString( EssenceType iEssenceType ) const
{
    static const GChar * s_arrConvert[ESSENCE_TYPE_COUNT] = {
        TEXT("Low"), // ESSENCE_LOW
        TEXT("Mid"), // ESSENCE_MID
        TEXT("High") // ESSENCE_HIGH
    };
    Assert( iEssenceType < ESSENCE_TYPE_COUNT );
    return s_arrConvert[iEssenceType];
}
EssenceType GameParameters::EssenceTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i ) {
        EssenceType iEnum = (EssenceType)i;
        if ( StringFn->Cmp(strValue, EssenceTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return ESSENCE_TYPE_COUNT;
}

const GChar * GameParameters::MonsterTypeToString( MonsterType iMonsterType ) const
{
    static const GChar * s_arrConvert[MONSTER_TYPE_COUNT] = {
        TEXT("Attack"),  // MONSTER_ATTACK
        TEXT("Support"), // MONSTER_SUPPORT
        TEXT("TankHP"),  // MONSTER_TANK_HP
        TEXT("TankDEF")  // MONSTER_TANK_DEF
    };
    Assert( iMonsterType < MONSTER_TYPE_COUNT );
    return s_arrConvert[iMonsterType];
}
MonsterType GameParameters::MonsterTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < MONSTER_TYPE_COUNT; ++i ) {
        MonsterType iEnum = (MonsterType)i;
        if ( StringFn->Cmp(strValue, MonsterTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return MONSTER_TYPE_COUNT;
}

const GChar * GameParameters::MonsterElementToString( MonsterElement iMonsterElement ) const
{
    static const GChar * s_arrConvert[MONSTER_ELEMENT_COUNT] = {
        TEXT("Magic"), // MONSTER_ELEMENT_MAGIC
        TEXT("Fire"),  // MONSTER_ELEMENT_FIRE
        TEXT("Water"), // MONSTER_ELEMENT_WATER
        TEXT("Wind"),  // MONSTER_ELEMENT_WIND
        TEXT("Light"), // MONSTER_ELEMENT_LIGHT
        TEXT("Dark")   // MONSTER_ELEMENT_DARK

    };
    Assert( iMonsterElement < MONSTER_ELEMENT_COUNT );
    return s_arrConvert[iMonsterElement];
}
MonsterElement GameParameters::MonsterElementFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < MONSTER_ELEMENT_COUNT; ++i ) {
        MonsterElement iEnum = (MonsterElement)i;
        if ( StringFn->Cmp(strValue, MonsterElementToString(iEnum)) == 0 )
            return iEnum;
    }
    return MONSTER_ELEMENT_COUNT;
}

const GChar * GameParameters::MonsterStatisticToString( MonsterStatistic iMonsterStatistic ) const
{
    static const GChar * s_arrConvert[MONSTER_STAT_COUNT] = {
        TEXT("Health"),     // MONSTER_STAT_HEALTH
        TEXT("Attack"),     // MONSTER_STAT_ATTACK
        TEXT("Defense"),    // MONSTER_STAT_DEFENSE
        TEXT("Speed"),      // MONSTER_STAT_SPEED
        TEXT("CritRate"),   // MONSTER_STAT_CRIT_RATE
        TEXT("CritDamage"), // MONSTER_STAT_CRIT_DMG
        TEXT("Accuracy"),   // MONSTER_STAT_ACCURACY
        TEXT("Resistance")  // MONSTER_STAT_RESISTANCE
    };
    Assert( iMonsterStatistic < MONSTER_STAT_COUNT );
    return s_arrConvert[iMonsterStatistic];
}
MonsterStatistic GameParameters::MonsterStatisticFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < MONSTER_STAT_COUNT; ++i ) {
        MonsterStatistic iEnum = (MonsterStatistic)i;
        if ( StringFn->Cmp(strValue, MonsterStatisticToString(iEnum)) == 0 )
            return iEnum;
    }
    return MONSTER_STAT_COUNT;
}

const GChar * GameParameters::MonsterAwakeningBonusToString( MonsterAwakeningBonus iMonsterAwakeningBonus ) const
{
    static const GChar * s_arrConvert[MONSTER_AWAKENING_BONUS_COUNT] = {
        TEXT("Speed"),       // MONSTER_AWAKENING_SPEED
        TEXT("CritRate"),    // MONSTER_AWAKENING_CRITICALRATE
        TEXT("Accuracy"),    // MONSTER_AWAKENING_ACCURACY
        TEXT("Resistance"),  // MONSTER_AWAKENING_RESISTANCE
        TEXT("SkillNew"),    // MONSTER_AWAKENING_SKILLNEW
        TEXT("SkillUpgrade") // MONSTER_AWAKENING_SKILLUPGRADE
    };
    Assert( iMonsterAwakeningBonus < MONSTER_AWAKENING_BONUS_COUNT );
    return s_arrConvert[iMonsterAwakeningBonus];
}
MonsterAwakeningBonus GameParameters::MonsterAwakeningBonusFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < MONSTER_AWAKENING_BONUS_COUNT; ++i ) {
        MonsterAwakeningBonus iEnum = (MonsterAwakeningBonus)i;
        if ( StringFn->Cmp(strValue, MonsterAwakeningBonusToString(iEnum)) == 0 )
            return iEnum;
    }
    return MONSTER_AWAKENING_BONUS_COUNT;
}

const GChar * GameParameters::StatusEffectTypeToString( StatusEffectType iStatusEffectType ) const
{
    static const GChar * s_arrConvert[STATUSEFFECT_COUNT] = {
        TEXT("BuffATT"),           // STATUSEFFECT_BUFF_ATTACK
        TEXT("BuffDEF"),           // STATUSEFFECT_BUFF_DEFENSE
        TEXT("BuffSPD"),           // STATUSEFFECT_BUFF_SPEED
        TEXT("BuffCritR"),         // STATUSEFFECT_BUFF_CRITRATE
        TEXT("BuffRegen"),         // STATUSEFFECT_BUFF_REGEN
        TEXT("BuffShield"),        // STATUSEFFECT_BUFF_SHIELD
        TEXT("BuffInvincibility"), // STATUSEFFECT_BUFF_INVINCIBILITY
        TEXT("BuffImmunity"),      // STATUSEFFECT_BUFF_IMMUNITY
        TEXT("DebuffATT"),         // STATUSEFFECT_DEBUFF_ATTACK
        TEXT("DebuffDEF"),         // STATUSEFFECT_DEBUFF_DEFENSE
        TEXT("DebuffSPD"),         // STATUSEFFECT_DEBUFF_SPEED
        TEXT("DebuffCritR"),       // STATUSEFFECT_DEBUFF_CRITRATE
        TEXT("DebuffDestroy"),     // STATUSEFFECT_DEBUFF_DESTROY
        TEXT("DebuffDOT"),         // STATUSEFFECT_DEBUFF_DOT
        TEXT("DebuffBomb"),        // STATUSEFFECT_DEBUFF_BOMB
        TEXT("DebuffMarkDmg"),     // STATUSEFFECT_DEBUFF_MARK_DMG
        TEXT("DebuffMarkDrain"),   // STATUSEFFECT_DEBUFF_MARK_DRAIN
        TEXT("DebuffNoHeal"),      // STATUSEFFECT_DEBUFF_NOHEAL
        TEXT("DebuffStun"),        // STATUSEFFECT_DEBUFF_STUN
        TEXT("DebuffFreeze"),      // STATUSEFFECT_DEBUFF_FREEZE
        TEXT("DebuffSleep")        // STATUSEFFECT_DEBUFF_SLEEP
    };
    Assert( iStatusEffectType < STATUSEFFECT_COUNT );
    return s_arrConvert[iStatusEffectType];
}
StatusEffectType GameParameters::StatusEffectTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < STATUSEFFECT_COUNT; ++i ) {
        StatusEffectType iEnum = (StatusEffectType)i;
        if ( StringFn->Cmp(strValue, StatusEffectTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return STATUSEFFECT_COUNT;
}

const GChar * GameParameters::SkillEffectTypeToString( SkillEffectType iSkillEffectType ) const
{
    static const GChar * s_arrConvert[SKILLEFFECT_COUNT] = {
        TEXT("Damage"), // SKILLEFFECT_DAMAGE
        TEXT("Heal"),   // SKILLEFFECT_HEAL
        TEXT("ATB"),    // SKILLEFFECT_ATB
        TEXT("Status")  // SKILLEFFECT_STATUS
    };
    Assert( iSkillEffectType < SKILLEFFECT_COUNT );
    return s_arrConvert[iSkillEffectType];
}
SkillEffectType GameParameters::SkillEffectTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < SKILLEFFECT_COUNT; ++i ) {
        SkillEffectType iEnum = (SkillEffectType)i;
        if ( StringFn->Cmp(strValue, SkillEffectTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return SKILLEFFECT_COUNT;
}

const GChar * GameParameters::SkillTargetPatternToString( SkillTargetPattern iSkillTargetPattern ) const
{
    static const GChar * s_arrConvert[SKILL_TARGET_COUNT] = {
        TEXT("Self"),               // SKILL_TARGET_SELF
        TEXT("AllySingle"),         // SKILL_TARGET_ALLY_SINGLE
        TEXT("AllySingleRandom"),   // SKILL_TARGET_ALLY_SINGLE_RANDOM
        TEXT("AllyDual"),           // SKILL_TARGET_ALLY_DUAL
        TEXT("AllyDualRandom"),     // SKILL_TARGET_ALLY_DUAL_RANDOM
        TEXT("AllyWide"),           // SKILL_TARGET_ALLY_WIDE
        TEXT("AllyWideRandom"),     // SKILL_TARGET_ALLY_WIDE_RANDOM
        TEXT("AllyAll"),            // SKILL_TARGET_ALLY_ALL
        TEXT("AllyAllExcept"),      // SKILL_TARGET_ALLY_ALL_EXCEPT
        TEXT("EnnemySingle"),       // SKILL_TARGET_ENNEMY_SINGLE
        TEXT("EnnemySingleRandom"), // SKILL_TARGET_ENNEMY_SINGLE_RANDOM
        TEXT("EnnemyDual"),         // SKILL_TARGET_ENNEMY_DUAL
        TEXT("EnnemyDualRandom"),   // SKILL_TARGET_ENNEMY_DUAL_RANDOM
        TEXT("EnnemyWide"),         // SKILL_TARGET_ENNEMY_WIDE
        TEXT("EnnemyWideRandom"),   // SKILL_TARGET_ENNEMY_WIDE_RANDOM
        TEXT("EnnemyAll"),          // SKILL_TARGET_ENNEMY_ALL
        TEXT("EnnemyAllExcept"),    // SKILL_TARGET_ENNEMY_ALL_EXCEPT
        TEXT("All")                 // SKILL_TARGET_ALL
    };
    Assert( iSkillTargetPattern < SKILL_TARGET_COUNT );
    return s_arrConvert[iSkillTargetPattern];
}
SkillTargetPattern GameParameters::SkillTargetPatternFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < SKILL_TARGET_COUNT; ++i ) {
        SkillTargetPattern iEnum = (SkillTargetPattern)i;
        if ( StringFn->Cmp(strValue, SkillTargetPatternToString(iEnum)) == 0 )
            return iEnum;
    }
    return SKILL_TARGET_COUNT;
}

const GChar * GameParameters::SkillEffectScalingToString( SkillEffectScaling iSkillEffectScaling ) const
{
    static const GChar * s_arrConvert[SKILLEFFECT_SCALING_COUNT] = {
        TEXT("Default"),         // SKILLEFFECT_SCALING_DEFAULT
        TEXT("SelfCurrentHP"),   // SKILLEFFECT_SCALING_SELF_HP_CURRENT
        TEXT("SelfMaxHP"),       // SKILLEFFECT_SCALING_SELF_HP_MAX
        TEXT("TargetCurrentHP"), // SKILLEFFECT_SCALING_TARGET_HP_CURRENT
        TEXT("TargetMaxHP")      // SKILLEFFECT_SCALING_TARGET_HP_MAX
    };
    Assert( iSkillEffectScaling < SKILLEFFECT_SCALING_COUNT );
    return s_arrConvert[iSkillEffectScaling];
}
SkillEffectScaling GameParameters::SkillEffectScalingFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < SKILLEFFECT_SCALING_COUNT; ++i ) {
        SkillEffectScaling iEnum = (SkillEffectScaling)i;
        if ( StringFn->Cmp(strValue, SkillEffectScalingToString(iEnum)) == 0 )
            return iEnum;
    }
    return SKILLEFFECT_SCALING_COUNT;
}

const GChar * GameParameters::SkillTypeToString( SkillType iSkillType ) const
{
    static const GChar * s_arrConvert[SKILL_TYPE_COUNT] = {
        TEXT("Active"),  // SKILL_TYPE_ACTIVE
        TEXT("Passive"), // SKILL_TYPE_PASSIVE
        TEXT("Leader")   // SKILL_TYPE_LEADER
    };
    Assert( iSkillType < SKILL_TYPE_COUNT );
    return s_arrConvert[iSkillType];
}
SkillType GameParameters::SkillTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < SKILL_TYPE_COUNT; ++i ) {
        SkillType iEnum = (SkillType)i;
        if ( StringFn->Cmp(strValue, SkillTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return SKILL_TYPE_COUNT;
}

const GChar * GameParameters::SkillActiveTypeToString( SkillActiveType iSkillActiveType ) const
{
    static const GChar * s_arrConvert[SKILL_ACTIVE_COUNT] = {
        TEXT("OnHit"),         // SKILL_ACTIVE_ONHIT
        TEXT("OnCrit"),        // SKILL_ACTIVE_ONCRIT
        TEXT("OnStatusEffect") // SKILL_ACTIVE_ONSTATUSEFFECT
    };
    Assert( iSkillActiveType < SKILL_ACTIVE_COUNT );
    return s_arrConvert[iSkillActiveType];
}
SkillActiveType GameParameters::SkillActiveTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < SKILL_ACTIVE_COUNT; ++i ) {
        SkillActiveType iEnum = (SkillActiveType)i;
        if ( StringFn->Cmp(strValue, SkillActiveTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return SKILL_ACTIVE_COUNT;
}

const GChar * GameParameters::SkillPassiveTypeToString( SkillPassiveType iSkillPassiveType ) const
{
    static const GChar * s_arrConvert[SKILL_PASSIVE_COUNT] = {
        TEXT("Persistent"),       // SKILL_PASSIVE_PERSISTENT
        TEXT("PeriodicSelf"),     // SKILL_PASSIVE_PERIODIC_SELF
        TEXT("PeriodicAllies"),   // SKILL_PASSIVE_PERIODIC_ALLIES
        TEXT("PeriodicEnnemies"), // SKILL_PASSIVE_PERIODIC_ENNEMIES
        TEXT("PeriodicAll"),      // SKILL_PASSIVE_PERIODIC_ALL
        TEXT("OnHit"),            // SKILL_PASSIVE_ONHIT
        TEXT("OnAllyHit"),        // SKILL_PASSIVE_ONHIT_ALLY
        TEXT("OnEnnemyHit"),      // SKILL_PASSIVE_ONHIT_ENNEMY
        TEXT("OnCrit"),           // SKILL_PASSIVE_ONCRIT
        TEXT("OnAllyCrit"),       // SKILL_PASSIVE_ONCRIT_ALLY
        TEXT("OnEnnemyCrit"),     // SKILL_PASSIVE_ONCRIT_ENNEMY
        TEXT("OnBeingHit"),       // SKILL_PASSIVE_ONBEINGHIT
        TEXT("OnAllyBeingHit"),   // SKILL_PASSIVE_ONBEINGHIT_ALLY
        TEXT("OnEnnemyBeingHit"), // SKILL_PASSIVE_ONBEINGHIT_ENNEMY
        TEXT("OnBeingCrit"),      // SKILL_PASSIVE_ONBEINGCRIT
        TEXT("OnAllyBeingCrit"),  // SKILL_PASSIVE_ONBEINGCRIT_ALLY
        TEXT("OnEnnemyBeingCrit") // SKILL_PASSIVE_ONBEINGCRIT_ENNEMY
    };
    Assert( iSkillPassiveType < SKILL_PASSIVE_COUNT );
    return s_arrConvert[iSkillPassiveType];
}
SkillPassiveType GameParameters::SkillPassiveTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < SKILL_PASSIVE_COUNT; ++i ) {
        SkillPassiveType iEnum = (SkillPassiveType)i;
        if ( StringFn->Cmp(strValue, SkillPassiveTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return SKILL_PASSIVE_COUNT;
}

const GChar * GameParameters::SkillLeaderConstraintToString( SkillLeaderConstraint iSkillLeaderConstraint ) const
{
    static const GChar * s_arrConvert[SKILL_LEADERCONSTRAINT_COUNT] = {
        TEXT("None"),       // SKILL_LEADERCONSTRAINT_NONE
        TEXT("Fire"),       // SKILL_LEADERCONSTRAINT_FIRE
        TEXT("Water"),      // SKILL_LEADERCONSTRAINT_WATER
        TEXT("Wind"),       // SKILL_LEADERCONSTRAINT_WIND
        TEXT("Light"),      // SKILL_LEADERCONSTRAINT_LIGHT
        TEXT("Dark"),       // SKILL_LEADERCONSTRAINT_DARK
        TEXT("Dungeon"),    // SKILL_LEADERCONSTRAINT_DUNGEON
        TEXT("Arena"),      // SKILL_LEADERCONSTRAINT_ARENA
        TEXT("GuildBattle") // SKILL_LEADERCONSTRAINT_GUILDBATTLE
    };
    Assert( iSkillLeaderConstraint < SKILL_LEADERCONSTRAINT_COUNT );
    return s_arrConvert[iSkillLeaderConstraint];
}
SkillLeaderConstraint GameParameters::SkillLeaderConstraintFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < SKILL_LEADERCONSTRAINT_COUNT; ++i ) {
        SkillLeaderConstraint iEnum = (SkillLeaderConstraint)i;
        if ( StringFn->Cmp(strValue, SkillLeaderConstraintToString(iEnum)) == 0 )
            return iEnum;
    }
    return SKILL_LEADERCONSTRAINT_COUNT;
}

const GChar * GameParameters::RuneTypeToString( RuneType iRuneType ) const
{
    static const GChar * s_arrConvert[RUNE_TYPE_COUNT] = {
        TEXT("Energy"),  // RUNE_ENERGY
        TEXT("Fatal"),   // RUNE_FATAL
        TEXT("Guard"),   // RUNE_GUARD
        TEXT("Swift"),   // RUNE_SWIFT
        TEXT("Blade"),   // RUNE_BLADE
        TEXT("Rage"),    // RUNE_RAGE
        TEXT("Focus"),   // RUNE_FOCUS
        TEXT("Endure"),  // RUNE_ENDURE
        TEXT("Shield"),  // RUNE_SHIELD
        TEXT("Will"),    // RUNE_WILL
        TEXT("Nemesis"), // RUNE_NEMESIS
        TEXT("Destroy"), // RUNE_DESTROY
        TEXT("Vampire"), // RUNE_VAMPIRE
        TEXT("Despair"), // RUNE_DESPAIR
        TEXT("Violent"), // RUNE_VIOLENT
        TEXT("Revenge")  // RUNE_REVENGE
    };
    Assert( iRuneType < RUNE_TYPE_COUNT );
    return s_arrConvert[iRuneType];
}
RuneType GameParameters::RuneTypeFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        RuneType iEnum = (RuneType)i;
        if ( StringFn->Cmp(strValue, RuneTypeToString(iEnum)) == 0 )
            return iEnum;
    }
    return RUNE_TYPE_COUNT;
}

const GChar * GameParameters::RuneStatisticToString( RuneStatistic iRuneStatistic ) const
{
    static const GChar * s_arrConvert[RUNE_STAT_COUNT] = {
        TEXT("Primary"),   // RUNE_STAT_PRIMARY
        TEXT("Secondary"), // RUNE_STAT_SECONDARY
        TEXT("Sub1"),      // RUNE_STAT_SUB_1
        TEXT("Sub2"),      // RUNE_STAT_SUB_2
        TEXT("Sub3"),      // RUNE_STAT_SUB_3
        TEXT("Sub4")       // RUNE_STAT_SUB_4
    };
    Assert( iRuneStatistic < RUNE_STAT_COUNT );
    return s_arrConvert[iRuneStatistic];
}
RuneStatistic GameParameters::RuneStatisticFromString( const GChar * strValue ) const
{
    for ( UInt i = 0; i < RUNE_STAT_COUNT; ++i ) {
        RuneStatistic iEnum = (RuneStatistic)i;
        if ( StringFn->Cmp(strValue, RuneStatisticToString(iEnum)) == 0 )
            return iEnum;
    }
    return RUNE_STAT_COUNT;
}

/////////////////////////////////////////////////////////////////////////////////

Void GameParameters::_LoadMonsterParameters( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("MonsterParameters") ) == 0 );

    // Base stats
    const XMLNode * pBaseStatsNode = pNode->GetChildByTag( TEXT("BaseStats"), 0 );
    Assert( pBaseStatsNode != NULL );

    m_fMonsterBaseCritRate = StringFn->ToFloat( pBaseStatsNode->GetAttribute(TEXT("CritRate"))->GetValue() );
    m_fMonsterBaseCritDamage = StringFn->ToFloat( pBaseStatsNode->GetAttribute(TEXT("CritDamage"))->GetValue() );
    m_fMonsterBaseAccuracy = StringFn->ToFloat( pBaseStatsNode->GetAttribute(TEXT("Accuracy"))->GetValue() );
    m_fMonsterBaseResistance = StringFn->ToFloat( pBaseStatsNode->GetAttribute(TEXT("Resistance"))->GetValue() );

    // Awakening bonus
    const XMLNode * pAwakeningBonusNode = pNode->GetChildByTag( TEXT("AwakeningBonus"), 0 );
    Assert( pAwakeningBonusNode != NULL );

    m_iAwakeningBonusBaseSpeed = (UInt)( StringFn->ToUInt( pAwakeningBonusNode->GetAttribute(TEXT("BaseSpeed"))->GetValue() ) );
    m_iAwakeningBonusSpeed = (UInt)( StringFn->ToUInt( pAwakeningBonusNode->GetAttribute(TEXT("Speed"))->GetValue() ) );
    m_fAwakeningBonusCritRate = StringFn->ToFloat( pAwakeningBonusNode->GetAttribute(TEXT("CritRate"))->GetValue() );
    m_fAwakeningBonusAccuracy = StringFn->ToFloat( pAwakeningBonusNode->GetAttribute(TEXT("Accuracy"))->GetValue() );
    m_fAwakeningBonusResistance = StringFn->ToFloat( pAwakeningBonusNode->GetAttribute(TEXT("Resistance"))->GetValue() );

    // XP tables
    const XMLNode * pXPByLevelNode = pNode->GetChildByTag( TEXT("XPByLevel"), 0 );
    Assert( pXPByLevelNode != NULL );
    const XMLNode * pXPFromPowerUpNode = pNode->GetChildByTag( TEXT("XPFromPowerUp"), 0 );
    Assert( pXPFromPowerUpNode != NULL );

    for( UInt i = 0; i < MONSTER_MAX_RANK; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Rank_%d"), i );

        const XMLNode * pXPByLevelRankNode = pXPByLevelNode->GetChildByTag( strNameI, 0 );
        Assert( pXPByLevelRankNode != NULL );
        const XMLNode * pXPFromPowerUpRankNode = pXPFromPowerUpNode->GetChildByTag( strNameI, 0 );
        Assert( pXPFromPowerUpRankNode != NULL );

        for( UInt j = 0; j < MONSTER_MAX_LEVEL; ++j ) {
            GChar strNameJ[16];
            StringFn->Format( strNameJ, TEXT("Level_%d"), i );

            m_arrMonsterXPByLevel[i][j] = (UInt)( StringFn->ToUInt( pXPByLevelRankNode->GetAttribute(strNameJ)->GetValue() ) );
            m_arrMonsterXPFromPowerUp[i][j] = (UInt)( StringFn->ToUInt( pXPFromPowerUpRankNode->GetAttribute(strNameJ)->GetValue() ) );
        }
    }
}
Void GameParameters::_LoadRuneParameters( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("RuneParameters") ) == 0 );

    // Allowed slots per stat
    const XMLNode * pSlotsAllowedNode = pNode->GetChildByTag( TEXT("SlotsAllowed"), 0 );
    Assert( pSlotsAllowedNode != NULL );

    for( UInt i = 0; i < MONSTER_STAT_COUNT; ++i ) {
        const GChar * strNameI = MonsterStatisticToString( (MonsterStatistic)i );

        const XMLNode * pMonsterStatNode = pSlotsAllowedNode->GetChildByTag( strNameI, 0 );
        Assert( pMonsterStatNode != NULL );

        m_arrRuneSlotsAllowed[0][i] = (Byte)( StringFn->ToUInt( pMonsterStatNode->GetAttribute(TEXT("Flat"))->GetValue() ) );
        m_arrRuneSlotsAllowed[1][i] = (Byte)( StringFn->ToUInt( pMonsterStatNode->GetAttribute(TEXT("Ratio"))->GetValue() ) );
    }

    // Rune sets
    const XMLNode * pRuneSetsNode = pNode->GetChildByTag( TEXT("RuneSets"), 0 );
    Assert( pRuneSetsNode != NULL );

    for( UInt i = 0; i < RUNE_TYPE_COUNT; ++i ) {
        const GChar * strNameI = RuneTypeToString( (RuneType)i );

        const XMLNode * pRuneTypeNode = pRuneSetsNode->GetChildByTag( strNameI, 0 );
        Assert( pRuneTypeNode != NULL );

        m_arrRuneSetRequiredCount[i] = (UInt)( StringFn->ToUInt( pRuneTypeNode->GetAttribute(TEXT("RequiredCount"))->GetValue() ) );
        m_arrRuneSetStatBonus[i] = StringFn->ToFloat( pRuneTypeNode->GetAttribute(TEXT("StatBonus"))->GetValue() );
    }

    // Leveling stats
    const XMLNode * pRuneLevelingStatsNode = pNode->GetChildByTag( TEXT("RuneLevelingStats"), 0 );
    Assert( pRuneLevelingStatsNode != NULL );

    m_hRuneLevelingStats.Load( pRuneLevelingStatsNode );
}
Void GameParameters::_LoadTownParameters_BuildingCosts( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("BuildingCosts") ) == 0 );

    // Building costs
    for( UInt i = 0; i < BUILDING_COUNT; ++i ) {
        const GChar * strNameI = BuildingTypeToString( (BuildingType)i );

        const XMLNode * pBuildingTypeNode = pNode->GetChildByTag( strNameI, 0 );
        Assert( pBuildingTypeNode != NULL );

        for( UInt j = 0; j < CURRENCY_COUNT; ++j ) {
            const GChar * strNameJ = CurrencyTypeToString( (CurrencyType)j );
            m_arrBuildingCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pBuildingTypeNode->GetAttribute(strNameJ)->GetValue() ) );
        }
    }
}
Void GameParameters::_LoadTownParameters_UnlockCosts( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("UnlockCosts") ) == 0 );

    // Unlock costs
    const XMLNode * pEssenceFusionNode = pNode->GetChildByTag( TEXT("EssenceFusion"), 0 );
    Assert( pEssenceFusionNode != NULL );
    const XMLNode * pMonsterSummoningNode = pNode->GetChildByTag( TEXT("MonsterSummoning"), 0 );
    Assert( pMonsterSummoningNode != NULL );
    const XMLNode * pMonsterFusionNode = pNode->GetChildByTag( TEXT("MonsterFusion"), 0 );
    Assert( pMonsterFusionNode != NULL );
    const XMLNode * pMonsterPowerUpNode = pNode->GetChildByTag( TEXT("MonsterPowerUp"), 0 );
    Assert( pMonsterPowerUpNode != NULL );
    const XMLNode * pMonsterEvolutionNode = pNode->GetChildByTag( TEXT("MonsterEvolution"), 0 );
    Assert( pMonsterEvolutionNode != NULL );
    const XMLNode * pMonsterAwakeningNode = pNode->GetChildByTag( TEXT("MonsterAwakening"), 0 );
    Assert( pMonsterAwakeningNode != NULL );
    const XMLNode * pRunePowerUpNode = pNode->GetChildByTag( TEXT("RunePowerUp"), 0 );
    Assert( pRunePowerUpNode != NULL );
    const XMLNode * pRuneEvolutionNode = pNode->GetChildByTag( TEXT("RuneEvolution"), 0 );
    Assert( pRuneEvolutionNode != NULL );

    for( UInt i = 0; i < CURRENCY_COUNT; ++i ) {
        const GChar * strNameI = CurrencyTypeToString( (CurrencyType)i );

        m_hEssenceFusionUnlockCost.arrCost[i] = (UInt)( StringFn->ToUInt( pEssenceFusionNode->GetAttribute(strNameI)->GetValue() ) );
        m_hMonsterSummoningUnlockCost.arrCost[i] = (UInt)( StringFn->ToUInt( pMonsterSummoningNode->GetAttribute(strNameI)->GetValue() ) );
        m_hMonsterFusionUnlockCost.arrCost[i] = (UInt)( StringFn->ToUInt( pMonsterFusionNode->GetAttribute(strNameI)->GetValue() ) );
        m_hMonsterPowerUpUnlockCost.arrCost[i] = (UInt)( StringFn->ToUInt( pMonsterPowerUpNode->GetAttribute(strNameI)->GetValue() ) );
        m_hMonsterEvolutionUnlockCost.arrCost[i] = (UInt)( StringFn->ToUInt( pMonsterEvolutionNode->GetAttribute(strNameI)->GetValue() ) );
        m_hMonsterAwakeningUnlockCost.arrCost[i] = (UInt)( StringFn->ToUInt( pMonsterAwakeningNode->GetAttribute(strNameI)->GetValue() ) );
        m_hRunePowerUpUnlockCost.arrCost[i] = (UInt)( StringFn->ToUInt( pRunePowerUpNode->GetAttribute(strNameI)->GetValue() ) );
        m_hRuneEvolutionUnlockCost.arrCost[i] = (UInt)( StringFn->ToUInt( pRuneEvolutionNode->GetAttribute(strNameI)->GetValue() ) );
    }
}
Void GameParameters::_LoadTownParameters_OperationCosts( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("OperationCosts") ) == 0 );

    // Essence fusion
    const XMLNode * pEssenceFusionNode = pNode->GetChildByTag( TEXT("EssenceFusion"), 0 );
    Assert( pEssenceFusionNode != NULL );

    for( UInt i = 0; i < ESSENCE_TYPE_COUNT; ++i ) {
        const GChar * strNameI = EssenceTypeToString( (EssenceType)i );
        m_arrEssenceFusionCost[i] = (UInt)( StringFn->ToUInt( pEssenceFusionNode->GetAttribute(strNameI)->GetValue() ) );
    }

    // Monster & Rune operations
    const XMLNode * pMonsterFusionNode = pNode->GetChildByTag( TEXT("MonsterFusion"), 0 );
    Assert( pMonsterFusionNode != NULL );
    const XMLNode * pMonsterPowerUpNode = pNode->GetChildByTag( TEXT("MonsterPowerUp"), 0 );
    Assert( pMonsterPowerUpNode != NULL );
    const XMLNode * pMonsterEvolutionNode = pNode->GetChildByTag( TEXT("MonsterEvolution"), 0 );
    Assert( pMonsterEvolutionNode != NULL );
    const XMLNode * pRunePowerUpNode = pNode->GetChildByTag( TEXT("RunePowerUp"), 0 );
    Assert( pRunePowerUpNode != NULL );
    const XMLNode * pRuneEvolutionNode = pNode->GetChildByTag( TEXT("RuneEvolution"), 0 );
    Assert( pRuneEvolutionNode != NULL );

    for( UInt i = 0; i < MONSTER_MAX_RANK; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Rank_%d"), i );

        const XMLNode * pMonsterFusionRankNode = pMonsterFusionNode->GetChildByTag( strNameI, 0 );
        Assert( pMonsterFusionRankNode != NULL );
        const XMLNode * pMonsterPowerUpRankNode = pMonsterPowerUpNode->GetChildByTag( strNameI, 0 );
        Assert( pMonsterPowerUpRankNode != NULL );
        const XMLNode * pMonsterEvolutionRankNode = pMonsterEvolutionNode->GetChildByTag( strNameI, 0 );
        Assert( pMonsterEvolutionRankNode != NULL );
        const XMLNode * pRunePowerUpRankNode = pRunePowerUpNode->GetChildByTag( strNameI, 0 );
        Assert( pRunePowerUpRankNode != NULL );
        const XMLNode * pRuneEvolutionRankNode = pRuneEvolutionNode->GetChildByTag( strNameI, 0 );
        Assert( pRuneEvolutionRankNode != NULL );

        for( UInt j = 0; j < CURRENCY_COUNT; ++j ) {
            const GChar * strNameJ = CurrencyTypeToString( (CurrencyType)j );

            m_arrMonsterFusionCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pMonsterFusionRankNode->GetAttribute(strNameJ)->GetValue() ) );
            m_arrMonsterEvolutionCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pMonsterEvolutionRankNode->GetAttribute(strNameJ)->GetValue() ) );
            m_arrRuneEvolutionCosts[i].arrCost[j] = (UInt)( StringFn->ToUInt( pRuneEvolutionRankNode->GetAttribute(strNameJ)->GetValue() ) );
        }

        for( UInt j = 0; j < MONSTER_MAX_RANK; ++j ) {
            GChar strNameJ[16];
            StringFn->Format( strNameJ, TEXT("FoodRank_%d"), j );

            const XMLNode * pMonsterPowerUpFoodRankNode = pMonsterPowerUpRankNode->GetChildByTag( strNameJ, 0 );
            Assert( pMonsterPowerUpFoodRankNode != NULL );
            const XMLNode * pRunePowerUpFoodRankNode = pRunePowerUpRankNode->GetChildByTag( strNameJ, 0 );
            Assert( pRunePowerUpFoodRankNode != NULL );

            for( UInt k = 0; k < CURRENCY_COUNT; ++k ) {
                const GChar * strNameK = CurrencyTypeToString( (CurrencyType)k );

                m_arrMonsterPowerUpCost[i][j].arrCost[k] = (UInt)( StringFn->ToUInt( pMonsterPowerUpFoodRankNode->GetAttribute(strNameK)->GetValue() ) );
                m_arrRunePowerUpCosts[i][j].arrCost[k] = (UInt)( StringFn->ToUInt( pRunePowerUpFoodRankNode->GetAttribute(strNameK)->GetValue() ) );
            }
        }
    }
}
Void GameParameters::_LoadTownParameters_UpgradeCosts( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("UpgradeCosts") ) == 0 );

    // Monster collection
    const XMLNode * pMonsterCollectionNode = pNode->GetChildByTag( TEXT("MonsterCollection"), 0 );
    Assert( pMonsterCollectionNode != NULL );

    for( UInt i = 0; i < MONSTER_COLLECTION_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        const XMLNode * pLevelNode = pMonsterCollectionNode->GetChildByTag( strNameI, 0 );
        Assert( pLevelNode != NULL );

        for( UInt j = 0; j < CURRENCY_COUNT; ++j ) {
            const GChar * strNameJ = CurrencyTypeToString( (CurrencyType)j );
            m_arrMonsterCollectionUpgradeCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pLevelNode->GetAttribute(strNameJ)->GetValue() ) );
        }
    }

    // Monster storage
    const XMLNode * pMonsterStorageNode = pNode->GetChildByTag( TEXT("MonsterStorage"), 0 );
    Assert( pMonsterStorageNode != NULL );

    for( UInt i = 0; i < MONSTER_STORAGE_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        const XMLNode * pLevelNode = pMonsterStorageNode->GetChildByTag( strNameI, 0 );
        Assert( pLevelNode != NULL );

        for( UInt j = 0; j < CURRENCY_COUNT; ++j ) {
            const GChar * strNameJ = CurrencyTypeToString( (CurrencyType)j );
            m_arrMonsterStorageUpgradeCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pLevelNode->GetAttribute(strNameJ)->GetValue() ) );
        }
    }

    // Rune collection
    const XMLNode * pRuneCollectionNode = pNode->GetChildByTag( TEXT("RuneCollection"), 0 );
    Assert( pRuneCollectionNode != NULL );

    for( UInt i = 0; i < RUNE_COLLECTION_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        const XMLNode * pLevelNode = pRuneCollectionNode->GetChildByTag( strNameI, 0 );
        Assert( pLevelNode != NULL );

        for( UInt j = 0; j < CURRENCY_COUNT; ++j ) {
            const GChar * strNameJ = CurrencyTypeToString( (CurrencyType)j );
            m_arrRuneCollectionUpgradeCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pLevelNode->GetAttribute(strNameJ)->GetValue() ) );
        }
    }

    // Rune storage
    const XMLNode * pRuneStorageNode = pNode->GetChildByTag( TEXT("RuneStorage"), 0 );
    Assert( pRuneStorageNode != NULL );

    for( UInt i = 0; i < RUNE_STORAGE_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        const XMLNode * pLevelNode = pRuneStorageNode->GetChildByTag( strNameI, 0 );
        Assert( pLevelNode != NULL );

        for( UInt j = 0; j < CURRENCY_COUNT; ++j ) {
            const GChar * strNameJ = CurrencyTypeToString( (CurrencyType)j );
            m_arrRuneStorageUpgradeCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pLevelNode->GetAttribute(strNameJ)->GetValue() ) );
        }
    }

    // Mana & Crystal
    const XMLNode * pManaProductionRateNode = pNode->GetChildByTag( TEXT("ManaProductionRate"), 0 );
    Assert( pManaProductionRateNode != NULL );
    const XMLNode * pManaCapacityNode = pNode->GetChildByTag( TEXT("ManaCapacity"), 0 );
    Assert( pManaCapacityNode != NULL );
    const XMLNode * pCrystalProductionRateNode = pNode->GetChildByTag( TEXT("CrystalProductionRate"), 0 );
    Assert( pCrystalProductionRateNode != NULL );
    const XMLNode * pCrystalCapacityNode = pNode->GetChildByTag( TEXT("CrystalCapacity"), 0 );
    Assert( pCrystalCapacityNode != NULL );

    for( UInt i = 0; i < BUILDING_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        const XMLNode * pManaProductionRateLevelNode = pManaProductionRateNode->GetChildByTag( strNameI, 0 );
        Assert( pManaProductionRateLevelNode != NULL );
        const XMLNode * pManaCapacityLevelNode = pManaCapacityNode->GetChildByTag( strNameI, 0 );
        Assert( pManaCapacityLevelNode != NULL );
        const XMLNode * pCrystalProductionRateLevelNode = pCrystalProductionRateNode->GetChildByTag( strNameI, 0 );
        Assert( pCrystalProductionRateLevelNode != NULL );
        const XMLNode * pCrystalCapacityLevelNode = pCrystalCapacityNode->GetChildByTag( strNameI, 0 );
        Assert( pCrystalCapacityLevelNode != NULL );


        for( UInt j = 0; j < CURRENCY_COUNT; ++j ) {
            const GChar * strNameJ = CurrencyTypeToString( (CurrencyType)j );

            m_arrManaProductionRateUpgradeCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pManaProductionRateLevelNode->GetAttribute(strNameJ)->GetValue() ) );
            m_arrManaCapacityUpgradeCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pManaCapacityLevelNode->GetAttribute(strNameJ)->GetValue() ) );
            m_arrCrystalProductionRateUpgradeCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pCrystalProductionRateLevelNode->GetAttribute(strNameJ)->GetValue() ) );
            m_arrCrystalCapacityUpgradeCost[i].arrCost[j] = (UInt)( StringFn->ToUInt( pCrystalCapacityLevelNode->GetAttribute(strNameJ)->GetValue() ) );
        }
    }
}
Void GameParameters::_LoadTownParameters_UpgradeParameters( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp( pNode->GetTagName(), TEXT("UpgradeParameters") ) == 0 );

    // monster collection
    const XMLNode * pMonsterCollectionRoomNode = pNode->GetChildByTag( TEXT("MonsterCollectionRoom"), 0 );
    Assert( pMonsterCollectionRoomNode != NULL );

    for( UInt i = 0; i < MONSTER_COLLECTION_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        m_arrMonsterCollectionRoom[i] = (UInt)( StringFn->ToUInt( pMonsterCollectionRoomNode->GetAttribute(strNameI)->GetValue() ) );
    }

    // monster storage
    const XMLNode * pMonsterStorageRoomNode = pNode->GetChildByTag( TEXT("MonsterStorageRoom"), 0 );
    Assert( pMonsterStorageRoomNode != NULL );

    for( UInt i = 0; i < MONSTER_STORAGE_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        m_arrMonsterStorageRoom[i] = (UInt)( StringFn->ToUInt( pMonsterStorageRoomNode->GetAttribute(strNameI)->GetValue() ) );
    }

    // Rune collection
    const XMLNode * pRuneCollectionRoomNode = pNode->GetChildByTag( TEXT("RuneCollectionRoom"), 0 );
    Assert( pRuneCollectionRoomNode != NULL );

    for( UInt i = 0; i < RUNE_COLLECTION_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        m_arrRuneCollectionRoom[i] = (UInt)( StringFn->ToUInt( pRuneCollectionRoomNode->GetAttribute(strNameI)->GetValue() ) );
    }

    // Rune storage
    const XMLNode * pRuneStorageRoomNode = pNode->GetChildByTag( TEXT("RuneStorageRoom"), 0 );
    Assert( pRuneStorageRoomNode != NULL );

    for( UInt i = 0; i < RUNE_STORAGE_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        m_arrRuneStorageRoom[i] = (UInt)( StringFn->ToUInt( pRuneStorageRoomNode->GetAttribute(strNameI)->GetValue() ) );
    }

    // Mana & Crystal
    const XMLNode * pManaProductionRateNode = pNode->GetChildByTag( TEXT("ManaProductionRate"), 0 );
    Assert( pManaProductionRateNode != NULL );
    const XMLNode * pManaCapacityNode = pNode->GetChildByTag( TEXT("ManaCapacity"), 0 );
    Assert( pManaCapacityNode != NULL );
    const XMLNode * pCrystalProductionRateNode = pNode->GetChildByTag( TEXT("CrystalProductionRate"), 0 );
    Assert( pCrystalProductionRateNode != NULL );
    const XMLNode * pCrystalCapacityNode = pNode->GetChildByTag( TEXT("CrystalCapacity"), 0 );
    Assert( pCrystalCapacityNode != NULL );

    for( UInt i = 0; i < BUILDING_MAX_LEVEL; ++i ) {
        GChar strNameI[16];
        StringFn->Format( strNameI, TEXT("Level_%d"), i );

        m_arrManaProductionRate[i] = (UInt)( StringFn->ToUInt( pManaProductionRateNode->GetAttribute(strNameI)->GetValue() ) );
        m_arrManaCapacity[i] = (UInt)( StringFn->ToUInt( pManaCapacityNode->GetAttribute(strNameI)->GetValue() ) );
        m_arrCrystalProductionRate[i] = (UInt)( StringFn->ToUInt( pCrystalProductionRateNode->GetAttribute(strNameI)->GetValue() ) );
        m_arrCrystalCapacity[i] = (UInt)( StringFn->ToUInt( pCrystalCapacityNode->GetAttribute(strNameI)->GetValue() ) );
    }
}

