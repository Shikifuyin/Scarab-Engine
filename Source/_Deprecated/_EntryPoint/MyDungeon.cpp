/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/MyDungeon.cpp
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
// Includes
#include "MyDungeon.h"

/////////////////////////////////////////////////////////////////////////////////
// MyDungeon implementation
MyDungeon::MyDungeon( VertexFormat * pVF, EffectLightPointPhong * pLightEffect ):
    WorldIndoorArea( TEXT("MyDungeon") )
{
    m_fDungeonHeight = 10.0f;
    m_fCorridorHeight = 5.0f;

    // Effects
    m_vSelfLight.Diffuse = Color4::White;
    m_vSelfLight.Position = Vertex3( 30.0f, 5.0f, 1.0f );
    m_matRoomColor.Diffuse = Color4::Cyan;
    m_pEffect = pLightEffect->CreateInstance( &m_vSelfLight, &m_matRoomColor );

    // Instanced geometry
    Box vBox( Vertex3::Null, Vector3(0.5f, 0.5f, 0.5f) );
    m_pInteriorMesh = New MeshBox( vBox, pVF, false, true );
    m_pExteriorMesh = New MeshBox( vBox, pVF, false, false );

    // Dungeon structure
    _CreateDungeonRooms();
    _CreateDungeonTree();

    m_pEntranceRoom->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pEntranceCorridor->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pEntranceAccess_Kitchen->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pEntranceAccess_Central->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );

    m_pCentralRoom->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pCentralAccess_Kitchen->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pCentralAccess_Barracks->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pKitchenRoom->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pBarracksRoom->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );

    m_pKitchenCorridor->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pKitchenAccess_Library->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pLibraryRoom->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pLibraryAccess_Armory->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pBarracksAccess_Armory->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
    m_pArmoryRoom->SetDebugFlags( WORLD_DEBUGDRAW_PORTALS );
}
MyDungeon::~MyDungeon()
{
    // Dungeon structure
    _DestroyDungeonTree();
    _DestroyDungeonRooms();

    // Instanced geometry
    Delete( m_pExteriorMesh );
    m_pExteriorMesh = NULL;
    Delete( m_pInteriorMesh );
    m_pInteriorMesh = NULL;

    // Effects
    Delete( m_pEffect );
    m_pEffect = NULL;
}

