/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Gameplay/Monsters/StatusEffect.h
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPLAY_MONSTERS_STATUSEFFECT_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPLAY_MONSTERS_STATUSEFFECT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Monsters/LevelingStats.h"

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
    StatusEffect( StatusEffectType iType );
    StatusEffect( const StatusEffect & rhs );
    ~StatusEffect();

    // operators
    StatusEffect & operator=( const StatusEffect & rhs );

    // Test for validity
    inline Bool IsNull() const;
    inline Bool IsPresent() const;

    // Type
    inline Bool IsBuff() const;
    inline Bool IsDebuff() const;
    inline Bool IsDisabling() const;

    inline StatusEffectType GetType() const;

    // Stacks
    inline Bool IsStackable() const;
    inline UInt GetMaxStacks() const;
    inline UInt GetStackCount() const;

    Void AddStacks( UInt iStackCount, UInt iDuration, Float fAmplitude );
    Bool RemoveStacks( UInt iStackCount );
    Void RemoveAllStacks();

    Bool UpdateStackDurations();
    Bool RemoveExpiredStacks();

    // Duration
    inline Bool IsExpired( UInt iStack ) const;
    inline UInt GetDuration( UInt iStack ) const;

    inline Void IncreaseDuration( UInt iStack, UInt iAmount );
    inline Void DecreaseDuration( UInt iStack, UInt iAmount );
    inline Void SetDuration( UInt iStack, UInt iAmount );
    inline Void ResetDuration( UInt iStack );

    // Amplitude
    inline Bool IsExhausted( UInt iStack ) const;
    inline Float GetAmplitude( UInt iStack ) const;

    inline Void IncreaseAmplitude( UInt iStack, Float fAmount );
    inline Void DecreaseAmplitude( UInt iStack, Float fAmount );
    inline Void SetAmplitude( UInt iStack, Float fAmount );
    inline Void ResetAmplitude( UInt iStack );

protected:
    // Helpers
    static UInt sm_arrMaxStacks[STATUSEFFECT_COUNT];

    // Type
    StatusEffectType m_iType;

    // Stacks
    UInt m_iStackCount;
    UInt m_arrDurations[STATUSEFFECT_MAX_STACKS];
    Float m_arrAmplitudes[STATUSEFFECT_MAX_STACKS];
};

/////////////////////////////////////////////////////////////////////////////////
// The StatusEffectSet class
class StatusEffectSet
{
public:
    StatusEffectSet();
    ~StatusEffectSet();

    // Status effects access
    inline Bool HasStatusEffect( StatusEffectType iType ) const;
    inline const StatusEffect * GetStatusEffect( StatusEffectType iType ) const;
    inline StatusEffect * GetStatusEffect( StatusEffectType iType );

    Void Add( StatusEffectType iType, UInt iStackCount, UInt iDuration, Float fAmplitude );
    Void Remove( StatusEffectType iType, UInt iStackCount );
    Void RemoveAll();
    
    Void RemoveExpired();

private:
    // Status effects map
    StatusEffect m_arrEffects[STATUSEFFECT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "StatusEffect.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_GAMEPLAY_MONSTERS_STATUSEFFECT_H

