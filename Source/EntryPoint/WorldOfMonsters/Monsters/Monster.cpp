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

/////////////////////////////////////////////////////////////////////////////////
// Monster implementation
Monster::Monster( const GChar * strMonsterFile ):
    m_hLevelingStats(), m_hSkillSet()
{
    // Load everything
    XMLDocument * pMonsterFile = XMLDocument::CreateDocument( strMonsterFile );
    Assert( pMonsterFile != NULL );
    Assert( StringFn->Cmp(pMonsterFile->GetTagName(), TEXT("MonsterFile")) == 0 );

        // Identifier
    m_iMonsterID = (MonsterID)( StringFn->ToUInt( pMonsterFile->GetAttribute(TEXT("MonsterID"))->GetValue() ) );
    StringFn->NCopy( m_strName, pMonsterFile->GetAttribute(TEXT("Name"))->GetValue(), MONSTER_NAME_LENGTH - 1 );
    StringFn->NCopy( m_strAwakenedName, pMonsterFile->GetAttribute(TEXT("AwakenedName"))->GetValue(), MONSTER_NAME_LENGTH - 1 );

        // Leveling stats
    XMLNode * pLevelingStatsNode = pMonsterFile->GetChildByTag( TEXT("MonsterLevelingStats"), 0 );
    Assert( pLevelingStatsNode != NULL );
    m_hLevelingStats.Load( pLevelingStatsNode );

        // Skill set
    XMLNode * pSkillSetNode = pMonsterFile->GetChildByTag( TEXT("MonsterSkillSet"), 0 );
    Assert( pSkillSetNode != NULL );
    m_hSkillSet.Load( pSkillSetNode );

    // Done
    XMLDocument::DestroyDocument( pMonsterFile );
    pMonsterFile = NULL;
}
Monster::~Monster()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

MonsterElement Monster::sm_arrElementWeakAgainst[MONSTER_ELEMENT_COUNT] = {
    MONSTER_ELEMENT_COUNT, // Weak vs MONSTER_ELEMENT_NONE
    MONSTER_ELEMENT_WIND,  // Weak vs MONSTER_ELEMENT_FIRE
    MONSTER_ELEMENT_FIRE,  // Weak vs MONSTER_ELEMENT_WATER
    MONSTER_ELEMENT_WATER, // Weak vs MONSTER_ELEMENT_WIND
    MONSTER_ELEMENT_COUNT, // Weak vs MONSTER_ELEMENT_LIGHT
    MONSTER_ELEMENT_COUNT  // Weak vs MONSTER_ELEMENT_DARK
};
MonsterElement Monster::sm_arrElementStrongAgainst[MONSTER_ELEMENT_COUNT] = {
    MONSTER_ELEMENT_COUNT, // Strong vs MONSTER_ELEMENT_NONE
    MONSTER_ELEMENT_WATER, // Strong vs MONSTER_ELEMENT_FIRE
    MONSTER_ELEMENT_WIND,  // Strong vs MONSTER_ELEMENT_WATER
    MONSTER_ELEMENT_FIRE,  // Strong vs MONSTER_ELEMENT_WIND
    MONSTER_ELEMENT_DARK,  // Strong vs MONSTER_ELEMENT_LIGHT
    MONSTER_ELEMENT_LIGHT  // Strong vs MONSTER_ELEMENT_DARK
};

/////////////////////////////////////////////////////////////////////////////////
// MonsterInstance implementation
MonsterInstance::MonsterInstance( const Monster * pMonster ):
    m_hStats( pMonster->GetLevelingStats() ), m_hSkillSet( pMonster->GetSkillSet() ), m_hRuneSet()
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
    Assert( m_hStats.GetLevel() < MONSTER_MAX_LEVELBYRANK(m_hStats.GetRank()) );
    if ( m_hStats.GetLevel() != iLevel ) {
        m_hStats.SetLevel( iLevel );
        _UpdateEffectiveStats();
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void MonsterInstance::_UpdateEffectiveStats()
{
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