Void MyDungeon::_Create_EntranceRoom()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("EntranceRoom_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(30.0f,5.0f,m_fDungeonHeight * 0.5f) );
    m_pEntranceRoom->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("EntranceRoom_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(30.0f,5.0f,m_fDungeonHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 25.0f, 4.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 25.0f, 6.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 25.0f, 6.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 25.0f, 4.0f, m_fCorridorHeight );
    pPortal = m_pEntranceRoom->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pEntranceCorridor;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_EntranceCorridor()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("EntranceCorridor_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(21.0f,2.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(14.5f,5.0f,m_fCorridorHeight * 0.5f) );
    m_pEntranceCorridor->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("EntranceCorridor_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(21.0f,2.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(14.5f,5.0f,m_fCorridorHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 25.0f, 6.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 25.0f, 4.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 25.0f, 4.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 25.0f, 6.0f, m_fCorridorHeight );
    pPortal = m_pEntranceCorridor->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pEntranceRoom;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 4.0f, 6.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 6.0f, 6.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 6.0f, 6.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 4.0f, 6.0f, m_fCorridorHeight );
    pPortal = m_pEntranceCorridor->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pEntranceAccess_Kitchen;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 19.0f, 6.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 21.0f, 6.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 21.0f, 6.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 19.0f, 6.0f, m_fCorridorHeight );
    pPortal = m_pEntranceCorridor->CreatePortal( 2, arrPortalPolygon, 4 );
    pPortal->Target = m_pEntranceAccess_Central;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_EntranceAccess_Kitchen()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("EntranceAccess_Kitchen_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(2.0f,5.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(5.0f,8.5f,m_fCorridorHeight * 0.5f) );
    m_pEntranceAccess_Kitchen->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("EntranceAccess_Kitchen_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(2.0f,5.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(5.0f,8.5f,m_fCorridorHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 6.0f, 6.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 4.0f, 6.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 4.0f, 6.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 6.0f, 6.0f, m_fCorridorHeight );
    pPortal = m_pEntranceAccess_Kitchen->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pEntranceCorridor;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 4.0f, 11.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 6.0f, 11.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 6.0f, 11.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 4.0f, 11.0f, m_fCorridorHeight );
    pPortal = m_pEntranceAccess_Kitchen->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pKitchenRoom;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_EntranceAccess_Central()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("EntranceAccess_Central_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(2.0f,5.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(20.0f,8.5f,m_fCorridorHeight * 0.5f) );
    m_pEntranceAccess_Central->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("EntranceAccess_Central_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(2.0f,5.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(20.0f,8.5f,m_fCorridorHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 21.0f, 6.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 19.0f, 6.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 19.0f, 6.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 21.0f, 6.0f, m_fCorridorHeight );
    pPortal = m_pEntranceAccess_Central->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pEntranceCorridor;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 19.0f, 11.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 21.0f, 11.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 21.0f, 11.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 19.0f, 11.0f, m_fCorridorHeight );
    pPortal = m_pEntranceAccess_Central->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pCentralRoom;
    pPortal->Closed = false;
}

Void MyDungeon::_Create_CentralRoom()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("CentralRoom_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(20.0f,16.0f,m_fDungeonHeight * 0.5f) );
    m_pCentralRoom->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("CentralRoom_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(20.0f,16.0f,m_fDungeonHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 21.0f, 11.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 19.0f, 11.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 19.0f, 11.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 21.0f, 11.0f, m_fCorridorHeight );
    pPortal = m_pCentralRoom->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pEntranceAccess_Central;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 15.0f, 15.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 15.0f, 17.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 15.0f, 17.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 15.0f, 15.0f, m_fCorridorHeight );
    pPortal = m_pCentralRoom->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pCentralAccess_Kitchen;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 25.0f, 17.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 25.0f, 15.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 25.0f, 15.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 25.0f, 17.0f, m_fCorridorHeight );
    pPortal = m_pCentralRoom->CreatePortal( 2, arrPortalPolygon, 4 );
    pPortal->Target = m_pCentralAccess_Barracks;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_CentralAccess_Kitchen()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("CentralAccess_Kitchen_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(5.0f,2.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(12.5f,16.0f,m_fCorridorHeight * 0.5f) );
    m_pCentralAccess_Kitchen->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("CentralAccess_Kitchen_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(5.0f,2.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(12.5f,16.0f,m_fCorridorHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 15.0f, 17.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 15.0f, 15.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 15.0f, 15.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 15.0f, 17.0f, m_fCorridorHeight );
    pPortal = m_pCentralAccess_Kitchen->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pCentralRoom;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 10.0f, 15.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 10.0f, 17.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 10.0f, 17.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 10.0f, 15.0f, m_fCorridorHeight );
    pPortal = m_pCentralAccess_Kitchen->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pKitchenRoom;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_CentralAccess_Barracks()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("CentralAccess_Barracks_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(5.0f,2.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(27.5f,16.0f,m_fCorridorHeight * 0.5f) );
    m_pCentralAccess_Barracks->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("CentralAccess_Barracks_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(5.0f,2.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(27.5f,16.0f,m_fCorridorHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 25.0f, 15.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 25.0f, 17.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 25.0f, 17.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 25.0f, 15.0f, m_fCorridorHeight );
    pPortal = m_pCentralAccess_Barracks->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pCentralRoom;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 30.0f, 17.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 30.0f, 15.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 30.0f, 15.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 30.0f, 17.0f, m_fCorridorHeight );
    pPortal = m_pCentralAccess_Barracks->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pBarracksRoom;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_KitchenRoom()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("KitchenRoom_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(5.0f,16.0f,m_fDungeonHeight * 0.5f) );
    m_pKitchenRoom->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("KitchenRoom_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(5.0f,16.0f,m_fDungeonHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 6.0f, 11.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 4.0f, 11.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 4.0f, 11.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 6.0f, 11.0f, m_fCorridorHeight );
    pPortal = m_pKitchenRoom->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pEntranceAccess_Kitchen;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 10.0f, 17.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 10.0f, 15.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 10.0f, 15.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 10.0f, 17.0f, m_fCorridorHeight );
    pPortal = m_pKitchenRoom->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pCentralAccess_Kitchen;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 4.0f, 21.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 6.0f, 21.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 6.0f, 21.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 4.0f, 21.0f, m_fCorridorHeight );
    pPortal = m_pKitchenRoom->CreatePortal( 2, arrPortalPolygon, 4 );
    pPortal->Target = m_pKitchenCorridor;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_BarracksRoom()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("BarracksRoom_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(35.0f,16.0f,m_fDungeonHeight * 0.5f) );
    m_pBarracksRoom->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("BarracksRoom_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(35.0f,16.0f,m_fDungeonHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 30.0f, 15.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 30.0f, 17.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 30.0f, 17.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 30.0f, 15.0f, m_fCorridorHeight );
    pPortal = m_pBarracksRoom->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pCentralAccess_Barracks;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 34.0f, 21.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 36.0f, 21.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 36.0f, 21.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 34.0f, 21.0f, m_fCorridorHeight );
    pPortal = m_pBarracksRoom->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pBarracksAccess_Armory;
    pPortal->Closed = false;
}

Void MyDungeon::_Create_KitchenCorridor()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("KitchenCorridor_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(2.0f,11.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(5.0f,26.5f,m_fCorridorHeight * 0.5f) );
    m_pKitchenCorridor->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("KitchenCorridor_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(2.0f,11.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(5.0f,26.5f,m_fCorridorHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 6.0f, 21.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 4.0f, 21.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 4.0f, 21.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 6.0f, 21.0f, m_fCorridorHeight );
    pPortal = m_pKitchenCorridor->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pKitchenRoom;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 6.0f, 32.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 6.0f, 30.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 6.0f, 30.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 6.0f, 32.0f, m_fCorridorHeight );
    pPortal = m_pKitchenCorridor->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pKitchenAccess_Library;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_KitchenAccess_Library()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("KitchenAccess_Library_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(9.0f,2.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(10.5f,31.0f,m_fCorridorHeight * 0.5f) );
    m_pKitchenAccess_Library->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("KitchenAccess_Library_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(9.0f,2.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(10.5f,31.0f,m_fCorridorHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 6.0f, 30.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 6.0f, 32.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 6.0f, 32.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 6.0f, 30.0f, m_fCorridorHeight );
    pPortal = m_pKitchenAccess_Library->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pKitchenCorridor;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 15.0f, 32.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 15.0f, 30.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 15.0f, 30.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 15.0f, 32.0f, m_fCorridorHeight );
    pPortal = m_pKitchenAccess_Library->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pLibraryRoom;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_LibraryRoom()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("LibraryRoom_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(20.0f,31.0f,m_fDungeonHeight * 0.5f) );
    m_pLibraryRoom->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("LibraryRoom_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(20.0f,31.0f,m_fDungeonHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 15.0f, 30.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 15.0f, 32.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 15.0f, 32.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 15.0f, 30.0f, m_fCorridorHeight );
    pPortal = m_pLibraryRoom->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pKitchenAccess_Library;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 25.0f, 32.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 25.0f, 30.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 25.0f, 30.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 25.0f, 32.0f, m_fCorridorHeight );
    pPortal = m_pLibraryRoom->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pLibraryAccess_Armory;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_LibraryAccess_Armory()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("LibraryAccess_Armory_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(5.0f,2.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(27.5f,31.0f,m_fCorridorHeight * 0.5f) );
    m_pLibraryAccess_Armory->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("LibraryAccess_Armory_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(5.0f,2.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(27.5f,31.0f,m_fCorridorHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 25.0f, 30.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 25.0f, 32.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 25.0f, 32.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 25.0f, 30.0f, m_fCorridorHeight );
    pPortal = m_pLibraryAccess_Armory->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pLibraryRoom;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 30.0f, 32.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 30.0f, 30.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 30.0f, 30.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 30.0f, 32.0f, m_fCorridorHeight );
    pPortal = m_pLibraryAccess_Armory->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pArmoryRoom;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_BarracksAccess_Armory()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("BarracksAccess_Armory_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(2.0f,5.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(35.0f,23.5f,m_fCorridorHeight * 0.5f) );
    m_pBarracksAccess_Armory->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("BarracksAccess_Armory_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(2.0f,5.0f,m_fCorridorHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(35.0f,23.5f,m_fCorridorHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 36.0f, 21.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 34.0f, 21.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 34.0f, 21.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 36.0f, 21.0f, m_fCorridorHeight );
    pPortal = m_pBarracksAccess_Armory->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pBarracksRoom;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 34.0f, 26.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 36.0f, 26.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 36.0f, 26.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 34.0f, 26.0f, m_fCorridorHeight );
    pPortal = m_pBarracksAccess_Armory->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pArmoryRoom;
    pPortal->Closed = false;
}
Void MyDungeon::_Create_ArmoryRoom()
{
    // Create geometry
    WorldLeaf * pGeometry;

    pGeometry = New WorldLeaf( TEXT("ArmoryRoom_Interior") );
    pGeometry->SetMesh( m_pInteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(35.0f,31.0f,m_fDungeonHeight * 0.5f) );
    m_pArmoryRoom->AddChild( pGeometry );

    pGeometry = New WorldLeaf( TEXT("ArmoryRoom_Exterior") );
    pGeometry->SetMesh( m_pExteriorMesh );
    pGeometry->SetEffectInstance( m_pEffect );
    pGeometry->WorldTransform.SetScale( Vector3(10.0f,10.0f,m_fDungeonHeight) );
    pGeometry->WorldTransform.SetTranslate( Vector3(35.0f,31.0f,m_fDungeonHeight * 0.5f) );
    m_pOutsideTree->AddChild( pGeometry );

    // Create portals
    Vertex3 arrPortalPolygon[4];
    WorldPortal * pPortal;

    arrPortalPolygon[0] = Vertex3( 36.0f, 26.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 34.0f, 26.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 34.0f, 26.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 36.0f, 26.0f, m_fCorridorHeight );
    pPortal = m_pArmoryRoom->CreatePortal( 0, arrPortalPolygon, 4 );
    pPortal->Target = m_pBarracksAccess_Armory;
    pPortal->Closed = false;

    arrPortalPolygon[0] = Vertex3( 30.0f, 30.0f, 0.0f );
    arrPortalPolygon[1] = Vertex3( 30.0f, 32.0f, 0.0f );
    arrPortalPolygon[2] = Vertex3( 30.0f, 32.0f, m_fCorridorHeight );
    arrPortalPolygon[3] = Vertex3( 30.0f, 30.0f, m_fCorridorHeight );
    pPortal = m_pArmoryRoom->CreatePortal( 1, arrPortalPolygon, 4 );
    pPortal->Target = m_pLibraryAccess_Armory;
    pPortal->Closed = false;
}

Void MyDungeon::_CreateDungeonRooms()
{
    // Outside tree
    m_pOutsideTree = New WorldNode( TEXT("<Outside>") );

    // Entrance rooms
    m_pEntranceRoom           = New WorldConvexRoom( 1, TEXT("<EntranceRoom>") );
    m_pEntranceCorridor       = New WorldConvexRoom( 3, TEXT("<EntranceCorridor>") );
    m_pEntranceAccess_Kitchen = New WorldConvexRoom( 2, TEXT("<EntranceAccess_Kitchen>") );
    m_pEntranceAccess_Central = New WorldConvexRoom( 2, TEXT("<EntranceAccess_Central>") );

    // Central rooms
    m_pCentralRoom            = New WorldConvexRoom( 3, TEXT("<CentralRoom>") );
    m_pCentralAccess_Kitchen  = New WorldConvexRoom( 2, TEXT("<CentralAccess_Kitchen>") );
    m_pCentralAccess_Barracks = New WorldConvexRoom( 2, TEXT("<CentralAccess_Barracks>") );
    m_pKitchenRoom            = New WorldConvexRoom( 3, TEXT("<KitchenRoom>") );
    m_pBarracksRoom           = New WorldConvexRoom( 2, TEXT("<BarracksRoom>") );

    // Backhouse rooms
    m_pKitchenCorridor       = New WorldConvexRoom( 2, TEXT("<KitchenCorridor>") );
    m_pKitchenAccess_Library = New WorldConvexRoom( 2, TEXT("<KitchenAccess_Library>") );
    m_pLibraryRoom           = New WorldConvexRoom( 2, TEXT("<LibraryRoom>") );
    m_pLibraryAccess_Armory  = New WorldConvexRoom( 2, TEXT("<LibraryAccess_Armory>") );
    m_pBarracksAccess_Armory = New WorldConvexRoom( 2, TEXT("<BarracksAccess_Armory>") );
    m_pArmoryRoom            = New WorldConvexRoom( 2, TEXT("<ArmoryRoom>") );

    // Entrance rooms
    _Create_EntranceRoom();
    _Create_EntranceCorridor();
    _Create_EntranceAccess_Kitchen();
    _Create_EntranceAccess_Central();

    // Central rooms
    _Create_CentralRoom();
    _Create_CentralAccess_Kitchen();
    _Create_CentralAccess_Barracks();
    _Create_KitchenRoom();
    _Create_BarracksRoom();

    // Backhouse rooms
    _Create_KitchenCorridor();
    _Create_KitchenAccess_Library();
    _Create_LibraryRoom();
    _Create_LibraryAccess_Armory();
    _Create_BarracksAccess_Armory();
    _Create_ArmoryRoom();
}
Void MyDungeon::_DestroyDungeonRooms()
{
    // Backhouse rooms
    Delete( m_pOutsideTree->GetChild( TEXT("ArmoryRoom_Exterior") ) );
    Delete( m_pArmoryRoom->GetChild( TEXT("ArmoryRoom_Interior") ) );
    Delete( m_pArmoryRoom );
    m_pArmoryRoom = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("BarracksAccess_Armory_Exterior") ) );
    Delete( m_pBarracksAccess_Armory->GetChild( TEXT("BarracksAccess_Armory_Interior") ) );
    Delete( m_pBarracksAccess_Armory );
    m_pBarracksAccess_Armory = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("LibraryAccess_Armory_Exterior") ) );
    Delete( m_pLibraryAccess_Armory->GetChild( TEXT("LibraryAccess_Armory_Interior") ) );
    Delete( m_pLibraryAccess_Armory );
    m_pLibraryAccess_Armory = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("LibraryRoom_Exterior") ) );
    Delete( m_pLibraryRoom->GetChild( TEXT("LibraryRoom_Interior") ) );
    Delete( m_pLibraryRoom );
    m_pLibraryRoom = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("KitchenAccess_Library_Exterior") ) );
    Delete( m_pKitchenAccess_Library->GetChild( TEXT("KitchenAccess_Library_Interior") ) );
    Delete( m_pKitchenAccess_Library );
    m_pKitchenAccess_Library = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("KitchenCorridor_Exterior") ) );
    Delete( m_pKitchenCorridor->GetChild( TEXT("KitchenCorridor_Interior") ) );
    Delete( m_pKitchenCorridor );
    m_pKitchenCorridor = NULL;

    // Central rooms
    Delete( m_pOutsideTree->GetChild( TEXT("BarracksRoom_Exterior") ) );
    Delete( m_pBarracksRoom->GetChild( TEXT("BarracksRoom_Interior") ) );
    Delete( m_pBarracksRoom );
    m_pBarracksRoom = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("KitchenRoom_Exterior") ) );
    Delete( m_pKitchenRoom->GetChild( TEXT("KitchenRoom_Interior") ) );
    Delete( m_pKitchenRoom );
    m_pKitchenRoom = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("CentralAccess_Barracks_Exterior") ) );
    Delete( m_pCentralAccess_Barracks->GetChild( TEXT("CentralAccess_Barracks_Interior") ) );
    Delete( m_pCentralAccess_Barracks );
    m_pCentralAccess_Barracks = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("CentralAccess_Kitchen_Exterior") ) );
    Delete( m_pCentralAccess_Kitchen->GetChild( TEXT("CentralAccess_Kitchen_Interior") ) );
    Delete( m_pCentralAccess_Kitchen );
    m_pCentralAccess_Kitchen = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("CentralRoom_Exterior") ) );
    Delete( m_pCentralRoom->GetChild( TEXT("CentralRoom_Interior") ) );
    Delete( m_pCentralRoom );
    m_pCentralRoom = NULL;

    // Entrance rooms
    Delete( m_pOutsideTree->GetChild( TEXT("EntranceAccess_Central_Exterior") ) );
    Delete( m_pEntranceAccess_Central->GetChild( TEXT("EntranceAccess_Central_Interior") ) );
    Delete( m_pEntranceAccess_Central );
    m_pEntranceAccess_Central = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("EntranceAccess_Kitchen_Exterior") ) );
    Delete( m_pEntranceAccess_Kitchen->GetChild( TEXT("EntranceAccess_Kitchen_Interior") ) );
    Delete( m_pEntranceAccess_Kitchen );
    m_pEntranceAccess_Kitchen = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("EntranceCorridor_Exterior") ) );
    Delete( m_pEntranceCorridor->GetChild( TEXT("EntranceCorridor_Interior") ) );
    Delete( m_pEntranceCorridor );
    m_pEntranceCorridor = NULL;
    Delete( m_pOutsideTree->GetChild( TEXT("EntranceRoom_Exterior") ) );
    Delete( m_pEntranceRoom->GetChild( TEXT("EntranceRoom_Interior") ) );
    Delete( m_pEntranceRoom );
    m_pEntranceRoom = NULL;

    // Outside tree
    Delete( m_pOutsideTree );
    m_pOutsideTree = NULL;
}

