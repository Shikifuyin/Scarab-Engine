/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Battle/BattleMonsterInstance.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Battle : BattleMonsterInstance interface
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_BATTLE_BATTLEMONSTERINSTANCE_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_BATTLE_BATTLEMONSTERINSTANCE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Monsters/Monster.h"
#include "../Town/PlayerTown.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// ATB Capacity
#define BATTLE_ATB_CAPACITY 1000 // Base speed is 100, 10 steps to take a turn
                                 // Max speeds around 200-300, 3-5 steps to take a turn

/////////////////////////////////////////////////////////////////////////////////
// The BattleMonsterInstance class
class BattleMonsterInstance
{
public:
    BattleMonsterInstance();
    BattleMonsterInstance( PlayerTown * pPlayerTown, MonsterInstance * pMonsterInstance, LeaderSkill * pActiveLeaderSkill );
    BattleMonsterInstance( const BattleMonsterInstance & rhs );
    ~BattleMonsterInstance();

    // operators
    BattleMonsterInstance & operator=( const BattleMonsterInstance & rhs );

    // Test for validity
    inline Bool IsNull() const;
    inline Bool IsPresent() const;

    // Base instance
    inline MonsterInstance * GetBaseInstance() const;
    inline LeaderSkill * GetActiveLeaderSkill() const;

    // Battle stats
    inline UInt GetHP() const;
    inline UInt GetATT() const;
    inline UInt GetDEF() const;
    inline UInt GetSPD() const;

    inline Float GetCritR() const;
    inline Float GetCritD() const;
    inline Float GetACC() const;
    inline Float GetRES() const;

    // Battle state
        // Current HP
    inline Bool IsAlive() const;
    inline Bool IsDead() const;

    inline UInt GetCurrentHP() const;
    inline Void SetCurrentHP( UInt iAmount );
    inline Void ResetCurrentHP();

    inline UInt GetShieldHP() const;
    inline Void SetShieldHP( UInt iAmount );

    UInt Damage( UInt iAmount, UInt iDestroyRunes = 0 );
    Void Heal( UInt iAmount );

        // ATB
    inline Bool CheckATB() const;
    inline UInt GetATB() const;

    inline Void IncreaseATB( Float fRatio );
    inline Void IncreaseATB( UInt iAmount );
    inline Void DecreaseATB( Float fRatio );
    inline Void DecreaseATB( UInt iAmount );
    inline Void FillATB();
    inline Void ResetATB();

        // Skills & cooldowns
    inline UInt GetSkillCount() const;
    inline SkillInstance * GetSkillInstance( UInt iSlot );

    inline Bool HasSkillUp( UInt iSlot ) const;
    inline UInt GetSkillCooldown( UInt iSlot ) const;

    inline Void IncreaseSkillCooldown( UInt iSlot );
    inline Void DecreaseSkillCooldown( UInt iSlot );
    inline Void TriggerSkillCooldown( UInt iSlot );
    inline Void ResetSkillCooldown( UInt iSlot );

        // Active effects
    inline Bool HasStatusEffect( StatusEffectType iType ) const;
    inline const StatusEffect * GetStatusEffect( StatusEffectType iType ) const;
    inline StatusEffect * GetStatusEffect( StatusEffectType iType );

    Void AddStatusEffect( StatusEffectType iType, UInt iStackCount, UInt iDuration, Float fAmplitude );
    Void RemoveStatusEffect( StatusEffectType iType, UInt iStackCount );
    Void RemoveExpiredStatusEffects();

    inline Bool IsDisabled() const;

    // Battle interface
    Void Initialize();

private:
    // Helpers
    Void _UpdateBattleStats();

    // Base instances
    PlayerTown * m_pPlayerTown;
    MonsterInstance * m_pMonsterInstance;
    LeaderSkill * m_pActiveLeaderSkill;

    // Battle stats
    UInt m_iHealth;
    UInt m_iAttack;
    UInt m_iDefense;
    UInt m_iSpeed;
    Float m_fCriticalRate;
    Float m_fCriticalDamage;
    Float m_fAccuracy;
    Float m_fResistance;

    // Battle state
    UInt m_iCurrentHP;
    UInt m_iShieldHP;
    UInt m_iHPLostCounter_Nemesis;
    UInt m_iHPLostCounter_Destroy;

    UInt m_iATB;

    UInt m_arrSkillCooldowns[SKILL_SLOT_COUNT];

    StatusEffectSet m_hActiveEffects;

    Bool m_bDisabled;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BattleMonsterInstance.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_BATTLE_BATTLEMONSTERINSTANCE_H

