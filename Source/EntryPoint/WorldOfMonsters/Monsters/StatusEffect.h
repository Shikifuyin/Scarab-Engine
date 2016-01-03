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
    inline Bool IsDisabling() const;

    inline Bool IsStackable() const;

    // Interface
    virtual Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

    virtual Void OnTurnStart( MonsterBattleInstance * pHost ) const;
    //virtual Void OnTurnEnd( MonsterBattleInstance * pHost ) const;


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
    StatusBuffStat( MonsterStatistic iStat, Float fAmount );
    virtual ~StatusBuffStat();

    // Interface
    inline MonsterStatistic GetStat() const;
    inline Float GetAmount() const;

    virtual Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

protected:
    MonsterStatistic m_iStat;
    Float m_fAmount;
};





/////////////////////////////////////////////////////////////////////////////////
// The StatusDebuffStat class
class StatusDebuffStat : public StatusEffect
{
public:
    StatusDebuffStat( MonsterStatistic iStat, Float fAmount );
    virtual ~StatusDebuffStat();

    // Interface
    inline MonsterStatistic GetStat() const;
    inline Float GetAmount() const;

    virtual Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

protected:
    MonsterStatistic m_iStat;
    Float m_fAmount;
};






/////////////////////////////////////////////////////////////////////////////////
// The StatusEffectInstance class
class StatusEffectInstance
{
public:
    StatusEffectInstance( StatusEffect * pStatusEffect, Bool bRemovable, UInt iDuration );
    ~StatusEffectInstance();

    // Getters
    inline StatusEffectType GetType() const;

    inline StatusEffect * GetStatusEffect() const;

    inline Bool IsBuff() const;
    inline Bool IsDebuff() const;
    inline Bool IsDisabling() const;

    inline Bool IsStackable() const;

    inline Bool IsRemovable() const;

    // Interface
    inline Bool IsExpired() const;
    inline UInt GetDuration() const;

    inline Void DecreaseDuration();

    inline Void OnUpdateBattleStats( MonsterBattleInstance * pHost ) const;

    inline Void OnTurnStart( MonsterBattleInstance * pHost ) const;
    //inline Void OnTurnEnd( MonsterBattleInstance * pHost ) const;

protected:
    StatusEffect * m_pStatusEffect;
    Bool m_bRemovable;

    UInt m_iDuration;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "StatusEffect.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_MONSTERS_STATUSEFFECT_H

