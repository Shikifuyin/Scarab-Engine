/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Animation/SkillAnimations/FireBoltAnimation.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Animation, SkillAnimations : FireBoltAnimation
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_ANIMATION_SKILLANIMATIONS_FIREBOLTANIMATION_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_ANIMATION_SKILLANIMATIONS_FIREBOLTANIMATION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "SkillAnimationClasses.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The FireBoltAnimation class
class FireBoltAnimation : public SkillAnimation
{
public:
    FireBoltAnimation();
    virtual ~FireBoltAnimation();

    // Animation callbacks
    virtual Void OnStart( AnimationInstance * pInstance );
    virtual Void OnStop( AnimationInstance * pInstance );
    virtual Void OnUpdate( AnimationInstance * pInstance );

protected:
    // Animation state
    typedef struct _animation_state {
        FireBoltEntity * pFireBolt;
    } _AnimationState;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "FireBoltAnimation.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_ANIMATION_SKILLANIMATIONS_FIREBOLTANIMATION_H

