/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/Skill.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Base Skill interface
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
#include "Skill.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Skill implementation
Skill::Skill():
    m_hLevelingStats()
{
    m_iSkillID = INVALID_OFFSET;
    m_strName[0] = NULLBYTE;
}
Skill::~Skill()
{
    // nothing to do
}

Skill * Skill::StaticLoad( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("Skill")) == 0 );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    SkillType iType = pGameParams->SkillTypeFromString( pNode->GetAttribute(TEXT("Type"))->GetValue() );
    Skill * pSkill = NULL;

    GameplayFn->SelectMemory( TEXT("Scratch") );
    switch( iType ) {
        case SKILL_TYPE_ACTIVE:  pSkill = New ActiveSkill();  break;
        case SKILL_TYPE_PASSIVE: pSkill = New PassiveSkill(); break;
        case SKILL_TYPE_LEADER:  pSkill = New LeaderSkill();  break;
        default: Assert( false ); break;
    }
    GameplayFn->UnSelectMemory();

    pSkill->Load( pNode );

    return pSkill;
}
Void Skill::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("Skill")) == 0 );

    m_iSkillID = (SkillID)( StringFn->ToUInt(pNode->GetAttribute(TEXT("SkillID"))->GetValue()) );
    StringFn->NCopy( m_strName, pNode->GetAttribute(TEXT("Name"))->GetValue(), SKILL_NAME_LENGTH - 1 );

    const XMLNode * pLevelingStatsNode = pNode->GetChildByTag( TEXT("SkillLevelingStats"), 0 );
    Assert( pLevelingStatsNode != NULL );
    m_hLevelingStats.Load( pLevelingStatsNode );
}

/////////////////////////////////////////////////////////////////////////////////
// ActiveSkill implementation
ActiveSkill::ActiveSkill():
    Skill()
{
    m_bIsAttack = false;

    for( UInt i = 0; i < SKILL_ACTIVE_COUNT; ++i ) {
        m_arrEffectCounts[i] = 0;
        for( UInt j = 0; j < SKILL_MAX_EFFECTS; ++j )
            m_arrEffects[i][j] = NULL;
    }
}
ActiveSkill::~ActiveSkill()
{
    // nothing to do
}

Void ActiveSkill::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("Skill")) == 0 );

    Skill::Load( pNode );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    m_bIsAttack = ( StringFn->ToUInt(pNode->GetAttribute(TEXT("IsAttack"))->GetValue()) != 0 );

    for( UInt i = 0; i < SKILL_ACTIVE_COUNT; ++i ) {
        m_arrEffectCounts[i] = 0;
        for( UInt j = 0; j < SKILL_MAX_EFFECTS; ++j )
            m_arrEffects[i][j] = NULL;
    }

    UInt iCount = pNode->GetChildCount();
    for( UInt i = 0; i < iCount; ++i ) {
        const XMLNode * pActiveEffectNode = pNode->GetChildByTag( TEXT("ActiveEffect"), i );
        Assert( pActiveEffectNode != NULL );

        SkillActiveType iActiveType = pGameParams->SkillActiveTypeFromString( pActiveEffectNode->GetAttribute(TEXT("ActiveType"))->GetValue() );

        const XMLNode * pSkillEffectNode = pActiveEffectNode->GetChildByTag( TEXT("SkillEffect"), 0 );
        Assert( pSkillEffectNode != NULL );

        SkillEffect * pSkillEffect = SkillEffect::StaticLoad( pSkillEffectNode );
        Assert( pSkillEffect != NULL );

        m_arrEffects[iActiveType][m_arrEffectCounts[iActiveType]] = pSkillEffect;
        ++(m_arrEffectCounts[iActiveType]);
    }
}

/////////////////////////////////////////////////////////////////////////////////
// PassiveSkill implementation
PassiveSkill::PassiveSkill():
    Skill()
{
    for( UInt i = 0; i < SKILL_ACTIVE_COUNT; ++i ) {
        m_arrEffectCounts[i] = 0;
        for( UInt j = 0; j < SKILL_MAX_EFFECTS; ++j )
            m_arrEffects[i][j] = NULL;
    }
}
PassiveSkill::~PassiveSkill()
{
    // nothing to do
}

Void PassiveSkill::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("Skill")) == 0 );

    Skill::Load( pNode );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    for( UInt i = 0; i < SKILL_ACTIVE_COUNT; ++i ) {
        m_arrEffectCounts[i] = 0;
        for( UInt j = 0; j < SKILL_MAX_EFFECTS; ++j )
            m_arrEffects[i][j] = NULL;
    }

    UInt iCount = pNode->GetChildCount();
    for( UInt i = 0; i < iCount; ++i ) {
        const XMLNode * pPassiveEffectNode = pNode->GetChildByTag( TEXT("PassiveEffect"), i );
        Assert( pPassiveEffectNode != NULL );

        SkillPassiveType iPassiveType = pGameParams->SkillPassiveTypeFromString( pPassiveEffectNode->GetAttribute(TEXT("PassiveType"))->GetValue() );

        const XMLNode * pSkillEffectNode = pPassiveEffectNode->GetChildByTag( TEXT("SkillEffect"), 0 );
        Assert( pSkillEffectNode != NULL );

        SkillEffect * pSkillEffect = SkillEffect::StaticLoad( pSkillEffectNode );
        Assert( pSkillEffect != NULL );

        m_arrEffects[iPassiveType][m_arrEffectCounts[iPassiveType]] = pSkillEffect;
        ++(m_arrEffectCounts[iPassiveType]);
    }
}

