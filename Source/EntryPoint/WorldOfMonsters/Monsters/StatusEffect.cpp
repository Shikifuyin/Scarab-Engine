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

#include "../Battle/Battle.h"

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

/////////////////////////////////////////////////////////////////////////////////

Bool StatusEffect::sm_arrIsStackable[STATUSEFFECT_COUNT] = {
    false, // STATUSEFFECT_BUFF_STAT
    false, // STATUSEFFECT_BUFF_REGEN
    false, // STATUSEFFECT_BUFF_SHIELD
    false, // STATUSEFFECT_BUFF_INVINCIBILITY
    false, // STATUSEFFECT_BUFF_IMMUNITY

    false, // STATUSEFFECT_DEBUFF_STAT
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
// StatusBuffStat implementation
StatusBuffStat::StatusBuffStat( MonsterStatistic iStat, Float fAmount ):
    StatusEffect( STATUSEFFECT_BUFF_STAT )
{
    m_iStat = iStat;
    m_fAmount = fAmount;
}
StatusBuffStat::~StatusBuffStat()
{
    // nothing to do
}

Void StatusBuffStat::OnUpdateBattleStats( MonsterBattleInstance * pHost ) const
{
    switch( m_iStat ) {
        case MONSTER_STAT_HEALTH:
            pHost->m_iHealth += (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iHealth)) );
            break;
        case MONSTER_STAT_ATTACK:
            pHost->m_iAttack += (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iAttack)) );
            break;
        case MONSTER_STAT_DEFENSE:
            pHost->m_iDefense += (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iDefense)) );
            break;
        case MONSTER_STAT_SPEED:
            pHost->m_iSpeed += (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iSpeed)) );
            break;
        case MONSTER_STAT_CRIT_RATE:
            pHost->m_fCriticalRate += m_fAmount;
            if ( pHost->m_fCriticalRate > 1.0f )
                pHost->m_fCriticalRate = 1.0f;
            break;
        case MONSTER_STAT_CRIT_DMG:
            pHost->m_fCriticalDamage += m_fAmount;
            break;
        case MONSTER_STAT_ACCURACY:
            pHost->m_fAccuracy += m_fAmount;
            if ( pHost->m_fAccuracy > 1.0f )
                pHost->m_fAccuracy = 1.0f;
            break;
        case MONSTER_STAT_RESISTANCE:
            pHost->m_fResistance += m_fAmount;
            if ( pHost->m_fResistance > 1.0f )
                pHost->m_fResistance = 1.0f;
            break;
        default: Assert( false ); break;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// StatusDebuffStat implementation
StatusDebuffStat::StatusDebuffStat( MonsterStatistic iStat, Float fAmount ):
    StatusEffect( STATUSEFFECT_DEBUFF_STAT )
{
    m_iStat = iStat;
    m_fAmount = fAmount;
}
StatusDebuffStat::~StatusDebuffStat()
{
    // nothing to do
}

Void StatusDebuffStat::OnUpdateBattleStats( MonsterBattleInstance * pHost ) const
{
    switch( m_iStat ) {
        case MONSTER_STAT_HEALTH:
            pHost->m_iHealth -= (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iHealth)) );
            break;
        case MONSTER_STAT_ATTACK:
            pHost->m_iAttack -= (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iAttack)) );
            break;
        case MONSTER_STAT_DEFENSE:
            pHost->m_iDefense -= (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iDefense)) );
            break;
        case MONSTER_STAT_SPEED:
            pHost->m_iSpeed -= (UInt)( MathFn->Floor(m_fAmount * (Float)(pHost->m_iSpeed)) );
            break;
        case MONSTER_STAT_CRIT_RATE:
            pHost->m_fCriticalRate -= m_fAmount;
            if ( pHost->m_fCriticalRate < 0.0f )
                pHost->m_fCriticalRate = 0.0f;
            break;
        case MONSTER_STAT_CRIT_DMG:
            pHost->m_fCriticalDamage -= m_fAmount;
            if ( pHost->m_fCriticalDamage < 0.0f )
                pHost->m_fCriticalDamage = 0.0f;
            break;
        case MONSTER_STAT_ACCURACY:
            pHost->m_fAccuracy -= m_fAmount;
            if ( pHost->m_fAccuracy < 0.0f )
                pHost->m_fAccuracy = 0.0f;
            break;
        case MONSTER_STAT_RESISTANCE:
            pHost->m_fResistance -= m_fAmount;
            if ( pHost->m_fResistance < 0.0f )
                pHost->m_fResistance = 0.0f;
            break;
        default: Assert( false ); break;
    }
}


/////////////////////////////////////////////////////////////////////////////////
// StatusEffectInstance implementation
StatusEffectInstance::StatusEffectInstance( StatusEffect * pStatusEffect, Bool bRemovable, UInt iDuration )
{
    m_pStatusEffect = pStatusEffect;
    m_bRemovable = bRemovable;

    m_iDuration = iDuration;
}
StatusEffectInstance::~StatusEffectInstance()
{
    // nothing to do
}
