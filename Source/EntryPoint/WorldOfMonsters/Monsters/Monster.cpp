/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Monster.cpp
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
// Includes
#include "Monster.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Monster implementation
Monster::Monster( XMLNode * pMonsterNode ):
    m_hLevelingStats()
{
    Assert( pMonsterNode != NULL );
    Assert( StringFn->Cmp( pMonsterNode->GetTagName(), TEXT("Monster") ) == 0 );

    // Load everything
        // Identifier
    m_iMonsterID = (MonsterID)( StringFn->ToUInt( pMonsterNode->GetAttribute(TEXT("MonsterID"))->GetValue() ) );
    StringFn->NCopy( m_strName, pMonsterNode->GetAttribute(TEXT("Name"))->GetValue(), MONSTER_NAME_LENGTH - 1 );
    StringFn->NCopy( m_strAwakenedName, pMonsterNode->GetAttribute(TEXT("AwakenedName"))->GetValue(), MONSTER_NAME_LENGTH - 1 );

        // Leveling stats
    XMLNode * pLevelingStatsNode = pMonsterNode->GetChildByTag( TEXT("LevelingStats"), 0 );
    Assert( pLevelingStatsNode != NULL );
    m_hLevelingStats.Load( pLevelingStatsNode );

        // Skill set
    XMLNode * pSkillSetNode = pMonsterNode->GetChildByTag( TEXT("SkillSet"), 0 );
    Assert( pSkillSetNode != NULL );

    m_iSkillCount = (UInt)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("SkillCount"))->GetValue()) );
    for( UInt i = 0; i < SKILL_SLOT_COUNT; ++i )
        m_arrSkills[i] = INVALID_OFFSET;
    if ( m_iSkillCount > 0 ) {
        m_arrSkills[0] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot0"))->GetValue()) );
        if ( m_iSkillCount > 1 ) {
            m_arrSkills[1] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot1"))->GetValue()) );
            if ( m_iSkillCount > 2 ) {
                m_arrSkills[2] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot2"))->GetValue()) );
                if ( m_iSkillCount > 3 ) {
                    m_arrSkills[3] = (SkillID)( StringFn->ToUInt(pSkillSetNode->GetAttribute(TEXT("Slot3"))->GetValue()) );
                }
            }
        }
    }
}
Monster::~Monster()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// MonsterInstance implementation
MonsterInstance::MonsterInstance( const Monster * pMonster ):
    m_hStats( pMonster->GetLevelingStats() ), m_hSkillSet(), m_hRuneSet()
{
    m_pMonster = pMonster;

    _UpdateEffectiveStats();
}
MonsterInstance::~MonsterInstance()
{
    // nothing to do
}

Void MonsterInstance::Awake()
{
    if ( !(m_hStats.IsAwakened()) ) {
        m_hStats.Awake();
        _UpdateEffectiveStats();
    }
}
Void MonsterInstance::UnAwake()
{
    if ( m_hStats.IsAwakened() ) {
        m_hStats.UnAwake();
        _UpdateEffectiveStats();
    }
}

UInt MonsterInstance::RankUp()
{
    if ( m_hStats.GetRank() < (MONSTER_MAX_RANK - 1) ) {
        m_hStats.RankUp();
        _UpdateEffectiveStats();
    }
    return m_hStats.GetRank();
}
UInt MonsterInstance::RankDown()
{
    if ( m_hStats.GetRank() > 0 ) {
        m_hStats.RankDown();
        _UpdateEffectiveStats();
    }
    return m_hStats.GetRank();
}
Void MonsterInstance::SetRank( UInt iRank )
{
    Assert( iRank < MONSTER_MAX_RANK );
    if ( m_hStats.GetRank() != iRank ) {
        m_hStats.SetRank( iRank );
        _UpdateEffectiveStats();
    }
}

UInt MonsterInstance::LevelUp()
{
    if ( m_hStats.GetLevel() < (MONSTER_MAX_LEVELBYRANK(m_hStats.GetRank()) - 1) ) {
        m_hStats.LevelUp();
        _UpdateEffectiveStats();
    }
    return m_hStats.GetLevel();
}
UInt MonsterInstance::LevelDown()
{
    if ( m_hStats.GetLevel() > 0 ) {
        m_hStats.LevelDown();
        _UpdateEffectiveStats();
    }
    return m_hStats.GetLevel();
}
Void MonsterInstance::SetLevel( UInt iLevel )
{
    Assert( iLevel < MONSTER_MAX_LEVELBYRANK(m_hStats.GetRank()) );
    if ( m_hStats.GetLevel() != iLevel ) {
        m_hStats.SetLevel( iLevel );
        _UpdateEffectiveStats();
    }
}