/////////////////////////////////////////////////////////////////////////////////
// LeaderSkill implementation
LeaderSkill::LeaderSkill():
    Skill()
{
    m_iBonusStat = MONSTER_STAT_COUNT;
    m_fBonusAmount = 0.0f;

    m_iConstraint = SKILL_LEADERCONSTRAINT_NONE;
}
LeaderSkill::~LeaderSkill()
{
    // nothing to do
}

Void LeaderSkill::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("Skill")) == 0 );

    Skill::Load( pNode );

    const GameParameters * pGameParams = GameplayFn->GetGameParameters();

    m_iBonusStat = pGameParams->MonsterStatisticFromString( pNode->GetAttribute(TEXT("LeaderBonusStat"))->GetValue() );
    Assert( m_iBonusStat < MONSTER_STAT_COUNT );

    m_fBonusAmount = StringFn->ToFloat( pNode->GetAttribute(TEXT("LeaderBonusAmount"))->GetValue() );

    m_iConstraint = pGameParams->SkillLeaderConstraintFromString( pNode->GetAttribute(TEXT("LeaderConstraint"))->GetValue() );
    Assert( m_iConstraint < SKILL_LEADERCONSTRAINT_COUNT );
}

/////////////////////////////////////////////////////////////////////////////////
// SkillInstance implementation
SkillInstance::SkillInstance()
{
    m_pSkill = NULL;

    m_iLevel = 0;
}
SkillInstance::SkillInstance( Skill * pSkill )
{
    Assert( pSkill != NULL );

    m_pSkill = pSkill;

    m_iLevel = 0;
}
SkillInstance::SkillInstance( const SkillInstance & rhs )
{
    m_pSkill = rhs.m_pSkill;

    m_iLevel = rhs.m_iLevel;
}
SkillInstance::~SkillInstance()
{
    // nothing to do
}

SkillInstance & SkillInstance::operator=( const SkillInstance & rhs )
{
    m_pSkill = rhs.m_pSkill;

    m_iLevel = rhs.m_iLevel;

    return (*this);
}

Void SkillInstance::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("SkillInstance")) == 0 );

    SkillID iSkillID = (SkillID)( StringFn->ToUInt(pNode->GetAttribute(TEXT("SkillID"))->GetValue()) );
    m_pSkill = GameplayFn->GetSkill( iSkillID );
    Assert( m_pSkill != NULL );

    m_iLevel = (UInt)( StringFn->ToUInt(pNode->GetAttribute(TEXT("Level"))->GetValue()) );
    Assert( m_iLevel < m_pSkill->GetLevelingStats()->GetMaxLevel() );
}
Void SkillInstance::Save( XMLNode * outNode ) const
{
    Assert( outNode != NULL );
    Assert( StringFn->Cmp(outNode->GetTagName(), TEXT("SkillInstance")) == 0 );

    GChar strBuffer[1024];

    StringFn->FromUInt( strBuffer, m_pSkill->GetID() );
    outNode->CreateAttribute( TEXT("SkillID"), strBuffer );
    StringFn->FromUInt( strBuffer, m_iLevel );
    outNode->CreateAttribute( TEXT("Level"), strBuffer );
}

/////////////////////////////////////////////////////////////////////////////////
// SkillSet implementation
SkillSet::SkillSet()
{
    m_iSkillCount = 0;
}
SkillSet::~SkillSet()
{
    // nothing to do
}

Void SkillSet::Load( const XMLNode * pNode )
{
    Assert( pNode != NULL );
    Assert( StringFn->Cmp(pNode->GetTagName(), TEXT("SkillSet")) == 0 );

    m_iSkillCount = pNode->GetChildCount();
    for ( UInt i = 0; i < m_iSkillCount; ++i ) {
        const XMLNode * pSkillInstanceNode = pNode->GetChildByTag( TEXT("SkillInstance"), i );
        Assert( pSkillInstanceNode != NULL );

        m_arrSkills[i].Load( pSkillInstanceNode );
        Assert( m_arrSkills[i].IsPresent() );
    }
}
Void SkillSet::Save( XMLNode * outNode ) const
{
    Assert( outNode != NULL );
    Assert( StringFn->Cmp(outNode->GetTagName(), TEXT("SkillSet")) == 0 );

    for( UInt i = 0; i < m_iSkillCount; ++i ) {
        XMLNode * pSkillInstanceNode = XMLDocument::CreateNode( TEXT("SkillInstance"), true );

        Assert( m_arrSkills[i].IsPresent() );
        m_arrSkills[i].Save( pSkillInstanceNode );

        outNode->AppendChild( pSkillInstanceNode );
    }
}

SkillInstance * SkillSet::AddSkill( SkillID iSkillID, UInt iLevel )
{
    Skill * pSkill = GameplayFn->GetSkill( iSkillID );
    Assert( pSkill != NULL );

    Assert( iLevel < pSkill->GetLevelingStats()->GetMaxLevel() );
    Assert( m_iSkillCount < SKILL_SLOT_COUNT );

    m_arrSkills[m_iSkillCount] = SkillInstance( pSkill );
    m_arrSkills[m_iSkillCount].SetLevel( iLevel );
    ++m_iSkillCount;

    return ( m_arrSkills + m_iSkillCount - 1 );
}
Void SkillSet::RemoveAll()
{
    for( UInt i = 0; i < m_iSkillCount; ++i )
        m_arrSkills[i] = SkillInstance();

    m_iSkillCount = 0;
}