Void MyDungeon::_CreateDungeonTree()
{
    // Entrance tree
    m_pEntranceTree[0] = New WorldBSPNode( TEXT("<Entrance_0>") );
    m_pEntranceTree[1] = New WorldBSPNode( TEXT("<Entrance_1>") );
    m_pEntranceTree[2] = New WorldBSPNode( TEXT("<Entrance_2>") );

    m_pEntranceTree[0]->SetNegativeChild( m_pEntranceTree[1] );
    m_pEntranceTree[0]->SetPositiveChild( m_pEntranceRoom );
    m_pEntranceTree[0]->WorldPlane = Plane( Vector3::eI, 25.0f );

    m_pEntranceTree[1]->SetNegativeChild( m_pEntranceCorridor );
    m_pEntranceTree[1]->SetPositiveChild( m_pEntranceTree[2] );
    m_pEntranceTree[1]->WorldPlane = Plane( Vector3::eJ, 6.0f );

    m_pEntranceTree[2]->SetNegativeChild( m_pEntranceAccess_Kitchen );
    m_pEntranceTree[2]->SetPositiveChild( m_pEntranceAccess_Central );
    m_pEntranceTree[2]->WorldPlane = Plane( Vector3::eI, 12.5f );

    // Central tree
    m_pCentralTree[0] = New WorldBSPNode( TEXT("<Central_0>") );
    m_pCentralTree[1] = New WorldBSPNode( TEXT("<Central_1>") );
    m_pCentralTree[2] = New WorldBSPNode( TEXT("<Central_2>") );
    m_pCentralTree[3] = New WorldBSPNode( TEXT("<Central_3>") );

    m_pCentralTree[0]->SetNegativeChild( m_pCentralTree[1] );
    m_pCentralTree[0]->SetPositiveChild( m_pCentralTree[2] );
    m_pCentralTree[0]->WorldPlane = Plane( Vector3::eI, 25.0f );

    m_pCentralTree[1]->SetNegativeChild( m_pCentralTree[3] );
    m_pCentralTree[1]->SetPositiveChild( m_pCentralRoom );
    m_pCentralTree[1]->WorldPlane = Plane( Vector3::eI, 15.0f );

    m_pCentralTree[2]->SetNegativeChild( m_pCentralAccess_Barracks );
    m_pCentralTree[2]->SetPositiveChild( m_pBarracksRoom );
    m_pCentralTree[2]->WorldPlane = Plane( Vector3::eI, 30.0f );

    m_pCentralTree[3]->SetNegativeChild( m_pKitchenRoom );
    m_pCentralTree[3]->SetPositiveChild( m_pCentralAccess_Kitchen );
    m_pCentralTree[3]->WorldPlane = Plane( Vector3::eI, 10.0f );

    // Backhouse tree
    m_pBackhouseTree[0] = New WorldBSPNode( TEXT("<Backhouse_0>") );
    m_pBackhouseTree[1] = New WorldBSPNode( TEXT("<Backhouse_1>") );
    m_pBackhouseTree[2] = New WorldBSPNode( TEXT("<Backhouse_2>") );
    m_pBackhouseTree[3] = New WorldBSPNode( TEXT("<Backhouse_3>") );
    m_pBackhouseTree[4] = New WorldBSPNode( TEXT("<Backhouse_4>") );

    m_pBackhouseTree[0]->SetNegativeChild( m_pBackhouseTree[1] );
    m_pBackhouseTree[0]->SetPositiveChild( m_pBackhouseTree[2] );
    m_pBackhouseTree[0]->WorldPlane = Plane( Vector3::eI, 25.0f );

    m_pBackhouseTree[1]->SetNegativeChild( m_pBackhouseTree[3] );
    m_pBackhouseTree[1]->SetPositiveChild( m_pLibraryRoom );
    m_pBackhouseTree[1]->WorldPlane = Plane( Vector3::eI, 15.0f );

    m_pBackhouseTree[2]->SetNegativeChild( m_pLibraryAccess_Armory );
    m_pBackhouseTree[2]->SetPositiveChild( m_pBackhouseTree[4] );
    m_pBackhouseTree[2]->WorldPlane = Plane( Vector3::eI, 30.0f );

    m_pBackhouseTree[3]->SetNegativeChild( m_pKitchenCorridor );
    m_pBackhouseTree[3]->SetPositiveChild( m_pKitchenAccess_Library );
    m_pBackhouseTree[3]->WorldPlane = Plane( Vector3::eI, 6.0f );

    m_pBackhouseTree[4]->SetNegativeChild( m_pBarracksAccess_Armory );
    m_pBackhouseTree[4]->SetPositiveChild( m_pArmoryRoom );
    m_pBackhouseTree[4]->WorldPlane = Plane( Vector3::eJ, 26.0f );

    // Dungeon tree
    m_pDungeonTree = New WorldBSPNode( TEXT("<Dungeon_0>") );

    SetNegativeChild( m_pDungeonTree );
    SetPositiveChild( m_pBackhouseTree[0] );
    WorldPlane = Plane( Vector3::eJ, 21.0f );

    m_pDungeonTree->SetNegativeChild( m_pEntranceTree[0] );
    m_pDungeonTree->SetPositiveChild( m_pCentralTree[0] );
    m_pDungeonTree->WorldPlane = Plane( Vector3::eJ, 11.0f );

    SetOutsideNode( m_pOutsideTree );
}
Void MyDungeon::_DestroyDungeonTree()
{
    UInt i;

    // Dungeon tree
    SetOutsideNode( NULL );
    SetNegativeChild( NULL );
    SetPositiveChild( NULL );

    Delete( m_pDungeonTree );
    m_pDungeonTree = NULL;

    // Backhouse tree
    for( i = 0; i < 5; ++i ) {
        Delete( m_pBackhouseTree[i] );
        m_pBackhouseTree[i] = NULL;
    }

    // Central tree
    for( i = 0; i < 4; ++i ) {
        Delete( m_pCentralTree[i] );
        m_pCentralTree[i] = NULL;
    }

    // Entrance tree
    for( i = 0; i < 3; ++i ) {
        Delete( m_pEntranceTree[i] );
        m_pEntranceTree[i] = NULL;
    }
}

