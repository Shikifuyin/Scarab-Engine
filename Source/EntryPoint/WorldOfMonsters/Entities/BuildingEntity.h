/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Entities/BuildingEntity.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, Entities : BuildingEntity class
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_ENTITIES_BUILDINGENTITY_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_ENTITIES_BUILDINGENTITY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Entity.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The BuildingEntity class
class BuildingEntity : public Entity
{
public:
    BuildingEntity();
    virtual ~BuildingEntity();

    // Type
    inline virtual EntityType GetType() const;

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BuildingEntity.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_ENTITIES_BUILDINGENTITY_H


