/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Interface/MonsterDatabase/HUDMonsterDB.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Framework, Interface : HUDMonsterDB
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
#ifndef SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_MONSTERDATABASE_HUDMONSTERDB_H
#define SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_MONSTERDATABASE_HUDMONSTERDB_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Engine/GUI/GUIManager.h"

#include "../../Gameplay/GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// DB Tabs
enum HUDMonsterDBTab {
    HUDMONSTERDB_TAB_FIRE = 0,
    HUDMONSTERDB_TAB_WATER,
    HUDMONSTERDB_TAB_WIND,
    HUDMONSTERDB_TAB_LIGHT,
    HUDMONSTERDB_TAB_DARK,

    HUDMONSTERDB_TAB_COUNT
};

// Prototypes
class HUDMonsterDB;

/////////////////////////////////////////////////////////////////////////////////
// The HUDMonsterInfos class
class HUDMonsterInfos
{
public:
    HUDMonsterInfos();
    ~HUDMonsterInfos();

    // Window access
    inline GUIWindow * GetWindow() const;

private:
    // Helpers
    Void _CreateWindow();
    Void _DestroyWindow();

    // Models

    // Window elements
    GUIWindow * m_pWindow;


};

/////////////////////////////////////////////////////////////////////////////////
// The HUDMonsterDB class
class HUDMonsterDB
{
public:
    HUDMonsterDB();
    ~HUDMonsterDB();

    // Window access
    inline GUIWindow * GetWindow() const;

private:
    // Helpers
    Void _CreateWindow();
    Void _DestroyWindow();

    // Models
    GUISwitchTabsModel m_hTabsModel;
    GUISectionModel m_arrTabsSectionModel[HUDMONSTERDB_TAB_COUNT];
    GUIButtonModel m_hMonsterButtonModel;

    // Window elements
    GUIWindow * m_pWindow;

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "HUDMonsterDB.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_MONSTERDATABASE_HUDMONSTERDB_H

