/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Gameplay/Monsters/StatusEffect.cpp
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

    m_iStackCount = 0;
    for( UInt i = 0; i < STATUSEFFECT_MAX_STACKS; ++i ) {
        m_arrDurations[i] = 0;
        m_arrAmplitudes[i] = 0.0f;
    }
}
StatusEffect::StatusEffect( StatusEffectType iType )
{
    Assert( iType < STATUSEFFECT_COUNT );

    m_iType = iType;

    m_iStackCount = 0;
    for( UInt i = 0; i < STATUSEFFECT_MAX_STACKS; ++i ) {
        m_arrDurations[i] = 0;
        m_arrAmplitudes[i] = 0.0f;
    }
}
StatusEffect::StatusEffect( const StatusEffect & rhs )
{
    m_iType = rhs.m_iType;

    m_iStackCount = rhs.m_iStackCount;
    for( UInt i = 0; i < STATUSEFFECT_MAX_STACKS; ++i ) {
        m_arrDurations[i] = rhs.m_arrDurations[i];
        m_arrAmplitudes[i] = rhs.m_arrAmplitudes[i];
    }
}
StatusEffect::~StatusEffect()
{
    // nothing to do
}

StatusEffect & StatusEffect::operator=( const StatusEffect & rhs )
{
    m_iType = rhs.m_iType;

    m_iStackCount = rhs.m_iStackCount;
    for( UInt i = 0; i < STATUSEFFECT_MAX_STACKS; ++i ) {
        m_arrDurations[i] = rhs.m_arrDurations[i];
        m_arrAmplitudes[i] = rhs.m_arrAmplitudes[i];
    }

    return (*this);
}

Void StatusEffect::AddStacks( UInt iStackCount, UInt iDuration, Float fAmplitude )
{
    Assert( m_iStackCount < sm_arrMaxStacks[m_iType] );

    UInt iFreeSlotCount = ( sm_arrMaxStacks[m_iType] - m_iStackCount );
    if ( iStackCount > iFreeSlotCount )
        iStackCount = iFreeSlotCount;

    for( UInt i = 0; i < iStackCount; ++i ) {
        m_arrDurations[m_iStackCount] = iDuration;
        m_arrAmplitudes[m_iStackCount] = fAmplitude;
        ++m_iStackCount;
    }
}
Bool StatusEffect::RemoveStacks( UInt iStackCount )
{
    Assert( iStackCount <= m_iStackCount );

    for ( UInt i = 0; i < iStackCount; ++i ) {
        m_arrDurations[m_iStackCount - 1 - i] = 0;
        m_arrAmplitudes[m_iStackCount - 1 - i] = 0.0f;
    }
    m_iStackCount -= iStackCount;
    return ( m_iStackCount == 0 );
}
Void StatusEffect::RemoveAllStacks()
{
    for ( UInt i = 0; i < m_iStackCount; ++i ) {
        m_arrDurations[i] = 0;
        m_arrAmplitudes[i] = 0.0f;
    }
    m_iStackCount = 0;
}

Bool StatusEffect::UpdateStackDurations()
{
    Bool bSomeExpired = false;

    for ( UInt i = 0; i < m_iStackCount; ++i ) {
        if ( m_arrDurations[i] > 0 ) {
            --(m_arrDurations[i]);
            if ( m_arrDurations[i] == 0 )
                bSomeExpired = true;
        }
    }

    return bSomeExpired;
}
Bool StatusEffect::RemoveExpiredStacks()
{
    Bool bAllExpired = true;
    
    for( UInt i = 0; i < m_iStackCount; ++i ) {
        if ( m_arrDurations[i] == 0 ) {
            m_arrDurations[i] = m_arrDurations[m_iStackCount - 1];
            m_arrAmplitudes[i] = m_arrAmplitudes[m_iStackCount - 1];
            m_arrDurations[m_iStackCount - 1] = 0;
            m_arrAmplitudes[m_iStackCount - 1] = 0.0f;
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

Void StatusEffectSet::Add( StatusEffectType iType, UInt iStackCount, UInt iDuration, Float fAmplitude )
{
    Assert( iType < STATUSEFFECT_COUNT );
    Assert( iStackCount > 0 );
    Assert( fAmplitude > 0.0f );
    Assert( iDuration > 0 );

    // New effect case
    if ( m_arrEffects[iType].IsNull() ) {
        m_arrEffects[iType] = StatusEffect( iType );
        m_arrEffects[iType].AddStacks( iStackCount, iDuration, fAmplitude );
        return;
    }

    // Grow stack case
    if ( m_arrEffects[iType].IsStackable() ) {
        m_arrEffects[iType].AddStacks( iStackCount, iDuration, fAmplitude );
        return;
    }

    // Replace if better case
    if ( iDuration > m_arrEffects[iType].GetDuration(0) || fAmplitude > m_arrEffects[iType].GetAmplitude(0) ) {
        m_arrEffects[iType].SetDuration( 0, iDuration );
        m_arrEffects[iType].SetAmplitude( 0, fAmplitude );
    }
}
Void StatusEffectSet::Remove( StatusEffectType iType, UInt iStackCount )
{
    Assert( iType < STATUSEFFECT_COUNT );

    // Nothing to remove or non removable
    if ( m_arrEffects[iType].IsNull() )
        return;

    if ( m_arrEffects[iType].IsStackable() ) {
        Bool bExpired = m_arrEffects[iType].RemoveStacks( iStackCount );
        if ( bExpired )
            m_arrEffects[iType] = StatusEffect();
    } else
        m_arrEffects[iType] = StatusEffect();
}
Void StatusEffectSet::RemoveAll()
{
    for( UInt i = 0; i < STATUSEFFECT_COUNT; ++i ) {
        if ( m_arrEffects[i].IsNull() )
            continue;
        m_arrEffects[i] = StatusEffect();
    }
}

Void StatusEffectSet::RemoveExpired()
{
    for( UInt i = 0; i < STATUSEFFECT_COUNT; ++i ) {
        if ( m_arrEffects[i].IsNull() )
            continue;
        Bool bExpired = m_arrEffects[i].RemoveExpiredStacks();
        if ( bExpired )
            m_arrEffects[i] = StatusEffect();
    }
}