UInt MonsterInstance::SkillLevelUp( UInt iSlot )
{
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    SkillInstance * pSkillInstance = m_hSkillSet.GetSkillInstance( iSlot );

    if ( pSkillInstance->GetLevel() < (pSkillInstance->GetMaxLevel() - 1) ) {
        pSkillInstance->LevelUp();
        _UpdateEffectiveStats();
    }
    return pSkillInstance->GetLevel();
}
UInt MonsterInstance::SkillLevelDown( UInt iSlot )
{
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    SkillInstance * pSkillInstance = m_hSkillSet.GetSkillInstance( iSlot );

    if ( pSkillInstance->GetLevel() > 0 ) {
        pSkillInstance->LevelDown();
        _UpdateEffectiveStats();
    }
    return pSkillInstance->GetLevel();
}
Void MonsterInstance::SetSkillLevel( UInt iSlot, UInt iLevel )
{
    Assert( iSlot < m_hSkillSet.GetSkillCount() );
    SkillInstance * pSkillInstance = m_hSkillSet.GetSkillInstance( iSlot );

    Assert( iLevel < pSkillInstance->GetMaxLevel() );
    if ( pSkillInstance->GetLevel() != iLevel ) {
        pSkillInstance->SetLevel( iLevel );
        _UpdateEffectiveStats();
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void MonsterInstance::_UpdateEffectiveStats()
{
    // Resolve skill set
    UInt m_arrLevels[SKILL_SLOT_COUNT];
    for( UInt i = 0; i < m_hSkillSet.GetSkillCount(); ++i )
        m_arrLevels[i] = m_hSkillSet.GetSkillInstance(i)->GetLevel();

    m_hSkillSet.RemoveAll();

    for( UInt i = 0; i < m_pMonster->GetSkillCount(); ++i ) {
        Skill * pSkill = GameplayFn->GetSkill( m_pMonster->GetSkill(i) );

        if ( m_hStats.IsAwakened() && pSkill->HasAwakeningUpgrade() )
            pSkill = GameplayFn->GetSkill( pSkill->GetAwakeningUpgrade() );

        if ( pSkill->RequiresAwakening() && !(m_hStats.IsAwakened()) )
            continue;

        m_hSkillSet.Add( pSkill, m_arrLevels[i] );
    }

    // Compile rune stats
    UInt arrFlatBonuses[4];       // HP, ATT, DEF, SPD
    Float arrRatioBonuses[3];     // %HP, %ATT, %DEF
    Float arrSecondaryBonuses[4]; // CritR, CritD, Acc, Res
    m_hRuneSet.CompileStats( arrFlatBonuses, arrRatioBonuses, arrSecondaryBonuses );

    // Compute effective stats
    m_iHealth = ( m_hStats.GetHP() + arrFlatBonuses[0] );
    m_iHealth += (UInt)( MathFn->Floor(arrRatioBonuses[0] * (Float)m_iHealth) );

    m_iAttack = ( m_hStats.GetATT() + arrFlatBonuses[1] );
    m_iAttack += (UInt)( MathFn->Floor(arrRatioBonuses[1] * (Float)m_iAttack) );

    m_iDefense = ( m_hStats.GetDEF() + arrFlatBonuses[2] );
    m_iDefense += (UInt)( MathFn->Floor(arrRatioBonuses[2] * (Float)m_iDefense) );

    m_iSpeed = ( m_hStats.GetSPD() + arrFlatBonuses[3] );
    // flat only !

    m_fCriticalRate = ( m_hStats.GetCritR() + arrSecondaryBonuses[0] );
    if ( m_fCriticalRate > 1.0f )
        m_fCriticalRate = 1.0f;

    m_fCriticalDamage = ( m_hStats.GetCritD() + arrSecondaryBonuses[1] );
    // no cap !

    m_fAccuracy = ( m_hStats.GetACC() + arrSecondaryBonuses[2] );
    if ( m_fAccuracy > 1.0f )
        m_fAccuracy = 1.0f;

    m_fResistance = ( m_hStats.GetRES() + arrSecondaryBonuses[3] );
    if ( m_fResistance > 1.0f )
        m_fResistance = 1.0f;
}

