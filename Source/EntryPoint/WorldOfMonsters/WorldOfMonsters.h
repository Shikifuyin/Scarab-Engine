/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/WorldOfMonsters.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldOfMonsters, Entry Point
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
#ifndef SCARAB_ENTRYPOINT_WORLDOFMONSTERS_WORLDOFMONSTERS_H
#define SCARAB_ENTRYPOINT_WORLDOFMONSTERS_WORLDOFMONSTERS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Graphics/GraphicsManager.h"
#include "Gameplay/GameplayManager.h"
#include "Entities/EntityManager.h"
#include "Interface/InterfaceManager.h"

#include "World\BattleField.h"
#include "World\TownField.h"

#include "../MainSelector.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Game states
enum GameState {
    GAME_STATE_INIT = 0,
    GAME_STATE_LOADING,

    GAME_STATE_LOGIN,

    GAME_STATE_PLAYER_TOWN,
    GAME_STATE_MOVE_BUILDING,

    GAME_STATE_TEAM_SELECTION,
    GAME_STATE_BATTLE_START,
    GAME_STATE_BATTLE_RUN,
    GAME_STATE_BATTLE_PLAYER_TURN,
    GAME_STATE_BATTLE_VICTORY,
    GAME_STATE_BATTLE_DEFEAT,


    GAME_STATE_CLEANUP,
    GAME_STATE_EXIT,

    GAME_STATE_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The WorldOfMonsters class
class WorldOfMonsters : public WindowApplication
{
    // Application class
    DeclareApplication;

public:
    WorldOfMonsters();
    virtual ~WorldOfMonsters();

    // Callbacks
    virtual Bool OnInitialize();
    virtual Void OnCleanup();

    virtual Bool OnFrameMove();
    virtual Void OnFrameRender();

    virtual Void OnExit();

    virtual Bool OnKeyPress( KeyCode iKey );
    virtual Bool OnKeyRelease( KeyCode iKey );

    virtual Void OnMouseMove( UInt iX, UInt iY );
    virtual Bool OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta );
    virtual Bool OnMousePress( UInt iX, UInt iY, KeyCode iButton );
    virtual Bool OnMouseRelease( UInt iX, UInt iY, KeyCode iButton );
    virtual Bool OnMouseDblClick( UInt iX, UInt iY, KeyCode iButton );

    // Application state transitions
    /////////////////////////////////////////////////

private:
    friend class TestBackboardModel;

    // Debug
    Bool m_bWireFrame;

    GChar m_strFPSText[64];
    GChar m_strCoordsText[64];
    GChar m_strCullerText[64];

    GPU2DSolidColorBrush * m_pDebugTextBrush;
    GPU2DTextFormat * m_pDebugTextFormat;

    // Game state
    Void _ProcessState_Loading();
    Void _UpdateGameState();

    GameState m_iState;
};
RegisterApplication( WorldOfMonsters );

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldOfMonsters.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDOFMONSTERS_WORLDOFMONSTERS_H



