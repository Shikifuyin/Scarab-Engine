/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/TimeController/TimeWarpMessage.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Message support for TimeWarp ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef KOALA_ENGINE_PHYSICS_TIMECONTROLLER_TIMEWARPMESSAGE_H
#define KOALA_ENGINE_PHYSICS_TIMECONTROLLER_TIMEWARPMESSAGE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Core/Math/Formats/Scalar.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Message types
enum TimeWarpMessageType
{
    TIMEWARP_RENDER = 0,
    TIMEWARP_CALLBACK,

    TIMEWARP_CHECK_COLLISION,
    TIMEWARP_CHECK_ISLAND
};

// Prototypes
class Body;
class Island;

/////////////////////////////////////////////////////////////////////////////////
// The TimeWarpMessage class
class TimeWarpMessage
{
public:
    TimeWarpMessage();
    virtual ~TimeWarpMessage();

    virtual TimeWarpMessageType GetType() const = 0;

protected:
    TimeWarpMessageType m_iType;
    Bool m_bCounterMessage;

    Scalar m_fTimeStamp;
};

/////////////////////////////////////////////////////////////////////////////////
// The TimeWarpRender class
class TimeWarpRender : public TimeWarpMessage
{
public:
    TimeWarpRender();
    virtual ~TimeWarpRender();

    inline virtual TimeWarpMessageType GetType() const;

protected:
    // Message target
    Body * m_pBody;
};

/////////////////////////////////////////////////////////////////////////////////
// The TimeWarpCallback class
class TimeWarpCallback : public TimeWarpMessage
{
public:
    TimeWarpCallback();
    virtual ~TimeWarpCallback();

    inline virtual TimeWarpMessageType GetType() const;

protected:
    // Message target
    Body * m_pBody;
};

/////////////////////////////////////////////////////////////////////////////////
// The TimeWarpCheckCollision class
class TimeWarpCheckCollision : public TimeWarpMessage
{
public:
    TimeWarpCheckCollision();
    virtual ~TimeWarpCheckCollision();

    inline virtual TimeWarpMessageType GetType() const;

protected:
    Scalar m_fSafeTime;

    // Message target
    Body * m_pBodyA; // A allways has
    Body * m_pBodyB; // lowest LocalTime
};

/////////////////////////////////////////////////////////////////////////////////
// The TimeWarpCheckIsland class
class TimeWarpCheckIsland : public TimeWarpMessage
{
public:
    TimeWarpCheckIsland();
    virtual ~TimeWarpCheckIsland();

    inline virtual TimeWarpMessageType GetType() const;

protected:
    Scalar m_fSafeTime;

    // Message target
    Island * m_pIsland;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TimeWarpMessage.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_PHYSICS_TIMECONTROLLER_TIMEWARPMESSAGE_H

