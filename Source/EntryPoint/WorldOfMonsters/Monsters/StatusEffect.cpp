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

/////////////////////////////////////////////////////////////////////////////////
// StatusEffect implementation
StatusEffect::StatusEffect()
{
    m_iType = STATUSEFFECT_COUNT;

    m_bRemovable = false;
    m_fAmplitude = 0.0f;

    m_iMaxStacks = 0;
    m_iStackCount = 0;
    for( UInt i = 0; i < STATUSEFFECT_MAX_STACKS; ++i )
        m_arrDurations[i] = 0;
}
StatusEffect::StatusEffect( StatusEffectType iType, Bool bRemovable, Float fAmplitude, UInt iMaxStacks, UInt iStackCount, UInt iDuration )
{
    Assert( iType < STATUSEFFECT_COUNT );
    Assert( fAmplitude > 0.0f );
    Assert( iStackCount > 0 );
    Assert( iMaxStacks >= iStackCount );
    Assert( iMaxStacks == 1 || sm_arrIsStackable[iType] );
    Assert( iDuration > 0 );

    m_iType = iType;

    m_bRemovable = bRemovable;
    m_fAmplitude = fAmplitude;

    m_iMaxStacks = iMaxStacks;
    m_iStackCount = iStackCount;
    for( UInt i = 0; i < STATUSEFFECT_MAX_STACKS; ++i )
        m_arrDurations[i] = (i < m_iStackCount) ? iDuration : 0;
}
StatusEffect::StatusEffect( const StatusEffect & rhs )
{
    m_iType = rhs.m_iType;

    m_bRemovable = rhs.m_bRemovable;
    m_fAmplitude = rhs.m_fAmplitude;

    m_iMaxStacks = rhs.m_iMaxStacks;
    m_iStackCount = rhs.m_iStackCount;
    for( UInt i = 0; i < STATUSEFFECT_MAX_STACKS; ++i )
        m_arrDurations[i] = rhs.m_arrDurations[i];
}
StatusEffect::~StatusEffect()
{
    // nothing to do
}

StatusEffect & StatusEffect::operator=( const StatusEffect & rhs )
{
    m_iType = rhs.m_iType;

    m_bRemovable = rhs.m_bRemovable;
    m_fAmplitude = rhs.m_fAmplitude;

    m_iMaxStacks = rhs.m_iMaxStacks;
    m_iStackCount = rhs.m_iStackCount;
    for( UInt i = 0; i < STATUSEFFECT_MAX_STACKS; ++i )
        m_arrDurations[i] = rhs.m_arrDurations[i];

    return (*this);
}

Void StatusEffect::AddStacks( UInt iStackCount, UInt iDuration )
{
    Assert( IsStackable() );

    UInt iFreeSlotCount = ( m_iMaxStacks - m_iStackCount );
    if ( iStackCount > iFreeSlotCount )
        iStackCount = iFreeSlotCount;

    for( UInt i = 0; i < iStackCount; ++i ) {
        m_arrDurations[m_iStackCount] = iDuration;
        ++m_iStackCount;
    }
}
Void StatusEffect::RemoveStacks( UInt iStackCount )
{
    Assert( IsStackable() );

    for ( UInt i = 0; i < iStackCount; ++i ) {
        if ( m_iStackCount == 0 )
            return;
        --m_iStackCount;
        m_arrDurations[m_iStackCount] = 0;
    }
}
Bool StatusEffect::RemoveExpiredStacks()
{
    Bool bAllExpired = true;
    
    for( UInt i = 0; i < m_iStackCount; ++i ) {
        if ( m_arrDurations[i] == 0 ) {
            m_arrDurations[i] = m_arrDurations[m_iStackCount - 1];
            --m_iStackCount;
            --i;
        } else
            bAllExpired = false;
    }

    return bAllExpired;
}

/////////////////////////////////////////////////////////////////////////////////

