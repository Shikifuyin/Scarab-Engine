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
    STATUSEFFECT_DEBUFF_MISSRATE,
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

// Prototypes
class MonsterBattleInstance;

/////////////////////////////////////////////////////////////////////////////////
// The StatusEffect class
class StatusEffect
{
public:
    StatusEffect( StatusEffectType iType );
    virtual ~StatusEffect();

    // Getters
    inline StatusEffectType GetType() const;

    inline Bool IsBuff() const;
    inline Bool IsDebuff() const;
    inline Bool IsDisabling() const;

    inline Bool IsStackable() const;

    // Interface
    virtual Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

    virtual Void OnTurnStart( MonsterBattleInstance * pHost ) const;
    virtual Void OnTurnEnd( MonsterBattleInstance * pHost ) const;

protected:
    // Helpers
    static Bool sm_arrIsStackable[STATUSEFFECT_COUNT];

    // Type
    StatusEffectType m_iType;
};

/////////////////////////////////////////////////////////////////////////////////
// The StatusBuffAttack class
class StatusBuffAttack : public StatusEffect
{
public:
    StatusBuffAttack( Float fAmount );
    virtual ~StatusBuffAttack();

    // Interface
    inline Float GetAmount() const;

    virtual Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

protected:
    Float m_fAmount;
};

/////////////////////////////////////////////////////////////////////////////////
// The StatusBuffDefense class
class StatusBuffDefense : public StatusEffect
{
public:
    StatusBuffDefense( Float fAmount );
    virtual ~StatusBuffDefense();

    // Interface
    inline Float GetAmount() const;

    virtual Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

protected:
    Float m_fAmount;
};

/////////////////////////////////////////////////////////////////////////////////
// The StatusBuffSpeed class
class StatusBuffSpeed : public StatusEffect
{
public:
    StatusBuffSpeed( Float fAmount );
    virtual ~StatusBuffSpeed();

    // Interface
    inline Float GetAmount() const;

    virtual Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

protected:
    Float m_fAmount;
};



/////////////////////////////////////////////////////////////////////////////////
// The StatusDebuffAttack class
class StatusDebuffAttack : public StatusEffect
{
public:
    StatusDebuffAttack( Float fAmount );
    virtual ~StatusDebuffAttack();

    // Interface
    inline Float GetAmount() const;

    virtual Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

protected:
    Float m_fAmount;
};

/////////////////////////////////////////////////////////////////////////////////
// The StatusDebuffDefense class
class StatusDebuffDefense : public StatusEffect
{
public:
    StatusDebuffDefense( Float fAmount );
    virtual ~StatusDebuffDefense();

    // Interface
    inline Float GetAmount() const;

    virtual Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

protected:
    Float m_fAmount;
};

/////////////////////////////////////////////////////////////////////////////////
// The StatusDebuffSpeed class
class StatusDebuffSpeed : public StatusEffect
{
public:
    StatusDebuffSpeed( Float fAmount );
    virtual ~StatusDebuffSpeed();

    // Interface
    inline Float GetAmount() const;

    virtual Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

protected:
    Float m_fAmount;
};




/////////////////////////////////////////////////////////////////////////////////
// The StatusEffectInstance class
class StatusEffectInstance
{
public:
    StatusEffectInstance();
    StatusEffectInstance( StatusEffect * pStatusEffect, Bool bRemovable, UInt iMaxStacks, UInt iStackCount, UInt iDuration );
    ~StatusEffectInstance();

    // Base class access
    inline StatusEffect * GetStatusEffect() const;

    // Getters
    inline StatusEffectType GetType() const;

    inline Bool IsBuff() const;
    inline Bool IsDebuff() const;
    inline Bool IsDisabling() const;

    inline Bool IsStackable() const;
    inline Bool IsRemovable() const;

    // Stacks access
    inline UInt GetMaxStacks() const;
    inline UInt GetStackCount() const;

    inline Bool IsExpired( UInt iStack ) const;
    inline UInt GetDuration( UInt iStack ) const;

    inline Void IncreaseDuration( UInt iStack, UInt iAmount );
    inline Void DecreaseDuration( UInt iStack, UInt iAmount );
    inline Void SetDuration( UInt iStack, UInt iAmount );

    Void AddStacks( UInt iStackCount, UInt iDuration );
    Bool RemoveExpiredStacks();

    // Interface
    inline Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

    inline Void OnTurnStart( MonsterBattleInstance * pHost ) const;
    inline Void OnTurnEnd( MonsterBattleInstance * pHost ) const;

protected:
    StatusEffect * m_pStatusEffect;
    Bool m_bRemovable;

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

    inline StatusEffectInstance * GetStatusEffect( StatusEffectType iType );

    Void Add( StatusEffectType iType, Bool bRemovable, UInt iMaxStacks, UInt iStackCount, UInt iDuration );
    Void Remove( StatusEffectType iType );

private:
    // Status effect map
    StatusEffectInstance m_arrEffects[STATUSEFFECT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "StatusEffect.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_STATUSEFFECT_H

