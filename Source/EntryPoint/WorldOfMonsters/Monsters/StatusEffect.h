/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Monsters/StatusEffect.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_STATUSEFFECT_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_STATUSEFFECT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Statistics.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Status effects list
enum StatusEffectType {
    STATUSEFFECT_BUFF_ATTACK = 0,
    STATUSEFFECT_BUFF_DEFENSE,
    STATUSEFFECT_BUFF_SPEED,
    STATUSEFFECT_BUFF_CRITRATE,
    STATUSEFFECT_BUFF_REGEN,
    STATUSEFFECT_BUFF_SHIELD,
    STATUSEFFECT_BUFF_INVINCIBILITY,
    STATUSEFFECT_BUFF_IMMUNITY,

    STATUSEFFECT_DEBUFF_ATTACK,
    STATUSEFFECT_DEBUFF_DEFENSE,
    STATUSEFFECT_DEBUFF_SPEED,
    STATUSEFFECT_DEBUFF_CRITRATE,
    STATUSEFFECT_DEBUFF_DESTROY,
    STATUSEFFECT_DEBUFF_DOT,
    STATUSEFFECT_DEBUFF_BOMB,
    STATUSEFFECT_DEBUFF_MARK_DMG,
    STATUSEFFECT_DEBUFF_MARK_DRAIN,
    STATUSEFFECT_DEBUFF_NOHEAL,
    STATUSEFFECT_DEBUFF_STUN,
    STATUSEFFECT_DEBUFF_FREEZE,
    STATUSEFFECT_DEBUFF_SLEEP,

    STATUSEFFECT_COUNT
};

// Stacking
#define STATUSEFFECT_MAX_STACKS 64 // MORE than enough ... (if a player ever overflows this with legit gameplay ... WOW !!!)

/////////////////////////////////////////////////////////////////////////////////
// The StatusEffect class
class StatusEffect
{
public:
    StatusEffect();
    StatusEffect( StatusEffectType iType, Bool bRemovable, Float fAmplitude, UInt iMaxStacks, UInt iStackCount, UInt iDuration );
    StatusEffect( const StatusEffect & rhs );
    ~StatusEffect();

    // operators
    StatusEffect & operator=( const StatusEffect & rhs );

    // Test for validity
    inline Bool IsNull() const;
    inline Bool IsPresent() const;

    // Type access
    inline Bool IsBuff() const;
    inline Bool IsDebuff() const;
    inline Bool IsDisabling() const;

    inline StatusEffectType GetType() const;

    // Properties access
    inline Bool IsRemovable() const;
    inline Float GetAmplitude() const;

    inline Void SetRemovable( Bool bRemovable );
    inline Void SetAmplitude( Float fAmplitude );

    // Stacks access
    inline Bool IsStackable() const;
    inline UInt GetMaxStacks() const;
    inline UInt GetStackCount() const;

    inline Void SetMaxStacks( UInt iMaxStacks );

    Void AddStacks( UInt iStackCount, UInt iDuration );
    Void RemoveStacks( UInt iStackCount );
    Bool RemoveExpiredStacks();

    // Durations access
    inline Bool IsExpired( UInt iStack ) const;
    inline UInt GetDuration( UInt iStack ) const;

    inline Void IncreaseDuration( UInt iStack, UInt iAmount );
    inline Void DecreaseDuration( UInt iStack, UInt iAmount );
    inline Void SetDuration( UInt iStack, UInt iAmount );
    inline Void ResetDuration( UInt iStack );

protected:
    // Helpers
    static Bool sm_arrIsStackable[STATUSEFFECT_COUNT];

    // Type
    StatusEffectType m_iType;

    // Properties
    Bool m_bRemovable;
    Float m_fAmplitude;

    // Stacks & durations
    UInt m_iMaxStacks;
    UInt m_iStackCount;
    UInt m_arrDurations[STATUSEFFECT_MAX_STACKS];
};

/////////////////////////////////////////////////////////////////////////////////
// The StatusEffectSet class
class StatusEffectSet
{
public:
    StatusEffectSet();
    ~StatusEffectSet();

    inline Bool HasStatusEffect( StatusEffectType iType ) const;

    inline StatusEffect * GetStatusEffect( StatusEffectType iType );

    Void Add( StatusEffectType iType, Bool bRemovable, Float fAmplitude, UInt iMaxStacks, UInt iStackCount, UInt iDuration );
    Void Remove( StatusEffectType iType, UInt iStackCount );
    Void RemoveExpiredStatusEffects();
    Void RemoveAll();
    
private:
    // Status effect map
    StatusEffect m_arrEffects[STATUSEFFECT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "StatusEffect.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_STATUSEFFECT_H

