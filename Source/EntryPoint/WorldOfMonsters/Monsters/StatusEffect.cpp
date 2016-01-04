/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/StatusEffect.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Monsters : Base StatusEffect interface
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
#include "StatusEffect.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// StatusEffect implementation
StatusEffect::StatusEffect( StatusEffectType iType )
{
    m_iType = iType;
}
StatusEffect::~StatusEffect()
{
    // nothing to do
}

Void StatusEffect::OnUpdateBattleStats( MonsterBattleInstance * /*pHost*/ ) const
{
    // Stub method, do nothing
}

Void StatusEffect::OnTurnStart( MonsterBattleInstance * /*pHost*/ ) const
{
    // Stub method, do nothing
}
Void StatusEffect::OnTurnEnd( MonsterBattleInstance * /*pHost*/ ) const
{
    // Stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////

Bool StatusEffect::sm_arrIsStackable[STATUSEFFECT_COUNT] = {
    false, // STATUSEFFECT_BUFF_ATTACK
    false, // STATUSEFFECT_BUFF_DEFENSE
    false, // STATUSEFFECT_BUFF_SPEED
    false, // STATUSEFFECT_BUFF_CRITRATE
    false, // STATUSEFFECT_BUFF_REGEN
    false, // STATUSEFFECT_BUFF_SHIELD
    false, // STATUSEFFECT_BUFF_INVINCIBILITY
    false, // STATUSEFFECT_BUFF_IMMUNITY

    false, // STATUSEFFECT_DEBUFF_ATTACK
    false, // STATUSEFFECT_DEBUFF_DEFENSE
    false, // STATUSEFFECT_DEBUFF_SPEED
    false, // STATUSEFFECT_DEBUFF_MISSRATE
    true,  // STATUSEFFECT_DEBUFF_DOT
    true,  // STATUSEFFECT_DEBUFF_BOMB
    false, // STATUSEFFECT_DEBUFF_MARK_DMG
    false, // STATUSEFFECT_DEBUFF_MARK_DRAIN
    false, // STATUSEFFECT_DEBUFF_NOHEAL
    false, // STATUSEFFECT_DEBUFF_STUN
    false, // STATUSEFFECT_DEBUFF_FREEZE
    false, // STATUSEFFECT_DEBUFF_SLEEP
};

/////////////////////////////////////////////////////////////////////////////////
// StatusBuffAttack implementation
StatusBuffAttack::StatusBuffAttack( Float fAmount ):
    StatusEffect( STATUSEFFECT_BUFF_ATTACK )
{
    m_fAmount = fAmount;
}
StatusBuffAttack::~StatusBuffAttack()
{
    // nothing to do
}

Void StatusBuffAttack::OnUpdateBattleStats( MonsterBattleInstance * pHost ) const
{
    pHost->m_iAttack += (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iAttack)) );
}

/////////////////////////////////////////////////////////////////////////////////
// StatusBuffDefense implementation
StatusBuffDefense::StatusBuffDefense( Float fAmount ):
    StatusEffect( STATUSEFFECT_BUFF_DEFENSE )
{
    m_fAmount = fAmount;
}
StatusBuffDefense::~StatusBuffDefense()
{
    // nothing to do
}

Void StatusBuffDefense::OnUpdateBattleStats( MonsterBattleInstance * pHost ) const
{
    pHost->m_iDefense += (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iDefense)) );
}

/////////////////////////////////////////////////////////////////////////////////
// StatusBuffSpeed implementation
StatusBuffSpeed::StatusBuffSpeed( Float fAmount ):
    StatusEffect( STATUSEFFECT_BUFF_SPEED )
{
    m_fAmount = fAmount;
}
StatusBuffSpeed::~StatusBuffSpeed()
{
    // nothing to do
}

Void StatusBuffSpeed::OnUpdateBattleStats( MonsterBattleInstance * pHost ) const
{
    pHost->m_iSpeed += (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iSpeed)) );
}

/////////////////////////////////////////////////////////////////////////////////
// StatusDebuffAttack implementation
StatusDebuffAttack::StatusDebuffAttack( Float fAmount ):
    StatusEffect( STATUSEFFECT_DEBUFF_ATTACK )
{
    m_fAmount = fAmount;
}
StatusDebuffAttack::~StatusDebuffAttack()
{
    // nothing to do
}

Void StatusDebuffAttack::OnUpdateBattleStats( MonsterBattleInstance * pHost ) const
{
    pHost->m_iAttack -= (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iAttack)) );
}

/////////////////////////////////////////////////////////////////////////////////
// StatusDebuffDefense implementation
StatusDebuffDefense::StatusDebuffDefense( Float fAmount ):
    StatusEffect( STATUSEFFECT_DEBUFF_DEFENSE )
{
    m_fAmount = fAmount;
}
StatusDebuffDefense::~StatusDebuffDefense()
{
    // nothing to do
}