UInt StatusEffect::sm_arrMaxStacks[STATUSEFFECT_COUNT] = {
    1,                       // STATUSEFFECT_BUFF_ATTACK
    1,                       // STATUSEFFECT_BUFF_DEFENSE
    1,                       // STATUSEFFECT_BUFF_SPEED
    1,                       // STATUSEFFECT_BUFF_CRITRATE
    1,                       // STATUSEFFECT_BUFF_REGEN
    1,                       // STATUSEFFECT_BUFF_SHIELD
    1,                       // STATUSEFFECT_BUFF_INVINCIBILITY
    1,                       // STATUSEFFECT_BUFF_IMMUNITY

    1,                       // STATUSEFFECT_DEBUFF_ATTACK
    1,                       // STATUSEFFECT_DEBUFF_DEFENSE
    1,                       // STATUSEFFECT_DEBUFF_SPEED
    1,                       // STATUSEFFECT_DEBUFF_MISSRATE
    STATUSEFFECT_MAX_STACKS, // STATUSEFFECT_DEBUFF_DOT
    STATUSEFFECT_MAX_STACKS, // STATUSEFFECT_DEBUFF_BOMB
    1,                       // STATUSEFFECT_DEBUFF_MARK_DMG
    1,                       // STATUSEFFECT_DEBUFF_MARK_DRAIN
    1,                       // STATUSEFFECT_DEBUFF_NOHEAL
    1,                       // STATUSEFFECT_DEBUFF_STUN
    1,                       // STATUSEFFECT_DEBUFF_FREEZE
    1,                       // STATUSEFFECT_DEBUFF_SLEEP
};

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

Void StatusEffectSet::Add( StatusEffectType iType, Bool bRemovable, Float fAmplitude, UInt iMaxStacks, UInt iStackCount, UInt iDuration )
{
    Assert( iType < STATUSEFFECT_COUNT );
    Assert( fAmplitude > 0.0f );
    Assert( iStackCount > 0 );
    Assert( iMaxStacks >= iStackCount );
    Assert( iDuration > 0 );

    // New effect case
    if ( m_arrEffects[iType].IsNull() ) {
        m_arrEffects[iType] = StatusEffect( iType, bRemovable, fAmplitude, iMaxStacks, iStackCount, iDuration );
        return;
    }

    // Grow stack case
    if ( m_arrEffects[iType].IsStackable() ) {
        Assert( fAmplitude == m_arrEffects[iType].GetAmplitude() );
        m_arrEffects[iType].AddStacks( iStackCount, iDuration );
        return;
    }

    // Replace if better case
    if ( m_arrEffects[iType].IsRemovable() && !bRemovable )
        m_arrEffects[iType].SetRemovable( false );
    if ( fAmplitude > m_arrEffects[iType].GetAmplitude() )
        m_arrEffects[iType].SetAmplitude( fAmplitude );
    if ( iDuration > m_arrEffects[iType].GetDuration(0) )
        m_arrEffects[iType].SetDuration( 0, iDuration );
}
Void StatusEffectSet::Remove( StatusEffectType iType, UInt iStackCount )
{
    Assert( iType < STATUSEFFECT_COUNT );

    // Nothing to remove or non removable
    if ( m_arrEffects[iType].IsNull() )
        return;

    if ( m_arrEffects[iType].IsStackable() )
        m_arrEffects[iType].RemoveStacks( iStackCount );
    else
        m_arrEffects[iType] = StatusEffect();
}
Void StatusEffectSet::RemoveExpiredStatusEffects()
{
    for( UInt i = 0; i < STATUSEFFECT_COUNT; ++i ) {
        if ( m_arrEffects[i].IsNull() )
            continue;
        Bool bExpired = m_arrEffects[i].RemoveExpiredStacks();
        if ( bExpired )
            m_arrEffects[i] = StatusEffect();
    }
}
Void StatusEffectSet::RemoveAll()
{
    for( UInt i = 0; i < STATUSEFFECT_COUNT; ++i ) {
        if ( m_arrEffects[i].IsNull() )
            continue;
        m_arrEffects[i] = StatusEffect();
    }
}

