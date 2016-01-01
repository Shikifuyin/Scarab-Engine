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
#include "../../Framework/Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Status effects list
enum StatusEffectType {
    STATUSEFFECT_BUFF_STAT = 0,
    STATUSEFFECT_BUFF_REGEN,
    STATUSEFFECT_BUFF_SHIELD,
    STATUSEFFECT_BUFF_INVINCIBILITY,
    STATUSEFFECT_BUFF_IMMUNITY,

    STATUSEFFECT_DEBUFF_STAT,
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

    inline Bool IsRemovable() const;
    inline Bool IsStackable() const;

    // Interface
    virtual Void ApplyEffect( MonsterBattleInstance * pHost ) const = 0;

protected:
    // Helpers
    static Bool sm_arrIsStackable[STATUSEFFECT_COUNT];

    // Type
    StatusEffectType m_iType;
};

/////////////////////////////////////////////////////////////////////////////////
// The StatusBuffStat class
class StatusBuffStat : public StatusEffect
{
public:
    StatusBuffStat( MonsterStatistic iStat, UInt iAmount );
    StatusBuffStat( MonsterStatistic iStat, Float fAmount );
    virtual ~StatusBuffStat();

    // Interface
    inline MonsterStatistic GetStat() const;
    inline UInt GetAmountI() const;
    inline Float GetAmountF() const;

    virtual Void ApplyEffect( MonsterBattleInstance * pHost ) const;

protected:
    MonsterStatistic m_iStat;
    union {
        UInt iValue;
        Float fValue;
    } m_hAmount;
};









/////////////////////////////////////////////////////////////////////////////////
// The StatusEffectInstance class
class StatusEffectInstance
{
public:
    StatusEffectInstance( StatusEffect * pStatusEffect, UInt iDuration );
    ~StatusEffectInstance();

protected:
    StatusEffect * m_pStatusEffect;
    UInt m_iDuration;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "StatusEffect.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_STATUSEFFECT_H