Void StatusDebuffDefense::OnUpdateBattleStats( MonsterBattleInstance * pHost ) const
{
    pHost->m_iDefense -= (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iDefense)) );
}

/////////////////////////////////////////////////////////////////////////////////
// StatusDebuffSpeed implementation
StatusDebuffSpeed::StatusDebuffSpeed( Float fAmount ):
    StatusEffect( STATUSEFFECT_DEBUFF_SPEED )
{
    m_fAmount = fAmount;
}
StatusDebuffSpeed::~StatusDebuffSpeed()
{
    // nothing to do
}

Void StatusDebuffSpeed::OnUpdateBattleStats( MonsterBattleInstance * pHost ) const
{
    pHost->m_iSpeed -= (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iSpeed)) );
}

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectInstance implementation
StatusEffectInstance::StatusEffectInstance()
{
    m_pStatusEffect = NULL;
    m_bRemovable = false;

    m_iMaxStacks = 0;
    m_iStackCount = 0;
    for( UInt i = 0; i < STATUSEFFECT_MAX_STACKS; ++i )
        m_arrDurations[i] = 0;
}
StatusEffectInstance::StatusEffectInstance( StatusEffect * pStatusEffect, Bool bRemovable, UInt iMaxStacks, UInt iStackCount, UInt iDuration )
{
    Assert( pStatusEffect != NULL );
    Assert( iMaxStacks <= STATUSEFFECT_MAX_STACKS );
    Assert( iStackCount <= iMaxStacks );
    Assert( iStackCount > 0 );
    Assert( iDuration > 0 );

    m_pStatusEffect = pStatusEffect;
    m_bRemovable = bRemovable;

    m_iMaxStacks = iMaxStacks;
    m_iStackCount = iStackCount;
    for( UInt i = 0; i < STATUSEFFECT_MAX_STACKS; ++i )
        m_arrDurations[i] = (i < m_iStackCount) ? iDuration : 0;
}
StatusEffectInstance::~StatusEffectInstance()
{
    // nothing to do
}

Void StatusEffectInstance::AddStacks( UInt iStackCount, UInt iDuration )
{
    Assert( IsStackable() );
    Assert( iDuration > 0 );

    if ( m_iStackCount + iStackCount > m_iMaxStacks )
        iStackCount = (m_iMaxStacks - m_iStackCount);

    for( UInt i = 0; i < iStackCount; ++i ) {
        m_arrDurations[m_iStackCount] = iDuration;
        ++m_iStackCount;
    }
}
Bool StatusEffectInstance::RemoveExpiredStacks()
{
    for( UInt i = 0; i < m_iStackCount; ++i ) {
        if ( m_arrDurations[i] == 0 ) {
            m_arrDurations[i] = m_arrDurations[m_iStackCount - 1];
            --m_iStackCount;
            --i;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectSet implementation
StatusEffectSet::StatusEffectSet()
{
    // nothing to do
}
StatusEffectSet::~StatusEffectSet()
{
    // nothing to do
}

Void StatusEffectSet::Add( StatusEffectType iType, Bool bRemovable, UInt iMaxStacks, UInt iStackCount, UInt iDuration )
{
    Assert( iType < STATUSEFFECT_COUNT );
    Assert( iMaxStacks <= STATUSEFFECT_MAX_STACKS );
    Assert( iStackCount <= iMaxStacks );
    Assert( iStackCount > 0 );
    Assert( iDuration > 0 );

    // New effect case
    if ( m_arrEffects[iType] == NULL ) {
        m_arrEffects[iType] = GameplayFn->CreateStatusEffect( iType, bRemovable, iDuration, iStacks );
        return;
    }

    // Grow stack case
    if ( m_arrEffects[iType]->IsStackable() ) {
        m_arrEffects[iType]->AddStacks( iStacks, iDuration );
        return;
    }
    
    // Replace if better case
    if ( m_arrEffects[iType]->IsRemovable() && !bRemovable )
        m_arrEffects[iType]->SetRemovable( false );
    if ( iDuration > m_arrEffects[iType]->GetDuration() )
        m_arrEffects[iType]->SetDuration( iDuration );
}
Void StatusEffectSet::Remove( StatusEffectType iType )
{
    Assert( iType < STATUSEFFECT_COUNT );

    // Nothing to remove or non removable
    if ( m_arrEffects[iType] == NULL || !(m_arrEffects[iType]->IsRemovable()) )
        return;

    if ( m_arrEffects[iType]->IsStackable() ) {
        m_arrEffects[iType]->RemoveStacks( iStacks );
        if ( m_arrEffects[iType]->GetStacks() == 0 ) {
            GameplayFn->DestroyStatusEffect( m_arrEffects[iType] );
            m_arrEffects[iType] = NULL;
        }
    } else {
        GameplayFn->DestroyStatusEffect( m_arrEffects[iType] );
        m_arrEffects[iType] = NULL;
    }
}

