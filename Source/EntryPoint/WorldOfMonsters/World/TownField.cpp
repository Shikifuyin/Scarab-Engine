/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/World/TownField.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters, World : TownField class
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
#include "TownField.h"

/////////////////////////////////////////////////////////////////////////////////
// TownField implementation
TownField::TownField()
{
    m_pPlayerTown = NULL;

    m_pRoot = NULL;
    m_pFloor = NULL;
    for( UInt i = 0; i < BUILDING_COUNT; ++i )
        m_arrBuildingEntities[i] = NULL;

    for( UInt i = 0; i < BUILDING_COUNT; ++i )
        m_arrBuildingPositions[i] = Point2::Null;
}
TownField::~TownField()
{
    Assert( m_pPlayerTown == NULL );
}

Void TownField::Load( PlayerTown * pPlayerTown )
{
    Assert( pPlayerTown != NULL );
    Assert( m_pPlayerTown == NULL );

    m_pPlayerTown = pPlayerTown;

    m_pRoot = WorldFn->CreateNode( TEXT("TownField") );

    WorldFn->SelectMemory( TEXT("Nodes") );
    m_pFloor = New TownFieldFloor();
    WorldFn->UnSelectMemory();

    for ( UInt i = 0; i < BUILDING_COUNT; ++i ) {
        BuildingType iType = (BuildingType)i;
        if ( pPlayerTown->HasBuilding(iType) ) {
            m_arrBuildingEntities[iType] = EntityFn->CreateBuilding( iType );
            m_arrBuildingPositions[iType] = pPlayerTown->GetBuildingPosition( iType );
        }
    }

    m_pRoot->AddChild( m_pFloor );
    for( UInt i = 0; i < BUILDING_COUNT; ++i ) {
        if ( m_arrBuildingEntities[i] != NULL )
            m_pRoot->AddChild( m_arrBuildingEntities[i] );
    }
}
Void TownField::UnLoad()
{
    Assert( m_pPlayerTown != NULL );

    WorldFn->DestroyNode( m_pRoot, true );

    m_pPlayerTown = NULL;

    m_pRoot = NULL;
    m_pFloor = NULL;
    for( UInt i = 0; i < BUILDING_COUNT; ++i )
        m_arrBuildingEntities[i] = NULL;

    for( UInt i = 0; i < BUILDING_COUNT; ++i )
        m_arrBuildingPositions[i] = Point2::Null;
}

Bool TownField::PlaceBuilding( BuildingEntity * pBuilding, UInt iX, UInt iY )
{
    Assert( iX < TOWNFIELD_CELL_COUNT_X );
    Assert( iY < TOWNFIELD_CELL_COUNT_Y );

    BuildingType iType = pBuilding->GetBuildingType();
    Assert( m_arrBuildingEntities[iType] == NULL );

    Point2 hPosition( iX, iY );
    Rectangle2 hNewRect( hPosition, pBuilding->GetWidth(), pBuilding->GetHeight() );
    Bool bValidPlace = _CheckOverlaps( hNewRect );
    if ( !bValidPlace )
        return false;

    m_arrBuildingEntities[iType] = pBuilding;
    m_arrBuildingPositions[iType] = hPosition;

    return true;
}
Bool TownField::MoveBuilding( BuildingType iBuilding, UInt iX, UInt iY )
{
    Assert( iBuilding < BUILDING_COUNT );
    Assert( iX < TOWNFIELD_CELL_COUNT_X );
    Assert( iY < TOWNFIELD_CELL_COUNT_Y );

    BuildingEntity * pMovingBuilding = m_arrBuildingEntities[iBuilding];
    Assert( pMovingBuilding != NULL );

    Point2 hPosition( iX, iY );
    Rectangle2 hNewRect( hPosition, pMovingBuilding->GetWidth(), pMovingBuilding->GetHeight() );
    Bool bValidPlace = _CheckOverlaps( hNewRect );
    if ( !bValidPlace )
        return false;

    m_arrBuildingPositions[iBuilding] = hPosition;

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Bool TownField::_CheckOverlaps( const Rectangle2 & hRect ) const
{
    // Check against all placed buildings
    // No need for fancy SAP-like stuff here ... dataset is tiny.
    for( UInt i = 0; i < BUILDING_COUNT; ++i ) {
        BuildingEntity * pBuilding = m_arrBuildingEntities[i];
        if ( pBuilding == NULL )
            continue;

        Rectangle2 hBuildingRect( m_arrBuildingPositions[i], pBuilding->GetWidth(), pBuilding->GetHeight() );
        if ( hRect.TopLeft.X > hBuildingRect.DownRight.X || hRect.DownRight.X < hBuildingRect.TopLeft.X )
            continue;
        if ( hRect.TopLeft.Y > hBuildingRect.DownRight.Y || hRect.DownRight.Y < hBuildingRect.TopLeft.Y )
            continue;

        // Overlap !
        return true;
    }
    return false;
}

