/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Entities/Entity.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Entities : Entity base class
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_ENTITIES_ENTITY_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_ENTITIES_ENTITY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Framework/Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Types
enum EntityType {
    ENTITY_MONSTER = 0,
    ENTITY_BUILDING,

    ENTITY_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The Entity class
class Entity
{
public:
    Entity();
    virtual ~Entity();

    // Type
    virtual EntityType GetType() const = 0;

    // Graphical representation
    inline const GeometryInstance * GetGeometry() const;

    // Physical representation
    inline Skeleton * GetSkeleton() const;
    inline CharacterController * GetController() const;

    // World node
    inline WorldNode * GetNode() const;

    // Methods
    Void Update();

protected:
    // Helpers
    Void _LoadGraphics();
    Void _LoadPhysics();
    Void _LoadAnimations();

    



    // World node
    WorldNode * m_pWorldNode;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Entity.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_ENTITIES_ENTITY_H


