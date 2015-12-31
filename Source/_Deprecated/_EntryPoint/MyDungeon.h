/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/MyDungeon.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Testing stuff ...
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
#ifndef SCARAB_ENTRYPOINT_MYDUNGEON_H
#define SCARAB_ENTRYPOINT_MYDUNGEON_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Engine/Application/ApplicationManager.h"

#include "../Engine/Rendering/Effects/EffectLibrary/Lighting/EffectLightPointPhong.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// MyDungeon
class MyDungeon : public WorldIndoorArea
{
public:
    MyDungeon( VertexFormat * pVF, EffectLightPointPhong * pLightEffect );
    virtual ~MyDungeon();

    inline Void UpdateSelfLight( const Vertex3 & vPosition );

protected:
    // Helpers
    Void _Create_EntranceRoom();
    Void _Create_EntranceCorridor();
    Void _Create_EntranceAccess_Kitchen();
    Void _Create_EntranceAccess_Central();

    Void _Create_CentralRoom();
    Void _Create_CentralAccess_Kitchen();
    Void _Create_CentralAccess_Barracks();
    Void _Create_KitchenRoom();
    Void _Create_BarracksRoom();

    Void _Create_KitchenCorridor();
    Void _Create_KitchenAccess_Library();
    Void _Create_LibraryRoom();
    Void _Create_LibraryAccess_Armory();
    Void _Create_BarracksAccess_Armory();
    Void _Create_ArmoryRoom();

    Void _CreateDungeonRooms();
    Void _DestroyDungeonRooms();

    Void _CreateDungeonTree();
    Void _DestroyDungeonTree();

    // Height scaling
    Scalar m_fDungeonHeight;
    Scalar m_fCorridorHeight;

    // Effects
    Light m_vSelfLight;
    Material m_matRoomColor;
    EffectInstance * m_pEffect;

    // Instanced geometry
    MeshBox * m_pInteriorMesh;
    MeshBox * m_pExteriorMesh;

    // Dungeon structure
    WorldBSPNode * m_pDungeonTree;
    WorldNode * m_pOutsideTree;

    WorldBSPNode * m_pEntranceTree[3];
    WorldConvexRoom * m_pEntranceRoom;
    WorldConvexRoom * m_pEntranceCorridor;
    WorldConvexRoom * m_pEntranceAccess_Kitchen;
    WorldConvexRoom * m_pEntranceAccess_Central;

    WorldBSPNode * m_pCentralTree[4];
    WorldConvexRoom * m_pCentralRoom;
    WorldConvexRoom * m_pCentralAccess_Kitchen;
    WorldConvexRoom * m_pCentralAccess_Barracks;
    WorldConvexRoom * m_pKitchenRoom;
    WorldConvexRoom * m_pBarracksRoom;

    WorldBSPNode * m_pBackhouseTree[5];
    WorldConvexRoom * m_pKitchenCorridor;
    WorldConvexRoom * m_pKitchenAccess_Library;
    WorldConvexRoom * m_pLibraryRoom;
    WorldConvexRoom * m_pLibraryAccess_Armory;
    WorldConvexRoom * m_pBarracksAccess_Armory;
    WorldConvexRoom * m_pArmoryRoom;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MyDungeon.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_MYDUNGEON_H



